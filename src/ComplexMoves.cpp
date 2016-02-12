/* 
   SDS, a bridge single-suit double-dummy quick-trick solver.

   Copyright (C) 2015-16 by Soren Hein.

   See LICENSE and README.
*/

#include <iomanip>
#include <assert.h>

#include "Header.h"
#include "Trick.h"
#include "DefList.h"
#include "MoveList.h"
#include "ComplexMoves.h"
#include "stats.h"
#include "portab.h"
#include "files.h"
#include "single.h"
#include "options.h"
#include "summary.h"
#include "hist.h"

using namespace std;


/*
   The multi-threading is rather simple:  Each thread gets a
   single batch of holdings, and we wait for all batches to complete
   before giving any of them more holdings.

   OPENMP multi-threading is untested.
*/

#define SDS_BATCH_SIZE 1000
#define SDS_MAX_HOLDINGS (SDS_MAX_CORES * SDS_BATCH_SIZE)


extern FilesType files;
extern OptionsType options;
extern SingleType * singles[14];
extern MoveList moveList;
extern SummaryType summary;


// Globals for multi-threading.
Holding holdingList[SDS_MAX_HOLDINGS];
DefList defList1[SDS_MAX_HOLDINGS];
DefList defList2[SDS_MAX_HOLDINGS];
unsigned batchLen;


bool MakeComplexTables(
  unsigned& slStart,
  unsigned& cStart,
  const unsigned dlen);

int MakeComplexDistribute();

void BestMoveAfterSide(
  Holding& holding,
  DefList& def);

void BestMoveAfterLead(
  Holding& holding,
  DefList& def);

void BestMoveAfterLho(
  Holding& holding,
  DefList& def);

bool BestMoveAfterPard(
  Holding& holding,
  DefList& def);

void DumpStatus(
  DefList& def,
  const char text[]);

void DumpStatus(
  DefList& def1,
  DefList& def2,
  const char text[]);

void DumpMoves(
  ostream& out,
  const Holding& holding,
  DefList& def1,
  DefList& def2,
  unsigned& count);

extern vector<unsigned> holdCtr;
void MakeComplexMoves()
{
  DefList def1, def2;
  Holding holding;
  bool newFlag;

  for (unsigned dlen = 2; dlen <= 13; dlen++)
  {
    for (unsigned sl = 1; sl <= 13; sl++)
    {
      cout << "\nComplex moves (dlen " << dlen << "): " << sl << "\n";

      for (unsigned c = 0; c < SDS_NUM_SINGLES[sl]; c++)
      {
        if ((c & 0x3ffff) == 0)
        {
          cout << "Count " <<
            setw(8) << c << " (" <<
            setw(5) << fixed << setprecision(2) <<
            100. * c / SDS_NUM_SINGLES[sl] << "%)" << endl;
        }

        if (singles[sl][c].moveNo || singles[sl][c].declLen != dlen)
          continue;

        holding.Set(sl, c);
        def1.Reset();
        def2.Reset();
        MakeComplexSingleMove(holding, def1, def2);

        StartTimer();
        singles[sl][c].moveNo = moveList.AddMove(def1, def2, 
          holding, newFlag);
        EndTimer();

        unsigned r = moveList.GetMaxRank(singles[sl][c].moveNo);
        UpdateHist(HIST_COMPLEX, singles[sl][c].declLen, r, newFlag);

        if (options.findFlag)
        {
          unsigned d, a;
          moveList.GetAD(singles[sl][c].moveNo, d, a);
          /*
          if (d == 1 && a == 1)
          {
            DumpMoves(files.simpleRest, holding, 
              def1, def2, summary.countRest);
          }
          */

          // Other possibility:
          unsigned cc = holding.GetCounter();
          unsigned s = holding.GetSuitLength();
          unsigned top = cc >> (2*s - 8);
          holdCtr[0x1700+top]++;
          if (top == 1 || top == 4 || top == 16)
          {
            DumpMoves(files.simpleRest, holding, 
              def1, def2, summary.countRest);
          }
          // Specialized: AKQJ with ace.
          // if (top == 0) ...

          // To help in table development, certain tricks are printed.
          unsigned t = moveList.GetSymmTricks(singles[sl][c].moveNo);
          if (t == 0)
            continue;

          unsigned gl1 = holding.GetLength(QT_ACE);
          unsigned gl2 = holding.GetLength(QT_PARD);
          unsigned gl = Max(gl1, gl2);

          if (t < gl)
          {
            DumpMoves(files.simpleShort, holding, 
              def1, def2, summary.countShort);
          }
          else 
          if (gl1 == gl2)
          {
            DumpMoves(files.simpleEqual, holding, 
              def1, def2, summary.countEqual);
          }
          else
          {
            DumpMoves(files.simpleDiff, holding, 
              def1, def2, summary.countDiff);
          }

        }
      }
    }
  }
}


void MakeComplexSingleMove(
  Holding& holding,
  DefList& def1,
  DefList& def2)
{
  if (options.numCores == 1)
  {
    // holding.Print(files.track);
    // files.track.flush();
  }

  assert(holding.GetLength(QT_ACE) > 0);
  if (holding.GetLength(QT_PARD) == 0)
  {
    holding.SetSide(QT_ACE);
    BestMoveAfterSide(holding, def1);

    if (options.debugFlow)
      DumpStatus(def1, "MakeComplexSingleMove");
  }
  else
  {
    holding.SetSide(QT_ACE);
    BestMoveAfterSide(holding, def1);

    holding.SetSide(QT_PARD);
    BestMoveAfterSide(holding, def2);

    if (options.debugFlow)
      DumpStatus(def1, def2, "MakeComplexSingleMove");
  }
}


bool MakeComplexTables(
  unsigned& slStart,
  unsigned& cStart,
  const unsigned dlen)
{
  batchLen = 0;

  for (unsigned sl = slStart; sl <= 13; sl++)
  {
    unsigned cs = (sl == slStart ? cStart : 0);
    for (unsigned c = 0; c < SDS_NUM_SINGLES[sl]; c++)
    {
      if (singles[sl][c].moveNo)
        continue;

      if (singles[sl][c].declLen != dlen)
        continue;

      holdingList[batchLen].Set(sl, c);

      batchLen++;
      if (batchLen == options.numCores * SDS_BATCH_SIZE)
      {
        slStart = sl;
        cStart = c+1;
        return true;
      }
    }
  }

  // Last batch
  return false;
}


void MakeComplexMovesParallel()
{
  bool newFlag;

  for (unsigned dlen = 2; dlen <= 13; dlen++)
  {
    unsigned slStart = 1, cStart = 0;
    bool flag;
    cout << "\nDeclaring length " << dlen << ":\n";

    unsigned skip = 0;
    do
    {
      if (++skip == 10)
        PrintPercentHist(cout, cStart, dlen);

      flag = MakeComplexTables(slStart, cStart, dlen);

      StartTimer();
      MakeComplexDistribute();

      for (unsigned i = 0; i < batchLen; i++)
      {
        unsigned sl = holdingList[i].GetSuitLength();
        unsigned c = holdingList[i].GetCounter();

        singles[sl][c].moveNo = moveList.AddMove(
         defList1[i], defList2[i], holdingList[i], newFlag);

        unsigned r = moveList.GetMaxRank(singles[sl][c].moveNo);
        UpdateHist(HIST_COMPLEX, singles[sl][c].declLen, r, newFlag);
      }
      EndTimer();

    }
    while (flag);
  }
  cout << "\n";
}


#if (defined(_WIN32) || defined(__CYGWIN__)) && !defined(_OPENMP)

DWORD CALLBACK MakeComplexBatch(void *);


HANDLE solveAllEvents[SDS_MAX_CORES];
LONG volatile threadIndex;


DWORD CALLBACK MakeComplexBatch(void *)
{
  int thid;
  thid = InterlockedIncrement(&threadIndex);

  unsigned startNo = SDS_BATCH_SIZE * static_cast<unsigned>(thid);
  unsigned endNo = Min(batchLen, startNo + SDS_BATCH_SIZE);

  for (unsigned n = startNo; n < endNo; n++)
    MakeComplexSingleMove(holdingList[n], defList1[n], defList2[n]);

  if (SetEvent(solveAllEvents[thid]) == 0)
    return 0;

  return 1;

}


int MakeComplexDistribute()
{
  DWORD solveAllWaitResult;
  unsigned k;
  int res;

  threadIndex = -1;

  for (k = 0; k < options.numCores; k++)
  {
    solveAllEvents[k] = CreateEvent(NULL, FALSE, FALSE, 0);
    if (solveAllEvents[k] == 0)
      return -1;
  }

  for (k = 0; k < options.numCores; k++)
  {
    res = QueueUserWorkItem(MakeComplexBatch, NULL, WT_EXECUTELONGFUNCTION);
    if (res != 1)
      return res;
  }

  solveAllWaitResult = WaitForMultipleObjects(options.numCores,
    solveAllEvents, TRUE, INFINITE);

  if (solveAllWaitResult != WAIT_OBJECT_0)
    return -2;

  for (k = 0; k < options.numCores; k++)
    CloseHandle(solveAllEvents[k]);

  return 1;
}

#else

void MakeComplexBatch(int thid);


void MakeComplexBatch(int thid)
{
  unsigned startNo = SDS_BATCH_SIZE * thid;
  unsigned endNo = Min(batchLen, startNo + SDS_BATCH_SIZE);

  for (unsigned n = startNo; n < endNo; n++)
    MakeComplexSingleMove(&holdingList[n], &defList1[n], &defList2[n]);

  return 1;
}


int MakeComplexDistribute()
{
  fail = 1;

#if defined (_OPENMP)
  if (omp_get_dynamic())
    omp_set_dynamic(0);
  omp_set_num_threads(options.numCores);
#endif

  int thid;
  schedType st;

  #pragma omp parallel default(none) shared(fail) private(thid, res)
  {
    #pragma omp while schedule(dynamic, chunk)

    while (1)
    {
#if defined (_OPENMP)
      thid = omp_get_thread_num();
#else
      thid = 0;
#endif

      res = MakeComplexBatch(thid);

      if (res != 1)
        fail = res;
    }
  }

  return fail;
}

#endif


void BestMoveAfterSide(
  Holding& holding,
  DefList& def)
{
  DefList defNew;

  def.Reset();
  bool seen = false;

  holding.RewindLead();
  while (holding.NextLead())
  {
    if (! seen)
    {
      seen = true;
      BestMoveAfterLead(holding, def);
    }
    else
    {
      BestMoveAfterLead(holding, defNew);
      def *= defNew;
    }

    if (options.debugFlow)
      DumpStatus(def, "BestMoveAfterSide: leadNo");
  }
}


void BestMoveAfterLead(
  Holding& holding,
  DefList& def)
{
  DefList defNew;

  def.Reset();
  bool seen = false;

  holding.RewindLho();
  while (holding.NextLho())
  {
    if (! seen)
    {
      seen = true;
      BestMoveAfterLho(holding, def);
    }
    else
    {
      BestMoveAfterLho(holding, defNew);
      def += defNew;
    }

    if (options.debugFlow)
      DumpStatus(def, "BestMoveAfterLead: lhoNo");
  }
}


void BestMoveAfterLho(
  Holding& holding,
  DefList& def)
{
  DefList defNew;

  bool seen = false;

  holding.RewindPard();
  while (holding.NextPard())
  {
    if (! seen)
    {
      seen = true;
      seen = BestMoveAfterPard(holding, def);
    }
    else
    {
      seen = BestMoveAfterPard(holding, defNew);
      def *= defNew;
    }

    if (options.debugFlow)
      DumpStatus(def, "BestMoveAfterLho: pardNo");
  }
}


bool BestMoveAfterPard(
  Holding& holding,
  DefList& def)
{
  unsigned slNew = 0;
  unsigned cNew = 0;

  holding.SetRhoNo();
  if (! holding.MakePlay(slNew, cNew))
  {
    def.Set1(holding.GetTrick());

    if (options.debugFlow)
    {
      DumpStatus(def, "BestMoveAfterPard: Simple play");
      holding.PrintPlay(files.debug);
    }

    return true;
  }

  assert(slNew >= 1 && slNew <= 12);
  assert(cNew < SDS_NUM_SINGLES[slNew]);


  if (singles[slNew][cNew].moveNo == 0)
  {
    Holding tmpHolding;
    tmpHolding.Set(slNew, cNew);

    if (options.debugFlow)
    {
      files.debug << "Start recursing\n";
      tmpHolding.Print(files.debug);
    }

    DefList deftmp1, deftmp2;
    MakeComplexSingleMove(tmpHolding, deftmp1, deftmp2);
      
    bool newFlag;
    singles[slNew][cNew].moveNo = moveList.AddMove(
      deftmp1, deftmp2, tmpHolding, newFlag);
// tmpHolding.Print();
// moveList.Print(singles[slNew][cNew].moveNo);

    if (options.debugFlow)
    {
      deftmp1.Print(files.debug);
      deftmp2.Print(files.debug);
      files.debug << "Done recursing\n";
    }
  }

  assert(singles[slNew][cNew].moveNo != 0);
  def = moveList.GetCombinedMove(singles[slNew][cNew].moveNo);

  if (options.debugFlow)
  {
    DumpStatus(def, "BestMoveAfterPard: Stored play count");
    Holding htmp;
    htmp.Set(slNew, cNew);
    htmp.Print(files.debug);
    holding.PrintPlay(files.debug);
  }

  def += holding;

  if (options.debugFlow)
    DumpStatus(def, "Prepended: Stored play count");

  return true;
}


void DumpStatus(
  DefList& def,
  const char text[])
{
  files.debug << text << "\n";
  def.GetHeader().Print(files.debug);
  def.Print(files.debug);
}


void DumpStatus(
  DefList& def1,
  DefList& def2,
  const char text[])
{
  files.debug << text << "\n";
  def1.GetHeader().Print(files.debug);
  def1.Print(files.debug);
  def2.GetHeader().Print(files.debug);
  def2.Print(files.debug);
}


inline void DumpMoves(
  ostream& out,
  const Holding& holding,
  DefList& def1,
  DefList& def2,
  unsigned& count)
{
  holding.Print(out);
  def1.Print(out);
  out << "---\n";
  def2.Print(out);
  count++;
}


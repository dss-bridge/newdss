/* 
   SDS, a bridge single-suit double-dummy quick-trick solver.

   Copyright (C) 2015-16 by Soren Hein.

   See LICENSE and README.
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <assert.h>

using namespace std;

#include "portab.h"
#include "Header.h"
#include "MoveList.h"
#include "SimpleMoves.h"
#include "ComplexMoves.h"
#include "stats.h"
#include "Twiddle.h"
#include "Dist.h"
#include "LoopHold.h"
#include "files.h"
#include "options.h"
#include "hist.h"
#include "summary.h"
#include "single.h"
#include "const.h"


// Globals

FilesType files;
OptionsType options;
SingleType * singles[14];
MoveList moveList;
vector<unsigned> holdCtr;
SummaryType summary;

unsigned highestDefNo = 0;
unsigned highestAltNo = 0;

void MainIdentify();

void InitSingles();

void FreeSingles();

void CheckSinglesRanks();

void CheckSinglesRecurse(
  const unsigned sl,
  const unsigned cOld,
  const unsigned mask,
  const unsigned step,
  const unsigned distHex,
  const unsigned rank,
  const unsigned mno,
  const unsigned depth);


int main(int argc, char * argv[])
{
  ReadArgs(argc, argv);

  MainIdentify();

  InitTimer();
  InitTimerList();
  StartTimerNo(0);

  InitFiles();
  InitHist();
  InitSingles();
  InitDist();

#if 0
  options.suitLength = 9;
  options.counter = 0xca2f;
  options.debugFlow = true;
  options.debugDef = true;
  options.debugAlt = true;
#endif
#if 0
LoopHold lh;
lh.Set(12, 0xe9805a);
// lh.Set(8, 0x82b);
// lh.Set(8, 0x829);
lh.Print();
DefList def;
unsigned r;
// lh.SolveCrashTricks(def, r);
lh.CashoutBoth(def, r);
def.Print();
cout << endl;
PrintHoldCounter(cout);
exit(0);
#endif
  if (options.suitLength)
  {
    LoopHold holding;
    holding.Set(options.suitLength, options.counter);
    holding.Print(cout);

    DefList def;
    if (MakeSimpleSingleMoveWrapper(options.suitLength, options.counter, 
      def, holding))
    {
      cout << "Table match:\n\n";
      def.Print();
    }
    else
      cout << "No table match\n\n";

    // If the tables seem suspect, can try this as well.
    // MakeSimpleMoves();

    singles[options.suitLength][options.counter].moveNo = 0;

    DefList def1, def2;
    MakeComplexSingleMove(holding, def1, def2);

    cout << "Direct solve:\n";
    def1.Print();
    def2.Print();

    DefList defMerge;
    if (defMerge.MergeSidesSoft(def1, def2))
      defMerge.Print(cout, "Soft merge:");
    else
    {
      defMerge.MergeSidesHard(def1, def2);
      defMerge.Print(cout, "Hard merge:");
    }

    if (def == defMerge)
      cout << "Identical\n";
    else
      cout << "Different\n";


    PrintHoldCounter(cout);
    FreeSingles();
    CloseFiles();
    exit(1);
  }
  else if (options.numCores > 1)
  {
    MakeSimpleMoves();

    if (! options.tablesOnly)
    {
      cout << "numCores " << options.numCores << "\n";
      MakeComplexMovesParallel();
      moveList.PrintMovesByKeys();
      moveList.PrintLists();
    }
  }
  else
  {
    MakeSimpleMoves();

    if (! options.tablesOnly)
    {
      MakeComplexMoves();
      moveList.PrintMovesByKeys();
      moveList.PrintLists();
    }
  }

CheckSinglesRanks();

  FreeSingles();
  EndTimerNo(0);

  moveList.PrintStats();
  PrintHistLowCards(files.statsAll);
  PrintHist(files.movesAll);
  PrintTimer();
  PrintTimerList();
  PrintHoldCounter(files.statsAll);

  CloseFiles();

  SumFiles();
  PrintSummary(cout);
}


void MainIdentify()
{
  cout << "single\n----------\n";

#if defined(_WIN32) || defined(__CYGWIN__)
  cout << setw(12) << left << "System" << setw(23) << 
    right << "Windows\n";
#if defined(_MSC_VER)
  cout << setw(12) << left << "Compiler" << setw(23) << 
      right << "Microsoft Visual C++\n";
#elif defined(__MINGW32__)
  cout << setw(12) << left << "Compiler" << setw(23) << 
    right << "MinGW\n";
#else
  cout << setw(12) << left << "Compiler" << setw(23) << 
    right << "GNU g++\n";
#endif

#elif defined(__linux)
  cout << setw(12) << left << "System" << 
    right << setw(23) << "Linux\n";
  cout << setw(12) << left << "Compiler" << 
    right << setw(23) << "GNU g++\n";

#elif defined(__APPLE__)
  cout << setw(12) << left << "System" << 
    right << setw(23) << "Apple\n";
#if defined(__clang__)
  cout << setw(12) << left << "Compiler" << 
    right << setw(23) << "clang\n";
#else
  cout << setw(12) << left << "Compiler" << 
    right << setw(23) << "GNU g++\n";
#endif
#endif

#if defined(__cplusplus)
  cout << setw(12) << left << "Dialect" << 
    right << setw(22) << __cplusplus << "\n";
#endif

  cout << "\n";
}

void InitSingles()
{
  singles[0] = nullptr;

  for (unsigned l = 1; l <= 13; l++)
  {
    size_t n = static_cast<size_t>(SDS_NUM_SINGLES[l]);
    singles[l] = static_cast<SingleType *>(malloc(n * sizeof(SingleType)));
  }

  for (unsigned sl = 1; sl <= 13; sl++)
  {
    unsigned n = SDS_NUM_SINGLES[sl];
    for (unsigned c = 0; c < n; c++)
    {
      unsigned tmp = c;
      unsigned count[SDS_HANDS] = {1, 0, 0, 0}; // The ace
      for (unsigned m = 0; m < sl-1; m++)
      {
        count[tmp & 3]++;
        tmp >>= 2;
      }
      singles[sl][c].moveNo = 0;
      singles[sl][c].declLen = (count[0] >= count[2] ? count[0] : count[2]);
      singles[sl][c].minLen = (count[0] <= count[2] ? count[0] : count[2]);
      singles[sl][c].oppLen = (count[1] >= count[3] ? count[1] : count[3]);

      UpdateHist(HIST_ORIG_COUNT, singles[sl][c].declLen, SDS_VOID, false);
    }
  }
}


void FreeSingles()
{
  for (unsigned l = 1; l <= 13; l++)
    free(singles[l]);
}


map<string, unsigned> nodeMap;

void CheckSinglesRanks()
{
  for (unsigned sl = 1; sl <= 13; sl++)
  {
    cout << "\nChecking ranks for length " << sl << "\n";

    for (unsigned c = 0; c < SDS_NUM_SINGLES[sl]; c++)
      singles[sl][c].oppLen = 0; // Misuse...

    CheckSinglesRecurse(sl, 0x0, 0x0, SDS_NUM_SINGLES[sl], 
      0x0, SDS_VOID, 0, 0);

    for (unsigned c = 0; c < SDS_NUM_SINGLES[sl]; c++)
    {
      if (singles[sl][c].oppLen == 0)
      {
        cout << "Missed one, " << sl << ", " << c << endl;
        assert(false);
      }
    }
  }
}


void CheckSinglesRecurse(
  const unsigned sl,
  const unsigned cOld,
  const unsigned mask,
  const unsigned step,
  const unsigned distHex,
  const unsigned rank,
  const unsigned mno,
  const unsigned depth)
{
  Holding holding;
  string tag;

  for (unsigned c = mask; c < mask+step; c++)
  {
    if (singles[sl][c].oppLen)
      continue;

    holding.Set(sl, c);
    unsigned newDistHex = holding.GetDistHex();
    if (distHex != 0 && newDistHex != distHex)
      continue;

    unsigned newMno = singles[sl][c].moveNo;
    if (newMno == 0)
    {
      cout << "Should this be happening?  " << sl << ", " << c << endl;
      assert(false);
    }

    singles[sl][c].oppLen = 1;
    unsigned newRank = moveList.GetMaxRank(newMno);

    if (distHex != 0 && newMno != mno && newRank >= rank)
    {
      cout << "Entering CSR:\n";
      cout << "sl      " << sl << "\n";
      cout << "mask    " << hex << mask << dec << "\n";
      cout << "step    " << hex << step << dec << "\n";
      cout << "distHex " << hex << distHex << dec << "\n";
      cout << "rank    " << SDS_RANK_NAMES[rank] << "\n";
      cout << "mno     " << mno << "\n";
      cout << "depth   " << depth << "\n";
      cout << "RANK ERROR from newRank " << SDS_RANK_NAMES[newRank] <<
        " newMno " << newMno << " depth " << depth << ":\n";

      holding.Print();
      moveList.Print(newMno);

      Holding hold2;
      hold2.Set(sl, cOld);
      hold2.Print();
      cout << "mno was " << mno << ", rank was " << 
        SDS_RANK_NAMES[rank] << "\n";
      moveList.Print(mno);
      cout << "\n";
    }

    if (newMno == mno || newRank == SDS_VOID)
      continue;

    // Make new mask, new step.
    unsigned numBits = 2*(newRank - (13 - sl));
    unsigned newStep = static_cast<unsigned>(1 << numBits);
    unsigned newMask = ((static_cast<unsigned>(1 << 2*(sl-1u)) - 1u) 
      ^ (newStep - 1u)) & c;
    unsigned newMaskBase = newMask >> numBits;

    // Look up new mask, new dist in hash.
    stringstream ss;
    ss << newMaskBase << "." << hex << newDistHex << dec;
    tag = ss.str();

    map<string, unsigned>::iterator it = nodeMap.find(tag);
    if (it != nodeMap.end())
      continue;

    nodeMap[tag] = newMno;

    CheckSinglesRecurse(sl, c, newMask, newStep, newDistHex, 
      newRank, newMno, depth+1);
  }
}


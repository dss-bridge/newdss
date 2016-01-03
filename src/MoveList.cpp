/* 
   SDS, a bridge single-suit double-dummy quick-trick solver.

   Copyright (C) 2015-16 by Soren Hein.

   See LICENSE and README.
*/

#include <sstream>
#include <algorithm>
#include <assert.h>

#include "Header.h"
#include "MoveList.h"
#include "files.h"

using namespace std;

#define MOVE_CHUNK_SIZE 1000

extern FilesType files;
extern SummaryType summary;


MoveList::MoveList()
{
  noToAggr.clear();

  len = MOVE_CHUNK_SIZE;
  noToAggr.resize(MOVE_CHUNK_SIZE);
  count = 1;

  for (unsigned i = 0; i < len; i++)
    noToAggr[i].count = 0;

  noToAggr[0].noComb = 0;
  noToAggr[0].no1 = 0;
  noToAggr[0].no2 = 0;
  noToAggr[0].keyComb = 0;
  noToAggr[0].key1 = 0;
  noToAggr[0].key2 = 0;
  noToAggr[0].number = 0;
}


MoveList::~MoveList()
{
}


void MoveList::Extend()
{
  len += MOVE_CHUNK_SIZE;
  noToAggr.resize(len);

  for (unsigned i = count; i < len; i++)
    noToAggr[i].count = 0;
}


unsigned MoveList::Update(
  const AggrMoveType& mno,
  const bool newFlag,
  const unsigned ret)
{
  unsigned r;
  if (newFlag)
  {
    if (count == len)
      MoveList::Extend();

    noToAggr[count] = mno;
    noToAggr[count].number = count;
    MoveList::SetAggr(mno);
    r = count++;
  }
  else if (ret)
  {
    // Small optimization in order not to search twice sometimes.
    r = ret;
  }
  else
  {
    r = MoveList::AggrToNo(mno);
    assert(r > 0);
  }

  noToAggr[r].count++;
  return r;
}


unsigned MoveList::AddMove(
  DefList& def, 
  const Holding& holding, 
  bool& newFlag)
{
  unsigned hashKey;
  unsigned no = sideComb.AddMove(def, holding, newFlag, hashKey);
  unsigned ret;

  AggrMoveType mno;
  mno.noComb = no;
  mno.no1 = 0;
  mno.no2 = 0;
  mno.keyComb = hashKey;
  mno.key1 = 0;
  mno.key2 = 0;
  mno.count = 0;

  return MoveList::Update(mno, newFlag);
}


unsigned MoveList::AddMove(
  DefList& def1,
  DefList& def2,
  const Holding& holding, 
  bool& newFlag)
{
  AggrMoveType mno;
  DefList def;
  unsigned ret;

  if (def.MergeSidesSoft(def1, def2))
    return MoveList::AddMove(def, holding, newFlag);
  else
  {
    // This is a forced merge.
    def.MergeSidesHard(def1, def2);
    return MoveList::AddMove(def, holding, newFlag);

    // This keeps the moves separate.
    unsigned key1;
    unsigned key2;
    bool newFlag1 = false;
    bool newFlag2 = false;

    mno.noComb = 0;
    mno.no1 = sideList1.AddMove(def1, holding, newFlag1, key1);
    mno.no2 = sideList2.AddMove(def2, holding, newFlag2, key2);
    mno.keyComb = 0;
    mno.key1 = key1;
    mno.key2 = key2;
    mno.count = 0;
   
    if (newFlag1 || newFlag2 || ! (ret = MoveList::AggrToNo(mno)))
      newFlag = true;
    else
      newFlag = false;

    return MoveList::Update(mno, newFlag, ret);
  }

  // For checking whether merged defense becomes simpler.

  /*
  unsigned d1, d2, dm, a;
  def1.GetHeader().GetAD(d1, a);
  def2.GetHeader().GetAD(d2, a);
  def.GetHeader().GetAD(dm, a);
  if (d1*d2 != dm)
    cout << "defMerged " << dm << " != " << d1 << " * " << d2 << "\n";
  */
}


#define u2s(x) static_cast<ostringstream *>(&(ostringstream() << (x)))->str()

unsigned MoveList::AggrToNo(
  const AggrMoveType& mno)
{
  string str = u2s(mno.noComb) + '.' + u2s(mno.no1) + '.' + u2s(mno.no2);
  map<string, unsigned>::iterator it = aggrMap.find(str);
  if (it == aggrMap.end())
    return 0;
  else
    return aggrMap[str];
}


void MoveList::SetAggr(
  const AggrMoveType& mno)
{
  string str = u2s(mno.noComb) + '.' + u2s(mno.no1) + '.' + u2s(mno.no2);
  aggrMap[str] = count;
}


unsigned MoveList::GetMaxRank(
  const unsigned no)
{
  if (noToAggr[no].noComb)
    return sideComb.GetMaxRank(noToAggr[no].noComb);
  else
  {
    // Very rare differences from merged move rank.
    unsigned n1 = sideList1.GetMaxRank(noToAggr[no].no1);
    unsigned n2 = sideList2.GetMaxRank(noToAggr[no].no2);
    return Min(n1, n2);
  }
}

unsigned MoveList::GetSymmTricks(
  const unsigned no) const
{
  if (noToAggr[no].noComb)
    return sideComb.GetSymmTricks(noToAggr[no].noComb);
  else
  {
    unsigned t1 = sideList1.GetSymmTricks(noToAggr[no].no1);
    unsigned t2 = sideList2.GetSymmTricks(noToAggr[no].no2);
    if (t1 == 0 || t2 == 0 || t1 != t2)
      return 0;
    else
      return t1;
  }
}


DefList MoveList::GetCombinedMove(
  const unsigned no)
{
  if (noToAggr[no].noComb)
    return sideComb.GetMove(noToAggr[no].noComb);
  else
  {
    DefList def1 = sideList1.GetMove(noToAggr[no].no1);
    DefList def2 = sideList2.GetMove(noToAggr[no].no2);
    DefList defMerged;
    defMerged.MergeSidesHard(def1, def2);
    return defMerged;
  }
}

extern int globalTrigger;

void MoveList::Print(
  const unsigned no) const
{
  if (noToAggr[no].noComb)
    sideComb.Print(noToAggr[no].noComb);
  else
  {
    sideList1.Print(noToAggr[no].no1);
    sideList2.Print(noToAggr[no].no2);
  }
}


void MoveList::PrintMovesByList(
  ostream& out,
  const vector<AggrMoveType>& list)
{
  for (unsigned i = 1; i < count; i++)
  {
    if (list[i].noComb)
      sideComb.PrintMove(out, list[i].noComb);
    else
    {
      sideList1.PrintMove(out, list[i].no1);
      sideList2.PrintMove(out, list[i].no2);
    }
  }
}


void MoveList::PrintMovesByOrder(
  ostream& out)
{
  MoveList::PrintMovesByList(out, noToAggr);
}


void MoveList::PrintMovesByKeys()
{
  vector<AggrMoveType> sortList = noToAggr;
  sort(sortList.begin()+1, sortList.begin()+count, SortKeyIsGreater);

  const string divider(52, '=');
  for (unsigned i = 1; i < count; i++)
  {
    if (sortList[i].keyComb != sortList[i-1].keyComb || i == 1)
    {
      sideComb.GetMove(sortList[i].number).GetHeader().PrintKey(
        files.movesAll, sortList[i].keyComb);
      files.movesAll << divider << "\n\n";
    }

    if (sortList[i].noComb)
      sideComb.PrintMove(files.movesAll, sortList[i].noComb);
    else
    {
      sideList1.PrintMove(files.movesAll, sortList[i].no1);
      sideList2.PrintMove(files.movesAll, sortList[i].no2);
    }
  }

  sideComb.PrintMoveListByKeys(files.movesComb);
  sideList1.PrintMoveListByKeys(files.movesA);
  sideList2.PrintMoveListByKeys(files.movesP);
}


void MoveList::PrintMovesByCount(
  ostream& out)
{
  vector<AggrMoveType> sortList = noToAggr;
  sort(sortList.begin()+1, sortList.begin()+count, SortCountIsGreater);

  MoveList::PrintMovesByList(out, sortList);

  sideComb.PrintMoveListByCount(files.movesComb);
  sideList1.PrintMoveListByCount(files.movesA);
  sideList2.PrintMoveListByCount(files.movesP);
}


void MoveList::PrintStats() const
{
  sideComb.PrintStats(files.statsComb);
  sideList1.PrintStats(files.statsA);
  sideList2.PrintStats(files.statsP);
}


void MoveList::PrintLists() const
{
  sideComb.PrintLists(files.movesComb, summary.comb);
  sideList1.PrintLists(files.movesA, summary.A);
  sideList2.PrintLists(files.movesP, summary.P);
}


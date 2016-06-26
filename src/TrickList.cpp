/* 
   SDS, a bridge single-suit double-dummy quick-trick solver.

   Copyright (C) 2015-16 by Soren Hein.

   See LICENSE and README.
*/

#include <assert.h>

#include "Holding.h"
#include "Trick.h"
#include "TrickList.h"
#include "Header.h"

using namespace std;


TrickList::TrickList()
{
  TrickList::Reset();
}


TrickList::~TrickList()
{
}


void TrickList::Reset()
{
  len = 0;
}


bool TrickList::Set1(
  const Trick& trick)
{
  len = 1;
  list[0].Set1(trick);
  return true;
}


bool TrickList::Set2(
  const Trick& trick1,
  const Trick& trick2)
{
  if (trick2.Extends(trick1))
  {
    list[0].Set2(trick1, trick2);
    len = 1;
  }
  else
  {
    list[1].Set1(trick1);
    list[0].Set1(trick2);
    len = 2;
  }
  return true;
}


bool TrickList::Set3(
  const Trick& trick1,
  const Trick& trick2,
  const Trick& trick3)
{
  if (trick2.Extends(trick1))
  {
    list[1].Set2(trick1, trick2);
    list[0].Set1(trick3);
    len = 2;
  }
  else if (trick3.Extends(trick2))
  {
    list[1].Set1(trick1);
    list[0].Set2(trick2, trick3);
    len = 2;
  }
  else
  {
    list[2].Set1(trick1);
    list[1].Set1(trick2);
    list[0].Set1(trick3);
    len = 3;
  }

  return true;
}


bool TrickList::Set4(
  const Trick& trick1,
  const Trick& trick2,
  const Trick& trick3,
  const Trick& trick4)
{
  if (trick2.Extends(trick1))
  {
    if (trick4.Extends(trick3))
    {
      list[1].Set2(trick1, trick2);
      list[0].Set2(trick3, trick4);
      len = 2;
    }
    else
    {
      list[2].Set2(trick1, trick2);
      list[1].Set1(trick3);
      list[0].Set1(trick4);
      len = 3;
    }
  }
  else if (trick3.Extends(trick2))
  {
    list[2].Set1(trick1);
    list[1].Set2(trick2, trick3);
    list[0].Set1(trick4);
    len = 3;
  }
  else if (trick4.Extends(trick3))
  {
    list[2].Set1(trick1);
    list[1].Set1(trick2);
    list[0].Set2(trick3, trick4);
    len = 3;
  }
  else
  {
    list[3].Set1(trick1);
    list[2].Set1(trick2);
    list[1].Set1(trick3);
    list[0].Set1(trick4);
    len = 4;
  }

  return true;
}


bool TrickList::Set21(
  const Trick& trick10,
  const Trick& trick11,
  const Trick& trick2)
{
  assert(trick11.Extends(trick10));
  list[1].Set2(trick10, trick11);
  list[0].Set1(trick2);
  len = 2;
  return true;
}


void TrickList::SetStart(
  const PosType start)
{
  assert(len > 0);
  list[len-1].SetStart(start);
}


PosType TrickList::GetFirstStart() const
{
  assert(len > 0);
  return list[len-1].GetStart();
}


PosType TrickList::GetFirstEnd() const
{
  assert(len > 0);
  return list[len-1].GetEnd();
}


unsigned TrickList::GetLength() const
{
  return len;
}


void TrickList::GetHeader(
  Header& header,
  const unsigned startNo) const
{
  assert(len > startNo);

  Trick t;
  list[len-1-startNo].GetSummaryTrick(t);
  header.Set(t, list[len-1-startNo].GetLength());

  for (unsigned l = startNo+1; l < len; l++)
  {
    list[len-1-l].GetSummaryTrick(t);
    header.Increase(t, list[len-1-l].GetLength());
  }
}


unsigned TrickList::GetComplexity() const
{
  unsigned c = 0;
  for (unsigned l = 0; l < len; l++)
    c += list[l].GetLength();
  return c;
}


unsigned TrickList::GetTricks() const
{
  unsigned t = 0;
  for (unsigned l = 0; l < len; l++)
    t += list[l].GetTricks();
  return t;
}


void TrickList::GetFirstSummaryTrick(
  Trick& t) const
{
  assert(len > 0);
  list[len-1].GetSummaryTrick(t);
}


unsigned TrickList::xBNo(
  const PosType start) const
{
  if (len == 1)
    return list[0].xBNo(start);
  else
    return 0;
}


bool TrickList::CanExpand() const
{
  if (len == 1)
    return list[0].CanExpand();
  else
    return false;
}


void TrickList::ExpandEnd(
  PosType endVal,
  TrickList& tl) const
{
  Trick t;
  TrickList::GetFirstSummaryTrick(t);
  t.SetEnd(endVal);
  tl.Set1(t);
}


bool TrickList::CanSplit() const
{
  if (len == 1)
    return false;
  else
    return list[len-1].CanSplit();
}


void TrickList::Split(
  const PosType endval)
{
  assert(len > 1);
  if (endval == list[len-2].GetStart())
  {
    // Only the initial segment survives.
    list[0] = list[len-1];
    list[0].Split(endval);
    len = 1;
  }
  else
    list[len-1].Split(endval);
}


bool TrickList::IsSimpleComplement(
  const TrickList& lNew) const
{
  if (len > 1 || lNew.len > 1)
    return false;

  return list[0].IsSimpleComplement(lNew.list[0]);
}


CmpDetailType TrickList::CompareInit(
  const TrickList& lNew,
  CompareStruct& cdata) const
{
  CmpTrickType cc, cRun = SDS_TRICK_SAME;
  do
  {
    if (cdata.lenOld == 0 && cdata.lenNew == 0)
      return cmpTrickToDetail[cRun];
    else if (cdata.lenOld == 0)
      return SDS_HEADER_PLAY_NEW_BETTER;
    else if (cdata.lenNew == 0)
      return SDS_HEADER_PLAY_OLD_BETTER;

    cdata.lenOld--;
    cdata.lenNew--;

    cc = list[cdata.lenOld].Compare(lNew.list[cdata.lenNew],
      cdata.ranksOld, cdata.ranksNew);
    if (cc == SDS_TRICK_PLAY_DIFFERENT)
      return SDS_HEADER_PLAY_DIFFERENT;

    cRun = cmpTrickMatrix[cRun][cc];
    if (cRun == SDS_TRICK_PLAY_DIFFERENT)
      return SDS_HEADER_PLAY_DIFFERENT;

    Trick t1;
    list[cdata.lenOld].GetSummaryTrick(t1);
    cdata.tricksOld += t1.GetCashing();
    cdata.ranksOld = Min(cdata.ranksOld, t1.GetRanks());

    Trick t2;
    lNew.list[cdata.lenNew].GetSummaryTrick(t2);
    cdata.tricksNew += t2.GetCashing();
    cdata.ranksNew = Min(cdata.ranksNew, t2.GetRanks());
  }
  while (cc != SDS_TRICK_PLAY_NEW_BETTER &&
      cc != SDS_TRICK_PLAY_OLD_BETTER &&
      cc != SDS_TRICK_RANK_NEW_BETTER &&
      cc != SDS_TRICK_RANK_OLD_BETTER);
  // while (cc != SDS_TRICK_PLAY_NEW_BETTER &&
      // cc != SDS_TRICK_PLAY_OLD_BETTER);

  return cmpTrickToDetail[cc];
}


CmpDetailType TrickList::Compare(
  const TrickList& lNew) const
{
  assert(len > 0);
  assert(lNew.len > 0);

  CompareStruct cdata;
  cdata.lenOld = len;
  cdata.lenNew = lNew.len;
  cdata.tricksOld = 0;
  cdata.tricksNew = 0;
  cdata.ranksOld = SDS_VOID;
  cdata.ranksNew = SDS_VOID;
  cdata.winnerFirst = SDS_HEADER_SAME;
  cdata.winnerRunning = SDS_HEADER_SAME;

  CmpDetailType cc = TrickList::CompareInit(lNew, cdata);

  if (cc == SDS_HEADER_PLAY_DIFFERENT)
    return SDS_HEADER_PLAY_DIFFERENT;
  else
  {
    cdata.winnerRunning = cc;
    return TrickList::CompareTail(lNew, cdata);
  }
}


CmpDetailType TrickList::CompareRunning(
  const CompareStruct& cdata) const
{
  if (cdata.tricksOld > cdata.tricksNew)
    return SDS_HEADER_PLAY_OLD_BETTER;
  else if (cdata.tricksOld < cdata.tricksNew)
    return SDS_HEADER_PLAY_NEW_BETTER;
  else if (cdata.ranksOld > cdata.ranksNew)
    return SDS_HEADER_RANK_OLD_BETTER;
  else if (cdata.ranksOld < cdata.ranksNew)
    return SDS_HEADER_RANK_NEW_BETTER;
  else
    return SDS_HEADER_SAME;
}


CmpDetailType TrickList::CompareTail(
  const TrickList& lNew,
  CompareStruct& cdata) const
{
  if (cdata.lenOld == 0 && cdata.lenNew == 0)
  {
    return cmpDetailHardMatrix[cdata.winnerFirst][cdata.winnerRunning];
  }
  else if (cdata.lenOld == 0)
  {
    CmpDetailType c = cdata.winnerRunning;
    if (c == SDS_HEADER_SAME ||
        c == SDS_HEADER_PLAY_NEW_BETTER ||
        c == SDS_HEADER_RANK_NEW_BETTER)
    {
      return cmpDetailHardMatrix[cdata.winnerFirst][SDS_HEADER_PLAY_NEW_BETTER];
    }
  }
  else if (cdata.lenNew == 0)
  {
    CmpDetailType c = cdata.winnerRunning;
    if (c == SDS_HEADER_SAME ||
        c == SDS_HEADER_PLAY_OLD_BETTER ||
        c == SDS_HEADER_RANK_OLD_BETTER)
    {
      return cmpDetailHardMatrix[cdata.winnerFirst][SDS_HEADER_PLAY_OLD_BETTER];
    }
  }

  if (cdata.lenOld > 0)
  {
    Trick t;
    list[cdata.lenOld-1].GetSummaryTrick(t);
    cdata.tricksOld += t.GetCashing();
    cdata.ranksOld = Min(cdata.ranksOld, t.GetRanks());
    cdata.lenOld--;
  }

  if (cdata.lenNew > 0)
  {
    Trick t;
    lNew.list[cdata.lenNew-1].GetSummaryTrick(t);
    cdata.tricksNew += t.GetCashing();
    cdata.ranksNew = Min(cdata.ranksNew, t.GetRanks());
    cdata.lenNew--;
  }

  CmpDetailType d = TrickList::CompareRunning(cdata);
  cdata.winnerRunning = cmpDetailHardMatrix[cdata.winnerRunning][d];

  CmpDetailType c = TrickList::CompareTail(lNew, cdata);
  return cmpDetailMatrix[cdata.winnerFirst][c];
}


bool TrickList::EqualsExceptStart(
  const TrickList& lNew) const
{
  assert(len > 0 && lNew.len > 0);

  if (len != lNew.len)
    return false;
  else if (! list[len-1].EqualsExceptStart(lNew.list[lNew.len-1], len == 1))
    return false;
  else
  {
    unsigned p = 1;
    while (p <= len-1)
    {
      if (list[len-1-p] != lNew.list[lNew.len-1-p])
        return false;
      p++;
    }
  }

  return true;
}


bool TrickList::EqualsExceptEnd(
  const TrickList& lNew) const
{
  assert(len > 0 && lNew.len > 0);

  if (len != lNew.len)
    return false;
  else if (! list[0].EqualsExceptEnd(lNew.list[0]))
    return false;
  else
  {
    unsigned p = 0;
    while (p < len-1)
    {
      if (list[len-1-p] != lNew.list[lNew.len-1-p])
        return false;
      p++;
    }
  }

  return true;
}


void TrickList::FixRanks(
  const unsigned rLower)
{
  // Only fix lists that start with a segment of length 2 that
  // changes side.
  // A9 / - / KQJ872 / T6543: PA29 + PP4-, don't fix.
  // AT / - / KJ9 / Q87: PP1T (or J) + BA1- + PP1-, do fix.
  // AJ9 / - / QT / K8: PA1A + BP1Q + AA1-, do fix.
  // We're not actually testing for the side change here.
  if (list[len-1].GetLength() == 1)
    return;

  unsigned l = 0;
  while (l < len && list[l].GetLowestRank() == 0) // void
    l++;

  if (l == len)
  {
    assert(false);
    return;
  }

  list[l].FixLowestRanks(rLower);
}


bool TrickList::operator == (
  const TrickList& t2) const
{
  if (t2.len != len)
    return false;

  for (unsigned p = 0; p < len; p++)
    if (list[p] != t2.list[p])
      return false;

  return true;
}


bool TrickList::operator != (
  const TrickList& t2) const
{
  return ! (* this == t2);
}


void TrickList::operator += (
  const Holding& holding)
{
  // Add a move in front of the first segment.

  assert(len > 0);
  bool tailIsAA1A = (len == 1 && list[0].IsAA1ATrick());

  for (unsigned p = 0; p < len; p++)
    list[p].Localize(holding);

  AdjustRankType adjust;
  holding.PossiblyFixRank(adjust);
  if (adjust.lower1 != SDS_VOID)
  {
    for (unsigned p = 0; p < len; p++)
      list[p].FixEquals(adjust.upper1, adjust.lower1);
  }

  if (adjust.lower2 != SDS_VOID)
  {
    for (unsigned p = 0; p < len; p++)
      list[p].FixEquals(adjust.upper2, adjust.lower2);
  }

  /*
  unsigned fixedRank;
  unsigned rankToFix = holding.PossiblyFixRank(fixedRank);
  if (rankToFix != SDS_VOID)
  {
    for (unsigned p = 0; p < len; p++)
      list[p].FixEquals(rankToFix, fixedRank);
  }
  */

  if (list[len-1].Prepend(holding, len == 1))
  {
    if (list[len-1].GetLength() == 2 &&
        holding.GetLength(QT_PARD) == 2 &&
        holding.GetLength(QT_RHO) >= 2 &&
        holding.GetLength(QT_LHO) == 0 && 
        holding.IsAATrick())
    {
      // Essentially AKx / - / Qx / xx.  Conservative.
      // Never start with AA.
      TrickList::Set1(holding.GetTrick());
    }
    else
    {
      // Normal case.  It can happen that the prepending trick
      // has the same rank as a later segment, as we sometimes
      // take the rank below the real one.
      for (unsigned p = 1; p < len; p++)
      {
        if (list[len-1-p].FixRanks(holding.GetPrependRank()))
          break;
      }
    }
    return;
  }

  if (holding.GetMinDeclLength() < 2)
    goto PREP_DEFAULT;

  if (tailIsAA1A && holding.IsPA1ACasher())
  {
    // PA1A (not as a finesse) + AA1x = PB2x.
    Trick trick;
    trick.Set(QT_PARD, QT_BOTH, list[0].GetRanks(), 2);
    list[0].Set1(trick);
    return;
    // goto PREP_DEFAULT;
  }

  if (! holding.IsAATrick())
  {
    // No special case.
    assert(len < TRICKLIST_MAXSEGS);
    len++;
    list[len-1].Set1(holding.GetTrick());
    return;
  }

  if (tailIsAA1A)
  {
    // AA1A + PP1x, where the second trick is a straight casher.
    // There must be communication, so this is AB2x!
    Trick trick;
    trick.Set(QT_ACE, QT_BOTH, list[0].GetRanks(), 2);
    list[0].Set1(trick);
    return;
  }
  else if (list[len-1].GetRanks() > holding.GetLHOMaxRank())
  {
    // Rather special case: AA1A + Pxyz, where LHO is either void
    // or is below the first rank to win the next trick.
    // There is no reason ever to start from A if there is a PA move
    // available.  If there isn't, we'll just have to cash from A.
    // So we can always consider this to be AA1A.
    TrickList::Set1(holding.GetTrick());
    return;
  }

PREP_DEFAULT:

  // Didn't fit, so make a new segment.
  assert(len < TRICKLIST_MAXSEGS);
  len++;
  list[len-1].Set1(holding.GetTrick());
}


CmpDetailType TrickList::CompareToTrick(
  const Trick& trick) const
{
  assert(len > 0);
  Trick ltrick;
  TrickList::GetFirstSummaryTrick(ltrick);
  return cmpTrickToDetail[ltrick.Compare(trick)];
}


CmpDetailType TrickList::FixOrCompare(
  TrickList& lOther,
  FixType& fix1,
  FixType& fix2)
{
  if (TrickList::Fix(lOther, fix1, fix2))
    return SDS_HEADER_PLAY_DIFFERENT;

  CmpDetailType c;
  switch (c = TrickList::Compare(lOther))
  {
    case SDS_HEADER_PLAY_OLD_BETTER:
    case SDS_HEADER_RANK_OLD_BETTER:
    case SDS_HEADER_SAME:
      fix1 = SDS_FIX_UNCHANGED;
      fix2 = SDS_FIX_PURGED;
      break;

    case SDS_HEADER_PLAY_NEW_BETTER:
    case SDS_HEADER_RANK_NEW_BETTER:
      fix1 = SDS_FIX_PURGED;
      fix2 = SDS_FIX_UNCHANGED;
      break;

    default:
      break;
  }

  return c;
}


bool TrickList::Fix(
  TrickList& lOther,
  FixType& fix1,
  FixType& fix2)
{
  fix1 = SDS_FIX_UNCHANGED;
  fix2 = SDS_FIX_UNCHANGED;

  if (TrickList::EqualsExceptStart(lOther))
  {
    list[len-1].SetStart(QT_BOTH);
    fix1 = SDS_FIX_STRONGER;
    fix2 = SDS_FIX_PURGED;
  }
  else if (TrickList::EqualsExceptEnd(lOther))
  {
    list[0].SetEnd(QT_BOTH);
    fix1 = SDS_FIX_STRONGER;
    fix2 = SDS_FIX_PURGED;
  }
  else if (len == 1 && lOther.len == 1)
  {
    if (list[0].Fix11(lOther.list[0], fix1, fix2))
    {
      if (fix1 == SDS_FIX_SPLIT)
      {
        TrickList::Split();
        fix1 = SDS_FIX_WEAKER;
      }

      if (fix2 == SDS_FIX_SPLIT)
      {
        lOther.Split();
        fix2 = SDS_FIX_WEAKER;
      }
    }
  }
  else if (len == 1 && lOther.len == 2)
  {
    if (list[0].Fix12(lOther.list[1], lOther.list[0], fix1, fix2))
    {
      if (fix2 == SDS_FIX_COLLAPSE)
      {
        lOther.list[0] = lOther.list[1];
        lOther.len = 1;
        fix2 = SDS_FIX_WEAKER;
      }
    }
  }
  else if (len == 2 && lOther.len == 1)
  {
    if (lOther.list[0].Fix12(list[1], list[0], fix2, fix1))
    {
      if (fix1 == SDS_FIX_COLLAPSE)
      {
        list[0] = list[1];
        len = 1;
        fix1 = SDS_FIX_WEAKER;
      }
    }
  }
  else if (len == 1 && lOther.len >= 3)
    list[0].Fix1n(lOther.list[lOther.len-1], fix1, fix2);
  else if (len >= 3 && lOther.len == 1)
    lOther.list[0].Fix1n(list[len-1], fix2, fix1);

  if (fix1 == SDS_FIX_UNCHANGED && fix2 == SDS_FIX_UNCHANGED)
    return false;

  return true;
}


void TrickList::Split()
{
  len = 2;
  list[1] = list[0];
  list[1].PunchOut(1);
  list[0].PunchOut(0);
}


PosType TrickList::ConnectFirst(
  const PosType pend)
{
  if (TrickList::GetFirstEnd() != pend)
    return QT_LHO; // Anything
  
  return TrickList::ConnectFirst();
}


PosType TrickList::ConnectFirst()
{
  assert(len > 0);
  if (len == 1)
  {
    // Must be a connector to the other side.
    list[0].SetEnd(QT_BOTH);
    return QT_BOTH;
  }
  
  len--;
  return list[len-1].Connect(list[len]);
}


void TrickList::Print(
  ostream& out,
  const unsigned d,
  const unsigned a) const
{
  unsigned offset = 0;
  for (unsigned p = 0; p < len; p++)
    list[len-1-p].Print(out, d, a, offset);
}


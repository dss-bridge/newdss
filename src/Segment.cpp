/* 
   SDS, a bridge single-suit double-dummy quick-trick solver.

   Copyright (C) 2015-16 by Soren Hein.

   See LICENSE and README.
*/

#include <assert.h>

#include "Trick.h"
#include "Segment.h"
#include "Holding.h"

using namespace std;


Segment::Segment()
{
  Segment::Reset();
}


Segment::~Segment()
{
}


void Segment::Reset()
{
  len = 0;
}


bool Segment::Set1(
  const Trick& trick)
{
  len = 1;
  list[0] = trick;
  return true;
}


bool Segment::Set2(
  const Trick& trick1,
  const Trick& trick2)
{
  len = 2;
  list[1] = trick1;
  list[0] = trick2;
  assert(list[0].Extends(list[1]));
  return true;
}


void Segment::PunchOut(
  const unsigned no)
{
  // no is the LOGICAL number to punch out.  The fact that
  // Segment stores moves in reverse order internally is
  // transparent to TrickList.
  
  assert(no < len);
  assert(len > 1);

  unsigned realNo = len - 1 - no;
  for (unsigned i = realNo+1; i < len; i++)
    list[i-1] = list[i];
  
  len--;
  if (realNo+1 < len)
    assert(list[realNo].Extends(list[realNo+1]));
}


void Segment::SetStart(
  const PosType start)
{
  assert(len > 0);
  list[len-1].SetStart(start);

  if (len > 1 && list[len-2].ReduceBoth(list[len-1]))
    len--;
}


PosType Segment::GetStart() const
{
  assert(len > 0);
  return list[len-1].trick.start;
}


void Segment::SetEnd(
  const PosType end)
{
  assert(len > 0);
  list[0].SetEnd(end);
}


PosType Segment::GetEnd() const
{
  assert(len > 0);
  return list[0].GetEnd();
}


unsigned Segment::GetRanks() const
{
  assert(len > 0);
  return list[len-1].trick.ranks;
}


unsigned Segment::GetLength() const
{
  return len;
}


unsigned Segment::GetTricks() const
{
  unsigned t = 0;
  for (unsigned l = 0; l < len; l++)
    t += list[l].trick.cashing;
  return t;
}


void Segment::GetSummaryTrick(
  Trick& summaryTrick,
  const bool lastFlag) const
{
  if (len == 1)
  {
    summaryTrick = list[0];
    return;
  }

  assert(len == 2);

  summaryTrick = list[1];
  summaryTrick.trick.cashing += list[0].trick.cashing;
  if (list[0].trick.ranks < summaryTrick.trick.ranks)
    summaryTrick.trick.ranks = list[0].trick.ranks;

  PosType e = list[0].trick.end;
  if (e == QT_BOTH)
    summaryTrick.trick.end = QT_BOTH;
  else if (list[0].trick.start == QT_BOTH && 
      list[1].trick.end != e &&
      lastFlag)
    summaryTrick.trick.end = QT_BOTH;
  else
    summaryTrick.trick.end = e;
}


bool Segment::CanExpand() const
{
  if (len == 1)
    return (list[0].trick.end == QT_BOTH);
  else
    return false;
}


bool Segment::CanSplit() const
{
  // Actually it should also be OK for first start to be QT_BOTH.
  return (len == 2 && 
    list[1].trick.start != QT_BOTH &&
    list[0].trick.start == QT_BOTH);
}


void Segment::Split(
  const PosType endVal)
{
  assert(len == 2);
  list[0].trick.start = list[1].trick.start;
  list[0].trick.end = endVal;
  list[0].trick.cashing += list[1].trick.cashing;
  if (list[1].trick.ranks < list[0].trick.ranks)
    list[0].trick.ranks = list[1].trick.ranks;
  len = 1;
}


bool Segment::IsSimpleComplement(
  const Segment& seg2) const
{
  if (len != 1 || seg2.len != 1)
    return false;

  return list[0].IsSimpleComplement(seg2.list[0]);
}


bool Segment::IsAA1ATrick() const
{
  if (len != 1)
    return false;

  return (list[0].trick.start == QT_ACE &&
      list[0].trick.end == QT_ACE &&
      list[0].trick.cashing == 1 &&
      list[0].trick.ranks == SDS_ACE);
}


unsigned Segment::xBNo(
  const PosType start) const
{
  if (len == 1 &&
      list[0].trick.start == start &&
      list[0].trick.end == QT_BOTH)
    return list[0].trick.cashing;
  else
    return 0;
}


CmpTrickType Segment::Compare(
  const Segment& seg2,
  const unsigned runRankOld,
  const unsigned runRankNew) const
{
  if (len == 2 && seg2.len == 2)
  {
    CmpTrickType c1 = list[1].Compare(seg2.list[1]);
    if (c1 == SDS_TRICK_RANK_NEW_BETTER ||
        c1 == SDS_TRICK_RANK_OLD_BETTER)
    {
      if (list[0].Compare(seg2.list[0]) == SDS_TRICK_SAME)
        return c1;
    }
  }

  Trick t1;
  Segment::GetSummaryTrick(t1);
  t1.trick.ranks = Min(t1.trick.ranks, runRankOld);
  Trick t2;
  seg2.GetSummaryTrick(t2);
  t2.trick.ranks = Min(t2.trick.ranks, runRankNew);
  return t1.Compare(t2);
}


unsigned Segment::GetLowestRank() const
{
  assert(len > 0);
  if (len == 1)
    return (list[0].trick.ranks == SDS_VOID ? 0 : list[0].trick.ranks);
  else
    return Min(list[0].trick.ranks, list[1].trick.ranks);
}


void Segment::FixLowestRanks(
  const unsigned rLower)
{
  if (len == 1)
  {
    assert(list[0].trick.ranks > rLower);
    list[0].trick.ranks = rLower;
  }
  else if (list[0].trick.ranks == SDS_VOID)
  {
    assert(list[1].trick.ranks > rLower);
    list[1].trick.ranks = rLower;
  }
  else
  {
    assert(list[0].trick.ranks > rLower);
    list[0].trick.ranks = rLower;
  }
}


bool Segment::EqualsExceptStart(
  const Segment& seg2,
  const bool lastFlag) const
{
  Trick t1;
  Segment::GetSummaryTrick(t1, lastFlag);
  Trick t2;
  seg2.GetSummaryTrick(t2, lastFlag);
  return t1.EqualsExceptStart(t2);
}


bool Segment::EqualsExceptEnd(
  const Segment& seg2) const
{
  Trick t1;
  Segment::GetSummaryTrick(t1);
  Trick t2;
  seg2.GetSummaryTrick(t2);
  return t1.EqualsExceptEnd(t2);
}


bool Segment::operator == (
  const Segment& seg2) const
{
  if (seg2.len != len)
    return false;

  for (unsigned p = 0; p < len; p++)
    if (list[p] != seg2.list[p])
      return false;

  return true;
}


bool Segment::operator != (
  const Segment& t2) const
{
  return ! (* this == t2);
}


void Segment::Localize(
  const Holding& holding)
{
  for (unsigned p = 0; p < len; p++)
    list[p].Localize(holding);
}


void Segment::FixEquals(
  const Holding& holding)
{
  unsigned fixedRank;
  unsigned rankToFix = holding.PossiblyFixRank(fixedRank);
  if (rankToFix == SDS_VOID)
    return;

  Segment::FixEquals(rankToFix, fixedRank);
}


void Segment::FixEquals(
  const unsigned rankToFix,
  const unsigned fixedRank)
{
  // AKQ2 / - / T9 / J876: Turn later T to 9.
  for (unsigned l = 0; l < len; l++)
    if (list[len-1-l].trick.ranks <= rankToFix &&
        list[len-1-l].trick.ranks > fixedRank)
      list[len-1-l].trick.ranks = fixedRank;
}


bool Segment::Prepend(
  const Holding& holding,
  const bool lastSegFlag)
{
  /*
     When adding a move in front of the segment:

     * The stored ranks are relative to the stored ranks and
       have to be mapped back up to the ranks of the new trick.
     * The ace holder (winning side) of the stored trick may
       not be the same either.
     * It may be possible to merge the new move into the first
       stored move.

     In merging, we do lose some information or freedom in general.
     AQ / Kx may give two entries to the AQ side.  Our goal is
     that we at least retain information that lets us communicate
     once.
  */

  assert(len > 0);

  const Trick& mergingMove = holding.GetTrick();
/*
if (holding.GetLength(QT_PARD) == 5)
{
cout << "mergingMove\n";
mergingMove.Print();
cout << "\n";
holding.PrintPlay();
cout << "\n";
holding.Print();
cout << "\n";
}
*/

  // Segment::FixEquals(holding);
  bool lastFlag = (lastSegFlag && (len == 1));
  const bool mergeSpecialFlag = holding.GetMergeType();


  switch(list[len-1].Prepend(mergingMove, mergeSpecialFlag, lastFlag))
  {
    case SDS_APPEND_MERGED:
      return true;
    case SDS_APPEND_NEW_SEG:
      if (len == 1 && 
          (lastSegFlag || holding.GetMinDeclLength() == 2) &&
          Segment::PrependCommon(mergingMove))
        return true;
      else if (lastFlag && Segment::PrependSpecial(mergingMove, holding))
        return true;
      else
        return false;
    default:
      if (len == 1)
      {
        if (lastFlag && Segment::PrependSpecialAlreadyMerged(
          mergingMove, holding))
        {
          // Worked.
        }
        else
        {
          list[1] = mergingMove;
          len = 2;
          if (holding.GetLength(QT_LHO) == 0)
            Segment::PrependFix(lastFlag);
        }
      }
      else if (len == 2)
        Segment::PrependDeep(mergingMove);
      else
        assert(false);

      return true;
  }
}


void Segment::PrependSimple(
  const Trick& tPrep)
{
  switch(list[0].Prepend(tPrep, false, false))
  {
    case SDS_APPEND_MERGED:
      break;
    case SDS_APPEND_NEW_SEG:
      assert(false);
      break;
    default:
      list[1] = tPrep;
      len = 2;
      break;
  }
}


bool Segment::PrependCommon(
  const Trick& mergingMove)
{
  if (mergingMove.trick.start != QT_BOTH &&
      mergingMove.trick.end == QT_ACE &&
      list[0].trick.start == QT_PARD &&
      list[0].trick.end == QT_ACE &&
      list[0].trick.cashing == 1)
  {
    // AAnr + PA1s = AA(n+1)min(r,s).
    // PAnr + PA1s = PA(n+1)min(r,s).
    // It is always also possible to cash the last trick from A.

    list[0].trick.start = mergingMove.trick.start;
    list[0].trick.cashing += mergingMove.trick.cashing;
    if (mergingMove.trick.ranks < list[0].trick.ranks)
      list[0].trick.ranks = mergingMove.trick.ranks;
    return true;
  }
  else if (mergingMove.trick.start != QT_BOTH &&
      mergingMove.trick.end == QT_PARD &&
      list[0].trick.start == QT_ACE &&
      list[0].trick.end == QT_PARD &&
      list[0].trick.cashing == 1)
  {
    // APnr + AP1s = AP(n+1)min(r,s).
    // PPnr + AP1s = PP(n+1)min(r,s).
    // It is always also possible to cash the last trick from A.

    list[0].trick.start = mergingMove.trick.start;
    list[0].trick.cashing += mergingMove.trick.cashing;
    if (mergingMove.trick.ranks < list[0].trick.ranks)
      list[0].trick.ranks = mergingMove.trick.ranks;
    return true;
  }
  else
    return false;
}


bool Segment::PrependSpecial(
  const Trick& mergingMove,
  const Holding& holding)
{
  if (list[0].trick.start == QT_ACE &&
      list[0].trick.end == QT_ACE)
  {
    unsigned mdl = holding.GetMinDeclLength();
    if (mdl >= 2 &&
        mergingMove.trick.start == QT_PARD &&
        mergingMove.trick.end == QT_PARD &&
        (list[0].trick.cashing == 1 ||
        holding.IsNotFinesse(QT_PARD)))
    {
      // PPnr + AAms = PB(n+m)min(r,s).
      // It is always also possible to cash the last trick from P.
      list[0].trick.start = QT_PARD;
      list[0].trick.end = QT_BOTH;
      list[0].trick.cashing += mergingMove.trick.cashing;
      if (mergingMove.trick.ranks < list[0].trick.ranks)
        list[0].trick.ranks = mergingMove.trick.ranks;
      return true;
    }
    else if (mdl >= 2 &&
        mergingMove.trick.start == QT_ACE &&
        mergingMove.trick.end == QT_PARD &&
        mergingMove.trick.cashing == 1)
    {
      // AP1top + AAns = AP(n+1)min(r,s).  Can't be a finesse.
      // Could probably make AB here as well?
      list[0].trick.start = QT_ACE;
      list[0].trick.end = QT_BOTH;
      list[0].trick.cashing += mergingMove.trick.cashing;
      if (mergingMove.trick.ranks < list[0].trick.ranks)
        list[0].trick.ranks = mergingMove.trick.ranks;
      return true;
    }
    else
      return false;
  }
  else if (list[0].trick.start == QT_PARD &&
      list[0].trick.end == QT_PARD)
  {
    // Can combine with previous once they have grown to be identical.

    unsigned mdl = holding.GetMinDeclLength();
    if (mdl >= 2 &&
        mergingMove.trick.start == QT_ACE &&
        mergingMove.trick.end == QT_ACE &&
        (list[0].trick.cashing == 1 ||
        holding.IsNotFinesse(QT_ACE)))
    {
      // AAnr + PPms = AB(n+m)min(r,s).
      // It is always also possible to cash the last trick from A.
      list[0].trick.start = QT_ACE;
      list[0].trick.end = QT_BOTH;
      list[0].trick.cashing += mergingMove.trick.cashing;
      if (mergingMove.trick.ranks < list[0].trick.ranks)
        list[0].trick.ranks = mergingMove.trick.ranks;
      return true;
    }
    else if (mdl >= 2 &&
        mergingMove.trick.start == QT_PARD &&
        mergingMove.trick.end == QT_ACE &&
        mergingMove.trick.cashing == 1 &&
        holding.HasRemainingPTop())
    {
      // PA1A + PPns = PA(n+1)min(s,A).  Can't be a finesse.
      // IsRealPP distinguishes between A9 / T6543 / KQJ87 / -,
      // where 7-void-A-3 leaves a "real" PP3J move, and
      // AQT8 / - / 9765 / KJ4, where 5-K-A-void leaves
      // QT8 / - / 976 / J4 which is only PP39 with a finesse.
      // Could probably make PB here as well?
      list[0].trick.start = QT_PARD;
      list[0].trick.end = QT_BOTH;
      list[0].trick.cashing += mergingMove.trick.cashing;
      if (mergingMove.trick.ranks < list[0].trick.ranks)
        list[0].trick.ranks = mergingMove.trick.ranks;
      return true;
    }
    else
      return false;
  }
  else
    return false;
}


bool Segment::PrependSpecialAlreadyMerged(
  const Trick& mergingMove,
  const Holding& holding)
{
  if (list[0].trick.start == QT_ACE &&
      list[0].trick.end == QT_PARD)
  {
    if (mergingMove.trick.start == QT_PARD &&
        mergingMove.trick.end == QT_ACE &&
        holding.APIsBP())
    {
      // PAnr + APms = PB(n+m)min(r,s) if AP can be BP.
      list[0].trick.start = QT_PARD;
      list[0].trick.end = QT_BOTH;
      list[0].trick.cashing += mergingMove.trick.cashing;
      if (mergingMove.trick.ranks < list[0].trick.ranks)
        list[0].trick.ranks = mergingMove.trick.ranks;
      return true;
    }
    else
      return false;
  }
  return false;
}


void Segment::PrependDeep(
  const Trick& mergingMove)
{
  if (mergingMove.trick.start == QT_BOTH)
  {
    if (mergingMove.trick.end != QT_BOTH &&
        list[1].trick.start == QT_BOTH &&
        list[1].trick.end == SDS_PARTNER[mergingMove.trick.end] &&
        list[0].trick.start == list[1].trick.end &&
        list[0].trick.end == mergingMove.trick.end)
    {
      // BA-BP-PA = BP-PA.
      // BP-BA-AP = BA-AP.

      list[1].trick.cashing += mergingMove.trick.cashing;
      if (mergingMove.trick.ranks < list[1].trick.ranks)
        list[1].trick.ranks = mergingMove.trick.ranks;
    }
    else
      assert(false);
  }
  else if (mergingMove.trick.start == list[1].trick.end)
  {
    // PA-AP-BA = PP-BA.
    // AP-PA-BP = AA-BP.
    // AP-BA-BP = AA-BP.
    // PA-BP-BA = PP-BA.

    list[1].trick.start = mergingMove.trick.start;
    list[1].trick.cashing += mergingMove.trick.cashing;
    if (mergingMove.trick.ranks < list[1].trick.ranks)
      list[1].trick.ranks = mergingMove.trick.ranks;

    if (mergingMove.trick.start == list[0].trick.start)
    {
      // AP-PA-AP = AP.
      // PA-AP-PA = PA.
      list[0].trick.cashing += list[1].trick.cashing;
      if (list[1].trick.ranks < list[0].trick.ranks)
        list[0].trick.ranks = list[1].trick.ranks;

      len--;
    }
  }
  else if (mergingMove.trick.start == mergingMove.trick.end &&
    list[1].trick.start == QT_BOTH &&
    list[1].trick.end == SDS_PARTNER[mergingMove.trick.start] &&
    list[0].trick.start == QT_BOTH &&
    list[0].trick.end == mergingMove.trick.start)
  {
    // AA-BP-BA = AP-BA.
    // PP-BA-BP = PA-BP.

    list[1].trick.start = mergingMove.trick.start;
    list[1].trick.cashing += mergingMove.trick.cashing;
    if (mergingMove.trick.ranks < list[1].trick.ranks)
      list[1].trick.ranks = mergingMove.trick.ranks;
  }
  else if (mergingMove.trick.start == mergingMove.trick.end &&
    list[1].trick.start == QT_BOTH)
  {
    // AA-B..., PP-B...

    list[1].trick.start = mergingMove.trick.start;
    list[1].trick.cashing += mergingMove.trick.cashing;
    if (mergingMove.trick.ranks < list[1].trick.ranks)
      list[1].trick.ranks = mergingMove.trick.ranks;
  }
  else
    assert(false);
}


void Segment::PrependFix(
  const bool lastFlag)
{
  if (list[1].trick.start == QT_PARD &&
      list[1].trick.end == QT_ACE &&
      list[0].trick.start == QT_ACE &&
      list[0].trick.end == QT_PARD)
  {
    // LHO is assumed void.
    if (lastFlag)
    {
      // PA + BP = PB.
      list[0].trick.start = QT_PARD;
      list[0].trick.end = QT_BOTH;
      list[0].trick.cashing += list[1].trick.cashing;
      if (list[1].trick.ranks < list[0].trick.ranks)
        list[0].trick.ranks = list[1].trick.ranks;
      len = 1;
    }
    else
    {
      // Keep separate.
      list[0].trick.start = QT_BOTH;
    }
  }
}


bool Segment::FixRanks(
  const unsigned rPrep)
{
  for (unsigned t = 0; t < len; t++)
  {
    if (list[len-1-t].trick.ranks == rPrep)
    {
      list[len-1-t].trick.ranks = SDS_VOID;
      return true;
    }
    else if (list[len-1-t].trick.ranks < rPrep)
      return true;
  }
  return false;
}


PosType Segment::Connect(
  const Segment& sPrepend)
{
  // This arises when we collapse the first two segments in a TrickList.
  // It only works for segments of 1 or 2 tricks, which is the case.

  PosType pend = sPrepend.list[0].trick.end;

  list[0].trick.start = pend;
  if (len == 2)
  {
    // This works for all possible combinations.

    list[0].trick.cashing += list[1].trick.cashing;
    if (list[1].trick.ranks < list[0].trick.ranks)
      list[0].trick.ranks = list[1].trick.ranks;
    len = 1;
  }

  Segment::PrependSimple(sPrepend.list[0]);

  // A third move must be prependable.
  if (sPrepend.len == 1)
  {
    // Nothing more.
  }
  else if (len == 1)
    Segment::PrependSimple(sPrepend.list[1]);
  else
    Segment::PrependDeep(sPrepend.list[1]);

  return pend;
}


bool Segment::Fix11(
  Segment& seg2,
  FixType& fix1,
  FixType& fix2)
{
  fix1 = SDS_FIX_UNCHANGED;
  fix2 = SDS_FIX_UNCHANGED;

  if (len == 1 && seg2.len == 2)
    return Segment::Fix11_12(seg2, fix1, fix2);
  else if (len == 2 && seg2.len == 1)
    return seg2.Fix11_12(* this, fix2, fix1);
  else if (len > 1 || seg2.len > 1)
    return false;

  /*
     We only want the candidates for weakening, not purging.

          AA  AP  AB  PA  PP  PB  BA  BP  BB (old, t1)
        +-----------------------------------
     AA |  -   -  C0   -   -   -  CA  C8  -
     AP |  -   -  C0   -   -  C2   -  C8  -
     AB | C1  C1   -  C2   -  C2  C9  C8 CB
        |
     PA |  -   -  C2   -   -  C0  C8   -  -
     PP |  -   -   -   -   -  C0  C8  CA  -
     PB |  -  C2  C2  C1  C1   -  C8  C9 CB
        |
     BA | CA   -  C9  C8  C8  C8   - C35 C67
     BP | C8  C8  C8   -  CA  C9 C35   - C67
     BB |  -   -  CB   -   -  CB C67 C67   - 
     (new, t2)

  */

  Trick& t1 = list[0];
  Trick& t2 = seg2.list[0];

  if (t1.trick.start != QT_BOTH && t2.trick.start != QT_BOTH)
  {
    CmpType c = t1.CashRankOrder(t2.trick.cashing, t2.trick.ranks);
    if (t1.trick.start == t2.trick.start)
    {
      if (t1.trick.end == QT_BOTH && 
          t2.trick.end != QT_BOTH &&
          c == SDS_NEW_BETTER)
      {
        // C0: AA+AB = AA+AP, AP+AB = AP+AA, PA+PB = PA+PP, PA+PB = PA+PP.
        t1.trick.end = SDS_PARTNER[t2.trick.end];
        fix1 = SDS_FIX_WEAKER;
        return true;
      }
      else if (t1.trick.end != QT_BOTH && 
          t2.trick.end == QT_BOTH &&
          c == SDS_OLD_BETTER)
      {
        // C1: AB+AA = AP+AA, AB+AP = AA+AP, PB+PA = PP+PA, PB+PA = PP+PA.
        t2.trick.end = SDS_PARTNER[t1.trick.end];
        fix2 = SDS_FIX_WEAKER;
        return true;
      }
    }
    else if (c == SDS_SAME &&
        ((t1.trick.end == QT_BOTH && t2.trick.start != t2.trick.end ) ||
         (t2.trick.end == QT_BOTH && t1.trick.start != t1.trick.end)))
    {
      // C2: AB+Ax either way, PB+Px either way,
      // when tricks and ranks are the same.
      t1.trick.start = QT_BOTH;
      t1.trick.end = QT_BOTH;
      fix1 = SDS_FIX_STRONGER;
      fix2 = SDS_FIX_PURGED;
      return true;
    }
  }
  else if (t1.trick.start == QT_BOTH && t2.trick.start == QT_BOTH)
  {
    CmpType c = t1.CashRankOrder(t2.trick.cashing, t2.trick.ranks);
    if (t1.trick.end != QT_BOTH &&
        t2.trick.end == SDS_PARTNER[t1.trick.end])
    {
      // C3-C5: BA+BP either way.
      switch (c)
      {
        case SDS_SAME:
          t1.trick.end = QT_BOTH;
          fix1 = SDS_FIX_STRONGER;
          fix2 = SDS_FIX_PURGED;
          return true;
        case SDS_OLD_BETTER:
          t2.trick.start = t1.trick.end;
          fix1 = SDS_FIX_UNCHANGED;
          fix2 = SDS_FIX_WEAKER;
          return true;
        case SDS_NEW_BETTER:
          t1.trick.start = t2.trick.end;
          fix1 = SDS_FIX_WEAKER;
          fix2 = SDS_FIX_UNCHANGED;
          return true;
        default:
          break;
      }
    }
    // C6, C7 don't happen:  BB+BA with BA better etc.
  }
  else if (t1.trick.start == QT_BOTH)
    return Segment::Fix11_OneB(seg2, fix1, fix2);
  else
    return seg2.Fix11_OneB(* this, fix2, fix1);

  return false;
}


bool Segment::Fix11_OneB(
  Segment& seg2,
  FixType& fix1,
  FixType& fix2) const
{
  const Trick& t1 = list[0];
  Trick& t2 = seg2.list[0];

  CmpType c = t1.CashRankOrder(t2.trick.cashing, t2.trick.ranks);

  // CA: BA+AA where AA is better, BP+PP ditto.  Doesn't happen.
  // CB: BB+AB or PB where the latter is better.  Doesn't happen.

  if (t1.trick.end != QT_BOTH &&
      t2.trick.start == SDS_PARTNER[t1.trick.end] &&
      (c == SDS_OLD_BETTER || c == SDS_SAME))
  {
    // C8: BAnr or P*ms equals BAnr if t1 >= t2.
    // Similarly for BPnr or A*ms.
    fix1 = SDS_FIX_UNCHANGED;
    fix2 = SDS_FIX_PURGED;
    return true;
  }
  else if (t2.trick.start == t1.trick.end &&
    t2.trick.end == QT_BOTH && 
      (c == SDS_OLD_BETTER || c == SDS_SAME))
  {
    // C9: BA or AB; or BP or PB.

    t2.trick.end = SDS_PARTNER[t2.trick.start];

    fix1 = SDS_FIX_UNCHANGED;
    fix2 = SDS_FIX_WEAKER;
    return true;
  }
  else
    return false;
}


bool Segment::Fix11_12(
  Segment& seg2,
  FixType& fix1,
  FixType& fix2)
{
  /* Two single segments with 1 and 2 Tricks, respectively.
     There are eight possibilities for seg2, considering that
     these are last segments (see Trick::Prepend).
     We only want the candidates for weakening, not purging.

          PA+AP PB+AP BA+AP BB+AP AP+PA AB+PA BP+PA BB+PA 
            ~PP   ~PP   ~BP   ~BP   ~AA   ~AA   ~BA   ~BA
        +------------------------------------------------
     AA |                                      long  long  
     AP |              long  long                          
     AB |              both  both short short  both  both  (*)
        |
     PA |                                      long  long  
     PP |              long  long                          
     PB | short short  both  both              both  both  (*)
        |
     BA |                         short short              
     BP | short short                                      
     BB | shspl shspl short short shspl shspl short short  

     long  = Only long segment might be weakened.
     short = Only short segments might be weakened.
     shspl = Short segment might be split (BB -> AB or PA).
             Not clear and currently not implemented.
     both  = Both segments might be weakened.

     There are many theoretical options.  Only (*) happen.
  */

  if (list[0] == seg2.list[1])
  {
    // seg2 clearly better.
    fix1 = SDS_FIX_PURGED;
    fix2 = SDS_FIX_UNCHANGED;
    return true;
  }

  Trick& t1 = list[0];
  Trick& t20 = seg2.list[1];
  Trick& t21 = seg2.list[0];

  // PB vs PA+BP will taken care of elsewhere:  The latter will
  // be followed by another segment, so will win.
  if (t21.trick.start == QT_BOTH)
    return false;

  assert(t21.trick.end == SDS_PARTNER[t21.trick.start] &&
      t20.trick.start != SDS_PARTNER[t21.trick.end]);

  CmpType c = t1.CashRankOrder(
      t20.trick.cashing + t21.trick.cashing,
      Min(t20.trick.ranks, t21.trick.ranks));

  if (t1.trick.start != QT_BOTH &&
      t1.trick.end == QT_BOTH &&
      t20.trick.start != SDS_PARTNER[t1.trick.start] &&
      c == SDS_NEW_BETTER)
  {
    // C1: AB / AP+PA or AB+PA, PB / PA+AP or PB+AP.
    t1.trick.end = t21.trick.start;
    fix1 = SDS_FIX_WEAKER;
    fix2 = SDS_FIX_UNCHANGED;
    return true;
  }
  else if (c == SDS_SAME &&
      t1.trick.start != QT_BOTH &&
      t1.trick.start == t20.trick.start &&
      t1.trick.end == SDS_PARTNER[t1.trick.start] &&
      t21.trick.end == t1.trick.start)
  {
    // AP or AP+PA, or PA or PA+AP.
    t1.trick.end = QT_BOTH;
    fix1 = SDS_FIX_STRONGER;
    fix2 = SDS_FIX_PURGED;
    return true;
  }
  else if (t1.trick.start != QT_BOTH &&
    t1.trick.end == SDS_PARTNER[t1.trick.start] &&
    t20.trick.start == t1.trick.start &&
    t21.trick.end == t20.trick.start &&
    t20.trick.end == t1.trick.end &&
    t1.CashRankOrder(t20.trick.cashing, t20.trick.ranks) != SDS_NEW_BETTER)
  {
    // We will consider PA1A + AP29 to be PP39 if compared
    // against PA3Q.  There is no point in having PA1A available
    // when PA3Q is also there.
    t21.trick.start = t20.trick.start;
    t21.trick.cashing += t20.trick.cashing;
    if (t20.trick.ranks < t21.trick.ranks)
      t21.trick.ranks = t20.trick.ranks;
    seg2.len = 1;
    fix2 = SDS_FIX_WEAKER;
    return true;

  }
  else
    return false;
}


bool Segment::Fix12(
  Segment& seg20,
  Segment& seg21,
  FixType& fix1,
  FixType& fix2)
{
  /* One single segment versus two segments, all with length 1.
     We only want the candidates for weakening, not purging.

               AA   AP   AB   PA   PP   PB   BA   BP   BB (old)
           +---------------------------------------------
     AA+Px |    -    -   up    -    -    -   up    - spec
     AP+Ax |    -    -   up    -    -    -    -   up spec
           |
     PA+Px |    -    -    -    -    -   up   up    - spec
     PP+Ax |    -    -    -    -    -   up    -   up spec
           |
     BA+Px | left    - both left    - both    - both   up
     BP+Ax |    - left both    - left both    - both   up
     (new)

     up   = The short one is the candidate for weakening.
     left = The long one is the candidate for weakening.
     both = Either might be weakened.
     spec = Special case (not implemented).

     Several cases don't happen in practice.
     We use PP <= BA and AA <= BP in several places.  Think! :-)
  */

  if (len == 1 && seg20.len == 2 && seg21.len == 1 &&
      Segment::Fix12Special(seg20, seg21, fix1, fix2))
    return true;

  if (len == 1 && seg20.len == 2 &&
      Segment::Fix1nSpecial(seg20, fix1, fix2))
    return true;

  if (len > 1 || seg20.len > 1 || seg21.len > 1)
    return false;

  Trick& t1 = list[0];
  Trick& t20 = seg20.list[0];
  Trick& t21 = seg21.list[0];

  CmpType c = t1.CashRankOrder(
      t20.trick.cashing + t21.trick.cashing,
      Min(t20.trick.ranks, t21.trick.ranks));

  if (t1.trick.start == t20.trick.start &&
      t1.trick.end == QT_BOTH &&
      (t1.trick.cashing < t20.trick.cashing ||
      (t1.trick.cashing == t20.trick.cashing && 
       t1.trick.ranks <= t20.trick.ranks)))
  {
    // AB / Ax+yz, PB / Px+yz, BB / Bx+yz
    // Note that BB turns into x-(not x), not just B-(not x).
    if (t1.trick.start == QT_BOTH)
      t1.trick.start = t20.trick.end;
    t1.trick.end = SDS_PARTNER[t20.trick.end];
    fix1 = SDS_FIX_WEAKER;
    fix2 = SDS_FIX_UNCHANGED;
    return true;
  }
  else if (t1.trick.start != QT_BOTH &&
      t1.trick.end == QT_BOTH &&
      t20.trick.start == QT_BOTH &&
      t1.trick.cashing <= t20.trick.cashing)
  {
    // AB / BA+Px or BP+Ax, PB / BA+Px or BP+Ax.
    t1.trick.end = SDS_PARTNER[t20.trick.end];
    fix1 = SDS_FIX_WEAKER;
    fix2 = SDS_FIX_UNCHANGED;
    return true;
  }
  else if (t1.trick.start == QT_BOTH &&
      t1.trick.end != QT_BOTH &&
      t20.trick.start == QT_BOTH &&
      t20.trick.end == SDS_PARTNER[t1.trick.end])
  {
    if (t1.trick.cashing >= t20.trick.cashing + t21.trick.cashing)
    {
      // BAnr (r > s) or _B_Pms+AA(n-m)-  equals  BAnr or _A_Pms+AA(n-m)-.
      // BPnr (r > s) or _B_Ams+PP(n-m)-  equals  BPnr or _P_Ams+PP(n-m)-.
      t20.trick.start = t21.trick.start;
      fix1 = SDS_FIX_UNCHANGED;
      fix2 = SDS_FIX_WEAKER;
      return true;
    }
    else if (t1.trick.cashing <= t20.trick.cashing)
    {
      // BA2K or BP3T + AA1-  equals  PA2K or same.
      t1.trick.start = t20.trick.end;
      fix1 = SDS_FIX_WEAKER;
      fix2 = SDS_FIX_UNCHANGED;
      return true;
    }
    else if (t1.trick.start != QT_BOTH &&
        t1.trick.start == t1.trick.end &&
        t20.trick.start == QT_BOTH &&
        t20.trick.end == SDS_PARTNER[t1.trick.start] &&
        t1.trick.cashing <= t20.trick.cashing + t21.trick.cashing)
    {
      // AA or BP+Ax, PP or BA+Px.
      fix1 = SDS_FIX_PURGED;
      fix2 = SDS_FIX_UNCHANGED;
      return true;
    }
  }
  else if (t1.trick.start != QT_BOTH &&
      t1.trick.end == SDS_PARTNER[t1.trick.start] &&
      t20.trick.start == SDS_PARTNER[t1.trick.start] &&
      t20.trick.end == SDS_PARTNER[t1.trick.start] &&
      t21.trick.start == t1.trick.start &&
      t21.trick.end == SDS_PARTNER[t1.trick.start] &&
      c == SDS_OLD_BETTER)
  {
    // PAnr  ">="  AA1A + PAms (r <= s, n >= m+1).
    // APnr  ">="  PP1A + APms (r <= s, n >= m+1).
    fix1 = SDS_FIX_UNCHANGED;
    fix2 = SDS_FIX_PURGED;
    return true;
  }

  return false;
}


bool Segment::Fix1n(
  Segment& seg20,
  FixType& fix1,
  FixType& fix2) const
{
  if (len == 1 && seg20.len == 2 &&
      Segment::Fix1nSpecial(seg20, fix1, fix2))
    return true;
  else
  return false;
}


bool Segment::Fix12Special(
  Segment& seg20,
  Segment& seg21,
  FixType& fix1,
  FixType& fix2) const
{
  const Trick& t1 = list[0];
  Trick& t200 = seg20.list[1];
  Trick& t201 = seg20.list[0];
  Trick& t21 = seg21.list[0];

  // AB3Q or AA19 + BP1- + AA2-: The first segment can be simplified
  // so AP29 + AA2-, as AA29 would never be used (no AP0- move).

  if (t200.trick.start != QT_BOTH &&
      t200.trick.end == t200.trick.start &&
      t201.trick.start == QT_BOTH &&
      t201.trick.end == SDS_PARTNER[t200.trick.start] &&
      t1.trick.start != t201.trick.end &&
      t1.trick.end != t201.trick.end)
  {
    unsigned char m = t200.trick.cashing + t201.trick.cashing;
    unsigned char n = m + t21.trick.cashing;
    unsigned char s = Min(t200.trick.ranks, t201.trick.ranks);
    unsigned char t = s;
    if (t21.trick.ranks < t)
      t = t21.trick.ranks;

    if ((n > t1.trick.cashing ||
       (n == t1.trick.cashing && t > t1.trick.ranks)) && 
       (m < t1.trick.cashing || 
       (m == t1.trick.cashing && s <= t1.trick.ranks)))
    {
      t201.trick.start = t200.trick.start;
      t201.trick.cashing += t200.trick.cashing;
      if (t200.trick.ranks < t201.trick.ranks)
        t201.trick.ranks = t200.trick.ranks;
      seg20.len = 1;
      fix1 = SDS_FIX_UNCHANGED;
      fix2 = SDS_FIX_WEAKER;
      return true;
    }
  }

  // xAnr or xA-m1-s1 + BP-m2-s2 + AA-m3-s3,
  // with m1+m2+m3 and min(s1,s2,s3) <= n, r).
  // xPnr or xP-m1-s1 + BA-m2-s2 + PP-m3-s3 similarly.
  // (a) If we stop after the first segment, we get xB-(m1+m2)-min(s1,s2)
  // which simplifies to xP.
  // (b) If we have a move of PA0-, then
  // xA-m1-s1 + BP-m2-s2 + PA-m3-s3 = xA-m1-s1 + BA-(m1+m2)-min(s1,s2)
  // which is xA(m1+m2+m3)-min(s1,s2,s3) which is worse than xAnr.
  // This is following our usual rules which do throw away some
  // information, but as we don't mind this in general, we don't mind
  // it here either.
  // If we have xBnr, not xAnr, then (a) loses as well.

  if (t1.trick.start != QT_BOTH &&
      (t1.trick.end == QT_BOTH || t1.trick.end == t21.trick.end) &&
      t200.trick.start == t1.trick.start &&
      t200.trick.end == t21.trick.end &&
      t201.trick.start == QT_BOTH &&
      t201.trick.end == SDS_PARTNER[t200.trick.end] &&
      t21.trick.end == t21.trick.start)
  {
    unsigned char m = t200.trick.cashing + t201.trick.cashing +
      t21.trick.cashing;
    unsigned char s = Min(t200.trick.ranks, t201.trick.ranks);
    if (t21.trick.ranks < s)
      s = t21.trick.ranks;

    if (m <= t1.trick.cashing && s <= t1.trick.ranks)
    {
      if (t1.trick.end == QT_BOTH)
      {
        fix1 = SDS_FIX_UNCHANGED;
        fix2 = SDS_FIX_PURGED;
        return true;
      }
      else
      {
return false;
        t201.trick.start = t200.trick.start;
        t201.trick.cashing += t200.trick.cashing;
        if (t200.trick.ranks < t201.trick.ranks)
          t201.trick.ranks = t200.trick.ranks;
        seg20.len = 1;

        fix1 = SDS_FIX_UNCHANGED;
        fix2 = SDS_FIX_COLLAPSE;
        return true;
      }
    }
  }
  return false;
}


bool Segment::Fix1nSpecial(
  Segment& seg20,
  FixType& fix1,
  FixType& fix2) const
{
  // APnr or AP-m1-s1 + PA-m2-s2 + P...
  // PAnr or PA-m1-s1 + AP-m2-s2 + A...
  // Will combine to AA if the AP move in itself is inferior.
  // We could do it anyway (losing more information), but this is
  // what we're comfortable with.
  // Same for ABnr rather than APnr.

  const Trick& t1 = list[0];
  Trick& t200 = seg20.list[1];
  Trick& t201 = seg20.list[0];

  if (t200.trick.start == t1.trick.start &&
      t201.trick.end == t200.trick.start &&
      (t1.trick.end == QT_BOTH || t1.trick.end == t200.trick.end))
  {
    if (t200.trick.cashing < t1.trick.cashing)
    {
      t201.trick.start = t200.trick.start;
      t201.trick.cashing += t200.trick.cashing;
      if (t200.trick.ranks < t201.trick.ranks)
        t201.trick.ranks = t200.trick.ranks;
      seg20.len = 1;
      fix1 = SDS_FIX_UNCHANGED;
      fix2 = SDS_FIX_WEAKER;
      return true;
    }
  }
  return false;
}


void Segment::Print(
  ostream& out) const
{
  for (unsigned p = 0; p < len; p++)
    list[len-1-p].Print(out);
}


void Segment::Print(
  ostream& out,
  const unsigned d,
  const unsigned a,
  unsigned& offset) const
{
  for (unsigned p = 0; p < len; p++)
    list[len-1-p].Print(out, d, a, offset + p);

  offset += len;
}


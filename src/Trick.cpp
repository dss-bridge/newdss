/* 
   SDS, a bridge single-suit double-dummy quick-trick solver.

   Copyright (C) 2015-16 by Soren Hein.

   See LICENSE and README.
*/

#include <iomanip>
#include <assert.h>

#include "Trick.h"
#include "Holding.h"

using namespace std;


const CmpDetailType cmpPlayToDetail[SDS_CMP_SIZE] =
{
  SDS_HEADER_SAME,
  SDS_HEADER_PLAY_NEW_BETTER,
  SDS_HEADER_PLAY_OLD_BETTER,
  SDS_HEADER_PLAY_DIFFERENT
};

const CmpDetailType cmpRanksToDetail[SDS_CMP_SIZE] =
{
  SDS_HEADER_SAME,
  SDS_HEADER_RANK_NEW_BETTER,
  SDS_HEADER_RANK_OLD_BETTER,
  SDS_HEADER_RANK_DIFFERENT
};

const CmpType cmpMergeMatrix[SDS_CMP_SIZE][SDS_CMP_SIZE] =
{
  // Same            New             Old             Diff
  {  SDS_SAME,       SDS_NEW_BETTER, SDS_OLD_BETTER, SDS_DIFFERENT },
  {  SDS_NEW_BETTER, SDS_NEW_BETTER, SDS_DIFFERENT,  SDS_DIFFERENT },
  {  SDS_OLD_BETTER, SDS_DIFFERENT,  SDS_OLD_BETTER, SDS_DIFFERENT },
  {  SDS_DIFFERENT,  SDS_DIFFERENT,  SDS_DIFFERENT,  SDS_DIFFERENT }
};

enum reachType
{
  SIDE_NONE = 0, 
  SIDE_ACE = 1, 
  SIDE_PARD = 2, 
  SIDE_BOTH = 3, 
  SIDE_SIZE = 4
};

const reachType posToReach[QT_SIZE] =
{ 
  SIDE_ACE, 
  SIDE_NONE, 
  SIDE_PARD, 
  SIDE_NONE, 
  SIDE_BOTH 
};

const reachType reachMap[SIDE_SIZE][QT_SIZE] =
{
  // LHO, RHO and BOTH as 2nd index can't happen.
  // QT_ACE     QT_LHO     QT_PARD    QT_RHO     QT_BOTH 
  {  SIDE_ACE,  SIDE_NONE, SIDE_PARD, SIDE_NONE, SIDE_NONE },
  {  SIDE_ACE,  SIDE_NONE, SIDE_BOTH, SIDE_NONE, SIDE_NONE },
  {  SIDE_BOTH, SIDE_NONE, SIDE_PARD, SIDE_NONE, SIDE_NONE },
  {  SIDE_BOTH, SIDE_NONE, SIDE_BOTH, SIDE_NONE, SIDE_NONE }
};

const CmpType reachMatrix[SIDE_SIZE][SIDE_SIZE] =
{
  // NONE            ACE             PARD            BOTH
  {  SDS_SAME,       SDS_NEW_BETTER, SDS_NEW_BETTER, SDS_NEW_BETTER },
  {  SDS_OLD_BETTER, SDS_SAME,       SDS_DIFFERENT,  SDS_NEW_BETTER },
  {  SDS_OLD_BETTER, SDS_DIFFERENT,  SDS_SAME,       SDS_NEW_BETTER },
  {  SDS_OLD_BETTER, SDS_OLD_BETTER, SDS_OLD_BETTER, SDS_SAME }
};

const char POS_NAMES[QT_SIZE][10] =
{
  "Ace", "LHO", "Pard", "RHO", "Both"
};



Trick::Trick()
{
  Trick::Reset();
}


Trick::~Trick()
{
}


void Trick::Reset()
{
}


bool Trick::Set(
  const PosType start,
  const PosType end,
  const unsigned ranks,
  const unsigned cashing)
{
  trick.start = start;
  trick.end = end;
  trick.ranks = ranks;
  trick.cashing = cashing;
  return true;
}


void Trick::SetStart(
  const PosType start)
{
  trick.start = start;
}


void Trick::SetEnd(
  const PosType end)
{
  trick.end = end;
}


PosType Trick::GetStart() const
{
  return trick.start;
}


PosType Trick::GetEnd() const
{
  return trick.end;
}


unsigned Trick::GetCashing() const
{
  return trick.cashing;
}


unsigned Trick::GetRanks() const
{
  return trick.ranks;
}


bool Trick::operator == (
  const Trick& t2) const
{
  return (t2.trick.start == trick.start &&
          t2.trick.end == trick.end &&
          t2.trick.cashing == trick.cashing &&
          t2.trick.ranks == trick.ranks);
}


bool Trick::operator != (
  const Trick& t2) const
{
  return ! (* this == t2);
}


bool Trick::EqualsExceptStart(
  const Trick& t2) const
{
  if (t2.trick.start == trick.start ||
      trick.start == QT_BOTH ||
      t2.trick.start == QT_BOTH)
    return false;

  return (t2.trick.end == trick.end &&
          t2.trick.cashing == trick.cashing &&
          t2.trick.ranks == trick.ranks);
}


bool Trick::IsSimpleComplement(
  const Trick& t2) const
{
  if (t2.trick.start == trick.start ||
      trick.start == QT_BOTH ||
      t2.trick.start == QT_BOTH)
    return false;

  if (t2.trick.cashing != trick.cashing ||
      t2.trick.ranks != trick.ranks)
    return false;

  if (t2.trick.end == QT_BOTH && trick.end == QT_BOTH)
    return false;
  else if (t2.trick.end != QT_BOTH && trick.end != QT_BOTH)
    return false;
  else
    return true;
  // So AP+PB, AA+PB, AB+PA, AP+PB.
}


bool Trick::Extends(
  const Trick& tEarlier) const
{
  if (tEarlier.trick.end != QT_BOTH &&
      trick.start != QT_BOTH &&
      trick.start != tEarlier.trick.end)
    return false;
  else
    return true;
}


void Trick::Localize(
  const Holding& holding)
{
  // When a trick list is looked up, its parameters are relative
  // to its circumstances.  When a trick is appended to that list,
  // the trick has to be updated.

  if (holding.GetAceFlip())
  {
    if (trick.start != QT_BOTH)
      trick.start = SDS_PARTNER[trick.start];

    if (trick.end != QT_BOTH)
      trick.end = SDS_PARTNER[trick.end];
  }

  trick.ranks = holding.GetNewRank(trick.ranks);
}


AppendType Trick::Prepend(
  const Trick& tPrep,
  const bool mergeSpecialFlag,
  const bool lastFlag)
{
  /*      AA AP AB   PA PP PB   BA BP BB (this)
        +-------------------------------
     AA |  m  m  B    -  -  -    m sb  B
     AP |  -  -  -    s  m  B   sb  m  B
     AB |  m  m  B    s  m  B   sb sb  B
        |
     PA |  m  s  B    -  -  -    m sb  B
     PP |  -  -  -    m  m  B   sb  m  B
     PB |  m  s  B    m  m  B   sb sb  B
        |
     BA |  m  s  B    -  -  -    m sb  B
     BP |  -  -  -    s  m  B   sb  m  B
     BB |  m  s  B    s  m  B   sb sb  B
     (tPrep)

     B = Merges to end on B in any case.
     b = Only merges to end on B if lastFlag is set.
     s = Same segment but does not merge.
     m = Merges normally (first start to last end).
     - = Does not merge (with possible exception).
  */

  // Single case of combining tricks that don't match up.
  if (mergeSpecialFlag &&
      lastFlag &&
      tPrep.trick.start != QT_BOTH &&
      tPrep.trick.end == QT_ACE &&
      trick.start == QT_PARD &&
      trick.end == QT_ACE)
  {
    // (A|P)A + PA = (A|P)A with maximum rank.
    trick.start = tPrep.trick.start;
    trick.ranks = Max(tPrep.trick.ranks, trick.ranks);
    trick.cashing += tPrep.trick.cashing;
    return SDS_APPEND_MERGED;
  }

  if (! Trick::Extends(tPrep))
    return SDS_APPEND_NEW_SEG;

  // The ending side is generally trick.end.
  // This gets overridden to QT_BOTH in some lastFlag cases.
  // This in turn gets overridden in some special void cases below.
  PosType newEnd;

  if (lastFlag &&
      trick.start == QT_BOTH &&
      tPrep.trick.end != trick.end)
  {
    // Special cases where we deviate from trick.end:
    // AA+BP, AP+BA, AB+BA, AB+BP, PA+BP, PP+BA, PB+BA, PB+BP,
    // BA+BP, BP+BA, BB+BA, BB+BP.
    newEnd = QT_BOTH;
  }
  else
    newEnd = trick.end;

  if (mergeSpecialFlag && lastFlag)
  {
    if (trick.end == QT_BOTH ||
       (tPrep.trick.start == QT_PARD && newEnd == QT_BOTH))
    {
      // Anything ending on B; PA+BP, PP+BA, PB+BA, PB+BP.
      // Rather special case.
      trick.start = QT_BOTH;
      trick.end = QT_ACE;
      trick.ranks = Max(tPrep.trick.ranks, trick.ranks);
      trick.cashing += tPrep.trick.cashing;
      return SDS_APPEND_MERGED;
    }
    else if (tPrep.trick.end == trick.end)
    {
      // AA+AA, AA+BA, AP+PP, AP+BP, 
      // PA+AA, PA+BA, PP+PP, PP+BP,
      // BA+AA, BA+BA, BP+PP, BP+BP: Use maximum rank.
      trick.start = tPrep.trick.start;
      trick.ranks = Max(tPrep.trick.ranks, trick.ranks);
      trick.cashing += tPrep.trick.cashing;
      return SDS_APPEND_MERGED;
    }
  }

  // Now we are down to regular prepends.
  if (trick.start != QT_BOTH &&
      trick.end == SDS_PARTNER[trick.start] &&
      tPrep.trick.start != trick.start)
  {
    // PA+AP, PB+AP, BA+AP, BB+AP; AP+PA, AB+PA, BP+PA, BB+PA.
    return SDS_APPEND_SAME_SEG;
  }
  else if (newEnd == QT_BOTH ||
      trick.start != QT_BOTH ||
      tPrep.trick.end == trick.end ||
      (tPrep.trick.end == QT_BOTH && tPrep.trick.start != trick.end))

      // tPrep.trick.start == tPrep.trick.end ||
      // tPrep.trick.start == SDS_PARTNER[trick.end] ||
      // tPrep.trick.end == trick.end)
  {
    trick.start = tPrep.trick.start;
    trick.end = newEnd;
    trick.ranks = Min(tPrep.trick.ranks, trick.ranks);
    trick.cashing += tPrep.trick.cashing;
    return SDS_APPEND_MERGED;
  }
  else
  {
    // AP+BA, AB+BA, PA+BP, PB+BP, BA+BP, BP+BA without lastFlag.
    return SDS_APPEND_SAME_SEG;
  }
}


bool Trick::ReduceBoth(
  const Trick& t1)
{
  if (t1.trick.start == QT_BOTH &&
    t1.trick.end != QT_BOTH &&
    t1.trick.end == trick.start &&
    trick.end == SDS_PARTNER[t1.trick.end])
  {
    // BA+AP = BP, BP+PA = BA.
    trick.start = QT_BOTH;
    trick.cashing += t1.trick.cashing;
    trick.ranks = Min(trick.ranks, t1.trick.ranks);
    return true;
  }
  else
    return false;
}


CmpType Trick::CashRankOrder(
  const unsigned char c,
  const unsigned char r) const
{
  if (trick.cashing > c)
    return SDS_OLD_BETTER;
  else if (trick.cashing < c)
    return SDS_NEW_BETTER;
  else if (trick.ranks > r)
    return SDS_OLD_BETTER;
  else if (trick.ranks < r)
    return SDS_NEW_BETTER;
  else
    return SDS_SAME;
}


CmpType Trick::ComparePlay(
  const Trick& t1,
  const PosType side) const
{
  reachType oldReach = posToReach[trick.end];
  reachType newReach = posToReach[t1.trick.end];

  if (trick.start == QT_BOTH)
    oldReach = reachMap[oldReach][side];

  if (t1.trick.start == QT_BOTH)
    newReach = reachMap[newReach][side];

  CmpType runningScore = reachMatrix[oldReach][newReach];
  if (runningScore == SDS_DIFFERENT)
    return SDS_DIFFERENT;

  CmpType trickScore;
  if (trick.cashing > t1.trick.cashing)
    trickScore = SDS_OLD_BETTER;
  else if (trick.cashing < t1.trick.cashing)
    trickScore = SDS_NEW_BETTER;
  else
    trickScore = SDS_SAME;

  return cmpMergeMatrix[runningScore][trickScore];
}


CmpDetailType Trick::Compare(
  const Trick& t1) const
{
  CmpType sideScore =
    reachMatrix[posToReach[trick.start]][posToReach[t1.trick.start]];
  CmpType runningScore = sideScore;
  if (runningScore == SDS_DIFFERENT)
    return SDS_HEADER_PLAY_DIFFERENT;

  CmpType playScore;
  if (sideScore == SDS_OLD_BETTER)
    playScore = Trick::ComparePlay(t1, t1.trick.start);
  else if (sideScore == SDS_NEW_BETTER)
    playScore = Trick::ComparePlay(t1, trick.start);
  else if (trick.start != QT_BOTH)
    playScore = Trick::ComparePlay(t1, trick.start);
  else
  {
    CmpType playScore1 = Trick::ComparePlay(t1, QT_ACE);
    CmpType playScore2 = Trick::ComparePlay(t1, QT_PARD);
    playScore = cmpMergeMatrix[playScore1][playScore2];
  }

  if (playScore == SDS_DIFFERENT)
    return SDS_HEADER_PLAY_DIFFERENT;

  runningScore = cmpMergeMatrix[runningScore][playScore];
  if (runningScore == SDS_DIFFERENT)
    return SDS_HEADER_PLAY_DIFFERENT;

  if (runningScore != SDS_SAME && trick.cashing != t1.trick.cashing)
    return cmpPlayToDetail[runningScore];

  CmpType rankScore;
  if (trick.ranks > t1.trick.ranks)
    rankScore = SDS_OLD_BETTER;
  else if (trick.ranks < t1.trick.ranks)
    rankScore = SDS_NEW_BETTER;
  else
    rankScore = SDS_SAME;

  if (runningScore == SDS_SAME)
    return cmpRanksToDetail[rankScore];
  else if (rankScore == SDS_SAME)
    return cmpPlayToDetail[runningScore];
  else if (runningScore == rankScore)
    return cmpPlayToDetail[rankScore];
  else
    return SDS_HEADER_RANK_DIFFERENT;

  // SDS_HEADER_RANK_DIFFERENT signifies that one move would win
  // on end range, but the other would win on rank.
}


void Trick::Print(
  ostream& out) const
{
  out <<
    setw(6) << POS_NAMES[trick.start] <<
    setw(6) << POS_NAMES[trick.end] <<
    setw(6) << static_cast<unsigned>(trick.cashing) <<
    setw(8) << SDS_RANK_NAMES[trick.ranks] << "\n";
}


void Trick::Print(
  ostream& out,
  const unsigned d,
  const unsigned a,
  const unsigned p) const
{
  out <<
    setw(4) << d <<
    setw(6) << a <<
    setw(6) << p;

  Trick::Print(out);
}


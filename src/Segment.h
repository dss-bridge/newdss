/* 
   SDS, a bridge single-suit double-dummy quick-trick solver.

   Copyright (C) 2015-16 by Soren Hein.

   See LICENSE and README.
*/

#ifndef SDS_SEGMENT_H
#define SDS_SEGMENT_H

#include <iostream>

#include "compare.h"

class Holding;


#define SEGMENT_MAXMOVES 2


class Segment
{
  private:

    #include "Trick.h"
    
    Trick list[SEGMENT_MAXMOVES];
    unsigned len;

    void Reset();

    void PrependSimple(
      const Trick& tPrep);

    bool PrependCommon(
      const Trick& mergingMove);

    bool PrependSpecial(
      const Trick& mergingMove,
      const Holding& holding);

    void PrependDeep(
      const Trick& mergingMove);

    void PrependFix(
      const bool lastFlag);

    bool Fix11_12(
      Segment& seg2,
      FixType& fix1,
      FixType& fix2);

    bool Fix11_OneB(
      Segment& seg2,
      FixType& fix1,
      FixType& fix2) const;

    bool Fix12Special(
      Segment& seg20,
      Segment& seg21,
      FixType& fix1,
      FixType& fix2) const;

    bool Fix1nSpecial(
      Segment& seg20,
      FixType& fix1,
      FixType& fix2) const;

  public:

    Segment();

    ~Segment();

    bool Set1(const Trick& trick);

    bool Set2(
      const Trick& trick1,
      const Trick& trick2);

    void PunchOut(
      const unsigned no);

    void SetStart(
      const PosType start);

    PosType GetStart() const;

    void SetEnd(
      const PosType end);

    PosType GetEnd() const;

    unsigned GetRanks() const;

    unsigned GetLength() const;

    unsigned GetTricks() const;

    void GetSummaryTrick(
      Trick& summaryTrick,
      const bool lastFlag = true) const;

    bool CanExpand() const;

    bool IsSimpleComplement(
      const Segment& seg2) const;

    bool IsAA1ATrick() const;

    unsigned xBNo(
      const PosType start) const;

    CmpTrickType Compare(
      const Segment& seg2,
      const unsigned runRankOld = SDS_VOID,
      const unsigned runRankNew = SDS_VOID) const;

    unsigned GetLowestRank() const;

    void FixLowestRanks(
      const unsigned rLower);

    bool EqualsExceptStart(
      const Segment& seg2,
      const bool lastFlag) const;

    bool EqualsExceptEnd(
      const Segment& seg2) const;

    bool operator == (
      const Segment& seg2) const;

    bool operator != (
      const Segment& seg2) const;

    void Localize(
      const Holding& holding);

    void FixEquals(
      const Holding& holding);

    bool Prepend(
      const Holding& holding,
      const bool lastSegFlag);

    bool FixRanks(
      const unsigned rPrep);

    PosType Connect(
      const Segment& sPrepend);

    bool Fix11(
      Segment& seg2,
      FixType& fix1,
      FixType& fix2);
    
    bool Fix12(
      Segment& seg20,
      Segment& seg21,
      FixType& fix1,
      FixType& fix2);
    
    bool Fix1n(
      Segment& seg20,
      FixType& fix1,
      FixType& fix2) const;
    
    void Print(
      std::ostream& out = std::cout) const;

    void Print(
      std::ostream& out,
      const unsigned d,
      const unsigned a,
      unsigned& offset) const;
};

#endif


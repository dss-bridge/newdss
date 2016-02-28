/* 
   SDS, a bridge single-suit double-dummy quick-trick solver.

   Copyright (C) 2015-16 by Soren Hein.

   See LICENSE and README.
*/

#ifndef SDS_TRICK_H
#define SDS_TRICK_H

#include <iostream>

#include "compare.h"
#include "const.h"

class Holding;
class Segment;


class Trick
{
  private:

    struct TrickType
    {
      PosType start;
      PosType end;
      unsigned char cashing;
      unsigned char ranks;
    };

    TrickType trick;

    void Reset();

    CmpType ComparePlay(
      const Trick& t1,
      const PosType side) const;

  public:

    friend class Segment;

    Trick();

    ~Trick();

    bool Set(
      const PosType start,
      const PosType end,
      const unsigned ranks,
      const unsigned cashing);

    void SetStart(
      const PosType start);

    void SetEnd(
      const PosType end);

    PosType GetStart() const;

    PosType GetEnd() const;

    unsigned GetCashing() const;

    unsigned GetRanks() const;

    bool operator == (
      const Trick& t2) const;

    bool operator != (
      const Trick& t2) const;

    bool EqualsExceptStart(
      const Trick& t2) const;

    bool EqualsExceptEnd(
      const Trick& t2) const;

    bool IsSimpleComplement(
      const Trick& t2) const;

    bool Extends(
      const Trick& tEarlier) const;

    void Localize(
      const Holding& holding);

    AppendType Prepend(
      const Trick& tPrep,
      const bool mergeSpecialFlag,
      const bool lastFlag);

    bool ReduceBoth(
      const Trick& t1);

    CmpDetailType Compare(
      const Trick& t1) const;

    CmpType CashRankOrder(
      const unsigned char c,
      const unsigned char r) const;

    void Print(
      std::ostream& out = std::cout) const;

    void Print(
      std::ostream& out,
      const unsigned d,
      const unsigned a,
      const unsigned p) const;
};

#endif


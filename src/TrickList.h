/* 
   SDS, a bridge single-suit double-dummy quick-trick solver.

   Copyright (C) 2015-16 by Soren Hein.

   See LICENSE and README.
*/

#ifndef SDS_TRICKLIST_H
#define SDS_TRICKLIST_H

#include <iostream>

#include "Segment.h"

class Holding;
class Header;
class Trick;

#define TRICKLIST_MAXSEGS 4


class TrickList
{
  private:

    Segment list[TRICKLIST_MAXSEGS];
    unsigned len;

    struct CompareStruct
    {
      unsigned lenOld;
      unsigned lenNew;
      CmpDetailType winnerFirst;
      CmpDetailType winnerRunning;
      unsigned tricksOld;
      unsigned tricksNew;
      unsigned ranksOld;
      unsigned ranksNew;
    };

    CmpDetailType CompareInit(
      const TrickList& lNew,
      CompareStruct cdata) const;

    CmpDetailType CompareRunning(
      const CompareStruct cdata) const;

    CmpDetailType CompareTail(
      const TrickList& lNew,
      CompareStruct cdata) const;

    void Split();

  public:

    TrickList();

    ~TrickList();

    void Reset();

    bool Set1(
      const Trick& trick);

    bool Set2(
      const Trick& trick1,
      const Trick& trick2);

    bool Set3(
      const Trick& trick1,
      const Trick& trick2,
      const Trick& trick3);

    bool Set4(
      const Trick& trick1,
      const Trick& trick2,
      const Trick& trick3,
      const Trick& trick4);

    bool Set21(
      const Trick& trick10,
      const Trick& trick11,
      const Trick& trick2);

    void SetStart(
      const PosType start);

    PosType GetFirstStart() const;

    PosType GetFirstEnd() const;

    unsigned GetLength() const;

    void GetHeader(
      Header& header,
      const unsigned startNo = 0) const;

    void GetFirstSummaryTrick(
      Trick& t) const;

    CmpDetailType Compare(
      const TrickList& lNew) const;

    bool IsSimpleComplement(
      const TrickList& lNew) const;

    bool EqualsExceptStart(
      const TrickList& lNew) const;

    bool operator == (
      const TrickList& t2) const;

    bool operator != (
      const TrickList& t2) const;

    void operator += (
      const Holding& holding);

    CmpDetailType CompareToTrick (
      const Trick& trick) const;

    CmpDetailType FixOrCompare(
      TrickList& lOther,
      FixType& fix1,
      FixType& fix2);
    
    bool Fix(
      TrickList& lOther,
      FixType& fix1,
      FixType& fix2);
    
    PosType ConnectFirst(
      const PosType pend);

    PosType ConnectFirst();

    void Print(
      std::ostream& out = std::cout,
      const unsigned d = 0,
      const unsigned a = 0) const;
};

#endif


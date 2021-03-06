/* 
   SDS, a bridge single-suit double-dummy quick-trick solver.

   Copyright (C) 2015-16 by Soren Hein.

   See LICENSE and README.
*/

#ifndef SDS_ALTLIST_H
#define SDS_ALTLIST_H

#include <iostream>
#include <vector>

#include "TrickList.h"
#include "compare.h"
#include "const.h"

class AltMatrix1D;
class AltMatrix2D;
class Holding;
class Header;
class Trick;


class AltList
{
  private:

    TrickList list[SDS_MAX_ALT];
    unsigned len;

    bool CanExpand(
      bool expandList[]) const;

    void ExpandInto(
      AltList& aNew,
      const bool expandList[]) const;

    bool MergeSoftSpecial12(
      const AltList& aNew) const;

    void Backtrack1D(
      AltMatrix1D& comp,
      const unsigned dimFixed,
      const unsigned dimVar,
      std::vector<FixType>& fixVector);

    void FillMatrix1D(
      AltMatrix1D& comp);

    void FillMatrix2D(
      AltList& aNew,
      AltMatrix1D& compX,
      AltMatrix1D& compY,
      const std::vector<bool>& softX,
      const std::vector<bool>& softY);

    bool AllFalse(
      const std::vector<bool>& vec,
      const unsigned vlen) const;

    void PurgeList(
      const AltMatrix1D& comp);

    void PurgeList(
      const std::vector<bool>& purgeList);

    void PurgeMulti();

    void PunchOut(
      const AltList * alist,
      const unsigned purgeNo,
      const std::vector<bool>& purgeList,
      const PosType pstart);

    void PunchOut(
      const AltList * alist,
      const PosType pstart);

    void Reduce();

    void HardReduce();

    void ReduceIfDominated(
      const unsigned n,
      const unsigned a0,
      const unsigned a1);

    CmpDetailType CompareMulti(
      const TrickList& tref,
      const CmpDetailType cmpMat[][SDS_HEADER_CMP_SIZE] = cmpDetailMatrix)
        const;

    bool CompareMultiSide(
      const PosType sideToLose,
      const AltMatrix2D& comp,
      const AltList& altToLose,
      const CmpDetailType cmpMat[][SDS_HEADER_CMP_SIZE] = cmpDetailMatrix)
        const;

    CmpDetailType CompareMultiTrickList(
      TrickList& tlist,
      const CmpDetailType cmpMat[][SDS_HEADER_CMP_SIZE] = cmpDetailHardMatrix);

    CmpDetailType FrontIsGE(
      const TrickList& tlist,
      const CmpDetailType cmpMat[][SDS_HEADER_CMP_SIZE] = cmpDetailMatrix)
        const;

    CmpDetailType FrontIsGE(
      const Trick& trick) const;

    void ConnectFirst(
      const PosType pend);

    void RegisterSize(
      const std::string& text);


  public:

    AltList();

    ~AltList();

    void Reset();

    bool Set1(
      const Trick& trick);

    bool Set2(
      const Trick& trick10,
      const Trick& trick11);

    bool Set3(
      const Trick& trick10,
      const Trick& trick11,
      const Trick& trick12);

    bool Set11(
      const Trick& trick1,
      const Trick& trick2);

    bool Set12(
      const Trick& trick1,
      const Trick& trick20,
      const Trick& trick21);

    bool Set13(
      const Trick trick[]);

    bool Set14(
      const Trick trick[]);

    bool Set31(
      const Trick trick[]);

    bool Set111(
      const Trick trick[]);

    bool Set112(
      const Trick trick[]);

    bool Set122(
      const Trick trick[]);

    bool Set123(
      const Trick trick[]);

    bool Set113(
      const Trick trick[]);

    bool Set114(
      const Trick trick[]);

    bool Set1112(
      const Trick trick[]);

    bool Set1122(
      const Trick trick[]);

    bool Set1123(
      const Trick trick[]);

    bool Set1124(
      const Trick trick[]);

    bool Set11223(
      const Trick trick[]);

    void GetHeader(
      Header& header) const;

    unsigned GetLength() const;

    unsigned GetComplexity() const;

    unsigned GetTricks() const;

    CmpDetailType Compare(
      const AltList& aNew) const;

    CmpDetailType CompareWithExpand(
      const AltList& aNew) const;

    CmpDetailType CompareSemiHard(
      const AltList& aNew) const;

    CmpDetailType CompareAlmostHard(
      const AltList& aNew) const;

    CmpDetailType CompareHard(
      const AltList& aNew) const;

    void FixRanks(
      const unsigned rLower,
      const unsigned tLower);

    bool operator == (
      const AltList& alt2) const;

    bool operator != (
      const AltList& alt2) const;

    void operator += (
      const Holding& holding);

    AltList operator + (
      const AltList& aRight) const;

    bool CanMergeSides(
      const AltList& aNew) const;

    bool MergeSoftSpecial(
      const AltList& aNew) const;

    void Concatenate(
      const AltList& aNew);

    void SetStart();

    bool MergeSides(
      const AltList& aNew);

    void ReduceSpecial();

    void Print(
      std::ostream& pout,
      const std::string text,
      const unsigned d = 0) const;
};

#endif


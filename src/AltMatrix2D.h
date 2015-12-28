/* 
   SDS, a bridge single-suit double-dummy quick-trick solver.

   Copyright (C) 2015-16 by Soren Hein.

   See LICENSE and README.
*/

#ifndef SDS_ALTMATRIX2D_H
#define SDS_ALTMATRIX2D_H

#include <string>

#include "compare.h"
#include "const.h"


class AltMatrix2D
{
  private:

    CmpDetailType matrix[SDS_MAX_ALT][SDS_MAX_ALT];

    unsigned numX;
    unsigned numY;

    bool activeX[SDS_MAX_ALT];
    bool activeY[SDS_MAX_ALT];

    // Actually does not have to be stored.
    bool hasX[SDS_MAX_ALT][SDS_HEADER_CMP_SIZE];
    bool hasY[SDS_MAX_ALT][SDS_HEADER_CMP_SIZE];
    bool hasXsum[SDS_HEADER_CMP_SIZE];
    bool hasYsum[SDS_HEADER_CMP_SIZE];

    CmpDetailType cval;

    void ResetVectors(
      bool has[SDS_MAX_ALT][SDS_HEADER_CMP_SIZE],
      const bool active[],
      const unsigned len,
      bool hasSum[]);

    void VerifyVector(
      const bool has[SDS_MAX_ALT][SDS_HEADER_CMP_SIZE],
      const bool active[],
      const unsigned len,
      bool hasSum[],
      const bool verifyFlag);

    CmpDetailType ComparePartial(
      const CmpDetailType diff,
      const CmpDetailType winX,
      const CmpDetailType winY);

    CmpDetailType ComparePartialDeclarer(
      const CmpDetailType diff,
      const CmpDetailType winX,
      const CmpDetailType winY);

    void PrintVector(
      std::ostream& out,
      const std::string text,
      const bool cvec[SDS_MAX_ALT][SDS_HEADER_CMP_SIZE],
      const unsigned len) const;

  public:

    AltMatrix2D();

    ~AltMatrix2D();

    void Reset();

    void SetDimensions(
      const unsigned x,
      const unsigned y);
    
    bool IsPurgedX(
      const unsigned x) const;
    
    bool IsPurgedY(
      const unsigned y) const;

    void SetValue(
      const unsigned x,
      const unsigned y,
      const CmpDetailType c);

    void PurgeX(
      const unsigned x);

    void PurgeY(
      const unsigned y);

    void Verify(
      const bool verifyFlag = true);

    CmpDetailType Compare();

    CmpDetailType CompareDeclarer();

    CmpDetailType CompareHard();

    bool CandList(
      const PosType side,
      bool use[],
      CmpDetailType& c) const;

    void Print(
      std::ostream& out,
      const std::string text) const;
};

#endif


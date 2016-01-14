/* 
   SDS, a bridge single-suit double-dummy quick-trick solver.

   Copyright (C) 2015-16 by Soren Hein.

   See LICENSE and README.
*/

#ifndef SDS_HEADER_H
#define SDS_HEADER_H

#include "Trick.h"
#include "const.h"


class Header
{
  private:

    PosType start;
    PosType end;
    unsigned char maxTricks;
    unsigned char maxRanks;
    unsigned char minRanks;
    unsigned char cashTricks[SDS_HANDS];
    unsigned char cashRanks[SDS_HANDS];
    unsigned keyNew;

    unsigned dCum;
    unsigned aCum;
    unsigned sCum;
    unsigned tCum;
    unsigned aMax;

    unsigned startNew;
    unsigned endNew;

    unsigned cashAceMin;
    unsigned cashAceMax;
    unsigned cashPardMin;
    unsigned cashPardMax;
    unsigned rankAceMin;
    unsigned rankAceMax;
    unsigned rankPardMin;
    unsigned rankPardMax;

  public:

    Header();

    ~Header();

    void Set(
      const Trick& trick,
      unsigned tno);

    void Increase(
      const Trick& tLater,
      unsigned tno);

    void MergeMax(
      const Header& newHeader);

    void MergeMin(
      const Header& newHeader);

    void GetAD0(
      unsigned& d,
      unsigned& a) const;
    void GetAD(
      unsigned& d,
      unsigned& asum) const;
    unsigned GetTrickKey() const;
    unsigned GetRankKey() const;
    unsigned GetMaxRank() const;
    unsigned GetKeyNew() const;
    unsigned GetSymmTricks(
      const unsigned r) const;

    void PrintKey(
      std::ostream& out,
      const unsigned key) const;

    void Print(
      std::ostream& out = std::cout,
      const bool skipMax = false) const;
};

#endif


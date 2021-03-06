/* 
   SDS, a bridge single-suit double-dummy quick-trick solver.

   Copyright (C) 2015-16 by Soren Hein.

   See LICENSE and README.
*/

#ifndef SDS_SIDEMOVELIST_H
#define SDS_SIDEMOVELIST_H

#include <string>
#include <vector>

#include "Header.h"
#include "Holding.h"
#include "DefList.h"
#include "Hash.h"
#include "summary.h"
#include "const.h"


#define SIDEMOVE_CHUNK_SIZE 1000


class SideMoveList
{
  private:

    struct ListEntry
    {
      unsigned no;
      ListEntry * next;
    };

    struct MoveData
    {
      DefList def;
      Header header;
      unsigned suitLengthExample;
      unsigned counterExample;
    };

    std::vector<MoveData> list;
    std::vector<unsigned> moveCount;
    unsigned listLength;

    ListEntry * index[ML_MAXKEY];

    unsigned indexCount[ML_MAXKEY];

    Hash hash;

    unsigned numEntries;

    unsigned histD[SDS_MAX_DEF];
    unsigned histAsum[SDS_MAX_DEF * SDS_MAX_ALT];

    void Extend();

    void PrintMoveStats(
      std::ostream& out,
      SideSummaryType& ssumm) const;

    void PrintHashStats(
      std::ostream& out,
      SideSummaryType& ssumm) const;
    
    void PrintList(
      std::ostream& out,
      const unsigned hist[],
      const unsigned l,
      const char text[]) const;

  public:

    SideMoveList();

    ~SideMoveList();

    unsigned AddMove(
      DefList& def, 
      const Holding& holding,
      bool& newFlag,
      unsigned& hashKey);

    unsigned GetMaxRank(
      const unsigned no);

    unsigned GetSymmTricks(
      const unsigned no) const;

    void GetAD(
      const unsigned no,
      unsigned& dno,
      unsigned& ano);

    DefList& GetMove(
      const unsigned no);

    void Print(
      const unsigned no) const;

    void PrintMove(
      std::ostream& out,
      const unsigned no);

    void PrintMovesByOrder(
      std::ostream& out = std::cout);

    void PrintMoveListByKeys(
      std::ostream& out = std::cout);

    void PrintMoveListByCount(
      std::ostream& out = std::cout);

    void PrintLists(
      std::ostream& out,
      SideSummaryType& ssumm) const;

    void PrintStats(
      std::ostream& out) const;
};

#endif

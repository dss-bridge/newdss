/* 
   SDS, a bridge single-suit double-dummy quick-trick solver.

   Copyright (C) 2015-16 by Soren Hein.

   See LICENSE and README.
*/

#ifndef SDS_MOVELIST_H
#define SDS_MOVELIST_H

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "SideMoveList.h"
#include "Holding.h"
#include "DefList.h"
#include "sort.h"


class MoveList
{
  private:

    SideMoveList sideComb;
    SideMoveList sideList1;
    SideMoveList sideList2;

    std::vector<AggrMoveType> noToAggr;
    unsigned len;
    unsigned count;

    std::map<std::string, unsigned> aggrMap;

    unsigned Update(
      const AggrMoveType& mno,
      const bool newFlag,
      const unsigned ret = 0);

    unsigned AggrToNo(
      const AggrMoveType& mno);

    void SetAggr(
      const AggrMoveType& mno);

    void PrintMovesByList(
      std::ostream& out,
      const std::vector<AggrMoveType>& list);

    void Extend();

  public:

    MoveList();

    ~MoveList();

    unsigned AddMove(
      DefList& def, 
      const Holding& holding,
      bool& newFlag);

    unsigned AddMove(
      DefList& def1, 
      DefList& def2, 
      const Holding& holding,
      bool& newFlag);

    unsigned GetMaxRank(
      const unsigned no);

    unsigned GetSymmTricks(
      const unsigned no);

    DefList GetCombinedMove(
      const unsigned no);

    void Print(
      const unsigned no) const;

    void PrintMovesByOrder(
      std::ostream& out = std::cout);

    void PrintMovesByKeys();

    void PrintMovesByCount(
      std::ostream& out = std::cout);

    void PrintStats() const;
    
    void PrintLists() const;
};

#endif

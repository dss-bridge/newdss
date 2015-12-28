/* 
   SDS, a bridge single-suit double-dummy quick-trick solver.

   Copyright (C) 2015-16 by Soren Hein.

   See LICENSE and README.
*/

#ifndef SDS_SORT_H
#define SDS_SORT_H

struct SortType
{
  unsigned no;
  unsigned count;
};

struct AggrMoveType
{
  unsigned noComb;
  unsigned no1;
  unsigned no2;
  unsigned keyComb;
  unsigned key1;
  unsigned key2;
  unsigned number;
  unsigned count;
};

bool SortIsGreater(
  const SortType& a,
  const SortType& b);

bool SortKeyIsGreater(
  const AggrMoveType& a,
  const AggrMoveType& b);

bool SortCountIsGreater(
  const AggrMoveType& a,
  const AggrMoveType& b);

#endif


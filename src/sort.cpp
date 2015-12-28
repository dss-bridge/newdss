/* 
   SDS, a bridge single-suit double-dummy quick-trick solver.

   Copyright (C) 2015-16 by Soren Hein.

   See LICENSE and README.
*/

#include "sort.h"


bool SortIsGreater(
  const SortType& a,
  const SortType& b)
{
  return (b.count < a.count || (b.count == a.count && b.no > a.no));
}


bool SortKeyIsGreater(
  const AggrMoveType& a,
  const AggrMoveType& b)
{
  if (a.noComb && ! b.noComb)
    return true;
  else if (! a.noComb && b.noComb)
    return false;
  else if (a.noComb && b.noComb)
    return (a.keyComb < b.keyComb ||
      ((a.keyComb == b.keyComb && a.number < b.number)));
  else if (a.key1 < b.key1)
    return true;
  else if (a.key1 > b.key1)
    return false;
  else if (a.key2 < b.key2)
    return true;
  else if (a.key2 > b.key2)
    return false;
  else 
    return (a.number < b.number);
}


bool SortCountIsGreater(
  const AggrMoveType& a,
  const AggrMoveType& b)
{
  if (a.count > b.count)
    return true;
  else if (a.count < b.count)
    return false;
  else
    return (a.number < b.number);
}


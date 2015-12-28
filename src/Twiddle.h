/* 
   SDS, a bridge single-suit double-dummy quick-trick solver.

   Copyright (C) 2015-16 by Soren Hein.

   See LICENSE and README.
*/

#ifndef SDS_TWIDDLE_H
#define SDS_TWIDDLE_H

#include "hist.h"

class Holding;

void SetAllPermutations(
  const unsigned& suitLength,
  const unsigned& counter,
  unsigned moveNo,
  Holding& holding,
  const unsigned rank,
  const HistIndexType histNo,
  bool& newFlag);

#endif

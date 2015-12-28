/* 
   SDS, a bridge single-suit double-dummy quick-trick solver.

   Copyright (C) 2015-16 by Soren Hein.

   See LICENSE and README.
*/

#ifndef SDS_CHECK_H
#define SDS_CHECK_H

#include "hist.h"

class Holding;


void CompareRecurse(
  unsigned moveNo,
  Holding& Holding,
  const HistIndexType histNo);

#endif

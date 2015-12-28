/* 
   SDS, a bridge single-suit double-dummy quick-trick solver.

   Copyright (C) 2015-16 by Soren Hein.

   See LICENSE and README.
*/

#ifndef SDS_DIST_H
#define SDS_DIST_H

#include <iostream>

#include "hist.h"


void InitDist();

void SetAllLowCards(
  const unsigned& suitLength,
  const unsigned& counter,
  unsigned moveNo,
  const unsigned rank,
  const HistIndexType histNo,
  bool& newFlag);

void PrintDist(
  std::ostream& out = std::cout);

void PrintHistLowCards(
  std::ostream& out = std::cout);

#endif

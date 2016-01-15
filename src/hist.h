/* 
   SDS, a bridge single-suit double-dummy quick-trick solver.

   Copyright (C) 2015-16 by Soren Hein.

   See LICENSE and README.
*/

#ifndef SDS_HIST_H
#define SDS_HIST_H

#include <iostream>


enum HistIndexType
{
  HIST_ORIG_COUNT = 0,
  HIST_ACE_SHORT = 1,
  HIST_BLOCKED = 2,
  HIST_PARD_VOID = 3,
  HIST_CRASH = 4,
  HIST_SINGLE = 5,
  HIST_REST = 6,
  HIST_CASHES = 7,
  HIST_COMPLEX = 8,
  HIST_SIZE = 9
};


void InitHist();

void UpdateHist(
  const HistIndexType histNo,
  const unsigned declLen,
  const unsigned rank,
  const bool newFlag);

const char * GetHistName(
  const HistIndexType histNo);

void PrintPercentHist(
  std::ostream& out,
  const unsigned cStart,
  const unsigned dlen);

void PrintHist(
  std::ostream& out);

void PrintHoldCounter(
  std::ostream& out);

#endif


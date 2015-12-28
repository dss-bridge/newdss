/* 
   SDS, a bridge single-suit double-dummy quick-trick solver.

   Copyright (C) 2015-16 by Soren Hein.

   See LICENSE and README.
*/

#ifndef SDS_SUMMARY_H
#define SDS_SUMMARY_H

#include <iostream>
#include <string>

struct SideSummaryType
{
  std::string numMoves;
  std::string sumMoves;
  std::string gini;
  std::string search;
  std::string dsum;
  std::string asum;
};

struct SummaryType
{
  std::string runtime;

  SideSummaryType all;
  SideSummaryType comb;
  SideSummaryType A;
  SideSummaryType P;

  unsigned countShort;
  unsigned countEqual;
  unsigned countDiff;
};

void InitSummary();

void PrintSummary(
  std::ostream& out);

#endif


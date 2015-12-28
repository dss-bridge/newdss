/* 
   SDS, a bridge single-suit double-dummy quick-trick solver.

   Copyright (C) 2015-16 by Soren Hein.

   See LICENSE and README.
*/

using namespace std;

#include "summary.h"

extern SummaryType summary;


void InitSideSummary(
  SideSummaryType& side);

void PrintSideSummary(
  ostream& out,
  const string text,
  const SideSummaryType& side);


void InitSideSummary(
  SideSummaryType& side)
{
  side.numMoves = "";
  side.sumMoves = "";
  side.gini = "";
  side.search = "";
  side.dsum = "";
  side.asum = "";
}


void InitSummary()
{
  summary.runtime = "";

  InitSideSummary(summary.all);
  InitSideSummary(summary.comb);
  InitSideSummary(summary.A);
  InitSideSummary(summary.P);

  summary.countShort = 0;
  summary.countEqual = 0;
  summary.countDiff = 0;
}


void PrintSummary(
  ostream& out)
{
  out << summary.runtime << "\n\n";

  PrintSideSummary(out, "all", summary.all);
  PrintSideSummary(out, "comb", summary.comb);
  PrintSideSummary(out, "A", summary.A);
  PrintSideSummary(out, "P", summary.P);
}


void PrintSideSummary(
  ostream& out,
  const string text,
  const SideSummaryType& side)
{
  out << text << "\n";
  out << side.numMoves << "\n";
  out << side.sumMoves << "\n";
  out << side.gini << "\n";
  out << side.search << "\n";
  out << side.dsum << "\n";
  out << side.asum << "\n";
  out << "\n";
}


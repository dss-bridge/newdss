/* 
   SDS, a bridge single-suit double-dummy quick-trick solver.

   Copyright (C) 2015-16 by Soren Hein.

   See LICENSE and README.
*/

using namespace std;

#include <iomanip>
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

  setfill(' ');
  if (summary.countShort)
    cout << setw(18) << left << "Moves short" << summary.countShort << "\n";
  if (summary.countEqual)
    cout << setw(18) << left << "Moves equal" << summary.countEqual << "\n";
  if (summary.countDiff)
    cout << setw(18) << left << "Moves diff" << summary.countDiff << "\n";
}


void PrintSideSummary(
  ostream& out,
  const string text,
  const SideSummaryType& side)
{
  if (side.numMoves == "" &&
      side.sumMoves == "")
    return;

  out << text << "\n";
  out << "----\n";

  if (side.numMoves != "")
    out << side.numMoves << "\n";
  if (side.sumMoves != "")
    out << side.sumMoves << "\n";
  if (side.gini != "")
    out << side.gini << "\n";
  if (side.search != "")
    out << side.search << "\n";
  if (side.dsum != "")
    out << side.dsum << "\n";
  if (side.asum != "")
    out << side.asum << "\n";
  out << "\n";
}


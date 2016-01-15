/* 
   SDS, a bridge single-suit double-dummy quick-trick solver.

   Copyright (C) 2015-16 by Soren Hein.

   See LICENSE and README.
*/

#include <iomanip>
#include <vector>
#include <assert.h>

#include "hist.h"
#include "const.h"

using namespace std;


#define NUM_COUNTERS 0x2000

extern vector<unsigned> holdCtr;


const char HIST_NAMES[HIST_SIZE][10] =
{
  "Lengths",
  "Ace tops",
  "Blocked",
  "Pd void",
  "Crash",
  "One",
  "Rest",
  "Cashes",
  "General"
};


unsigned histCount[HIST_SIZE][14];
unsigned histRank[HIST_SIZE][14];
unsigned histMoveCount[HIST_SIZE][14];
unsigned histMoveRank[HIST_SIZE][14];


void PrintSingleHist(
  std::ostream& out,
  const unsigned hist[][14],
  const unsigned type);


void InitHist()
{
  holdCtr.resize(NUM_COUNTERS);
  for (unsigned i = 0; i < NUM_COUNTERS; i++)
    holdCtr[i] = 0;

  for (unsigned hno = 0; hno < HIST_SIZE; hno++)
  {
    for (unsigned sl = 0; sl < 14; sl++)
    {
      histCount[hno][sl] = 0;
      histRank[hno][sl] = 0;

      histMoveCount[hno][sl] = 0;
      histMoveRank[hno][sl] = 0;
    }
  }
}


void UpdateHist(
  const HistIndexType histNo,
  const unsigned declLen,
  const unsigned rank,
  const bool newFlag)
{
  if (rank >= 14)
  {
    cout << "rank " << rank << endl;
    assert(rank < 14);
  }
  histCount[histNo][declLen]++;
  histRank[histNo][rank]++;

  if (newFlag)
  {
    histMoveCount[histNo][declLen]++;
    histMoveRank[histNo][rank]++;
  }
}


const char * GetHistName(
  const HistIndexType histNo)
{
  return HIST_NAMES[histNo];
}


void PrintPercentHist(
  ostream& out,
  const unsigned cStart,
  const unsigned dlen)
{
  double histRest = histCount[HIST_ORIG_COUNT][dlen];
  for (unsigned hno = 1; hno < HIST_COMPLEX; hno++)
    histRest -= histCount[hno][dlen];
  if (histRest < 1)
    histRest = 1;

  out << "Count " << setw(8) << cStart << " (" <<
    setw(5) << fixed << setprecision(2) <<
    100. * histCount[HIST_COMPLEX][dlen] / histRest << "%)" << endl;
}


void PrintSingleHist(
  ostream& out,
  const unsigned hist[][14],
  const unsigned type)
{
  if (type == 0)
    out << "Suit length count\n\n";
  else
    out << "Max ranks\n\n";

  out << setw(2) << "No";
  for (unsigned hno = 0; hno < HIST_SIZE; hno++)
    out << setw(10) << HIST_NAMES[hno];
  out << "\n";

  unsigned sum[HIST_SIZE] = {0};

  for (unsigned sl = 0; sl < 12; sl++)
  {
    if (type == 0)
      out << setw(2) << sl;
    else
      out << setw(2) << SDS_RANK_NAMES[sl];

    for (unsigned hno = 0; hno < HIST_SIZE; hno++)
    {
      out << setw(10) << hist[hno][sl];
      sum[hno] += hist[hno][sl];
    }
    out << "\n";
  }

  out << "\n  ";
  for (unsigned hno = 0; hno < HIST_SIZE; hno++)
    out << setw(10) << sum[hno];
  out << "\n\n";
}


void PrintHist(
  ostream& out)
{
  PrintSingleHist(out, histCount, 0);
  PrintSingleHist(out, histRank, 1);

  PrintSingleHist(out, histMoveCount, 0);
  PrintSingleHist(out, histMoveRank, 1);
}


void PrintHoldCounter(
  ostream& out)
{
  out << "\nHolding hits\n";
  for (unsigned i = 0; i < NUM_COUNTERS; i++)
  {
    if (holdCtr[i] == 0)
      continue;
    out << setw(4) << hex << i << dec << setw(10) << holdCtr[i] << "\n";
  }
}


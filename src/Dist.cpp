/* 
   SDS, a bridge single-suit double-dummy quick-trick solver.

   Copyright (C) 2015-16 by Soren Hein.

   See LICENSE and README.
*/

#include <iostream>
#include <iomanip>
#include <assert.h>

#include "Dist.h"
#include "Holding.h"
#include "single.h"
#include "const.h"

using namespace std;

extern SingleType * singles[14];


const unsigned SDS_DIST_LENGTH0 = 0x1;
const unsigned SDS_DIST_LENGTH1 = 0x4;
const unsigned SDS_DIST_LENGTH2 = 0x10;
const unsigned SDS_DIST_LENGTH3 = 0x40;
const unsigned SDS_DIST_LENGTH4 = 0x100;
const unsigned SDS_DIST_LENGTH5 = 0x400;
const unsigned SDS_DIST_LENGTH6 = 0x1000;
const unsigned SDS_DIST_LENGTH7 = 0x4000;
const unsigned SDS_DIST_LENGTH8 = 0x10000;
const unsigned SDS_DIST_LENGTH9 = 0x40000;
const unsigned SDS_DIST_LENGTH10 = 0x100000;

const unsigned SDS_NUM_DIST_LENGTH = 11;

const unsigned SDS_DIST_LENGTH[SDS_NUM_DIST_LENGTH] =
{
  SDS_DIST_LENGTH0,
  SDS_DIST_LENGTH1,
  SDS_DIST_LENGTH2,
  SDS_DIST_LENGTH3,
  SDS_DIST_LENGTH4,
  SDS_DIST_LENGTH5,
  SDS_DIST_LENGTH6,
  SDS_DIST_LENGTH7,
  SDS_DIST_LENGTH8,
  SDS_DIST_LENGTH9,
  SDS_DIST_LENGTH10
};

struct chainType
{
  int next;
  int dist;
};

chainType chain0[SDS_DIST_LENGTH0];
chainType chain1[SDS_DIST_LENGTH1];
chainType chain2[SDS_DIST_LENGTH2];
chainType chain3[SDS_DIST_LENGTH3];
chainType chain4[SDS_DIST_LENGTH4];
chainType chain5[SDS_DIST_LENGTH5];
chainType chain6[SDS_DIST_LENGTH6];
chainType chain7[SDS_DIST_LENGTH7];
chainType chain8[SDS_DIST_LENGTH8];
chainType chain9[SDS_DIST_LENGTH9];
chainType chain10[SDS_DIST_LENGTH10];

chainType * chain[] =
{
  chain0, 
  chain1, 
  chain2, 
  chain3, 
  chain4,
  chain5, 
  chain6, 
  chain7, 
  chain8, 
  chain9, 
  chain10
};

const unsigned SDS_NUM_DIST = 0x10000;

struct distType
{
  int first;
  int last;
};


distType distToChain[SDS_NUM_DIST_LENGTH][SDS_NUM_DIST];


void InitDist()
{
  for (unsigned d = 0; d < SDS_NUM_DIST_LENGTH; d++)
  {
    for (unsigned i = 0; i < SDS_NUM_DIST; i++)
    {
      distToChain[d][i].first = -1;
      distToChain[d][i].last = -1;
    }
  }

  for (unsigned d = 0; d < SDS_NUM_DIST_LENGTH; d++)
  {
    for (unsigned i = 0; i < SDS_DIST_LENGTH[d]; i++)
    {
      chain[d][i].next = static_cast<int>(i);
      chain[d][i].dist = -1;
    }
  }

  chain[0][0].dist = 0x0000;
  distToChain[0][0].first = 0;
  distToChain[0][0].last = 0;

  for (unsigned d = 1; d < SDS_NUM_DIST_LENGTH; d++)
  {
    int step = 2*(static_cast<int>(d)-1);
    for (unsigned i = 0; i < SDS_DIST_LENGTH[d-1]; i++)
    {
      for (int n = 0; n < 4; n++)
      {
        int c = (n << step) | static_cast<int>(i);
        int dNew = chain[d-1][i].dist + (1 << (4*n));
        int l = distToChain[d][dNew].last;
        if (l == -1)
          distToChain[d][dNew].first = c;
        else
        {
          chain[d][l].next = c;
          chain[d][c].next = distToChain[d][dNew].first;
        }

        chain[d][c].dist = dNew;
        distToChain[d][dNew].last = c;
      }
    }
  }
}

#define MAX_HIST 10000
unsigned histCalls = 0;
unsigned histXes[11] = {0};
unsigned histCycles[MAX_HIST] = {0};


void SetAllLowCards(
  const unsigned& suitLength,
  const unsigned& counter,
  unsigned moveNo,
  const unsigned rank,
  const HistIndexType histNo,
  bool &newFlag)
{
  assert(suitLength + rank > SDS_VOID);
  unsigned numXes = suitLength - (SDS_VOID - rank);
  if (numXes > 10)
    numXes = 10;

  histCalls++;
  histXes[numXes]++;

  if (numXes == 1)
  {
    singles[suitLength][counter].moveNo = moveNo;
    UpdateHist(histNo, singles[suitLength][counter].declLen, rank, newFlag);
    histCycles[1]++;

    return;
  }

  int mask = (1 << (2*numXes)) - 1;
  int first = static_cast<int>(counter) & mask;
  int tops = static_cast<int>(counter) ^ first;

  int r = first;
  unsigned num = 0;
  bool wasNew = false;
  do
  {
    int c = tops | r;

    assert(singles[suitLength][c].moveNo == 0);
    singles[suitLength][c].moveNo = moveNo;

    if (! wasNew)
    {
      UpdateHist(histNo, singles[suitLength][c].declLen, rank, newFlag);
      wasNew = true;
    }
    num++;
    r = chain[numXes][r].next;
  }
  while (r != first);

  if (num >= MAX_HIST)
    num = MAX_HIST-1;
  histCycles[num]++;
}


void PrintDist(
  ostream& out)
{
  for (unsigned d = 1; d < 5; d++)
  {
    out << "Suit length " << d << "\n";
    out << "--------------\n";
    unsigned dCount = 0;

    for (unsigned dNew = 0; dNew < SDS_NUM_DIST; dNew++)
    {
      int f = distToChain[d][dNew].first;
      if (f == -1)
        continue;

      dCount++;
      out << hex << dNew << dec << ": " <<
        (dNew & 0x3) << "-" <<
        ((dNew >> 4) & 0x3) << "-" <<
        ((dNew >> 8) & 0x3) << "-" <<
        ((dNew >> 12) & 0x3) << "\n";

      int r = f;
      do
      {
        Holding holding;
        holding.Set(d+1, static_cast<unsigned>(r));
        holding.Print(cout);
        out << "\n";

        r = chain[d][r].next;
      }
      while (r != f);
    }
    
    out << "\n" << dCount << " distributions\n";
  }
}


void PrintHistLowCards(
  ostream& out)
{
  out << "Number of calls: " << histCalls << "\n";

  out << "\nNumber of Xes\n";
  for (unsigned i = 0; i < 11; i++)
    out << setw(2) << i << setw(10) << histXes[i] << "\n";
  out << "\n";

  unsigned sum = 0;
  out << "\nNumber of cycles\n";
  for (unsigned i = 0; i < MAX_HIST; i++)
  {
    if (histCycles[i] == 0)
      continue;

    out << setw(4) << i << setw(10) << histCycles[i] << "\n";
    sum = i * histCycles[i];
  }

  if (sum == 0)
    return;

  out << "Average cycle length " <<
    setw(6) << fixed << setprecision(2) << 
      sum / static_cast<double>(histCalls) << "\n";
}

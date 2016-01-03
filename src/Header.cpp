/* 
   SDS, a bridge single-suit double-dummy quick-trick solver.

   Copyright (C) 2015-16 by Soren Hein.

   See LICENSE and README.
*/

#include <iostream>
#include <iomanip>
#include <assert.h>

#include "Header.h"
#include "Trick.h"

using namespace std;


Header::Header()
{
  dCum = 0;
  aCum = 0;
  sCum = 0;
  tCum = 0;
}


Header::~Header()
{
}


void Header::Set(
  const Trick& trick,
  unsigned tno)
{
  dCum = 1;
  aCum = 1;
  sCum = 1;
  tCum = tno;

  start = trick.GetStart();
  if (start == QT_ACE)
    startNew = 1;
  else if (start == QT_PARD)
    startNew = 2;
  else if (start == QT_BOTH)
    startNew = 4;
  else
    assert(false);

  end = trick.GetEnd();
  if (end == QT_ACE)
    endNew = 1;
  else if (end == QT_PARD)
    endNew = 2;
  else if (end == QT_BOTH)
    endNew = 4;
  else
    assert(false);

  maxTricks = trick.GetCashing();

  maxRanks = trick.GetRanks();
  minRanks = maxRanks;

  for (unsigned h = 0; h < SDS_HANDS; h++)
  {
    cashTricks[h] = 0;
    cashRanks[h] = 0;
  }

  if (start == QT_ACE || start == QT_BOTH)
  {
    cashTricks[QT_ACE] = maxTricks;
    cashRanks[QT_ACE] = maxRanks;

    cashAceMin = maxTricks;
    cashAceMax = maxTricks;
    rankAceMin = maxRanks;
    rankAceMax = maxRanks;
  }
  else
  {
    cashAceMin = 0;
    cashAceMax = 0;
    rankAceMin = SDS_VOID;
    rankAceMax = 0;
  }

  if (start == QT_PARD || start == QT_BOTH)
  {
    cashTricks[QT_PARD] = maxTricks;
    cashRanks[QT_PARD] = maxRanks;

    cashPardMin = maxTricks;
    cashPardMax = maxTricks;
    rankPardMin = maxRanks;
    rankPardMax = maxRanks;
  }
  else
  {
    cashPardMin = 0;
    cashPardMax = 0;
    rankPardMin = SDS_VOID;
    rankPardMax = 0;
  }
}


void Header::Increase(
  const Trick& tLater,
  const unsigned tno)
{
  sCum++;
  tCum += tno;

  maxTricks += tLater.GetCashing();
  unsigned mr = tLater.GetRanks();
  if (mr < maxRanks)
    maxRanks = mr;
}


void Header::MergeMax(const Header& newHeader)
{
  aCum++;
  sCum += newHeader.sCum;
  tCum += newHeader.tCum;

  if (newHeader.start == QT_ACE)
    startNew |= 1;
  else if (newHeader.start == QT_PARD)
    startNew |= 2;
  else if (newHeader.start == QT_BOTH)
    startNew |= 4;
  else
    assert(false);

  if (newHeader.end == QT_ACE)
    endNew |= 1;
  else if (newHeader.end == QT_PARD)
    endNew |= 2;
  else if (newHeader.end == QT_BOTH)
    endNew |= 4;
  else
    assert(false);

  unsigned t = newHeader.maxTricks;
  cashAceMin = Min(cashAceMin, t);
  cashAceMax = Max(cashAceMax, t);
  cashPardMin = Min(cashPardMin, t);
  cashPardMax = Max(cashPardMax, t);

  unsigned r = newHeader.maxRanks;
  rankAceMin = Min(rankAceMin, r);
  rankAceMax = Max(rankAceMax, r);
  rankPardMin = Min(rankPardMin, r);
  rankPardMax = Max(rankPardMax, r);

  if (newHeader.maxTricks > maxTricks)
  {
    maxTricks = newHeader.maxTricks;

    // Irrelevant, really.
    start = newHeader.start;
    end = newHeader.end;
  }

  if (newHeader.maxRanks < maxRanks)
    maxRanks = newHeader.maxRanks;

  if (newHeader.minRanks < minRanks && newHeader.minRanks > 0)
    minRanks = newHeader.minRanks;
    
  for (unsigned h = 0; h < SDS_HANDS; h++)
  {
    if (newHeader.cashTricks[h] > cashTricks[h])
    {
      cashTricks[h] = newHeader.cashTricks[h];
      cashRanks[h] = newHeader.cashRanks[h];
    }
    else if (newHeader.cashTricks[h] == cashTricks[h] &&
      newHeader.cashRanks[h] > cashRanks[h])
    {
      cashRanks[h] = newHeader.cashRanks[h];
    }
  }
}


void Header::MergeMin(const Header& newHeader)
{
  dCum++;
  aCum += newHeader.aCum;
  sCum += newHeader.sCum;
  tCum += newHeader.tCum;

  if (newHeader.start == QT_ACE)
    startNew |= 1;
  else if (newHeader.start == QT_PARD)
    startNew |= 2;
  else if (newHeader.start == QT_BOTH)
    startNew |= 4;
  else
    assert(false);

  if (newHeader.end == QT_ACE)
    endNew |= 1;
  else if (newHeader.end == QT_PARD)
    endNew |= 2;
  else if (newHeader.end == QT_BOTH)
    endNew |= 4;
  else
    assert(false);

  unsigned t = newHeader.maxTricks;
  cashAceMin = Min(cashAceMin, t);
  cashAceMax = Max(cashAceMax, t);
  cashPardMin = Min(cashPardMin, t);
  cashPardMax = Max(cashPardMax, t);

  unsigned r = newHeader.maxRanks;
  rankAceMin = Min(rankAceMin, r);
  rankAceMax = Max(rankAceMax, r);
  rankPardMin = Min(rankPardMin, r);
  rankPardMax = Max(rankPardMax, r);

  assert(aCum >= dCum);
  assert(sCum >= aCum);
  assert(tCum >= sCum);

  if (newHeader.maxTricks < maxTricks)
  {
    maxTricks = newHeader.maxTricks;

    // Irrelevant, really.
    start = newHeader.start;
    end = newHeader.end;
  }

  if (newHeader.maxRanks < maxRanks)
    maxRanks = newHeader.maxRanks;

  if (newHeader.minRanks < minRanks && newHeader.minRanks > 0)
    minRanks = newHeader.minRanks;
    
  for (unsigned h = 0; h < SDS_HANDS; h++)
  {
    if (newHeader.cashTricks[h] < cashTricks[h])
    {
      cashTricks[h] = newHeader.cashTricks[h];
      cashRanks[h] = newHeader.cashRanks[h];
    }
    else if (newHeader.cashTricks[h] == cashTricks[h] &&
      newHeader.cashRanks[h] < cashRanks[h])
    {
      cashRanks[h] = newHeader.cashRanks[h];
    }
  }
}


void Header::GetAD(
  unsigned& d,
  unsigned& asum) const
{
  d = dCum;
  asum = aCum;
}

unsigned Header::GetSymmTricks(
  const unsigned r) const
{
  if (cashTricks[QT_ACE] != cashTricks[QT_PARD] &&
      cashTricks[QT_ACE] != 0 &&
      cashTricks[QT_PARD] != 0)
    return 0;

  if (cashRanks[QT_ACE] < r || cashRanks[QT_PARD] < r)
    return 0;
  else if (cashTricks[QT_ACE] == 0)
    return (maxTricks == cashTricks[QT_PARD] ? maxTricks : 0);
  else if (cashTricks[QT_PARD] == 0)
    return (maxTricks == cashTricks[QT_ACE] ? maxTricks : 0);
  else if (cashRanks[QT_ACE] != cashRanks[QT_PARD])
    return 0;
  else
    return (maxTricks == cashTricks[QT_ACE] ? maxTricks: 0);
}


unsigned Header::GetKeyNew() const
{
  return (dCum-1) + (aCum << 4);
}


unsigned Header::GetTrickKey() const
{
  return static_cast<unsigned>(
    (maxTricks << 8) | (cashTricks[QT_ACE] << 4) | cashTricks[QT_PARD]);
}


unsigned Header::GetRankKey() const
{
  return static_cast<unsigned>((minRanks << 12) | (maxRanks << 8) | 
         (cashRanks[QT_ACE] << 4) | cashRanks[QT_PARD]);
}


unsigned Header::GetMaxRank() const
{
  return static_cast<unsigned>(maxRanks);
}


void Header::PrintKey(
  ostream& out,
  const unsigned key) const
{
  out << "\n\nKey " <<
    setw(6) << key <<
    " (" <<
    "maxTricks " << static_cast<unsigned>(maxTricks) <<
    ", maxRank '" <<
    SDS_RANK_NAMES[maxRanks] <<
    "')\n";
}


void Header::Print(
  ostream& out,
  const bool skipMax) const
{
  if (! skipMax)
  {
    Trick t;
    t.Set(start, end, maxRanks, maxTricks);
    t.Print(out);
  }

  out <<
    setw(10) << "cashTricks" <<
    setw(3) << static_cast<unsigned>(cashTricks[0]) <<
    setw(3) << static_cast<unsigned>(cashTricks[1]) <<
    setw(3) << static_cast<unsigned>(cashTricks[2]) <<
    setw(3) << static_cast<unsigned>(cashTricks[3]) << "\n";
  
  out <<
    setw(10) << "cashRanks " <<
    setw(3) << SDS_RANK_NAMES[cashRanks[0]] <<
    setw(3) << SDS_RANK_NAMES[cashRanks[1]] <<
    setw(3) << SDS_RANK_NAMES[cashRanks[2]] <<
    setw(3) << SDS_RANK_NAMES[cashRanks[3]] << "\n";
}


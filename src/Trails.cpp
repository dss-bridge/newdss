/* 
   SDS, a bridge single-suit double-dummy quick-trick solver.

   Copyright (C) 2015-16 by Soren Hein.

   See LICENSE and README.
*/

#include <iostream>
#include <iomanip>
#include <assert.h>

#include "Trails.h"
#include "Segment.h"
#include "Trick.h"

using namespace std;


Trails::Trails()
{
  firstFlag = true;

  for (unsigned v = 0; v < 4; v++)
  {
    active[v] = false;
    trails[v].trick.cashing = 0;
    trails[v].trick.ranks = SDS_VOID;
  }

  trails[0].trick.start = QT_ACE;
  trails[0].trick.end = QT_ACE;

  trails[1].trick.start = QT_ACE;
  trails[1].trick.end = QT_PARD;

  trails[2].trick.start = QT_PARD;
  trails[2].trick.end = QT_ACE;

  trails[3].trick.start = QT_PARD;
  trails[3].trick.end = QT_PARD;
}


Trails::~Trails()
{
}


void Trails::Add(
  const Segment& seg)
{
  if (firstFlag)
  {
    switch(seg.GetStart())
    {
      case QT_ACE:
        active[0] = true;
        active[1] = true;
        break;
      case QT_PARD:
        active[2] = true;
        active[3] = true;
        break;
      case QT_BOTH:
        active[0] = true;
        active[1] = true;
        active[2] = true;
        active[3] = true;
        break;
      default:
        assert(false);
        break;
    }
    firstFlag = false;
  }

  assert(seg.len == 1 || seg.len == 2);

  if (seg.len == 1)
  {
    Trails::Extend(seg.list[0]);
  }
  else
  {
    // May have to do more here.
    assert(seg.list[0].trick.end != QT_BOTH);
    assert(seg.list[1].trick.end == SDS_PARTNER[seg.list[0].trick.end]);
    Trails::Extend(seg.list[1]);

    Trick tNew = seg.list[0];
    tNew.trick.cashing += seg.list[1].trick.cashing;
    if (seg.list[1].trick.ranks < tNew.trick.ranks)
      tNew.trick.ranks = seg.list[1].trick.ranks;
    Trails::Extend(tNew);
  }
}


void Trails::Extend(
  const Trick& tNew)
{
  if (tNew.trick.end == QT_ACE || tNew.trick.end == QT_BOTH)
  {
    if (active[0])
      Trails::Increment(trails[0], 0, tNew);
    if (active[2])
      Trails::Increment(trails[2], 2, tNew);
  }

  if (tNew.trick.end == QT_PARD || tNew.trick.end == QT_BOTH)
  {
    if (active[1])
      Trails::Increment(trails[1], 0, tNew);
    if (active[3])
      Trails::Increment(trails[3], 2, tNew);
  }
}


void Trails::Increment(
  Trick& trail,
  const unsigned base,
  const Trick& tNew)
{
  // Build on the longest trail.
  unsigned l = (trails[base].trick.cashing > trails[base+1].trick.cashing ?
    base : base+1);
  
  trail.trick.cashing = trails[l].trick.cashing;
  trail.trick.ranks = trails[l].trick.ranks;

  trail.trick.cashing += tNew.trick.cashing;
  if (tNew.trick.ranks < trail.trick.ranks)
    trail.trick.ranks = tNew.trick.ranks;
}


void Trails::Combine(
  const Trails trailList[],
  const unsigned len)
{
  // Choose the best for each start-end combination.

  for (unsigned t = 0; t < len; t++)
  {
    for (unsigned v = 0; v < 4; v++)
    {
      if (! trailList[t].active[v])
        continue;

      const Trick& tNew = trailList[t].trails[v];
      if (tNew.trick.cashing == 0)
        continue;

      active[v] = true;
      if (tNew.trick.cashing > trails[v].trick.cashing ||
         (tNew.trick.cashing == trails[v].trick.cashing &&
          tNew.trick.ranks > trails[v].trick.ranks))
      {
        trails[v].trick.cashing = tNew.trick.cashing;
        trails[v].trick.ranks = tNew.trick.ranks;
      }
    }
  }
}


CmpDetailType Trails::Compare(
  const Trails& tNew) const
{
  CmpDetailType c = SDS_HEADER_SAME;
  CmpDetailType cNew = SDS_HEADER_SAME;

  for (unsigned v = 0; v < 4; v++)
  {
    if (active[v] == tNew.active[v])
    {
      if (! active[v])
        continue;

      const Trick& t = tNew.trails[v];
      if (t.trick.cashing == trails[v].trick.cashing &&
          t.trick.ranks == trails[v].trick.ranks)
        continue;
      else if (t.trick.cashing > trails[v].trick.cashing)
        cNew = SDS_HEADER_PLAY_NEW_BETTER;
      else if (t.trick.cashing < trails[v].trick.cashing)
        cNew = SDS_HEADER_PLAY_OLD_BETTER;
      else if (t.trick.ranks > trails[v].trick.ranks)
        cNew = SDS_HEADER_RANK_NEW_BETTER;
      else
        cNew = SDS_HEADER_RANK_OLD_BETTER;
    }
    else
    {
      cNew = (active[v] ? 
        SDS_HEADER_PLAY_OLD_BETTER : SDS_HEADER_PLAY_NEW_BETTER);
    }

    c = cmpDetailMatrix[c][cNew];
    if (c == SDS_HEADER_PLAY_DIFFERENT || c == SDS_HEADER_RANK_DIFFERENT)
      return c;
  }
  return c;
}


void Trails::Print(
  ostream& out) const
{
  out << "Trails:\n";
  for (unsigned v = 0; v < 4; v++)
  {
    if (active[v])
      trails[v].Print(out);
  }
  out << "\n";
}


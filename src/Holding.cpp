/* 
   SDS, a bridge single-suit double-dummy quick-trick solver.

   Copyright (C) 2015-16 by Soren Hein.

   See LICENSE and README.
*/

#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>

#include <string.h>
#include <assert.h>

#include "Holding.h"
#include "const.h"

using namespace std;


Holding::Holding()
{
}


Holding::~Holding()
{
}


void Holding::Set(
  const unsigned sl,
  const unsigned c)
{
  this->suitLength = sl;
  this->counter = c;

  Holding::MakeRanks();
}


void Holding::MakeRanks()
{
  unsigned h, p, r;

  for (h = 0; h < SDS_HANDS; h++)
  {
    cardNo[h] = 0;
    length[h] = 0;
    for (r = 0; r < SDS_MAX_RANKS; r++)
    {
      cardListLo[h][r] = 0;
      cardListHi[h][r] = 0;
      completeList[h][r] = 0;
    }
  }

  cardNo[QT_ACE] = 1;
  cardListLo[QT_ACE][0] = static_cast<int>(suitLength-1);
  cardListHi[QT_ACE][0] = static_cast<int>(suitLength-1);

  length[QT_ACE] = 1;
  completeList[QT_ACE][0] = suitLength-1;

  unsigned hlast = QT_ACE;

  for (int m = static_cast<int>(suitLength)-2; m >= 0; m--)
  {
    p = (counter >> (2*static_cast<unsigned>(m))) & 3;

    // Play lowest of equals, other than the ace.
    if (p == hlast)
    {
      if (cardListLo[p][cardNo[p]-1] != static_cast<int>(suitLength)-1)
        cardListLo[p][cardNo[p]-1] = m;
    }
    else
    {
      cardListLo[p][cardNo[p]] = m;
      cardListHi[p][cardNo[p]] = m;
      cardNo[p]++;
    }

    hlast = p;

    completeList[p][length[p]] = static_cast<unsigned>(m);
    length[p]++;
  }

  for (h = 0; h < SDS_HANDS; h++)
  {
    if (cardNo[h])
      continue;
    
    cardListLo[h][0] = -1;
    cardListHi[h][0] = -1;

    cardNo[h] = 1;
  }

  for (h = 0; h < SDS_HANDS; h++)
    minRank[h] = cardListLo[h][cardNo[h]-1];

  maxDef = Max(cardListLo[QT_LHO][0], cardListLo[QT_RHO][0]);

  // To have something.
  for (r = 0; r < suitLength; r++)
    rankHolder[r] = QT_SIZE;

  for (h = 0; h < SDS_HANDS; h++)
    for (unsigned n = 0; n < length[h]; n++)
      rankHolder[completeList[h][n]] = h;
}


void Holding::SetSide(
  const PosType sideVal)
{
  if (sideVal == QT_ACE)
  {
    side = QT_ACE;
    lho = QT_LHO;
    pard = QT_PARD;
    rho = QT_RHO;
  }
  else if (sideVal == QT_LHO)
  {
    side = QT_LHO;
    lho = QT_PARD;
    pard = QT_RHO;
    rho = QT_ACE;
  }
  else if (sideVal == QT_PARD)
  {
    side = QT_PARD;
    lho = QT_RHO;
    pard = QT_ACE;
    rho = QT_LHO;
  }
  else if (sideVal == QT_RHO)
  {
    side = QT_RHO;
    lho = QT_ACE;
    pard = QT_LHO;
    rho = QT_PARD;
  }
  else
    assert(false);

  // If leader's lowest card can theoretically be run as a finesse,
  // we introduce a special lead that partner MUST overtake.
  // For example, with AQTx / - / 9876 / KJx we make the 6 lead
  // a card that LHO can duck, because we must overtake anyway.
  // We still keep the normal 6 lead as well, though.

  lowLeadPresentFlag = false;
  if (length[side] == 1 || length[lho] <= 1 || length[pard] <= 1)
    return;

  unsigned lrLoU = completeList[side][length[side]-1];
  int lrLo = static_cast<int>(lrLoU);
  if (lrLo < minRank[lho] || 
      lrLoU > completeList[lho][0])
    return;
  else if (lrLo < minRank[pard] ||
      lrLoU > completeList[pard][0])
    return;
  else if (length[rho] >= 1 && lrLoU < completeList[rho][0])
    return;
  else if (completeList[lho][0] > completeList[pard][0])
    return;

  lowLeadPresentFlag = true;
    
}


void Holding::RewindLead()
{
  numLeads = 0;
  leadCurrIndex = 0;
  int maxPard = cardListHi[pard][0];

  if (lowLeadPresentFlag)
  {
    // The special lead that can be run but must be overtaken.
    leadList[numLeads] = minRank[side];
    numLeads++;
  }

  for (unsigned l = 0; l < cardNo[side]; l++)
  {
    int lrHi = cardListHi[side][l];
    int lrLo = cardListLo[side][l];
    int m = Max(maxPard, lrHi);

    if (m < maxDef)
    {
      // Will lose trick.
      continue;
    }
    else if (lrHi > maxPard)
    {
      // Highest, will win.
      leadList[numLeads] = lrHi;
      numLeads++;
    }
    else if (lrHi < cardListHi[pard][cardNo[pard]-1])
    {
      // Even partner's lowest card is higher, so play lowest.
      if (l == cardNo[side]-1)
      {
        // Only play the lowest of such ranks.
        leadList[numLeads] = lrLo;
        numLeads++;
      }
    }
    else if (lrHi < cardListHi[rho][0])
    {
      // Just the lowest option.
      leadList[numLeads] = lrLo;
      numLeads++;
    }
    else if (lrHi > cardListHi[lho][0])
    {
      // Order is 1-2-0-3 or 1-3-0-2.  Either leader will win
      // (play highest), or partner will overtake (play lowest).
      leadList[numLeads] = lrLo;
      numLeads++;
      if (lrHi != lrLo)
      {
        leadList[numLeads] = lrHi;
        numLeads++;
      }
    }
    else if (cardListHi[lho][cardNo[lho]-1] > lrHi)
    {
      // Order is 2-1-0-3.  LHO must cover, so start low.
      leadList[numLeads] = lrLo;
      numLeads++;
    }
    else
    {
      // Order is 2-1-0-3 and LHO may or may not cover.
      // Consider all the options.
      for (int mm = lrLo; mm <= lrHi; mm++)
      {
        leadList[numLeads] = mm;
        numLeads++;
      }
    }
  }
}


void Holding::RewindLho()
{
  numLhos = 0;
  lhoCurrIndex = 0;

  unsigned numLhoGroups = cardNo[lho];

  if (numLhoGroups > 1 && 
      cardListHi[lho][numLhoGroups-1] == leadRank-1 &&
      cardListLo[lho][numLhoGroups-2] == leadRank+1 &&
      cardListHi[rho][0] < leadRank)
  {
    // Rather special case of type AJx / QT / Kxx / -.
    // If the jack is led, never play the T, so skip a group.

    numLhoGroups--;
  }

  // if (leadCurrIndex == 1 && lowLeadPresentFlag)
  // {
    // The special lead that partner must overtake.  LHO must duck.
    // lhoList[numLhos] = minRank[lho];
    // numLhos++;
    // return;
  // }

  for (unsigned l = 0; l < numLhoGroups; l++)
  {
    int c = cardListLo[lho][l];
    if (l == numLhoGroups-1)
    {
      // Lowest card is always an option.
      // Could also be void.
      lhoList[numLhos] = c;
      numLhos++;
      continue;
    }
    else if (c < leadRank)
    {
      // Only the lowest of these cards counts.
      continue;
    }
    else if (c < minRank[pard])
    {
      // No point in going higher.
      continue;
    }
    else if (c < cardListHi[rho][0])
    {
      // RHO has a higher card anyway.
      continue;
    }
    else
    {
      int nextDown = Max(cardListHi[rho][0], cardListLo[lho][l+1]);
      if (nextDown < leadRank)
      {
        // No lower card that still beats the lead
        lhoList[numLhos] = c;
        numLhos++;
        continue;
      }

      for (unsigned i = 0; i < cardNo[pard]; i++)
      {
        int pc = cardListLo[pard][i];
        if (pc < nextDown)
        {
          // Nothing more forthcoming
          break;
        }
        else if (pc < c)
        {
          // Partner has finesse, so the card is useful.
          lhoList[numLhos] = c;
          numLhos++;
          break;
        }
      }
    }
  }
}


void Holding::RewindPard()
{
  numPards = 0;
  pardCurrIndex = 0;

  for (unsigned l = 0; l < cardNo[pard]; l++)
  {
    int prLo = cardListLo[pard][l];
    int prHi = cardListHi[pard][l];

    int m = Max(leadRank, prLo);
    if (m < maxDefRun)
      continue;

    // If special lead, must duck.
    if (lowLeadPresentFlag && 
        leadCurrIndex == 1 &&
        prHi < leadRank)
      continue;

    bool skip = false;
    if (l+1 != cardNo[pard] && prHi < leadRank)
    {
      // AT / J654 / KQ97 / 8: On the A, no sense in playing the 9.
      // In general, skip unless the leader has a card in between.
      skip = true;
      for (int r = cardListHi[pard][l+1]+1; r < prLo && skip; r++)
      {
        if (rankHolder[r] == side)
          skip = false;
      }
    }
    if (skip)
      continue;

    if (numLeads > 1 &&
        leadRank > maxDef && 
        cardListHi[pard][0] > leadRank &&
        cardListLo[pard][cardNo[pard]-1] < leadRank)
    {
      if (rankHolder[leadRank+1] == side && prLo < leadRank)
      {
        // Have led a low top card -- don't duck.
        continue;
      }
      else if (rankHolder[leadRank-1] == side && prLo > leadRank)
      {
        // Have led a high top card -- don't overtake.
        continue;
      }
    }


    pardList[numPards] = (prLo > leadRank ? prHi : prLo);
    numPards++;
  }
}


bool Holding::NextLead()
{
  if (leadCurrIndex >= numLeads)
    return false;

  leadRank = leadList[leadCurrIndex];
  leadCurrIndex++;
  return true;
}


bool Holding::NextLho()
{
  if (lhoCurrIndex >= numLhos)
    return false;

  lhoRank = lhoList[lhoCurrIndex];
  lhoCurrIndex++;
  maxDefRun = Max(lhoRank, cardListHi[rho][0]);
  return true;
}


bool Holding::NextPard()
{
  if (pardCurrIndex >= numPards)
    return false;

  pardRank = pardList[pardCurrIndex];
  pardCurrIndex++;

  if (leadRank > pardRank)
  {
    winSide = side;
    if (maxDef == -1 && length[QT_PARD] == 0)
      winRank = suitLength; // This means "void"
    else
      winRank = static_cast<unsigned>(leadRank);
  }
  else
  {
    winSide = pard;
    winRank = static_cast<unsigned>(pardRank);
  }
  return true;
}


void Holding::SetRhoNo()
{
  // RHO will lose and so will play low.
  rhoRank = cardListLo[rho][cardNo[rho]-1];
}


unsigned Holding::GetSuitLength() const
{
  return suitLength;
}


unsigned Holding::GetCounter() const
{
  return counter;
}


PosType Holding::GetSide() const
{
  return side;
}


PosType Holding::GetWinSide() const
{
  return winSide;
}


unsigned Holding::GetLength(
  const int player) const
{
  assert(player >= 0 && player < SDS_HANDS);
  return length[player];
}


unsigned Holding::GetMinDeclLength() const
{
  return Min(length[QT_ACE], length[QT_PARD]);
}


unsigned Holding::GetDistHex() const
{
  return (length[QT_ACE] << 12) | (length[QT_LHO] << 8) |
    (length[QT_PARD] << 4) | length[QT_RHO];
}


unsigned Holding::GetPrependRank() const
{
  return Holding::ListToRank(winRank);
}


unsigned Holding::GetLHOMaxRank() const
{
  if (length[QT_LHO] == 0)
    return 0;
  else
    return Holding::ListToRank(static_cast<unsigned>(cardListHi[QT_LHO][0]));
}


unsigned Holding::GetMaxOppRank() const
{
  unsigned m = static_cast<unsigned>(
    Max(cardListHi[QT_LHO][0], cardListHi[QT_RHO][0]));

  return Holding::ListToRank(m);
}


Trick Holding::GetTrick() const
{
  Trick trick;
  trick.Set(side, winSide, Holding::GetPrependRank(), 1);
  return trick;
}



unsigned Holding::GetNumTops() const
{
  unsigned n = 1;
  while (n < length[QT_ACE] &&
      completeList[QT_ACE][n] == completeList[QT_ACE][n-1]-1)
    n++;

  return n;
}


PosType Holding::GetOppBest() const
{
  if (length[QT_LHO] == 0)
    return QT_RHO;
  else if (length[QT_RHO] == 0)
    return QT_LHO;
  else
  {
    int lhoLo = cardListLo[QT_LHO][0];
    int lhoHi = cardListHi[QT_LHO][0];
    int rhoLo = cardListLo[QT_RHO][0];
    int rhoHi = cardListHi[QT_RHO][0];
    
    if (lhoLo == rhoHi+1 || rhoLo == lhoHi+1)
      return QT_BOTH;
    else
      return (lhoLo > rhoLo ? QT_LHO : QT_RHO);
  }
}


bool Holding::IsAATrick() const
{
  return (side == QT_ACE && winSide == QT_ACE);
}


bool Holding::IsRealPP(const unsigned pRank) const
{
  return (length[QT_ACE] >= 2 && length[QT_PARD] >= 2 &&
      Holding::ListToRank(completeList[QT_ACE][1]) < pRank);
}


bool Holding::APIsBP() const
{
  if (length[QT_PARD] == 2 ||
      length[QT_LHO] <= 2)
    return true;

  unsigned aTop = completeList[QT_ACE][0];
  if (static_cast<int>(aTop) == pardRank)
    aTop = completeList[QT_ACE][1];

  // Note that side == QT_PARD, so rho is QT_LHO.
  unsigned maxl;
  if (length[QT_LHO] == 0)
    maxl = 0;
  else if (static_cast<int>(completeList[QT_LHO][0]) != rhoRank)
    maxl = completeList[QT_LHO][0];
  else if (length[QT_LHO] == 1)
    maxl = 0;
  else
    maxl = completeList[QT_LHO][1];

  if (aTop > maxl)
    return true;
  else
    return false;
}


bool Holding::HasRemainingPTop() const
{
  if (length[QT_ACE] <= 1 || length[QT_PARD] <= 1)
    return false;

  unsigned pTop = completeList[QT_PARD][0];
  if (static_cast<int>(pTop) == leadRank)
    pTop = completeList[QT_PARD][1];

  // Note that side == QT_PARD, so rho is QT_LHO.
  unsigned maxl;
  if (length[QT_LHO] == 0)
    maxl = 0;
  else if (static_cast<int>(completeList[QT_LHO][0]) != rhoRank)
    maxl = completeList[QT_LHO][0];
  else if (length[QT_LHO] == 1)
    maxl = 0;
  else
    maxl = completeList[QT_LHO][1];

  unsigned maxr;
  if (length[QT_RHO] == 0)
    maxr = 0;
  else if (static_cast<int>(completeList[QT_RHO][0]) != lhoRank)
    maxr = completeList[QT_RHO][0];
  else if (length[QT_RHO] == 1)
    maxr = 0;
  else
    maxr = completeList[QT_RHO][1];

  unsigned maxd = Max(maxl, maxr);
  return (pTop > maxd);
}


bool Holding::IsNotFinesse(const PosType pStart) const
{
  // Checking whether AA+PPn can be combined to AB.  pStart = QT_ACE then.
  if (pStart == QT_ACE)
  {
    return (completeList[QT_RHO][0] < completeList[QT_PARD][0]);
  }
  else if (pStart == QT_PARD)
  {
    return (completeList[QT_LHO][0] < completeList[QT_ACE][0]);
  }
  else
    return false;
}
 

bool Holding::IsPA1ACasher() const
{
  return (side == QT_PARD && 
      winSide == QT_ACE && 
      winRank == suitLength-1 &&
      (length[QT_RHO] <= 1 || 
       completeList[QT_RHO][0] < static_cast<unsigned>(leadRank)));
}
 

const unsigned loMask[13] =
{
  0x000000, // 2
  0x000003, // 3
  0x00000f, // 4
  0x00003f, // 5
  0x0000ff, // 6
  0x0003ff, // 7
  0x000fff, // 8
  0x003fff, // 9
  0x00ffff, // T
  0x03ffff, // J
  0x0fffff, // Q
  0x3fffff, // K
  0xffffff  // A
};

const unsigned hiMask[13] =
{
  0xfffffc, // 2
  0xfffff0, // 3
  0xffffc0, // 4
  0xffff00, // 5
  0xfffc00, // 6
  0xfff000, // 7
  0xffc000, // 8
  0xff0000, // 9
  0xfc0000, // T
  0xf00000, // J
  0xc00000, // Q
  0x000000, // K
  0x000000, // A
};


void Holding::AdjustWinRank()
{
  // We adjust winRank when the lead wins and when the defense
  // has a card higher than the winner (so a finesse).

  if (static_cast<int>(winRank) != leadRank || 
      static_cast<int>(winRank) > maxDef)
    return;

  // Skip if the leader has the next HIGHER card as well.
  // leadList is currently not in top-down order in this case,
  // which is not so pretty.
  if (leadCurrIndex < numLeads &&
      static_cast<int>(winRank) + 1 == leadList[leadCurrIndex])
    return;

  // We look for the next rank down, skipping over played cards.
  unsigned soughtRank = winRank-1;
  while (rankHolder[soughtRank] == QT_LHO ||
         rankHolder[soughtRank] == QT_RHO)
    soughtRank--;

  winRank = soughtRank;
  return;

  /*
  if (soughtRank == static_cast<unsigned>(lhoRank))
  {
    if (soughtRank == 0) return;
    soughtRank--;
    if (soughtRank == static_cast<unsigned>(rhoRank))
    {
      if (soughtRank == 0) return;
      soughtRank--;
    }
  }
  else if (soughtRank == static_cast<unsigned>(rhoRank))
  {
    if (soughtRank == 0) return;
    soughtRank--;
    if (soughtRank == static_cast<unsigned>(lhoRank))
    {
      if (soughtRank == 0) return;
      soughtRank--;
    }
  }
  */

  // If declarer holds that card, 
  if (leadCurrIndex < numLeads)
  {
    for (unsigned p = 0; p < length[side]; p++)
    {
      if (completeList[side][p] == soughtRank)
      {
        winRank = soughtRank;
        return;
      }
    }
  }

  if (leadCurrIndex > 1 && 
      leadList[leadCurrIndex-2] == static_cast<int>(soughtRank))
  {
    // Effectively same rank, e.g. JT where we already played T.
    winRank = soughtRank;
  }
  else if (rankHolder[soughtRank] == pard)
    winRank = soughtRank;
}


unsigned Holding::GetSideRun(
  const unsigned start) const
{
  // Requires start to belong to declarer's side.
  unsigned r = start+1;
  while (r < SDS_MAX_RANKS && 
      (rankHolder[r] == QT_ACE || rankHolder[r] == QT_PARD))
    r++;
  return r - start;
}


void Holding::PossiblyFixRankFinesse(
  AdjustRankType& adjust) const
{
  // First case: Partner could have finessed against LHO with one
  // of his own cards (i.e. not by running the lead).

  // Follow the run upwards.  By definition, the top card can only
  // be from leader's side, as partner would otherwise have won 
  // the trick with a higher card from the run.

  if (minRank[pard] > lhoRank)
    return;

  int nextPard = lhoRank;
  while (rankHolder[nextPard] != pard)
    nextPard--;

  if (nextPard < leadRank || minRank[lho] > nextPard)
    return;

  unsigned np = static_cast<unsigned>(nextPard);
  if (length[rho] > 0 && completeList[rho][0] > np)
    return;

  unsigned run = Holding::GetSideRun(np);
  if (run == 1)
    return;

  // A bit suspect, perhaps: Don't count a double split yet.
  // if (static_cast<unsigned>(lhoRank) != completeList[lho][0])
    // return;

  // Not sure this is exactly right.
  // if (length[side] <= length[lho])
    // return;

  // if (completeList[lho][0] + length[lho] >= suitLength)
  // if (length[lho] >= Holding::TopsOverRank(side, np) + 1)
      // Holding::TopsOverRank(pard, np) + 
      // Holding::TopsOverRank(side, static_cast<unsigned>(lhoRank)))
  unsigned lr;
  if (static_cast<unsigned>(lhoRank) < completeList[lho][0])
    lr = completeList[lho][0];
  else
    lr = completeList[lho][1];

  if (length[lho] > 
        Holding::TopsOverRank(pard, lr) + Holding::TopsOverRank(side, lr))
  {
    unsigned a = Holding::ListToRank(np);
    adjust.lower1 = a;
    adjust.upper1 = a + run - 1;
  }
}


void Holding::PossiblyFixRankRun(
  AdjustRankType& adjust) const
{
  // Second case: Leader could have run his lead if not covered.
  
  // Start from the the higher of (a) the lead, 
  // (b) LHO's lowest card, (c) RHO's highest card if not void.  
  // If partner's lowest card is higher, skip.
  // Otherwise, work up a run from there.

  if (minRank[lho] > leadRank)
    return;
  if (length[rho] > 0 && static_cast<int>(completeList[rho][0]) > leadRank)
    return;
  if (minRank[pard] > leadRank)
    return;
  if (lowLeadPresentFlag && leadCurrIndex == 1)
    return;

  unsigned nl = static_cast<unsigned>(leadRank);
  unsigned run = Holding::GetSideRun(nl);
  if (run == 1)
    return;

  unsigned topRunSide = nl + run - 1;

  // Don't adjust if there are enough tops higher than the card led,
  // such that LHO can be cashed out after this cover.
  // A8 / 7654 / KJT / Q9,
  // AJ6 / 75 / KT98 / Q4,
  // AT6 / - / KQ98 / J754,
  // AKT7 / - / Q98 / J654.
  // unsigned cashers = Holding::TopsOverRank(side, nl) - 1 +
    // Holding::TopsOverRank(pard, nl);
  // if (lr < nl && cashers >= length[lho])
    // return;

  unsigned lh = static_cast<unsigned>(lhoRank);
  unsigned cashers = 
    Holding::TopsOverRank(side, lh) +
    Holding::TopsOverRank(pard, lh);
  if (cashers >= length[lho] && length[side] > length[lho])
  {
    // AKQ54 / - / 9873 -/ JT6 on the 7 lead: Adjust 8 to 7.
    if (run == 2)
    {
      // Not AKQ54 / 2 / T873 / J96.
      if (topRunSide == completeList[side][0])
        return;
    }
    else if (topRunSide == completeList[side][0])
      topRunSide--;
  }

  // Only adjust up the highest side card in that run.
  // Can write a single, short function to find this later...

  // while (rankHolder[topRunSide] == pard)
    // topRunSide--;

  // if (topRunSide == nl)
    // return;

  adjust.lower2 = Holding::ListToRank(nl);
  adjust.upper2 = Holding::ListToRank(topRunSide);
  return;


  unsigned lr;
  if (static_cast<unsigned>(lhoRank) < completeList[lho][0])
    lr = completeList[lho][0];
  else
    lr = completeList[lho][1];

  int nextPard = lhoRank;
  while (rankHolder[nextPard] != pard)
    nextPard--;
  unsigned np = static_cast<unsigned>(nextPard);
  if (nextPard < leadRank || minRank[lho] > nextPard)
    np = SDS_VOID;
  else if (length[rho] > 0 && completeList[rho][0] > np)
    np = SDS_VOID;


  if (np != SDS_VOID)
  {
    // If there is a finesse position, stay on lead side.
    while (rankHolder[topRunSide] != side)
      topRunSide--;
    if (topRunSide == nl)
      return;

    adjust.lower2 = Holding::ListToRank(nl);
    adjust.upper2 = Holding::ListToRank(topRunSide);
    return;
  }

  if (length[lho] <= Holding::TopsOverRank(pard, lr))
  {
    // If the suit can be cashed out completely from partner after this
    // trick, then only consider leader's side for any correction.
    while (rankHolder[topRunSide] != side)
      topRunSide--;
    if (topRunSide == nl)
      return;
  }

  // if (length[lho] >= Holding::TopsOverRank(side, nl))
  // unsigned lr = static_cast<unsigned>(lhoRank);
  // if (length[lho] >= Holding::TopsOverRank(side, nl))
  // {
  // if (length[side] <= length[lho])
  // if (length[lho] > 
        // Holding::TopsOverRank(pard, lr) + Holding::TopsOverRank(side, lr))
  if (length[lho] >= Holding::TopsOverRank(side, nl))
  {
    adjust.lower2 = Holding::ListToRank(nl);
    adjust.upper2 = Holding::ListToRank(topRunSide);
  }
}


#include "options.h"
extern OptionsType options;
void Holding::PossiblyFixRank(
  AdjustRankType& adjust) const
{
  adjust.lower1 = SDS_VOID;
  adjust.lower2 = SDS_VOID;

  if (length[side] == 1 ||
      length[pard] == 1 ||
      length[lho] == 1 ||
      leadRank > lhoRank)
    return;

  Holding::PossiblyFixRankFinesse(adjust);
  Holding::PossiblyFixRankRun(adjust);

if (options.debugFlow)
{
  if (adjust.lower1 != SDS_VOID)
  {
Holding::Print();
Holding::PrintPlay();
    cout << "adjust high (finesse): " << 
      adjust.upper1 << " to " << adjust.lower1 << "\n\n";
  }
  if (adjust.lower2 != SDS_VOID)
  {
Holding::Print();
Holding::PrintPlay();
    cout << "adjust low (run): " << 
      adjust.upper2 << " to " << adjust.lower2 << "\n\n";
  }
}
}


unsigned Holding::PossiblyFixRank(
  unsigned& fixedRank) const
{
  // Make sure the trick is a cover that was "actively" made.
  unsigned lr = static_cast<unsigned>(leadRank);
  if (length[side] == 1 ||
      length[pard] == 1 ||
      leadRank > lhoRank)
    return SDS_VOID;

  // if (minRank[pard] > leadRank)
  // It's probably enough that LHO has a card below lhoRank that
  // beats leadRank (doesn't have to be the lowest LHO card).
  int nextLho;
  if (minRank[lho] == lhoRank)
    nextLho = static_cast<int>(lhoRank);
  else
  {
    nextLho = static_cast<int>(lhoRank) - 1;
    while (rankHolder[nextLho] != lho)
      nextLho--;
  }

  // From AQT98 / - / 76 / KJ5, use the T and not the 8 on the J.
  int nextPard = static_cast<int>(lhoRank);
  if (minRank[pard] < lhoRank)
  {
    while (rankHolder[nextPard] != pard)
      nextPard--;
  }

  // if (minRank[pard] > leadRank || minRank[lho] > leadRank)
  bool simpleFlag = 
      (minRank[lho] > leadRank ||
       minRank[pard] > static_cast<int>(completeList[side][0]) ||
      (minRank[pard] > leadRank && nextLho < minRank[pard]));
      // (nextLho > leadRank && nextLho > nextPard)); // ||
      // (length[side] > length[lho] && 
          // nextPard > leadRank))
          // nextPard > static_cast<int>(completeList[side][0])));
  // if (minRank[pard] > leadRank || nextLho > leadRank)
  if (simpleFlag)
  {
    if (minRank[pard] > lhoRank)
      return SDS_VOID;

    if (minRank[lho] > nextPard)
      return SDS_VOID;

    // From AQT98 / - / 76 / KJ5, use the T and not the 8 on the J.
    lr = static_cast<unsigned>(nextPard);
    
  }

  if (length[rho] > 0 && completeList[rho][0] > lr)
    return SDS_VOID;

  // Find the number of consecutive higher cards with our side.
  unsigned run = Holding::GetSideRun(lr);
  if (run == 1)
    return SDS_VOID;
  unsigned h = lr + run - 1;
  fixedRank = Holding::ListToRank(lr);

  if (run == 2)
  {
    // AKQ2 / - / T9 / J876.
    // Lead 9: 9-J-A-void, then later PA1A + AP1T + AA2-.
    // But only covers when we have T9.
    // General rule turns out to be: length >= #tops with leader.
    // Ditto to change above.
    // if (minRank[pard] > leadRank || minRank[lho] > leadRank)
    // if (minRank[pard] > leadRank || nextLho > leadRank)
    if (simpleFlag)
    {
      if (completeList[lho][0] + length[lho] >= suitLength &&
          rankHolder[h] == side)
      {
        // Probably not the final truth, but works for now.
        // Kx, Qxx, Jxxx etc.
        return Holding::ListToRank(h);
      }
      else
        return SDS_VOID;
    }
    else
    {
      if (length[lho] >= Holding::TopsOverRank(side, lr) &&
          rankHolder[h] == side)
      {
        return Holding::ListToRank(h);
      }
      else
        return SDS_VOID;
    }
  }

  // At least three in a row at the top, leader with no very high card.
  unsigned s0 = completeList[side][0];
  unsigned p1 = completeList[pard][1];
  if (s0 > h)
  {
    return SDS_VOID;
    // Need to permit some of these.

    if ((length[lho] == 2 && p1 < lr) ||
        (length[lho] == 3 &&
          (length[pard] < length[lho] ||
           completeList[pard][2] < completeList[lho][0])) ||
        (length[lho] >= 4 &&
          (length[pard] < length[lho] ||
           completeList[pard][length[lho]-1] < completeList[lho][0])))
    {
      return Holding::ListToRank(h);
    }
    else 
    if (length[lho] >= 3)
    {
      // Pick the last of the leader's own run.
      unsigned reduced = h;
      while (reduced >= lr && rankHolder[reduced] != side)
        reduced--;
      if (reduced > lr)
        return Holding::ListToRank(reduced);
      else
        return SDS_VOID;
    }
    else
      return SDS_VOID;
  }

  if (length[side] == 2 && 
    s0 > lr)
    return Holding::ListToRank(s0);
  else if ((length[lho] == 2 && p1 < lr) ||
      (length[lho] == 3 &&
        (length[pard] < length[lho] ||
         completeList[pard][2] < completeList[lho][0])) ||
      (length[lho] >= 4 &&
        (length[pard] < length[lho] ||
         completeList[pard][length[lho]-1] < completeList[lho][0])))
  {
    return Holding::ListToRank(h);
  }
  else if (s0 == h && length[lho] >= 3)
  {
    // Pick the last of the leader's own run.
    unsigned reduced = h-1;
    while (reduced >= lr && rankHolder[reduced] != side)
      reduced--;
    if (reduced > lr)
      return Holding::ListToRank(reduced);
    else
      return SDS_VOID;
  }
  else
    return SDS_VOID;
}


bool Holding::MakePlay(
  unsigned& slNew,
  unsigned& cNew)
{
  // true if a move can be looked up, false if the trick is the
  // last one that can be won by declarer.

  // If the lead ends up winning the trick, we want to play the
  // highest of equals in order to have the most sparing use of ranks.  
  // If not, the lowest of equals for the same reason.

  slNew = suitLength;
  cNew = counter;

  int used[13] = {0};
  if (rhoRank != -1)
    used[rhoRank] = 1;

  if (lhoRank != -1)
    used[lhoRank] = 1;

  bool maybeNewAce = false;

  if (leadRank == static_cast<int>(suitLength)-1)
  {
    maybeNewAce = true;
    slNew--;
  }
  else if (leadRank != -1)
  {
    // In certain cases we still have to determine the actual lead
    // from among equals, e.g. Ax / - / QJ / Kx.  If the H was covered,
    // then it was retroactively the Q; if ducked, the J.

    used[leadRank] = 1;
  }


  if (pardRank == static_cast<int>(suitLength)-1)
  {
    maybeNewAce = true;
    slNew--;
  }
  else if (pardRank != -1)
    used[pardRank] = 1;

  Holding::AdjustWinRank();

  for (int u = static_cast<int>(suitLength)-2; u >= 0; u--)
  {
    if (! used[u])
      continue;
    slNew--;
    cNew = (cNew & loMask[u]) | ((cNew & hiMask[u]) >> 2);
  }

  if (slNew <= 0)
    return false;

  // 0 is 2, ..., 12 is A, 13 is void(!).
  rankMap[13] = 13;

  int revRunRank = 12;
  if (! maybeNewAce)
  {
    // Original ace still in play.
    rankMap[revRunRank--] = 12;
  }

  for (int u = static_cast<int>(suitLength)-2; u >= 0; u--)
  {
    if (used[u])
      continue;

    rankMap[revRunRank--] = 12 - (suitLength-static_cast<unsigned>(u)-1);
  }

  if (maybeNewAce)
  {
    // cNew is in a sense done already, as the "ace" is implicit.
    // But we have to check who has the new "ace".

    PosType newTop = static_cast<PosType>((cNew >> 2*(slNew-1)) & 0x3);
    if (newTop == QT_LHO || newTop == QT_RHO)
    {
      // This was the last quick trick for declarer.
      return false;
    }
    else if (newTop == QT_PARD)
      aceFlip = true;
    else
      aceFlip = false;
  }
  else
    aceFlip = false;

  if (aceFlip)
  {
    unsigned cFlip = cNew;
    unsigned flipBit;
    for (unsigned s = 0; s < slNew; s++)
    {
      flipBit = static_cast<unsigned>(0x2) << (2*s);
      cFlip ^= flipBit;
    }

    cNew = cFlip;
  }

  mergeSpecialFlag = false;
  if ((length[QT_LHO] == 0 && length[QT_RHO] == 0 &&
      length[QT_ACE] >= length[QT_PARD] && length[QT_PARD] >= 2) ||
     (length[QT_ACE] == length[QT_PARD] && 
      length[QT_LHO] <= 1 && length[QT_RHO] <= 1))
    mergeSpecialFlag = true;

  return true;
}


unsigned Holding::FlipTops(
  const unsigned numTops,
  const unsigned nMask) const
{
  unsigned cFlipped = counter;
  unsigned maskFull = (1u << (2*(suitLength-1))) - 1u;

  // Flip the ace cards to partner.
  for (unsigned n = 0; n < suitLength-1; n++)
  {
    unsigned mask = 0x3u << (2*n);
    unsigned p = (counter >> (2*n)) & 0x3u;
    if (p == QT_ACE)
      cFlipped = (cFlipped & (maskFull ^ mask)) | 
        (static_cast<unsigned>(QT_PARD) << (2*n));
  }

  // Ace is implicit.  Expand the rest of the mask, so each single
  // bit becomes doubled up.
  unsigned expMask = 0;
  for (unsigned n = 0; n < numTops-1u; n++)
  {
    if (nMask & (1u << n))
      expMask |= (0x3u << (2*n));
  }
  expMask = (expMask << 2*(suitLength-numTops));

  if (nMask & (1u << (numTops-1))) // Ace also to be flipped
    return (expMask & counter) | ((maskFull ^ expMask) & cFlipped);
  else
    return (expMask & cFlipped) | ((maskFull ^ expMask) & counter);
}


bool Holding::GetAceFlip() const
{
  return aceFlip;
}


unsigned Holding::GetNewRank(
  const unsigned r) const
{
  // Used when updating a looked-up trick list and appending
  // a new trick.

  unsigned rNew = rankMap[r];

  // Normally PP1Q + BA1A = PP1A + BA1- = PA2Q.
  // But there is a special Prepend case where it becomes PA2A.
  // So we can't throw away that information.

  if (mergeSpecialFlag)
    return rNew;
  else
    return (Holding::GetPrependRank() < rNew ? SDS_VOID : rNew);
}


bool Holding::GetMergeType() const
{
  return mergeSpecialFlag;
}


unsigned Holding::ListToRank(
  const unsigned listValue) const
{
  return (SDS_VOID - suitLength + listValue);
  
}

unsigned Holding::TopsOverRank(
  const PosType& player,
  const unsigned rank) const
{
  unsigned c = 0;
  for (; c < length[player]; c++)
    if (completeList[player][c] < rank)
      break;
  return c;
}


void Holding::Print(
  ostream& out,
  const bool showBinary) const
{
  if (showBinary)
    out << "sl " << suitLength << " c " << hex << counter << dec << "\n";

  char text[SDS_HANDS][20];
  char * p[SDS_HANDS];
  unsigned h;
  for (h = 0; h < SDS_HANDS; h++)
  {
    strcpy(text[h], "");
    p[h] = text[h];
  }

  * p[QT_ACE] = SDS_RANK_NAMES[SDS_ACE];
  p[QT_ACE]++;
  unsigned r = SDS_ACE - 1;

  for (unsigned m = 0; m < suitLength-1; m++)
  {
    h = (counter >> (2*(suitLength-2-m))) & 0x3;
    * p[h] = SDS_RANK_NAMES[r--];
    p[h]++;
  }

  for (h = 0; h < SDS_HANDS; h++)
  {
    if (p[h] == text[h])
    {
      * p[h] = '-';
      p[h]++;
    }
    * p[h] = '\0';
  }

  out <<
    setw(8) << "" << setw(12) << left << text[0] << "\n" <<
    setw(12) << left << text[3] << setw(4) << "" << 
      setw(12) << text[1] << "\n" <<
    setw(8) << "" << setw(12) << left << text[2] << "\n";
}


void Holding::PrintRanksList(
  const unsigned list[][SDS_MAX_RANKS],
  const unsigned listLen[SDS_HANDS],
  const string text,
  std::ostream& out) const
{
  if (text != "")
    out << text << "\n";

  for (unsigned i = 0; i < SDS_HANDS; i++)
  {
    out << i << ": " << setw(2) << listLen[i] << ", ";
    for (unsigned j = 0; j < listLen[i]; j++)
      out << list[i][j] << "-";
    out << "\n";
  }
  out << "\n";
}


void Holding::PrintRanks(
  std::ostream& out) const
{
  // Later on, chen cardList is unsigned, switch these as well.

  out << "Lowest new ranks\n";
  for (unsigned i = 0; i < SDS_HANDS; i++)
  {
    assert(cardNo[i] <= 13);
    out << i << ": " << setw(2) << cardNo[i] << ", ";
    for (unsigned j = 0; j < cardNo[i]; j++)
      out << cardListLo[i][j] << "-";
    out << "\n";
  }
  out << "\n";

  out << "Highest new ranks\n";
  for (unsigned i = 0; i < SDS_HANDS; i++)
  {
    out << i << ": " << setw(2) << cardNo[i] << ", ";
    for (unsigned j = 0; j < cardNo[i]; j++)
      out << cardListHi[i][j] << "-";
    out << "\n";
  }
  out << "\n";

  Holding::PrintRanksList(completeList, length, "Complete ranks", out);
}


void Holding::PrintPlay(
  std::ostream& out) const
{
  out << "side " << 
    static_cast<int>(side) << " trick " << leadRank <<
    " (" << lhoRank << ") " << pardRank << " (" << rhoRank << ")\n";
}


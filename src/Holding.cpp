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
}


void Holding::RewindLead()
{
  numLeads = 0;
  leadCurrIndex = 0;
  int maxPard = cardListHi[pard][0];

  for (unsigned l = 0; l < cardNo[side]; l++)
  {
    int lrHi = cardListHi[side][l];
    int lrLo = cardListLo[side][l];
    int m = Max(maxPard, lrHi);

    multiLead[lrHi] = false;
    multiLead[lrLo] = false;
      
    if (m < maxDef)
    {
      // Will lose trick.
      continue;
    }
    else if (lrHi < cardListHi[rho][0])
    {
      // Just the lowest option.
      leadList[numLeads] = lrLo;
      numLeads++;
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


PosType Holding::GetWinSide()
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


unsigned Holding::GetMaxOppRank()
{
  unsigned m = static_cast<unsigned>(
    Max(cardListHi[QT_LHO][0], cardListHi[QT_RHO][0]));

  return Holding::ListToRank(m);
}


const Trick Holding::GetTrick() const
{
  Trick trick;
  trick.Set(side, winSide, Holding::GetPrependRank(), 1);
  return trick;
}


bool Holding::IsAATrick() const
{
  return (side == QT_ACE && winSide == QT_ACE);
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
}


unsigned Holding::GetNumTops()
{
  unsigned n = 1;
  while (n < length[QT_ACE] &&
      completeList[QT_ACE][n] == completeList[QT_ACE][n-1]-1)
    n++;

  return n;
}


PosType Holding::GetOppBest()
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
  // We only adjust winRank when the lead wins and when the defense
  // has a card higher than the winner (so a finesse).
  if (static_cast<int>(winRank) != leadRank || static_cast<int>(winRank) > maxDef)
    return;

  // We look for the next rank down, skipping over played cards.
  unsigned soughtRank = winRank-1;
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

  // If declarer holds that card, 
  if (leadCurrIndex < numLeads && leadList[leadCurrIndex] == 
    static_cast<int>(soughtRank))
  {
    winRank = soughtRank;
  }
  else
  {
    // Could have some kind of table instead.
    for (unsigned p = 0; p < length[pard]; p++)
    {
      if (completeList[pard][p] == soughtRank)
      {
        winRank = soughtRank;
        return;
      }
    }
  }
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


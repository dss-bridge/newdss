/* 
   SDS, a bridge single-suit double-dummy quick-trick solver.

   Copyright (C) 2015-16 by Soren Hein.

   See LICENSE and README.
*/

#include <iostream>
#include <iomanip>
#include <string>

#include <assert.h>

#include "Trick.h"
#include "LoopHold.h"
#include "DefList.h"
#include "const.h"

using namespace std;


extern vector<unsigned> holdCtr;

bool pickFlag;

const char POS_NAMES[QT_SIZE][10] =
{
  "Ace", "LHO", "Pard", "RHO", "Both"
};

// Shorthand.
#define HR(pl, pos) (Holding::ListToRank(completeList[pl][pos]))


LoopHold::LoopHold()
{
  // For historical reasons the combinations and functions
  // are labeled as 16*Jack + 4*Queen + King.
  // The lookup is done with 16*King * 4*Queen + Jack.
  // The dispatch table includes both this translation and the
  // fact that one function sometime covers several combination.

  SolveSimpleFunction[0] = &LoopHold::SolveSimple0;
  SolveSimpleFunction[1] = &LoopHold::SolveSimple1;
  SolveSimpleFunction[2] = &LoopHold::SolveSimple2;
  SolveSimpleFunction[3] = &LoopHold::SolveSimple48;
  SolveSimpleFunction[4] = &LoopHold::SolveSimple1;
  SolveSimpleFunction[5] = &LoopHold::SolveSimple20;
  SolveSimpleFunction[6] = &LoopHold::SolveSimple36;
  SolveSimpleFunction[7] = &LoopHold::SolveSimple28;
  SolveSimpleFunction[8] = &LoopHold::SolveSimple2;
  SolveSimpleFunction[9] = &LoopHold::SolveSimple18;

  SolveSimpleFunction[10] = &LoopHold::SolveSimple2;
  SolveSimpleFunction[11] = &LoopHold::SolveSimple26;
  SolveSimpleFunction[12] = &LoopHold::SolveSimple12;
  SolveSimpleFunction[13] = &LoopHold::SolveSimple28;
  SolveSimpleFunction[14] = &LoopHold::SolveSimple44;
  SolveSimpleFunction[15] = &LoopHold::SolveSimple60;
  SolveSimpleFunction[16] = &LoopHold::SolveSimple1;
  SolveSimpleFunction[17] = &LoopHold::SolveSimple15;
  SolveSimpleFunction[18] = &LoopHold::SolveSimple9;
  SolveSimpleFunction[19] = &LoopHold::SolveSimple49;

  SolveSimpleFunction[20] = &LoopHold::SolveSimple5;
  SolveSimpleFunction[21] = &LoopHold::SolveSimple5;
  SolveSimpleFunction[22] = &LoopHold::SolveSimple5;
  SolveSimpleFunction[23] = &LoopHold::SolveSimple5;
  SolveSimpleFunction[24] = &LoopHold::SolveSimple9;
  SolveSimpleFunction[25] = &LoopHold::SolveSimple15;
  SolveSimpleFunction[26] = &LoopHold::SolveSimple41;
  SolveSimpleFunction[27] = &LoopHold::SolveSimple57;
  SolveSimpleFunction[28] = &LoopHold::SolveSimple7;
  SolveSimpleFunction[29] = &LoopHold::SolveSimple15;

  SolveSimpleFunction[30] = &LoopHold::SolveSimple7;
  SolveSimpleFunction[31] = &LoopHold::SolveSimple15;
  SolveSimpleFunction[32] = &LoopHold::SolveSimple2;
  SolveSimpleFunction[33] = &LoopHold::SolveSimple18;
  SolveSimpleFunction[34] = &LoopHold::SolveSimple2;
  SolveSimpleFunction[35] = &LoopHold::SolveSimple26;
  SolveSimpleFunction[36] = &LoopHold::SolveSimple6;
  SolveSimpleFunction[37] = &LoopHold::SolveSimple20;
  SolveSimpleFunction[38] = &LoopHold::SolveSimple14;
  SolveSimpleFunction[39] = &LoopHold::SolveSimple28;

  SolveSimpleFunction[40] = &LoopHold::SolveSimple2;
  SolveSimpleFunction[41] = &LoopHold::SolveSimple26;
  SolveSimpleFunction[42] = &LoopHold::SolveSimple2;
  SolveSimpleFunction[43] = &LoopHold::SolveSimple18;
  SolveSimpleFunction[44] = &LoopHold::SolveSimple14;
  SolveSimpleFunction[45] = &LoopHold::SolveSimple28;
  SolveSimpleFunction[46] = &LoopHold::SolveSimple6;
  SolveSimpleFunction[47] = &LoopHold::SolveSimple60;
  SolveSimpleFunction[48] = &LoopHold::SolveSimple3;
  SolveSimpleFunction[49] = &LoopHold::SolveSimple19;

  SolveSimpleFunction[50] = &LoopHold::SolveSimple11;
  SolveSimpleFunction[51] = &LoopHold::SolveSimple15;
  SolveSimpleFunction[52] = &LoopHold::SolveSimple7;
  SolveSimpleFunction[53] = &LoopHold::SolveSimple15;
  SolveSimpleFunction[54] = &LoopHold::SolveSimple7;
  SolveSimpleFunction[55] = &LoopHold::SolveSimple15;
  SolveSimpleFunction[56] = &LoopHold::SolveSimple11;
  SolveSimpleFunction[57] = &LoopHold::SolveSimple27;
  SolveSimpleFunction[58] = &LoopHold::SolveSimple43;
  SolveSimpleFunction[59] = &LoopHold::SolveSimple15;

  SolveSimpleFunction[60] = &LoopHold::SolveSimple15;
  SolveSimpleFunction[61] = &LoopHold::SolveSimple15;
  SolveSimpleFunction[62] = &LoopHold::SolveSimple15;
  SolveSimpleFunction[63] = &LoopHold::SolveSimple15;

  SolveComplexFunction[0] = &LoopHold::SolveComplex0;
  SolveComplexFunction[1] = &LoopHold::SolveComplex1;
  SolveComplexFunction[2] = &LoopHold::SolveComplex2;
  SolveComplexFunction[3] = &LoopHold::SolveComplex48;
  SolveComplexFunction[4] = &LoopHold::SolveComplex1;
  SolveComplexFunction[5] = &LoopHold::SolveComplex20;
  SolveComplexFunction[6] = &LoopHold::SolveComplex36;
  SolveComplexFunction[7] = &LoopHold::SolveComplex28;
  SolveComplexFunction[8] = &LoopHold::SolveComplex2;
  SolveComplexFunction[9] = &LoopHold::SolveComplex18;

  SolveComplexFunction[10] = &LoopHold::SolveComplex10;
  SolveComplexFunction[11] = &LoopHold::SolveComplex26;
  SolveComplexFunction[12] = &LoopHold::SolveComplex12;
  SolveComplexFunction[13] = &LoopHold::SolveComplex28;
  SolveComplexFunction[14] = &LoopHold::SolveComplex44;
  SolveComplexFunction[15] = &LoopHold::SolveComplex60;
  SolveComplexFunction[16] = &LoopHold::SolveComplex1;
  SolveComplexFunction[17] = &LoopHold::SolveUnused;
  SolveComplexFunction[18] = &LoopHold::SolveComplex9;
  SolveComplexFunction[19] = &LoopHold::SolveComplex49;

  SolveComplexFunction[20] = &LoopHold::SolveUnused;
  SolveComplexFunction[21] = &LoopHold::SolveUnused;
  SolveComplexFunction[22] = &LoopHold::SolveUnused;
  SolveComplexFunction[23] = &LoopHold::SolveUnused;
  SolveComplexFunction[24] = &LoopHold::SolveComplex9;
  SolveComplexFunction[25] = &LoopHold::SolveUnused;
  SolveComplexFunction[26] = &LoopHold::SolveComplex41;
  SolveComplexFunction[27] = &LoopHold::SolveUnused;
  SolveComplexFunction[28] = &LoopHold::SolveUnused;
  SolveComplexFunction[29] = &LoopHold::SolveUnused;

  SolveComplexFunction[30] = &LoopHold::SolveUnused;
  SolveComplexFunction[31] = &LoopHold::SolveUnused;
  SolveComplexFunction[32] = &LoopHold::SolveComplex2;
  SolveComplexFunction[33] = &LoopHold::SolveComplex18;
  SolveComplexFunction[34] = &LoopHold::SolveComplex10;
  SolveComplexFunction[35] = &LoopHold::SolveComplex26;
  SolveComplexFunction[36] = &LoopHold::SolveComplex6;
  SolveComplexFunction[37] = &LoopHold::SolveComplex20;
  SolveComplexFunction[38] = &LoopHold::SolveComplex14;
  SolveComplexFunction[39] = &LoopHold::SolveComplex28;

  SolveComplexFunction[40] = &LoopHold::SolveComplex10;
  SolveComplexFunction[41] = &LoopHold::SolveComplex26;
  SolveComplexFunction[42] = &LoopHold::SolveComplex2;
  SolveComplexFunction[43] = &LoopHold::SolveComplex18;
  SolveComplexFunction[44] = &LoopHold::SolveComplex14;
  SolveComplexFunction[45] = &LoopHold::SolveComplex28;
  SolveComplexFunction[46] = &LoopHold::SolveComplex6;
  SolveComplexFunction[47] = &LoopHold::SolveComplex62;
  SolveComplexFunction[48] = &LoopHold::SolveComplex3;
  SolveComplexFunction[49] = &LoopHold::SolveComplex19;

  SolveComplexFunction[50] = &LoopHold::SolveComplex11;
  SolveComplexFunction[51] = &LoopHold::SolveComplex15;
  SolveComplexFunction[52] = &LoopHold::SolveUnused;
  SolveComplexFunction[53] = &LoopHold::SolveUnused;
  SolveComplexFunction[54] = &LoopHold::SolveUnused;
  SolveComplexFunction[55] = &LoopHold::SolveUnused;
  SolveComplexFunction[56] = &LoopHold::SolveComplex11;
  SolveComplexFunction[57] = &LoopHold::SolveComplex27;
  SolveComplexFunction[58] = &LoopHold::SolveComplex43;
  SolveComplexFunction[59] = &LoopHold::SolveUnused;

  SolveComplexFunction[60] = &LoopHold::SolveUnused;
  SolveComplexFunction[61] = &LoopHold::SolveUnused;
  SolveComplexFunction[62] = &LoopHold::SolveUnused;
  SolveComplexFunction[63] = &LoopHold::SolveUnused;
}


LoopHold::~LoopHold()
{
}


bool LoopHold::CashoutAceSideBlocked(
  DefList& def,
  unsigned& rank) const
{
  // Aceholder has blocking tops that count: AK / JT9.
  const unsigned la = length[QT_ACE];
  const unsigned lp = length[QT_PARD];
  const unsigned ll = length[QT_LHO];
  const unsigned lr = length[QT_RHO];
  assert(lp > la);

  const unsigned lenMaxOpp = Max(ll, lr);
  const unsigned lenMinOpp = Min(ll, lr);
  const unsigned ld = lp - la;
  if (lenMaxOpp <= la)
  {
    // Both opponents are cashed out already by the ace side.
    // tricks = ld;
    rank = SDS_VOID-la;

    Trick btrick, rtrick;
    btrick.Set(QT_BOTH, QT_ACE, rank, la);
    rtrick.Set(QT_PARD, QT_PARD, SDS_VOID, ld);
    def.Set2(btrick, rtrick);
    return true;
  }

  unsigned nLho = Holding::TopsOverRank(QT_PARD, completeList[QT_LHO][0]);
  unsigned nRho = Holding::TopsOverRank(QT_PARD, completeList[QT_RHO][0]);

  if (nLho == 0 && nRho == 0)
    return false;

  unsigned minCashing = Min(nLho, nRho);

  bool beatWholeLho = (nLho + la >= Min(ll, lp));
  bool beatWholeRho = (nRho + la >= Min(lr, lp));
  unsigned tricks;

  if (minCashing >= ld || (beatWholeLho && beatWholeRho))
  {
    tricks = ld;
  }
  else if (! beatWholeLho && ! beatWholeRho)
  {
    if (minCashing == 0)
      return false;
    tricks = minCashing;
  }
  else if (beatWholeLho)
  {
    if (nRho == 0)
      return false;
    tricks = nRho;
  }
  else
  {
    if (nLho == 0)
      return false;
    tricks = nLho;
  }

  unsigned numPardTops = Min(lenMaxOpp-la, tricks);
  rank = Holding::ListToRank(completeList[QT_PARD][numPardTops-1]);

  Trick btrick, rtrick;
  btrick.Set(QT_BOTH, QT_ACE, SDS_VOID-la, la);
  rtrick.Set(QT_PARD, QT_PARD, rank, tricks);
  def.Set2(btrick, rtrick);
  return true;
}


void LoopHold::CashAceShort(
  DefList& def,
  unsigned& rank) const
{
  const unsigned opps = Max(length[QT_LHO], length[QT_RHO]);
  unsigned tops;

  if (opps == 0)
    tops = (length[QT_PARD] ? 1 : 0);
  else if (length[QT_PARD] <= opps || length[QT_PARD] == length[QT_ACE])
    tops = Min(opps, length[QT_ACE]);
  else if (length[QT_PARD] > length[QT_ACE])
    tops = length[QT_ACE];
  else
    tops = Min(opps+1, length[QT_ACE]);

  rank = SDS_VOID - tops;
  Trick trick;
  trick.Set(length[QT_PARD] ? QT_BOTH : QT_ACE, 
    QT_ACE, rank, length[QT_ACE]);
  def.Set1(trick);
}


void LoopHold::CashoutAce(
  DefList& def,
  unsigned& rank) const
{
  const unsigned la = length[QT_ACE];
  const unsigned ll = length[QT_LHO];
  const unsigned lr = length[QT_RHO];

  unsigned nLho = Holding::TopsOverRank(QT_ACE, completeList[QT_LHO][0]);
  unsigned nRho = Holding::TopsOverRank(QT_ACE, completeList[QT_RHO][0]);
  unsigned numCashing, tricks;

  if (nLho >= ll && nRho >= lr)
  {
    numCashing = Max(ll, lr);
    tricks = la;
  }
  else if (nLho >= ll)
  {
    numCashing = nRho;
    tricks = numCashing;
  }
  else if (nRho >= lr)
  {
    numCashing = nLho;
    tricks = numCashing;
  }
  else
  {
    numCashing = Min(nLho, nRho);
    tricks = numCashing;
  }

  rank = Holding::ListToRank(completeList[QT_ACE][numCashing-1]);

  Trick trick;
  trick.Set(QT_ACE, QT_ACE, rank, tricks);
  def.Set1(trick);
}



bool LoopHold::SolveCrashTricks(
  DefList& def,
  unsigned& rank,
  bool& flag) const
{
  // There is always only a single defense.
  // If the function returns true, the solution is of the form
  // crashing trick or blocking trick + remainder trick.
  // The crashing trick can be PA, BB or Bx.
  // The blocking trick ends on the short side.
  // The remainder trick always starts and ends opposite the end.

  // If the function returns false, the solution is either
  // Bx
  // or
  // BA or AP (depending on the cards).

  HoldingDetails hdet;
  SideDetails sdet;
  CrashDetails cr;
  LoopHold::SetGeneralDetails(hdet);
  PosType oppBest = Holding::GetOppBest();

  if ((oppBest == QT_LHO && length[QT_LHO] < length[QT_RHO]) ||
      (oppBest == QT_RHO && length[QT_RHO] < length[QT_LHO]))
  {

    LoopHold::SetSpecificDetails(hdet, sdet, true, QT_RHO);
    LoopHold::ShiftMinUp(sdet, QT_RHO);
    LoopHold::SolveCrashTricksHand(hdet, sdet, length[QT_LHO], cr);
    LoopHold::ShiftMinDown(sdet, cr);

    CrashDetails cr2;

    LoopHold::SetSpecificDetails(hdet, sdet, true, QT_LHO);
    LoopHold::ShiftMinUp(sdet, QT_LHO);
    LoopHold::SolveCrashTricksHand(hdet, sdet, length[QT_RHO], cr2);
    LoopHold::ShiftMinDown(sdet, cr2);

    LoopHold::MinCrashDetails(cr, cr2);
  }
  else
  {
    LoopHold::SetSpecificDetails(hdet, sdet, false);
    LoopHold::SolveCrashTricksHand(hdet, sdet, hdet.lenMaxOpp, cr);
  }

  // It can happen that the blocking trick from one side is
  // more demanding, while the cashing trick from the other
  // side is more demanding:  AT98 / J76 / KQ / 5432.
  // So we end up with BB49 or BB2Q+AA2T.  This is actually
  // a legitimate way to view the situation, but we probably
  // prefer to reduce this to BB49, as the number of tricks is
  // the same.

  if (cr.remTricks > 0 && 
      hdet.lenShort > 1 && 
      cr.blockTricks+cr.remTricks == cr.crashTricks)
    cr.remTricks = 0;


  if (cr.remTricks == 0)
  {
    if (cr.crashRank2 != SDS_VOID && cr.crashRank != cr.crashRank2)
    {
      assert(cr.crashRank < cr.crashRank2);

      Trick trick1, trick2;
      trick1.Set(QT_BOTH, QT_ACE, cr.crashRank2, cr.crashTricks);
      trick2.Set(QT_ACE, QT_PARD, cr.crashRank, cr.crashTricks);
      rank = Min(cr.crashRank, cr.crashRank2);
      flag = false;
      return def.Set11(trick1, trick2);
    }
    else
    {
      Trick trick;
      trick.Set(QT_BOTH, cr.crashEnd, cr.crashRank, cr.crashTricks);
      rank = cr.crashRank;
      flag = false;
      return def.Set1(trick);
    }
  }
  else
  {
    PosType blocked = SDS_PARTNER[cr.blockEnd];
    PosType bstart = (cr.blockTricks + cr.remTricks > cr.crashTricks ? 
      QT_BOTH : cr.blockEnd);

    Trick trick1, trick2a, trick2b;

    if (bstart == QT_BOTH &&
        cr.crashTricks <= cr.blockTricks)
    {
      if (Holding::GetMinDeclLength() == 1)
        trick1.Set(QT_BOTH, QT_ACE, cr.crashRank, cr.crashTricks);
      else
        trick1.Set(QT_BOTH, QT_BOTH, cr.crashRank, cr.crashTricks);
    }
    else
      trick1.Set(QT_BOTH, cr.crashEnd, cr.crashRank, cr.crashTricks);

    trick2a.Set(bstart, blocked, cr.blockRank, cr.blockTricks);
    trick2b.Set(cr.blockEnd, cr.blockEnd, cr.remRank, cr.remTricks);

    def.Set12(trick1, trick2a, trick2b);
        
    rank = Min(cr.blockRank, cr.remRank);
    rank = Min(rank, cr.crashRank);
    flag = true;
    return true;
  }
}


void LoopHold::SolveCrashTricksHand(
  const HoldingDetails& hdet,
  const SideDetails& sdet,
  const unsigned lenOpp,
  CrashDetails& cr) const
{
  // This code is extremely finicky.  It was developed with the help
  // of a big spreadsheet, and it is virtually impossible to verify
  // manually...

  // The crash trick, always present.

  if (sdet.numTopsAll >= lenOpp + 1)
    cr.crashTricks = hdet.lenLong;
  else
  {
    cr.crashTricks = sdet.numTopsLong;
    if (hdet.lenShort >= 2 && sdet.numTopsAll < hdet.declLen)
      cr.crashTricks += Min(hdet.lenShort-1, sdet.xLong);
  }

  if (hdet.lenShort == 1 || 
     (hdet.maxTopShort < sdet.minTopLong && sdet.xLong == 0))
    cr.crashEnd = hdet.pLong;
  else
    cr.crashEnd = QT_BOTH;

  unsigned ocash = Max(lenOpp, 1);
  unsigned ccash = SDS_VOID - Min(ocash, cr.crashTricks);

  unsigned cextra = ccash;
  if ((ccash <= sdet.minTopShort && sdet.xShort == 0) || 
     (hdet.lenShort >= 2 && lenOpp <= 1))
    cextra--;

  unsigned cspecial = SDS_VOID;
  if (hdet.lenLong > hdet.lenShort && 
      hdet.lenShort >= 3 &&
      lenOpp < hdet.lenShort)
    cspecial = SDS_VOID - (lenOpp + 1);

  cr.crashRank = Min(cextra, cspecial);
  if (cr.crashEnd == QT_BOTH)
  {
    unsigned minMaxTops = Min(hdet.maxTopShort, hdet.maxTopLong);
    cr.crashRank = Min(cr.crashRank, minMaxTops);
  }

  cr.crashRank2 = SDS_VOID;
  if (lenOpp == 0 && hdet.pShort >= 2 &&
    length[QT_ACE] >= length[QT_PARD])
  {
    // Special case where the cash from the ace side only 
    // requires the ace, no matter what else.
    cr.crashRank2 = SDS_VOID - 1;
  }
  else if (lenOpp == 1 && length[QT_ACE] == length[QT_PARD])
  {
    // Another special case where the ace is enough, starting
    // from partner's side.  Example AJT9 / Q / K876 / x.
    cr.crashRank2 = SDS_VOID - 1;
  }
  else if (lenOpp > 0 &&
    lenOpp < Max(hdet.numTopsLongLho, hdet.numTopsLongRho) &&
    length[QT_ACE] > length[QT_PARD])
  {
    if (length[QT_PARD] > lenOpp &&
      completeList[QT_ACE][lenOpp] > completeList[QT_PARD][0])
    {
      // AKJT / 7 / Q98 / -.
      cr.crashRank2 = SDS_VOID - (lenOpp+1);
    }
    else if (length[QT_PARD] <= lenOpp &&
      completeList[QT_ACE][lenOpp-1] > completeList[QT_PARD][0])
    {
      // AKQJ9 / - / T8 -/ 76.
      cr.crashRank2 = SDS_VOID - lenOpp;
    }
  }
  else if (length[QT_ACE] > length[QT_PARD] &&
      length[QT_PARD] >= 2 &&
      hdet.maxTopShort <= SDS_ACE - length[QT_PARD] &&
      lenOpp <= SDS_ACE - hdet.maxTopShort)
  {
    cr.crashRank2 = Holding::ListToRank(
      completeList[QT_ACE][Max(lenOpp, length[QT_PARD]) - 1]);
  }
  else if (lenOpp > 0 && 
    lenOpp < length[QT_ACE] &&
    length[QT_ACE] == length[QT_PARD] &&
    completeList[QT_ACE][lenOpp-1] > completeList[QT_PARD][0])
  {
    // AKJ / 87 / QT9 / -.
    cr.crashRank2 = SDS_VOID - lenOpp;
  }


  // The block trick, sometimes present (if not, cr.remTricks == 0).

  // To have something, in case of no block.
  cr.blockRank = SDS_VOID; 
  cr.remRank = SDS_VOID; 

  // Figure out whether there is a block trick.
  bool poss = (hdet.lenLong > 1 &&
    (hdet.declLen > sdet.numTopsAll || 
     sdet.minTopLong < hdet.maxTopShort) ?  true : false);

  bool must = ((sdet.numTopsAll <= Min(lenOpp, hdet.lenLong) ||
    (sdet.xShort == 0 && sdet.minTopShort > cr.crashRank) ) &&
    hdet.lenLong > hdet.lenShort ? true : false);

  if ((! poss) || (hdet.lenShort > 1 && ! must))
  {
    cr.blockTricks = 0;
    cr.remTricks = 0;
    return;
  }

  // So now there is a trick.

  cr.blockTricks = hdet.lenShort;
  cr.blockRank = sdet.minTopShort;
  cr.blockEnd = hdet.pLong;

  if (sdet.numTopsAll >= lenOpp)
    cr.remTricks = hdet.lenLong - sdet.numTopsShort;
  else
    cr.remTricks = Min(sdet.numTopsLong, 
      hdet.lenLong - sdet.numTopsShort);

  ocash = SDS_VOID - Min(lenOpp, hdet.lenLong);
  if (ocash >= cr.blockRank)
    cr.remRank = SDS_VOID;
  else
  {
    unsigned x = (sdet.minTopLong >= cr.blockRank ? SDS_VOID : 
      sdet.minTopLong);
    cr.remRank = Max(ocash, x);
  }
}


void LoopHold::MinCrashDetails(
  CrashDetails& cr1,
  CrashDetails& cr2) const
{
  if (cr2.crashTricks < cr1.crashTricks || 
     (cr2.crashTricks == cr1.crashTricks && cr2.crashRank < cr1.crashRank))
  {
    cr1.crashEnd = cr2.crashEnd;
    cr1.crashRank = cr2.crashRank;
    cr1.crashTricks = cr2.crashTricks;
  }

  if (cr2.crashRank2 == SDS_VOID)
    cr1.crashRank2 = SDS_VOID;
  else if (cr1.crashRank2 != SDS_VOID)
    cr1.crashRank2 = Min(cr2.crashRank2, cr1.crashRank2);

  if (cr2.remTricks > 0)
  {
    unsigned st1 = cr1.remTricks + cr1.blockTricks;
    unsigned st2 = cr2.remTricks + cr2.blockTricks;
    unsigned mr1 = Min(cr1.blockRank, cr1.remRank);
    unsigned mr2 = Min(cr2.blockRank, cr2.remRank);
    if (cr1.remTricks == 0 || st2 < st1 || (st2 == st1 && mr2 < mr1))
    {
      cr1.blockEnd = cr2.blockEnd;
      cr1.blockRank = cr2.blockRank;
      cr1.remRank = cr2.remRank;
      cr1.blockTricks = cr2.blockTricks;
      cr1.remTricks = cr2.remTricks;
    }
  }
}


bool LoopHold::CashoutBoth(
  DefList& def,
  unsigned& lowestRank) const
{
  CashoutBothDetails cb;
  if (! LoopHold::SetCashoutBothDetails(cb))
  {
    // Combination does not satisfy requirements for CashoutBoth.
    Holding::Print();
    PrintCashoutDetails(cb);
    return false;
  }

  if (cb.numTopsHigh < cb.lenCashHigh || cb.numTopsLow < cb.lenCashLow)
  {
    // Not enough tops for one or the other opponent.
    return false;
  }

  pickFlag = true;

  if (cb.lenLong == cb.lenShort)
    return LoopHold::CashoutBothSameLength(def, lowestRank, cb);
  else if (length[QT_PARD] > length[QT_ACE] && cb.minAce > cb.maxPard)
    return LoopHold::CashoutBothDiffBlocked(def, lowestRank, cb);
  else if (cb.numTopsHigh >= Min(cb.lenOppMax, cb.lenLong))
  {
    if (cb.numTopsLongHigh > 0 && cb.numTopsShortHigh > 0)
      return LoopHold::CashoutBothDiffStrong(def, lowestRank, cb);
    else
      return LoopHold::CashoutBothDiffStrongTops(def, lowestRank, cb);
  }
  else if (cb.numTopsLongHigh == 0)
    return LoopHold::CashoutBothDiffPdLongWeak(def, lowestRank, cb);
  else if (cb.numTopsShortHigh == 0)
    return LoopHold::CashoutBothDiffLongStrong(def, lowestRank, cb);
  else
    return LoopHold::CashoutBothDiffSplit(def, lowestRank, cb);
}


bool LoopHold::CashoutBothSameLength(
  DefList& def,
  unsigned& lowestRank,
  const CashoutBothDetails& cb) const
{
  Trick trick, trick2;
  unsigned r;

  assert(cb.numTopsLongHigh > 0);
  assert(cb.numTopsLongLow > 0);

  if (cb.minAce > cb.maxPard)
  {
    // AKT / - / 987 / QJ.
    if (pickFlag) holdCtr[0xa00]++;
    lowestRank = Holding::ListToRank(
      completeList[QT_ACE][cb.lenCashLow - 1]);
    trick.Set(QT_BOTH, QT_ACE, lowestRank, cb.lenLong);
    return def.Set1(trick);
  }

  PlayDetails pd;
  LoopHold::SetPlayDetails(cb.lenCashLow, cb, pd);

  if (cb.maxPard >= pd.prevPlay)
  {
    if (pickFlag) holdCtr[0xa01]++;
    lowestRank = Holding::ListToRank(pd.prevPlay);
    trick.Set(QT_BOTH, QT_BOTH, lowestRank, cb.lenLong);
    return def.Set1(trick);
  }
  else
  {
    if (pickFlag) holdCtr[0xa02]++;
    lowestRank = Holding::ListToRank(cb.maxPard);
    r = Holding::ListToRank(pd.prevPlay);
    trick.Set(QT_BOTH, QT_ACE, r, cb.lenLong);
    trick2.Set(QT_ACE, QT_PARD, lowestRank, cb.lenLong);
    return def.Set11(trick, trick2);
  }
}


bool LoopHold::CashoutBothDiffBlocked(
  DefList& def,
  unsigned& lowestRank,
  const CashoutBothDetails& cb) const
{
  Trick trick, trick2;

  if (cb.lenOppMax <= length[QT_ACE])
  {
    if (pickFlag) holdCtr[0xa10]++;
    lowestRank = Holding::ListToRank(cb.minAce);
    trick.Set(QT_BOTH, QT_ACE, lowestRank, cb.lenShort);
    trick2.Set(QT_PARD, QT_PARD, SDS_VOID, cb.lenLong - cb.lenShort);
    return def.Set2(trick, trick2);
  }
  else
  {
    if (pickFlag) holdCtr[0xa11]++;
    unsigned l = Min(cb.lenLong, cb.lenOppMax) - cb.lenShort;
    lowestRank = Holding::ListToRank(completeList[QT_PARD][l-1]);
    unsigned r = Holding::ListToRank(cb.minAce);
    trick.Set(QT_BOTH, QT_ACE, r, cb.lenShort);
    trick2.Set(QT_PARD, QT_PARD, lowestRank, cb.lenLong - cb.lenShort);
    return def.Set2(trick, trick2);
  }
}


bool LoopHold::CashoutBothDiffStrongTops(
  DefList& def,
  unsigned& lowestRank,
  const CashoutBothDetails& cb) const
{
  // assert(cb.numTopsHigh >= Min(cb.lenOppMax, cb.lenLong));
  // assert(cb.lenLong > cb.lenOppMax);
  // assert(cb.numTopsLongHigh == 0 || b.numTopsShortHigh == 0);

  Trick trick[4];
  unsigned l, r;

  if (cb.numTopsLongHigh == 0)
  {
    // Partner is longer than ace holder.
    if (pickFlag) holdCtr[0xa20]++;
    lowestRank = Holding::ListToRank(cb.maxPard);
    trick[0].Set(QT_BOTH, QT_BOTH, lowestRank, cb.lenLong);
    return def.Set1(trick[0]);
  }
  else if (cb.lenShort >= cb.lenOppMax + 2 &&
      cb.pOppHighest == QT_LHO && 
      completeList[QT_ACE][cb.lenOppHighest] < cb.maxPard &&
      completeList[QT_ACE][cb.lenOppHighest] > cb.minPard)
  {
    if (pickFlag) holdCtr[0xa21]++;
    lowestRank = Holding::ListToRank(
      completeList[QT_ACE][cb.lenOppHighest]);
    trick[0].Set(QT_BOTH, QT_BOTH, lowestRank, cb.lenLong);
    return def.Set1(trick[0]);
  }

  else if (cb.maxPard < cb.minAce || cb.lenShort <= cb.lenOppHighest ||
      (cb.lenShort <= cb.lenOppLowest && cb.maxPard < cb.minOpp))
  {
    // Declarer cannot leave the ace side, or
    // Partner is shorter than a relevant opponent.
    if (pickFlag) holdCtr[0xa22]++;
    l = cb.lenOppMax + (cb.lenShort > cb.lenOppMax ? 1 : 0);
    lowestRank = Holding::ListToRank(completeList[QT_ACE][l - 1]);
    trick[0].Set(QT_BOTH, QT_ACE, lowestRank, cb.lenLong);
    return def.Set1(trick[0]);
  }

  else if ((cb.lenShort == cb.lenOppHighest + 1 &&
      (cb.numTopsLongHigh >= cb.lenShort ||
        (completeList[QT_ACE][cb.lenOppHighest] > cb.maxPard &&
         cb.numTopsLongLow >= cb.lenOppLowest))) ||
      (cb.lenShort == cb.lenOppMax + 1 &&
        (cb.numTopsLongHigh >= cb.lenShort &&
         cb.numTopsShortLow == 0)))
  {
    // AQJT8 / K / 97 / 65.
    // AKQ843 / 9 / 765 / JT.
    // AKQT7 / J / 865 / 94.
    if (pickFlag) holdCtr[0xa23]++;
    lowestRank = Holding::ListToRank(cb.maxPard);
    l = Max(cb.lenShort, cb.lenOppLowest);
    r = Holding::ListToRank(completeList[QT_ACE][l-1]);
    trick[0].Set(QT_BOTH, QT_ACE, r, cb.lenLong);
    trick[1].Set(QT_ACE, QT_PARD, lowestRank, cb.lenShort);
    trick[2].Set(QT_ACE, QT_ACE, SDS_VOID, cb.lenLong - cb.lenShort);
    return def.Set12(trick[0], trick[1], trick[2]);
  }
  else if (cb.lenShort >= cb.lenOppMax + 2 &&
      (cb.numTopsLongHigh >= cb.lenOppMax + 1 ||
          (cb.lenOppHighest == cb.lenOppMax &&
           completeList[QT_ACE][cb.lenOppHighest] > cb.maxPard)))
  {
    // AKQTx / J / 7xxx / 98.
    if (pickFlag) holdCtr[0xa24]++;
    lowestRank = Holding::ListToRank(cb.maxPard);
    r = Holding::ListToRank(completeList[QT_ACE][cb.lenOppMax]);
    trick[0].Set(QT_BOTH, QT_ACE, r, cb.lenLong);
    trick[1].Set(QT_ACE, QT_PARD, lowestRank, cb.lenLong);
    return def.Set11(trick[0], trick[1]);
  }

  else if (cb.lenShort > cb.lenOppHighest &&
      cb.numTopsLongHigh == cb.lenOppHighest &&
      (cb.minPard > completeList[cb.pLong][cb.lenOppHighest] ||
      (cb.maxPard > completeList[cb.pLong][cb.lenOppHighest] &&
       cb.numTopsLow == cb.lenOppLowest &&
       cb.lenShort == cb.lenOppHighest + 1)))
  {
    // ATx / K / QJ / xxx.
    // AT9x / K / Qx / Jx.
    if (pickFlag) holdCtr[0xa25]++;
    lowestRank = Holding::ListToRank(
      completeList[QT_PARD][cb.lenShort - cb.lenOppHighest - 1]);

    // trick[0].Set(QT_BOTH, QT_ACE, SDS_VOID - cb.lenOppHighest, 
      // cb.lenOppHighest);
    // trick[1].Set(QT_BOTH, QT_PARD, lowestRank, 
      // cb.lenShort - cb.lenOppHighest);
    // trick[2].Set(QT_ACE, QT_ACE, SDS_VOID, cb.lenLong - cb.lenShort);
    // return def.Set3(trick[0], trick[1], trick[2]);

    trick[0].Set(QT_PARD, QT_ACE, lowestRank, cb.lenShort);
    trick[1].Set(QT_BOTH, QT_PARD, lowestRank, cb.lenShort);
    trick[2].Set(QT_ACE, QT_ACE, SDS_VOID, cb.lenLong - cb.lenShort);
    return def.Set12(trick[0], trick[1], trick[2]);
  }

  else if (cb.lenShort == cb.lenOppLowest + 1 &&
      cb.maxPard < cb.oppMaxLowest)
  {
    if (pickFlag) holdCtr[0xa26]++;
    r = Holding::ListToRank(completeList[QT_ACE][cb.lenOppLowest]);
    lowestRank = Holding::ListToRank(cb.maxPard);
    trick[0].Set(QT_BOTH, QT_ACE, r, cb.lenLong);
    trick[1].Set(QT_ACE, QT_PARD, lowestRank, cb.lenShort);
    trick[2].Set(QT_ACE, QT_ACE, SDS_VOID, cb.lenLong - cb.lenShort);
    return def.Set12(trick[0], trick[1], trick[2]);
  }
  else if (cb.lenShort > cb.lenOppHighest + 1 && 
      cb.maxPard > cb.oppMaxLowest)
  {
    l = cb.lenOppMax;
    if (cb.lenShort > cb.lenOppMax)
      l++;
    r = Holding::ListToRank(completeList[QT_ACE][l-1]);
    lowestRank = Holding::ListToRank(
      Min(cb.maxPard, completeList[QT_ACE][cb.lenOppHighest]));

    if (r > lowestRank)
    {
      if (pickFlag) holdCtr[0xa27]++;
      trick[0].Set(QT_BOTH, QT_ACE, r, cb.lenLong);
      trick[1].Set(QT_ACE, QT_PARD, lowestRank, cb.lenLong);
      return def.Set11(trick[0], trick[1]);
    }
    else
    {
      if (pickFlag) holdCtr[0xa28]++;
      trick[0].Set(QT_BOTH, QT_BOTH, lowestRank, cb.lenLong);
      return def.Set1(trick[0]);
    }
  }
  else if (cb.lenShort == cb.lenOppMax + 1 &&
      cb.lenOppHighest >= cb.lenOppLowest &&
      cb.maxPard < cb.oppMaxHighest)
  {
    if (pickFlag) holdCtr[0xa29]++;
    r = Holding::ListToRank(completeList[QT_ACE][cb.lenOppMax]);
    unsigned r2 = 
      Holding::ListToRank(completeList[QT_ACE][cb.lenOppMax-1]);
    unsigned r3 = Holding::ListToRank(cb.maxPard);
    lowestRank = Min(r, r3);

    PosType p = (cb.lenOppHighest == 1 && cb.lenShort == 2 &&
        cb.numTopsLongLow < cb.lenOppLowest ?
        QT_BOTH : QT_ACE);

    trick[0].Set(QT_BOTH, QT_ACE, r, cb.lenLong);
    trick[1].Set(p, QT_PARD, r3, cb.lenOppMax+1);
    trick[2].Set(QT_ACE, QT_ACE, SDS_VOID, cb.lenLong - cb.lenShort);
    return def.Set12(trick[0], trick[1], trick[2]);
    // trick[0].Set(QT_BOTH, QT_ACE, r, cb.lenLong);
    // trick[1].Set(QT_BOTH, QT_ACE, r2, cb.lenOppMax);
    // trick[2].Set(QT_BOTH, QT_PARD, r3, 1);
    // trick[3].Set(QT_ACE, QT_ACE, SDS_VOID, cb.lenLong - cb.lenShort);
    // return def.Set13(trick);
  }
  else if (completeList[QT_ACE][cb.lenOppMax] > cb.maxPard)
  {
    if (pickFlag) holdCtr[0xa2a]++;
    r = Holding::ListToRank(completeList[QT_ACE][cb.lenOppMax]);
    lowestRank = Holding::ListToRank(cb.maxPard);
    trick[0].Set(QT_BOTH, QT_ACE, r, length[QT_ACE]);
    trick[1].Set(QT_ACE, QT_PARD, lowestRank, length[QT_ACE]);
    return def.Set11(trick[0],trick[1]);
  }
  else
  {
    PlayDetails pd;
    l = Min(cb.lenOppMax, cb.lenLong);
    LoopHold::SetPlayDetails(l, cb, pd);

    if (l < cb.lenLong)
      pd.prevPlay = Min(pd.nextLong, pd.nextShort);

    if (pickFlag) holdCtr[0xa2b]++;
    lowestRank = Holding::ListToRank(pd.prevPlay);
    trick[0].Set(QT_BOTH, QT_BOTH, lowestRank, cb.lenLong);
    return def.Set1(trick[0]);
  }
}


bool LoopHold::CashoutBothDiffPdLongWeak(
  DefList& def,
  unsigned& lowestRank,
  const CashoutBothDetails& cb) const
{
  // assert(cb.numTopsLongHigh == 0);
  // Therefore QT_PARD is longer and has none of the high tops.
  Trick trick[3];
  unsigned r, l, m;

  if (cb.numTopsLow == cb.lenOppMax &&
      cb.lenShort > cb.lenOppMax &&
      cb.minAce > completeList[cb.pLong][cb.numTopsLongLow])
  {
    if (pickFlag) holdCtr[0xa30]++;
    if (cb.numTopsShortLow > cb.numTopsShortHigh &&
        completeList[QT_ACE][cb.numTopsShortLow-1] < cb.maxPard &&
        (completeList[QT_ACE][cb.numTopsShortLow] <
          completeList[cb.pOppLowest][1] || cb.pOppLowest == QT_LHO))
    {
      // This comparison doesn't make much sense, but that seems
      // to be what the recursion produces.  And it's not wrong.
      // l = cb.numTopsShortHigh + 1;
      // r = Holding::ListToRank(cb.maxPard);
      // m = 2;

      l = cb.numTopsLow;
      r = Holding::ListToRank(
        Min(completeList[QT_PARD][cb.numTopsLongLow-1],
        completeList[QT_ACE][cb.numTopsShortLow-1]));
      m = cb.numTopsLongLow + 1;
    }
    else
    {
      l = cb.numTopsLow;
      r = Holding::ListToRank(
        Min(completeList[QT_PARD][cb.numTopsLongLow-1],
        completeList[QT_ACE][cb.numTopsShortLow-1]));
      m = cb.numTopsLongLow + 1;
    }
    lowestRank = 
      Holding::ListToRank(completeList[QT_ACE][length[QT_ACE]-m]);
    // trick[0].Set(QT_BOTH, QT_PARD, r, l);
    // trick[1].Set(QT_BOTH, QT_ACE, lowestRank, cb.lenShort - l);
    // trick[2].Set(QT_PARD, QT_PARD, SDS_VOID, cb.lenLong - cb.lenShort);
    // return def.Set3(trick[0], trick[1], trick[2]);
    trick[0].Set(QT_ACE, QT_PARD, lowestRank, cb.lenShort);
    trick[1].Set(QT_BOTH, QT_ACE, lowestRank, cb.lenShort);
    trick[2].Set(QT_PARD, QT_PARD, SDS_VOID, cb.lenLong - cb.lenShort);
    return def.Set12(trick[0], trick[1], trick[2]);
  }
  else if (cb.numTopsLow <= cb.lenOppMax && cb.minAce > cb.minOpp)
  {
    if (cb.numTopsLow > cb.lenLong)
    {
      if (pickFlag) holdCtr[0xa31]++;
      l = Min(cb.numTopsLongLow-1, cb.lenLong - cb.lenShort);
      lowestRank = Holding::ListToRank(
        Min(completeList[QT_PARD][l], 
            completeList[QT_ACE][length[QT_ACE]-2]));
      trick[0].Set(QT_BOTH, QT_BOTH, lowestRank, cb.lenLong);
      return def.Set1(trick[0]);
    }
    else if ((length[QT_ACE] > 2 && cb.lenOppHighest == 1)|| 
        cb.numTopsLongLow > 1)
    {
      // Again, a distinction without a difference.
      if (pickFlag) holdCtr[0xa32]++;
      r = Holding::ListToRank(completeList[QT_PARD][cb.numTopsLongLow-1]);
      unsigned r1 = Min(r, Holding::ListToRank(
        completeList[QT_ACE][length[QT_ACE]-2]));
      unsigned r2 = ListToRank(cb.minAce);
      unsigned r3 = (r > r2 ? SDS_VOID : r);
      lowestRank = Min(r1, r2);
      trick[0].Set(QT_BOTH, QT_BOTH, r1, cb.numTopsLow-1);
      trick[1].Set(QT_BOTH, QT_ACE, r2, cb.lenShort);
      trick[2].Set(QT_PARD, QT_PARD, r3, cb.lenLong - cb.lenShort);
      return def.Set12(trick[0], trick[1], trick[2]);
    }
    else
    {
      if (pickFlag) holdCtr[0xa33]++;
      lowestRank = Holding::ListToRank(cb.minAce);
      r = Holding::ListToRank(cb.maxPard);
      trick[0].Set(QT_BOTH, QT_BOTH, r, cb.lenShort);
      trick[1].Set(QT_BOTH, QT_ACE, lowestRank, cb.lenShort);
      trick[2].Set(QT_PARD, QT_PARD, SDS_VOID, cb.lenLong - cb.lenShort);
      return def.Set12(trick[0], trick[1], trick[2]);
    }
  }

  PlayDetails pd;
  l = Min(cb.lenOppMax, cb.lenLong);
  LoopHold::SetPlayDetails(l, cb, pd);

  if (pd.numLong == 0)
  {
    if (pickFlag) holdCtr[0xa34]++;
    lowestRank = Holding::ListToRank(cb.maxPard);
    trick[0].Set(QT_BOTH, QT_BOTH, lowestRank, cb.lenLong);
    return def.Set1(trick[0]);
  }
  else if (pd.prevPlay == pd.prevLong)
  {
    if (cb.lenOppMax < cb.lenShort)
    {
      if (pd.nextShort > cb.oppMaxLowest)
        pd.prevPlay = Max(pd.nextShort, pd.nextLong);
      else
        pd.prevPlay = pd.nextLong;
  
      if (pickFlag) holdCtr[0xa35]++;
      lowestRank = Holding::ListToRank(pd.prevPlay);
      trick[0].Set(QT_BOTH, QT_BOTH, lowestRank, cb.lenLong);
      return def.Set1(trick[0]);
    }
    else
    {
      if (cb.minAce > cb.minOpp && cb.minAce > pd.prevLong)
        pd.prevPlay = pd.nextLong;

      if (pickFlag) holdCtr[0xa36]++;
      lowestRank = Holding::ListToRank(pd.prevPlay);
      trick[0].Set(QT_BOTH, QT_BOTH, lowestRank, cb.lenLong);
      return def.Set1(trick[0]);
    }
  }
  else if (cb.lenOppMax < cb.lenShort)
  {
    if (cb.lenShort == cb.lenOppMax + 1 &&
        pd.nextShort > Max(cb.oppMaxLowest, pd.nextLong))
      pd.prevPlay = pd.nextShort;
    else
      pd.prevPlay = pd.nextLong;
  
    if (pickFlag) holdCtr[0xa37]++;
    lowestRank = Holding::ListToRank(pd.prevPlay);
    trick[0].Set(QT_BOTH, QT_BOTH, lowestRank, cb.lenLong);
    return def.Set1(trick[0]);
  }
  else if (pd.nextLong > cb.oppMaxLowest &&
    cb.lenShort <= cb.lenOppHighest + pd.numLong)
  {
    if (cb.xShortLow == 0 && pd.prevPlay == cb.minAce)
      pd.prevPlay = pd.nextLong;

    if (pickFlag) holdCtr[0xa38]++;
    lowestRank = Holding::ListToRank(pd.prevPlay);
    trick[0].Set(QT_BOTH, QT_BOTH, lowestRank, cb.lenLong);
    return def.Set1(trick[0]);
  }
  else
  {
    if (pd.prevPlay == cb.minAce)
      pd.prevPlay = pd.nextLong;
    if (pickFlag) holdCtr[0xa39]++;
    lowestRank = Holding::ListToRank(pd.prevPlay);
    trick[0].Set(QT_BOTH, QT_BOTH, lowestRank, cb.lenLong);
    return def.Set1(trick[0]);
  }
}


unsigned LoopHold::CorrectPrevPlay(
  const CashoutBothDetails& cb,
  const PlayDetails& pd) const
{
  if (pd.numShort == 0)
  {
    if (cb.numTopsShortLow == 0)
      return Min(cb.maxPard, pd.nextLong);
    else
      return cb.maxPard;
  }
  else if (pd.prevPlay == pd.prevLong)
  {
    if (cb.lenOppMax < cb.lenShort)
    {
      if (pd.nextShort > cb.oppMaxLowest)
        return Max(pd.nextShort, pd.nextLong);
      else
        return pd.nextLong;
    }
    else if (cb.lenOppHighest + pd.numShort >= cb.lenShort)
      return pd.nextLong;
  }
  else
  {
    if (cb.lenOppMax < cb.lenShort)
    {
      if (cb.lenShort == cb.lenOppMax + 1 &&
          pd.nextShort > Max(cb.oppMaxLowest, pd.nextLong) &&
            completeList[cb.pLong][cb.lenOppHighest] > cb.maxPard)
        return pd.nextShort;
      else
        return pd.nextLong;
    }
    else if (pd.nextLong > cb.oppMaxLowest &&
      (cb.lenShort == cb.lenOppHighest + 1 ||
       cb.lenShort <= cb.lenOppHighest + pd.numShort))
      return pd.nextLong;
  }

  return pd.prevPlay;
}


bool LoopHold::CashoutBothDiffLongStrong(
  DefList& def,
  unsigned& lowestRank,
  const CashoutBothDetails& cb) const
{
  // QT_ACE is longer and has all the high tops.
  
  Trick trick[4];
  PlayDetails pd;
  unsigned r, r1, r2, r3, r4, l, t;

  if ((cb.numTopsShortLow == 0 && cb.lenOppLowest >= cb.lenShort) ||
      cb.minAce > cb.maxPard ||
     (cb.lenOppHighest >= cb.lenShort && 
      cb.numTopsLongLow >= cb.lenCashLow))
  {
    if (pickFlag) holdCtr[0xa40]++;
    l = cb.lenCashLow - 1;
    if (cb.lenShort > cb.lenOppLowest)
      l++;
    lowestRank = HR(QT_ACE, l);
    trick[0].Set(QT_BOTH, QT_ACE, lowestRank, cb.lenLong);
    return def.Set1(trick[0]);
  }

  if (cb.numTopsLongLow >= cb.lenOppLowest &&
      completeList[QT_ACE][cb.lenOppLowest-1] > cb.maxPard &&
      ((cb.lenShort > cb.lenOppLowest+1 &&
        completeList[QT_ACE][cb.lenOppLowest] > cb.maxPard) || 
       (cb.lenShort > cb.lenOppHighest+1 && 
        cb.maxPard > cb.minOpp &&
        completeList[QT_ACE][cb.lenShort-1] > cb.maxPard)))
  {
    if (pickFlag) holdCtr[0xa41]++;
    l = cb.lenOppLowest - 1;
    if (cb.lenShort > cb.lenOppLowest)
      l++;
    r = HR(QT_ACE, l);
    lowestRank = HR(QT_PARD, 0);
    trick[0].Set(QT_BOTH, QT_ACE, r, cb.lenLong);
    trick[1].Set(QT_ACE, QT_PARD, lowestRank, cb.lenLong);
    return def.Set11(trick[0], trick[1]);
  }

  if (cb.maxPard > cb.oppMaxLowest &&
      cb.lenShort > cb.lenOppHighest+1 &&
      completeList[QT_ACE][Max(cb.lenOppHighest + cb.numTopsShortLow,
        cb.lenShort > cb.lenOppHighest+2 ? cb.lenCashLow : 0) - 
          cb.numTopsShortLow] > cb.minPard &&
      cb.numTopsLow >= cb.lenCashLow +
        Max(cb.xLongLow, Min(cb.numTopsShortLow, cb.lenShort-2)) - 
          cb.xLongLow +
        Max(cb.xShortLow, cb.lenOppHighest+1) - cb.xShortLow)
  {
    // Partner beats lowest opponent.
    if (pickFlag) holdCtr[0xa42]++;
    LoopHold::SetPlayDetails(cb.lenCashLow, cb, pd);
    pd.prevPlay = LoopHold::CorrectPrevPlay(cb, pd);
    lowestRank = Holding::ListToRank(pd.prevPlay);
    trick[0].Set(QT_BOTH, QT_BOTH, lowestRank, cb.lenLong);
    return def.Set1(trick[0]);
  }
  else if (cb.maxPard < cb.oppMaxLowest &&
      cb.lenShort > cb.lenOppLowest+1 &&
      completeList[QT_ACE][cb.lenCashLow] > cb.minPard &&
      cb.numTopsLow >= cb.lenCashLow +
      Max(cb.xShortLow, cb.numTopsLongLow+1) - cb.xShortLow)
  {
    // Partner does not beat lowest opponent.
    if (pickFlag) holdCtr[0xa43]++;
    LoopHold::SetPlayDetails(cb.lenCashLow, cb, pd);
    pd.prevPlay = LoopHold::CorrectPrevPlay(cb, pd);
    lowestRank = Holding::ListToRank(pd.prevPlay);
    trick[0].Set(QT_BOTH, QT_BOTH, lowestRank, cb.lenLong);
    return def.Set1(trick[0]);
  }

  // Tricks and ranks that are useful for all remaining moves.

  unsigned t0 = 0, t1 = 0, t2 = 0, t3 = 0, s = 0,
    ra0 = SDS_VOID, ra1 = SDS_VOID, ra2 = SDS_VOID, ra3 = SDS_VOID;

  if (cb.numTopsLongLow >= cb.lenCashLow)
  {
    t0 = cb.lenLong;
    l = (cb.lenShort > cb.lenCashLow ? cb.lenCashLow : cb.lenCashLow-1);
    ra0 = HR(QT_ACE, l);
  }
  else
  {
    t0 = cb.numTopsLongLow;
    ra0 = HR(QT_ACE, t0-1);
  }

  if (cb.numTopsShortLow == 0)
    t1 = cb.lenOppLowest;
  else if (cb.numTopsLow == cb.lenOppLowest &&
      cb.lenOppLowest < cb.lenShort &&
      HR(QT_ACE, cb.numTopsLongLow-1) < 
        HR(QT_PARD, cb.numTopsShortLow-1))
  {
    s = cb.numTopsShortLow;
    t1 = cb.lenOppLowest;
  }
  else if (cb.xShortLow >= cb.numTopsLongLow)
    t1 = cb.numTopsLongLow;
  else
    t1 = cb.lenOppHighest;

  t2 = cb.lenShort - t1;

  if (cb.numTopsLow >= cb.lenCashLow + 
      Max(cb.xLongLow, t2) - cb.xLongLow +
      Max(cb.xShortLow, t1) - cb.xShortLow)
    t3 = cb.lenLong - cb.lenShort;
  else
    t3 = cb.numTopsLongLow - t1;

  ra1 = HR(QT_ACE, t1-s-1);

  ra2 = HR(QT_PARD, t2+s-1);

  if (cb.lenShort >= cb.lenOppMax)
    ra3 = SDS_VOID;
  else if (t3 == cb.lenLong - cb.lenShort)
    ra3 = HR(QT_ACE, t1-1 + cb.lenCashLow - cb.lenShort);
  else
    ra3 = HR(QT_ACE, t1+t3-1);

  if (ra3 > ra2)
    ra3 = SDS_VOID;

  // Clean up this mess?
  if (cb.numTopsLongLow < cb.lenOppLowest && 
      cb.numTopsLongLow <= cb.lenShort)
  {
    if (cb.lenShort == 2)
    {
      if (cb.lenOppHighest == 1 && cb.numTopsLongLow > 1)
      {
        // The way it comes out.  Is it right?
        if (completeList[cb.pLong][1] > completeList[cb.pShort][0])
        {
          if (pickFlag) holdCtr[0xa44]++;
          lowestRank = HR(cb.pShort, 0);
          trick[0].Set(QT_BOTH, QT_ACE, HR(cb.pLong, 1), 2);
          trick[1].Set(QT_BOTH, QT_PARD, lowestRank, 2);
          trick[2].Set(QT_ACE, QT_ACE, SDS_VOID, t3);
          return def.Set12(trick[0], trick[1], trick[2]);
        }
        else
        {
          if (pickFlag) holdCtr[0xa44]++;
          lowestRank = HR(cb.pLong, 1);
          // trick[0].Set(QT_BOTH, QT_ACE, SDS_ACE, 1);
          // trick[1].Set(QT_BOTH, QT_PARD, HR(cb.pShort, 0), 1);
          // trick[2].Set(QT_ACE, QT_ACE, lowestRank, t3);
          // return def.Set3(trick[0], trick[1], trick[2]);
          trick[0].Set(QT_PARD, QT_ACE, HR(cb.pShort, 0), 2);
          trick[1].Set(QT_BOTH, QT_PARD, HR(cb.pShort, 0), 2);
          trick[2].Set(QT_ACE, QT_ACE, lowestRank, t3);
          return def.Set12(trick[0], trick[1], trick[2]);
        }
      }
      else
      {
        if (pickFlag) holdCtr[0xa45]++;
        lowestRank = Min(ra2, ra3);
        // trick[0].Set(QT_BOTH, QT_ACE, ra1, t1);
        // trick[1].Set(QT_BOTH, QT_PARD, ra2, t2);
        // trick[2].Set(QT_ACE, QT_ACE, ra3, t3);
        // return def.Set3(trick[0], trick[1], trick[2]);

        trick[0].Set(QT_PARD, QT_ACE, ra2, t1+t2);
        trick[1].Set(QT_BOTH, QT_PARD, ra2, t1+t2);
        trick[2].Set(QT_ACE, QT_ACE, ra3, t3);
        return def.Set12(trick[0], trick[1], trick[2]);
      }
    }
    else if (cb.lenShort == 3)
    {
      if (cb.lenOppHighest == 1)
      {
        if (cb.numTopsLongLow == 1 || 
            completeList[cb.pShort][1] > completeList[cb.pLong][1])
        {
          if (pickFlag) holdCtr[0xa46]++;
          lowestRank = Min(ra2, ra3);
          // trick[0].Set(QT_BOTH, QT_ACE, ra1, t1);
          // trick[1].Set(QT_BOTH, QT_PARD, ra2, t2);
          // trick[2].Set(QT_ACE, QT_ACE, ra3, t3);
          // return def.Set3(trick[0], trick[1], trick[2]);
          trick[0].Set(QT_PARD, QT_ACE, ra2, t1+t2);
          trick[1].Set(QT_BOTH, QT_PARD, ra2, t1+t2);
          trick[2].Set(QT_ACE, QT_ACE, ra3, t3);
          return def.Set12(trick[0], trick[1], trick[2]);
        }
        else
        {
          if (pickFlag) holdCtr[0xa46]++;
          lowestRank = Min(ra2, ra3);
          unsigned ra4 = HR(cb.pLong, 1);
          unsigned ra5 = HR(cb.pShort, 0);

          trick[0].Set(QT_BOTH, QT_BOTH, Min(ra4, ra5), t1+t2);
          trick[1].Set(QT_BOTH, QT_PARD, ra2, t1+t2);
          trick[2].Set(QT_ACE, QT_ACE, ra3, t3);
          return def.Set12(trick[0], trick[1], trick[2]);
        }
      }
      else if (completeList[cb.pShort][0] > completeList[cb.pLong][2])
      {
        if (pickFlag) holdCtr[0xa47]++;
        lowestRank = Min(ra2, ra3);
        // trick[0].Set(QT_BOTH, QT_ACE, SDS_KING, 2);
        // trick[1].Set(QT_BOTH, QT_PARD, ra2, 1);
        // trick[2].Set(QT_ACE, QT_ACE, ra3, t3);
        // return def.Set3(trick[0], trick[1], trick[2]);
        trick[0].Set(QT_PARD, QT_ACE, ra2, 3);
        trick[1].Set(QT_BOTH, QT_PARD, ra2, 3);
        trick[2].Set(QT_ACE, QT_ACE, ra3, t3);
        return def.Set12(trick[0], trick[1], trick[2]);
      }
      else
      {
        if (pickFlag) holdCtr[0xa48]++;
        lowestRank = Min(ra2, ra3);
        unsigned rr = HR(QT_ACE, 2);
        PosType st = (rr > ra2 ? QT_BOTH : QT_PARD);
        trick[0].Set(st, QT_ACE, rr, 3);
        trick[1].Set(QT_BOTH, QT_PARD, ra2, 3);
        trick[2].Set(QT_ACE, QT_ACE, ra3, t3);
        return def.Set12(trick[0], trick[1], trick[2]);
      }
    }
    else
    {
      if (pickFlag) holdCtr[0xa49]++;

  if (// cb.numTopsLongLow > cb.numTopsLongHigh ||
      cb.numTopsLongLow > cb.lenOppHighest &&
      completeList[cb.pLong][cb.lenOppHighest] >
      completeList[cb.pShort][cb.lenShort-1])
  {
    // Maybe this is really only one condition?
  t1 = cb.lenOppLowest;
  unsigned r1ab = HR(QT_PARD, cb.numTopsShortLow-1);
  if (r1ab < ra1)
    ra1 = r1ab;
  t2 = cb.lenShort - t1;
  }

      lowestRank = Min(ra2, ra3);
      // trick[0].Set(QT_BOTH, QT_ACE, ra1, t1);
      // trick[1].Set(QT_BOTH, QT_PARD, ra2, t2);
      // trick[2].Set(QT_ACE, QT_ACE, ra3, t3);
      // return def.Set3(trick[0], trick[1], trick[2]);
      trick[0].Set(QT_PARD, QT_ACE, ra2, t1+t2);
      trick[1].Set(QT_BOTH, QT_PARD, ra2, t1+t2);
      trick[2].Set(QT_ACE, QT_ACE, ra3, t3);
      return def.Set12(trick[0], trick[1], trick[2]);
    }
  }
  else if (Holding::TopsOverRank(QT_ACE, cb.minPard) < cb.lenShort)
  {
    if (pickFlag) holdCtr[0xa44]++;
    lowestRank = Min(ra2, ra3);
    // trick[0].Set(QT_BOTH, QT_ACE, ra1, t1);
    // trick[1].Set(QT_BOTH, QT_PARD, ra2, t2);
    // trick[2].Set(QT_ACE, QT_ACE, ra3, t3);
    // return def.Set3(trick[0], trick[1], trick[2]);

    trick[0].Set(QT_PARD, QT_ACE, ra2, t1+t2);
    trick[1].Set(QT_BOTH, QT_PARD, ra2, t1+t2);
    trick[2].Set(QT_ACE, QT_ACE, ra3, t3);
    return def.Set12(trick[0], trick[1], trick[2]);
  }

  if (cb.numTopsLongLow >= cb.lenCashLow &&
      cb.maxPard <= 
        completeList[QT_ACE][Min(cb.lenCashLow, cb.lenShort-1)])
  {
    // The split between 1+2 and 1+3 moves has no good reason.
    // It just comes out of the recursion that way.
    if (pickFlag) holdCtr[0xa4a]++;
    lowestRank = Min(ra0, Min(ra2, ra3));
    trick[0].Set(QT_BOTH, QT_ACE, ra0, t0);
    trick[1].Set(QT_ACE, QT_PARD, ra2, t1+t2);
    trick[2].Set(QT_ACE, QT_ACE, ra3, t3);
    return def.Set12(trick[0], trick[1], trick[2]);
  }
  else
  {
    if (pickFlag) holdCtr[0xa4b]++;
    lowestRank = Min(Min(ra0, ra1), Min(ra2, ra3));
    // The way it comes out for now -- wrong.
    PosType p = (cb.lenOppHighest == 1 && cb.lenShort == 2 &&
        cb.numTopsLongLow < cb.lenCashLow ?
      QT_BOTH : QT_ACE);
    trick[0].Set(QT_BOTH, QT_ACE, ra0, t0);
    trick[1].Set(p, QT_PARD, ra2, t1+t2);
    trick[2].Set(QT_ACE, QT_ACE, ra3, t3);
    return def.Set12(trick[0], trick[1], trick[2]);
    // trick[0].Set(QT_BOTH, QT_ACE, ra0, t0);
    // trick[1].Set(QT_BOTH, QT_ACE, ra1, t1);
    // trick[2].Set(QT_BOTH, QT_PARD, ra2, t2);
    // trick[3].Set(QT_ACE, QT_ACE, ra3, t3);
    // return def.Set13(trick);
  }
}


bool LoopHold::CashoutBothDiffStrong(
  DefList& def,
  unsigned& lowestRank,
  const CashoutBothDetails& cb) const
{
  Trick trick[3];
  PlayDetails pd;
  unsigned l;
  LoopHold::SetPlayDetails(cb.lenCashLow, cb, pd);

  if (pd.nextLong > pd.nextShort)
  {
    if (cb.lenShort > cb.lenOppMax)
      pd.prevPlay = pd.nextLong;
  
    if (pickFlag) holdCtr[0xa50]++;
    lowestRank = Holding::ListToRank(pd.prevPlay);
    trick[0].Set(QT_BOTH, QT_BOTH, lowestRank, cb.lenLong);
    return def.Set1(trick[0]);
  }
  else if (cb.numTopsHigh > cb.lenOppHighest ||
      completeList[cb.pShort][cb.lenShort-1] < 
        completeList[cb.pLong][cb.numTopsLongHigh] ||
      (cb.numTopsHigh >= cb.lenOppMax &&
      cb.lenShort <= cb.lenOppMax))
  {
    if (cb.numTopsHigh == cb.lenOppMax)
    {
      if (cb.lenShort > cb.lenOppMax)
      {
        if (cb.numTopsShortHigh >= cb.lenOppHighest)
          pd.prevPlay = Max(pd.nextLong, pd.nextShort);
        else
          pd.prevPlay = pd.nextLong;
      }
    }
    else if (cb.numTopsHigh > cb.lenOppMax)
      pd.prevPlay = Max(pd.nextLong, pd.nextShort);

    if (pickFlag) holdCtr[0xa51]++;
    lowestRank = Holding::ListToRank(pd.prevPlay);
    trick[0].Set(QT_BOTH, QT_BOTH, lowestRank, cb.lenLong);
    return def.Set1(trick[0]);
  }

  // This comes out of the recursion a bit arbitrarily.
  if (pickFlag) holdCtr[0xa52]++;
  if (cb.pLong == QT_ACE && cb.numTopsLongHigh == 1)
    // l = cb.numTopsLongHigh;
    l = cb.numTopsHigh;
  else 
  if (cb.pLong == QT_ACE && completeList[QT_ACE][1] > cb.maxPard)
    // l = cb.numTopsLongHigh;
    l = cb.numTopsHigh;
  else if (cb.pLong == QT_PARD && 
      cb.numTopsShortHigh >= 2 &&
      completeList[QT_ACE][1] < cb.maxPard)
    // l = cb.numTopsShortHigh;
    l = cb.numTopsHigh;
  else
    l = cb.numTopsHigh;

  // unsigned r1 = HR(cb.pLong, cb.numTopsLongHigh-1);
  unsigned r1a = HR(cb.pLong, cb.numTopsLongHigh-1);
  unsigned r1b = HR(cb.pShort, cb.numTopsShortHigh-1);
  unsigned r1 = Min(r1a, r1b);
  unsigned r2 = HR(cb.pShort, cb.lenShort-1 - cb.numTopsLongHigh);
  lowestRank = Min(r1, r2);

  // trick[0].Set(QT_BOTH, cb.pLong, r1, l);
  // trick[1].Set(QT_BOTH, cb.pShort, r2, cb.lenShort - l);
  // trick[2].Set(cb.pLong, cb.pLong, SDS_VOID, cb.lenLong - cb.lenShort);
  // return def.Set3(trick[0], trick[1], trick[2]);
  trick[0].Set(cb.pShort, cb.pLong, r2, cb.lenShort);
  trick[1].Set(QT_BOTH, cb.pShort, r2, cb.lenShort);
  trick[2].Set(cb.pLong, cb.pLong, SDS_VOID, cb.lenLong - cb.lenShort);
  return def.Set12(trick[0], trick[1], trick[2]);
}


bool LoopHold::CashoutBothDiffSplit(
  DefList& def,
  unsigned& lowestRank,
  const CashoutBothDetails& cb) const
{
  Trick trick[4];

  if (cb.numTopsShortLow == cb.lenShort &&
    cb.numTopsLow == Min(cb.lenLong, cb.lenOppLowest))
  {
    if (pickFlag) holdCtr[0xa80]++;
    unsigned r1 = HR(cb.pLong, cb.numTopsLongLow-1);
    unsigned r2 = HR(cb.pShort, cb.numTopsShortLow-2);
    unsigned rm = Min(r1, r2);
    unsigned r3 = HR(cb.pShort, cb.lenShort-1);
    PosType e, s, p;

    if (cb.pShort == QT_ACE)
    {
      s = QT_ACE;
      p = QT_RHO;
    }
    else
    {
      s = QT_PARD;
      p = QT_LHO;
    }

    // The way it comes out.
    if (length[p] >= length[s] && 
        completeList[s][length[s]-2] > completeList[p][0] &&
        completeList[s][length[s]-1] < completeList[p][0])
      e = cb.pLong;
    else
      e = QT_BOTH;

    unsigned re = (cb.lenOppLowest > cb.lenShort && r1 < r3 ?
      r1 : SDS_VOID);
    lowestRank = Min(rm, r3);

    trick[0].Set(QT_BOTH, QT_BOTH, rm, cb.numTopsLow-1);
    trick[1].Set(e, cb.pShort, r3, cb.lenShort);
    trick[2].Set(cb.pLong, cb.pLong, re, cb.lenLong - cb.lenShort);
    return def.Set12(trick[0], trick[1], trick[2]);
  }


  unsigned delta = cb.numTopsLow - cb.lenCashLow;
  unsigned delta1 = (cb.xLongLow >= cb.numTopsShortLow-1 ? 0 :
    cb.numTopsShortLow-1 - cb.xLongLow);
  bool BBflag;
  if (delta1 > delta)
    BBflag = false;
  else if (cb.lenLong == 5 && cb.lenShort == 4 &&
      cb.lenOppHighest == 1 && cb.lenOppLowest == 3 &&
      cb.numTopsHigh == 2 &&
      cb.numTopsLow == 3 &&
      completeList[cb.pShort][3] > 
        completeList[cb.pLong][cb.numTopsLongLow])
    BBflag = false;
  else if (cb.numTopsShortHigh + cb.numTopsLongLow >= cb.lenCashLow)
  {
    if (cb.lenShort == 3 && 
        cb.lenOppHighest == 2 && 
        cb.numTopsHigh == 2 &&
        completeList[cb.pShort][2] > completeList[cb.pLong][1])
      BBflag = false;
    else
      BBflag = true;
  }
  else if (cb.lenOppHighest == 2 &&
      cb.lenShort > 2 && 
      cb.maxOpp > completeList[cb.pShort][1])
    BBflag = false;
  else
    BBflag = true;

  if (BBflag)
  {
    PlayDetails pd;
    unsigned l = Min(cb.lenOppMax, cb.lenLong);
    LoopHold::SetPlayDetails(l, cb, pd);

    if (cb.numTopsShortLow == cb.lenShort &&
        cb.lenShort <= cb.lenOppMax &&
        pd.prevPlay == pd.prevLong && 
        completeList[cb.pShort][cb.lenShort-1] > pd.prevLong) 
    {
      pd.prevPlay = pd.nextLong;
    }
    else if (cb.numTopsShortLow == cb.lenShort &&
        cb.lenShort <= cb.lenOppMax &&
        pd.prevPlay == pd.prevShort && 
        pd.numShort == cb.lenShort)
    {
      pd.prevPlay = pd.nextLong;
    }
    else if (l < cb.lenShort)
    {
      if (pd.nextShort > cb.minOpp)
        pd.prevPlay = Max(pd.nextLong, pd.nextShort);
      else
        pd.prevPlay = pd.nextLong;
    }
    else if (cb.numTopsHigh == cb.lenOppHighest &&
        pd.prevPlay == pd.prevShort && 
        cb.numTopsShortLow + pd.numLong >= cb.lenShort)
    {
      pd.prevPlay = pd.nextLong;
    }
    else if (cb.lenShort == cb.lenOppHighest + 1 &&
      cb.numTopsShortHigh < cb.lenOppHighest &&
        pd.prevPlay == pd.prevShort)
    {
      pd.prevPlay = pd.nextLong;
    }
    else if (cb.lenLong == 4 && cb.lenShort == 3 &&
        cb.lenOppHighest == 2 && cb.lenOppLowest == 4 &&
        cb.numTopsHigh == 2 &&
        completeList[cb.pShort][1] > completeList[cb.pLong][1])
    {
      pd.prevPlay = pd.nextLong;
    }
  
    if (pickFlag) holdCtr[0xa81]++;
    lowestRank = Holding::ListToRank(pd.prevPlay);
    trick[0].Set(QT_BOTH, QT_BOTH, lowestRank, cb.lenLong);
    return def.Set1(trick[0]);
  }


  unsigned t0 = 0, t1 = 0, r0 = SDS_VOID, r1 = SDS_VOID, r2 = SDS_VOID;
  if (cb.lenShort == 3 && cb.lenLong == 4 &&
    cb.lenOppHighest == 2 && cb.lenOppLowest == 4)
  {
    // The way it comes out.
    r0 = HR(cb.pLong, 0);
    t0 = (cb.pLong == QT_PARD ? 2 : 1);
    r1 = HR(cb.pShort, 1);
  }
  else if (cb.lenShort == 4 && cb.lenLong == 5 &&
    cb.lenOppHighest == 1 && cb.lenOppLowest == 3)
  {
    // The way it comes out.
    if (cb.numTopsLongLow > 1 ||
       (cb.pShort == QT_ACE &&
        cb.pOppHighest == QT_LHO && 
        completeList[QT_ACE][2] > completeList[QT_RHO][1]))
    {
      t0 = 3;
      r0 = SDS_JACK;
    }
    else if (cb.pShort == QT_ACE)
    {
      t0 = 2;
      r0 = SDS_KING;
    }
    else
    {
      t0 = 1;
      r0 = SDS_ACE;
    }
    r1 = HR(cb.pShort, cb.numTopsShortLow);
  }
  else
  {
    // The way it comes out.
    if (cb.pLong == QT_PARD)
    {
      t0 = cb.numTopsHigh;
      r0 = HR(cb.pLong, 0);
    }
    else
    {
      t0 = cb.numTopsLongHigh;
      r0 = HR(cb.pLong, t0-1);
    }
    r1 = HR(cb.pShort, cb.numTopsShortHigh);
  }

  t1 = cb.lenShort - t0;
  if (cb.lenOppLowest <= cb.lenShort)
    r2 = SDS_VOID;
  else if (cb.lenLong == 4 && cb.lenShort == 3 &&
      cb.lenOppHighest == 2 && cb.lenOppLowest == 4 &&
      cb.numTopsHigh == 3 && cb.numTopsShortLow >= 2)
    r2 = SDS_VOID;
  else
    r2 = HR(cb.pLong, cb.numTopsLongHigh);
  if (r2 > r1)
    r2 = SDS_VOID;

  if (pickFlag) holdCtr[0xa82]++;
  lowestRank = Min(r1, r2);

  // Need to clean up these branches some more.
  if (cb.numTopsHigh == cb.lenOppHighest)
  {
    if (cb.numTopsHigh == 2 &&
        completeList[cb.pLong][1] > completeList[cb.pShort][1])
    {
      trick[0].Set(QT_BOTH, QT_BOTH, HR(cb.pLong, 1), cb.lenShort);
      trick[1].Set(QT_BOTH, cb.pShort, r1, cb.lenShort);
      trick[2].Set(cb.pLong, cb.pLong, SDS_VOID, cb.lenLong - cb.lenShort);
      return def.Set12(trick[0], trick[1], trick[2]);
    }
    else
    {
  t0 = cb.numTopsHigh;
  t1 = cb.lenShort - t0;
  r0 = Min(HR(cb.pLong, cb.numTopsLongHigh-1),
      HR(cb.pShort, cb.numTopsShortHigh-1));

      // trick[0].Set(QT_BOTH, cb.pLong, r0, t0);
      // trick[1].Set(QT_BOTH, cb.pShort, r1, t1);
      // trick[2].Set(cb.pLong, cb.pLong, r2, cb.lenLong - cb.lenShort);
      // return def.Set3(trick[0], trick[1], trick[2]);
      trick[0].Set(cb.pShort, cb.pLong, r1, t0+t1);
      trick[1].Set(QT_BOTH, cb.pShort, r1, t0+t1);
      trick[2].Set(cb.pLong, cb.pLong, r2, cb.lenLong - cb.lenShort);
      return def.Set12(trick[0], trick[1], trick[2]);
    }
  }
  else if (cb.numTopsLow == cb.lenOppLowest)
  {
    if (cb.numTopsHigh == 2)
    {
  t0 = cb.numTopsLow;
  t1 = cb.lenShort - t0;
  r0 = Min(HR(cb.pLong, cb.numTopsLongLow-1),
      HR(cb.pShort, cb.numTopsShortLow-1));
      // trick[0].Set(QT_BOTH, cb.pLong, r0, t0);
      // trick[1].Set(QT_BOTH, cb.pShort, r1, t1);
      // trick[2].Set(cb.pLong, cb.pLong, r2, cb.lenLong - cb.lenShort);
      // return def.Set3(trick[0], trick[1], trick[2]);
      trick[0].Set(cb.pShort, cb.pLong, r1, t0+t1);
      trick[1].Set(QT_BOTH, cb.pShort, r1, t0+t1);
      trick[2].Set(cb.pLong, cb.pLong, r2, cb.lenLong - cb.lenShort);
      return def.Set12(trick[0], trick[1], trick[2]);
    }
    else
    {
      unsigned ra1 = HR(cb.pLong, cb.numTopsLongHigh-1);
      unsigned ra2 = HR(cb.pShort, cb.numTopsShortHigh-1);
      unsigned ra = Min(ra1, ra2);
      trick[0].Set(QT_BOTH, QT_BOTH, ra, cb.lenShort);
      trick[1].Set(QT_BOTH, cb.pShort, r1, cb.lenShort);
      trick[2].Set(cb.pLong, cb.pLong, SDS_VOID, cb.lenLong - cb.lenShort);
      return def.Set12(trick[0], trick[1], trick[2]);
    }
  }
  else
  {
    unsigned ra1 = HR(cb.pLong, cb.numTopsLongHigh-1);
    unsigned ra2 = HR(cb.pShort, cb.numTopsShortHigh-1);
    unsigned ra = Min(ra1, ra2);
    trick[0].Set(QT_BOTH, QT_BOTH, ra, cb.lenShort);
    trick[1].Set(QT_BOTH, cb.pShort, r1, cb.lenShort);
    trick[2].Set(cb.pLong, cb.pLong, SDS_VOID, cb.lenLong - cb.lenShort);
    return def.Set12(trick[0], trick[1], trick[2]);
  }
}


void LoopHold::SetPlayDetails(
  const unsigned len,
  const CashoutBothDetails &cb,
  PlayDetails &pd) const
{
  pd.prevPlay = 0;
  pd.numLong = 0;
  pd.numShort = 0;

  for (unsigned no = 0; no < len; no++)
  {
    if (completeList[cb.pLong][pd.numLong] > 
        completeList[cb.pShort][pd.numShort])
      pd.prevPlay = completeList[cb.pLong][pd.numLong++];
    else
      pd.prevPlay = completeList[cb.pShort][pd.numShort++];
  }

  pd.nextLong = completeList[cb.pLong][pd.numLong];
  pd.nextShort = completeList[cb.pShort][pd.numShort];
  pd.prevLong = (pd.numLong > 0 ? 
    completeList[cb.pLong][pd.numLong-1] : suitLength);
  pd.prevShort = (pd.numShort > 0 ?
    completeList[cb.pShort][pd.numShort-1] : suitLength);
}


bool LoopHold::SetCashoutBothDetails(
  CashoutBothDetails& cb) const
{
  HoldingDetails hdet;
  LoopHold::SetGeneralDetails(hdet);

  cb.pLong = hdet.pLong;
  cb.pShort = hdet.pShort;
  cb.lenLong = length[cb.pLong];
  cb.lenShort = length[cb.pShort];
  if (cb.lenLong == 0 || cb.lenShort == 0)
    return false;

  LoopHold::GetOppLengths(cb);
  if (cb.lenOppHighest == 0 && cb.lenOppLowest == 0)
    return false;

  cb.lenOppMax = Max(cb.lenOppHighest, cb.lenOppLowest);

  cb.lenCashHigh = Min(cb.lenLong, cb.lenOppHighest);
  if (cb.lenOppLowest == 0)
    cb.lenCashLow = Min(cb.lenLong, cb.lenOppHighest);
  else
    cb.lenCashLow = Min(cb.lenLong, cb.lenOppLowest);

  cb.minAce = completeList[QT_ACE][length[QT_ACE]-1];
  cb.maxPard = completeList[QT_PARD][0];
  if (length[QT_PARD] == 0)
    cb.minPard = cb.maxPard;
  else
    cb.minPard = completeList[QT_PARD][length[QT_PARD]-1];

  // Might be SDS_VOID?
  cb.maxOpp = Max(completeList[QT_RHO][0], completeList[QT_LHO][0]);

  // Should be Min(oppMaxHighest, oppMaxLowest)?
  if (length[QT_LHO] == 0)
    cb.minOpp = completeList[QT_RHO][0];
  else if (length[QT_RHO] == 0)
    cb.minOpp = completeList[QT_LHO][0];
  else
    cb.minOpp = Min(completeList[QT_LHO][0], completeList[QT_RHO][0]);

  // Might these be SDS_VOID?
  cb.oppMaxHighest = completeList[cb.pOppHighest][0];
  cb.oppMaxLowest = completeList[cb.pOppLowest][0];

  if (cb.pOppHighest == QT_LHO)
  {
    cb.numTopsLongHigh = hdet.numTopsLongLho;
    cb.numTopsLongLow = hdet.numTopsLongRho;
    cb.numTopsShortHigh = hdet.numTopsShortLho;
    cb.numTopsShortLow = hdet.numTopsShortRho;
  }
  else
  {
    cb.numTopsLongHigh = hdet.numTopsLongRho;
    cb.numTopsLongLow = hdet.numTopsLongLho;
    cb.numTopsShortHigh = hdet.numTopsShortRho;
    cb.numTopsShortLow = hdet.numTopsShortLho;
  }

  cb.numTopsHigh = cb.numTopsLongHigh + cb.numTopsShortHigh;
  cb.numTopsLow = cb.numTopsLongLow + cb.numTopsShortLow;

  cb.xLongHigh = cb.lenLong - cb.numTopsLongHigh;
  cb.xShortHigh = cb.lenShort - cb.numTopsShortHigh;
  if (cb.xLongHigh == 0 || cb.xShortHigh == 0)
    return false;

  cb.xLongLow = cb.lenLong - cb.numTopsLongLow;
  cb.xShortLow = cb.lenShort - cb.numTopsShortLow;

  return true;
}


void LoopHold::GetOppLengths(
  CashoutBothDetails& cb) const
{
  if (length[QT_LHO] == 0)
  {
    cb.pOppHighest = QT_RHO;
    cb.pOppLowest = QT_LHO;
    cb.lenOppHighest = length[QT_RHO];
    cb.lenOppLowest = 0;
  }
  else if (length[QT_RHO] == 0)
  {
    cb.pOppHighest = QT_LHO;
    cb.pOppLowest = QT_RHO;
    cb.lenOppHighest = length[QT_LHO];
    cb.lenOppLowest = 0;
  }
  else
  {
    PosType bestOpp = Holding::GetOppBest();

    if (bestOpp == QT_BOTH)
    {
      // Really the same rank: Ignore the shorter opponent.
      if (length[QT_LHO] >= length[QT_RHO])
      {
        cb.pOppHighest = QT_LHO;
        cb.pOppLowest = QT_RHO;
        cb.lenOppHighest = length[QT_LHO];
        cb.lenOppLowest = 0;
      }
      else
      {
        cb.pOppHighest = QT_RHO;
        cb.pOppLowest = QT_LHO;
        cb.lenOppHighest = length[QT_RHO];
        cb.lenOppLowest = 0;
      }
    }
    else if (bestOpp == QT_LHO)
    {
      if (length[QT_RHO] <= length[QT_LHO])
      {
        // Worse suit is also shorter.
        cb.pOppHighest = QT_LHO;
        cb.pOppLowest = QT_RHO;
        cb.lenOppHighest = length[QT_LHO];
        cb.lenOppLowest = 0;
      }
      else
      {
        cb.pOppHighest = QT_LHO;
        cb.pOppLowest = QT_RHO;
        cb.lenOppHighest = length[QT_LHO];
        cb.lenOppLowest = length[QT_RHO];
      }
    }
    else if (length[QT_LHO] <= length[QT_RHO])
    {
      // Worse suit is also shorter.
      cb.pOppHighest = QT_RHO;
      cb.pOppLowest = QT_LHO;
      cb.lenOppHighest = length[QT_RHO];
      cb.lenOppLowest = 0;
    }
    else
    {
      cb.pOppHighest = QT_RHO;
      cb.pOppLowest = QT_LHO;
      cb.lenOppHighest = length[QT_RHO];
      cb.lenOppLowest = length[QT_LHO];
    }
  }
}


void LoopHold::PrintCashoutDetails(
  const CashoutBothDetails& cb) const
{
  cout << left;
  cout << setw(18) << "lenLong" << cb.lenLong << "\n"; 
  cout << setw(18) << "lenShort" << cb.lenShort << "\n"; 
  cout << setw(18) << "pLong" << POS_NAMES[cb.pLong] << "\n"; 
  cout << setw(18) << "pShort" << POS_NAMES[cb.pShort] << "\n"; 
  cout << setw(18) << "pOppHighest" << POS_NAMES[cb.pOppHighest] << "\n"; 
  cout << setw(18) << "pOppLowest" << POS_NAMES[cb.pOppLowest] << "\n"; 
  cout << setw(18) << "lenOppHighest" << cb.lenOppHighest << "\n"; 
  cout << setw(18) << "lenOppLowest" << cb.lenOppLowest << "\n"; 
  cout << setw(18) << "lenOppMax" << cb.lenOppMax << "\n"; 
  cout << setw(18) << "lenCashHigh" << cb.lenCashHigh << "\n"; 
  cout << setw(18) << "lenCashLow" << cb.lenCashLow << "\n"; 
  cout << setw(18) << "minAce" << cb.minAce << "\n"; 
  cout << setw(18) << "maxPard" << cb.maxPard << "\n"; 
  cout << setw(18) << "minPard" << cb.minPard << "\n"; 
  cout << setw(18) << "maxOpp" << cb.maxOpp << "\n"; 
  cout << setw(18) << "minOpp" << cb.minOpp << "\n"; 
  cout << setw(18) << "oppMaxHighest" << cb.oppMaxHighest << "\n"; 
  cout << setw(18) << "oppMaxLowest" << cb.oppMaxLowest << "\n"; 
  cout << setw(18) << "numTopsLongHigh" << cb.numTopsLongHigh << "\n"; 
  cout << setw(18) << "numTopsLongLow" << cb.numTopsLongLow << "\n"; 
  cout << setw(18) << "numTopsShortHigh" << cb.numTopsShortHigh << "\n"; 
  cout << setw(18) << "numTopsShortLow" << cb.numTopsShortLow << "\n"; 
  cout << setw(18) << "numTopsHigh" << cb.numTopsHigh << "\n"; 
  cout << setw(18) << "numTopsLow" << cb.numTopsLow << "\n"; 
  cout << setw(18) << "xLongHigh" << cb.xLongHigh << "\n"; 
  cout << setw(18) << "xShortHigh" << cb.xShortHigh << "\n"; 
}


void LoopHold::SetGeneralDetails(
  HoldingDetails& hdet) const
{
  hdet.declLen = length[QT_ACE] + length[QT_PARD];

  if (length[QT_ACE] >= length[QT_PARD])
  {
    hdet.pLong = QT_ACE;
    hdet.pShort = QT_PARD;
  }
  else
  {
    hdet.pLong = QT_PARD;
    hdet.pShort = QT_ACE;
  }

  hdet.lenLong = length[hdet.pLong];
  hdet.lenShort = length[hdet.pShort];
  hdet.lenMaxOpp = Max(length[QT_LHO], length[QT_RHO]);

  if (length[QT_LHO] == 0)
  {
    hdet.numTopsLongLho = hdet.lenLong;
    hdet.numTopsShortLho = hdet.lenShort;
  }
  else
  {
    hdet.numTopsLongLho = Holding::TopsOverRank(hdet.pLong, 
      completeList[QT_LHO][0]);
    hdet.numTopsShortLho = Holding::TopsOverRank(hdet.pShort, 
      completeList[QT_LHO][0]);
  }

  if (length[QT_RHO] == 0)
  {
    hdet.numTopsLongRho = hdet.lenLong;
    hdet.numTopsShortRho = hdet.lenShort;
  }
  else
  {
    hdet.numTopsLongRho = Holding::TopsOverRank(hdet.pLong, 
      completeList[QT_RHO][0]);
    hdet.numTopsShortRho = Holding::TopsOverRank(hdet.pShort, 
      completeList[QT_RHO][0]);
  }

  hdet.maxTopLong = HR(hdet.pLong, 0);
  hdet.maxTopShort = HR(hdet.pShort, 0);

}


void LoopHold::SetSpecificDetails(
  const HoldingDetails& hdet,
  SideDetails& sdet,
  const bool oppSkippedFlag,
  const PosType oppSkipped) const
{
  if (oppSkippedFlag)
  {
    if (oppSkipped == QT_LHO)
    {
      sdet.numTopsLong = hdet.numTopsLongRho;
      sdet.numTopsShort = hdet.numTopsShortRho;
    }
    else
    {
      sdet.numTopsLong = hdet.numTopsLongLho;
      sdet.numTopsShort = hdet.numTopsShortLho;
    }
  }
  else
  {
    sdet.numTopsLong = Min(hdet.numTopsLongLho, hdet.numTopsLongRho);
    sdet.numTopsShort = Min(hdet.numTopsShortLho, hdet.numTopsShortRho);
  }

  // If oppSkippedFlag is set, these two may be modified
  // in ShiftMinUp().
  sdet.minTopLong = HR(hdet.pLong, sdet.numTopsLong-1);
  sdet.minTopShort = HR(hdet.pShort, sdet.numTopsShort-1);

  assert(hdet.lenLong >= sdet.numTopsLong);
  assert(hdet.lenShort >= sdet.numTopsShort);

  sdet.xLong = hdet.lenLong - sdet.numTopsLong;
  sdet.xShort = hdet.lenShort - sdet.numTopsShort;
  sdet.numTopsAll = sdet.numTopsLong + sdet.numTopsShort;
}


void LoopHold::PrintHoldingDetails(
  const HoldingDetails& hdet) const
{
  cout << "Long player " << static_cast<int>(hdet.pLong) << 
    ", short " << static_cast<int>(hdet.pShort) << "\n";

  cout << "Lengths long " << hdet.lenLong << 
    ", short " << hdet.lenShort << 
    ", maxOpp " << hdet.lenMaxOpp << "\n";

  cout << "Tops long " << hdet.numTopsLongLho << "-" <<
    hdet.numTopsLongRho << ", short " <<
    hdet.numTopsShortLho << "-" <<
    hdet.numTopsShortRho << "\n";

  cout << "Long " << hdet.maxTopLong << 
    ", short " << hdet.maxTopShort << "\n";
}


void LoopHold::ShiftMinUp(
  SideDetails& sdet,
  const PosType oppSkipped) const
{
  // Compensate for ranks with skipped opponent.
  // This is a bit of a kludge.

  unsigned mapRealToShifted[SDS_MAX_RANKS] = {0};
  for (int k = 0; k < SDS_MAX_RANKS; k++)
    sdet.mapShiftedToReal[k] = 0;

  // Compensate for skipped ranks with other opponent.
  // This is a bit of a kludge -- maybe there is a better way.

  int used[SDS_MAX_RANKS] = {0};
  unsigned m = Min(sdet.minTopLong, sdet.minTopShort);
  for (unsigned j = 0; j < length[oppSkipped]; j++)
  {
    unsigned x = HR(oppSkipped, j);
    if (x <= m)
      break;
    else
      used[x] = 1;
  }

  unsigned i = SDS_VOID;
  unsigned c = SDS_VOID;
  do
  {
    mapRealToShifted[i] = c;
    sdet.mapShiftedToReal[c] = i;
    c--;
    i--;
    while (used[i])
      i--;
  }
  while (i >= m);

  sdet.minTopLong = mapRealToShifted[sdet.minTopLong];
  sdet.minTopShort = mapRealToShifted[sdet.minTopShort];
}


void LoopHold::ShiftMinDown(
  const SideDetails& sdet,
  CrashDetails& cr) const
{
  cr.blockRank = sdet.mapShiftedToReal[cr.blockRank];
  cr.remRank = sdet.mapShiftedToReal[cr.remRank];
  cr.crashRank = sdet.mapShiftedToReal[cr.crashRank];
  cr.crashRank2 = sdet.mapShiftedToReal[cr.crashRank2];
}


bool LoopHold::SolveSimple(
  DefList& def,
  unsigned& rank)
{
  assert(suitLength >= 4);
  assert(length[QT_PARD] > 0);

  htop.K = (counter >> (2*(suitLength-2))) & 0x3;
  htop.Q = (counter >> (2*(suitLength-3))) & 0x3;
  htop.J = (counter >> (2*(suitLength-4))) & 0x3;
  htop.T = (counter >> (2*(suitLength-5))) & 0x3;
  htop.N = (counter >> (2*(suitLength-6))) & 0x3;
  htop.E = (counter >> (2*(suitLength-7))) & 0x3;

  hopp.K = (htop.K == QT_LHO || htop.K == QT_RHO ? true : false);
  hopp.Q = (htop.Q == QT_LHO || htop.Q == QT_RHO ? true : false);
  hopp.J = (htop.J == QT_LHO || htop.J == QT_RHO ? true : false);
  hopp.T = (htop.T == QT_LHO || htop.T == QT_RHO ? true : false);
  hopp.N = (htop.N == QT_LHO || htop.N == QT_RHO ? true : false);
  hopp.E = (htop.E == QT_LHO || htop.E == QT_RHO ? true : false);

  distHex = (length[QT_ACE] << 12) | (length[QT_LHO] << 8) |
    (length[QT_PARD] << 4) | length[QT_RHO];

  // pickFlag = false;
  // if (! hopp.K && htop.Q == QT_RHO && htop.J == QT_RHO)
    // pickFlag = true;
  pickFlag = true;

  unsigned topIndex = (counter >> (2*(suitLength-4))) & 0x3f;
  Trick trick;
  if ((this->*SolveSimpleFunction[topIndex])(trick))
  {
    def.Set1(trick);
    rank = trick.GetRanks();
    return true;
  }
  else
    return false;
}


bool LoopHold::SolveComplex(
  DefList& def,
  unsigned& rank)
{
  assert(suitLength >= 4);
  assert(length[QT_PARD] > 0);

  pickFlag = true;

  unsigned topIndex = (counter >> (2*(suitLength-4))) & 0x3f;
  return ((this->*SolveComplexFunction[topIndex])(def, rank));
}


bool LoopHold::StopFinesse(
  const unsigned numFinesses,
  const unsigned firstNonTopAce,
  const unsigned firstNonTopPard,
  const bool ignoreOtherOpp,
  const PosType anchor) const
{
  // Take 1 finesse through the opponent indicated.
  // Return true if opponent prevents the finesse.

  PosType pa, pl, pp, pr;
  unsigned fna, fnp;
  if (anchor == QT_ACE)
  {
    // Finesse through RHO toward the ace.
    pa = QT_ACE;
    pl = QT_LHO;
    pp = QT_PARD;
    pr = QT_RHO;
    fna = firstNonTopAce;
    fnp = firstNonTopPard;
  }
  else
  {
    // Finesse through LHO behind the ace.
    pa = QT_PARD;
    pl = QT_RHO;
    pp = QT_ACE;
    pr = QT_LHO;
    fna = firstNonTopPard;
    fnp = firstNonTopAce;
  }

  bool skipOnePard = false;
  bool skipTwoPard = false;
  int effLenPard = static_cast<int>(length[pp] - fnp);

  if (! ignoreOtherOpp)
  {
    if (completeList[pl][0] > completeList[pp][fnp] &&
        completeList[pl][0] > completeList[pa][fna])
    {
      if (length[pl] == 1)
      {
        skipOnePard = true;
        effLenPard--;
      }
      else
      {
        skipTwoPard = true;
        effLenPard -= 2;
      }
    }
    else if (length[pl] > 1 &&
      completeList[pl][0] > completeList[pp][fnp+1] &&
      completeList[pl][0] > completeList[pa][fna])
    {
      skipOnePard = true;
      effLenPard--;
    }
  }

  if (effLenPard <= 0 || (skipTwoPard && numFinesses <= 2))
  {
    if (completeList[pr][0] < completeList[pa][fna])
      return false;
    else
      return true;
  }


  // Figure out the top situation of RHO vs partner.
  unsigned topOf3 = 1, topOf5 = 1, topOf7 = 1, num = 1;
  unsigned defNo = 1, pNo = fnp;
  while (num < 7 && defNo < length[pr])
  {
    if (completeList[pr][defNo] >= completeList[pp][pNo])
    {
      if (num < 3) topOf3++;
      if (num < 5) topOf5++;
      topOf7++;
      defNo++;
    }
    else
    {
      pNo++;
      if (skipTwoPard && pNo + 2 == length[pp])
        pNo += 2;
      else if (skipOnePard && pNo + 1 == length[pp])
        pNo++;
    }
    num++;
  }

  if (topOf3 < 2 ||
      completeList[pr][0] < completeList[pp][fnp] ||
      completeList[pr][1] < completeList[pa][fna])
    return false;

  if (effLenPard == 1 || numFinesses == 1)
    return true;

  if (topOf5 < 3 ||
      completeList[pr][2] < completeList[pa][fna])
    return false;

  if (effLenPard == 2 || numFinesses == 2 || skipOnePard)
    return true;
  
  if (topOf7 < 4 ||
      completeList[pr][3] < completeList[pa][fna] ||
      completeList[pr][3] < completeList[pp][3])
    return false;

  return true;
}


bool LoopHold::SolveSimple0(Trick& move) const
{
  // ==== G0 ===========================================================
  //      AKQJ+
  // +          +
  //      +
  // ==== G0 ===========================================================

  if (length[QT_ACE] < 5 || htop.T == QT_PARD)
    return false;

  if (htop.T == QT_ACE)
  {
    if (hopp.N && length[htop.N] >= 6 && (htop.N == QT_LHO ||
      LoopHold::StopFinesse(1, 5, 0, true, QT_ACE)))
    {
      // AKQJTx / 9xxxxx / x / -.
      // AKQJTx / - / x / 9xxxxx.
      if (pickFlag) holdCtr[0x000]++;
      return move.Set(QT_BOTH, QT_ACE, SDS_VOID-5, 5);
    }
  }
  else if (length[QT_LHO] >= 5)
  {
    if (completeList[QT_LHO][0] > completeList[QT_ACE][4] &&
      ((completeList[QT_LHO][0] > completeList[QT_PARD][0] ||
       (htop.T == QT_RHO && length[QT_PARD] == 1))))
    {
      // AKQJx+ / xxxxx+ / x+ / ?.
      // AKQJx+ / 8xxxx / 9 / T(x).
      if (pickFlag) holdCtr[0x001]++;
      return move.Set(QT_BOTH, QT_ACE, SDS_VOID-4, 4);
    }
  }
  else if (length[QT_RHO] >= 5)
  {
    if (completeList[QT_RHO][0] < completeList[QT_ACE][4])
      return false;

    if (length[QT_PARD] == 1)
    {
      if (htop.T == QT_LHO || LoopHold::StopFinesse(1, 4, 0, true, QT_ACE))
      {
        // AKQJx+ / ? / x / xxxxx+.
        if (pickFlag) holdCtr[0x002]++;
        return move.Set(QT_BOTH, QT_ACE, SDS_VOID-4, 4);
      }
    }
    else if (length[QT_PARD] == 2)
    {
      if (completeList[QT_LHO][0] > completeList[QT_PARD][0] &&
          completeList[QT_LHO][0] > completeList[QT_ACE][4])
      {
        if (LoopHold::StopFinesse(1, 4, 0, false, QT_ACE))
        {
          // AKQJx+ / ? / x / xxxxx+.
          if (pickFlag) holdCtr[0x003]++;
          return move.Set(QT_BOTH, QT_ACE, SDS_VOID-4, 4);
        }
      }
      else if (LoopHold::StopFinesse(2, 4, 0, true, QT_ACE))
      {
        // AKQJx+ / ? / x / xxxxx+.
        if (pickFlag) holdCtr[0x004]++;
        return move.Set(QT_BOTH, QT_ACE, SDS_VOID-4, 4);
      }
    }
    else if (LoopHold::StopFinesse(3, 4, 0, false, QT_ACE))
    {
      // AKQJx+ / ? / x / xxxxx+.
      if (pickFlag) holdCtr[0x005]++;
      return move.Set(QT_BOTH, QT_ACE, SDS_VOID-4, 4);
    }
  }
  return false;
}


bool LoopHold::SolveSimple1(Trick& move) const
{
  // ==== G1 ================= G4 ================= G16 ================
  //      AQJ+       |         AKJ+       |         AKQ+
  // +          K+   |    +          Q+   |    +          J+
  //      +          |         +          |         + 
  // ==== G1 ================= G4 ================= G16 ================

  unsigned r3 = (htop.J == QT_LHO ? SDS_VOID-3 : SDS_VOID-4);

  if (htop.K == QT_LHO && length[QT_LHO] >= 2)
  {
    // Protected K behind the ace.
    if (pickFlag) holdCtr[0x010]++;
    return move.Set(QT_BOTH, QT_ACE, SDS_VOID-1, 1);
  }
  else if (htop.Q == QT_LHO && length[QT_LHO] >= 3)
  {
    // Protected queen behind AK.
    if (pickFlag) holdCtr[0x011]++;
    return move.Set(QT_BOTH, QT_ACE, SDS_VOID-2, 2);
  }
  else if (htop.J == QT_LHO && length[QT_LHO] >= 4)
  {
    // Protected jack behind AKQ.
    if (pickFlag) holdCtr[0x012]++;
    return move.Set(QT_BOTH, QT_ACE, SDS_VOID-3, 3);
  }

  if (length[QT_RHO] <= 3)
    return false;

  if (length[QT_PARD] <= length[QT_LHO])
  {
    if (length[QT_RHO] >= 5 &&
      completeList[QT_RHO][0] < completeList[QT_ACE][3] &&
      completeList[QT_RHO][0] > completeList[QT_ACE][4])
    {
      // AHHT+ / H / x / 9xxxx+.
      // AHHT+ / H / 9 / 8xxxx+.
      // AHH9+ / H / T / 8xxxx+.
      // AKJ9+ / Qx / x(x) / xxxxx+.
      // AKQ9+ / Jx / x(x) / xxxxx+.
      if (pickFlag) holdCtr[0x013]++;
      unsigned r = HR(QT_ACE, 3);
      return move.Set(QT_BOTH, QT_ACE, r, 4);
    }
    else if (completeList[QT_RHO][0] < completeList[QT_ACE][2] &&
      completeList[QT_RHO][0] > completeList[QT_ACE][3])
    {
      // AHH+ / H / x / Txxx.
      // AHH+ / H / T / 9xxx.
      // AHHx / Hx / x(x) / xxxx.
      if (pickFlag) holdCtr[0x014]++;
      return move.Set(QT_BOTH, QT_ACE, r3, 3);
    }
  }
  else if (length[QT_ACE] == 3)
  {
    if (completeList[QT_RHO][0] > completeList[QT_PARD][0])
    {
      // AHH / H / ? / Txxx.
      if (pickFlag) holdCtr[0x015]++;
      return move.Set(QT_BOTH, QT_ACE, r3, 3);
    }
  }
  else if (length[QT_PARD] == 2)
  {
    if (length[QT_RHO] >= 5 && htop.T == QT_ACE && hopp.N && 
      LoopHold::StopFinesse(1, 4, 0, false, QT_ACE))
    {
      // AHHT+ / H / xx / 9xxxx+.
      if (pickFlag) holdCtr[0x016]++;
      return move.Set(QT_BOTH, QT_ACE, SDS_VOID-5, 4);
    }
    else if (htop.T == QT_RHO &&
      LoopHold::StopFinesse(1, 3, 0, false, QT_ACE))
    {
      // AHHx+ / H / xx / Txxx+.
      if (pickFlag) holdCtr[0x017]++;
      return move.Set(QT_BOTH, QT_ACE, r3, 3);
    }
  }
  else if (htop.K == QT_LHO && htop.T == QT_RHO &&
    LoopHold::StopFinesse(2, 3, 0, false, QT_ACE))
  {
    // AHHx / H / xxx+ / Txxx.
    if (pickFlag) holdCtr[0x018]++;
    return move.Set(QT_BOTH, QT_ACE, r3, 3);
  }
  else if (htop.K == QT_ACE && length[QT_LHO] == 1 &&
    LoopHold::StopFinesse(3, 3, 0, false, QT_ACE))
  {
    // AKHx+ / H / xx+ / xxxx+.
    if (pickFlag) holdCtr[0x019]++;
    return move.Set(QT_BOTH, QT_ACE, r3, 3);
  }
  else if (distHex == 0x4234 && htop.K == QT_ACE &&
    completeList[QT_RHO][1] > completeList[QT_ACE][3] &&
    completeList[QT_RHO][0] > completeList[QT_PARD][0])
  {
    // AKHx / Hx / xxX / xxxx.
    if (pickFlag) holdCtr[0x01a]++;
    return move.Set(QT_BOTH, QT_ACE, r3, 3);
  }
  return false;
}


bool LoopHold::SolveSimple2(Trick& move) const
{
  // ==== G2 ================= G8 ================= G32 ================
  //      AQJ+       |         AKJ+       |         AKQ+
  // +          +    |    +          +    |    +          +
  //      K+         |         Q+         |         J+
  // ==== G1 ================= G8 ================= G32 ================

  // ==== G10 ================ G34 ================ G40 ================
  //      AJ+        |         AQ+        |         AK+ 
  // +          +    |    +          +    |    +          +
  //      KQ+        |         KJ+        |         QJ+
  // ==== G10 ================ G34 ================ G40 ================

  // ==== G42 ==========================================================
  //      A+
  // +          +
  //      KQJ+
  // ==== G42 ==========================================================


  if (! hopp.T)
   return false;

  unsigned pardTops = 0;
  if (htop.K == QT_PARD) pardTops++;
  if (htop.Q == QT_PARD) pardTops++;
  if (htop.J == QT_PARD) pardTops++;

  if (length[QT_ACE] >= 5)
  {
    if (length[QT_RHO] >= 5)
    {
      if (length[QT_PARD] == 2 || pardTops == 2)
      {
        if (completeList[QT_RHO][0] > completeList[QT_PARD][pardTops] &&
            completeList[QT_RHO][1] > completeList[QT_ACE][4-pardTops])
        {
          // AHHxx+ / ? / Hx / hxxxx+.
          // AHxxx / - / HHx / Txxxx.
          if (pickFlag) holdCtr[0x020]++;
          return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-4, 4);
        }
      }
      else if (distHex == 0x5035 &&
        LoopHold::StopFinesse(2, 3, 1, true, QT_ACE))
      {
        // AHHxx / - / Hxx / Txxxx.
        if (pickFlag) holdCtr[0x021]++;
        return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-4, 4);
      }
    }
    else if (length[QT_LHO] >= 5)
    {
      if (pardTops == 1 && htop.T && (hopp.N ||
         (completeList[QT_LHO][0] > completeList[QT_ACE][3] &&
          completeList[QT_LHO][1] > completeList[QT_ACE][4] &&
          completeList[QT_LHO][1] > completeList[QT_PARD][1])))
      {
        // AHHxx / Txxxx / Hxx / -.
        if (pickFlag) holdCtr[0x022]++;
        return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-4, 4);
      }
      else if (pardTops == 2 && 
        LoopHold::StopFinesse(2, 2, 2, true, QT_PARD))
      {
        // AHxxx / Txxxx / HHx / -.
        if (pickFlag) holdCtr[0x023]++;
        return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-4, 4);
      }
    }
  }
  else if (length[QT_PARD] >= 5)
  {
    if (length[QT_LHO] >= 5)
    {
      if (length[QT_ACE] == 2 || pardTops == 2)
      {
        if (completeList[QT_LHO][0] > completeList[QT_ACE][4-pardTops] &&
            completeList[QT_LHO][1] > completeList[QT_PARD][pardTops])
        {
          // AHx / Txxxx / HHxxx / -.
          // Ax / xxxxx / KQJxx / ?.
          if (pickFlag) holdCtr[0x024]++;
          return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-4, 4);
        }
      }
      else if (distHex == 0x3550 &&
        LoopHold::StopFinesse(2, 1, 3, true, QT_PARD))
      {
        // Axx / Txxxx / KQJxx / -.
        if (pickFlag) holdCtr[0x025]++;
        return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-4, 4);
      }
    }
    else if (length[QT_RHO] >= 5)
    {
      if (pardTops == 3 && hopp.T && (hopp.N ||
         (completeList[QT_RHO][0] > completeList[QT_PARD][3] &&
          completeList[QT_RHO][1] > completeList[QT_PARD][4] &&
          completeList[QT_RHO][1] > completeList[QT_ACE][1])))
      {
        // Ax / ? / KQJxx / Txxxx.
        if (pickFlag) holdCtr[0x026]++;
        return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-4, 4);
      }
      else if (pardTops == 2 && 
        LoopHold::StopFinesse(2, 2, 2, true, QT_ACE))
      {
        // AHx / - / HHxxx / Txxxx.
        if (pickFlag) holdCtr[0x027]++;
        return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-4, 4);
      }
    }
  }
  return false;
}


bool LoopHold::SolveSimple3(Trick& move) const
{
  // ==== G3 ===========================================================
  //      AQJ+
  // K+         +
  //      +
  // ==== G3 ===========================================================

  if (length[QT_RHO] >= 2 || length[QT_LHO] <= 3)
    return false;

  if (hopp.T || (length[htop.T] == 1 && hopp.N))
  {
    // AQJ+ / Txxx+ / x+ / K.
    // AQJ+ / 9xxx+ / T / K.
    if (pickFlag) holdCtr[0x030]++;
    return move.Set(QT_BOTH, QT_ACE, SDS_VOID-4, 3);
  }

  if (length[QT_LHO] == 4)
    return false;

  if (htop.T == QT_ACE && hopp.N)
  {
    // AQJT+ / 9xxxx+ / x+ / K.
    if (pickFlag) holdCtr[0x031]++;
    return move.Set(QT_BOTH, QT_ACE, SDS_VOID-5, 4);
  }
  else if (! hopp.T && ! hopp.N && htop.T != htop.N && hopp.E &&
    length[QT_PARD] == 1)
  {
    // AQJT+ / 8xxxx+ / 9 / K.
    // AQJ9+ / 8xxxx+ / T / K.
    if (pickFlag) holdCtr[0x032]++;
    unsigned r = HR(QT_ACE, 3);
    return move.Set(QT_BOTH, QT_ACE, r, 4);
  }
  return false;
}


bool LoopHold::SolveSimple5(Trick& move) const
{
  // ==== G5 ============== G21 =========== G37 =========== G53 ========
  //      AJ+        |      A+       |      A+       |      A+
  // +          KQ+  |  +       KQJ+ |  +       KQ+  |  J+      KQ+
  //      +          |      +        |      J+       |      +
  // ==== G5 ============== G21 =========== G37 =========== G53 ========

  if (pickFlag) holdCtr[0x050]++;
  return move.Set(QT_BOTH, QT_ACE, SDS_VOID-1, 1);
}


bool LoopHold::SolveSimple6(Trick& move) const
{
  // ==== G6 ================= G46 =====================================
  //      AJ+        |         A+
  // +          Q+   |  Q+            +
  //      K+         |         KJ+
  // ==== G6 ================= G46 =====================================

  PosType pa, pl, pp, pr;
  if (htop.Q == QT_LHO)
  {
    pa = QT_ACE;
    pl = QT_LHO;
    pp = QT_PARD;
    pr = QT_RHO;
  }
  else
  {
    pa = QT_PARD;
    pl = QT_RHO;
    pp = QT_ACE;
    pr = QT_LHO;
  }

  if (length[pa] >= 3 && length[pp] >= 3 && length[pl] >= 4 &&
    (htop.T == pa || 
      (htop.T == pr && htop.N == pa && length[pr] == 1)) &&
    completeList[pl][1] > completeList[pa][3] &&
    completeList[pl][1] > completeList[pp][1])
  {
    // AJT+ / Q9xx+ / Kxx+ / +.
    // AJ9+ / Q8xx+ / Kxx+ / T.
    if (pickFlag) holdCtr[0x060]++;
    unsigned r = HR(pa, 2);
    return move.Set(QT_BOTH, QT_BOTH, r, 3);
  }
  else if (((length[pa] == 3 && length[pp] == 3 && length[pl] >= 3) ||
    (length[pa] == 4 && length[pp] == 4 && length[pl] == 3)) &&
    (htop.T == pa ||
      (htop.T == pr && length[pr] <= 2 && 
        (htop.N == pa || (htop.N == pr && htop.E == pa)))))
  {
    // AJT / Q+ / Kxx / +.
    // AJ9 / Q+ / Kxx / T, Tx.
    // AJ8 / Q+ / Kxx / T9.
    if (pickFlag) holdCtr[0x061]++;
    unsigned r = HR(pa, 2);
    return move.Set(QT_BOTH, QT_BOTH, r, length[pa]);
  }
  else if (length[pa] >= 4 && length[pp] == length[pa] &&
    length[pl] == 3 && length[pr] <= 3 && htop.T == pa)
  {
    // AJTx+ / Q(xx) / Kxxx+ / (xxx).
    if (pickFlag) holdCtr[0x062]++;
    unsigned r = HR(pa, 2);
    return move.Set(QT_BOTH, QT_BOTH, r, length[pa]);
  }
  else if (length[pa] == 4 && length[pl] >= 4 && length[pp] == 4 &&
    htop.T == pa && htop.N == pa)
    // ! hopp.T && htop.N == pa)
  {
    // Going to restrict this one a bit for now.
    // AJT9 / Qxxx+ / Kxxx / +.
    // AJ9x / Qxxx+ / KTxx / +.
    if (pickFlag) holdCtr[0x063]++;
    return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-6, 4);
  }
  else if (length[pa] == 4 && length[pr] == 4 && 
    length[pp] == 4 && length[pl] == 1 &&
    htop.T == pr && htop.N == pp && htop.E == pp)
  {
    // AJxx / Q / K98x / Txxx.
    // A98x / Txxx / KJxx / Q.
    if (pickFlag) holdCtr[0x064]++;
    return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-7, 4);
  }
  else if (length[pa] == 4 && length[pp] == 4 && 
    length[pr] == 1 && length[pl] == 4 &&
    htop.T == pr && ! hopp.N && ! hopp.E && 
    (htop.N == pa && htop.E == pa))
    // (htop.N == pa || htop.E == pa))
  {
    // AJ8x / Qxxx / K9xx / T.
    // AJ9x / Qxxx / K8xx / T.
    // A9xx / T / KJ8x / Qxxx.
    // A8xx / T / KJ9x / Qxxx.
    if (pickFlag) holdCtr[0x065]++;
    return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-7, 4);
  }
  else if (length[pa] == 4 && length[pp] == 4 && 
    length[pr] == 1 && length[pl] == 4 &&
    htop.N == pr && ! hopp.T && ! hopp.E && 
    (htop.T == pa && htop.E == pa))
    // (htop.T == pa || htop.E == pa))
  {
    // AJ8x / Qxxx / KTxx / 9.
    // AJTx / Qxxx / K8xx / 9.
    // ATxx / 9 / KJ8x / Qxxx.
    // A8xx / 9 / KJTx / Qxxx.
    if (pickFlag) holdCtr[0x066]++;
    return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-7, 4);
  }

  if (length[pp] == 2)
  {
    if (length[pl] == 1)
    {
      if (htop.T == pa && length[pa] >= 5 &&
        htop.N == pr && length[pr] >= 5)
      {
        if (completeList[pr][1] > completeList[pa][3])
        {
          // AJTxx / Q / Kx / 9xxxx.
          if (pickFlag) holdCtr[0x067]++;
          return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-5, 4);
        }
      }
      else if (length[pa] >= 3 && 
        htop.T == pr && length[pr] >= 4 &&
        completeList[pr][1] > completeList[pa][2])
      {
        // AJx+ / Q / Kx / Txxx+.
        if (pickFlag) holdCtr[0x068]++;
        return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-4, 3);
      }
    }
    else if (length[pl] == 2)
    {
      if (length[pa] >= 3 && length[pr] >= 4 &&
        completeList[pr][0] > completeList[pa][2] &&
        (completeList[pl][1] > completeList[pp][1] ||
         completeList[pr][0] > completeList[pp][1]))
      {
        // AJx+ / Qx / Kx / Txxx+.
        if (pickFlag) holdCtr[0x069]++;
        return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-4, 3);
      }
    }
    else if (completeList[pl][1] > completeList[pa][2] &&
      ((length[pr] >= 2 && htop.T == pr) ||
       completeList[pl][1] > completeList[pp][1]))
    {
      // AJx / Qxx+ / Kx / Tx+.
      if (pickFlag) holdCtr[0x06a]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-2, 2);
    }
    else if (length[pr] >= 3 && htop.T == pr &&
      completeList[pr][1] > completeList[pa][2])
    {
      // AJx / Qxx / Kx / Txx.
      if (pickFlag) holdCtr[0x06b]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-2, 2);
    }

    return false;
  }

  if (length[pa] == 2)
  {
    if (length[pl] >= 3 && completeList[pl][1] > completeList[pp][1])
    {
      // AJ / QTx+ / K+ / ?.
      if (pickFlag) holdCtr[0x06c]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-2, 2);
    }
    else if (length[pl] >= 2 && length[pr] >= 3 &&
      completeList[pr][0] > completeList[pp][1])
    {
      // AJ / Qx+ / K+ / Txx+.
      if (pickFlag) holdCtr[0x06d]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-2, 2);
    }
    else if (length[pl] == 2 && length[pr] >= 4 &&
      completeList[pr][0] < completeList[pp][1] &&
      completeList[pr][0] > completeList[pp][2])
    {
      // AJ / Qx / Kxx+ / xxxx.
      if (pickFlag) holdCtr[0x06e]++;
      unsigned r = HR(pp, 1);
      return move.Set(QT_BOTH, QT_BOTH, r, 3);
    }

    return false;
  }

  if (length[pl] == 1)
  {
    if ((distHex == 0x4351 || distHex == 0x5143) && htop.T == pr &&
      (htop.N == pa || (htop.N == pp && ! hopp.E)))
    {
  
      // AJ98x / Q / Kxxx / Txx.
      // AJ9xx / Q / K8xx / Txx.
      // AJxxx / Q / K9xx / Txx.
      if (pickFlag) holdCtr[0x06f]++;
      unsigned r = (htop.N == pa ? SDS_VOID-6 : SDS_VOID-7);
      return move.Set(QT_BOTH, QT_BOTH, r, 5);
    }

    if (length[pr] <= 3)
      return false;

    if (htop.T == pr)
    {
      if (length[pa] == 3)
      {
        if ((pa == QT_ACE && LoopHold::StopFinesse(1, 2, 1, true, pa)) ||
           (pa == QT_PARD && LoopHold::StopFinesse(1, 1, 2, true, pa)))
        {
          // AJx / Q / Kxxx+ / Txxx+.
          if (pickFlag) holdCtr[0x460]++;
          return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-4, 3);
        }
      }
      else if ((pa == QT_ACE && LoopHold::StopFinesse(2, 2, 1, true, pa))
           || (pa == QT_PARD && LoopHold::StopFinesse(2, 1, 2, true, pa)))
      {
        // AJx+ / Q / Kxx+ / T98x+, T97x+, T87x, etc.
        if (pickFlag) holdCtr[0x461]++;
        return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-4, 3);
      }
    }

    return false;
  }

  if (length[pl] == 2)
  {
    if (length[pr] <= 3)
      return false;

    if (length[pa] == 3 && length[pp] == 3)
    {
      // AJx / Qx / Kxx / ?.
      if (pickFlag) holdCtr[0x462]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-4, 3);
    }
    else if (length[pa] == 3 && length[pp] == 4 &&
    completeList[pr][0] > completeList[pp][1])
    {
      // AJx / Qx / Kxxx / 9xxx or so.
      if (pickFlag) holdCtr[0x463]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-4, 3);
    }
    else if (length[pa] == 4 && length[pp] == 3 &&
      completeList[pr][1] > completeList[pa][2] &&
      completeList[pr][0] > completeList[pp][1])
    {
      // AJxx / Qx / Kxx / T9xx.
      if (pickFlag) holdCtr[0x464]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-4, 3);
    }

    return false;
  }

  if (completeList[pl][1] > completeList[pa][2] &&
    completeList[pl][1] > completeList[pp][1])
  {
    // AJ+ / QTx+ / Kxx+ / ?.
    // AJ+ / Q9x+ / Kxx+ / T+.
    if (pickFlag) holdCtr[0x465]++;
    return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-2, 2);
  }

  if (htop.T != pr)
    return false;

  if (length[pr] == 2)
  {
    if (((htop.Q == QT_LHO && distHex == 0x3442) ||
         (htop.Q == QT_RHO && distHex == 0x4234)) &&
      completeList[pl][1] < completeList[pa][2] &&
      completeList[pl][1] > completeList[pp][1])
    {
      // AJ9 / Q8xx / Kxxx / Tx.
      if (pickFlag) holdCtr[0x466]++;
      unsigned r = HR(pa, 2);
      return move.Set(QT_BOTH, QT_BOTH, r, 3);
    }
    else if (((htop.Q == QT_LHO && distHex == 0x4432) ||
      (htop.Q == QT_RHO && distHex == 0x3244)) && 
      completeList[pl][1] > completeList[pa][3] &&
      completeList[pl][1] > completeList[pp][1])
    {
      // AJxx / Qxxx / Kxx / Tx.
      if (pickFlag) holdCtr[0x467]++;
      unsigned r = HR(pa, 2);
      return move.Set(QT_BOTH, QT_BOTH, r, 3);
    }
  }
  else if (length[pr] >= 3)
  {
    if ((htop.N == pp && hopp.E) || htop.N == pr)
    {
      // AJ+ / Qxx+ / K9x+ / T8x+.
      // AJ+ / Q8x+ / K9x+ / Txx+.
      // AJ+ / Qxx+ / Kxx+ / T9x+.
      if (pickFlag) holdCtr[0x468]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-2, 2);
    }
  }
  return false;
}


bool LoopHold::SolveSimple7(Trick& move) const
{
  // ==== G7 ============== G13 =========== G39 =========== G45 ========
  //      AJ+        |      AJ+      |      A+       |      A+
  // K+         Q+   | Q+       K+   |  K+      Q+   |  Q+      K+ 
  //      +          |      +        |      J+       |      J+
  // ==== G7 ============== G13 =========== G39 =========== G45 ========

  if (length[QT_LHO] >= 2 || length[QT_RHO] >= 2)
  {
    if (pickFlag) holdCtr[0x070]++;
    return move.Set(QT_BOTH, QT_ACE, SDS_VOID-1, 1);
  }
  else if (length[QT_PARD] == 1)
  {
    // Ax / K / x / x.
    // This and the following are tops, not real stops.
    if (pickFlag) holdCtr[0x071]++;
    return move.Set(QT_BOTH, QT_ACE, SDS_VOID-1, length[QT_ACE]);
  }
  else if (length[QT_ACE] == 2 && length[QT_PARD] == 2)
  {
    if (htop.J == QT_ACE)
    {
      // AJ / K / xx / Q.
      if (pickFlag) holdCtr[0x072]++;
      return move.Set(QT_BOTH, QT_ACE, SDS_VOID-1, 2);
      // return move.Set(QT_BOTH, QT_ACE, SDS_VOID-4, 2);
    }
  }
  return false;
}


bool LoopHold::SolveSimple9(Trick& move) const
{
  // ==== G9 ================= G33 =====================================
  //      AJ+        |         AQ+    
  // +          K+   |    +          K+
  //      Q+         |         J+      
  // ==== G9 ================= G33 =====================================

  if (length[QT_LHO] >= 2)
  {
    // Protected K behind the ace.
    if (pickFlag) holdCtr[0x090]++;
    return move.Set(QT_BOTH, QT_ACE, SDS_ACE, 1);
  }

  if (length[QT_ACE] == 2 && length[QT_PARD] <= 2 && htop.Q == QT_ACE)
  {
    if (length[QT_RHO] >= 2)
    {
      // AQ / K / J(x) / xx+.
      if (pickFlag) holdCtr[0x091]++;
      return move.Set(QT_BOTH, QT_ACE, SDS_VOID-3, 2);
    }
    else
    {
      // AQ / K / J(x) / (x).
      if (pickFlag) holdCtr[0x092]++;
      return move.Set(QT_BOTH, QT_ACE, SDS_VOID-1, 2);
    }
  }

  if (length[QT_PARD] == 1)
  {
    if (htop.T == QT_RHO && length[QT_RHO] >= 3)
    {
      // AJ+ / K / Q / Txx+.
      if (pickFlag) holdCtr[0x093]++;
      unsigned r = HR(QT_ACE, 1);
      return move.Set(QT_BOTH, QT_ACE, r, 2);
    }
    else if (htop.T == QT_ACE && htop.N == QT_RHO && length[QT_RHO] >= 4)
    {
      // AJT+ / K / Q / 9xxx+.
      if (pickFlag) holdCtr[0x094]++;
      return move.Set(QT_BOTH, QT_ACE, SDS_VOID-5, 3);
    }
    else if (htop.T == QT_ACE && htop.N == QT_ACE &&
      htop.E == QT_RHO && length[QT_RHO] >= 5)
    {
      // AJT9+ / K / Q / 8xxxx+.
      if (pickFlag) holdCtr[0x095]++;
      return move.Set(QT_BOTH, QT_ACE, SDS_VOID-6, 4);
    }
  }
  else if (length[QT_PARD] <= 2 || length[QT_ACE] <= 2 || 
    length[QT_RHO] <= 3 || htop.T != QT_RHO)
  {
    return false;
  }
  else if (distHex == 0x4144 && htop.N == QT_PARD && htop.E == QT_PARD)
  {
    // AQxx / K / J98x / Txxx.
    if (pickFlag) holdCtr[0x096]++;
    return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-7, 4);
  }
  else if ((length[QT_ACE] == 3 && length[QT_PARD] == 3) ||
    LoopHold::StopFinesse(1, 2, 1, false, QT_ACE))
  {
    // AJx / K / Qxxx+ / Txxx+.
    // AJxx / K / Qxxx / Txxx.
    // AJxxx / K / Qxx / Txxx.
    if (pickFlag) holdCtr[0x097]++;
    return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-4, 3);
  }
  return false;
}


bool LoopHold::SolveSimple11(Trick& move) const
{
  // ==== G11 ================ G35 =====================================
  //      AJ+        |         AQ+    
  // K+         +    |   K+          + 
  //      Q+         |         J+      
  // ==== G11 ================ G35 =====================================

  if (length[QT_RHO] >= 2)
    return false;

  if (length[QT_ACE] == 2 && length[QT_PARD] <= 2 && htop.Q == QT_ACE)
  {
    if (length[QT_LHO] >= 2)
    {
      if (pickFlag) holdCtr[0x110]++;
      return move.Set(QT_BOTH, QT_ACE, SDS_VOID-3, 2);
    }
    else
    {
      if (pickFlag) holdCtr[0x111]++;
      return move.Set(QT_BOTH, QT_ACE, SDS_VOID-1, 2);
    }
  }

  if (length[QT_ACE] == 2 && length[QT_PARD] == 1 && length[QT_LHO] >= 2)
  {
    // AJ / + / Q / K.
    if (pickFlag) holdCtr[0x112]++;
    return move.Set(QT_BOTH, QT_ACE, SDS_VOID-4, 2);
  }
  else if (length[QT_LHO] <= 2 && length[QT_PARD] == 1 && 
    htop.Q == QT_ACE)
  {
    // AQ+ / x(x) / J(x) / K.
    if (pickFlag) holdCtr[0x113]++;
    unsigned r = (length[QT_LHO] <= 1 && length[QT_PARD] <= 1 ?
      SDS_VOID-1 : SDS_VOID-3);
    return move.Set(QT_BOTH, QT_ACE, r, length[QT_ACE]);
  }
  else if (length[QT_PARD] == 1)
  {
    if (length[QT_LHO] <= 2)
    {
      // AH / (x) / H / H.
      if (pickFlag) holdCtr[0x114]++;
      unsigned r = (length[QT_LHO] == 2 ? HR(QT_ACE, 1) : SDS_VOID-1);
      return move.Set(QT_BOTH, QT_ACE, r, length[QT_ACE]);
    }
    else if (htop.T == QT_LHO)
    {
      // AH+ / Txx+ / H / K.
      if (pickFlag) holdCtr[0x115]++;
      unsigned r = HR(QT_ACE, 1);
      return move.Set(QT_BOTH, QT_ACE, r, 2);
    }
    else if (htop.T == QT_ACE && htop.N == QT_LHO && length[QT_LHO] >= 4)
    {
      // AQT+ / K / J / 9xxx+.
      if (pickFlag) holdCtr[0x116]++;
      return move.Set(QT_BOTH, QT_ACE, SDS_VOID-5, 3);
    }
    else if (htop.T == QT_ACE && htop.N == QT_ACE &&
      htop.E == QT_LHO && length[QT_LHO] >= 5)
    {
      // AQT9+ / K / J / 8xxxx+.
      if (pickFlag) holdCtr[0x117]++;
      return move.Set(QT_BOTH, QT_ACE, SDS_VOID-6, 4);
    }
  }
  else if (distHex == 0x4441 && htop.T == QT_LHO && 
    (htop.N == QT_PARD || (htop.N == QT_ACE && ! hopp.E)))
  {
    // AJxx / Txxx / Q9xx / K.
    // AJ9x / Txxx / Q8xx / K.
    // AJ98 / Txxx / Qxxx / K.
    if (pickFlag) holdCtr[0x118]++;
    unsigned r = (htop.N == QT_PARD ? SDS_VOID-6 : SDS_VOID-7);
    return move.Set(QT_BOTH, QT_BOTH, r, 4);
  }
  else if (length[QT_ACE] >= 3 && length[QT_LHO] >= 4 &&
    length[QT_PARD] >= 3 && htop.T == QT_LHO &&
    (htop.N == QT_LHO || (htop.N == QT_ACE && htop.E == QT_LHO)))
  {
    // AHxx / Txxx / Hxx / K.
    if (pickFlag) holdCtr[0x119]++;
    return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-4, 3);
  }
  else if (distHex == 0x5341 && htop.T == QT_LHO && htop.N == QT_PARD)
  {
    // AJxxx / Txx / Q9xx / K.
    if (pickFlag) holdCtr[0x11a]++;
    return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-6, 5);
  }
  return false;
}


bool LoopHold::SolveSimple12(Trick& move) const
{
  // ==== G12 ==========================================================
  //      AKJ+    
  // Q+         +
  //      +     
  // ==== G12 ==========================================================

  if (length[QT_RHO] >= 3 || length[QT_LHO] <= 3)
    return false;

  if (length[QT_ACE] == 3)
  {
    if (length[QT_PARD] <= 3 ||
      completeList[QT_LHO][0] > completeList[QT_PARD][0])
    {
      // AKJ / xxxx+ / ? / Q+.
      if (pickFlag) holdCtr[0x120]++;
      return move.Set(QT_BOTH, QT_ACE, SDS_VOID-4, 3);
    }
  }
  else if (completeList[QT_LHO][0] > completeList[QT_ACE][3])
  {
    if (length[QT_PARD] == 1 ||
      completeList[QT_LHO][0] > completeList[QT_PARD][0])
    {
      // AKJx+ / Txxx+ / ? / Q(x).
      if (pickFlag) holdCtr[0x121]++;
      return move.Set(QT_BOTH, QT_ACE, SDS_VOID-4, 3);
    }
    else if (length[QT_PARD] == 2 && length[QT_RHO] == 2 &&
      (hopp.T || hopp.N))
    {
      // AKJx+ / Txxx+ / xx / Qx.
      if (pickFlag) holdCtr[0x122]++;
      return move.Set(QT_BOTH, QT_ACE, SDS_VOID-4, 3);
    }
  }
  else if (length[QT_ACE] >= 5 &&
      completeList[QT_LHO][0] > completeList[QT_ACE][4])
  {
    if (length[QT_LHO] >= 5 &&
      (length[QT_RHO] == 2 || (length[QT_PARD] == 1 ||
        completeList[QT_PARD][0] < completeList[QT_LHO][0])))
    {
      // AKJxx / xxxxx / x / Q(x).
      if (pickFlag) holdCtr[0x123]++;
      unsigned r = HR(QT_ACE, 3);
      return move.Set(QT_BOTH, QT_ACE, r, 4);
    }
  }
  return false;
}


bool LoopHold::SolveSimple14(Trick& move) const
{
  // ==== G14 ================ G38 =====================================
  //      AJ+        |         A+
  // Q+         +    |  +             Q+
  //      K+         |         KJ+
  // ==== G14 ================ G38 =====================================

  PosType pa, pl, pp, pr;
  if (htop.Q == QT_RHO)
  {
    pa = QT_ACE;
    pl = QT_LHO;
    pp = QT_PARD;
    pr = QT_RHO;
  }
  else
  {
    pa = QT_PARD;
    pl = QT_RHO;
    pp = QT_ACE;
    pr = QT_LHO;
  }

  if ((distHex == 0x5341 || distHex == 0x4153) && 
    htop.T == pl && htop.N == pp)
  {
    // AJxxx / Txx / K9xx / Q.
    if (pickFlag) holdCtr[0x140]++;
    unsigned r = (htop.N == pp ? SDS_VOID-6 : SDS_VOID-7);
    return move.Set(QT_BOTH, QT_BOTH, r, 5);
  }

  if (length[pa] <= 2 || length[pr] >= 3 || length[pl] <= 3)
    return false;

  if (length[pr] == 1)
  {
    if (htop.T == pl)
    {
      if (length[pp] == 2)
      {
        // AJx+ / Q / Kx / Txxx+.
        if (pickFlag) holdCtr[0x141]++;
        return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-4, 3);
      }
      else if (htop.N == pl || (htop.N == pa && htop.E == pl))
      {
         // AJ9x / T8xx / Kxx / Q.
        if (pickFlag) holdCtr[0x142]++;
        return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-4, 3);
      }
      else if (length[pa] == 4 && length[pp] == 4 && length[pl] == 4 &&
        ! hopp.N)
      {
        if (htop.N == pp)
        {
          // AJxx / T8xx / K9xx / Q.
          if (pickFlag) holdCtr[0x143]++;
          return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-6, 4);
        }
        else if (! hopp.E)
        {
          // AJ9x / Txxx / K8xx / Q.
          // AJ8x / Txxx / K9xx / Q.
          if (pickFlag) holdCtr[0x144]++;
          return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-7, 4);
        }
      }
    }
    else if (length[pa] >= 5 && length[pl] >= 5 && htop.T == pa && hopp.N)
    {
      // AJTxx / 9xxxx / Kx / Q.
      if (pickFlag) holdCtr[0x145]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-5, 4);
    }
  }
  else if (htop.T == pl || (htop.T == pr && htop.N == pl))
  {
    if (length[pp] <= 3)
    {
      // AJ65 / T743 / K98 / Q2.
      if (pickFlag) holdCtr[0x146]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-4, 3);
    }
    else if (length[pa] == 3 && completeList[pl][1] > completeList[pp][1])
    {
      // AJ9 / T654 / K732 / Q8.
      if (pickFlag) holdCtr[0x147]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-4, 3);
    }
  }
  else if (length[pa] >= 4 && length[pp] == 2 &&
    completeList[pl][0] > completeList[pa][2])
  {
    // AJxx / 9xxx / KT / Qx, etc.
    if (pickFlag) holdCtr[0x148]++;
    return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-4, 3);
  }

  return false;
}


bool LoopHold::SolveSimple15(Trick& move) const
{
  // ==== G15 ================ G17 ================ G23 ================
  //      AJ+        |         AQ+        |         A+  
  // KQ+        +    |   +           KJ+  |   K+          QJ+
  //      +          |         +          |         + 
  // ==== G15 ================ G17 ================ G23 ================

  // ==== G25 ================ G29 ================ G31 ================
  //      A+         |         A+         |         A+  
  // +          KJ+  |   Q+          KJ+  |   KQ+         J+ 
  //      Q+         |         +          |         + 
  // ==== G25 ================ G29 ================ G31 ================

  // ==== G47 ================ G51 ================ G55 ================
  //      A+         |         AQ+        |         A+  
  // KQ+        +    |   KJ+         +    |   KJ+         Q+ 
  //      J+         |         +          |         + 
  // ==== G47 ================ G51 ================ G55 ================

  // ==== G59 ================ G61 ================ G63 ================
  //      A+         |         A+         |         A+  
  // KJ+        +    |   QJ+         K+   |   KQJ+        +  
  //      Q+         |         +          |         + 
  // ==== G59 ================ G61 ================ G63 ================

  unsigned fourTops = 1;
  if (! hopp.K) fourTops++;
  if (! hopp.Q) fourTops++;
  if (! hopp.J) fourTops++;

  if (fourTops == 1)
  {
    if (pickFlag) holdCtr[0x150]++;
    return move.Set(QT_BOTH, QT_ACE, SDS_VOID-1, 1);
  }
  else if (fourTops == 2 && hopp.K)
  {
    if (htop.K == QT_LHO && htop.J == QT_LHO)
    {
      if (pickFlag) holdCtr[0x151]++;
      return move.Set(QT_BOTH, QT_ACE, SDS_VOID-1, 1);
    }
    else if (htop.K == QT_RHO)
    {
      if (htop.Q == QT_RHO || (htop.Q == QT_PARD && htop.J == QT_RHO))
      {
        if (pickFlag) holdCtr[0x152]++;
        return move.Set(QT_BOTH, QT_ACE, SDS_VOID-1, 1);
      }
    }
  }

  return false;
}


bool LoopHold::SolveSimple18(Trick& move) const
{
  // ==== G18 ================ G24 ================ G58 ================
  //      AQ+        |         AK+        |         A+
  // +          J+   |   +           J+   |  J+            +
  //      K+         |         Q+         |         KQ+
  // ==== G18 ================ G24 ================ G58 ================

  if (length[QT_ACE] <= 3 && length[QT_PARD] <= 3)
    return false;

  PosType pa, pl, pp, pr;
  if (htop.K == QT_ACE || htop.Q == QT_ACE)
  {
    pa = QT_ACE;
    pl = QT_LHO;
    pp = QT_PARD;
    pr = QT_RHO;
  }
  else
  {
    pa = QT_PARD;
    pl = QT_RHO;
    pp = QT_ACE;
    pr = QT_LHO;
  }

  if (length[pa] >= 5 && length[pp] == 4)
  {
    if (length[pl] == 3)
    {
      if (htop.T == pp)
      {
        // AKxxx+ / Jxx / QTxx / (x).
        if (pickFlag) holdCtr[0x180]++;
        return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-5, length[pa]);
      }
      else if (htop.T == pr && htop.N == pp)
      {
        // AKxxx / Jxx / Q9xx / T.
        if (pickFlag) holdCtr[0x181]++;
        return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-6, length[pa]);
      }
    }
    else if (length[pr] == 3)
    {
      if (htop.T == pr && htop.N == pp && htop.E == pp)
      {
        // AKxxx / J / Q98x / Txx.
        if (pickFlag) holdCtr[0x182]++;
        return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-7, length[pa]);
      }
    }
  }

  if (length[QT_LHO] <= 3 && length[QT_RHO] <= 3)
    return false;

  unsigned rnum = 0;
  if (htop.J == pr) rnum++;
  if (htop.T == pr) rnum++;
  if (htop.N == pr) rnum++;

  if (length[pl] == 1)
  {
    if (length[pp] == 2)
    {
      if (htop.T == pa)
      {
        if (((pa == QT_ACE && distHex == 0x5125) ||
             (pa == QT_PARD && distHex == 0x2551)) && 
          completeList[pr][1] > completeList[pa][3] &&
          completeList[pr][0] > completeList[pp][1])
        {
          // AQTxx / J / Kx / 9xxxx.
          if (pickFlag) holdCtr[0x183]++;
          return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-5, 4);
        }
      }
      else if (hopp.T && 
        (rnum == 2 || (htop.N == pp && htop.E == pr)))
      {
        // AHxx+ / h / Hx / hxxx+ with enough middle cards.
        if (pickFlag) holdCtr[0x184]++;
        return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-3, 3);
      }
    }
    else if (length[pa] == 3)
    {
      if (hopp.T && LoopHold::StopFinesse(1, 2, 1, true, QT_ACE))
      {
        // AHx / h / Hxxx+ / hxxx+.
        if (pickFlag) holdCtr[0x185]++;
        return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-3, 3);
      }
    }
    else if (length[pa] == 4 && length[pl] == 1 && length[pp] == 4 &&
      length[pr] == 4 && htop.T == pr && htop.N == pp && htop.E == pp)
    {
      // AKxx / J / Q98x / Txxx.
      if (pickFlag) holdCtr[0x186]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-7, 4);
    }
    else if (hopp.T && LoopHold::StopFinesse(2, 2, 1, true, QT_ACE))
    {
      // AHxx+ / h / Hxx+ / hxxx+.
      if (pickFlag) holdCtr[0x187]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-3, 3);
    }
  }
  else if (length[pl] == 2)
  {
    if (hopp.T && rnum >= 1)
    {
      if (length[pp] == 2 || length[pa] == 3 ||
        (((pa == QT_ACE && distHex == 0x4234) ||
          (pa == QT_PARD && distHex == 0x3442)) &&
        completeList[pr][1] > completeList[pa][2]))
      {
        // AHxx / hx / Hxx / hxxx.
        // AHx / hx / Hxxx / hxxx.
        // AHx+ / hx / Hx / hxxx.
        if (pickFlag) holdCtr[0x188]++;
        return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-3, 3);
      }
    }
    else if (length[pp] == 2 && length[pr] >= 4 &&
      htop.T == pl && htop.N == pp && htop.E == pr)
    {
      // AKxx / JT / Q9 / 8xxx.
      if (pickFlag) holdCtr[0x189]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-3, 3);
    }
  }
  else if (length[pa] == 4 && length[pp] == 4 && length[pl] >= 4 && 
    htop.J == pl && ! hopp.T && (htop.T == pp || ! hopp.N))
  {
    // AKT9 / Jxxx+ / Qxxx / +.
    // AKTx / Jxxx+ / Q9xx / +.
    // AK9x / Jxxx+ / QTxx / +.
    // Reset numbers.
    if (pickFlag) holdCtr[0x18a]++;
    unsigned r;
    if (htop.T == pa && htop.N == pa)
      r = SDS_VOID - 6;
    else
      r = HR(pp, 1);

    return move.Set(QT_BOTH, QT_BOTH, r, 4);
  }
  else if (length[pa] == 4 && length[pl] == 4 && length[pp] == 4 &&
    length[pr] == 1 && ! hopp.T && htop.N == pr && ! hopp.E)
  {
    // AKTx / Jxxx / Q8xx / 9.
    // AKT8 / Jxxx / Qxxx / 9.
    if (pickFlag) holdCtr[0x18b]++;
    return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-7, 4);
  }
  else if (length[pl] >= 4 && htop.J == pl &&
    (htop.T == pl || (htop.T == pa && htop.N == pl)))
  {
    // AHx+ / JTxx+ / Hx+ / ?.
    // AHT+ / J9xx+ / Hx+ / ?.
    if (pickFlag) holdCtr[0x18c]++;
    return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-3, 3);
  }
  else if (length[pa] >= 4 && length[pl] >= 4 &&
    length[pp] == 2 && length[pr] >= 2 &&
    htop.J == pl && htop.T == pa &&
    completeList[pl][1] > completeList[pa][3] &&
    completeList[pr][0] > completeList[pp][1])
  {
    // AKTx / Jxxx / Qx / xx.
    if (pickFlag) holdCtr[0x18d]++;
    return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-3, 3);
  }
  else if (length[pr] == 1)
  {
    if (length[pl] >= 4 && hopp.T && rnum >= 1)
    {
      if (length[pp] == 2)
      {
        // AHxx / hxxx+ / Hx / h.
        if (pickFlag) holdCtr[0x18e]++;
        return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-3, 3);
      }
      else if ((htop.N == pl || 
        (htop.N == pa && htop.E == pl)))
      {
        // AHxx / hxxx+ / Hxx+ / h.
        if (pickFlag) holdCtr[0x18f]++;
        return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-3, 3);
      }
    }
  }
  else if (hopp.T && rnum >= 1)
  {
    if (length[pr] >= 3)
    {
      // AHx+ / hxx+ / Hx+ / hxx+.
      if (pickFlag) holdCtr[0x240]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-3, 3);
    }
    else if (length[pp] <= 3)
    {
      // AHxx / hxxx / Hx / hx.
      // AHxx / hxxx / Hxx / hx.
      if (pickFlag) holdCtr[0x241]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-3, 3);
    }
    else if (completeList[pl][1] > completeList[pp][1])
    {
      // AHx / hxxx / Hxxx / hx.
      if (pickFlag) holdCtr[0x242]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-3, 3);
    }
  }
  else if (length[pp] == 2)
  {
    if (length[pr] == 4 &&
      completeList[pl][1] > completeList[pp][1] &&
      completeList[pr][0] > completeList[pa][2])
    {
      // AKxx / Jxx / Qx / xxxx.
      if (pickFlag) holdCtr[0x243]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-3, 3);
    }
  }
  return false;
}


bool LoopHold::SolveSimple19(Trick& move) const
{
  // ==== G19 ==========================================================
  //      AQ+    
  // K+         J+
  //      +      
  // ==== G19 ==========================================================

  if (length[QT_RHO] >= 2)
    return false;

  if (length[QT_LHO] >= 3)
  {
    // AQ+ / Jxx / ? / K.
    if (pickFlag) holdCtr[0x190]++;
    return move.Set(QT_BOTH, QT_ACE, SDS_VOID-3, 2);
  }
  else if (length[QT_PARD] <= 1)
  {
    if (pickFlag) holdCtr[0x191]++;
    unsigned r = (length[QT_LHO] <= 1 ? SDS_VOID-1 : SDS_VOID-3);
    return move.Set(QT_BOTH, QT_ACE, r, length[QT_ACE]);
  }
  else if (length[QT_PARD] == 2 && length[QT_ACE] == 2)
  {
    if (pickFlag) holdCtr[0x192]++;
    unsigned r = HR(QT_ACE, length[QT_LHO]-1);
    return move.Set(QT_BOTH, QT_ACE, r, length[QT_ACE]);
  }
  return false;
}


bool LoopHold::SolveSimple20(Trick& move) const
{
  // ==== G20 ============= G22 ========================================
  //      AK+        |      A+     
  // +          QJ+  |  +       QJ+
  //      +          |      K+     
  // ==== G20 ============= G22 ========================================

  PosType pend = (htop.K == QT_ACE ? QT_ACE : QT_BOTH);
  unsigned x = (htop.K == QT_ACE ? 2 : 1);

  if (length[QT_LHO] >= 3 || (length[QT_RHO] >= 3 &&
    (htop.T == QT_RHO || ((length[QT_PARD] <= 2 &&
      completeList[QT_RHO][0] > completeList[QT_ACE][x]) ||
        (length[QT_ACE] == 2 && 
          completeList[QT_RHO][0] > completeList[QT_PARD][1])))))
  {
    if (pickFlag) holdCtr[0x200]++;
    return move.Set(QT_BOTH, pend, SDS_VOID-2, 2);
  }
  else if (length[QT_ACE] >= 3 &&
    length[QT_PARD] <= 2 && length[QT_RHO] >= 4 && 
    completeList[QT_RHO][0] > completeList[QT_ACE][x+1] &&
    completeList[QT_RHO][0] < completeList[QT_ACE][x])
  {
    // AKxx / QJ / xx / xxxx.
    // Axxx / QJ / Kx / xxxx.
    if (pickFlag) holdCtr[0x201]++;
    unsigned r = HR(QT_ACE, x);
    return move.Set(QT_BOTH, pend, r, 3);
  }
  else if (length[QT_ACE] >= 4 &&
    length[QT_PARD] <= 2 && length[QT_RHO] >= 5 &&
    completeList[QT_RHO][0] > completeList[QT_ACE][x+2] &&
    completeList[QT_RHO][0] < completeList[QT_ACE][x+1])
  {
    // AKT9x / QJ / xx / 8xxxx.
    // AT9xx / QJ / Kx / 8xxxx.
    if (pickFlag) holdCtr[0x202]++;
    unsigned r = HR(QT_ACE, x+1);
    return move.Set(QT_BOTH, pend, r, 4);
  }

  if (length[QT_RHO] <= 3)
    return false;

  if (distHex == 0x4234)
  {
    if (htop.T == QT_PARD &&
         ((htop.K == QT_ACE &&
           completeList[QT_RHO][0] > completeList[QT_ACE][2]) ||
          (htop.K == QT_PARD &&
           completeList[QT_RHO][0] > completeList[QT_ACE][1])))
    {
      // AKxx / QJ / Txx / xxxx.
      // Axxx / QJ / KTx / xxxx.
      if (pickFlag) holdCtr[0x203]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-5, 3);
    }
    else if (htop.T == QT_ACE && htop.N == QT_RHO &&
      ((htop.K == QT_ACE &&
        completeList[QT_RHO][1] > completeList[QT_ACE][3]) ||
       (htop.K == QT_PARD &&
        completeList[QT_RHO][1] > completeList[QT_ACE][2])))
    {
      // AKTx / QJ / xxx / 9xxx.
      // ATxx / QJ / Kxx / 9xxx.
      if (pickFlag) holdCtr[0x204]++;
      return move.Set(QT_BOTH, pend, SDS_VOID-5, 3);
    }
  }
  else if (length[QT_PARD] >= 4 && htop.K == QT_PARD)
  {
    if (completeList[QT_RHO][0] < completeList[QT_PARD][1] &&
        completeList[QT_RHO][0] > completeList[QT_PARD][2] &&
        (length[QT_ACE] == 2 ||
          completeList[QT_RHO][0] > completeList[QT_ACE][1]))
    {
      // Ax / QJ / KTxx / 9xxx.
      // Axx / QJ / KTxx / 9xxx.
      if (pickFlag) holdCtr[0x205]++;
      unsigned r = HR(QT_PARD, 1);
      return move.Set(QT_BOTH, QT_BOTH, r, 3);
    }
    else if (length[QT_ACE] == 3 && htop.T == QT_ACE &&
      completeList[QT_RHO][0] > completeList[QT_PARD][1])
    {
      // ATx / QJ / Kxxx / 9xxx.
      if (pickFlag) holdCtr[0x206]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-5, 3);
    }
  }
  else if (distHex == 0x3244 && htop.K == QT_ACE && ! hopp.T && hopp.N)
  {
    // AKT / QJ / xxxx / 9xxx.
    // AKx / QJ / Txxx / 9xxx.
    if (pickFlag) holdCtr[0x207]++;
    PosType e = (htop.T == QT_ACE ? QT_ACE : QT_BOTH);
    return move.Set(QT_BOTH, e, SDS_VOID-5, 3);
  }
  return false;
}


bool LoopHold::SolveSimple26(Trick& move) const
{
  // ==== G26 ================ G50 ================ G56 ================
  //      A+         |         AQ+        |         AK+ 
  // +          J+   |   J+          +    |   J+          +  
  //      KQ+        |         K+         |         Q+
  // ==== G26 ================ G50 ================ G56 ================

  PosType pa, pl, pp, pr;
  if (htop.K == QT_PARD && htop.Q == QT_PARD)
  {
    pa = QT_ACE;
    pl = QT_LHO;
    pp = QT_PARD;
    pr = QT_RHO;
  }
  else
  {
    pa = QT_PARD;
    pl = QT_RHO;
    pp = QT_ACE;
    pr = QT_LHO;
  }

  if (length[pa] == 2 && length[pl] >= 1 && 
      length[pp] >= 4 && length[pr] >= 5 &&
      htop.T == pp && 
      completeList[pr][0] > completeList[pa][1] &&
      completeList[pr][0] > completeList[pp][3])
  {
    // AQTxx / 9xxxx / Kx / J.
    // AKTxx / 9xxxx / Qx / J.
    // Ax / J / KQTxx / 9xxxx.
    if (pickFlag) holdCtr[0x260]++;
    return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-5, 4);
  }

  if (length[pa] == 2)
  {
    // Jack must be left.
    if (length[pp] <= 2)
      return false;

    if (length[pl] >= 4 && completeList[pl][1] > completeList[pp][2])
    {
      // Single defender, e.g. JTxx, J9xx over T.
      if (pickFlag) holdCtr[0x261]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-3, 3);
    }
    else if (length[pr] >= 4 &&
      completeList[pr][0] > completeList[pp][2] &&
      (htop.T == pr || length[pl] >= 2))
    {
      // AT / Jx+ / KQxx / 9xxx.
      if (pickFlag) holdCtr[0x262]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-3, 3);
    }
  }
  else if (length[pa] == 4 && length[pl] == 4 &&
    length[pp] == 3 && length[pr] == 2 &&
    htop.J == pl && htop.T == pa &&
    completeList[pr][0] > completeList[pa][2] &&
    completeList[pr][0] > completeList[pp][2] &&
    completeList[pl][1] > completeList[pa][2] &&
    completeList[pl][1] > completeList[pp][2])
  {
    // AKx / 9x / QTxx / Jxxx.
    if (pickFlag) holdCtr[0x263]++;
    return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-3, 3);
  }
  else if (length[pl] >= 4 && length[pr] <= 2 && htop.J == pl && 
    (((pp == QT_ACE && LoopHold::StopFinesse(2, 2, 1, true, pp)) ||
      (pp == QT_PARD && LoopHold::StopFinesse(2, 1, 2, true, pp)))))
  {
    // AT8x / J9xx / KQx / xx.
    if (pickFlag) holdCtr[0x264]++;
    return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-3, 3);
  }
  else if (length[pp] == 3)
  {
    if (completeList[SDS_PARTNER[htop.J]][0] > completeList[pa][1] &&
        completeList[SDS_PARTNER[htop.J]][0] > completeList[pp][2])
    {
      // Split honors.
      if (length[pl] >= 4 && (length[pr] >= 2 ||
        ((htop.N == pl || (htop.N == pa && htop.E == pl)))))
      {
        // AKxx / hxxx / KQx / h.
        if (pickFlag) holdCtr[0x265]++;
        return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-3, 3);
      }
    }
    else if (length[pl] >= 4 && length[pr] >= 2 && 
      htop.J == pl && htop.T == pl && htop.N == pa && hopp.E)
    {
      // Axx+ / JTxx / KQx / xx.
      if (pickFlag) holdCtr[0x266]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-3, 3);
    }
  }
  else if (length[pa] == 4 && length[pp] == 4 && length[pl] >= 4 && 
    htop.T == pa && (htop.N == pa || (htop.N == pr && length[pr] == 1 &&
      htop.E == pa)))
  {
    // AT9x / J+ / KQxx / +.
    // AT8x / J+ / KQxx / 9.
    if (pickFlag) holdCtr[0x267]++;
    unsigned r = HR(pa, 2);
    return move.Set(QT_BOTH, QT_BOTH, r, 4);
  }
  else if (length[pa] == 4 && length[pp] == 4 && 
    length[pr] == 4 && length[pl] == 1 && 
    htop.T == pr &&
      (htop.N == pa || (! hopp.N && ! hopp.E)))
  {
    // Axxx / h / KQ9x / h+.
    // A9xx / h / KQ8x / h+.
    // A98x / h / KQxx / h+.
    if (pickFlag) holdCtr[0x268]++;
    unsigned r = (htop.N == pa ? SDS_VOID - 6 : SDS_VOID - 7);
    return move.Set(QT_BOTH, QT_BOTH, r, 4);
  }
  else if (length[pa] == 4 && length[pp] >= 5)
  {
    if (length[pl] == 3)
    {
      if (htop.T == pa && ! hopp.N)
      {
        // HHxxx / (x) / HT9x / Jxx.
        // HH9xx / (x) / HTxx / Jxx.
        if (pickFlag) holdCtr[0x269]++;
        return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-6, length[pp]);
      }
      else if (htop.T == pa && htop.N == pr && htop.E == pa)
      {
        // HHxxx / 9 / HT8x / Jxx.
        if (pickFlag) holdCtr[0x26a]++;
        return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-7, length[pp]);
      }
      else if (htop.T == pr && htop.N == pa && htop.E == pa)
      {
        // HHxxx / h / H98x / hxx.
        if (pickFlag) holdCtr[0x26b]++;
        return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-7, length[pp]);
      }
    }
    else if (length[pr] == 3 && htop.T == pr && htop.N == pa)
    {
        // HHxxx / hxx / H9xx / h.
      if (pickFlag) holdCtr[0x26c]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-6, length[pp]);
    }
  }
    
  return false;
}


bool LoopHold::SolveSimple27(Trick& move) const
{
  // ==== G27 ==========================================================
  //      A+     
  // K+         J+
  //      Q+     
  // ==== G27 ==========================================================

  if (length[QT_ACE] >= 3 && length[QT_LHO] >= 4 &&
    length[QT_PARD] >= 3 && length[QT_RHO] == 1 &&
    htop.T == QT_ACE && htop.N == QT_ACE && htop.E == QT_LHO)
  {
    // AT9+ / J8xx+ / Qxx / K.
    if (pickFlag) holdCtr[0x270]++;
    return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-6, 3);
  }
  else if (length[QT_ACE] == 3 && length[QT_LHO] >= 3 &&
    length[QT_PARD] == 3 && length[QT_RHO] == 1 &&
    htop.T == QT_ACE && htop.N == QT_ACE)
  {
    // AT9 / Jxx+ / Qxx / K.
    if (pickFlag) holdCtr[0x271]++;
    return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-6, 3);
  }
  else if (distHex == 0x4341 && htop.T == QT_ACE && htop.N == QT_ACE)
  {
    // AT9x / Jxx / Qxxxx / K.
    if (pickFlag) holdCtr[0x272]++;
    return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-6, 4);
  }
  else if (distHex == 0x4441 && ! hopp.T && ! hopp.N && ! hopp.E)
  {
    unsigned rnum = 0;
    if (htop.T == QT_ACE) rnum++;
    if (htop.N == QT_ACE) rnum++;
    if (htop.E == QT_ACE) rnum++;

    if (rnum == 3)
    {
      // AT98 / Jxxx / Qxxx / K.
      // Ahhx / Jxxx / Qhxx / K.
      if (pickFlag) holdCtr[0x273]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-7, 4);
    }
  }


  if (length[QT_RHO] >= 2)
  {
    if (length[QT_LHO] >= 2 || hopp.T)
    {
      // K not finessable.
      if (pickFlag) holdCtr[0x274]++;
      return move.Set(QT_BOTH, QT_ACE, SDS_ACE, 1);
    }
  }
  else if (length[QT_PARD] == 1 && length[QT_LHO] >= 2)
  {
    // K not finessable.
    if (pickFlag) holdCtr[0x275]++;
    return move.Set(QT_BOTH, QT_ACE, SDS_ACE, 1);
  }
  else if (length[QT_LHO] == 1 && length[QT_PARD] == 1)
  {
    // Ax+ / K / x / J.
    if (pickFlag) holdCtr[0x276]++;
    return move.Set(QT_BOTH, QT_ACE, SDS_ACE, length[QT_ACE]);
  }
  else if (length[QT_ACE] == 2 && length[QT_PARD] == 2 &&
    length[QT_LHO] >= 2)
  {
    // Ax / J / QT / K.
    // Ax / Jx+ / Qx / K.
    if (pickFlag) holdCtr[0x277]++;
    return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-3, 2);
  }
  else if (length[QT_LHO] >= 3 && (length[QT_PARD] == 2 ||
    ((completeList[QT_LHO][1] > completeList[QT_PARD][1]) &&
      completeList[QT_LHO][1] > completeList[QT_ACE][2])))
  {
    // Ax+ / Jxx+ / Qx+ / K.
    if (pickFlag) holdCtr[0x278]++;
    return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-3, 2);
  }
  return false;
}


bool LoopHold::SolveSimple28(Trick& move) const
{
  // ==== G28 ============= G30 =========== G52 =========== G54 ========
  //      AK+        |      A+       |      AK+      |      A+
  // Q+         J+   |  Q+      J+   |  J+      Q+   |  J+      Q+ 
  //      +          |      K+       |      +        |      +
  // ==== G28 ============= G30 =========== G52 =========== G54 ========

  if (length[QT_LHO] <= 2 && length[QT_RHO] <= 2)
    return false;

  PosType pa, pl, pp, pr;
  bool stiff = (length[QT_RHO] == 1 || length[QT_LHO] == 1);
  if (length[QT_RHO] == 1)
  {
    pa = QT_ACE;
    pl = QT_LHO;
    pp = QT_PARD;
    pr = QT_RHO;
  }
  else
  {
    pa = QT_PARD;
    pl = QT_RHO;
    pp = QT_ACE;
    pr = QT_LHO;
  }

  PosType pend = (htop.K == QT_ACE ? QT_ACE : QT_BOTH);

  if (! stiff && length[QT_LHO] + length[QT_RHO] >= 5)
  {
    // Axx / Hxx / Kxx / Hx.
    // Axx / Hx / Kxx / Hxx.
    if (pickFlag) holdCtr[0x280]++;
    return move.Set(QT_BOTH, pend, SDS_VOID-2, 2);
  }
  else if (hopp.T && (length[QT_LHO] >= 3 || length[QT_RHO] >= 3))
  {
    // Axx / H / Kxx / HHx.
    // Axx / HHx / Kxx / H.
    if (pickFlag) holdCtr[0x281]++;
    return move.Set(QT_BOTH, pend, SDS_VOID-2, 2);
  }
  else if (htop.K == QT_ACE)
  {
    if (length[QT_LHO] >= 3)
    {
      // AKx / Hxx / xxx / H.
      if (pickFlag) holdCtr[0x282]++;
      return move.Set(QT_BOTH, pend, SDS_VOID-2, 2);
    }
    else if (length[QT_RHO] >= 3 &&
      (length[QT_PARD] == 1 ||
        (completeList[QT_RHO][1] > completeList[QT_ACE][2] &&
           (length[QT_PARD] == 2 ||
             completeList[QT_RHO][1] > completeList[QT_PARD][1]))))
    {
      if (pickFlag) holdCtr[0x283]++;
      return move.Set(QT_BOTH, pend, SDS_VOID-2, 2);
    }
  }
  else if (length[pp] == 2 || (htop.T == pa && htop.N == pl))
  {
    // Ax+ / Hxx+ / Kx / H.
    // Ax / H / Kx+ / Hxx+.
    // A+ / H / KT+ / H9x+.
    // AT+ / H9x+ / K+ / H.
    if (pickFlag) holdCtr[0x284]++;
    return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-2, 2);
  }
  else if (htop.T != pa || htop.N != pa)
  {
    return false;
  }
  else if ((distHex == 0x4441 || distHex == 0x4144) && htop.E == pa)
  {
    // AT98 / Hxxx / Kxxx / H.
    // Axxx / H / KT98 / Hxxx.
    if (pickFlag) holdCtr[0x285]++;
    return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-7, 4);
  }
  else if (length[pa] >= 3)
  {
    if (htop.E == pl && length[pl] >= 4)
    {
      // AT9+ / H8xx+ / Kxx+ / H.
      if (pickFlag) holdCtr[0x286]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-6, 3);
    }
    else if (length[pa] == length[pp] &&
      (length[pa] == 3 || length[pl] == 3))
    {
      if (pickFlag) holdCtr[0x287]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-6, length[pa]);
    }
  }
  return false;
}


bool LoopHold::SolveSimple36(Trick& move) const
{
  // ==== G36 ==========================================================
  //      AK+     
  // +          Q+
  //      J+     
  // ==== G36 ==========================================================

  if (length[QT_LHO] >= 3)
  {
    // AKx / Qxx+ / J+ / ?.
    if (pickFlag) holdCtr[0x360]++;
    return move.Set(QT_BOTH, QT_ACE, SDS_VOID-2, 2);
  }
  else if (length[QT_PARD] <= length[QT_LHO] &&
    length[QT_ACE] >= 4 && length[QT_RHO] >= 5 &&
    completeList[QT_RHO][0] > completeList[QT_ACE][4] &&
    completeList[QT_RHO][0] < completeList[QT_ACE][3])
  {
    // AKxxx / xxxxx / x / Q(x).
    if (pickFlag) holdCtr[0x361]++;
    unsigned r = HR(QT_ACE, 3);
    return move.Set(QT_BOTH, QT_ACE, r, 4);
  }
  else if (length[QT_ACE] >= 4 && length[QT_PARD] >= 3 &&
    length[QT_ACE] > length[QT_PARD] && 
    (length[QT_RHO] == 2 || 
      (length[QT_RHO] == 3 && length[QT_PARD] == 3)) &&
    length[QT_LHO] == 1)
  {
    // AKxx / Q / Jxx / xx(x).
    if (pickFlag) holdCtr[0x362]++;
    return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-4, length[QT_ACE]);
  }
  else if (length[QT_ACE] == 3 && length[QT_PARD] >= 4 &&
    length[QT_RHO] <= 3 &&
    length[QT_LHO] <= 2 &&
    Max(length[QT_LHO], length[QT_RHO]) >= 2)
  {
    // AKx / Q(x) / Jxxx+ / x(xx).
    if (pickFlag) holdCtr[0x363]++;
    return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-4, length[QT_PARD]);
  }
  else if (length[QT_LHO] == 2)
  {
    if (length[QT_PARD] <= 2)
    {
      if (length[QT_RHO] >= 3 &&
        completeList[QT_RHO][0] > completeList[QT_ACE][2])
      {
        // With enough middle cards:
        // AKx / Qx / Jx / xxx.
        // AKx / QJ / xx / xxx.
        if (pickFlag) holdCtr[0x364]++;
        return move.Set(QT_BOTH, QT_ACE, SDS_VOID-2, 2);
      }
      else if (length[QT_RHO] >= 4 &&
        completeList[QT_RHO][0] < completeList[QT_ACE][2] &&
        completeList[QT_RHO][0] > completeList[QT_ACE][3])
      {
        // AKx+ / Qx / J(x) / xxxx+.
        if (pickFlag) holdCtr[0x365]++;
        unsigned r = HR(QT_ACE, 2);
        return move.Set(QT_BOTH, QT_ACE, r, 3);
      }
    }
    else if (length[QT_RHO] >= 4 &&
      completeList[QT_RHO][0] > completeList[QT_ACE][2] &&
      (length[QT_PARD] == 3 ||
        completeList[QT_RHO][0] > completeList[QT_PARD][1]))
    {
      // AKx+ / Qx / Jxx+ / Txxx.
      if (pickFlag) holdCtr[0x366]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-4, 3);
    }
  }
  else if (length[QT_PARD] == 1)
  {
    if (htop.T == QT_RHO && length[QT_RHO] >= 3)
    {
      // AKx / Q / J / Txx+.
      if (pickFlag) holdCtr[0x367]++;
      return move.Set(QT_BOTH, QT_ACE, SDS_VOID-2, 2);
    }
    else if (htop.T == QT_ACE && htop.N == QT_RHO && length[QT_RHO] >= 4)
    {
      // AKT+ / 9xxx+ / J / Q.
      if (pickFlag) holdCtr[0x368]++;
      return move.Set(QT_BOTH, QT_ACE, SDS_VOID-5, 3);
    }
  }
  else if (length[QT_PARD] >= 3 && length[QT_RHO] >= 4 &&
    htop.T == QT_RHO &&
    completeList[QT_RHO][1] > completeList[QT_ACE][2] &&
    (length[QT_PARD] == 3 ||
      completeList[QT_RHO][1] > completeList[QT_PARD][2]))
  {
    // AKx+ / Q / Jxx+ / Txxx+.
    if (pickFlag) holdCtr[0x369]++;
    return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-4, 3);
  }
  else if (distHex == 0x4144 && 
    htop.T == QT_RHO && htop.N == QT_PARD && htop.E == QT_PARD)
  {
    // AKxx / Q / J98x / Txxx.
    if (pickFlag) holdCtr[0x36a]++;
    return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-7, 4);
  }
  return false;
}


bool LoopHold::SolveSimple41(Trick& move) const
{
  // ==== G41 ==========================================================
  //      A+      
  // +          K+
  //      QJ+    
  // ==== G41 ==========================================================

  if (length[QT_LHO] >= 2)
  {
    // Protected K behind the ace.
    if (pickFlag) holdCtr[0x410]++;
    return move.Set(QT_BOTH, QT_ACE, SDS_ACE, 1);
  }
  else if (length[QT_ACE] == 2)
  {
    if (length[QT_PARD] >= 4 && length[QT_RHO] >= 5 && 
      htop.T == QT_PARD && htop.N == QT_RHO)
    {
      // Ax / 9xxxx / QJTxx / K.
      if (pickFlag) holdCtr[0x411]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-5, 4);
    }
    else if (length[QT_PARD] == 2)
    {
      if (length[QT_RHO] >= 2)
      {
        if (pickFlag) holdCtr[0x412]++;
        return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-3, 2);
      }
    }
    else if (length[QT_RHO] <= 2)
    {
      // Ax / K / QJx+ / xx.
      // Ax / K / QJ+ / xx+.
      if (pickFlag) holdCtr[0x413]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-3, length[QT_PARD]);
    }
    else if (length[QT_PARD] == 3 || 
      (length[QT_RHO] >= 4 && htop.T == QT_RHO))
    {
      // Ax / K / QJ+ / Txx+.
      if (pickFlag) holdCtr[0x414]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-4, 3);
    }
    else if (length[QT_RHO] == 3)
    {
      // Ax / K / QJxx+ / xxx.
      if (pickFlag) holdCtr[0x415]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-4, length[QT_PARD]);
    }
  }
  else if (length[QT_PARD] == 2)
  {
    if (htop.T == QT_RHO && length[QT_RHO] >= 3)
    {
      // Axx+ / Txx+ / QJ / K.
      if (pickFlag) holdCtr[0x416]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-3, 2);
    }
  }
  else if (length[QT_RHO] >= 4 &&
    completeList[QT_RHO][0] > completeList[QT_ACE][1] &&
    (length[QT_PARD] == 3 ||
      completeList[QT_RHO][0] > completeList[QT_PARD][2]))
  {
    // Axx / K / QJT / 9xxx.
    // Axxx / K / QJx / Txxx+.
    // Axx+ / K / QJx+ / Txxx+.
    if (pickFlag) holdCtr[0x417]++;
    return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-4, 3);
  }
  else if (length[QT_ACE] == 3 && length[QT_PARD] >= 4 &&
    length[QT_RHO] <= 3)
  {
    if (length[QT_RHO] <= 1)
    {
      // Axx / K / QJxx+ / x.
      if (pickFlag) holdCtr[0x418]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-3, length[QT_PARD]);
    }
    else
    {
      // Axx / K / QJxx+ / xx(x).
      if (pickFlag) holdCtr[0x419]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-4, length[QT_PARD]);
    }
  }

  return false;
}


bool LoopHold::SolveSimple43(Trick& move) const
{
  // ==== G43 ==========================================================
  //      A+      
  // K+         + 
  //      QJ+    
  // ==== G43 ==========================================================

  if (length[QT_RHO] >= 2)
    return false;
  else if (length[QT_ACE] == 2)
  {
    if (length[QT_PARD] == 2)
    {
      if (length[QT_LHO] >= 2)
      {
        if (pickFlag) holdCtr[0x430]++;
        return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-3, 2);
      }
    }
    else if (length[QT_PARD] >= 4 && length[QT_LHO] >= 5 && 
      htop.T == QT_PARD && htop.N == QT_LHO &&
      completeList[QT_LHO][1] > completeList[QT_PARD][3])
    {
      // Ax / K / QJTx+ / 9xxxx.
      if (pickFlag) holdCtr[0x431]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-5, 4);
    }
    // else if (length[QT_LHO] <= 2 || length[QT_PARD] == 2)
    else if (length[QT_LHO] <= 2)
    {
      // Ax / K / QJx+ / xx.
      // Ax / K / QJ+ / xx+.
      if (pickFlag) holdCtr[0x432]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-3, length[QT_PARD]);
    }
    else if (length[QT_PARD] == 3 || 
      (length[QT_LHO] >= 4 && htop.T == QT_LHO &&
        completeList[QT_LHO][1] > completeList[QT_PARD][2]))
    {
      // Ax / K / QJ+ / Txx+.
      if (pickFlag) holdCtr[0x433]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-4, 3);
    }
  }
  else if (length[QT_PARD] == 2)
  {
    if (htop.T == QT_LHO && length[QT_LHO] >= 3)
    {
      // Axx+ / K / QJ / Txx+.
      if (pickFlag) holdCtr[0x434]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-3, 2);
    }
  }
  else if (length[QT_PARD] == 3)
  {
    if (length[QT_ACE] >= 4 && length[QT_LHO] >= 4 && 
      completeList[QT_LHO][0] > completeList[QT_ACE][1])
    {
      // Axx / ? / QJx / K.
      if (pickFlag) holdCtr[0x435]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-4, 3);
    }
  }
  else if (length[QT_ACE] == 4 && length[QT_LHO] >= 4 &&
    length[QT_PARD] == 4 && 
    htop.T == QT_LHO && htop.N == QT_ACE && htop.E == QT_ACE)
  {
    // A98x / Txxx / QJxx / K.
    if (pickFlag) holdCtr[0x436]++;
    return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-7, 4);
  }
  else if (length[QT_LHO] >= 4 && htop.T == QT_LHO &&
    LoopHold::StopFinesse(2, 1, 2, true, QT_PARD))
  {
    // Axx / Txxx / QJx(x) / K.
    if (pickFlag) holdCtr[0x437]++;
    return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-4, 3);
  }
  else if (length[QT_ACE] == 3 && length[QT_PARD] >= 4 &&
    length[QT_LHO] <= 3)
  {
    if (length[QT_LHO] <= 1)
    {
      // Axx / K / QJxx+ / x.
      if (pickFlag) holdCtr[0x438]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-3, length[QT_PARD]);
    }
    else
    {
      // Axx / K / QJxx+ / xx(x).
      if (pickFlag) holdCtr[0x439]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-4, length[QT_PARD]);
    }
  }
  else if (distHex == 0x4351 && 
    hopp.T && htop.N == QT_ACE && htop.E == QT_ACE)
  {
    // A98x / Txx / QJxxx / K.
    if (pickFlag) holdCtr[0x43a]++;
    return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-7, 5);
  }
  return false;
}


bool LoopHold::SolveSimple44(Trick& move) const
{
  // ==== G44 ==========================================================
  //      AK+     
  // Q+         + 
  //      J+    
  // ==== G44 ==========================================================

  if (length[QT_PARD] == 1)
  {
    if (length[QT_RHO] <= 2 &&
      length[QT_ACE] >= 4 && length[QT_LHO] >= 5 &&
      completeList[QT_LHO][0] > completeList[QT_ACE][4] &&
      completeList[QT_LHO][0] < completeList[QT_ACE][3])
    {
      // AKxxx / xxxxx / x / Q(x).
      if (pickFlag) holdCtr[0x440]++;
      unsigned r = HR(QT_ACE, 3);
      return move.Set(QT_BOTH, QT_ACE, r, 4);
    }
    else if (htop.T == QT_ACE && htop.N == QT_LHO &&
      length[QT_RHO] == 1 && length[QT_LHO] >= 4)
    {
      // AKT+ / 9xxx+ / J / Q.
      if (pickFlag) holdCtr[0x441]++;
      return move.Set(QT_BOTH, QT_ACE, SDS_VOID-5, 3);
    }
    else if ((length[QT_RHO] >= 3 &&
      completeList[QT_RHO][1] > completeList[QT_ACE][2]) ||
      (hopp.T && length[htop.T] >= 3))
    {
      // AK+ / Txx+ / J / Q+.
      // AK+ / QTx+ / J / +.
      // AK+ / Q9x+ / J / Tx.
      if (pickFlag) holdCtr[0x442]++;
      return move.Set(QT_BOTH, QT_ACE, SDS_VOID-2, 2);
    }
  }

  if (length[QT_ACE] >= 4 && length[QT_PARD] >= 3 &&
    length[QT_ACE] > length[QT_PARD] && 
    (length[QT_LHO] == 2 || 
      (length[QT_LHO] == 3 && length[QT_PARD] == 3)) &&
    length[QT_RHO] == 1)
  {
    // AKxx / xx(x) / Jxx / Q.
    if (pickFlag) holdCtr[0x443]++;
    return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-4, length[QT_ACE]);
  }
  else if (length[QT_ACE] == 3 && length[QT_PARD] >= 4 &&
    length[QT_LHO] <= 3 &&
    length[QT_RHO] <= 2 &&
    Max(length[QT_LHO], length[QT_RHO]) >= 2)
  {
    // AKx / xx(x) / Jxxx+ / Q(x).
    if (pickFlag) holdCtr[0x444]++;
    return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-4, length[QT_PARD]);
  }
  else if (length[QT_RHO] == 1)
  {
    if (length[QT_LHO] >= 4 && length[QT_PARD] >= 3 && 
      htop.T == QT_LHO && 
      completeList[QT_LHO][1] > completeList[QT_PARD][1] &&
      completeList[QT_LHO][1] > completeList[QT_ACE][3])
    {
      // AKx / Txxx / Jxx / Q.
      if (pickFlag) holdCtr[0x445]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-4, 3);
    }
    else if (distHex == 0x4441 && htop.T == QT_LHO &&
      (htop.N == QT_PARD || (htop.N == QT_ACE && ! hopp.E)))
    {
      // AKxx / Txxx / J9xx / Q.
      // AK98 / Txxx / Jxxx / Q.
      // AK9x / Txxx / J8xx / Q.
      if (pickFlag) holdCtr[0x446]++;
      unsigned r = (htop.N == QT_PARD ? SDS_VOID-6 : SDS_VOID-7);
      return move.Set(QT_BOTH, QT_BOTH, r, 4);
    }
    else if (distHex == 0x5341 && htop.T == QT_LHO && htop.N == QT_PARD)
    {
      // AKxxx / Txx / J9xx / Q.
      if (pickFlag) holdCtr[0x447]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-6, 5);
    }
  }
  else if (htop.T == QT_ACE)
  {
    if (length[QT_LHO] >= 4 && length[QT_PARD] <= 2 && 
      length[QT_RHO] == 2 && 
      completeList[QT_LHO][0] > completeList[QT_ACE][3] &&
      (completeList[QT_LHO][0] > completeList[QT_PARD][1] ||
       completeList[QT_RHO][1] > completeList[QT_PARD][1]))
    {
      // AKT+ / xxxx+ / J(x) / Qx with enough middle cards.
      if (pickFlag) holdCtr[0x448]++;
      return move.Set(QT_BOTH, QT_ACE, SDS_VOID-5, 3);
    }
  }
  else if (htop.T == QT_PARD)
  {
    if (distHex == 0x4432 && htop.N == QT_LHO && (hopp.E || 
      (completeList[QT_LHO][1] > completeList[QT_PARD][2] &&
       completeList[QT_LHO][1] > completeList[QT_ACE][3])))
    {
      // AKxx / 9xxx / / JTx / Qx.
      if (pickFlag) holdCtr[0x449]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-4, 3);
    }
  }
  else if (length[QT_ACE] == 3 && length[QT_PARD] == 3 &&
    length[QT_RHO] <= 2 && length[QT_LHO] >= 3)
  {
    // AKx / + / Jxx / Q(x).
    if (pickFlag) holdCtr[0x44a]++;
    return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-4, 3);
  }
  else if (length[QT_ACE] >= 4 && length[QT_LHO] >= 4 &&
    length[QT_PARD] == 3)
  {
    if (completeList[QT_LHO][0] > completeList[QT_ACE][2])
    {
      // AKxx / xxxx / Jxx / QT.
      if (pickFlag) holdCtr[0x44b]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-4, 3);
    }
  }
  else if (distHex == 0x3442 && 
    (htop.T == QT_LHO || (htop.T == QT_RHO && htop.N == QT_LHO)) &&
    LoopHold::StopFinesse(2, 2, 1, true, QT_PARD))
  {
    // AKx / Txxx / Jxxx / Qx.
    if (pickFlag) holdCtr[0x44c]++;
    return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-4, 3);
  }
  else if (length[QT_LHO] >= 3 && length[QT_RHO] >= 2 && 
    htop.T == QT_LHO)
  {
    if (length[QT_PARD] <= 2 || (length[QT_RHO] >= 3))
    {
      // AKx / Txx+ / J(x) / Qx.
      if (pickFlag) holdCtr[0x44d]++;
      return move.Set(QT_BOTH, QT_ACE, SDS_VOID-2, 2);
    }
  }
  else if (length[QT_LHO] >= 3 && length[QT_RHO] >= 2 && 
    htop.T == QT_RHO && length[QT_PARD] <= 2 &&
    completeList[QT_ACE][2] < 
      Max(completeList[QT_LHO][0], completeList[QT_RHO][2]))
  {
    // AKx / 9xx+ / J(x) / QT.
    if (pickFlag) holdCtr[0x44e]++;
    return move.Set(QT_BOTH, QT_ACE, SDS_VOID-2, 2);
  }
  else if (length[QT_RHO] >= 3)
  {
    if (length[QT_LHO] == 2 &&
      (htop.T == QT_LHO || htop.N == QT_LHO) &&
      completeList[QT_RHO][1] > completeList[QT_ACE][2] &&
      completeList[QT_RHO][1] > completeList[QT_PARD][1])
    {
      // AKx / Tx / Jxx / Qxx.
      if (pickFlag) holdCtr[0x44f]++;
      return move.Set(QT_BOTH, QT_ACE, SDS_VOID-2, 2);
    }
    else if (length[QT_LHO] <= 2 && length[QT_PARD] >= 2 &&
      LoopHold::StopFinesse(2, 2, 0, false, QT_ACE))
    {
      // AKx+ / h / Jx+ / Qhx+.
      // AKx+ / ? / Jx+ / Qhx+.
      if (pickFlag) holdCtr[0x450]++;
      return move.Set(QT_BOTH, QT_ACE, SDS_VOID-2, 2);
    }
    else if (length[QT_LHO] >= 3 && 
      (hopp.N || (htop.N == QT_PARD && hopp.E)))
    {
      // AKx / 8xx / J9+ / QTx.
      // AKx / Txx / J9+ / Q8x.
      // AKx / T8x / J9+ / Qxx.
      if (pickFlag) holdCtr[0x451]++;
      return move.Set(QT_BOTH, QT_ACE, SDS_VOID-2, 2);
    }
  }
  else if (length[QT_LHO] >= 4 && length[QT_PARD] <= 2)
  {
    if (completeList[QT_LHO][0] < completeList[QT_ACE][2] &&
        completeList[QT_LHO][0] > completeList[QT_ACE][3])
    {
      // AK9x / 8xxx / J(x) / QT.
      // AK9x / 7xxx / J8 / QT.
      // AK8x / 7xxx / J9 / QT.
      if (pickFlag) holdCtr[0x452]++;
      unsigned r = HR(QT_ACE, 2);
      return move.Set(QT_BOTH, QT_ACE, r, 3);
    }
  }
  return false;
}


bool LoopHold::SolveSimple48(Trick& move) const
{
  // ==== G48 ==========================================================
  //      AKQ+    
  // J+         + 
  //      +     
  // ==== G48 ==========================================================

  if ((length[QT_LHO] >= 4 || length[QT_RHO] >= 4) &&
    hopp.T && hopp.N && hopp.E)
  {
    if (pickFlag) holdCtr[0x480]++;
    return move.Set(QT_BOTH, QT_ACE, SDS_VOID-3, 3);
  }
  else if (length[QT_LHO] >= 5 && length[QT_PARD] <= 2 && 
    completeList[QT_LHO][0] < completeList[QT_ACE][3] &&
    completeList[QT_LHO][0] > completeList[QT_ACE][4] &&
    (length[QT_PARD] == 1 ||
    completeList[QT_LHO][0] > completeList[QT_PARD][0]))
  {
    // AHHxx / xxxxx / Hx / H.
    if (pickFlag) holdCtr[0x481]++;
    unsigned r = HR(QT_ACE, 3);
    return move.Set(QT_BOTH, QT_ACE, r, 4);
  }
  else if (length[QT_PARD] == 1)
  {
    if ((length[QT_RHO] >= 4 &&
        completeList[QT_RHO][1] > completeList[QT_ACE][3]) ||
      (length[QT_LHO] >= 4 &&
        completeList[QT_LHO][0] > completeList[QT_ACE][3]))
    {
      // AKQ+ / ? / x / JTxx.
      // AKQ+ / ? / T / J9xx.
      // AKQ+ / xxxx / x / J+ with enough middle cards.
      if (pickFlag) holdCtr[0x482]++;
      return move.Set(QT_BOTH, QT_ACE, SDS_VOID-3, 3);
    }
  }
  else if (length[QT_RHO] >= 4 && 
    LoopHold::StopFinesse(3, 3, 0, false, QT_ACE))
  {
    // All kinds of combinations such as
    // AKQ3 / 2 / T864 / J975.
    if (pickFlag) holdCtr[0x483]++;
    return move.Set(QT_BOTH, QT_ACE, SDS_VOID-3, 3);
  }
  else if (length[QT_LHO] >= 4 && length[QT_PARD] == 2 && 
    length[QT_RHO] >= 2 &&
    completeList[QT_LHO][0] > completeList[QT_ACE][3] &&
    (completeList[QT_RHO][1] > completeList[QT_PARD][1] ||
     completeList[QT_LHO][0] > completeList[QT_PARD][1]))
  {
    // AKQ+ / xxxx / xx / Jx with enough middle cards.
    if (pickFlag) holdCtr[0x484]++;
    return move.Set(QT_BOTH, QT_ACE, SDS_VOID-3, 3);
  }
  return false;
}


bool LoopHold::SolveSimple49(Trick& move) const
{
  // ==== G49 ==========================================================
  //      AQ+     
  // J+         K+
  //      +     
  // ==== G49 ==========================================================

  if (length[QT_LHO] >= 2)
  {
    // Protected K behind the ace.
    if (pickFlag) holdCtr[0x490]++;
    return move.Set(QT_BOTH, QT_ACE, SDS_ACE, 1);
  }
  else if (length[QT_RHO] == 1 && length[QT_PARD] == 1)
  {
    // Ax / K / x / J.
    if (pickFlag) holdCtr[0x491]++;
    return move.Set(QT_BOTH, QT_ACE, SDS_ACE, length[QT_ACE]);
  }
  else if (length[QT_ACE] == 2 && length[QT_PARD] <= 2 &&
    length[QT_RHO] <= 2)
  {
    // AQ / K / x(x) / J(x).
    if (pickFlag) holdCtr[0x492]++;
    unsigned r = HR(QT_ACE, length[QT_RHO]-1);
    return move.Set(QT_BOTH, QT_ACE, r, 2);
  }
  else if (length[QT_RHO] >= 3 &&
    (length[QT_ACE] == 2 || 
     length[QT_PARD] == 1 ||
     (completeList[QT_RHO][1] > completeList[QT_ACE][2] && 
       (length[QT_PARD] == 2 ||
          completeList[QT_RHO][1] > completeList[QT_PARD][1]))))
  {
    // AQ+  / Jxx+ / x+ / K.
    // AQ   / K    / x+ / Jxx+.
    // AQx+ / K    / x  / Jxx+.
    // AQx+ / K    / xx / Jxx with middle cards.
    if (pickFlag) holdCtr[0x493]++;
    return move.Set(QT_BOTH, QT_ACE, SDS_VOID-3, 2);
  }
  return false;
}


bool LoopHold::SolveSimple57(Trick& move) const
{
  // ==== G57 ==========================================================
  //      A+      
  // J+         K+
  //      Q+    
  // ==== G57 ==========================================================

  if (length[QT_LHO] >= 2)
  {
    // A+ / Kx+ / Q+ / J+.
    if (pickFlag) holdCtr[0x570]++;
    return move.Set(QT_BOTH, QT_ACE, SDS_VOID-1, 1);
  }
  else if (length[QT_PARD] == 1)
  {
    if (length[QT_RHO] == 1)
    {
      // A+ / K / Q / J.
      if (pickFlag) holdCtr[0x571]++;
      return move.Set(QT_BOTH, QT_ACE, SDS_VOID-1, length[QT_ACE]);
    }
    else
    {
      // A+ / K / Q / Jx+.
      if (pickFlag) holdCtr[0x572]++;
      return move.Set(QT_BOTH, QT_ACE, SDS_VOID-1, 1);
    }
  }
  else if (length[QT_RHO] >= 3)
  {
    // A+ / K / Q+ / Jxx+.
    if (pickFlag) holdCtr[0x573]++;
    return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-3, 2);
  }
  else if (length[QT_ACE] == 2 && length[QT_PARD] == 2 &&
    length[QT_RHO] == 2)
    // (length[QT_RHO] == 2 || htop.T == QT_PARD))
  {
    // Ax / K / Qx / Jx.
    if (pickFlag) holdCtr[0x574]++;
    return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-3, 2);
  }
  return false;
}


bool LoopHold::SolveSimple60(Trick& move) const
{
  // ==== G60 ============= G62 ========================================
  //      AK+        |      A+     
  // QJ+        +    | QJ+      +  
  //      +          |      K+     
  // ==== G60 ============= G62 ========================================

  PosType pend = (htop.K == QT_ACE ? QT_ACE : QT_BOTH);
  unsigned x = (htop.K == QT_ACE ? 2 : 1);

  if (htop.K == QT_ACE && length[QT_PARD] >= 2 && length[QT_RHO] >= 3)
  {
    if (! hopp.T && ! hopp.N && ! hopp.E)
    {
      // AKx / xxx / T98 / QJx.
      return false;
    }
    else if (completeList[QT_ACE][2] > completeList[QT_RHO][2] &&
      completeList[QT_ACE][2] > completeList[QT_LHO][0])
    {
      // QJ directly finessable.
      return false;
    }
    else if (length[QT_LHO] >= 3 &&
      completeList[QT_LHO][0] > completeList[QT_ACE][2] &&
      completeList[QT_LHO][0] > completeList[QT_PARD][2])
    {
      if (pickFlag) holdCtr[0x600]++;
      return move.Set(QT_BOTH, QT_ACE, SDS_VOID-2, 2);
    }
    else if (length[QT_LHO] >= 2 && htop.T == QT_PARD &&
      htop.N == QT_LHO)
    {
      // AKx+ / 9x+ / Tx+ / QJx+.
      if (pickFlag) holdCtr[0x601]++;
      return move.Set(QT_BOTH, QT_ACE, SDS_VOID-2, 2);
    }
    else if (LoopHold::StopFinesse(2, 2, 0, true, QT_ACE))
    {
      if (pickFlag) holdCtr[0x602]++;
      return move.Set(QT_BOTH, QT_ACE, SDS_VOID-2, 2);
    }
  }
  else if (length[QT_RHO] >= 3 || (length[QT_LHO] >= 3 &&
    (htop.T == QT_LHO || ((length[QT_PARD] <= 2 &&
      completeList[QT_LHO][0] > completeList[QT_ACE][x]) ||
        (length[QT_ACE] == 2 && 
          completeList[QT_LHO][0] > completeList[QT_PARD][1])))))
  {
    if (pickFlag) holdCtr[0x603]++;
    return move.Set(QT_BOTH, pend, SDS_VOID-2, 2);
  }
  else if (length[QT_ACE] >= 3 &&
    length[QT_PARD] <= 2 && length[QT_LHO] >= 4 && 
    completeList[QT_LHO][0] > completeList[QT_ACE][x+1] &&
    completeList[QT_LHO][0] < completeList[QT_ACE][x])
  {
    // AKxx / xxxx / xx / QJ.
    // Axxx / xxxx / Kx / QJ.
    if (pickFlag) holdCtr[0x604]++;
    unsigned r = HR(QT_ACE, x);
    return move.Set(QT_BOTH, pend, r, 3);
  }
  else if (length[QT_PARD] >= 3 &&
    length[QT_ACE] <= 2 && length[QT_LHO] >= 4 && 
    completeList[QT_LHO][0] > completeList[QT_PARD][2] &&
    completeList[QT_LHO][0] < completeList[QT_PARD][1])
  {
    // Ax / xxxx / Kxxx / QJ.
    if (pickFlag) holdCtr[0x605]++;
    unsigned r = HR(QT_PARD, 1);
    return move.Set(QT_BOTH, pend, r, 3);
  }
  else if (length[QT_ACE] >= 4 &&
    length[QT_PARD] <= 2 && length[QT_LHO] >= 5 &&
    completeList[QT_LHO][0] > completeList[QT_ACE][x+2] &&
    completeList[QT_LHO][0] < completeList[QT_ACE][x+1])
  {
    // AKT9x / 8xxxx / xx / QJ.
    // AT9xx / 8xxxx / Kx / QJ.
    if (pickFlag) holdCtr[0x606]++;
    unsigned r = HR(QT_ACE, x+1);
    return move.Set(QT_BOTH, pend, r, 4);
  }

  if (length[QT_LHO] <= 3)
    return false;

  if (distHex == 0x4432)
  {
    unsigned xx = 1;
    if (htop.K == QT_ACE) xx++;
    if (htop.T == QT_ACE) xx++;

    if (completeList[QT_LHO][0] > completeList[QT_ACE][xx] &&
      (htop.T == QT_PARD || (htop.T == QT_ACE && 
        completeList[QT_LHO][0] > completeList[QT_PARD][3-xx])))
    {
      // AKTx / 9xxx / xxx / QJ.
      // ATxx / 9xxx / Kxx / QJ.
      // AKxx / 9xxx / Txx / QJ.
      // Axxx / 9xxx / KTx / QJ.
      if (pickFlag) holdCtr[0x607]++;
      PosType pe = (htop.K == QT_PARD || htop.T == QT_PARD ?
        QT_BOTH : QT_ACE);
      return move.Set(QT_BOTH, pe, SDS_VOID-5, 3);
    }
  }
  else if (distHex == 0x3442 && ! hopp.T && 
    ((htop.K == QT_PARD && htop.T == QT_ACE && htop.N != QT_PARD) ||
     ((htop.K == QT_ACE || htop.T == QT_PARD) && hopp.N)))
  {
    unsigned xx = 0;
    if (htop.K == QT_PARD) xx++;
    if (htop.T == QT_PARD) xx++;

    if (completeList[QT_LHO][0] > completeList[QT_PARD][xx] &&
      (htop.T == QT_ACE ||
        completeList[QT_LHO][1] > completeList[QT_PARD][xx]))
    {
      // AKT / 9xxx / xxxx / QJ.
      // ATx / 9xxx / Kxxx / QJ.
      // AKx / 9xxx / Txxx / QJ.
      // Axx / 9xxx / KTxx / QJ.
      if (pickFlag) holdCtr[0x608]++;
      PosType e = (xx == 0 ? QT_ACE : QT_BOTH);
      return move.Set(QT_BOTH, e, SDS_VOID-5, 3);
    }
  }
  return false;
}

#include "portab.h"

bool LoopHold::SolveComplex0(DefList& def, unsigned& rank) const
{
  // ==== G0 ===========================================================
  //      AKQJ+
  // +          +
  //      +
  // ==== G0 ===========================================================

  Trick trick[6];
  unsigned l;
  if (distHex == 0x6016)
  {
    if (htop.T == QT_ACE && htop.N == QT_RHO && htop.E == QT_ACE)
    {
      // AKQJT8 / - / x / 9xxxxx.
      if (pickFlag) holdCtr[0x1000]++;
      rank = SDS_EIGHT;
      trick[0].Set(QT_ACE, QT_ACE, SDS_TEN, 5);
      trick[1].Set(QT_PARD, QT_ACE, rank, 6);
      return def.Set11(trick[0], trick[1]);
    }
    else if (htop.T == QT_RHO && htop.N == QT_ACE && htop.E == QT_PARD)
    {
      // AKQJ9x / - / 8 / Txxxxx.
      return false;
    }
  }
  else if (distHex == 0x6025)
  {
    if (((htop.N == QT_ACE && htop.E == QT_PARD) ||
         (htop.N == QT_PARD && htop.E == QT_ACE)) &&
        completeList[QT_PARD][1] > completeList[QT_ACE][5] &&
        completeList[QT_PARD][1] > completeList[QT_RHO][1])
    {
      // AKQJ9x / - / 87 / Txxxx.
      // AKQJ8x / - / 97 / Txxxx.
      if (pickFlag) holdCtr[0x1001]++;
      rank = HR(QT_PARD, 1);
      unsigned r1 = HR(QT_ACE, 4);
      trick[0].Set(QT_ACE, QT_ACE, SDS_JACK, 4);
      trick[1].Set(QT_PARD, QT_ACE, r1, 6);
      trick[2].Set(QT_PARD, QT_PARD, rank, 2);
      trick[3].Set(QT_ACE, QT_ACE, SDS_VOID, 4);
      return def.Set112(trick);
    }
  }
  else if (distHex == 0x5125)
  {
    if (htop.T == QT_LHO && htop.N == QT_PARD && htop.E == QT_RHO &&
      completeList[QT_ACE][4] > completeList[QT_PARD][1] &&
      completeList[QT_ACE][4] > completeList[QT_RHO][1])
    {
      // AKQJ7 / T / 9x / 8xxxx.
      if (pickFlag) holdCtr[0x1001]++;
      rank = HR(QT_ACE, 4);
      trick[0].Set(QT_BOTH, QT_ACE, SDS_JACK, 4);
      trick[1].Set(QT_BOTH, QT_ACE, SDS_ACE, 1);
      trick[2].Set(QT_PARD, QT_ACE, rank, 4);
      trick[3].Set(QT_BOTH, QT_PARD, SDS_NINE, 2);
      trick[4].Set(QT_ACE, QT_ACE, SDS_VOID, 3);
      return def.Set122(trick);
    }
  }

  if (length[QT_RHO] <= 4 || htop.T != QT_RHO)
    return false;

  if (length[QT_PARD] == 1)
  {
    if (! LoopHold::StopFinesse(1, 4, 0, true, QT_ACE) && htop.N == QT_ACE)
    {
      // AKQJ9+ / ? / x / Txxxx+.
      if (pickFlag) holdCtr[0x1002]++;
      rank = SDS_NINE;
      l = (length[QT_RHO] == 5 ? length[QT_ACE] : 5);
      trick[0].Set(QT_ACE, QT_ACE, SDS_JACK, 4);
      trick[1].Set(QT_PARD, QT_ACE, rank, l);
      return def.Set11(trick[0], trick[1]);
    }
  }

  else if (length[QT_PARD] == 3) // 0x5035
  {
    if (! LoopHold::StopFinesse(1, 4, 1, true, QT_ACE) ||
       (htop.N == QT_PARD && htop.E == QT_PARD))
    {
      if (pickFlag) holdCtr[0x1003]++;
      PosType e = QT_ACE;
      if (htop.N == QT_PARD && htop.E == QT_PARD)
      {
        rank = SDS_EIGHT;
        e = QT_BOTH;
      }
      else
      {
        rank = HR(QT_ACE, 4);
        e = QT_ACE;
      }

      trick[0].Set(QT_ACE, QT_ACE, SDS_JACK, 4);
      trick[1].Set(QT_PARD, e, rank, 5);
      return def.Set11(trick[0], trick[1]);
    }
    else if (! LoopHold::StopFinesse(2, 4, 1, true, QT_ACE))
    {
      if (completeList[QT_PARD][1] > completeList[QT_ACE][4])
      {
        if (completeList[QT_PARD][2] > completeList[QT_ACE][4] &&
            completeList[QT_ACE][4] > completeList[QT_RHO][2])
        {
          // AKQJ5 / - / 976 / T8432.
          if (pickFlag) holdCtr[0x1004]++;
          rank = HR(QT_ACE, 4);
          unsigned ra1 = HR(QT_PARD, 2);
          trick[0].Set(QT_BOTH, QT_ACE, SDS_JACK, 4);
          trick[1].Set(QT_PARD, QT_ACE, SDS_ACE, 1);
          trick[2].Set(QT_PARD, QT_ACE, rank, 4);
          trick[3].Set(QT_PARD, QT_ACE, SDS_ACE, 1);
          trick[4].Set(QT_PARD, QT_PARD, ra1, 2);
          trick[5].Set(QT_ACE, QT_ACE, SDS_VOID, 2);
          return def.Set123(trick);
        }
        else
        {
          // PROBLEM? AKQJ6 / - / 872 / T9543.
          // Perhaps really mixed.
          return false;
        }
      }
      else
      {
        // AKQJ8 / - / xxx / T9xxx.
        if (pickFlag) holdCtr[0x1004]++;
        rank = HR(QT_ACE, 4);
        trick[0].Set(QT_BOTH, QT_ACE, SDS_JACK, 4);
        trick[1].Set(QT_PARD, QT_ACE, SDS_ACE, 1);
        trick[2].Set(QT_PARD, QT_ACE, rank, 4);
        return def.Set12(trick[0], trick[1], trick[2]);
      }
    }
    else if (completeList[QT_PARD][2] < completeList[QT_ACE][4])
    {
      // AKQJ4 / - / 765 / T9832.
      if (pickFlag) holdCtr[0x1005]++;
      rank = HR(QT_ACE, 4);
      trick[0].Set(QT_BOTH, QT_ACE, SDS_JACK, 4);
      trick[1].Set(QT_PARD, QT_ACE, SDS_ACE, 1);
      trick[2].Set(QT_PARD, QT_ACE, SDS_KING, 1);
      trick[3].Set(QT_PARD, QT_ACE, rank, 3);
      return def.Set13(trick);
    }
  }

  else if (! LoopHold::StopFinesse(1, 4, 0, false, QT_ACE) &&
      completeList[QT_PARD][1] < completeList[QT_ACE][4])
  {
    if (length[QT_LHO] == 0 || 
        completeList[QT_LHO][0] < completeList[QT_ACE][4])
    {
      // AKQJ9 / ? / x(xx) / Txxxx+.
      if (pickFlag) holdCtr[0x1006]++;
      rank = HR(QT_ACE, 4);
      l = (length[QT_RHO] == 5 ? length[QT_ACE] : 5);
      trick[0].Set(QT_ACE, QT_ACE, SDS_JACK, 4);
      trick[1].Set(QT_PARD, QT_ACE, rank, l);
      return def.Set11(trick[0], trick[1]);
    }
    else if (completeList[QT_LHO][0] > completeList[QT_PARD][0])
    {
      // AKQJ8 / T / xx / 9xxxx.
      if (pickFlag) holdCtr[0x1007]++;
      rank = HR(QT_ACE, 4);
      trick[0].Set(QT_BOTH, QT_ACE, SDS_JACK, 4);
      trick[1].Set(QT_BOTH, QT_ACE, SDS_ACE, 1);
      trick[2].Set(QT_PARD, QT_ACE, rank, 4);
      return def.Set12(trick[0], trick[1], trick[2]);
    }
    else
    {
      // AKQJ7 / 8 / 95 / T6432.
      if (pickFlag) holdCtr[0x1008]++;
      rank = HR(QT_ACE, 4);
      trick[0].Set(QT_ACE, QT_ACE, SDS_JACK, 4);
      trick[1].Set(QT_PARD, QT_ACE, rank, 5);
      trick[2].Set(QT_ACE, QT_ACE, SDS_ACE, 1);
      trick[3].Set(QT_PARD, QT_ACE, rank, 4);
      return def.Set112(trick);
    }
  }
  else if (htop.T == QT_RHO && htop.N == QT_PARD && htop.E == QT_PARD)
  {
    if (completeList[QT_RHO][1] > completeList[QT_ACE][4])
    {
      // AKQJ2 / 4 / 98 / T7xxx.
      if (pickFlag) holdCtr[0x1009]++;
      rank = SDS_EIGHT;
      l = (length[QT_RHO] == 5 ? length[QT_ACE] : 5);
      trick[0].Set(QT_BOTH, QT_ACE, SDS_JACK, 4);
      trick[1].Set(QT_PARD, QT_PARD, rank, 2);
      trick[2].Set(QT_ACE, QT_ACE, SDS_VOID, l-2);
      return def.Set12(trick[0], trick[1], trick[2]);
    }
    else if (length[QT_LHO] == 0 ||
        completeList[QT_LHO][0] < completeList[QT_ACE][4])
    {
      // AKQJ7 / ? / 98 / Txxxx.
      if (pickFlag) holdCtr[0x100a]++;
      rank = HR(QT_ACE, 4);
      l = (length[QT_RHO] == 5 ? length[QT_ACE] : 5);
      trick[0].Set(QT_ACE, QT_ACE, SDS_JACK, 4);
      trick[1].Set(QT_PARD, QT_ACE, rank, l);
      trick[2].Set(QT_PARD, QT_PARD, SDS_EIGHT, 2);
      trick[3].Set(QT_ACE, QT_ACE, SDS_VOID, l-2);
      return def.Set112(trick);
    }
  }
  else if (! LoopHold::StopFinesse(2, 4, 0, true, QT_ACE) &&
      completeList[QT_PARD][1] < completeList[QT_ACE][4])
  {
    // AKQJ73 / - / 92 / T8654.
    if (pickFlag) holdCtr[0x100b]++;
    rank = HR(QT_ACE, 4);
    l = (length[QT_RHO] == 5 ? length[QT_ACE] : 5);
    trick[0].Set(QT_BOTH, QT_ACE, SDS_JACK, 4);
    trick[1].Set(QT_PARD, QT_ACE, SDS_ACE, 1);
    trick[2].Set(QT_PARD, QT_ACE, rank, l-1);
    return def.Set12(trick[0], trick[1], trick[2]);
  }

  return false;
}


bool LoopHold::SolveComplex1(DefList& def, unsigned& rank) const
{
  // ==== G1 ================= G4 ================= G16 ================
  //      AQJ+       |         AKJ+       |         AKQ+
  // +          K+   |    +          Q+   |    +          J+
  //      +          |         +          |         + 
  // ==== G1 ================= G4 ================= G16 ================

  Trick trick[6];
  unsigned l;

  if (distHex == 0x4234)
  {
    if (completeList[QT_ACE][3] > completeList[QT_PARD][0] &&
        completeList[QT_ACE][3] > completeList[QT_RHO][0])
      return false;

    if (completeList[QT_ACE][3] > completeList[QT_PARD][2] &&
        completeList[QT_ACE][3] > completeList[QT_RHO][1])
    {
      if (completeList[QT_PARD][0] > completeList[QT_ACE][3] &&
          completeList[QT_PARD][0] > completeList[QT_RHO][0])
      {
        if (completeList[QT_ACE][3] < completeList[QT_RHO][0])
        {
          // AKQ2 / JT / 9xx / 8xxx.
          if (pickFlag) holdCtr[0x1010]++;
          unsigned r1 = HR(QT_ACE, 1);
          unsigned r2 = HR(QT_ACE, 2);
          unsigned r3 = HR(QT_ACE, 3);
          unsigned r4 = HR(QT_PARD, 0);
          rank = r3;
          trick[0].Set(QT_BOTH, QT_ACE, r2, 3);
          trick[1].Set(QT_BOTH, QT_ACE, r1, 2);
          trick[2].Set(QT_PARD, QT_ACE, r3, 2);
          trick[3].Set(QT_BOTH, QT_PARD, r4, 3);
          trick[4].Set(QT_ACE, QT_ACE, SDS_VOID, 1);
          return def.Set122(trick);
        }
        else if (completeList[QT_ACE][3] < completeList[QT_PARD][0])
        {
          // AKQ8 / JT / 9xx / 7xxx.
          if (pickFlag) holdCtr[0x1011]++;
          rank = HR(QT_ACE, 3);
          unsigned r1 = HR(QT_PARD, 0);
          trick[0].Set(QT_BOTH, QT_ACE, rank, 4);
          trick[1].Set(QT_ACE, QT_PARD, r1, 3);
          trick[2].Set(QT_ACE, QT_ACE, SDS_VOID, 1);
          return def.Set12(trick[0], trick[1], trick[2]);
        }
      }
      else
      {
        // AKQ9 / Jx / 8xx / Txxx.
        if (pickFlag) holdCtr[0x1012]++;
        rank = HR(QT_ACE, 3);
        unsigned r1 = HR(QT_ACE, 1);
        unsigned r2 = HR(QT_ACE, 2);
        trick[0].Set(QT_BOTH, QT_ACE, r2, 3);
        trick[1].Set(QT_BOTH, QT_ACE, r1, 2);
        trick[2].Set(QT_PARD, QT_ACE, rank, 2);
        return def.Set12(trick[0], trick[1], trick[2]);
      }
    }
  }

  if (distHex == 0x5125)
  {
    if (completeList[QT_LHO][0] > completeList[QT_ACE][3] &&
      htop.T == QT_ACE && htop.N == QT_PARD && htop.E == QT_RHO &&
      completeList[QT_ACE][4] > completeList[QT_PARD][1] &&
      completeList[QT_ACE][4] > completeList[QT_RHO][1])
    {
      // AKQT7 / J / 9x / 8xxxx.
      if (pickFlag) holdCtr[0x1001]++;
      rank = HR(QT_ACE, 4);
      trick[0].Set(QT_BOTH, QT_ACE, SDS_TEN, 4);
      trick[1].Set(QT_BOTH, QT_ACE, SDS_ACE, 1);
      trick[2].Set(QT_PARD, QT_ACE, rank, 4);
      trick[3].Set(QT_BOTH, QT_PARD, SDS_NINE, 2);
      trick[4].Set(QT_ACE, QT_ACE, SDS_VOID, 3);
      return def.Set122(trick);
    }
  }

  if (distHex == 0x5143)
  {
    if (completeList[QT_LHO][0] > completeList[QT_RHO][0] &&
      htop.T == QT_RHO && htop.N == QT_PARD && htop.E == QT_PARD)
    {
      if (completeList[QT_ACE][3] > completeList[QT_PARD][3])
      {
        // AKQ6x / J / 987x / Txx.
        if (pickFlag) holdCtr[0x1001]++;
        rank = HR(QT_ACE, 3);
        trick[0].Set(QT_BOTH, QT_ACE, rank, 5);
        trick[1].Set(QT_BOTH, QT_ACE, SDS_ACE, 1);
        trick[2].Set(QT_PARD, QT_BOTH, SDS_EIGHT, 4);
        trick[3].Set(QT_ACE, QT_PARD, SDS_NINE, 4);
        trick[4].Set(QT_ACE, QT_ACE, SDS_VOID, 1);
        return def.Set122(trick);
      }
      else
      {
        // AQJ32 / K / 9854 / T76.
        if (pickFlag) holdCtr[0x1001]++;
        rank = HR(QT_ACE, 3);
        trick[0].Set(QT_PARD, QT_ACE, SDS_NINE, 4);
        trick[1].Set(QT_BOTH, QT_PARD, SDS_NINE, 4);
        trick[2].Set(QT_ACE, QT_ACE, SDS_VOID, 1);
        trick[3].Set(QT_BOTH, QT_ACE, SDS_ACE, 1);
        trick[4].Set(QT_PARD, QT_BOTH, SDS_EIGHT, 4);
        return def.Set122(trick);
      }
    }
  }

  if (length[QT_LHO] != 1 || length[QT_RHO] <= 3 || 
      length[QT_PARD] <= 1 || length[QT_ACE] <= 3)
    return false;

  if (length[QT_PARD] == 2)
  {
    if (htop.T == QT_PARD)
    {
      if (distHex == 0x5125 &&
          completeList[QT_ACE][4] > completeList[QT_RHO][0])
      {
        // AKQ98 / J / T7 / 65432.
        if (pickFlag) holdCtr[0x1013]++;
        rank = HR(QT_ACE, 4);
        unsigned r1 = HR(QT_ACE, 3);
        trick[0].Set(QT_BOTH, QT_ACE, rank, 5);
        trick[1].Set(QT_ACE, QT_PARD, SDS_TEN, 2);
        trick[2].Set(QT_ACE, QT_ACE, r1, 3);
        return def.Set12(trick[0], trick[1], trick[2]);
      }
      else if (distHex != 0x5125 &&
          completeList[QT_ACE][3] > completeList[QT_RHO][0])
      {
        // AQJ8 / K / T9 / 7654.
        if (pickFlag) holdCtr[0x1014]++;
        rank = HR(QT_ACE, 3);
        l = (length[QT_RHO] == 4 ? length[QT_ACE] : 4);
        trick[0].Set(QT_BOTH, QT_ACE, rank, l);
        trick[1].Set(QT_ACE, QT_PARD, SDS_TEN, 2);
        trick[2].Set(QT_ACE, QT_ACE, SDS_VOID, l-2);
        return def.Set12(trick[0], trick[1], trick[2]);
      }
      else if (htop.N == QT_RHO && htop.E == QT_ACE)
      {
        // AQJ82 / K / T3 / 9xxxx.
        if (pickFlag) holdCtr[0x1016]++;
        unsigned r2 = HR(QT_ACE, 2);
        rank = HR(QT_ACE, 3);
        l = (length[QT_RHO] == 4 ? length[QT_ACE] : 4);
        trick[0].Set(QT_BOTH, QT_ACE, r2, 3);
        trick[1].Set(QT_BOTH, QT_PARD, SDS_TEN, 2);
        trick[2].Set(QT_ACE, QT_ACE, SDS_VOID, l-2);
        trick[3].Set(QT_BOTH, QT_ACE, SDS_ACE, 1);
        trick[4].Set(QT_PARD, QT_ACE, rank, l-1);
        return def.Set122(trick);
      }
      else if (distHex == 0x5125 &&
          htop.T == QT_PARD && htop.N == QT_ACE && htop.E == QT_RHO &&
          completeList[QT_ACE][4] > completeList[QT_PARD][1] &&
          completeList[QT_ACE][4] > completeList[QT_RHO][1])
      {
        // AQJ97 / K / T3 / 8xxxx.
        if (pickFlag) holdCtr[0x1016]++;
        rank = HR(QT_ACE, 4);
        trick[0].Set(QT_BOTH, QT_ACE, HR(QT_ACE, 3), 4);
        trick[1].Set(QT_BOTH, QT_PARD, SDS_TEN, 2);
        trick[2].Set(QT_ACE, QT_ACE, SDS_NINE, 3);
        trick[3].Set(QT_BOTH, QT_ACE, SDS_ACE, 1);
        trick[4].Set(QT_PARD, QT_ACE, rank, 4);
        return def.Set122(trick);
      }
      else if (distHex == 0x5125 &&
          completeList[QT_ACE][3] > completeList[QT_RHO][0])
      {
        // AKQ95 / J / T8 / 76432.
        if (pickFlag) holdCtr[0x1015]++;
        rank = HR(QT_ACE, 3);
        trick[0].Set(QT_BOTH, QT_ACE, rank, 4);
        trick[1].Set(QT_BOTH, QT_PARD, SDS_TEN, 2);
        trick[2].Set(QT_ACE, QT_ACE, rank, 3);
        return def.Set12(trick[0], trick[1], trick[2]);
      }
      else
      {
        // AQJ32 / K / T3 / 98765.
        if (pickFlag) holdCtr[0x1016]++;
        rank = SDS_TEN;
        unsigned r2 = HR(QT_ACE, 2);
        l = (length[QT_RHO] == 4 ? length[QT_ACE] : 4);
        trick[0].Set(QT_BOTH, QT_ACE, r2, 3);
        trick[1].Set(QT_BOTH, QT_PARD, rank, 2);
        trick[2].Set(QT_ACE, QT_ACE, SDS_VOID, l-2);
        return def.Set12(trick[0], trick[1], trick[2]);
      }
    }
    else if (htop.T == QT_ACE)
    {
      if (distHex == 0x5125 && htop.N == QT_RHO && 
          completeList[QT_ACE][4] > completeList[QT_PARD][1] &&
          completeList[QT_ACE][4] > completeList[QT_RHO][1])
      {
        if (pickFlag) holdCtr[0x1017]++;
        rank = HR(QT_ACE, 4);
        trick[0].Set(QT_BOTH, QT_ACE, SDS_TEN, 4);
        trick[1].Set(QT_BOTH, QT_ACE, SDS_ACE, 1);
        trick[2].Set(QT_PARD, QT_ACE, rank, 4);
        return def.Set12(trick[0], trick[1], trick[2]);
      }
    }
    else if (htop.T == QT_RHO &&
        completeList[QT_PARD][1] < completeList[QT_ACE][3] &&
        ! LoopHold::StopFinesse(1, 3, 0, true, QT_ACE))
    {
      // AKQ9 / J / 76 / T8543.
      if (pickFlag) holdCtr[0x1018]++;
      rank = HR(QT_ACE, 3);
      unsigned r1 = HR(QT_ACE, 2);
      l = (length[QT_RHO] == 4 ? length[QT_ACE] : 4);
      trick[0].Set(QT_BOTH, QT_ACE, r1, 3);
      trick[1].Set(QT_BOTH, QT_ACE, SDS_ACE, 1);
      trick[2].Set(QT_PARD, QT_ACE, rank, l-1);
      return def.Set12(trick[0], trick[1], trick[2]);
    }
  }

  else if (length[QT_PARD] == 3)
  {
    if (htop.T == QT_RHO &&
        ! LoopHold::StopFinesse(1, 3, 0, false, QT_ACE))
    {
      if (htop.N == QT_PARD && htop.E == QT_PARD &&
          completeList[QT_RHO][1] > completeList[QT_ACE][3])
      {
        if (completeList[QT_ACE][3] > completeList[QT_RHO][2] &&
            completeList[QT_ACE][3] > completeList[QT_PARD][2])
          return false;

        // AKQ3 / J / 987 / T654.
        if (pickFlag) holdCtr[0x1019]++;
        rank = HR(QT_PARD, 1);
        unsigned r1 = HR(QT_ACE, 1);
        unsigned r2 = HR(QT_ACE, 2);
        l = (length[QT_RHO] == 4 ? length[QT_ACE] : 4);
        trick[0].Set(QT_BOTH, QT_ACE, r2, 3);
        trick[1].Set(QT_BOTH, QT_ACE, SDS_ACE, 1);
        trick[2].Set(QT_PARD, QT_PARD, rank, 2);
        trick[3].Set(QT_ACE, QT_ACE, SDS_VOID, l-3);
        return def.Set13(trick);
      }
      else if (completeList[QT_PARD][2] < completeList[QT_ACE][3] ||
          completeList[QT_RHO][1] < completeList[QT_ACE][3])
      {
        if (completeList[QT_PARD][1] > completeList[QT_ACE][3])
        {
          // AQJ95 / K / 763 / T842.
          if (pickFlag) holdCtr[0x101a]++;
          rank = HR(QT_ACE, 3);
          unsigned r1 = HR(QT_ACE, 2);
          unsigned r2 = HR(QT_PARD, 1);
          l = (length[QT_RHO] == 4 ? length[QT_ACE] : 4);
          trick[0].Set(QT_BOTH, QT_ACE, r1, 3);
          trick[1].Set(QT_BOTH, QT_ACE, SDS_ACE, 1);
          trick[2].Set(QT_PARD, QT_ACE, rank, l-1);
          trick[3].Set(QT_BOTH, QT_ACE, SDS_ACE, 1);
          trick[4].Set(QT_PARD, QT_PARD, r2, 2);
          trick[5].Set(QT_ACE, QT_ACE, SDS_VOID, l-3);
          return def.Set123(trick);
        }
        else
        {
          // AQJ95 / K / 763 / T842.
          if (pickFlag) holdCtr[0x101a]++;
          rank = HR(QT_ACE, 3);
          unsigned r1 = HR(QT_ACE, 2);
          l = (length[QT_RHO] == 4 ? length[QT_ACE] : 4);
          trick[0].Set(QT_BOTH, QT_ACE, r1, 3);
          trick[1].Set(QT_BOTH, QT_ACE, SDS_ACE, 1);
          trick[2].Set(QT_PARD, QT_ACE, rank, l-1);
          return def.Set12(trick[0], trick[1], trick[2]);
        }
      }
    }
    else if (htop.T == QT_RHO &&
        completeList[QT_PARD][2] < completeList[QT_ACE][3] &&
        ! LoopHold::StopFinesse(2, 3, 0, false, QT_ACE))
    {
      // AKQ7 / J / 954 / T863.
      if (pickFlag) holdCtr[0x101b]++;
      rank = HR(QT_ACE, 3);
      unsigned r1 = HR(QT_ACE, 1);
      unsigned r2 = HR(QT_ACE, 2);
      // The way it comes out.
      PosType s = (htop.K == QT_LHO ? QT_BOTH : QT_PARD);
      l = (length[QT_RHO] == 4 ? length[QT_ACE] : 4);
      trick[0].Set(QT_BOTH, QT_ACE, r2, 3);
      trick[1].Set(s, QT_ACE, SDS_ACE, 1);
      trick[2].Set(QT_PARD, QT_ACE, r1, 1);
      trick[3].Set(QT_PARD, QT_ACE, rank, l-2);
      return def.Set13(trick);
    }
  }
  else if (htop.T == QT_RHO &&
      ! LoopHold::StopFinesse(1, 3, 0, false, QT_ACE))
  {
    // AKQ9 / J / 6542 / T873.
    if (pickFlag) holdCtr[0x101c]++;
    unsigned r1 = HR(QT_ACE, 2);
    PosType e;
    if (htop.N == QT_PARD && htop.E == QT_PARD)
    {
      rank = HR(QT_PARD, 1);
      e = QT_BOTH;
    }
    else
    {
      rank = HR(QT_ACE, 3);
      e = QT_ACE;
    }
    trick[0].Set(QT_BOTH, QT_ACE, r1, 3);
    trick[1].Set(QT_BOTH, QT_ACE, SDS_ACE, 1);
    trick[2].Set(QT_PARD, e, rank, 3);
    return def.Set12(trick[0], trick[1], trick[2]);
  }
  else if (htop.T == QT_RHO &&
      ! LoopHold::StopFinesse(2, 3, 0, false, QT_ACE))
  {
    if (completeList[QT_ACE][3] < completeList[QT_PARD][1] &&
        completeList[QT_ACE][3] > completeList[QT_PARD][3])
      return false;

    // AKQ7 / J / 9642 / T853.
    if (pickFlag) holdCtr[0x101d]++;
    unsigned r1 = HR(QT_ACE, 1);
    unsigned r2 = HR(QT_ACE, 2);
    PosType s = (htop.K == QT_LHO ? QT_BOTH : QT_PARD);
    PosType e;
    if (completeList[QT_PARD][2] > completeList[QT_ACE][3])
    {
      rank = HR(QT_PARD, 2);
      e = QT_BOTH;
    }
    else
    {
      rank = HR(QT_ACE, 3);
      e = QT_ACE;
    }
    trick[0].Set(QT_BOTH, QT_ACE, r2, 3);
    trick[1].Set(s, QT_ACE, SDS_ACE, 1);
    trick[2].Set(QT_PARD, QT_ACE, r1, 1);
    trick[3].Set(QT_PARD, e, rank, 2);
    return def.Set13(trick);
  }

  return false;
}


bool LoopHold::SolveComplex2(DefList& def, unsigned& rank) const
{
  // ==== G2 ============== G8 =========== G32 ============ G42 ========
  //      AQJ+       |      AKJ+     |     AKQ+      |      A+
  // +          +    |  +       +    |  +       +    |  +         +
  //      K+         |      Q+       |     J+        |      KQJ+
  // ==== G2 ============== G8 =========== G32 ============ G42 ========

  Trick trick[5];
  PosType pa, pl, pp, pr;
  if (htop.K == QT_PARD && htop.Q == QT_PARD)
  {
    // "G42".  The aceholder has 3 honors.
    pa = QT_PARD;
    pl = QT_RHO;
    pp = QT_ACE;
    pr = QT_LHO;
  }
  else
  {
    pa = QT_ACE;
    pl = QT_LHO;
    pp = QT_PARD;
    pr = QT_RHO;
  }

  if (length[pp] == 2)
  {
    if (length[pl] >= 5)
    {
      unsigned l = (length[pl] == 5 ? length[pa] : 5);
      if (htop.T == pr && (htop.N == pa || htop.E == pa))
      {
        // AKQ9x / xxxxx / Jx / T.
        if (pickFlag) holdCtr[0x1020]++;
        rank = HR(pa, 3);
        trick[0].Set(QT_BOTH, QT_BOTH, rank, l);
        return def.Set1(trick[0]);
      }
      else if (htop.T == pr || htop.N == pp || 
         (completeList[pp][1] > completeList[pl][1] &&
          completeList[pa][4] < completeList[pl][1]))
      {
          // AKQxx / xxxxx / J9 / T.
          if (pickFlag) holdCtr[0x1021]++;
          rank = HR(pp, 1);
          PosType s = (htop.T == pr ? QT_BOTH : pa);
          trick[0].Set(QT_BOTH, QT_BOTH, SDS_JACK, 4);
          trick[1].Set(s, pp, rank, 2);
          trick[2].Set(pa, pa, SDS_VOID, l-2);
          return def.Set12(trick[0], trick[1], trick[2]);
      }
      else // if (htop.N == pa && htop.E == pp &&
          // completeList[pa][4] > completeList[pl][1] &&
          // (length[pr] == 0 || completeList[pa][4] > completeList[pr][0]))
      {
        // AKQ97 / T6543 / J8 / -.
        // Can no doubt be cleaned up logically.
        if (pickFlag) holdCtr[0x1022]++;
        unsigned r1 = HR(pa, 4);
        unsigned r2 = HR(pp, 1);
        rank = Max(r1, r2);
        trick[0].Set(QT_BOTH, QT_BOTH, SDS_JACK, 4);
        trick[1].Set(pa, pp, rank, 2);
        trick[2].Set(pa, pa, SDS_VOID, l-2);
        return def.Set12(trick[0], trick[1], trick[2]);
      }
      // else
      // {
        // AKQ98 / Txxxx / Jx / ?.
        // if (pickFlag) holdCtr[0x1022]++;
        // unsigned r1 = HR(pa, 4);
        // unsigned r2 = HR(pp, 1);
        // rank = Max(r1, r2);
        // trick[0].Set(QT_BOTH, QT_BOTH, SDS_JACK, 4);
        // trick[1].Set(pa, pp, rank, 2);
        // trick[2].Set(pa, pa, SDS_VOID, l-2);
        // return def.Set12(trick[0], trick[1], trick[2]);
      // }
    }
    else // length[pr] >= 5
    {
      unsigned l = (length[pr] == 5 ? length[pa] : 5);
      if (htop.T == pl)
      {
        if (completeList[pa][3] > completeList[pr][0])
        {
          if (pickFlag) holdCtr[0x1023]++;
          rank = HR(pa, 3);
          trick[0].Set(QT_BOTH, QT_BOTH, rank, l);
          return def.Set1(trick[0]);
        }
        else if (completeList[pp][1] > completeList[pr][0])
        {
          if (pickFlag) holdCtr[0x1024]++;
          rank = HR(pp, 1);
          trick[0].Set(QT_BOTH, QT_BOTH, SDS_JACK, 4);
          trick[1].Set(QT_BOTH, pp, rank, 2);
          trick[2].Set(pa, pa, SDS_VOID, l-2);
          return def.Set12(trick[0], trick[1], trick[2]);
        }
      }
      else if (completeList[pa][3] > completeList[pp][1] &&
          completeList[pa][3] > completeList[pr][1])
      {
        // The way it comes out.
        if (completeList[pa][4] < completeList[pr][1])
        {
          if (pickFlag) holdCtr[0x1025]++;
          rank = HR(pa, 3);
          trick[0].Set(QT_BOTH, pa, rank, l);
          trick[1].Set(pa, pp, SDS_JACK, 4);
          return def.Set11(trick[0], trick[1]);
        }
        else if (pa == QT_ACE && length[QT_LHO] == 0)
        {
          if (pickFlag) holdCtr[0x1026]++;
          unsigned r1 = HR(pp, 0);
          unsigned r2 = HR(pa, 2);
          unsigned r = (r2 < r1 ? r2 : SDS_VOID);
          rank = HR(pa, 3);
          trick[0].Set(QT_BOTH, pa, rank, l);
          trick[1].Set(pa, pp, Min(r1, r2), 4);
          return def.Set11(trick[0], trick[1]);
          // trick[0].Set(QT_BOTH, pa, rank, l);
          // trick[1].Set(pa, pp, r1, 1);
          // trick[2].Set(pa, pa, r, 3);
          // return def.Set12(trick[0], trick[1], trick[2]);
        }
        else if (pa == QT_PARD && htop.N == pl)
        {
          if (pickFlag) holdCtr[0x1027]++;
          rank = HR(pa, 3);
          trick[0].Set(QT_BOTH, pa, rank, l);
          trick[1].Set(pa, pp, SDS_JACK, 4);
          return def.Set11(trick[0], trick[1]);
          // trick[0].Set(QT_BOTH, pa, rank, l);
          // trick[1].Set(pa, pp, SDS_KING, 2);
          // trick[2].Set(pa, pa, SDS_JACK, 2);
          // return def.Set12(trick[0], trick[1], trick[2]);
        }
        else
        {
          if (pickFlag) holdCtr[0x1028]++;
          unsigned r1 = HR(pp, 0);
          unsigned r2 = HR(pa, 0);
          unsigned r = (r1 < r2 ? r1 : SDS_VOID);
          unsigned rr = (SDS_JACK < r ? SDS_JACK : SDS_VOID);
          rank = HR(pa, 3);
          trick[0].Set(QT_BOTH, pa, rank, l);
          trick[1].Set(pa, pp, SDS_JACK, 4);
          return def.Set11(trick[0], trick[1]);
          // trick[0].Set(QT_BOTH, pa, rank, l);
          // trick[1].Set(pa, pa, r2, 1);
          // trick[2].Set(QT_BOTH, pp, r, 1);
          // trick[3].Set(pa, pa, rr, 2);
          // return def.Set13(trick);
        }
      }
    }
  }
  else if (length[pl] >= 5) // now length[pp] == 3
  {
    // AKQ98 / Txxxx / Jxx / -.
    if (pickFlag) holdCtr[0x1029]++;
    rank = (htop.N == pp ? SDS_NINE : SDS_EIGHT);
    trick[0].Set(pa, QT_BOTH, rank, 5);
    trick[1].Set(pp, QT_BOTH, SDS_JACK, 4);
    trick[2].Set(pp, pp, rank, 3);
    trick[3].Set(pa, pa, SDS_VOID, 2);
    return def.Set112(trick);
  }
  else if (htop.N == pa || (htop.N == pp && htop.E == pa))
  {
    // The way it comes out.
    if ((completeList[pp][2] > completeList[pr][1] &&
         completeList[pp][2] > completeList[pa][4]) ||
        completeList[pr][1] > completeList[pa][4])
    {
      if (completeList[pp][2] > completeList[pr][1] &&
          completeList[pp][2] > completeList[pa][4] &&
          completeList[pr][1] < completeList[pa][4])
      {
        if (pickFlag) holdCtr[0x102a]++;
        rank = HR(pa, 3);
        unsigned r1 = HR(pa, 2);
        unsigned r2 = HR(pp, 0);
        trick[0].Set(pa, pa, rank, 5);
        trick[1].Set(pp, QT_BOTH, rank, 5);
        trick[2].Set(pa, pp, Min(r1, r2), 4);
        return def.Set111(trick);
        // trick[0].Set(pa, pa, r, 5);
        // trick[1].Set(pp, QT_BOTH, r, 5);
        // trick[2].Set(pa, pp, rank, 3);
        // trick[3].Set(pa, pa, SDS_VOID, 2);
        // return def.Set112(trick);
      }
      else
      {
        if (pickFlag) holdCtr[0x102b]++;
        rank = HR(pa, 3);
        unsigned r = HR(pp, 0);
        trick[0].Set(pa, pa, rank, 5);
        trick[1].Set(pp, QT_BOTH, rank, 5);
        trick[2].Set(pa, pp, SDS_JACK, 4);
        return def.Set111(trick);
      }
    }
    else if (completeList[pp][0] > completeList[pa][1] &&
        completeList[pa][4] > completeList[pr][1])
    {
      if (pickFlag) holdCtr[0x102c]++;
      rank = HR(pa, 3);
      trick[0].Set(pa, pa, rank, 5);
      trick[1].Set(pp, QT_BOTH, rank, 5);
      trick[2].Set(pa, pp, HR(pa, 2), 4);
      return def.Set111(trick);
      // trick[0].Set(pa, pa, rank, 5);
      // trick[1].Set(pp, QT_BOTH, rank, 5);
      // trick[2].Set(pa, pa, SDS_QUEEN, 2);
      // trick[3].Set(QT_BOTH, pp, SDS_VOID, 1);
      // trick[4].Set(pa, pa, SDS_JACK, 1);
      // return def.Set113(trick);
    }
    else
    {
      if (pickFlag) holdCtr[0x102d]++;
      rank = HR(pa, 3);
      unsigned r1 = HR(pp, 0);
      unsigned r2 = HR(pa, 2);
      if (r2 > r1)
        r2 = SDS_VOID;

      // The way it comes out.  Not necessarily right.
      // Recombine?
      if (htop.J == QT_PARD)
      {
        trick[0].Set(pa, pa, rank, 5);
        trick[1].Set(pp, QT_BOTH, rank, 5);
        trick[2].Set(pa, pp, Min(r1, r2), 4);
        return def.Set111(trick);
      }
      else if (htop.N == QT_ACE && htop.E == QT_PARD &&
          completeList[QT_ACE][4] > completeList[QT_PARD][2] &&
          completeList[QT_ACE][4] > completeList[QT_RHO][1])
      {
        trick[0].Set(pa, pa, rank, 5);
        trick[1].Set(pp, QT_BOTH, rank, 5);
        trick[2].Set(pa, pp, Min(r1, r2), 4);
        return def.Set111(trick);
      }
      else if (htop.N == QT_PARD && htop.E == QT_ACE)
      {
        // PROBLEM.  Are ranks wrong?
        return false;
      }
      else
      {
        trick[0].Set(pa, pa, rank, 5);
        trick[1].Set(pa, pp, Min(r1, r2), 4);
        trick[2].Set(pp, QT_BOTH, rank, 5);
        return def.Set111(trick);
      }
    }
  }
  else if (htop.N == pp)
  {
    if (completeList[pr][1] > completeList[pa][3])
    {
      if (completeList[pp][2] < completeList[pa][3])
      {
        if (pickFlag) holdCtr[0x102e]++;
        rank = HR(pa, 3);
        unsigned r1 = HR(pp, 0);
        unsigned r2 = HR(pa, 0);
        unsigned r = Min(r1, r2);
        trick[0].Set(pp, pp, SDS_JACK, 4);
        trick[1].Set(pp, pa, rank, 5);
        trick[2].Set(pa, QT_BOTH, SDS_JACK, 4);
        trick[3].Set(pa, pa, r, 2);
        trick[4].Set(pp, pa, rank, 3);
        return def.Set1112(trick);
      }
      else if (completeList[pp][2] > completeList[pr][1])
      {
        if (pickFlag) holdCtr[0x102f]++;
        rank = HR(pp, 2);
        trick[0].Set(QT_BOTH, QT_BOTH, SDS_JACK, 4);
        trick[1].Set(QT_BOTH, pp, rank, 3);
        trick[2].Set(pa, pa, SDS_VOID, 2);
        return def.Set12(trick[0], trick[1], trick[2]);
      }
    }
    else if (completeList[pr][1] > completeList[pa][4])
    {
      if (pickFlag) holdCtr[0x1080]++;
      rank = HR(pa, 3);
      unsigned r1 = HR(pp, 0);
      unsigned r2 = HR(pp, 2);
      trick[0].Set(pp, QT_BOTH, rank, 5);
      trick[1].Set(pa, pp, SDS_JACK, 4);
      trick[2].Set(pa, pa, rank, 5);
      trick[3].Set(QT_BOTH, pp, SDS_EIGHT, 3);
      trick[4].Set(pa, pa, SDS_VOID, 2);
      return def.Set1112(trick);
    }
    else if (completeList[pp][0] > completeList[pa][0])
    {
      // Combine this and next?
      if (pickFlag) holdCtr[0x1081]++;
      rank = HR(pa, 3);
      unsigned r1 = HR(pp, 0);
      unsigned r2 = HR(pp, 2);
      trick[0].Set(pp, QT_BOTH, rank, 5);
      trick[1].Set(pa, pp, SDS_JACK, 4);
      trick[2].Set(pa, pa, rank, 5);
      trick[3].Set(QT_BOTH, pp, r2, 3);
      trick[4].Set(pa, pa, SDS_VOID, 2);
      return def.Set1112(trick);
    }
    else
    {
      if (pickFlag) holdCtr[0x1082]++;
      rank = HR(pa, 3);
      unsigned r1 = HR(pp, 0);
      unsigned r2 = HR(pa, 2);
      trick[0].Set(pp, QT_BOTH, rank, 5);
      trick[1].Set(pa, pa, rank, 5);
      trick[2].Set(pa, pp, Min(r1, r2), 4);
      trick[3].Set(QT_BOTH, pp, SDS_EIGHT, 3);
      trick[4].Set(pa, pa, SDS_VOID, 2);
      return def.Set1112(trick);
    }
  }
  else if (completeList[pp][2] < completeList[pa][3])
  {
    if (pickFlag) holdCtr[0x1083]++;
    rank = HR(pa, 3);
    unsigned r1 = HR(pp, 0);
    unsigned r2 = HR(pa, 0);
    unsigned r = Min(r1, r2);
    trick[0].Set(pp, pp, SDS_JACK, 4);
    trick[1].Set(pp, pa, rank, 5);
    trick[2].Set(pa, QT_BOTH, SDS_JACK, 4);
    trick[3].Set(pa, pa, r, 2);
    trick[4].Set(pp, pa, rank, 3);
    return def.Set1112(trick);
  }

  return false;
}


bool LoopHold::SolveComplex3(DefList& def, unsigned& rank) const
{
  // ==== G3 ===========================================================
  //      AQJ+
  // K+         +
  //      +
  // ==== G3 ===========================================================

  if (pickFlag) holdCtr[0x1030]++;
  Trick trick[4];

  unsigned r1, r2, r3;
  unsigned topsHeld; // Ace tops over LHO.
  if (length[QT_LHO] == 0)
    topsHeld = length[QT_ACE];
  else
    topsHeld = Holding::TopsOverRank(QT_ACE, completeList[QT_LHO][0]);

  if (length[QT_RHO] == 2)
  {
    unsigned topsNeeded = Min(length[QT_ACE], length[QT_LHO]);
    unsigned topsUsed = Min(topsNeeded, topsHeld);
    unsigned l = (topsHeld < topsNeeded ? topsHeld : length[QT_ACE]);

    if (length[QT_PARD] == 1)
    {
      // AQJ+ / ? / x / Kx.
      if (pickFlag) holdCtr[0x1031]++;
      rank = (length[QT_LHO] <= 2 ? SDS_QUEEN : HR(QT_ACE, topsUsed-1));
      trick[0].Set(QT_ACE, QT_ACE, SDS_ACE, 1);
      trick[1].Set(QT_PARD, QT_ACE, rank, l);
      return def.Set11(trick[0], trick[1]);
    }
    else if (length[QT_PARD] == 2)
    {
      if (completeList[QT_ACE][length[QT_ACE]-1] >
            completeList[QT_PARD][1] ||
          completeList[QT_RHO][1] > completeList[QT_PARD][1] ||
         (length[QT_LHO] > 0 &&
          completeList[QT_LHO][0] > completeList[QT_PARD][0]) ||
         (length[QT_LHO] > 1 &&
          completeList[QT_LHO][0] > completeList[QT_PARD][1]))
      {
        // AQJ+ / ? xx / Kx.
        if (pickFlag) holdCtr[0x1032]++;
        rank = (length[QT_LHO] <= 2 ? SDS_QUEEN : HR(QT_ACE, topsUsed-1));
        trick[0].Set(QT_ACE, QT_ACE, SDS_ACE, 1);
        trick[1].Set(QT_PARD, QT_ACE, rank, l);
        return def.Set11(trick[0], trick[1]);
      }
      else if (length[QT_LHO] == 0)
      {
        if (pickFlag) holdCtr[0x1033]++;
        r1 = (length[QT_LHO] <= 2 ? SDS_QUEEN : HR(QT_ACE, topsUsed-1));
        r2 = HR(QT_PARD, 1);
        rank = Min(r1, r2);
        trick[0].Set(QT_ACE, QT_ACE, SDS_ACE, 1);
        trick[1].Set(QT_PARD, QT_ACE, r1, l);
        trick[2].Set(QT_PARD, QT_PARD, r2, 2);
        trick[3].Set(QT_ACE, QT_ACE, SDS_VOID, l-2);
        return def.Set112(trick);
      }
      else
      {
        // AQJ52 / 8764 / T9 / K3.
        if (pickFlag) holdCtr[0x1034]++;
        r1 = (length[QT_LHO] <= 2 ? SDS_QUEEN : HR(QT_ACE, topsUsed-1));
        r2 = HR(QT_PARD, 1);
        rank = Min(r1, r2);
        r3 = (length[QT_LHO] <= 2 ? SDS_VOID : HR(QT_ACE, topsUsed-2));
        if (r3 > r2)
          r3 = SDS_VOID;
        unsigned l2 = (topsHeld + 1 < topsNeeded ? 
          topsHeld : length[QT_ACE]);
        trick[0].Set(QT_ACE, QT_ACE, SDS_ACE, 1);
        trick[1].Set(QT_PARD, QT_ACE, r1, l);
        trick[2].Set(QT_PARD, QT_PARD, r2, 2);
        trick[3].Set(QT_ACE, QT_ACE, r3, l2-2);
        return def.Set112(trick);
      }
    }
  }

  return false;
}


bool LoopHold::SolveComplex6(DefList& def, unsigned& rank) const
{
  // ==== G6 ================= G46 =====================================
  //      AJ+        |         A+
  // +          Q+   |  Q+            +
  //      K+         |         KJ+
  // ==== G6 ================= G46 =====================================

  if (pickFlag) holdCtr[0x1060]++;

  Trick trick[4];
  PosType pa, pl, pp, pr;
  if (htop.Q == QT_LHO)
  {
    // "G6".  The aceholder has the jack.
    pa = QT_ACE;
    pl = QT_LHO;
    pp = QT_PARD;
    pr = QT_RHO;
  }
  else
  {
    pa = QT_PARD;
    pl = QT_RHO;
    pp = QT_ACE;
    pr = QT_LHO;
  }

  if (length[pa] >= 4 && length[pp] == length[pa] && length[pl] == 3)
  {
    if (htop.T == pp || 
       (length[pr] == 1 && htop.T == pr && htop.N == pp))
    {
      if (pickFlag) holdCtr[0x1061]++;
      rank = HR(pp, 1);
      unsigned l = Max(length[pa], length[pp]);
      trick[0].Set(QT_BOTH, QT_BOTH, rank, l);
      return def.Set1(trick[0]);
    }
  }
  else if (length[pp] >= 4 && length[pa] > length[pp] && length[pl] == 3)
  {
    if (htop.T == pa)
    {
      if (! hopp.N || 
         (htop.N == pr && htop.E == pp) ||
         (hopp.N && completeList[pa][3] > completeList[pp][length[pp]-1]))
      {
        // Axxx / - / KJTxxx / Q98.
        if (pickFlag) holdCtr[0x1062]++;
        if (htop.N == pl || 
            htop.N == pa || 
           (htop.N == pr && completeList[pl][1] > completeList[pp][1]))
          rank = HR(pa, 3);
        else
        {
          unsigned r = HR(pa, 3);
          rank = HR(pp, 1);
          if (r > rank)
            rank = r;
        }
        trick[0].Set(QT_BOTH, QT_BOTH, rank, length[pa]);
        return def.Set1(trick[0]);
      }
      else
      {
        // A765 / - / KJT432 / Q98.
        if (pickFlag) holdCtr[0x1063]++;
        rank = HR(pp, 1);
        trick[0].Set(pp, pa, rank, 4);
        trick[1].Set(QT_BOTH, pp, rank, 4);
        trick[2].Set(pa, pa, SDS_VOID, length[pa]-4);
        return def.Set12(trick[0], trick[1], trick[2]);
      }
    }
    else if (htop.T == pr)
    {
      if ((htop.N == pa && htop.E == pp) ||
          (htop.N == pp && htop.E == pa))
      {
        // AJ9xx / Qxx / K8xx / T.
        if (pickFlag) holdCtr[0x1064]++;
        rank = HR(pp, 1);
        trick[0].Set(QT_BOTH, QT_BOTH, rank, length[pa]);
        return def.Set1(trick[0]);
      }
      else if (htop.N == pa && htop.E == pl &&
          completeList[pp][3] > completeList[pa][3])
      {
        // AJ9xx / Q8x / K7xx / T.
        if (pickFlag) holdCtr[0x1065]++;
        rank = HR(pp, 1);
        trick[0].Set(pp, pa, rank, 4);
        trick[1].Set(QT_BOTH, pp, rank, 4);
        trick[2].Set(pa, pa, SDS_VOID, length[pa]-4);
        return def.Set12(trick[0], trick[1], trick[2]);
      }
    }
    else if (htop.T == pp)
    {
      // ATxx / - / KJxxxx / Q98.
      if (pickFlag) holdCtr[0x1066]++;
      rank = SDS_TEN;
      trick[0].Set(QT_BOTH, QT_BOTH, rank, length[pa]);
      return def.Set1(trick[0]);
    }
  }
  else if (length[pa] >= 4 && length[pp] > length[pa] && length[pl] == 3)
  {
    if (htop.T == pa)
    {
      if (completeList[pp][1] > completeList[pa][length[pa]-1])
      {
        // A7xxxx / - / KJT6 / Q98.
        if (pickFlag) holdCtr[0x1067]++;
        rank = HR(pp, 1);
        trick[0].Set(QT_BOTH, QT_BOTH, rank, length[pp]);
        return def.Set1(trick[0]);
      }
      else if (htop.N == pa || (htop.N == pr && htop.E == pa))
      {
        // The way it comes out.
        // AJT9 / Q43 / K8765 / -.
        // Combine again?
        if (pickFlag) holdCtr[0x1068]++;
        rank = HR(pa, length[pa]-1);
        trick[0].Set(pa, pp, rank, 4);
        trick[1].Set(QT_BOTH, pa, rank, 4);
        trick[2].Set(pp, pp, SDS_VOID, length[pp]-4);
        return def.Set12(trick[0], trick[1], trick[2]);
      }
      else
      {
        // A65432 / - / KJT7 / Q98.
        if (pickFlag) holdCtr[0x1069]++;
        rank = HR(pa, length[pa]-1);
        trick[0].Set(pa, pp, rank, 4);
        trick[1].Set(QT_BOTH, pa, rank, 4);
        trick[2].Set(pp, pp, SDS_VOID, length[pp]-4);
        return def.Set12(trick[0], trick[1], trick[2]);
      }
    }
    else if ((htop.T == pp && 
        (htop.N == pa || (htop.N == pr && htop.E == pa))) ||
        (htop.T == pr && htop.N == pp && htop.E == pa))
    {
      // AT6432 / - / KJ75 / Q98.
      if (pickFlag) holdCtr[0x106a]++;
      rank = HR(pa, 2);
      trick[0].Set(QT_BOTH, QT_BOTH, rank, length[pp]);
      return def.Set1(trick[0]);
    }
    else if (htop.T == pp)
    {
      if (completeList[pp][2] < completeList[pa][length[pa]-1])
      {
        // AT5432 / - / KJ76 / Q98.
        if (pickFlag) holdCtr[0x106b]++;
        rank = SDS_TEN;
        trick[0].Set(pa, pp, rank, 4);
        trick[1].Set(QT_BOTH, pa, rank, 4);
        trick[2].Set(pp, pp, SDS_VOID, length[pp]-4);
        return def.Set12(trick[0], trick[1], trick[2]);
      }
    }
    else if (completeList[pl][1] > completeList[pa][2] &&
        completeList[pa][3] > completeList[pp][2])
    {
      // A9432 / T / KJ65 / Q87.
      if (pickFlag) holdCtr[0x106c]++;
      rank = HR(pp, 1);
      trick[0].Set(pa, pp, rank, 4);
      trick[1].Set(QT_BOTH, pa, rank, 4);
      trick[2].Set(pp, pp, SDS_VOID, 1);
      return def.Set12(trick[0], trick[1], trick[2]);
    }
    else if (completeList[pp][1] > completeList[pa][length[pa]-1] &&
        completeList[pa][2] > completeList[pp][1] &&
        completeList[pa][2] > completeList[pl][1])
    {
      // A8xxx / T / KJ9x / Qxx.
      if (pickFlag) holdCtr[0x106d]++;
      rank = HR(pp, 1);
      trick[0].Set(QT_BOTH, QT_BOTH, rank, length[pp]);
      return def.Set1(trick[0]);
    }
  }
  else if (length[pa] >= 4 && length[pp] == 3 && length[pl] == 2)
  {
    if (length[pr] == 4)
    {
      if (htop.T == pp)
      {
        if (completeList[pa][2] > completeList[pr][0])
        {
          // ATx / xxxx / KJ8x / Q9.
          if (pickFlag) holdCtr[0x106e]++;
          rank = HR(pa, 2);
          trick[0].Set(pa, QT_BOTH, SDS_VOID-5, length[pa]);
          trick[1].Set(pp, QT_BOTH, rank, length[pa]);
          return def.Set11(trick[0], trick[1]);
        }
        else if (completeList[pa][2] > completeList[pr][1] &&
            completeList[pa][2] > completeList[pp][2])
        {
          // AT2 / 9543 / KJ76 / Q8.
          if (pickFlag) holdCtr[0x106f]++;
          rank = HR(pa, 2);
          trick[0].Set(pp, pa, rank, length[pa]);
          trick[1].Set(pa, QT_BOTH, SDS_TEN, length[pa]);
          trick[2].Set(pp, pp, SDS_JACK, 3);
          trick[3].Set(pp, pp, SDS_VOID-5, length[pa]-1);
          trick[4].Set(pa, pa, SDS_VOID, 1);
          return def.Set1112(trick);
        }
        else
        {
          // AT8 / 7532 / KJ64 / Q9.
          // AT2 / 9843 / KJ65 / Q7.
          if (pickFlag) holdCtr[0x1461]++;
          rank = HR(pp, 1);
          trick[0].Set(pa, QT_BOTH, rank, length[pa]);
          trick[1].Set(pp, QT_BOTH, SDS_JACK, 3);
          trick[2].Set(pp, pp, SDS_TEN, 3);
          trick[3].Set(pa, pa, SDS_VOID, 1);
          return def.Set112(trick);
        }
      }
      else if (htop.T == pl)
      {
        if (htop.N == pp)
        {
          if (htop.E == pr &&
              completeList[pa][2] > completeList[pp][2] &&
              completeList[pa][2] > completeList[pr][1])
          {
            // The way it comes out.  
            if (length[QT_ACE] == 3)
            {
              // A9x / 8xxx / KJ7x / QT.
              if (pickFlag) holdCtr[0x1463]++;
              rank = HR(pa, 2);
              unsigned r1 = HR(pp, 1);
              // The way it comes out.
              trick[0].Set(QT_BOTH, pa, rank, 4);
              trick[1].Set(pa, pp, r1, 3);
              trick[2].Set(pa, pa, SDS_VOID, 1);
              return def.Set12(trick[0], trick[1], trick[2]);
            }
            else
            {
              rank = HR(pa, 2);
              unsigned r1 = HR(pp, 1);
              // The way it comes out.
              trick[0].Set(QT_BOTH, pa, rank, 4);
              trick[1].Set(pa, pp, SDS_JACK, 3);
              trick[2].Set(pa, pp, r1, 3);
              trick[3].Set(pa, pa, SDS_VOID, 1);
              return def.Set112(trick);
            }
          }
          else if (htop.E == pp && 
              completeList[pr][0] > completeList[pa][2])
          {
            // AJ65 / QT / K98 / 7432.
            if (pickFlag) holdCtr[0x1464]++;
            rank = HR(pp, 1);
            trick[0].Set(QT_BOTH, QT_BOTH, SDS_JACK, 3);
            trick[1].Set(QT_BOTH, pp, rank, 3);
            trick[2].Set(pa, pa, SDS_VOID, 1);
            return def.Set12(trick[0], trick[1], trick[2]);
          }
        }
      }
    }
  }
  else if (length[pa] >= 4 && length[pp] == 3 &&
      length[pl] == 3 && length[pr] == 2)
  {
    if (htop.T == pr && htop.N == pp && hopp.E)
    {
      if (completeList[pl][1] > completeList[pa][2] &&
          completeList[pl][1] > completeList[pp][2])
      {
        if (pickFlag) holdCtr[0x1466]++;
        rank = HR(pp, 1);
        trick[0].Set(pp, pa, SDS_KING, 2);
        trick[1].Set(QT_BOTH, pp, rank, 3);
        trick[2].Set(pa, pa, SDS_VOID, length[pa] - 3);
        return def.Set12(trick[0], trick[1], trick[2]);
      }
    }
  }
  else if (length[pa] == 3 && length[pp] >= 4 &&
      length[pl] == 3 && length[pr] <= 3)
  {
    if (htop.T == pa || 
       (length[pr] <= 2 && htop.T == pr && 
        completeList[pa][2] > completeList[pp][1] &&
        completeList[pa][2] > completeList[pl][1]))
    {
      // Axxx / xxx / KJT / Qxx.
      if (pickFlag) holdCtr[0x1467]++;
      rank = HR(pa, 2);
      trick[0].Set(pa, pp, rank, 3);
      trick[1].Set(QT_BOTH, pa, rank, 3);
      trick[2].Set(pp, pp, SDS_VOID, length[pp] - length[pa]);
      return def.Set12(trick[0], trick[1], trick[2]);
    }
  }

  return false;
}


bool LoopHold::SolveComplex9(DefList& def, unsigned& rank) const
{
  // ==== G9 ================= G33 =====================================
  //      AJ+        |         AQ+    
  // +          K+   |    +          K+
  //      Q+         |         J+      
  // ==== G9 ================= G33 =====================================

  if (pickFlag) holdCtr[0x1090]++;

  Trick trick[5];
  if (length[QT_LHO] == 1 && length[QT_PARD] == 2 && 
     ((length[QT_ACE] == 3 && length[QT_RHO] >= 3) ||
      (length[QT_ACE] >= 3 && length[QT_RHO] == 3)))
  {
    if (htop.Q == QT_ACE && htop.T == QT_RHO && htop.N == QT_ACE)
    {
      // AQ9643 / K / J4 / T87.
      if (pickFlag) holdCtr[0x1092]++;
      rank = SDS_NINE;
      unsigned l = (length[QT_RHO] == 3 ? length[QT_ACE] : 3);
      unsigned r1 = HR(QT_PARD, 0);
      trick[0].Set(QT_BOTH, QT_ACE, SDS_QUEEN, 2);
      trick[1].Set(QT_BOTH, QT_ACE, SDS_ACE, 1);
      trick[2].Set(QT_PARD, QT_ACE, SDS_NINE, l-1);
      trick[3].Set(QT_BOTH, QT_PARD, HR(QT_PARD, 0), 2);
      trick[4].Set(QT_ACE, QT_ACE, SDS_VOID, l-2);
      return def.Set122(trick);
    }
    else if (htop.Q == QT_PARD && htop.T == QT_RHO && htop.N == QT_ACE)
    {
      // AJ9 / K / Q2 / T876543.
      if (pickFlag) holdCtr[0x1092]++;
      rank = SDS_NINE;
      unsigned l = (length[QT_RHO] == 3 ? length[QT_ACE] : 3);
      unsigned r1 = HR(QT_PARD, 0);
      trick[0].Set(QT_PARD, QT_ACE, SDS_QUEEN, 2);
      trick[1].Set(QT_BOTH, QT_ACE, SDS_ACE, 1);
      trick[2].Set(QT_PARD, QT_ACE, SDS_NINE, l-1);
      trick[3].Set(QT_BOTH, QT_PARD, HR(QT_PARD, 0), 2);
      trick[4].Set(QT_ACE, QT_ACE, SDS_JACK, l-2);
      return def.Set122(trick);
    }
  }

  if (length[QT_ACE] >= 4 && length[QT_LHO] == 1 && length[QT_RHO] >= 4)
  {
    if (length[QT_PARD] == 3)
    {
      if (htop.T == QT_PARD && htop.N == QT_RHO && htop.E == QT_ACE)
      {
        // AH8x / K / HTx / 9xxx.
        if (pickFlag) holdCtr[0x1091]++;
        rank = SDS_VOID - 7;
        unsigned l = (length[QT_RHO] == 4 ? length[QT_ACE] : 4);
        trick[0].Set(QT_BOTH, QT_ACE, rank, l);
        trick[1].Set(QT_ACE, QT_PARD, SDS_JACK, 3);
        trick[2].Set(QT_ACE, QT_PARD, SDS_TEN, 3);
        trick[3].Set(QT_ACE, QT_ACE, SDS_VOID, l-3);
        return def.Set112(trick);
      }
    }
    else if (length[QT_PARD] == 2)
    {
      if (htop.T == QT_ACE && htop.N == QT_RHO && htop.E == QT_ACE)
      {
        // AQT8 / K / Jx / 9xxx.
        if (pickFlag) holdCtr[0x1092]++;
        rank = SDS_EIGHT;
        unsigned l = (length[QT_RHO] == 4 ? length[QT_ACE] : 4);
        unsigned r1 = HR(QT_PARD, 0);
        trick[0].Set(QT_BOTH, QT_ACE, SDS_TEN, 3);
        trick[1].Set(QT_BOTH, QT_PARD, r1, 2);
        trick[2].Set(QT_ACE, QT_ACE, SDS_TEN, l-2);
        trick[3].Set(QT_BOTH, QT_ACE, SDS_ACE, 1);
        trick[4].Set(QT_PARD, QT_ACE, SDS_EIGHT, l-1);
        return def.Set122(trick);
      }
      else if (htop.Q == QT_ACE && htop.T == QT_RHO && htop.N == QT_ACE)
      {
        // AQ98 / K / Jx / Txxx.
        if (pickFlag) holdCtr[0x1093]++;
        rank = SDS_NINE;
        trick[0].Set(QT_BOTH, QT_ACE, SDS_QUEEN, 2);
        trick[1].Set(QT_BOTH, QT_PARD, SDS_JACK, 2);
        trick[2].Set(QT_ACE, QT_ACE, SDS_VOID, 1);
        trick[3].Set(QT_BOTH, QT_ACE, SDS_ACE, 1);
        trick[4].Set(QT_PARD, QT_ACE, SDS_NINE, 2);
        return def.Set122(trick);
      }
      else if (htop.Q == QT_PARD && htop.T == QT_RHO)
      {
        if (htop.N == QT_ACE)
        {
          // AJ9x / K / Qx / Txxx+.
          if (pickFlag) holdCtr[0x1094]++;
          trick[0].Set(QT_PARD, QT_ACE, SDS_QUEEN, 2);
          trick[1].Set(QT_BOTH, QT_PARD, SDS_QUEEN, 2);
          trick[2].Set(QT_ACE, QT_ACE, SDS_JACK, 1);
          trick[3].Set(QT_BOTH, QT_ACE, SDS_ACE, 1);
          trick[4].Set(QT_PARD, QT_ACE, SDS_NINE, 2);
          return def.Set122(trick);
        }
        else
        {
          // AJxx+ / K / Qx / Txxx+.
          if (pickFlag) holdCtr[0x1094]++;
          trick[0].Set(QT_PARD, QT_ACE, SDS_QUEEN, 2);
          trick[1].Set(QT_BOTH, QT_PARD, SDS_QUEEN, 2);
          trick[2].Set(QT_ACE, QT_ACE, SDS_JACK, 1);
          return def.Set12(trick[0], trick[1], trick[2]);
        }
      }
      else if (htop.Q == QT_PARD && htop.T == QT_PARD && htop.N == QT_RHO)
      {
        // AJxx+ / K / Qx / Txxx+.
        if (pickFlag) holdCtr[0x1094]++;
        trick[0].Set(QT_PARD, QT_ACE, SDS_QUEEN, 2);
        trick[1].Set(QT_BOTH, QT_PARD, SDS_QUEEN, 2);
        trick[2].Set(QT_ACE, QT_ACE, SDS_JACK, 1);
        return def.Set12(trick[0], trick[1], trick[2]);
      }
      else if (htop.Q == QT_ACE &&
          (htop.T == QT_RHO || (htop.T == QT_PARD && htop.N == QT_RHO)))
      {
        // AQxx+ / K / Jx / Txxx+.
        if (pickFlag) holdCtr[0x1095]++;
        trick[0].Set(QT_BOTH, QT_ACE, SDS_QUEEN, 2);
        trick[1].Set(QT_BOTH, QT_PARD, SDS_JACK, 2);
        trick[2].Set(QT_ACE, QT_ACE, SDS_VOID, 1);
        return def.Set12(trick[0], trick[1], trick[2]);
      }
      else if ((htop.T == QT_ACE && htop.N == QT_ACE) ||
          (completeList[QT_RHO][0] < completeList[QT_PARD][1] &&
           completeList[QT_RHO][0] < completeList[QT_ACE][3]))
      {
        if (distHex == 0x5125)
        {
          if (htop.T == QT_ACE && htop.N == QT_ACE && htop.E == QT_RHO &&
              completeList[QT_ACE][4] > completeList[QT_PARD][1] &&
              completeList[QT_ACE][4] > completeList[QT_RHO][1])
          {
            // AQT97 / K / J6 / 85432.
            if (pickFlag) holdCtr[0x1096]++;
            rank = HR(QT_ACE, 4);
            trick[0].Set(QT_BOTH, QT_ACE, SDS_NINE, 4);
            trick[1].Set(QT_BOTH, QT_ACE, SDS_ACE, 1);
            trick[2].Set(QT_PARD, QT_ACE, rank, 4);
            trick[3].Set(QT_BOTH, QT_PARD, HR(QT_PARD, 0), 2);
            trick[4].Set(QT_ACE, QT_ACE, SDS_NINE, 3);
            return def.Set122(trick);
          }
          else if (completeList[QT_ACE][4] > completeList[QT_RHO][0])
          {
            // AQT98 / K / J7 / 65432.
            if (pickFlag) holdCtr[0x1096]++;
            rank = HR(QT_ACE, 4);
            trick[0].Set(QT_BOTH, QT_ACE, rank, 5);
            trick[1].Set(QT_ACE, QT_PARD, HR(QT_PARD, 0), 2);
            trick[2].Set(QT_ACE, QT_ACE, HR(QT_ACE, 3), 3);
            return def.Set12(trick[0], trick[1], trick[2]);
          }
          else
          {
            // AQT96 / K / J8 / 75432.
            if (pickFlag) holdCtr[0x1096]++;
            rank = HR(QT_ACE, 3);
            trick[0].Set(QT_BOTH, QT_ACE, rank, 4);
            trick[1].Set(QT_BOTH, QT_PARD, HR(QT_PARD, 0), 2);
            trick[2].Set(QT_ACE, QT_ACE, rank, 3);
            return def.Set12(trick[0], trick[1], trick[2]);
          }
        }
        else
        {
          // AQT9 / K / Jx / xxxx.
          if (pickFlag) holdCtr[0x1096]++;
          unsigned l = (length[QT_RHO] == 4 ? length[QT_ACE] : 4);
          unsigned r1 = HR(QT_ACE, 3);
          unsigned r2 = HR(QT_PARD, 0);
          unsigned r3 = HR(QT_ACE, 2);
          trick[0].Set(QT_BOTH, QT_ACE, r1, l);
          trick[1].Set(QT_ACE, QT_PARD, r2, 2);
          trick[2].Set(QT_ACE, QT_ACE, r3, l-2);
          return def.Set12(trick[0], trick[1], trick[2]);
        }
      }
      else
      {
        if (pickFlag) holdCtr[0x1097]++;
        unsigned l = (length[QT_RHO] == 4 ? length[QT_ACE] : 4);
        unsigned r1 = HR(QT_PARD, 0);
        unsigned r2 = HR(QT_ACE, 2);
        trick[0].Set(QT_BOTH, QT_ACE, r2, 3);
        trick[1].Set(QT_BOTH, QT_PARD, r1, 2);
        trick[2].Set(QT_ACE, QT_ACE, r2, l-2);
        return def.Set12(trick[0], trick[1], trick[2]);
      }
    }
  }
  else if (length[QT_ACE] == 2 && length[QT_RHO] >= 4)
  {
    if (htop.Q == QT_ACE)
    {
      if (length[QT_PARD] == 3 || htop.T == QT_RHO)
      {
        // AQ / K / Jxx / Txxx.
        if (pickFlag) holdCtr[0x1098]++;
        rank = SDS_JACK;
        trick[0].Set(QT_BOTH, QT_ACE, SDS_QUEEN, 2);
        trick[1].Set(QT_PARD, QT_PARD, rank, 1);
        return def.Set2(trick[0], trick[1]);
      }
      else if (htop.T == QT_PARD)
      {
        if (length[QT_PARD] == 4 || htop.N == QT_RHO)
        {
          // AQ / K / JTxxx / 9xxxx.
          if (pickFlag) holdCtr[0x1099]++;
          rank = SDS_TEN;
          unsigned l = (length[QT_RHO] == 4 ? length[QT_PARD] : 4);
          trick[0].Set(QT_BOTH, QT_ACE, SDS_QUEEN, 2);
          trick[1].Set(QT_PARD, QT_PARD, rank, l-2);
          return def.Set2(trick[0], trick[1]);
        }
        else
        {
          // AQ / K / JT9xx / 8xxxx.
          if (pickFlag) holdCtr[0x109a]++;
          rank = (length[QT_RHO] == 4 ? SDS_TEN : SDS_NINE);
          trick[0].Set(QT_BOTH, QT_ACE, SDS_QUEEN, 2);
          trick[1].Set(QT_PARD, QT_PARD, rank, length[QT_PARD]-2);
          return def.Set2(trick[0], trick[1]);
        }
      }
    }
  }

  return false;
}


bool LoopHold::SolveComplex10(DefList& def, unsigned& rank) const
{
  // ==== G10 ================ G34 ================ G40 ================
  //      AJ+        |         AQ+        |         AK+ 
  // +          +    |    +          +    |    +          +
  //      KQ+        |         KJ+        |         QJ+
  // ==== G10 ================ G34 ================ G40 ================

  if (pickFlag) holdCtr[0x1100]++;

  PosType pa, pl, pp, pr;
  if (length[QT_ACE] == 5)
  {
    // Aceholder has 5.
    if (distHex != 0x5035 && distHex != 0x5530)
      return false;
    pa = QT_ACE;
    pl = QT_LHO;
    pp = QT_PARD;
    pr = QT_RHO;
  }
  else if (length[QT_PARD] == 5)
  {
    if (distHex != 0x3055 && distHex != 0x3550)
      return false;
    pa = QT_PARD;
    pl = QT_RHO;
    pp = QT_ACE;
    pr = QT_LHO;
  }
  else
    return false;

  Trick trick[4];
  if (htop.T == pr)
  {
    if (htop.N == pa)
    {
      // AK9xx / - / QJx / Txxxx.
      if (pickFlag) holdCtr[0x1101]++;
      rank = SDS_NINE;
      trick[0].Set(QT_BOTH, QT_BOTH, rank, 5);
      return def.Set1(trick[0]);
    }
  }
  
  return false;
}


bool LoopHold::SolveComplex11(DefList& def, unsigned& rank) const
{
  // ==== G11 ================ G35 =====================================
  //      AJ+        |         AQ+    
  // K+         +    |   K+          + 
  //      Q+         |         J+      
  // ==== G11 ================ G35 =====================================

  if (pickFlag) holdCtr[0x1110]++;

  Trick trick[3];
  if (length[QT_ACE] >= 3 && length[QT_PARD] == 2 &&
      length[QT_RHO] == 1 && length[QT_LHO] > 3)
  {
    if (htop.T == QT_LHO)
    {
      if (htop.Q == QT_ACE)
      {
        // AQx / Txx / Jx / K.
        if (pickFlag) holdCtr[0x1111]++;
        rank = SDS_JACK;
        trick[0].Set(QT_BOTH, QT_ACE, SDS_QUEEN, 2);
        trick[1].Set(QT_BOTH, QT_PARD, SDS_JACK, 2);
        trick[2].Set(QT_ACE, QT_ACE, SDS_VOID, 1);
        return def.Set12(trick[0], trick[1], trick[2]);
      }
      else
      {
        // AJx / Txx / Qx / K.
        // Not the same!
        if (pickFlag) holdCtr[0x1111]++;
        rank = SDS_JACK;
        trick[0].Set(QT_PARD, QT_ACE, SDS_QUEEN, 2);
        trick[1].Set(QT_BOTH, QT_PARD, SDS_QUEEN, 2);
        trick[2].Set(QT_ACE, QT_ACE, SDS_JACK, 1);
        return def.Set12(trick[0], trick[1], trick[2]);
      }
    }
    else if (htop.T == QT_PARD)
    {
      if (htop.N == QT_LHO)
      {
        if (htop.Q == QT_ACE)
        {
          // AJxxx / 9xxxx / QT / K.
          if (pickFlag) holdCtr[0x1112]++;
          rank = SDS_JACK;
          trick[0].Set(QT_BOTH, QT_ACE, SDS_QUEEN, 2);
          trick[1].Set(QT_BOTH, QT_PARD, SDS_JACK, 2);
          trick[2].Set(QT_ACE, QT_ACE, SDS_VOID, 1);
          return def.Set12(trick[0], trick[1], trick[2]);
        }
        else
        {
          // AJxxx / 9xxxx / QT / K.
          if (pickFlag) holdCtr[0x1112]++;
          unsigned r1 = HR(QT_ACE, 1);
          unsigned r2 = HR(QT_PARD, 0);
          rank = Min(r1, r2);
          if (r1 > r2)
            r1 = SDS_VOID;
          trick[0].Set(QT_PARD, QT_ACE, r2, 2);
          trick[1].Set(QT_BOTH, QT_PARD, r2, 2);
          trick[2].Set(QT_ACE, QT_ACE, r1, 1);
          return def.Set12(trick[0], trick[1], trick[2]);
        }
      }
    }
  }
  else if (length[QT_ACE] == 3 && length[QT_PARD] >= 4 &&
      length[QT_LHO] >= 4 && length[QT_RHO] == 1)
  {
    if (htop.T == QT_LHO && htop.N == QT_PARD)
    {
      // AQx / Txxx / J9xx / K.
      if (pickFlag) holdCtr[0x1113]++;
      rank = SDS_NINE;
      unsigned l = (length[QT_LHO] > 4 ? 4 : length[QT_PARD]);
      trick[0].Set(QT_BOTH, QT_BOTH, rank, l);
      return def.Set1(trick[0]);
    }
  }
  else if (length[QT_ACE] >= 4 && length[QT_PARD] == 3 &&
      length[QT_LHO] >= 4 && length[QT_RHO] == 1)
  {
    if (htop.T == QT_LHO && htop.N == QT_ACE)
    {
      if (pickFlag) holdCtr[0x1114]++;
      rank = SDS_EIGHT;
      unsigned l = (length[QT_LHO] > 4 ? 4 : length[QT_ACE]);
      trick[0].Set(QT_BOTH, QT_BOTH, SDS_JACK, 3);
      trick[1].Set(QT_BOTH, QT_PARD, rank, 3);
      trick[2].Set(QT_ACE, QT_ACE, SDS_VOID, l-3);
      return def.Set12(trick[0], trick[1], trick[2]);
    }
  }
  else if (length[QT_ACE] == 2 && length[QT_PARD] >= 3 &&
      length[QT_RHO] == 1)
  {
    if (htop.Q == QT_ACE)
    {
      // AQ / Txxx / Jxx / K.
      if (pickFlag) holdCtr[0x1116]++;
      unsigned r1 = HR(QT_ACE, 1);
      unsigned r2;
      unsigned l;
      if (length[QT_LHO] <= 3 || length[QT_PARD] == 3)
      {
        l = length[QT_PARD] - 2;
        r2 = length[QT_LHO] <= 2 ? SDS_VOID : SDS_JACK;
      }
      else if (length[QT_LHO] == 4 || length[QT_LHO] == 4)
      {
        if (htop.T == QT_PARD)
        {
          l = length[QT_PARD] - 2;
          r2 = SDS_TEN;
        }
        else
        {
          l = 1;
          r2 = SDS_JACK;
        }
      }
      else
      {
        if (htop.T == QT_PARD && htop.N == QT_PARD)
        {
          l = length[QT_PARD] - 2;
          r2 = (l > 2 ? SDS_NINE : SDS_TEN);
        }
        else if (htop.T == QT_PARD)
        {
          l = 2;
          r2 = SDS_TEN;
        }
        else
        {
          l = 1;
          r2 = SDS_JACK;
        }
      }

      rank = Min(r1, r2);
      trick[0].Set(QT_BOTH, QT_ACE, r1, 2);
      trick[1].Set(QT_PARD, QT_PARD, r2, l);
      return def.Set2(trick[0], trick[1]);
    }
  }

  return false;
}


bool LoopHold::SolveComplex12(DefList& def, unsigned& rank) const
{
  // ==== G12 ==========================================================
  //      AKJ+    
  // Q+         +
  //      +     
  // ==== G12 ==========================================================

  if (pickFlag) holdCtr[0x1120]++;
  Trick trick[5];

  if (distHex == 0x4432 && htop.T == QT_PARD && htop.N == QT_PARD)
  {
    if (completeList[QT_ACE][3] > completeList[QT_LHO][0])
    {
      // AKJ8 / 7654 / T93 / Q2.
      if (pickFlag) holdCtr[0x1121]++;
      rank = HR(QT_ACE, 3);
      trick[0].Set(QT_ACE, QT_ACE, rank, 4);
      trick[1].Set(QT_PARD, QT_BOTH, SDS_NINE, 4);
      trick[2].Set(QT_BOTH, QT_PARD, SDS_TEN, 3);
      trick[3].Set(QT_ACE, QT_ACE, SDS_VOID, 1);
      return def.Set112(trick);
    }
    else
    {
      // AKJ7 / 8xxx / T9x / Qx.
      if (pickFlag) holdCtr[0x1122]++;
      rank = SDS_VOID - 6;
      trick[0].Set(QT_PARD, QT_BOTH, rank, 4);
      trick[1].Set(QT_BOTH, QT_PARD, SDS_TEN, 3);
      trick[2].Set(QT_ACE, QT_ACE, SDS_VOID, 1);
      return def.Set12(trick[0], trick[1], trick[2]);
    }
  }
  else if (length[QT_ACE] >= 4 && length[QT_PARD] == 2 &&
      length[QT_LHO] >= 4 && length[QT_RHO] == 2)
  {
    if (htop.T == QT_PARD && htop.N == QT_PARD)
    {
      if (completeList[QT_LHO][0] > completeList[QT_ACE][3])
      {
        // AKJx / 8xxx / T9 / Qx.
        if (pickFlag) holdCtr[0x1123]++;
        rank = SDS_NINE;
        unsigned l = (length[QT_LHO] == 4 ? length[QT_ACE] : 4);
        trick[0].Set(QT_BOTH, QT_ACE, SDS_JACK, 3);
        trick[1].Set(QT_PARD, QT_PARD, rank, 2);
        trick[2].Set(QT_ACE, QT_ACE, SDS_VOID, l-2);
        return def.Set12(trick[0], trick[1], trick[2]);
      }
      else
      {
        // AKJ8 / 7654 / T9 / Q3.
        if (pickFlag) holdCtr[0x1124]++;
        rank = HR(QT_ACE, 3);
        trick[0].Set(QT_BOTH, QT_ACE, rank, length[QT_ACE]);
        trick[1].Set(QT_PARD, QT_PARD, SDS_NINE, 2);
        trick[2].Set(QT_ACE, QT_ACE, SDS_VOID, length[QT_ACE]-2);
        return def.Set12(trick[0], trick[1], trick[2]);
      }
    }
  }

  return false;
}


bool LoopHold::SolveComplex14(DefList& def, unsigned& rank) const
{
  // ==== G14 ================ G38 =====================================
  //      AJ+        |         A+
  // Q+         +    |  +             Q+
  //      K+         |         KJ+
  // ==== G14 ================ G38 =====================================

  if (pickFlag) holdCtr[0x1140]++;

  Trick trick[4];
  PosType pa, pl, pp, pr;
  if (htop.Q == QT_LHO)
  {
    // "G38".  The aceholder does not have the J.
    pa = QT_ACE;
    pl = QT_LHO;
    pp = QT_PARD;
    pr = QT_RHO;
  }
  else
  {
    pa = QT_PARD;
    pl = QT_RHO;
    pp = QT_ACE;
    pr = QT_LHO;
  }

  if (length[pa] == 4 && length[pp] > length[pa] && length[pl] == 3)
  {
    if (htop.T == pa)
    {
      if (htop.N == pa || (htop.N == pr && htop.E == pa))
      {
        // AJxxx / x / KT9x / Qxx.
        if (pickFlag) holdCtr[0x1141]++;
        rank = HR(pa, 2);
        trick[0].Set(QT_BOTH, QT_BOTH, rank, length[pp]);
        return def.Set1(trick[0]);
      }
      else if (completeList[pa][3] > completeList[pp][2])
      {
        // AJxxxx / - / KTxx / Q98.
        if (pickFlag) holdCtr[0x1142]++;
        rank = SDS_TEN;
        trick[0].Set(pa, pp, rank, 4);
        trick[1].Set(QT_BOTH, pa, rank, 4);
        trick[2].Set(pp, pp, SDS_VOID, length[pp]-4);
        return def.Set12(trick[0], trick[1], trick[2]);
      }
    }
    else if (completeList[pp][2] > completeList[pa][3])
    {
      // AJ8xxx / - / Kxxx / QT9.
      // For now
      return false;
    }
    else if (htop.T == pl)
    {
      // AJxxx / x / Kxxx / QTx.
      if (pickFlag) holdCtr[0x1143]++;
      rank = HR(pa, 1);
      trick[0].Set(pa, pp, rank, 4);
      trick[1].Set(QT_BOTH, pa, rank, 4);
      trick[2].Set(pp, pp, SDS_VOID, length[pp]-4);
      return def.Set12(trick[0], trick[1], trick[2]);
    }
  }
  else if (length[pp] == 4 && length[pa] > length[pp] && length[pl] == 3)
  {
    if (completeList[pa][1] > completeList[pp][3])
    {
      // AJxx / x / KTxxx / Qxx.
      if (pickFlag) holdCtr[0x1144]++;
      rank = HR(pa, 1);
      trick[0].Set(QT_BOTH, QT_BOTH, rank, length[pa]);
      return def.Set1(trick[0]);
    }
    else
    {
      // A76432 / QT5 / KJ98 / -.
      // Actually AP2K or BA48+PP1- seems right.
      return false;

      if (pickFlag) holdCtr[0x1145]++;
      rank = HR(pp, 2);
      trick[0].Set(QT_BOTH, pa, SDS_JACK, 3);
      trick[1].Set(QT_BOTH, pp, rank, 1);
      trick[2].Set(pa, pa, SDS_VOID, length[pa]-4);
      return def.Set3(trick[0], trick[1], trick[2]);
    }
  }
  else if (length[pa] >= 4 && length[pp] == 3 && length[pl] == 2)
  {
    if (length[pr] <= 3)
    {
      if (completeList[pa][1] > completeList[pp][2] &&
         (length[pr] <= 2 ||
          completeList[pa][1] > completeList[pr][0]))
      {
        // ATxx / Qx / KJx / (xxx).
        if (pickFlag) holdCtr[0x1146]++;
        rank = HR(pa, 1);
        trick[0].Set(pa, QT_BOTH, SDS_JACK, length[pa]);
        trick[1].Set(pp, QT_BOTH, rank, length[pa]);
        return def.Set11(trick[0], trick[1]);
      }
      else
      {
        // Axxx+ / QT / KJ9 / (xx).
        // Axxx+ / QT / KJx / 9xx.
        if (pickFlag) holdCtr[0x1147]++;
        rank = SDS_JACK;
        // trick[0].Set(pa, QT_BOTH, SDS_JACK, length[pa]);
        // trick[1].Set(pp, pa, SDS_KING, 2);
        // trick[2].Set(QT_BOTH, pp, SDS_JACK, 1);
        // trick[3].Set(pa, pa, SDS_VOID, length[pa]-3);
        // return def.Set13(trick);
        trick[0].Set(pa, QT_BOTH, SDS_JACK, length[pa]);
        trick[1].Set(pp, pa, SDS_JACK, 3);
        trick[2].Set(pp, pp, SDS_JACK, 3);
        trick[3].Set(pa, pa, SDS_VOID, length[pa]-3);
        return def.Set112(trick);
      }
    }
    else if (completeList[pp][2] > completeList[pa][1] &&
        completeList[pp][2] > completeList[pr][0])
    {
      if (completeList[pr][0] > completeList[pa][1])
      {
        // Axxx / Qx / KJT / 9xxx.
        if (pickFlag) holdCtr[0x1148]++;
        rank = HR(pp, 2);
        trick[0].Set(QT_BOTH, QT_BOTH, SDS_JACK, 3);
        trick[1].Set(pa, pp, rank, 3);
        trick[2].Set(pa, pa, SDS_VOID, 1);
        return def.Set12(trick[0], trick[1], trick[2]);
      }
      else
      {
        // A9xx / Qx / KJT / 8xxx.
        if (pickFlag) holdCtr[0x1149]++;
        rank = HR(pa, 1);
        trick[0].Set(pa, QT_BOTH, rank, 4);
        trick[1].Set(pp, pa, SDS_JACK, 3);
        trick[2].Set(pp, pp, SDS_JACK, 3);
        trick[3].Set(pa, pa, rank, 1);
        return def.Set112(trick);
      }
    }
    else if (completeList[pa][2] > completeList[pr][0])
    {
      // AT76 / Q9 / KJ8 / 5432.
      if (pickFlag) holdCtr[0x114a]++;
      unsigned r = HR(pa, 1);
      rank = HR(pa, 2);
      trick[0].Set(pa, QT_BOTH, r, 4);
      trick[1].Set(pp, QT_BOTH, rank, 4);
      return def.Set11(trick[0], trick[1]);
    }
    else if (completeList[pp][2] > completeList[pa][2] &&
        completeList[pp][2] > completeList[pr][0])
    {
      // AJ8 / 7654 / K932 / QT.
      if (pickFlag) holdCtr[0x114b]++;
      rank = HR(pa, 1);
      trick[0].Set(pa, QT_BOTH, rank, length[pa]);
      trick[1].Set(pp, pa, SDS_JACK, 3);
      trick[2].Set(pp, pp, SDS_JACK, 3);
      trick[3].Set(pa, pa, rank, 1);
      return def.Set112(trick);
    }
    else if (completeList[pa][1] > completeList[pr][0] &&
        completeList[pa][2] > completeList[pp][2] &&
        completeList[pa][2] < completeList[pr][0] &&
        completeList[pa][2] > completeList[pr][1])
    {
      // AT83 / Q7 / KJ6 / 9542.
      if (pickFlag) holdCtr[0x114c]++;
      unsigned r = HR(pa, 1);
      rank = HR(pa, 2);
      trick[0].Set(pa, QT_BOTH, r, 4);
      trick[1].Set(pp, pa, rank, 4);
      trick[2].Set(pp, pp, SDS_JACK, 3);
      trick[3].Set(pa, pa, r, 1);
      return def.Set112(trick);
    }
    else if (completeList[pa][1] > completeList[pr][1] &&
        completeList[pa][1] < completeList[pr][0] && 
        completeList[pa][1] > completeList[pp][2])
    {
      // AJ3 / T642 / K875 / Q9.
      if (pickFlag) holdCtr[0x114d]++;
      rank = HR(pa, 1);
      trick[0].Set(pa, QT_BOTH, rank, length[pa]);
      trick[1].Set(pp, QT_BOTH, SDS_JACK, length[pa]-1);
      return def.Set11(trick[0], trick[1]);
    }
    else if (completeList[pa][1] > completeList[pr][1] &&
        completeList[pa][1] > completeList[pp][2])
    {
      // AJ7 / 9643 / KT52 / Q8.
      if (pickFlag) holdCtr[0x114e]++;
      rank = HR(pa, 1);
      // trick[0].Set(pa, QT_BOTH, rank, length[pa]);
      // trick[1].Set(pp, pa, SDS_KING, 2);
      // trick[2].Set(QT_BOTH, pp, SDS_JACK, 1);
      // trick[3].Set(pa, pa, rank, 1);
      // return def.Set13(trick);
      trick[0].Set(pp, pa, SDS_JACK, 3);
      trick[1].Set(pa, QT_BOTH, rank, length[pa]);
      trick[2].Set(pp, pp, SDS_JACK, 3);
      trick[3].Set(pa, pa, rank, length[pa]-3);
      return def.Set112(trick);
    }
  }
  else if (length[pa] > length[pp] && length[pp] >= 4 && length[pl] == 2)
  {
    if (completeList[pp][length[pp]-1] > completeList[pa][1])
    {
      // Axxxx+ / QT / KJ98 / (xx).
      // Axxxxx / QT / KJ987 / -.
      if (pickFlag) holdCtr[0x114f]++;
      rank = HR(pp, length[pp]-2);
      trick[0].Set(pa, QT_BOTH, SDS_JACK, length[pa]);
      trick[1].Set(pp, pa, rank, length[pp]);
      trick[2].Set(pp, pp, rank, length[pp]);
      trick[3].Set(pa, pa, SDS_VOID, length[pa]-length[pp]);
      return def.Set112(trick);
    }
    else
    {
      // ATxxx / Qx / KJxx / (xx).
      // ATxxxx / Qx / KJxxx / -.
      if (pickFlag) holdCtr[0x1381]++;
      rank = HR(pa, 1);
      trick[0].Set(pa, QT_BOTH, SDS_JACK, length[pa]);
      trick[1].Set(pp, QT_BOTH, rank, length[pa]);
      return def.Set11(trick[0], trick[1]);
    }
  }
  else if (length[pa] >= 4 && length[pp] == 3 &&
      length[pl] == 1 && length[pr] >= 4)
  {
    if (htop.T == pr && htop.N == pa)
    {
      // A9xx / Q / KJx / Txxx.
      if (pickFlag) holdCtr[0x1382]++;
      rank = SDS_NINE;
      unsigned l = (length[pp] > 4 ? 4 : length[pa]);
      trick[0].Set(QT_BOTH, QT_BOTH, rank, l);
      return def.Set1(trick[0]);
    }
  }
  else if (length[pa] == 3 && length[pp] >= 4 &&
      length[pl] == 1 && length[pr] >= 4)
  {
    if (htop.T == pr && htop.N == pp && htop.E == pp)
    {
      // AJ98x / Txxx / Kxx / Q.
      if (pickFlag) holdCtr[0x1383]++;
      rank = SDS_EIGHT;
      unsigned l = (length[pr] > 4 ? 4 : length[pp]);
      trick[0].Set(QT_BOTH, QT_BOTH, SDS_JACK, 3);
      trick[1].Set(QT_BOTH, pa, rank, 3);
      trick[2].Set(pp, pp, SDS_VOID, l-3);
      return def.Set12(trick[0], trick[1], trick[2]);
      // trick[0].Set(QT_BOTH, pp, rank, 2);
      // trick[1].Set(QT_BOTH, pa, SDS_VOID, 1);
      // trick[2].Set(pp, pp, SDS_VOID, l-3);
      // return def.Set3(trick[0], trick[1], trick[2]);
    }
  }
  else if (length[pa] == 3 && length[pl] == 2 &&
      length[pp] == 4 && length[pr] == 4)
  {
    if (completeList[pa][1] > completeList[pr][0] &&
        completeList[pr][0] > completeList[pp][2])
    {
      // AJxx / 7xxx / K98 / QT.
      if (pickFlag) holdCtr[0x1385]++;
      rank = HR(pa, 1);
      trick[0].Set(QT_BOTH, QT_BOTH, HR(pp, 1), 3);
      trick[1].Set(QT_BOTH, pa, rank, 3);
      trick[2].Set(pp, pp, SDS_VOID, 1);
      return def.Set12(trick[0], trick[1], trick[2]);
    }
  }
  
  return false;
}


bool LoopHold::SolveComplex15(DefList& def, unsigned& rank) const
{
  // ==== G51 ==========================================================
  //      AQ+     
  // KJ+        +
  //      +     
  // ==== G51 ==========================================================

  if (pickFlag) holdCtr[0x1150]++;
  Trick trick[9];

  // Default values.  trick[1] is sometimes overwritten.
  trick[0].Set(QT_ACE, QT_ACE, SDS_ACE, 1);
  trick[1].Set(QT_PARD, QT_ACE, SDS_QUEEN, 2);

  unsigned m = Max(length[QT_LHO], length[QT_RHO]);

  if (length[QT_PARD] == 1 || length[QT_ACE] == 2)
  {
    if (length[QT_RHO] > 2 || 
       (length[QT_ACE] == 2 && length[QT_PARD] <= 2) ||
       (length[QT_LHO] > 2 && htop.T == QT_LHO))
    {
      // AQ / Txx / xxx / KJ.
      if (pickFlag) holdCtr[0x1151]++;
      rank = SDS_QUEEN;
      return def.Set11(trick[0], trick[1]);
    }
    else if (length[QT_PARD] == 1)
    {
      // AQ9 / 876 / T / KJ.
      if (pickFlag) holdCtr[0x1152]++;
      unsigned hasTops = Holding::TopsOverRank(QT_ACE,
        completeList[QT_LHO][0]);
      unsigned needTops = m;
      unsigned useTops = Min(hasTops, needTops);
      rank = HR(QT_ACE, useTops-1);
      unsigned l = (hasTops >= needTops ? length[QT_ACE] : hasTops);
      trick[1].Set(QT_PARD, QT_ACE, rank, l);
      return def.Set11(trick[0], trick[1]);
    }
    else if (length[QT_LHO] <= 2)
    {
      // AQ / xx / xxx / KJ.
      if (pickFlag) holdCtr[0x1153]++;
      rank = SDS_QUEEN;
      trick[2].Set(QT_PARD, QT_PARD, SDS_VOID, length[QT_PARD]-2);
      return def.Set12(trick[0], trick[1], trick[2]);
    }
    else 
    {
      // AQ / xxx / Txx / KJ.
      if (pickFlag) holdCtr[0x1154]++;
      unsigned hasTops = Holding::TopsOverRank(QT_PARD,
        completeList[QT_LHO][0]);
      unsigned needTops = Min(length[QT_LHO], length[QT_PARD]) - 2;
      unsigned useTops = Min(hasTops, needTops);
      rank = HR(QT_PARD, useTops-1);
      unsigned l = (hasTops >= needTops ? length[QT_PARD]-2 : useTops);
      trick[2].Set(QT_PARD, QT_PARD, rank, l);
      return def.Set12(trick[0], trick[1], trick[2]);
    }
  }
  else if (length[QT_RHO] == 2)
  {
    if (length[QT_PARD] == 2)
    {
      if (length[QT_LHO] <= 2)
      {
        // AQ9 / Tx / xx / KJ.
        if (pickFlag) holdCtr[0x1155]++;
        rank = SDS_QUEEN;
        trick[1].Set(QT_PARD, QT_ACE, rank, length[QT_ACE]);
        return def.Set11(trick[0], trick[1]);
      }
      else if (completeList[QT_LHO][0] > completeList[QT_ACE][2])
      {
        // AQ9 / Txx / xx / KJ.
        if (pickFlag) holdCtr[0x1156]++;
        rank = SDS_QUEEN;
        return def.Set11(trick[0], trick[1]);
      }
      else
      {
        // AQTx / 9xx / xx / KJ.
        if (pickFlag) holdCtr[0x1157]++;
        unsigned hasTops = Holding::TopsOverRank(QT_ACE,
          completeList[QT_LHO][0]);
        unsigned needTops = length[QT_LHO];
        unsigned useTops = Min(hasTops, needTops);
        rank = HR(QT_ACE, useTops-1);
        unsigned l = (hasTops >= needTops ? length[QT_ACE] : hasTops);
        trick[1].Set(QT_PARD, QT_ACE, rank, l);
        return def.Set11(trick[0], trick[1]);
      }
    }
    else if (length[QT_LHO] <= 2)
    {
      if (length[QT_ACE] == length[QT_PARD])
      {
        // AQ9 / xxx / Txx / KJ.
        if (completeList[QT_PARD][0] > 
            completeList[QT_ACE][length[QT_ACE]-1])
        {
          if (pickFlag) holdCtr[0x1158]++;
          rank = HR(QT_PARD, 0);
          trick[1].Set(QT_PARD, QT_PARD, rank, length[QT_ACE]);
          trick[2].Set(QT_PARD, QT_ACE, SDS_QUEEN, length[QT_ACE]);
          return def.Set111(trick);
        }
        else
        {
          if (pickFlag) holdCtr[0x1159]++;
          rank = SDS_QUEEN;
          trick[1].Set(QT_PARD, QT_ACE, rank, length[QT_ACE]);
          return def.Set11(trick[0], trick[1]);
        }
      }
      else if (length[QT_PARD] > length[QT_ACE])
      {
        if (completeList[QT_PARD][0] > 
            completeList[QT_ACE][length[QT_ACE]-1])
        {
          // AQx / xx / Txxx / KJ.
          if (pickFlag) holdCtr[0x115a]++;
          rank = HR(QT_PARD, 0);
          trick[1].Set(QT_PARD, QT_BOTH, rank, length[QT_PARD]);
          return def.Set11(trick[0], trick[1]);
        }
        else
        {
          // AQT / xx / 9xxx / KJ.
          if (pickFlag) holdCtr[0x115b]++;
          rank = HR(QT_ACE, length[QT_ACE]-1);
          trick[1].Set(QT_PARD, QT_ACE, rank, length[QT_ACE]);
          trick[2].Set(QT_PARD, QT_PARD, SDS_VOID, 
            length[QT_PARD] - length[QT_ACE]);
          return def.Set12(trick[0], trick[1], trick[2]);
        }
      }
      else if (length[QT_PARD] == 3)
      {
        if (completeList[QT_PARD][2] > completeList[QT_ACE][2])
        {
          // AQxx / xx / T98 / KJ.
          if (pickFlag) holdCtr[0x115c]++;
          rank = HR(QT_PARD, 0);
          trick[1].Set(QT_PARD, QT_ACE, rank, 3);
          trick[2].Set(QT_PARD, QT_PARD, rank, 3);
          trick[3].Set(QT_ACE, QT_ACE, SDS_VOID, length[QT_ACE]-3);
          return def.Set112(trick);
        }
        else if (completeList[QT_PARD][0] < 
            completeList[QT_ACE][length[QT_ACE]-1])
        {
          // AQT9 / xx / xxx / KJ.
          if (pickFlag) holdCtr[0x115d]++;
          rank = HR(QT_ACE, 2);
          trick[1].Set(QT_PARD, QT_ACE, rank, length[QT_ACE]);
          return def.Set11(trick[0], trick[1]);
        }
        else
        {
          // AQ8x / xx / T97 / KJ.
          if (pickFlag) holdCtr[0x115e]++;
          unsigned r1 = HR(QT_PARD, 0);
          unsigned r2 = HR(QT_ACE, 2);
          rank = Min(r1, r2);
          trick[1].Set(QT_PARD, QT_ACE, r2, length[QT_ACE]);
          trick[2].Set(QT_PARD, QT_PARD, r1, 3);
          trick[3].Set(QT_ACE, QT_ACE, SDS_VOID, length[QT_ACE]-3);
          return def.Set112(trick);
        }
      }
      else if (completeList[QT_PARD][0] > 
          completeList[QT_ACE][length[QT_ACE]-1])
      {
        // AQxxx / xx / Txxx / KJ.
        rank = HR(QT_PARD, 0);
        unsigned r = HR(QT_ACE, 2);
        if (length[QT_PARD] >= 4 &&
            completeList[QT_PARD][length[QT_PARD]-1] >
            completeList[QT_ACE][2])
        {
          // QT543 / 92 / T876 / KJ.
          if (pickFlag) holdCtr[0x115f]++;
          rank = HR(QT_PARD, length[QT_PARD]-3);
          trick[1].Set(QT_PARD, QT_ACE, rank, length[QT_PARD]);
          trick[2].Set(QT_PARD, QT_PARD, rank, length[QT_PARD]);
          trick[3].Set(QT_ACE, QT_ACE, SDS_VOID, 
            length[QT_ACE] - length[QT_PARD]);
          return def.Set112(trick);
        }
        else if (r < rank)
        {
          // The way it comes out.
          if (completeList[QT_PARD][2] > completeList[QT_ACE][2])
          {
            if (length[QT_PARD] == 5 &&
                completeList[QT_ACE][2] > completeList[QT_PARD][3])
            {
              if (pickFlag) holdCtr[0x1160]++;
              rank = r;
              trick[1].Set(QT_PARD, QT_BOTH, rank, length[QT_ACE]);
              return def.Set11(trick[0], trick[1]);
            }
            else
            {
              // PROBLEM: Should be HR(QT_PARD, 0)!
              if (pickFlag) holdCtr[0x1160]++;
              rank = r;
              unsigned ra1 = (length[QT_PARD] == 4 ? HR(QT_PARD, 1) :
                HR(QT_PARD, 2));
              trick[1].Set(QT_PARD, QT_BOTH, rank, length[QT_ACE]);
              trick[2].Set(QT_PARD, QT_PARD, ra1, length[QT_PARD]);
              trick[3].Set(QT_ACE, QT_ACE, SDS_VOID, 
                length[QT_ACE] - length[QT_PARD]);
              return def.Set112(trick);
            }
          }
          else
          {
            if (pickFlag) holdCtr[0x1160]++;
            rank = r;
            trick[1].Set(QT_PARD, QT_BOTH, rank, length[QT_ACE]);
            return def.Set11(trick[0], trick[1]);
          }
        }
        else
        {
          if (pickFlag) holdCtr[0x1161]++;
          trick[1].Set(QT_PARD, QT_ACE, r, length[QT_ACE]);
          trick[2].Set(QT_PARD, QT_PARD, rank, length[QT_ACE]);
          return def.Set111(trick);
        }
      }
      else
      {
        // AQT98 / xx / xxxx / KJ.
        if (pickFlag) holdCtr[0x1162]++;
        rank = HR(QT_ACE, 2);
        trick[1].Set(QT_PARD, QT_ACE, rank, length[QT_ACE]);
        return def.Set11(trick[0], trick[1]);
      }
    }
    else if (htop.T == QT_LHO)
    {
      // AQx / Txx / xxx / KJ.
      if (pickFlag) holdCtr[0x1163]++;
      rank = SDS_QUEEN;
      return def.Set11(trick[0], trick[1]);
    }
    else if (length[QT_ACE] == 3 && length[QT_PARD] == 3)
    {
      // AQT / xxx / xxx / KJ.
      // AQx / xxx / Txx / KJ.
      PosType e = (htop.T == QT_ACE ? QT_ACE : QT_BOTH);
      if (pickFlag) holdCtr[0x1164]++;
      rank = SDS_TEN;
      trick[1].Set(QT_PARD, e, rank, 3);
      return def.Set11(trick[0], trick[1]);
    }
    else if (length[QT_ACE] == 4 && length[QT_PARD] == 4)
    {
      if (completeList[QT_PARD][0] > completeList[QT_ACE][3] &&
          htop.T == QT_ACE)
      {
        // AQTx / xxx / 9xxx / KJ.
        if (pickFlag) holdCtr[0x1165]++;
        rank = HR(QT_PARD, 0);
        trick[1].Set(QT_PARD, QT_ACE, SDS_TEN, 4);
        trick[2].Set(QT_PARD, QT_PARD, rank, 4);
        return def.Set111(trick);
      }
      else
      {
        // AQT9 / xxx / xxxx / KJ.
        // AQxx / xxx / Txxx / KJ.
        if (pickFlag) holdCtr[0x1166]++;
        PosType e = (completeList[QT_PARD][0] < completeList[QT_ACE][3] ?
          QT_ACE : QT_BOTH);
        rank = SDS_TEN;
        trick[1].Set(QT_PARD, e, rank, 4);
        return def.Set11(trick[0], trick[1]);
      }
    }
    else if (length[QT_ACE] < length[QT_PARD])
    {
      if (completeList[QT_ACE][length[QT_ACE]-1] > 
          completeList[QT_PARD][0])
      {
        if (completeList[QT_PARD][0] < completeList[QT_LHO][0] &&
            distHex == 0x3442)
        {
          if (pickFlag) holdCtr[0x1167]++;
          rank = SDS_TEN;
          trick[1].Set(QT_PARD, QT_ACE, SDS_TEN, 3);
          return def.Set11(trick[0], trick[1]);
        }
        else
        {
          if (pickFlag) holdCtr[0x1168]++;
          unsigned r = (distHex == 0x3442 ? HR(QT_PARD, 0) : SDS_VOID);
          rank = Min(SDS_TEN, r);
          trick[1].Set(QT_PARD, QT_ACE, SDS_TEN, 3);
          trick[2].Set(QT_PARD, QT_PARD, r, length[QT_PARD]-3);
          return def.Set12(trick[0], trick[1], trick[2]);
        }
      }
      else if (distHex == 0x3442)
      {
        if (completeList[QT_LHO][0] > completeList[QT_PARD][1] &&
            completeList[QT_ACE][2] > completeList[QT_LHO][0])
        {
          if (pickFlag) holdCtr[0x1169]++;
          rank = HR(QT_ACE, 2);
          trick[1].Set(QT_PARD, QT_BOTH, SDS_TEN, 3);
          trick[2].Set(QT_PARD, QT_ACE, rank, 3);
          trick[3].Set(QT_PARD, QT_PARD, SDS_VOID, 1);
          return def.Set112(trick);
        }
        else if (completeList[QT_LHO][1] > completeList[QT_PARD][1])
        {
          if (pickFlag) holdCtr[0x116a]++;
          rank = SDS_TEN;
          trick[1].Set(QT_PARD, QT_BOTH, rank, 3);
          return def.Set11(trick[0], trick[1]);
        }
        else if (completeList[QT_ACE][2] > completeList[QT_PARD][1] &&
            completeList[QT_LHO][0] > completeList[QT_PARD][1])
        {
          // Mixed.
          if (pickFlag) holdCtr[0x116b]++;
          return false;
        }
        else
        {
          if (pickFlag) holdCtr[0x116c]++;
          rank = HR(QT_PARD, 1);
          trick[1].Set(QT_PARD, QT_BOTH, rank, 4);
          return def.Set11(trick[0], trick[1]);
        }
      }
      else
      {
        if (pickFlag) holdCtr[0x116d]++;
        unsigned l = 0;
        if (length[QT_LHO] > 3)
          l = Min(length[QT_PARD], length[QT_LHO]) - 3;
        rank = HR(QT_PARD, l);
        trick[1].Set(QT_PARD, QT_BOTH, rank, length[QT_PARD]);
        return def.Set11(trick[0], trick[1]);
      }
    }
    else if (length[QT_PARD] == 3)
    {
      if (length[QT_LHO] == 3)
      {
        if (completeList[QT_ACE][2] < completeList[QT_LHO][0] ||
            completeList[QT_ACE][2] < completeList[QT_PARD][2])
        {
          if (pickFlag) holdCtr[0x116e]++;
          rank = SDS_TEN;
          trick[1].Set(QT_PARD, QT_ACE, rank, 3);
          trick[2].Set(QT_PARD, QT_PARD, rank, 3);
          trick[3].Set(QT_ACE, QT_ACE, SDS_VOID, length[QT_ACE]-3);
          return def.Set112(trick);
        }
        else if (completeList[QT_PARD][0] >
            completeList[QT_ACE][length[QT_ACE]-1] &&
            completeList[QT_PARD][0] > completeList[QT_LHO][0])
        {
          if (pickFlag) holdCtr[0x116f]++;
          unsigned r1 = HR(QT_PARD, 0);
          unsigned r2 = HR(QT_ACE, 2);
          rank = Min(r1, r2);
          trick[1].Set(QT_PARD, QT_ACE, r2, length[QT_ACE]);
          trick[2].Set(QT_PARD, QT_PARD, r1, 3);
          trick[3].Set(QT_ACE, QT_ACE, SDS_VOID, length[QT_ACE]-3);
          return def.Set112(trick);
        }
        else
        {
          if (pickFlag) holdCtr[0x1170]++;
          rank = HR(QT_ACE, 2);
          trick[1].Set(QT_PARD, QT_ACE, rank, length[QT_ACE]);
          return def.Set11(trick[0], trick[1]);
        }
      }
      else if (completeList[QT_ACE][2] < completeList[QT_LHO][0])
      {
        if (pickFlag) holdCtr[0x1171]++;
        rank = SDS_TEN;
        trick[1].Set(QT_PARD, QT_BOTH, rank, 3);
        return def.Set11(trick[0], trick[1]);
      }
      else if (completeList[QT_PARD][0] > completeList[QT_LHO][0])
      {
        if (completeList[QT_ACE][3] > completeList[QT_LHO][0] &&
            completeList[QT_ACE][2] > completeList[QT_PARD][2])
        {
          if (completeList[QT_ACE][3] > completeList[QT_PARD][0])
          {
            if (pickFlag) holdCtr[0x1172]++;
            rank = HR(QT_ACE, 3);
            trick[1].Set(QT_PARD, QT_ACE, rank, 4);
            return def.Set11(trick[0], trick[1]);
          }
          else
          {
            if (pickFlag) holdCtr[0x1173]++;
            unsigned r1 = HR(QT_PARD, 0);
            unsigned r2 = HR(QT_ACE, 3);
            unsigned r3 = HR(QT_ACE, 2);
            if (r3 > r1)
              r3 = SDS_VOID;
            rank = Min(r2, r3);
            trick[1].Set(QT_PARD, QT_ACE, r2, 4);
            trick[2].Set(QT_PARD, QT_PARD, r1, 3);
            trick[3].Set(QT_ACE, QT_ACE, r3, 1);
            return def.Set112(trick);
          }
        }
        else
        {
          if (pickFlag) holdCtr[0x1174]++;
          unsigned r1 = HR(QT_PARD, 0);
          unsigned r2 = HR(QT_ACE, 2);
          if (r2 < r1)
          {
            rank = r2;
            trick[1].Set(QT_PARD, QT_ACE, r1, 3);
            trick[2].Set(QT_PARD, QT_PARD, r1, 3);
            trick[3].Set(QT_ACE, QT_ACE, r2, length[QT_ACE]-3);
            return def.Set112(trick);
          }
          else
          {
            rank = r1;
            trick[1].Set(QT_PARD, QT_ACE, r2, 3);
            trick[2].Set(QT_PARD, QT_PARD, r1, 3);
            trick[3].Set(QT_ACE, QT_ACE, SDS_VOID, length[QT_ACE]-3);
            return def.Set112(trick);
          }
        }
      }
      else if (distHex == 0x4432 &&
          completeList[QT_ACE][3] < completeList[QT_LHO][0])
      {
        if (pickFlag) holdCtr[0x1175]++;
        rank = HR(QT_ACE, 2);
        trick[1].Set(QT_PARD, QT_ACE, rank, 3);
        return def.Set11(trick[0], trick[1]);
      }
      else
      {
        if (pickFlag) holdCtr[0x1176]++;
        rank = HR(QT_ACE, length[QT_ACE]-1);
        trick[1].Set(QT_PARD, QT_ACE, rank, length[QT_ACE]);
        return def.Set11(trick[0], trick[1]);
      }
    }
    else if (completeList[QT_PARD][0] <
        completeList[QT_ACE][length[QT_ACE]-1])
    {
      if (pickFlag) holdCtr[0x1177]++;
      rank = SDS_TEN;
      trick[1].Set(QT_PARD, QT_ACE, rank, 2);
      return def.Set11(trick[0], trick[1]);
    }
    else if (completeList[QT_PARD][length[QT_PARD]-1] >
        completeList[QT_ACE][2])
    {
      if (pickFlag) holdCtr[0x1178]++;
      rank = HR(QT_PARD, 0);
      trick[2].Set(QT_BOTH, QT_PARD, rank, length[QT_PARD]-2);
      trick[3].Set(QT_ACE, QT_ACE, SDS_VOID, 
        length[QT_ACE]-length[QT_PARD]);
      return def.Set13(trick);
    }
    else
    {
      if (pickFlag) holdCtr[0x1179]++;
      rank = HR(QT_PARD, 0);
      trick[1].Set(QT_PARD, QT_BOTH, rank, 2);
      return def.Set11(trick[0], trick[1]);
    }
  }
  else if (hopp.T || 
      (htop.T == QT_PARD && 
        (htop.N == QT_RHO || 
          (htop.N == QT_LHO && 
            (length[QT_LHO] >= 2 || 
              (completeList[QT_RHO][2] > completeList[QT_ACE][2] &&
                (length[QT_PARD] == 2 ||
                completeList[QT_RHO][2] > completeList[QT_PARD][2])))))) ||
      (htop.T == QT_PARD && htop.N == QT_PARD &&
        (htop.E == QT_RHO || 
          (htop.E == QT_LHO && 
            (length[QT_LHO] >= 3 || 
              (completeList[QT_RHO][2] > completeList[QT_ACE][2] &&
                (length[QT_PARD] == 2 ||
                  completeList[QT_RHO][2] > completeList[QT_PARD][2])))))))
  {
    // AQx / Txx / 9xx / KJ.
    if (pickFlag) holdCtr[0x117a]++;
    rank = SDS_QUEEN;
    return def.Set11(trick[0], trick[1]);
  }
  else if (length[QT_PARD] == 2)
  {
    if ((htop.T == QT_PARD && htop.N == QT_PARD && htop.E == QT_ACE) ||
        (length[QT_LHO] == 1 && htop.T == QT_PARD && 
         completeList[QT_PARD][1] > completeList[QT_ACE][2] &&
         completeList[QT_ACE][2] > completeList[QT_RHO][2]) ||
        (length[QT_LHO] == 2 && htop.T == QT_PARD && htop.N == QT_PARD &&
         completeList[QT_PARD][1] > completeList[QT_ACE][2] &&
         completeList[QT_ACE][2] > completeList[QT_RHO][2]))
    {
      // Mixed.
      if (pickFlag) holdCtr[0x117b]++;
      return false;
    }
    else if (htop.T == QT_ACE ||
       (htop.T == QT_PARD && htop.N == QT_ACE))
    {
      if (pickFlag) holdCtr[0x117c]++;
      unsigned l = (length[QT_RHO] == 3 ? length[QT_ACE] : 3);
      unsigned r1, r2;

      if (distHex == 0x4423 &&
          htop.T == QT_PARD && htop.N == QT_ACE && htop.E == QT_PARD &&
          completeList[QT_ACE][3] > completeList[QT_LHO][0])
      {
        // AQ97 / 6543 / T8 / KJ2, AQ96 / 5432 / T8 / KJ7.
        // PROBLEM, or perhaps really mixed.
        return false;
      }

      if (completeList[QT_PARD][0] > completeList[QT_RHO][2] &&
          completeList[QT_PARD][0] > 
            completeList[QT_ACE][length[QT_ACE]-1] &&
          (length[QT_LHO] == 0 ||
            completeList[QT_PARD][0] > completeList[QT_LHO][0]))
      {
        if (distHex == 0x4423)
        {
          if (completeList[QT_ACE][3] > completeList[QT_LHO][0])
          {
            r1 = SDS_QUEEN;
            r2 = HR(QT_ACE, 3);
          }
          else if (htop.T == QT_ACE)
          {
            r1 = SDS_TEN;
            r2 = SDS_VOID;
            l--;
          }
          else
          {
            r1 = SDS_QUEEN;
            r2 = HR(QT_ACE, 2);
            l--;
          }
        }
        else if (htop.T == QT_PARD)
        {
          r1 = SDS_QUEEN;
          r2 = HR(QT_ACE, 2);
        }
        else if (length[QT_ACE] == 4)
        {
          // r1 = SDS_QUEEN;
          // r2 = SDS_TEN;
          r1 = SDS_TEN;
          r2 = SDS_VOID;
        }
        else if (htop.N == QT_PARD)
        {
          // r1 = SDS_QUEEN;
          // r2 = SDS_TEN;
          r1 = SDS_TEN;
          r2 = SDS_VOID;
        }
        else if (completeList[QT_PARD][1] > completeList[QT_RHO][2] &&
            completeList[QT_PARD][1] > 
              completeList[QT_ACE][length[QT_ACE]-1] &&
            (length[QT_LHO] == 0 || 
             completeList[QT_PARD][1] > completeList[QT_LHO][0]))
        {
          // r1 = SDS_QUEEN;
          // r2 = SDS_TEN;
          r1 = SDS_TEN;
          r2 = SDS_VOID;
        }
        else
        {
          // PROBLEM:  Shouldn't be necessary to distinguish length.
          // But here we have a run of 3+ for the ranks, and somehow
          // that is treated differently in PossiblyFixRank.
          r1 = SDS_TEN;
          r2 = SDS_VOID;
        }
      }
      else if (distHex == 0x4423)
      {
        if (completeList[QT_ACE][3] > completeList[QT_LHO][0])
        {
          r1 = SDS_TEN;
          r2 = HR(QT_ACE, 3);
        }
        else
        {
          r1 = SDS_TEN;
          r2 = SDS_VOID;
          l--;
        }
      }
      else
      {
        r1 = SDS_TEN;
        r2 = SDS_VOID;
      }
      rank = Min(r1, r2);
      trick[2].Set(QT_PARD, QT_ACE, r1, 1);
      trick[3].Set(QT_PARD, QT_ACE, r2, l-1);
      return def.Set112(trick);
    }
    else
    {
      if (pickFlag) holdCtr[0x117d]++;
      rank = HR(QT_ACE, 2);
      unsigned l = (length[QT_RHO] == 3 ? length[QT_ACE] : 3);
      trick[2].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
      trick[3].Set(QT_PARD, QT_ACE, rank, l-1);
      return def.Set112(trick);
    }
  }
  else if (length[QT_ACE] == 3)
  {
    if (htop.T == QT_PARD && 
        completeList[QT_PARD][2] > completeList[QT_ACE][2] &&
        completeList[QT_PARD][2] > completeList[QT_RHO][2] &&
        (length[QT_LHO] <= 1 ||
        (length[QT_LHO] == 2 && 
          completeList[QT_PARD][1] > completeList[QT_LHO][0]) ||
        (length[QT_LHO] >= 3 &&
          completeList[QT_PARD][2] > completeList[QT_LHO][0])))
    {
      if (length[QT_PARD] == 3 || length[QT_RHO] > 3)
      {
        if (pickFlag) holdCtr[0x117e]++;
        rank = HR(QT_PARD, 2);
        trick[2].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
        trick[3].Set(QT_PARD, QT_BOTH, rank, length[QT_ACE]-1);
        return def.Set112(trick);
      }
      else
      {
        if (pickFlag) holdCtr[0x117f]++;
        rank = HR(QT_PARD, 2);
        // trick[2].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
        // trick[3].Set(QT_PARD, QT_PARD, rank, 1);
        // trick[4].Set(QT_BOTH, QT_ACE, SDS_VOID, 1);
        // trick[5].Set(QT_PARD, QT_PARD, SDS_VOID, length[QT_PARD]-3);
        // return def.Set114(trick);
        trick[2].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
        trick[3].Set(QT_PARD, QT_PARD, rank, 2);
        trick[4].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
        trick[5].Set(QT_PARD, QT_ACE, rank, 2);
        trick[6].Set(QT_PARD, QT_PARD, SDS_VOID, length[QT_PARD]-3);
        return def.Set1123(trick);
      }
    }
    else if (length[QT_PARD] > 3 && length[QT_RHO] > 3 &&
        completeList[QT_ACE][2] > completeList[QT_PARD][2] &&
        completeList[QT_ACE][2] < completeList[QT_PARD][1] &&
        completeList[QT_ACE][2] > completeList[QT_RHO][2] &&
        ((length[QT_LHO] == 0 ||
         (length[QT_LHO] == 1 && 
          completeList[QT_LHO][0] < completeList[QT_PARD][0]) ||
         (length[QT_LHO] == 2 &&
          completeList[QT_LHO][0] < completeList[QT_PARD][1]))))
    {
      // Mixed.
      if (pickFlag) holdCtr[0x11a0]++;
      return false;
    }
    else
    {
      unsigned r1, r2;
      if (htop.T == QT_ACE)
      {
        r1 = SDS_TEN;
        r2 = SDS_VOID;
      }
      else
      {
        r1 = SDS_QUEEN;
        r2 = HR(QT_ACE, 2);
      }
      rank = Min(r1, r2);

      if (length[QT_PARD] == 3 || length[QT_RHO] > 3)
      {
        if (pickFlag) holdCtr[0x11a1]++;
        trick[2].Set(QT_PARD, QT_ACE, r1, 1);
        trick[3].Set(QT_PARD, QT_ACE, r2, length[QT_ACE]-1);
        return def.Set112(trick);
      }
      else
      {
        if (pickFlag) holdCtr[0x11a2]++;
        trick[2].Set(QT_PARD, QT_ACE, r1, 1);
        trick[3].Set(QT_PARD, QT_ACE, r2, 2);
        trick[4].Set(QT_PARD, QT_PARD, SDS_VOID, length[QT_PARD]-3);
        return def.Set113(trick);
      }
    }
  }
  else if (length[QT_RHO] == 3)
  {
    if (htop.T == QT_ACE)
    {
      if (completeList[QT_PARD][0] > completeList[QT_RHO][2] &&
          completeList[QT_PARD][0] > 
            completeList[QT_ACE][length[QT_ACE]-1] &&
          (length[QT_LHO] == 0 ||
          completeList[QT_PARD][0] > completeList[QT_LHO][0]))
      {
        if (length[QT_PARD] == 3)
        {
          if ((length[QT_LHO] == 3 &&
              completeList[QT_LHO][0] > completeList[QT_PARD][2]) ||
              (length[QT_LHO] == 2 &&
              completeList[QT_LHO][0] > completeList[QT_PARD][1]) ||
             completeList[QT_PARD][2] < 
                completeList[QT_ACE][length[QT_ACE]-1] ||
              completeList[QT_PARD][2] < completeList[QT_RHO][2])

          {
            // AQT97 / - / 865 / KJ4.
            // PA1Q + PA48 is also possible, in a sense.
            if (pickFlag) holdCtr[0x11a3]++;

            // The way it comes out.  Shouldn't be necessary.
            unsigned r1, r2;
            if (htop.N == QT_ACE)
            {
              r1 = SDS_TEN;
              r2 = SDS_VOID;
            }
            else
            {
              // r1 = SDS_QUEEN;
              // r2 = SDS_TEN;
              r1 = SDS_TEN;
              r2 = SDS_VOID;
            }
            rank = SDS_TEN;

            trick[2].Set(QT_PARD, QT_ACE, r1, 1);
            trick[3].Set(QT_PARD, QT_ACE, r2, length[QT_ACE]-1);
            return def.Set112(trick);
          }
          else
          {
            if (pickFlag) holdCtr[0x11a4]++;
            // The way it comes out.  Probably wrong.
            rank = HR(QT_PARD, 2);
            unsigned r1, r2;
            if (htop.N == QT_PARD)
            {
              // Even worse than before...
              if (length[QT_LHO] > 0 &&
                  completeList[QT_LHO][0] > 
                  completeList[QT_PARD][length[QT_PARD]-1])
              {
                // Have to lead the high card.
                // r1 = SDS_QUEEN;
                // r2 = SDS_TEN;
                r1 = SDS_TEN;
                r2 = SDS_VOID;
              }
              else
              {
                r1 = SDS_TEN;
                r2 = SDS_VOID;
              }
            }
            else if (length[QT_LHO] > 0 &&
                completeList[QT_LHO][0] > completeList[QT_PARD][2])
            {
              r1 = SDS_TEN;
              r2 = SDS_VOID;
            }
            else
            {
              // Ditto...
              if (length[QT_LHO] > 0 &&
                  completeList[QT_LHO][0] > 
                  completeList[QT_PARD][length[QT_PARD]-1])
              {
                // Have to lead the high card.
                // r1 = SDS_QUEEN;
                // r2 = HR(QT_PARD, 0);
                r1 = SDS_TEN;
                r2 = SDS_VOID;
              }
              else
              {
                r1 = SDS_TEN;
                r2 = SDS_VOID;
              }
            }

            trick[2].Set(QT_PARD, QT_ACE, r1, 1);
            trick[3].Set(QT_PARD, QT_ACE, r2, length[QT_ACE]-1);
            trick[4].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
            trick[5].Set(QT_PARD, QT_PARD, rank, 2);
            trick[6].Set(QT_ACE, QT_ACE, SDS_VOID, length[QT_ACE]-3);
            return def.Set1123(trick);
          }
        }
        else if (length[QT_ACE] == length[QT_PARD])
        {
          if (completeList[QT_PARD][1] > 
              completeList[QT_ACE][length[QT_ACE]-1])
          {
            if (length[QT_ACE] == 4)
            {
              if (pickFlag) holdCtr[0x11a5]++;
              /*
              unsigned r1a, r1b, r2a, r2b;
              unsigned pLast = completeList[QT_PARD][length[QT_PARD]-1];
              r1a = SDS_QUEEN;
              r1b = (htop.N == QT_PARD ? SDS_TEN : HR(QT_PARD, 0));
              r2a = SDS_QUEEN;
              r2b = HR(QT_PARD, 1);
              rank = r2b;

              trick[2].Set(QT_PARD, QT_ACE, r1a, 1);
              trick[3].Set(QT_PARD, QT_ACE, r1b, length[QT_ACE]-1);
              trick[4].Set(QT_PARD, QT_ACE, r2a, 1);
              trick[5].Set(QT_PARD, QT_PARD, r2b, length[QT_ACE]-1);
              return def.Set1122(trick);
              */
              if (pickFlag) holdCtr[0x11a5]++;
              unsigned r1a, r1b, r2a, r2b;
              unsigned pLast = completeList[QT_PARD][length[QT_PARD]-1];
              r1a = SDS_QUEEN;
              r1b = (htop.N == QT_PARD ? SDS_TEN : HR(QT_PARD, 0));
              r2a = SDS_QUEEN;
              r2b = HR(QT_PARD, 1);
              rank = r2b;

              trick[2].Set(QT_PARD, QT_ACE, SDS_TEN, 1);
              trick[3].Set(QT_PARD, QT_ACE, SDS_VOID, length[QT_ACE]-1);
              trick[4].Set(QT_PARD, QT_ACE, SDS_TEN, 1);
              trick[5].Set(QT_PARD, QT_PARD, HR(QT_PARD, 0), 
                length[QT_ACE]-1);
              return def.Set1122(trick);
            }
            else if (htop.N == QT_ACE &&
               (completeList[QT_PARD][4] < completeList[QT_ACE][4] ||
                completeList[QT_PARD][4] < completeList[QT_RHO][2]))
            {
              // PROBLEM.
              if (pickFlag) holdCtr[0x11a5]++;
              unsigned r1a, r1b, r2a, r2b;
              unsigned pLast = completeList[QT_PARD][length[QT_PARD]-1];
              r1a = SDS_QUEEN;
              r1b = (htop.N == QT_PARD ? SDS_TEN : HR(QT_PARD, 0));
              r2a = SDS_QUEEN;
              r2b = HR(QT_PARD, 1);
              rank = r2b;

              trick[2].Set(QT_PARD, QT_ACE, SDS_TEN, 1);
              trick[3].Set(QT_PARD, QT_ACE, SDS_VOID, length[QT_ACE]-1);
              trick[4].Set(QT_PARD, QT_ACE, SDS_TEN, 1);
              trick[5].Set(QT_PARD, QT_PARD, HR(QT_PARD, 0), 
                length[QT_ACE]-1);
              return def.Set1122(trick);
            }
            else
            {
              if (pickFlag) holdCtr[0x11a5]++;
              unsigned r1a, r1b, r2a, r2b;
              unsigned pLast = completeList[QT_PARD][length[QT_PARD]-1];
              r1a = SDS_QUEEN;
              r1b = (htop.N == QT_PARD ? SDS_TEN : HR(QT_PARD, 0));
              r2a = SDS_QUEEN;
              r2b = HR(QT_PARD, 1);
              rank = r2b;

              trick[2].Set(QT_PARD, QT_ACE, SDS_TEN, 1);
              trick[3].Set(QT_PARD, QT_ACE, SDS_VOID, length[QT_ACE]-1);
              trick[4].Set(QT_PARD, QT_ACE, SDS_TEN, 1);
              trick[5].Set(QT_PARD, QT_PARD, HR(QT_PARD, 0), 
                length[QT_ACE]-1);
              return def.Set1122(trick);
            }
          }
          else if (length[QT_ACE] == 4)
          {
            if (pickFlag) holdCtr[0x11a6]++;
            // rank = SDS_TEN;
            // trick[2].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
            // trick[3].Set(QT_PARD, QT_ACE, SDS_TEN, length[QT_ACE]-1);
            // return def.Set112(trick);

              unsigned r1a, r1b, r2a, r2b;
              unsigned pLast = completeList[QT_PARD][length[QT_PARD]-1];
              r1a = SDS_QUEEN;
              r1b = (htop.N == QT_PARD ? SDS_TEN : HR(QT_PARD, 0));
              r2a = SDS_QUEEN;
              r2b = HR(QT_PARD, 1);
              rank = r2b;

              trick[2].Set(QT_PARD, QT_ACE, SDS_TEN, 1);
              trick[3].Set(QT_PARD, QT_ACE, SDS_VOID, length[QT_ACE]-1);
              trick[4].Set(QT_PARD, QT_ACE, SDS_TEN, 1);
              trick[5].Set(QT_PARD, QT_PARD, HR(QT_PARD, 0), 
                length[QT_ACE]-1);
              return def.Set1122(trick);

          }
          else if (htop.N == QT_PARD)
          {
            if (pickFlag) holdCtr[0x11a6]++;
            /*
            rank = SDS_TEN;
            trick[2].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
            trick[3].Set(QT_PARD, QT_ACE, SDS_TEN, length[QT_ACE]-1);
            return def.Set112(trick);
            */

              unsigned r1a, r1b, r2a, r2b;
              unsigned pLast = completeList[QT_PARD][length[QT_PARD]-1];
              r1a = SDS_QUEEN;
              r1b = (htop.N == QT_PARD ? SDS_TEN : HR(QT_PARD, 0));
              r2a = SDS_QUEEN;
              r2b = HR(QT_PARD, 1);
              rank = r2b;

              trick[2].Set(QT_PARD, QT_ACE, SDS_TEN, 1);
              trick[3].Set(QT_PARD, QT_ACE, SDS_VOID, length[QT_ACE]-1);
              trick[4].Set(QT_PARD, QT_ACE, SDS_TEN, 1);
              trick[5].Set(QT_PARD, QT_PARD, HR(QT_PARD, 0), 
                length[QT_ACE]-1);
              return def.Set1122(trick);

          }
          else
          {
              if (pickFlag) holdCtr[0x11a5]++;
              unsigned r1a, r1b, r2a, r2b;
              unsigned pLast = completeList[QT_PARD][length[QT_PARD]-1];
              r1a = SDS_QUEEN;
              r1b = (htop.N == QT_PARD ? SDS_TEN : HR(QT_PARD, 0));
              r2a = SDS_QUEEN;
              r2b = HR(QT_PARD, 1);
              rank = r2b;

              trick[2].Set(QT_PARD, QT_ACE, SDS_TEN, 1);
              trick[3].Set(QT_PARD, QT_ACE, SDS_VOID, length[QT_ACE]-1);
              trick[4].Set(QT_PARD, QT_ACE, SDS_TEN, 1);
              trick[5].Set(QT_PARD, QT_PARD, HR(QT_PARD, 0), 
                length[QT_ACE]-1);
              return def.Set1122(trick);
          }
        }
        else if (length[QT_ACE] == 4)
        {
          if (pickFlag) holdCtr[0x11a7]++;
          unsigned r1, r2;
          unsigned pLast = completeList[QT_PARD][length[QT_PARD]-1];
          // The way is comes out, should not be necessary.
          if (pLast > completeList[QT_ACE][3] &&
              pLast > completeList[QT_RHO][2] &&
              (length[QT_LHO] == 0 || pLast > completeList[QT_LHO][0]))
          {
            // r1 = SDS_QUEEN;
            // r2 = HR(QT_PARD, 1);
            r1 = SDS_TEN;
            r2 = HR(QT_PARD, 0);
          }
          else
          {
            r1 = SDS_TEN;
            r2 = HR(QT_PARD, 0);
          }
          rank = r2;
          trick[2].Set(QT_PARD, QT_ACE, r1, 1);
          trick[3].Set(QT_PARD, QT_BOTH, r2, length[QT_PARD]-1);
          return def.Set112(trick);
        }
        else if (completeList[QT_PARD][3] > completeList[QT_ACE][3])
        {
          // The way it comes out, should not be necessary.
          unsigned pLast = completeList[QT_PARD][2];
          if (pLast > completeList[QT_ACE][3] &&
              pLast > completeList[QT_RHO][2] &&
              (length[QT_LHO] == 0 || pLast > completeList[QT_LHO][0] ||
              (length[QT_LHO] == 1 &&
               completeList[QT_PARD][0] > completeList[QT_LHO][0])))
          {
            // Includes Set1123 and Set1124 variants.  Not Set112.
            if (pickFlag) holdCtr[0x11a8]++;
            rank = HR(QT_PARD, 2);
            unsigned r1 = HR(QT_PARD, 0);
            trick[2].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
            trick[3].Set(QT_PARD, QT_BOTH, rank, length[QT_ACE]-1);
            trick[4].Set(QT_PARD, QT_ACE, SDS_TEN, 1);
            trick[5].Set(QT_PARD, QT_PARD, r1, 3);
            trick[6].Set(QT_ACE, QT_ACE, SDS_VOID, length[QT_ACE]-4);
            return def.Set1123(trick);
          }
          else
          {
            if (pickFlag) holdCtr[0x11a9]++;
            rank = HR(QT_PARD, 0);
            // trick[2].Set(QT_PARD, QT_ACE, SDS_TEN, 1);
            // trick[3].Set(QT_PARD, QT_ACE, SDS_VOID, 2);
            // trick[4].Set(QT_BOTH, QT_PARD, rank, 1);
            // trick[5].Set(QT_ACE, QT_ACE, SDS_VOID, length[QT_ACE]-4);
            // return def.Set114(trick);
            trick[2].Set(QT_PARD, QT_ACE, SDS_TEN, 1);
            trick[3].Set(QT_PARD, QT_ACE, rank, 3);
            trick[4].Set(QT_PARD, QT_ACE, SDS_TEN, 1);
            trick[5].Set(QT_PARD, QT_PARD, rank, 3);
            trick[6].Set(QT_ACE, QT_ACE, SDS_VOID, length[QT_ACE]-4);
            return def.Set1123(trick);
          }
        }
        // The way it comes out.
        else if (completeList[QT_PARD][2] <
            completeList[QT_ACE][length[QT_ACE]-1] ||
            completeList[QT_PARD][2] < completeList[QT_RHO][2] ||
            (length[QT_LHO] >= 1 &&
              completeList[QT_LHO][0] > completeList[QT_PARD][2] &&
               (length[QT_LHO] > 1 ||
                 completeList[QT_PARD][0] < completeList[QT_LHO][0])))
        {
          if (pickFlag) holdCtr[0x11aa]++;
          unsigned r = HR(QT_PARD, 0);
          unsigned r1, r2;
          if (htop.N == QT_ACE)
          {
            r1 = HR(QT_ACE, 2);
            r2 = HR(QT_ACE, 3);
          }
          else if (completeList[QT_PARD][1] < 
              completeList[QT_ACE][length[QT_ACE]-1])
          {
            r1 = HR(QT_ACE, 2);
            r2 = HR(QT_ACE, 3);
          }
          else
          {
            // r1 = SDS_QUEEN;
            r1 = HR(QT_ACE, 2);
            r2 = HR(QT_ACE, 3);
          }
          rank = Min(r, r2);
          trick[2].Set(QT_PARD, QT_ACE, r1, 1);
          trick[3].Set(QT_PARD, QT_ACE, r2, length[QT_ACE]-1);
          trick[4].Set(QT_PARD, QT_ACE, SDS_TEN, 1);
          trick[5].Set(QT_PARD, QT_PARD, r, length[QT_PARD]-1);
          trick[6].Set(QT_ACE, QT_ACE, SDS_VOID, length[QT_ACE]-4);
          return def.Set1123(trick);
        }
        else if (completeList[QT_PARD][2] > completeList[QT_ACE][3])
        {
          if (pickFlag) holdCtr[0x11ab]++;
          rank = HR(QT_PARD, 2);
          trick[2].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
          trick[3].Set(QT_PARD, QT_BOTH, rank, length[QT_ACE]-1);
          trick[4].Set(QT_PARD, QT_ACE, SDS_TEN, 1);
          trick[5].Set(QT_PARD, QT_PARD, HR(QT_PARD, 0), 
            length[QT_PARD]-1);
          trick[6].Set(QT_ACE, QT_ACE, SDS_VOID, length[QT_ACE]-4);
          return def.Set1123(trick);
        }
        else if (htop.N == QT_ACE)
        {
          // This is actually a reduction bug.
          // The way it comes out for now.
          if (pickFlag) holdCtr[0x11ab]++;
          PosType e;
          if (completeList[QT_PARD][3] > 
                completeList[QT_ACE][length[QT_ACE]-1] &&
             (length[QT_ACE] == 5 ||
              completeList[QT_ACE][4] > completeList[QT_PARD][0] ||
              completeList[QT_ACE][4] < completeList[QT_PARD][2]) &&
              completeList[QT_PARD][3] > completeList[QT_RHO][2] &&
             (length[QT_LHO] == 0 ||
              completeList[QT_PARD][3] > completeList[QT_LHO][0]))
            e = QT_BOTH;
          else
            e = QT_PARD;

          rank = HR(QT_PARD, 2);
          trick[2].Set(QT_PARD, QT_ACE, SDS_TEN, 1);
          trick[3].Set(QT_PARD, QT_ACE, SDS_NINE, length[QT_ACE]-1);
          trick[4].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
          trick[5].Set(QT_PARD, e, rank, length[QT_ACE]-1);
          return def.Set1122(trick);
        }
        else
        {
          if (pickFlag) holdCtr[0x11ac]++;
          rank = HR(QT_PARD, 2);
          unsigned ra1 = HR(QT_ACE, 3);
          trick[2].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
          trick[3].Set(QT_PARD, QT_ACE, ra1, length[QT_ACE]-1);
          trick[4].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
          trick[5].Set(QT_PARD, QT_PARD, rank, length[QT_ACE]-1);
          trick[6].Set(QT_PARD, QT_ACE, SDS_TEN, 1);
          trick[7].Set(QT_PARD, QT_PARD, HR(QT_PARD, 0), 3);
          trick[8].Set(QT_ACE, QT_ACE, SDS_VOID, length[QT_ACE]-4);
          return def.Set11223(trick);
        }
      }
      else if (length[QT_PARD] == 3)
      {
        if (pickFlag) holdCtr[0x11ad]++;
        rank = SDS_TEN;
        trick[2].Set(QT_PARD, QT_ACE, rank, 1);
        trick[3].Set(QT_PARD, QT_ACE, SDS_VOID, length[QT_ACE]-1);
        return def.Set112(trick);
      }
      else if (completeList[QT_ACE][length[QT_ACE]-1] >
          completeList[QT_PARD][0])
      {
        if (length[QT_PARD] > length[QT_ACE])
        {
          if (pickFlag) holdCtr[0x11ae]++;
          rank = HR(QT_ACE, 3);
          trick[2].Set(QT_PARD, QT_ACE, SDS_TEN, 1);
          trick[3].Set(QT_PARD, QT_ACE, rank, 3);
          trick[4].Set(QT_PARD, QT_PARD, SDS_VOID, length[QT_PARD]-4);
          return def.Set113(trick);
        }
        else
        {
          if (pickFlag) holdCtr[0x11af]++;
          unsigned r;
          if (length[QT_PARD] == length[QT_ACE])
            r = SDS_VOID;
          else
            r = HR(QT_ACE, 3);
          rank = Min(SDS_TEN, r);
          trick[2].Set(QT_PARD, QT_ACE, SDS_TEN, 1);
          trick[3].Set(QT_PARD, QT_ACE, r, length[QT_ACE]-1);
          return def.Set112(trick);
        }
      }
      else if (length[QT_PARD] > length[QT_ACE])
      {
        if (pickFlag) holdCtr[0x11b0]++;
        rank = HR(QT_PARD, 0);
        trick[2].Set(QT_PARD, QT_ACE, SDS_TEN, 1);
        trick[3].Set(QT_PARD, QT_BOTH, rank, length[QT_PARD]-1);
        return def.Set112(trick);
      }
      else if (length[QT_PARD] == length[QT_ACE])
      {
        if (pickFlag) holdCtr[0x11b1]++;
        rank = HR(QT_PARD, 0);
        trick[2].Set(QT_PARD, QT_ACE, SDS_TEN, 1);
        trick[3].Set(QT_PARD, QT_PARD, rank, length[QT_PARD]-1);
        trick[4].Set(QT_PARD, QT_ACE, SDS_TEN, 1);
        trick[5].Set(QT_PARD, QT_ACE, SDS_VOID, length[QT_PARD]-1);
        return def.Set1122(trick);
      }
      else if (completeList[QT_PARD][length[QT_PARD]-1] >
          completeList[QT_ACE][3])
      {
        if (pickFlag) holdCtr[0x11b2]++;
        rank = HR(QT_PARD, 0);
        // trick[2].Set(QT_PARD, QT_ACE, SDS_TEN, 1);
        // trick[3].Set(QT_PARD, QT_ACE, SDS_VOID, 2);
        // trick[4].Set(QT_BOTH, QT_PARD, rank, 1);
        // trick[5].Set(QT_ACE, QT_ACE, SDS_VOID, length[QT_ACE]-4);
        // return def.Set114(trick);
        trick[2].Set(QT_PARD, QT_ACE, SDS_TEN, 1);
        trick[3].Set(QT_PARD, QT_ACE, rank, 3);
        trick[4].Set(QT_PARD, QT_ACE, SDS_TEN, 1);
        trick[5].Set(QT_PARD, QT_PARD, rank, 3);
        trick[6].Set(QT_ACE, QT_ACE, SDS_VOID, length[QT_ACE]-4);
        return def.Set1123(trick);
      }
      else
      {
        if (pickFlag) holdCtr[0x11b3]++;
        unsigned r = HR(QT_ACE, 3);
        unsigned r2 = HR(QT_PARD, 0);
        rank = Min(r, r2);
        trick[2].Set(QT_PARD, QT_ACE, SDS_TEN, 1);
        trick[3].Set(QT_PARD, QT_ACE, r, length[QT_ACE]-1);
        trick[4].Set(QT_PARD, QT_ACE, SDS_TEN, 1);
        trick[5].Set(QT_PARD, QT_PARD, r2, length[QT_PARD]-1);
        trick[6].Set(QT_ACE, QT_ACE, SDS_VOID, 
          length[QT_ACE] - length[QT_PARD]);
        return def.Set1123(trick);
      }
    }
    else if (completeList[QT_PARD][length[QT_PARD]-1] > 
          completeList[QT_ACE][2] &&
        completeList[QT_PARD][length[QT_PARD]-1] > 
          completeList[QT_RHO][2] &&
        (length[QT_LHO] <= 1 ||
        (length[QT_LHO] == 2 && 
          completeList[QT_PARD][1] > completeList[QT_LHO][0]) ||
        (length[QT_LHO] >= 3 &&
          completeList[QT_PARD][2] > completeList[QT_LHO][0])))
    {
      if (length[QT_ACE] <= length[QT_PARD])
      {
        if (pickFlag) holdCtr[0x11b4]++;
        if (length[QT_ACE] == 3 || length[QT_ACE] == length[QT_PARD])
          rank = HR(QT_PARD, 2);
        else
          rank = HR(QT_PARD, 3);
        trick[2].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
        trick[3].Set(QT_PARD, QT_BOTH, rank, length[QT_PARD]-1);
        return def.Set112(trick);
      }
      else if (length[QT_PARD] == 3)
      {
        if (pickFlag) holdCtr[0x11b5]++;
        rank = HR(QT_PARD, 2);
        // trick[2].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
        // trick[3].Set(QT_PARD, QT_ACE, SDS_VOID, 1);
        // trick[4].Set(QT_BOTH, QT_PARD, rank, 1);
        // trick[5].Set(QT_ACE, QT_ACE, SDS_VOID, length[QT_ACE]-3);
        // return def.Set114(trick);
        trick[2].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
        trick[3].Set(QT_PARD, QT_ACE, rank, 2);
        trick[4].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
        trick[5].Set(QT_PARD, QT_PARD, rank, 2);
        trick[6].Set(QT_ACE, QT_ACE, SDS_VOID, length[QT_ACE]-3);
        return def.Set1123(trick);
      }
      else
      {
        if (pickFlag) holdCtr[0x11b6]++;
        rank = HR(QT_PARD, length[QT_PARD]-1);
        // trick[2].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
        // trick[3].Set(QT_PARD, QT_ACE, SDS_VOID, 1);
        // trick[4].Set(QT_BOTH, QT_PARD, rank, 2);
        // trick[5].Set(QT_ACE, QT_ACE, SDS_VOID, length[QT_ACE]-4);
        // return def.Set114(trick);
        trick[2].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
        trick[3].Set(QT_PARD, QT_ACE, rank, 3);
        trick[4].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
        trick[5].Set(QT_PARD, QT_PARD, rank, 3);
        trick[6].Set(QT_ACE, QT_ACE, SDS_VOID, length[QT_ACE]-4);
        return def.Set1123(trick);
      }
    }
    else if (completeList[QT_PARD][1] > completeList[QT_RHO][2] &&
        (length[QT_LHO] == 0 ||
          completeList[QT_PARD][1] > completeList[QT_LHO][0]))
    {
      unsigned a2 = completeList[QT_ACE][2];
      unsigned p1 = completeList[QT_PARD][1];
      unsigned p2 = completeList[QT_PARD][2];
      unsigned r2 = completeList[QT_RHO][2];
      unsigned p3 = completeList[QT_PARD][3];
      unsigned l0 = (length[QT_LHO] == 0 ? 0 : completeList[QT_LHO][0]);
      unsigned l0p = (length[QT_LHO] <= 1 ? 0 : completeList[QT_LHO][0]);
      unsigned aLast = completeList[QT_ACE][length[QT_ACE]-1];
      unsigned pLast = completeList[QT_PARD][length[QT_PARD]-1];

      if (length[QT_ACE] == length[QT_PARD])
      {
        // The way it comes out.
        if ((p2 > a2 && p2 > r2 && p2 > l0p) ||
            (p1 > a2 && p1 > r2 && p1 > l0p && a2 > r2 && a2 > l0p))
        {
          if (pickFlag) holdCtr[0x11b7]++;
          rank = Holding::ListToRank(Max(p2, a2));
          trick[2].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
          trick[3].Set(QT_PARD, QT_BOTH, rank, length[QT_ACE]-1);
          return def.Set112(trick);
        }
        else if (distHex == 0x4243 && 
          htop.T == QT_PARD && htop.N == QT_PARD && htop.E == QT_LHO)
        {
          if (aLast > p2)
          {
            if (pickFlag) holdCtr[0x11b8]++;
            rank = HR(QT_ACE, 2);
            trick[2].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
            trick[3].Set(QT_PARD, QT_ACE, rank, length[QT_ACE]-1);
            return def.Set112(trick);
          }
          else if (p2 > a2)
          {
            if (pickFlag) holdCtr[0x11b9]++;
            rank = HR(QT_PARD, 2);
            trick[2].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
            trick[3].Set(QT_PARD, QT_BOTH, rank, length[QT_ACE]-1);
            return def.Set112(trick);
          }
          else
          {
            if (pickFlag) holdCtr[0x11ba]++;
            rank = Holding::ListToRank(Min(p2, a2));
            trick[2].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
            trick[3].Set(QT_PARD, QT_ACE, HR(QT_ACE, 2), 3);
            trick[4].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
            trick[5].Set(QT_PARD, QT_PARD, HR(QT_PARD, 2), 3);
            return def.Set1122(trick);
          }
        }
        else if (pLast > aLast && pLast > r2 && pLast > l0)
        {
          // Should this be necessary?  Probably yes.
          if (pickFlag) holdCtr[0x11bb]++;
          rank = HR(QT_PARD, 1);
          trick[2].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
          trick[3].Set(QT_PARD, QT_ACE, HR(QT_ACE, 2), length[QT_ACE]-1);
          trick[4].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
          trick[5].Set(QT_PARD, QT_PARD, rank, length[QT_ACE]-1);
          return def.Set1122(trick);
        }
        else if (p1 < a2)
        {
          if (aLast > p1)
          {
            if (pickFlag) holdCtr[0x11bc]++;
            rank = HR(QT_ACE, 2);
            trick[2].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
            trick[3].Set(QT_PARD, QT_ACE, rank, length[QT_ACE]-1);
            return def.Set112(trick);
          }
          else
          {
            if (pickFlag) holdCtr[0x11bc]++;
            rank = HR(QT_PARD, 1);
            trick[2].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
            trick[3].Set(QT_PARD, QT_ACE, HR(QT_ACE, 2), length[QT_ACE]-1);
            trick[4].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
            trick[5].Set(QT_PARD, QT_PARD, rank, length[QT_ACE]-1);
            return def.Set1122(trick);
          }
        }
        else
        {
          if (pickFlag) holdCtr[0x11bc]++;
          rank = HR(QT_ACE, 2);
          trick[2].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
          trick[3].Set(QT_PARD, QT_BOTH, rank, length[QT_ACE]-1);
          return def.Set112(trick);
        }
      }
      else if (length[QT_PARD] == 4)
      {
        if (p2 > completeList[QT_ACE][3] && 
            p2 > r2 && p2 > l0p && pLast < a2)
        {
          if (p2 > a2)
          {
            if (pickFlag) holdCtr[0x11bd]++;
            unsigned ra1 = HR(QT_ACE, 2);
            unsigned ra2 = HR(QT_PARD, 2);
            rank = Min(ra1, ra2);
            trick[2].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
            trick[3].Set(QT_PARD, QT_BOTH, rank, length[QT_ACE]-1);
            return def.Set112(trick);
          }
          else
          {
            if (pickFlag) holdCtr[0x11bd]++;
            unsigned ra1 = HR(QT_ACE, 2);
            unsigned ra2 = HR(QT_PARD, 2);
            unsigned ra3, ra4;
            if (p1 > a2)
            {
              ra3 = SDS_QUEEN;
              ra4 = ra1;
            }
            else
            {
              ra3 = ra1;
              ra4 = SDS_VOID;
            }
            rank = Min(ra1, ra2);
            trick[2].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
            trick[3].Set(QT_PARD, QT_BOTH, rank, length[QT_ACE]-1);
            trick[4].Set(QT_PARD, QT_ACE, ra3, 1);
            trick[5].Set(QT_PARD, QT_PARD, ra4, 3);
            trick[6].Set(QT_ACE, QT_ACE, SDS_VOID, length[QT_ACE]-4);
            return def.Set1123(trick);
          }
        }
        else if (pLast > a2)
        {
          if (pickFlag) holdCtr[0x11be]++;
          rank = HR(QT_PARD, 3);
          // trick[2].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
          // trick[3].Set(QT_PARD, QT_ACE, SDS_VOID, 1);
          // trick[4].Set(QT_BOTH, QT_PARD, rank, 2);
          // trick[5].Set(QT_ACE, QT_ACE, SDS_VOID, length[QT_ACE]-4);
          // return def.Set114(trick);
          trick[2].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
          trick[3].Set(QT_PARD, QT_ACE, rank, 3);
          trick[4].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
          trick[5].Set(QT_PARD, QT_PARD, rank, 3);
          trick[6].Set(QT_ACE, QT_ACE, SDS_VOID, length[QT_ACE]-4);
          return def.Set1123(trick);
        }
        else if (p2 > r2 && p2 > aLast)
        {
          if (a2 > p1)
          {
            if (pickFlag) holdCtr[0x11bf]++;
            unsigned ra1 = HR(QT_ACE, 3);
            unsigned ra2 = HR(QT_PARD, 2);
            rank = Min(ra1, ra2);
            trick[2].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
            trick[3].Set(QT_PARD, QT_ACE, ra1, length[QT_ACE]-1);
            trick[4].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
            trick[5].Set(QT_PARD, QT_PARD, ra2, length[QT_ACE]-1);
            trick[6].Set(QT_PARD, QT_ACE, HR(QT_ACE, 2), 1);
            trick[7].Set(QT_PARD, QT_PARD, SDS_VOID, 3);
            trick[8].Set(QT_ACE, QT_ACE, SDS_VOID, length[QT_ACE]-4);
            return def.Set11223(trick);
          }
          else
          {
            if (pickFlag) holdCtr[0x11bf]++;
            unsigned ra1 = HR(QT_ACE, 3);
            unsigned ra2 = HR(QT_PARD, 2);
            rank = Min(ra1, ra2);
            trick[2].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
            trick[3].Set(QT_PARD, QT_ACE, ra1, length[QT_ACE]-1);
            trick[4].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
            trick[5].Set(QT_PARD, QT_PARD, ra2, length[QT_ACE]-1);
            trick[6].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
            trick[7].Set(QT_PARD, QT_PARD, HR(QT_ACE, 2), 3);
            trick[8].Set(QT_ACE, QT_ACE, SDS_VOID, length[QT_ACE]-4);
            return def.Set11223(trick);
          }
        }
        else if (pLast > r2 && pLast < aLast)
        {
          if (htop.N == QT_ACE)
          {
            if (pickFlag) holdCtr[0x11c0]++;
            unsigned ra0 = HR(QT_ACE, 2);
            unsigned ra1 = HR(QT_ACE, 3);
            unsigned ra2 = HR(QT_ACE, 2);
            rank = ra1;
            trick[2].Set(QT_PARD, QT_ACE, ra0, 1);
            trick[3].Set(QT_PARD, QT_ACE, ra1, length[QT_ACE]-1);
            trick[4].Set(QT_PARD, QT_ACE, ra2, 1);
            trick[5].Set(QT_PARD, QT_PARD, SDS_VOID, 3);
            trick[6].Set(QT_ACE, QT_ACE, SDS_VOID, length[QT_ACE]-4);
            return def.Set1123(trick);
          }
          else
          {
            if (pickFlag) holdCtr[0x11c1]++;
            unsigned ra1 = HR(QT_ACE, 3);
            unsigned ra2 = HR(QT_ACE, 2);
            rank = ra1;
            trick[2].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
            trick[3].Set(QT_PARD, QT_ACE, ra1, length[QT_ACE]-1);
            trick[4].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
            trick[5].Set(QT_PARD, QT_PARD, ra2, 3);
            trick[6].Set(QT_ACE, QT_ACE, SDS_VOID, length[QT_ACE]-4);
            return def.Set1123(trick);
          }
        }
        else if (htop.N == QT_ACE)
        {
          if (aLast > p1)
          {
            if (pickFlag) holdCtr[0x11c2]++;
            unsigned ra1 = HR(QT_ACE, 3);
            unsigned ra2 = HR(QT_ACE, 2);
            rank = ra1;
            trick[2].Set(QT_PARD, QT_ACE, ra2, 1);
            trick[3].Set(QT_PARD, QT_ACE, ra1, length[QT_ACE]-1);
            trick[4].Set(QT_PARD, QT_ACE, ra2, 1);
            trick[5].Set(QT_PARD, QT_PARD, SDS_VOID, 3);
            trick[6].Set(QT_ACE, QT_ACE, SDS_VOID, length[QT_ACE]-4);
            return def.Set1123(trick);
          }
          else if (pLast > completeList[QT_ACE][3])
          {
            if (pickFlag) holdCtr[0x11c3]++;
            rank = HR(QT_ACE, 2);
            trick[2].Set(QT_PARD, QT_ACE, rank, 1);
            trick[3].Set(QT_PARD, QT_ACE, SDS_VOID, 3);
            trick[4].Set(QT_PARD, QT_ACE, rank, 1);
            trick[5].Set(QT_PARD, QT_PARD, SDS_VOID, 3);
            trick[6].Set(QT_ACE, QT_ACE, SDS_VOID, length[QT_ACE]-4);
            return def.Set1123(trick);
          }
          else 
          {
            if (pickFlag) holdCtr[0x11c4]++;
            unsigned ra1 = HR(QT_ACE, 3);
            unsigned ra2 = HR(QT_ACE, 2);
            rank = ra1;
            // trick[2].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
            trick[2].Set(QT_PARD, QT_ACE, ra2, 1);
            trick[3].Set(QT_PARD, QT_ACE, ra1, length[QT_ACE]-1);
            trick[4].Set(QT_PARD, QT_ACE, ra2, 1);
            trick[5].Set(QT_PARD, QT_PARD, SDS_VOID, 3);
            trick[6].Set(QT_ACE, QT_ACE, SDS_VOID, length[QT_ACE]-4);
            return def.Set1123(trick);
          }
        }
        /*
        else if (distHex == 0x5143 && counter == 0xce93a0)
        {
          // AQ732 / 8 / T954 / KJ6...
          if (pickFlag) holdCtr[0x11c5]++;
          // rank = HR(QT_ACE, 2);
          // trick[2].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
          // trick[3].Set(QT_PARD, QT_ACE, rank, 2);
          // trick[4].Set(QT_BOTH, QT_PARD, SDS_VOID, 1);
          // trick[5].Set(QT_ACE, QT_ACE, SDS_VOID, length[QT_ACE]-4);
          // return def.Set114(trick);
          unsigned ra2 = HR(QT_ACE, 2);
          rank = ra2;
          trick[2].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
          trick[3].Set(QT_PARD, QT_ACE, ra2, 3);
          trick[4].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
          trick[5].Set(QT_PARD, QT_PARD, ra2, 3);
          trick[6].Set(QT_ACE, QT_ACE, SDS_VOID, length[QT_ACE]-4);
          return def.Set1123(trick);
        }
        */
        else if (pLast > completeList[QT_ACE][3])
        {
          if (pickFlag) holdCtr[0x11c6]++;
          // rank = HR(QT_ACE, 2);
          // trick[2].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
          // trick[3].Set(QT_PARD, QT_ACE, rank, 2);
          // trick[4].Set(QT_BOTH, QT_PARD, SDS_VOID, 1);
          // trick[5].Set(QT_ACE, QT_ACE, SDS_VOID, length[QT_ACE]-4);
          // return def.Set114(trick);
          unsigned ra2 = HR(QT_ACE, 2);
          rank = ra2;
          trick[2].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
          trick[3].Set(QT_PARD, QT_ACE, ra2, 3);
          trick[4].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
          trick[5].Set(QT_PARD, QT_PARD, ra2, 3);
          trick[6].Set(QT_ACE, QT_ACE, SDS_VOID, length[QT_ACE]-4);
          return def.Set1123(trick);
        }
        else
        {
          if (pickFlag) holdCtr[0x11c7]++;
          unsigned ra1 = HR(QT_ACE, 3);
          unsigned ra2 = HR(QT_ACE, 2);
          rank = ra1;
          trick[2].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
          trick[3].Set(QT_PARD, QT_ACE, ra1, length[QT_ACE]-1);
          trick[4].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
          trick[5].Set(QT_PARD, QT_PARD, ra2, 3);
          trick[6].Set(QT_ACE, QT_ACE, SDS_VOID, length[QT_ACE]-4);
          return def.Set1123(trick);
        }
      }
      else if (length[QT_PARD] > 4)
      {
        if (htop.N == QT_ACE && 
           (pLast < aLast || pLast < r2 || pLast < l0))
        {
          if (pickFlag) holdCtr[0x11c8]++;
          rank = HR(QT_ACE, 2);
          trick[2].Set(QT_PARD, QT_ACE, rank, 1);
          trick[3].Set(QT_PARD, QT_BOTH, SDS_VOID, length[QT_PARD]-1);
          return def.Set112(trick);
        }
        else if (aLast > p3 && r2 > a2)
        {
          if (pickFlag) holdCtr[0x11c9]++;
          unsigned ra1 = HR(QT_ACE, 2);
          unsigned ra2 = HR(QT_PARD, 2);
          rank = Min(ra1, ra2);
          // trick[2].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
          // trick[3].Set(QT_PARD, QT_PARD, ra2, 2);
          // trick[4].Set(QT_BOTH, QT_ACE, ra1, 1);
          // trick[5].Set(QT_PARD, QT_PARD, SDS_VOID, length[QT_PARD]-4);
          // return def.Set114(trick);
          trick[2].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
          trick[3].Set(QT_PARD, QT_PARD, ra1, 3);
          trick[4].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
          trick[5].Set(QT_PARD, QT_ACE, ra1, 3);
          trick[6].Set(QT_PARD, QT_PARD, SDS_VOID, length[QT_PARD]-4);
          return def.Set1123(trick);
        }
        else if (a2 > p3 && a2 < r2)
        {
          // AQ64 / - / T9853 / KJ7.
          if (pickFlag) holdCtr[0x11ca]++;
          unsigned ra1 = HR(QT_PARD, 2);
          unsigned ra2 = HR(QT_ACE, 2);
          rank = HR(QT_PARD, 3);

          trick[2].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
          trick[3].Set(QT_PARD, QT_BOTH, rank, length[QT_PARD]-1);
          trick[4].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
          trick[5].Set(QT_PARD, QT_ACE, SDS_VOID, 1);
          trick[6].Set(QT_PARD, QT_ACE, ra2, 2);
          trick[7].Set(QT_PARD, QT_PARD, SDS_VOID, length[QT_PARD]-4);
          return def.Set1124(trick);
        }
        else
        {
          if (pickFlag) holdCtr[0x11ca]++;
          if (p3 > a2 || r2 > Max(a2, p3))
          {
            rank = HR(QT_PARD, 3);
            trick[2].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
            trick[3].Set(QT_PARD, QT_BOTH, rank, length[QT_PARD]-1);
            return def.Set112(trick);
          }
          else if (r2 > p3)
          {
            rank = HR(QT_ACE, 2);
            trick[2].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
            trick[3].Set(QT_PARD, QT_BOTH, rank, length[QT_PARD]-1);
            return def.Set112(trick);
          }
          else if (htop.N == QT_PARD)
          {
            // The way it comes out, PROBLEM.
            rank = HR(QT_ACE, 2);
            trick[2].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
            trick[3].Set(QT_PARD, QT_BOTH, rank, length[QT_PARD]-1);
            return def.Set112(trick);
          }
          else
          {
            // PROBLEM: Could also be Q, rank.
            // Need to look more carefully here.
            // AQ93 / 2 / T8765 / KJ4.
            // The early special 5 lead comes out this way.
            // It probably then is "the same" as the later rank, void.
            // 13, 0xce2ab1.
            // unsigned ra1 = HR(QT_ACE, 2);
            // unsigned ra2 = HR(QT_PARD, 1);
            // rank = Min(ra1, ra2);
            rank = HR(QT_ACE, 2);
            trick[2].Set(QT_PARD, QT_ACE, rank, 1);
            trick[3].Set(QT_PARD, QT_BOTH, SDS_VOID, length[QT_PARD]-1);
            return def.Set112(trick);
          }

        }
      }
      else if (completeList[QT_PARD][2] > 
          completeList[QT_ACE][length[QT_ACE]-1] &&
          completeList[QT_PARD][2] > completeList[QT_RHO][2] &&
          (length[QT_LHO] <= 2 ||
            completeList[QT_PARD][2] > completeList[QT_LHO][0]))
      {
        // PROBLEM (rank ra2)?
        if (pickFlag) holdCtr[0x11cb]++;
        unsigned ra1 = HR(QT_ACE, 2);
        // unsigned ra2 = (p2 > l0p ? HR(QT_PARD, 1) : HR(QT_PARD, 2));
        unsigned ra2 = HR(QT_PARD, 2);
        rank = Min(ra1, ra2);
        trick[2].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
        trick[3].Set(QT_PARD, QT_ACE, ra1, length[QT_ACE]-1);
        trick[4].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
        trick[5].Set(QT_PARD, QT_PARD, ra2, 2);
        trick[6].Set(QT_ACE, QT_ACE, SDS_VOID, length[QT_ACE]-3);
        return def.Set1123(trick);
      }
      else
      {
        if (pickFlag) holdCtr[0x11cc]++;
        rank = HR(QT_ACE, 2);
        trick[2].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
        trick[3].Set(QT_PARD, QT_ACE, rank, length[QT_ACE]-1);
        return def.Set112(trick);
      }
    }
    else if (length[QT_PARD] == 3)
    {
      if (completeList[QT_RHO][2] > completeList[QT_PARD][2] ||
          completeList[QT_PARD][2] < 
            completeList[QT_ACE][length[QT_ACE]-1] ||
          (length[QT_LHO] >= 2 &&
          completeList[QT_LHO][0] > completeList[QT_PARD][1]))
      {
        if (pickFlag) holdCtr[0x11cd]++;
        rank = HR(QT_ACE, 2);
        trick[2].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
        trick[3].Set(QT_PARD, QT_ACE, rank, length[QT_ACE]-1);
        return def.Set112(trick);
      }
      else
      {
        if (pickFlag) holdCtr[0x11ce]++;
        unsigned ra1 = HR(QT_ACE, 2);
        unsigned ra2;
        // if (completeList[QT_ACE][2] > completeList[QT_PARD][1])
          ra2 = HR(QT_PARD, 2);
        // else
          // ra2 = HR(QT_PARD, 1);
        rank = Min(ra1, ra2);
        trick[2].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
        trick[3].Set(QT_PARD, QT_ACE, ra1, length[QT_ACE]-1);
        trick[4].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
        trick[5].Set(QT_PARD, QT_PARD, ra2, 2);
        trick[6].Set(QT_ACE, QT_ACE, SDS_VOID, length[QT_ACE]-3);
        return def.Set1123(trick);
      }
    }
    else if (length[QT_ACE] > length[QT_PARD])
    {
      unsigned a2 = completeList[QT_ACE][2];
      unsigned a3 = completeList[QT_ACE][3];
      unsigned p0 = completeList[QT_PARD][0];
      unsigned p1 = completeList[QT_PARD][1];
      unsigned p2 = completeList[QT_PARD][2];
      unsigned p3 = completeList[QT_PARD][3];
      unsigned r2 = completeList[QT_RHO][2];
      unsigned aLast = completeList[QT_ACE][length[QT_ACE]-1];

      // if (p2 > a3 && p2 > r2 && a3 > p3)
      if (p2 > a3 && p2 > r2 && a2 > p3)
      {
        if (((htop.N == QT_ACE && htop.E == QT_LHO) ||
             (htop.N == QT_LHO && htop.E == QT_PARD)) && 
            p2 > a2)
        {
          if (pickFlag) holdCtr[0x11cf]++;
          unsigned ra1 = HR(QT_ACE, 2);
          unsigned ra2 = HR(QT_PARD, 2);
          rank = Min(ra1, ra2);
          trick[2].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
          trick[3].Set(QT_PARD, QT_BOTH, rank, length[QT_ACE]-1);
          return def.Set112(trick);
        }
        else if (a2 > p2 && (a3 > p3 || htop.N == QT_LHO))
        {
          // Surely not right.  The way it comes out.
          if (pickFlag) holdCtr[0x11cf]++;
          unsigned ra1 = HR(QT_ACE, 2);
          unsigned ra2 = HR(QT_PARD, 2);
          rank = Min(ra1, ra2);
          unsigned ra3, ra4;
          if (htop.N == QT_ACE)
          {
            ra3 = ra1;
            ra4 = SDS_VOID;
          }
          else
          {
            ra3 = SDS_QUEEN;
            unsigned ra5 = HR(QT_PARD, 1);
            ra4 = Min(ra1, ra5);
          }
          trick[2].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
          trick[3].Set(QT_PARD, QT_BOTH, rank, length[QT_ACE]-1);
          trick[4].Set(QT_PARD, QT_ACE, ra3, 1);
          trick[5].Set(QT_PARD, QT_PARD, ra4, 3);
          trick[6].Set(QT_ACE, QT_ACE, SDS_VOID, 1);
          return def.Set1123(trick);
        }
        else
        {
          if (pickFlag) holdCtr[0x11cf]++;
          unsigned ra1 = HR(QT_ACE, 2);
          unsigned ra2 = HR(QT_PARD, 2);
          rank = Min(ra1, ra2);
          trick[2].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
          trick[3].Set(QT_PARD, QT_BOTH, rank, length[QT_ACE]-1);
          trick[4].Set(QT_PARD, QT_ACE, HR(QT_ACE, 2), 1);
          trick[5].Set(QT_PARD, QT_PARD, SDS_VOID, 3);
          trick[6].Set(QT_ACE, QT_ACE, SDS_VOID, 1);
          return def.Set1123(trick);
        }
      }
      else if (aLast > p1 && // && r2 < p1 && r2 > p2)
          htop.N != QT_ACE &&
          (r2 > p1 || (length[QT_LHO] == 1 &&
            completeList[QT_LHO][0] > p1)))
          // r2 < p1 &&
          // (length[QT_LHO] == 0 || completeList[QT_LHO][0] < p1))
          // (length[QT_LHO] == 0 || completeList[QT_LHO][0] < p1))
      {
        if (pickFlag) holdCtr[0x11d0]++;
        rank = HR(QT_ACE, 3);
        trick[2].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
        trick[3].Set(QT_PARD, QT_ACE, rank, length[QT_ACE]-1);
        return def.Set112(trick);
      }
      else if (p3 > a2)
      {
        if (pickFlag) holdCtr[0x11d1]++;
        rank = HR(QT_PARD, 3);
        // trick[2].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
        // trick[3].Set(QT_PARD, QT_ACE, SDS_VOID, 1);
        // trick[4].Set(QT_BOTH, QT_PARD, rank, 2);
        // trick[5].Set(QT_ACE, QT_ACE, SDS_VOID, length[QT_ACE]-4);
        // return def.Set114(trick);
        trick[2].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
        trick[3].Set(QT_PARD, QT_ACE, rank, 3);
        trick[4].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
        trick[5].Set(QT_PARD, QT_PARD, rank, 3);
        trick[6].Set(QT_ACE, QT_ACE, SDS_VOID, length[QT_ACE]-4);
        return def.Set1123(trick);
      }
      else if (p3 > a3)
      {
        if (pickFlag) holdCtr[0x11d2]++;
        unsigned ra0, ra1, ra2;
        if (htop.N == QT_ACE)
        {
          ra0 = SDS_NINE;
          ra1 = SDS_VOID;
          ra2 = SDS_VOID;
          rank = ra0;
          trick[2].Set(QT_PARD, QT_ACE, ra0, 1);
          trick[3].Set(QT_PARD, QT_ACE, SDS_VOID, 3);
          trick[4].Set(QT_PARD, QT_ACE, ra0, 1);
          trick[5].Set(QT_PARD, QT_PARD, ra2, 3);
          trick[6].Set(QT_ACE, QT_ACE, SDS_VOID, length[QT_ACE]-4);
          return def.Set1123(trick);
        }
        else if (a2 > p1)
        {
          ra0 = SDS_QUEEN;
          ra1 = HR(QT_ACE, 2);
          ra2 = HR(QT_PARD, 1);
          rank = Min(ra1, ra2);
          trick[2].Set(QT_PARD, QT_ACE, ra0, 1);
          trick[3].Set(QT_PARD, QT_ACE, ra2, 3);
          trick[4].Set(QT_PARD, QT_ACE, ra0, 1);
          trick[5].Set(QT_PARD, QT_PARD, ra2, 3);
          trick[6].Set(QT_ACE, QT_ACE, SDS_VOID, length[QT_ACE]-4);
          return def.Set1123(trick);
        }
        else
        {
          ra0 = SDS_QUEEN;
          ra1 = HR(QT_ACE, 2);
          ra2 = SDS_VOID;
          rank = ra1;
          trick[2].Set(QT_PARD, QT_ACE, ra0, 1);
          trick[3].Set(QT_PARD, QT_ACE, ra1, 3);
          trick[4].Set(QT_PARD, QT_ACE, ra0, 1);
          trick[5].Set(QT_PARD, QT_PARD, ra1, 3);
          trick[6].Set(QT_ACE, QT_ACE, SDS_VOID, length[QT_ACE]-4);
          return def.Set1123(trick);
        }
        // trick[2].Set(QT_PARD, QT_ACE, ra0, 1);
        // trick[3].Set(QT_PARD, QT_ACE, ra1, 2);
        // trick[4].Set(QT_BOTH, QT_PARD, ra2, 1);
        // trick[5].Set(QT_ACE, QT_ACE, SDS_VOID, length[QT_ACE]-4);
        // return def.Set114(trick);
      }
      else if (p2 > r2 && p2 > aLast && p2 < a3)
      {
        if (a3 > p1 && a3 > r2 && 
           length[QT_LHO] == 1 &&
           completeList[QT_LHO][0] > a2)
        {
          if (pickFlag) holdCtr[0x11d3]++;
          unsigned ra1 = HR(QT_ACE, 3);
          rank = HR(QT_PARD, 2);
          trick[2].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
          trick[3].Set(QT_PARD, QT_ACE, ra1, length[QT_ACE]-1);
          trick[4].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
          trick[5].Set(QT_PARD, QT_PARD, rank, length[QT_ACE]-1);
          return def.Set1122(trick);
        }
        else if (p1 > a2 || 
            (p1 > a3 && p1 > r2 && length[QT_LHO] == 1 &&
            completeList[QT_LHO][0] > a2))
        {
          if (pickFlag) holdCtr[0x11d3]++;
          unsigned ra1 = HR(QT_ACE, 3);
          rank = HR(QT_PARD, 2);
          unsigned ra2 = HR(QT_ACE, 2);
          unsigned ra3 = HR(QT_PARD, 1);
          unsigned ra4 = Min(ra2, ra3);
          trick[2].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
          trick[3].Set(QT_PARD, QT_ACE, ra1, length[QT_ACE]-1);
          trick[4].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
          trick[5].Set(QT_PARD, QT_PARD, rank, length[QT_ACE]-1);
          trick[6].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
          trick[7].Set(QT_PARD, QT_PARD, ra4, 3);
          trick[8].Set(QT_ACE, QT_ACE, SDS_VOID, length[QT_ACE]-4);
          return def.Set11223(trick);
        }
        else
        {
          if (pickFlag) holdCtr[0x11d3]++;
          unsigned ra1 = HR(QT_ACE, 3);
          rank = HR(QT_PARD, 2);
          trick[2].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
          trick[3].Set(QT_PARD, QT_ACE, ra1, length[QT_ACE]-1);
          trick[4].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
          trick[5].Set(QT_PARD, QT_PARD, rank, length[QT_ACE]-1);
          trick[6].Set(QT_PARD, QT_ACE, HR(QT_ACE, 2), 1);
          trick[7].Set(QT_PARD, QT_PARD, SDS_VOID, 3);
          trick[8].Set(QT_ACE, QT_ACE, SDS_VOID, length[QT_ACE]-4);
          return def.Set11223(trick);
        }
      }
      else if (htop.N == QT_ACE)
      {
        if (pickFlag) holdCtr[0x11d4]++;
        rank = HR(QT_ACE, 3);
        // unsigned ra0 = (aLast > p1 ? HR(QT_ACE, 2) : SDS_QUEEN);
        unsigned ra0 = HR(QT_ACE, 2);
        unsigned ra1 = HR(QT_ACE, 3);
        unsigned ra2 = HR(QT_ACE, 2);
        trick[2].Set(QT_PARD, QT_ACE, ra0, 1);
        trick[3].Set(QT_PARD, QT_ACE, ra1, length[QT_ACE]-1);
        trick[4].Set(QT_PARD, QT_ACE, ra2, 1);
        trick[5].Set(QT_PARD, QT_PARD, SDS_VOID, 3);
        trick[6].Set(QT_ACE, QT_ACE, SDS_VOID, length[QT_ACE]-4);
        return def.Set1123(trick);
      }
      else
      {
        if (pickFlag) holdCtr[0x11d5]++;
        unsigned ra1 = HR(QT_ACE, 3);
        unsigned ra2 = HR(QT_PARD, 1);
        unsigned ra3 = HR(QT_ACE, 2);
        unsigned ra4 = Min(ra2, ra3);
        rank = Min(ra1, ra4);
        trick[2].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
        trick[3].Set(QT_PARD, QT_ACE, ra1, length[QT_ACE]-1);
        trick[4].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
        trick[5].Set(QT_PARD, QT_PARD, ra4, 3);
        trick[6].Set(QT_ACE, QT_ACE, SDS_VOID, length[QT_ACE]-4);
        return def.Set1123(trick);
      }
    }
    else if (completeList[QT_ACE][2] > completeList[QT_RHO][2] &&
        completeList[QT_ACE][2] > completeList[QT_PARD][length[QT_PARD]-1])
    {
      unsigned l = Max(length[QT_ACE], length[QT_PARD]);
      if (completeList[QT_PARD][1] > 
          completeList[QT_ACE][length[QT_ACE]-1])
      {
        if (htop.N == QT_LHO && htop.E == QT_ACE &&
            length[QT_ACE] == length[QT_PARD] &&
            completeList[QT_PARD][1] > completeList[QT_ACE][3] &&
            completeList[QT_PARD][1] > completeList[QT_RHO][2])
        {
          // AQ8x / 9 / T7xx / KJ3.
          if (pickFlag) holdCtr[0x11d6]++;
          unsigned r1 = HR(QT_ACE, 2);
          unsigned r2 = HR(QT_PARD, 1);
          rank = r2;
          trick[2].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
          trick[3].Set(QT_PARD, QT_ACE, r1, l-1);
          trick[4].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
          trick[5].Set(QT_PARD, QT_PARD, r2, l-1);
          return def.Set1122(trick);
        }
        else if (length[QT_LHO] == 0 || 
          completeList[QT_LHO][0] < completeList[QT_ACE][2])
        {
          if (length[QT_ACE] == length[QT_PARD])
          {
            if (pickFlag) holdCtr[0x11d7]++;
            rank = HR(QT_PARD, 1);
            trick[2].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
            trick[3].Set(QT_PARD, QT_ACE, SDS_NINE, l-1);
            trick[4].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
            trick[5].Set(QT_PARD, QT_PARD, rank, l-1);
            return def.Set1122(trick);
          }
          else
          {
            if (pickFlag) holdCtr[0x11d7]++;
            if (htop.N == QT_LHO &&
               (htop.E == QT_ACE ||
               (length[QT_ACE] == length[QT_PARD] &&
               completeList[QT_PARD][2] > completeList[QT_ACE][2])))
              rank = HR(QT_PARD, 2);
            else
              rank = HR(QT_ACE, 2);

            trick[2].Set(QT_PARD, QT_ACE, rank, 1);
            trick[3].Set(QT_PARD, QT_BOTH, SDS_VOID, l-1);
            return def.Set112(trick);
          }
        }
        else if (length[QT_PARD] > length[QT_ACE] ||
            (length[QT_PARD] == length[QT_ACE] &&
            completeList[QT_RHO][2] < completeList[QT_PARD][1]))
        {
          if (pickFlag) holdCtr[0x11d8]++;

          if (length[QT_ACE] == length[QT_PARD] &&
              completeList[QT_PARD][2] > completeList[QT_ACE][2] &&
              completeList[QT_PARD][2] > completeList[QT_RHO][2])
          {
            rank = HR(QT_PARD, 2);
          }
          else if (length[QT_PARD] > length[QT_ACE] &&
              completeList[QT_PARD][3] > completeList[QT_ACE][2])
          {
            rank = HR(QT_PARD, 3);
          }
          else if (length[QT_PARD] > length[QT_ACE] &&
              completeList[QT_ACE][2] > completeList[QT_PARD][1])
          {
            rank = HR(QT_PARD, 1);
          }
          else
            rank = HR(QT_ACE, 2);

          trick[2].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
          trick[3].Set(QT_PARD, QT_BOTH, rank, length[QT_PARD]-1);
          return def.Set112(trick);
        }
        else
        {
          if (pickFlag) holdCtr[0x11d9]++;
          unsigned ra1 = HR(QT_ACE, 2);
          unsigned ra2 = HR(QT_PARD, 1);
          rank = ra2;
          trick[2].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
          trick[3].Set(QT_PARD, QT_PARD, ra2, 3);
          trick[4].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
          trick[5].Set(QT_PARD, QT_ACE, ra1, 3);
          return def.Set1122(trick);
        }
      }
      else if (length[QT_ACE] < length[QT_PARD] &&
          length[QT_LHO] >= 1 &&
          completeList[QT_LHO][0] > completeList[QT_ACE][2])
      {
        if (pickFlag) holdCtr[0x11da]++;
        rank = HR(QT_ACE, length[QT_ACE]-1);
        trick[2].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
        trick[3].Set(QT_PARD, QT_ACE, rank, length[QT_ACE]-1);
        trick[4].Set(QT_PARD, QT_PARD, SDS_VOID,
          length[QT_PARD] - length[QT_ACE]);
        return def.Set113(trick);
      }
      else
      {
        if (pickFlag) holdCtr[0x11db]++;
        PosType e;
        unsigned r1, r2;

        if (length[QT_ACE] == length[QT_PARD]) // &&
            // length[QT_LHO] >= 1 &&
            // completeList[QT_LHO][0] > completeList[QT_ACE][2])
        {
          r1 = SDS_QUEEN;
          r2 = HR(QT_ACE, 2);
          e = QT_ACE;
        }
        else
        {
          r1 = HR(QT_ACE, 2);
          r2 = SDS_VOID;
          e = QT_BOTH;
        }
        rank = Min(r1, r2);
        trick[2].Set(QT_PARD, QT_ACE, r1, 1);
        trick[3].Set(QT_PARD, e, r2, l-1);
        return def.Set112(trick);
      }
    }
    else if (distHex == 0x4153 && counter == 0xce6b0a)
    {
      // AQ54 / 9 / T8732 / KJ6.  Doesn't get more specific than that!
      if (pickFlag) holdCtr[0x11dc]++;
      unsigned r = HR(QT_PARD, 2);
      rank = HR(QT_ACE, 2);
      // trick[2].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
      // trick[3].Set(QT_PARD, QT_PARD, r, 2);
      // trick[4].Set(QT_BOTH, QT_ACE, rank, 1);
      // trick[5].Set(QT_PARD, QT_PARD, SDS_VOID, 1);
      // return def.Set114(trick);
      trick[2].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
      trick[3].Set(QT_PARD, QT_PARD, rank, 3);
      trick[4].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
      trick[5].Set(QT_PARD, QT_ACE, rank, 3);
      trick[6].Set(QT_PARD, QT_PARD, SDS_VOID, 1);
      return def.Set1123(trick);
    }
    else if (distHex == 0x4153 &&
        (counter == 0xce6b22 || counter == 0xce6b28))
    {
      // AQ53|2 / 9 / T8742|3 / KJ6.
      if (pickFlag) holdCtr[0x11dc]++;
      unsigned r1 = HR(QT_PARD, 2);
      unsigned r2 = HR(QT_ACE, 2);
      rank = HR(QT_PARD, 3);
      trick[2].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
      trick[3].Set(QT_PARD, QT_BOTH, rank, 4);
      trick[4].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
      trick[5].Set(QT_PARD, QT_ACE, SDS_VOID, 1);
      trick[6].Set(QT_PARD, QT_ACE, r2, 2);
      trick[7].Set(QT_PARD, QT_PARD, SDS_VOID, 1);
      return def.Set1124(trick);
    }
    else
    {
      if (pickFlag) holdCtr[0x11dd]++;
      unsigned l = Max(length[QT_ACE], length[QT_PARD]);
      if (length[QT_PARD] > length[QT_ACE] &&
          completeList[QT_RHO][2] > completeList[QT_ACE][2])
        rank = HR(QT_PARD, 3);
      else
        rank = HR(QT_PARD, 2);
      trick[2].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
      trick[3].Set(QT_PARD, QT_BOTH, rank, l-1);
      return def.Set112(trick);
    }
  }
  else if (LoopHold::StopFinesse(3, 3, 0, false, QT_ACE))
  {
    if (htop.N == QT_RHO || (htop.N == QT_LHO && htop.T == QT_ACE))
    {
      if (pickFlag) holdCtr[0x11de]++;
      rank = HR(QT_ACE, 2);
      trick[2].Set(QT_PARD, QT_ACE, rank, 1);
      trick[3].Set(QT_PARD, QT_ACE, SDS_VOID, 2);
      return def.Set112(trick);
    }
    else if (htop.T == QT_ACE)
    {
      // The way it comes out.  Is it right?
      if (pickFlag) holdCtr[0x11df]++;
      rank = HR(QT_ACE, 2);
      trick[2].Set(QT_PARD, QT_ACE, rank, 1);
      trick[3].Set(QT_PARD, QT_ACE, SDS_VOID, 2);
      return def.Set112(trick);
    }
    else
    {
      // Could also be queen, rank, but effectively the same.
      if (pickFlag) holdCtr[0x11df]++;
      rank = HR(QT_ACE, 2);
      trick[2].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
      trick[3].Set(QT_PARD, QT_ACE, rank, 2);
      return def.Set112(trick);
    }
  }
  else if (length[QT_PARD] == 3)
  {
    unsigned a2 = completeList[QT_ACE][2];
    unsigned a3 = completeList[QT_ACE][3];
    unsigned p0 = completeList[QT_PARD][0];
    unsigned p1 = completeList[QT_PARD][1];
    unsigned p2 = completeList[QT_PARD][2];
    unsigned p3 = completeList[QT_PARD][3];
    unsigned r2 = completeList[QT_RHO][2];
    unsigned r3 = completeList[QT_RHO][3];
    unsigned aLast = completeList[QT_ACE][length[QT_ACE]-1];
    bool lhoDown = (length[QT_LHO] == 0 ||
        completeList[QT_LHO][0] < completeList[QT_PARD][0]);

    if (p2 > r2 && r2 > a2 && lhoDown)
    {
      if (pickFlag) holdCtr[0x11e0]++;
      rank = HR(QT_PARD, 2);
      trick[2].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
      trick[3].Set(QT_PARD, QT_BOTH, rank, 2);
      return def.Set112(trick);
    }
    else if (length[QT_ACE] >= 5 && lhoDown &&
        a3 > p1 && p1 > r2 && p1 > aLast && p2 < Max(aLast, r2) &&
        (length[QT_RHO] == 4 || (r2 > p2 && r3 > aLast)))
    {
      if (htop.T == QT_ACE && htop.N == QT_ACE)
      {
        if (pickFlag) holdCtr[0x11e1]++;
        rank = HR(QT_PARD, 1);
        unsigned l = (length[QT_RHO] == 4 ? length[QT_ACE] : 4);
        trick[2].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
        trick[3].Set(QT_PARD, QT_ACE, rank, l-1);
        trick[4].Set(QT_PARD, QT_ACE, SDS_TEN, 1);
        trick[5].Set(QT_PARD, QT_ACE, SDS_NINE, 1);
        trick[6].Set(QT_PARD, QT_ACE, SDS_VOID, l-2);
        return def.Set1123(trick);
      }
      else
      {
        if (pickFlag) holdCtr[0x11e1]++;
        rank = HR(QT_PARD, 1);
        unsigned ra1 = HR(QT_ACE, 2);
        unsigned ra2 = HR(QT_PARD, 0);
        unsigned ra3 = Min(ra1, ra2);
        unsigned l = (length[QT_RHO] == 4 ? length[QT_ACE] : 4);
        trick[2].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
        trick[3].Set(QT_PARD, QT_ACE, rank, l-1);
        trick[4].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
        trick[5].Set(QT_PARD, QT_ACE, ra3, 1);
        trick[6].Set(QT_PARD, QT_ACE, HR(QT_ACE, 3), l-2);
        return def.Set1123(trick);
      }
    }
    else if (p1 > a3 && a3 > r2 && lhoDown && p2 < Max(aLast, r2))
    {
      if (distHex == 0x5035)
      {
        if (r2 > p2 && r3 > aLast)
        {
          if (pickFlag) holdCtr[0x11e2]++;
          rank = HR(QT_ACE, 3);
          trick[2].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
          trick[3].Set(QT_PARD, QT_ACE, rank, 3);
          return def.Set112(trick);
        }
      }
      else if (distHex == 0x4234 && htop.E == QT_LHO)
      {
        if (htop.T == QT_PARD && htop.N == QT_PARD && a3 > p2 && a3 > r2)
        {
          if (pickFlag) holdCtr[0x11e3]++;
          rank = HR(QT_ACE, 3);
          trick[2].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
          trick[3].Set(QT_PARD, QT_ACE, rank, 3);
          return def.Set112(trick);
        }
      }
      else
      {
        if (pickFlag) holdCtr[0x11e4]++;
        rank = HR(QT_ACE, 3);
        trick[2].Set(QT_PARD, QT_ACE, SDS_QUEEN, 1);
        trick[3].Set(QT_PARD, QT_ACE, rank, length[QT_ACE]-1);
        return def.Set112(trick);
      }
    }

    if (a3 > r2 && p2 > r2 && p2 < a2 && p2 > aLast && lhoDown &&
      (length[QT_LHO] <= 1 || htop.E != QT_LHO || 
       p1 > completeList[QT_LHO][0]))
    {
      if (pickFlag) holdCtr[0x11e5]++;
      // PA1Q + PP2x, PA1Q + PA(l-1)x.
      // PROBLEM: There are too many versions here.
      // So we won't spell them out for now.
return false;
    }

    if (pickFlag) holdCtr[0x11e6]++;
    // PROBLEM:  Too much going on here for now.
    // At least there are probably no 4's and 6's anymore.
return false;
  }
  else
  {
    if (pickFlag) holdCtr[0x11ef]++;
return false;
  }

  return false;
}


bool LoopHold::SolveComplex18(DefList& def, unsigned& rank) const
{
  // ==== G18 ================ G24 ================ G58 ================
  //      AQ+        |         AK+        |         A+
  // +          J+   |   +           J+   |  J+            +
  //      K+         |         Q+         |         KQ+
  // ==== G18 ================ G24 ================ G58 ================

  if (pickFlag) holdCtr[0x1180]++;

  Trick trick[4];
  PosType pa, pl, pp, pr;
  unsigned l;
  if (htop.K == QT_ACE || htop.Q == QT_ACE)
  {
    // "G18", "G24".  The aceholder has two honors.
    pa = QT_ACE;
    pl = QT_LHO;
    pp = QT_PARD;
    pr = QT_RHO;
  }
  else
  {
    pa = QT_PARD;
    pl = QT_RHO;
    pp = QT_ACE;
    pr = QT_LHO;
  }

  if (length[pa] == 3 && length[pp] >= 4 && length[pl] >= 4)
  {
    if (completeList[pp][1] > completeList[pa][2] &&
        completeList[pp][1] > completeList[pl][1])
    {
      // AQx / Jxxx / KTxx / x.
      if (pickFlag) holdCtr[0x1181]++;
      rank = HR(pp, 1);
      l = (length[pl] > 4 ? 4 : length[pp]);
      trick[0].Set(QT_BOTH, QT_BOTH, rank, l);
      return def.Set1(trick[0]);
    }
  }

  if (distHex == 0x4153)
  {
    if (htop.K == QT_PARD && htop.Q == QT_ACE && 
        htop.T == QT_RHO && htop.N == QT_ACE &&
        completeList[QT_ACE][3] > completeList[QT_PARD][1])
    {
      // Odd recursion case: AQ97 / J / K5432 / T85.
      if (pickFlag) holdCtr[0x1182]++;
      rank = SDS_VOID - 6;
      trick[0].Set(QT_ACE, QT_PARD, rank, 4);
      trick[1].Set(QT_BOTH, QT_ACE, rank, 4);
      trick[2].Set(QT_PARD, QT_PARD, SDS_VOID, 1);
      return def.Set12(trick[0], trick[1], trick[2]);
    }
  }
  else if (((length[pa] == 5 && length[pp] == 4) ||
      (length[pa] == 4 && length[pp] == 5)) && 
       length[pl] == 4)
  {
    if (htop.T == pp)
    {
      // ATxx / - / KQxxx / Jxxx.
      // ATxxx / - / KQxx / Jxxx.
      if (pickFlag) holdCtr[0x1183]++;
      rank = SDS_TEN;
      trick[0].Set(QT_BOTH, QT_BOTH, rank, 5);
      return def.Set1(trick[0]);
    }
    else if (htop.T == pa)
    {
      if (htop.N == pp || (htop.N == pa && length[pa] == 5))
      {
        // AKTxx / Jxxx / Q9xx / -.
        if (pickFlag) holdCtr[0x1184]++;
        rank = SDS_NINE;
        trick[0].Set(QT_BOTH, QT_BOTH, rank, 5);
        return def.Set1(trick[0]);
      }
      else
      {
        // Axxxx / - / KQT9 / Jxxx.
        if (pickFlag) holdCtr[0x1185]++;
        rank = SDS_NINE;
        trick[0].Set(pa, pp, rank, 4);
        trick[1].Set(QT_BOTH, pa, rank, 4);
        trick[2].Set(pp, pp, SDS_VOID, 1);
        return def.Set12(trick[0], trick[1], trick[2]);
      }
    }
  }
  else if (length[pa] >= 4 && length[pp] == 3 &&
      length[pl] == 4 && length[pr] <= 1)
  {
    if (htop.T == pr && htop.N == pa && htop.E == pa)
    {
      // AK98 / J765 / Q43 / T.
      if (pickFlag) holdCtr[0x1186]++;
      rank = SDS_EIGHT;
      l = (length[pl] > 4 ? 4 : length[pa]);
      trick[0].Set(QT_BOTH, QT_BOTH, SDS_QUEEN, 3);
      trick[1].Set(QT_BOTH, pp, rank, 3);
      trick[2].Set(pa, pa, SDS_VOID, l-3);
      return def.Set12(trick[0], trick[1], trick[2]);
    }
    else if (htop.Q == pa && htop.T == pa &&
        completeList[pa][3] > completeList[pl][1])
    {
      // AQT9 / J876 / K54 / -, 11-8415a.
      // The way it comes out for now.  Isn't right.
      if (pickFlag) holdCtr[0x1186]++;
      unsigned ra1 = HR(pa, 3);
      unsigned ra2 = HR(pp, 1);
      rank = Max(ra1, ra2);
      l = (length[pl] > 4 ? 4 : length[pa]);
      trick[0].Set(pa, QT_BOTH, rank, l);
      trick[1].Set(pp, QT_BOTH, SDS_QUEEN, 3);
      trick[2].Set(pp, pp, rank, 3);
      trick[3].Set(pa, pa, SDS_VOID, l-3);
      return def.Set112(trick);
    }
    else if (htop.T == pa && htop.N == pa)
    {
      // AKT9 / J876 / Q54 / -, 11-2415a.
      // The way it comes out for now.  Isn't right.
      if (pickFlag) holdCtr[0x1186]++;
      rank = SDS_NINE;
      l = (length[pl] > 4 ? 4 : length[pa]);
      trick[0].Set(pa, QT_BOTH, SDS_NINE, l);
      trick[1].Set(pp, QT_BOTH, SDS_QUEEN, 3);
      trick[2].Set(pp, pp, SDS_NINE, 3);
      trick[3].Set(pa, pa, SDS_VOID, l-3);
      return def.Set112(trick);
    }
    else if (htop.T == pp || (htop.T == pa && htop.N == pp))
    {
      // AT6 / - / KQ95 / J874.
      if (pickFlag) holdCtr[0x1186]++;
      rank = HR(pp, 1);
      l = (length[pl] > 4 ? 4 : length[pa]);
      trick[0].Set(pa, QT_BOTH, rank, l);
      trick[1].Set(pp, QT_BOTH, SDS_QUEEN, 3);
      trick[2].Set(pp, pp, rank, 3);
      trick[3].Set(pa, pa, SDS_VOID, l-3);
      return def.Set112(trick);
    }
  }
  else if (distHex == 0x4234 || distHex == 0x3442)
  {
    if (completeList[pp][1] > completeList[pa][2] &&
        completeList[pp][1] > completeList[pr][0] && 
        length[pp] == 3)
    {
      if (completeList[pr][0] > completeList[pp][2] &&
          completeList[pr][0] > completeList[pa][2])
      {
        // Will have to unblock and finesse.
        if (completeList[pp][1] > completeList[pl][1] &&
           (completeList[pr][1] > completeList[pa][2] ||
            completeList[pp][2] > completeList[pa][2]))
        {
          // Have a finesse for the jack, but not for the other side.
          // AK54 / J4 / QT2 / 9873.
          // AT8 / 9643 / KQ75 / J2.
          if (pickFlag) holdCtr[0x1189]++;
          rank = HR(pp, 1);
          unsigned r1 = HR(pp, 0);
          unsigned r2 = HR(pa, 0);
          trick[0].Set(pa, QT_BOTH, rank, 4);
          trick[1].Set(pp, QT_BOTH, SDS_QUEEN, 3);
          trick[2].Set(pp, pp, rank, 3);
          trick[3].Set(pa, pa, SDS_VOID, 1);
          return def.Set112(trick);
        }
        else if (completeList[pp][2] < completeList[pa][2])
        {
          // Possible to unblock the short side.
          // The way it comes out.
          if (completeList[pp][1] > completeList[pl][1])
          {
            // AK8x / Jx / QTx / 9xxx.
            if (pickFlag) holdCtr[0x118a]++;
            rank = HR(pa, 2);
            unsigned r3 = HR(pp, 1);
            trick[0].Set(pa, QT_BOTH, r3, 4);
            trick[1].Set(pp, pa, rank, 4);
            trick[2].Set(pp, pp, SDS_QUEEN, 3);
            trick[3].Set(pp, pp, r3, 3);
            trick[4].Set(pa, pa, SDS_VOID, 1);
            return def.Set1112(trick);
          }
          else if (length[QT_ACE] == 4)
          {
            // The way it comes out.
            // Actually can eliminate length 4 test now.
            if (completeList[pa][2] > completeList[pr][1])
            {
              // AK7x / JT / Q9x / 8xxx.
              if (pickFlag) holdCtr[0x118b]++;
              rank = HR(pa, 2);
              unsigned r1 = HR(pp, 1);
              trick[0].Set(QT_BOTH, pa, rank, 4);
              trick[1].Set(QT_ACE, pp, SDS_QUEEN, 3);
              trick[2].Set(QT_ACE, pp, r1, 3);
              trick[3].Set(pa, pa, SDS_VOID, 1);
              return def.Set112(trick);
            }
            else
            {
              if (pickFlag) holdCtr[0x118c]++;
              rank = HR(pp, 1);
              trick[0].Set(QT_BOTH, QT_BOTH, SDS_QUEEN, 3);
              trick[1].Set(QT_BOTH, pp, rank, 3);
              trick[2].Set(pa, pa, SDS_VOID, 1);
              return def.Set12(trick[0], trick[1], trick[2]);
            }
          }
          else if (completeList[pa][2] > completeList[pr][1])
          {
            // A92 / 8543 / KQ76 / JT.
            if (pickFlag) holdCtr[0x118d]++;
            rank = HR(pa, 2);
            unsigned r3 = HR(pp, 1);
            trick[0].Set(QT_BOTH, pa, rank, 4);
            trick[1].Set(pa, pp, r3, 3);
            trick[2].Set(pa, pa, SDS_VOID, 1);
            return def.Set12(trick[0], trick[1], trick[2]);
          }
          else
          {
            // A92 / 8743 / KQ65 / JT.
            if (pickFlag) holdCtr[0x118e]++;
            rank = HR(pp, 1);
            trick[0].Set(QT_BOTH, QT_BOTH, SDS_QUEEN, 3);
            trick[1].Set(QT_BOTH, pp, rank, 3);
            trick[2].Set(pa, pa, SDS_VOID, 1);
            return def.Set12(trick[0], trick[1], trick[2]);
          }
        }
        else
        {
          // The way it comes out.  Not necessarily right.
          // AQ54 / JT / K93 / 8762.
          if (pickFlag) holdCtr[0x118f]++;
          rank = HR(pp, 1);
          trick[0].Set(QT_BOTH, QT_BOTH, SDS_QUEEN, 3);
          trick[1].Set(QT_BOTH, pp, rank, 3);
          trick[2].Set(pa, pa, SDS_VOID, 1);
          return def.Set12(trick[0], trick[1], trick[2]);
        }
      }
      else if (completeList[pr][0] < completeList[pa][2] &&
          completeList[pr][0] < completeList[pp][1] &&
          completeList[pp][1] > completeList[pl][1])
      {
        // Will have to unblock.
        // AK9x / Jx / QT8 / 65xx.
        if (pickFlag) holdCtr[0x11f2]++;
        unsigned r1 = HR(pp, 1);
        rank = HR(pa, 2);
        trick[0].Set(pa, QT_BOTH, r1, 4);
        trick[1].Set(pp, QT_BOTH, rank, 4);
        return def.Set11(trick[0], trick[1]);
      }
      else if (completeList[pr][0] < completeList[pp][2])
      {
        if (completeList[pr][0] < completeList[pp][2])
        {
          if (completeList[pp][1] > completeList[pl][1])
          {
            // AK65 / J9 / QT8 / 7432.
            if (pickFlag) holdCtr[0x11f3]++;
            unsigned r1 = HR(pp, 0);
            unsigned r2 = HR(pa, 0);
            rank = HR(pp, 1);
            trick[0].Set(pa, QT_BOTH, rank, 4);
            trick[1].Set(pp, QT_BOTH, SDS_QUEEN, 3);
            trick[2].Set(pp, pp, rank, 3);
            trick[3].Set(pa, pa, SDS_VOID, 1);
            return def.Set112(trick);
          }
          else if (completeList[pp][2] > completeList[pa][2] &&
              completeList[pr][0] > completeList[pa][2])
          {
            // Recombine.
            // The way it comes out.
            if (length[QT_ACE] == 4)
            {
              // AK65 / JT / Q98 / 7432.
              if (pickFlag) holdCtr[0x11f4]++;
              rank = HR(pp, 1);
              trick[0].Set(QT_BOTH, QT_BOTH, SDS_QUEEN, 3);
              trick[1].Set(QT_BOTH, pp, rank, 3);
              trick[2].Set(pa, pa, SDS_VOID, 1);
              return def.Set12(trick[0], trick[1], trick[2]);
            }
            else
            {
              // A98 / 7432 / KQ65 / JT.
              if (pickFlag) holdCtr[0x11f5]++;
              unsigned r1 = HR(pp, 0);
              unsigned r2 = HR(pa, 0);
              rank = HR(pp, 1);
              trick[0].Set(QT_BOTH, QT_BOTH, SDS_QUEEN, 3);
              trick[1].Set(QT_BOTH, pp, rank, 3);
              trick[2].Set(pa, pa, SDS_VOID, 1);
              return def.Set12(trick[0], trick[1], trick[2]);
            }
          }
        }
      }
    }
  }

  return false;
}


bool LoopHold::SolveComplex19(DefList& def, unsigned& rank) const
{
  // ==== G19 ==========================================================
  //      AQ+    
  // K+         J+
  //      +      
  // ==== G19 ==========================================================

  if (pickFlag) holdCtr[0x1190]++;
  UNUSED(def);
  UNUSED(rank);
  return false;
}


bool LoopHold::SolveComplex20(DefList& def, unsigned& rank) const
{
  // ==== G20 ============= G22 ========================================
  //      AK+        |      A+     
  // +          QJ+  |  +       QJ+
  //      +          |      K+     
  // ==== G20 ============= G22 ========================================

  if (distHex != 0x4234)
    return false;

  Trick trick[5];
  if (htop.K == QT_ACE)
  {
    if (htop.T == QT_PARD && 
        completeList[QT_ACE][3] > completeList[QT_PARD][2] &&
        completeList[QT_ACE][2] > completeList[QT_RHO][0] &&
        completeList[QT_ACE][3] < completeList[QT_RHO][0] &&
        completeList[QT_ACE][3] > completeList[QT_RHO][1])
    {
      // AK96 / QJ / T75 / 8432.
      if (pickFlag) holdCtr[0x1200]++;
      unsigned r1 = HR(QT_ACE, 2);
      unsigned r2 = HR(QT_ACE, 3);
      rank = r2;
      trick[0].Set(QT_PARD, QT_ACE, SDS_TEN, 3);
      trick[1].Set(QT_BOTH, QT_ACE, SDS_KING, 2);
      trick[2].Set(QT_PARD, QT_ACE, r2, 2);
      trick[3].Set(QT_BOTH, QT_PARD, SDS_TEN, 3);
      trick[4].Set(QT_ACE, QT_ACE, r1, 1);
      return def.Set122(trick);
    }
    else if (htop.T == QT_ACE && htop.N == QT_PARD &&
        completeList[QT_ACE][3] > completeList[QT_PARD][2] &&
        completeList[QT_ACE][2] > completeList[QT_RHO][0] &&
        completeList[QT_ACE][3] < completeList[QT_RHO][0] &&
        completeList[QT_ACE][3] > completeList[QT_RHO][1])
    {
      // AKT6 / QJ / 975 / 8432.
      if (pickFlag) holdCtr[0x1200]++;
      unsigned r2 = HR(QT_ACE, 3);
      rank = r2;
      trick[0].Set(QT_BOTH, QT_ACE, SDS_TEN, 3);
      trick[1].Set(QT_BOTH, QT_ACE, SDS_KING, 2);
      trick[2].Set(QT_PARD, QT_ACE, r2, 2);
      trick[3].Set(QT_BOTH, QT_PARD, SDS_NINE, 3);
      trick[4].Set(QT_ACE, QT_ACE, SDS_VOID, 1);
      return def.Set122(trick);
    }
    else if (htop.T == QT_ACE && htop.N == QT_RHO &&
        completeList[QT_ACE][3] > completeList[QT_RHO][1])
    {
      if (completeList[QT_PARD][2] < completeList[QT_ACE][3])
      {
        // AKT8 / QJ / 532 / 9764.
        if (pickFlag) holdCtr[0x1200]++;
        rank = HR(QT_ACE, 3);
        trick[0].Set(QT_BOTH, QT_ACE, SDS_TEN, 3);
        trick[1].Set(QT_BOTH, QT_ACE, SDS_KING, 2);
        trick[2].Set(QT_PARD, QT_ACE, rank, 2);
        return def.Set12(trick[0], trick[1], trick[2]);
      }
    }
  }
  else if ((htop.T == QT_PARD && htop.N == QT_ACE) ||
      (htop.T == QT_ACE && htop.N == QT_PARD))
  {
    if (htop.E == QT_RHO &&
        completeList[QT_ACE][2] > completeList[QT_PARD][2] &&
        completeList[QT_ACE][2] > completeList[QT_RHO][1])
    {
      // A97x / QJ / KTx / 8xxx.
      // Clean up.
      if (htop.T == QT_ACE)
      {
        if (pickFlag) holdCtr[0x1201]++;
        rank = HR(QT_ACE, 2);

        unsigned r1 = HR(QT_PARD, 1);
        unsigned r2 = HR(QT_ACE, 1);
        if (r2 > r1)
          r2 = SDS_VOID;
        trick[0].Set(QT_BOTH, QT_ACE, rank, 4);
        trick[1].Set(QT_ACE, QT_PARD, SDS_TEN, 3);
        trick[2].Set(QT_ACE, QT_PARD, r1, 3);
        trick[3].Set(QT_ACE, QT_ACE, r2, 1);
        return def.Set112(trick);
      }
      else
      {
        if (pickFlag) holdCtr[0x1202]++;
        rank = HR(QT_ACE, 2);

        unsigned r1 = HR(QT_PARD, 1);
        unsigned r2 = HR(QT_ACE, 1);
        if (r2 > r1)
          r2 = SDS_VOID;
        trick[0].Set(QT_BOTH, QT_ACE, rank, 4);
        trick[1].Set(QT_ACE, QT_PARD, r1, 3);
        trick[2].Set(QT_ACE, QT_ACE, r2, 1);
        return def.Set12(trick[0], trick[1], trick[2]);
      }
    }
  }
  else if (htop.T == QT_ACE && htop.N == QT_RHO && 
      completeList[QT_ACE][2] > completeList[QT_RHO][1])
  {
    if (completeList[QT_PARD][2] < completeList[QT_ACE][2])
    {
      // AT8x / QJ / Kxx / 9xxx.
      // The defense doesn't matter.  If declarer unblocks
      // the 8 from A82 / 9643 / KT75 / QJ, he can always get BP47.
      // If he doesn't, LHO can still cover and hold him to BP47.
      // Or he can duck.  Declarer only permits this when he
      // really wants to be in North.  And so the defender should
      // always cover anyway.

      if (pickFlag) holdCtr[0x1202]++;
      rank = HR(QT_ACE, 2);
      trick[0].Set(QT_BOTH, QT_ACE, rank, 4);
      trick[1].Set(QT_ACE, QT_PARD, SDS_TEN, 3);
      return def.Set11(trick[0], trick[1]);
    } 
  }

  return false;
}


bool LoopHold::SolveComplex26(DefList& def, unsigned& rank) const
{
  // ==== G26 ================ G50 ================ G56 ================
  //      A+         |         AQ+        |         AK+ 
  // +          J+   |   J+          +    |   J+          +  
  //      KQ+        |         K+         |         Q+
  // ==== G26 ================ G50 ================ G56 ================

  if (pickFlag) holdCtr[0x1260]++;

  Trick trick[5];
  PosType pa, pl, pp, pr;
  if (htop.K == QT_PARD && htop.Q == QT_PARD)
  {
    // "G26".  Aceholder has a single honor.
    pa = QT_ACE;
    pl = QT_LHO;
    pp = QT_PARD;
    pr = QT_RHO;
  }
  else
  {
    pa = QT_PARD;
    pl = QT_RHO;
    pp = QT_ACE;
    pr = QT_LHO;
  }

  if (length[pa] >= 5 && length[pp] == 4 && length[pl] == 3)
  {
    if (htop.T == pp)
    {
      if (completeList[pp][3] > completeList[pa][1])
      {
        // AQT9 / x / Kxxxx / Jxx.
        if (pickFlag) holdCtr[0x1261]++;
        rank = HR(pp, 2);
        trick[0].Set(pa, QT_BOTH, rank, length[pa]);
        trick[1].Set(pp, pa, rank, 4);
        trick[2].Set(pp, pp, rank, 4);
        trick[3].Set(pa, pa, SDS_VOID, length[pa]-4);
        return def.Set112(trick);
      }
      else
      {
        // AKT8 / x / Q9765 / J43.
        if (pickFlag) holdCtr[0x1262]++;
        rank = HR(pa, 1);
        trick[0].Set(pp, QT_BOTH, rank, length[pa]);
        trick[1].Set(pa, QT_BOTH, SDS_TEN, length[pa]);
        return def.Set11(trick[0], trick[1]);
      }
    }
  }
  else if (length[pa] == 4 && length[pp] == 5 && length[pl] == 4)
  {
    if (htop.T == pa && htop.N == pa)
    {
      // AK643 / - / QT9x / J8xx.
      if (pickFlag) holdCtr[0x1263]++;
      rank = SDS_NINE;
      trick[0].Set(QT_BOTH, QT_BOTH, rank, length[pp]);
      return def.Set1(trick[0]);
    }
  }
  else if (length[pa] == 3 && length[pp] >= 4 && length[pl] >= 4)
  {
    if (completeList[pp][2] > completeList[pl][1] &&
        completeList[pp][2] > completeList[pa][2])
    {
      if (completeList[pa][2] > completeList[pp][3] &&
          completeList[pp][3] > completeList[pl][1])
      {
        if (length[pr] == 2 && htop.T == pr)
        {
          if (pickFlag) holdCtr[0x1264]++;
          rank = HR(pp, 2);
          trick[0].Set(pp, pa, SDS_QUEEN, 3);
          trick[1].Set(QT_BOTH, pp, rank, length[pp]);
          return def.Set11(trick[0], trick[1]);
        }
        else if (length[pr] == 2 && htop.T == pp && htop.N == pr)
        {
          if (pickFlag) holdCtr[0x1265]++;
          rank = HR(pp, 2);
          trick[0].Set(pp, pa, SDS_QUEEN, 3);
          trick[1].Set(pp, pp, rank, length[pp]);
          trick[2].Set(pa, QT_BOTH, rank, length[pp]);
          return def.Set111(trick);
        }
        else if (length[pr] == 1 && htop.T == pr)
        {
          // The way it comes out.  Is it right?
          // if (length[pp] == 4)
          // {
            if (pickFlag) holdCtr[0x1266]++;
            unsigned l = (length[pl] == 4 ? length[pp] : 4);
            rank = HR(pa, 2);
            trick[0].Set(QT_BOTH, pp, HR(pp, 2), length[pp]);
            trick[1].Set(pp, pa, SDS_QUEEN, 3);
            return def.Set11(trick[0], trick[1]);
          // }
          // else
          // {
            // if (pickFlag) holdCtr[0x1266]++;
            // unsigned l = (length[pl] == 4 ? length[pp] : 4);
            // rank = HR(pa, 2);
            // trick[0].Set(QT_BOTH, pp, HR(pp, 2), length[pp]);
            // trick[1].Set(pp, pa, rank, 3);
            // trick[2].Set(pp, pp, SDS_VOID, length[pp]-3);
            // return def.Set12(trick[0], trick[1], trick[2]);
          // }
        }
        else if (length[pp] == 5 && length[pl] == 5)
        {
          if (pickFlag) holdCtr[0x1267]++;
          rank = HR(pp, 3);
          trick[0].Set(pa, QT_BOTH, rank, 5);
          trick[1].Set(pp, pp, rank, 5);
          trick[2].Set(QT_BOTH, pa, HR(pa, 2), 3);
          trick[3].Set(pp, pp, SDS_VOID, 2);
          return def.Set112(trick);
        }
        else // if (length[pp] == 4)
        {
          if (pickFlag) holdCtr[0x1269]++;
          rank = HR(pp, 2);
          trick[0].Set(pp, pa, SDS_QUEEN, 3);
          trick[1].Set(pp, pp, rank, length[pp]);
          trick[2].Set(pa, QT_BOTH, rank, length[pp]);
          return def.Set111(trick);
        }
        // else
        // {
          // The way it comes out, may not be right.
          // if (pickFlag) holdCtr[0x1269]++;
          // unsigned r1 = HR(pa, 2);
          // unsigned r2 = HR(pp, 2);
          // rank = Min(r1, r2);
          // unsigned l = (length[pl] == 4 ? length[pp] : 4);
          // trick[0].Set(pa, QT_BOTH, r2, l);
          // trick[1].Set(pp, pp, r2, l);
          // trick[2].Set(pp, pa, rank, 3);
          // trick[3].Set(pp, pp, SDS_VOID, l-3);
          // return def.Set112(trick);
        // }
      }
      else if (length[pp] >= 5)
      {
        // Many possibilities.
        if (pickFlag) holdCtr[0x126a]++;
        return false;
      }
      else if (length[pr] == 0 ||
          completeList[pr][0] < completeList[pp][2] ||
          completeList[pr][0] < completeList[pa][1])
      {
        // AKT6 / - / Q94 / J875.
        if (pickFlag) holdCtr[0x12a1]++;
        rank = HR(pp, 2);
        trick[0].Set(pp, pa, SDS_QUEEN, 3);
        trick[1].Set(pp, pp, rank, length[pp]);
        trick[2].Set(pa, QT_BOTH, rank, length[pp]);
        return def.Set111(trick);
      }
      else
      {
        // AK98 / T / Q54 / J763.
        if (pickFlag) holdCtr[0x12a2]++;
        rank = HR(pp, 2);
        trick[0].Set(pp, pa, SDS_QUEEN, 3);
        trick[1].Set(QT_BOTH, pp, rank, length[pp]);
        return def.Set11(trick[0], trick[1]);
      }
    }
  }
  else if (length[pa] == 3 && length[pp] >= 4 && length[pr] >= 4)
  {
    if (length[pl] == 1)
    {
      if (htop.T == pr && htop.N == pp && ! hopp.E)
      {
        // AK9x / T7543 / Q82 / J.
        if (pickFlag) holdCtr[0x12a4]++;
        rank = SDS_EIGHT;
        unsigned l = (length[pr] == 4 ? length[pp] : 4);
        trick[0].Set(QT_BOTH, QT_BOTH, SDS_QUEEN, 3);
        trick[1].Set(QT_BOTH, pa, rank, 3);
        trick[2].Set(pp, pp, SDS_VOID, l-3);
        return def.Set12(trick[0], trick[1], trick[2]);
      }
    }
    else if (htop.T == pr)
    {
      // Axx / Jx / KQxx / Txxx.
      if (pickFlag) holdCtr[0x12a5]++;
      rank = SDS_QUEEN;
      trick[0].Set(QT_BOTH, QT_BOTH, SDS_QUEEN, 3);
      return def.Set1(trick[0]);
    }
    else if (htop.T == pl &&
        completeList[pa][1] > completeList[pr][0] &&
        completeList[pr][0] > completeList[pp][2])
    {
      return false;
      // Check that these are all caught above as well.
      
      // The way it comes out.
      if (length[QT_ACE] == 4)
      {
        // AQ32 / 7654 / K98 / JT.
        if (pickFlag) holdCtr[0x1268]++;
        rank = HR(pa, 1);
        trick[0].Set(QT_BOTH, pp, SDS_ACE, 1);
        trick[1].Set(QT_BOTH, pa, rank, 2);
        trick[2].Set(pp, pp, SDS_VOID, 1);
        return def.Set3(trick[0], trick[1], trick[2]);
      }
      else
      {
        // A98 / JT / KQ65 / 7432.
        if (pickFlag) holdCtr[0x1269]++;
        rank = HR(pa, 1);
        unsigned r1 = HR(QT_PARD, 0);
        unsigned r2 = HR(QT_ACE, 0);
        trick[0].Set(QT_BOTH, pp, Min(r1, r2), 2);
        trick[1].Set(QT_BOTH, pa, rank, 1);
        trick[2].Set(pp, pp, SDS_VOID, 1);
        return def.Set3(trick[0], trick[1], trick[2]);
      }
    }
    else if (htop.T == pa && completeList[pr][0] > completeList[pp][2])
    {
      return false;
      // Check that these are all caught above as well.

      // The way it comes out.
      if (length[QT_ACE] == 3)
      {
        // AT2 / J3 / KQ54 / 9876.
        if (pickFlag) holdCtr[0x126a]++;
        rank = HR(pa, 1);
        unsigned r1 = HR(QT_PARD, 0);
        unsigned r2 = HR(QT_ACE, 0);
        trick[0].Set(QT_BOTH, pp, Min(r1, r2), 2);
        trick[1].Set(QT_BOTH, pa, rank, 1);
        trick[2].Set(pp, pp, SDS_VOID, 1);
        return def.Set3(trick[0], trick[1], trick[2]);
      }
    }
  }

  return false;
}


bool LoopHold::SolveComplex27(DefList& def, unsigned& rank) const
{
  // ==== G27 ==========================================================
  //      A+     
  // K+         J+
  //      Q+     
  // ==== G27 ==========================================================

  if (pickFlag) holdCtr[0x1270]++;

  Trick trick[3];
  if (length[QT_ACE] == 3 && length[QT_PARD] > 3 &&
      length[QT_RHO] == 1 && length[QT_LHO] == 3)
  {
    if (htop.T == QT_ACE && htop.N == QT_ACE)
    {
      // AT9 / Jxx / Qxxx+ / K.
      if (pickFlag) holdCtr[0x1271]++;
      rank = SDS_NINE;
      trick[0].Set(QT_ACE, QT_PARD, rank, 3);
      trick[1].Set(QT_BOTH, QT_ACE, rank, 3);
      trick[2].Set(QT_PARD, QT_PARD, SDS_VOID, length[QT_PARD]-3);
      return def.Set12(trick[0], trick[1], trick[2]);
    }
  }
  else if (distHex == 0x4341)
  {
    if (htop.T == QT_ACE && htop.N == QT_PARD)
    {
      if (pickFlag) holdCtr[0x1272]++;
      rank = SDS_NINE;
      trick[0].Set(QT_BOTH, QT_BOTH, rank, 4);
      return def.Set1(trick[0]);
    }
    else if (htop.T == QT_PARD && htop.N == QT_ACE)
    {
      if (pickFlag) holdCtr[0x1273]++;
      rank = SDS_NINE;
      trick[0].Set(QT_BOTH, QT_PARD, SDS_TEN, 4);
      trick[1].Set(QT_PARD, QT_ACE, rank, 4);
      return def.Set11(trick[0], trick[1]);
    }
  }
  else if (length[QT_ACE] > 3 && length[QT_PARD] == 3 &&
      length[QT_RHO] == 1)
  {
    if (length[QT_LHO] > 3)
    {
      if (htop.T == QT_PARD)
      {
        if (htop.N == QT_ACE && htop.E == QT_ACE)
        {
          // A98x / Jxxx / QTx / K.
          if (pickFlag) holdCtr[0x1274]++;
          rank = SDS_EIGHT;
          trick[0].Set(QT_BOTH, QT_PARD, SDS_TEN, 3);
          trick[1].Set(QT_PARD, QT_ACE, rank, 3);
          return def.Set11(trick[0], trick[1]);
        }
        else if (hopp.N && hopp.E)
        {
          // Axxx / Jxxx / QTx / K.
          if (pickFlag) holdCtr[0x1275]++;
          rank = SDS_TEN;
          trick[0].Set(QT_BOTH, QT_PARD, SDS_TEN, 3);
          trick[1].Set(QT_PARD, QT_ACE, SDS_QUEEN, 2);
          return def.Set11(trick[0], trick[1]);
        }
      }
      else if (htop.T == QT_ACE && htop.N == QT_ACE)
      {
        // AT9x / Jxxx / Qxx / K.
        if (pickFlag) holdCtr[0x1276]++;
        rank = SDS_NINE;
        trick[0].Set(QT_BOTH, QT_BOTH, rank, 3);
        return def.Set1(trick[0]);
      }
    }
    else if (length[QT_LHO] == 3)
    {
      if (htop.T == QT_ACE && htop.N == QT_ACE)
      {
        // AT9x / Jxx / Qxx / K.
        if (pickFlag) holdCtr[0x1277]++;
        rank = SDS_NINE;
        trick[0].Set(QT_PARD, QT_ACE, rank, 3);
        trick[1].Set(QT_BOTH, QT_PARD, rank, 3);
        trick[2].Set(QT_ACE, QT_ACE, SDS_VOID, length[QT_ACE]-3);
        return def.Set12(trick[0], trick[1], trick[2]);
      }
    }
  }
  else if (distHex == 0x5341)
  {
    if (htop.T == QT_PARD)
    {
      if (completeList[QT_ACE][1] > completeList[QT_PARD][3])
      {
        // A9xxx / J8x / QTxx / K.
        if (pickFlag) holdCtr[0x1278]++;
        rank = HR(QT_ACE, 1);
        trick[0].Set(QT_BOTH, QT_BOTH, rank, length[QT_ACE]);
        return def.Set1(trick[0]);
      }
    }
    else if (htop.N == QT_PARD)
    {
      // ATxxx / Jxx / Q9xx / K.
      if (pickFlag) holdCtr[0x1279]++;
      rank = SDS_NINE;
      trick[0].Set(QT_BOTH, QT_BOTH, rank, length[QT_ACE]);
      return def.Set1(trick[0]);
    }
    else if (htop.E == QT_PARD)
    {
      // AT9xx / Jxx / Q8xx / K.
      if (pickFlag) holdCtr[0x127a]++;
      rank = SDS_EIGHT;
      trick[0].Set(QT_BOTH, QT_BOTH, rank, length[QT_ACE]);
      return def.Set1(trick[0]);
    }
    else if (completeList[QT_ACE][3] > completeList[QT_PARD][3])
    {
      // AT942 / J87 / Q653 / K.
      if (pickFlag) holdCtr[0x127b]++;
      rank = HR(QT_ACE, 3);
      trick[0].Set(QT_BOTH, QT_BOTH, rank, length[QT_ACE]);
      return def.Set1(trick[0]);
    }
    else
    {
      // AT9xx / J8x / Q7xx / K.
      if (pickFlag) holdCtr[0x127c]++;
      rank = HR(QT_PARD, 1);
      trick[0].Set(QT_PARD, QT_ACE, rank, 4);
      trick[1].Set(QT_BOTH, QT_PARD, rank, 4);
      trick[2].Set(QT_ACE, QT_ACE, SDS_VOID, 1);
      return def.Set12(trick[0], trick[1], trick[2]);
    }
  }
  else if (distHex == 0x4351)
  {
    if (htop.T == QT_PARD)
    {
      if (htop.N == QT_ACE && htop.E == QT_ACE)
      {
        // A987 / J62 / QT543 / K.
        if (pickFlag) holdCtr[0x127d]++;
        rank = SDS_EIGHT;
        trick[0].Set(QT_BOTH, QT_BOTH, rank, 5);
        return def.Set1(trick[0]);
      }
      else if (completeList[QT_ACE][3] > completeList[QT_PARD][2])
      {
        // A876 / J92 / QT543 / K.
        if (pickFlag) holdCtr[0x127e]++;
        rank = HR(QT_ACE, 1);
        trick[0].Set(QT_ACE, QT_PARD, rank, 4);
        trick[1].Set(QT_BOTH, QT_ACE, rank, 4);
        trick[2].Set(QT_PARD, QT_PARD, SDS_VOID, 1);
        return def.Set12(trick[0], trick[1], trick[2]);
      }
    }
    else if (htop.N == QT_PARD)
    {
      if (completeList[QT_ACE][2] > completeList[QT_PARD][2] &&
          completeList[QT_ACE][2] > completeList[QT_LHO][1])
      {
        // AT8x / J7x / Q9xxx / K.
        if (pickFlag) holdCtr[0x127f]++;
        rank = HR(QT_ACE, 2);
        trick[0].Set(QT_BOTH, QT_BOTH, rank, 5);
        return def.Set1(trick[0]);
      }
      else if (completeList[QT_ACE][3] > completeList[QT_PARD][2])
      {
        // AT75 / J86 / Q9432 / K.
        if (pickFlag) holdCtr[0x1291]++;
        rank = SDS_NINE;
        trick[0].Set(QT_ACE, QT_PARD, rank, 4);
        trick[1].Set(QT_BOTH, QT_ACE, rank, 4);
        trick[2].Set(QT_PARD, QT_PARD, SDS_VOID, 1);
        return def.Set12(trick[0], trick[1], trick[2]);
      }
    }
    else if (completeList[QT_ACE][3] > completeList[QT_PARD][1])
    {
      // AT97 / J84 / Q6532 / K.
      if (pickFlag) holdCtr[0x1292]++;
      rank = HR(QT_ACE, 3);
      trick[0].Set(QT_ACE, QT_PARD, rank, 4);
      trick[1].Set(QT_BOTH, QT_ACE, rank, 4);
      trick[2].Set(QT_PARD, QT_PARD, SDS_VOID, 1);
      return def.Set12(trick[0], trick[1], trick[2]);
    }
  }

  return false;
}


bool LoopHold::SolveComplex28(DefList& def, unsigned& rank) const
{
  // ==== G28 ============= G30 =========== G52 =========== G54 ========
  //      AK+        |      A+       |      AK+      |      A+
  // Q+         J+   |  Q+      J+   |  J+      Q+   |  J+      Q+ 
  //      +          |      K+       |      +        |      K+
  // ==== G28 ============= G30 =========== G52 =========== G54 ========

  if (pickFlag) holdCtr[0x1280]++;

  Trick trick[3];
  PosType pa, pl, pp, pr;
  if (length[QT_ACE] > length[QT_PARD] ||
     (length[QT_ACE] == length[QT_PARD] &&
      length[QT_LHO] <= length[QT_RHO]))
  {
    // Aceholder is longer, or if equal, LHO is short.
    pa = QT_ACE;
    pl = QT_LHO;
    pp = QT_PARD;
    pr = QT_RHO;
  }
  else
  {
    pa = QT_PARD;
    pl = QT_RHO;
    pp = QT_ACE;
    pr = QT_LHO;
  }

  if (htop.K == QT_PARD)
  {
    if (distHex == 0x4143 || distHex == 0x4341)
    {
      if (htop.T == pa && htop.N == pp)
      {
        // ATxx / Q / K9xx / Jxx.
        if (pickFlag) holdCtr[0x1281]++;
        rank = SDS_NINE;
        trick[0].Set(QT_BOTH, pa, SDS_TEN, 4);
        trick[1].Set(pa, pp, rank, 4);
        return def.Set11(trick[0], trick[1]);
      }
      else if (completeList[pp][1] < completeList[pa][3])
      {
        // AT98 / Q / K765 / J43.
        if (pickFlag) holdCtr[0x1282]++;
        rank = SDS_TEN;
        trick[0].Set(QT_BOTH, pa, rank, 4);
        trick[1].Set(pa, pp, SDS_KING, 2);
        return def.Set11(trick[0], trick[1]);
      }
      else if (htop.T == pp && htop.N == pa)
      {
        // A9xx / Q / KTxx / Jxx.
        if (pickFlag) holdCtr[0x1283]++;
        rank = SDS_NINE;
        trick[0].Set(QT_BOTH, QT_BOTH, rank, 4);
        return def.Set1(trick[0]);
      }
    }
    else if (length[pa] == 5 && length[pp] == 4 &&
        length[pr] == 1 && length[pl] == 3)
    {
      if (completeList[pp][1] > completeList[pl][1] &&
          completeList[pp][1] > completeList[pa][4] &&
          completeList[pa][1] > completeList[pp][3])
      {
        if (completeList[pa][3] > completeList[pp][1] &&
            completeList[pa][3] > completeList[pl][1])
        {
          // A7xx / Q / KT98x / Jxx.
          if (pickFlag) holdCtr[0x1284]++;
          rank = HR(pa, 3);
          trick[0].Set(QT_BOTH, QT_BOTH, rank, length[pa]);
          return def.Set1(trick[0]);
        }
        else
        {
          // A8xxx / Jxx / KT97 / Q.
          if (pickFlag) holdCtr[0x1285]++;
          unsigned r1 = HR(pa, 1);
          unsigned r2 = HR(pp, 1);
          rank = Min(r1, r2);
          trick[0].Set(QT_BOTH, QT_BOTH, rank, length[pa]);
          return def.Set1(trick[0]);
        }
      }
    }
    else if (length[pa] == 5 && length[pp] == 4 &&
        length[pr] == 3 && length[pl] == 1)
    {
      if (completeList[pp][2] > completeList[pr][1] &&
          completeList[pp][2] > completeList[pa][2] &&
          completeList[pa][1] > completeList[pr][1])
      {
        if (completeList[pp][3] > completeList[pa][1])
        {
          // AT98 / Q65 / K7432 / J.
          if (pickFlag) holdCtr[0x1286]++;
          rank = HR(pp, 3);
          trick[0].Set(pp, pa, rank, 4);
          trick[1].Set(QT_BOTH, pp, rank, 4);
          trick[2].Set(pa, pa, SDS_VOID, 1);
          return def.Set12(trick[0], trick[1], trick[2]);
        }
        else
        {
          // A9xxx / J / KT8x / Qxx.
          if (pickFlag) holdCtr[0x1287]++;
          unsigned r1 = HR(pa, 1);
          unsigned r2 = HR(pp, 2);
          rank = Min(r1, r2);
          trick[0].Set(QT_BOTH, QT_BOTH, rank, length[pa]);
          return def.Set1(trick[0]);
        }
      }
    }
    else if (length[pa] >= 4 && length[pp] == 3)
    {
      if (length[pr] == 1)
      {
        if (length[pl] > 3)
        {
          if (htop.T == pa && htop.N == pa)
          {
            // A53 / J / KT98 / Q7642.
            if (pickFlag) holdCtr[0x1288]++;
            rank = SDS_NINE;
            trick[0].Set(QT_BOTH, QT_BOTH, rank, 3);
            return def.Set1(trick[0]);
          }
        }
        else if (length[pl] == 3)
        {
          if (! hopp.T && ! hopp.N)
          {
            if (htop.T == pp || htop.N == pp)
            {
              if ((htop.T == pp && htop.N == pp) ||
                  (htop.T != htop.N && htop.E != pa))
              {
                // A654 / Q87 / KT9 / J.
                if (pickFlag) holdCtr[0x1289]++;
                rank = HR(pp, 1);
                trick[0].Set(pp, pa, SDS_KING, 2);
                trick[1].Set(QT_BOTH, pp, rank, 3);
                trick[2].Set(pa, pa, SDS_VOID, length[pa]-3);
                return def.Set12(trick[0], trick[1], trick[2]);
              }
            }
            else
            {
              // A86 / J / KT9743 / Q52.
              if (pickFlag) holdCtr[0x128a]++;
              rank = SDS_NINE;
              trick[0].Set(pp, pa, rank, 3);
              trick[1].Set(QT_BOTH, pp, rank, 3);
              trick[2].Set(pa, pa, SDS_VOID, length[pa]-3);
              return def.Set12(trick[0], trick[1], trick[2]);
            }
          }
        }
        else if (completeList[pa][1] > completeList[pp][2])
        {
          // A63 / Q / KT9754 / J8.
          if (pickFlag) holdCtr[0x128b]++;
          rank = HR(pa, 1);
          trick[0].Set(QT_BOTH, QT_BOTH, rank, length[pa]);
        }
      }
      else if (length[pl] == 1 && length[pr] == 3)
      {
        if (htop.T == pp && htop.N == pp)
        {
          // Axxx / J / KT9 / Qxx.
          if (pickFlag) holdCtr[0x128c]++;
          rank = SDS_NINE;
          trick[0].Set(pp, pa, rank, 3);
          trick[1].Set(QT_BOTH, pp, rank, 3);
          trick[2].Set(pa, pa, SDS_VOID, length[pa]-3);
          return def.Set12(trick[0], trick[1], trick[2]);
        }
      }
    }
  }

  return false;
}


bool LoopHold::SolveComplex36(DefList& def, unsigned& rank) const
{
  // ==== G36 ==========================================================
  //      AK+     
  // +          Q+
  //      J+     
  // ==== G36 ==========================================================

  Trick trick[5];

  if (distHex == 0x4234)
  {
    if (completeList[QT_ACE][2] > completeList[QT_RHO][0] &&
        completeList[QT_ACE][3] > completeList[QT_RHO][1] &&
        completeList[QT_ACE][3] < completeList[QT_RHO][0] &&
        completeList[QT_ACE][3] > completeList[QT_PARD][2])
    {
      // AK86 / Q9 / JT3 / 7542.
      if (pickFlag) holdCtr[0x1360]++;
      unsigned r1 = HR(QT_ACE, 2);
      unsigned r2 = HR(QT_ACE, 3);
      rank = r2;
      trick[0].Set(QT_PARD, QT_ACE, SDS_JACK, 3);
      trick[1].Set(QT_BOTH, QT_ACE, SDS_KING, 2);
      trick[2].Set(QT_PARD, QT_ACE, r2, 2);
      trick[3].Set(QT_BOTH, QT_PARD, SDS_JACK, 3);
      trick[4].Set(QT_ACE, QT_ACE, r1, 1);
      return def.Set122(trick);
    }
  }

  if (length[QT_LHO] != 1)
    return false;

  if (length[QT_PARD] == 2)
  {
    if (length[QT_ACE] >= 3 && length[QT_RHO] >= 3)
    {
      if (htop.T == QT_RHO && htop.N == QT_ACE)
      {
        // AK9x / Q / Jx / Txx.
        if (pickFlag) holdCtr[0x1360]++;
        unsigned l = (length[QT_RHO] == 3 ? length[QT_ACE] : 3);
        rank = SDS_NINE;
        trick[0].Set(QT_BOTH, QT_ACE, SDS_KING, 2);
        trick[1].Set(QT_BOTH, QT_PARD, SDS_JACK, 2);
        trick[2].Set(QT_ACE, QT_ACE, SDS_VOID, l-2);
        trick[3].Set(QT_BOTH, QT_ACE, SDS_ACE, 1);
        trick[4].Set(QT_PARD, QT_ACE, SDS_NINE, l-1);
        return def.Set122(trick);
      }
      else if (htop.T == QT_RHO || (htop.T == QT_PARD && htop.N == QT_RHO))
      {
        // AKxx / Q / Jx / Txx.
        if (pickFlag) holdCtr[0x1360]++;
        unsigned l = (length[QT_RHO] == 3 ? length[QT_ACE] : 3);
        rank = SDS_JACK;
        trick[0].Set(QT_BOTH, QT_ACE, SDS_KING, 2);
        trick[1].Set(QT_BOTH, QT_PARD, rank, 2);
        trick[2].Set(QT_ACE, QT_ACE, SDS_VOID, l-2);
        return def.Set12(trick[0], trick[1], trick[2]);
      }
      else if (distHex == 0x5125 && 
        htop.T == QT_ACE && htop.N == QT_RHO && htop.E == QT_RHO)
      {
        // AKTxx / Q / Jx / 9xxxx.
        if (pickFlag) holdCtr[0x1361]++;
        rank = SDS_TEN;
        trick[0].Set(QT_BOTH, QT_ACE, SDS_TEN, 3);
        trick[1].Set(QT_BOTH, QT_PARD, SDS_JACK, 2);
        trick[2].Set(QT_ACE, QT_ACE, SDS_TEN, 2);
        return def.Set12(trick[0], trick[1], trick[2]);
      }
      else if (htop.T == QT_ACE && htop.N == QT_RHO && htop.E == QT_ACE &&
          length[QT_RHO] >= 4)
      {
        // AKT8 / Q / Jx / 9xxx.
        if (pickFlag) holdCtr[0x1361]++;
        unsigned l = (length[QT_RHO] == 4 ? length[QT_ACE] : 4);
        rank = SDS_EIGHT;
        trick[0].Set(QT_BOTH, QT_ACE, SDS_TEN, 3);
        trick[1].Set(QT_BOTH, QT_PARD, SDS_JACK, 2);
        trick[2].Set(QT_ACE, QT_ACE, SDS_TEN, l-2);
        trick[3].Set(QT_BOTH, QT_ACE, SDS_ACE, 1);
        trick[4].Set(QT_PARD, QT_ACE, SDS_EIGHT, l-1);
        return def.Set122(trick);
      }
      else if (distHex == 0x5125 &&
          htop.T == QT_ACE && htop.N == QT_ACE && htop.E == QT_RHO &&
          completeList[QT_ACE][4] > completeList[QT_PARD][1] &&
          completeList[QT_ACE][4] > completeList[QT_RHO][1])
      {
        // AKT97 / Q / J6 / 85432.
        if (pickFlag) holdCtr[0x1361]++;
        rank = HR(QT_ACE, 4);
        trick[0].Set(QT_BOTH, QT_ACE, SDS_NINE, 4);
        trick[1].Set(QT_BOTH, QT_PARD, SDS_JACK, 2);
        trick[2].Set(QT_ACE, QT_ACE, SDS_NINE, 3);
        trick[3].Set(QT_BOTH, QT_ACE, SDS_ACE, 1);
        trick[4].Set(QT_PARD, QT_ACE, rank, 4);
        return def.Set122(trick);
      }
    }
  }
  else if (length[QT_PARD] == 3)
  {
    if (length[QT_ACE] >= 4 && length[QT_RHO] >= 4)
    {
      if (htop.T == QT_PARD && htop.N == QT_RHO && htop.E == QT_ACE)
      {
        // AK8x / Q / JTx / 9xxx.
        if (pickFlag) holdCtr[0x1362]++;
        rank = SDS_EIGHT;
        unsigned l = (length[QT_RHO] == 4 ? length[QT_ACE] : 4);
        trick[0].Set(QT_BOTH, QT_ACE, rank, l);
        trick[1].Set(QT_ACE, QT_PARD, SDS_JACK, 3);
        trick[2].Set(QT_ACE, QT_PARD, SDS_TEN, 3);
        trick[3].Set(QT_ACE, QT_ACE, SDS_VOID, l-3);
        return def.Set112(trick);
      }
      // Why doesn't this work?  We don't get here.  13, 1a31ef
      /*
      else if (distHex == 0x4234 &&
          htop.T == QT_PARD && htop.N == QT_ACE && 
          htop.E == QT_RHO &&
          completeList[QT_ACE][3] > completeList[QT_PARD][2] &&
          completeList[QT_ACE][3] > completeList[QT_RHO][1])
      {
        // AK9x / Qx / JTx / 8xxx.
        if (pickFlag) holdCtr[0x1362]++;
        rank = HR(QT_ACE, 3);
        trick[0].Set(QT_PARD, QT_ACE, SDS_JACK, 3);
        trick[1].Set(QT_BOTH, QT_PARD, SDS_JACK, 3);
        trick[2].Set(QT_ACE, QT_ACE, SDS_NINE, 1);
        trick[3].Set(QT_BOTH, QT_ACE, SDS_KING, 2);
        trick[4].Set(QT_PARD, QT_ACE, rank, 2);
        return def.Set122(trick);
      }
      */
      else if (htop.T == QT_RHO && htop.N == QT_ACE)
      {
        if (htop.E != QT_PARD ||
            completeList[QT_RHO][1] > completeList[QT_ACE][3])
        {
          // AK9x / Q / Jxx / Txxx.
          if (pickFlag) holdCtr[0x1363]++;
          rank = SDS_NINE;
          unsigned l = (length[QT_RHO] == 4 ? length[QT_ACE] : 4);
          trick[0].Set(QT_BOTH, QT_ACE, rank, l);
          trick[1].Set(QT_ACE, QT_PARD, SDS_JACK, 3);
          return def.Set11(trick[0], trick[1]);
        }
      }
    }
  }
  else if (length[QT_ACE] == 3)
  {
    if (length[QT_PARD] >= 4 && length[QT_RHO] >= 4)
    {
      if (htop.T == QT_RHO && htop.N == QT_PARD && htop.E == QT_PARD)
      {
        // AKx / Q / J98xx / Txxx.
        // The way it comes out, or correct?
        if (pickFlag) holdCtr[0x1364]++;
        rank = SDS_EIGHT;
        unsigned l = (length[QT_RHO] == 4 ? length[QT_PARD] : 4);
        trick[0].Set(QT_BOTH, QT_BOTH, SDS_JACK, 3);
        trick[1].Set(QT_BOTH, QT_ACE, SDS_ACE, 1);
        trick[2].Set(QT_PARD, QT_ACE, rank, 2);
        trick[3].Set(QT_PARD, QT_PARD, SDS_VOID, l-3);
        return def.Set13(trick);
      }
      else if (htop.T == QT_RHO && htop.N == QT_PARD && 
          htop.E == QT_ACE &&
          completeList[QT_PARD][2] > completeList[QT_RHO][1])
      {
        // Should really depend on p2, I think. Or?
        if (pickFlag) holdCtr[0x1365]++;
        rank = HR(QT_ACE, 2);
        unsigned l = (length[QT_RHO] == 4 ? length[QT_PARD] : 4);
        trick[0].Set(QT_BOTH, QT_BOTH, SDS_JACK, 3);
        trick[1].Set(QT_BOTH, QT_ACE, SDS_ACE, 1);
        trick[2].Set(QT_PARD, QT_ACE, rank, 2);
        trick[3].Set(QT_PARD, QT_PARD, SDS_VOID, l-3);
        return def.Set13(trick);
      }
      else if (htop.T == QT_RHO && 
          completeList[QT_ACE][2] > completeList[QT_RHO][1])
      {
        // AK9 / Q / J876 / Txxx.
        // AK9 / Q / Jxxx / Txxx.
        if (pickFlag) holdCtr[0x1365]++;
        rank = HR(QT_ACE, 2);
        unsigned l = (length[QT_RHO] == 4 ? length[QT_PARD] : 4);
        trick[0].Set(QT_BOTH, QT_BOTH, SDS_JACK, 3);
        trick[1].Set(QT_BOTH, QT_ACE, SDS_ACE, 1);
        trick[2].Set(QT_PARD, QT_ACE, rank, 2);
        trick[3].Set(QT_PARD, QT_PARD, SDS_VOID, l-3);
        return def.Set13(trick);
      }
    }
  }
  else if (distHex == 0x4144)
  {
    if (htop.T == QT_RHO)
    {
      if (! hopp.N &&
          completeList[QT_PARD][2] > completeList[QT_RHO][1] &&
          completeList[QT_PARD][2] < completeList[QT_ACE][2] &&
          completeList[QT_PARD][2] > completeList[QT_ACE][3])
      {
        // AK8x / Q / J97x / Txxx.
        if (pickFlag) holdCtr[0x1366]++;
        unsigned r1 = HR(QT_ACE, 2);
        rank = HR(QT_PARD, 2);
        trick[0].Set(QT_BOTH, QT_ACE, r1, 4);
        trick[1].Set(QT_ACE, QT_PARD, rank, 4);
        return def.Set11(trick[0], trick[1]);
      }
      else
      {
        // AK9x / Q / Jxxx / Txxx.
        // AK8x / Q / Jxxx / 9xxx.
        if (pickFlag) holdCtr[0x1367]++;
        rank = HR(QT_ACE, 2);
        trick[0].Set(QT_BOTH, QT_ACE, rank, 4);
        trick[1].Set(QT_ACE, QT_PARD, SDS_JACK, 3);
        return def.Set11(trick[0], trick[1]);
      }
    }
  }

  return false;
}


bool LoopHold::SolveComplex41(DefList& def, unsigned& rank) const
{
  // ==== G41 ==========================================================
  //      A+      
  // +          K+
  //      QJ+    
  // ==== G41 ==========================================================

  Trick trick[4];
  unsigned l;
  if (length[QT_ACE] >= 4 && length[QT_PARD] == 3 &&
      length[QT_LHO] == 1)
  {
    if (length[QT_RHO] >= 4)
    {
      if (htop.T == QT_ACE && htop.N == QT_RHO && htop.E == QT_ACE)
      {
        // AT8x / K / QJx / 9xxx.
        if (pickFlag) holdCtr[0x1410]++;
        rank = SDS_EIGHT;
        l = (length[QT_RHO] == 4 ? length[QT_ACE] : 4);
        trick[0].Set(QT_BOTH, QT_ACE, rank, l);
        trick[1].Set(QT_ACE, QT_PARD, SDS_JACK, 3);
        trick[2].Set(QT_ACE, QT_ACE, SDS_TEN, l-3);
        return def.Set12(trick[0], trick[1], trick[2]);
      }
    }
  }
  else if (length[QT_ACE] >= 3 && length[QT_PARD] == 2 && 
      length[QT_RHO] >= 3)
  {
    if (htop.N == QT_RHO || length[QT_ACE] == 3 || length[QT_RHO] == 3)
    {
      // ATx / K / QJ / 9xx.
      if (pickFlag) holdCtr[0x1411]++;
      l = (length[QT_RHO] == 3 ? length[QT_ACE] : 3);
      rank = SDS_TEN;
      trick[0].Set(QT_PARD, QT_ACE, SDS_QUEEN, 2);
      trick[1].Set(QT_BOTH, QT_PARD, SDS_QUEEN, 2);
      trick[2].Set(QT_ACE, QT_ACE, rank, l-2);
      return def.Set12(trick[0], trick[1], trick[2]);
    }
    else if (htop.E == QT_RHO || 
        length[QT_ACE] == 4 || length[QT_RHO] == 4)
    {
      // AT9x / K / QJ / 8xxx.
      if (pickFlag) holdCtr[0x1412]++;
      l = (length[QT_RHO] <= 4 ? length[QT_ACE] : 4);
      rank = SDS_NINE;
      trick[0].Set(QT_PARD, QT_ACE, SDS_QUEEN, 2);
      trick[1].Set(QT_BOTH, QT_PARD, SDS_QUEEN, 2);
      trick[2].Set(QT_ACE, QT_ACE, rank, l-2);
      return def.Set12(trick[0], trick[1], trick[2]);
    }
  }
  else if (distHex == 0x2155)
  {
    if (htop.T == QT_ACE && htop.N == QT_RHO)
    {
      // AT / K / QJ754 / 98632.
      if (pickFlag) holdCtr[0x1413]++;
      rank = SDS_TEN;
      trick[0].Set(QT_BOTH, QT_BOTH, SDS_JACK, 3);
      trick[1].Set(QT_BOTH, QT_ACE, rank, 2);
      trick[2].Set(QT_PARD, QT_PARD, SDS_VOID, 2);
      return def.Set12(trick[0], trick[1], trick[2]);
    }
  }

  return false;
}


bool LoopHold::SolveComplex43(DefList& def, unsigned& rank) const
{
  // ==== G43 ==========================================================
  //      A+      
  // K+         + 
  //      QJ+    
  // ==== G43 ==========================================================

  if (pickFlag) holdCtr[0x1430]++;

  Trick trick[4];
  unsigned l;
  if (length[QT_ACE] >= 3 && length[QT_PARD] == 2 && length[QT_LHO] >= 3)
  {
    if (length[QT_RHO] == 1)
    {
      assert(htop.T == QT_ACE);
      if (htop.N == QT_LHO || length[QT_ACE] == 3 || length[QT_LHO] == 3)
      {
        // ATx / 9xx / QJ / K.
        if (pickFlag) holdCtr[0x1431]++;
        l = (length[QT_LHO] == 3 ? length[QT_ACE] : 3);
        rank = SDS_TEN;
        trick[0].Set(QT_PARD, QT_ACE, SDS_QUEEN, 2);
        trick[1].Set(QT_BOTH, QT_PARD, SDS_QUEEN, 2);
        trick[2].Set(QT_ACE, QT_ACE, rank, l-2);
        return def.Set12(trick[0], trick[1], trick[2]);
      }
      else if (htop.E == QT_LHO || 
          length[QT_ACE] == 4 || length[QT_LHO] == 4)
      {
        // AT9x / 8xxx / QJ / K.
        if (pickFlag) holdCtr[0x1432]++;
        l = (length[QT_LHO] <= 4 ? length[QT_ACE] : 4);
        rank = SDS_NINE;
        trick[0].Set(QT_PARD, QT_ACE, SDS_QUEEN, 2);
        trick[1].Set(QT_BOTH, QT_PARD, SDS_QUEEN, 2);
        trick[2].Set(QT_ACE, QT_ACE, rank, l-2);
        return def.Set12(trick[0], trick[1], trick[2]);
      }
    }
  }

  return false;
}


bool LoopHold::SolveComplex44(DefList& def, unsigned& rank) const
{
  // ==== G44 ==========================================================
  //      AK+     
  // Q+         + 
  //      J+    
  // ==== G44 ==========================================================

  if (pickFlag) holdCtr[0x1440]++;

  Trick trick[5];
  unsigned l;
  if (length[QT_ACE] > length[QT_PARD] && length[QT_PARD] == 2 &&
      length[QT_RHO] == 2 && length[QT_LHO] >= 3)
  {
    if (htop.T == QT_PARD ||
       (htop.T == QT_ACE && htop.N == QT_PARD))
    {
      if (completeList[QT_LHO][0] > completeList[QT_ACE][2])
      {
        // AKx+ / 9xx / JT / Qx.
        if (pickFlag) holdCtr[0x1441]++;
        rank = SDS_TEN;
        l = (length[QT_LHO] == 3 ? length[QT_ACE]-2 : 1);
        trick[0].Set(QT_BOTH, QT_ACE, SDS_KING, 2);
        trick[1].Set(QT_PARD, QT_PARD, rank, 2);
        trick[2].Set(QT_ACE, QT_ACE, SDS_VOID, l);
        return def.Set12(trick[0], trick[1], trick[2]);
      }
      else if (length[QT_ACE] >= 4 && length[QT_LHO] >= 4 &&
          completeList[QT_LHO][0] > completeList[QT_ACE][3])
      {
        // AK9x /  8xxx / JT / Qx.
        // AKTx / 8xxx / J9 / Qx.
        if (pickFlag) holdCtr[0x1442]++;
        l = (length[QT_LHO] == 4 ? length[QT_ACE] : 4);
        unsigned r1 = HR(QT_ACE, 2);
        unsigned r2 = HR(QT_PARD, 1);
        unsigned r3 = (r1 < r2 ? r1 : SDS_VOID);
        rank = Min(r1, r2);
        trick[0].Set(QT_BOTH, QT_ACE, r1, 3);
        trick[1].Set(QT_PARD, QT_PARD, r2, 2);
        trick[2].Set(QT_ACE, QT_ACE, r3, l-2);
        return def.Set12(trick[0], trick[1], trick[2]);
      }
      else
      {
        l = Min(length[QT_ACE], length[QT_LHO]);
        if (completeList[QT_ACE][l-1] > completeList[QT_LHO][0] &&
           (htop.T == QT_PARD || l >= 4))
        {
          // AK985 / 743 / JT / Q6.
          if (pickFlag) holdCtr[0x1442]++;
          unsigned r1 = HR(QT_ACE, l-1);
          unsigned r2 = HR(QT_ACE, l-2);
          unsigned r3 = HR(QT_PARD, 1);
          unsigned r4 = (r2 < r3 ? r2 : SDS_VOID);
          rank = Min(r1, r4);
          trick[0].Set(QT_BOTH, QT_ACE, r1, length[QT_ACE]);
          trick[1].Set(QT_PARD, QT_PARD, r3, 2);
          trick[2].Set(QT_ACE, QT_ACE, r4, length[QT_ACE]-2);
          return def.Set12(trick[0], trick[1], trick[2]);
        }
      }
    }
  }
  else if (length[QT_ACE] == 3 && length[QT_PARD] >= 4 &&
      length[QT_RHO] <= 2 && length[QT_LHO] >= 4)
  {
    if (completeList[QT_PARD][1] > completeList[QT_ACE][2] &&
        completeList[QT_PARD][1] > completeList[QT_LHO][1] &&
        completeList[QT_LHO][0] > completeList[QT_PARD][1])
    {
      // AKx / Txxx / J9xx / Q.
      if (pickFlag) holdCtr[0x1443]++;
      l = (length[QT_LHO] == 4 ? length[QT_PARD] : 4);
      rank = HR(QT_PARD, 1);
      trick[0].Set(QT_BOTH, QT_BOTH, rank, l);
      return def.Set1(trick[0]);
    }
  }
  else if (length[QT_ACE] >= 4 && length[QT_PARD] == 2 &&
      length[QT_RHO] == 1 && length[QT_LHO] >= 3)
  {
    if (htop.T == QT_LHO || 
       (htop.T == QT_PARD && htop.N == QT_LHO))
    {
      // AKxx / Txx / Jx / Q.
      if (pickFlag) holdCtr[0x1444]++;
      l = (length[QT_LHO] == 3 ? length[QT_ACE]-2 : 1);
      rank = SDS_JACK;
      trick[0].Set(QT_BOTH, QT_ACE, SDS_KING, 2);
      trick[1].Set(QT_BOTH, QT_PARD, rank, 2);
      trick[2].Set(QT_ACE, QT_ACE, SDS_VOID, l);
      return def.Set12(trick[0], trick[1], trick[2]);
    }
  }
  else if (length[QT_ACE] > length[QT_PARD] && length[QT_PARD] == 3 &&
      length[QT_RHO] == 2)
  {
    if (length[QT_LHO] == 4)
    {
      // 4=4=3=2.
      if (! hopp.T && (! hopp.N || htop.T == QT_PARD) && 
        completeList[QT_PARD][1] > Max(completeList[QT_ACE][3],
          completeList[QT_LHO][0]) &&
        completeList[QT_PARD][2] < completeList[QT_ACE][2] &&
        completeList[QT_ACE][3] > completeList[QT_LHO][0])
      {
        // AKT8 / 7654 / J93 / Q2, 
        // AK98 / 7652 / JT3 / Q4.
        if (pickFlag) holdCtr[0x1445]++;
        rank = HR(QT_ACE, 3);
        unsigned ra = HR(QT_PARD, 1);
        unsigned rb = HR(QT_ACE, 2);
        unsigned r1 = Min(ra, rb);
        unsigned r2 = HR(QT_ACE, 2);
        // The way it comes out for now.
        PosType p = (htop.T == QT_ACE ? QT_BOTH : QT_ACE);
        trick[0].Set(QT_ACE, QT_ACE, rank, 4);
        trick[1].Set(QT_PARD, QT_BOTH, r1, 4);
        trick[2].Set(p, QT_PARD, SDS_JACK, 3);
        trick[3].Set(QT_ACE, QT_ACE, r2, 1);
        return def.Set112(trick);
      }
      else if (completeList[QT_PARD][1] > completeList[QT_RHO][1] &&
          completeList[QT_PARD][1] > completeList[QT_LHO][0] &&
          completeList[QT_PARD][1] > completeList[QT_ACE][3] &&
          completeList[QT_ACE][2] > completeList[QT_LHO][0])
      {
        if (htop.T == QT_ACE)
        {
          if (pickFlag) holdCtr[0x1446]++;
          unsigned r1 = HR(QT_ACE, 2);
          unsigned r2 = HR(QT_PARD, 1);
          rank = Min(r1, r2);
          // The way it comes out for now.
          PosType p = (htop.T == QT_ACE ? QT_BOTH : QT_ACE);
          trick[0].Set(QT_PARD, QT_BOTH, rank, 4);
          trick[1].Set(p, QT_PARD, SDS_JACK, 3);
          trick[2].Set(QT_ACE, QT_ACE, r1, 1);
          return def.Set12(trick[0], trick[1], trick[2]);
        }
        else
        {
          if (pickFlag) holdCtr[0x1446]++;
          unsigned r1 = HR(QT_ACE, 2);
          unsigned r2 = HR(QT_PARD, 1);
          rank = Min(r1, r2);
          // The way it comes out for now.
          PosType p = (htop.T == QT_ACE ? QT_BOTH : QT_ACE);
          trick[0].Set(QT_PARD, QT_BOTH, rank, 4);
          trick[1].Set(QT_ACE, QT_ACE, SDS_JACK, 3);
          trick[2].Set(p, QT_PARD, SDS_JACK, 3);
          trick[3].Set(QT_ACE, QT_ACE, r1, 1);
          return def.Set112(trick);
        }
      }
      else if (htop.T == QT_PARD)
      {
        if (completeList[QT_PARD][2] > completeList[QT_ACE][2])
        {
          // Partially the way it comes out.
          if (htop.E == QT_PARD)
          {
            // AKxx / 9xxx / JT8 / Qx.
            // AKxx / xxxx / JT8 / Q9.  Should be 3-2-1-1, though?
            if (pickFlag) holdCtr[0x1447]++;
            rank = HR(QT_PARD, 2);
            trick[0].Set(QT_BOTH, QT_BOTH, SDS_JACK, 3);
            trick[1].Set(QT_PARD, QT_PARD, rank, 3);
            trick[2].Set(QT_ACE, QT_ACE, SDS_VOID, 1);
            return def.Set12(trick[0], trick[1], trick[2]);
          }
          else if (htop.N == QT_PARD)
          {
            // AKxx / 8xxx / JT9 / Qx.
            if (pickFlag) holdCtr[0x1447]++;
            rank = HR(QT_PARD, 2);
            trick[0].Set(QT_BOTH, QT_BOTH, SDS_JACK, 3);
            trick[1].Set(QT_PARD, QT_PARD, rank, 3);
            trick[2].Set(QT_ACE, QT_ACE, SDS_VOID, 1);
            return def.Set12(trick[0], trick[1], trick[2]);
          }
        }
        else if (htop.N == QT_LHO && htop.E == QT_ACE)
        {
          if (completeList[QT_ACE][3] > completeList[QT_LHO][1] &&
              (completeList[QT_ACE][3] > completeList[QT_PARD][2] ||
               completeList[QT_ACE][3] < completeList[QT_RHO][1]))
          {
            // AK86 / 9543 / JT7 / Q2.
            // AK86 / 9654 / JT3 / Q7.
            if (pickFlag) holdCtr[0x1448]++;
            unsigned r1 = HR(QT_ACE, 3);
            unsigned r2 = HR(QT_PARD, 2);
            rank = Max(r1, r2);
            trick[0].Set(QT_BOTH, QT_BOTH, SDS_JACK, 3);
            trick[1].Set(QT_PARD, QT_PARD, rank, 3);
            trick[2].Set(QT_ACE, QT_ACE, SDS_VOID, 1);
            return def.Set12(trick[0], trick[1], trick[2]);
          }
          else
          {
            // AK8x / 9xxx / JT7 / Qx.
            if (pickFlag) holdCtr[0x1449]++;
            rank = HR(QT_PARD, 2);
            trick[0].Set(QT_BOTH, QT_BOTH, SDS_JACK, 3);
            trick[1].Set(QT_PARD, QT_PARD, rank, 3);
            trick[2].Set(QT_ACE, QT_ACE, SDS_VOID, 1);
            return def.Set12(trick[0], trick[1], trick[2]);
          }
        }
      }
    }
    else if ((htop.T == QT_ACE ||
        (htop.T == QT_LHO && htop.N == QT_ACE && length[QT_LHO] == 1)) &&
        completeList[QT_ACE][2] > completeList[QT_PARD][1] &&
        completeList[QT_PARD][1] > completeList[QT_RHO][1] &&
        completeList[QT_PARD][1] > completeList[QT_ACE][length[QT_ACE]-1]
          && (length[QT_LHO] == 1 || 
            completeList[QT_PARD][1] > completeList[QT_LHO][0]))
    {
      if (pickFlag) holdCtr[0x144a]++;
      rank = HR(QT_ACE, 2);
      trick[0].Set(QT_BOTH, QT_ACE, rank, length[QT_ACE]);
      trick[1].Set(QT_ACE, QT_PARD, SDS_JACK, 3);
      trick[2].Set(QT_ACE, QT_ACE, SDS_VOID, length[QT_ACE]-3);
      return def.Set12(trick[0], trick[1], trick[2]);
    }
    else if (htop.T == QT_PARD ||
        (htop.T == QT_LHO && htop.N == QT_PARD && length[QT_LHO] == 1))
    {
      if (completeList[QT_ACE][2] > completeList[QT_PARD][2] &&
         (length[QT_LHO] <= 2 ||
          completeList[QT_ACE][2] > completeList[QT_LHO][0]))
      {
        // AK8x / T / J9x / Qx.
        if (pickFlag) holdCtr[0x144b]++;
        rank = HR(QT_ACE, 2);
        unsigned r2 = HR(QT_PARD, 1);
        trick[0].Set(QT_ACE, QT_ACE, rank, length[QT_ACE]);
        trick[1].Set(QT_PARD, QT_BOTH, r2, length[QT_ACE]);
        trick[2].Set(QT_BOTH, QT_PARD, SDS_JACK, 3);
        trick[3].Set(QT_ACE, QT_ACE, SDS_VOID, length[QT_ACE]-3);
        return def.Set112(trick);
      }
      else
      {
        // AKxx(+) / (xxx) / JT9 / Qx.
        // AK7x / 8xx / JTx / Q9.
        if (pickFlag) holdCtr[0x144c]++;
        rank = HR(QT_PARD, 1);
        // trick[0].Set(QT_BOTH, QT_ACE, SDS_KING, 2);
        // trick[1].Set(QT_BOTH, QT_PARD, SDS_JACK, 1);
        // trick[2].Set(QT_ACE, QT_ACE, SDS_VOID, length[QT_ACE]-3);
        // trick[3].Set(QT_PARD, QT_BOTH, rank, length[QT_ACE]);
        // return def.Set31(trick);
        trick[0].Set(QT_PARD, QT_BOTH, rank, length[QT_ACE]);
        trick[1].Set(QT_BOTH, QT_PARD, SDS_JACK, 3);
        trick[2].Set(QT_ACE, QT_ACE, SDS_VOID, length[QT_ACE]-3);
        return def.Set12(trick[0], trick[1], trick[2]);
      }
    }
  }
  else if (length[QT_ACE] > length[QT_PARD] && length[QT_PARD] >= 4 &&
      length[QT_RHO] == 2)
  {
    if (htop.T == QT_PARD ||
       (htop.T == QT_LHO && length[QT_LHO] == 1 && htop.N == QT_PARD))
    {
      if (completeList[QT_PARD][length[QT_PARD]-1] > 
          completeList[QT_ACE][2])
      {
        // AKxxx(+) / (xx) / JT98 / Qx.
        if (pickFlag) holdCtr[0x144d]++;
        rank = HR(QT_PARD, length[QT_PARD]-3);
        unsigned rank2 = HR(QT_PARD, 1);
        // trick[0].Set(QT_ACE, QT_ACE, SDS_KING, 2);
        // trick[1].Set(QT_BOTH, QT_PARD, rank, length[QT_PARD] - 2);
        // trick[2].Set(QT_ACE, QT_ACE, SDS_VOID, 
          // length[QT_ACE] - length[QT_PARD]);
        // trick[3].Set(QT_PARD, QT_BOTH, rank2, length[QT_ACE]);
        // return def.Set31(trick);
        trick[0].Set(QT_PARD, QT_BOTH, rank2, length[QT_ACE]);
        trick[1].Set(QT_ACE, QT_ACE, rank, length[QT_PARD]);
        trick[2].Set(QT_ACE, QT_PARD, rank, length[QT_PARD]);
        trick[3].Set(QT_ACE, QT_ACE, SDS_VOID, 
          length[QT_ACE] - length[QT_PARD]);
        return def.Set112(trick);
      }
      else
      {
        // AK8xx / - / JT9x / Qx.
        if (pickFlag) holdCtr[0x144e]++;
        rank = HR(QT_ACE, 2);
        unsigned r2 = HR(QT_PARD, 1);
        trick[0].Set(QT_ACE, QT_BOTH, rank, length[QT_ACE]);
        trick[1].Set(QT_PARD, QT_BOTH, r2, length[QT_ACE]);
        return def.Set11(trick[0], trick[1]);
      }
    }
  }
  else if (length[QT_ACE] >= 4 && length[QT_PARD] == 3 &&
      length[QT_RHO] == 1 && length[QT_LHO] >= 4)
  {
    if (htop.T == QT_LHO && htop.N == QT_ACE && htop.E == QT_ACE)
    {
      // AK983 / T654 / J72 / Q.
      if (pickFlag) holdCtr[0x144f]++;
      rank = HR(QT_ACE, 3);
      l = (length[QT_LHO] == 4 ? length[QT_ACE] : 4);
      trick[0].Set(QT_BOTH, QT_BOTH, SDS_JACK, 3);
      trick[1].Set(QT_BOTH, QT_PARD, rank, 3);
      trick[2].Set(QT_ACE, QT_ACE, SDS_VOID, l-3);
      return def.Set12(trick[0], trick[1], trick[2]);
    }
  }

  return false;
}


bool LoopHold::SolveComplex48(DefList& def, unsigned& rank) const
{
  // ==== G48 ==========================================================
  //      AKQ+    
  // J+         + 
  //      +     
  // ==== G48 ==========================================================

  if (pickFlag) holdCtr[0x1480]++;

  Trick trick[5];
  if (length[QT_ACE] >= 5 && length[QT_PARD] == 4 && length[QT_RHO] == 3)
  {
    unsigned a3 = completeList[QT_ACE][3];
    unsigned p2 = completeList[QT_PARD][2];
    unsigned p3 = completeList[QT_PARD][3];
    unsigned l0 = (length[QT_LHO] == 0 ? 0 : completeList[QT_LHO][0]);
    unsigned r2 = completeList[QT_RHO][2];

    if (htop.T == QT_RHO)
    {
      // if (p2 > a3 && a3 > p3 && p2 > r2 && p2 > l0)
      if (p2 > a3 && p2 > r2)
      {
        if (a3 > p3)
        {
          // AKQ6x / (x) / 987x / JTx.
          if (pickFlag) holdCtr[0x1481]++;
          rank = HR(QT_ACE, 3);
          trick[0].Set(QT_BOTH, QT_ACE, rank, length[QT_ACE]);
          trick[1].Set(QT_ACE, QT_PARD, HR(QT_PARD, 0), 4);
          trick[2].Set(QT_ACE, QT_ACE, SDS_VOID, length[QT_ACE]-4);
          trick[3].Set(QT_PARD, QT_ACE, SDS_ACE, 1);
          trick[4].Set(QT_PARD, QT_BOTH, HR(QT_PARD, 2), length[QT_ACE]-1);
          return def.Set122(trick);
        }
        else
        {
          // AKQ43 / - / 9875 / JT6.
          if (pickFlag) holdCtr[0x1481]++;
          unsigned ra0 = HR(QT_PARD, 0);
          rank = HR(QT_PARD, 2);
          trick[0].Set(QT_PARD, QT_ACE, ra0, 4);
          trick[1].Set(QT_BOTH, QT_PARD, ra0, 4);
          trick[2].Set(QT_ACE, QT_ACE, SDS_VOID, length[QT_ACE]-4);
          trick[3].Set(QT_PARD, QT_ACE, SDS_ACE, 1);
          trick[4].Set(QT_PARD, QT_BOTH, rank, length[QT_ACE]-1);
          return def.Set122(trick);
        }
      }
    }
    else if (htop.T == QT_LHO)
    {
      // For now
      return false;
    }
    else if (htop.T == QT_ACE)
    {
      // For now
      return false;
    }
    else
    if (htop.N == QT_PARD || (htop.N == QT_LHO && htop.E == QT_PARD))
    {
      if (p3 > a3)
      {
        // AKQxx / (x) / T9xx / Jxx.
        if (pickFlag) holdCtr[0x1481]++;
        rank = HR(QT_PARD, 1);
        // trick[0].Set(QT_PARD, QT_BOTH, rank, length[QT_ACE]);
        // trick[1].Set(QT_BOTH, QT_ACE, SDS_QUEEN, 3);
        // trick[2].Set(QT_BOTH, QT_PARD, SDS_TEN, 1);
        // trick[3].Set(QT_ACE, QT_ACE, SDS_VOID, length[QT_ACE] - 4);
        // return def.Set13(trick);
        trick[0].Set(QT_PARD, QT_BOTH, rank, length[QT_ACE]);
        trick[1].Set(QT_BOTH, QT_PARD, SDS_TEN, 4);
        trick[2].Set(QT_ACE, QT_ACE, SDS_VOID, length[QT_ACE] - 4);
        return def.Set12(trick[0], trick[1], trick[2]);
      }
      else
      {
        if (pickFlag) holdCtr[0x1482]++;
        rank = HR(QT_ACE, 3);
        unsigned r = HR(QT_PARD, 1);
        trick[0].Set(QT_ACE, QT_ACE, rank, length[QT_ACE]);
        trick[1].Set(QT_PARD, QT_BOTH, r, length[QT_ACE]);
        trick[2].Set(QT_BOTH, QT_PARD, SDS_TEN, 4);
        trick[3].Set(QT_ACE, QT_ACE, SDS_VOID, length[QT_ACE]-4);
        return def.Set112(trick);
      }
    }
    else if (htop.N == QT_ACE || (htop.N == QT_LHO && htop.E == QT_ACE))
    {
      if (pickFlag) holdCtr[0x1483]++;
      rank = HR(QT_ACE, 3);
      trick[0].Set(QT_BOTH, QT_ACE, rank, length[QT_ACE]);
      trick[1].Set(QT_ACE, QT_PARD, SDS_TEN, 4);
      trick[2].Set(QT_ACE, QT_ACE, SDS_VOID, length[QT_ACE]-4);
      return def.Set12(trick[0], trick[1], trick[2]);
    }
    else if (p2 > r2 && p2 > a3 && completeList[QT_PARD][0] > l0)
    {
      if (p3 < a3)
      {
        if (pickFlag) holdCtr[0x1483]++;
        rank = HR(QT_PARD, 2);
        trick[0].Set(QT_BOTH, QT_ACE, HR(QT_ACE, 3), length[QT_ACE]);
        trick[1].Set(QT_ACE, QT_PARD, SDS_TEN, 4);
        trick[2].Set(QT_ACE, QT_ACE, SDS_VOID, length[QT_ACE]-4);
        trick[3].Set(QT_PARD, QT_ACE, SDS_ACE, 1);
        trick[4].Set(QT_PARD, QT_BOTH, rank, length[QT_ACE]-1);
        return def.Set122(trick);
      }
      else
      {
        if (pickFlag) holdCtr[0x1483]++;
        rank = HR(QT_PARD, 2);
        trick[0].Set(QT_PARD, QT_ACE, SDS_TEN, 4);
        trick[1].Set(QT_BOTH, QT_PARD, SDS_TEN, 4);
        trick[2].Set(QT_ACE, QT_ACE, SDS_VOID, length[QT_ACE]-4);
        trick[3].Set(QT_PARD, QT_ACE, SDS_ACE, 1);
        trick[4].Set(QT_PARD, QT_BOTH, rank, length[QT_ACE]-1);
        return def.Set122(trick);
      }
    }
  }
  else if (distHex == 0x4432 && htop.T == QT_PARD && htop.N == QT_PARD)
  {
    if (completeList[QT_ACE][3] > completeList[QT_LHO][0])
    {
      // AKQ8 / 7654 / T93 / J2.
      if (pickFlag) holdCtr[0x1484]++;
      rank = HR(QT_ACE, 3);
      trick[0].Set(QT_ACE, QT_ACE, rank, 4);
      trick[1].Set(QT_PARD, QT_BOTH, SDS_NINE, 4);
      trick[2].Set(QT_BOTH, QT_PARD, SDS_TEN, 3);
      trick[3].Set(QT_ACE, QT_ACE, SDS_VOID, 1);
      return def.Set112(trick);
    }
    else
    {
      // AKQ7 / 8654 / T93 / J2.
      if (pickFlag) holdCtr[0x1485]++;
      rank = SDS_NINE;
      trick[0].Set(QT_PARD, QT_BOTH, rank, 4);
      trick[1].Set(QT_BOTH, QT_PARD, SDS_TEN, 3);
      trick[2].Set(QT_ACE, QT_ACE, SDS_VOID, 1);
      return def.Set12(trick[0], trick[1], trick[2]);
      // trick[0].Set(QT_PARD, QT_BOTH, rank, 4);
      // trick[1].Set(QT_BOTH, QT_ACE, SDS_ACE, 1);
      // trick[2].Set(QT_BOTH, QT_PARD, SDS_TEN, 2);
      // trick[3].Set(QT_ACE, QT_ACE, SDS_VOID, 1);
      // return def.Set13(trick);
    }
  }
  else if (length[QT_ACE] >= 4 && length[QT_PARD] == 2 &&
      length[QT_LHO] >= 4 && 
      (length[QT_RHO] == 2 || length[QT_RHO] == 3))
  {
    if (htop.T == QT_PARD && htop.N == QT_PARD)
    {
      if (completeList[QT_ACE][3] < completeList[QT_LHO][0])
      {
        // AKQx / 8xxx / T9 / Jx.
        if (pickFlag) holdCtr[0x1486]++;
        rank = SDS_NINE;
        unsigned l = (length[QT_LHO] == 4 ? length[QT_ACE] : 4);
        trick[0].Set(QT_BOTH, QT_ACE, SDS_QUEEN, 3);
        trick[1].Set(QT_PARD, QT_PARD, rank, 2);
        trick[2].Set(QT_ACE, QT_ACE, SDS_VOID, l-2);
        return def.Set12(trick[0], trick[1], trick[2]);
      }
      else
      {
        // AKQ8 / 7654 / T9 / J3.
        if (pickFlag) holdCtr[0x1487]++;
        rank = HR(QT_ACE, 3);
        trick[0].Set(QT_BOTH, QT_ACE, rank, length[QT_ACE]);
        trick[1].Set(QT_PARD, QT_PARD, SDS_NINE, 2);
        trick[2].Set(QT_ACE, QT_ACE, SDS_VOID, length[QT_ACE]-2);
        return def.Set12(trick[0], trick[1], trick[2]);
      }
    }
  }
  return false;
}


bool LoopHold::SolveComplex49(DefList& def, unsigned& rank) const
{
  // ==== G49 ==========================================================
  //      AQ+     
  // J+         K+
  //      +     
  // ==== G49 ==========================================================

  if (pickFlag) holdCtr[0x1490]++;
  UNUSED(def);
  UNUSED(rank);
  return false;
}


bool LoopHold::SolveComplex60(DefList& def, unsigned& rank) const
{
  // ==== G60 ==========================================================
  //      AK+                      
  // QJ+        +                  
  //      +                        
  // ==== G60 ==========================================================

  if (pickFlag) holdCtr[0x1600]++;

  UNUSED(def);
  UNUSED(rank);
  return false;
}


bool LoopHold::SolveComplex62(DefList& def, unsigned& rank) const
{
  // ==== G62 ==========================================================
  //      A+                      
  // QJ+        +                  
  //      K+                        
  // ==== G62 ==========================================================

  if (distHex != 0x3442)
    return false;

  Trick trick[4];
  if ((htop.T == QT_ACE && htop.N == QT_PARD) ||
      (htop.T == QT_PARD && htop.N == QT_ACE))
  {
    if (htop.E == QT_LHO &&
        completeList[QT_PARD][2] > completeList[QT_LHO][1] &&
        completeList[QT_PARD][2] > completeList[QT_ACE][2])
    {
      // ATx / 8xxx / K97x / Qx.
      if (pickFlag) holdCtr[0x1620]++;

      rank = SDS_VOID-8;
      unsigned r1 = HR(QT_ACE, 1);
      unsigned r2 = HR(QT_PARD, 1);
      if (r2 > r1)
        r2 = SDS_VOID;
      /*
      if (r2 > r1)
      {
        r2 = SDS_VOID;
        trick[0].Set(QT_BOTH, QT_PARD, rank, 4);
        trick[1].Set(QT_PARD, QT_ACE, HR(QT_PARD, 1), 3);
        trick[2].Set(QT_PARD, QT_ACE, r1, 3);
        trick[3].Set(QT_PARD, QT_PARD, r2, 1);
        return def.Set112(trick);
      }
      else
      {
      */
        trick[0].Set(QT_BOTH, QT_PARD, rank, 4);
        trick[1].Set(QT_PARD, QT_ACE, r1, 3);
        trick[2].Set(QT_PARD, QT_PARD, r2, 1);
        return def.Set12(trick[0], trick[1], trick[2]);
      // }
    }
  }
  else if (htop.T == QT_PARD && htop.N == QT_LHO &&
      completeList[QT_PARD][2] > completeList[QT_LHO][1] &&
      completeList[QT_ACE][2] < completeList[QT_PARD][2])
  {
    // A52 / 9643 / KT87 / QJ.
    // The defense doesn't matter.  If declarer unblocks
    // the 8 from A82 / 9643 / KT75 / QJ, he can always get BP47.
    // If he doesn't, LHO can still cover and hold him to BP47.
    // Or he can duck.  Declarer only permits this when he
    // really wants to be in North.  And so the defender should
    // always cover anyway.

    if (pickFlag) holdCtr[0x1621]++;
    rank = HR(QT_PARD, 2);
    trick[0].Set(QT_BOTH, QT_PARD, rank, 4);
    trick[1].Set(QT_PARD, QT_ACE, SDS_TEN, 3);
    return def.Set11(trick[0], trick[1]);
  }

  return false;
}


bool LoopHold::SolveUnused(DefList& def, unsigned& rank) const
{
  UNUSED(def);
  UNUSED(rank);
  return false;
}


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

  SolveComplexFunction[10] = &LoopHold::SolveComplex2;
  SolveComplexFunction[11] = &LoopHold::SolveComplex26;
  SolveComplexFunction[12] = &LoopHold::SolveComplex12;
  SolveComplexFunction[13] = &LoopHold::SolveComplex28;
  SolveComplexFunction[14] = &LoopHold::SolveComplex44;
  SolveComplexFunction[15] = &LoopHold::SolveComplex60;
  SolveComplexFunction[16] = &LoopHold::SolveComplex1;
  SolveComplexFunction[17] = &LoopHold::SolveComplex15;
  SolveComplexFunction[18] = &LoopHold::SolveComplex9;
  SolveComplexFunction[19] = &LoopHold::SolveComplex49;

  SolveComplexFunction[20] = &LoopHold::SolveComplex5;
  SolveComplexFunction[21] = &LoopHold::SolveComplex5;
  SolveComplexFunction[22] = &LoopHold::SolveComplex5;
  SolveComplexFunction[23] = &LoopHold::SolveComplex5;
  SolveComplexFunction[24] = &LoopHold::SolveComplex9;
  SolveComplexFunction[25] = &LoopHold::SolveComplex15;
  SolveComplexFunction[26] = &LoopHold::SolveComplex41;
  SolveComplexFunction[27] = &LoopHold::SolveComplex57;
  SolveComplexFunction[28] = &LoopHold::SolveComplex7;
  SolveComplexFunction[29] = &LoopHold::SolveComplex15;

  SolveComplexFunction[30] = &LoopHold::SolveComplex7;
  SolveComplexFunction[31] = &LoopHold::SolveComplex15;
  SolveComplexFunction[32] = &LoopHold::SolveComplex2;
  SolveComplexFunction[33] = &LoopHold::SolveComplex18;
  SolveComplexFunction[34] = &LoopHold::SolveComplex2;
  SolveComplexFunction[35] = &LoopHold::SolveComplex26;
  SolveComplexFunction[36] = &LoopHold::SolveComplex6;
  SolveComplexFunction[37] = &LoopHold::SolveComplex20;
  SolveComplexFunction[38] = &LoopHold::SolveComplex14;
  SolveComplexFunction[39] = &LoopHold::SolveComplex28;

  SolveComplexFunction[40] = &LoopHold::SolveComplex2;
  SolveComplexFunction[41] = &LoopHold::SolveComplex26;
  SolveComplexFunction[42] = &LoopHold::SolveComplex2;
  SolveComplexFunction[43] = &LoopHold::SolveComplex18;
  SolveComplexFunction[44] = &LoopHold::SolveComplex14;
  SolveComplexFunction[45] = &LoopHold::SolveComplex28;
  SolveComplexFunction[46] = &LoopHold::SolveComplex6;
  SolveComplexFunction[47] = &LoopHold::SolveComplex60;
  SolveComplexFunction[48] = &LoopHold::SolveComplex3;
  SolveComplexFunction[49] = &LoopHold::SolveComplex19;

  SolveComplexFunction[50] = &LoopHold::SolveComplex11;
  SolveComplexFunction[51] = &LoopHold::SolveComplex15;
  SolveComplexFunction[52] = &LoopHold::SolveComplex7;
  SolveComplexFunction[53] = &LoopHold::SolveComplex15;
  SolveComplexFunction[54] = &LoopHold::SolveComplex7;
  SolveComplexFunction[55] = &LoopHold::SolveComplex15;
  SolveComplexFunction[56] = &LoopHold::SolveComplex11;
  SolveComplexFunction[57] = &LoopHold::SolveComplex27;
  SolveComplexFunction[58] = &LoopHold::SolveComplex43;
  SolveComplexFunction[59] = &LoopHold::SolveComplex15;

  SolveComplexFunction[60] = &LoopHold::SolveComplex15;
  SolveComplexFunction[61] = &LoopHold::SolveComplex15;
  SolveComplexFunction[62] = &LoopHold::SolveComplex15;
  SolveComplexFunction[63] = &LoopHold::SolveComplex15;
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
      def.Set11(trick1, trick2);
    }
    else
    {
      Trick trick;
      trick.Set(QT_BOTH, cr.crashEnd, cr.crashRank, cr.crashTricks);
      def.Set1(trick);
    }
    rank = Min(cr.remRank, cr.crashRank);
    flag = false;
    return true;
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
        trick1.Set(QT_PARD, QT_ACE, cr.crashRank, cr.crashTricks);
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
    trick[0].Set(QT_BOTH, QT_ACE, SDS_VOID - cb.lenOppHighest, 
      cb.lenOppHighest);

    if (pickFlag) holdCtr[0xa25]++;
    lowestRank = Holding::ListToRank(
      completeList[QT_PARD][cb.lenShort - cb.lenOppHighest - 1]);
    trick[1].Set(QT_BOTH, QT_PARD, lowestRank, 
      cb.lenShort - cb.lenOppHighest);
    trick[2].Set(QT_ACE, QT_ACE, SDS_VOID, cb.lenLong - cb.lenShort);
    return def.Set3(trick[0], trick[1], trick[2]);
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
    lowestRank = Holding::ListToRank(cb.maxPard);

    trick[0].Set(QT_BOTH, QT_ACE, r, cb.lenLong);
    trick[1].Set(QT_BOTH, QT_ACE, r2, cb.lenOppMax);
    trick[2].Set(QT_BOTH, QT_PARD, lowestRank, 1);
    trick[3].Set(QT_ACE, QT_ACE, SDS_VOID, cb.lenLong - cb.lenShort);
    return def.Set13(trick);
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
      l = cb.numTopsShortHigh + 1;
      r = Holding::ListToRank(cb.maxPard);
      m = 2;
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
    trick[0].Set(QT_BOTH, QT_PARD, r, l);
    trick[1].Set(QT_BOTH, QT_ACE, lowestRank, cb.lenShort - l);
    trick[2].Set(QT_PARD, QT_PARD, SDS_VOID, cb.lenLong - cb.lenShort);
    return def.Set3(trick[0], trick[1], trick[2]);
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
      trick[0].Set(QT_ACE, QT_PARD, r, cb.lenShort);
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


bool LoopHold::CashoutBothDiffLongStrong(
  DefList& def,
  unsigned& lowestRank,
  const CashoutBothDetails& cb) const
{
  // assert(cb.numTopsShortHigh == 0);
  // Therefore QT_ACE is longer and has all the high tops.
if (pickFlag) holdCtr[0xa99]++;
  
  Trick trick[4];
  unsigned r, l, t;

  if (cb.lenLong > cb.lenShort && 
    cb.lenOppHighest == 1 && 
    ((cb.lenOppLowest == 2 && cb.lenShort >= 3) ||
     (cb.lenOppLowest == 3 && cb.lenShort == 3)) &&
    cb.numTopsLow >= 3 &&
    Min(cb.maxPard, completeList[QT_ACE][1]) >= suitLength - 4)
  {
    // AQxx+ / K / Jxx+ / xx.
    if (pickFlag) holdCtr[0xa40]++;
    lowestRank = SDS_VOID - 4;
    trick[0].Set(QT_BOTH, QT_BOTH, lowestRank, cb.lenLong);
    return def.Set1(trick[0]);
  }
  else if (cb.lenOppHighest >= cb.lenShort &&
      cb.numTopsLongLow >= cb.lenCashLow)
  {
    if (pickFlag) holdCtr[0xa41]++;
    lowestRank = Holding::ListToRank(
      completeList[QT_ACE][cb.lenCashLow-1]);
    trick[0].Set(QT_BOTH, QT_ACE, lowestRank, cb.lenLong);
    return def.Set1(trick[0]);
  }
  else if (cb.lenShort > cb.lenOppHighest &&
      cb.numTopsLongHigh == cb.lenOppHighest &&
      (cb.minPard > completeList[cb.pLong][cb.lenOppHighest] ||
      (cb.maxPard > completeList[cb.pLong][cb.lenOppHighest] &&
       cb.numTopsLow == cb.lenOppLowest &&
       cb.minPard < cb.minOpp &&
       cb.lenLong >= cb.lenOppLowest &&
       cb.lenShort == cb.lenOppHighest + 1)))
  {
    // ATx / K / QJ / xxx.
    // AT9x / K / Qx / Jx.
    trick[0].Set(QT_BOTH, QT_ACE, SDS_VOID - cb.lenOppHighest, 
      cb.lenOppHighest);

    unsigned r2 = Holding::ListToRank(
      completeList[QT_PARD][cb.lenShort - cb.lenOppHighest - 1]);
    trick[1].Set(QT_BOTH, QT_PARD, r2, cb.lenShort - cb.lenOppHighest);

    bool cashFlag = false;

    if (cb.lenOppLowest > cb.lenShort)
    {
      if (cb.lenOppLowest >= cb.lenLong)
      {
        if (cb.minPard > completeList[cb.pLong][cb.lenOppHighest])
        {
          if (cb.numTopsLow > cb.lenLong)
          {
            if (pickFlag) holdCtr[0xa42]++;
            l = Max(cb.numTopsLongHigh, cb.lenLong - cb.lenShort);
            t = cb.lenLong - cb.lenShort;
            lowestRank = Holding::ListToRank(completeList[QT_ACE][l]);
            trick[2].Set(QT_ACE, QT_ACE, lowestRank, t);
          }
          else
          {
            if (pickFlag) holdCtr[0xa43]++;
            l = Min(cb.numTopsLongLow - 1, cb.lenLong - cb.lenShort);
            t = cb.numTopsLow - cb.lenShort - 1;
            lowestRank = Holding::ListToRank(completeList[QT_ACE][l]);
            trick[2].Set(QT_ACE, QT_ACE, lowestRank, t);
          }
        }
        else
        {
          if (pickFlag) holdCtr[0xa44]++;
          l = cb.lenLong - 2;
          t = cb.lenLong - cb.lenShort;
          lowestRank = Holding::ListToRank(completeList[QT_ACE][l]);
          trick[2].Set(QT_ACE, QT_ACE, lowestRank, t);

          if (cb.numTopsLongLow > cb.lenShort)
          {
            cashFlag = true;
            l = (cb.numTopsLongLow >= cb.lenOppLowest ? cb.lenLong :
              cb.numTopsLongLow);
            lowestRank = Holding::ListToRank(completeList[QT_ACE][l-1]);
            trick[3].Set(QT_BOTH, QT_ACE, lowestRank, l);
          }
        }
      }
      else
      {
        if (cb.minPard > completeList[cb.pLong][cb.lenOppHighest])
        {
          if (cb.numTopsLow > cb.lenOppLowest)
          {
            if (pickFlag) holdCtr[0xa45]++;
            l = cb.lenOppLowest - cb.lenShort + cb.lenOppHighest - 1;
            t = cb.lenLong - cb.lenShort;
            lowestRank = Holding::ListToRank(completeList[QT_ACE][l]);
            trick[2].Set(QT_ACE, QT_ACE, lowestRank, t);
          }
          else
          {
            if (pickFlag) holdCtr[0xa46]++;
            l = cb.numTopsLongLow - 1;
            t = cb.numTopsLow - cb.lenShort - 1;
            lowestRank = Holding::ListToRank(completeList[QT_ACE][l]);
            trick[2].Set(QT_ACE, QT_ACE, lowestRank, t);
          }
        }
        else
        {
          if (pickFlag) holdCtr[0xa47]++;
          l = cb.lenOppLowest-2;
          t = cb.lenLong - cb.lenShort;
          lowestRank = Holding::ListToRank(completeList[QT_ACE][l]);
          trick[2].Set(QT_ACE, QT_ACE, lowestRank, t);

          if (cb.numTopsLongLow > cb.lenShort)
          {
            cashFlag = true;
            l = (cb.numTopsLongLow >= cb.lenOppLowest ? cb.lenLong :
              cb.numTopsLongLow);
            lowestRank = Holding::ListToRank(completeList[QT_ACE][l-1]);
            trick[3].Set(QT_BOTH, QT_ACE, lowestRank, l);
          }
        }
      }
    }
    else
    {
      if (pickFlag) holdCtr[0xa48]++;
      if (cb.minPard > cb.minOpp)
        l = cb.lenOppLowest - 1 - cb.lenShort;
      else
        l = cb.lenLong - cb.lenShort;
      lowestRank = r2;
      trick[2].Set(QT_ACE, QT_ACE, SDS_VOID, cb.lenLong - cb.lenShort);
    }

    if (cashFlag)
      return def.Set31(trick);
    else
      return def.Set3(trick[0], trick[1], trick[2]);
  }

  if (cb.minAce > cb.maxPard)
  {
    if (pickFlag) holdCtr[0xa49]++;
    if (cb.lenOppLowest >= cb.lenLong)
      l = cb.lenLong;
    else if (cb.lenShort <= cb.lenOppLowest)
      l = cb.lenOppLowest;
    else
      l = cb.lenOppLowest + 1;
    lowestRank = Holding::ListToRank(completeList[QT_ACE][l-1]);
    trick[0].Set(QT_BOTH, QT_ACE, lowestRank, cb.lenLong);
    return def.Set1(trick[0]);
  }
  else if (cb.numTopsLongLow >= cb.lenOppLowest &&
      completeList[cb.pLong][cb.lenOppLowest] > cb.maxPard)
  {
    if (cb.lenOppLowest >= cb.lenShort &&
        cb.oppMaxLowest > cb.maxPard)
    {
      if (pickFlag) holdCtr[0xa4a]++;
      l = cb.lenOppLowest;
      lowestRank = Holding::ListToRank(completeList[QT_ACE][l-1]);
      trick[0].Set(QT_BOTH, QT_ACE, lowestRank, cb.lenLong);
      return def.Set1(trick[0]);
    }
    else if (cb.lenShort > cb.lenOppLowest + 1 &&
        cb.numTopsLongLow >= cb.lenOppLowest)
    {
      if (pickFlag) holdCtr[0xa4b]++;
      l = cb.lenOppLowest;
      r = Holding::ListToRank(completeList[QT_ACE][l]);
      lowestRank = Holding::ListToRank(cb.maxPard);
      trick[0].Set(QT_BOTH, QT_ACE, r, cb.lenLong);
      trick[1].Set(QT_ACE, QT_PARD, lowestRank, cb.lenLong);
      return def.Set11(trick[0], trick[1]);

    }
    else if (cb.maxPard < cb.minOpp || cb.lenShort == cb.lenOppHighest + 1)
    {
      if (pickFlag) holdCtr[0xa4c]++;
      if (cb.lenShort <= cb.lenOppLowest)
        l = cb.lenOppLowest;
      else
        l = cb.lenOppLowest + 1;
      r = Holding::ListToRank(completeList[QT_ACE][l-1]);
      lowestRank = Holding::ListToRank(cb.maxPard);
      trick[0].Set(QT_BOTH, QT_ACE, r, cb.lenLong);
      trick[1].Set(QT_ACE, QT_PARD, lowestRank, cb.lenShort);
      trick[2].Set(QT_ACE, QT_ACE, SDS_VOID, cb.lenLong - cb.lenShort);
      return def.Set12(trick[0], trick[1], trick[2]);
    }
    else
    {
      if (pickFlag) holdCtr[0xa4d]++;
      r = Holding::ListToRank(completeList[QT_ACE][cb.lenShort - 1]);
      lowestRank = Holding::ListToRank(cb.maxPard);
      trick[0].Set(QT_BOTH, QT_ACE, r, length[QT_ACE]);
      trick[1].Set(QT_ACE, QT_PARD, lowestRank, length[QT_ACE]);
      return def.Set11(trick[0], trick[1]);
    }
  }
  else if (cb.minPard > completeList[cb.pLong][cb.lenOppMax])
  {
    if (cb.lenShort == cb.lenOppHighest + 1 &&
        cb.numTopsShortLow > 0)
    {
      if (cb.numTopsLongLow >= cb.lenOppLowest)
      {
        l = Min(cb.numTopsLongLow, cb.lenOppLowest);
        t = cb.lenLong - cb.lenShort;
        lowestRank = Holding::ListToRank(completeList[QT_ACE][l-1]);
        unsigned maxp = Holding::ListToRank(cb.maxPard);
        if (cb.lenShort >= cb.lenOppLowest)
          r = SDS_VOID;
        else
          r = Holding::ListToRank(completeList[QT_ACE][cb.lenOppLowest-2]);
        if (r > maxp)
          r = SDS_VOID;

        // There is no good reason for this, but it comes out like that
        // in the recursion.
        if (completeList[QT_ACE][cb.lenShort-1] > cb.maxPard)
        {
          if (pickFlag) holdCtr[0xa4e]++;
          trick[0].Set(QT_BOTH, QT_ACE, lowestRank, cb.lenLong);
          trick[1].Set(QT_ACE, QT_PARD, maxp, cb.lenShort);
          trick[2].Set(QT_ACE, QT_ACE, r, t);
          return def.Set12(trick[0], trick[1], trick[2]);
        }
        else
        {
          if (pickFlag) holdCtr[0xa4f]++;
          trick[0].Set(QT_BOTH, QT_ACE, lowestRank, cb.lenLong);
          trick[1].Set(QT_BOTH, QT_ACE, SDS_VOID - (cb.lenShort-1),
            cb.lenShort-1);
          trick[2].Set(QT_BOTH, QT_PARD, maxp, 1);
          trick[3].Set(QT_ACE, QT_ACE, r, t);
          return def.Set13(trick);
        }
      }
      else if (cb.numTopsLongLow == cb.lenShort)
      {
        if (pickFlag) holdCtr[0xa50]++;
        unsigned r2, r3;
        r = Holding::ListToRank(completeList[QT_ACE][cb.lenOppHighest-1]);
        r2 = Holding::ListToRank(cb.maxPard);
        r3 = Holding::ListToRank(
          completeList[QT_ACE][cb.numTopsLongLow-1]);
        if (r3 > r2)
          r3 = SDS_VOID;
        lowestRank = Min(r2, r3);
        if (cb.lenShort == cb.numTopsShortLow &&
            cb.numTopsLow <= cb.lenOppLowest &&
            cb.lenLong >= cb.numTopsLow)
          l = cb.numTopsLow - cb.lenShort - 1;
        else
          l = cb.lenLong - cb.lenShort;

        trick[0].Set(QT_BOTH, QT_ACE, r, cb.lenShort - 1);
        trick[1].Set(QT_BOTH, QT_PARD, r2, 1);
        trick[2].Set(QT_ACE, QT_ACE, r3, l);
        return def.Set3(trick[0], trick[1], trick[2]);
      }
      else if (cb.numTopsLongLow == cb.lenLong &&
          completeList[QT_ACE][cb.lenShort-1] > cb.maxPard)
      {
        // There is no good reason for this.
        if (pickFlag) holdCtr[0xa51]++;
        r = Holding::ListToRank(completeList[QT_ACE][cb.lenLong-1]);
        unsigned r2, r3;
        r2 = Holding::ListToRank(cb.maxPard);
        if (cb.numTopsLongLow == cb.lenLong &&
            cb.lenLong <= cb.lenOppLowest) 
          r3 = Holding::ListToRank(completeList[QT_ACE][cb.lenLong-2]);
        else
          r3 = SDS_VOID;
        if (r3 > r2)
          r3 = SDS_VOID;
          
        lowestRank = Min(r, r2);
        trick[0].Set(QT_BOTH, QT_ACE, r, cb.lenLong);
        trick[1].Set(QT_ACE, QT_PARD, r2, cb.lenShort);
        trick[2].Set(QT_ACE, QT_ACE, r3, cb.lenLong - cb.lenShort);
        return def.Set12(trick[0], trick[1], trick[2]);
      }
      else if (cb.numTopsLongLow == cb.lenShort-1)
      {
        if (pickFlag) holdCtr[0xa52]++;
        unsigned r2, r3;
        r = Holding::ListToRank(
          completeList[QT_ACE][cb.numTopsLongHigh-1]);
        lowestRank = Holding::ListToRank(cb.maxPard);
        trick[0].Set(QT_BOTH, QT_ACE, r, cb.lenShort-1);
        trick[1].Set(QT_BOTH, QT_PARD, lowestRank, 1);
        trick[2].Set(QT_ACE, QT_ACE, SDS_VOID, cb.lenLong - cb.lenShort);
        return def.Set3(trick[0], trick[1], trick[2]);
      }
      else
      {
        if (pickFlag) holdCtr[0xa53]++;
        r = Holding::ListToRank(completeList[QT_ACE][cb.numTopsLongLow-1]);
        unsigned r2, r3, r4;
        r2 = Holding::ListToRank(completeList[QT_ACE][cb.lenShort-2]);
        r3 = Holding::ListToRank(cb.maxPard);
        if (cb.numTopsLongLow == cb.lenLong)
          r4 = Holding::ListToRank(completeList[QT_ACE][length[QT_ACE]-2]);
        else
          r4 = r;
        if (r4 > r3)
          r4 = SDS_VOID;
        lowestRank = Min(r2, r3);
        if (cb.numTopsLow == cb.lenOppLowest &&
            cb.numTopsLow <= cb.lenLong + 1 &&
            cb.numTopsShortLow == cb.lenShort)
          l = cb.numTopsLow - cb.lenShort - 1;
        else
          l = cb.lenLong - cb.lenShort;
        trick[0].Set(QT_BOTH, QT_ACE, r, cb.numTopsLongLow);
        trick[1].Set(QT_BOTH, QT_ACE, r2, cb.lenShort-1);
        trick[2].Set(QT_BOTH, QT_PARD, r3, 1);
        trick[3].Set(QT_ACE, QT_ACE, r4, l);
        return def.Set13(trick);
      }
    }
    else if (length[QT_PARD] <= cb.lenOppLowest)
    {
      if (cb.lenOppHighest <= cb.numTopsLongHigh &&
          cb.lenOppLowest <= cb.numTopsLongLow &&
          (cb.lenShort <= cb.lenOppHighest ||
           cb.maxPard < cb.minOpp))
      {
        if (pickFlag) holdCtr[0xa59]++;
        l = Min(cb.lenLong, cb.lenOppLowest);
        lowestRank = Holding::ListToRank(completeList[QT_ACE][l-1]);
        trick[0].Set(QT_BOTH, QT_ACE, lowestRank, length[QT_ACE]);
        return def.Set1(trick[0]);
      }
      else
        if (pickFlag) holdCtr[0xa80]++;
        return false;
    }
    else
    {
      // Holding::Print();
      if (pickFlag) holdCtr[0xa54]++;
      return false;
    }
  }
  else if (cb.lenShort <= cb.lenOppHighest + 1)
  {
    if (pickFlag) holdCtr[0xa55]++;
    return false;
  }
  else if (cb.lenShort == cb.lenOppMax + 1 &&
    cb.numTopsShortLow == 0)
  {
    if (pickFlag) holdCtr[0xa56]++;
    return false;
  }
  else if (cb.numTopsLongLow >= Max(cb.lenOppMax, cb.lenShort) &&
      completeList[cb.pLong][Max(cb.lenOppMax, cb.lenShort) - 1] > 
      cb.maxPard)
  {
    if (pickFlag) holdCtr[0xa57]++;
    return false;
  }
  else if (cb.numTopsLow == Min(cb.lenLong, cb.lenOppMax) &&
      cb.lenOppHighest + cb.numTopsShortLow == cb.lenShort)
  {
    if (pickFlag) holdCtr[0xa58]++;
    return false;
  }


  PlayDetails pd;
  l = Min(cb.lenOppMax, cb.lenLong);
  LoopHold::SetPlayDetails(l, cb, pd);

  if (pd.numShort == 0)
  {
    if (cb.numTopsShortLow == 0)
      pd.prevPlay = Min(cb.maxPard, pd.nextLong);
    else
      pd.prevPlay = cb.maxPard;
  }
  else if (pd.prevPlay == pd.prevLong)
  {
    if (cb.lenOppMax < cb.lenShort)
    {
      if (pd.nextShort > cb.oppMaxLowest)
        pd.prevPlay = Max(pd.nextShort, pd.nextLong);
      else
        pd.prevPlay = pd.nextLong;
    }
    else if (cb.lenOppHighest + pd.numShort >= cb.lenShort)
      pd.prevPlay = pd.nextLong;
  }
  else
  {
    if (cb.lenOppMax < cb.lenShort)
    {
      if (cb.lenShort == cb.lenOppMax + 1 &&
          pd.nextShort > Max(cb.oppMaxLowest, pd.nextLong) &&
            completeList[cb.pLong][cb.lenOppHighest] > cb.maxPard)
        pd.prevPlay = pd.nextShort;
      else
        pd.prevPlay = pd.nextLong;
    }
    else if (pd.nextLong > cb.oppMaxLowest &&
      (cb.lenShort == cb.lenOppHighest + 1 ||
       cb.lenShort <= cb.lenOppHighest + pd.numShort))
      pd.prevPlay = pd.nextLong;
  }

  if (pickFlag) holdCtr[0xa56]++;
  lowestRank = Holding::ListToRank(pd.prevPlay);
  trick[0].Set(QT_BOTH, QT_BOTH, lowestRank, cb.lenLong);
  return def.Set1(trick[0]);
}


bool LoopHold::CashoutBothDiffStrong(
  DefList& def,
  unsigned& lowestRank,
  const CashoutBothDetails& cb) const
{
  Trick trick;
  PlayDetails pd;
  unsigned l = Min(cb.lenOppMax, cb.lenLong);
  LoopHold::SetPlayDetails(l, cb, pd);

  if (pd.nextLong > pd.nextShort)
  {
    if (cb.lenShort > cb.lenOppMax)
      pd.prevPlay = pd.nextLong;
  
    if (pickFlag) holdCtr[0xa60]++;
    lowestRank = Holding::ListToRank(pd.prevPlay);
    trick.Set(QT_BOTH, QT_BOTH, lowestRank, cb.lenLong);
    return def.Set1(trick);
  }
  else if (cb.lenShort == 2 && cb.lenOppMax == 2 && cb.numTopsHigh == 2)
  {
    if (pickFlag) holdCtr[0xa61]++;
    lowestRank = SDS_VOID - 2;
    trick.Set(QT_BOTH, QT_BOTH, lowestRank, cb.lenLong);
    return def.Set1(trick);
  }

  PosType pl, pr;
  if (cb.pLong == QT_ACE)
  {
    pl = QT_LHO;
    pr = QT_RHO;
  }
  else
  {
    pl = QT_RHO;
    pr = QT_LHO;
  }

  if (cb.numTopsHigh == cb.lenOppHighest &&
      completeList[cb.pShort][cb.lenShort-1] > 
        completeList[cb.pLong][cb.numTopsLongHigh])
  {
    if (pickFlag) holdCtr[0xa62]++;
    return false;
  }
  else if (cb.numTopsLongHigh == 1 && cb.numTopsShortHigh == 1 &&
    cb.lenLong >= 4 && cb.lenShort >= 3 &&
    cb.lenOppHighest == 2 && cb.lenOppLowest <= 3 &&
    cb.pOppHighest == pl)
  {
    // AJx / (xxx) / Kxx / Qx.
    if (pickFlag) holdCtr[0xa63]++;
    return false;
  }
  else if (cb.lenLong >= 5 && cb.lenShort == 4 && length[pl] == 3 &&
    cb.numTopsShortHigh == 2 && cb.numTopsLongHigh == 1 &&
    completeList[pl][0] == suitLength-4 && 
    completeList[cb.pShort][2] == suitLength-5 &&
    completeList[cb.pLong][1] > completeList[cb.pShort][3])
  {
    // A9xxx+ / Jxx / HHTx / (x).
    if (pickFlag) holdCtr[0xa64]++;
    return false;
  }


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

  if (pickFlag) holdCtr[0xa65]++;
  lowestRank = Holding::ListToRank(pd.prevPlay);
  trick.Set(QT_BOTH, QT_BOTH, lowestRank, cb.lenLong);
  return def.Set1(trick);
}


bool LoopHold::CashoutBothDiffSplit(
  DefList& def,
  unsigned& lowestRank,
  const CashoutBothDetails& cb) const
{
  Trick trick;

  if (cb.lenLong >= 4 && cb.lenShort == 3 &&
    cb.lenOppMax == 3 &&
    cb.numTopsLongHigh == 1 && cb.numTopsShortHigh == 1 &&
    completeList[cb.pLong][1] > completeList[cb.pShort][2] &&
    (completeList[cb.pLong][1] > completeList[cb.pShort][1] ||
      cb.lenOppHighest == 1))
  {
    if (pickFlag) holdCtr[0xa70]++;
    lowestRank = Holding::ListToRank(Max(
      completeList[cb.pLong][1], completeList[cb.pShort][1]));
    trick.Set(QT_BOTH, QT_BOTH, lowestRank, cb.lenLong);
    return def.Set1(trick);
  }
  else if (cb.lenLong == 4 && cb.lenShort == 3 &&
    cb.lenOppHighest == 2 && cb.lenOppLowest == 4 &&
    cb.numTopsLongHigh == 2 && cb.numTopsShortHigh == 1 &&
    completeList[cb.pLong][2] > cb.oppMaxLowest &&
    completeList[cb.pLong][2] > completeList[cb.pShort][1])
  {
    // AK9x / JT / Qxx / xxxx.
    if (pickFlag) holdCtr[0xa71]++;
    lowestRank = Holding::ListToRank(completeList[cb.pLong][2]);
    trick.Set(QT_BOTH, QT_BOTH, lowestRank, cb.lenLong);
    return def.Set1(trick);
  }

  if (cb.lenShort == 3 && cb.lenLong >= 4 && cb.numTopsHigh == 2 &&
      cb.lenOppHighest == 2 && cb.lenOppLowest >= 3 &&
      (completeList[cb.pShort][1] == suitLength-4 ||
      (completeList[cb.pShort][1] == suitLength-5 ||
       completeList[cb.pShort][2] == suitLength-6)))
  {
    // Holding::Print();
    if (pickFlag) holdCtr[0xa72]++;
    return false;
  }
  else if (cb.numTopsShortLow == cb.lenShort &&
    cb.numTopsLow == Min(cb.lenLong, cb.lenOppLowest))
  {
    if (pickFlag) holdCtr[0xa73]++;
    return false;
  }
  else if (cb.lenShort == 3 && cb.lenLong == 4 &&
    cb.lenOppHighest == 2 && cb.lenOppLowest == 4)
  {
    if (cb.numTopsLow == cb.lenOppMax &&
        cb.numTopsShortHigh == 1 &&
        cb.numTopsShortLow > 1)
    {
      if (pickFlag) holdCtr[0xa74]++;
      return false;
    }
    else if (cb.numTopsLow >= cb.lenOppMax + 1 &&
        cb.numTopsShortHigh == 1 &&
        cb.numTopsShortLow == 3)
    {
      if (pickFlag) holdCtr[0xa75]++;
      return false;
    }
    else if (cb.numTopsHigh == 3 && cb.numTopsShortHigh == 1 &&
      ((cb.pShort == QT_ACE && cb.pOppHighest == QT_RHO) ||
       (cb.pShort == QT_PARD && cb.pOppHighest == QT_LHO)) &&
      completeList[cb.pShort][1] == suitLength-5)
    {
      // ATx / xxxx / KQxx / Jx.
      if (pickFlag) holdCtr[0xa76]++;
      return false;
    }
  }
  else if (cb.lenShort == 4 && cb.lenLong == 5 &&
    cb.lenOppHighest == 1 && cb.lenOppLowest == 3)
  {
    if (cb.numTopsHigh == 2 && 
        cb.numTopsLow == 3 &&
        completeList[cb.pLong][cb.numTopsLongLow] < 
          completeList[cb.pShort][3])
    {
      if (pickFlag) holdCtr[0xa77]++;
      return false;
    }
  }

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

  if (pickFlag) holdCtr[0xa78]++;
  lowestRank = Holding::ListToRank(pd.prevPlay);
  trick.Set(QT_BOTH, QT_BOTH, lowestRank, cb.lenLong);
  return def.Set1(trick);
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

  hdet.maxTopLong = Holding::ListToRank(completeList[hdet.pLong][0]);
  hdet.maxTopShort = Holding::ListToRank(completeList[hdet.pShort][0]);

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
  sdet.minTopLong = Holding::ListToRank(
    completeList[hdet.pLong][sdet.numTopsLong-1]);
  sdet.minTopShort = Holding::ListToRank(
    completeList[hdet.pShort][sdet.numTopsShort-1]);

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
    unsigned x = Holding::ListToRank(completeList[oppSkipped][j]);
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
      unsigned r = Holding::ListToRank(completeList[QT_ACE][3]);
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
    unsigned r = Holding::ListToRank(completeList[QT_ACE][3]);
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
    unsigned r = Holding::ListToRank(completeList[pa][2]);
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
    unsigned r = Holding::ListToRank(completeList[pa][2]);
    return move.Set(QT_BOTH, QT_BOTH, r, length[pa]);
  }
  else if (length[pa] >= 4 && length[pp] == length[pa] &&
    length[pl] == 3 && length[pr] <= 3 && htop.T == pa)
  {
    // AJTx+ / Q(xx) / Kxxx+ / (xxx).
    if (pickFlag) holdCtr[0x062]++;
    unsigned r = Holding::ListToRank(completeList[pa][2]);
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
      unsigned r = Holding::ListToRank(completeList[pp][1]);
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
      unsigned r = Holding::ListToRank(completeList[pa][2]);
      return move.Set(QT_BOTH, QT_BOTH, r, 3);
    }
    else if (((htop.Q == QT_LHO && distHex == 0x4432) ||
      (htop.Q == QT_RHO && distHex == 0x3244)) && 
      completeList[pl][1] > completeList[pa][3] &&
      completeList[pl][1] > completeList[pp][1])
    {
      // AJxx / Qxxx / Kxx / Tx.
      if (pickFlag) holdCtr[0x467]++;
      unsigned r = Holding::ListToRank(completeList[pa][2]);
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
      unsigned r = Holding::ListToRank(completeList[QT_ACE][1]);
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
      unsigned r = (length[QT_LHO] == 2 ? 
        Holding::ListToRank(completeList[QT_ACE][1]) : SDS_VOID-1);
      return move.Set(QT_BOTH, QT_ACE, r, length[QT_ACE]);
    }
    else if (htop.T == QT_LHO)
    {
      // AH+ / Txx+ / H / K.
      if (pickFlag) holdCtr[0x115]++;
      unsigned r = Holding::ListToRank(completeList[QT_ACE][1]);
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
  else if (completeList[QT_LHO][0] > completeList[QT_ACE][4])
  {
    if (length[QT_LHO] >= 5 &&
      (length[QT_RHO] == 2 || (length[QT_PARD] == 1 ||
        completeList[QT_PARD][0] < completeList[QT_LHO][0])))
    {
      // AKJxx / xxxxx / x / Q(x).
      if (pickFlag) holdCtr[0x123]++;
      unsigned r = Holding::ListToRank(completeList[QT_ACE][3]);
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
      r = Holding::ListToRank(completeList[pp][1]);

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
    unsigned r = Holding::ListToRank(
      completeList[QT_ACE][length[QT_LHO]-1]);
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
    unsigned r = Holding::ListToRank(completeList[QT_ACE][x]);
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
    unsigned r = Holding::ListToRank(completeList[QT_ACE][x+1]);
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
      unsigned r = Holding::ListToRank(completeList[QT_PARD][1]);
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
    unsigned r = Holding::ListToRank(completeList[pa][2]);
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
    unsigned r = Holding::ListToRank(completeList[QT_ACE][3]);
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
        unsigned r = Holding::ListToRank(completeList[QT_ACE][2]);
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
      unsigned r = Holding::ListToRank(completeList[QT_ACE][3]);
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
      unsigned r = Holding::ListToRank(completeList[QT_ACE][2]);
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
    unsigned r = Holding::ListToRank(completeList[QT_ACE][3]);
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
    unsigned r = Holding::ListToRank(completeList[QT_ACE][length[QT_RHO]-1]);
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
    unsigned r = Holding::ListToRank(completeList[QT_ACE][x]);
    return move.Set(QT_BOTH, pend, r, 3);
  }
  else if (length[QT_PARD] >= 3 &&
    length[QT_ACE] <= 2 && length[QT_LHO] >= 4 && 
    completeList[QT_LHO][0] > completeList[QT_PARD][2] &&
    completeList[QT_LHO][0] < completeList[QT_PARD][1])
  {
    // Ax / xxxx / Kxxx / QJ.
    if (pickFlag) holdCtr[0x605]++;
    unsigned r = Holding::ListToRank(completeList[QT_PARD][1]);
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
    unsigned r = Holding::ListToRank(completeList[QT_ACE][x+1]);
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

  if (pickFlag) holdCtr[0x1000]++;
  UNUSED(def);
  UNUSED(rank);
  return false;
}


bool LoopHold::SolveComplex1(DefList& def, unsigned& rank) const
{
  // ==== G1 ================= G4 ================= G16 ================
  //      AQJ+       |         AKJ+       |         AKQ+
  // +          K+   |    +          Q+   |    +          J+
  //      +          |         +          |         + 
  // ==== G1 ================= G4 ================= G16 ================

  if (pickFlag) holdCtr[0x1010]++;
  UNUSED(def);
  UNUSED(rank);
  return false;
}


bool LoopHold::SolveComplex2(DefList& def, unsigned& rank) const
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

  if (pickFlag) holdCtr[0x1020]++;
  UNUSED(def);
  UNUSED(rank);
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
  UNUSED(def);
  UNUSED(rank);
  return false;
}


bool LoopHold::SolveComplex5(DefList& def, unsigned& rank) const
{
  // ==== G5 ============== G21 =========== G37 =========== G53 ========
  //      AJ+        |      A+       |      A+       |      A+
  // +          KQ+  |  +       KQJ+ |  +       KQ+  |  J+      KQ+
  //      +          |      +        |      J+       |      +
  // ==== G5 ============== G21 =========== G37 =========== G53 ========

  if (pickFlag) holdCtr[0x1050]++;
  UNUSED(def);
  UNUSED(rank);
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
  UNUSED(def);
  UNUSED(rank);
  return false;
}


bool LoopHold::SolveComplex7(DefList& def, unsigned& rank) const
{
  // ==== G7 ============== G13 =========== G39 =========== G45 ========
  //      AJ+        |      AJ+      |      A+       |      A+
  // K+         Q+   | Q+       K+   |  K+      Q+   |  Q+      K+ 
  //      +          |      +        |      J+       |      J+
  // ==== G7 ============== G13 =========== G39 =========== G45 ========

  if (pickFlag) holdCtr[0x1070]++;
  UNUSED(def);
  UNUSED(rank);
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
  UNUSED(def);
  UNUSED(rank);
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
  UNUSED(def);
  UNUSED(rank);
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
  Trick trick[4];

  if (distHex == 0x4432 && htop.T == QT_PARD && htop.N == QT_PARD)
  {
    if (completeList[QT_ACE][3] > completeList[QT_LHO][0])
    {
      // AKJ8 / 7654 / T93 / Q2.
      if (pickFlag) holdCtr[0x1121]++;
      rank = Holding::ListToRank(completeList[QT_ACE][3]);
      trick[0].Set(QT_ACE, QT_ACE, rank, 4);
      trick[1].Set(QT_PARD, QT_BOTH, SDS_VOID-6, 4);
      trick[2].Set(QT_ACE, QT_PARD, SDS_VOID-5, 3);
      trick[3].Set(QT_ACE, QT_ACE, SDS_VOID, 1);
      return def.Set112(trick);
    }
    else
    {
      // AKJ7 / 8xxx / T9x / Qx.
      if (pickFlag) holdCtr[0x1121]++;
      rank = SDS_VOID - 6;
      trick[0].Set(QT_ACE, QT_ACE, SDS_VOID-2, 2);
      trick[1].Set(QT_BOTH, QT_PARD, SDS_VOID-5, 1);
      trick[2].Set(QT_ACE, QT_ACE, SDS_VOID, 1);
      trick[3].Set(QT_PARD, QT_BOTH, rank, 4);
      return def.Set31(trick);
    }
  }
  UNUSED(def);
  UNUSED(rank);
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
  UNUSED(def);
  UNUSED(rank);
  return false;
}


bool LoopHold::SolveComplex15(DefList& def, unsigned& rank) const
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

  if (pickFlag) holdCtr[0x1150]++;
  UNUSED(def);
  UNUSED(rank);
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
  UNUSED(def);
  UNUSED(rank);
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

  if (pickFlag) holdCtr[0x1200]++;
  UNUSED(def);
  UNUSED(rank);
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
  UNUSED(def);
  UNUSED(rank);
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
  UNUSED(def);
  UNUSED(rank);
  return false;
}


bool LoopHold::SolveComplex28(DefList& def, unsigned& rank) const
{
  // ==== G28 ============= G30 =========== G52 =========== G54 ========
  //      AK+        |      A+       |      AK+      |      A+
  // Q+         J+   |  Q+      J+   |  J+      Q+   |  J+      Q+ 
  //      +          |      K+       |      +        |      +
  // ==== G28 ============= G30 =========== G52 =========== G54 ========

  if (pickFlag) holdCtr[0x1280]++;
  UNUSED(def);
  UNUSED(rank);
  return false;
}


bool LoopHold::SolveComplex36(DefList& def, unsigned& rank) const
{
  // ==== G36 ==========================================================
  //      AK+     
  // +          Q+
  //      J+     
  // ==== G36 ==========================================================

  if (pickFlag) holdCtr[0x1360]++;
  UNUSED(def);
  UNUSED(rank);
  return false;
}


bool LoopHold::SolveComplex41(DefList& def, unsigned& rank) const
{
  // ==== G41 ==========================================================
  //      A+      
  // +          K+
  //      QJ+    
  // ==== G41 ==========================================================

  if (pickFlag) holdCtr[0x1410]++;
  UNUSED(def);
  UNUSED(rank);
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
  UNUSED(def);
  UNUSED(rank);
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
  if (length[QT_ACE] > length[QT_PARD] && length[QT_PARD] == 3 &&
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
        // AKT8 / 7654 / J93 / Q2, AK98 / 7652 / JT3 / Q4.
        if (pickFlag) holdCtr[0x1440]++;
        rank = Holding::ListToRank(completeList[QT_ACE][3]);
        unsigned r1 = Holding::ListToRank(
          Min(completeList[QT_PARD][1], completeList[QT_ACE][2]));
        unsigned r2 = Holding::ListToRank(completeList[QT_ACE][2]);
        trick[0].Set(QT_ACE, QT_ACE, rank, 4);
        trick[1].Set(QT_PARD, QT_BOTH, r1, 4);
        trick[2].Set(QT_ACE, QT_ACE, SDS_VOID-2, 2);
        trick[3].Set(QT_BOTH, QT_PARD, SDS_VOID-4, 1);
        trick[4].Set(QT_ACE, QT_ACE, r2, 1);
        return def.Set113(trick);
      }
      else if (htop.T == QT_PARD)
      {
        if (completeList[QT_PARD][2] > completeList[QT_ACE][2])
        {
          if ((htop.N == QT_PARD || 
              (htop.N == QT_RHO && htop.E == QT_PARD)) &&
              completeList[QT_ACE][2] > completeList[QT_LHO][0])
          {
            // AK8x / xxxx / JT9 / Qx.
            // AK7x / 6xxx / JT8 / Q9.
            if (pickFlag) holdCtr[0x1441]++;
            rank = Holding::ListToRank(completeList[QT_ACE][2]);
            trick[0].Set(QT_ACE, QT_ACE, SDS_VOID-2, 2);
            trick[1].Set(QT_BOTH, QT_PARD, SDS_VOID-4, 1);
            trick[2].Set(QT_ACE, QT_ACE, rank, 1);
            trick[3].Set(QT_PARD, QT_BOTH, rank, 4);
            return def.Set31(trick);
          }
          else if (htop.N == QT_PARD || htop.E == QT_PARD)
          {
            // AKxx / 8xxx / JT9 / Qx.
            // AKxx / 9xxx / JT8 / Qx.
            // AKxx / xxxx / JT8 / Q9.
            if (pickFlag) holdCtr[0x1442]++;
            if (htop.N == QT_PARD)
              rank = Holding::ListToRank(completeList[QT_PARD][1]);
            else
              rank = Holding::ListToRank(completeList[QT_PARD][2]);
            trick[0].Set(QT_BOTH, QT_BOTH, SDS_VOID-4, 3);
            trick[1].Set(QT_PARD, QT_PARD, rank, 3);
            trick[2].Set(QT_ACE, QT_ACE, SDS_VOID, 1);
            return def.Set12(trick[0], trick[1], trick[2]);
          }
        }
        else if (htop.N == QT_LHO && htop.E == QT_ACE)
        {
          if (completeList[QT_ACE][3] > completeList[QT_LHO][1])
          {
            // AK86 / 9543 / JT7 / Q2.
            // AK86 / 9654 / JT3 / Q7.
            if (pickFlag) holdCtr[0x1443]++;
            rank = Holding::ListToRank(
              Max(completeList[QT_ACE][3], completeList[QT_PARD][2]));
            trick[0].Set(QT_PARD, QT_ACE, rank, 2);
            trick[1].Set(QT_BOTH, QT_PARD, SDS_VOID, 1);
            trick[2].Set(QT_ACE, QT_ACE, SDS_VOID, 1);
            trick[3].Set(QT_ACE, QT_BOTH, SDS_VOID-4, 3);
            return def.Set31(trick);
          }
          else
          {
            // AK8x / 9xxx / JT7 / Qx.
            if (pickFlag) holdCtr[0x1444]++;
            rank = Holding::ListToRank(completeList[QT_PARD][2]);
            trick[0].Set(QT_BOTH, QT_BOTH, SDS_VOID-4, 3);
            trick[1].Set(QT_PARD, QT_PARD, rank, 3);
            trick[2].Set(QT_ACE, QT_ACE, SDS_VOID, 1);
            return def.Set12(trick[0], trick[1], trick[2]);
          }
        }
        else if (completeList[QT_ACE][2] > completeList[QT_LHO][0] &&
            completeList[QT_ACE][3] < completeList[QT_LHO][0])
        {
          // AK8x / 7xxx / JTx / Q9.
          if (pickFlag) holdCtr[0x1445]++;
          rank = Holding::ListToRank(completeList[QT_ACE][2]);
          trick[0].Set(QT_ACE, QT_ACE, SDS_VOID-2, 2);
          trick[1].Set(QT_BOTH, QT_PARD, SDS_VOID-4, 1);
          trick[2].Set(QT_ACE, QT_ACE, rank, 1);
          trick[3].Set(QT_PARD, QT_BOTH, rank, 4);
          return def.Set31(trick);
        }
      }
      else if (htop.T == QT_ACE)
      {
        if (htop.N == QT_PARD &&
            completeList[QT_LHO][0] > completeList[QT_ACE][3])
        {
          // AKTx / 8xxx / J9x / Qx.
          if (pickFlag) holdCtr[0x1446]++;
          rank = SDS_VOID - 6;
          trick[0].Set(QT_ACE, QT_ACE, SDS_VOID-2, 2);
          trick[1].Set(QT_BOTH, QT_PARD, SDS_VOID-4, 1);
          trick[2].Set(QT_ACE, QT_ACE, SDS_VOID-5, 1);
          trick[3].Set(QT_PARD, QT_BOTH, rank, 4);
          return def.Set31(trick);
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
      if (pickFlag) holdCtr[0x1447]++;
      rank = Holding::ListToRank(completeList[QT_ACE][2]);
      trick[0].Set(QT_BOTH, QT_ACE, rank, length[QT_ACE]);
      trick[1].Set(QT_ACE, QT_ACE, SDS_VOID-2, 2);
      trick[2].Set(QT_BOTH, QT_PARD, SDS_VOID-4, 1);
      trick[3].Set(QT_ACE, QT_ACE, SDS_VOID, length[QT_ACE]-3);
      return def.Set13(trick);
    }
    else if (htop.T == QT_PARD ||
        (htop.T == QT_LHO && htop.N == QT_PARD && length[QT_LHO] == 1))
    {
      if (completeList[QT_PARD][2] > completeList[QT_ACE][2])
      {
        // AKxx(+) / (xxx) / JT9 / Qx.
        if (pickFlag) holdCtr[0x1448]++;
        rank = Holding::ListToRank(completeList[QT_PARD][1]);
        trick[0].Set(QT_ACE, QT_ACE, SDS_VOID-2, 2);
        trick[1].Set(QT_BOTH, QT_PARD, SDS_VOID-4, 1);
        trick[2].Set(QT_ACE, QT_ACE, SDS_VOID, length[QT_ACE]-3);
        trick[3].Set(QT_PARD, QT_BOTH, rank, length[QT_ACE]);
        return def.Set31(trick);
      }
      else if (length[QT_LHO] <= 2 ||
          completeList[QT_ACE][2] > completeList[QT_LHO][0])
      {
        // AK8x / T / J9x / Qx.
        if (pickFlag) holdCtr[0x1449]++;
        rank = Holding::ListToRank(completeList[QT_ACE][2]);
        unsigned r2 = Holding::ListToRank(completeList[QT_PARD][1]);
        trick[0].Set(QT_ACE, QT_ACE, rank, length[QT_ACE]);
        trick[1].Set(QT_PARD, QT_BOTH, r2, length[QT_ACE]);
        trick[2].Set(QT_ACE, QT_ACE, SDS_VOID-2, 2);
        trick[3].Set(QT_BOTH, QT_PARD, SDS_VOID-4, 1);
        trick[4].Set(QT_ACE, QT_ACE, SDS_VOID, length[QT_ACE]-3);
        return def.Set113(trick);
      }
      else
      {
        // AK7x / 8xx / JTx / Q9.
        if (pickFlag) holdCtr[0x144a]++;
        rank = SDS_VOID - 5;
        trick[0].Set(QT_ACE, QT_ACE, SDS_VOID-2, 2);
        trick[1].Set(QT_BOTH, QT_PARD, SDS_VOID-4, 1);
        trick[2].Set(QT_ACE, QT_ACE, SDS_VOID, length[QT_ACE]-3);
        trick[3].Set(QT_PARD, QT_BOTH, rank, length[QT_ACE]);
        return def.Set31(trick);
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
        if (pickFlag) holdCtr[0x144b]++;
        rank = Holding::ListToRank(
        completeList[QT_PARD][length[QT_PARD]-3]);
        unsigned rank2 = Holding::ListToRank(completeList[QT_PARD][1]);
        trick[0].Set(QT_ACE, QT_ACE, SDS_VOID-2, 2);
        trick[1].Set(QT_BOTH, QT_PARD, rank, length[QT_PARD] - 2);
        trick[2].Set(QT_ACE, QT_ACE, SDS_VOID, 
          length[QT_ACE] - length[QT_PARD]);
        trick[3].Set(QT_PARD, QT_BOTH, rank2, length[QT_ACE]);
        return def.Set31(trick);
      }
      else
      {
        // AK8xx / - / JT9x / Qx.
        if (pickFlag) holdCtr[0x144c]++;
        rank = Holding::ListToRank(completeList[QT_ACE][2]);
        unsigned r2 = Holding::ListToRank(completeList[QT_PARD][1]);
        trick[0].Set(QT_ACE, QT_BOTH, rank, length[QT_ACE]);
        trick[1].Set(QT_PARD, QT_BOTH, r2, length[QT_ACE]);
        return def.Set11(trick[0], trick[1]);
      }
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
  if (length[QT_ACE] >= 5 && length[QT_PARD] == 4 &&
      length[QT_RHO] == 3 && htop.T == QT_PARD)
  {
    if (htop.N == QT_PARD || (htop.N == QT_LHO && htop.E == QT_PARD))
    {
      if (completeList[QT_PARD][3] > completeList[QT_ACE][3])
      {
        // AKQxx / (x) / T9xx / Jxx.
        if (pickFlag) holdCtr[0x1481]++;
        rank = Holding::ListToRank(completeList[QT_PARD][1]);
        trick[0].Set(QT_ACE, QT_ACE, SDS_VOID-3, 3);
        trick[1].Set(QT_BOTH, QT_PARD, SDS_VOID-5, 1);
        trick[2].Set(QT_ACE, QT_ACE, SDS_VOID, length[QT_ACE] - 4);
        trick[3].Set(QT_PARD, QT_BOTH, rank, length[QT_ACE]);
        return def.Set31(trick);
      }
      else
      {
        if (pickFlag) holdCtr[0x1482]++;
        rank = Holding::ListToRank(completeList[QT_ACE][3]);
        unsigned r = Holding::ListToRank(completeList[QT_PARD][1]);
        trick[0].Set(QT_ACE, QT_ACE, rank, length[QT_ACE]);
        trick[1].Set(QT_PARD, QT_BOTH, r, length[QT_ACE]);
        trick[2].Set(QT_ACE, QT_ACE, SDS_VOID-3, 3);
        trick[3].Set(QT_BOTH, QT_PARD, SDS_VOID-5, 1);
        trick[4].Set(QT_ACE, QT_ACE, SDS_VOID, length[QT_ACE]-4);
        return def.Set113(trick);
      }
    }
    else if (htop.N == QT_ACE || (htop.N == QT_LHO && htop.E == QT_ACE))
    {
      if (pickFlag) holdCtr[0x1483]++;
      rank = Holding::ListToRank(completeList[QT_ACE][3]);
      PosType s;
      if (completeList[QT_PARD][1] > completeList[QT_RHO][1] &&
          completeList[QT_PARD][1] > completeList[QT_ACE][length[QT_ACE]-1])
        s = QT_ACE;
      else
        s = QT_BOTH;
      trick[0].Set(QT_BOTH, QT_ACE, rank, length[QT_ACE]);
      trick[1].Set(s, QT_ACE, SDS_VOID-3, 3);
      trick[2].Set(QT_BOTH, QT_PARD, SDS_VOID-5, 1);
      trick[3].Set(QT_ACE, QT_ACE, SDS_VOID, length[QT_ACE]-4);
      return def.Set13(trick);
    }
  }
  else if (distHex == 0x4432 && htop.T == QT_PARD && htop.N == QT_PARD)
  {
    if (completeList[QT_ACE][3] > completeList[QT_LHO][0])
    {
      // AKQ8 / 7654 / T93 / J2.
      if (pickFlag) holdCtr[0x1484]++;
      rank = Holding::ListToRank(completeList[QT_ACE][3]);
      trick[0].Set(QT_ACE, QT_ACE, rank, 4);
      trick[1].Set(QT_PARD, QT_BOTH, SDS_VOID-6, 4);
      trick[2].Set(QT_ACE, QT_PARD, SDS_VOID-5, 3);
      trick[3].Set(QT_ACE, QT_ACE, SDS_VOID, 1);
      return def.Set112(trick);
    }
    else
    {
      // AKQ7 / 8654 / T93 / J2.
      if (pickFlag) holdCtr[0x1485]++;
      rank = SDS_VOID - 6;
      trick[0].Set(QT_ACE, QT_ACE, SDS_VOID-2, 2);
      trick[1].Set(QT_BOTH, QT_PARD, SDS_VOID-5, 1);
      trick[2].Set(QT_ACE, QT_ACE, SDS_VOID, 1);
      trick[3].Set(QT_PARD, QT_BOTH, rank, 4);
      return def.Set31(trick);
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


bool LoopHold::SolveComplex57(DefList& def, unsigned& rank) const
{
  // ==== G57 ==========================================================
  //      A+      
  // J+         K+
  //      Q+    
  // ==== G57 ==========================================================

  if (pickFlag) holdCtr[0x1570]++;
  UNUSED(def);
  UNUSED(rank);
  return false;
}


bool LoopHold::SolveComplex60(DefList& def, unsigned& rank) const
{
  // ==== G60 ============= G62 ========================================
  //      AK+        |      A+     
  // QJ+        +    | QJ+      +  
  //      +          |      K+     
  // ==== G60 ============= G62 ========================================

  if (pickFlag) holdCtr[0x1600]++;
  UNUSED(def);
  UNUSED(rank);
  return false;
}


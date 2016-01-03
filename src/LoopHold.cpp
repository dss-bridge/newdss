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

  SolveStoppedFunction[0] = &LoopHold::SolveStopped0;
  SolveStoppedFunction[1] = &LoopHold::SolveStopped1;
  SolveStoppedFunction[2] = &LoopHold::SolveStopped2;
  SolveStoppedFunction[3] = &LoopHold::SolveStopped48;
  SolveStoppedFunction[4] = &LoopHold::SolveStopped1;
  SolveStoppedFunction[5] = &LoopHold::SolveStopped20;
  SolveStoppedFunction[6] = &LoopHold::SolveStopped36;
  SolveStoppedFunction[7] = &LoopHold::SolveStopped28;
  SolveStoppedFunction[8] = &LoopHold::SolveStopped2;
  SolveStoppedFunction[9] = &LoopHold::SolveStopped18;

  SolveStoppedFunction[10] = &LoopHold::SolveStopped2;
  SolveStoppedFunction[11] = &LoopHold::SolveStopped26;
  SolveStoppedFunction[12] = &LoopHold::SolveStopped12;
  SolveStoppedFunction[13] = &LoopHold::SolveStopped28;
  SolveStoppedFunction[14] = &LoopHold::SolveStopped44;
  SolveStoppedFunction[15] = &LoopHold::SolveStopped60;
  SolveStoppedFunction[16] = &LoopHold::SolveStopped1;
  SolveStoppedFunction[17] = &LoopHold::SolveStopped15;
  SolveStoppedFunction[18] = &LoopHold::SolveStopped9;
  SolveStoppedFunction[19] = &LoopHold::SolveStopped49;

  SolveStoppedFunction[20] = &LoopHold::SolveStopped5;
  SolveStoppedFunction[21] = &LoopHold::SolveStopped5;
  SolveStoppedFunction[22] = &LoopHold::SolveStopped5;
  SolveStoppedFunction[23] = &LoopHold::SolveStopped5;
  SolveStoppedFunction[24] = &LoopHold::SolveStopped9;
  SolveStoppedFunction[25] = &LoopHold::SolveStopped15;
  SolveStoppedFunction[26] = &LoopHold::SolveStopped41;
  SolveStoppedFunction[27] = &LoopHold::SolveStopped57;
  SolveStoppedFunction[28] = &LoopHold::SolveStopped7;
  SolveStoppedFunction[29] = &LoopHold::SolveStopped15;

  SolveStoppedFunction[30] = &LoopHold::SolveStopped7;
  SolveStoppedFunction[31] = &LoopHold::SolveStopped15;
  SolveStoppedFunction[32] = &LoopHold::SolveStopped2;
  SolveStoppedFunction[33] = &LoopHold::SolveStopped18;
  SolveStoppedFunction[34] = &LoopHold::SolveStopped2;
  SolveStoppedFunction[35] = &LoopHold::SolveStopped26;
  SolveStoppedFunction[36] = &LoopHold::SolveStopped6;
  SolveStoppedFunction[37] = &LoopHold::SolveStopped20;
  SolveStoppedFunction[38] = &LoopHold::SolveStopped14;
  SolveStoppedFunction[39] = &LoopHold::SolveStopped28;

  SolveStoppedFunction[40] = &LoopHold::SolveStopped2;
  SolveStoppedFunction[41] = &LoopHold::SolveStopped26;
  SolveStoppedFunction[42] = &LoopHold::SolveStopped2;
  SolveStoppedFunction[43] = &LoopHold::SolveStopped18;
  SolveStoppedFunction[44] = &LoopHold::SolveStopped14;
  SolveStoppedFunction[45] = &LoopHold::SolveStopped28;
  SolveStoppedFunction[46] = &LoopHold::SolveStopped6;
  SolveStoppedFunction[47] = &LoopHold::SolveStopped60;
  SolveStoppedFunction[48] = &LoopHold::SolveStopped3;
  SolveStoppedFunction[49] = &LoopHold::SolveStopped19;

  SolveStoppedFunction[50] = &LoopHold::SolveStopped11;
  SolveStoppedFunction[51] = &LoopHold::SolveStopped15;
  SolveStoppedFunction[52] = &LoopHold::SolveStopped7;
  SolveStoppedFunction[53] = &LoopHold::SolveStopped15;
  SolveStoppedFunction[54] = &LoopHold::SolveStopped7;
  SolveStoppedFunction[55] = &LoopHold::SolveStopped15;
  SolveStoppedFunction[56] = &LoopHold::SolveStopped11;
  SolveStoppedFunction[57] = &LoopHold::SolveStopped27;
  SolveStoppedFunction[58] = &LoopHold::SolveStopped43;
  SolveStoppedFunction[59] = &LoopHold::SolveStopped15;

  SolveStoppedFunction[60] = &LoopHold::SolveStopped15;
  SolveStoppedFunction[61] = &LoopHold::SolveStopped15;
  SolveStoppedFunction[62] = &LoopHold::SolveStopped15;
  SolveStoppedFunction[63] = &LoopHold::SolveStopped15;
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
  unsigned& rank)
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
    return false;
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
    lenOpp < length[QT_ACE] &&
    length[QT_ACE] == length[QT_PARD] &&
    completeList[QT_ACE][lenOpp-1] > completeList[QT_PARD][0])
  {
    // AKJ / 87 / QT9 / -.
    cr.crashRank2 = SDS_VOID - lenOpp;
  }
  else if (length[QT_ACE] > length[QT_PARD] &&
      length[QT_PARD] >= 2 &&
      hdet.maxTopShort <= SDS_ACE - length[QT_PARD] &&
      lenOpp <= SDS_ACE - hdet.maxTopShort)
  {
    cr.crashRank2 = Holding::ListToRank(
      completeList[QT_ACE][Max(lenOpp, length[QT_PARD]) - 1]);
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
  unsigned& lowestRank)
{
  CashoutBothDetails cb;
  if (! LoopHold::SetCashoutBothDetails(cb))
  {
    Holding::Print();
    PrintCashoutDetails(cb);
    assert(false);
  }

  pickFlag = true;

  if (cb.lenLong == cb.lenShort)
    return LoopHold::CashoutBothSameLength(def, lowestRank, cb);
  else
    return LoopHold::CashoutBothDiffLength(def, lowestRank, cb);
}


bool LoopHold::CashoutBothSameLength(
  DefList& def,
  unsigned& lowestRank,
  const CashoutBothDetails& cb) const
{
  // This function is complete.

  Trick trick, trick2;
  unsigned r;

  if (cb.numTopsHigh < cb.lenCashHigh || cb.numTopsLow < cb.lenCashLow)
  {
    // We don't have enough tops.
    if (pickFlag) holdCtr[1000]++;
    return false;
  }
  else if (cb.numTopsLongHigh > 0 && 
      cb.numTopsShortHigh > 0 &&
      cb.numTopsHigh == cb.lenLong && 
      cb.lenLong <= cb.lenOppMax)
  {
    // AJ / Q98 / KT / -.
    if (pickFlag) holdCtr[1001]++;
    lowestRank = SDS_VOID - cb.lenLong;
    trick.Set(QT_BOTH, QT_BOTH, lowestRank, cb.lenLong);
    return def.Set1(trick);
  }
  else if (cb.numTopsHigh >= cb.lenOppLowest)
  {
    // pOppLowest opponent doesn't matter.

    if (cb.minAce > cb.maxPard)
    {
      // AKT / - / 987 / QJ.
      if (pickFlag) holdCtr[1002]++;
      lowestRank = Holding::ListToRank(
        completeList[QT_ACE][cb.lenOppMax - 1]);
      trick.Set(QT_BOTH, QT_ACE, lowestRank, cb.lenLong);
      return def.Set1(trick);
    }
    else if (cb.numTopsLongHigh > 0 && cb.numTopsShortHigh > 0)
    {
      // Both ace holder and partner have tops over pOppHighest.

      unsigned m = Holding::ListToRank(cb.maxPard);
      r = SDS_VOID - cb.lenOppMax;
      if (m >= r)
      {
        if (pickFlag) holdCtr[1003]++;
        lowestRank = r;
        trick.Set(QT_BOTH, QT_BOTH, lowestRank, cb.lenLong);
        return def.Set1(trick);
      }
      else
      {
        if (pickFlag) holdCtr[1004]++;
        lowestRank = m;
        trick.Set(QT_BOTH, QT_ACE, r, cb.lenLong);
        trick2.Set(QT_ACE, QT_PARD, lowestRank, cb.lenLong);
        return def.Set11(trick, trick2);
      }
    }
    else
    {
      // AK8 / Q7 / JT9 / -.
      if (pickFlag) holdCtr[1005]++;
      r = Holding::ListToRank(completeList[QT_ACE][cb.lenOppMax - 1]);
      lowestRank = Holding::ListToRank(cb.maxPard);
      trick.Set(QT_BOTH, QT_ACE, r, cb.lenLong);
      trick2.Set(QT_ACE, QT_PARD, lowestRank, cb.lenLong);
      return def.Set11(trick, trick2);
    }
  }
  else
  {
    // pOppHighest opponent doesn't matter.
    assert(cb.numTopsLow >= cb.lenCashLow);
    assert(cb.numTopsHigh >= cb.lenOppHighest);

    if (cb.minAce > cb.maxPard)
    {
      if (pickFlag) holdCtr[1006]++;
      lowestRank = Holding::ListToRank(
        completeList[QT_ACE][cb.lenCashLow - 1]);
      trick.Set(QT_BOTH, QT_ACE, lowestRank, cb.lenLong);
      return def.Set1(trick);
    }
    else if (cb.numTopsLongLow > 0 && cb.numTopsShortLow > 0)
    {
      // Both ace holder and partner have tops over pOppLowest.
      // Skip over lenCashLow top cards with declarer's side.
      unsigned na = 0, np = 0;
      for (unsigned no = 0; no < cb.lenCashLow; no++)
      {
        if (completeList[QT_ACE][na] > completeList[QT_PARD][np])
          na++;
        else
          np++;
      }

      unsigned pa = completeList[QT_ACE][na-1];
      unsigned pl = (np == 0 ? pa : Min(pa, completeList[QT_PARD][np-1]));

      unsigned m = Holding::ListToRank(cb.maxPard);
      r = Holding::ListToRank(pl);

      if (m >= r)
      {
        if (pickFlag) holdCtr[1007]++;
        lowestRank = r;
        trick.Set(QT_BOTH, QT_BOTH, lowestRank, cb.lenLong);
        return def.Set1(trick);
      }
      else
      {
        if (pickFlag) holdCtr[1008]++;
        lowestRank = Holding::ListToRank(cb.maxPard);
        r = Holding::ListToRank(pa);
        trick.Set(QT_BOTH, QT_ACE, r, cb.lenLong);
        trick2.Set(QT_ACE, QT_PARD, lowestRank, cb.lenLong);
        return def.Set11(trick, trick2);
      }
    }
    else
    {
      if (pickFlag) holdCtr[1009]++;
      r = Holding::ListToRank(completeList[QT_ACE][cb.lenCashLow - 1]);
      lowestRank = Holding::ListToRank(cb.maxPard);
      trick.Set(QT_BOTH, QT_ACE, r, cb.lenLong);
      trick2.Set(QT_ACE, QT_PARD, lowestRank, cb.lenLong);
      return def.Set11(trick, trick2);
    }
  }
}


bool LoopHold::CashoutBothDiffLength(
  DefList& def,
  unsigned& lowestRank,
  const CashoutBothDetails& cb) const
{
  if (cb.numTopsHigh < cb.lenCashHigh || cb.numTopsLow < cb.lenCashLow)
  {
    // Not enough tops for one or the other opponent.
    if (pickFlag) holdCtr[1020]++;
    return false;
  }
  else if (length[QT_PARD] > length[QT_ACE] && cb.minAce > cb.maxPard)
  {
    // Blocked.
    if (pickFlag) holdCtr[1021]++;
    return false;
  }

  Trick trick, trick2;
  unsigned r;

  if (cb.numTopsHigh >= cb.lenOppMax)
  {
    // Declarer has enough high tops to cash out both opponents.
    return LoopHold::CashoutBothDiffStrongTops(def, lowestRank, cb);
  }


  if (cb.numTopsLongLow > 0 && 
      cb.numTopsShortLow > 0 &&
      cb.lenOppLowest > 0 && 
      cb.numTopsHigh >= cb.lenOppHighest)
  {
    // if (pickFlag) holdCtr[921]++;
    // Holding::Print();
  }


  if (pickFlag) holdCtr[932]++;

  if (cb.numTopsLongHigh == 0 || cb.numTopsShortHigh == 0)
  {
    if (pickFlag) holdCtr[800]++;
    return false;
  }


  if (cb.numTopsLongLow + cb.numTopsShortHigh < cb.lenOppLowest)
  {
    if (pickFlag) holdCtr[804]++;
    return false;
  }
  else
  {
    if (cb.lenShort <= cb.numTopsHigh)
    {
      // This is the general case where we can also cash out the 
      // longer opponent with the lower highest card.  Our short
      // partner's intermediate cards are ignored, as they will fall.
      // Example: AJ / Qx / KTx / 987, the jack won't matter for here.

      int minorsNeeded = 
        static_cast<int>(
        Min(cb.lenOppLowest, cb.lenLong) - 
          cb.numTopsLongHigh - cb.numTopsShortHigh);

      // Skip over the long hand's tops.
      unsigned i = 0;
      while (i < cb.lenLong && completeList[cb.pLong][i] > cb.oppMaxHighest)
        i++;

      int c = 1; // We're at the first minor card.
      while (i < cb.lenLong && c < minorsNeeded)
      {
        i++;
        c++;
      }

      unsigned shortSecond = completeList[cb.pShort][cb.numTopsShortHigh];

if (shortSecond > completeList[cb.pLong][i])
{
  // AK98 / J / QT7 / 6543.  The T also cashes over the 6.
  // Skip for now.
  if (pickFlag) holdCtr[805]++;
  return false;
}
      if (pickFlag) holdCtr[706]++;
      assert(i < cb.lenLong);
      lowestRank = Holding::ListToRank(completeList[cb.pLong][i]);
      trick.Set(QT_BOTH, QT_BOTH, lowestRank, cb.lenLong);
      return def.Set1(trick);
    }
    else
    {
      assert(cb.numTopsHigh == 2);
      if (cb.lenOppHighest == 1)
      {
        if (cb.lenOppLowest == 3)
        {
          if (cb.lenShort == 4)
          {
            // Too complex, e.g. A765 / T98 / KJ432 / Q blocks.
            if (pickFlag) holdCtr[806]++;
            return false;
          }
          else if (completeList[cb.pLong][cb.numTopsLongHigh] <
                   completeList[cb.pShort][cb.numTopsShortHigh])
          {
            // Don't want the jack on the short side.
            if (pickFlag) holdCtr[807]++;
            return false;
          }
          else
          {
            // 4=3, 5=3, 6=3.
            if (pickFlag) holdCtr[707]++;
            lowestRank = Holding::ListToRank(
              completeList[cb.pLong][cb.numTopsLongHigh]);

            trick.Set(QT_BOTH, QT_BOTH, lowestRank, cb.lenLong);
            return def.Set1(trick);
          }
        }
        else // (lenOppLowest == 4 or 5)
        {
          if (completeList[cb.pLong][cb.numTopsLongHigh] <
                   completeList[cb.pShort][cb.numTopsShortHigh])
          {
            // Don't want the jack on the short side.
            if (pickFlag) holdCtr[808]++;
            return false;
          }
          else
          {
            // Need JT.
            if (pickFlag) holdCtr[708]++;
            lowestRank = Holding::ListToRank(
                   completeList[cb.pLong][cb.numTopsLongHigh] - 1);
            trick.Set(QT_BOTH, QT_BOTH, lowestRank, cb.lenLong);
            return def.Set1(trick);
          }
        }
      }
      else // lenOppHighest == 2
      {
        if (cb.lenOppLowest == 3)
        {
          if (completeList[cb.pLong][cb.numTopsLongHigh] <
                   completeList[cb.pShort][cb.numTopsShortHigh])
          {
            // Don't want the "jack" on the short side.
            if (pickFlag) holdCtr[809]++;
            return false;
          }
          else
          {
            if (pickFlag) holdCtr[709]++;
            lowestRank = Holding::ListToRank(
                   completeList[cb.pLong][cb.numTopsLongHigh]);
            trick.Set(QT_BOTH, QT_BOTH, lowestRank, cb.lenLong);
            return def.Set1(trick);
          }
        }
        else
        {
          // 4=2=3=4 with Qx, possibly QJ stiff.
// AJ7 / Q8 / KT93 / 6542, 13/0x9293fb, what's even right?
            if (pickFlag) holdCtr[810]++;
return false;

          lowestRank = Holding::ListToRank(
                 completeList[cb.pLong][cb.numTopsLongHigh + 1]);
          trick.Set(QT_BOTH, QT_BOTH, lowestRank, cb.lenLong);
          return def.Set1(trick);
        }
      }
    }
  }

  return false;
}


bool LoopHold::CashoutBothDiffStrongTops(
  DefList& def,
  unsigned& lowestRank,
  const CashoutBothDetails& cb) const
{
  // This function is not yet complete.

  Trick trick, trick2;
  unsigned r;

  assert(cb.lenLong > cb.lenOppMax);
  if (cb.numTopsLongHigh > 0 && cb.numTopsShortHigh > 0)
  {
    // Declarer has tops over pOppHighest on both sides.

    unsigned m = SDS_VOID - cb.lenOppMax;
    r = Holding::ListToRank(cb.maxPard);
    if (r >= m)
    {
      if (LoopHold::GetAsymmRanks(cb.pLong, cb.pShort, 
        cb.lenOppMax, cb.maxOpp, lowestRank))
      {
        if (pickFlag) holdCtr[1030]++;
        trick.Set(QT_BOTH, QT_BOTH, lowestRank, cb.lenLong);
        return def.Set1(trick);
      }
      else
      {
        if (pickFlag) holdCtr[1031]++;
        return false;
      }
    }
    else
    {
      if (pickFlag) holdCtr[1032]++;
      lowestRank = r;
      trick.Set(QT_BOTH, QT_BOTH, lowestRank, cb.lenLong);
      return def.Set1(trick);
    }
  }
  else if (length[QT_PARD] <= cb.lenOppHighest ||
    (length[QT_PARD] <= cb.lenOppLowest && cb.maxPard < cb.minOpp))
  {
    // Partner is shorter than a relevant opponent.
    if (pickFlag) holdCtr[1040]++;
    lowestRank = Holding::ListToRank(
      completeList[QT_ACE][cb.lenOppMax - 1]);
    trick.Set(QT_BOTH, QT_ACE, lowestRank, cb.lenLong);
    return def.Set1(trick);
  }
  else if (length[QT_PARD] > length[QT_ACE])
  {
    // Partner is longer than ace holder.
    if (pickFlag) holdCtr[1041]++;
    lowestRank = Holding::ListToRank(cb.maxPard);
    trick.Set(QT_BOTH, QT_BOTH, lowestRank, cb.lenLong);
    return def.Set1(trick);
  }
  else if (cb.maxPard < cb.minAce)
  {
    // Declarer cannot leave the ace side.
    if (pickFlag) holdCtr[1042]++;
    unsigned l = cb.lenOppMax + (cb.lenShort > cb.lenOppMax ? 1 : 0);
    lowestRank = Holding::ListToRank(completeList[QT_ACE][l - 1]);
    trick.Set(QT_BOTH, QT_ACE, lowestRank, cb.lenLong);
    return def.Set1(trick);
  }
  else if (length[QT_PARD] > cb.lenOppHighest + 1 &&
      cb.lenOppLowest < cb.lenOppHighest &&
      cb.pOppHighest == QT_LHO && 
      completeList[QT_ACE][cb.lenOppHighest] < cb.maxPard &&
      completeList[QT_ACE][cb.lenOppHighest] > 
        completeList[QT_PARD][length[QT_PARD]-1])
  {
    if (pickFlag) holdCtr[1043]++;
    lowestRank = Holding::ListToRank(
      completeList[QT_ACE][cb.lenOppHighest]);
    trick.Set(QT_BOTH, QT_BOTH, lowestRank, cb.lenLong);
    return def.Set1(trick);
  }
  else if (length[QT_PARD] > cb.lenOppHighest + 1 &&
      cb.lenOppLowest < cb.lenOppHighest &&
      completeList[QT_ACE][cb.lenOppHighest] > cb.maxPard)
  {
    // This is probably mostly correct.  But it exposes problems
    // in the recursion, so it is commented out for now.

    if (pickFlag) holdCtr[1044]++;
    return false;

    // Holding::Print();
    // r = Holding::ListToRank(completeList[QT_ACE][cb.lenOppHighest]);
    // lowestRank = Holding::ListToRank(cb.maxPard);
    // trick.Set(QT_BOTH, QT_ACE, r, cb.lenLong);
    // trick2.Set(QT_ACE, QT_PARD, lowestRank, cb.lenLong);
    // return def.Set11(trick, trick2);
  }
  else
  {
    if (pickFlag) holdCtr[1050]++;
    return false;
  }
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
  cb.lenCashLow = Min(cb.lenLong, cb.lenOppLowest);

  cb.minAce = completeList[QT_ACE][length[QT_ACE]-1];
  cb.maxPard = completeList[QT_PARD][0];

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


bool LoopHold::GetAsymmRanks(
  const PosType pLong,
  const PosType pShort,
  const unsigned cashLength,
  const unsigned toBeat,
  unsigned& lowestRank) const
{
  // First cash on the short side, then if necessary the long side.
  // After that, if the first unused card on the long side is large
  // enough, then for sure the suit doesn't block.  Otherwise it might.

  unsigned cashed = 0;
  while (cashed < length[pShort] && cashed < cashLength &&
      completeList[pShort][cashed] > toBeat)
  {
    cashed++;
  }

  unsigned shortStop = cashed;
  unsigned longStop = 0;

  if (cashed < cashLength)
  {
    while (longStop < length[pLong] && cashed < cashLength &&
        completeList[pLong][longStop] > toBeat)
    {
      cashed++;
      longStop++;
    }
  }

  // Couldn't cash enough tops.
  if (cashed < cashLength)
    return false;

  // Blocked for sure.
  if (completeList[pLong][longStop] < completeList[pShort][shortStop])
    return false;

  if (shortStop == 0)
  {
    // Might be QT_BOTH or pLong.  For now we give up.
    return false;
  }

  if (length[pShort] <= cashLength)
  {
    unsigned m = Min(
      completeList[pShort][shortStop-1],
      completeList[pLong][longStop-1]);
    lowestRank = Holding::ListToRank(m);
  }
  else
    lowestRank = Holding::ListToRank(completeList[pLong][longStop]);
  
  return true;
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
  const PosType oppSkipped)
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
  const PosType oppSkipped)
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


bool LoopHold::SolveStopped(
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
  if ((this->*SolveStoppedFunction[topIndex])(trick))
  {
    def.Set1(trick);
    rank = trick.GetRanks();
    return true;
  }
  else
    return false;
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


bool LoopHold::SolveStopped0(Trick& move) const
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
      if (pickFlag) holdCtr[0]++;
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
      if (pickFlag) holdCtr[1]++;
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
        if (pickFlag) holdCtr[2]++;
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
          if (pickFlag) holdCtr[3]++;
          return move.Set(QT_BOTH, QT_ACE, SDS_VOID-4, 4);
        }
      }
      else if (LoopHold::StopFinesse(2, 4, 0, true, QT_ACE))
      {
        // AKQJx+ / ? / x / xxxxx+.
        if (pickFlag) holdCtr[4]++;
        return move.Set(QT_BOTH, QT_ACE, SDS_VOID-4, 4);
      }
    }
    else if (LoopHold::StopFinesse(3, 4, 0, false, QT_ACE))
    {
      // AKQJx+ / ? / x / xxxxx+.
      if (pickFlag) holdCtr[5]++;
      return move.Set(QT_BOTH, QT_ACE, SDS_VOID-4, 4);
    }
  }
  return false;
}


bool LoopHold::SolveStopped1(Trick& move) const
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
    if (pickFlag) holdCtr[10]++;
    return move.Set(QT_BOTH, QT_ACE, SDS_VOID-1, 1);
  }
  else if (htop.Q == QT_LHO && length[QT_LHO] >= 3)
  {
    // Protected queen behind AK.
    if (pickFlag) holdCtr[11]++;
    return move.Set(QT_BOTH, QT_ACE, SDS_VOID-2, 2);
  }
  else if (htop.J == QT_LHO && length[QT_LHO] >= 4)
  {
    // Protected jack behind AKQ.
    if (pickFlag) holdCtr[12]++;
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
      if (pickFlag) holdCtr[13]++;
      unsigned r = Holding::ListToRank(completeList[QT_ACE][3]);
      return move.Set(QT_BOTH, QT_ACE, r, 4);
    }
    else if (completeList[QT_RHO][0] < completeList[QT_ACE][2] &&
      completeList[QT_RHO][0] > completeList[QT_ACE][3])
    {
      // AHH+ / H / x / Txxx.
      // AHH+ / H / T / 9xxx.
      // AHHx / Hx / x(x) / xxxx.
      if (pickFlag) holdCtr[14]++;
      return move.Set(QT_BOTH, QT_ACE, r3, 3);
    }
  }
  else if (length[QT_ACE] == 3)
  {
    if (completeList[QT_RHO][0] > completeList[QT_PARD][0])
    {
      // AHH / H / ? / Txxx.
      if (pickFlag) holdCtr[15]++;
      return move.Set(QT_BOTH, QT_ACE, r3, 3);
    }
  }
  else if (length[QT_PARD] == 2)
  {
    if (length[QT_RHO] >= 5 && htop.T == QT_ACE && hopp.N && 
      LoopHold::StopFinesse(1, 4, 0, false, QT_ACE))
    {
      // AHHT+ / H / xx / 9xxxx+.
      if (pickFlag) holdCtr[16]++;
      return move.Set(QT_BOTH, QT_ACE, SDS_VOID-5, 4);
    }
    else if (htop.T == QT_RHO &&
      LoopHold::StopFinesse(1, 3, 0, false, QT_ACE))
    {
      // AHHx+ / H / xx / Txxx+.
      if (pickFlag) holdCtr[17]++;
      return move.Set(QT_BOTH, QT_ACE, r3, 3);
    }
  }
  else if (htop.K == QT_LHO && htop.T == QT_RHO &&
    LoopHold::StopFinesse(2, 3, 0, false, QT_ACE))
  {
    // AHHx / H / xxx+ / Txxx.
    if (pickFlag) holdCtr[18]++;
    return move.Set(QT_BOTH, QT_ACE, r3, 3);
  }
  else if (htop.K == QT_ACE && length[QT_LHO] == 1 &&
    LoopHold::StopFinesse(3, 3, 0, false, QT_ACE))
  {
    // AKHx+ / H / xx+ / xxxx+.
    if (pickFlag) holdCtr[19]++;
    return move.Set(QT_BOTH, QT_ACE, r3, 3);
  }
  else if (distHex == 0x4234 && htop.K == QT_ACE &&
    completeList[QT_RHO][1] > completeList[QT_ACE][3] &&
    completeList[QT_RHO][0] > completeList[QT_PARD][0])
  {
    // AKHx / Hx / xxX / xxxx.
    if (pickFlag) holdCtr[40]++;
    return move.Set(QT_BOTH, QT_ACE, r3, 3);
  }
  return false;
}


bool LoopHold::SolveStopped2(Trick& move) const
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
          if (pickFlag) holdCtr[20]++;
          return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-4, 4);
        }
      }
      else if (distHex == 0x5035 &&
        LoopHold::StopFinesse(2, 3, 1, true, QT_ACE))
      {
        // AHHxx / - / Hxx / Txxxx.
        if (pickFlag) holdCtr[21]++;
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
        if (pickFlag) holdCtr[22]++;
        return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-4, 4);
      }
      else if (pardTops == 2 && 
        LoopHold::StopFinesse(2, 2, 2, true, QT_PARD))
      {
        // AHxxx / Txxxx / HHx / -.
        if (pickFlag) holdCtr[23]++;
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
          if (pickFlag) holdCtr[24]++;
          return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-4, 4);
        }
      }
      else if (distHex == 0x3550 &&
        LoopHold::StopFinesse(2, 1, 3, true, QT_PARD))
      {
        // Axx / Txxxx / KQJxx / -.
        if (pickFlag) holdCtr[25]++;
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
        if (pickFlag) holdCtr[26]++;
        return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-4, 4);
      }
      else if (pardTops == 2 && 
        LoopHold::StopFinesse(2, 2, 2, true, QT_ACE))
      {
        // AHx / - / HHxxx / Txxxx.
        if (pickFlag) holdCtr[27]++;
        return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-4, 4);
      }
    }
  }
  return false;
}


bool LoopHold::SolveStopped3(Trick& move) const
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
    if (pickFlag) holdCtr[30]++;
    return move.Set(QT_BOTH, QT_ACE, SDS_VOID-4, 3);
  }

  if (length[QT_LHO] == 4)
    return false;

  if (htop.T == QT_ACE && hopp.N)
  {
    // AQJT+ / 9xxxx+ / x+ / K.
    if (pickFlag) holdCtr[31]++;
    return move.Set(QT_BOTH, QT_ACE, SDS_VOID-5, 4);
  }
  else if (! hopp.T && ! hopp.N && htop.T != htop.N && hopp.E &&
    length[QT_PARD] == 1)
  {
    // AQJT+ / 8xxxx+ / 9 / K.
    // AQJ9+ / 8xxxx+ / T / K.
    if (pickFlag) holdCtr[32]++;
    unsigned r = Holding::ListToRank(completeList[QT_ACE][3]);
    return move.Set(QT_BOTH, QT_ACE, r, 4);
  }
  return false;
}


bool LoopHold::SolveStopped5(Trick& move) const
{
  // ==== G5 ============== G21 =========== G37 =========== G53 ========
  //      AJ+        |      A+       |      A+       |      A+
  // +          KQ+  |  +       KQJ+ |  +       KQ+  |  J+      KQ+
  //      +          |      +        |      J+       |      +
  // ==== G5 ============== G21 =========== G37 =========== G53 ========

  if (pickFlag) holdCtr[50]++;
  return move.Set(QT_BOTH, QT_ACE, SDS_VOID-1, 1);
}


bool LoopHold::SolveStopped6(Trick& move) const
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
    if (pickFlag) holdCtr[59]++;
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
    if (pickFlag) holdCtr[58]++;
    unsigned r = Holding::ListToRank(completeList[pa][2]);
    return move.Set(QT_BOTH, QT_BOTH, r, length[pa]);
  }
  else if (length[pa] >= 4 && length[pp] == length[pa] &&
    length[pl] == 3 && length[pr] <= 3 && htop.T == pa)
  {
    // AJTx+ / Q(xx) / Kxxx+ / (xxx).
    if (pickFlag) holdCtr[57]++;
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
    if (pickFlag) holdCtr[56]++;
    return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-6, 4);
  }
  else if (length[pa] == 4 && length[pr] == 4 && 
    length[pp] == 4 && length[pl] == 1 &&
    htop.T == pr && htop.N == pp && htop.E == pp)
  {
    // AJxx / Q / K98x / Txxx.
    // A98x / Txxx / KJxx / Q.
    if (pickFlag) holdCtr[55]++;
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
    if (pickFlag) holdCtr[54]++;
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
    if (pickFlag) holdCtr[53]++;
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
          if (pickFlag) holdCtr[60]++;
          return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-5, 4);
        }
      }
      else if (length[pa] >= 3 && 
        htop.T == pr && length[pr] >= 4 &&
        completeList[pr][1] > completeList[pa][2])
      {
        // AJx+ / Q / Kx / Txxx+.
        if (pickFlag) holdCtr[61]++;
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
        if (pickFlag) holdCtr[62]++;
        return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-4, 3);
      }
    }
    else if (completeList[pl][1] > completeList[pa][2] &&
      ((length[pr] >= 2 && htop.T == pr) ||
       completeList[pl][1] > completeList[pp][1]))
    {
      // AJx / Qxx+ / Kx / Tx+.
      if (pickFlag) holdCtr[63]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-2, 2);
    }
    else if (length[pr] >= 3 && htop.T == pr &&
      completeList[pr][1] > completeList[pa][2])
    {
      // AJx / Qxx / Kx / Txx.
      if (pickFlag) holdCtr[64]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-2, 2);
    }

    return false;
  }

  if (length[pa] == 2)
  {
    if (length[pl] >= 3 && completeList[pl][1] > completeList[pp][1])
    {
      // AJ / QTx+ / K+ / ?.
      if (pickFlag) holdCtr[65]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-2, 2);
    }
    else if (length[pl] >= 2 && length[pr] >= 3 &&
      completeList[pr][0] > completeList[pp][1])
    {
      // AJ / Qx+ / K+ / Txx+.
      if (pickFlag) holdCtr[66]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-2, 2);
    }
    else if (length[pl] == 2 && length[pr] >= 4 &&
      completeList[pr][0] < completeList[pp][1] &&
      completeList[pr][0] > completeList[pp][2])
    {
      // AJ / Qx / Kxx+ / xxxx.
      if (pickFlag) holdCtr[67]++;
      unsigned r = Holding::ListToRank(completeList[pp][1]);
      return move.Set(QT_BOTH, QT_BOTH, r, 3);
    }

    return false;
  }

  if (length[pl] == 1)
  {
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
          if (pickFlag) holdCtr[68]++;
          return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-4, 3);
        }
      }
      else if ((pa == QT_ACE && LoopHold::StopFinesse(2, 2, 1, true, pa))
           || (pa == QT_PARD && LoopHold::StopFinesse(2, 1, 2, true, pa)))
      {
        // AJx+ / Q / Kxx+ / T98x+, T97x+, T87x, etc.
        if (pickFlag) holdCtr[69]++;
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
      if (pickFlag) holdCtr[460]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-4, 3);
    }
    else if (length[pa] == 3 && length[pp] == 4 &&
    completeList[pr][0] > completeList[pp][1])
    {
      // AJx / Qx / Kxxx / 9xxx or so.
      if (pickFlag) holdCtr[461]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-4, 3);
    }
    else if (length[pa] == 4 && length[pp] == 3 &&
      completeList[pr][1] > completeList[pa][2] &&
      completeList[pr][0] > completeList[pp][1])
    {
      // AJxx / Qx / Kxx / T9xx.
      if (pickFlag) holdCtr[462]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-4, 3);
    }

    return false;
  }

  if (completeList[pl][1] > completeList[pa][2] &&
    completeList[pl][1] > completeList[pp][1])
  {
    // AJ+ / QTx+ / Kxx+ / ?.
    // AJ+ / Q9x+ / Kxx+ / T+.
    if (pickFlag) holdCtr[463]++;
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
      if (pickFlag) holdCtr[464]++;
      unsigned r = Holding::ListToRank(completeList[pa][2]);
      return move.Set(QT_BOTH, QT_BOTH, r, 3);
    }
    else if (((htop.Q == QT_LHO && distHex == 0x4432) ||
      (htop.Q == QT_RHO && distHex == 0x3244)) && 
      completeList[pl][1] > completeList[pa][3] &&
      completeList[pl][1] > completeList[pp][1])
    {
      // AJxx / Qxxx / Kxx / Tx.
      if (pickFlag) holdCtr[465]++;
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
      if (pickFlag) holdCtr[466]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-2, 2);
    }
  }
  return false;
}


bool LoopHold::SolveStopped7(Trick& move) const
{
  // ==== G7 ============== G13 =========== G39 =========== G45 ========
  //      AJ+        |      AJ+      |      A+       |      A+
  // K+         Q+   | Q+       K+   |  K+      Q+   |  Q+      K+ 
  //      +          |      +        |      J+       |      J+
  // ==== G7 ============== G13 =========== G39 =========== G45 ========

  if (length[QT_LHO] >= 2 || length[QT_RHO] >= 2)
  {
    if (pickFlag) holdCtr[70]++;
    return move.Set(QT_BOTH, QT_ACE, SDS_VOID-1, 1);
  }
  else if (length[QT_PARD] == 1)
  {
    // Ax / K / x / x.
    // This and the following are tops, not real stops.
    if (pickFlag) holdCtr[71]++;
    return move.Set(QT_BOTH, QT_ACE, SDS_VOID-1, length[QT_ACE]);
  }
  else if (length[QT_ACE] == 2 && length[QT_PARD] == 2)
  {
    if (htop.J == QT_ACE)
    {
      // AJ / K / xx / Q.
      if (pickFlag) holdCtr[72]++;
      return move.Set(QT_BOTH, QT_ACE, SDS_VOID-1, 2);
      // return move.Set(QT_BOTH, QT_ACE, SDS_VOID-4, 2);
    }
    else if (htop.T == QT_PARD)
    {
      return false;
      // A9 / K / JT / Q.
      if (pickFlag) holdCtr[73]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-4, 2);
    }
  }
  return false;
}


bool LoopHold::SolveStopped9(Trick& move) const
{
  // ==== G9 ================= G33 =====================================
  //      AJ+        |         AQ+    
  // +          K+   |    +          K+
  //      Q+         |         J+      
  // ==== G9 ================= G33 =====================================

  if (length[QT_LHO] >= 2)
  {
    // Protected K behind the ace.
    if (pickFlag) holdCtr[90]++;
    return move.Set(QT_BOTH, QT_ACE, SDS_ACE, 1);
  }

  if (length[QT_ACE] == 2 && length[QT_PARD] <= 2 && htop.Q == QT_ACE)
  {
    if (length[QT_RHO] >= 2)
    {
      // AQ / K / J(x) / xx+.
      if (pickFlag) holdCtr[91]++;
      return move.Set(QT_BOTH, QT_ACE, SDS_VOID-3, 2);
    }
    else
    {
      // AQ / K / J(x) / (x).
      if (pickFlag) holdCtr[92]++;
      return move.Set(QT_BOTH, QT_ACE, SDS_VOID-1, 2);
    }
  }

  if (length[QT_PARD] == 1)
  {
    if (htop.T == QT_RHO && length[QT_RHO] >= 3)
    {
      // AJ+ / K / Q / Txx+.
      if (pickFlag) holdCtr[93]++;
      unsigned r = Holding::ListToRank(completeList[QT_ACE][1]);
      return move.Set(QT_BOTH, QT_ACE, r, 2);
    }
    else if (htop.T == QT_ACE && htop.N == QT_RHO && length[QT_RHO] >= 4)
    {
      // AJT+ / K / Q / 9xxx+.
      if (pickFlag) holdCtr[94]++;
      return move.Set(QT_BOTH, QT_ACE, SDS_VOID-5, 3);
    }
    else if (htop.T == QT_ACE && htop.N == QT_ACE &&
      htop.E == QT_RHO && length[QT_RHO] >= 5)
    {
      // AJT9+ / K / Q / 8xxxx+.
      if (pickFlag) holdCtr[95]++;
      return move.Set(QT_BOTH, QT_ACE, SDS_VOID-6, 4);
    }

    return false;
  }

  if (length[QT_PARD] <= 2 || length[QT_ACE] <= 2 || 
    length[QT_RHO] <= 3 || htop.T != QT_RHO)
    return false;

  if (distHex == 0x4144 && htop.N == QT_PARD && htop.E == QT_PARD)
  {
    // AQxx / K / J98x / Txxx.
    if (pickFlag) holdCtr[96]++;
    return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-7, 4);
  }
  else if ((length[QT_ACE] == 3 && length[QT_PARD] == 3) ||
    LoopHold::StopFinesse(1, 2, 1, false, QT_ACE))
  {
    // AJx / K / Qxxx+ / Txxx+.
    // AJxx / K / Qxxx / Txxx.
    // AJxxx / K / Qxx / Txxx.
    if (pickFlag) holdCtr[97]++;
    return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-4, 3);
  }
  return false;
}


bool LoopHold::SolveStopped11(Trick& move) const
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
      if (pickFlag) holdCtr[110]++;
      return move.Set(QT_BOTH, QT_ACE, SDS_VOID-3, 2);
    }
    else
    {
      if (pickFlag) holdCtr[111]++;
      return move.Set(QT_BOTH, QT_ACE, SDS_VOID-1, 2);
    }
  }

  if (length[QT_ACE] == 2 && length[QT_PARD] <= 2 && 
    (htop.Q == QT_ACE || length[QT_PARD] == 1))
  {
return false;
// Probably simplifies to 2=1 with AJ+ / Q.
    // AQ / + / J(x) / K.
    if (pickFlag) holdCtr[110]++;
    unsigned r = (length[QT_LHO] <= 1 && length[QT_PARD] <= 1 ?
      SDS_VOID-1 : SDS_VOID - (htop.Q == QT_ACE ? 3 : 4));
    return move.Set(QT_BOTH, QT_ACE, r, 2);
  }
  else if (length[QT_LHO] <= 2 && length[QT_PARD] == 1 && 
    htop.Q == QT_ACE)
  {
    // AQ+ / x(x) / J(x) / K.
    if (pickFlag) holdCtr[111]++;
    unsigned r = (length[QT_LHO] <= 1 && length[QT_PARD] <= 1 ?
      SDS_VOID-1 : SDS_VOID-3);
    return move.Set(QT_BOTH, QT_ACE, r, length[QT_ACE]);
  }
  else if (length[QT_PARD] == 1)
  {
    if (length[QT_LHO] <= 2)
    {
      // AH / (x) / H / H.
      if (pickFlag) holdCtr[112]++;
      unsigned r = (length[QT_LHO] == 2 ? 
        Holding::ListToRank(completeList[QT_ACE][1]) : SDS_VOID-1);
      return move.Set(QT_BOTH, QT_ACE, r, length[QT_ACE]);
    }
    else if (htop.T == QT_LHO)
    {
      // AH+ / Txx+ / H / K.
      if (pickFlag) holdCtr[113]++;
      unsigned r = Holding::ListToRank(completeList[QT_ACE][1]);
      return move.Set(QT_BOTH, QT_ACE, r, 2);
    }
    else if (htop.T == QT_ACE && htop.N == QT_LHO && length[QT_LHO] >= 4)
    {
      // AQT+ / K / J / 9xxx+.
      if (pickFlag) holdCtr[114]++;
      return move.Set(QT_BOTH, QT_ACE, SDS_VOID-5, 3);
    }
    else if (htop.T == QT_ACE && htop.N == QT_ACE &&
      htop.E == QT_LHO && length[QT_LHO] >= 5)
    {
      // AQT9+ / K / J / 8xxxx+.
      if (pickFlag) holdCtr[115]++;
      return move.Set(QT_BOTH, QT_ACE, SDS_VOID-6, 4);
    }
  }
  else if (distHex == 0x4441 && htop.T == QT_LHO && 
    (htop.N == QT_PARD || (htop.N == QT_ACE && ! hopp.E)))
  {
    // AJxx / Txxx / Q9xx / K.
    // AJ9x / Txxx / Q8xx / K.
    // AJ98 / Txxx / Qxxx / K.
    if (pickFlag) holdCtr[116]++;
    unsigned r = (htop.N == QT_PARD ? SDS_VOID-6 : SDS_VOID-7);
    return move.Set(QT_BOTH, QT_BOTH, r, 4);
  }
  else if (length[QT_ACE] >= 3 && length[QT_LHO] >= 4 &&
    length[QT_PARD] >= 3 && htop.T == QT_LHO &&
    (htop.N == QT_LHO || (htop.N == QT_ACE && htop.E == QT_LHO)))
  {
    // AHxx / Txxx / Hxx / K.
    if (pickFlag) holdCtr[117]++;
    return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-4, 3);
  }
  return false;
}


bool LoopHold::SolveStopped12(Trick& move) const
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
      if (pickFlag) holdCtr[120]++;
      return move.Set(QT_BOTH, QT_ACE, SDS_VOID-4, 3);
    }
  }
  else if (completeList[QT_LHO][0] > completeList[QT_ACE][3])
  {
    if (length[QT_PARD] == 1 ||
      completeList[QT_LHO][0] > completeList[QT_PARD][0])
    {
      // AKJx+ / Txxx+ / ? / Q(x).
      if (pickFlag) holdCtr[121]++;
      return move.Set(QT_BOTH, QT_ACE, SDS_VOID-4, 3);
    }
    else if (length[QT_PARD] == 2 && length[QT_RHO] == 2 &&
      (hopp.T || hopp.N))
    {
      // AKJx+ / Txxx+ / xx / Qx.
      if (pickFlag) holdCtr[122]++;
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
      if (pickFlag) holdCtr[123]++;
      unsigned r = Holding::ListToRank(completeList[QT_ACE][3]);
      return move.Set(QT_BOTH, QT_ACE, r, 4);
    }
  }
  return false;
}


bool LoopHold::SolveStopped14(Trick& move) const
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

  if (length[pa] == 4 && length[pp] == 4 && length[pr] >= 4 && 
    htop.T == pp && htop.N == pp)
  {
    // We're going to limit this one for now.
    // AJxx / + / KT9x / Q+.
    // if (pickFlag) holdCtr[146]++;
    // return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-6, 4);
    return false;
  }
  else if (length[pa] == 4 && length[pl] == 1 && length[pp] == 4 &&
    length[pr] == 4 && htop.T == pl && htop.N == pp && htop.E == pp)
  {
    return false;
    // AJxx / T / K98x / Qxxx.
    if (pickFlag) holdCtr[148]++;
    return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-7, 4);
  }
  else if (length[pa] == 4 && length[pl] == 1 && length[pp] == 4 &&
    length[pr] == 4 && htop.T == pp && htop.N == pl && htop.E == pp)
  {
    return false;
    // AT8x / Qxxx / KJxx / 9.
    // AJxx / 9 / KT8x / Qxxx.
    if (pickFlag) holdCtr[381]++;
    return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-7, 4);
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
        if (pickFlag) holdCtr[140]++;
        return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-4, 3);
      }
      else if (htop.T == pl &&
        (htop.N == pl || (htop.N == pa && htop.E == pl)))
      {
         // AJ9x / T8xx / Kxx / Q.
        if (pickFlag) holdCtr[141]++;
        return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-4, 3);
      }
      else if (length[pa] == 4 && length[pp] == 4 && length[pl] == 4 &&
        ! hopp.N)
      {
        if (htop.N == pp)
        {
          // AJxx / T8xx / K9xx / Q.
          if (pickFlag) holdCtr[380]++;
          return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-6, 4);
        }
        else if (! hopp.E)
        {
          // AJ9x / Txxx / K8xx / Q.
          // AJ8x / Txxx / K9xx / Q.
          if (pickFlag) holdCtr[147]++;
          return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-7, 4);
        }
      }
    }
    else if (length[pa] >= 5 && length[pl] >= 5 && htop.T == pa && hopp.N)
    {
      // AJTxx / 9xxxx / Kx / Q.
      if (pickFlag) holdCtr[142]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-5, 4);
    }
  }
  else if (htop.T == pl || (htop.T == pr && htop.N == pl))
  {
    if (length[pp] <= 3)
    {
      // AJ65 / T743 / K98 / Q2.
      if (pickFlag) holdCtr[143]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-4, 3);
    }
    else if (length[pa] == 3 && completeList[pl][1] > completeList[pp][1])
    {
      // AJ9 / T654 / K732 / Q8.
      if (pickFlag) holdCtr[144]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-4, 3);
    }
  }
  else if (length[pa] >= 4 && length[pp] == 2 &&
    completeList[pl][0] > completeList[pa][2])
  {
    // AJxx / 9xxx / KT / Qx, etc.
    if (pickFlag) holdCtr[145]++;
    return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-4, 3);
  }
  return false;
}


bool LoopHold::SolveStopped15(Trick& move) const
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
    if (pickFlag) holdCtr[150]++;
    return move.Set(QT_BOTH, QT_ACE, SDS_VOID-1, 1);
  }
  else if (fourTops == 2 && hopp.K)
  {
    if (htop.K == QT_LHO && htop.J == QT_LHO)
    {
      if (pickFlag) holdCtr[151]++;
      return move.Set(QT_BOTH, QT_ACE, SDS_VOID-1, 1);
    }
    else if (htop.K == QT_RHO)
    {
      if (htop.Q == QT_RHO || (htop.Q == QT_PARD && htop.J == QT_RHO))
      {
        if (pickFlag) holdCtr[152]++;
        return move.Set(QT_BOTH, QT_ACE, SDS_VOID-1, 1);
      }
    }
  }

  return false;
}


bool LoopHold::SolveStopped18(Trick& move) const
{
  // ==== G18 ================ G24 ================ G58 ================
  //      AQ+        |         AK+        |         A+
  // +          J+   |   +           J+   |  J+            +
  //      K+         |         Q+         |         KQ+
  // ==== G18 ================ G24 ================ G58 ================

  if ((length[QT_ACE] <= 3 && length[QT_PARD] <= 3) ||
      (length[QT_LHO] <= 3 && length[QT_RHO] <= 3))
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
          if (pickFlag) holdCtr[180]++;
          return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-5, 4);
        }
      }
      else if (hopp.T && 
        (rnum == 2 || (htop.N == pp && htop.E == pr)))
      {
        // AHxx+ / h / Hx / hxxx+ with enough middle cards.
        if (pickFlag) holdCtr[181]++;
        return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-3, 3);
      }
    }
    else if (length[pa] == 3)
    {
      if (hopp.T && LoopHold::StopFinesse(1, 2, 1, true, QT_ACE))
      {
        // AHx / h / Hxxx+ / hxxx+.
        if (pickFlag) holdCtr[182]++;
        return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-3, 3);
      }
    }
    else if (length[pa] == 4 && length[pl] == 1 && length[pp] == 4 &&
      length[pr] == 4 && htop.T == pr && htop.N == pp && htop.E == pp)
    {
      // AKxx / J / Q98x / Txxx.
      if (pickFlag) holdCtr[244]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-7, 4);
    }
    else if (hopp.T && LoopHold::StopFinesse(2, 2, 1, true, QT_ACE))
    {
      // AHxx+ / h / Hxx+ / hxxx+.
      if (pickFlag) holdCtr[183]++;
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
        if (pickFlag) holdCtr[184]++;
        return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-3, 3);
      }
    }
    else if (length[pp] == 2 && length[pr] >= 4 &&
      htop.T == pl && htop.N == pp && htop.E == pr)
    {
      // AKxx / JT / Q9 / 8xxx.
      if (pickFlag) holdCtr[185]++;
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
    if (pickFlag) holdCtr[249]++;
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
    if (pickFlag) holdCtr[248]++;
    return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-7, 4);
  }
  else if (length[pl] >= 4 && htop.J == pl &&
    (htop.T == pl || (htop.T == pa && htop.N == pl)))
  {
    // AHx+ / JTxx+ / Hx+ / ?.
    // AHT+ / J9xx+ / Hx+ / ?.
    if (pickFlag) holdCtr[186]++;
    return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-3, 3);
  }
  else if (length[pa] >= 4 && length[pl] >= 4 &&
    length[pp] == 2 && length[pr] >= 2 &&
    htop.J == pl && htop.T == pa &&
    completeList[pl][1] > completeList[pa][3] &&
    completeList[pr][0] > completeList[pp][1])
  {
    // AKTx / Jxxx / Qx / xx.
    if (pickFlag) holdCtr[187]++;
    return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-3, 3);
  }
  else if (length[pr] == 1)
  {
    if (length[pl] >= 4 && hopp.T && rnum >= 1)
    {
      if (length[pp] == 2)
      {
        // AHxx / hxxx+ / Hx / h.
        if (pickFlag) holdCtr[188]++;
        return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-3, 3);
      }
      else if ((htop.N == pl || 
        (htop.N == pa && htop.E == pl)))
      {
        // AHxx / hxxx+ / Hxx+ / h.
        if (pickFlag) holdCtr[189]++;
        return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-3, 3);
      }
    }
  }
  else if (hopp.T && rnum >= 1)
  {
    if (length[pr] >= 3)
    {
      // AHx+ / hxx+ / Hx+ / hxx+.
      if (pickFlag) holdCtr[240]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-3, 3);
    }
    else if (length[pp] <= 3)
    {
      // AHxx / hxxx / Hx / hx.
      // AHxx / hxxx / Hxx / hx.
      if (pickFlag) holdCtr[241]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-3, 3);
    }
    else if (completeList[pl][1] > completeList[pp][1])
    {
      // AHx / hxxx / Hxxx / hx.
      if (pickFlag) holdCtr[242]++;
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
      if (pickFlag) holdCtr[243]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-3, 3);
    }
  }
  return false;
}


bool LoopHold::SolveStopped19(Trick& move) const
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
    if (pickFlag) holdCtr[190]++;
    return move.Set(QT_BOTH, QT_ACE, SDS_VOID-3, 2);
  }
  else if (length[QT_PARD] <= 1)
  {
    if (pickFlag) holdCtr[191]++;
    unsigned r = (length[QT_LHO] <= 1 ? SDS_VOID-1 : SDS_VOID-3);
    return move.Set(QT_BOTH, QT_ACE, r, length[QT_ACE]);
  }
  else if (length[QT_PARD] == 2 && length[QT_ACE] == 2)
  {
    if (pickFlag) holdCtr[192]++;
    unsigned r = Holding::ListToRank(completeList[QT_ACE][length[QT_LHO]-1]);
    return move.Set(QT_BOTH, QT_ACE, r, length[QT_ACE]);
  }
  return false;
}


bool LoopHold::SolveStopped20(Trick& move) const
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
    if (pickFlag) holdCtr[200]++;
    return move.Set(QT_BOTH, pend, SDS_VOID-2, 2);
  }
  else if (length[QT_ACE] >= 3 &&
    length[QT_PARD] <= 2 && length[QT_RHO] >= 4 && 
    completeList[QT_RHO][0] > completeList[QT_ACE][x+1] &&
    completeList[QT_RHO][0] < completeList[QT_ACE][x])
  {
    // AKxx / QJ / xx / xxxx.
    // Axxx / QJ / Kx / xxxx.
    if (pickFlag) holdCtr[201]++;
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
    if (pickFlag) holdCtr[202]++;
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
      if (pickFlag) holdCtr[203]++;
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
      if (pickFlag) holdCtr[204]++;
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
      if (pickFlag) holdCtr[205]++;
      unsigned r = Holding::ListToRank(completeList[QT_PARD][1]);
      return move.Set(QT_BOTH, QT_BOTH, r, 3);
    }
    else if (length[QT_ACE] == 3 && htop.T == QT_ACE &&
      completeList[QT_RHO][0] > completeList[QT_PARD][1])
    {
      // ATx / QJ / Kxxx / 9xxx.
      if (pickFlag) holdCtr[206]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-5, 3);
    }
  }
  else if (distHex == 0x3244 && htop.K == QT_ACE && ! hopp.T && hopp.N)
  {
    // AKT / QJ / xxxx / 9xxx.
    // AKx / QJ / Txxx / 9xxx.
    if (pickFlag) holdCtr[207]++;
    PosType e = (htop.T == QT_ACE ? QT_ACE : QT_BOTH);
    return move.Set(QT_BOTH, e, SDS_VOID-5, 3);
  }
  return false;
}


bool LoopHold::SolveStopped26(Trick& move) const
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
    if (pickFlag) holdCtr[260]++;
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
      if (pickFlag) holdCtr[261]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-3, 3);
    }
    else if (length[pr] >= 4 &&
      completeList[pr][0] > completeList[pp][2] &&
      (htop.T == pr || length[pl] >= 2))
    {
      // AT / Jx+ / KQxx / 9xxx.
      if (pickFlag) holdCtr[262]++;
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
    if (pickFlag) holdCtr[263]++;
    return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-3, 3);
  }
  else if (length[pl] >= 4 && length[pr] <= 2 && htop.J == pl && 
    (((pp == QT_ACE && LoopHold::StopFinesse(2, 2, 1, true, pp)) ||
      (pp == QT_PARD && LoopHold::StopFinesse(2, 1, 2, true, pp)))))
  {
    // AT8x / J9xx / KQx / xx.
    if (pickFlag) holdCtr[264]++;
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
        if (pickFlag) holdCtr[265]++;
        return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-3, 3);
      }
    }
    else if (length[pl] >= 4 && length[pr] >= 2 && 
      htop.J == pl && htop.T == pl && htop.N == pa && hopp.E)
    {
      // Axx+ / JTxx / KQx / xx.
      if (pickFlag) holdCtr[266]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-3, 3);
    }
  }
  else if (length[pa] == 4 && length[pp] == 4 && length[pl] >= 4 && 
    htop.T == pa && (htop.N == pa || (htop.N == pr && length[pr] == 1 &&
      htop.E == pa)))
  {
    // AT9x / J+ / KQxx / +.
    // AT8x / J+ / KQxx / 9.
    if (pickFlag) holdCtr[267]++;
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
    if (pickFlag) holdCtr[268]++;
    unsigned r = (htop.N == pa ? SDS_VOID - 6 : SDS_VOID - 7);
    return move.Set(QT_BOTH, QT_BOTH, r, 4);
  }
  return false;
}


bool LoopHold::SolveStopped27(Trick& move) const
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
    if (pickFlag) holdCtr[270]++;
    return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-6, 3);
  }
  else if (length[QT_ACE] == 3 && length[QT_LHO] >= 3 &&
    length[QT_PARD] == 3 && length[QT_RHO] == 1 &&
    htop.T == QT_ACE && htop.N == QT_ACE)
  {
    // AT9 / Jxx+ / Qxx / K.
    if (pickFlag) holdCtr[278]++;
    return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-6, 3);
  }
  else if (distHex == 0x4341 && htop.T == QT_ACE && htop.N == QT_ACE)
  {
    // AT9x / Jxx / Qxxxx / K.
    if (pickFlag) holdCtr[277]++;
    return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-6, 4);
  }
  else if (distHex == 0x4441 && ! hopp.T && ! hopp.N && ! hopp.E)
  {
    unsigned rnum = 0;
    if (htop.T == QT_ACE) rnum++;
    if (htop.N == QT_ACE) rnum++;
    if (htop.E == QT_ACE) rnum++;

    // if (rnum >= 2 || (rnum == 1 && htop.E == QT_PARD))
    // if (rnum >= 2)
    if (rnum == 3)
    {
      // AT98 / Jxxx / Qxxx / K.
      // Ahhx / Jxxx / Qhxx / K.
      if (pickFlag) holdCtr[279]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-7, 4);
    }
    else if (rnum == 1 && htop.E == QT_ACE &&
      completeList[QT_ACE][2] > completeList[QT_LHO][1] &&
      completeList[QT_ACE][2] > completeList[QT_PARD][3])
    {
      return false;
      // A87x / Jxxx / QT9x / K.
      if (pickFlag) holdCtr[276]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-8, 4);
    }
    // Ahxx / Jxxx / Qh8x / K.
    // A87x / Jxxx / QT9x / K.
  }


  if (length[QT_RHO] >= 2)
  {
    if (length[QT_LHO] >= 2 || hopp.T)
    {
      // K not finessable.
      if (pickFlag) holdCtr[271]++;
      return move.Set(QT_BOTH, QT_ACE, SDS_ACE, 1);
    }
  }
  else if (length[QT_PARD] == 1 && length[QT_LHO] >= 2)
  {
    // K not finessable.
    if (pickFlag) holdCtr[272]++;
    return move.Set(QT_BOTH, QT_ACE, SDS_ACE, 1);
  }
  else if (length[QT_LHO] == 1 && length[QT_PARD] == 1)
  {
    // Ax+ / K / x / J.
    if (pickFlag) holdCtr[273]++;
    return move.Set(QT_BOTH, QT_ACE, SDS_ACE, length[QT_ACE]);
  }
  else if (length[QT_ACE] == 2 && length[QT_PARD] == 2 &&
    length[QT_LHO] >= 2)
    // (htop.T == QT_PARD || length[QT_LHO] >= 2))
  {
    // Ax / J / QT / K.
    // Ax / Jx+ / Qx / K.
    if (pickFlag) holdCtr[274]++;
    return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-3, 2);
  }
  else if (length[QT_LHO] >= 3 && (length[QT_PARD] == 2 ||
    ((completeList[QT_LHO][1] > completeList[QT_PARD][1]) &&
      completeList[QT_LHO][1] > completeList[QT_ACE][2])))
  {
    // Ax+ / Jxx+ / Qx+ / K.
    if (pickFlag) holdCtr[275]++;
    return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-3, 2);
  }
  return false;
}


bool LoopHold::SolveStopped28(Trick& move) const
{
  // ==== G28 ============= G30 =========== G52 =========== G54 ========
  //      AK+        |      A+       |      AK+      |      A+
  // Q+         J+   |  Q+      J+   |  J+      Q+   |  J+      Q+ 
  //      +          |      K+       |      +        |      +
  // ==== G28 ============= G30 =========== G52 =========== G54 ========

  if (length[QT_LHO] <= 2 && length[QT_RHO] <= 2)
    return false;

  //
  // Should rotate to have the stiff opposing honor with pr.
  //

  PosType pend = (htop.K == QT_ACE ? QT_ACE : QT_BOTH);

  if (pend == QT_BOTH && distHex == 0x4441 && 
    ! hopp.T && ! hopp.N && ! hopp.E &&
    (htop.T == QT_ACE && htop.N == QT_ACE && htop.E == QT_ACE))
    // (htop.T == QT_ACE || htop.N == QT_ACE))
  {
    // Axxx / Hxxx / Kxxx / H with T98, T9, T8, 98, T, 9 with ace.
    if (pickFlag) holdCtr[302]++;
    return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-7, 4);
  }
  else if (pend == QT_BOTH && ! hopp.T && htop.T == htop.N &&
    length[QT_ACE] >= 3)
  {
    if (htop.T == QT_ACE && htop.E == QT_LHO &&
      length[QT_LHO] >= 4 && length[QT_PARD] >= 3 && length[QT_RHO] == 1)
    {
      // AT9+ / H8xx+ / Kxx+ / H.
      if (pickFlag) holdCtr[280]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-6, 3);
    }
    else if (htop.T == QT_PARD && htop.E == QT_RHO &&
      length[QT_RHO] >= 4 && length[QT_PARD] >= 3 && length[QT_LHO] == 1)
    {
      // Axx+ / H / KT9+ / H8xx+.
      if (pickFlag) holdCtr[281]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-6, 3);
    }
    else if (length[QT_ACE] == 3 && length[QT_PARD] == 3 &&
      ((htop.T == QT_ACE && length[QT_RHO] == 1 && length[QT_LHO] >= 3) ||
       (htop.T == QT_PARD && length[QT_LHO] == 1 && length[QT_RHO] >= 3)))
    {
      // AT9 / Hxx+ / Kxx / H.
      // Axx / H / KT9 / Hxx+.
      if (pickFlag) holdCtr[282]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-6, 3);
    }
    else if (length[QT_ACE] == 4 && length[QT_PARD] == 4 &&
      ((htop.T == QT_ACE && length[QT_RHO] == 1 && length[QT_LHO] == 3) ||
       (htop.T == QT_PARD && length[QT_LHO] == 1 && length[QT_RHO] == 3)))
    {
      // AT9x / Hxx / Kxxx / H.
      // Axxx / H / KT9x / Hxx.
      if (pickFlag) holdCtr[301]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-6, 4);
    }
    else if (distHex == 0x4144 && 
      htop.T == QT_PARD && htop.N == QT_PARD && htop.E == QT_PARD)
    {
      // Axxx / H / KT98 / Hxxx.
      if (pickFlag) holdCtr[302]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-7, 4);
    }
    else if (distHex == 0x4144 &&
      htop.T == QT_ACE && htop.N == QT_ACE && htop.E == QT_PARD &&
      completeList[QT_PARD][2] > completeList[QT_ACE][3] &&
      completeList[QT_PARD][2] > completeList[QT_RHO][1])
    {
      return false;
      // AT9x / H / K87x / Hxxx.
      if (pickFlag) holdCtr[303]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-8, 4);
    }
    else if (distHex == 0x4441 &&
      htop.T == QT_PARD && htop.N == QT_PARD && htop.E == QT_ACE &&
      completeList[QT_ACE][2] > completeList[QT_PARD][3] &&
      completeList[QT_ACE][2] > completeList[QT_LHO][1])
    {
      return false;
      // A87x / Hxxx / KT9x / H.
      if (pickFlag) holdCtr[305]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-8, 4);
    }
  }
  else if (pend == QT_BOTH && 
    length[QT_ACE] == 4 && length[QT_PARD] == 4 && length[QT_LHO] >= 4 &&
    ! hopp.T && ! hopp.N && ! hopp.E)
  {
    if (length[QT_RHO] == 1 && htop.T != htop.E && htop.E == QT_ACE)
    {
      return false;
      // AT8x / H+ / K9xx / H.
      // A98x / H+ / KTxx / H.
      if (pickFlag) holdCtr[302]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-7, 4);
    }
    else if (length[QT_LHO] == 1 && htop.T != htop.E && htop.E == QT_PARD)
    {
      return false;
      // A9xx / H / KT8x / H+.
      // ATxx / H / K98x / H+.
      if (pickFlag) holdCtr[303]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-7, 4);
    }
  }
  else if (pend == QT_BOTH && distHex == 0x4144 &&
    ! hopp.T && ! hopp.N && ! hopp.E && 
    (htop.T == QT_PARD && htop.N == QT_PARD && htop.E == QT_PARD))
    // htop.T != htop.N)
  {
    // ATxx / H / K98x / Jxxx.
    // A9xx / H / KT8x / Jxxx.
    if (pickFlag) holdCtr[304]++;
    return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-7, 4);
  }

  if (length[QT_LHO] >= 2 && length[QT_RHO] >= 2 &&
    length[QT_LHO] + length[QT_RHO] >= 5)
  {
    // Axx / Hxx / Kxx / Hx.
    // Axx / Hx / Kxx / Hxx.
    if (pickFlag) holdCtr[283]++;
    return move.Set(QT_BOTH, pend, SDS_VOID-2, 2);
  }
  else if (hopp.T && (length[QT_LHO] >= 3 || length[QT_RHO] >= 3))
  {
    // Axx / H / Kxx / HHx.
    // Axx / HHx / Kxx / H.
    if (pickFlag) holdCtr[284]++;
    return move.Set(QT_BOTH, pend, SDS_VOID-2, 2);
  }
  else if (htop.K == QT_ACE && length[QT_LHO] >= 3)
  {
    // AKx / Hxx / xxx / H.
    if (pickFlag) holdCtr[285]++;
    return move.Set(QT_BOTH, pend, SDS_VOID-2, 2);
  }
  else if (htop.K == QT_ACE && length[QT_RHO] >= 3 &&
    (length[QT_PARD] == 1 ||
      (completeList[QT_RHO][1] > completeList[QT_ACE][2] &&
         (length[QT_PARD] == 2 ||
           completeList[QT_RHO][1] > completeList[QT_PARD][1]))))
  {
    if (pickFlag) holdCtr[286]++;
    return move.Set(QT_BOTH, pend, SDS_VOID-2, 2);
  }
  else if (pend == QT_BOTH)
  {
    if (htop.T == QT_PARD && htop.N == QT_RHO && length[QT_LHO] == 1)
    {
      // A+ / H / KT+ / H9x+.
      if (pickFlag) holdCtr[287]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-2, 2);
    }
    else if (htop.T == QT_ACE && htop.N == QT_LHO && length[QT_RHO] == 1)
    {
      // AT+ / H9x+ / K+ / H.
      if (pickFlag) holdCtr[288]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-2, 2);
    }
    else if (length[QT_ACE] == 2 && length[QT_RHO] >= 3)
    {
      // Ax / H / Kx+ / Hxx+.
      if (pickFlag) holdCtr[289]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-2, 2);
    }
    else if (length[QT_PARD] == 2 && length[QT_LHO] >= 3)
    {
      // Ax+ / Hxx+ / Kx / H.
      if (pickFlag) holdCtr[300]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-2, 2);
    }
  }
  return false;
}


bool LoopHold::SolveStopped36(Trick& move) const
{
  // ==== G36 ==========================================================
  //      AK+     
  // +          Q+
  //      J+     
  // ==== G36 ==========================================================

  if (length[QT_LHO] >= 3)
  {
    // AKx / Qxx+ / J+ / ?.
    if (pickFlag) holdCtr[360]++;
    return move.Set(QT_BOTH, QT_ACE, SDS_VOID-2, 2);
  }
  else if (length[QT_PARD] <= length[QT_LHO] &&
    length[QT_ACE] >= 4 && length[QT_RHO] >= 5 &&
    completeList[QT_RHO][0] > completeList[QT_ACE][4] &&
    completeList[QT_RHO][0] < completeList[QT_ACE][3])
  {
    // AKxxx / xxxxx / x / Q(x).
    if (pickFlag) holdCtr[361]++;
    unsigned r = Holding::ListToRank(completeList[QT_ACE][3]);
    return move.Set(QT_BOTH, QT_ACE, r, 4);
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
        if (pickFlag) holdCtr[362]++;
        return move.Set(QT_BOTH, QT_ACE, SDS_VOID-2, 2);
      }
      else if (length[QT_RHO] >= 4 &&
        completeList[QT_RHO][0] < completeList[QT_ACE][2] &&
        completeList[QT_RHO][0] > completeList[QT_ACE][3])
      {
        // AKx+ / Qx / J(x) / xxxx+.
        if (pickFlag) holdCtr[363]++;
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
      if (pickFlag) holdCtr[364]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-4, 3);
    }
  }
  else if (length[QT_PARD] == 1)
  {
    if (htop.T == QT_RHO && length[QT_RHO] >= 3)
    {
      // AKx / Q / J / Txx+.
      if (pickFlag) holdCtr[365]++;
      return move.Set(QT_BOTH, QT_ACE, SDS_VOID-2, 2);
    }
    else if (htop.T == QT_ACE && htop.N == QT_RHO && length[QT_RHO] >= 4)
    {
      // AKT+ / 9xxx+ / J / Q.
      if (pickFlag) holdCtr[366]++;
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
    if (pickFlag) holdCtr[367]++;
    return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-4, 3);
  }
  else if (distHex == 0x4144 && 
    htop.T == QT_RHO && htop.N == QT_PARD && htop.E == QT_PARD)
  {
    // AKxx / Q / J98x / Txxx.
    if (pickFlag) holdCtr[368]++;
    return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-7, 4);
  }
  return false;
}


bool LoopHold::SolveStopped41(Trick& move) const
{
  // ==== G41 ==========================================================
  //      A+      
  // +          K+
  //      QJ+    
  // ==== G41 ==========================================================

  if (length[QT_LHO] >= 2)
  {
    // Protected K behind the ace.
    if (pickFlag) holdCtr[410]++;
    return move.Set(QT_BOTH, QT_ACE, SDS_ACE, 1);
  }
  else if (length[QT_ACE] == 2)
  {
    if (length[QT_PARD] >= 4 && length[QT_RHO] >= 5 && 
      htop.T == QT_PARD && htop.N == QT_RHO)
    {
      // Ax / 9xxxx / QJTxx / K.
      if (pickFlag) holdCtr[411]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-5, 4);
    }
    else if (length[QT_PARD] == 2)
    {
      if (length[QT_RHO] >= 2)
      {
        if (pickFlag) holdCtr[412]++;
        return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-3, 2);
      }
      else
      {
        return false;
        if (pickFlag) holdCtr[413]++;
        return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-1, 2);
      }
    }
    else if (length[QT_RHO] <= 2)
    {
      // Ax / K / QJx+ / xx.
      // Ax / K / QJ+ / xx+.
      if (pickFlag) holdCtr[414]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-3, length[QT_PARD]);
    }
    else if (length[QT_PARD] == 3 || 
      (length[QT_RHO] >= 4 && htop.T == QT_RHO))
    {
      // Ax / K / QJ+ / Txx+.
      if (pickFlag) holdCtr[415]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-4, 3);
    }
  }
  else if (length[QT_PARD] == 2)
  {
    if (htop.T == QT_RHO && length[QT_RHO] >= 3)
    {
      // Axx+ / Txx+ / QJ / K.
      if (pickFlag) holdCtr[416]++;
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
    if (pickFlag) holdCtr[417]++;
    return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-4, 3);
  }
  return false;
}


bool LoopHold::SolveStopped43(Trick& move) const
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
        if (pickFlag) holdCtr[430]++;
        return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-3, 2);
      }
      else
      {
        return false;
        if (pickFlag) holdCtr[431]++;
        return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-1, 2);
      }
    }
    else if (length[QT_PARD] >= 4 && length[QT_LHO] >= 5 && 
      htop.T == QT_PARD && htop.N == QT_LHO &&
      completeList[QT_LHO][1] > completeList[QT_PARD][3])
    {
      // Ax / K / QJTx+ / 9xxxx.
      if (pickFlag) holdCtr[432]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-5, 4);
    }
    // else if (length[QT_LHO] <= 2 || length[QT_PARD] == 2)
    else if (length[QT_LHO] <= 2)
    {
      // Ax / K / QJx+ / xx.
      // Ax / K / QJ+ / xx+.
      if (pickFlag) holdCtr[433]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-3, length[QT_PARD]);
    }
    else if (length[QT_PARD] == 3 || 
      (length[QT_LHO] >= 4 && htop.T == QT_LHO &&
        completeList[QT_LHO][1] > completeList[QT_PARD][2]))
    {
      // Ax / K / QJ+ / Txx+.
      if (pickFlag) holdCtr[434]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-4, 3);
    }
  }
  else if (length[QT_PARD] == 2)
  {
    if (htop.T == QT_LHO && length[QT_LHO] >= 3)
    {
      // Axx+ / K / QJ / Txx+.
      if (pickFlag) holdCtr[435]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-3, 2);
    }
  }
  else if (length[QT_PARD] == 3)
  {
    if (length[QT_ACE] == 3)
      return false;
    else if (length[QT_LHO] >= 4 && 
      completeList[QT_LHO][0] > completeList[QT_ACE][1])
    {
      // Axx / ? / QJx / K.
      if (pickFlag) holdCtr[436]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-4, 3);
    }
  }
  else if (length[QT_ACE] == 4 && length[QT_LHO] >= 4 &&
    length[QT_PARD] == 4 && 
    htop.T == QT_LHO && htop.N == QT_ACE && htop.E == QT_ACE)
  {
    // A98x / Txxx / QJxx / K.
    if (pickFlag) holdCtr[437]++;
    return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-7, 4);
  }
  else if (length[QT_LHO] >= 4 && htop.T == QT_LHO &&
    LoopHold::StopFinesse(2, 1, 2, true, QT_PARD))
  {
    // Axx / Txxx / QJx(x) / K.
    if (pickFlag) holdCtr[438]++;
    return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-4, 3);
  }
  return false;
}


bool LoopHold::SolveStopped44(Trick& move) const
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
      if (pickFlag) holdCtr[440]++;
      unsigned r = Holding::ListToRank(completeList[QT_ACE][3]);
      return move.Set(QT_BOTH, QT_ACE, r, 4);
    }
    else if (htop.T == QT_ACE && htop.N == QT_LHO &&
      length[QT_RHO] == 1 && length[QT_LHO] >= 4)
    {
      // AKT+ / 9xxx+ / J / Q.
      if (pickFlag) holdCtr[441]++;
      return move.Set(QT_BOTH, QT_ACE, SDS_VOID-5, 3);
    }
    else if ((length[QT_RHO] >= 3 &&
      completeList[QT_RHO][1] > completeList[QT_ACE][2]) ||
      (hopp.T && length[htop.T] >= 3))
    {
      // AK+ / Txx+ / J / Q+.
      // AK+ / QTx+ / J / +.
      // AK+ / Q9x+ / J / Tx.
      if (pickFlag) holdCtr[442]++;
      return move.Set(QT_BOTH, QT_ACE, SDS_VOID-2, 2);
    }
  }

  if (length[QT_RHO] == 1)
  {
    if (length[QT_LHO] >= 4 && length[QT_PARD] >= 3 && 
      htop.T == QT_LHO && 
      completeList[QT_LHO][1] > completeList[QT_PARD][1] &&
      completeList[QT_LHO][1] > completeList[QT_ACE][3])
    {
      // AKx / Txxx / Jxx / Q.
      if (pickFlag) holdCtr[443]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-4, 3);
    }
    else if (distHex == 0x4441 && htop.T == QT_LHO &&
      (htop.N == QT_PARD || (htop.N == QT_ACE && ! hopp.E)))
    {
      // AKxx / Txxx / J9xx / Q.
      // AK98 / Txxx / Jxxx / Q.
      // AK9x / Txxx / J8xx / Q.
      if (pickFlag) holdCtr[459]++;
      unsigned r = (htop.N == QT_PARD ? SDS_VOID-6 : SDS_VOID-7);
      return move.Set(QT_BOTH, QT_BOTH, r, 4);
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
      if (pickFlag) holdCtr[444]++;
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
      if (pickFlag) holdCtr[445]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-4, 3);
    }
  }
  else if (length[QT_ACE] == 3 && length[QT_PARD] == 3 &&
    length[QT_RHO] <= 2)
  {
    if (length[QT_LHO] <= 2)
      return false;

    // AKx / + / Jxx / Q(x).
    if (pickFlag) holdCtr[446]++;
    return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-4, 3);
  }
  else if (length[QT_ACE] >= 4 && length[QT_LHO] >= 4 &&
    length[QT_PARD] == 3)
  {
    if (completeList[QT_LHO][0] > completeList[QT_ACE][2])
    {
      // AKxx / xxxx / Jxx / QT.
      if (pickFlag) holdCtr[447]++;
      return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-4, 3);
    }
  }
  else if (distHex == 0x3442 && 
    (htop.T == QT_LHO || (htop.T == QT_RHO && htop.N == QT_LHO)) &&
    LoopHold::StopFinesse(2, 2, 1, true, QT_PARD))
  {
    // AKx / Txxx / Jxxx / Qx.
    if (pickFlag) holdCtr[448]++;
    return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-4, 3);
  }
  else if (length[QT_LHO] >= 3 && length[QT_RHO] >= 2 && 
    htop.T == QT_LHO)
  {
    if (length[QT_PARD] <= 2 || (length[QT_RHO] >= 3))
      // && completeList[QT_RHO][2] > completeList[QT_PARD][2]))
    {
      // AKx / Txx+ / J(x) / Qx.
      if (pickFlag) holdCtr[449]++;
      return move.Set(QT_BOTH, QT_ACE, SDS_VOID-2, 2);
    }
  }
  else if (length[QT_LHO] >= 3 && length[QT_RHO] >= 2 && 
    htop.T == QT_RHO && length[QT_PARD] <= 2 &&
    ((completeList[QT_LHO][0] > completeList[QT_ACE][2] &&
       completeList[QT_LHO][0] > completeList[QT_PARD][2]) || // del
      (completeList[QT_RHO][2] > completeList[QT_ACE][2] &&
       completeList[QT_RHO][2] > completeList[QT_PARD][2]))) // del
  {
    // AKx / 9xx+ / J(x) / QT.
    if (pickFlag) holdCtr[450]++;
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
      if (pickFlag) holdCtr[451]++;
      return move.Set(QT_BOTH, QT_ACE, SDS_VOID-2, 2);
    }
    else if (length[QT_LHO] <= 2 && length[QT_PARD] >= 2 &&
      LoopHold::StopFinesse(2, 2, 0, false, QT_ACE))
    {
      // AKx+ / h / Jx+ / Qhx+.
      // AKx+ / ? / Jx+ / Qhx+.
      if (pickFlag) holdCtr[452]++;
      return move.Set(QT_BOTH, QT_ACE, SDS_VOID-2, 2);
    }
    else if (length[QT_LHO] >= 3 && 
      (hopp.N || (htop.N == QT_PARD && hopp.E)))
    {
      // AKx / 8xx / J9+ / QTx.
      // AKx / Txx / J9+ / Q8x.
      // AKx / T8x / J9+ / Qxx.
      if (pickFlag) holdCtr[453]++;
      return move.Set(QT_BOTH, QT_ACE, SDS_VOID-2, 2);
    }
  }
  else if (length[QT_LHO] <= 3)
    return false;
  else if (length[QT_PARD] <= 2)
  {
    if (completeList[QT_LHO][0] < completeList[QT_ACE][2] &&
        completeList[QT_LHO][0] > completeList[QT_ACE][3])
    {
      // AK9x / 8xxx / J(x) / QT.
      // AK9x / 7xxx / J8 / QT.
      // AK8x / 7xxx / J9 / QT.
      if (pickFlag) holdCtr[454]++;
      unsigned r = Holding::ListToRank(completeList[QT_ACE][2]);
      return move.Set(QT_BOTH, QT_ACE, r, 3);
    }
  }
  return false;
}


bool LoopHold::SolveStopped48(Trick& move) const
{
  // ==== G48 ==========================================================
  //      AKQ+    
  // J+         + 
  //      +     
  // ==== G48 ==========================================================

  if ((length[QT_LHO] >= 4 || length[QT_RHO] >= 4) &&
    hopp.T && hopp.N && hopp.E)
  {
    if (pickFlag) holdCtr[480]++;
    return move.Set(QT_BOTH, QT_ACE, SDS_VOID-3, 3);
  }
  else if (length[QT_LHO] >= 5 && length[QT_PARD] <= 2 && 
    completeList[QT_LHO][0] < completeList[QT_ACE][3] &&
    completeList[QT_LHO][0] > completeList[QT_ACE][4] &&
    (length[QT_PARD] == 1 ||
    completeList[QT_LHO][0] > completeList[QT_PARD][0]))
  {
    // AHHxx / xxxxx / Hx / H.
    if (pickFlag) holdCtr[481]++;
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
      if (pickFlag) holdCtr[482]++;
      return move.Set(QT_BOTH, QT_ACE, SDS_VOID-3, 3);
    }
  }
  else if (length[QT_RHO] >= 4 && 
    LoopHold::StopFinesse(3, 3, 0, false, QT_ACE))
  {
    // All kinds of combinations such as
    // AKQ3 / 2 / T864 / J975.
    if (pickFlag) holdCtr[483]++;
    return move.Set(QT_BOTH, QT_ACE, SDS_VOID-3, 3);
  }
  else if (length[QT_LHO] >= 4 && length[QT_PARD] == 2 && 
    length[QT_RHO] >= 2 &&
    completeList[QT_LHO][0] > completeList[QT_ACE][3] &&
    (completeList[QT_RHO][1] > completeList[QT_PARD][1] ||
     completeList[QT_LHO][0] > completeList[QT_PARD][1]))
  {
    if (pickFlag) holdCtr[484]++;
    // AKQ+ / xxxx / xx / Jx with enough middle cards.
    return move.Set(QT_BOTH, QT_ACE, SDS_VOID-3, 3);
  }
  return false;
}


bool LoopHold::SolveStopped49(Trick& move) const
{
  // ==== G49 ==========================================================
  //      AQ+     
  // J+         K+
  //      +     
  // ==== G49 ==========================================================

  if (length[QT_LHO] >= 2)
  {
    // Protected K behind the ace.
    if (pickFlag) holdCtr[490]++;
    return move.Set(QT_BOTH, QT_ACE, SDS_ACE, 1);
  }
  else if (length[QT_RHO] == 1 && length[QT_PARD] == 1)
  {
    // Ax / K / x / J.
    if (pickFlag) holdCtr[491]++;
    return move.Set(QT_BOTH, QT_ACE, SDS_ACE, length[QT_ACE]);
  }
  else if (length[QT_ACE] == 2 && length[QT_PARD] <= 2 &&
    length[QT_RHO] <= 2)
  {
    // AQ / K / x(x) / J(x).
    if (pickFlag) holdCtr[492]++;
    unsigned r = Holding::ListToRank(completeList[QT_ACE][length[QT_RHO]-1]);
    return move.Set(QT_BOTH, QT_ACE, r, 2);
    // return move.Set(QT_BOTH, QT_ACE, SDS_VOID-3, 2);
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
    if (pickFlag) holdCtr[493]++;
    return move.Set(QT_BOTH, QT_ACE, SDS_VOID-3, 2);
  }
  return false;
}


bool LoopHold::SolveStopped57(Trick& move) const
{
  // ==== G57 ==========================================================
  //      A+      
  // J+         K+
  //      Q+    
  // ==== G57 ==========================================================

  if (length[QT_LHO] >= 2)
  {
    // A+ / Kx+ / Q+ / J+.
    if (pickFlag) holdCtr[570]++;
    return move.Set(QT_BOTH, QT_ACE, SDS_VOID-1, 1);
  }
  else if (length[QT_PARD] == 1)
  {
    if (length[QT_RHO] == 1)
    {
      // A+ / K / Q / J.
      if (pickFlag) holdCtr[571]++;
      return move.Set(QT_BOTH, QT_ACE, SDS_VOID-1, length[QT_ACE]);
    }
    else
    {
      // A+ / K / Q / Jx+.
      if (pickFlag) holdCtr[572]++;
      return move.Set(QT_BOTH, QT_ACE, SDS_VOID-1, 1);
    }
  }
  else if (length[QT_RHO] >= 3)
  {
    // A+ / K / Q+ / Jxx+.
    if (pickFlag) holdCtr[573]++;
    return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-3, 2);
  }
  else if (length[QT_ACE] == 2 && length[QT_PARD] == 2 &&
    length[QT_RHO] == 2)
    // (length[QT_RHO] == 2 || htop.T == QT_PARD))
  {
    // Ax / K / Qx / Jx.
    if (pickFlag) holdCtr[574]++;
    return move.Set(QT_BOTH, QT_BOTH, SDS_VOID-3, 2);
  }
  return false;
}


bool LoopHold::SolveStopped60(Trick& move) const
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
      if (pickFlag) holdCtr[600]++;
      return move.Set(QT_BOTH, QT_ACE, SDS_VOID-2, 2);
    }
    else if (length[QT_LHO] >= 2 && htop.T == QT_PARD &&
      htop.N == QT_LHO)
    {
      // AKx+ / 9x+ / Tx+ / QJx+.
      if (pickFlag) holdCtr[601]++;
      return move.Set(QT_BOTH, QT_ACE, SDS_VOID-2, 2);
    }
    else if (LoopHold::StopFinesse(2, 2, 0, true, QT_ACE))
    {
      if (pickFlag) holdCtr[602]++;
      return move.Set(QT_BOTH, QT_ACE, SDS_VOID-2, 2);
    }
  }
  else if (length[QT_RHO] >= 3 || (length[QT_LHO] >= 3 &&
    (htop.T == QT_LHO || ((length[QT_PARD] <= 2 &&
      completeList[QT_LHO][0] > completeList[QT_ACE][x]) ||
        (length[QT_ACE] == 2 && 
          completeList[QT_LHO][0] > completeList[QT_PARD][1])))))
  {
    if (pickFlag) holdCtr[603]++;
    return move.Set(QT_BOTH, pend, SDS_VOID-2, 2);
  }
  else if (length[QT_ACE] >= 3 &&
    length[QT_PARD] <= 2 && length[QT_LHO] >= 4 && 
    completeList[QT_LHO][0] > completeList[QT_ACE][x+1] &&
    completeList[QT_LHO][0] < completeList[QT_ACE][x])
  {
    // AKxx / xxxx / xx / QJ.
    // Axxx / xxxx / Kx / QJ.
    if (pickFlag) holdCtr[604]++;
    unsigned r = Holding::ListToRank(completeList[QT_ACE][x]);
    return move.Set(QT_BOTH, pend, r, 3);
  }
  else if (length[QT_PARD] >= 3 &&
    length[QT_ACE] <= 2 && length[QT_LHO] >= 4 && 
    completeList[QT_LHO][0] > completeList[QT_PARD][2] &&
    completeList[QT_LHO][0] < completeList[QT_PARD][1])
  {
    // Ax / xxxx / Kxxx / QJ.
    if (pickFlag) holdCtr[605]++;
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
    if (pickFlag) holdCtr[606]++;
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
      if (pickFlag) holdCtr[607]++;
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
      if (pickFlag) holdCtr[608]++;
      PosType e = (xx == 0 ? QT_ACE : QT_BOTH);
      return move.Set(QT_BOTH, e, SDS_VOID-5, 3);
    }
  }
  return false;
}



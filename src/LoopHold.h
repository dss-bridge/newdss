/* 
   SDS, a bridge single-suit double-dummy quick-trick solver.

   Copyright (C) 2015-16 by Soren Hein.

   See LICENSE and README.
*/

#ifndef SDS_LOOPHOLD_H
#define SDS_LOOPHOLD_H

#include "Holding.h"
#include "const.h"

class DefList;

#define SDS_MAX_RANKS 14


class LoopHold: public Holding
{
  private:

    typedef bool (LoopHold::*SolvePtr)(Trick& move) const;
    SolvePtr SolveSimpleFunction[64];

    struct htopType
    {
      int K, Q, J, T, N, E;
    };

    struct hoppType
    {
      bool K, Q, J, T, N, E;
    };

    htopType htop;
    hoppType hopp;
    unsigned distHex;

    struct HoldingDetails
    {
      unsigned declLen;

      PosType pLong;
      PosType pShort;

      unsigned lenLong;
      unsigned lenShort;
      unsigned lenMaxOpp;

      unsigned numTopsLongLho;
      unsigned numTopsLongRho;
      unsigned numTopsShortLho;
      unsigned numTopsShortRho;

      unsigned maxTopLong;
      unsigned maxTopShort;
    };

    struct SideDetails
    {
      unsigned numTopsAll;
      unsigned numTopsLong;
      unsigned numTopsShort;

      unsigned xLong;
      unsigned xShort;

      unsigned minTopLong;
      unsigned minTopShort;

      unsigned mapShiftedToReal[SDS_MAX_RANKS];
    };

    struct CrashDetails
    {
      PosType blockEnd;
      PosType crashEnd;
      unsigned blockRank;
      unsigned remRank;
      unsigned crashRank;
      unsigned crashRank2;
      unsigned blockTricks;
      unsigned remTricks;
      unsigned crashTricks;
    };

    struct CashoutBothDetails
    {
      unsigned lenLong; // Length of longest NS player
      unsigned lenShort; // Length of shortest NS player

      PosType pLong; // Longest NS player
      PosType pShort; // Other NS player
      PosType pOppHighest; // Opponent with highest card
      PosType pOppLowest; // Other opponent

      unsigned lenOppHighest; // Length of pOppHighest
      unsigned lenOppLowest; // Effective length of pOppLowest.
          // May be 0 if irrelevant, may be > lenOppHighest
      unsigned lenOppMax; // Maximum of the two

      unsigned lenCashHigh; // Lower of lenLong and lenOppHighest
      unsigned lenCashLow; // Lower of lenLong and lenOppLowest

      unsigned minAce; // Lowest card of ace holder
      unsigned maxPard; // Highest card of partner
      unsigned maxOpp; // Higher of oppMaxHighest and oppMaxLowest
      unsigned minOpp; // Lower of oppMaxHighest and oppMaxLowest

      unsigned oppMaxHighest; // Highest card of pOppHighest
      unsigned oppMaxLowest; // Highest card of pOppLowest;

      // The "long" declarer is the ace holder if same length.
      unsigned numTopsLongHigh; // Long's tops over pOppHighest
      unsigned numTopsLongLow; // Long's tops over pOppLowest
      unsigned numTopsShortHigh; // Short's tops over pOppHighest;
      unsigned numTopsShortLow; // Short's tops over pOppLowest;

      unsigned numTopsHigh; // Sum of numTopsLongHigh and numTopsShortHigh
      unsigned numTopsLow; // Sum of numTopsLongLow and numTopsShortLow

      // First two x's must be > 0 by assertion.
      unsigned xLongHigh; // After pOppHighest: lenLong - numTopsLongHigh
      unsigned xShortHigh; // After pOppHighest: lenShort - numTopsShortHigh
      unsigned xLongLow; // After pOppLowest: lenLong - numTopsLongLow
      unsigned xShortLow; // After pOppLowest: lenShort - numTopsShortLow
    };


    void SolveCrashTricksHand(
      const HoldingDetails& hdet,
      const SideDetails& sdet,
      const unsigned lenOpp,
      CrashDetails& cr) const;
      
    void MinCrashDetails(
      CrashDetails& cr1,
      CrashDetails& cr2) const;

    void SetGeneralDetails(
      HoldingDetails& hdet) const;

    void SetSpecificDetails(
      const HoldingDetails& hdet,
      SideDetails& sdet,
      const bool oppSkippedFlag,
      const PosType oppSkipped = QT_ACE); // Anything

    void PrintHoldingDetails(
      const HoldingDetails& hdet) const;

    void PrintCashoutDetails(
      const CashoutBothDetails& cb) const;

    void ShiftMinUp(
      SideDetails& sdet,
      const PosType oppSkipped);

    void ShiftMinDown(
      const SideDetails& sdet,
      CrashDetails& cr) const;

    bool CashoutBothSameLength(
      DefList& def,
      unsigned& rank,
      const CashoutBothDetails& cb) const;
     
    bool CashoutBothDiffLength(
      DefList& def,
      unsigned& rank,
      const CashoutBothDetails& cb) const;
     
    bool CashoutBothDiffStrongTops(
      DefList& def,
      unsigned& rank,
      const CashoutBothDetails& cb) const;
     
    bool CashoutBothDiffPdLongWeak(
      DefList& def,
      unsigned& rank,
      const CashoutBothDetails& cb) const;
     
    bool CashoutBothDiffLongStrong(
      DefList& def,
      unsigned& rank,
      const CashoutBothDetails& cb) const;
     
    bool CashoutBothDiffStrong(
      DefList& def,
      unsigned& rank,
      const CashoutBothDetails& cb) const;
     
    bool CashoutBothDiffSplit(
      DefList& def,
      unsigned& rank,
      const CashoutBothDetails& cb) const;
     
    bool SetCashoutBothDetails(
      CashoutBothDetails& cb) const;

    void GetOppLengths(
      CashoutBothDetails& cb) const;

    bool StopFinesse(
      const unsigned numFinesses,
      const unsigned firstNonTopAce,
      const unsigned firstNonTopPard,
      const bool ignoreOtherOpp,
      const PosType anchor) const;

    bool SolveSimple0(Trick& move) const;
    bool SolveSimple1(Trick& move) const;
    bool SolveSimple2(Trick& move) const;
    bool SolveSimple3(Trick& move) const;
    bool SolveSimple5(Trick& move) const;
    bool SolveSimple6(Trick& move) const;
    bool SolveSimple7(Trick& move) const;
    bool SolveSimple9(Trick& move) const;
    bool SolveSimple11(Trick& move) const;
    bool SolveSimple12(Trick& move) const;
    bool SolveSimple14(Trick& move) const;
    bool SolveSimple15(Trick& move) const;
    bool SolveSimple18(Trick& move) const;
    bool SolveSimple19(Trick& move) const;
    bool SolveSimple20(Trick& move) const;
    bool SolveSimple26(Trick& move) const;
    bool SolveSimple27(Trick& move) const;
    bool SolveSimple28(Trick& move) const;
    bool SolveSimple36(Trick& move) const;
    bool SolveSimple41(Trick& move) const;
    bool SolveSimple43(Trick& move) const;
    bool SolveSimple44(Trick& move) const;
    bool SolveSimple48(Trick& move) const;
    bool SolveSimple49(Trick& move) const;
    bool SolveSimple57(Trick& move) const;
    bool SolveSimple60(Trick& move) const;


  public:

    LoopHold();

    ~LoopHold();

    bool CashoutAceSideBlocked(
      DefList& def,
      unsigned& rank) const;

    void CashAceShort(
      DefList& def,
      unsigned& rank) const;

    void CashoutAce(
      DefList& def,
      unsigned& rank) const;

    bool SolveCrashTricks(
      DefList& def,
      unsigned& rank,
      bool& flag);

    bool CashoutBoth(
      DefList& def,
      unsigned& rank);
     
    bool SolveSimple(
      DefList& def,
      unsigned& rank);
};

#endif

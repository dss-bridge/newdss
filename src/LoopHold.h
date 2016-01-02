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
    SolvePtr SolveStoppedFunction[64];

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

      unsigned numTopsAll;
      unsigned numTopsLong;
      unsigned numTopsShort;

      unsigned lenLong;
      unsigned lenShort;
      unsigned lenMaxOpp;

      unsigned xLong;
      unsigned xShort;

      unsigned maxTopLong;
      unsigned minTopLong;
      unsigned maxTopShort;
      unsigned minTopShort;

      unsigned mapRealToShifted[SDS_MAX_RANKS];
      unsigned mapShiftedToReal[SDS_MAX_RANKS];
    };

    HoldingDetails hdet;

    struct CrashRecordStruct
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


    void SolveCrashTricksHand(
      const unsigned& lenOpp,
      CrashRecordStruct& cr) const;
      
    void SetDetails();

    void PrintDetails();

    bool GetAsymmRanks(
      const PosType plong,
      const PosType pShort,
      const unsigned cashLength,
      const unsigned toBeat,
      unsigned& lowestRank) const;

    void UpdateDetailsForOpp(
      const bool oppSkippedFlag,
      const PosType& oppSkipped = QT_ACE); // Anything

    void MinCrashRecord(
      CrashRecordStruct& cr1,
      CrashRecordStruct& cr2) const;

    bool StopFinesse(
      const unsigned mumFinesses,
      const unsigned firstNonTopAce,
      const unsigned firstNonTopPard,
      const bool ignoreOtherOpp,
      const PosType anchor) const;

    bool SolveStopped0(Trick& move) const;
    bool SolveStopped1(Trick& move) const;
    bool SolveStopped2(Trick& move) const;
    bool SolveStopped3(Trick& move) const;
    bool SolveStopped5(Trick& move) const;
    bool SolveStopped6(Trick& move) const;
    bool SolveStopped7(Trick& move) const;
    bool SolveStopped9(Trick& move) const;
    bool SolveStopped11(Trick& move) const;
    bool SolveStopped12(Trick& move) const;
    bool SolveStopped14(Trick& move) const;
    bool SolveStopped15(Trick& move) const;
    bool SolveStopped18(Trick& move) const;
    bool SolveStopped19(Trick& move) const;
    bool SolveStopped20(Trick& move) const;
    bool SolveStopped26(Trick& move) const;
    bool SolveStopped27(Trick& move) const;
    bool SolveStopped28(Trick& move) const;
    bool SolveStopped36(Trick& move) const;
    bool SolveStopped41(Trick& move) const;
    bool SolveStopped43(Trick& move) const;
    bool SolveStopped44(Trick& move) const;
    bool SolveStopped48(Trick& move) const;
    bool SolveStopped49(Trick& move) const;
    bool SolveStopped57(Trick& move) const;
    bool SolveStopped60(Trick& move) const;


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
      unsigned& rank);

    bool CashoutBoth(
      DefList& def,
      unsigned& rank);
     
    bool SolveStopped(
      DefList& def,
      unsigned& rank);
};

#endif

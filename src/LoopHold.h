/* 
   SDS, a bridge single-suit double-dummy quick-trick solver.

   Copyright (C) 2015-16 by Soren Hein.

   See LICENSE and README.
*/

#ifndef SDS_LOOPHOLD_H
#define SDS_LOOPHOLD_H

#include "DefList.h"
#include "Holding.h"
#include "const.h"

#define SDS_MAX_RANKS 14


struct HoldingSimpleMove
{
  PosType start;
  PosType end;
  int rank;
  int tricks;
};


class LoopHold: public Holding
{
  private:

    typedef bool (LoopHold::*SolvePtr)(HoldingSimpleMove& move);
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
    int distHex;

    struct HoldingDetails
    {
      int cFlipped;
      int cFlippedUp; // All partner cards flipped to ace side
      int maskFull;
      int declLen;

      PosType pLong;
      PosType pShort;

      int numTopsAll;
      int numTopsLong;
      int numTopsShort;

      int lenLong;
      int lenShort;
      int lenMaxOpp;

      int xLong;
      int xShort;

      int maxTopLong;
      int minTopLong;
      int maxTopShort;
      int minTopShort;

      int mapRealToShifted[SDS_MAX_RANKS];
      int mapShiftedToReal[SDS_MAX_RANKS];
    };

    HoldingDetails hdet;



    void UpdateDetailsForOpp(
      const int& oppRank,
      const bool oppSkippedFlag,
      const PosType& oppSkipped);

    void SolveCrashTricksHand(
      const int& lenOpp,
      PosType& bend,
      PosType& cend,
      int& brank,
      int& rrank,
      int& crank,
      int& crank2,
      int& btricks,
      int& rtricks,
      int& ctricks);
      
    bool GetAsymmRanks(
      const int plong,
      const int pShort,
      const int cashLength,
      const int toBeat,
      unsigned& lowestRank);

    bool SetMove(
      HoldingSimpleMove& move,
      const PosType& start,
      const PosType& end,
      const int rank,
      const int tricks);

    bool StopFinesse(
      const int mumFinesses,
      const int firstNonTopAce,
      const int firstNonTopPard,
      const bool ignoreOtherOpp,
      const PosType anchor);

    bool SolveStopped0(HoldingSimpleMove& move);
    bool SolveStopped1(HoldingSimpleMove& move);
    bool SolveStopped2(HoldingSimpleMove& move);
    bool SolveStopped3(HoldingSimpleMove& move);
    bool SolveStopped5(HoldingSimpleMove& move);
    bool SolveStopped6(HoldingSimpleMove& move);
    bool SolveStopped7(HoldingSimpleMove& move);
    bool SolveStopped9(HoldingSimpleMove& move);
    bool SolveStopped11(HoldingSimpleMove& move);
    bool SolveStopped12(HoldingSimpleMove& move);
    bool SolveStopped14(HoldingSimpleMove& move);
    bool SolveStopped15(HoldingSimpleMove& move);
    bool SolveStopped18(HoldingSimpleMove& move);
    bool SolveStopped19(HoldingSimpleMove& move);
    bool SolveStopped20(HoldingSimpleMove& move);
    bool SolveStopped26(HoldingSimpleMove& move);
    bool SolveStopped27(HoldingSimpleMove& move);
    bool SolveStopped28(HoldingSimpleMove& move);
    bool SolveStopped36(HoldingSimpleMove& move);
    bool SolveStopped41(HoldingSimpleMove& move);
    bool SolveStopped43(HoldingSimpleMove& move);
    bool SolveStopped44(HoldingSimpleMove& move);
    bool SolveStopped48(HoldingSimpleMove& move);
    bool SolveStopped49(HoldingSimpleMove& move);
    bool SolveStopped57(HoldingSimpleMove& move);
    bool SolveStopped60(HoldingSimpleMove& move);


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


    bool CashoutBoth(
      DefList& def,
      unsigned& lowestRank);
     
    bool SolveStopped(HoldingSimpleMove& move);

    void SetDetails();

    void SolveCrashTricks(
      const PosType StupidCompiler,
      PosType& bend,
      PosType& cend,
      int& brank,
      int& rrank,
      int& crank,
      int& crank2,
      int& btricks,
      int& rtricks,
      int& ctricks);
    
    void PrintDetails();
};

#endif

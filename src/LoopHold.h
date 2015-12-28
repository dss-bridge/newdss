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

    typedef bool (LoopHold::*SolvePtr)(Trick& move);
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

    void SetDetails();

    void PrintDetails();

    bool SolveStopped0(Trick& move);
    bool SolveStopped1(Trick& move);
    bool SolveStopped2(Trick& move);
    bool SolveStopped3(Trick& move);
    bool SolveStopped5(Trick& move);
    bool SolveStopped6(Trick& move);
    bool SolveStopped7(Trick& move);
    bool SolveStopped9(Trick& move);
    bool SolveStopped11(Trick& move);
    bool SolveStopped12(Trick& move);
    bool SolveStopped14(Trick& move);
    bool SolveStopped15(Trick& move);
    bool SolveStopped18(Trick& move);
    bool SolveStopped19(Trick& move);
    bool SolveStopped20(Trick& move);
    bool SolveStopped26(Trick& move);
    bool SolveStopped27(Trick& move);
    bool SolveStopped28(Trick& move);
    bool SolveStopped36(Trick& move);
    bool SolveStopped41(Trick& move);
    bool SolveStopped43(Trick& move);
    bool SolveStopped44(Trick& move);
    bool SolveStopped48(Trick& move);
    bool SolveStopped49(Trick& move);
    bool SolveStopped57(Trick& move);
    bool SolveStopped60(Trick& move);


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
      unsigned& rank);
     
    bool SolveStopped(
      DefList& def,
      unsigned& rank);
      
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
};

#endif

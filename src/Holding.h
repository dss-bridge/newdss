/* 
   SDS, a bridge single-suit double-dummy quick-trick solver.

   Copyright (C) 2015-16 by Soren Hein.

   See LICENSE and README.
*/

#ifndef SDS_HOLDING_H
#define SDS_HOLDING_H

#include "Trick.h"
#include "const.h"

class Trick;

#define SDS_MAX_RANKS 14


class Holding
{
  private:

    PosType side;
    PosType lho;
    PosType pard;
    PosType rho;

    unsigned numLeads;
    unsigned numLhos;
    unsigned numPards;
    unsigned leadCurrIndex;
    unsigned lhoCurrIndex;
    unsigned pardCurrIndex;
    int leadList[SDS_MAX_RANKS];
    int lhoList[SDS_MAX_RANKS];
    int pardList[SDS_MAX_RANKS];

    int leadRank;
    int lhoRank;
    int pardRank;
    int rhoRank;
    int maxDef;
    int maxDefRun;
    int minRank[SDS_HANDS];

    PosType winSide;
    unsigned winRank;

    int cardListLo[SDS_HANDS][SDS_MAX_RANKS];
    int cardListHi[SDS_HANDS][SDS_MAX_RANKS];
    unsigned cardNo[SDS_HANDS];

    unsigned rankMap[14];
    bool aceFlip;
    bool mergeSpecialFlag;

    void MakeRanks();

    void AdjustWinRank();
    
    void PrintRanksList(
      const unsigned list[][SDS_MAX_RANKS],
      const unsigned listLen[SDS_HANDS],
      const std::string text ="",
      std::ostream& out = std::cout) const;


  protected:

    unsigned suitLength;
    unsigned counter;
    unsigned length[SDS_HANDS];
    unsigned completeList[SDS_HANDS][SDS_MAX_RANKS];
    unsigned rankHolder[SDS_MAX_RANKS];

    PosType GetOppBest() const;

    unsigned ListToRank(
      const unsigned listValue) const;

    unsigned TopsOverRank(
      const PosType& player,
      const unsigned rank) const;


  public:

    Holding();

    ~Holding();

    void Set(
      const unsigned suitLength,
      const unsigned counter);

    void SetSide(
      const PosType side);

    void RewindLead();
    void RewindLho();
    void RewindPard();

    bool NextLead();
    bool NextLho();
    bool NextPard();
    void SetRhoNo();

    unsigned GetSuitLength() const;
    unsigned GetCounter() const;
    PosType GetSide() const;
    PosType GetWinSide() const;
    unsigned GetLength(const int player) const;
    unsigned GetMinDeclLength() const;
    unsigned GetDistHex() const;
    unsigned GetPrependRank() const;
    unsigned GetLHOMaxRank() const;
    unsigned GetMaxOppRank() const;
    Trick GetTrick() const;
    unsigned GetNumTops() const;
    unsigned PossiblyFixRank(
      unsigned& fixedRank) const;
    unsigned GetSideRun(
      const unsigned start) const;

    bool IsAATrick() const;

    bool IsRealPP(const unsigned pRank) const;
    bool APIsBP()const;
    bool HasRemainingPTop() const;
    bool IsNotFinesse(const PosType pStart) const;

    bool IsPA1ACasher() const;

    bool MakePlay(
      unsigned& slNew,
      unsigned& cNew);

    unsigned FlipTops(
      const unsigned numTops,
      const unsigned nMask) const;

    bool GetAceFlip() const;

    unsigned GetNewRank(
      const unsigned r) const;

    bool GetMergeType() const;

    void Print(
      std::ostream& out = std::cout,
      const bool showBinary = true) const;

    void PrintRanks(
      std::ostream& out = std::cout) const;
    
    void PrintPlay(
      std::ostream& out = std::cout) const;
};

#endif

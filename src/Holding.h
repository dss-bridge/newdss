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

    int numLeads;
    int numLhos;
    int numPards;
    int leadCurrIndex;
    int lhoCurrIndex;
    int pardCurrIndex;
    int leadList[SDS_MAX_RANKS]; // Too large
    bool multiLead[SDS_MAX_RANKS]; // Set if certain touching honors
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

    int rankMap[14];
    bool aceFlip;
    bool mergeSpecialFlag;

    void MakeRanks();

    void PrintRanksList(
      const unsigned list[][SDS_MAX_RANKS],
      const unsigned listLen[SDS_HANDS],
      const std::string text ="",
      std::ostream& out = std::cout) const;
    
  protected:

    unsigned suitLength;
    unsigned counter;

    int cardListLo[SDS_HANDS][SDS_MAX_RANKS];
    int cardListHi[SDS_HANDS][SDS_MAX_RANKS];
    unsigned cardNo[SDS_HANDS];
    unsigned completeList[SDS_HANDS][SDS_MAX_RANKS];
    unsigned length[SDS_HANDS];

    unsigned ListToRank(
      const unsigned listValue) const;

    unsigned TopsOverRank(
      const PosType& player,
      const unsigned rank) const;

    void AdjustWinRank();


  public:

    Holding();

    ~Holding();

    void Set(
      const unsigned suitLength,
      const unsigned counter);

    void RewindLead();
    void RewindLho();
    void RewindPard();

    bool NextLead();
    bool NextLho();
    bool NextPard();
    void SetRhoNo();

    void SetSide(const int side);

    PosType GetSide() const;
    PosType GetWinSide();

    const Trick GetTrick() const;

    unsigned GetPrependRank() const;
    int GetMaxOppRank();
    unsigned GetSuitLength() const;
    unsigned GetCounter() const;
    unsigned GetLength(const int player) const;
    unsigned GetMinDeclLength() const;
    bool LHOIsVoid() const;
    unsigned GetLHOMaxRank() const;
    bool IsAATrick() const;

    unsigned GetNumTops();
    PosType GetOppBest();

    bool MakePlay(
      unsigned& slNew,
      unsigned& cNew);

    bool GetAceFlip() const;

    int GetNewRank(int r) const;

    bool GetMergeType() const;

    unsigned FlipTops(
      const unsigned numTops,
      const unsigned nMask);

    void Print(
      std::ostream& out = std::cout,
      const bool showBinary = true) const;

    void PrintRanks(
      std::ostream& out = std::cout) const;
    
    void PrintPlay(
      std::ostream& out = std::cout) const;
};

#endif

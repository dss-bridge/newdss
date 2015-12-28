/* 
   SDS, a bridge single-suit double-dummy quick-trick solver.

   Copyright (C) 2015-16 by Soren Hein.

   See LICENSE and README.
*/

#include <iomanip>
#include <assert.h>

#include "Trick.h"
#include "Header.h"
#include "DefList.h"
#include "MoveList.h"
#include "SimpleMoves.h"
#include "ComplexMoves.h"
#include "Twiddle.h"
#include "Dist.h"
#include "LoopHold.h"
#include "hist.h"
#include "single.h"
#include "const.h"

using namespace std;


extern SingleType * singles[14];
extern MoveList moveList;
extern vector<unsigned> holdCtr;


void MakeSimpleMoves()
{
  DefList def;
  LoopHold holding;
  for (unsigned sl = 1; sl <= 13; sl++)
  {
    cout << "\nSimple moves: " << sl << "\n";

    for (unsigned c = 0; c < SDS_NUM_SINGLES[sl]; c++)
    {
      if ((c & 0x3ffff) == 0)
      {
        cout << "Count " <<
          setw(8) << c << " (" <<
          setw(5) << fixed << setprecision(2) <<
            100. * c / SDS_NUM_SINGLES[sl] << "%)" << endl;
      }

      if (singles[sl][c].moveNo == 0)
      {
        holding.Set(sl, c);
        MakeSimpleSingleMove(sl, c, def, holding);
      }
    }
  }
}


// So we don't create and destroy all the time...
HoldingSimpleMove zhmove;
LoopHold zhNew;
HoldingDetails zhdetNew;
Trick ztrick, ztrick2, ztrick3;

inline bool MakeSimpleSingleMove(
  const unsigned sl,
  const unsigned c,
  DefList& def,
  LoopHold& holding)
{
  unsigned lenAce = holding.GetLength(QT_ACE);
  unsigned lenPard = holding.GetLength(QT_PARD);

  if (lenAce == holding.GetNumTops())
  {
    unsigned r, t;
    bool newFlag;

    if (lenPard > lenAce && holding.CashoutAceSideBlocked(t, r))
    {
      // Aceholder has blocking tops that count: AK / JT9.

      assert(t > 0);

      unsigned brank = SDS_VOID - lenAce;
      unsigned rmin = Min(brank, r);
          
      ztrick.Set(QT_BOTH, QT_ACE, brank, lenAce);
      ztrick2.Set(QT_PARD, QT_PARD, r, t);
      def.Set2(ztrick, ztrick2);

      unsigned mno = moveList.AddMove(def, holding, newFlag);
      SetAllPermutations(sl, c, mno, holding, rmin, HIST_BLOCKED, newFlag);
    }
    else
    {
      // Tops opposite partner who is either cashed out,
      // or whose cards are too low to yield more tricks.
      // Examples:  AK / xx / x / - or AK / Qxx / JT9 / -.

      holding.CashAceShort(r);

      Trick trick;
      trick.Set(lenPard ? QT_BOTH : QT_ACE, QT_ACE, r, lenAce);
      def.Set1(trick);

      unsigned mno = moveList.AddMove(def, holding, newFlag);
      SetAllPermutations(sl, c, mno, holding, r, HIST_ACE_SHORT, newFlag);
    }
  }

  if (singles[sl][c].minLen == 0)
  {
    bool newFlag;
    PosType start, end;
    unsigned r, t;
    int brank, rrank, crank, crank2, btricks, rtricks, ctricks;

    if (singles[sl][c].moveNo == 0)
    {
      // Partner is void.  Cash maximum from aceholder.

      holding.CashoutAce(t, r);

      ztrick.Set(QT_ACE, QT_ACE, static_cast<unsigned>(r), 
        static_cast<unsigned>(t));
      def.Set1(ztrick);
      unsigned mno = moveList.AddMove(def, holding, newFlag);
      SetAllPermutations(sl, c, mno, holding, static_cast<unsigned>(r), 
        HIST_PARD_VOID, newFlag);
    }

    // Can also make some crash positions out of this, e.g. AQ / K.

    unsigned numTops = holding.GetNumTops();
    PosType oppBest = holding.GetOppBest();

    for (unsigned nMask = 1; nMask < (1u << numTops)-1u; nMask++)
    {
      // This also takes care of the surely-blocked flips from above.
      unsigned cNew = holding.FlipTops(numTops, nMask);
      if (singles[sl][cNew].moveNo)
        continue;

      zhNew.Set(sl, cNew);
      // No complete blocks.
      if (zhNew.GetNumTops() == zhNew.GetLength(QT_ACE))
        continue;

      zhNew.SetDetails(zhdetNew);

      PosType blocked, bend, cend;
      zhNew.SolveCrashTricks(zhdetNew, oppBest, bend, cend,
        brank, rrank, crank, crank2, btricks, rtricks, ctricks);

      if (rtricks == 0 && crank2 != SDS_VOID && crank != crank2)
      {
        assert(crank < crank2);

        ztrick.Set(QT_BOTH, QT_ACE, static_cast<unsigned>(crank2), 
          static_cast<unsigned>(ctricks));
        ztrick2.Set(QT_ACE, QT_PARD, static_cast<unsigned>(crank), 
          static_cast<unsigned>(ctricks));
        def.Set11(ztrick, ztrick2);

        r = static_cast<unsigned>(Min(brank, rrank));
        r = Min(r, static_cast<unsigned>(crank));
        unsigned mno = moveList.AddMove(def, zhNew, newFlag);

        SetAllLowCards(sl, cNew, mno, r, HIST_CRASH, newFlag);
      }
      else if (rtricks <= 0)
      {
        ztrick.Set(QT_BOTH, cend, static_cast<unsigned>(crank), 
          static_cast<unsigned>(ctricks));
        def.Set1(ztrick);
        
        r = static_cast<unsigned>(Min(brank, rrank));
        r = Min(r, static_cast<unsigned>(crank));
        unsigned mno = moveList.AddMove(def, zhNew, newFlag);

        SetAllLowCards(sl, cNew, mno, r, HIST_CRASH, newFlag);
      }
      else
      {
        blocked = (bend == QT_ACE ? QT_PARD : QT_ACE);
        PosType bstart = (btricks + rtricks > ctricks ? QT_BOTH : bend);

        if (bstart == QT_BOTH &&
            ctricks <= btricks &&
            ((cend == QT_ACE && blocked == QT_PARD) ||
             (cend == QT_BOTH && blocked == QT_PARD) ||
             (cend == QT_BOTH && blocked == QT_ACE)))
        {
          if (zhNew.GetMinDeclLength() == 1)
            ztrick.Set(QT_PARD, QT_ACE, static_cast<unsigned>(crank), 
              static_cast<unsigned>(ctricks));
          else
            ztrick.Set(QT_BOTH, QT_BOTH, static_cast<unsigned>(crank), 
              static_cast<unsigned>(ctricks));
        }
        else
          ztrick.Set(QT_BOTH, cend, static_cast<unsigned>(crank), 
            static_cast<unsigned>(ctricks));

        ztrick2.Set(bstart, blocked, static_cast<unsigned>(brank), 
          static_cast<unsigned>(btricks));
        ztrick3.Set(bend, bend, static_cast<unsigned>(rrank), 
          static_cast<unsigned>(rtricks));

        def.Set12(ztrick, ztrick2, ztrick3);
        
        r = static_cast<unsigned>(Min(brank, rrank));
        r = Min(r, static_cast<unsigned>(crank));
        unsigned mno = moveList.AddMove(def, zhNew, newFlag);
        SetAllPermutations(sl, static_cast<unsigned>(cNew), mno, zhNew, 
          r, HIST_CRASH, newFlag);
      }
    }
  }

  if (singles[sl][c].moveNo)
    return true;

  bool newFlag;
  int r, t;
  if (holding.SolveStopped(zhmove))
  {
    // Opponents have enough tops and length to hold declarer to
    // a number of tricks, often 1.  (For instance LHO has Kx.)

    // >>>>> Need to turn zhmove into a trick to begin with!
    ztrick.Set(zhmove.start, zhmove.end, static_cast<unsigned>(zhmove.rank), 
      static_cast<unsigned>(zhmove.tricks));
    def.Set1(ztrick);

    unsigned mno = moveList.AddMove(def, holding, newFlag);
    SetAllPermutations(sl, c, mno, holding, static_cast<unsigned>(zhmove.rank), 
      HIST_SINGLE, newFlag);
  }
  else if (holding.CashoutBoth(def, r))
  {
    // Suits such as AQx / Kx.
    unsigned mno = moveList.AddMove(def, holding, newFlag);

    SetAllPermutations(sl, c, mno, holding, static_cast<unsigned>(r), 
      HIST_CASHES, newFlag);
  }

  return (singles[sl][c].moveNo > 0);
}



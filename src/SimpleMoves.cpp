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


inline bool MakeSimpleSingleMove(
  const unsigned sl,
  const unsigned c,
  DefList& def,
  LoopHold& holding)
{
  unsigned lenAce = holding.GetLength(QT_ACE);
  unsigned lenPard = holding.GetLength(QT_PARD);
  bool newFlag;

  if (lenAce == holding.GetNumTops())
  {
    unsigned r;

    if (lenPard > lenAce && holding.CashoutAceSideBlocked(def, r))
    {
      // Aceholder has blocking tops: AK / Qx / JT9 / xx.
      // Partner can cash afterwards.

      unsigned mno = moveList.AddMove(def, holding, newFlag);
      SetAllPermutations(sl, c, mno, holding, r, HIST_BLOCKED, newFlag);
    }
    else
    {
      // Aceholder has blocking tops: AK / Qxx / JT9 / xx.
      // Partner cannot cash afterwards.

      holding.CashAceShort(def, r);
      unsigned mno = moveList.AddMove(def, holding, newFlag);
      SetAllPermutations(sl, c, mno, holding, r, HIST_ACE_SHORT, newFlag);
    }
  }

  if (singles[sl][c].minLen == 0)
  {
    unsigned r;

    if (singles[sl][c].moveNo == 0)
    {
      // Partner is void.  Cash maximum from aceholder.

      holding.CashoutAce(def, r);
      unsigned mno = moveList.AddMove(def, holding, newFlag);
      SetAllPermutations(sl, c, mno, holding, r, HIST_PARD_VOID, newFlag);
    }

    // Can also make some crash positions out of this, e.g. AQ / K.

    PosType start, end;
    unsigned brank, rrank, crank, crank2, btricks, rtricks, ctricks;
    unsigned numTops = holding.GetNumTops();

    for (unsigned nMask = 1; nMask < (1u << numTops)-1u; nMask++)
    {
      // This also takes care of the surely-blocked flips from above.
      unsigned cNew = holding.FlipTops(numTops, nMask);
      if (singles[sl][cNew].moveNo)
        continue;

      LoopHold zhNew;
      zhNew.Set(sl, cNew);
      // No complete blocks.
      if (zhNew.GetNumTops() == zhNew.GetLength(QT_ACE))
        continue;

      PosType blocked, bend, cend;
      zhNew.SolveCrashTricks(
        bend, cend, brank, rrank, crank, crank2, btricks, rtricks, ctricks);

      if (rtricks == 0)
      {
        if (crank2 != SDS_VOID && crank != crank2)
        {
          assert(crank < crank2);

          Trick ztrick, ztrick2;
          ztrick.Set(QT_BOTH, QT_ACE, crank2, ctricks);
          ztrick2.Set(QT_ACE, QT_PARD, crank, ctricks);
          def.Set11(ztrick, ztrick2);
        }
        else
        {
          Trick ztrick;
          ztrick.Set(QT_BOTH, cend, crank, ctricks);
          def.Set1(ztrick);
        }
        r = Min(brank, rrank);
        r = Min(r, crank);

        unsigned mno = moveList.AddMove(def, zhNew, newFlag);
        SetAllLowCards(sl, cNew, mno, r, HIST_CRASH, newFlag);
      }
      else
      {
        blocked = (bend == QT_ACE ? QT_PARD : QT_ACE);
        PosType bstart = (btricks + rtricks > ctricks ? QT_BOTH : bend);

        Trick ztrick, ztrick2, ztrick3;

        if (bstart == QT_BOTH &&
            ctricks <= btricks &&
            ((cend == QT_ACE && blocked == QT_PARD) ||
             (cend == QT_BOTH && blocked == QT_PARD) ||
             (cend == QT_BOTH && blocked == QT_ACE)))
        {
          if (zhNew.GetMinDeclLength() == 1)
            ztrick.Set(QT_PARD, QT_ACE, crank, ctricks);
          else
            ztrick.Set(QT_BOTH, QT_BOTH, crank, ctricks);
        }
        else
          ztrick.Set(QT_BOTH, cend, crank, ctricks);

        ztrick2.Set(bstart, blocked, brank, btricks);
        ztrick3.Set(bend, bend, rrank, rtricks);

        def.Set12(ztrick, ztrick2, ztrick3);
        
        r = Min(brank, rrank);
        r = Min(r, crank);
        unsigned mno = moveList.AddMove(def, zhNew, newFlag);
        SetAllPermutations(sl, cNew, mno, zhNew, r, HIST_CRASH, newFlag);
      }
    }
  }

  if (singles[sl][c].moveNo)
    return true;

  unsigned r;
  if (holding.SolveStopped(def, r))
  {
    // Opponents have enough tops and length to hold declarer to
    // a number of tricks, often 1.  (For instance LHO has Kx.)

    unsigned mno = moveList.AddMove(def, holding, newFlag);
    SetAllPermutations(sl, c, mno, holding, r, HIST_SINGLE, newFlag);
  }
  else if (holding.CashoutBoth(def, r))
  {
    // Suits such as AQx / Kx.

    unsigned mno = moveList.AddMove(def, holding, newFlag);
    SetAllPermutations(sl, c, mno, holding, r, HIST_CASHES, newFlag);
  }

  return (singles[sl][c].moveNo > 0);
}



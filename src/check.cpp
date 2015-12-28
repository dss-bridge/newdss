/* 
   SDS, a bridge single-suit double-dummy quick-trick solver.

   Copyright (C) 2015-16 by Soren Hein.

   See LICENSE and README.
*/

#include <iostream>
#include <assert.h>

#include "MoveList.h"
#include "ComplexMoves.h"
#include "DefList.h"
#include "Holding.h"
#include "hist.h"
#include "check.h"

using namespace std;

extern MoveList moveList;


void CompareRecurse(
  unsigned moveNo,
  Holding& holding,
  const HistIndexType histNo)
{
  const DefList& def = moveList.GetCombinedMove(moveNo);

  DefList defCmp1, defCmp2;
  MakeComplexSingleMove(holding, defCmp1, defCmp2);

  DefList defCmp;
  if (! defCmp.MergeSidesSoft(defCmp1, defCmp2))
    defCmp.MergeSidesHard(defCmp1, defCmp2);

  if (defCmp != def)
  {
    cout << "CompareRecurse error: histogram " << 
      static_cast<unsigned>(histNo) <<
      ", " << GetHistName(histNo) << "\n";
    holding.Print(cout);
    def.Print(cout);
    cout << "Actually solved\n";
    defCmp.Print(cout);
    cout.flush();
  }
}


/* 
   SDS, a bridge single-suit double-dummy quick-trick solver.

   Copyright (C) 2015-16 by Soren Hein.

   See LICENSE and README.
*/

#ifndef SDS_SIMPLEMOVES_H
#define SDS_SIMPLEMOVES_H

class DefList;
class LoopHold;


void MakeSimpleMoves();

bool MakeSimpleSingleMoveWrapper(
  const unsigned suitLength,
  const unsigned counter,
  DefList& def,
  LoopHold& holding);

void UpdateHist(
  const unsigned histNo,
  const unsigned suitLength,
  const unsigned counter,
  const unsigned rank,
  bool newFlag);

#endif

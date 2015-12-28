/* 
   SDS, a bridge single-suit double-dummy quick-trick solver.

   Copyright (C) 2015-16 by Soren Hein.

   See LICENSE and README.
*/

#ifndef SDS_COMPLEXMOVES_H
#define SDS_COMPLEXMOVES_H

class Holding;
class DefList;


void MakeComplexMoves();

void MakeComplexSingleMove(
  Holding& holding,
  DefList& def1,
  DefList& def2);

void MakeComplexMovesParallel();

#endif

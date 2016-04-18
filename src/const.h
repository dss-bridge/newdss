/* 
   SDS, a bridge single-suit double-dummy quick-trick solver.

   Copyright (C) 2015-16 by Soren Hein.

   See LICENSE and README.
*/

#ifndef SDS_CONST_H
#define SDS_CONST_H

#define SDS_MAX_CORES 12

#define SDS_HANDS 4
#define SDS_VOID 13
#define SDS_ACE 12
#define SDS_KING 11
#define SDS_QUEEN 10
#define SDS_JACK 9
#define SDS_TEN 8
#define SDS_NINE 7
#define SDS_EIGHT 6

#define SDS_MAX_DEF 4
#define SDS_MAX_ALT 7

#define Max(x, y) (((x) >= (y)) ? (x) : (y))
#define Min(x, y) (((x) <= (y)) ? (x) : (y))


const char SDS_RANK_NAMES[14] =
{
  '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A', '-'
};

enum PosType
{
  QT_ACE = 0, 
  QT_LHO = 1, 
  QT_PARD = 2, 
  QT_RHO = 3, 
  QT_BOTH = 4,
  QT_SIZE = 5
};

const PosType SDS_PARTNER[QT_BOTH] =
{
  QT_PARD,
  QT_RHO,
  QT_ACE,
  QT_LHO
};

#endif


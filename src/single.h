/* 
   SDS, a bridge single-suit double-dummy quick-trick solver.

   Copyright (C) 2015-16 by Soren Hein.

   See LICENSE and README.
*/

#ifndef SDS_SINGLE_H
#define SDS_SINGLE_H

const unsigned SDS_NUM_SINGLES[14] = 
{ 
  0x0000001, // length 0
  0x0000001, // length 1,
  0x0000004, // length 2
  0x0000010, // length 3
  0x0000040, // length 4
  0x0000100, // length 5
  0x0000400, // length 6
  0x0001000, // length 7
  0x0004000, // length 8
  0x0010000, // length 9
  0x0040000, // length 10
  0x0100000, // length 11
  0x0400000, // length 12, 4.2 million
  0x1000000, // length 13, 16.8 million
};

const unsigned SDS_NUM_BOTTOMS[8] = 
{ 
  1, 4, 10, 20, 35, 56, 84, 120 
};

struct SingleType
{
  unsigned moveNo;
  unsigned char declLen;
  unsigned char minLen;
  unsigned char oppLen;
};

#endif


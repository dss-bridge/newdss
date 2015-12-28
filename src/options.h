/* 
   SDS, a bridge single-suit double-dummy quick-trick solver.

   Copyright (C) 2015-16 by Soren Hein.

   See LICENSE and README.
*/

#ifndef SDS_ARGS_H
#define SDS_ARGS_H

struct OptionsType
{
  unsigned numCores;
  bool combSet;
  unsigned suitLength;
  unsigned counter;
  bool tablesOnly;
  unsigned histCompare;
  bool findFlag;
  
  bool debugFlow;
  bool debugDef;
  bool debugAlt;
};

void Usage(
  const char base[]);

void PrintOptions();

void ReadArgs(
  int argc,
  char * argv[]);

#endif


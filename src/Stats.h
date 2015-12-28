/* 
   SDS, a bridge single-suit double-dummy quick-trick solver.

   Copyright (C) 2015-16 by Soren Hein.

   See LICENSE and README.
*/

#ifndef SDS_STATS_H
#define SDS_STATS_H

void InitTimer();

void SetTimerName(const char * name);

void StartTimer();

void EndTimer();

void PrintTimer();

void InitTimerList();

void StartTimerNo(const unsigned n);

void EndTimerNo(const unsigned n);

void EndTimerNoAndComp(
  const unsigned n, 
  const int pred);

void PrintTimerList();

void InitCounter();

void PrintCounter();

#endif

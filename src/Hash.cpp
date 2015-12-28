/* 
   SDS, a bridge single-suit double-dummy quick-trick solver.

   Copyright (C) 2015-16 by Soren Hein.

   See LICENSE and README.
*/

#include <iostream>
#include <assert.h>

#include "Hash.h"

using namespace std;


Hash::Hash()
{
  keyMapNo = 1;
  rankMapNo = 1;
  caseMapNo = 1;
  rankCaseMapNo = 1;

  trickKeyMap.resize(SDS_HASH_LENTRICK);
  rankKeyMap.resize(SDS_HASH_LENRANK);
  caseKeyMap.resize(SDS_HASH_LENCASE);
  rankCaseKeyMap.resize(SDS_HASH_LENRANKCASE);
}


Hash::~Hash()
{
}


unsigned Hash::GetKey(
  const Header& header)
{
  // TRICK: 12 bits (4096) --> 132 combinations, 8 bits.
  unsigned tFullTrick = header.GetTrickKey();
  unsigned tRedTrick;
  if (trickKeyMap[tFullTrick] == 0)
    trickKeyMap[tFullTrick] = keyMapNo++;
  tRedTrick = trickKeyMap[tFullTrick]-1;
  assert(tFullTrick < SDS_HASH_LENTRICK);
  assert(tRedTrick < 250);

  // RANK: 12 bits ( 4096) --> 399 combinations,  9 bits.
  // RANK: 16 bits (65536) --> 593 combinations, 10 bits.
  unsigned tFullRank = header.GetRankKey();
  unsigned tRedRank;
  if (rankKeyMap[tFullRank] == 0)
    rankKeyMap[tFullRank] = rankMapNo++;
  tRedRank = rankKeyMap[tFullRank]-1;
  assert(tFullRank < SDS_HASH_LENRANK);
  assert(tRedRank < 1000);

  // CASE: ? bits --> 20 combinations, 5 bits.
  unsigned tFullCase = header.GetKeyNew();
  unsigned tRedCase;
  if (caseKeyMap[tFullCase] == 0)
    caseKeyMap[tFullCase] = caseMapNo++;
  tRedCase = caseKeyMap[tFullCase]-1;
  assert(tFullCase < SDS_HASH_LENCASE);
  assert(tRedCase < 128);

  unsigned tFullRankCase = (tRedRank << 7) | tRedCase;
  unsigned tRedRankCase;
  if (rankCaseKeyMap[tFullRankCase] == 0)
    rankCaseKeyMap[tFullRankCase] = rankCaseMapNo++;
  tRedRankCase = rankCaseKeyMap[tFullRankCase]-1;
  assert(tFullRankCase < SDS_HASH_LENRANKCASE);
  assert(tRedRankCase < 4096);

  unsigned key = (tRedRankCase << 8) | tRedTrick;
  assert(key < ML_MAXKEY);

  return key;
}


void Hash::PrintCounts(
  ostream& out) const
{
  out << "Trick count: " << keyMapNo-1 << "\n";
  out << "Rank count: " << rankMapNo-1 << "\n";
  out << "Case count: " << caseMapNo-1 << "\n";
  out << "Rank-case count: " << rankCaseMapNo-1 << "\n";
}

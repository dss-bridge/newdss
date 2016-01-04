/* 
   SDS, a bridge single-suit double-dummy quick-trick solver.

   Copyright (C) 2015-16 by Soren Hein.

   See LICENSE and README.
*/

#include <iostream>
#include <iomanip>
#include <sstream>
#include <assert.h>

#include "summary.h"
#include "files.h"

using namespace std;

extern FilesType files;
extern SummaryType summary;

void SumFile(
  const char fname[],
  string& result);


void InitFiles()
{
  files.movesAll.open("output/movesAll.txt");
  files.movesComb.open("output/movesComb.txt");
  files.movesA.open("output/movesA.txt");
  files.movesP.open("output/movesP.txt");
  files.statsAll.open("output/statsAll.txt");
  files.statsComb.open("output/statsComb.txt");
  files.statsA.open("output/statsA.txt");
  files.statsP.open("output/statsP.txt");
  files.simpleShort.open("output/simpleShort.txt");
  files.simpleEqual.open("output/simpleEqual.txt");
  files.simpleDiff.open("output/simpleDiff.txt");
  files.track.open("output/track.txt");
  files.summary.open("output/track.txt");
  files.debug.open("output/debug.txt");

}


void CloseFiles()
{
  files.movesAll.flush();
  files.movesComb.flush();
  files.movesA.flush();
  files.movesP.flush();
  files.statsAll.flush();
  files.statsComb.flush();
  files.statsA.flush();
  files.statsP.flush();
  files.simpleShort.flush();
  files.simpleEqual.flush();
  files.simpleDiff.flush();
  files.track.flush();
  files.summary.flush();
  files.debug.flush();

  files.movesAll.close();
  files.movesComb.close();
  files.movesA.close();
  files.movesP.close();
  files.statsAll.close();
  files.statsComb.close();
  files.statsA.close();
  files.statsP.close();
  files.simpleShort.close();
  files.simpleEqual.close();
  files.simpleDiff.close();
  files.track.close();
  files.summary.close();
  files.debug.close();
}


void SumFiles()
{
  SumFile("output/movesAll.txt", summary.all.sumMoves);
  SumFile("output/movesComb.txt", summary.comb.sumMoves);
  SumFile("output/movesA.txt", summary.A.sumMoves);
  SumFile("output/movesP.txt", summary.P.sumMoves);
}


void SumFile(
  const char fname[],
  string& result)
{
  char cmd[100];
  sprintf(cmd, "sum %s", fname);

#ifdef _MSC_VER
  FILE *fp = _popen(cmd, "rt");
#else
  FILE *fp = popen(cmd, "r");
#endif
  if (! fp)
    return;

  char buffer[100];
  char * lp = fgets(buffer, sizeof(buffer), fp);

  ostringstream s;
  s << setw(18) << left << "Binary sum" <<
    setw(8) << buffer;
  result = s.str();
  result.erase(result.end() - 1);

  if (result.find("00000") != string::npos)
    result = "";
}

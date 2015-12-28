/* 
   SDS, a bridge single-suit double-dummy quick-trick solver.

   Copyright (C) 2015-16 by Soren Hein.

   See LICENSE and README.
*/

#include <iostream>
#include <fstream>
#include <assert.h>

#include "files.h"

using namespace std;

extern FilesType files;


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


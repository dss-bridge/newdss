/* 
   SDS, a bridge single-suit double-dummy quick-trick solver.

   Copyright (C) 2015-16 by Soren Hein.

   See LICENSE and README.
*/

#ifndef SDS_FILES_H
#define SDS_FILES_H

#include <iostream>
#include <fstream>


struct FilesType
{
  std::ofstream movesAll;
  std::ofstream movesComb;
  std::ofstream movesA;
  std::ofstream movesP;
  std::ofstream statsAll;
  std::ofstream statsComb;
  std::ofstream statsA;
  std::ofstream statsP;
  std::ofstream simpleShort;
  std::ofstream simpleEqual;
  std::ofstream simpleDiff;
  std::ofstream track;
  std::ofstream summary;
  std::ofstream debug;
};

void InitFiles();

void CloseFiles();

#endif


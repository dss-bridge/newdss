/* 
   SDS, a bridge single-suit double-dummy quick-trick solver.

   Copyright (C) 2015-16 by Soren Hein.

   See LICENSE and README.
*/

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <assert.h>

using namespace std;

#include "portab.h"
#include "Header.h"
#include "MoveList.h"
#include "SimpleMoves.h"
#include "ComplexMoves.h"
#include "stats.h"
#include "Twiddle.h"
#include "Dist.h"
#include "LoopHold.h"
#include "files.h"
#include "options.h"
#include "hist.h"
#include "summary.h"
#include "single.h"
#include "const.h"


// Globals

FilesType files;
OptionsType options;
SingleType * singles[14];
MoveList moveList;
vector<unsigned> holdCtr;
SummaryType summary;

unsigned highestDefNo = 0;
unsigned highestAltNo = 0;

void MainIdentify();

void InitSingles();

void FreeSingles();


int main(int argc, char * argv[])
{
  ReadArgs(argc, argv);

  MainIdentify();

  InitTimer();
  InitTimerList();
  StartTimerNo(0);

  InitFiles();
  InitHist();
  InitSingles();
  InitDist();

  if (options.suitLength)
  {
    LoopHold holding;
    holding.Set(options.suitLength, options.counter);
    holding.Print(cout);

    DefList def;
    if (MakeSimpleSingleMoveWrapper(options.suitLength, options.counter, 
      def, holding))
    {
      cout << "Table match:\n\n";
      def.Print();
    }
    else
      cout << "No table match\n\n";

    // If the tables seem suspect, can try this as well.
    // MakeSimpleMoves();

    singles[options.suitLength][options.counter].moveNo = 0;

    DefList def1, def2;
    MakeComplexSingleMove(holding, def1, def2);

    cout << "Direct solve:\n";
    def1.Print();
    def2.Print();
    PrintHoldCounter(files.statsAll);
    FreeSingles();
    CloseFiles();
    exit(1);
  }
  else if (options.numCores > 1)
  {
    MakeSimpleMoves();

    if (! options.tablesOnly)
    {
      cout << "numCores " << options.numCores << "\n";
      MakeComplexMovesParallel();
      moveList.PrintMovesByKeys();
      moveList.PrintLists();
    }
  }
  else
  {
    MakeSimpleMoves();

    if (! options.tablesOnly)
    {
      MakeComplexMoves();
      moveList.PrintMovesByKeys();
      moveList.PrintLists();
    }
  }

  FreeSingles();
  EndTimerNo(0);

  moveList.PrintStats();
  PrintHistLowCards(files.statsAll);
  PrintHist(files.movesAll);
  PrintTimer();
  PrintTimerList();
  PrintHoldCounter(files.statsAll);

  CloseFiles();

  SumFiles();
  PrintSummary(cout);
}


void MainIdentify()
{
  cout << "single\n----------\n";

#if defined(_WIN32) || defined(__CYGWIN__)
  cout << setw(12) << left << "System" << setw(23) << 
    right << "Windows\n";
#if defined(_MSC_VER)
  cout << setw(12) << left << "Compiler" << setw(23) << 
      right << "Microsoft Visual C++\n";
#elif defined(__MINGW32__)
  cout << setw(12) << left << "Compiler" << setw(23) << 
    right << "MinGW\n";
#else
  cout << setw(12) << left << "Compiler" << setw(23) << 
    right << "GNU g++\n";
#endif

#elif defined(__linux)
  cout << setw(12) << left << "System" << 
    right << setw(23) << "Linux\n";
  cout << setw(12) << left << "Compiler" << 
    right << setw(23) << "GNU g++\n";

#elif defined(__APPLE__)
  cout << setw(12) << left << "System" << 
    right << setw(23) << "Apple\n";
#if defined(__clang__)
  cout << setw(12) << left << "Compiler" << 
    right << setw(23) << "clang\n";
#else
  cout << setw(12) << left << "Compiler" << 
    right << setw(23) << "GNU g++\n";
#endif
#endif

#if defined(__cplusplus)
  cout << setw(12) << left << "Dialect" << 
    right << setw(22) << __cplusplus << "\n";
#endif

  cout << "\n";
}

void InitSingles()
{
  singles[0] = nullptr;

  for (unsigned l = 1; l <= 13; l++)
  {
    size_t n = static_cast<size_t>(SDS_NUM_SINGLES[l]);
    singles[l] = static_cast<SingleType *>(malloc(n * sizeof(SingleType)));
  }

  for (unsigned sl = 1; sl <= 13; sl++)
  {
    unsigned n = SDS_NUM_SINGLES[sl];
    for (unsigned c = 0; c < n; c++)
    {
      unsigned tmp = c;
      unsigned count[SDS_HANDS] = {1, 0, 0, 0}; // The ace
      for (unsigned m = 0; m < sl-1; m++)
      {
        count[tmp & 3]++;
        tmp >>= 2;
      }
      singles[sl][c].moveNo = 0;
      singles[sl][c].declLen = (count[0] >= count[2] ? count[0] : count[2]);
      singles[sl][c].minLen = (count[0] <= count[2] ? count[0] : count[2]);
      singles[sl][c].oppLen = (count[1] >= count[3] ? count[1] : count[3]);

      UpdateHist(HIST_ORIG_COUNT, singles[sl][c].declLen, SDS_VOID, false);
    }
  }
}


void FreeSingles()
{
  for (unsigned l = 1; l <= 13; l++)
    free(singles[l]);
}


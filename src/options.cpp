/* 
   SDS, a bridge single-suit double-dummy quick-trick solver.

   Copyright (C) 2015-16 by Soren Hein.

   See LICENSE and README.
*/


#include <iostream>
#include <iomanip>
#include <string>
#include <stdlib.h>
#include <string.h>

#include "single.h"
#include "options.h"
#include "const.h"

using namespace std;


extern OptionsType options;

struct optEntry
{
  string shortName;
  string longName;
  unsigned numArgs;
};

#define SDS_NUM_OPTIONS 7

const optEntry optList[SDS_NUM_OPTIONS] =
{
  {"n", "numcores", 1},
  {"s", "suitlen", 1},
  {"c", "counter", 1},
  {"t", "tablesonly", 0},
  {"h", "histtest", 1},
  {"f", "find", 0},
  {"d", "debug", 1}
};

string shortOptsAll, shortOptsWithArg;

int GetNextArgToken(
  int argc,
  char * argv[]);

void SetDefaults();


void Usage(
  const char base[])
{
  string basename(base);
  const size_t l = basename.find_last_of("\\/");
  if (l != string::npos)
    basename.erase(0, l+1);

  cout <<
    "Usage: " << basename << " [options]\n\n" <<
    "-n, --numcores n   Number of cores to use (1.." << 
      SDS_MAX_CORES << ", default: 1).\n" <<
    "\n" <<
    "-s, --suitlen n    Suit length (2..13, default range).\n" <<
    "\n" <<
    "-c, --counter n    Counter (consistent with suit length).\n" <<
    "\n" <<
    "-t, --tablesonly   Stop after the constant tables (default: no).\n" <<
    "\n" <<
    "-h, --histtest     Histogram (default: 0 = none, 7 = all).\n" <<
    "\n" <<
    "-f, --find         Find and output table candidates.\n" <<
    "\n" <<
    "-d, --debug        3-bit vector (default: 0 = 000).\n" <<
    endl;
}


int nextToken = 1;
char * optarg;

int GetNextArgToken(
  int argc,
  char * argv[])
{
  // 0 means done, -1 means error.

  if (nextToken >= argc)
    return 0;

  string str(argv[nextToken]);
  if (str[0] != '-' || str.size() == 1)
    return -1;

  if (str[1] == '-')
  {
    if (str.size() == 2)
      return -1;
    str.erase(0, 2);
  }
  else if (str.size() == 2)
    str.erase(0, 1);
  else
    return -1;

  for (unsigned i = 0; i < SDS_NUM_OPTIONS; i++)
  {
    if (str == optList[i].shortName || str == optList[i].longName)
    {
      if (optList[i].numArgs == 1)
      {
        if (nextToken+1 >= argc)
          return -1;

        optarg = argv[nextToken+1];
        nextToken += 2;
      }
      else
        nextToken++;

      return str[0];
    }
  }

  return -1;
}


void SetDefaults()
{
  options.numCores = 1;
  options.combSet = false;
  options.suitLength = 0;
  options.counter = 0;
  options.tablesOnly = false;
  options.findFlag = false;

  options.debugFlow = false;
  options.debugDef = false;
  options.debugAlt = false;
}


void PrintOptions()
{
  cout << left;
  cout << setw(12) << "numcores" << setw(12) << 
    options.numCores << "\n";
  if (options.combSet)
  {
    cout << setw(12) << "suitlen" << setw(12) <<
      options.suitLength << "\n";
    cout << setw(12) << "counter" << setw(12) <<
      "0x" << hex << options.counter << dec << "\n";
  }
  else
    cout << setw(12) << "combo" << setw(12) << "not set\n";

  cout << setw(12) << "Table check" <<
    (options.tablesOnly ? "yes" : "no") << "\n";
  cout << setw(12) << "histtest" << setw(12) << 
    options.histCompare << "\n";
  cout << setw(12) << "find" << setw(12) << 
    options.findFlag << "\n";

  cout << setw(12) << "debug" <<
    (options.debugFlow ? "yes" : "no") << "-" <<
    (options.debugDef ? "yes" : "no") << "-" <<
    (options.debugAlt ? "yes" : "no") << "\n";

}


void ReadArgs(
  int argc,
  char * argv[])
{
  for (unsigned i = 0; i < SDS_NUM_OPTIONS; i++)
  {
    shortOptsAll += optList[i].shortName;
    if (optList[i].numArgs)
      shortOptsWithArg += optList[i].shortName;
  }

  SetDefaults();
  if (argc == 1)
    return;

  int c, m;
  bool errFlag = false;
  char * temp;

  while ((c = GetNextArgToken(argc, argv)) > 0)
  {
    switch(c)
    {
      case 'n':
        m = static_cast<int>(strtol(optarg, &temp, 0));
        if (m < 1 || m > SDS_MAX_CORES)
        {
          cout << "Number of cores must be 1..12\n\n";
          nextToken -= 2;
          errFlag = true;
        }
        options.numCores = static_cast<unsigned>(m);
        break;

      case 's':
        m = static_cast<int>(strtol(optarg, &temp, 0));
        if (m < 2 || m > 13)
        {
          cout << "Suit length must be 1..12\n\n";
          nextToken -= 2;
          errFlag = true;
        }
        options.suitLength = static_cast<unsigned>(m);
        break;

      case 'c':
        m = static_cast<int>(strtol(optarg, &temp, 0));
        if (m < 0 || m > static_cast<int>(SDS_NUM_SINGLES[13]))
        {
          cout << "Counter must be 0..2^13\n\n";
          nextToken -= 2;
          errFlag = true;
        }
        options.counter = static_cast<unsigned>(m);
        break;

      case 't':
        options.tablesOnly = true;
        break;

      case 'h':
        m = static_cast<int>(strtol(optarg, &temp, 0));
        if (m > 7)
        {
          cout << "Histogram must be 0..7\n\n";
          nextToken -= 2;
          errFlag = true;
        }
        options.histCompare = static_cast<unsigned>(m);
        break;

      case 'f':
        options.findFlag = true;
        break;

      case 'd':
        m = static_cast<int>(strtol(optarg, &temp, 0));
        if (m > 7)
        {
          cout << "Debug must be 0..7\n\n";
          nextToken -= 2;
          errFlag = true;
        }
        options.debugFlow = ((m & 0x4) != 0);
        options.debugDef = ((m & 0x2) != 0);
        options.debugAlt = ((m & 0x1) != 0);
        break;

      default:
        cout << "Unknown option\n";
        errFlag = true;
        break;
    }
    if (errFlag)
      break;
  }

  if (errFlag || c == -1)
  {
    cout << "Error parsing option '" << argv[nextToken] << "'" << endl;
    exit(0);
  }
}


/* 
   SDS, a bridge single-suit double-dummy quick-trick solver.

   Copyright (C) 2015-16 by Soren Hein.

   See LICENSE and README.
*/

#ifndef SDS_TRAILS_H
#define SDS_TRAILS_H

#include "Trick.h"
#include "const.h"
#include "compare.h"

class Segment;


class Trails
{
  private:

    bool firstFlag;
    bool active[4];
    Trick trails[4];

    void Extend(
      const Trick& tNew);

    void Increment(
      Trick& trail,
      const unsigned base,
      const Trick& tNew);

  public:

    Trails();

    ~Trails();

    void Add(
        const Segment& seg);

    void Combine(
        const Trails trailList[],
        const unsigned len);

    CmpDetailType Compare(
        const Trails& tNew) const;
        
    void Print(
      std::ostream& out = std::cout) const;
};

#endif


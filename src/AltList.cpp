/* 
   SDS, a bridge single-suit double-dummy quick-trick solver.

   Copyright (C) 2015-16 by Soren Hein.

   See LICENSE and README.
*/


#include <assert.h>

#include "Trick.h"
#include "AltList.h"
#include "TrickList.h"
#include "AltMatrix1D.h"
#include "AltMatrix2D.h"
#include "Holding.h"
#include "Header.h"
#include "files.h"
#include "options.h"

using namespace std;


extern FilesType files;
extern OptionsType options;
extern unsigned highestAltNo;


const bool cmpDetailToGE[SDS_HEADER_CMP_SIZE] =
{
  true,
  false,
  true,
  false,
  false,
  true,
  false
};


AltList::AltList()
{
  AltList::Reset();
}


AltList::~AltList()
{
}


void AltList::Reset()
{
  len = 0;
}


bool AltList::Set1(
  const Trick& trick)
{
  len = 1;
  return list[0].Set1(trick);
}


bool AltList::Set2(
  const Trick& trick10,
  const Trick& trick11)
{
  len = 1;
  return list[0].Set2(trick10, trick11);
}


bool AltList::Set3(
  const Trick& trick10,
  const Trick& trick11,
  const Trick& trick12)
{
  len = 1;
  return list[0].Set3(trick10, trick11, trick12);
}


bool AltList::Set11(
  const Trick& trick1,
  const Trick& trick2)
{
  len = 2;
  (void) list[0].Set1(trick1);
  return list[1].Set1(trick2);
}


bool AltList::Set12(
  const Trick& trick1,
  const Trick& trick20,
  const Trick& trick21)
{
  len = 2;
  (void) list[0].Set1(trick1);
  return list[1].Set2(trick20, trick21);
}


bool AltList::Set13(
  const Trick trick[])
{
  len = 2;
  (void) list[0].Set1(trick[0]);
  return list[1].Set3(trick[1], trick[2], trick[3]);
}


bool AltList::Set14(
  const Trick trick[])
{
  len = 2;
  (void) list[0].Set1(trick[0]);
  return list[1].Set4(trick[1], trick[2], trick[3], trick[4]);
}


bool AltList::Set31(
  const Trick trick[])
{
  len = 2;
  (void) list[0].Set21(trick[0], trick[1], trick[2]);
  return list[1].Set1(trick[3]);
}


bool AltList::Set111(
  const Trick trick[])
{
  len = 3;
  (void) list[0].Set1(trick[0]);
  (void) list[1].Set1(trick[1]);
  return list[2].Set1(trick[2]);
}


bool AltList::Set112(
  const Trick trick[])
{
  len = 3;
  (void) list[0].Set1(trick[0]);
  (void) list[1].Set1(trick[1]);
  return list[2].Set2(trick[2], trick[3]);
}


bool AltList::Set122(
  const Trick trick[])
{
  len = 3;
  (void) list[0].Set1(trick[0]);
  (void) list[1].Set2(trick[1], trick[2]);
  return list[2].Set2(trick[3], trick[4]);
}


bool AltList::Set123(
  const Trick trick[])
{
  len = 3;
  (void) list[0].Set1(trick[0]);
  (void) list[1].Set2(trick[1], trick[2]);
  return list[2].Set3(trick[3], trick[4], trick[5]);
}


bool AltList::Set113(
  const Trick trick[])
{
  len = 3;
  (void) list[0].Set1(trick[0]);
  (void) list[1].Set1(trick[1]);
  return list[2].Set3(trick[2], trick[3], trick[4]);
}


bool AltList::Set114(
  const Trick trick[])
{
  len = 3;
  (void) list[0].Set1(trick[0]);
  (void) list[1].Set1(trick[1]);
  return list[2].Set4(trick[2], trick[3], trick[4], trick[5]);
}


bool AltList::Set1112(
  const Trick trick[])
{
  len = 4;
  (void) list[0].Set1(trick[0]);
  (void) list[1].Set1(trick[1]);
  (void) list[2].Set1(trick[2]);
  return list[3].Set2(trick[3], trick[4]);
}


bool AltList::Set1122(
  const Trick trick[])
{
  len = 4;
  (void) list[0].Set1(trick[0]);
  (void) list[1].Set1(trick[1]);
  (void) list[2].Set2(trick[2], trick[3]);
  return list[3].Set2(trick[4], trick[5]);
}


bool AltList::Set1123(
  const Trick trick[])
{
  len = 4;
  (void) list[0].Set1(trick[0]);
  (void) list[1].Set1(trick[1]);
  (void) list[2].Set2(trick[2], trick[3]);
  return list[3].Set3(trick[4], trick[5], trick[6]);
}


bool AltList::Set1124(
  const Trick trick[])
{
  len = 4;
  (void) list[0].Set1(trick[0]);
  (void) list[1].Set1(trick[1]);
  (void) list[2].Set2(trick[2], trick[3]);
  return list[3].Set4(trick[4], trick[5], trick[6], trick[7]);
}


bool AltList::Set11223(
  const Trick trick[])
{
  len = 5;
  (void) list[0].Set1(trick[0]);
  (void) list[1].Set1(trick[1]);
  (void) list[2].Set2(trick[2], trick[3]);
  (void) list[3].Set2(trick[4], trick[5]);
  return list[4].Set3(trick[6], trick[7], trick[8]);
}


void AltList::GetHeader(
  Header& header) const
{
  assert(len > 0);

  list[0].GetHeader(header);
  Header hNew;
  for (unsigned a = 1; a < len; a++)
  {
    list[a].GetHeader(hNew);
    header.MergeMax(hNew);
  }
}


unsigned AltList::GetLength() const
{
  return len;
}


unsigned AltList::GetComplexity() const
{
  unsigned c = 0;
  for (unsigned a = 0; a < len; a++)
    c += list[a].GetComplexity();
  return c;
}


unsigned AltList::GetTricks() const
{
  unsigned t = 0;
  for (unsigned l = 0; l < len; l++)
  {
    unsigned tNew = list[l].GetTricks();
    t = Max(t, tNew);
  }
  return t;
}


CmpDetailType AltList::Compare(
  const AltList& aNew) const
{
  unsigned numOld = len;
  unsigned numNew = aNew.len;

  AltMatrix2D comp;
  comp.SetDimensions(numOld, numNew);

  for (unsigned lOld = 0; lOld < numOld; lOld++)
    for (unsigned lNew = 0; lNew < numNew; lNew++)
      comp.SetValue(lOld, lNew, list[lOld].Compare(aNew.list[lNew]));

  CmpDetailType c = comp.Compare();
  if (c != SDS_HEADER_PLAY_DIFFERENT && c != SDS_HEADER_RANK_DIFFERENT)
    return c;

  if (AltList::CompareMultiSide(QT_PARD, comp, aNew))
    return SDS_HEADER_PLAY_OLD_BETTER;
  else if (aNew.CompareMultiSide(QT_ACE, comp, * this))
    return SDS_HEADER_PLAY_NEW_BETTER;
  else
    return c;
}


CmpDetailType AltList::CompareWithExpand(
  const AltList& aNew) const
{
  CmpDetailType c0 = AltList::Compare(aNew);
  if (c0 != SDS_HEADER_PLAY_OLD_BETTER &&
      c0 != SDS_HEADER_PLAY_NEW_BETTER)
    return c0;

  bool expandList[SDS_MAX_ALT];
  if (AltList::CanExpand(expandList))
  {
    AltList oldExpanded;
    AltList::ExpandInto(oldExpanded, expandList);
    oldExpanded.HardReduce();

    if (aNew.CanExpand(expandList))
    {
      AltList newExpanded;
      aNew.ExpandInto(newExpanded, expandList);
    newExpanded.HardReduce();
      return oldExpanded.Compare(newExpanded);
    }
    else
      return oldExpanded.Compare(aNew);
  }
  else if (aNew.CanExpand(expandList))
  {
    AltList newExpanded;
    aNew.ExpandInto(newExpanded, expandList);
    newExpanded.HardReduce();
    return AltList::Compare(newExpanded);
  }
  else
    return c0;
}


CmpDetailType AltList::CompareSemiHard(
  const AltList& aNew) const
{
  unsigned numOld = len;
  unsigned numNew = aNew.len;

  AltMatrix2D comp;
  comp.SetDimensions(numOld, numNew);

  for (unsigned lOld = 0; lOld < numOld; lOld++)
    for (unsigned lNew = 0; lNew < numNew; lNew++)
      comp.SetValue(lOld, lNew, list[lOld].Compare(aNew.list[lNew]));

  CmpDetailType c = comp.CompareHard();
  if (c != SDS_HEADER_PLAY_DIFFERENT && c != SDS_HEADER_RANK_DIFFERENT)
    return c;

  if (AltList::CompareMultiSide(QT_PARD, comp, aNew,
      cmpDetailHardMatrix))
  {
    // Bit of a kludge.  If the new one wins a soft comparison,
    // then it's probably too complex to decide here.
    // 13 - 0x8328ff.
    if (aNew.CompareMultiSide(QT_ACE, comp, * this))
      return SDS_HEADER_PLAY_DIFFERENT;
    else
      return SDS_HEADER_PLAY_OLD_BETTER;
  }
  else if (aNew.CompareMultiSide(QT_ACE, comp, * this,
      cmpDetailHardMatrix))
  {
    if (AltList::CompareMultiSide(QT_PARD, comp, aNew))
      return SDS_HEADER_PLAY_DIFFERENT;
    else
      return SDS_HEADER_PLAY_NEW_BETTER;
  }
  else
    return c;
}


CmpDetailType AltList::CompareAlmostHard(
  const AltList& aNew) const
{
  unsigned numOld = len;
  unsigned numNew = aNew.len;

  AltMatrix2D comp;
  comp.SetDimensions(numOld, numNew);

  for (unsigned lOld = 0; lOld < numOld; lOld++)
    for (unsigned lNew = 0; lNew < numNew; lNew++)
      comp.SetValue(lOld, lNew, list[lOld].Compare(aNew.list[lNew]));

  CmpDetailType c = comp.CompareHard();
  if (c != SDS_HEADER_PLAY_DIFFERENT && c != SDS_HEADER_RANK_DIFFERENT)
    return c;

  if (AltList::CompareMultiSide(QT_PARD, comp, aNew,
      cmpDetailHardMatrix))
  {
    // Unlike CompareSemiHard, don't worry about the soft comparison
    // in the opposite direction.
    return SDS_HEADER_PLAY_OLD_BETTER;
  }
  else if (aNew.CompareMultiSide(QT_ACE, comp, * this,
      cmpDetailHardMatrix))
  {
    return SDS_HEADER_PLAY_NEW_BETTER;
  }
  else
    return c;
}


CmpDetailType AltList::CompareHard(
  const AltList& aNew) const
{
  unsigned numOld = len;
  unsigned numNew = aNew.len;

  AltMatrix2D comp;
  comp.SetDimensions(numOld, numNew);

  for (unsigned lOld = 0; lOld < numOld; lOld++)
    for (unsigned lNew = 0; lNew < numNew; lNew++)
      comp.SetValue(lOld, lNew, list[lOld].Compare(aNew.list[lNew]));

  return comp.CompareHard();
}


void AltList::FixRanks(
  const unsigned rLower,
  const unsigned tLower)
{
  for (unsigned l = 0; l < len; l++)
  {
    if (list[l].GetTricks() == tLower)
      list[l].FixRanks(rLower);
  }
}


bool AltList::CanExpand(
  bool expandList[]) const
{
  for (unsigned l = 0; l < len; l++)
  {
    expandList[l] = list[l].CanExpand();
    if (expandList[l])
      return true;
  }

  return false;
}


void AltList::ExpandInto(
  AltList& aNew,
  const bool expandList[]) const
{
  // Somewhat duplicative, but at least we only make a new AltList
  // when we really need one.
  for (unsigned l = 0; l < len; l++)
  {
    if (expandList[l])
    {
      assert(aNew.len < SDS_MAX_ALT-1);
      TrickList tl;
      list[l].ExpandEnd(QT_ACE, tl);
      aNew.list[aNew.len++] = tl;
      list[l].ExpandEnd(QT_PARD, tl);
      aNew.list[aNew.len++] = tl;
    }
    else
    {
      assert(aNew.len < SDS_MAX_ALT);
      aNew.list[aNew.len++] = list[l];
    }
  }

  aNew.RegisterSize("EXPALT");
}


bool AltList::operator == (
  const AltList& alt2) const
{
  if (len != alt2.len)
    return false;

  vector<bool> same1(len, false);
  vector<bool> same2(len, false);
  unsigned num = 0;

  for (unsigned a2 = 0; a2 < len; a2++)
  {
    if (same2[a2])
      continue;

    for (unsigned a1 = 0; a1 < len; a1++)
    {
      if (same1[a1])
        continue;

      if (list[a1] == alt2.list[a2])
      {
        num++;
        same1[a1] = true;
        same2[a2] = true;
      }
    }
  }

  return (num == len);
}


bool AltList::operator != (
  const AltList& alt2) const
{
  return ! (* this == alt2);
}


void AltList::operator += (
  const Holding& holding)
{
  for (unsigned a = 0; a < len; a++)
    list[a] += holding;
  
  // TrickLists of the form PA29 + BP1- + AA1- can be expanded to
  // PP39 + AA1- or PA39.
/* */
  for (unsigned a1 = len; a1 >= 1; a1--)
  {
    if (! list[a1-1].CanSplit())
      continue;

// AltList::Print(cout, "before");
    // Copy the one to be expanded as well.
    for (unsigned a2 = len; a2 >= a1; a2--)
      list[a2] = list[a2-1];
    len++;

    list[a1-1].Split(QT_ACE);
    list[a1].Split(QT_PARD);
// AltList::Print(cout, "after");
  }
/* */

  AltList::Reduce();
}


AltList AltList::operator + (
  const AltList& aRight) const
{
  // We might change the alts, so we have to copy them (ugh).
  AltList aOld = * this;
  AltList aNew = aRight;

  if (options.debugAlt)
  {
    aOld.Print(files.debug, "AltList::++ aOld");
    aNew.Print(files.debug, "AltList::++ aNew");
    // aOld.Print(cout, "AltList::++ aOld");
    // aNew.Print(cout, "AltList::++ aNew");
  }

  vector<bool> softX(aOld.len, true);
  vector<bool> softY(aNew.len, true);

  AltMatrix1D compX, compY;
  compX.SetDimension(aOld.len);
  compY.SetDimension(aNew.len);

  aOld.FillMatrix2D(aNew, compX, compY, softX, softY);

  aOld.PurgeList(compX);

  for (unsigned a2 = 0; a2 < aNew.len; a2++)
  {
    if (compY.IsPurged(a2))
     continue;

    assert(aOld.len < SDS_MAX_ALT);
    aOld.list[aOld.len++] = aNew.list[a2];
  }

  aOld.RegisterSize("NEWALT");

  aOld.Reduce();

  if (options.debugAlt)
    aOld.Print(files.debug, "AltList::++ result");
    // aOld.Print(cout, "AltList::++ result");

  return aOld;
}


bool AltList::CanMergeSides(
  const AltList& aNew) const
{
  if (len != aNew.len)
    return false;

  vector<bool> oldUsed(len, false);
  vector<bool> newUsed(len, false);
  unsigned count = 0;

  for (unsigned a1 = 0; a1 < len; a1++)
  {
    if (oldUsed[a1])
      continue;

    for (unsigned a2 = 0; a2 < len; a2++)
    {
      if (newUsed[a2])
        continue;

      if (list[a1].EqualsExceptStart(aNew.list[a2]))
      {
        oldUsed[a1] = true;
        newUsed[a2] = true;
        count++;
      }
    }
  }

  return (count == len);
}


bool AltList::MergeSoftSpecial12(
  const AltList& aNew) const
{
  return 
    (list[0].IsSimpleComplement(aNew.list[0]) ||
     list[0].IsSimpleComplement(aNew.list[1]));
}


void AltList::Concatenate(
  const AltList& aNew)
{
  for (unsigned l = 0; l < aNew.len; l++)
    list[len+l] = aNew.list[l];
  len += aNew.len;
}


bool AltList::MergeSoftSpecial(
  const AltList& aNew) const
{
  // The regular merge (currently) takes AKT9 / - / Q87 / J654 and
  // turns AP3Q / AA4T + PB4T into BB4T.  This is avoided here, for now.

  if (len == 1 && aNew.len == 2)
    return AltList::MergeSoftSpecial12(aNew);
  else if (len == 2 && aNew.len == 1)
    return aNew.MergeSoftSpecial12(* this);
  else
    return false;
}


void AltList::SetStart()
{
  for (unsigned a = 0; a < len; a++)
    list[a].SetStart(QT_BOTH);

  AltList::Reduce();
}


bool AltList::MergeSides(
  const AltList& aNew) 
{
  if (! AltList::CanMergeSides(aNew))
    return false;

  AltList::SetStart();
  return true;
}


void AltList::Backtrack1D(
  AltMatrix1D& comp, 
  const unsigned dimFixed, 
  const unsigned dimVar, 
  vector<FixType>& fixVector)
{
  FixType fix1, fix2;
  CmpDetailType c;
 
  if (fixVector[dimFixed] == SDS_FIX_STRONGER)
  {
    for (unsigned b = 0; b < dimVar; b++)
    {
      unsigned a = dimVar-1 - b;
      if (a == dimFixed || comp.IsPurged(a))
        continue;

      c = list[dimFixed].FixOrCompare(list[a], fix1, fix2);
      comp.SetValue(dimFixed, a, c);

      assert(fix1 != SDS_FIX_STRONGER && fix1 != SDS_FIX_PURGED);
      assert(fix2 != SDS_FIX_STRONGER);

      if (fix1 == SDS_FIX_WEAKER)
      {
        // Unusual, but can happen.
        fixVector[dimFixed] = fix1;
	AltList::Backtrack1D(comp, dimFixed, dimVar, fixVector);
      }

      if (fix2 == SDS_FIX_PURGED)
        comp.Purge(a);
      else if (fix2 == SDS_FIX_WEAKER)
      {
        assert(fixVector[a] != SDS_FIX_STRONGER);

        fixVector[a] = fix2;
	AltList::Backtrack1D(comp, a, dimFixed, fixVector);
      }
    }
  }
  else if (fixVector[dimFixed] == SDS_FIX_WEAKER)
  {
    for (unsigned b = 0; b < dimVar; b++)
    {
      unsigned a = dimVar-1 - b;
      if (a == dimFixed || comp.IsPurged(a))
        continue;

      c = list[dimFixed].FixOrCompare(list[a], fix1, fix2);
      comp.SetValue(dimFixed, a, c);

      assert(fix1 != SDS_FIX_WEAKER);
      // assert(fix2 != SDS_FIX_WEAKER);

      if (fix1 == SDS_FIX_PURGED)
      {
        comp.Purge(dimFixed);
        break;
      }

      if (fix2 == SDS_FIX_STRONGER || fix2 == SDS_FIX_WEAKER)
      {
        assert(fixVector[a] == SDS_FIX_UNCHANGED);

        fixVector[a] = fix2;
	AltList::Backtrack1D(comp, a, dimFixed, fixVector);
      }
    }
  }

  if (options.debugAlt)
    files.debug << "Backtrack1D done\n";
}


void AltList::FillMatrix1D(
  AltMatrix1D& comp)
{
  FixType fix1, fix2;

  for (unsigned a1 = 0; a1 < len-1; a1++)
  {
    if (comp.IsPurged(a1))
      continue;

    for (unsigned a2 = a1+1; a2 < len; a2++)
    {
      if (comp.IsPurged(a2))
        continue;

      comp.SetValue(a1, a2, list[a1].FixOrCompare(list[a2], fix1, fix2));

      if (fix2 == SDS_FIX_PURGED)
        comp.Purge(a2);
      else if (fix2 == SDS_FIX_STRONGER || fix2 == SDS_FIX_WEAKER)
      {
        vector<FixType> fixVector(len, SDS_FIX_UNCHANGED);
	fixVector[a2] = fix2;

	AltList::Backtrack1D(comp, a2, a1+1, fixVector);
      }

      if (fix1 == SDS_FIX_PURGED && ! comp.IsPurged(a1))
        comp.Purge(a1);
      else if (fix1 == SDS_FIX_STRONGER || fix1 == SDS_FIX_WEAKER)
      {
        vector<FixType> fixVector(len, SDS_FIX_UNCHANGED);
	fixVector[a1] = fix1;

	AltList::Backtrack1D(comp, a1, a2+1, fixVector);
      }

      if (comp.IsPurged(a1))
        break;
    }
  }
}


void AltList::FillMatrix2D(
  AltList& aNew,
  AltMatrix1D& compX,
  AltMatrix1D& compY,
  const vector<bool>& softX,
  const vector<bool>& softY)
{
  // The soft lists are true for those entries that have just
  // been weakened or strengthened.

  FixType fix1, fix2;

  vector<bool> nextSoftX(len, false);
  vector<bool> nextSoftY(aNew.len, false);

  for (unsigned a1 = 0; a1 < len; a1++)
  {
    if (compX.IsPurged(a1))
      continue;

    for (unsigned a2 = 0; a2 < aNew.len; a2++)
    {
      if ((! softX[a1] && ! softY[a2]) || compY.IsPurged(a2))
        continue;

      // Don't really need c.
      CmpDetailType c = list[a1].FixOrCompare(aNew.list[a2], fix1, fix2);
      if (fix1 == SDS_FIX_UNCHANGED && fix2 == SDS_FIX_UNCHANGED)
        continue;

      if (fix2 == SDS_FIX_PURGED)
      {
        compY.Purge(a2);
        nextSoftY[a2] = false;
      }
      else if (fix2 == SDS_FIX_STRONGER || fix2 == SDS_FIX_WEAKER)
      {
        nextSoftY[a2] = true;
        vector<FixType> fixVector(aNew.len, SDS_FIX_UNCHANGED);
        fixVector[a2] = fix2;

        aNew.Backtrack1D(compY, a2, aNew.len, fixVector);
      }

      if (fix1 == SDS_FIX_PURGED && ! compX.IsPurged(a1))
      {
        compX.Purge(a1);
        nextSoftX[a1] = false;
      }
      else if (fix1 == SDS_FIX_STRONGER || fix1 == SDS_FIX_WEAKER)
      {
        nextSoftX[a1] = true;
        vector<FixType> fixVector(len, SDS_FIX_UNCHANGED);
        fixVector[a1] = fix1;

        AltList::Backtrack1D(compX, a1, len, fixVector);
      }

      if (compX.IsPurged(a1))
        break;
    }
  }

  if (AltList::AllFalse(nextSoftX, len) && 
      AltList::AllFalse(nextSoftY, len))
    return;

  AltList::FillMatrix2D(aNew, compX, compY, nextSoftX, nextSoftY);
}


bool AltList::AllFalse(
  const vector<bool>& vec, 
  const unsigned vlen) const
{
  for (unsigned i = 0; i < vlen; i++)
    if (vec[i])
      return false;

  return true;
}


void AltList::PurgeList(
  const AltMatrix1D& comp)
{
  unsigned p = 0;
  for (unsigned a1 = 0; a1 < len; a1++)
  {
    if (comp.IsPurged(a1))
      continue;

    if (a1 > p)
      list[p] = list[a1];
    p++;
  }

  len = p; 

  assert(p <= SDS_MAX_ALT);
}


void AltList::PurgeList(
  const vector<bool>& purgeList)
{
  // Later think of a way to have only one PurgeList function.

  unsigned p = 0;
  for (unsigned a1 = 0; a1 < len; a1++)
  {
    if (purgeList[a1])
      continue;

    if (a1 > p)
      list[p] = list[a1];
    p++;
  }

  len = p; 

  assert(p <= SDS_MAX_ALT);
}


void AltList::PunchOut(
  const AltList * alist,
  const unsigned purgeNo,
  const vector<bool>& purgeList,
  const PosType pstart)
{
  len = 0;
  for (unsigned a = 0; a < alist->len; a++)
  {
    if (a == purgeNo || 
        purgeList[a] || 
        alist->list[a].GetFirstStart() == SDS_PARTNER[pstart])
      continue;
    
    list[len++] = alist->list[a];
  }
}


void AltList::PunchOut(
  const AltList * alist,
  const PosType pstart)
{
  len = 0;
  for (unsigned a = 0; a < alist->len; a++)
  {
    if (alist->list[a].GetFirstStart() == SDS_PARTNER[pstart])
      continue;
    
    list[len++] = alist->list[a];
  }
}


void AltList::Reduce()
{
  if (len <= 1)
    return;

  if (options.debugAlt)
    AltList::Print(files.debug, "AltList::Reduce before");

  AltMatrix1D comp;
  comp.SetDimension(len);

  AltList::FillMatrix1D(comp);

  AltList::PurgeList(comp);

  AltList::PurgeMulti();

  if (options.debugAlt)
    AltList::Print(files.debug, "AltList::Reduce after");
}


void AltList::HardReduce()
{
  vector<bool> purgeList(len, false);

  for (unsigned a1 = 0; a1 < len-1; a1++)
  {
    if (purgeList[a1])
      continue;

    for (unsigned a2 = a1+1; a2 < len; a2++)
    {
      if (purgeList[a2])
        continue;

      CmpDetailType c = list[a1].Compare(list[a2]);

      if (c == SDS_HEADER_PLAY_OLD_BETTER ||
          c == SDS_HEADER_RANK_OLD_BETTER ||
          c == SDS_HEADER_SAME)
        purgeList[a2] = true;
      else if (c == SDS_HEADER_PLAY_NEW_BETTER ||
          c == SDS_HEADER_RANK_NEW_BETTER)
        purgeList[a1] = true;

      if (purgeList[a1])
        break;
    }
  }

  AltList::PurgeList(purgeList);
}


void AltList::ReduceSpecial()
{
  // Somewhat inconsistently, consider BBnr to beat other ways
  // to get the same number of tricks, even with better rank.
  // Similarly for ABnr or PBns.
  
  if (len <= 1)
    return;
  
  bool seenA = false, seenP = false;
  unsigned na = 0xffff, np = 0xffff, aOther = 0xffff;

  for (unsigned a = 0; a < len; a++)
  {
    unsigned nb = list[a].xBNo(QT_BOTH);
    if (nb > 0)
    {
      AltList::ReduceIfDominated(nb, a, a);
      return;
    }
    else if (seenA)
    {
      np = list[a].xBNo(QT_PARD);
      if (np > 0)
      {
        if (na == np)
          AltList::ReduceIfDominated(na, a, aOther);
        return;
      }
    }
    else if (seenP)
    {
      na = list[a].xBNo(QT_ACE);
      if (na > 0)
      {
        if (na == np)
          AltList::ReduceIfDominated(na, a, aOther);
        return;
      }
    }
    else
    {
      na = list[a].xBNo(QT_ACE);
      if (na > 0)
      {
        seenA = true;
        aOther = a;
        continue;
      }
      
      np = list[a].xBNo(QT_PARD);
      if (np > 0)
      {
        seenP = true;
        aOther = a;
      }
    }
  }
}


void AltList::ReduceIfDominated(
  const unsigned n,
  const unsigned a0,
  const unsigned a1)
{
  for (unsigned a = 0; a < len; a++)
  {
    if (a == a0 || a == a1)
      continue;

    if (list[a].GetTricks() > n)
      return;
  }

  vector<bool> purgeList(len, true);
  purgeList[a0] = false;
  purgeList[a1] = false;
  AltList::PurgeList(purgeList);
}


void AltList::PurgeMulti()
{
  if (len < 3)
    return;

  AltList aRed;
  TrickList tlist;
  vector<bool> purgeList(len, false);
  for (unsigned a = 0; a < len; a++)
  {
    tlist = list[a];
    PosType pstart = tlist.GetFirstStart();
    if (tlist.GetLength() == 1 || pstart == QT_BOTH)
      continue;

    aRed.PunchOut(this, a, purgeList, pstart);
    if (aRed.len < 2)
      continue;

    if (cmpDetailToGE[aRed.CompareMultiTrickList(tlist)])
      purgeList[a] = true;
  }

  AltList::PurgeList(purgeList);
}


CmpDetailType AltList::CompareMulti(
  const TrickList& tref,
  const CmpDetailType cmpMat[][SDS_HEADER_CMP_SIZE]) const
{
  if (len < 2)
    return SDS_HEADER_PLAY_DIFFERENT;

  AltList aRed;
  TrickList tlist = tref;
  PosType pstart = tlist.GetFirstStart();
  if (pstart == QT_BOTH)
    return SDS_HEADER_PLAY_DIFFERENT;

  aRed.PunchOut(this, pstart);
  if (aRed.len < 2)
    return SDS_HEADER_PLAY_DIFFERENT;

  return aRed.CompareMultiTrickList(tlist, cmpMat);
}


bool AltList::CompareMultiSide(
  const PosType sideToLose,
  const AltMatrix2D& comp,
  const AltList& altToLose,
  const CmpDetailType cmpMat[][SDS_HEADER_CMP_SIZE]) const
{
  bool use[SDS_MAX_ALT];
  CmpDetailType cRunning;
  if (! comp.CandList(sideToLose, use, cRunning))
    return false;

  for (unsigned a = 0; a < altToLose.len; a++)
  {
    if (! use[a])
      continue;

    CmpDetailType c = AltList::CompareMulti(altToLose.list[a], cmpMat);
    if (c == SDS_HEADER_PLAY_DIFFERENT)
      return false;
    else
      cRunning = cmpMat[cRunning][c];
  }
  return cmpDetailToGE[cRunning];
}


CmpDetailType AltList::CompareMultiTrickList(
  TrickList& tlist,
  const CmpDetailType cmpMat[][SDS_HEADER_CMP_SIZE])
{
  // Both AltList and tlist are modified in this call!
  // They should be temporary to the caller.
  // Returns true if AltList in totality is >= tlist,
  // possibly using more than one trick list to do so.
  // The comparison is done one segment at a time, then
  // connecting up the segments.

  unsigned tlen = tlist.GetLength();

  CmpDetailType cRunning = SDS_HEADER_SAME;
  for (unsigned s = 0; s < tlen; s++)
  {
    CmpDetailType c = AltList::FrontIsGE(tlist, cmpMat);
    if (c == SDS_HEADER_PLAY_DIFFERENT)
      return SDS_HEADER_PLAY_DIFFERENT;
    cRunning = cmpMat[cRunning][c];
    
    PosType pend = tlist.ConnectFirst();
    AltList::ConnectFirst(pend);
  }
  return cRunning;
}


CmpDetailType AltList::FrontIsGE(
  const TrickList& tlist,
  const CmpDetailType cmpMat[][SDS_HEADER_CMP_SIZE]) const
{
  // Uses same return values as below.

  Trick htrick;
  tlist.GetFirstSummaryTrick(htrick);
  if (htrick.GetEnd() == QT_BOTH)
  {
    htrick.SetEnd(QT_ACE);
    CmpDetailType ca = AltList::FrontIsGE(htrick);

    if (ca == SDS_HEADER_PLAY_DIFFERENT)
      return SDS_HEADER_PLAY_DIFFERENT;

    htrick.SetEnd(QT_PARD);
    CmpDetailType cp = FrontIsGE(htrick);
    if (cp == SDS_HEADER_PLAY_DIFFERENT)
      return SDS_HEADER_PLAY_DIFFERENT;

    return cmpMat[ca][cp];
  }
  else
    return AltList::FrontIsGE(htrick);
}


CmpDetailType AltList::FrontIsGE(
  const Trick& trick) const
{
  // Only uses SDS_HEADER_SAME, SDS_HEADER_PLAY_OLD_BETTER,
  // SDS_HEADER_RANK_NEW_BETTER, SDS_PLAY_DIFFERENT.

  bool rankNewFlag = false;
  for (unsigned a = 0; a < len; a++)
  {
    switch(list[a].CompareToTrick(trick))
    {
      case SDS_HEADER_PLAY_OLD_BETTER:
        return SDS_HEADER_PLAY_OLD_BETTER;
      case SDS_HEADER_SAME:
      case SDS_HEADER_RANK_OLD_BETTER:
        return SDS_HEADER_SAME;
      case SDS_HEADER_RANK_NEW_BETTER:
        rankNewFlag = true;
        break;
      case SDS_HEADER_PLAY_NEW_BETTER:
      case SDS_HEADER_PLAY_DIFFERENT:
      case SDS_HEADER_RANK_DIFFERENT:
      default:
        break;
    }
  }
  return (rankNewFlag ? SDS_HEADER_RANK_NEW_BETTER : 
    SDS_HEADER_PLAY_DIFFERENT);
}


void AltList::ConnectFirst(
  const PosType pend)
{
  for (unsigned a = 0; a < len; a++)
    list[a].ConnectFirst(pend);
}


void AltList::RegisterSize(
  const string& text)
{
  if (len <= highestAltNo)
    return;

  highestAltNo = len;
  files.track << text << ": " << len << "\n";

  if (len > SDS_MAX_ALT)
  {
    cout << "Too many alternatives" << endl;
    assert(false);
  }
}


void AltList::Print(
  std::ostream& out,
  const string text,
  const unsigned d) const
{
  if (! text.empty())
    out << text << "\n";

  for (unsigned a = 0; a < len; a++)
    list[a].Print(out, d, a);
}


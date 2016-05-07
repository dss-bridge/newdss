#!perl
use strict;
use warnings;

# PLAN, in order:

# Rank problem detection: Say we have p2.
# All p2 > x, p1 > x, p0 > x are OK.
# All p2 < x, p1 < x, p0 < x too (remember the x'es).
#   All involving x'es (and if p2 > a3, then also p1 OK) are OK.
# In a Min(), do it for both.
# In an absolute, no relative comparisons.
# Add good C comment for this:
#     // PROBLEM: p3 > a4, p3 > r3.

# Can perhaps simplify a2>p2 p2>r2 a2>r2 (lose a2>r2)
# Solution is to punch out.


# Check that profile never triggers in other branches for that
# length pattern.  Have to check each entry, not just the accum.
#
# Go through to find any branch at all with this property.
# Output it.
# Once these are gone, perhaps the ignore list should expand.
# Then we do the same again, until hopefully we're done.
# If there is more than one option, take the one with the smallest
# example "weight", then the lowest distribution number.
# Or the highest weight (most specific)?
# 
# Can leave the reduction to the end, after finding patterns.
#
# Reduction idea:
# We really want to get all patterns within a branch, to make simple code.
# Find a branch with one pattern, or where all patterns have same
# example (here branches 4-7).
# Can output distHex masks + the simple code.
# That leaves branches 0-3.
# Actually take one such branch, then somehow rerun as the global/local
# split may have simplified.
#


# Formats.
my $indent = ' ' x 2;
my $runIndex = 0x1000;

# Global constants.
my (%cardsToShort, @hands, %ranks, @cardNames, %fullPlayer, %posName);
my (@htopNames, @hfulltopNames, %hfulltopNumber);
setConstants();

# Global variables.
my (@entries, @profiles, %common);
my (@examples, @exampleCount, @exampleEntryNo, @exampleDistNo);
my $numExamples = 0;

parse(pop);
printExamples();

my (%empty, %reducedProfile);
getCommonProfile(\%common);
extractReducedProfile(\%common, \%empty, \%reducedProfile);

print "Common profile:\n";
printProfile(\%reducedProfile);
print "\n";

my @branchProfiles;
getAllProfiles(\%common);


sub setConstants
{
  %cardsToShort = (
    cardsAce => 'a', cardsLHO => 'l', cardsPard => 'p', cardsRHO => 'r');

  @hands = (qw(cardsAce cardsLHO cardsPard cardsRHO));

  %ranks = (
    A => 14, K => 13, Q => 12, J => 11, T => 10, 9 => 9, 8 => 8,
    7 => 7, 6 => 6, 5 => 5, 4 => 4, 3 => 3, 2 => 2, 0 => 0);

  @cardNames = (qw(A K Q J T 9 8 7 6 5 4 3 2));

  %fullPlayer = (
    a => 'QT_ACE', p => 'QT_PARD', l => 'QT_LHO', r => 'QT_RHO');

  %posName = (Ace => 'QT_ACE', Pard => 'QT_PARD', Both => 'QT_BOTH');

  @htopNames = (qw(A K Q J T N E S X F O H W));
  
  @hfulltopNames = (qw(SDS_ACE SDS_KING SDS_QUEEN SDS_JACK SDS_TEN 
    SDS_NINE SDS_EIGHT SDS_SEVEN SDS_SIX SDS_FIVE 
    SDS_FOUR SDS_THREE SDS_TWO));

  for my $hno (0 .. $#hfulltopNames)
  {
    $hfulltopNumber{$hfulltopNames[$hno]} = $hno;
  }
}



sub parse
{
  open my $fh, '<', pop() or die "Can't < file: $!";
  my @lines = <$fh>;
  close $fh;

  my $eno = 0;
  while (my $line = shift @lines)
  {
    next unless $line =~ /^sl (\d+) c (\w+)/;
    $entries[$eno]{length} = $1;
    $entries[$eno]{count} = $2;

    $line = shift @lines;
    $line =~ /(\w+)/;
    $entries[$eno]{cardsAce} = $1;
    $entries[$eno]{lenAce} = length $1;
    $line = shift @lines;
    $line =~ /(\S+)\s+(\S+)/;
    $entries[$eno]{cardsRHO} = ($1 eq '-' ? '' : $1);
    $entries[$eno]{lenRHO} = length $entries[$eno]{cardsRHO};
    $entries[$eno]{cardsLHO} = ($2 eq '-' ? '' : $2);
    $entries[$eno]{lenLHO} = length $entries[$eno]{cardsLHO};
    $line = shift @lines;
    $line =~ /(\w+)/;
    $entries[$eno]{cardsPard} = $1;
    $entries[$eno]{lenPard} = length $1;

    my %relMap;
    $relMap{'-'} = '-';
    addMap(\%relMap, $entries[$eno]{cardsAce}, 'a');
    addMap(\%relMap, $entries[$eno]{cardsLHO}, 'l');
    addMap(\%relMap, $entries[$eno]{cardsPard}, 'p');
    addMap(\%relMap, $entries[$eno]{cardsRHO}, 'r');

    enterTops(\%{$entries[$eno]}, \%{$profiles[$eno]});
    enterComps(\%{$entries[$eno]}, \%{$profiles[$eno]});

    do
    {
      $line = shift @lines;
    }
    until ($line =~ /Actually solved/);
    $line = shift @lines;

    my (@solText, $l);
    do
    {
      $l = shift @lines;
      chomp $l;
      $l =~ s/\s(\S)$/$relMap{$1}/;
      push @solText, $l;
    }
    until ($l =~ /^\s*$/);
    pop @solText;

    my $exno = getExampleNumber(\@solText);
    $entries[$eno]{example} = $exno;

    my $lkey = getLengthKey(\%{$entries[$eno]});
    if (! defined $exampleEntryNo[$exno]{$lkey})
    {
      $exampleEntryNo[$exno]{$lkey} = $eno;
      $exampleDistNo[$exno]++;
    }
    $exampleCount[$exno]{$lkey}++;

    $eno++;
  }
}


sub addMap
{
  my ($mapref, $s, $h) = @_;

  my $n = 0;
  for my $c (split //, $s)
  {
    $mapref->{$c} = $h . $n++;
  }
}


sub enterTops
{
  my ($entryref, $profileref) = @_;
  for my $h (@hands)
  {
    my $p = $cardsToShort{$h};
    for my $c (split //, $entryref->{$h})
    {
      $profileref->{"has$c"} = $p;
    }
  }
}


sub enterComps
{
  my ($entryref, $profileref) = @_;

  for my $h0 (0 .. 2)
  {
    my $hn0 = $hands[$h0];
    my $p0 = $cardsToShort{$hn0};
    my @array0 = split //, $entryref->{$hn0};
    push @array0, 0 if $#array0 == -1;

    for my $h1 ($h0+1 .. 3)
    {
      my $hn1 = $hands[$h1];
      my $p1 = $cardsToShort{$hn1};
      my @array1 = split //, $entryref->{$hn1};
      push @array1, 0 if $#array1 == -1;

      for my $a0 (0 .. $#array0)
      {
        my $c0 = $p0;
        for my $a1 (0 .. $#array1)
	{
	  my $s = "$p0$a0$p1$a1";
	  $profileref->{$s} = 
	    ($ranks{$array0[$a0]} > $ranks{$array1[$a1]} ? 1 : -1);
	}
      }
    }
  }
}


sub printEntry
{
  my ($entryref) = @_;
  for my $k (sort keys %$entryref)
  {
    print "$k\t$entryref->{$k}\n";
  }
}


sub addProfileEntry
{
  my ($firstref, $strref, $strcumref, $entry) = @_;

  if ($$firstref)
  {
    $$firstref = 0;
  }
  else
  {
    $$strref .= ' && ';
  }

  if (length $$strref > 60)
  {
    $$strcumref .= $$strref . "\n";
    $$strref = $indent . '    ';
  }

  $$strref .= $entry;
}


sub extractReducedProfile
{
  my ($entryref, $comref, $resref) = @_;
  for my $cno (0 .. $#cardNames)
  {
    my $a = 'has' . $cardNames[$cno];
    if (defined $entryref->{$a} && ! defined $comref->{$a})
    {
      $resref->{$a} = $entryref->{$a};
    }
  }

  for my $cno (0 .. $#cardNames)
  {
    my $a = 'opp' . $cardNames[$cno];
    if (defined $entryref->{$a} && ! defined $comref->{$a})
    {
      $resref->{$a} = $entryref->{$a};
    }
  }

  # Make a list of known top cards in order to avoid some comps.
  my %seenTop;
  $seenTop{a} = 0;
  $seenTop{l} = 0;
  $seenTop{p} = 0;
  $seenTop{r} = 0;
  for my $cno (0 .. $#cardNames)
  {
    my $a = 'has' . $cardNames[$cno];
    if (defined $entryref->{$a})
    {
      $seenTop{$entryref->{$a}}++;
    }
    elsif (defined $comref->{$a})
    {
      $seenTop{$comref->{$a}}++;
    }
    else
    {
      last;
    }
  }

  for my $k (sort keys %$entryref)
  {
    next if ($k =~ /has/);
    next if ($k =~ /opp/);
    next if (defined $comref->{$k});

    $k =~ /(.)(\d)(.)(\d)/;
    my ($a, $b, $c, $d) = ($1, $2, $3, $4);

    # Skip comparisons against specifically known tops.
    if ($seenTop{$a} > $b)
    {
      next;
    }
    if ($seenTop{$c} > $d)
    {
      next;
    }

    my ($k1, $k2);
    if ($entryref->{$k} == 1)
    {
      $k1 = $a . ($b+1) . $c . $d;
      $k2 = $a . $b . $c . ($d-1);
    }
    else
    {
      $k1 = $a . ($b-1) . $c . $d;
      $k2 = $a . $b . $c . ($d+1);
    }
    if ((defined $entryref->{$k1} && $entryref->{$k} == $entryref->{$k1}) ||
        (defined $entryref->{$k2} && $entryref->{$k} == $entryref->{$k2}))
    {
      next;
    }
    if ((defined $comref->{$k1} && $entryref->{$k} == $comref->{$k1}) ||
        (defined $comref->{$k2} && $entryref->{$k} == $comref->{$k2}))
    {
      next;
    }
    
    $resref->{$k} = $entryref->{$k};
  }
}


sub printProfile
{
  my ($reducedref) = @_;
  my $strcum = '';
  my $str = $indent . "if (";
  my $len;
  my $first = 1;

  for my $cno (0 .. $#cardNames)
  {
    my $a = 'has' . $cardNames[$cno];
    if (defined $reducedref->{$a})
    {
      my $entry .= 'htop.' . $htopNames[$cno] . ' = ' .  
        $fullPlayer{$reducedref->{$a}};
      addProfileEntry(\$first, \$str, \$strcum, $entry);
    }
  }

  for my $cno (0 .. $#cardNames)
  {
    my $a = 'opp' . $cardNames[$cno];
    if (defined $reducedref->{$a})
    {
      my $entry = '';
      $entry = '! ' if ($reducedref->{$a} == 0);
      $entry .= 'hopp.' . $htopNames[$cno];
      addProfileEntry(\$first, \$str, \$strcum, $entry);
    }
  }

  for my $k (sort keys %$reducedref)
  {
    next if ($k =~ /has/);
    next if ($k =~ /opp/);

    $k =~ /(.)(\d)(.)(\d)/;
    my ($a, $b, $c, $d) = ($1, $2, $3, $4);

    my $entry .= ($reducedref->{$k} == 1 ?  "$a$b > $c$d" : "$c$d > $a$b");
    addProfileEntry(\$first, \$str, \$strcum, $entry);
  }
  print "$strcum$str)\n";
}


sub printExamples
{
  for my $e (0 .. $numExamples-1)
  {
    print "Example number $e:\n";
    printExample($e);
    print "\n";
  }
}


sub printExample
{
  my ($exno) = @_;
  print "Line $_\n" for @{$examples[$exno]};
}


sub printExampleComment
{
  my ($exno, $lkey) = @_;

  my $eref = \%{$entries[$exampleEntryNo[$exno]{$lkey}]};
  print "$indent  // ",
    $eref->{cardsAce}, " / ",
    $eref->{cardsLHO} || '-', " / ",
    $eref->{cardsPard}, " / ",
    $eref->{cardsRHO}, " (",
    $eref->{length}, ", ",
    "0x", $eref->{count}, "), ",
    $exampleCount[$exno]{$lkey},
    " cases.\n";
}


sub extractKnownTops
{
  my ($entryref, $comref, $resref) = @_;

  for my $k (keys %$entryref)
  {
    $resref->{$k} = $entryref->{$k} if $k =~ /has/;
  }

  for my $k (keys %$comref)
  {
    $resref->{$k} = $comref->{$k} if $k =~ /has/;
  }
}


sub printExampleAsCode
{
  my ($exno, $knowntopref, $lkey) = @_;

  # Make a list of known top cards in order to avoid some comps.
  my %countTop;
  $countTop{a} = 0;
  $countTop{l} = 0;
  $countTop{p} = 0;
  $countTop{r} = 0;
  my %seenTop;
  for my $cno (0 .. $#cardNames)
  {
    my $a = 'has' . $cardNames[$cno];
    last unless defined $knowntopref->{$a};
    my $h = $knowntopref->{$a};
    $seenTop{$h . $countTop{$h}++} = $hfulltopNames[$cno];
  }

  my @result;
  my @groupCount;
  my $trickCount = 0;
  my %ranksSeen;
  $ranksSeen{a} = -1;
  $ranksSeen{l} = -1;
  $ranksSeen{p} = -1;
  $ranksSeen{r} = -1;
  for my $line (@{$examples[$exno]})
  {
    $line =~ s/^\s+//;
    my @list = split /\s+/, $line;

    if ($list[0] != 0)
    {
      print "$indent\{\n";
      printExampleComment($exno, $lkey);
      print "$indent  // DEFENSE: Multiple cases.\n";
      printf "%s  REGISTER(0x%x);\n", $indent, $runIndex;
      $runIndex++;
      print "$indent  return false;\n";
      print "$indent\}\n\n";
      return;
    }

    $groupCount[$list[1]]++;
    my $str = "trick[$trickCount].Set(" .
      $posName{$list[3]} . ", " .
      $posName{$list[4]} . ", ";
    $trickCount++;

    my $rank = $list[6];
    if ($rank eq '-')
    {
      $str .= 'SDS_VOID, ';
    }
    else
    {
      my $h = substr $rank, 0, 1;
      my $n = substr $rank, 1, 1;
      if ($n > $ranksSeen{$h})
      {
        $ranksSeen{$h} = $n;
      }
  
      if (defined $seenTop{$rank})
      {
        $str .= $seenTop{$rank} . ", ";
      }
      else
      {
        $str .= $rank . ", ";
      }
    }
    $str .= $list[5] . ");";
    push @result, $str;
  }

  # Determine the lowest rank overall.  Specific ranks are not needed
  # if there is at least one relative rank.
  my $numRel = 0;
  my $numAbs = 0;
  my ($exAbs, $exRel);
  for my $h (qw(a p l r))
  {
    next if ($ranksSeen{$h} == -1);
    my $relRank = $h . $ranksSeen{$h};
    if (defined $seenTop{$relRank})
    {
      $numAbs++;
      $exAbs = $seenTop{$relRank};
    }
    else
    {
      $numRel++;
      $exRel = $relRank;
    }
  }

  my $rank = '';
  if ($numRel == 0)
  {
    die "No ranks at all?" if $numAbs == 0;
    if ($numAbs == 1)
    {
      $rank = $exAbs;
    }
    else
    {
      # Need to pick the lowest.
      my $highestNo = -1;
      for my $h (qw(a p l r))
      {
        next if ($ranksSeen{$h} == -1);
        my $relRank = $h . $ranksSeen{$h};
        next unless defined $seenTop{$relRank};
        my $tag = $seenTop{$relRank};
        $highestNo = $hfulltopNumber{$tag} 
          if $hfulltopNumber{$tag} > $highestNo;
      }
      $rank = $hfulltopNames[$highestNo];
    }
  }
  elsif ($numRel == 1)
  {
    $rank = $exRel;
  }
  elsif ($numRel == 2)
  {
    $rank = 'Min(';
    my $first = 1;
    for my $h (qw(a p l r))
    {
      next if ($ranksSeen{$h} == -1);
      my $relRank = $h . $ranksSeen{$h};
      next if defined $seenTop{$relRank};
      if ($first)
      {
        $rank .= "$relRank, ";
        $first = 0;
      }
      else
      {
        $rank .= "$relRank)";
      }
    }
  }
  else
  {
    die "Minimum of more than two -- can be done";
  }

  print "$indent\{\n";
  printExampleComment($exno, $lkey);
  printf "%s  REGISTER(0x%x);\n", $indent, $runIndex;
  $runIndex++;
  print "$indent  rank = $rank;\n";

  for my $line (@result)
  {
    print "$indent  $line\n";
  }

  print "$indent  return def.Set";
  print join '', @groupCount;
  print "(trick);\n";
  print "$indent\}\n";
}


sub getExampleNumber
{
  my ($solref) = @_;

  my $ls = $#$solref;
  for my $e (0 .. $numExamples)
  {
    next if ($ls != $#{$examples[$e]});
    for my $i (0 .. $ls)
    {
      next if ($solref->[$i] ne $examples[$e][$i]);
    }

    # Found it.
    return $e;
  }
  
  # Didn't find it.
  for my $i (0 .. $ls)
  {
    $examples[$numExamples][$i] = $solref->[$i];
  }
  return $numExamples++;
}


sub getCommonProfile
{
  my ($profileref) = @_;
  my %accum;
  my %empty;
  for my $e (@profiles)
  {
    accumulateProfile($e, \%accum, \%empty, 'x');
  }
  extractProfile(\%accum, $profileref, $#profiles+1);
}


sub accumulateProfile
{
  my ($profileref, $accumref, $ignoreref, $skipOpp) = @_;

  for my $k (keys %$profileref)
  {
    next if (defined $ignoreref->{$k});
    next if ($k !~ /has/ && $k !~ /opp/ && $k =~ /$skipOpp/);
    $accumref->{$k}{$profileref->{$k}}++;
  }
}


sub extractProfile
{
  my ($accumref, $profileref, $targetCount) = @_;

  for my $k (keys %$accumref)
  {
    my $num = 0;
    my $s0;
    for my $s (keys %{$accumref->{$k}})
    {
      $num++;
      $s0 = $s;
    }
    if ($num == 1 && $accumref->{$k}{$s0} >= $targetCount)
    {
      $profileref->{$k} = $s0;
    }
    elsif ($num == 2 && $k =~ /has/)
    {
      if (defined $accumref->{$k}{a} && defined $accumref->{$k}{p} &&
          $accumref->{$k}{a} + $accumref->{$k}{p} >= $targetCount)
      {
        my $knew = $k;
        $knew =~ s/has/opp/;
        $profileref->{$knew} = 0;
      }
      elsif (defined $accumref->{$k}{l} && defined $accumref->{$k}{r} &&
          $accumref->{$k}{r} + $accumref->{$k}{r} >= $targetCount)
      {
        my $knew = $k;
        $knew =~ s/has/opp/;
        $profileref->{$knew} = 1;
      }
    }
  }
}


sub getLengthKey
{
  my ($eref) = @_;
  return $eref->{lenAce} . $eref->{lenLHO} . 
    $eref->{lenPard} . $eref->{lenRHO};
}


sub getAllProfiles
{
  my ($commonref) = @_;
  for my $n (0 .. $#entries)
  {
    my $e = \%{$entries[$n]};
    my $lkey = getLengthKey($e);
    my $skipOpp = 'x';
    $skipOpp = 'l' if ($e->{lenLHO} == 0);
    $skipOpp = 'r' if ($e->{lenRHO} == 0);

    accumulateProfile($profiles[$n],
      \%{$branchProfiles[$e->{example}]{$lkey}},
      $commonref, $skipOpp);
  }

  for my $k (0 .. $#branchProfiles)
  {
    print "Branch $k common:\n";
    for my $l (sort keys %{$branchProfiles[$k]})
    {
      print "Pattern $l\n";
      my (%bprof, %reducedProfile, %knownTops);
      extractProfile(\%{$branchProfiles[$k]{$l}}, \%bprof, 1);
      extractReducedProfile(\%bprof, $commonref, \%reducedProfile);
      printProfile(\%reducedProfile);
      extractKnownTops(\%bprof, $commonref, \%knownTops);
      # printEntry(\%bprof);
      printExampleAsCode($k, \%knownTops, $l);
      print "\n";
    }
  }
}



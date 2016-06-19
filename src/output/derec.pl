#!perl
use strict;
use warnings;

open my $fh, '<', pop() or die "Can't < file: $!";
my $line;
my $depth = 0;
while ($line = <$fh>)
{
  if ($line =~ /^Start recursing/)
  {
    $depth++;
  }
  elsif ($line =~ /^Done recursing/)
  {
    $depth--;
  }
  elsif ($depth == 0)
  {
    print $line;
  }
}
close $fh;

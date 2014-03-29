#!/usr/bin/perl

use strict;

# n-way splits to report
my $start = int(shift);
my $end = int(shift);
if ($start < 1) { $start = 1; }
if ($end > 16) {$end = 16; }
if ($start > $end) { $end = $start; }

# split ratio statistics
my @bins;
my @binSet;
for (my $i = 0; $i < 16; $i++) {
    push (@binSet, "0");
}
for (my $i = 0; $i < 16; $i++) {
    push (@bins, [@binSet]);
}

# temporary position and subproblem variables
my @subs;
my $subCount;

while (<>) {
    # new position
    if ($_ =~ /POSITION/) {
        $subCount = 0;
        foreach my $i (@subs) { @subs[$i] = 0; };
    }
    # new subproblem
    if ($_ =~ /SUBPROBLEM \d+:/) {
        my $missing = 0;
        $missing++ while $_ =~ /-1/g;
        @subs[$subCount++] = (16 - $missing);
    }
    # done with subproblems
    if ($_ =~ /SUBPROBLEM COUNT/) {
        # sort the subproblems into bins
        for (my $i = 0; $i < $subCount; $i++) {
            my $binNumber = int(@subs[$i])-1;
            $bins[$subCount-1][$binNumber]++;
        }
    }
}

# report the n-way splits requested
for (my $s = $start-1; $s < $end; $s++) {
    my $n = $s + 1;
    print "$n-way Splits:\n";
    for (my $b = 0; $b < 16; $b++) {
        my $count = $bins[$s][$b];
        my $binLabel = $b + 1;
        print "$binLabel\t$count\n";
    }
}









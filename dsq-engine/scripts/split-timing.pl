#!/usr/bin/perl

use strict;

# game statistics
my $maxGameLength = 0;
my @splits;

my $moveNumber = 0;

while (<>) {
    # new game
    if ($_ =~ /INPUT FILE:/) {
        if ($moveNumber > $maxGameLength) {
            $maxGameLength = $moveNumber;
        }
        $moveNumber = 0;
    }
    # next move
    if ($_ =~ /SUBPROBLEM COUNT:/) {
        $moveNumber++;
        s/(SUBPROBLEM COUNT: |\n)//g;
        my $count = int($_);
        # split found
        if ($count > 1) {
            $splits[$moveNumber]++;
        }
    }
}

for (my $i = 1; $i <= $maxGameLength; $i++) {
    print "$i\t$splits[$i]\n";
}
        

#!/usr/bin/perl

use strict;

my $aMoveCount = 0;
my $aSplitCount = 0;
my $aSplitFrequency = 0;

my $bMoveCount = 0;
my $bSplitCount = 0;
my $bSplitFrequency;

my $player;
my $filename;

while (<>) {
    # new game
    if ($_ =~ /FILENAME/) {
        s/(FILENAME: |\r|\n)//g;
        $filename = $_;
    }
    # end of game
    if ($_ =~ /WINNING/) {
        if ($aSplitCount > 0) {
            $aSplitFrequency = $aSplitCount / $aMoveCount;
            print "$filename\tA\t$aSplitFrequency\n";
        }
        if ($bSplitCount > 0) {
            $bSplitFrequency = $bSplitCount / $bMoveCount;
            print "$filename\tB\t$bSplitFrequency\n";
        }
        $aMoveCount = 0;
        $aSplitCount = 0;
        $bMoveCount = 0;
        $bSplitCount = 0;
    }
    # switch players
    if ($_ =~ /PLAYER/) {
        s/(PLAYER: |\r|\n)//g;
        $player = $_;
    }
    # next move
    if ($_ =~ /MOVE/) {
        if ($player =~ /A/) {
            $aMoveCount = $aMoveCount + 1;
        }
        else {
            $bMoveCount = $bMoveCount + 1;
        }
    }
    # found a split
    if ($_ =~ /SUBPROBLEM COUNT/) {
        s/(SUBPROBLEM COUNT: |\r|\n)//g;
        my $count = $_;
        if (($count > 1) && ($player =~ /A/)) {
            $aSplitCount = $aSplitCount + 1;
        }
        elsif (($count > 1) && ($player =~ /B/)) {
            $bSplitCount = $bSplitCount + 1;
        }
    }
}

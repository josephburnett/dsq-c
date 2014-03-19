#!/usr/bin/perl

use strict;

my $player;
my $aCount = 0;
my $bCount = 0;
my $aSum = 0;
my $bSum = 0;
my $aAverage = 0;
my $bAverage = 0;

while(<>) {
    if ($_ =~ /FILENAME: /) {
        s/(FILENAME: |\r|\n)//g;
        print "$_\t";
    }
    if ($_ =~ /WINNING/) {
        $aAverage = $aSum / $aCount;
        $bAverage = $bSum / $bCount;
        $aSum = 0;
        $aCount = 0;
        $bSum = 0;
        $bCount = 0;
        print "A\t$aAverage\tB\t$bAverage\n";
    }
    if ($_ =~ /PLAYER: /) {
        s/PLAYER: //g;
        $player = $_;
        $player =~ s/(\r|\n)//g;
    }
    if ($_ =~ /SEARCH DEPTH: /) {
        s/SEARCH DEPTH: //g;
        if ($player =~ /A/)
        {
            $aCount = $aCount + 1;
            $aSum = $aSum + $_;
        }
        else
        {
            $bCount = $bCount + 1;
            $bSum = $bSum + $_;
        }
    }
}



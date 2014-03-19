#!/usr/bin/perl

use strict;

my $player;

while(<>) {
    if ($_ =~ /PLAYER: /) {
        s/PLAYER: //g;
        $player = $_;
        $player =~ s/(\r|\n)//g;
    }
    if ($_ =~ /SUBPROBLEM COUNT: /) {
        s/SUBPROBLEM COUNT: //g;
        print "$player\t$_";
    }
}

#!/usr/bin/perl

use strict;

while (<>) {
    if ($_ =~ /FILENAME: /) {
        s/(FILENAME: |\n)//g;
        print;
        print "\t";
    }
    if ($_ =~ /WINNING PLAYER: /) {
        s/WINNING PLAYER: //g;
        print;
    }
}

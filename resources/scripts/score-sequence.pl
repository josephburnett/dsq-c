#!/usr/bin/perl

use strict;

while (<>) {
    if ($_ =~ /SCORE: /) {
        s/SCORE: //g;
        print;
    }
}

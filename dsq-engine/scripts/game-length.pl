#!/usr/bin/perl

use strict;

my $count = 0;
my $newCount = 0;

while (<>) {
    if ($_ =~ /^MOVE: /) {
        s/(MOVE: |\n)//g;
        $newCount = $_;
        if ($newCount < $count)
        {
            print "$count\n";
            $count = 0;
        }
        else
        {
            $count = $newCount;
        }
    }
}

print "$count\n";


#!/usr/bin/perl

use strict;

while (<>) {
    if ($_ =~ /PLAYER:/) { print; }
    if ($_ =~ /BEST MOVE:/) { print; }
    if ($_ =~ /WINNING PLAYER:/) { print; }
    if ($_ =~ /POSITION:/) {
        for (my $i = 0; $i < 20; $i++) {
            my $next = <>;
            print $next;
        }
    }
}

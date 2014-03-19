#!/usr/bin/perl

use strict;
import sys;

my $directory = shift;
my @list;

opendir(DIR, $directory) || die ("Cannot open directory");
@list = readdir(DIR);

my $file;

foreach $file (@list) {
    if ($file =~ m/.pos$/) {
        my @args = ("./a.out","$directory/$file");
        print "$directory/$file\n";
        system(@args);
    }
}

closedir(DIR);

#!/usr/bin/perl

use strict;
import sys;

my $time = shift;
my $directory = shift;
my @list;

opendir(DIR, $directory) or die("Cannot open directory");
@list = readdir(DIR);

my $file;
foreach $file (@list) {
    if ($file =~ m/.pos/) {
        local @ARGV = ("$directory/$file");
        local $^I = '.bac';
        while (<>) {
            if ($. == 5) {
                print "$time\n";
                next;
            }
            else {
                print;
            }
        }
    }
}

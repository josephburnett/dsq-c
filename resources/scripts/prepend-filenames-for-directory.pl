#!/usr/bin/perl

use Fcntl;
import sys;

my $directory = shift;
my @list;

opendir(DIR, $directory) || die ("Cannot open directory");
@list = readdir(DIR);

my $file;

foreach $file (@list) {
    if ($file =~ m/.game$/) {
        local @ARGV = ("$directory/$file");
        local $^I = '.bac';
        while(<>) {
            if ($. == 1) {
                print "FILENAME: $file\n";
            }
            print;
        }
    }
}

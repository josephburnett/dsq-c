#!/usr/bin/perl

use strict;
import sys;

my $i;
my $j;
my @pieces;
my $filename = shift;

for ($i = 0; $i < 16; $i++)
{
    @pieces[$i] = -1;
}

open FILE, "<", $filename or die $!;

for ($i = 0; $i < 9; $i++) 
{
    my $row = <FILE>;
    my @columns = split (/\s/,$row);
    for ($j = 0; $j < 7; $j++)
    {
        my $entry = @columns[$j];
        if ($entry =~ /a/)
        {
            $entry =~ s/a//g;
            @pieces[$entry-1] = $i*7 + $j;
        }
        elsif ($entry =~ /b/)
        {
            $entry =~ s/b//g;
            @pieces[$entry+7] = $i*7 + $j;
        }
    }
}

for ($i = 0; $i < 8; $i++)
{
    print "@pieces[$i] ";
}
print "\n";
for ($i = 8; $i < 16; $i++)
{
    print "@pieces[$i] ";
}
print "\n";


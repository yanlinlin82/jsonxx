#!/usr/bin/env perl
use strict;
use warnings;

die "Usage: $0 <demo.exe> <demo.cpp>\n" unless $#ARGV >= 1;

my ($tests, $failed) = (0, 0);
open my $fh, "-|", "./$ARGV[0]" or die;
while (my $line = <$fh>) {
	if ($line =~ /^\[TEST:([0-9]+)\]\s*([A-Za-z_].*?)\s*=\s*(.*?)\s*\/\/\s*expected:\s*(.*?)\s*$/) {
		my ($line_no, $var, $out, $exp) = ($1, $2, $3, $4);
		if ($out ne $exp) {
			print "$ARGV[1]:$line_no: Test failed: $var = $out, but expecting $exp\n";
			++$failed;
		}
		++$tests;
	}
}
close $fh;

print "Total $tests test(s), and $failed failed.\n";
exit $failed;

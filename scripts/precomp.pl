#!/usr/bin/env perl
use strict;
use warnings;

die "Usage: $0 <demo.cpp>\n" unless @ARGV;

open my $fh, "<", $ARGV[0] or die;
my $line_no = 0;
while (my $line = <$fh>) {
	++$line_no;
	if ($line =~ /^(.*)\/\/\s*([A-Za-z0-9_]+)\s*=\s*(.*?)\s*$/) {
		my ($code, $var, $exp) = ($1, $2, $3);
		$exp =~ s/"/\\"/g;
		print $code, "std::cout << \"[TEST:$line_no] $var = \" << $var << \" // expected: $exp\" << std::endl;\n";
	} else {
		print $line;
	}
}
close $fh;

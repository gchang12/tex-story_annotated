#!/usr/bin/env perl
use warnings;
use strict;

# Strips all characters reserved for special purposes in TeX, and converts them to literals
# Takes two arguments: the source file and the name of the target file, to be typeset
# - Should I force the second argument to have the .tex extension?
# - No. It should be left for the end-user (read:me) to do.
# - Also, it would make for an inflexible program.
# \, $, <, >, {, }, ^, &, #, _

# Backslashes must be converted to $\backslash${}
# - What if the source file has TeX commands?

my %pattern_subs=(
# All should be escaped with a few exceptions
    '\$' => "\\\$",
# Less-than and greater-than signs must be converted to $<$ and $>$
# - Important that this is done AFTER the math-shift and group characters are processed
# Group-characters must be escaped
    '{' => '$\{$',
    '<' => '{}$<${}',
    '>' => '{}$>${}',
    '}' => '$\}$',
# Carets must be converted to \^{}
# - Must be done AFTER group-characters are processed!
    '\\^' => '\^{}',
    '&' => '\&',
    '#' => '\#',
    '_' => '\_',
# Fix quoting patterns in prose works
    '^"\'|(?<=\s)"\'' => '``\,`',
    '^"|(?<=[^-]\s)"' => '``'
);

# Error starts here if I attempt to use qw list to list the order of the patterns
#   "Possible attempt to put comments in qw() list at tex-converter.pl line 52.
#   Use of uninitialized value in open at tex-converter.pl line 66.
#   readline() on closed filehandle $rfile at tex-converter.pl line 6."

my @pattern_list=('\$','{','}','<','>','\^','&','#','_','^"\'|(?<=\s)"\'','^"|(?<=[^-]\s)"');

foreach (1..9) { shift @pattern_list; }

sub convert_line {
    my $tex_sym;
    our $line;
    foreach my $sym (@pattern_list){
        $tex_sym=$pattern_subs{$sym};
        # To be initialized globally in convert_file subroutine
        $line=~s/$sym/$tex_sym/g;
    }
}

sub convert_all_lines {
# Unsure of whether or not to insert this into its own subroutine
    open(my $rfile,shift);
    my @lines=<$rfile>;
    my @tex_lines=();
    while (our $line=shift @lines){
        convert_line;
        push @tex_lines,$line;
    }
    open(my $wfile,'>',shift);
    while (my $tex_line=shift @tex_lines){
        print $wfile "$tex_line";
    }
}
# For each line in the file, substitute every key with its value
# Write the result to a new file, whose name is given as argument two
convert_all_lines @ARGV;

#! /usr/bin/env perl

###########################################################
# Prepare a LaTeX run for two-way communication with Perl #
# By Scott Pakin <scott+pt@pakin.org>                     #
###########################################################

#-------------------------------------------------------------------
# This is file `perltex.pl',
# generated with the docstrip utility.
#
# The original source files were:
#
# perltex.dtx  (with options: `perltex')
#
# This is a generated file.
#
# Copyright (C) 2003-2019 Scott Pakin <scott+pt@pakin.org>
#
# This file may be distributed and/or modified under the conditions
# of the LaTeX Project Public License, either version 1.3c of this
# license or (at your option) any later version.  The latest
# version of this license is in:
#
#    http://www.latex-project.org/lppl.txt
#
# and version 1.3c or later is part of all distributions of LaTeX
# version 2006/05/20 or later.
#-------------------------------------------------------------------

sub top_level_eval ($)
{
    return eval $_[0];
}
use Safe;
use Opcode;
use Getopt::Long;
use Pod::Usage;
use File::Basename;
use Fcntl;
use POSIX;
use File::Spec;
use IO::Handle;
use warnings;
use strict;
my $latexprog;
my $runsafely = 1;
my @permittedops;
my $usepipe = 1;
my $progname = basename $0;
my $jobname = "texput";
my $toperl;
my $fromperl;
my $toflag;
my $fromflag;
my $doneflag;
my $logfile;
my $pipe;
my @latexcmdline;
my $styfile;
my @macroexpansions;
my $sandbox = new Safe;
my $sandbox_eval;
my $latexpid;
my $pipestring = "\%\%\%\%\% Generated by $progname\n\\endinput\n";
$latexprog = $ENV{"PERLTEX"} || "latex";
Getopt::Long::Configure("require_order", "pass_through");
GetOptions("help"       => sub {pod2usage(-verbose => 1)},
           "latex=s"    => \$latexprog,
           "safe!"      => \$runsafely,
           "pipe!"      => \$usepipe,
           "synctext=s" => \$pipestring,
           "makesty"    => sub {$styfile = "noperltex.sty"},
           "permit=s"   => \@permittedops) || pod2usage(2);
@latexcmdline = @ARGV;
my $firstcmd = 0;
for ($firstcmd=0; $firstcmd<=$#latexcmdline; $firstcmd++) {
    my $option = $latexcmdline[$firstcmd];
    next if substr($option, 0, 1) eq "-";
    if (substr ($option, 0, 1) ne "\\") {
        $jobname = basename $option, ".tex" ;
        $latexcmdline[$firstcmd] = "\\input $option";
    }
    last;
}
push @latexcmdline, "" if $#latexcmdline==-1;
my $separator = "";
foreach (1 .. 20) {
    $separator .= chr(ord("A") + rand(26));
}
$toperl = $jobname . ".topl";
$fromperl = $jobname . ".frpl";
$toflag = $jobname . ".tfpl";
$fromflag = $jobname . ".ffpl";
$doneflag = $jobname . ".dfpl";
$logfile = $jobname . ".lgpl";
$pipe = $jobname . ".pipe";
$latexcmdline[$firstcmd] =
    sprintf '\makeatletter' . '\def%s{%s}' x 7 . '\makeatother%s',
    '\plmac@tag', $separator,
    '\plmac@tofile', $toperl,
    '\plmac@fromfile', $fromperl,
    '\plmac@toflag', $toflag,
    '\plmac@fromflag', $fromflag,
    '\plmac@doneflag', $doneflag,
    '\plmac@pipe', $pipe,
    $latexcmdline[$firstcmd];
$toperl = File::Spec->rel2abs($toperl);
$fromperl = File::Spec->rel2abs($fromperl);
$toflag = File::Spec->rel2abs($toflag);
$fromflag = File::Spec->rel2abs($fromflag);
$doneflag = File::Spec->rel2abs($doneflag);
$logfile = File::Spec->rel2abs($logfile);
$pipe = File::Spec->rel2abs($pipe);
$SIG{"ALRM"} = sub {
    undef $latexpid;
    exit 0;
};
$SIG{"PIPE"} = "IGNORE";
sub delete_files (@)
{
    foreach my $filename (@_) {
        unlink $filename;
        while (-e $filename) {
            unlink $filename;
            sleep 0;
        }
    }
}
sub awaitexists ($)
{
    while (!-e $_[0]) {
        sleep 0;
        if (waitpid($latexpid, &WNOHANG)==-1) {
            delete_files($toperl, $fromperl, $toflag,
                         $fromflag, $doneflag, $pipe);
            undef $latexpid;
            exit 0;
        }
    }
}
delete_files($toperl, $fromperl, $toflag, $fromflag, $doneflag, $pipe);
open (LOGFILE, ">$logfile") || die "open(\"$logfile\"): $!\n";
autoflush LOGFILE 1;
if (defined $styfile) {
    open (STYFILE, ">$styfile") || die "open(\"$styfile\"): $!\n";
}
if (!$usepipe || !eval {mkfifo($pipe, 0600)}) {
    sysopen PIPE, $pipe, O_WRONLY|O_CREAT, 0755;
    autoflush PIPE 1;
    print PIPE $pipestring;
    close PIPE;
    $usepipe = 0;
}
defined ($latexpid = fork) || die "fork: $!\n";
unshift @latexcmdline, $latexprog;
if (!$latexpid) {
    exec {$latexcmdline[0]} @latexcmdline;
    die "exec('@latexcmdline'): $!\n";
}
if ($runsafely) {
    @permittedops=(":browse") if $#permittedops==-1;
    $sandbox->permit_only (@permittedops);
    $sandbox_eval = sub {$sandbox->reval($_[0])};
}
else {
    $sandbox_eval = \&top_level_eval;
}
while (1) {
    awaitexists($toflag);
    my $entirefile;
    {
        local $/ = undef;
        open (TOPERL, "<$toperl") || die "open($toperl): $!\n";
        $entirefile = <TOPERL>;
        close TOPERL;
    }
    $entirefile =~ s/\r//g;
    my ($optag, $macroname, @otherstuff) =
        map {chomp; $_} split "$separator\n", $entirefile;
    $macroname =~ s/^[^A-Za-z]+//;
    $macroname =~ s/\W/_/g;
    $macroname = "latex_" . $macroname;
    if ($optag eq "USE") {
      foreach (@otherstuff) {
          s/\\/\\\\/g;
          s/\'/\\\'/g;
          $_ = "'$_'";
      }
    }
    my $perlcode;
    if ($optag eq "DEF") {
        $perlcode =
            sprintf "sub %s {%s}\n",
            $macroname, $otherstuff[0];
    }
    elsif ($optag eq "USE") {
        $perlcode = sprintf "%s (%s);\n", $macroname, join(", ", @otherstuff);
    }
    elsif ($optag eq "RUN") {
        $perlcode = $otherstuff[0];
    }
    else {
        die "${progname}: Internal error -- unexpected operation tag \"$optag\"\n";
    }
    print LOGFILE "#" x 31, " PERL CODE ", "#" x 32, "\n";
    print LOGFILE $perlcode, "\n";
    undef $_;
    my $result;
    {
        my $warningmsg;
        local $SIG{__WARN__} =
            sub {chomp ($warningmsg=$_[0]); return 0};
        $result = $sandbox_eval->($perlcode);
        if (defined $warningmsg) {
            $warningmsg =~ s/at \(eval \d+\) line \d+\W+//;
            print LOGFILE "# ===> $warningmsg\n\n";
        }
    }
    $result = "" if !$result || $optag eq "RUN";
    if ($@) {
        my $msg = $@;
        $msg =~ s/at \(eval \d+\) line \d+\W+//;
        $msg =~ s/\n/\\MessageBreak\n/g;
        $msg =~ s/\s+/ /;
        $result = "\\PackageError{perltex}{$msg}";
        my @helpstring;
        if ($msg =~ /\btrapped by\b/) {
            @helpstring =
                ("The preceding error message comes from Perl.  Apparently,",
                 "the Perl code you tried to execute attempted to perform an",
                 "`unsafe' operation.  If you trust the Perl code (e.g., if",
                 "you wrote it) then you can invoke perltex with the --nosafe",
                 "option to allow arbitrary Perl code to execute.",
                 "Alternatively, you can selectively enable Perl features",
                 "using perltex's --permit option.  Don't do this if you don't",
                 "trust the Perl code, however; malicious Perl code can do a",
                 "world of harm to your computer system.");
        }
        else {
            @helpstring =
              ("The preceding error message comes from Perl.  Apparently,",
               "there's a bug in your Perl code.  You'll need to sort that",
               "out in your document and re-run perltex.");
        }
        my $helpstring = join ("\\MessageBreak\n", @helpstring);
        $helpstring =~ s/\.  /.\\space\\space /g;
        $result .= "{$helpstring}";
    }
    push @macroexpansions, $result if defined $styfile && $optag eq "USE";
    print LOGFILE "%" x 30, " LATEX RESULT ", "%" x 30, "\n";
    print LOGFILE $result, "\n\n";
    $result .= '\endinput';
    open (FROMPERL, ">$fromperl") || die "open($fromperl): $!\n";
    syswrite FROMPERL, $result;
    close FROMPERL;
    delete_files($toflag, $toperl, $doneflag);
    open (FROMFLAG, ">$fromflag") || die "open($fromflag): $!\n";
    close FROMFLAG;
    if (open (PIPE, ">$pipe")) {
        autoflush PIPE 1;
        print PIPE $pipestring;
        close PIPE;
    }
    awaitexists($toperl);
    delete_files($fromflag);
    open (DONEFLAG, ">$doneflag") || die "open($doneflag): $!\n";
    close DONEFLAG;
    alarm 1;
    if (open (PIPE, ">$pipe")) {
        autoflush PIPE 1;
        print PIPE $pipestring;
        close PIPE;
    }
    alarm 0;
}
END {
    close LOGFILE;
    if (defined $latexpid) {
        kill (9, $latexpid);
        exit 1;
    }

    if (defined $styfile) {
        print STYFILE <<"STYFILEHEADER1";
\\NeedsTeXFormat{LaTeX2e}[1999/12/01]
\\ProvidesPackage{noperltex}
    [2007/09/29 v1.4 Perl-free version of PerlTeX specific to $jobname.tex]
STYFILEHEADER1
        ;
        print STYFILE <<'STYFILEHEADER2';
\RequirePackage{filecontents}

\let\noperltex@PackageError=\PackageError
\renewcommand{\PackageError}[3]{}
\RequirePackage{perltex}
\let\PackageError=\noperltex@PackageError

\newcount\plmac@macro@invocation@num
\gdef\plmac@show@placeholder#1#2\@empty{%
  \ifx#1U\relax
    \endgroup
    \advance\plmac@macro@invocation@num by 1\relax
    \global\plmac@macro@invocation@num=\plmac@macro@invocation@num
    \input{noperltex-\the\plmac@macro@invocation@num.tex}%
  \else
    \endgroup
  \fi
}
STYFILEHEADER2
        ;
        foreach my $e (0 .. $#macroexpansions) {
            print STYFILE "\n";
            printf STYFILE "%% Invocation #%d\n", 1+$e;
                printf STYFILE "\\begin{filecontents}{noperltex-%d.tex}\n", 1+$e;
            print STYFILE $macroexpansions[$e], "\\endinput\n";
            print STYFILE "\\end{filecontents}\n";
        }
        print STYFILE "\\endinput\n";
        close STYFILE;
    }

    exit 0;
}

__END__

=head1 NAME

perltex - enable LaTeX macros to be defined in terms of Perl code

=head1 SYNOPSIS

perltex
[B<--help>]
[B<--latex>=I<program>]
[B<-->[B<no>]B<safe>]
[B<--permit>=I<feature>]
[B<--makesty>]
[I<latex options>]

=head1 DESCRIPTION

LaTeX -- through the underlying TeX typesetting system -- produces
beautifully typeset documents but has a macro language that is
difficult to program.  In particular, support for complex string
manipulation is largely lacking.  Perl is a popular general-purpose
programming language whose forte is string manipulation.  However, it
has no typesetting capabilities whatsoever.

Clearly, Perl's programmability could complement LaTeX's typesetting
strengths.  B<perltex> is the tool that enables a symbiosis between
the two systems.  All a user needs to do is compile a LaTeX document
using B<perltex> instead of B<latex>.  (B<perltex> is actually a
wrapper for B<latex>, so no B<latex> functionality is lost.)  If the
document includes a C<\usepackage{perltex}> in its preamble, then
C<\perlnewcommand> and C<\perlrenewcommand> macros will be made
available.  These behave just like LaTeX's C<\newcommand> and
C<\renewcommand> except that the macro body contains Perl code instead
of LaTeX code.

=head1 OPTIONS

B<perltex> accepts the following command-line options:

=over 4

=item B<--help>

Display basic usage information.

=item B<--latex>=I<program>

Specify a program to use instead of B<latex>.  For example,
C<--latex=pdflatex> would typeset the given document using
B<pdflatex> instead of ordinary B<latex>.

=item B<-->[B<no>]B<safe>

Enable or disable sandboxing.  With the default of B<--safe>,
B<perltex> executes the code from a C<\perlnewcommand> or
C<\perlrenewcommand> macro within a protected environment that
prohibits ``unsafe'' operations such as accessing files or executing
external programs.  Specifying B<--nosafe> gives the LaTeX document
I<carte blanche> to execute any arbitrary Perl code, including that
which can harm the user's files.  See L<Safe> for more information.

=item B<--permit>=I<feature>

Permit particular Perl operations to be performed.  The B<--permit>
option, which can be specified more than once on the command line,
enables finer-grained control over the B<perltex> sandbox.  See
L<Opcode> for more information.

=item B<--makesty>

Generate a LaTeX style file called F<noperltex.sty>.  Replacing the
document's C<\usepackage{perltex}> line with C<\usepackage{noperltex}>
produces the same output but does not require PerlTeX, making the
document suitable for distribution to people who do not have PerlTeX
installed.  The disadvantage is that F<noperltex.sty> is specific to
the document that produced it.  Any changes to the document's PerlTeX
macro definitions or macro invocations necessitates rerunning
B<perltex> with the B<--makesty> option.

=back

These options are then followed by whatever options are normally
passed to B<latex> (or whatever program was specified with
C<--latex>), including, for instance, the name of the F<.tex> file to
compile.

=head1 EXAMPLES

In its simplest form, B<perltex> is run just like B<latex>:

    perltex myfile.tex

To use B<pdflatex> instead of regular B<latex>, use the B<--latex>
option:

    perltex --latex=pdflatex myfile.tex

If LaTeX gives a ``C<trapped by operation mask>'' error and you trust
the F<.tex> file you're trying to compile not to execute malicious
Perl code (e.g., because you wrote it yourself), you can disable
B<perltex>'s safety mechansisms with B<--nosafe>:

    perltex --nosafe myfile.tex

The following command gives documents only B<perltex>'s default
permissions (C<:browse>) plus the ability to open files and invoke the
C<time> command:

    perltex --permit=:browse --permit=:filesys_open
      --permit=time myfile.tex

=head1 ENVIRONMENT

B<perltex> honors the following environment variables:

=over 4

=item PERLTEX

Specify the filename of the LaTeX compiler.  The LaTeX compiler
defaults to ``C<latex>''.  The C<PERLTEX> environment variable
overrides this default, and the B<--latex> command-line option (see
L</OPTIONS>) overrides that.

=back

=head1 FILES

While compiling F<jobname.tex>, B<perltex> makes use of the following
files:

=over 4

=item F<jobname.lgpl>

log file written by Perl; helpful for debugging Perl macros

=item F<jobname.topl>

information sent from LaTeX to Perl

=item F<jobname.frpl>

information sent from Perl to LaTeX

=item F<jobname.tfpl>

``flag'' file whose existence indicates that F<jobname.topl> contains
valid data

=item F<jobname.ffpl>

``flag'' file whose existence indicates that F<jobname.frpl> contains
valid data

=item F<jobname.dfpl>

``flag'' file whose existence indicates that F<jobname.ffpl> has been
deleted

=item F<noperltex-#.tex>

file generated by F<noperltex.sty> for each PerlTeX macro invocation

=back

=head1 NOTES

B<perltex>'s sandbox defaults to what L<Opcode> calls ``C<:browse>''.

=head1 SEE ALSO

latex(1), pdflatex(1), perl(1), Safe(3pm), Opcode(3pm)

=head1 AUTHOR

Scott Pakin, I<scott+pt@pakin.org>

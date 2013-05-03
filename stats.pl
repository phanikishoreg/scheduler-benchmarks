#!/usr/bin/perl
use List::Util qw(sum);

my @tot = ();
my $in = 0;

while (<STDIN>) {
  chomp;
  # output > 10000000 appears to be instances where the initial
  # tick counter wasn't set before the interrupt occured
  # no output is actually close to 10000000, it is either much less or much more
  push (@tot, $_); #unless $_ > 10000000;
  $in++;
}

my $avg = sum(@tot) / ($#tot+1);
my $stdev = 0;
foreach my $n (@tot) { $stdev += ($n - $avg) * ($n - $avg); }
$stdev = sqrt ($stdev / ($#tot+1));
#@tot = sort (@tot);
print "Samples: " . ($#tot + 1) . "\n";
#print "Ignored: " . ($in - ($#tot+1)) . "\n";
print "Minimum: $tot[0]\n";
print "Average: $avg\n";
print "StdDev:  $stdev\n";
print "Maximum: $tot[-1]\n";
foreach my $p (50, 90, 95) { print "$p \%ile: $tot[$#tot * $p / 100]\n"; } 

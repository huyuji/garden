use strict;
use warnings;

if ($#ARGV != 1)
{
  print "Invalid arguments.\n";
  print "$0 <input file> <output file>\n";
  exit;
}

my $inputFileName = $ARGV[0];
open(my $input, '<', $inputFileName) or die "Could not open '$inputFileName'. $!";

my $outputFileName = $ARGV[1];
if(-e $outputFileName)
{
  while(1)
  {
    print "$outputFileName already exists. Do you want to overwrite it? (Y/N):";
    chomp(my $answer = <STDIN>);
    if($answer eq "N")
    {
      exit;
    }
    elsif($answer eq "Y")
    {
      last;
    }
  }
}
open(my $output, '>', $outputFileName) or die "Could not open '$outputFileName'. $!";
  
print $output "Class,Loan #,Principal Disbursed,Capitalized,Interest Charged,Fees Charged,Insurance Charged,Writeoff,Account Name,Principal Repaid,Interest Paid,Fees Paid,Insurance Paid,Recovery\n";

my $class;
my $isFirstLine = 1;
while (<$input>) 
{
  if(/^$/
    or /IAF Canada/
    or /815 17 Ave SW  210/
    or /Calgary, AB T2T 0A1/
    or /Work: \(403\)228\-9981 x 221/
    or /Toll\-Free: \(855\)423\-2262/
    or /Between/
    or /^,,,,,,,,,,,,,,,,,,,,,/
    or /Principal.+?Interest.+?Fees.+?Insurance.+?/
    or /Loan \#.+?Disbursed.+?Charged.+?Charged.+?Charged.+?Writeoff.+?/
    or /Account Name.+?Repaid.+?Capitalized.+?Paid.+?Paid.+?Paid.+?Recovery.+?/
    or /Page \#: /
    or /Account Name.+?Repaid.+?Paid.+?Paid.+?Paid.+?Recovery.+?/)
  {
  }
  elsif (/"?Class1:"?,"?(\w{2})"?/)
  {
    $class = $1;
    #print "processing class $class\n";
  }
  else
  {
    if (/^Grand Totals,,,,,,/)
    {
      $class = "Grand";
    }
    
    s/,{2,}/,/g;
    
    if($isFirstLine)
    {
      print $output "$class,";
      if(not /Total/)
      {
        print $output "Loan # ";
      }
      chomp;
    }
    print $output $_;
    $isFirstLine = !$isFirstLine;
  }
}

close $input;
close $output;
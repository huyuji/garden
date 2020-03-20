use strict;
use warnings;
use Text::CSV;

if ($#ARGV != 0)
{
  print "Invalid arguments.\n";
  print "$0 <csv file to verify>\n";
  exit;
}

my %total = ();

sub saveNumbers
{
  my ($class, @numbers) = @_;
  
  if($total{$class})
  {
    for my $i ( 0 .. $#numbers ) 
    {
      $total{$class}[$i] += $numbers[$i];
    }
  }
  else
  {
    $total{$class} = [@numbers];
  }
  
  $class = 'Grand';
  if($total{$class})
  {
    for my $i ( 0 .. $#numbers ) 
    {
      $total{$class}[$i] += $numbers[$i];
    }
  }
  else
  {
    $total{$class} = [@numbers];
  }  
}

my $fileName = $ARGV[0];
open(my $input, '<', $fileName) or die "Could not open '$fileName' $!\n";
my $csv = Text::CSV->new({ sep_char => ',' });
my $isFirstLine = 1;
while (<$input>) {
  # skip title
  if($isFirstLine)
  {
    $isFirstLine = !$isFirstLine;
    next;
  }
  
  chomp;
 
  if ($csv->parse($_)) {
      my @fields = $csv->fields();
      #print "@fields\n";
      my $class = $fields[0];
      my @numbers = (@fields[2...7], @fields[9...13]);
      for my $i ( 0 .. $#numbers )
      {
        $numbers[$i] =~ s/^'(.*)'$/$1/;
        $numbers[$i] =~ s/,//;
      }
      #print "@numbers\n";
      
      if($fields[1] eq "$class Total:"
         or $fields[1] eq "$class Totals")
      {
        print "verify $class Total.\n";
        for my $i ( 0 .. $#numbers ) 
        {
          #if ($total{$class}[$i] - $numbers[$i])
          if (abs($total{$class}[$i] - $numbers[$i]) > 0.000001)
          {
            print "Total doesn't match for $class. $total{$class}[$i] != $numbers[$i].\n";
            last;
          }
        }
      }
      else
      {
        saveNumbers($class, @numbers);
      }
  } else {
      print "Faile to parse line:\n";
      print;
      print "\n";
      exit;
  }
}
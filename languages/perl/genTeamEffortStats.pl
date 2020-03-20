#description:   Generate the "Individual Effort Stats" sheet.
#prerequisite:  The "Effort Details" sheet must exist with the right data.
#usage:         genIndividualEffortStats.pl <full path of xlsx file>
#example:       genIndividualEffortStats.pl "d:\China COE UPT Weekly Report 2012-02-22 draft"

use DBI;
use Win32::OLE;
use Win32::OLE::Variant;
use Win32::OLE::Const 'Microsoft Excel';
use Cwd;
use Time::Piece;
use Time::Seconds;
use XML::Simple;
use Class::Struct;

my $bookname = $ARGV[0];

my $srcSheetName = "Team Effort Details";
my $dstSheetName = 'Team Effort';

struct
(
	EFFORT_ENTRY =>
	[
    ProductName => '$',
    ReleaseName => '$',
    FeatureName => '$',
    PhaseName   => '$',
    Contributor => '$',
    Effort      => '$'
	]
);

my $xlApp = Win32::OLE->new('Excel.Application', 'Quit');

my $ret = eval {main()};
if($@)
{
  print $@;
  
  if($xlApp)
  {
    $xlApp->Quit;
    undef $xlApp;  
  }
}

sub main
{
  my @effortEntries = retrieveEffort($bookname, $srcSheetName);
  # foreach my $effortEntry (@effortEntries)
  # {
    # print $effortEntry->ProductName."-".$effortEntry->ReleaseName."-".$effortEntry->FeatureName."-".$effortEntry->PhaseName."-".$effortEntry->Contributor."-".$effortEntry->Effort."\n";
  # }
  
  my %effortMap = formatEffort(\@effortEntries);
  saveEffort(\%effortMap, $bookname, $dstSheetName);
}

sub saveEffort
{
  my ($effortMap, $bookname, $dstSheetName) = @_;
  
  $xlApp->{Visible} = 0;
  $xlApp->{DisplayAlerts} = 0;
  my $xlBook = $xlApp->Workbooks->Open($bookname) || die qq(File "$bookname" open failed.\n);
    
  my $xlSheet = $xlBook->Sheets->{$dstSheetName};
  if($xlSheet)
  {
    print qq(delete existing "$dstSheetName" sheet\n);
    $xlBook->Sheets->{$dstSheetName}->Delete;
  }
  $xlSheet = $xlBook->Sheets->Add;
  $xlSheet->{Name} = $dstSheetName;
  
  my $row = 1;
  while( my($name, $subItem) = each(%$effortMap))
  {
    $xlSheet->Cells($row, 1)->{Value} = $name;
    $xlSheet->Cells($row, 2)->{Value} = "Effort";
    
    while ( my($subname, $effort) = each(%$subItem))
    {
      $row++;
      $xlSheet->Cells($row, 1)->{Value} = $subname;
      $xlSheet->Cells($row, 2)->{Value} = $effort;
    }
    
    $row += 2;
  }
  
  $xlBook->Save;
  $xlBook->{Saved} = 1;
}

sub formatEffort
{
  my $effortEntries = shift;
  
  my %effortItems = ();
  foreach my $effortEntry (@$effortEntries)
  {
    my $effortName = $effortEntry->ProductName."-".$effortEntry->ReleaseName."-".$effortEntry->FeatureName;
    $effort = $effortItems{$effortEntry->Contributor}{$effortName};
    if(!$effort)
    {
      $effort = 0;
    }
    $effortItems{$effortEntry->Contributor}{$effortName} = $effort + $effortEntry->Effort;
  }
  
  return %effortItems;
}

sub retrieveEffort
{
  my ($bookname, $srcSheetName) = @_;
  
  $xlApp->{Visible} = 0;
  $xlApp->{DisplayAlerts} = 0;
  my $xlBook = $xlApp->Workbooks->Open($bookname) || die qq(File "$bookname" open failed.\n);
  my $xlSrcSheet = $xlBook->Sheets->{$srcSheetName};
  if(!$xlSrcSheet)
  {
    die qq(Can't open "$srcSheetName" sheet. Make sure it exists and have the right data.\n);
  }
  
  my $row = 2;
  my @effortEntries = ();
  while(1)
  {
    my $col = 1;
    if(!$xlSrcSheet->Cells($row, $col)->{Value})
    {
      last;
    }
    
    my $effortEntry = new EFFORT_ENTRY;
    @$effortEntry = (
      $xlSrcSheet->Cells($row, $col++)->{Value},
      $xlSrcSheet->Cells($row, $col++)->{Value},
      $xlSrcSheet->Cells($row, $col++)->{Value},
      $xlSrcSheet->Cells($row, $col++)->{Value},
      $xlSrcSheet->Cells($row, $col++)->{Value},
      $xlSrcSheet->Cells($row, $col++)->{Value}
    );
    
    push @effortEntries, $effortEntry;
    $row++;
  }  
  
  $xlBook->Save;
  $xlBook->{Saved} = 1;

  return @effortEntries;
}
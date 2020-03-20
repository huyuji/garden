#descrption:    Generate the "Feature Effort Stats" sheet
#prerequisite:  The "Effort Details" sheet must exists with correct data.

use DBI;
use Win32::OLE;
use Win32::OLE::Variant;
use Win32::OLE::Const 'Microsoft Excel';
use Cwd;
use Time::Piece;
use Time::Seconds;
use OnlineReport;
use Class::Struct;

my $usage = "$0 <full or relative path of xlsx file>";

my $bookname = $ARGV[0];

my $srcSheetName = "Effort Details";
my $dstSheetName = 'Feature Effort';

sub checkParam
{
	if($ARGV[0] eq ""
		)
	{
		printUsage();
		die "parameter missing\n";
	}
}

sub printUsage
{
	print "usage: $usage";
}

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

checkParam();
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
  my $xlBook = $xlApp->Workbooks->Open(File::Spec->rel2abs($bookname)) || die qq(File "$bookname" open failed.\n);
  
  my $xlSheet = createExcelSheet($xlBook, $dstSheetName, 1);
  
  my $row = 1;
  while( my($name, $subItem) = each(%$effortMap))
  {
		if($name eq "Effort by Product")
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
  }
  
  while( my($name, $subItem) = each(%$effortMap))
  {
		if(not $name eq "Effort by Product")
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
    my $effort = $effortItems{"Effort by Product"}{$effortEntry->ProductName};
    if(!$effort)
    {
      $effort = 0;
    }
    $effortItems{"Effort by Product"}{$effortEntry->ProductName} = $effort + $effortEntry->Effort;
    
    my $effortName = $effortEntry->ReleaseName."-".$effortEntry->FeatureName;
    $effort = $effortItems{"Effort for ".$effortEntry->ProductName}{$effortName};
    if(!$effort)
    {
      $effort = 0;
    }
    $effortItems{"Effort for ".$effortEntry->ProductName}{$effortName} = $effort + $effortEntry->Effort;
  }
  
  return %effortItems;
}

sub retrieveEffort
{
  my ($bookname, $srcSheetName) = @_;
  
  $xlApp->{Visible} = 0;
  $xlApp->{DisplayAlerts} = 0;
  my $xlBook = $xlApp->Workbooks->Open(File::Spec->rel2abs($bookname)) || die qq(File "$bookname" open failed.\n);
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
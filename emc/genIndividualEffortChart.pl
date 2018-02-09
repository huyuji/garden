#description:   Greate charts on "Individual Effort Stats" sheet.
#prerequisite:  The "Individual Effort Stats" sheet must exist and have data in the right format.

use Data::Dumper;
use Win32::OLE;
use Win32::OLE::Variant;
use Win32::OLE::Const 'Microsoft Excel';
use Cwd;
use Switch;
use File::Basename;
use File::Spec;
use Time::Piece;
use Time::Seconds;
use OnlineReport;
$| = 1;

my $usage = "$0 <full or relative path of xlsx file>";

my $xlFilename = $ARGV[0];

my $sheetname = "Individual Effort";

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

#prepare spreadsheet
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
  $xlApp->{Visible} = 0;
  $xlApp->{DisplayAlerts} = 0;
  my $xlBook = $xlApp->Workbooks->Open(File::Spec->rel2abs($xlFilename)) || die qq(File "$xlFilename" open failed.\n);

  #open sheet
  my $xlSrcSheet = $xlBook->Sheets($sheetname);
  if(!$xlSrcSheet)
  {
    die qq(Can't open $sheetname sheet. Make sure it exists and have the right data.\n);
  }
  $xlSrcSheet->Activate;

  #count total number of charts to generate
  my $row = 1;
  if(!$xlSrcSheet->Cells($row, 1)->{Value})
  {
    die qq(Data must start from the first row\n);
  }

  my $dataBlockCount = 0;
  while(1)
  {
    if(!$xlSrcSheet->Cells($row, 1)->{Value}
      || $xlSrcSheet->Cells($row, 1)->{Value} eq "")
    {
      $dataBlockCount++;
      if(!$xlSrcSheet->Cells($row+1, 1)->{Value}
      || $xlSrcSheet->Cells($row+1, 1)->{Value} eq "")
      {
        last;
      }
    }
    
    $row++;
  }

  #set chart width and height
  my $width = 10 * $xlSrcSheet->Columns("B")->{Width};
  my $height = 18 * $xlSrcSheet->Rows("2")->{Height};

  #insert blank column at the begin for charts
  my $chartRowCount = int($dataBlockCount/2) * 18 +1;
  if($dataBlockCount%2)
  {
    $chartRowCount += 18;
  }my $wkRow = $chartRowCount + 1;
  while($chartRowCount--)
  {
    $xlSrcSheet->Cells(1, 1)->EntireRow->Insert(xlShiftDown);
  }

  #create charts
  my $cntChart = 0;
  while (1)
  {
    $xlChartObj = createExcelChart($xlSrcSheet, $wkRow, 
                    $xlSrcSheet, 
										int($cntChart%2)*$width, int($cntChart/2)*$height, 
										$width, $height, xlPie);
										
		while (defined($xlSrcSheet->Cells($wkRow, 1)->{Value})) {$wkRow++}
		$wkRow++;

    if (defined($xlChartObj))
    {
        $cntChart++;
    }
    else
    {
      last;
    }
  };

	$xlSrcSheet->Columns("A")->EntireColumn->AutoFit;

  $xlBook->Save;
  $xlBook->{Saved} = 1;
  $xlApp->Quit;
  undef $xlBook;
  undef $xlApp;
}
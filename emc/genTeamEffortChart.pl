#description:   Greate charts on "Individual Effort Stats" sheet.
#prerequisite:  The "Individual Effort Stats" sheet must exist and have data in the right format.
#usage:         genIndividualEffortChart.pl <full path of xlsx file>
#example:       genIndividualEffortChart.pl "d:\China COE UPT Weekly Report 2012-02-22 draft"

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
$| = 1;

my $xlFilename = $ARGV[0];

my $sheetname = "Team Effort";

#prepare spreadsheet
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
  $xlApp->{Visible} = 0;
  $xlApp->{DisplayAlerts} = 0;
  my $xlBook = $xlApp->Workbooks->Open($xlFilename) || die qq(File "$xlFilename" open failed.\n);

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
  my $width = 20 * $xlSrcSheet->Columns("B")->{Width};
  my $height = 36 * $xlSrcSheet->Rows("2")->{Height};

  #insert blank column at the begin for charts
  my $chartRowCount = $dataBlockCount * 36 +1;

  my $wkRow = $chartRowCount + 1;
  while($chartRowCount--)
  {
    $xlSrcSheet->Cells(1, 1)->EntireRow->Insert(xlShiftDown);
  }

  #create charts
  my $cntChart = 0;
  while (1)
  {
    ($xlChartObj, $wkRow) = mkChart($xlSrcSheet, $wkRow, 
                    $xlSrcSheet, 0, 
                    $cntChart*$height, $width, 
                    $height, xlPie);
    if (defined($xlChartObj))
    {
        $cntChart++;
    }
    else
    {
      last;
    }
  };

  $xlBook->Save;
  $xlBook->{Saved} = 1;
  $xlApp->Quit;
  undef $xlBook;
  undef $xlApp;
}

sub mkChart
{
	my ($xlSrcSheet, $rowStart, $xlChartSheet, $toLeft, $toTop, $xScale, $yScale, $type) = @_;
	
	my $title = $xlSrcSheet->Cells($rowStart, 1)->{Value};
	return (undef, $rowStart) if (!defined($title));

	my $i = $rowStart;
	while (defined($xlSrcSheet->Cells($i, 1)->{Value})) {$i++}
	my $rowEnd = $i - 1;

	my $i = 1;
	while (defined($xlSrcSheet->Cells($rowStart, $i)->{Value})) {$i++}
	my $colEnd = $i - 1;
	
	my $chartObject = $xlChartSheet->ChartObjects->Add($toLeft, $toTop, $xScale, $yScale);
	$chartObject->{Placement} = xlFreeFloating;
	my $chart = $chartObject->{Chart};
	$chart->{ChartType} = $type;
	$chart->SetSourceData($xlSrcSheet->Range($xlSrcSheet->Cells($rowStart, 1), $xlSrcSheet->Cells($rowEnd, $colEnd)));
	$chart->{HasTitle} = True;
	$chart->{ChartTitle}->{Text} = $title;
	$chart->Axes(xlCategory, xlPrimary)->{CategoryType} = xlCategoryScale;
	for ($i=1; $i<=$chart->SeriesCollection->Count; $i++)
	{
		$chart->SeriesCollection($i)->ApplyDataLabels;
    if("Backlog" eq $chart->SeriesCollection($i)->{Name})
    {
      $chart->SeriesCollection($i)->{ChartType} = xlLineMarkers
    }
	}

	if($type == xlPie)
	{
	  $chart->ApplyDataLabels(xlDataLabelsShowLabel, false, false, false, false, true, true, false, false);
	}
		
	return ($chartObject, $rowEnd+2);
}
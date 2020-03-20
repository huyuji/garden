#description:   Generate "Feature Status" sheet
#prerequisite:  The "Statistics" sheet must exist and have the right data format.

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

my $srcSheetName = "Statistics";
my $chartSheetName = "AR Statistics";

my $xlFilename = $ARGV[0];

checkParam();
sub checkParam
{
	if($ARGV[0] eq "")
	{
		printUsage("Parameter missing\n");
		exit;
	}
}

sub printUsage
{
	my $errMsg = shift;
	print $errMsg;
	print "usage: $usage";
}

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
	$xlFilename = File::Spec->rel2abs($xlFilename);
	
  $xlApp->{Visible} = 0;
  $xlApp->{DisplayAlerts} = 0;
  my $xlBook = $xlApp->Workbooks->Open($xlFilename) || die qq(File "$xlFilename" open failed.\n);
  my $xlSrcSheet = $xlBook->Sheets($srcSheetName);
  if(!$xlSrcSheet)
  {
    die qq(Can't open "$srcSheetName" sheet. Make sure it exists and have the right data.\n);
  }

	my $xlChartSheet = createExcelSheet($xlBook, $chartSheetName, 1);
    
  my $wkRow = 1;
  my $width = 10 * $xlChartSheet->Columns("B")->{Width};
  my $length = 18 * $xlChartSheet->Rows("2")->{Height};
  my $cntChart = 0;

  while (1)
  {
    $xlChartObj = createExcelChart($xlSrcSheet, $wkRow, 
                    $xlChartSheet, 
										int($cntChart%2)*$width, int($cntChart/2)*$length, 
										$width, $length, xlColumnStacked);
										
		while (defined($xlSrcSheet->Cells($wkRow, 1)->{Value})) {$wkRow++}
		$wkRow++;
		
    if (defined($xlChartObj))
    {
      $cntChart++;
      my $chart = $xlChartObj->{Chart};
			
      # Special handling based on Chart Content
      switch ($chart->{ChartTitle}->{Text})
      {
        case 'Outgoing Defects vs. Bug Fix Effort'
        {
          $chart->{ChartType} = xlLineMarkers;
          $chart->SeriesCollection(1)->{AxisGroup} = xlSecondary;
          $chart->Axes(xlValue, xlPrimary)->{HasTitle} = True;
          $chart->Axes(xlValue, xlPrimary)->{AxisTitle}->{Text} = "Person x Hour";
        }
        case 'Bug Fix Rate'
        {
          $chart->{ChartType} = xlLineMarkers;
          $chart->Axes(xlValue, xlPrimary)->{HasTitle} = True;
          $chart->Axes(xlValue, xlPrimary)->{AxisTitle}->{Text} = "Bug Fixed / ( Person x Week )";
        }
      }
			
			for ($i=1; $i<=$chart->SeriesCollection->Count; $i++)
			{
				$chart->SeriesCollection($i)->ApplyDataLabels;
				if("Backlog" eq $chart->SeriesCollection($i)->{Name})
				{
					$chart->SeriesCollection($i)->{ChartType} = xlLineMarkers
				}
			}
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
#description:   Generate "Feature Status" sheet
#prerequisite:  The "Statistics" sheet, "Effort" sheet and "Customer Effort" sheet must exist and have the right data format.

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

my $dstSheetName = "Feature Status";

my @excludeByTitle = (
	'^.* by Type$',
	'^.* Non-PlannedContent by Priority$'
);

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
  my $xlSrcSheet = $xlBook->Sheets("Statistics");
  if(!$xlSrcSheet)
  {
    die qq(Can't open "Statistics" sheet. Make sure it exists and have the right data.\n);
  }
  $xlSrcSheet->Activate;

  my $xlEffortSheet = $xlBook->Sheets("Effort");
  if(!$xlEffortSheet)
  {
    die qq(Can't open "Effort" sheet. Make sure it exists and have the right data.\n);
  }

  my $xlCustomerEffortSheet = $xlBook->Sheets("Customer Effort");
  if(!$xlCustomerEffortSheet)
  {
    die qq(Can't open "Customer Effort" sheet. Make sure it exists and have the right data.\n);
  }

  my $xlChartSheet = createExcelSheet($xlBook, $dstSheetName, 1);
    
  # my $srcRow = 1;
  # my $srcCol = 1;
  # while ( !($xlSrcSheet->Cells($srcRow, $srcCol)->{Value} eq "Bug Fix Rate") )
  # {
    # my $label = $xlSrcSheet->Cells($srcRow, $srcCol)->{Value};
    # $label =~ s/Novalis/Neo B MR4/;
    # $xlSrcSheet->Cells($srcRow, $srcCol)->{Value} = $label;
    # $srcRow++;
  # }

  my $queryTime = localtime;

  $xlChartSheet->Columns("A")->{ColumnWidth} = 2;

  $xlChartSheet->Cells(1, 2)->{Value} = "China COE Unisphere Performance Tools - Weekly Status Report (".$queryTime->strftime("%b %d, %Y").")";
  #the time format depends on locale
  $xlChartSheet->Cells(1, 2)->Font->{Name} = "Arial";
  $xlChartSheet->Cells(1, 2)->Font->{Size} = 14;
  $xlChartSheet->Cells(1, 2)->Font->{Bold} = 1;

  $xlChartSheet->Cells(3, 2)->{Value} = "AR Status";
  $xlChartSheet->Cells(3, 2)->Font->{Name} = "Tahoma";
  $xlChartSheet->Cells(3, 2)->Font->{Size} = 10;
  $xlChartSheet->Cells(3, 2)->Font->{Bold} = 1;
  $xlChartSheet->Cells(3, 2)->Font->{ColorIndex} = 2;
  $xlChartSheet->Range("B3:U3")->Interior->{ColorIndex} = 1;

  my $widthIncrement = $xlChartSheet->Columns("A")->{Width};
  my $heightIncrement =  $xlChartSheet->Rows("1:3")->{Height};

  my $wkRow = 1; #row count of data source
  my $width = 10 * $xlChartSheet->Columns("B")->{Width};
  my $length = 18 * $xlChartSheet->Rows("2")->{Height};
  my $cntChart = 0;

  #generate line charts for AR
  while (1)
  {
		my $skip = 0;
		foreach my $excludePattern (@excludeByTitle)
		{
			if($xlSrcSheet->Cells($wkRow, 1)->{Value} =~ m/($excludePattern)/)
			{
				$skip = 1;
				last;
			}
		}
		
		if($skip)
		{
			while (defined($xlSrcSheet->Cells($wkRow, 1)->{Value})) {$wkRow++}
			$wkRow++;
			next;
		}
		
    $xlChartObj = createExcelChart($xlSrcSheet, $wkRow, 
                    $xlChartSheet, 
										int($cntChart%2)*$width + $widthIncrement, int($cntChart/2)*$length + $heightIncrement, 
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

  if ($cntChart %  2)
  {
    $cntChart = $cntChart + 1;
  }

  $labelCol = 4 + int($cntChart/2)*18;

  $xlChartSheet->Cells($labelCol, 2)->{Value} = "Effort Distribution";
  $xlChartSheet->Cells($labelCol, 2)->Font->{Name} = "Tahoma";
  $xlChartSheet->Cells($labelCol, 2)->Font->{Size} = 10;
  $xlChartSheet->Cells($labelCol, 2)->Font->{Bold} = 1;
  $xlChartSheet->Cells($labelCol, 2)->Font->{ColorIndex} = 2;
  $xlChartSheet->Range("B".$labelCol.":U".$labelCol)->Interior->{ColorIndex} = 1;

  $heightIncrement = $xlChartSheet->Rows($labelCol)->{Height} + $heightIncrement;

  #generate pie charts for effort
  my $wkRow = 1;
  my $effortChartCnt = 0;
  while ($effortChartCnt != 2)
  {
    $xlChartObj = createExcelChart($xlEffortSheet, $wkRow, 
                    $xlChartSheet, 
										int($cntChart%2)*$width + $widthIncrement, int($cntChart/2)*$length + $heightIncrement, 
										$width, $length, xlPie);
										
		while (defined($xlEffortSheet->Cells($wkRow, 1)->{Value})) {$wkRow++}
		$wkRow++;

    if (defined($xlChartObj))
    {
			my $chart = $xlChartObj->{Chart};
			$chart->ApplyDataLabels(xlDataLabelsShowPercent, false, false, false, false, true, true, false, false);

			$cntChart++;
      $effortChartCnt++;
    }
    else
    {
      last;
    }
  };

  #generate customer effort histo chart
  my $wkRow = 1;
  while (1)
  {
    $xlChartObj = createExcelChart($xlCustomerEffortSheet, $wkRow, 
                    $xlChartSheet, 
										int($cntChart%2)*$width + $widthIncrement, int($cntChart/2)*$length + $heightIncrement, 
										$width, $length, xlColumnStacked);
										
		while (defined($xlCustomerEffortSheet->Cells($wkRow, 1)->{Value})) {$wkRow++}
		$wkRow++;

    if (defined($xlChartObj))
    {
      $cntChart++;
      my $chart = $xlChartObj->{Chart};
      $chart->{ChartTitle}->{Text} = "Effort on Customer Incident";
      $chart->Axes(xlValue, xlPrimary)->{HasTitle} = True;
      $chart->Axes(xlValue, xlPrimary)->{AxisTitle}->{Text} = "Hours";    
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
	  $chart->ApplyDataLabels(xlDataLabelsShowPercent, false, false, false, false, true, true, false, false);
	}
	
	return ($chartObject, $rowEnd+2);
}
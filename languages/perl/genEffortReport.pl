use strict;
use warnings;
use Time::Piece;
use Time::Seconds;
use Win32::OLE;
use Win32::OLE::Variant;
use Win32::OLE::Const 'Microsoft Excel';
use XML::Simple;
use File::Spec;
use DBI;
use Cwd;
use DateTime::Format::Excel;
use Class::Struct;
use Switch;

my $usage = "$0 <full or relative path of xlsx file> <date in format YYYY-M-D> <full or relative path of configuration file>";

#external parameter
my $reportFilePath = $ARGV[0];
my $reportDate = $ARGV[1];
$reportDate = Time::Piece->strptime($reportDate, '%Y-%m-%d');
my $configFilePath = $ARGV[2];

#internal parameter
my $statisticsSheetName = 'Statistics';
my $customerEffortSheetName = 'Customer Effort';
my $effortSheetName = "Effort";
my $featureEffortSheetName = 'Feature Effort';
my $featureStatusSheetName = "Feature Status";
my $individualEffortSheetName = 'Individual Effort';
my $individualReportSheetName = 'Individual Reports';
my $teamDefectSheetName = 'AllTeamDefect';
my $arStatusSheetName = "AR Status";

my $incrementalBugFixEffort = 0;
my $weekCountOfCustomerEffort = 5;

my @excludeChartByTitle = (
	'^.* by Type$',
	'^.* Non-PlannedContent by Priority$'
);

my @sheetsToHide = (
  "AllTeamDefect",
  "Effort",
  "Customer Effort",
  "Statistics",
);

my @ReleaseOrder = (
  "Release 32",
);

#the column number of each field in "AllTeamDefect" sheet
my $iEntry_Id = 0+1;
my $iSummary = 1+1;
my $iProduct_Release = 3+1;
my $iStatus = 4+1;
my $iStatus_Details = 5+1;
my $iPriority = 6+1;
my $iAssigned_to_Full_Name = 8+1;
my $iMajorArea = 11+1;
my $iCreate_Date = 14+1;
my $iLast_Status_Change = 20+1;

#internal data structure
my %effortEntriesByDate = ();

struct
(
  EFFORT_ENTRY =>
  [
    ProductName => '$',
    ReleaseName => '$',
    FeatureName => '$',
    PhaseName   => '$',
    ContributorName => '$',
    Effort      => '$'
  ]
);

struct
(
	DEFECT_ENTRY =>
	[
		Entry_Id				=> '$',
		Summary					=> '$',
		Product_Release			=> '$',
		Status					=> '$',
		Status_Details			=> '$',
		Priority				=> '$',
		Assigned_to_Full_Name	=> '$',
		Major_Area				=> '$',
		Create_date				=> '$',
		Last_Status_Change		=> '$'
	]
);

main();

sub main
{
  checkParam();
    
  my $reportTitle = getReportTitle($configFilePath);
  my %releases = getReleases($configFilePath);
    
  my $xlApp = Win32::OLE->new('Excel.Application', 'Quit');
  $xlApp->{Visible} = 0;
  $xlApp->{DisplayAlerts} = 0;
  my $xlBook = $xlApp->Workbooks->Open(File::Spec->rel2abs($reportFilePath)) || die qq(File "$reportFilePath" open failed.\n);
  
  eval {
    genBugfixEffort($xlBook, $statisticsSheetName);
 #   genCustomerEffort($xlBook, $customerEffortSheetName, $weekCountOfCustomerEffort, $reportDate, %releases);
    genEffort($xlBook, $effortSheetName, $reportDate);
    genFeatureStatus($xlBook, $statisticsSheetName, $customerEffortSheetName, $effortSheetName, $featureStatusSheetName, $reportDate, $reportTitle);
    genFeatureEffort($xlBook, $featureEffortSheetName, $reportDate);
    genFeatureEffortChart($xlBook, $featureEffortSheetName);
    genIndividualEffort($xlBook, $individualEffortSheetName, $reportDate);
    genIndividualEffortChart($xlBook, $individualEffortSheetName);
    genIndividualReport($xlApp, $xlBook, $individualReportSheetName, $reportDate);
    genARStatus($xlApp, $xlBook, $teamDefectSheetName, $arStatusSheetName, $reportDate);
    configureSheets($xlBook);
  };
  
  if($@)
  {
    print $@;
  }
  
  if($xlApp)
  {
    $xlApp->Quit;
    undef $xlApp;  
  }
}

sub getEffortDetail
{
  my $reportDate = shift;
  $reportDate = switchToReportDay($reportDate);
  my $reportDateString = $reportDate->strftime("%Y-%m-%d");
  
  my $effortEntries = $effortEntriesByDate{$reportDateString};
  if($effortEntries)
  {
    return $effortEntries;
  }
  
  my @effortEntries = ();
  if($reportDate < get2ndDBDate())
  {
    @effortEntries = getDBEffortDetail($reportDate);
  }
  else
  {
    @effortEntries = get2ndDBEffortDetail($reportDate);
  }
  
  $effortEntriesByDate{$reportDateString} = \@effortEntries;

  return \@effortEntries;
}

sub retrieveBugfixEffort
{
  my $reportDate = shift;
  my $effortEntries = getEffortDetail($reportDate);
  
  my $bugfixEffort = 0;

  foreach my $effortEntry (@$effortEntries)
  {
    if($effortEntry->PhaseName eq 'Defect Work'
      || $effortEntry->FeatureName eq 'Patch Support'
      || $effortEntry->FeatureName eq 'VNX SP\'s & Hot Fixes')
    {
      $bugfixEffort += $effortEntry->Effort;
    }
  }
  
  return $bugfixEffort;
}

sub genBugfixEffort
{
  my ($xlBook, $statisticsSheetName) = @_;
  
  my $xlSheet = $xlBook->Sheets($statisticsSheetName);
  if(!$xlSheet)
  {
    die qq(Can't open "$statisticsSheetName" sheet. Make sure it exists with correct data.\n);
  }

  my $row = 1;
  my $col = 2;
  my @dateList = ();
  while (1)
  {
		if(!($xlSheet->Cells($row, $col)->{Value2}))
		{
			last;
		}
    my $dateTemp = DateTime::Format::Excel->parse_datetime($xlSheet->Cells($row, $col)->{Value2});
		if(!$dateTemp)
		{
			last;
		}
    $dateTemp = Time::Piece->strptime($dateTemp->ymd, "%Y-%m-%d");
	$dateTemp = $dateTemp + 1 * ONE_DAY;

    push @dateList, switchToReportDay($dateTemp);
    $col++;
  }

  $col = 1;
  $row = 1;
  while (1)
  {
    if($xlSheet->Cells($row, $col)->{Value} 
    && "Bug Fix Effort" eq $xlSheet->Cells($row, $col)->{Value})
    {
      last;
    }
    
    $row++;
  }

  foreach my $date (@dateList)
  {
    my $bugfixEffort = retrieveBugfixEffort($date);
    
    $col++;
    
    my $oldValue = 0;
    if($incrementalBugFixEffort)
    {
      $oldValue = $xlSheet->Cells($row, $col)->{Value};
    }
    
    if($bugfixEffort)
    {
      $xlSheet->Cells($row, $col)->{Value} = $oldValue + $bugfixEffort;
    }
    else
    {
      $xlSheet->Cells($row, $col)->{Value} = $oldValue;
    }
  }
  
  $xlBook->Save;
  $xlBook->{Saved} = 1;
}

sub printUsage
{
	print "usage: $usage";
}

sub getReportTitle
{
	my $configFile = shift;
	$configFile = File::Spec->rel2abs($configFile);
  
	my $config = XMLin($configFile);
  my $reportTitle = $config->{ReportTitle};
  
  return $reportTitle? $reportTitle : "";
}

sub getReleases
{
	my $configFile = shift;
	$configFile = File::Spec->rel2abs($configFile);
  
	my $config = XMLin($configFile);
  my @releases = ref($config->{Releases}->{Release}) eq "ARRAY" ? @{$config->{Releases}->{Release}} : ($config->{Releases}->{Release});
	
	my %releaseDates = ();
	foreach my $release (@releases)
	{
    my $date = $release->{Date};
    if(!$date || $date eq "")
    {
      $date = "";
    }
    
		$releaseDates{$release->{Name}} = $date;
	}
	
	return %releaseDates;
}

sub getEmployeeIDs
{
	my $configFile = shift;
	$configFile = File::Spec->rel2abs($configFile);
  
	my $config = XMLin($configFile);
  my @engineers = ref($config->{Engineers}->{Engineer}) eq "ARRAY" ? @{$config->{Engineers}->{Engineer}} : ($config->{Engineers}->{Engineer});
	
	my @employeeIDs = ();
	foreach my $engineer (@engineers)
	{
		push @employeeIDs, $engineer->{EmployeeID};
	}
	
	return @employeeIDs;
}

sub checkParam
{
	if($ARGV[0] eq ""
		|| $ARGV[1] eq ""
    || $ARGV[2] eq ""
		)
	{
		printUsage();
		die "parameter missing\n";
	}
  
  #check existence of xlsx file and config file
  #check format of date
}

# take a date and return the date of Thursday of that week
sub switchToReportDay
{
  my $date = shift;
  
  if($date < get2ndDBDate())
  {
    return $date = $date + -1 * ($date->day_of_week - 4) * ONE_DAY;
  }

  return $date;  
}

sub genCustomerEffort
{
  my ($xlBook, $customerEffortSheetName, $weekCountOfCustomerEffort, $reportDate, %releases) = @_;
  
  $reportDate = switchToReportDay($reportDate);
	$reportDate = $reportDate - ONE_WEEK * $weekCountOfCustomerEffort;
  my %releaseList = ();
  my @dateList = ();
  my %customerEffort = ();
  while ($weekCountOfCustomerEffort > 0)
  {
    $weekCountOfCustomerEffort--;
		$reportDate = $reportDate + ONE_WEEK;
    my $to = $reportDate->strftime("%Y-%m-%d");
    
    push @dateList, $to;
    
    my $effortEntries = getEffortDetail($reportDate);
    foreach my $effortEntry (@$effortEntries)
    {
      my $product = $effortEntry->ProductName;
      my $release = $effortEntry->ReleaseName;
      
      if (isPastRelease($product, $release, $to, %releases))
      {
        if((not -1 == index(lc($release), "constellation"))
          || (not -1 == index(lc($release), "franklin")))
        {
          $release = "Elias";
        }
          
        if( $release eq 'VNX SPs & Hot Fixes')
        {
          $release = 'VNX SP\'s & Hot Fixes';
        }
        
        my $label = $to.",".$release;

        my $item = $customerEffort{$label};
        if($item)
        {
          $customerEffort{$label} += $effortEntry->Effort;
        }
        else
        {
          $customerEffort{$label} = $effortEntry->Effort;
        }
        
        $releaseList{$release} = 1;
      }
    }
    
  }

  my $xlSheet = createExcelSheet($xlBook, $customerEffortSheetName, 1);

  my $col = 1;
  my $row = 1;

  $xlSheet->Cells($row, $col)->{Value} = "Releases";
  while (my ($release, $value) = each %releaseList)
  {
    $row++;
    $xlSheet->Cells($row, $col)->{Value} = $release;
  }

  foreach my $date (@dateList)
  {
    $col++;
    $row=1;
    $xlSheet->Cells($row, $col)->{Value} = $date;
    while (my ($release, $value) = each %releaseList)
    {
      $row++;
      my $label = $date.",".$release;
      
      my $effort = $customerEffort{$label};
      if(!defined($effort))
      {
        $effort = 0;
      }
      $xlSheet->Cells($row, $col)->{Value} = $effort;
    }
  }
  
  $xlBook->Save;
  $xlBook->{Saved} = 1;  
}

sub isPastRelease
{
  my ($product, $release, $date, %releases) = @_;
  if(lc($product) eq "general")
  {
    return 0;
  }
  
  while( my($releaseName, $releaseDate) = each %releases)
  {
    if(lc($release) eq lc($releaseName))
    {
      if(!$releaseDate || $releaseDate eq "")
      {
        return 0;
      }
      else
      {
        my $tempReleaseDate = Time::Piece->strptime($releaseDate, "%Y-%m-%d");
        my $tempDate = Time::Piece->strptime($date, "%Y-%m-%d");

        return $tempDate > $tempReleaseDate;
      }
    }
  }
  
  return 1;
}

sub createExcelSheet
{
	my ($xlBook, $sheetName, $overwrite) = @_;
	
  my $xlSheet = $xlBook->Sheets->{$sheetName};
  if($xlSheet && $overwrite)
  {
    $xlBook->Sheets->{$sheetName}->Delete;
  }
  $xlSheet = $xlBook->Sheets->Add;
  $xlSheet->{Name} = $sheetName;
	
	return $xlSheet;
}

sub genEffort
{
  my ($xlBook, $effortSheetName, $reportDate) = @_;
  	
	my %devEffortHash = ();
	my %taskEffortHash = ();
	my $allDevEffort = 0.0;
	my $allTaskEffort = 0.0;
  
  my $effortEntries = getEffortDetail($reportDate);
	foreach my $effortEntry (@$effortEntries)
	{
    my $product = $effortEntry->ProductName;
    my $release = $effortEntry->ReleaseName;
    my $feature = $effortEntry->FeatureName;
    my $effort = $effortEntry->Effort;
    
    if($product eq 'General')
    {
      $taskEffortHash{$release} += $effort;
			$allTaskEffort += $effort;
    }
    else
    {
			my $taskName = "";
			if ($release eq "Other"
          || $release eq "N/A")
			{
				$taskName = $feature;
			}
			else
			{
				$taskName = $release." ".$feature;
			}
				
			if($devEffortHash{$taskName})
			{
				$devEffortHash{$taskName} += $effort;
			}
			else
			{
				$devEffortHash{$taskName} = $effort;
			}
			
			$allDevEffort += $effort;
		}
	}
	
	$taskEffortHash{"Development"} = $allDevEffort;
	$allTaskEffort += $allDevEffort;
		
  my @devEffortList = ();
	foreach my $effortName ( keys %devEffortHash ) {
		push @devEffortList, [$effortName, $devEffortHash{$effortName}];
	}
	
  my @taskEffortList = ();
	foreach my $effortName ( keys %taskEffortHash ) {
		push @taskEffortList, [$effortName, $taskEffortHash{$effortName}];
	}
	
	foreach my $item (@devEffortList)
  {
    if($allDevEffort)
    {
      @$item[1] /= $allDevEffort;
    }
  }

  foreach my $item (@taskEffortList)
  {
    if($allTaskEffort)
    {
      @$item[1] /= $allTaskEffort;
    }
  }

  my $xlSheet = createExcelSheet($xlBook, $effortSheetName, 1);

  my $col = 1;
  my $row = 1;

  $xlSheet->Cells($row, $col)->{Value} = "General";
  $xlSheet->Cells($row+1, $col)->{Value} = "Effort";
  $col++;
  foreach my $item (@taskEffortList)
  {
    $xlSheet->Cells($row, $col)->{Value} = @$item[0];
    $xlSheet->Cells($row+1, $col)->{Value} = @$item[1];
    $xlSheet->Cells($row+1, $col)->{NumberFormat} = "0.0%";
    $col++;
  }

  $col = 1;
  $row += 3;
  $xlSheet->Cells($row, $col)->{Value} = "Development";
  $xlSheet->Cells($row+1, $col)->{Value} = "Effort";
  $col++;
  foreach my $item (@devEffortList)
  {
    $xlSheet->Cells($row, $col)->{Value} = @$item[0];
    $xlSheet->Cells($row+1, $col)->{Value} = @$item[1];
    $xlSheet->Cells($row+1, $col)->{NumberFormat} = "0.0%";
    $col++;
  }

  $xlBook->Save;
  $xlBook->{Saved} = 1;
}

sub genFeatureEffort
{
  my ($xlBook, $featureEffortSheetName, $reportDate) = @_;
  
  my $effortEntries = getEffortDetail($reportDate);
  
  my %effortMap = formatFeatureEffort($effortEntries);
  saveFeatureEffort(\%effortMap, $xlBook, $featureEffortSheetName);
}

sub saveFeatureEffort
{
  my ($effortMap, $xlBook, $featureEffortSheetName) = @_;
    
  my $xlSheet = createExcelSheet($xlBook, $featureEffortSheetName, 1);
  
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

sub formatFeatureEffort
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

sub genFeatureEffortChart()
{
  my ($xlBook, $featureEffortSheetName) = @_;
  
  my $xlSrcSheet = $xlBook->Sheets($featureEffortSheetName);
  if(!$xlSrcSheet)
  {
    die qq(Can't open "$featureEffortSheetName" sheet. Make sure it exists and have the right data.\n);
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
  }
  my $wkRow = $chartRowCount + 1;
  while($chartRowCount--)
  {
    $xlSrcSheet->Cells(1, 1)->EntireRow->Insert(xlShiftDown);
  }

  #create charts
  my $cntChart = 0;
  while (1)
  {
    my $xlChartObj = createExcelChart($xlSrcSheet, $wkRow, 
                    $xlSrcSheet, 
										int($cntChart%2)*$width, int($cntChart/2)*$height, 
										$width, $height, xlPie);
										
		while (defined($xlSrcSheet->Cells($wkRow, 1)->{Value})) {$wkRow++}
		$wkRow++;
										
    if (defined($xlChartObj))
    {
			my $chart = $xlChartObj->{Chart};
			$chart->ApplyDataLabels(xlDataLabelsShowLabel, 0, 0, 0, 0, 1, 1, 0, 0);
			
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
}

sub createExcelChart
{
	my ($xlSrcSheet, $rowStart, $xlChartSheet, $toLeft, $toTop, $xScale, $yScale, $type) = @_;
	
	my $title = $xlSrcSheet->Cells($rowStart, 1)->{Value};
	return undef if (!defined($title));

	my $i = $rowStart;
	while (defined($xlSrcSheet->Cells($i, 1)->{Value})) {$i++}
	my $rowEnd = $i - 1;

	$i = 1;
	while (defined($xlSrcSheet->Cells($rowStart, $i)->{Value})) {$i++}
	my $colEnd = $i - 1;

	if($rowStart == $rowEnd)
	{
		$rowEnd++;
		$xlSrcSheet->Cells($rowEnd, 1)->{Value} = "Value";
		$i = 2;
		while (defined($xlSrcSheet->Cells($rowStart, $i)->{Value})) 
		{
			$xlSrcSheet->Cells($rowEnd, $i)->{Value} = 0;
			$i++;
		}
		
		$xlSrcSheet->Cells($rowEnd+1, 1)->EntireRow->Insert(xlShiftDown);	
	}
	
	my $chartObject = $xlChartSheet->ChartObjects->Add($toLeft, $toTop, $xScale, $yScale);
	$chartObject->{Placement} = xlFreeFloating;
	my $chart = $chartObject->{Chart};
	$chart->{ChartType} = $type;
	$chart->SetSourceData($xlSrcSheet->Range($xlSrcSheet->Cells($rowStart, 1), $xlSrcSheet->Cells($rowEnd, $colEnd)));
	$chart->{HasTitle} = 1; #True;
	$chart->{ChartTitle}->{Text} = $title;
	$chart->Axes(xlCategory, xlPrimary)->{CategoryType} = xlCategoryScale;
	
	return $chartObject;
}

sub genFeatureStatus
{
  my ($xlBook, $statisticsSheetName, $customerEffortSheetName, $effortSheetName, $featureStatusSheetName, $reportDate, $reportTitle) = @_;

  my $xlSrcSheet = $xlBook->Sheets($statisticsSheetName);
  if(!$xlSrcSheet)
  {
    die qq(Can't open $statisticsSheetName sheet. Make sure it exists and have the right data.\n);
  }
  $xlSrcSheet->Activate;

  my $xlEffortSheet = $xlBook->Sheets($effortSheetName);
  if(!$xlEffortSheet)
  {
    die qq(Can't open $effortSheetName sheet. Make sure it exists and have the right data.\n);
  }

#  my $xlCustomerEffortSheet = $xlBook->Sheets($customerEffortSheetName);
#  if(!$xlCustomerEffortSheet)
#  {
#    die qq(Can't open $customerEffortSheetName sheet. Make sure it exists and have the right data.\n);
#  }

  my $xlChartSheet = createExcelSheet($xlBook, $featureStatusSheetName, 1);
    
  $xlChartSheet->Columns("A")->{ColumnWidth} = 2;

  $xlChartSheet->Cells(1, 2)->{Value} = $reportTitle." (".$reportDate->strftime("%b %d, %Y").")";
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
    if(!defined($xlSrcSheet->Cells($wkRow, 1)->{Value}))
    {
      last;
    }
    
		my $skip = 0;
		foreach my $excludePattern (@excludeChartByTitle)
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
		
    my $xlChartObj = createExcelChart($xlSrcSheet, $wkRow, 
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
          $chart->Axes(xlValue, xlPrimary)->{HasTitle} = 1;
          $chart->Axes(xlValue, xlPrimary)->{AxisTitle}->{Text} = "Person x Hour";
        }
        case 'Bug Fix Rate'
        {
          $chart->{ChartType} = xlLineMarkers;
          $chart->Axes(xlValue, xlPrimary)->{HasTitle} = 1;
          $chart->Axes(xlValue, xlPrimary)->{AxisTitle}->{Text} = "Bug Fixed / ( Person x Week )";
        }
      }
			
			for (my $i=1; $i<=$chart->SeriesCollection->Count; $i++)
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

  my $labelCol = 4 + int($cntChart/2)*18;

  $xlChartSheet->Cells($labelCol, 2)->{Value} = "Effort Distribution";
  $xlChartSheet->Cells($labelCol, 2)->Font->{Name} = "Tahoma";
  $xlChartSheet->Cells($labelCol, 2)->Font->{Size} = 10;
  $xlChartSheet->Cells($labelCol, 2)->Font->{Bold} = 1;
  $xlChartSheet->Cells($labelCol, 2)->Font->{ColorIndex} = 2;
  $xlChartSheet->Range("B".$labelCol.":U".$labelCol)->Interior->{ColorIndex} = 1;

  $heightIncrement = $xlChartSheet->Rows($labelCol)->{Height} + $heightIncrement;

  #generate pie charts for effort
  $wkRow = 1;
  my $effortChartCnt = 0;
  while ($effortChartCnt != 2)
  {
    my $xlChartObj = createExcelChart($xlEffortSheet, $wkRow, 
                    $xlChartSheet, 
										int($cntChart%2)*$width + $widthIncrement, int($cntChart/2)*$length + $heightIncrement, 
										$width, $length, xlPie);
										
		while (defined($xlEffortSheet->Cells($wkRow, 1)->{Value})) {$wkRow++}
		$wkRow++;

    if (defined($xlChartObj))
    {
			my $chart = $xlChartObj->{Chart};
			$chart->ApplyDataLabels(xlDataLabelsShowPercent, 0, 0, 0, 0, 1, 1, 0, 0);

			$cntChart++;
      $effortChartCnt++;
    }
    else
    {
      last;
    }
  };

  #generate customer effort histo chart
#  $wkRow = 1;
#  while (1)
#  {
#    my $xlChartObj = createExcelChart($xlCustomerEffortSheet, $wkRow, 
#                    $xlChartSheet, 
#										int($cntChart%2)*$width + $widthIncrement, int($cntChart/2)*$length + $heightIncrement, 
#										$width, $length, xlColumnStacked);
#										
#		while (defined($xlCustomerEffortSheet->Cells($wkRow, 1)->{Value})) {$wkRow++}
#		$wkRow++;
#
#    if (defined($xlChartObj))
#    {
#      $cntChart++;
#      my $chart = $xlChartObj->{Chart};
#      $chart->{ChartTitle}->{Text} = "Effort on Customer Incident";
#      $chart->Axes(xlValue, xlPrimary)->{HasTitle} = 1;
#      $chart->Axes(xlValue, xlPrimary)->{AxisTitle}->{Text} = "Hours";    
#    }
#    else
#    {
#      last;
#    }
#  };

  $xlBook->Save;
  $xlBook->{Saved} = 1;
}

sub genIndividualEffort
{
  my ($xlBook, $individualEffortSheetName, $reportDate) = @_;
  my $effortEntries = getEffortDetail($reportDate);
  
  my %effortMap = formatIndividualEffort($effortEntries);
  saveIndividualEffort(\%effortMap, $xlBook, $individualEffortSheetName);
}

sub saveIndividualEffort
{
  my ($effortMap, $xlBook, $individualEffortSheetName) = @_;
  
  my $xlSheet = createExcelSheet($xlBook, $individualEffortSheetName, 1);
  
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

sub formatIndividualEffort
{
  my $effortEntries = shift;
  
  my %effortItems = ();
  foreach my $effortEntry (@$effortEntries)
  {
    my $effortName = $effortEntry->ProductName."-".$effortEntry->ReleaseName."-".$effortEntry->FeatureName;
    my $effort = $effortItems{$effortEntry->ContributorName}{$effortName};
    if(!$effort)
    {
      $effort = 0;
    }
    $effortItems{$effortEntry->ContributorName}{$effortName} = $effort + $effortEntry->Effort;
  }
  
  return %effortItems;
}

sub genIndividualEffortChart
{
  my ($xlBook, $individualEffortSheetName) = @_;

  #open sheet
  my $xlSrcSheet = $xlBook->Sheets($individualEffortSheetName);
  if(!$xlSrcSheet)
  {
    die qq(Can't open $individualEffortSheetName sheet. Make sure it exists and have the right data.\n);
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
    my $xlChartObj = createExcelChart($xlSrcSheet, $wkRow, 
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
}

sub genIndividualReport
{
  my ($xlApp, $xlBook, $individualReportSheetName, $reportDate) = @_;
  
  $reportDate = switchToReportDay($reportDate);

	my @engineers = getEngineers($configFilePath);

	my $xlSheet = createExcelSheet($xlBook, $individualReportSheetName, 1);

	my $col = 2;
  my $row = 2;
	
	foreach my $engineer (@engineers)
	{
		my ($key, $engineerName);
    if($reportDate < get2ndDBDate())
    {
      ($key, $engineerName) = getDBIndividualReport($engineer, $reportDate);
    }
    else
    {
      ($key, $engineerName) = get2ndDBIndividualReport($engineer, $reportDate);
    }

    if(!$key || !$engineerName)
    {
      print "Warning:\tCan't find key accomplishment for ".getEngineerName($engineer)."\n";;
      next;
    }
    
		#write header
		$xlSheet->Cells($row, $col)->{Value} = $engineerName;
		$xlSheet->Cells($row, $col)->Font->{Name} = "Tahoma";
		$xlSheet->Cells($row, $col)->Font->{Size} = 10;
		$xlSheet->Cells($row, $col)->Font->{Bold} = 1;
		$xlSheet->Cells($row, $col)->Font->{ColorIndex} = 2;
		$xlSheet->Range("B".$row.":U".$row)->Interior->{ColorIndex} = 1;

		$row++;
		
		#adjust text format
		$key =~ s/<br \/>/\n/gi;
    $key =~ s/<br>/\n/gi;
    $key =~ s/<\/p>/\n/gi;
		$key =~ s/<ol([^>]*)>//gi;
		$key =~ s/<\/ol>//gi;
		$key =~ s/<li([^>]*)>//gi;
		$key =~ s/<\/li>/\n/gi;
		$key =~ s/<div([^>]*)>//gi;
		$key =~ s/<\/div>//gi;
		$key =~ s/<span([^>]*)>//gi;
		$key =~ s/<\/span>//gi;
		$key =~ s/<ul([^>]*)>//gi;
		$key =~ s/<\/ul>//gi;
		$key =~ s/<p([^>]*)>//gi;
		$key =~ s/<\/p>//gi;
		$key =~ s/&nbsp;/ /gi;
		$key =~ s/&amp;/&/gi;
		$key =~ s/<font([^>]*)>//gi;
    $key =~ s/<\/font>//gi;
    $key =~ s/<b([^>]*)>//gi;
    $key =~ s/<\/b>//gi;
    $key =~ s/<\/?o\:p>//gi;
    
    $key =~ s/<[^>]*\/>//gi;
    
		#write accomplishments
		my @keys = split(/\n/, $key, -1);
		foreach my $str (@keys)
		{
			if(length($str) != 0
				&& $str =~ m/[a-zA-Z0-9]/)
			{
				$xlSheet->Cells($row, $col)->{Value} = $str;
				$xlSheet->Cells($row, $col)->Font->{Name} = "Tahoma";
				$xlSheet->Cells($row, $col)->Font->{Size} = 8;
				
				$row++;
			}
		}
		$row++;
	}
	
  $xlSheet = $xlBook->Sheets($individualReportSheetName);
  $xlApp->ActiveWindow->{DisplayGridlines} = 0;
  
  $xlBook->Save;
  $xlBook->{Saved} = 1;
}

sub configureSheets
{
  my $xlBook = shift;

  foreach my $sheetname (@sheetsToHide)
  {
    my $xlSheet = $xlBook->Sheets->{$sheetname};
    if($xlSheet)
    {
      $xlSheet->{Visible} = 0;
    }
  }

	my $xlSheet = $xlBook->Sheets->{"Feature Status"};
	$xlSheet->Activate;
	
  $xlBook->Save;
  $xlBook->{Saved} = 1;
}

sub genARStatus
{
  my ($xlApp, $xlBook, $teamDefectSheetName, $arStatusSheetName, $reportDate) = @_;
  
  my $xlSrcSheet = $xlBook->Sheets->{$teamDefectSheetName};
  if(!$xlSrcSheet)
  {
    die qq(Can't open "$teamDefectSheetName" sheet. Make sure it exists with correct data.\n);;
  }

  my $xlDstSheet = createExcelSheet($xlBook, $arStatusSheetName, 1);

  my $row = 2;
  my @ars;
  my $count = 0;
  while(defined($xlSrcSheet->Cells($row, $iEntry_Id)->{Value}))
  {
    my $ar = new DEFECT_ENTRY;
    
    @$ar = (
      $xlSrcSheet->Cells($row, $iEntry_Id)->{Value},
      $xlSrcSheet->Cells($row, $iSummary)->{Value},
      $xlSrcSheet->Cells($row, $iProduct_Release)->{Value},
      $xlSrcSheet->Cells($row, $iStatus)->{Value},
      $xlSrcSheet->Cells($row, $iStatus_Details)->{Value},
      $xlSrcSheet->Cells($row, $iPriority)->{Value},
      $xlSrcSheet->Cells($row, $iAssigned_to_Full_Name)->{Value},
      $xlSrcSheet->Cells($row, $iMajorArea)->{Value},
      
      #"Value2" doesn't return Currency or Datetime format.
      $xlSrcSheet->Cells($row, $iCreate_Date)->{Value2},
      $xlSrcSheet->Cells($row, $iLast_Status_Change)->{Value2}
    );
      
    $row++;
    if(isQualified($ar, $reportDate))
    {
      $count++;
      push @ars, $ar;
    }
  };

  my @releases = ();
  foreach my $release (@ReleaseOrder)
  {
    foreach my $ar (@ars)
    {
      if(lc($ar->Product_Release) eq lc($release))
      {
        push @releases, $release;
        last;
      }
    }
  }

  foreach my $ar (@ars)
  {
    my $found = 0;
    foreach my $release (@releases)
    {
      if(lc($ar->Product_Release) eq lc($release))
      {
        $found = 1;
        last;
      }
    }
    
    if(!$found)
    {
      push @releases, $ar->Product_Release;
    }
  }
    
  $xlDstSheet->Columns("A:A")->{ColumnWidth} = 3.14;
  $xlDstSheet->Columns("B:B")->{ColumnWidth} = 7.14;
  $xlDstSheet->Columns("C:C")->{ColumnWidth} = 11;
  $xlDstSheet->Columns("D:D")->{ColumnWidth} = 11;
  $xlDstSheet->Columns("E:E")->{ColumnWidth} = 7.14;
  $xlDstSheet->Columns("F:F")->{ColumnWidth} = 18.86;
  $xlDstSheet->Columns("G:G")->{ColumnWidth} = 22.71;
  $xlDstSheet->Columns("H:H")->{ColumnWidth} = 14.86;
  $xlDstSheet->Columns("I:I")->{ColumnWidth} = 100;

  $row = 1;
  foreach my $release (@releases)
  {
    $row++;
    my $col = 2;
    $xlDstSheet->Cells($row, $col)->{Value} = $release;
    $xlDstSheet->Cells($row, $col)->Font->{Name} = "Tahoma";
    $xlDstSheet->Cells($row, $col)->Font->{Size} = 14;
    $xlDstSheet->Cells($row, $col)->Font->{Bold} = 1;
    $row++;
    
    $xlDstSheet->Cells($row, $col++)->{Value} = "Entry ID";
    $xlDstSheet->Cells($row, $col++)->{Value} = "Release";
    $xlDstSheet->Cells($row, $col++)->{Value} = "Product Area";
    $xlDstSheet->Cells($row, $col++)->{Value} = "Priority";
    $xlDstSheet->Cells($row, $col++)->{Value} = "Status";
    $xlDstSheet->Cells($row, $col++)->{Value} = "Status Detail";
    $xlDstSheet->Cells($row, $col++)->{Value} = "Assignee";
    $xlDstSheet->Cells($row, $col)->{Value} = "Summary";
    $xlDstSheet->Range("B".$row.":I".$row)->Font->{Name} = "Tahoma";
    $xlDstSheet->Range("B".$row.":I".$row)->Font->{Size} = 8;
    $xlDstSheet->Range("B".$row.":I".$row)->Font->{Bold} = 1;
    $xlDstSheet->Range("B".$row.":I".$row)->Interior->{Color} = 9868950;
    $xlDstSheet->Range("B".$row.":I".$row)->{HorizontalAlignment} = xlCenter;
    $row++;
    
    foreach my $ar (@ars)
    {
      my $col = 2;
      if(lc($ar->Product_Release) eq lc($release))
      {
        $xlDstSheet->Cells($row, $col)->{Value} = $ar->Entry_Id;
        #$xlDstSheet->Cells($row, $col)->{HorizontalAlignment} = xlCenter;
        $col++;
        $xlDstSheet->Cells($row, $col)->{Value} = $ar->Product_Release;
        #$xlDstSheet->Cells($row, $col)->{HorizontalAlignment} = xlCenter;
        $col++;
        $xlDstSheet->Cells($row, $col)->{Value} = $ar->Major_Area;
        $col++;
        $xlDstSheet->Cells($row, $col)->{Value} = $ar->Priority;
        #$xlDstSheet->Cells($row, $col)->{HorizontalAlignment} = xlCenter;
        $col++;
        $xlDstSheet->Cells($row, $col)->{Value} = $ar->Status;
        #$xlDstSheet->Cells($row, $col)->{HorizontalAlignment} = xlCenter;
        $col++;
        $xlDstSheet->Cells($row, $col)->{Value} = $ar->Status_Details;
        $col++;      
        $xlDstSheet->Cells($row, $col)->{Value} = $ar->Assigned_to_Full_Name;
        $col++;
        $xlDstSheet->Cells($row, $col)->{Value} = $ar->Summary;
        $xlDstSheet->Range("B".$row.":I".$row)->Font->{Name} = "Tahoma";
        $xlDstSheet->Range("B".$row.":I".$row)->Font->{Size} = 8;
        $xlDstSheet->Range("B".$row.":I".$row)->Font->{Bold} = 1;
        
        if(isChangedLastWeek($ar, $reportDate))
        {
          $xlDstSheet->Range("B".$row.":I".$row)->Interior->{Color} = 10147522;
        }
        
        $row++;
      }
    }
  }
      
  $xlSrcSheet = $xlBook->Sheets($arStatusSheetName);
  $xlApp->ActiveWindow->{DisplayGridlines} = 0;

  $xlBook->Save;
  $xlBook->{Saved} = 1;
}

sub isQualified
{
  my ($ar, $reportDate) = @_;
  
  if(lc($ar->Product_Release) eq lc('Futures') )
  {
    return 0;
  }
  
  if(
      (
           lc($ar->Assigned_to_Full_Name) eq lc('Chen, Roger')
        || lc($ar->Assigned_to_Full_Name) eq lc('Wei, Nicholas')
        || lc($ar->Assigned_to_Full_Name) eq lc('Zhuang, Pei Yu')
        || lc($ar->Assigned_to_Full_Name) eq lc('Wang, Hao')
        || lc($ar->Assigned_to_Full_Name) eq lc('Wang, Tony (wangw5)')
        || lc($ar->Assigned_to_Full_Name) eq lc('Hui, Poulls')
        || lc($ar->Assigned_to_Full_Name) eq lc('Peng, Peter')
        || lc($ar->Assigned_to_Full_Name) eq lc('Hu, Yuji')
      )
      && 
      (
        lc($ar->Status) eq lc('Open')
        || lc($ar->Status) eq lc('Waiting on Originator')
      )
    )
  {
    return 1;
  }

  if(isChangedLastWeek($ar, $reportDate))
  {
    return 1;
  }
  
  return 0;
}

sub isChangedLastWeek
{
  my ($ar, $reportDate) = @_;
  if(!($ar->Create_date))
  {
    return 0;
  }
  
  my $createDate = DateTime::Format::Excel->parse_datetime($ar->Create_date);
  $createDate = Time::Piece->strptime($createDate->ymd." ".$createDate->hms, "%Y-%m-%d %H:%M:%S");
  
  my $lastStatusChange = DateTime::Format::Excel->parse_datetime($ar->Last_Status_Change);
  $lastStatusChange = Time::Piece->strptime($lastStatusChange->ymd." ".$lastStatusChange->hms, "%Y-%m-%d %H:%M:%S");

  if($createDate > $reportDate - ONE_WEEK || $lastStatusChange > $reportDate - ONE_WEEK)
  {
    return 1;
  }

  return 0;
}

sub get2ndDBDate
{
  return Time::Piece->strptime("2012-8-3", "%Y-%m-%d");
}

sub getDBIndividualReport
{
  my ($engineer, $reportDate) = @_;
  my $reportDateString = $reportDate->strftime("%Y-%m-%d");
  
  my $dbh = getDBConnection() || die "\nCan't Connect to database: $DBI::errstr";
  my $query = "SELECT distinct weekly_log.key_accomplishment FROM user, weekly_log where weekly_log.date_reported=\"".$reportDateString."\" and weekly_log.user_id=user.id and user.name=\"".getEngineerNTID($engineer)."\"";

  my $timesheet = $dbh->prepare($query);
  $timesheet->execute() || die "\nFailed to execute statement: \n$query\nError: $DBI::errstr";

  my $key	= $timesheet->fetchrow_array();
  $timesheet->finish();

  return ($key, getEngineerName($engineer));
}

sub get2ndDBIndividualReport
{
  my ($engineer, $reportDate) = @_;
  my $reportDateString = $reportDate->strftime("%Y-%m-%d");

  my $dbh = get2ndDBConnection() || die "\nCan't Connect to database: $DBI::errstr";
  my $query = 'SELECT time_report.external_commit, employee.name FROM time_report, employee where time_report.status!=\'open\' and time_report.reporter=employee.idemployee
and time_report.to_date="'.$reportDateString.'" and employee.emp_id='.getEngineerEmployeeID($engineer);

  my $timesheet = $dbh->prepare($query);
  $timesheet->execute() || die "\nFailed to execute statement: \n$query\nError: $DBI::errstr";

  my ($key, $engineerName)	= $timesheet->fetchrow_array();
  $timesheet->finish();
  
  return ($key, $engineerName);
}

sub get2ndDBEffortQuery
{
  my $reportDate = shift;
  $reportDate = switchToReportDay($reportDate);
  my $reportDateString = $reportDate->strftime("%Y-%m-%d");

  my @engineerIDs = getEmployeeIDs($configFilePath);
  my $filter = "";
  my $isFirstClause = 1;
  foreach my $engineerID (@engineerIDs)
  {
    if($isFirstClause)
    {
      $filter = " (employee.emp_id=".$engineerID;
      $isFirstClause = 0;
    }
    else
    {
      $filter .= " or employee.emp_id=".$engineerID;    
    }
  }
  $filter .= ")";
  
  my $sql = "SELECT product.name,`release`.name, feature.name, phase.name, employee.name, effort_spent FROM effort_entry, report_entry, task_item, phase, feature,`release`, product, employee, time_report WHERE time_report.status!=\'open\' and ".$filter." and time_report.to_date=\'".$reportDateString."\' and time_report.reporter=employee.idemployee and report_entry.time_report=time_report.idtime_report and report_entry.idreport_entry=effort_entry.report_entry and report_entry.task_item=task_item.idtask_item and task_item.phase=phase.idphase and task_item.feature=feature.idfeature and feature.`release`=`release`.idrelease and `release`.product=product.idproduct and effort_spent>0 order by product.name,`release`.name, feature.name, phase.name, employee.name";
  return $sql;
}

sub get2ndDBConnection
{
  my $dbh = DBI->connect("DBI:mysql:database=unified_operating;host=10.32.177.99","root","timereport", {'RaiseError' => 1}) || die "\nCan't Connect to database: $DBI::errstr";
  
  return $dbh;
}

sub get2ndDBEffortDetail
{
  my $reportDate = shift;
  my $sql = get2ndDBEffortQuery($reportDate);
  my $dbh = get2ndDBConnection() || die "\nCan't Connect to database: $DBI::errstr";
  
  my $query = $dbh->prepare($sql);
  my @effortEntries = ();
  if($query->execute())
  {
    while ( my($productName, $releaseName, $featureName, $phaseName, $contributorName, $effort)	
            = $query->fetchrow_array())
    {    
      my $effortEntry = new EFFORT_ENTRY;
      @$effortEntry = (
        $productName,
        $releaseName,
        $featureName,
        $phaseName,
        $contributorName,
        $effort
      );
      
      push @effortEntries, $effortEntry;
    }
    $query->finish();
  }
  
  $dbh->disconnect;
  undef $dbh;
  
  return @effortEntries;
}

sub getDBConnection
{
  my $dbh = DBI->connect("DBI:mysql:database=timesheet;host=report.emccrdc.com","admin","admin", {'RaiseError' => 1}) || die "\nCan't Connect to database: $DBI::errstr";
  
  return $dbh;
}

sub getDBEffortDetail
{
  my $reportDate = shift;
  my @engineers = getEngineers($configFilePath);
	
  my $toDate = switchToReportDay($reportDate);
  my $fromDate = $toDate - ONE_WEEK;
  $toDate = $toDate->strftime("%Y-%m-%d");
  $fromDate = $fromDate->strftime("%Y-%m-%d");
  
  my @effortEntries = ();
  foreach my $engineer (@engineers)
  {
    my $dbh = DBI->connect("DBI:mysql:database=timesheet;host=report.emccrdc.com","admin","admin", {'RaiseError' => 1}) || die "\nCan't Connect to database: $DBI::errstr";

    my $sql = "SELECT std_product.name, std_release.name releaseName, std_feature.name featureName, timeReport.weeklyTime effort ,std_task.phase_id FROM std_task ,  std_product , std_release , std_feature ,       (SELECT       SUM(timelog.time) AS weeklytime,       timelog.std_task_id AS taskId       FROM     timelog ,    timesheet.user    WHERE       timelog.date > \"".$fromDate."\"     AND   timelog.date <= \"".$toDate."\"     AND   timelog.user_id=timesheet.user.id     AND timesheet.user.name=\"".getEngineerNTID($engineer)."\"       GROUP by timelog.std_task_id)timeReport where timeReport.taskId = std_task.id AND std_task.feature_id = std_feature.id AND std_feature.release_id = std_release.id AND std_release.product_id = std_product.id ";
    my $query = $dbh->prepare($sql);
    $query->execute() || die "\nFailed to execute statement: \n$query\nError: $DBI::errstr";

    while ( my($productName, $releaseName, $featureName, $effort, $phaseId)	= $query->fetchrow_array())
    {
      my $phaseName = "";
      if($phaseId)
      {
        $phaseName = getPhaseNameById($phaseId);
      }
      
      my $effortEntry = new EFFORT_ENTRY;
      @$effortEntry = (
        $productName,
        $releaseName,
        $featureName,
        $phaseName,
        getEngineerName($engineer),
        $effort
      );
      
      push @effortEntries, $effortEntry;
    }
    $query->finish();
    
    $dbh->disconnect;
    undef $dbh;
  }
  
  return @effortEntries;
}

sub getPhaseNameById
{
  my $id = shift;
  
  my $dbh = DBI->connect("DBI:mysql:database=timesheet;host=report.emccrdc.com","admin","admin", {'RaiseError' => 1}) || die "\nCan't Connect to database: $DBI::errstr";

  my $sql = "SELECT name FROM std_phase where  id = ".$id;

  my $query = $dbh->prepare($sql);
  $query->execute() || die "\nFailed to execute statement: \n$query\nError: $DBI::errstr";
  
  my ($name) = $query->fetchrow_array();
  $query->finish();
  
  $dbh->disconnect;
  undef $dbh;
  
  return $name;
}

sub getEngineers
{
	my $configFile = shift;
	$configFile = File::Spec->rel2abs($configFile);
  
	my $config = XMLin($configFile);
  my @engineers = ref($config->{Engineers}->{Engineer}) eq "ARRAY" ? @{$config->{Engineers}->{Engineer}} : ($config->{Engineers}->{Engineer});
	
	return @engineers;
}

sub getEngineerNTID
{
	my $engineer = shift;
	return $engineer->{NTID};
}

sub getEngineerName
{
	my $engineer = shift;
	return $engineer->{Name};
}

sub getEngineerEmployeeID
{
	my $engineer = shift;
	return $engineer->{EmployeeID};
}
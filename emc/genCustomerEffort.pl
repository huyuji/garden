#description:   Generate the "Customer Effort" sheet.
#prerequisite:  

use DBI;
use Win32::OLE;
use Win32::OLE::Variant;
use Win32::OLE::Const 'Microsoft Excel';
use Cwd;
use Time::Piece;
use Time::Seconds;
use OnlineReport;

my $usage = "$0 <full or relative path of xlsx file> <date in format YYYY-M-D> <full or relative path of configuration file>";

my $bookname = $ARGV[0];
my $fromDate = $ARGV[1];
my $configFile = $ARGV[2];

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
}

sub printUsage
{
	print "usage: $usage";
}

my $weekCount = 5;
my $sheetname = 'Customer Effort';

my @ActiveReleases = (
  "novalis",
  "draconis",
  "others",
  "electra",
  "rockies",
  "mozzo",
  "neo b mr4",
  "kittyhawk",
  "unallocated"
);

my %PastReleases = (
  franklin => '2011-9-12',
  dorado => '2012-7-20',
  inyo => '2012-7-20',
);

my $dbh;
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
  
  if($dbh)
  {
    $dbh->disconnect;
    undef $dbh;
  }
}

sub main
{
  my @engineerIDs = getEngineerIDs($configFile);
	$dbh = DBI->connect("DBI:mysql:database=unified_operating;host=reviewforge.clrcase.lab.emc.com","readonly","readonly", {'RaiseError' => 1}) || die "\nCan't Connect to database: $DBI::errstr";

  $fromDate = Time::Piece->strptime($fromDate, "%Y-%m-%d");
  $fromDate = switchToReportDay($fromDate);
	my $toDate = $fromDate - ONE_WEEK * $weekCount;
  $iterCount = $weekCount;
  my %releaseList = ();
  my @dateList = ();
  my %customerEffort = ();
  while ($iterCount > 0)
  {
    $iterCount--;
    
    $fromDate = $toDate;
		$toDate = $fromDate + ONE_WEEK;
    $from = $fromDate->strftime("%Y-%m-%d");
    $to = $toDate->strftime("%Y-%m-%d");
    
    push @dateList, $to;
    
		foreach my $engineerID (@engineerIDs)
		{
			$devEffortQuery = 'SELECT product.name,`release`.name, feature.name, phase.name, employee.name, effort_spent FROM effort_entry, report_entry, task_item, phase, feature,`release`, product, employee, time_report WHERE time_report.status!=\'open\' and employee.emp_id='.$engineerID.' and time_report.to_date="'.$to.'" and time_report.reporter=employee.idemployee and report_entry.time_report=time_report.idtime_report and report_entry.idreport_entry=effort_entry.report_entry and report_entry.task_item=task_item.idtask_item and task_item.phase=phase.idphase and task_item.feature=feature.idfeature and feature.`release`=`release`.idrelease and `release`.product=product.idproduct and effort_spent>0 order by product.name,`release`.name, feature.name, phase.name, employee.name';
			
			$devEffort = $dbh->prepare($devEffortQuery);
			$devEffort->execute() || die "\nFailed to execute statement: \n$query\nError: $DBI::errstr";

			while ( my($product, $release, $feature, undef, undef, $effort)	= $devEffort->fetchrow_array())
			{
				if (isPastRelease($product, $release, $to))
				{
					if((not -1 == index(lc($release), "constellation"))
						|| (not -1 == index(lc($release), "franklin")))
					{
						$release = "Elias";
					}
						
					$hashRef = \%customerEffort;
					my $label = $to.",".$release;

					my $item = $$hashRef{$label};
					if(defined($item))
					{
						$$hashRef{$label} += $effort;
					}
					else
					{
						$$hashRef{$label} = $effort;
					}
					
					$releaseListRef = \%releaseList;
					$$releaseListRef{$release} = 1;
				}
			}
			
			$devEffort->finish();
		}
  }

  $xlApp->{Visible} = 0;
  $xlApp->{DisplayAlerts} = 0;
  my $xlBook = $xlApp->Workbooks->Open(File::Spec->rel2abs($bookname)) || die qq(File "$bookname" open failed.\n);

  my $xlSheet = createExcelSheet($xlBook, $sheetname, 1);

  my $col = 1;
  my $row = 1;

  $xlSheet->Cells($row, $col)->{Value} = "Releases";
  while (($release, $value) = each %releaseList)
  {
    $row++;
    $xlSheet->Cells($row, $col)->{Value} = $release;
  }

  foreach my $date (@dateList)
  {
    $col++;
    $row=1;
    $xlSheet->Cells($row, $col)->{Value} = $date;
    while (($release, $value) = each %releaseList)
    {
      $row++;
      $hashRef = \%customerEffort;
      my $label = $date.",".$release;
      
      my $effort = $$hashRef{$label};
      if(!defined($effort))
      {
        $effort = 0;
      }
      $xlSheet->Cells($row, $col)->{Value} = $effort;
    }
  }

  $dbh->disconnect;
  undef $dbh;
  
  $xlBook->Save;
  $xlBook->{Saved} = 1;
  $xlApp->Quit;
  undef $xlApp;
}

sub isActiveRelease
{
  my $release = shift;
  
  foreach my $activeRelease (@ActiveReleases)
  {
    if(lc($release) eq lc($activeRelease))
    {
      return 1;
    }
  }

  return 0;
}

sub isPastRelease
{
  my ($product, $release, $date) = @_;
  
  if(lc($product) eq "general")
  {
    return 0;
  }
  
  while( my($releaseName, $releaseDate) = each %PastReleases)
  {
    my $tempReleaseDate = Time::Piece->strptime($releaseDate, "%Y-%m-%d");
    my $tempDate = Time::Piece->strptime($date, "%Y-%m-%d");

    if(lc($release) eq lc($releaseName)
      && $tempDate < $tempReleaseDate)
    {
      return 0;
    }
  }
  
  return !isActiveRelease($release);
}
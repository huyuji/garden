#description:   Generate the "Effort" sheet.
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
my $toDate = $ARGV[1];
my $configFile = $ARGV[2];

my $sheetname = 'Effort';

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

  $toDate = Time::Piece->strptime($toDate, "%Y-%m-%d");
  $toDate = switchToReportDay($toDate);
  $fromDate = $toDate - ONE_WEEK;
  $to = $toDate->strftime("%Y-%m-%d");
  $from = $fromDate->strftime("%Y-%m-%d");

	$dbh = DBI->connect("DBI:mysql:database=unified_operating;host=reviewforge.clrcase.lab.emc.com","readonly","readonly", {'RaiseError' => 1}) || die "\nCan't Connect to database: $DBI::errstr";
	
	my %devEffortHash = ();
	my %taskEffortHash = ();
	my $allDevEffort = 0.0;
	my $allTaskEffort = 0.0;
	foreach my $engineerID (@engineerIDs)
	{
		$devEffortQuery = "SELECT product.name,`release`.name, feature.name, phase.name, employee.name, effort_spent FROM effort_entry, report_entry, task_item, phase, feature,`release`, product, employee, time_report WHERE time_report.status!=\'open\' and employee.emp_id=".$engineerID." and time_report.to_date=\'".$to."\' and time_report.reporter=employee.idemployee and report_entry.time_report=time_report.idtime_report and report_entry.idreport_entry=effort_entry.report_entry and report_entry.task_item=task_item.idtask_item and task_item.phase=phase.idphase and task_item.feature=feature.idfeature and feature.`release`=`release`.idrelease and `release`.product=product.idproduct and effort_spent>0 and product.name<>'General' order by product.name,`release`.name, feature.name, phase.name, employee.name";

		$allEffortQuery = "SELECT product.name,`release`.name, feature.name, phase.name, employee.name, effort_spent FROM effort_entry, report_entry, task_item, phase, feature,`release`, product, employee, time_report WHERE time_report.status!=\'open\' and employee.emp_id=".$engineerID." and time_report.to_date=\'".$to."\' and time_report.reporter=employee.idemployee and report_entry.time_report=time_report.idtime_report and report_entry.idreport_entry=effort_entry.report_entry and report_entry.task_item=task_item.idtask_item and task_item.phase=phase.idphase and task_item.feature=feature.idfeature and feature.`release`=`release`.idrelease and `release`.product=product.idproduct and effort_spent>0 and product.name='General' order by product.name,`release`.name, feature.name, phase.name, employee.name";

		$devEffort = $dbh->prepare($devEffortQuery);
		$devEffort->execute() || die "\nFailed to execute statement: \n$query\nError: $DBI::errstr";

		while ( my($product, $release, $feature, undef, undef, $effort)	= $devEffort->fetchrow_array())
		{
			my $taskName = "";
			if ($release eq "Other")
			{
				$taskName = $feature;
			}
			else
			{
				$taskName = $release." ".$feature;
			}
				
			# my $item = [$taskName, $effort];
			# push @devEffortList, $item;
			if($devEffortHash->{$taskName})
			{
				$devEffortHash->{$taskName} += $effort;
			}
			else
			{
				$devEffortHash->{$taskName} = $effort;
			}
			
			$allDevEffort += $effort;
		}
		$devEffort->finish();

		$allEffort = $dbh->prepare($allEffortQuery);
		$allEffort->execute() || die "\nFailed to execute statement: \n$query\nError: $DBI::errstr";

		while ( my($product, $release, undef, undef, undef, $effort)	= $allEffort->fetchrow_array())
		{
			$taskEffortHash->{$release} += $effort;
			$allTaskEffort += $effort;
		}
		$allEffort->finish();
	}
	
	$taskEffortHash->{"Development"} = $allDevEffort;
	$allTaskEffort += $allDevEffort;
		
	foreach my $effortName ( keys %$devEffortHash ) {
		push @devEffortList, [$effortName, $devEffortHash->{$effortName}];
	}
	
	foreach my $effortName ( keys %$taskEffortHash ) {
		push @taskEffortList, [$effortName, $taskEffortHash->{$effortName}];
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

  $xlApp->{Visible} = 0;
  $xlApp->{DisplayAlerts} = 0;
  my $xlBook = $xlApp->Workbooks->Open(File::Spec->rel2abs($bookname)) || die qq(File "$bookname" open failed.\n);

  my $xlSheet = createExcelSheet($xlBook, $sheetname, 1);

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
  
  $dbh->disconnect;
  undef $dbh;

  $xlBook->Save;
  $xlBook->{Saved} = 1;
  $xlApp->Quit;
  undef $xlApp;
}
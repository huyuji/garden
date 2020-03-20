#description:   Generate the "Effort Details" sheet.
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

my $sheetname = 'Effort Details';

my $bookname = $ARGV[0];
my $toDate = $ARGV[1];
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

sub main()
{
  my @engineerIDs = getEngineerIDs($configFile);
	
  $toDate = Time::Piece->strptime($toDate, "%Y-%m-%d");
  $toDate = switchToReportDay($toDate);
  $fromDate = $toDate - ONE_WEEK;
  $to = $toDate->strftime("%Y-%m-%d");
  $from = $fromDate->strftime("%Y-%m-%d");
  
  $xlApp->{Visible} = 0;
  $xlApp->{DisplayAlerts} = 0;
  my $xlBook = $xlApp->Workbooks->Open(File::Spec->rel2abs($bookname)) || die qq(File "$bookname" open failed.\n);

  my $xlSheet = createExcelSheet($xlBook, $sheetname, 1);

  my $col = 1;
  my $row = 1;

  $xlSheet->Cells($row, $col++)->{Value} = "Product";
  $xlSheet->Cells($row, $col++)->{Value} = "Release";
  $xlSheet->Cells($row, $col++)->{Value} = "Feature";
  $xlSheet->Cells($row, $col++)->{Value} = "Phase";
  $xlSheet->Cells($row, $col++)->{Value} = "Contributor";
  $xlSheet->Cells($row, $col++)->{Value} = "Effort(hour)";

  foreach my $engineerID (@engineerIDs)
  {
    my $dbh = DBI->connect("DBI:mysql:database=unified_operating;host=reviewforge.clrcase.lab.emc.com","readonly","readonly", {'RaiseError' => 1}) || die "\nCan't Connect to database: $DBI::errstr";

    my $sql = "SELECT product.name,`release`.name, feature.name, phase.name, employee.name, effort_spent FROM effort_entry, report_entry, task_item, phase, feature,`release`, product, employee, time_report WHERE time_report.status!=\'open\' and employee.emp_id=".$engineerID." and time_report.to_date=\'".$to."\' and time_report.reporter=employee.idemployee and report_entry.time_report=time_report.idtime_report and report_entry.idreport_entry=effort_entry.report_entry and report_entry.task_item=task_item.idtask_item and task_item.phase=phase.idphase and task_item.feature=feature.idfeature and feature.`release`=`release`.idrelease and `release`.product=product.idproduct and effort_spent>0 order by product.name,`release`.name, feature.name, phase.name, employee.name";
    print $sql."\n";
    
    my $query = $dbh->prepare($sql);
    $query->execute() || die "\nFailed to execute statement: \n$query\nError: $DBI::errstr";

    while ( my($productName, $releaseName, $featureName, $phaseName, $contributor, $effort)	= $query->fetchrow_array())
    {
      $col = 1;
      $row++;
      $xlSheet->Cells($row, $col++)->{Value} = $productName;
      $xlSheet->Cells($row, $col++)->{Value} = $releaseName;
      $xlSheet->Cells($row, $col++)->{Value} = $featureName;
      $xlSheet->Cells($row, $col++)->{Value} = $phaseName;
      $xlSheet->Cells($row, $col++)->{Value} = $contributor;
      $xlSheet->Cells($row, $col++)->{Value} = $effort;
    }
    $query->finish();
    
    $dbh->disconnect;
    undef $dbh;
  }
  
  $xlBook->Save;
  $xlBook->{Saved} = 1;
  $xlApp->Quit;
  undef $xlApp;
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
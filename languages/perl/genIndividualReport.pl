#description:   Generate the "Individual Report" sheet.
#prerequisite:  
#usage:         genIndividualReport.pl <full path of xlsx file> <date in format YYYY-M-D> <team name in online report system>
#example:       genIndividualReport.pl "d:\China COE UPT Weekly Report 2012-02-22 draft" 2012-02-22 "Unified Metrics & Tools - SH"

use DBI;
use Win32::OLE;
use Win32::OLE::Variant;
use Win32::OLE::Const 'Microsoft Excel';
use Cwd;
use Time::Piece;
use Time::Seconds;
use DateTime;
use OnlineReport;

my $usage = "$0 <full or relative path of xlsx file> <date in format YYYY-M-D> <full or relative path of configuration file>";

$xlFilename = $ARGV[0];
$date = $ARGV[1];
$configFile = $ARGV[2];

my $sheetname = 'Individual Reports';

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
  $date = Time::Piece->strptime($date, "%Y-%m-%d");
  $date = switchToReportDay($date);
  $date = $date->strftime("%Y-%m-%d");

	my @engineerIDs = getEngineerIDs($configFile);

	$xlApp->{Visible} = 0;
	$xlApp->{DisplayAlerts} = 0;
	my $xlBook = $xlApp->Workbooks->Open(File::Spec->rel2abs($xlFilename)) || die qq(File "$xlFilename" open failed.\n);

	my $xlSheet = createExcelSheet($xlBook, $sheetname, 1);

	$col = $row = 2;
	
	foreach my $engineerID (@engineerIDs)
	{
		$dbh = DBI->connect("DBI:mysql:database=unified_operating;host=reviewforge.clrcase.lab.emc.com","readonly","readonly", {'RaiseError' => 1}) || die "\nCan't Connect to database: $DBI::errstr";
		$query = 'SELECT time_report.external_commit, employee.name FROM time_report, employee where time_report.status!=\'open\' and time_report.reporter=employee.idemployee
and time_report.to_date="'.$date.'" and employee.emp_id='.$engineerID;
    print $query."\n";
    
		$timesheet = $dbh->prepare($query);
		$timesheet->execute() || die "\nFailed to execute statement: \n$query\nError: $DBI::errstr";

		my ($key, $engineerName)	= $timesheet->fetchrow_array();
		$timesheet->finish();
    print $engineerName.":".$key."\n";

		#write header
		$xlSheet->Cells($row, $col)->{Value} = $engineerName;
		$xlSheet->Cells($row, $col)->Font->{Name} = "Tahoma";
		$xlSheet->Cells($row, $col)->Font->{Size} = 10;
		$xlSheet->Cells($row, $col)->Font->{Bold} = 1;
		$xlSheet->Cells($row, $col)->Font->{ColorIndex} = 2;
		$xlSheet->Range("B".$row.":U".$row)->Interior->{ColorIndex} = 1;

		$row++;
		
		#adjust text format
		$key =~ s/<br \/>/\n/g;
    $key =~ s/<br>/\n/g;
    $key =~ s/<\/p>/\n/g;
		$key =~ s/<ol([^>]*)>//g;
		$key =~ s/<\/ol>//g;
		$key =~ s/<li([^>]*)>//g;
		$key =~ s/<\/li>/\n/g;
		$key =~ s/<div([^>]*)>//g;
		$key =~ s/<\/div>//g;
		$key =~ s/<span([^>]*)>//g;
		$key =~ s/<\/span>//g;
		$key =~ s/<ul([^>]*)>//g;
		$key =~ s/<\/ul>//g;
		$key =~ s/<p([^>]*)>//g;
		$key =~ s/<\/p>//g;
		$key =~ s/&nbsp;/ /g;
		$key =~ s/&amp;/&/g;
		
		#write accomplishments
		@keys = split(/\n/, $key, -1);
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
	
  $xlBook->Save;
  $xlBook->{Saved} = 1;
  $xlApp->Quit;
  undef $xlBook;
  undef $xlApp;

  #don't display grid line
  my $xlApp = Win32::OLE->new('Excel.Application');
  my $xlBook = $xlApp->Workbooks->Open(File::Spec->rel2abs($xlFilename)) || die qq(File "$xlFilename" open failed.\n);
  my $xlSrcSheet = $xlBook->Sheets($sheetname);
  $xlApp->ActiveWindow->{DisplayGridlines} = 0;

  $xlBook->Save;
  $xlBook->{Saved} = 1;
  $xlApp->Quit;
  undef $xlBook;
  undef $xlApp;
}
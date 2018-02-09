#description:   Fill in the "Bug Fix Effort" row in "Statistics" sheet.
#prerequisite:  The "Statistics" sheet must exist and have correct data.

use DBI;
use Win32::OLE;
use Win32::OLE::Variant;
use Cwd;
use Time::Piece;
use Time::Seconds;
use DateTime::Format::Excel;
use OnlineReport;

my $usage = "$0 <full or relative path of xlsx file> <full or relative path of configuration file>";

my $sheetname = 'Statistics';
my $incremental = 0;

my $bookname = $ARGV[0];
my $configFile = $ARGV[1];

sub checkParam
{
	if($ARGV[0] eq ""
		|| $ARGV[1] eq ""
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

  $xlApp->{Visible} = 0;
  $xlApp->{DisplayAlerts} = 0;
  my $xlBook = $xlApp->Workbooks->Open(File::Spec->rel2abs($bookname)) || die qq(File "$bookname" open failed.\n);
  my $xlSheet = $xlBook->Sheets($sheetname);
  if(!$xlSheet)
  {
    die qq(Can't open "$sheetname" sheet. Make sure it exists with correct data.\n);
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

    push @dateList, switchToReportDay($dateTemp);
    $col++;
  }

  $col = 1;
  $row = 1;
  while (1)
  {
    if("Bug Fix Effort" eq $xlSheet->Cells($row, $col)->{Value})
    {
      last;
    }
    
    $row++;
  }

  foreach my $date (@dateList)
  {
    $toDate = $date;
    $fromDate = $toDate - ONE_WEEK;
    $from = $fromDate->strftime("%Y-%m-%d");
    $to = $toDate->strftime("%Y-%m-%d");
    
    my $bugfixEffort = 0;
		foreach my $engineerID (@engineerIDs)
		{
			$bugfixEffort += getIndividualBugfixEffort($engineerID, $from, $to);
		}
    
    $col++;
    
    my $oldValue = 0;
    if($incremental)
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
  $xlApp->Quit;
  undef $xlApp;
}
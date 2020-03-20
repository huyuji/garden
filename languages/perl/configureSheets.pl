#description:   Hide sheets with given name

use Win32::OLE;
use Win32::OLE::Variant;
use Win32::OLE::Const 'Microsoft Excel';
use Class::Struct;
use File::Spec;

#configuration
my @sheetsToHide = (
  "Effort Details",
  "AllTeamDefect",
  "Effort",
  "Customer Effort",
  "Statistics",
  "Feature Effort Stats",
  "Individual Effort Stats"
);

checkParam();
sub checkParam
{
	if($ARGV[0] eq "")
	{
		printUsage();
		die "parameter missing\n";
	}
}

sub printUsage
{
	print "usage: $0 <full or relative path of xlsx file>";
}

#global variables
my $bookname = $ARGV[0];
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
  my $xlBook = $xlApp->Workbooks->Open(File::Spec->rel2abs($bookname)) || die qq(File "$bookname" open failed.\n);

  foreach my $sheetname (@sheetsToHide)
  {
    my $xlSheet = $xlBook->Sheets->{$sheetname};
    if($xlSheet)
    {
      $xlSheet->{Visible} = 0;
    }
  }

	$xlSheet = $xlBook->Sheets->{"Feature Status"};
	$xlSheet->Activate;
	
  $xlBook->Save;
  $xlBook->{Saved} = 1;
  $xlApp->Quit;
  undef $xlApp;
}
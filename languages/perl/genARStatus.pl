#description:   Generate the "AR Status" sheet.
#prerequisite:  The "AllTeamDefect" sheet must exist.

use Class::Struct;
use Time::Piece;
use DateTime::Format::Excel;
use Time::Seconds;
use POSIX;
use File::Basename;
use File::Spec;
use Win32::OLE;
use Switch;
use Win32::OLE::Variant;
use OnlineReport;

my $usage = "$0 <full or relative path of xlsx file> <date in format YYYY-M-D>";

my @ReleaseOrder = (
  "Release 32",
);

my $srcSheetName = 'AllTeamDefect';
my $dstSheetName = "AR Status";

my $xlFilename = $ARGV[0];
my $chktime = $ARGV[1];

checkParam();
sub checkParam
{
	if($ARGV[0] eq ""
		|| $ARGV[1] eq "")
	{
		printUsage();
		die "parameter missing\n";
	}
}

sub printUsage
{
	print "usage: $usage";
}

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

my $xlApp = Win32::OLE->new('Excel.Application');

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
  $chktime = Time::Piece->strptime($chktime, "%Y-%m-%d");

  $| = 1;
  $xlApp->{Visible} = 0;
  $xlApp->{DisplayAlerts} = 0;
  my $xlBook = $xlApp->Workbooks->Open($xlFilename) || die qq(File "$xlFilename" open failed.\n);

  my $xlSrcSheet = $xlBook->Sheets->{$srcSheetName};
  if(!$xlSrcSheet)
  {
    $xlApp->Quit;
    undef $xlApp;
    die qq(Can't open "$srcSheetName" sheet. Make sure it exists with correct data.\n);;
  }

  my $xlDstSheet = createExcelSheet($xlBook, $dstSheetName, 1);

  $row = 2;
  my @ars;
  $count = 0;
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
    if(isQualified($ar))
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
    $col = 2;
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
      $col = 2;
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
        
        if(isChangedLastWeek($ar))
        {
          $xlDstSheet->Range("B".$row.":I".$row)->Interior->{Color} = 10147522;
        }
        
        $row++;
      }
    }
  }
      
  $xlBook->Save;
  $xlBook->{Saved} = 1;
  $xlApp->Quit;
  undef $xlBook;
  undef $xlApp;

  #don't display grid line
  $xlApp = Win32::OLE->new('Excel.Application');
  $xlBook = $xlApp->Workbooks->Open($xlFilename) || die qq(File "$xlFilename" open failed.\n);
  $xlSrcSheet = $xlBook->Sheets($sheetname);
  $xlApp->ActiveWindow->{DisplayGridlines} = 0;

  $xlBook->Save;
  $xlBook->{Saved} = 1;
  $xlApp->Quit;
  undef $xlBook;
  undef $xlApp;
}

sub isQualified
{
  my $ar = shift;
  
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

  if(isChangedLastWeek($ar))
  {
    return 1;
  }
  
  return 0;
}

sub isChangedLastWeek
{
  my $ar = shift;
  
  my $createDate = DateTime::Format::Excel->parse_datetime($ar->Create_date);
  $createDate = Time::Piece->strptime($createDate->ymd." ".$createDate->hms, "%Y-%m-%d %H:%M:%S");
  
  my $lastStatusChange = DateTime::Format::Excel->parse_datetime($ar->Last_Status_Change);
  $lastStatusChange = Time::Piece->strptime($lastStatusChange->ymd." ".$lastStatusChange->hms, "%Y-%m-%d %H:%M:%S");

  if($createDate > $chktime - ONE_WEEK || $lastStatusChange > $chktime - ONE_WEEK)
  {
    return 1;
  }

  return 0;
}
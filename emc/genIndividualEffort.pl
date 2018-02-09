#description:   Generate the "Individual Effort".
#prerequisite:  
#usage:         genIndividualEffort.pl <full path of xlsx file> <date in format YYYY-MM-DD> <full path of xml config file>
#example:       genIndividualEffort.pl "d:\China COE UPT Weekly Report 2012-02-22 draft" 2012-02-22 "d:\config.xml"

use DBI;
use Win32::OLE;
use Win32::OLE::Variant;
use Win32::OLE::Const 'Microsoft Excel';
use Cwd;
use Time::Piece;
use Time::Seconds;
use XML::Simple;
use Class::Struct;

$bookname = $ARGV[0];
$toDate = $ARGV[1];
$configfile = $ARGV[2];

struct
(
  EFFORT_ENTRY =>
  [
    ProductName => '$',
    ReleaseName => '$',
    FeatureName => '$',
    PhaseName   => '$',
    Effort      => '$'
  ]
);

struct
(
  EFFORT_ITEM =>
  [
    Effort    => '$',
    Count     => '$',
    Subitems  => '%'
  ]
);

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
  $toDate = Time::Piece->strptime($toDate, "%Y-%m-%d");
  $toDate = switchToThurdsay($toDate);
  $fromDate = $toDate - ONE_WEEK;
  $to = $toDate->strftime("%Y-%m-%d");
  $from = $fromDate->strftime("%Y-%m-%d");
  #print $from." ~ ".$to."\n";
  $sheetname = 'Individual Effort Stats';

  my $config = XMLin($configfile);
  my @teammembers = ref($config->{Engineer}) ? @{$config->{Engineer}} : ($config->{Engineer});

  $xlApp->{Visible} = 0;
  $xlApp->{DisplayAlerts} = 0;
  my $xlBook = $xlApp->Workbooks->Open($bookname) || die qq(File "$bookname" open failed.\n);

  my $xlSheet = $xlBook->Sheets->{$sheetname};
  if($xlSheet)
  {
    print qq(delete existing "$sheetname" sheet\n);
    $xlBook->Sheets->{$sheetname}->Delete;
  }
  $xlSheet = $xlBook->Sheets->Add;
  $xlSheet->{Name} = $sheetname;

  my $col = 1;
  my $row = 1;

  foreach my $membername (@teammembers)
  {
    my @effortEntries = retrieveIndividualEffort($membername);
    if(0 == scalar @effortEntries)
    {
      next;
    }
    my $effortMap = formatIndividualEffort(\@effortEntries);
    $row = saveIndividualEffort($membername, $effortMap, $row, $col, $xlSheet);
    $row++;
  }

  $xlBook->Save;
  $xlBook->{Saved} = 1;
  $xlApp->Quit;
  undef $xlApp;
}

sub saveIndividualEffort
{
  my ($membername, $rootItem, $row, $col, $xlSheet) = @_;
  
  $xlSheet->Cells($row, $col++)->{Value} = "Contributor";
  $xlSheet->Cells($row, $col++)->{Value} = "Effort";
  $xlSheet->Cells($row, $col++)->{Value} = "Product";
  $xlSheet->Cells($row, $col++)->{Value} = "Effort";
  $xlSheet->Cells($row, $col++)->{Value} = "Release";
  $xlSheet->Cells($row, $col++)->{Value} = "Effort";
  $xlSheet->Cells($row, $col++)->{Value} = "Feature";
  $xlSheet->Cells($row, $col++)->{Value} = "Effort";
  $xlSheet->Cells($row, $col++)->{Value} = "Phase";
  $xlSheet->Cells($row, $col++)->{Value} = "Effort";
  $xlSheet->Range("A".$row.":J".$row)->Font->{Bold} = 1;
  $row++;
  
  if($rootItem->Count > 1)
  {
    $xlSheet->Range("A".$row.":A".($row+$rootItem->Count-1))->Merge;
    $xlSheet->Range("A".$row.":A".($row+$rootItem->Count-1))->{VerticalAlignment} = xlCenter;
    $xlSheet->Range("B".$row.":B".($row+$rootItem->Count-1))->Merge;
    $xlSheet->Range("B".$row.":B".($row+$rootItem->Count-1))->{VerticalAlignment} = xlCenter;
  }
  #print "A".$row.":A".($row+$rootItem->Count-1)."\n";
  $xlSheet->Range("A".$row.":A".($row+$rootItem->Count-1))->{Value} = $membername;
  $xlSheet->Range("B".$row.":B".($row+$rootItem->Count-1))->{Value} = $rootItem->Effort;
  
  my $productItems = $rootItem->Subitems;
  my $productRow = $row;
  foreach my $productName ( keys %$productItems ) {
    my $productItem = $rootItem->Subitems($productName);
    #print $productName."*".$productItem->Effort."\n";
    
    if($productItem->Count > 1)
    {
      $xlSheet->Range("C".$productRow.":C".($productRow+$productItem->Count-1))->Merge;
      $xlSheet->Range("C".$productRow.":C".($productRow+$productItem->Count-1))->{VerticalAlignment} = xlCenter;
      $xlSheet->Range("D".$productRow.":D".($productRow+$productItem->Count-1))->Merge;
      $xlSheet->Range("D".$productRow.":D".($productRow+$productItem->Count-1))->{VerticalAlignment} = xlCenter;
    }
    #print "C".$productRow.":C".($productRow+$productItem->Count-1)."\n";    
    $xlSheet->Range("C".$productRow.":C".($productRow+$productItem->Count-1))->{Value} = $productName;
    $xlSheet->Range("D".$productRow.":D".($productRow+$productItem->Count-1))->{Value} = $productItem->Effort;
    
    my $releaseItems = $productItem->Subitems;
    my $releaseRow = $productRow;
    while( my($releaseName, $releaseItem) = each(%$releaseItems))
    {
      #print $releaseName."*".$releaseItem->Effort."\n";
      if($releaseItem->Count > 1)
      {
        $xlSheet->Range("E".$releaseRow.":E".($releaseRow+$releaseItem->Count-1))->Merge;
        $xlSheet->Range("E".$releaseRow.":E".($releaseRow+$releaseItem->Count-1))->{VerticalAlignment} = xlCenter;
        $xlSheet->Range("F".$releaseRow.":F".($releaseRow+$releaseItem->Count-1))->Merge;
        $xlSheet->Range("F".$releaseRow.":F".($releaseRow+$releaseItem->Count-1))->{VerticalAlignment} = xlCenter;
      }
      #print "E".$releaseRow.":E".($releaseRow+$releaseItem->Count-1)."\n";
      $xlSheet->Range("E".$releaseRow.":E".($releaseRow+$releaseItem->Count-1))->{Value} = $releaseName;
      $xlSheet->Range("F".$releaseRow.":F".($releaseRow+$releaseItem->Count-1))->{Value} = $releaseItem->Effort;
      
      my $featureItems = $releaseItem->Subitems;
      my $featureRow = $releaseRow;
      while( my($featureName, $featureItem) = each(%$featureItems))
      {
        #print $featureName."*".$featureItem->Effort."\n";
        if($featureItem->Count > 1)
        {
          $xlSheet->Range("G".$featureRow.":G".($featureRow+$featureItem->Count-1))->Merge;
          $xlSheet->Range("G".$featureRow.":G".($featureRow+$featureItem->Count-1))->{VerticalAlignment} = xlCenter;
          $xlSheet->Range("H".$featureRow.":H".($featureRow+$featureItem->Count-1))->Merge;
          $xlSheet->Range("H".$featureRow.":H".($featureRow+$featureItem->Count-1))->{VerticalAlignment} = xlCenter;
        }
        #print "G".$featureRow.":G".($featureRow+$featureItem->Count-1)."\n";
        $xlSheet->Range("G".$featureRow.":G".($featureRow+$featureItem->Count-1))->{Value} = $featureName;
        $xlSheet->Range("H".$featureRow.":H".($featureRow+$featureItem->Count-1))->{Value} = $featureItem->Effort;
        
        my $phaseItems = $featureItem->Subitems;
        my $phaseRow = $featureRow;
        while( my($phaseName, $phaseItem) = each(%$phaseItems))
        {
          if($phaseName eq "")
          {
            #print "Empty Phase\n";
            $phaseRow++;
            next;
          }
          else
          {
            #print $phaseName."*".$phaseItem->Effort."\n";
            #print "I".$phaseRow."\n";
            $xlSheet->Range("I".$phaseRow)->{Value} = $phaseName;
            $xlSheet->Range("J".$phaseRow)->{Value} = $phaseItem->Effort;
            $phaseRow++;
          }
        }
        
        $featureRow += $featureItem->Count;
      }
      
      $releaseRow += $releaseItem->Count;
    }
    
    $productRow += $productItem->Count;
  }
  
  return $row+$rootItem->Count;
}

sub formatIndividualEffort
{
  my $effortEntries = shift;
  
  my $root = new EFFORT_ITEM;
  @$root = ( 0, 0, ());
  
  foreach my $effortEntry (@$effortEntries)
  {      
    #print $effortEntry->ProductName."-".$effortEntry->ReleaseName."-".$effortEntry->FeatureName."-".$effortEntry->PhaseName."-".$effortEntry->Effort."\n";

    $root->Effort($root->Effort + $effortEntry->Effort);
    $root->Count($root->Count + 1);
    
    my $productItem = $root->Subitems($effortEntry->ProductName);
    if(!$productItem)
    {
      $productItem = new EFFORT_ITEM;
      @$productItem = (0, 0, ());
    }
    
    $productItem->Effort($productItem->Effort + $effortEntry->Effort);
    $productItem->Count($productItem->Count + 1);
    $root->Subitems($effortEntry->ProductName, $productItem);
    ##print $effortEntry->ProductName."*".$productItem->Effort."*".$productItem->Count."\n";
    
    my $releaseItem = $productItem->Subitems($effortEntry->ReleaseName);
    if(!$releaseItem)
    {
      $releaseItem = new EFFORT_ITEM;
      @$releaseItem = (0, 0, ());
    }
    
    $releaseItem->Count($releaseItem->Count + 1);
    $releaseItem->Effort($releaseItem->Effort + $effortEntry->Effort);
    $productItem->Subitems($effortEntry->ReleaseName, $releaseItem);
    ##print $effortEntry->ReleaseName."*".$releaseItem->Effort."*".$releaseItem->Count."\n";
    
    my $featureItem = $releaseItem->Subitems($effortEntry->FeatureName);
    if(!$featureItem)
    {
      $featureItem = new EFFORT_ITEM;
      @$featureItem = (0, 0, ());
    }
    
    $featureItem->Effort($featureItem->Effort + $effortEntry->Effort);
    $featureItem->Count($featureItem->Count + 1);
    $releaseItem->Subitems($effortEntry->FeatureName, $featureItem);
    ##print $effortEntry->FeatureName."*".$featureItem->Effort."*".$featureItem->Count."\n";
    
    my $phaseItem = $featureItem->Subitems($effortEntry->PhaseName);
    if(!$phaseItem)
    {
      $phaseItem = new EFFORT_ITEM;
      @$phaseItem = (0, 0, ());
    }
    
    $phaseItem->Effort($phaseItem->Effort + $effortEntry->Effort);
    $phaseItem->Count($phaseItem->Count + 1);
    $featureItem->Subitems($effortEntry->PhaseName, $phaseItem);
    ##print $effortEntry->PhaseName."*".$phaseItem->Effort."*".$phaseItem->Count."\n";    
  }
  
  return $root;
}

sub retrieveIndividualEffort
{
  my $membername = shift;
  
  my $memberid = $membername;
  if(-1 != index $teammembers, ',')
  {
    $memberid = getNTIDByName($membername);
    if($memberid eq "")
    {
      print qq(Can't find NTID for "$membername". Please specify NTID directly.\n);
      next;
    }
  }

  my $dbh = DBI->connect("DBI:mysql:database=unified_operating;host=reviewforge.clrcase.lab.emc.com","readonly","readonly", {'RaiseError' => 1}) || die "\nCan't Connect to database: $DBI::errstr";

  my $sql = "SELECT product.name,`release`.name, feature.name, phase.name, employee.name, effort_spent FROM effort_entry, report_entry, task_item, phase, feature,`release`, product, employee, time_report WHERE time_report.status!=\'open\' and employee.emp_id=".$engineerID." and time_report.to_date=\'".$to."\' and time_report.reporter=employee.idemployee and report_entry.time_report=time_report.idtime_report and report_entry.idreport_entry=effort_entry.report_entry and report_entry.task_item=task_item.idtask_item and task_item.phase=phase.idphase and task_item.feature=feature.idfeature and feature.`release`=`release`.idrelease and `release`.product=product.idproduct and effort_spent>0 order by product.name,`release`.name, feature.name, phase.name, employee.name";

  my $query = $dbh->prepare($sql);
  $query->execute() || die "\nFailed to execute statement: \n$query\nError: $DBI::errstr";

  my @effortEntries = ();
  while ( my($productName, $releaseName, $featureName, $phaseName, undef, $effort)	= $query->fetchrow_array())
  {    
    my $effortEntry = new EFFORT_ENTRY;
    @$effortEntry = (
      $productName,
      $releaseName,
      $featureName,
      $phaseName,
      $effort
    );
    
    push @effortEntries, $effortEntry;
  }
  $query->finish();
  
  $dbh->disconnect;
  
  return @effortEntries;
}

sub switchToThurdsay
{
  my $date = shift;
  
  $date = $date + -1 * ($date->day_of_week - 4) * ONE_DAY;
}

sub getNTIDByName
{
  my $membername = shift;
  
  my $dbh = DBI->connect("DBI:mysql:database=weeklyreport;host=report.emccrdc.com","admin","admin", {'RaiseError' => 1}) || die "\nCan't Connect to database: $DBI::errstr";
  
  my $sql = "select nt_account from weeklyreport.user where  weeklyreport.user.user_name=\"".$membername."\"";

  my $query = $dbh->prepare($sql);
  $query->execute() || die "\nFailed to execute statement: \n$query\nError: $DBI::errstr";

  my ($ntid) = $query->fetchrow_array();
  $query->finish();
  $dbh->disconnect;
  
  return $ntid;
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
  
  return $name;
}
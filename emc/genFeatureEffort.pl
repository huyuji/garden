#description:   Generate the "Feature Effort" sheet.
#prerequisite:  The "Effort Details" sheet must exists with correct data.
#usage:         genFeatureEffort.pl <full path of xlsx file>
#example:       genFeatureEffort.pl "d:\China COE UPT Weekly Report 2012-02-22 draft"

use DBI;
use Win32::OLE;
use Win32::OLE::Variant;
use Win32::OLE::Const 'Microsoft Excel';
use Cwd;
use Time::Piece;
use Time::Seconds;
use XML::Simple;
use Class::Struct;

my $bookname = $ARGV[0];

my $srcSheetName = "Effort Details";
my $dstSheetName = 'Feature Effort Stats';

struct
(
	EFFORT_ENTRY =>
	[
    ProductName => '$',
    ReleaseName => '$',
    FeatureName => '$',
    PhaseName   => '$',
    Contributor  => '$',
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
  my @effortEntries = retrieveIndividualEffort($bookname, $srcSheetName);
  # foreach my $effortEntry (@effortEntries)
  # {
    # print $effortEntry->ProductName."-".$effortEntry->ReleaseName."-".$effortEntry->FeatureName."-".$effortEntry->PhaseName."-".$effortEntry->Contributor."-".$effortEntry->Effort."\n";
  # }
  
  my %effortMap = formatIndividualEffort(\@effortEntries);
  saveIndividualEffort(\%effortMap, $bookname, $dstSheetName);
}

sub saveIndividualEffort
{
  my ($effortMap, $bookname, $dstSheetName) = @_;
  
  $xlApp->{Visible} = 0;
  $xlApp->{DisplayAlerts} = 0;
  my $xlBook = $xlApp->Workbooks->Open($bookname) || die qq(File "$bookname" open failed.\n);
  
  my $xlSheet = $xlBook->Sheets->{$dstSheetName};
  if($xlSheet)
  {
    print qq(delete existing "$dstSheetName" sheet\n);
    $xlBook->Sheets->{$dstSheetName}->Delete;
  }
  $xlSheet = $xlBook->Sheets->Add;
  $xlSheet->{Name} = $dstSheetName;
  
  my $row = 1;
  my $col = 1;
  $xlSheet->Cells($row, $col++)->{Value} = "Product";
  $xlSheet->Cells($row, $col++)->{Value} = "Effort";
  $xlSheet->Cells($row, $col++)->{Value} = "Release";
  $xlSheet->Cells($row, $col++)->{Value} = "Effort";
  $xlSheet->Cells($row, $col++)->{Value} = "Feature";
  $xlSheet->Cells($row, $col++)->{Value} = "Effort";
  $xlSheet->Cells($row, $col++)->{Value} = "Phase";
  $xlSheet->Cells($row, $col++)->{Value} = "Effort";
  $xlSheet->Cells($row, $col++)->{Value} = "Contributor";
  $xlSheet->Cells($row, $col++)->{Value} = "Effort";
  $xlSheet->Range("A".$row.":J".$row)->Font->{Bold} = 1;
  $row++;
  
  my $productRow = $row++;
  foreach my $productName ( keys %$effortMap ) {
    my $productItem = $effortMap->{$productName};
    #print $productName."*".$productItem->Effort."\n";
    
    if($productItem->Count > 1)
    {
      $xlSheet->Range("A".$productRow.":A".($productRow+$productItem->Count-1))->Merge;
      $xlSheet->Range("A".$productRow.":A".($productRow+$productItem->Count-1))->{VerticalAlignment} = xlCenter;
      $xlSheet->Range("B".$productRow.":B".($productRow+$productItem->Count-1))->Merge;
      $xlSheet->Range("B".$productRow.":B".($productRow+$productItem->Count-1))->{VerticalAlignment} = xlCenter;
    }
    #print "A".$productRow.":A".($productRow+$productItem->Count-1)."\n";    
    $xlSheet->Range("A".$productRow.":A".($productRow+$productItem->Count-1))->{Value} = $productName;
    $xlSheet->Range("B".$productRow.":B".($productRow+$productItem->Count-1))->{Value} = $productItem->Effort;
    
    my $releaseItems = $productItem->Subitems;
    my $releaseRow = $productRow;
    while( my($releaseName, $releaseItem) = each(%$releaseItems))
    {
      #print $releaseName."*".$releaseItem->Effort."\n";
      if($releaseItem->Count > 1)
      {
        $xlSheet->Range("C".$releaseRow.":C".($releaseRow+$releaseItem->Count-1))->Merge;
        $xlSheet->Range("C".$releaseRow.":C".($releaseRow+$releaseItem->Count-1))->{VerticalAlignment} = xlCenter;
        $xlSheet->Range("D".$releaseRow.":D".($releaseRow+$releaseItem->Count-1))->Merge;
        $xlSheet->Range("D".$releaseRow.":D".($releaseRow+$releaseItem->Count-1))->{VerticalAlignment} = xlCenter;
      }
      #print "C".$releaseRow.":C".($releaseRow+$releaseItem->Count-1)."\n";
      $xlSheet->Range("C".$releaseRow.":C".($releaseRow+$releaseItem->Count-1))->{Value} = $releaseName;
      $xlSheet->Range("D".$releaseRow.":D".($releaseRow+$releaseItem->Count-1))->{Value} = $releaseItem->Effort;
      
      my $featureItems = $releaseItem->Subitems;
      my $featureRow = $releaseRow;
      while( my($featureName, $featureItem) = each(%$featureItems))
      {
        #print $featureName."*".$featureItem->Effort."\n";
        if($featureItem->Count > 1)
        {
          $xlSheet->Range("E".$featureRow.":E".($featureRow+$featureItem->Count-1))->Merge;
          $xlSheet->Range("E".$featureRow.":E".($featureRow+$featureItem->Count-1))->{VerticalAlignment} = xlCenter;
          $xlSheet->Range("F".$featureRow.":F".($featureRow+$featureItem->Count-1))->Merge;
          $xlSheet->Range("F".$featureRow.":F".($featureRow+$featureItem->Count-1))->{VerticalAlignment} = xlCenter;
        }
        #print "E".$featureRow.":E".($featureRow+$featureItem->Count-1)."\n";
        $xlSheet->Range("E".$featureRow.":E".($featureRow+$featureItem->Count-1))->{Value} = $featureName;
        $xlSheet->Range("F".$featureRow.":F".($featureRow+$featureItem->Count-1))->{Value} = $featureItem->Effort;
        
        my $phaseItems = $featureItem->Subitems;
        my $phaseRow = $featureRow;
        while( my($phaseName, $phaseItem) = each(%$phaseItems))
        {
          #print $phaseName."*".$phaseItem->Effort."\n";
          if($phaseItem->Count > 1)
          {
            $xlSheet->Range("G".$phaseRow.":G".($phaseRow+$phaseItem->Count-1))->Merge;
            $xlSheet->Range("G".$phaseRow.":G".($phaseRow+$phaseItem->Count-1))->{VerticalAlignment} = xlCenter;
            $xlSheet->Range("H".$phaseRow.":H".($phaseRow+$phaseItem->Count-1))->Merge;
            $xlSheet->Range("H".$phaseRow.":H".($phaseRow+$phaseItem->Count-1))->{VerticalAlignment} = xlCenter;
          }
          #print "G".$phaseRow."G".($phaseRow+$phaseItem->Count-1)."\n";
          $xlSheet->Range("G".$phaseRow.":G".($phaseRow+$phaseItem->Count-1))->{Value} = $phaseName;
          $xlSheet->Range("H".$phaseRow.":H".($phaseRow+$phaseItem->Count-1))->{Value} = $phaseItem->Effort;
          
          my $contributorItems = $phaseItem->Subitems;
          my $contributorRow = $phaseRow;
          while ( my($contributorName, $contributorItem) = each(%$contributorItems))
          {
            #print $contributorName."*".$contributorItem->Effort."\n";
            $xlSheet->Range("I".$contributorRow)->{Value} = $contributorName;
            $xlSheet->Range("J".$contributorRow)->{Value} = $contributorItem->Effort;
            $contributorRow++;
          }
          
          $phaseRow += $phaseItem->Count;
        }
        
        $featureRow += $featureItem->Count;
      }
      
      $releaseRow += $releaseItem->Count;
    }
    
    $productRow += $productItem->Count;
  }
  
  $xlBook->Save;
  $xlBook->{Saved} = 1;
}

sub formatIndividualEffort
{
  my $effortEntries = shift;
  my %effortItems;
  #my $effortItems = \%effortHash;
  
  foreach my $effortEntry (@$effortEntries)
  {      
    my $productItem = $effortItems{$effortEntry->ProductName};
    if(!$productItem)
    {
      $productItem = new EFFORT_ITEM;
      @$productItem = (0, 0, ());
    }
    
    $productItem->Effort($productItem->Effort + $effortEntry->Effort);
    $productItem->Count($productItem->Count + 1);
    $effortItems{$effortEntry->ProductName} = $productItem;
    #print $effortEntry->ProductName."*".$productItem->Effort."*".$productItem->Count."\n";
    
    my $releaseItem = $productItem->Subitems($effortEntry->ReleaseName);
    if(!$releaseItem)
    {
      $releaseItem = new EFFORT_ITEM;
      @$releaseItem = (0, 0, ());
    }
    
    $releaseItem->Count($releaseItem->Count + 1);
    $releaseItem->Effort($releaseItem->Effort + $effortEntry->Effort);
    $productItem->Subitems($effortEntry->ReleaseName, $releaseItem);
    #print $effortEntry->ReleaseName."*".$releaseItem->Effort."*".$releaseItem->Count."\n";
    
    my $featureItem = $releaseItem->Subitems($effortEntry->FeatureName);
    if(!$featureItem)
    {
      $featureItem = new EFFORT_ITEM;
      @$featureItem = (0, 0, ());
    }
    
    $featureItem->Effort($featureItem->Effort + $effortEntry->Effort);
    $featureItem->Count($featureItem->Count + 1);
    $releaseItem->Subitems($effortEntry->FeatureName, $featureItem);
    #print $effortEntry->FeatureName."*".$featureItem->Effort."*".$featureItem->Count."\n";
    
    my $phaseItem = $featureItem->Subitems($effortEntry->PhaseName);
    if(!$phaseItem)
    {
      $phaseItem = new EFFORT_ITEM;
      @$phaseItem = (0, 0, ());
    }
    
    $phaseItem->Effort($phaseItem->Effort + $effortEntry->Effort);
    $phaseItem->Count($phaseItem->Count + 1);
    $featureItem->Subitems($effortEntry->PhaseName, $phaseItem);
    #print $effortEntry->PhaseName."*".$phaseItem->Effort."*".$phaseItem->Count."\n";   

    my $contributorItem = $phaseItem->Subitems($effortEntry->Contributor);
    if(!$contributorItem)
    {
      $contributorItem = new EFFORT_ITEM;
      @$contributorItem = (0, 0, ());
    }
    
    $contributorItem->Effort($contributorItem->Effort + $effortEntry->Effort);
    $contributorItem->Count($contributorItem->Count + 1);
    $phaseItem->Subitems($effortEntry->Contributor, $contributorItem);
    #print $effortEntry->Contributor."*".$contributorItem->Effort."*".$contributorItem->Count."\n";       
  }
  
  return %effortItems;
}

sub retrieveIndividualEffort
{
  my ($bookname, $srcSheetName) = @_;
  
  $xlApp->{Visible} = 0;
  $xlApp->{DisplayAlerts} = 0;
  my $xlBook = $xlApp->Workbooks->Open($bookname) || die qq(File "$bookname" open failed.\n);
  my $xlSrcSheet = $xlBook->Sheets->{$srcSheetName};
  if(!$xlSrcSheet)
  {
    die qq(Can't open "$srcSheetName" sheet. Make sure it exists with correct data.\n);
  }

  my $row = 2;
  my @effortEntries = ();
  while(1)
  {
    my $col = 1;
    if(!$xlSrcSheet->Cells($row, $col)->{Value})
    {
      last;
    }
    
    my $effortEntry = new EFFORT_ENTRY;
    @$effortEntry = (
      $xlSrcSheet->Cells($row, $col++)->{Value},
      $xlSrcSheet->Cells($row, $col++)->{Value},
      $xlSrcSheet->Cells($row, $col++)->{Value},
      $xlSrcSheet->Cells($row, $col++)->{Value},
      $xlSrcSheet->Cells($row, $col++)->{Value},
      $xlSrcSheet->Cells($row, $col++)->{Value}
    );
    
    push @effortEntries, $effortEntry;
    $row++;
  }  
  
  $xlBook->Save;
  $xlBook->{Saved} = 1;

  return @effortEntries;
}

sub switchToThurdsay
{
  my $date = shift;
  
  $date = $date + -1 * ($date->day_of_week - 4) * ONE_DAY;
}
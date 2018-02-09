package OnlineReport;

use 5.012003;
use strict;
use warnings;
use Time::Piece;
use Time::Seconds;
use Win32::OLE;
use Win32::OLE::Variant;
use Win32::OLE::Const 'Microsoft Excel';
use XML::Simple;
use File::Spec;

require Exporter;

our @ISA = qw(Exporter);

# Items to export into callers namespace by default. Note: do not export
# names by default without a very good reason. Use EXPORT_OK instead.
# Do not simply export all your public functions/methods/constants.

# This allows declaration	use OnlineReport ':all';
# If you do not need this, moving things directly into @EXPORT or @EXPORT_OK
# will save memory.
our %EXPORT_TAGS = ( 'all' => [ qw(
	
) ] );

our @EXPORT_OK = ( @{ $EXPORT_TAGS{'all'} } );

our @EXPORT = qw(
	switchToReportDay
	createExcelChart
	createExcelSheet
	getIndividualBugfixEffort
	getEngineerIDs
	getEngineerID
	getEngineerName
	getEngineers
	getReportDate
);

our $VERSION = '0.01';


# Preloaded methods go here.

# take a date and return the date of Thursday of that week
sub switchToReportDay
{
  my $date = shift;

  return $date;
  return $date = $date + -1 * ($date->day_of_week - 4) * ONE_DAY;
}

# take a user name, search online report database and return the corresponding NTID
sub getNTIDByName
{
  my $membername = shift;
  
  my $dbh = DBI->connect("DBI:mysql:database=weeklyreport;host=naviweb.emccrdc.com","admin","admin", {'RaiseError' => 1}) || die "\nCan't Connect to database: $DBI::errstr";
  
  my $sql = "select nt_account from weeklyreport.user where  weeklyreport.user.user_name=\"".$membername."\"";

  my $query = $dbh->prepare($sql);
  $query->execute() || die "\nFailed to execute statement: \n$query\nError: $DBI::errstr";

  my ($ntid) = $query->fetchrow_array();
  $query->finish();
  
  $dbh->disconnect;
  undef $dbh;
  
  return $ntid;
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

sub printErrorToFile
{
	my $errMsg = shift;
	
	my $now = strftime("%Y-%m-%d", localtime(time));
	my $errorLog = "$now.txt";
	if(open FILE, ">>$errorLog")
	{
		print FILE $errMsg;
	}
	else
	{
	  print "unable to open error log $errorLog $!";
	}
}

sub printError
{
	my $errMsg = shift;
	
	print $errMsg;
}

sub getReportDBConnection
{
	return DBI->connect("DBI:mysql:database=unified_operating;host=reviewforge.clrcase.lab.emc.com","readonly","readonly", {'RaiseError' => 1}) or die "\nCan't Connect to database: $DBI::errstr";
}

sub closeReportDBConnection
{
	my $dbh = shift;
	if($dbh)
	{
		$dbh->disconnect;
	}
}

sub getIndividualBugfixEffort
{
	my ($engineerID, $fromDate, $toDate) = @_;
	
	my $query = 'SELECT sum(effort_spent) FROM effort_entry, report_entry, task_item, phase, feature,`release`, product, employee, time_report WHERE time_report.status!=\'open\' and employee.emp_id='.$engineerID.' and time_report.to_date=\''.$toDate.'\' and time_report.reporter=employee.idemployee and report_entry.time_report=time_report.idtime_report and report_entry.idreport_entry=effort_entry.report_entry and report_entry.task_item=task_item.idtask_item and task_item.phase=phase.idphase and task_item.feature=feature.idfeature and feature.`release`=`release`.idrelease and `release`.product=product.idproduct and effort_spent>0 and (phase.name=\'Defect Work\' or feature.name=\'Patch Support\' or feature.name=\'VNX SP\\\'s & Hot Fixes\') order by product.name,`release`.name, feature.name, phase.name, employee.name';
  print $query."\n";
	my $bugfixEffort = 0;
	
	eval
	{
		my $dbh = getReportDBConnection();
		
		my $effort = $dbh->prepare($query);
		$effort->execute() or die "\nFailed to execute statement: \n$query\nError: $DBI::errstr";
		$bugfixEffort = $effort->fetchrow_array();
    print $bugfixEffort."\n";
		$effort->finish();		
	};
	
	if($@)
	{
		printError $@;
		closeReportDBConnection();
	}
		
	return $bugfixEffort;
}

#following are utilities for online report configuration
sub getEngineerIDs
{
	my $configFile = shift;
	$configFile = File::Spec->rel2abs($configFile);
  
	my $config = XMLin($configFile);
  my @engineerIDs = ref($config->{Engineers}->{Engineer}) eq "ARRAY" ? @{$config->{Engineers}->{Engineer}} : ($config->{Engineers}->{Engineer});
	
	return @engineerIDs;
}

sub getEngineers
{
	my $configFile = shift;
	$configFile = File::Spec->rel2abs($configFile);
  
	my $config = XMLin($configFile);
  my @engineers = ref($config->{Engineers}->{Engineer}) eq "ARRAY" ? @{$config->{Engineers}->{Engineer}} : ($config->{Engineers}->{Engineer});
	
	return @engineers;
}

sub getEngineerID
{
	my $engineer = shift;
	return $engineer->{NTID};
}

sub getEngineerName
{
	my $engineer = shift;
	return $engineer->{Name};
}

sub getReportDate
{
	my $xlSheet = shift;
	
	my $row = 1;
	my $col = 2;
	my $date;
	while($xlSheet->Cells($row, $col)->{Value2})
	{
		my $dateTemp = DateTime::Format::Excel->parse_datetime($xlSheet->Cells($row, $col)->{Value2});
		if(!$date)
		{
			$date = $dateTemp;
		}
		else
		{
			if($date <= $dateTemp)
			{
				$date = $dateTemp;
			}
		}
			$col++;
	}
	
	return $date;
}

1;
__END__
# Below is stub documentation for your module. You'd better edit it!

=head1 NAME

OnlineReport - Perl extension for blah blah blah

=head1 SYNOPSIS

  use OnlineReport;
  blah blah blah

=head1 DESCRIPTION

Stub documentation for OnlineReport, created by h2xs. It looks like the
author of the extension was negligent enough to leave the stub
unedited.

Blah blah blah.

=head2 EXPORT

None by default.



=head1 SEE ALSO

Mention other useful documentation such as the documentation of
related modules or operating system documentation (such as man pages
in UNIX), or any relevant external documentation such as RFCs or
standards.

If you have a mailing list set up for your module, mention it here.

If you have a web site set up for your module, mention it here.

=head1 AUTHOR

A. U. Thor, E<lt>a.u.thor@a.galaxy.far.far.awayE<gt>

=head1 COPYRIGHT AND LICENSE

Copyright (C) 2012 by A. U. Thor

This library is free software; you can redistribute it and/or modify
it under the same terms as Perl itself, either Perl version 5.12.3 or,
at your option, any later version of Perl 5 you may have available.


=cut

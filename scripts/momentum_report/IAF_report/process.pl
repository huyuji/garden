use strict;
use warnings;
use File::Basename;

my $prefix = `perl time.pl`;
my $filename = `perl chooseFile.pl`;
my $basename = basename($filename);

if ($filename)
{
    print `perl xls2csv.pl --excel "$filename" --sheet Report > "$filename-$prefix-temp.csv"`;
    print `perl report.pl "$filename-$prefix-temp.csv" "$filename-$prefix.csv"`;
    print `perl verify.pl "$filename-$prefix.csv"`;
	system('pause')
}
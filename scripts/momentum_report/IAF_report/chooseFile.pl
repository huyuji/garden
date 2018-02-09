use strict; 
use warnings;
use Tk 8.0;

my $mw = MainWindow->new( -title => 'IAF Report Processing' );
my $filename = $mw->getOpenFile( -title => 'Select an IAF Report:',
  -filetypes => [['Excel Files', ['.xls', '.xlsx']],['All Files', ['*']]], -initialdir => '.' );
print $filename;

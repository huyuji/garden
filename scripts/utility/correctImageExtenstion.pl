use Image::Info qw(image_type);
use strict;
use warnings;
use File::Find;
use File::Basename;

my %type2ext = (PNG => ".png", JPEG => ".jpg");

sub process 
{
  my $file = $File::Find::name;
  if (!-d $file)
  {
    my $type = image_type($file);
    if (my $error = $type->{error}) 
    {
       print "$file\n";
       return;
    }
    
    my $ext = $type2ext{$type->{file_type}};
    if($ext)
    {
      my ($name, $path, $suffix) = fileparse($file, qr/\.[^.]*/);
      if(lc($ext) ne $suffix)
      {
        my $newName = $path.$name.$ext;
        print "rename $file to $newName\n";
        rename $file, $newName;
      }
    }
  }
}

find(\&process, $1);
my %TestData = (
  "https://hip.curvehero.com"                           => "https://curve.curvehero.com/hip",
  "https://hip.curvehero.com/"                          => "https://curve.curvehero.com/hip/",
  "https://hip.curvehero.com/something"                 => "https://curve.curvehero.com/hip/something",
  "https://preprod_hip.curvehero.net"                   => "https://preprod_curve.curvehero.net/hip",
  "https://preprod_hip.curvehero.net/"                  => "https://preprod_curve.curvehero.net/hip/",
  "https://preprod_hip.curvehero.net/something"         => "https://preprod_curve.curvehero.net/hip/something",
  "https://curve.curvehero.com/hip"                     => "https://curve.curvehero.com/hip",
  "https://curve.curvehero.com/hip/"                    => "https://curve.curvehero.com/hip/",
  "https://curve.curvehero.com/hip/something"           => "https://curve.curvehero.com/hip/something",
  "https://preprod_curve.curvehero.net/hip"             => "https://preprod_curve.curvehero.net/hip",
  "https://preprod_curve.curvehero.net/hip/"            => "https://preprod_curve.curvehero.net/hip/",
  "https://preprod_curve.curvehero.net/hip/something"   => "https://preprod_curve.curvehero.net/hip/something",
  "https://dentaloffice.curvehero.com/hip"              => "https://dentaloffice.curvehero.com",
  "https://dentaloffice.curvehero.com/hip/"             => "https://dentaloffice.curvehero.com",
  "https://dentaloffice.curvehero.com/hippy"            => "https://dentaloffice.curvehero.com/hippy",
  "https://dentaloffice.curvehero.net/hip"              => "https://dentaloffice.curvehero.net",
  "https://dentaloffice.curvehero.net/hip/"             => "https://dentaloffice.curvehero.net",
  "https://dentaloffice.curvehero.net/hippy"            => "https://dentaloffice.curvehero.net/hippy",
  "https://hip.dentaloffice.curvehero.com"              => "https://curve.dentaloffice.curvehero.com/hip"
);

sub debug
{
#  print @_;
}

sub splitUrl
{
  my $url = shift;
  debug "split $url\n";
  my $pos = index $url, "://";
  $url = substr $url, $pos + 3;
  $pos = index $url, "/";
  if($pos == -1)
  {
    return ($url, "");
  }
  else
  {
    my $HTTP_HOST = substr $url, 0, $pos;
    my $REQUEST_URI = substr $url, $pos;
    return ($HTTP_HOST, $REQUEST_URI);
  }
}

sub process 
{
  my ($HTTP_HOST, $REQUEST_URI) = @_;
  debug "processing $HTTP_HOST $REQUEST_URI\n";
  if ($HTTP_HOST =~ /^(preprod_|)hip\.(.*)$/)
  {
    return "https://$1curve.$2/hip${REQUEST_URI}"
  }
  elsif ($HTTP_HOST !~ /^(preprod_|)curve\./
    and $REQUEST_URI =~ /^\/hip(\/.*)?$/)
  {
    return "https://${HTTP_HOST}"
  }
  else
  {
    return "https://${HTTP_HOST}${REQUEST_URI}"
  }
}

while (($input, $expected) = each (%TestData))
{
  debug "process $input\n";
  my $output = process(splitUrl($input));
  debug "output $output\n";
  if(not $output eq $expected)
  {
    print "input:\t\t$input\nexpected:\t$expected\noutput:\t\t$output\n\n";
  }
}
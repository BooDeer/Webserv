#!/usr/bin/perl -w
use CGI;

$upload_dir = $ENV{'UPLOAD_LCOATION'};

$query = new CGI;

$filename = $query->param("uploadedFile");
$filename =~ s/.*[\/\\](.*)/$1/;
$upload_filehandle = $query->upload("uploadedFile");

open UPLOADFILE, ">$upload_dir/$filename";

while ( <$upload_filehandle> )
{
print UPLOADFILE;
}

close UPLOADFILE;
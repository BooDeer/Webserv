#!/usr/bin/perl -w

print "Content-Type: text/html\r\n\r\n";

print <<END_HTML;

<!DOCTYPE html>
<html>
<head>
  <title>Perl File Upload</title>
</head>
<body>
  <form method="POST" action="upload.pl" enctype="multipart/form-data">
    <div>
      <span>Upload a File:</span>
      <input type="file" name="uploadedFile" />
    </div>

    <input type="submit" name="uploadBtn" value="Upload" />
  </form>
</body>
</html>

END_HTML
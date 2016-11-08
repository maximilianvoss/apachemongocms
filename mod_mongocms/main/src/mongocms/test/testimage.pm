package test;
use strict;
use warnings FATAL => 'all';

our $TEST_PUBLISH_HOST;
our $DEBUG;

sub testimage_uploadPUT($) {
    my ($tokenID) = @_;

    print 'Testcase testimage_uploadPUT: ';

    my $cmd = 'curl -i -X PUT -b "tokenId='.$tokenID.'" --upload-file resource/image.jpg '.$TEST_PUBLISH_HOST.'/dam/ 2>/dev/null';
    my $output = `$cmd`;

    # status code
    $output =~ /\nHTTP\/1.1 (\d\d\d)/;
    my $statuscode = $1;
    die 'Wrong statuscode: '.$statuscode."\n".$output unless ( $statuscode eq 200 );

    $output =~ /"myId":"(\w+)"/;
    my $oid = $1;

    print "PASSED\n";
    print $cmd."\n".$output."\n\n" if ($DEBUG);

    return $oid;
}1;


sub testimage_uploadPOST($) {
    my ($tokenID) = @_;

    print 'Testcase testimage_uploadPOST: ';

    my $cmd = 'curl -i -X POST -b "tokenId='.$tokenID.'" --upload-file resource/image.jpg '.$TEST_PUBLISH_HOST.'/dam/ 2>/dev/null';
    my $output = `$cmd`;

    # status code
    $output =~ /\nHTTP\/1.1 (\d\d\d)/;
    my $statuscode = $1;
    die 'Wrong statuscode: '.$statuscode."\n".$output unless ( $statuscode eq 200 );

    $output =~ /"myId":"(\w+)"/;
    my $oid = $1;

    print "PASSED\n";
    print $cmd."\n".$output."\n\n" if ($DEBUG);

    return $oid;
}1;


sub testimage_getValidRendition($) {
    my ($imageID) = @_;

    print 'Testcase testimage_getValidRendition: ';

    my $cmd = 'curl -I -X GET '.$TEST_PUBLISH_HOST.'/dam/'.$imageID.'.huhu.jpg 2>/dev/null';
    my $output = `$cmd`;

    # status code
    $output =~ /HTTP\/1.1 (\d\d\d)/;
    my $statuscode = $1;
    die 'Wrong statuscode: '.$statuscode."\n".$output unless ( $statuscode eq 200 );

    $output =~ /Content-Type: ([\w|\/]+)/;
    my $contenttype = $1;
    die 'Wrong contenttype: |'.$contenttype."|\n".$output unless ($contenttype eq 'image/jpeg' );

    print "PASSED\n";
    print $cmd."\n".$output."\n\n" if ($DEBUG);
}1;


sub testimage_getInValidRendition($) {
    my ($imageID) = @_;

    print 'Testcase testimage_getInValidRendition: ';

    my $cmd = 'curl -I -X GET '.$TEST_PUBLISH_HOST.'/dam/'.$imageID.'.huhuabcd.jpg 2>/dev/null';
    my $output = `$cmd`;

    # status code
    $output =~ /HTTP\/1.1 (\d\d\d)/;
    my $statuscode = $1;
    die 'Wrong statuscode: '.$statuscode."\n".$output unless ( $statuscode eq 404 );

    print "PASSED\n";
    print $cmd."\n".$output."\n\n" if ($DEBUG);
}1;


sub testimage_getValidRenditionPNG($) {
    my ($imageID) = @_;

    print 'Testcase testimage_getValidRenditionPNG: ';

    my $cmd = 'curl -I -X GET '.$TEST_PUBLISH_HOST.'/dam/'.$imageID.'.huhu.png 2>/dev/null';
    my $output = `$cmd`;

    # status code
    $output =~ /HTTP\/1.1 (\d\d\d)/;
    my $statuscode = $1;
    die 'Wrong statuscode: '.$statuscode."\n".$output unless ( $statuscode eq 200 );

    $output =~ /Content-Type: ([\w|\/]+)/;
    my $contenttype = $1;
    die 'Wrong contenttype: |'.$contenttype."|\n".$output unless ($contenttype eq 'image/png' );

    print "PASSED\n";
    print $cmd."\n".$output."\n\n" if ($DEBUG);
}1;


sub testimage_getProperties($) {
    my ($imageID) = @_;

    print 'Testcase testimage_getProperties: ';

    my $cmd = 'curl -i -X GET '.$TEST_PUBLISH_HOST.'/dam/'.$imageID.'.json 2>/dev/null';
    my $output = `$cmd`;

    # status code
    $output =~ /HTTP\/1.1 (\d\d\d)/;
    my $statuscode = $1;
    die 'Wrong statuscode: '.$statuscode."\n".$output unless ( $statuscode eq 200 );

    $output =~ /"fi":"([\w|\\|\/]+)"/;
    my $file = $1;
    die 'Wrong filepath: '.$file."\n".$output unless ( $file eq '\/var\/www\/html\/assetstore\/69\/e9\/69e92ac37ec6c1859d074d1048460acc' );

    print "PASSED\n";
    print $cmd."\n".$output."\n\n" if ($DEBUG);
}1;
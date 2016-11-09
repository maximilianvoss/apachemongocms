package test;
use strict;
use warnings FATAL => 'all';

our $TEST_PUBLISH_HOST;
our $DEBUG;

sub testlist_asset_valid($) {
    my ($tokenID) = @_;

    print 'Testcase testlist_asset_valid: ';

    my $cmd = 'curl -i -X GET -b "tokenId='.$tokenID.'" '.$TEST_PUBLISH_HOST.'/dam/list.test.json 2>/dev/null';
    my $output = `$cmd`;

    # status code
    $output =~ /HTTP\/1.1 (\d\d\d)/;
    my $statuscode = $1;
    die 'Wrong statuscode: '.$statuscode."\n".$output unless ( $statuscode eq 200 );

    die 'Didn\'t ouput a result' unless ( $output =~ /{"result":/);

    print "PASSED\n";
    print $cmd."\n".$output."\n\n" if ($DEBUG);
}1;

sub testlist_asset_invalid($) {
    my ($tokenID) = @_;

    print 'Testcase testlist_asset_invalid: ';

    my $cmd = 'curl -i -X GET -b "tokenId='.$tokenID.'" '.$TEST_PUBLISH_HOST.'/dam/list.test2.json 2>/dev/null';
    my $output = `$cmd`;

    # status code
    $output =~ /HTTP\/1.1 (\d\d\d)/;
    my $statuscode = $1;
    die 'Wrong statuscode: '.$statuscode."\n".$output unless ( $statuscode eq 403 );

    print "PASSED\n";
    print $cmd."\n".$output."\n\n" if ($DEBUG);
}1;

sub testlist_user_valid($) {
    my ($tokenID) = @_;

    print 'Testcase testlist_user_valid: ';

    my $cmd = 'curl -i -X GET -b "tokenId='.$tokenID.'" '.$TEST_PUBLISH_HOST.'/user/list.test.json 2>/dev/null';
    my $output = `$cmd`;

    # status code
    $output =~ /HTTP\/1.1 (\d\d\d)/;
    my $statuscode = $1;
    die 'Wrong statuscode: '.$statuscode."\n".$output unless ( $statuscode eq 200 );

    die 'Didn\'t ouput a result\n'.$output unless ( $output =~ /{"result":/);

    print "PASSED\n";
    print $cmd."\n".$output."\n\n" if ($DEBUG);
}1;

sub testlist_user_invalid($) {
    my ($tokenID) = @_;

    print 'Testcase testlist_user_invalid: ';

    my $cmd = 'curl -i -X GET -b "tokenId='.$tokenID.'" '.$TEST_PUBLISH_HOST.'/user/list.test2.json 2>/dev/null';
    my $output = `$cmd`;

    # status code
    $output =~ /HTTP\/1.1 (\d\d\d)/;
    my $statuscode = $1;
    die 'Wrong statuscode: '.$statuscode."\n".$output unless ( $statuscode eq 403 );

    print "PASSED\n";
    print $cmd."\n".$output."\n\n" if ($DEBUG);
}1;

sub testlist_document_valid($$) {
    my ($tokenID, $user) = @_;

    print 'Testcase testlist_document_valid: ';

    my $cmd = 'curl -i -X GET -b "tokenId='.$tokenID.'" '.$TEST_PUBLISH_HOST.'/list.test.json?username='.$user.' 2>/dev/null';
    my $output = `$cmd`;

    # status code
    $output =~ /HTTP\/1.1 (\d\d\d)/;
    my $statuscode = $1;
    die 'Wrong statuscode: '.$statuscode."\n".$output unless ( $statuscode eq 200 );

    die 'Didn\'t ouput a result' unless ( $output =~ /{"result":/);

    print "PASSED\n";
    print $cmd."\n".$output."\n\n" if ($DEBUG);
}1;

sub testlist_document_invalid($) {
    my ($tokenID) = @_;

    print 'Testcase testlist_document_invalid: ';

    my $cmd = 'curl -i -X GET -b "tokenId='.$tokenID.'" '.$TEST_PUBLISH_HOST.'/list.test2.json 2>/dev/null';
    my $output = `$cmd`;

    # status code
    $output =~ /HTTP\/1.1 (\d\d\d)/;
    my $statuscode = $1;
    die 'Wrong statuscode: '.$statuscode."\n".$output unless ( $statuscode eq 403 );

    print "PASSED\n";
    print $cmd."\n".$output."\n\n" if ($DEBUG);
}1;
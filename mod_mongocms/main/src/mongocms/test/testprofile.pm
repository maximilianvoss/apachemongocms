package test;
use strict;
use warnings FATAL => 'all';

our $TEST_PUBLISH_HOST;
our $TEST_USERNAME;
our $TEST_PASSWORD;
our $DEBUG;

sub testprofile_getProfile($) {
    my ($tokenID) = @_;

    print 'Testcase testprofile_getProfile: ';

    my $cmd = 'curl -i -X GET -b "tokenId='.$tokenID.'" '.$TEST_PUBLISH_HOST.'/user/profile.json 2>/dev/null';
    my $output = `$cmd`;

    # status code
    $output =~ /HTTP\/1.1 (\d\d\d)/;
    my $statuscode = $1;
    die 'Wrong statuscode: '.$statuscode."\n".$output unless ( $statuscode eq 200 );

    $output =~ /"\$oid":"(\w+)"/;
    my $oid = $1;
    die 'Wrong oid: '.$oid."\n".$output unless ( $oid eq '579e6964cfec53671e77481a' );

    print "PASSED\n";
    print $cmd."\n".$output."\n\n" if ($DEBUG);
}1;


sub testprofile_setProfileWhite($) {
    my ($tokenID) = @_;

    print 'Testcase testprofile_setProfileWhite: ';

    my $randVal = int(rand(1000));

    my $cmd = 'curl -i -X POST -b "tokenId='.$tokenID.'" -d "test='.$randVal.'" '.$TEST_PUBLISH_HOST.'/user/profile.json 2>/dev/null';
    my $output = `$cmd`;

    # status code
    $output =~ /HTTP\/1.1 (\d\d\d)/;
    my $statuscode = $1;
    die 'Wrong statuscode: '.$statuscode."\n".$output unless ( $statuscode eq 200 );

    $output =~ /"test":(\d+)/;
    my $setVal = $1;
    die 'Wrong randVal: '.$randVal.' vs setVal: '.$setVal."\n".$output unless ( $setVal eq $randVal );

    print "PASSED\n";
    print $cmd."\n".$output."\n\n" if ($DEBUG);
}1;


sub testprofile_setProfileBlack($) {
    my ($tokenID) = @_;

    print 'Testcase testprofile_setProfileBlack: ';

    my $randVal = int(rand(1000));

    my $cmd = 'curl -i -X POST -b "tokenId='.$tokenID.'" -d "test'.$randVal.'=abc" '.$TEST_PUBLISH_HOST.'/user/profile.json 2>/dev/null';
    my $output = `$cmd`;

    # status code
    $output =~ /HTTP\/1.1 (\d\d\d)/;
    my $statuscode = $1;
    die 'Wrong statuscode: '.$statuscode."\n".$output unless ( $statuscode eq 200 );

    die 'Value was set without being on whitelist'."\n".$output unless ( $output !~ /"test$randVal":"abc"/ );

    print "PASSED\n";
    print $cmd."\n".$output."\n\n" if ($DEBUG);
}1;
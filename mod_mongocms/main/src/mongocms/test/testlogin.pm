package test;
use strict;
use warnings FATAL => 'all';

our $TEST_PUBLISH_HOST;
our $DEBUG;


sub testlogin_register($$) {
    my ($username, $password) = @_;
    print 'Testcase testlogin_register: ';

    my $cmd = 'curl -i -X POST -d "username='.$username.'&password='.$password.'" '.$TEST_PUBLISH_HOST.'/user/register.json 2>/dev/null';
    my $output = `$cmd`;

    # status code
    $output =~ /HTTP\/1.1 (\d\d\d)/;
    my $statuscode = $1;
    die 'Wrong statuscode: '.$statuscode."\n".$output unless ( $statuscode eq 200 );

    print "PASSED\n";
    print $cmd."\n".$output."\n\n" if ($DEBUG);
}1;

sub testlogin_login($$) {
    my ($username, $password) = @_;
    print 'Testcase testlogin_login: ';

    my $cmd = 'curl -i -X POST -d "username='.$username.'&password='.$password.'" '.$TEST_PUBLISH_HOST.'/user/login.json 2>/dev/null';
    my $output = `$cmd`;

    # status code
    $output =~ /HTTP\/1.1 (\d\d\d)/;
    my $statuscode = $1;
    die 'Wrong statuscode: '.$statuscode."\n".$output unless ( $statuscode eq 200 );

    # cookie tokenId
    $output =~ /Set-Cookie: tokenId=(\w+)/;
    my $tokenIdCookie = $1;

    # json tokenId
    $output =~ /"tokenId": "(\w+)"/;
    my $tokenIdJson = $1;

    die 'tokenId don\'t match: Cookie: '.$tokenIdCookie.' vs Json: '.$tokenIdJson."\n".$output unless ( $tokenIdCookie eq $tokenIdJson );

    print "PASSED\n";
    print $cmd."\n".$output."\n\n" if ($DEBUG);

    return $tokenIdCookie;
}1;


sub testlogin_logout($) {
    my ($tokenID) = @_;

    print 'Testcase testlogin_logout: ';

    my $cmd = 'curl -i -X GET -b "tokenId='.$tokenID.'" '.$TEST_PUBLISH_HOST.'/user/logout.json 2>/dev/null';
    my $output = `$cmd`;

    # status code
    $output =~ /HTTP\/1.1 (\d\d\d)/;
    my $statuscode = $1;
    die 'Wrong statuscode: '.$statuscode."\n".$output unless ( $statuscode eq 200 );

    print "PASSED\n";
    print $cmd."\n".$output."\n\n" if ($DEBUG);
}1;

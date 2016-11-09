package test;
use strict;
use warnings FATAL => 'all';

our $HOST;
our $DEBUG;

sub testdocument_newDocument($) {
    my ($tokenID) = @_;

    print 'Testcase testdocument_newDocument: ';

    my $cmd = 'curl -i -X POST -d \'param1=value1&param2=value2\' -b "tokenId='.$tokenID.'" '.$HOST.'/document.json 2>/dev/null';
    my $output = `$cmd`;

    # status code
    $output =~ /HTTP\/1.1 (\d\d\d)/;
    my $statuscode = $1;
    die 'Wrong statuscode: '.$statuscode."\n".$output unless ( $statuscode eq 200 );

    die 'Was not able to find param1 with correct value'."\n".$output unless ( $output =~ /"param1":"value1"/);
    die 'Was not able to find param2 with correct value'."\n".$output unless ( $output =~ /"param2":"value2"/);

    $output =~ /"myId\":\"(\w+)"/;
    my $oid = $1;

    print "PASSED\n";
    print $cmd."\n".$output."\n\n" if ($DEBUG);
    return $oid;
}1;


sub testdocument_getDocument($) {
    my ($documentId) = @_;

    print 'Testcase testdocument_getDocument: ';

    my $cmd = 'curl -i -X GET '.$HOST.'/'.$documentId.'.json 2>/dev/null';
    my $output = `$cmd`;

    # status code
    $output =~ /HTTP\/1.1 (\d\d\d)/;
    my $statuscode = $1;
    die 'Wrong statuscode: '.$statuscode."\n".$output unless ( $statuscode eq 200 );

    die 'Was not able to find param1 with correct value'."\n".$output unless ( $output =~ /"param1":"value1"/);
    die 'Was not able to find param2 with correct value'."\n".$output unless ( $output =~ /"param2":"value2"/);

    $output =~ /"myId\":\"(\w+)"/;
    my $oid = $1;

    print "PASSED\n";
    print $cmd."\n".$output."\n\n" if ($DEBUG);
    return $oid;
}1;


sub testdocument_updateDocument($$) {
    my ($tokenID, $documentId) = @_;

    print 'Testcase testdocument_updateDocument: ';

    my $cmd = 'curl -i -X POST -d \'param1=value3&param2=value4\' -b "tokenId='.$tokenID.'" '.$HOST.'/'.$documentId.'.json 2>/dev/null';
    my $output = `$cmd`;

    # status code
    $output =~ /HTTP\/1.1 (\d\d\d)/;
    my $statuscode = $1;
    die 'Wrong statuscode: '.$statuscode."\n".$output unless ( $statuscode eq 200 );

    die 'Was not able to find param1 with correct value'."\n".$output unless ( $output =~ /"param1":"value3"/);
    die 'Was not able to find param2 with correct value'."\n".$output unless ( $output =~ /"param2":"value4"/);

    $output =~ /"myId\":\"(\w+)"/;
    my $oid = $1;

    print "PASSED\n";
    print $cmd."\n".$output."\n\n" if ($DEBUG);
    return $oid;
}1;


sub testdocument_deleteDocumentPOST($$) {
    my ($tokenID, $documentId) = @_;

    print 'Testcase testdocument_deleteDocumentPOST: ';

    my $cmd = 'curl -i -X POST -d \'myId=' . $documentId . '\' -b "tokenId='.$tokenID.'" '.$HOST.'/delete.json 2>/dev/null';
    my $output = `$cmd`;

    # status code
    $output =~ /HTTP\/1.1 (\d\d\d)/;
    my $statuscode = $1;
    die 'Wrong statuscode: '.$statuscode."\n".$output unless ( $statuscode eq 200 );

    print "PASSED\n";
    print $cmd."\n".$output."\n\n" if ($DEBUG);
}1;

sub testdocument_deleteDocumentGET($$) {
    my ($tokenID, $documentId) = @_;

    print 'Testcase testdocument_deleteDocumentGET: ';

    my $cmd = 'curl -i -X GET -b "tokenId='.$tokenID.'" '.$HOST.'/delete.json?myId=' . $documentId . ' 2>/dev/null';
    my $output = `$cmd`;

    # status code
    $output =~ /HTTP\/1.1 (\d\d\d)/;
    my $statuscode = $1;
    die 'Wrong statuscode: '.$statuscode."\n".$output unless ( $statuscode eq 200 );

    print "PASSED\n";
    print $cmd."\n".$output."\n\n" if ($DEBUG);
}1;
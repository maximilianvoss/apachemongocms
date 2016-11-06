#!/usr/bin/perl
#

package test;
use strict;
use warnings FATAL => 'all';

our $TEST_PUBLISH_HOST = 'http://vmware2';
our $TEST_USERNAME = 'user';
our $TEST_PASSWORD = 'password';
our $DEBUG = 0;

use testlogin;
use testprofile;
use testimage;
use testrating;
use testdocument;

my $tokenID = testlogin_login();
testprofile_getProfile($tokenID);
testprofile_setProfileWhite($tokenID);
testprofile_setProfileBlack($tokenID);

my $documentId = testdocument_newDocument($tokenID);
testdocument_getDocument($documentId);
testdocument_updateDocument($tokenID, $documentId);

my $imagePUT = testimage_uploadPUT($tokenID);
testimage_getValidRendition($imagePUT);
testimage_getInValidRendition($imagePUT);
testimage_getValidRenditionPNG($imagePUT);
testimage_getProperties($imagePUT);

my $imagePOST = testimage_uploadPOST($tokenID);
testimage_getValidRendition($imagePOST);
testimage_getInValidRendition($imagePOST);
testimage_getValidRenditionPNG($imagePOST);
testimage_getProperties($imagePOST);

testlogin_logout($tokenID);

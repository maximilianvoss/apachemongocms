#!/usr/bin/perl
#

package test;
use strict;
use warnings FATAL => 'all';

our $TEST_PUBLISH_HOST = 'http://vmware2';
our $DEBUG = 0;

use testlogin;
use testprofile;
use testimage;
use testrating;
use testdocument;
use testlists;

my $user = 'testuser_'.int(rand(65535));

testlogin_register($user, 'password');
my $tokenID = testlogin_login($user, 'password');
testprofile_getProfile($tokenID, $user);
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

testlist_asset_valid($tokenID);
testlist_asset_invalid($tokenID);
testlist_user_valid($tokenID);
testlist_user_invalid($tokenID);
testlist_document_valid($tokenID);
testlist_document_invalid($tokenID);

testlogin_logout($tokenID);

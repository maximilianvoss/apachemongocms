#!/usr/bin/perl
#

package test;
use strict;
use warnings FATAL => 'all';

our $HOST = 'http://vmware';
our $DEBUG = 0;

use testlogin;
use testprofile;
use testimage;
use testrating;
use testdocument;
use testlists;

my $user = 'testuser_'.int(rand(65535));
my $password = 'userpassword_'.int(rand(65535));

testlogin_register($user, $password);
my $tokenID = testlogin_login($user, $password);
testprofile_getProfile($tokenID, $user);
testprofile_setProfileWhite($tokenID);
testprofile_setProfileBlack($tokenID);

my $documentId = testdocument_newDocument($tokenID);
testdocument_getDocument($documentId);
testdocument_updateDocument($tokenID, $documentId);
testlist_document_valid($tokenID, $user);
testlist_document_invalid($tokenID);
testdocument_deleteDocumentPOST($tokenID, $documentId);
$documentId = testdocument_newDocument($tokenID);
testdocument_deleteDocumentGET($tokenID, $documentId);

my $imagePUT = testimage_uploadPUT($tokenID);
testimage_getValidRendition($imagePUT);
testimage_getInValidRendition($imagePUT);
testimage_getValidRenditionPNG($imagePUT);
testimage_getProperties($imagePUT);
testimage_setProperties($tokenID, $imagePUT);
testimage_deletePOST($tokenID, $imagePUT);

my $imagePOST = testimage_uploadPOST($tokenID);
testimage_getValidRendition($imagePOST);
testimage_getInValidRendition($imagePOST);
testimage_getValidRenditionPNG($imagePOST);
testimage_getProperties($imagePOST);
testimage_setProperties($tokenID, $imagePOST);
testimage_deleteGET($tokenID, $imagePOST);

testlist_asset_valid($tokenID);
testlist_asset_invalid($tokenID);

testlist_user_valid($tokenID);
testlist_user_invalid($tokenID);
testlogin_logout($tokenID);

$tokenID = testlogin_login($user, $password);
testlogin_remove($user, $password, $tokenID);
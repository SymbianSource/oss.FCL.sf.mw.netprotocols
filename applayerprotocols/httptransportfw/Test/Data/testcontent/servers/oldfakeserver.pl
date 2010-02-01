#!/usr/bin/perl
# Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
# All rights reserved.
# This component and the accompanying materials are made available
# under the terms of "Eclipse Public License v1.0"
# which accompanies this distribution, and is available
# at the URL "http://www.eclipse.org/legal/epl-v10.html".
#
# Initial Contributors:
# Nokia Corporation - initial contribution.
#
# Contributors:
#
# Description:
#

$count= 0;
# print "[Server $0 accepting clients]\n";
# if ($client = $server->accept()) {
#   $client->autoflush(1);
#   $hostinfo = gethostbyaddr($client->peeraddr);
#   printf "Connection\n";
#   printf "[Connect from %s]\n", $hostinfo->name || $client->peerhost;
open (LOGFILE, ">>/var/log/fakeserver");
print LOGFILE "**********************************************************\n";
print LOGFILE "Accepting connection\n";
print LOGFILE "**********************************************************\n";
$firstline=<STDIN>;
$firstline =~ /[A-Za-z]+\s(.*)\sHTTP.*/i;
print LOGFILE $firstline;
$path=$1;
$path =~ /.*\?req=([^&]+)&resp=(.*)/;
$request=$1;
$response=$2;
$savereq = 0;
$reqopen=0;
$respopen=0;
$finished = 0;
if ($path =~ /.*SAVEREQUEST.*/) {
    $savereq = 1;
    open (SAVEREQ, ">$request");
} 
else {
    open (REQUEST , "<$request") or $reqopen = 1;
#       open (SAVEREQ, ">/tmp/fakeserver_save_$count");# or print "Savereq didn't open\n";
    $count++;
    $our_line = readline (*REQUEST);
    if ($our_line ne $firstline) {
	print LOGFILE "**********************************************************\n";
	print LOGFILE "Error: Wrong input at first line\n";
	print LOGFILE "Got $firstline\n";
	print LOGFILE "Expected $our_line\n";
	print LOGFILE "**********************************************************\n";
	print <<EOD;
HTTP/1.0 404 Not Found
Content-Type: text/plain

The document did not match the expected response
received $firstline
expected $our_line
EOD
    exit;
       }
   };
open (RESPONSE , "<$response") or $respopen = 1;
if ($respopen == 1 || ($reqopen == 1 && $savereq == 0)) {
    print <<EOD;
HTTP/1.0 404 Not Found
Content-Type: text/plain

One of the input files was not found.
Sorry.
EOD
   }
   else {
       while (<STDIN>) {
#   while ( ! $finished) {
#       $line = <$client>;
	   $line=$_;
	   print LOGFILE $line;
#	   print SAVEREQ;
	   if (! $savereq) {
	       $our_line = readline (*REQUEST);
	       if ($our_line ne $line) {
print LOGFILE "**********************************************************\n";
		   print LOGFILE "Error: Wrong input\n";
		   print LOGFILE "Expected $our_line\n";
		   print LOGFILE "Received $line\n";
print LOGFILE "**********************************************************\n";
		   print <<EOD;
HTTP/1.0 404 Not Found
Content-Type: text/plain

The document did not match the expected response
received $line
expected $our_line
EOD
		   last;
	       }
	       if (eof (*REQUEST)) {
		   print LOGFILE "**********************************************************\n";
		   while (<RESPONSE>) {
		       print "$_";
		       print LOGFILE "$_";
		   }
		   last;
		   $finished = 1;
	       }
	   }
       }
   close SAVEREQ;
   close REQUEST;
   close RESPONSE;
 }


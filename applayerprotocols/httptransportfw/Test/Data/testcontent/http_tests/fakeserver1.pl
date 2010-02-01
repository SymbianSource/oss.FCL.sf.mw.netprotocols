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

 use IO::Socket;
 use Net::hostent;		# for OO version of gethostbyaddr
 $PORT = 9000;			# pick something not in use
 $server = IO::Socket::INET->new( Proto     => 'tcp',
                                  LocalPort => $PORT,
                                  Listen    => SOMAXCONN,
                                  Reuse     => 1);
 die "can't setup server" unless $server;
$count= 0;
 print "[Server $0 accepting clients]\n";
 while ($client = $server->accept()) {
   $client->autoflush(1);
   $hostinfo = gethostbyaddr($client->peeraddr);
   printf "[Connect from %s]\n", $hostinfo->name || $client->peerhost;
   $firstline=<$client>;
   $firstline =~ /[A-Za-z]+\s(.*)\sHTTP.*/i;
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
       open (SAVEREQ, ">/tmp/fakeserver_save_$count") or print "Savereq didn't open\n";
       $count++;
       $our_line = readline (*REQUEST);
       if ($our_line ne $firstline) {
	   print "Error: Wrong input at first line\n";
	   print $client <<EOD;
HTTP/1.0 404 Not Found
Content-Type: text/plain

The document did not match the expected response
received $firstline
expected $our_line
EOD
       	   close $client;
       }
   };
   open (RESPONSE , "<$response") or $respopen = 1;
   if ($respopen == 1 || ($reqopen == 1 && $savereq == 0)) {
       print $client <<EOD;
HTTP/1.0 404 Not Found
Content-Type: text/plain

One of the input files was not found.
Sorry.
EOD
   close $client;
   }
   else {
       while (<$client>) {
#   while ( ! $finished) {
#       $line = <$client>;
	   $line=$_;
	   print SAVEREQ;
	   if (! $savereq) {
	       $our_line = readline (*REQUEST);
	       if ($our_line ne $line) {
		   print "Error: Wrong input\n";
		   print $client <<EOD;
HTTP/1.0 404 Not Found
Content-Type: text/plain

The document did not match the expected response
received $line
expected $our_line
EOD
    		   close $client;
		   last;
	       }
	       if (eof (*REQUEST)) {
		   while (<RESPONSE>) {
		       print $client "$_";
		   }
		   close $client;
		   last;
		   $finished = 1;
	       }
	   }
       }
   }
   close SAVEREQ;
   close REQUEST;
   close RESPONSE;
 }

#!/usr/bin/perl -w
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
 print "[Server $0 accepting clients]\n";
 while ($client = $server->accept()) {
   $client->autoflush(1);
   print $client "Welcome to $0; type help for command list.\n";
   $hostinfo = gethostbyaddr($client->peeraddr);
   printf "[Connect from %s]\n", $hostinfo->name || $client->peerhost;
   print $client "Command? ";
   while ( <$client>) {
     next unless /\S/;	     # blank line
     if    (/quit|exit/i)    { last;                                     }
     elsif (/date|time/i)    { printf $client "%s\n", scalar localtime;  }
     elsif (/who/i )         { print  $client `who 2>&1`;                }
     elsif (/cookie/i )      { print  $client `/usr/games/fortune 2>&1`; }
     elsif (/motd/i )        { print  $client `cat /etc/motd 2>&1`;      }
     else {
       print $client "Commands: quit date who cookie motd\n";
     }
   } continue {
      print $client "Command? ";
   }
   close $client;
 }

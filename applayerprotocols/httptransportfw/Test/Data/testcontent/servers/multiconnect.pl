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
# Multiconnect - A test server to test multiple HTTP connections.
# This runs from inetd and is incredibly nieve; it totaly ignores
# all aspects of the request.
# 
#

open (LOGFILE, ">>/var/log/multiconnect") or die "aargh";
print LOGFILE "**********************************************************\n";
print LOGFILE "Accepting connection\n";
print LOGFILE "**********************************************************\n";
$| = 1;
$count = 0;
while (<STDIN>) {
    $line=$_;
    print LOGFILE $line;
    if ( length $line == 2 ) {
	$count++;
	print LOGFILE "******************GOT REQUEST $count **********************\n";
	print <<EOD;
HTTP/1.1 200 OK\r
Content-Type: text/plain\r
Content-Length: 4\r
X-Transaction-Count: $count\r
\r
OK

EOD
};
}

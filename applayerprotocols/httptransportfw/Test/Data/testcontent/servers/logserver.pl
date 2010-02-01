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

open (LOGFILE, ">>/var/log/digesttest");
print LOGFILE "**********************************************************\n";
print LOGFILE "Accepting connection\n";
print LOGFILE "**********************************************************\n";
$| = 1;
$firstline=<STDIN>;
$auth = "";
$firstline =~ /.*digest.(.).*/i;
$test=$1;
if ($test < 1 || $test > 4) {
    print <<EOD;
HTTP/1.0 404 Digest test not found\r
Content-Type: text/plain\r
\r
The URI must include the text 'digext.x ' where x is between 1 and 3.\r
\r
EOD
    exit;
}
while (<STDIN>) {
    $line=$_;
    print LOGFILE $line;
    if ($line =~ /Authorization:(.*)/i) {
	$auth = $1;
	};

    if ( length $line == 2 ) {
	if ($auth eq "") {
	    # No auth header. Make the first response
	    if ($test == 1) {
		$wwwAuthenticate="WWW-Authenticate: Basic realm=\"somewhere\" Digest realm=\"Somewhere Else\", qop=\"well-dodgy,auth,auth-int\", nonce=\"Nonce\", opaque=\"This means nothing to you\", domain=\"/digest.1\"";
	    }
	    if ($test == 2) {
		$wwwAuthenticate="WWW-Authenticate: Basic realm=\"somewhere\" Digest realm=\"Somewhere Else\", qop=\"well-dodgy,auth,auth-int\", nonce=\"Nonce\", opaque=\"This means nothing to you\", domain=\"/digest.1\" Wacko parrot=dead, walk=silly";
	    }
	    if ($test == 3) {
		$wwwAuthenticate="WWW-Authenticate: Basic realm=\"somewhere\" Digest realm=\"Somewhere Else\", qop=\"well-dodgy,auth-int\", nonce=\"Nonce\", opaque=\"This means nothing to you\", domain=\"/digest.1\"";
	    }
	    if ($test == 4) {
		$wwwAuthenticate="WWW-Authenticate: Basic Digest realm=\"Somewhere Else\", qop=\"well-dodgy,auth-int\", nonce=\"Nonce\", opaque=\"This means nothing to you\", domain=\"/digest.1\" Wacko parrot=dead, walk=silly";
	    }
	    print <<EOD;
HTTP/1.1 401 Authorization Required\r
Content-Type: text/plain\r
$wwwAuthenticate\r
\r
You need to authenticate\r
\r
EOD
    exit;
	}
	$success = 0;
	if ($test == 1 && $auth =~ /.*Digest.*/ &&
	    $auth =~ /qop="auth"/ &&
	    $auth =~ /opaque="This means nothing to you"/) {
	    $success = 1;
	}
	if ($test == 2 && $auth =~ /.*Digest.*/ &&
	    $auth =~ /qop="auth"/ &&
	    $auth =~ /opaque="This means nothing to you"/) {
	    $success = 1;
	}
	if ($test == 3 && $auth =~ /.*Basic.*/) {
	    $success = 1;
	}
	if ($success == 1) {
	    
	print <<EOD;
HTTP/1.1 200 OK\r
Content-Type: text/plain\r
Content-Length: 4\r
X-Transaction-Count: $count\r
\r
OK

EOD
    exit;
    } else {
    
	print <<EOD;
HTTP/1.1 404 Test failed\r
Content-Type: text/plain\r
\r
The test has failed\r
Authorization header was $auth\r
This failed $match\r

EOD
    exit;
    }
};
}

#! C:\Perl\bin\perl.exe
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

use CGI;

@content = <>;

$content = join( '', @content );

$query = new CGI( $content );
$query_url = new CGI( $ENV{'QUERY_STRING'} );

$ENV{'PATH'} = '/bin:/usr/bin:/usr/local/bin';

print $query->header;
print $query->start_html( -title => "Echo page" );

if( $ENV{'QUERY_STRING'} ) {
  print "<h2>Values received within URL</h2>\n";
  print "<pre>\n";
  print "Field name           Field value\n";
  print "------------------------------------------------\n";
  foreach $p ($query_url->param) {
    print sprintf("%-20s %s\n", $p, $query_url->param($p) );
  }
  print "</pre>\n";
  print "<br>\n";
  print "<h3>Raw URL content</h3>\n";
  print "<pre>$ENV{'QUERY_STRING'}</pre>\n";
}


if( $content ) {
  print "<h2>Values received within ENTITY</h2>\n";
  print "<pre>\n";
  print "Field name           Field value\n";
  print "------------------------------------------------\n";
  foreach $p ($query->param) {
    print sprintf("%-20s %s\n", $p, $query->param($p) );
  }
  print "</pre>\n";
  print "<br>\n";
  print "<h3>Raw ENTITY content</h3>\n";
  print "<pre>$content</pre>\n";
}

if( (! $content) && (! $ENV{'QUERY_STRING'} ) ) {
  print "<h2>Nothing was received from browser!</h2>";
}

print $query->end_html;

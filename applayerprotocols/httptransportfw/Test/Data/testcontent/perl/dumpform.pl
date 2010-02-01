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

BEGIN { unshift( @INC, "/usr/local/httpd/htdocs/cgi-bin/" ); }

use CGI;
$query = new CGI;
$query->use_named_parameters(1);

print $query->header;
print $query->start_html(-title=>'POST dump (with CGI.pm library)',
                            -author=>'root@cellulardata.com');
#                         -BGCOLOR=>'#EEA9B8' );

print "<H1> POST lomakkeessa tulleet arvot: </H1>\n";

print $query->dump;
print $query->end_html;



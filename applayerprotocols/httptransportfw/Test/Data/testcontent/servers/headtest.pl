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
# This runs from inetd and is incredibly nieve; it totaly ignores
# all aspects of the request.
# 
#

print <<EOD;
HTTP/1.1 200 Head Succeeded\r
Content-Type: text/plain\r
Content-Length: 20000
\r
EOD

while (<STDIN>) {
    
}

#
# Copyright (c) 2009-2010 Nokia Corporation and/or its subsidiary(-ies).
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
#!/bin/sh

# GT 0102 HTTP Client
#
# Test case 7 - Content-Type: application/vnd.syncml-xml in response headers


# Output response headers first
echo 'Transfer-Encoding: chunked'
echo 'Content-Type: text/html; charset=iso-8859-1'
echo ''
echo '115'
echo '<!DOCTYPE HTML PUBLIC "-//IETF//DTD HTML 2.0//EN">'
echo '<HTML><HEAD>'
echo '<TITLE>404 Not Found</TITLE>'
echo '</HEAD><BODY>'
echo '<H1>Not Found</H1>'
echo 'The requested URL /sdfsdf was not found on this server.<P>'
echo '<HR>'
echo '<ADDRESS>Apache/1.3.12 Server at lon-paulha.INTRA Port 80</ADDRESS>'
echo '</BODY></HTML>'
echo '0'
echo ''


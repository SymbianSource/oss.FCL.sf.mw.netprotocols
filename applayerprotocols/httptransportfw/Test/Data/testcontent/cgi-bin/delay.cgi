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

# sh cgi-script to output a trivial file after a longish delay
# usage:  http://localhost/cgi-bin/delay.cgi?15 gives a 15 second delay.

# HTTP Headers first

echo 'Content-Type: text/html'
echo 'Pramga: no-cache'
echo ''

# HTTP Body next (in HTML)

echo '<HTML>'
echo '<HEAD>'
echo '<TITLE>'
echo 'Delay cgi-script for time-out tests'
echo '</TITLE>'
echo '</HEAD>'
echo '<BODY>'
echo '<H1>'
echo '<P> Going to sleep for '$QUERY_STRING' seconds </P>'
sleep $QUERY_STRING
echo '<P> finished</P>'
echo '</BODY>'
echo '</HTML>'

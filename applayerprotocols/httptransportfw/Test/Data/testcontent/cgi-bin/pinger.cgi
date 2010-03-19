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

# sh cgi-script to keep output coming from the server and repeat,
# effecting a kind of keepalive pinger

# HTTP Headers first

echo 'Content-Type: text/html'
echo 'Pragma: no-cache'
echo 'Refresh: 1'
echo ''

# HTTP Body next (in HTML)

echo '<HTML>'
echo '<HEAD>'
echo '<TITLE>'
echo 'Pinger cgi-script for keepalive tests'
echo '</TITLE>'
echo '</HEAD>'
echo '<BODY>'
echo '<H1>'
echo '<P> 1.... </P>'
sleep 1
echo '<P> 2.... </P>'
sleep 1
echo '<P> 3.... </P>'
sleep 1
echo '<P> 4.... </P>'
sleep 1
echo '<P> 5.... </P>'
sleep 1
echo '<P> 6.... </P>'
sleep 1
echo '<P> 7.... </P>'
sleep 1
echo '<P> 8.... </P>'
sleep 1
echo '<P> 9.... </P>'
sleep 1
echo '<P> 10.... </P>'
echo '</BODY>'
echo '</HTML>'

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

echo 'Content-Type: application/vnd.syncml-xml'
echo 'Content-Length: 173'
echo ''

# HTTP Body next (in plain-text, since I don't know what the real data format is)

echo 'Test case 7'
echo ''
echo 'Returns this text marked with the content-type application/vnd.syncml-xml'
echo 'Client code should be able to detect this header and validate it.'



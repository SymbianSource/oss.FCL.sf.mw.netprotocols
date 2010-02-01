// Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
//

#ifndef	__TESTDATA_H__
#define __TESTDATA_H__

_LIT8(KRequestData,"POST /wappush HTTP/1.1\r\nHost:\r\nDate: Tue, 31 Jul 2001 10:13:05 GMT\r\nContent-Type: application/http\r\nContent-Length: 504\r\nX-Wap-Push-OTA-Version: 1.0\r\n\r\nHTTP/1.1 200 OK\r\nDate: Tue, 31 Jul 2001 10:13:00 GMT\r\nLast-modified: Tue, 31 Jul 2001 10:13:00 GMT\r\nContent-Language: en\r\nContent-Length: 268\r\nContent-Type: text/vnd.wap.si\r\nX-Wap-Application-Id: x-wap-application:wml.ua\r\n\r\n<?xml version=\"1.0\"?><!DOCTYPE si PUBLIC \"-//WAPFORUM//DTD SI 1.0//EN\" \"http://www.wapforum.org/DTD/si.dtd\"><si><indication href=\"http://www.xyz.com/email/123/abc.wml\"created=\"2001-07-31T10:13:00Z\"si-expires=\"2001-08-07T10:13:00Z\">You have 4 new emails</indication></si>");

_LIT8(KResponseData,"HTTP/1.1 204 No Content\r\nX-Wap-Push-Status: 400 Accepted");

_LIT8(KSecureRequestData, "GET / HTTP/1.1\r\nHost: waptest2.intra\r\n\r\n");

_LIT8(KSecureResponseData, "HTTP/1.1 200 OK");

#endif	// __TESTDATA_H__

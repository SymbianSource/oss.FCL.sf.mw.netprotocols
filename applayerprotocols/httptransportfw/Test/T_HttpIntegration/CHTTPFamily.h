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
// $Header$
// CHTTPFamilys.h
// The header file introducing the collection of HTTP command classes
// of the Integration Harness framework.
// by:	mjdavey, mjd systems solutions ltd. www.mjdss.com
// on:	aug 2002,
// 
//

#ifndef __CHTTPFAMILY_H__
#define __CHTTPFAMILY_H__

//-----------------------------------------------------------------------------

#include "MWspEventCallback.h"

#include "TEngine.h"

//----------------------------------------------------------------------------
//	specifics to this module

#include <http.h>
#include <etel.h>
#include <es_sock.h>
#include <http/mhttpauthenticationcallback.h>
#include <http/mhttpsessioneventcallback.h>

//	add initial event type values (must not==THTTPEventType)
const TInt KEventAny = 10000;

//	general timeout value
const TInt KEventTimeoutValue = 1000;

//	content type buffer size
const TInt KMaxContentTypeSize = 64;
const TInt KMaxContentLengthSize = 8;
const TInt KMaxAcceptLengthSize = KMaxContentTypeSize;
const TInt KMaxAcceptCharSetSize = 32;
const TInt KMaxAcceptEncodingSize = 24;
const TInt KMaxUserAgentSize = 24;

//----------------------------------------------------------------------------
//	HTTP Integration Command identifiers. 

enum THA_HTTP_INTEGRATION
	{
	THA_KHTTP_Connect = 100,
	THA_KHTTP_Disconnect,
	THA_KHTTP_Transaction,
	THA_KHTTP_EndTransaction,
	THA_KHTTP_ShowTransaction,

	//	add new identifers above this line
	THA_KHTTPMax
	};

//-----------------------------------------------------------------------------
// Framework command phrases

_LIT(THA_KErrInvalidConnect,		"Error: command format is CONNECT <framework> <name> [%S]\r\n");

_LIT(THA_TxtCmdConnect,					"CONNECT");
_LIT(THA_TxtCmdConnectHelp,			"CONNECT <Framework> <Name>. Framework=HTTP/WSP");
_LIT(THA_KErrCnxionExists,			"Error: Name [%S] is already defined for Connection or Transaction\r\n");
_LIT(THA_KErrConnectionNotExists,	"Error: connection [%S] is not defined or active\r\n");
_LIT(THA_KErrMethodNotExists,		"Error: transaction method [%S] is not defined or recognized\r\n");
_LIT(THA_KErrNoContentTypeDefined,	"Error: No Content Type specified in ContentType define\r\n");
	
_LIT(THA_KTxtFrameworkHTTP,			"HTTP" );
_LIT(THA_KTxtFrameworkWSP,			"WSP" );
_LIT(THA_KErrFrameworkErr,			"Error: framework [%S] is not recognized or supported. Use HTTP or WSP.\r\n");
_LIT(THA_KErrConnectionErr,			"Error: connection [%S] is not defined or found\r\n");

_LIT(THA_KErrInvalidTransaction,	"Error: transaction [%S] is not defined\r\n");
_LIT(THA_TxtCmdDisconnect,			"DISCONNECT");
_LIT(THA_TxtCmdDisconnectHelp,	"DISCONNECT <Name>. Close session named <Name>");

_LIT(THA_TxtCmdTransaction,			"TRANSACTION");
_LIT(THA_TxtCmdTransactionHelp,	"TRANSACTION <trans name> <sess name> <method> <uri>");

_LIT(THA_TxtCmdEndTrans,			"ENDTRANS");
_LIT(THA_TxtCmdEndTransHelp,		"ENDTRANS <trans name>. Closes transaction named <trans name>");

_LIT(THA_TxtCmdShowTrans,			"SHOWTRANS");
_LIT(THA_TxtCmdShowTransHelp,		"SHOWTRANS.  Displays all open transactions.");

_LIT(THA_TxtCmdTransactionGET,		"GET");
_LIT(THA_TxtCmdTransactionPOST,		"POST");
_LIT(THA_TxtCmdTransactionPUT,		"PUT");
_LIT(THA_TxtCmdTransactionHEAD,		"HEAD");
_LIT(THA_TxtCmdTransactionTRACE,	"TRACE");
_LIT(THA_TxtCmdTransactionOPTIONS,	"OPTIONS");
_LIT(THA_TxtCmdTransactionDELETE,	"DELETE");

//	default standard headers...

_LIT8(KUserAgent, "HTTPIntegrationTest (1.0)");
_LIT8(KAccept, "*/*");


//-----------------------------------------------------------------------------

#endif  // __CHTTPFAMILY_H__

//-----------------------------------------------------------------------------
// End of File
//-----------------------------------------------------------------------------


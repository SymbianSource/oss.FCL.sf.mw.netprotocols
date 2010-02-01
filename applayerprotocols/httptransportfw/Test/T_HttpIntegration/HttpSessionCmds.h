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
// The header file introducing the collection of HTTP command classes
// of the Integration Harness framework.
// 
//

#ifndef __HTTPSESSIONCMDS_H__
#define __HTTPSESSIONCMDS_H__

//-----------------------------------------------------------------------------

#include "MWspEventCallback.h"

#include "GT0149.h"
#include "TEngine.h"
#include "CCmdFamily.h"
#include "CCmdBase.h"
#include "CObjCatalogue.h"
#include "CTimeoutTimer.h"
#include "CWspEventDispatcher.h"

//----------------------------------------------------------------------------
//	specifics to this module


#include <http.h>
#include <etel.h>
#include <e32base.h>
#include <es_sock.h>
#include <http/mhttpauthenticationcallback.h>
#include <http/mhttpsessioneventcallback.h>

class CCmdConnect : public CCmdBase, public MWspEventCallback
{
	friend class CCmdDisconnect;
	friend class CCmdTransaction;

public:

	static CCmdConnect *NewL (TInt, const TDesC& aKeyphrase, const TDesC& aHelpPhrase);
	static CCmdConnect *NewLC(TInt, const TDesC& aKeyphrase, const TDesC& aHelpPhrase);
	virtual ~CCmdConnect();

	void DisconnectL(const TInt &aIndex);
	void DisconnectL(const TDesC& aLabel);
	void Delete(const TInt &aIndex);
	void Delete(const TDesC& aLabel);
		

protected:	//	data types
	
	enum THTTPReceivedType
		{
		EHTTPReceived,
		EHTTPUnexpected,
		EHTTPNotReceived,
		EMaxReceivedEventType
		};

protected:  // from CCmdBase

	void ConstructL( TInt aCommandId, const TDesC& aKeyphrase, const TDesC& aHelpPhrase);
	CCmdConnect() {};		//	ensure a concrete class!

//	void DoneWaitEvents();
	void SetWspProxyPropertiesL(RHTTPSession & aSession, RStringPool &aStrPool) ;
	void SetWspCapabilityPropertiesL( RHTTPSession & aSession, RStringPool &aStrPool) ;

public:     // from CCmdBase
	TInt  ProcessL (const TDesC& aCommand);

private:
	CWspEventDispatcher *iEventDispatcher;
	void HandleWspEvent(TRequestStatus aStatus);
	// Get command parameters = the remainder after the Keyword Phrase
	// in the command string. Leaves (KErrArgument) if cannot recogize
	// the command.
	TInt ParseCmdArgs( const TDesC& aCommand, TPtrC& aFramework, TPtrC& aConName );
	void ShowSessions();
	void WaitForNextEvent(TInt);

	RHTTPSession *iSession;

	THTTPReceivedType iReceived;

	TInt iMaxEvents;
	TInt iEventCount;
	TInt iEventRequired;

	TBool iNowaitGiven;        // ETrue if command contains NOWAIT option
	TBool iNbrGiven;           // ETrue if command contains number option
	TInt  iNbrValue;           // value of the number option
};

//-----------------------------------------------------------------------------
//	Command:	The 'Disconnect' command which closes a named session

class CCmdDisconnect :  public CCmdBase
{
public:

	static CCmdDisconnect *NewL ( TInt, const TDesC& aKeyphrase, const TDesC& aHelpPhrase);
	static CCmdDisconnect *NewLC( TInt, const TDesC& aKeyphrase, const TDesC& aHelpPhrase);
	virtual ~CCmdDisconnect();

private:
	CWspEventDispatcher *iEventDispatcher;
	enum TReceivedType
        {
        EReceived,
        EUnexpected,
        ENotReceived
        };
	TReceivedType iReceived;

	TInt iMaxEvents;
	TInt iEventCount;

	TBool iNowaitGiven;        // ETrue if command contains NOWAIT option
	TBool iNbrGiven;           // ETrue if command contains number option
	TInt  iNbrValue;           // value of the number option


protected:  // from CCmdBase

	void ConstructL( TInt aCommandId, const TDesC& aKeyphrase, const TDesC& aHelpPhrase);
//	void DoneWaitEvents();
	TInt iEventRequired;

public:     // from CCmdBase
	TInt  ProcessL ( const TDesC& aCommand );

private:
	TInt ParseCmdArgs( const TDesC& aCommand, TPtrC& aConName );
	void WaitForNextEvent(TInt);
};

//-----------------------------------------------------------------------------

#endif  // __HTTPSESSIONCMDS_H__

//-----------------------------------------------------------------------------
// End of File
//-----------------------------------------------------------------------------


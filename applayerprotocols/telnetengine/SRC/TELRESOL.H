/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:
*
*/


#ifndef _TELRESOL_H_
#define _TELRESOL_H_

#include <telsess.h>

class MTelnetResolver
/**
@internalComponent
*/
	{
 public:
	virtual void ResolverConnectedL() = 0;
	virtual void ResolverDisconnected() = 0;
	virtual void ResolverError(TInt aError) = 0;
	};

NONSHARABLE_CLASS(CTelnetResolver) : public CActive
/**
@internalComponent
*/
	{
 public:
	enum TResolverState 
        {
		EDisconnected,ELookingUp,EConnecting,EConnected,EShuttingDown,EDisconnecting,EErrorState
        };
	enum TEvent
        {
		ENone,EDoDisconnect,EEofDetected,EErrorEvent
        };
 public:
	static CTelnetResolver* NewL(MTelnetResolver* aNotifier);
	void ConstructL(MTelnetResolver* aNotifier);

	TInt	 State(){return(iState);}
	RSocket* Socket(){return(&iSocket);}
	
	TInt IssueConnect(const TDesC& aServerName, TUint aPort);
	TInt IssueConnect(const TInetAddr& aInetAddr, TUint aPort);
	TInt IssueDisconnect();
	void HandleEof();

	void DoCancel();
	void RunL();
	void Event();

	CTelnetResolver();
	~CTelnetResolver();

	void TriggerActive(const TEvent aEvent);

 private:
	RSocketServ iSocketServ;
	RHostResolver iResolver;
	RSocket iSocket;

	TInetAddr iHostAddr;
	TNameEntry iNameEntry;
	TNameRecord iNameRecord;
	TBuf<256> iServerName;

	TResolverState iState;
	TEvent iEvent;

	MTelnetResolver* iNotify;
	};

#endif

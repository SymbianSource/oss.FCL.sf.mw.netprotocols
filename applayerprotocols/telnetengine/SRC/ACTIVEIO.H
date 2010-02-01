/**
* Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* CActiveReader & CActiveWriter
* Contains basic Read and Write class Active objects for Socket I/O
* 
*
*/



/**
 @file Activeio.h
 @internalComponent
*/

#ifndef __ACTIVE_IO_H
#define __ACTIVE_IO_H

#include <telsess.h>

#define _TELNET_IOCTL_RECEIVE

class MIONotifier;
class MTelnetNotification;

NONSHARABLE_CLASS(CActiveWriter) : public CActive
/**
@internalComponent
*/
	{
public:
	CActiveWriter();
	~CActiveWriter();

	static CActiveWriter* NewL(MIONotifier* aNotifier);
	void ConstructL(MIONotifier* aNotifier);

	TInt IssueUrgentWrite(const TDesC8& aBuffer);
	TInt IssueWrite(const TDesC8& aBuffer);
	void SetSocket(RSocket* aSocket);

	void RunL();
	void DoCancel(){}

private:
	RSocket*			iSocket;
	MIONotifier*		iNotifier;
	TSockXfrLength		iXfrLength;
	};



NONSHARABLE_CLASS(CActiveReader) : public CActive
/**
@internalComponent
*/
	{

public:
	CActiveReader();
	~CActiveReader();

	static CActiveReader* NewL(MIONotifier* aNotifier);
	void ConstructL(MIONotifier* aNotifier);

	TInt IssueRead(TDes8& aBuffer);
	void SetSocket(RSocket* aSocket);

	void RunL();
	void DoCancel(){}
	virtual TInt RunError(TInt aError);

private:

#ifdef _TELNET_IOCTL_RECEIVE

	enum TRequestCodes
		{
		ERequestNone,
		EReadRequest,
		EIOctlRequest
		};

	TPckgBuf<TUint>		iflags;
	TRequestCodes		iRequest;
	TDes8*				iClientBuffer;

#endif

	RSocket*			iSocket;
	MIONotifier*		iNotifier;
	TSockXfrLength		iXfrLength;
	};

#endif

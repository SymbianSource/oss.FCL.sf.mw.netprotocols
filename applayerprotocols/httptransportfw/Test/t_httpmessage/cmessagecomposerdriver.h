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

#ifndef __CMESSAGECOMPOSERDRIVER_H__
#define __CMESSAGECOMPOSERDRIVER_H__

#include <e32base.h>
#include <rhttpmessagecomposer.h>
#include <mhttpmessagecomposerobserver.h>

#include "mbodydatasupplierobserver.h" 

class MDriverObserver;
class CBodyDataSupplier;

class CMessageComposerDriver : public CActive,
							   public MHttpMessageComposerObserver,
							   public MBodyDataSupplierObserver
	{
public:	// methods

	static CMessageComposerDriver* NewL(MDriverObserver& aObserver);

	virtual ~CMessageComposerDriver();

	void SetMessageData(const TDesC8& aMessageData);
	void SetStartLine(const TDesC8& aToken1, const TDesC8& aToken2, const TDesC8& aToken3);
	void SetHeaderL(const TDesC8& aFieldName, const TDesC8& aFieldValue);
	void SetBodyDataL(const TDesC8& aBodyPart);
	void SetBodySize(TInt aSize);
	void SetTrailerL(const TDesC8& aFieldName, const TDesC8& aFieldValue);

	void Start(TInt aExpectedError, TBool aTestReset = EFalse);

private:	// methods from CActive

	virtual void RunL();
	virtual void DoCancel();
	virtual TInt RunError(TInt aError);

private:	// methods from MHttpMessageComposerObserver

	virtual void StartLineL(TPtrC8& aToken1, TPtrC8& aToken2, TPtrC8& aToken3);
	virtual TInt NextHeaderL(TPtrC8& aHeaderName, TPtrC8& aHeaderValue);
	virtual MHTTPDataSupplier* HasBodyL();
	virtual TInt NextTrailerL(TPtrC8& aHeaderName, TPtrC8& aHeaderValue);
	virtual void MessageComplete();
	virtual void MessageDataReadyL();

	virtual TInt HandleComposeError(TInt aError);

	virtual void Reserved_MHttpMessageComposerObserver();

private:	// methods from MBodyDataSupplierObserver

	virtual void NotifyMoreData();

private:	// enums

	enum TMessageState
		{
		EPendingStartLine			= 0,
		EPendingNextHeader,
		EPendingHasBody,
		EPendingNextTrailer,
		EPendingMessageComplete,
		EDone
		};

private:	// helper classes

	class THeaderField
		{
	public:	// methods

		THeaderField(const TDesC8& aName, const TDesC8& aValue) : iName(aName), iValue(aValue) {};

	public:	// attributes

		TPtrC8		iName;
		TPtrC8		iValue;

		};

private:	// methods

	CMessageComposerDriver(MDriverObserver& aObserver);
	void ConstructL();

	void CompleteSelf();
	void DoReset();

private:

	MDriverObserver&		iObserver;
	RHttpMessageComposer	iMessageComposer;
	TMessageState			iState;

	TPtrC8					iMessageData;
	TPtrC8					iExpectedData;
	TPtrC8					iStartLine1;
	TPtrC8					iStartLine2;
	TPtrC8					iStartLine3;

	RArray<THeaderField>	iHeaders;
	TInt					iHeaderIndex;

	RArray<THeaderField>	iTrailers;
	TInt					iTrailerIndex;
	CBodyDataSupplier*		iBodyData;

	TInt					iExpectedError;
	TBool					iTestFailed;

	TInt					iResetIndex;
	TBool					iDoReset;
	TBool					iReset;
	TMessageState			iResetState;

	TInt					iChunkCount;
	TInt					iChunkIndex;

	TInt					iMessageChunk;
	TInt					iMessageIndex;

	};

#endif	// __CMESSAGECOMPOSERDRIVER_H__

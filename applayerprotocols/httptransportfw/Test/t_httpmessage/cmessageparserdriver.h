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

#ifndef __CMESSAGEPARSERDRIVER_H__
#define __CMESSAGEPARSERDRIVER_H__

#include <e32base.h>
#include <rhttpmessageparser.h>
#include <mhttpmessageparserobserver.h>

#include "mdatasupplierobserver.h"

class CMessageDataSupplier;
class MDriverObserver;

class CMessageParserDriver : public CActive,
							 public MHttpMessageParserObserver,
							 public MDataSupplierObserver
	{
public:	// methods

	static CMessageParserDriver* NewL(MDriverObserver& aObserver);

	virtual ~CMessageParserDriver();

	void SetMessageData(const TDesC8& aMessageData);
	void SetStartLine(const TDesC8& aStartLine);
	void SetHeaderL(const TDesC8& aFieldName, const TDesC8& aFieldValue);
	void SetBodyData(const TDesC8& aBodyData, TBool aIsChunked, TBool aIsUnknownLength = EFalse);
	void SetTrailerL(const TDesC8& aFieldName, const TDesC8& aFieldValue);

	void Start(TInt aExpectedError, TBool aTestReset = EFalse);

private:	// methods from CActive

	virtual void RunL();
	virtual void DoCancel();
	virtual TInt RunError(TInt aError);

private:	// methods from MHttpMessageParserObserver

	virtual void GetDataPacket(TPtrC8& aData);
	virtual void ReleaseDataPacket();

	virtual void StartLineL(const TDesC8& aStartLine);
	virtual void HeaderL(const TDesC8& aFieldName, TDesC8& aFieldValue);
	virtual TInt BodySizeL();
	virtual void BodyChunkL(const TDesC8& aData);
	virtual void BodyCompleteL();
	virtual void MessageCompleteL(const TPtrC8& aExcessData);

	virtual TInt HandleParserError(TInt aError);

	virtual void Reserved_MHttpMessageParserObserver();

private:	// methods from MDataSupplierObserver

	virtual void DataReady();

private:	// enums

	enum TMessageState
		{
		EPendingStartLine			= 0,
		EPendingHeaders,
		EPendingBodySize,
		EPendingBody,
		EPendingBodyComplete,
		EPendingTrailers,
		EPendingMessageComplete,
		EDone
		};

	enum TEntityBodyState
		{
		ENoEntityBody				= 0,
		ENonEncodedBody,
		EChunkEncodedBody,
		EUnknownBodyLength
		};

private:	// helper classes

	class THeaderField
		{
	public:	// methods
		
		THeaderField() : iName(0, NULL), iValue(0, NULL) {}
		THeaderField(const TDesC8& aName, const TDesC8& aValue) : iName(aName), iValue(aValue) {};

		THeaderField& operator=(const THeaderField& aField) { this->iName.Set(aField.iName); this->iValue.Set(aField.iValue); return *this; }

	public:	// attributes

		TPtrC8		iName;
		TPtrC8		iValue;

		};

private:	// methods

	CMessageParserDriver(MDriverObserver& aObserver);

	void ConstructL();

	void CompleteSelf();
	void DoReset();

private:

	MDriverObserver&		iObserver;
	RHttpMessageParser		iMessageParser;
	CMessageDataSupplier*	iDataSupplier;
	TMessageState			iState;
	TPtrC8					iMessageData;
	TPtrC8					iStartLine;
	TPtrC8					iEntityBody;
	TPtrC8					iExpectBodyData;
	TEntityBodyState		iExpectBodyState;

	RArray<THeaderField>	iHeaders;
	TInt					iHeaderIndex;
	RArray<THeaderField>	iTrailers;
	TInt					iTrailerIndex;
	
	TInt					iExpectedError;
	TInt					iBufferSize;
	TBool					iTestFailed;

	TInt					iResetIndex;
	TInt					iBodyIndex;
	TBool					iDoReset;
	TBool					iReset;
	TMessageState			iResetState;

	TInt					iChunkCount;
	TInt					iChunkIndex;

	TBool					iIsUnknownLength;
	};

#endif	// __CMESSAGEPARSERDRIVER_H__

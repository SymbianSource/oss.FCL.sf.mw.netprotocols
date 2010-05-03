// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __CHTTPRESPONSEPARSER_H__
#define __CHTTPRESPONSEPARSER_H__

#include <e32base.h>
#include <http/framework/crxdata.h>
#include <http/framework/logging.h>

#include "mhttpmessageparserobserver.h"
#include "rhttpmessageparser.h"
#include "mhttpresponse.h"
#include "timerlogger.h"

class MHttpResponseObserver;

class CHttpResponseParser : public CRxData,
							public MHttpResponse,
							public MHttpMessageParserObserver
	{
public:

	static CHttpResponseParser* NewL(CProtTransaction& aProtTrans, MRxDataObserver& aObserver, MHttpResponseObserver& aResponseObserver);
	virtual ~CHttpResponseParser();

	TInt CancelTransaction(TInt aError);
	void ConvertToNotifyOnDisconnectError ( TInt& aError );

public:	// methods from CRxData

	virtual void ResetRxData();
	
private:	// methods from MHTTPDataSupplier via CRxData

	virtual TBool GetNextDataPart(TPtrC8& aDataPart);
	virtual void ReleaseData();
	virtual TInt OverallDataSize();
	virtual TInt Reset();
	
private:	// methods from MHttpResponse

	virtual void ResponseDataReceived(const TDesC8& aData);
	virtual void CancelResponse();
	virtual void ConnectionError(TInt aError);	
	virtual TBool ResponseCompleted ();

	virtual TBool NeedCompletion ();	
	virtual TBool CompleteResponse ( const TDesC8& aData );
	virtual void FailTransactionL();
	virtual void OnResponseReceiveTimeOut ();
	virtual TInt ReceiveTimeOutValue ();
	TBool ResponseInformational ();
	virtual void FlushBodyDataIfNotRead();

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

private:

	CHttpResponseParser(CProtTransaction& aProtTrans, MRxDataObserver& aObserver, MHttpResponseObserver& aResponseObserver);
	void ConstructL();

	void NotifyObserverError(TInt aError);

	TBool CheckForNonPersistentConnection();

	inline TBool ConsumingResponse();
	inline TBool BodyComplete();
	inline TBool MessageComplete();
	inline TBool GotTrailers();
	inline TBool NotifyReleaseData();
    inline TBool BodyPresent();
    
private:

	enum THttpResponseFlags
		{
		EConsumingResponse		= 0x01,
		EBodyComplete			= 0x02,
		EMessageComplete		= 0x04,
		EGotTrailers			= 0x08,
	    ENotifyReleaseData		= 0x10,
		EBodyPresent			= 0x20

		};

private:
	TBool				iCancellingResponse;	
	TInt				iOverallDataSize;
	TInt				iFlags;
	const TStringTable&    iStringTable;
	MHttpResponseObserver&	iResponseObserver;
	RHttpMessageParser		iMessageParser;
	TPtrC8				iRawData;
	
	RArray<TPtrC8>		iBodyParts;

	
	__DECLARE_PERFORMANCE_LOG
public:

	__FLOG_DECLARATION_MEMBER2

	};

inline TBool CHttpResponseParser::ConsumingResponse()
	{
	return iFlags & EConsumingResponse;
	}

inline TBool CHttpResponseParser::BodyComplete()
	{
	return iFlags & EBodyComplete;
	}

inline TBool CHttpResponseParser::MessageComplete()
	{
	return iFlags & EMessageComplete;
	}

inline TBool CHttpResponseParser::GotTrailers()
	{
	return iFlags & EGotTrailers;
	}

inline TBool CHttpResponseParser::NotifyReleaseData()
	{
	return iFlags & ENotifyReleaseData;
	}
	
	inline TBool CHttpResponseParser::BodyPresent()
	{
	return iFlags & EBodyPresent;
	}


#endif	// __CHTTPRESPONSEPARSER_H__

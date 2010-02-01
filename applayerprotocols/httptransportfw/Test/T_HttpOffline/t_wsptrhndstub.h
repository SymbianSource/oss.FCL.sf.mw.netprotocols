// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __T_WSPTRHNDSTUB_H__
#define __T_WSPTRHNDSTUB_H__


// System includes
//
#include <wsp/mwspcapabilityprovider.h>
#include <wsp/mwspproxyinfoprovider.h>
#include <wsp/mwspsessionheadersprovider.h>
#include <wsp/mwspcosessioncallback.h>
#include <wsp/mwspcomethodcallback.h>

// Local includes
//
#include "HttpTestCore.h"

// Forward class declarations
//
class CWspTransportHandler;
class CWspTrHndTestDriver;

// Definition of class 'CWspTrHndStubTest'.  This test instantiates a WSP Transport Handler,
// via ECOM, to obtain the stub version.
class CWspTrHndStubTest : public CHttpTestBase, public MWspCapabilityProvider,
						  public MWspProxyInfoProvider, public MWspSessionHeadersProvider
	{
public: // methods

	static CWspTrHndStubTest* NewLC();
	virtual ~CWspTrHndStubTest();
	void DoTestsL();
	virtual void DoRunL();
	virtual const TDesC& TestName();
	virtual void DoCancel();
	void Done();

public: // methods inherited from MWspCapabilityProvider

	virtual const MWspCapabilityViewer& ClientCapabilities() const;
	virtual MWspCapabilitySetter& ServerCapabilities() const;

public: // methods inherited from MWspProxyInfoProvider

	virtual const TDesC8& ProxyAddress() const;
	virtual TUint16 LocalPort() const;
	virtual TUint16 RemotePort() const;
	virtual TWspBearer Bearer() const;
	virtual const TDesC8& ServiceCentreNumber() const;
	virtual TWspSession WspSessionService() const;
	virtual TBool SecureConnection() const;

public: // methods inherited from MWspSessionHeadersProvider

	virtual const TDesC8& ClientHeaders() const;
	virtual void SetServerHeadersL(const TDesC8& aBuffer);

private: // methods

	CWspTrHndStubTest();
	void ConstructL();

private: // attributes

	RStringPool iStringPool;
	CWspTransportHandler* iTrHnd;
	CWspTrHndTestDriver* iDriver;
	};


class CWspTrHndTestDriver : public CActive, public MWspCOSessionCallback, public MWspCOMethodCallback
	{
public: // methods

	static CWspTrHndTestDriver* NewL(CWspTrHndStubTest& aOwner, RStringPool aStringPool);
	virtual ~CWspTrHndTestDriver();
	void Start();
	void SetTransportHandler(CWspTransportHandler& aTrHnd);

public: // methods from CActive

	virtual void RunL();
	virtual void DoCancel();
	virtual TInt RunError(TInt aError);

public: // methods from MWspCOSessionCallback

	virtual void ConnectCnf();
	virtual void ResumeCnf();
	virtual void DisconnectInd(
							  TWspReason				aReasonCode, 
							  TBool						aRedirectSecurity, 
							  TWspRedirectedAddress&	aRedirectAddresses, 
							  const TDesC8&				aErrorHeader, 
							  const TDesC8&				aErrorBody
							  );

	virtual void SuspendInd(TWspReason aReason);
	virtual void ExceptionInd(const TDesC8& aExceptionData);

public: // methods from MWspCOSessionCallback

	virtual void MethodInvokeCnf();
	virtual void MethodInvokeDataCnf();
	virtual void MethodAbortInd(TWspReason aReason);
	virtual void MethodResultInd(
								TInt					aStatus, 
								const TDesC8&			aResponseHeaders, 
								MHTTPDataSupplier&		aResponseBody,
								TBool					aMoreData
								);


	virtual void MethodResultDataInd(const TDesC8& aTrailerHeaders, TBool aMoreData);

private: // embedded classes

/*
	class TBody : public MHTTPDataSupplier
		{
	public:
		TBody();
		virtual TBool GetNextDataPart(TPtrC8& aDataPart);
		virtual void ReleaseData();
		virtual TInt OverallDataSize();
		virtual TInt Reset();
	private:
		TInt iPartCount;
		};

	class THeader : public MHTTPDataSupplier
		{
	public:
		THeader();
		virtual TBool GetNextDataPart(TPtrC8& aDataPart);
		virtual void ReleaseData();
		virtual TInt OverallDataSize();
		virtual TInt Reset();
	private:
		TInt iPartCount;
		};
*/
private: // methods

	CWspTrHndTestDriver(CWspTrHndStubTest& aOwner, RStringPool aStringPool);
	void ConstructL();
	void CompleteSelf();
	void DoConnectReq();
	void DoMethodInvokeReq();
	void DoMethodInvokeDataReq();
	void DoDisconnectReq();

private: // type definitions

	enum TState
		{
		EInit,
		EConnecting,
		EConnected,
		EInvoking,
		EInvokingMore,
		EReplying,
		EReplyingMore,
		EReplyComplete,
		EDisconnecting,
		EDisconnected
		};

private: // attributes

	TState					iState;
	CWspTrHndStubTest&		iOwner;
	CWspTransportHandler*	iTrHnd;
	RStringPool				iStringPool;
	HBufC8*					iHeader;
	MHTTPDataSupplier*		iResponseBody;
	};

#endif // __T_WSPTRHNDSTUB_H__

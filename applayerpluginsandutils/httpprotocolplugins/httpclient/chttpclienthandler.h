// Copyright (c) 2003-2010 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __CHTTPCLIENTHANDLER_H__
#define __CHTTPCLIENTHANDLER_H__

#include <e32base.h>
#include <http/framework/cprotocolhandler.h>
#include <http/framework/mrxdataobserver.h>
#include <http/framework/logging.h>

#include "mconnectionprefsprovider.h"
#include "mhttpbatchingpropertiescallback.h"
#include "timerlogger.h"

class CHttpTransportLayer;
class CHttpConnectionInfo;
class CHttpConnectionManager;
class CX509Certificate;
class CX500DistinguishedName;
class TCertInfo;
class TDNInfo;
class CHttpPipelineFallback;

/**
 * Enumeration for NTLM authentication state.
 */
enum {
        ENtlmIdle,
        ENtlmNegotiate,
        ENtlmAuthenticate
    };

class CHttpClientHandler : public CProtocolHandler, 
						   public MConnectionPrefsProvider,
						   public MRxDataObserver,
						   public MHttpBatchingPropertiesCallback,
						   public MProtHandlerInterface

	{
public:

	static CHttpClientHandler* NewL(TAny* aSession);
	virtual ~CHttpClientHandler();

private:	// methods from CProtocolHandler
	virtual void MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent);
	virtual TInt SessionServerCert(TCertInfo& aServerCert);
	virtual TInt TransactionServerCert(TCertInfo& aServerCert, RHTTPTransaction aTransaction);
	virtual void CreateCodecL();
	virtual CProtTransaction* CreateProtTransactionL(RHTTPTransaction aTransaction) ;
	virtual TBool ServiceL(CProtTransaction& aTrans);
	virtual void ClosedTransactionHook(CProtTransaction* aTrans);
	virtual void CancelTransactionHook(CProtTransaction& aTransaction);
	virtual void NotifyNewRequestBodyPart(CProtTransaction& aTransaction);
	virtual void GetInterfaceL(TUid aInterfaceId, MProtHandlerInterface*& aInterfacePtr);
	
private:	// methods from MConnectionPrefsProvider

	virtual TBool SupplyCommsConnection( RConnection*& aConnectionPtr );
	virtual TBool SupplySocketServerHandle ( TInt& aSocketServerHandle );
	virtual void SetCommsConnectionL( RConnection* aConnectionPtr );
	virtual void SetSocketServerHandleL ( TInt aSocketServerHandle );
	virtual void GetSecurityPrefs( TBool& aDialogPrompt, MSecurityPolicy*& aSecurityPolicy );
	virtual TBool ImmediateSocketShutdown();
	virtual TInt SessionId();
	virtual TInt GetRecvBufferSize();

private:	// methods from MRxDataObserver
	
	virtual void SetStatusL(CRxData& aRxData, TInt aStatus);	

private:	// methods from MHttpBatchingPropertiesCallback

	virtual TInt GetMaxBatchingBufferSize();
	
private:	// methods from MProtHandlerInterface

	virtual const CCertificate* TransactionServerCert(RHTTPTransaction aTransaction);
	virtual const CCertificate*  SessionServerCert();

private:

	CHttpClientHandler(RHTTPSession aSession);
	void ConstructL();	
	
	CHttpConnectionInfo* PrepareTransactionL(RHTTPTransaction aTrans, TBool& aNeedTunnel, TBool& aCanPipeline);
	void CreateRequestUriL(TInt aMethodIndex, RHTTPTransaction aTrans, TPtrC8& aHost, TInt& aPort);
	TBool SetHostHeaderL(RHTTPTransaction aTrans, const TDesC8& aHost, TInt aPort);
	void AddTunnelInfoL(RHTTPTransaction aTrans, const TDesC8& aHost, TInt aPort);
	TBool SelectConnectionManagerL(const CHttpConnectionInfo& aConnectionInfo, RHTTPTransaction aTrans, TBool aCanPipeline, CHttpConnectionManager *&aManager);
	CHttpConnectionManager* SelectTunnelConnectionL(const CHttpConnectionInfo& aConnectionInfo, RHTTPTransaction aTrans, TBool aCanPipeline);
	CHttpConnectionManager* SelectNtlmConnectionL(const CHttpConnectionInfo& aConnectionInfo,RHTTPTransaction aTrans);
	TInt MaxNumConnectionManagers() const;
	void SetupProxyInformation(RHTTPTransaction aTrans);
	TBool CheckPipelineSupport(RHTTPTransaction aTrans);
	
	void GetCipherSuiteL(RHTTPTransaction aTransaction);
	
	TInt MaxNumTransactionsToPipeline() const;
	
	void __RecordConnectionManagerCreationL();
	void EnsureStrictConnectMethodHeaders(RHTTPTransaction aTransaction);
	TInt SetStatus(CRxData& aRxData, TInt aStatus);
	
#if defined (_DEBUG)
	TInt GetConnectionID(const CHttpConnectionManager* aConnectionManager);
	TInt GetConnectionID(const RHTTPTransaction &aTrans);
#endif

	static void GetCertInfoL(const CX509Certificate& aSource, TCertInfo& aDest);
	static void GetDNInfo(const CX500DistinguishedName& aSource, TDNInfo& aDest);
	
private:
	TBool                       iEnableOptimalPipeline;
	TBool						iUseProxy;
	TInt 						iRecvBufferSize;
	mutable TInt 				iMaxNumConnectionManagers;
	mutable TInt				iMaxNumTransactionsToPipeline;
	CHttpTransportLayer*		iTransportLayer;
	CHttpPipelineFallback*      iPipelineFallback;
	TBool*						iSessionClosingPtr; // Not owned, set in ConstructL
	const TStringTable&            iStringTable;
	RStringF					iProxyAddress;
	RPointerArray<CHttpConnectionManager>	iConnectionManagers;
	__FLOG_DECLARATION_MEMBER2
	
	};
	
#endif	// __CHTTPCLIENTHANDLER_H__


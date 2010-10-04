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

#ifndef __CHTTPCONNECTIONMANAGER_H__
#define __CHTTPCONNECTIONMANAGER_H__

#include <e32base.h>
#include <babitflags.h>
#include <stringpool.h>
#include <http/framework/logging.h>

#include "msocketconnectobserver.h"
#include "minputstreamobserver.h"
#include "moutputstreamobserver.h"
#include "mhttprequestobserver.h"
#include "mhttpresponseobserver.h"
#include "mhttpbatchingpropertiescallback.h"
#include "chttpclientoptimiser.h"

class MSocketFactory;
class MSocketConnector;
class MOutputStream;
class MInputStream;

class MHttpRequest;
class MHttpResponse;

class CHttpConnectionInfo;
class CX509Certificate;

class CHttpRequestBatcher;
class MHttpBatchingPropertiesCallback;


class CHttpHostElement : public CBase
    {
    public:
    static CHttpHostElement* New(const TDesC8& aHost);
    ~CHttpHostElement();
    inline const TDesC8& Host() const;
    inline void IncFailedCount();
    inline TInt FailedCount() const;
    inline HBufC8* AcquireHost();
    static inline TInt LinkOffset();
    
    private:
    TBool Construct(const TDesC8& aHost);
    
    public:
    TSglQueLink     iLink;
    HBufC8*         iHost;
    };

class CHttpPipelineFallback : public CBase
    {
    public:
    static CHttpPipelineFallback* NewL();
    ~CHttpPipelineFallback();
    
    TBool NeedPipelineFallback(const TDesC8& aHost);

/**
   If the pipeline has failed atleast 2 times due to network problems
   the host is inserted into the failed host array. The function checks 
   for the no. of failures for the given host. Only on time failed hosts
    are pushed into the probable pipeline failed host array
 */
     void InsertPipelineFailedHost(const TDesC8& aHost);
      
/**
     Add to the pipeline failed host array if not already added.
 */
    void AppendPipelineFailedHost(const TDesC8& aHost);
    
     private:
    CHttpPipelineFallback();
    
    private:
    RPointerArray<HBufC8>       iPipelineFailedHosts;
    TSglQue<CHttpHostElement>   iProbablePipelineFailedHosts;    
    };

class CHttpConnectionManager : public CBase,
							   public MHttpRequestObserver,
							   public MHttpResponseObserver, 
							   public MSocketConnectObserver,
							   public MInputStreamObserver,
							   public MOutputStreamObserver
	{
public:
	
	enum TConnectionStatus
		{
		ENotConnected			= 0,
		EConnectedAndAvailable,
		EConnectedNotAvailable,
		EConnectedAndBusy,
		EConnectingNotAvailable
		};

public:

	static CHttpConnectionManager* NewL(MSocketFactory& aSocketFactory, MHttpBatchingPropertiesCallback& aCallback,
                                    CHttpPipelineFallback& aPipelineFallback, TInt aMaxTransactionsToPipeline, TBool aEnableOptimalPipeline);
	virtual ~CHttpConnectionManager();
	
	void SubmitL(CHttpConnectionInfo& aConnectionInfo, MHttpRequest& aRequest, MHttpResponse& aResponse);
	TConnectionStatus Status() const;
	const CHttpConnectionInfo& ConnectionInfo() const;
	void CancelSubmission(MHttpRequest& aRequest, MHttpResponse& aResponse);
	const CX509Certificate* ServerCert();
	TInt CipherSuite(TDes8& aCipherSuite);
	void TunnelConnection(RStringF aHost);
	TBool TunnelMatches(RStringF aHost) const;
	void MakeConnectionNonPersistent();
	void CheckRequestComplete(MHttpRequest& aRequest);
	void DisablePipelining();
	void AppendPipelineFailedHost(const TDesC8& aHost);
	inline TBool SecureRetry(){ return iSecureRetry;}
	inline void SetNtlmConnId(TInt aId){ iNtlmId=aId;}
    inline TInt GetNtlmConnId(){ return iNtlmId;}


	
private:	// methods from MHttpRequestObserver

	virtual void SendRequestDataL(const TDesC8& aData);
	virtual void RequestComplete();
	virtual void SendingBodyData(TBool aValue);
		
private:	// methods from MHttpResponseObserver

	virtual void ResponseDataParsed();
	virtual void ResponseComplete(const TDesC8& aExcessData);

private:	// methods from MSocketConnnectObserver

	virtual void ConnectionMadeL(MInputStream& aInputStream, MOutputStream& aOutputStream);
	virtual TInt HandleConnectError(TInt aError);

	virtual void MSocketConnectObserver_Reserved();

private:	// methods from MInputStreamObserver

	virtual void ReceivedDataIndL(const TDesC8& aBuffer);
	virtual void SecureServerCnf();
	virtual void InputStreamCloseInd(TInt aError);
	virtual void MInputStreamObserver_Reserved();
	virtual MHttpResponse* CurrentResponse();
    virtual void OnReceiveTimeOut();
	
private:	// methods from MOutputStreamObserver

	virtual void SendDataCnfL();
	virtual void SecureClientCnf();
	virtual void OutputStreamCloseInd(TInt aError);

	virtual void MOutputStreamObserver_Reserved();
	virtual void OnSendTimeOut();
	virtual TInt SendTimeOutVal();

private:
	CHttpConnectionManager(MSocketFactory& aSocketFactory, MHttpBatchingPropertiesCallback& aCallback,
                            CHttpPipelineFallback& aPipelineFallback, TInt aMaxTransactionsToPipeline, TBool aEnableOptimalPipeline);

	void HandleSocketError(TInt aError);
	void CloseConnection();
	void UpgradeConnectionL();
	void SetIdleState();

	TInt FindRequest(MHttpRequest& aRequest);
	TInt FindResponse(MHttpResponse& aResponse);

	void NotifyPendingTransactions(TInt aError);
	void ReportSocketError(TInt aError);
	void ReconnectSocketL();
	
	void DoResponseCompletion ();

	inline TBool CannotPipeline() const;
	inline TBool IsFirstTransaction() const;
	inline TBool IsPendingWriteInConnectedState() const;
	inline TBool DiscardSndDataCnf() const;
	void CreateOptimiserL(MHttpDataOptimiser* aDataOptimiser);
	void StartRecvTimer();
	
private:

	enum TManagerState
		{
		EIdle			= 0,
		EIdleConnected,
		EConnecting,
		EUpgrading,
		EConnected,
		EClosing
		};

	enum TConnectionFlag
		{
		// Cannot currently pipeline. 
		// So will not queue transactions on this connection Manager.
		//
		ECannotPipeline = 0,

		// Currently sending/receiving the first transaction on this connection, 
		// so no other requests are sent out till the response from the server is 
		// examined to see of it can handle pipelining. However transactions are 
		// still queued on this connection. These will be cancelled with 
		// KErrNonPipeliningError should the server be HTTP/1.0.
		//
		EFirstTransaction = 1,

		// Specifies that this transaction is waiting to write its
		// request to the supposedly connected connection. Due to timing issues
		// the server may have closed the connection, but connection manager has
		// not been notified yet.
		// 
		EPendingWriteInConnectedState = 2,

		// The Response completed before notification, that a request was sent, was received.
		// So now we know when confirmation arrives (SndDataCnf) can disregard it.
		//
		EDiscardSndDataCnf = 3
		};
private:
	TBool                   iSecureRetry;
	TBool                   iEnableOptimalPipeline;
	TBool					iTunnel;
	TInt                    iNtlmId;
	TInt					iMaxTransactionsToPipeline;
	TBitFlags8				iFlags;
	MSocketFactory&			iSocketFactory;
	TManagerState			iState;

	MSocketConnector*		iSocketConnector;
	MOutputStream*			iOutputStream;
	MInputStream*			iInputStream;
	
	MHttpRequest*			iCurrentRequest;
	MHttpResponse*			iCurrentResponse;
	TPtrC8					iExcessData;
	RStringF				iTunnelHost;

	CHttpConnectionInfo*	iConnectionInfo;

	CHttpRequestBatcher*	iRequestBatcher;
	MHttpBatchingPropertiesCallback& iCallback;
	
	CHttpClientOptimiser*				iOptimiser;

	RPointerArray<MHttpRequest>		iPendingRequests;
	RPointerArray<MHttpResponse>	iPendingResponses;
	CHttpPipelineFallback&          iPipelineFallback;
public:
	
	__FLOG_DECLARATION_MEMBER2

	};


inline TBool CHttpConnectionManager::CannotPipeline() const
	{
	return iFlags.IsSet(ECannotPipeline);
	}

inline TBool CHttpConnectionManager::IsFirstTransaction() const
	{
	return iFlags.IsSet(EFirstTransaction);
	}

inline TBool CHttpConnectionManager::IsPendingWriteInConnectedState() const
	{
	return iFlags.IsSet(EPendingWriteInConnectedState);
	}

inline TBool CHttpConnectionManager::DiscardSndDataCnf() const
	{
	return iFlags.IsSet(EDiscardSndDataCnf);
	}


inline const TDesC8& CHttpHostElement::Host() const
    {
    return *iHost;
    }

inline HBufC8* CHttpHostElement::AcquireHost()
    {
    HBufC8* host = iHost;
    iHost = NULL;
    return host;
    }

inline TInt CHttpHostElement::LinkOffset()
    {
    return _FOFF(CHttpHostElement, iLink);
    }

#endif	// __CHTTPCONNECTIONMANAGER_H__

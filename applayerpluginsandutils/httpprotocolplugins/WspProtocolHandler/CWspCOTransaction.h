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

#ifndef __CWSPCOTRANSACTION_H__
#define __CWSPCOTRANSACTION_H__

// System includes
#include <e32base.h>
#include <wsp/mwspcomethodcallback.h>
#include <wsp/wsptypes.h>
#include <http/framework/cprottransaction.h>
#include <http/framework/httplogger.h>

// User includes
#include "mwspprimitivesendercallback.h"
#include "mwspcorxdatacallback.h"
#include "mwspcotxdatacallback.h"
#include "cwspheaderutils.h"

// Forward declarations
class MWspCapabilityViewer;
class MWspCOMethodInvoker;
class CWspPrimitiveSender;
class MWspCOMethodObserver;

// Types used for arguments in this file (e.g. TWspBearer) are from the Wap namespace
using namespace Wap;

// @todo docing

//##ModelId=3BBD80610048
class CWspCOTransaction : public CProtTransaction, public MWspCOMethodCallback,
												   public MWspPrimitiveSenderCallback,
												   public MWspCORxDataCallback,
												   public MWspCOTxDataCallback
	{
public:	// Methods

/**	@fn				NewL(RHTTPTransaction aTransaction, MWspCOMethodInvoker& aMethodInvoker, MWspCapabilityViewer& aNegotiatedCapInfo, CWspHeaderUtils& aHdrUtils)
	Intended Usage	:	Static factory constructor.
	@since			7.0
	@leave			Leaves with KErrNoMemory if there was not enough memory to
					create the object.
	@param			aTransaction		The HTTP transaction object associated
										with this WSP method transaction.
	@param			aMethodInvoker		A reference to the method invoker object.
	@param			aNegotiatedCapInfo	A reference to the negotiated capability
										info.
	@param			aObserver			A reference to the method observer.
	@param			aHdrUtils			A reference to the WSP header utilities
	@return			A pointer to newly created object.
*/
	//##ModelId=3C4C41A3032A
	static CWspCOTransaction* NewL(
								   RHTTPTransaction			aTransaction, 
								   MWspCOMethodInvoker&		aMethodInvoker, 
								   MWspCapabilityViewer&	aNegotiatedCapInfo,
								   MWspCOMethodObserver&	aObserver,
								   CWspHeaderUtils&			aHdrUtils
								   );

/** @fn				~CWspCOTransaction()
	Intended Usage	:	Standard destructor.
	@since			7.0
*/
	//##ModelId=3C4C41A3030C
	~CWspCOTransaction();

/**	@fn				InitRequestL()

	@since			7.0
*/
	void InitRequestL();

/**	@fn				NotifyMoreRequestData()

	@since			7.0
*/
	void NotifyMoreRequestData();

/**	@fn				AbortRequest()

	@since			7.0
*/
	void AbortRequest();

/**	@fn				Suicide()
	Intended Usage	:	The client has closed the RHTTPTransaction that this 
						object wraps. The protocol base base class has also 
						transferred ownership of this object to the sub-class, 
						the WSP protocol handler. This function ensures cleanup
						of this object. If the WSP method transaction was in 
						the Null state, then the object deletes itself. If the 
						WSP method transaction is in the Aborting state, then 
						the object will delete itself once it receives the 
						S-MethodAbort.ind primitive.
	@since			7.0
	@todo			finish docing
	@pre			The client has closed the RHTTPTransaction that this object
					wraps.
	@post			If the WSP method transaction was in the Null state, then 
					the object has deleted itself. If the WSP method transaction
					is in the Aborting state, then the object will delete itself
					once it receives the S-MethodAbort.ind primitive.
*/
	//##ModelId=3C4C41A3029E
	void Suicide();

/**	@fn				GetWspHeaderUtils() const
	Intended Usage	:	This method provides access to the WSP header utilities.
	@since			7.0
	@return			A reference to the WSP header utilities
*/
	//##ModelId=3C4C41A30276
	CWspHeaderUtils& GetWspHeaderUtils() const;

private:	// Methods from CProtTransaction

/**	@fn				CreateTxDataL()
	@see			CProtTransaction
*/
	//##ModelId=3C4C41A3023A
	virtual void CreateTxDataL();

/**	@fn				CreateRxDataL(MRxDataObserver& aObserver)
	@see			CProtTransaction
*/
	//##ModelId=3C4C41A301D5
	virtual void CreateRxDataL(MRxDataObserver& aObserver);

private:	// Methods from MWspCOMethodCallback

/** @fn				MethodInvokeCnf()
	@see			MWspCOMethodCallback
*/
	//##ModelId=3C4C41A30199
	virtual void MethodInvokeCnf();

/** @fn				MethodInvokeDataCnf()
	@see			MWspCOMethodCallback
*/
	//##ModelId=3C4C41A30059
	virtual void MethodInvokeDataCnf();

/** @fn				MethodAbortInd(TWSPReason aReason)
	@see			MWspCOMethodCallback
*/
	//##ModelId=3C4C41A203E7
	virtual void MethodAbortInd(TWspReason aReason);

/** @fn				MethodResultInd(TInt aStatus, const TDesC8& aResponseHeaders, MHTTPDataSupplier& aResponseBody, TBool aMoreData)
	@see			MWspCOMethodCallback
*/
	//##ModelId=3C4C41A2033D
	virtual void MethodResultInd(
								TInt					aStatus, 
								const TDesC8&			aResponseHeaders, 
								MHTTPDataSupplier&		aResponseBody,
								TBool					aMoreData
								);

/** @fn				MethodResultDataInd(const TDesC8& aTrailerHeaders, TBool aMoreData)
	@see			MWspCOMethodCallback
*/
	//##ModelId=3C4C41A202EC
	virtual void MethodResultDataInd(const TDesC8& aTrailerHeaders, TBool aMoreData);

private:	// Methods from MWspPrimitiveSenderCallback

/**	@fn				SendPrimitiveL(TWspPrimitive aPrimitive)
	@see			MWspPrimitiveSenderCallback
*/
	//##ModelId=3C4C41A2029C
	virtual void SendPrimitiveL(TWspPrimitive aPrimitive);

/** @fn				WspPrimitiveSenderCallbackError(TInt aError)
	@see			MWspPrimitiveSenderCallback
*/
	//##ModelId=3C4C41A20256
	virtual TInt WspPrimitiveSenderCallbackError(TInt aError);

private:	// Methods from MWspCORxDataCallback

/**	@fn				AbortResponse()
	@see			MWspCORxDataCallback
*/
	//##ModelId=3C4C41A20210
	virtual void AbortResponse();

/**	@fn				SendResponsePrimitive()
	@see			MWspCORxDataCallback
*/
	//##ModelId=3C4C41A201F2
	virtual void SendResponsePrimitive();

private:	// Methods from MWspCOTxDataCallback

/**	@fn				SendInvokePrimitive()
	Informs the Tx Data object observer that it should send a S-MethodInvoke or
	S-MethodInvokeData primitive.
	@since			7.0
*/
	virtual void SendInvokePrimitive();

/**	@fn				AbortInvoke()
	Informs the Tx Data object observer that the method should be aborted.
	@see			7.0
*/
	virtual void AbortInvoke();

private:	// Methods

/**	@fn				CWspCOTransaction(RHTTPTransaction aTransaction, MWspCOMethodInvoker& aMethodInvoker, MWspCapabilityViewer& aNegotiatedCapInfo, CWspHeaderUtils& aHdrUtils)
	Intended Usage	:	First phase constructor.
	@since			7.0
	@param			aTransaction		The HTTP transaction object associated
										with this WSP method transaction.
	@param			aMethodInvoker		A reference to the method invoker object.
	@param			aNegotiatedCapInfo	A reference to the negotiated capability
										info.
	@param			aObserver			A reference to the method observer.
	@param			aHdrUtils			A reference to the WSP header utilities
*/
	//##ModelId=3C4C41A20120
	CWspCOTransaction(
					  RHTTPTransaction			aTransaction, 
					  MWspCOMethodInvoker&		aMethodInvoker, 
					  MWspCapabilityViewer&		aNegotiatedCapInfo,
					  MWspCOMethodObserver&		aObserver,
					  CWspHeaderUtils&			aHdrUtils
					  );

/**	@fn				ConstructL()
	Intended Usage	:	Second phase constructor. Does any necessary allocations.
	@since			7.0
	@pre			First phase construction has been done.
	@post			The object is fully initialised.
*/
	//##ModelId=3C4C41A20116
	void ConstructL();

/**	@fn				MethodInvoke()
	Intended Usage	:	Invokes the method. Sends the S-MethodInvoke.req 
						primitive. Ensures that the Server SDU size and the
						Server message size are observed. Will leave if either
						of these session capabilities are exceeded.
	@since			7.0
	@pre			The WSP method transaction is in the Null state.
	@post			The S-MethodInvoke.req primitive has been sent and the WSP
					method transaction is in the Requesting state.
*/
	//##ModelId=3C4C41A302F8
	void MethodInvoke();

/**	@fn				MethodInvokeData()
	Intended Usage	:
	@since			7.0
	@todo			finish docing
	@pre			The WSP method transaction is in the Requesting state.
	@post			The S-MethodInvokeData.req primitive has been sent and the
					WSP method transaction remains in the Requesting state.
*/
	//##ModelId=3C4C41A302D0
	void MethodInvokeData();

/**	@fn				MethodAbort()
	Intended Usage	:
	@since			7.0
	@todo			finish docing
	@pre			The WSP method transaction is not in the Null or Aborting 
					state.
	@post			The S-MethodAbort.req primitive has been sent and the WSP
					method transaction is in the Aborting state.
*/
	//##ModelId=3C4C41A302BC
	void MethodAbort();

/**	@fn				MethodResultRes()
	Intended Usage	:
	@since			7.0
	@post			The WSP method transaction is in the Waiting or Completing
					state.
	@pre			The WSP method transaction is in the Waiting2 state if the 
					request response is not complete and there are subsequent 
					S-MethodResultData primitives to be received. If the request
					response is complete, then the WSP method transaction is in
					the Null state.
*/
	void MethodResultRes();

/**	@fn				MethodResultDataRes()
	Intended Usage	:
	@since			7.0
	@post			The WSP method transaction is in the Waiting2 or Completing
					state.
	@pre			The WSP method transaction remains in the Waiting2 state if 
					the request response is not complete and there are 
					subsequent S-MethodResultData primitives to be received. If
					the request response is complete, then the WSP method 
					transaction is in the Null state.
*/
	void MethodResultDataRes();

/**	@fn				ProcessResponseDataL(const TDesC8& aResponseHeaders, MHTTPDataSupplier& aResponseBody, TBool aMoreData)	

	@since			7.0
	@param			aResponseHeaders	A reference to a buffer	that provides 
										the encoded response headers.
	@param			aResponseBody		A reference to a data supplier object
										that provides the data associated 
										with the response.
	@param			aMoreData			A flag to indicate whether subsequent
										S-MethodResultData primitives are to be
										received for this transaction.
*/
	void ProcessResponseDataL(const TDesC8& aResponseHeaders, MHTTPDataSupplier& aResponseBody, TBool aMoreData);

/**	@fn				void PostResProcessing()
	Does the necessary processing given that either a S-MethodResult.res or a
	S-MethodResultData.res primitive has just been sent. If the method state is
	Completing, then the method moves into the Null state, and if the self-
	destruction has been flagged, the object deletes itself. If the method is
	not in the Completing state, then it is in the Waiting2 state, and it does
	not change state.
	@since			7.0
	@pre			The method is in either the Completing or the Waiting2 state.
	@post			The method is in either in the Null state or has remained in 
					the Waiting2 state.
*/
	void PostResProcessing();

/**	@fn				ResetFlags()
	Intended Usage	:	Resets all the flags in the object.
	@since			7.0
	@post			All flags are set to EFalse.
*/
	//##ModelId=3C4C41A20057
	void ResetFlags();

private:	// Attributes

	/** HTTP logger handle (debug only)
	*/
	__DECLARE_LOG

	/** A reference to the CO Transaction Invoker object. Owned by the transport 
		handler.
	*/
	//##ModelId=3C4C41A2004D
	MWspCOMethodInvoker&		iMethodInvoker;

	/** An object that holds the negotiated capabilties for the current Wsp 
		session.
	*/
	//##ModelId=3C4C41A20039
	MWspCapabilityViewer&		iNegotiatedCapInfo;

	/** A reference to the method observer.
	*/
	//##ModelId=3C4C41A20025
	MWspCOMethodObserver&		iObserver;

	/** A reference to the WSP header utilities
	*/
	//##ModelId=3C4C438001E5
	CWspHeaderUtils&			iHdrUtils;

	/** The state of the WSP method transaction.
	*/
	//##ModelId=3C4C41A20007
	TWspMethodState				iMethodState;	

	/** A flag to indicate whether there is more data to send in subsequent
		S-MethodInvokeData primitives.
	*/
	//##ModelId=3C4C41A103DB
	TBool						iMoreRequestData;

	/** A flag to incidcate whether the S-MethodResult.res primitive has been sent.
	*/
	TBool						iSentMethodResultRes;

	/** A flag to indicate whether the client aborted the method.
	*/
	//##ModelId=3C4C41A1039F
	TBool						iClientMethodAbort;

	/** A flag that inducates whether the final S-MethodResult.res or 
		S-MethodResultData.res primitive is waiting to be sent.
	*/
	TBool						iFinalResPending;

	/** A flag to indicate that the client has closed the transaction and that
		once the S-MethodAbort.ind primitive is received, the object must self-
		destruct.
	*/
	//##ModelId=3C4C41A10383
	TBool						iSuicide;

	/** An active object that is used to invoke the sending of certain WSP
		primitives.
	*/
	//##ModelId=3C4C41A10377
	CWspPrimitiveSender*		iPrimitiveSender;

	};

#endif	// __CWSPCOTRANSACTION_H__

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

// System includes
#include <httpstringconstants.h>
#include <wsp/mwspcapabilityviewer.h>
#include <wsp/mwspcomethodinvoker.h>
#include <wsperror.h>

// User includes
#include <http/rhttpsession.h>
#include "cwspcotxdata.h"
#include "cwspcorxdata.h"
#include "cwspprimitivesender.h"
#include "mwspcomethodobserver.h"
#include "wsppanic.h"

// Class signature
#include "cwspcotransaction.h"

CWspCOTransaction* CWspCOTransaction::NewL(
										   RHTTPTransaction			aTransaction, 
										   MWspCOMethodInvoker&		aMethodInvoker, 
										   MWspCapabilityViewer&	aNegotiatedCapInfo,
										   MWspCOMethodObserver&	aObserver,
										   CWspHeaderUtils&			aHdrUtils
										   )
	{
	CWspCOTransaction* self = new (ELeave) CWspCOTransaction(aTransaction, aMethodInvoker, aNegotiatedCapInfo, aObserver, aHdrUtils);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CWspCOTransaction::~CWspCOTransaction()
	{
	if( iPrimitiveSender )
		iPrimitiveSender->Cancel();
	delete iPrimitiveSender;
	}

CWspCOTransaction::CWspCOTransaction(
									 RHTTPTransaction			aTransaction, 
									 MWspCOMethodInvoker&		aMethodInvoker, 
									 MWspCapabilityViewer&		aNegotiatedCapInfo,
									 MWspCOMethodObserver&		aObserver,
									 CWspHeaderUtils&			aHdrUtils
									 )
: CProtTransaction(aTransaction), iMethodInvoker(aMethodInvoker),
								  iNegotiatedCapInfo(aNegotiatedCapInfo), 
								  iObserver(aObserver),
								  iHdrUtils(aHdrUtils),
								  iMethodState(ENullMethod)
	{
	__OPEN_LOG("WspProtocolHandler.txt")
	}

void CWspCOTransaction::ConstructL()
	{
	iPrimitiveSender = CWspPrimitiveSender::NewL(*this);	
	}

void CWspCOTransaction::InitRequestL()
	{
	__ASSERT_DEBUG( iMethodState == ENullMethod, Panic(KWspPanicMethodAlreadyActive) );

	// Reset all the flags
	ResetFlags();

	// Set the request data
	STATIC_CAST(CWspCOTxData*, iTxData)->SetRequestDataL();
	}

void CWspCOTransaction::NotifyMoreRequestData()
	{
	__ASSERT_DEBUG( iMethodState == ENullMethod || iMethodState == ERequesting, Panic(KWspPanicBadMethodState) );

	// Inform the Tx data object that there is more data
	STATIC_CAST(CWspCOTxData*, iTxData)->NotifyMoreRequestData();
	}

void CWspCOTransaction::AbortRequest()
	{
	// Check the state - ignore if in the WSP method transaction is in the Null
	// or Aborting state.
	if( iMethodState != ENullMethod && iMethodState != EAborting && !iFinalResPending )
		{
		// The client has cancelled the method - set flag.
		iClientMethodAbort = ETrue;

		// Do abort...
		MethodAbort();
		}
	}

void CWspCOTransaction::Suicide()
	{
	// Check the WSP method transaction state
	if( iMethodState == EAborting || iFinalResPending )
		{
		// Waiting for the S-MethodAbort.ind primitive or the final .res primitive
		// to be sent. Need to flag self-destruction suicide when the primitive
		// is received/sent.
		iSuicide = ETrue;
		}
	else
		{
		__ASSERT_DEBUG( iMethodState == ENullMethod, Panic(KWspPanicBadMethodState) );

		// The WSP method transaction is finished or not started - safe to 
		// delete now.
		delete this;
		}
	}

void CWspCOTransaction::ResetFlags()
	{
	iMoreRequestData		= EFalse;
	iSentMethodResultRes	= EFalse;
	iClientMethodAbort		= EFalse;
	iSuicide				= EFalse;
	iFinalResPending		= EFalse;
	}

CWspHeaderUtils& CWspCOTransaction::GetWspHeaderUtils() const
	{
	return iHdrUtils;
	}

void CWspCOTransaction::MethodInvoke()
	{
	__ASSERT_DEBUG( iMethodState == ENullMethod, Panic(KWspPanicMethodAlreadyActive) );

	__LOG1(_L("Trans %d - Sending S-MethodInvoke.req"), Transaction().Id());
	
	// Down-cast to derived CTxData object
	CWspCOTxData* txData = STATIC_CAST(CWspCOTxData*, iTxData);

	// Get the request body data supplier
	MHTTPDataSupplier& dataSupplier = txData->RequestBodyData();
	TPtrC8 bodyData;
	iMoreRequestData = !dataSupplier.GetNextDataPart(bodyData);

	// Send the S-MethodInvoke.req primitive
	RHTTPRequest request = iTrans.Request();
	iMethodInvoker.MethodInvokeReq(
								  *this, 
								  request.Method(), 
								  request.URI(), 
								  txData->RequestHeadersData(),
								  bodyData,
								  iMoreRequestData
								  );
	// WSP method transaction is requesting - update state
	iMethodState = ERequesting;

	__LOG(_L("---Method in Requesting state"));

	// Release request body data
	dataSupplier.ReleaseData();
	}

void CWspCOTransaction::MethodInvokeData()
	{
	__ASSERT_DEBUG( iNegotiatedCapInfo.GetProtocolOptions() & ELargeDataTransfer, Panic(KWspPanicLDTNotSuppoted) );
	__ASSERT_DEBUG( iMethodState == ERequesting, Panic(KWspPanicNotExpectingMoreRequestData) );

	__LOG1(_L("Trans %d - Sending S-MethodInvokeData.req"), Transaction().Id());

	// Down-cast to derived CTxData object
	CWspCOTxData* txData = STATIC_CAST(CWspCOTxData*, iTxData);

	// Get the request body data supplier
	MHTTPDataSupplier& dataSupplier = txData->RequestBodyData();
	TPtrC8 bodyData;
	iMoreRequestData = !dataSupplier.GetNextDataPart(bodyData);

	// Send the S-MethodInvoke.req primitive
	iMethodInvoker.MethodInvokeDataReq(
									  *this, 
									  bodyData,
									  txData->RequestHeadersData(),
									  iMoreRequestData
									  );

	__LOG(_L("---Method in Requesting state"));

	// WSP method transaction remains in requesting state - do nothing.
	// Release request body data
	dataSupplier.ReleaseData();
	}

void CWspCOTransaction::MethodAbort()
	{
	__ASSERT_DEBUG( iMethodState != EAborting && iMethodState != ENullMethod, Panic(KWspPanicBadMethodState) );

	__LOG1(_L("Trans %d - Sending S-MethodAbort.req"), Transaction().Id());

	// Cancel any pending primitives that are waiting to be sent
	iPrimitiveSender->Cancel();

   	// Abort the method - send the S-MethodAbort.req
   	iMethodInvoker.MethodAbortReq(*this);
   
   	// WSP method transaction is aborting - update state
   	iMethodState = EAborting;

	__LOG(_L("---Method in Aborting state"));
	}

void CWspCOTransaction::MethodResultRes()
	{
	// Send the S-MethodResult.res primitive
	iMethodInvoker.MethodResultRes(*this, KNullDesC8());

	// Ensure the S-MethodResultData.res is not sent again
	iSentMethodResultRes = ETrue;

	// Sent the .res primitive - update.
	PostResProcessing();
	}

void CWspCOTransaction::MethodResultDataRes()
	{
	// Send the S-MethodResultData.cnf primitive
	iMethodInvoker.MethodResultDataRes(*this, KNullDesC8());

	// Sent the .res primitive - update.
	PostResProcessing();
	}

void CWspCOTransaction::PostResProcessing()
	{
	// Check the WSP method transaction state
	if( iMethodState == ECompleting )
		{
		__ASSERT_DEBUG( iFinalResPending, Panic(KWspPanicBadMethodState) );

		// WSP method state is in the Null state - update state
		iMethodState = ENullMethod;

		__LOG(_L("---Method in Null state"));

		// Reset the final .res pending flag
		iFinalResPending = EFalse;

		// Need to inform observer that this method has sent the final .res 
		// primitive.
		iObserver.NotifyMethodComplete();

		// Check to see if the transaction has been closed.
		if( iSuicide )
			{
			__LOG(_L("---Transaction has been closed - suiciding!"));

			// Transaction is closed - self-destruct
			delete this;
			}
		}
#ifdef _DEBUG
	else
		{
		__ASSERT_DEBUG( iMethodState == EWaiting2, Panic(KWspPanicBadMethodState) );
		__LOG(_L("---Method in Waiting2 state"));
		}
#endif
	}

void CWspCOTransaction::ProcessResponseDataL(const TDesC8& aResponseHeaders, MHTTPDataSupplier& aResponseBody, TBool aMoreData)
	{
	// Create the rx data object
	CreateRxDataL(iObserver);
	
	// Set-up the response data object
	STATIC_CAST(CWspCORxData*, iRxData)->SetResponseDataL(aResponseHeaders, aResponseBody, aMoreData);
	}

/*
 *	Methods from CProtTransaction
 */

void CWspCOTransaction::CreateTxDataL()
	{
	__ASSERT_DEBUG( iTxData == NULL, Panic(KWspPanicTxDataObjectNotReset) );

	iTxData = CWspCOTxData::NewL(*this, *this, iNegotiatedCapInfo);
	}

void CWspCOTransaction::CreateRxDataL(MRxDataObserver& aObserver)
	{
	__ASSERT_DEBUG( iRxData == NULL, Panic(KWspPanicRxDataObjectNotReset) );

	iRxData = CWspCORxData::NewL(*this, aObserver, *this);	
	}

/*
 *	Methods from MWspCOMethodCallback
 */

void CWspCOTransaction::MethodInvokeCnf()
	{
	__ASSERT_DEBUG( iMethodState == ERequesting, Panic(KWspPanicBadMethodState) );

	__LOG1(_L("Trans %d - Received S-MethodInvoke.cnf"), Transaction().Id());
	__LOG1(_L("---More Data flag : %d."), iMoreRequestData);

	// Inform the Tx data object that cnf has been received.
	STATIC_CAST(CWspCOTxData*, iTxData)->ReceivedCnf();

	// Is the requst complete? Stay in requesting if not.
	if( !iMoreRequestData )
		{
		// The request is complete - all the request body data has been received
		// and, as iMoreRequestData is cleared, all the headers and body data 
		// have been sent. Can delete the Tx data object.
		ResetTxData();

		// WSP method transaction is waiting - update state
		iMethodState = EWaiting;

		__LOG(_L("---Method in Waiting state"));
		}
#if defined (_DEBUG) && defined (_LOGGING)
	else
		__LOG(_L("---Method in Requesting state"));
#endif
	}

void CWspCOTransaction::MethodInvokeDataCnf()
	{
	__ASSERT_DEBUG( iMethodState == ERequesting, Panic(KWspPanicBadMethodState) );

	__LOG1(_L("Trans %d - Received S-MethodInvokeData.cnf"), Transaction().Id());
	__LOG1(_L("---More Data flag : %d."), iMoreRequestData);

	// Inform the Tx data object that cnf has been received.
	STATIC_CAST(CWspCOTxData*, iTxData)->ReceivedCnf();

	// Is the requst complete? Stay in requesting if not.
	if( !iMoreRequestData )
		{
		// The request is complete - all the request body data has been received
		// and, as iMoreRequestData is cleared, all the headers and body data 
		// have been sent. Can delete the Tx data object.
		ResetTxData();

		// WSP method transaction is waiting - update state
		iMethodState = EWaiting;

		__LOG(_L("---Method in Waiting state"));
		}
#if defined (_DEBUG) && defined (_LOGGING)
	else
		__LOG(_L("---Method in Requesting state"));
#endif
	}

void CWspCOTransaction::MethodAbortInd(TWspReason aReason)
	{
	__ASSERT_DEBUG( iMethodState != ENullMethod, Panic(KWspPanicBadMethodState) );

	__LOG1(_L("Trans %d - Received S-MethodAbort.ind"), Transaction().Id());
	__LOG1(_L("---Abort reason : %d"), aReason);

	// Cancel any pending primitives that are waiting to be sent
	iPrimitiveSender->Cancel();
	if( iFinalResPending )
		{
		// Reset the final .res pending flag
		iFinalResPending = EFalse;

		// Need to inform observer that this method has sent the final .res 
		// primitive.
		iObserver.NotifyMethodComplete();
		}

	// The method has been aborted - check to see if the client initiated the 
	// abort or not.
	if( !iClientMethodAbort )
		{
		__LOG(_L("---Method was aborted by the proxy - need to  inform the client."));

		// The method was NOT aborted by the client - need to fail the 
		// transaction. Check the abort reason for EOutOfMemory.
		THTTPEvent event = THTTPEvent::EFailed;
		if( aReason == EOutOfMemory )
			{
			// Send KErrNoMemory event - the validation filter will ensure an 
			// EFailed event is also sent.
			event = KErrNoMemory;
			}
		TRAPD(err, Transaction().SendEventL(event,
											THTTPEvent::EIncoming,
											THTTPFilterHandle::EProtocolHandler));

		// Get the protocol handler to deal with the method abort.
		iObserver.HandleMethodAbort(*this);				

		// If the event could not be sent, we must take more drastic action.  Note that
		// this _must_ follow the observer's handling of method abort, since the use
		// of RHTTPTransaction::Fail() is drastic, and could result in the whole
		// transaction having been deleted by the time we get back here.
		if (err != KErrNone)
			Transaction().Fail(THTTPFilterHandle::EProtocolHandler);
		}

	// WSP method transaction is now Null - update state
	iMethodState = ENullMethod;

	__LOG(_L("---Method in Null state"));

	// Check to see if the client has closed the transaction. In this case the
	// suicide flag is set and need to self-destruct.
	if( iSuicide )
		{
		__LOG(_L("---Transaction has been closed - suiciding!"));

		// Transaction is closed - self-destruct
		delete this;
		}
	}

void CWspCOTransaction::MethodResultInd(
										TInt					aStatus, 
										const TDesC8&			aResponseHeaders, 
										MHTTPDataSupplier&		aResponseBody,
										TBool					aMoreData
										)
	{
	__ASSERT_DEBUG( iMethodState == EWaiting, Panic(KWspPanicBadMethodState) );

	__LOG1(_L("Trans %d - Received S-MethodResult.ind"), Transaction().Id());
	__LOG1(_L("---More Data flag : %d."), aMoreData);

	// Are there more S-MethodResultData primitives to follow?
	if( aMoreData )
		{
		// WSP method transaction is in Waiting2 state - update state
		iMethodState = EWaiting2;

		__LOG(_L("---Method in Waiting2 state"));
		}
	else
		{
		// WSP method transaction is in Completing state - update state
		iMethodState = ECompleting;

		__LOG(_L("---Method in Completing state"));
		}

	// Decode response status code from WSP binary representation
	TInt httpStatus = 0;
	if ((aStatus >= 0x10) && (aStatus <= 0x65))
		{
		// Calculate this status code in decimal
		httpStatus = 100*(aStatus/0x10);
		if (httpStatus == 500)
			httpStatus = 416;
		if (httpStatus == 600)
			httpStatus = 500;
		httpStatus += aStatus & 0xf;
		}

	// Set the response status
	iTrans.Response().SetStatusCode(httpStatus);

	// Process the response header and body data.
	TRAPD(error, ProcessResponseDataL(aResponseHeaders, aResponseBody, aMoreData));

	// Check everything went ok
	if( error != KErrNone )
		{
		// Ok the S-MethodResult primitive was not dealt with correctly - abort
		// the method
		iPrimitiveSender->InitiateSend(ESMethodAbort);

		__LOG1(_L("---Could not deal with S-MethodResult.ind primitive. Error : %d"), error);
		__LOG( _L("---Aborting the method."));
		}
	}

void CWspCOTransaction::MethodResultDataInd(const TDesC8& aTrailerHeaders, TBool aMoreData)
	{
	__ASSERT_DEBUG( iMethodState == EWaiting2, Panic(KWspPanicBadMethodState) );

	__LOG1(_L("Trans %d - Received S-MethodResultData.ind"), Transaction().Id());
	__LOG1(_L("---More Data flag : %d."), aMoreData);

	// Are there more S-MethodResultData primitives to follow? If there are
	// more S-MethodResultData primitives to follow then the WSP method 
	// transaction remains in Waiting2 state - no need to update the state.
	if( !aMoreData )
		{
		// No more S-MethodResultData primitives to follow. WSP method 
		// transaction is in Completing state - update state
		iMethodState = ECompleting;

		__LOG(_L("---Method in Completing state"));
		}
#if defined (_DEBUG) && defined (_LOGGING)
	else
		__LOG(_L("---Method in EWaiting2 state"));
#endif

	// Update the response data object
	TRAPD(error, 
		  STATIC_CAST(CWspCORxData*, iRxData)->UpdateResponseDataL(aTrailerHeaders, aMoreData));

	// Check everything went ok
	if( error != KErrNone )
		{
		// Ok the S-MethodResultData primitive was not dealt with correctly - 
		// abort the method
		iPrimitiveSender->InitiateSend(ESMethodAbort);

		__LOG1(_L("---Could not deal with S-MethodResultData.ind primitive. Error : %d"), error);
		__LOG( _L("---Aborting the method."));
		}
	}

/*
 * Methods from MWspPrimitiveSenderCallback
 */

void CWspCOTransaction::SendPrimitiveL(TWspPrimitive aPrimitive)
	{
	// Check that the primitive is one that is supported
	switch( aPrimitive )
		{
	case ESMethodInvokeData:
		{
		MethodInvokeData();
		} break;
	case ESMethodResult:
		{
		MethodResultRes();
		} break;
	case ESMethodResultData:
		{
		MethodResultDataRes();
		} break;
	case ESMethodAbort:
		{
		MethodAbort();
		} break;
	default:
		// Unsupported primitive
		User::Leave(KWspErrUnsupportedSendPrimitive);
		break;
		}
	}

TInt CWspCOTransaction::WspPrimitiveSenderCallbackError(TInt /*aError*/)
	{
	// Ok, sending one of the primitives failed. Abort the method
	iPrimitiveSender->InitiateSend(ESMethodAbort);

	// Signal that this leave has been dealt with.
	return KErrNone;
	}

/*
 * Methods from MWspCORxDataCallback
 */

void CWspCOTransaction::AbortResponse()
	{
	iPrimitiveSender->InitiateSend(ESMethodAbort);
	}

void CWspCOTransaction::SendResponsePrimitive()
	{
	// Set the pending last response primitive flag
	iFinalResPending = iMethodState == ECompleting;

	// Is this the final .res?
	if( iFinalResPending ) 
		{
		// Inform the observer that this method needs to send the final .res primitive
		iObserver.NotifyPendingCompletingMethod();
		}

	// Check to see if the S-MethodResult primitive has already been responded.
	if( iSentMethodResultRes )
		{
		// Need to send the S-MethodResultData.res
		iPrimitiveSender->InitiateSend(ESMethodResultData);
		}
	else
		{
		// Need to send the S-MethodResult.res
		iPrimitiveSender->InitiateSend(ESMethodResult);
		}	
	}

/*
 * Methods from MWspCOTxDataCallback
 */

void CWspCOTransaction::SendInvokePrimitive()
	{
	// Need to check the state to see what primitive to send.
	if( iMethodState == ENullMethod )
		{
		// Need to send S-MethodInvoke.req primitive - ok to send here as this
		// would have been caused either by InitRequestL() or by 
		// otifyMoreRequestData()
		MethodInvoke();
		}
	else
		{
		__ASSERT_DEBUG( iMethodState == ERequesting, Panic(KWspPanicBadMethodState) );

		// Need to send S-MethodInvokeData.req - defer sending as could be in 
		// callstack from either MethodInvokeCnf() or MethodInvokeDataCnf().
		iPrimitiveSender->InitiateSend(ESMethodInvokeData);
		}
	}

void CWspCOTransaction::AbortInvoke()
	{
	// Check the state - no need to send S-MethodAbort primitive if the method
	// is still in the null state.
	if( iMethodState == ENullMethod )
		{
		// Inform the client that the tranaction has failed.
		TRAPD(err, Transaction().SendEventL(THTTPEvent::EFailed,
											THTTPEvent::EIncoming,
											THTTPFilterHandle::EProtocolHandler));

		// Get the protocol handler to deal with the method abort.
		iObserver.HandleMethodAbort(*this);				

		// If the event could not be sent, we must take more drastic action.  Note that
		// this _must_ follow the observer's handling of method abort, since the use
		// of RHTTPTransaction::Fail() is drastic, and could result in the whole
		// transaction having been deleted by the time we get back here.
		if (err != KErrNone)
			Transaction().Fail(THTTPFilterHandle::EProtocolHandler);
		}
	else
		{
		__ASSERT_DEBUG( iMethodState == ERequesting, Panic(KWspPanicBadMethodState) );

		// Send MethodAbort.req primitive - defer sending as could be in 
		// callstack from either MethodInvokeCnf() or MethodInvokeDataCnf().
		iPrimitiveSender->InitiateSend(ESMethodAbort);
		}
	}

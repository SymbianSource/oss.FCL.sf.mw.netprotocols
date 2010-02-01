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
#include <http/framework/mrxdataobserver.h>
#include <http/mhttpdatasupplier.h>
#include <http/rhttptransaction.h>
#include <wsperror.h>

// User includes
#include "mwspcorxdatacallback.h"
#include "cwspcotransaction.h"
#include "cwspheaderutils.h"
#include "wsppanic.h"

// Class signature
#include "cwspcorxdata.h"



CWspCORxData* CWspCORxData::NewL(
								CProtTransaction&		aTransaction, 
								MRxDataObserver&		aObserver, 
								MWspCORxDataCallback&	aMethodCallback
								)
	{
	return new (ELeave) CWspCORxData(aTransaction, aObserver, aMethodCallback);
	}

CWspCORxData::CWspCORxData(
						  CProtTransaction&			aTransaction, 
						  MRxDataObserver&			aObserver, 
						  MWspCORxDataCallback&		aMethodCallback
						  )
: CRxData(aTransaction, aObserver), iMethodCallback(aMethodCallback), iOverallDataSize(KErrNotFound)
	{
	}

CWspCORxData::~CWspCORxData()
	{
	}

void CWspCORxData::SetResponseDataL(const TDesC8& aResponseHeaders, MHTTPDataSupplier& aResponseBody, TBool aMoreResponseData)
	{
	// Store the more data flag
	iMoreData = aMoreResponseData;

	// Set the header data
	SetHeaderDataL(aResponseHeaders);

	// Set the body data
	SetBodyDataL(aResponseBody);
	
	// Is the response complete? Is there any data to be passed to the client?
	if( !iMoreData && iBodyData.Length() == 0 )
		{
		// Have received the entire response - inform the client.
		ResponseCompleteL();
		}
	}

void CWspCORxData::UpdateResponseDataL(const TDesC8& aTrailerHeaders, TBool aMoreResponseData)
	{
	__ASSERT_DEBUG( iMoreData, Panic(KWspPanicNotExpectingMoreResponseData) );

	// Store the more data flag
	iMoreData = aMoreResponseData;

	// Is there trailer headers?
	if( aTrailerHeaders.Length() > 0 )
		{
		// Is this the last primitive
		if( iMoreData )
			{
			// This is not the last primitive so no trailer headers allowed.
			User::Leave(KWspErrTrailerHeadersNotExpected);
			}
		// Update the header data
		UpdateHeaderDataL(aTrailerHeaders);
		}
	// Update the body data
	UpdateBodyDataL();

	// Is the response complete? Is there any data to be passed to the client?
	if( !iMoreData && iBodyData.Length() == 0 )
		{
		// Have received the entire response - inform the client.
		ResponseCompleteL();
		}
	}

void CWspCORxData::SetHeaderDataL(const TDesC8& aResponseHeaders)
	{
	// Was any header data received in this primitive.
	if( aResponseHeaders.Length() > 0 )
		{
		// Segment the header data into the separate header fields in the response
		// object's headers.
		RHTTPTransaction trans	= iProtTrans->Transaction();
		RHTTPHeaders headers	= trans.Response().GetHeaderCollection();
		STATIC_CAST(CWspCOTransaction*, 
					iProtTrans)->GetWspHeaderUtils().DecodeReplyHeadersL(
																		trans.Session().StringPool(),
																		aResponseHeaders,
																		headers
																		);
		}
	// Inform the client that all the response headers have been received
	RHTTPTransaction tr = iProtTrans->Transaction();
	tr.SendEventL(
				 THTTPEvent::EGotResponseHeaders,
				 THTTPEvent::EIncoming, 
				 THTTPFilterHandle(THTTPFilterHandle::EProtocolHandler)
				 );
	}

void CWspCORxData::SetBodyDataL(MHTTPDataSupplier& aResponseBody)
	{
	__ASSERT_DEBUG( !iGotBodyData, Panic(KWspPanicRxDataObjectNotReset) );

	// Store the body data supplier
	iBodyDataSupplier = &aResponseBody;

	// Update the overall data size
	iOverallDataSize = iBodyDataSupplier->OverallDataSize();

   	// Add the body data supplier to the response object
   	iProtTrans->Transaction().Response().SetBody(*this);

	// Get the data...
	UpdateBodyDataL();
	}

void CWspCORxData::UpdateHeaderDataL(const TDesC8& aTrailerHeaders)
	{
	// This is the last S-MethodResultData primitive and there is header data -
	// these are probably Trailer Headers.

	// Segment the header data into the separate header fields in the response
	// object's headers.
	RHTTPTransaction trans	= iProtTrans->Transaction();
	RHTTPHeaders headers	= trans.Response().GetHeaderCollection();
	STATIC_CAST(CWspCOTransaction*, 
				iProtTrans)->GetWspHeaderUtils().DecodeHeadersL(
															   trans.Session().StringPool(),
															   aTrailerHeaders,
															   headers
															   );
	iHasTrailer = ETrue;
	}

void CWspCORxData::UpdateBodyDataL()
	{
	__ASSERT_DEBUG( !iGotBodyData, Panic(KWspPanicResponseDataNotReleased) );
	__ASSERT_DEBUG( iBodyDataSupplier, Panic(KWspPanicNoResponseDataReceived) );

	// Is there more data for this SDU?
	iLastChunk = iBodyDataSupplier->GetNextDataPart(iBodyData);

	// Check to see there is some data
	if( iBodyData.Length() == 0 )
		{
		// Are there more chunks? Keep trying to there is some data..
		while( !iLastChunk && iBodyData.Length() == 0 )
			{
			// Release the data in the body data supplier
			iBodyDataSupplier->ReleaseData();

			// Get the next chunk
			iLastChunk = iBodyDataSupplier->GetNextDataPart(iBodyData);
			}
		// Was some data received?
		if( iBodyData.Length() == 0 )
			{
			// Release the data in the body data supplier
			iBodyDataSupplier->ReleaseData();

			// No body data received - is this the last primitive? It is possible
			// to have no body data if the last primitive had the trailer headers
			// only.
			if( iMoreData )
				{
				// Something has gone wrong - more data expected, but none sent in 
				// this SDU. Response must be corrupt.
				User::Leave(KWspErrExpectingBodyData);
				}
			// Inform method callback to send the response to the primitive
			iMethodCallback.SendResponsePrimitive();

			// Nothing more to do
			return;
			}
		}
	// Set the flag to indicate that there is some response body data
	iGotBodyData = ETrue;

	// Inform the client that there is some (more) response data.
	RHTTPTransaction tr = iProtTrans->Transaction();
	tr.SendEventL(
				 THTTPEvent::EGotResponseBodyData,
				 THTTPEvent::EIncoming, 
				 THTTPFilterHandle(THTTPFilterHandle::EProtocolHandler)
				 );
	}

void CWspCORxData::ResponseCompleteL()
	{
	if( iHasTrailer )
		{
		// Inform the client that there are trailer headers
		RHTTPTransaction tr = iProtTrans->Transaction();
		tr.SendEventL(
					 THTTPEvent::EGotResponseTrailerHeaders,
					 THTTPEvent::EIncoming, 
					 THTTPFilterHandle(THTTPFilterHandle::EProtocolHandler)
					 );
		iHasTrailer = EFalse;
		}
	// Tell the observer that the response is complete - this object will
	// be deleted by this call, so is the last thing to be done.
	iObserver->SetStatusL(*this, THTTPEvent::EResponseComplete);
	}

/*
 * Methods from CRxData
 */

void CWspCORxData::ResetRxData()
	{
	User::Panic(KWspPanicCategory, KErrNotSupported);	
	}

/*
 * Methods from MHTTPDataSupplier
 */

TBool CWspCORxData::GetNextDataPart(TPtrC8& aDataPart)
	{
	__ASSERT_DEBUG( iGotBodyData, Panic(KWspPanicNoResponseDataReceived) );

	// Set the data part to the given chunk.
	aDataPart.Set(iBodyData);

	// Set the return value
	return (iLastChunk && !iMoreData);
	}

void CWspCORxData::ReleaseData()
	{
	__ASSERT_DEBUG( iGotBodyData, Panic(KWspPanicNoResponseDataReceived) );

	// Release the data in the body data supplier
	iBodyDataSupplier->ReleaseData();

	// Clear flag that indicates that there is a current data part.
	iGotBodyData = EFalse;

	// Has all the data for this SDU been passed to the client?
	TInt error = KErrNone;
	if( iLastChunk )
		{
		// Ok, all the response data has been received and the client has been 
		// passed all of it - it has just released the last batch. Inform method
		// callback to send the response to the primitive
		iMethodCallback.SendResponsePrimitive();

		// Are there subsequent S-MethodResultData primitives to follow?
		if( !iMoreData )
			{
			// No, have received all data from server - inform observer that the 
			// response is complete.
			TRAP(error, ResponseCompleteL());
			}
		}
	else
		{
		// No, data supplier still has got more - go get it.
		TRAP(error, UpdateBodyDataL());
		}
	// Any problems...?
	if( error != KErrNone )
		{
		// Could not send event - need to abort the method
		iMethodCallback.AbortResponse();
		}
	}
	
TInt CWspCORxData::OverallDataSize()
	{
	return iOverallDataSize;
	}

TInt CWspCORxData::Reset()
	{
	__ASSERT_DEBUG( iBodyDataSupplier, Panic(KWspPanicNoResponseDataReceived) );

	return iBodyDataSupplier->Reset();
	}

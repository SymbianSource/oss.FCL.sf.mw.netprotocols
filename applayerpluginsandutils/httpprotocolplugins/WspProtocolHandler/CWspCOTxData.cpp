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
#include <uri8.h>
#include <wspstringconstants.h>
#include <httpstringconstants.h>
#include <wsp/mwspcapabilityviewer.h>
#include <wsp/wsptypes.h>
#include <wsperror.h>

// User includes
#include <http/rhttpheaders.h>
#include <http/thttphdrval.h>
#include "cwspcotransaction.h"
#include "cwspheaderutils.h"
#include "mwspcotxdatacallback.h"
#include "wsppanic.h"

// Class signature
#include "cwspcotxdata.h"

// Constants used in this module
const TInt KPDUTypeFieldSize			= 1;	// Size in bytes
const TInt KUIntVarOctetShift			= 7;
const TInt KWspTxDataDefaultBufferSize	= 2048;

CWspCOTxData* CWspCOTxData::NewL(CProtTransaction& aTransaction, MWspCOTxDataCallback& aObserver, MWspCapabilityViewer& aNegotiatedCapInfo)
	{
	return new (ELeave) CWspCOTxData(aTransaction, aObserver, aNegotiatedCapInfo);
	}

CWspCOTxData::~CWspCOTxData()
	{
	Cleanup();
	}

CWspCOTxData::CWspCOTxData(CProtTransaction& aTransaction, MWspCOTxDataCallback& aObserver, MWspCapabilityViewer& aNegotiatedCapInfo)
: CTxData(aTransaction), iObserver(aObserver), iNegotiatedCapInfo(aNegotiatedCapInfo)
	{
	}

const TDesC8& CWspCOTxData::RequestHeadersData()
	{
	return *iHeaderData;
	}

MHTTPDataSupplier& CWspCOTxData::RequestBodyData()
	{
	return *this;
	}

void CWspCOTxData::SetRequestDataL()
	{
	// Set the required session properties. Note - assume that the server 
	// message size is at least the size of the server SDU size.
	iRemainingSDUSpace		= iNegotiatedCapInfo.GetServerSDUSize();
	if( iRemainingSDUSpace == 0 )
		{
		// SDU size is unlimited - set to highest value
		iRemainingSDUSpace = KMaxTUint32;
		}
	iRemainingMessageSpace	= iNegotiatedCapInfo.GetServerMessageSize();
	if( iRemainingMessageSpace != 0 )
		{
		// Message is not unlimited
		iLimitedMessageSize = ETrue;
		}
	iSupportLDT				= iNegotiatedCapInfo.GetProtocolOptions() & ELargeDataTransfer;

	// Get the request object from the transaction
	RHTTPTransaction trans	= ProtTrans().Transaction();
	RHTTPRequest request	= trans.Request();

	// Encode the headers - this checks for trailer headers. Trailer header are 
	// only allowed if LDT is supported in the session.
	SetHeaderDataL(request);

	// Headers encoded ok? Check for a request body.
	if( request.HasBody() )
		{
		// Set the remaining server SDU space property
		THTTPHdrVal remainingSDUProperty = iRemainingSDUSpace;
		SetSessionPropertyL(HTTP::EWspRemainingSDUSize, remainingSDUProperty);

		// Set the body data - this will set the iRequestComplete flag
		SetBodyDataL(*request.Body());
		}
	else
		{
		// If there is no body request then the function OverallDataSize will 
		// return zero. The transport handler should not try to access the data.
		iRequestComplete = ETrue;

		// Reset the body data
		iBodyData = KNullDesC8().AllocL();

		// Flag that there is some body data, although of zero length
		iGotBodyData = ETrue;

		// Set state so that primitive is sent
		iState = EDone;
		}
	// Tell oberver what to do.
	DecideObserverAction();

	// Set the remaining message size session property
	THTTPHdrVal remainingMessageSizeProperty = iRemainingMessageSpace;
	SetSessionPropertyL(HTTP::EWspRemainingMessageSize, remainingMessageSizeProperty);
	}

void CWspCOTxData::NotifyMoreRequestData()
	{
	__ASSERT_DEBUG( iState == EWaitForMoreData, Panic(KWspPanicTxDataInInvalidState) );

	// Get the next data part from the client.
	iRequestComplete = iBodyDataSupplier->GetNextDataPart(iDataPart);

	// Was the overall data size known?
	if( iOverallDataSize == -1 )
		{
		TInt dataSize = iDataPart.Length();

		// Is the message size limited?
		if( iLimitedMessageSize )
			{
			// Check to see if this new load of data has blown the message size. 
			// Need to include trailer info too.
			// NOTE - if LDT is not supported, this will be reflected in the message
			// size being the same as the SDU size.
			if( dataSize + iTrailerInfoLength > iRemainingMessageSpace )
				{
				// Too much data - abort the request.
				// NOTE - this could delete this object, so ensure that accessing data
				// members is not done after this call.
				iObserver.AbortInvoke();
				return;
				}
			}
		// Need to check to see if the SDU size is unlimited
		if( iNegotiatedCapInfo.GetServerSDUSize() == 0 )
			{
			// It is so calculate a sensible buffer size
			TPtr8 buf = iBodyData->Des();
			TInt totalDataSize = dataSize + buf.Length();
			if( totalDataSize > buf.MaxLength() )
				{
				TInt bufferSize = ((totalDataSize / KWspTxDataDefaultBufferSize) + 1) * KWspTxDataDefaultBufferSize;

				// Reallocate to a larger buffer
				iBodyData = iBodyData->ReAlloc(bufferSize);
				
				// Was there enough memory?
				if( iBodyData == NULL )
					{
					// Too much data - abort the request.
					// NOTE - this could delete this object, so ensure that accessing data
					// members is not done after this call.
					iObserver.AbortInvoke();
					return;
					}
				}
			}
		}
	// Update the body data
	UpdateBodyData();

	// Send trailer headers?
	if( iState == EDone && iHasTrailer )
		{
		// Cannot send trailer in the first SDU - has the header data been 
		// cleared? This implies that at least one SDU has been sent.
		if( iHeaderData->Des().Length() == 0 )
			{
			// Ensure that they fit in either this SDU or the message as a whole.
			CheckTrailer();
			}
		else
			{
			// This is the first SDU - send trailers in the next one, if they fit.
			if( iLimitedMessageSize && iRemainingMessageSpace - iRemainingSDUSpace < iTrailerInfoLength )
				{
				// Not enough space for the trailers - abort the method.
				iState = EAbortMethod;
				}
			}
		}
	// Tell oberver what to do.
	DecideObserverAction();
	}

void CWspCOTxData::ReceivedCnf()
	{
	__ASSERT_DEBUG( iState == EDone || iState == EGotMoreData, Panic(KWspPanicTxDataInInvalidState) );

	// Zero the encoded headers buffer
	iHeaderData->Des().Zero();

	// Check to see if the request is done.
	if( iState == EDone && !iHasTrailer )
		{
		// Client has sent all the request data and this has all been sent in 
		// the SDUs - done!
		iState = EIdle;

		// Was this a POST-type SDU?
		if( iBodyDataSupplier )
			{
			// Tell client to release the last data chunk.
			iBodyDataSupplier->ReleaseData();
			}

		// Release resources here
		Cleanup();
		}
	else
		{
		// Set the remaining SDU space
		iRemainingSDUSpace = iNegotiatedCapInfo.GetServerSDUSize();
		if( iRemainingSDUSpace == 0 )
			{
			// SDU size is unlimited - set to highest value
			iRemainingSDUSpace = KMaxTUint32;
			}
		if( iLimitedMessageSize && iRemainingSDUSpace > iRemainingMessageSpace )
			{
			// The remaining message space is less than the SDU size - adjust the 
			// remaining SDU space.
			iRemainingSDUSpace = iRemainingMessageSpace;
			}

		// Update the body data
		UpdateBodyData();

		// Send trailer headers?
		if( iState == EDone && iHasTrailer )
			{
			// Ensure that they fit in either this SDU or the message as a whole.
			CheckTrailer();
			}

		// Tell oberver what to do.
		DecideObserverAction();
		}
	}

void CWspCOTxData::SetHeaderDataL(RHTTPRequest aRequest)
	{
	// See how much space is required for URI and header info - initialise with
	// type field length.
	TUint32 infoLength = KPDUTypeFieldSize;

	// Calculate length of uri info
	TUint32 uriLength = aRequest.URI().UriDes().Length();
	TUint32 uriInfoLength = CalculateUIntVarLength(uriLength) + uriLength;

	// Check that the uri fits in the SDU
	infoLength += uriInfoLength;
	if( infoLength > iRemainingSDUSpace )
		{
		// Abort the method - SDU blown with just uri info!
		User::Leave(KWspErrRequestTooBig);
		}

	// Get the encoded headers...
	EncodeHeadersL(aRequest.GetHeaderCollection());

	// Calculate the header info size - this depends on the method
	TUint32 headersLength = iHeaderData->Des().Length();

	switch( aRequest.Method().Index(RHTTPSession::GetTable()) )
   		{
	case HTTP::EOPTIONS: case HTTP::EHEAD: case HTTP::EDELETE: case HTTP::ETRACE:
   	case HTTP::EGET:
   		{
		// Nothing more to do for get-type PDU.
   		} break;
	case HTTP::EPUT:
   	case HTTP::EPOST:
   		{
		// Need to add the UIntVar representation of the headers length
		TUint32 headerInfoLength = CalculateUIntVarLength(headersLength);
		infoLength += headerInfoLength;
		if( infoLength > iRemainingSDUSpace )
			{
			// Abort the method - SDU blown by the header length info!
			User::Leave(KWspErrRequestTooBig);
			}
   		} break;
   	default:
   		__ASSERT_DEBUG(0, Panic(KWspPanicBadMethodType));
   		break;
   		}
	// Check to see if the headers fit into the SDU
	infoLength += headersLength;
	if( infoLength > iRemainingSDUSpace )
		{
		// Abort the method - SDU blown
		User::Leave(KWspErrRequestTooBig);
		}
	// Update the remaining SDU and message space
	iRemainingSDUSpace		-= infoLength;
	if( iLimitedMessageSize )
		iRemainingMessageSpace	-= infoLength;

	// Are there trailer headers?
	if( iHasTrailer )
		{
		// Only allowed if LDT supported
		if( iSupportLDT )
			{
			if( iLimitedMessageSize )
				{
				// Trailers allowed - update message space
				TUint32 trailerLength = iTrailerData->Des().Length();
				iTrailerInfoLength = CalculateUIntVarLength(trailerLength) + trailerLength;

				if( iLimitedMessageSize && iTrailerInfoLength > iRemainingMessageSpace )
					{
					// Abort the method - Message size is blown by the trailer 
					// headers info!
					User::Leave(KWspErrRequestTooBig);
					}
				}
			}
		else
			{
			// Leave - cannot do trailer headers if LDT not supported.
			User::Leave(KWspErrTrailerHeadersNotExpected);
			}
		}
	}

void CWspCOTxData::EncodeHeadersL(RHTTPHeaders aHeaders)
	{
	// Is there a trailer header?
	RStringPool stringPool = ProtTrans().Transaction().Session().StringPool();
	THTTPHdrVal trailer;
	TInt err = aHeaders.GetField(
								stringPool.StringF(WSP::ETrailer, WSP::Table),
								0,		// Zero index -> first part
								trailer
								);
	iHasTrailer = (err == KErrNone);
	CWspCOTransaction& wspTrans = STATIC_CAST(CWspCOTransaction&, ProtTrans());
	if( iHasTrailer )
		{
		// Encode the header and trailer data
		iHeaderData = wspTrans.GetWspHeaderUtils().EncodeNoTrailerHeadersL(stringPool, aHeaders, iTrailerData);
		}
	else
		{
		// Encode the header data
		iHeaderData = wspTrans.GetWspHeaderUtils().EncodeHeadersL(stringPool, aHeaders);
		}
	}

void CWspCOTxData::SetBodyDataL(MHTTPDataSupplier& aRequestBody)
	{
	__ASSERT_DEBUG( iBodyData == NULL, Panic(KWspPanicTxDataInInvalidState) );

	// Store the body data supplier
	iBodyDataSupplier = &aRequestBody;

	// Set the overall data size
	iOverallDataSize = iBodyDataSupplier->OverallDataSize();

	// Is the data sized known?
	if( iLimitedMessageSize && iOverallDataSize != -1 )
		{
		// Check to see if it will fit in the message, including trailer info.
		// NOTE - if LDT is not supported, this will be reflected in the message
		// size being the same as the SDU size.
		if( iOverallDataSize + iTrailerInfoLength > iRemainingMessageSpace )
			{
			// Abort the method - Message size is blown by the request data.
			User::Leave(KWspErrRequestTooBig);
			}
		}
	// Get the data...
	iRequestComplete = iBodyDataSupplier->GetNextDataPart(iDataPart);

	if( iLimitedMessageSize && iOverallDataSize == -1 )
		{
		// Check to see if this data part exceeds the message space.
		// NOTE - if LDT is not supported, this will be reflected in the message
		// size being the same as the SDU size.
		TInt dataSize = iDataPart.Length();
		if( dataSize + iTrailerInfoLength > iRemainingMessageSpace )
			{
			// Abort the method - Message size is blown by the request data.
			User::Leave(KWspErrRequestTooBig);
			}
		// Check to see if there are any more chunks
		if( iRequestComplete )
			{
			// The overall data size is known
			iOverallDataSize = dataSize;
			}
		}

	// Create the buffer
	TInt bufferSize = iNegotiatedCapInfo.GetServerSDUSize();

	// Is the SDU size unlimited?
	if( bufferSize == 0 )
		{
		// Is the overall data size known?
		if( iOverallDataSize != -1 )
			{
			// Set buffer size to be the data size
			bufferSize = iOverallDataSize;
			}
		else
			{
			// Get just enough for this chunk...
			bufferSize = ((iDataPart.Length() / KWspTxDataDefaultBufferSize) + 1) * KWspTxDataDefaultBufferSize;
			}
		}
	// Allocate space for the request data.
	iBodyData = HBufC8::NewL(bufferSize);

	// Update the body data
	UpdateBodyData();
	}

void CWspCOTxData::UpdateBodyData()
	{
	__ASSERT_DEBUG( !iGotBodyData, Panic(KWspPanicTxDataInInvalidState) );

	// The data buffer iBodyData has (at least) a max size of the SDU size. Need
	// to copy enough data so that the SDU (and therefore the buffer) is not 
	// exceeded. 
	
	// Does the given data part exceed the SDU?
	TInt dataLength = iDataPart.Length();
	TInt copyLength = dataLength;
	if( STATIC_CAST(TUint32, dataLength) > iRemainingSDUSpace )
		{
		// Set the copy length to the remaining SDU space.
		copyLength = iRemainingSDUSpace;
		}

	// Copy the required amount of data.
	iBodyData->Des().Append(iDataPart.Left(copyLength));

	// Save remaining data part for later.
	iDataPart.Set(iDataPart.Right(dataLength - copyLength));

	// Update the remaining SDU and message space.
	iRemainingSDUSpace		-= copyLength;
	if( iLimitedMessageSize )
		iRemainingMessageSpace	-= copyLength;

	// Update the state according to the request data state.
	UpdateState();

	// Check the state...
	if( iState == EWaitForMoreData )
		{
		// Release the data from the client - need to get next batch.
		iBodyDataSupplier->ReleaseData();
		}
	else
		{
		// Flag the fact that there is body data
		iGotBodyData = ETrue;
		}
	}

void CWspCOTxData::UpdateState()
	{
	// Possible scenarios;
	// 1) The request is complete and the received data fits into a single SDU.
	// ACTION - MoreData flag is False. State is EDone.
	//
	// 2) The request is complete and the received data requires subsequent SDUs.
	// ACTION - MoreData flag is True. State is EGotMoreData.
	//
	// 3) The request is not complete and the received data does not fill the SDU.
	// ACTION - Need to wait for the next bit of data. State is EWaitForMoreData.
	//
	// 4) The request is not complete and the received fills the SDU.
	// ACTION - MoreData flag is True. Ensure request data given fits in the SDU. 
	//			State is EGotMoreData.
	
	if( iRequestComplete )
		{
		// Possible scenarios 1, 2, 5, 6 and NOTE.
		if( iDataPart.Length() == 0 )
			{
			// No more data to send - scenario 1.
			iState = EDone;
			}
		else
			{
			// Need to send remaining request data later - scenario 2.
			iState = EGotMoreData;
			}
		}
	else
		{
		// Possible scenarios here are 3 and 4.
		if( iRemainingSDUSpace == 0 )
			{
			// No space left in the SDU and need to send it - scenario 4.
			iState = EGotMoreData;
			}
		else
			{
			// There is still space in the SDU. Need to wait for more data from
			// the client - scenario 3.
			iState = EWaitForMoreData;
			}
		}
	}

void CWspCOTxData::CheckTrailer()
	{
	// Do they fit in this SDU?
	if( iLimitedMessageSize && iTrailerInfoLength > iRemainingSDUSpace )
		{
		// Trailers either do not fit in this SDU. Is there enough space 
		// to send them?
		if( iRemainingMessageSpace - iRemainingSDUSpace < iTrailerInfoLength )
			{
			// Not enough space for the trailers - abort the method.
			iState = EAbortMethod;
			}
		}
	else
		{
		// The trailer info fits into this SDU - update the header data.
		delete iHeaderData;
		iHeaderData		= iTrailerData;
		iTrailerData	= NULL;

		// Clear flag indicating that the trailers have yet to be sent.
		iHasTrailer = EFalse;
		}
	}

void CWspCOTxData::DecideObserverAction()
	{
	// Posible actions for the observer - 
	// 1)	If the state is either EDone or EGotMoreData, then the observer 
	//		needs to send a method primitive.
	// 2)	If the state is EAbortMethod, then the observer needs to abort the
	//		method.
	// 3)	If the state is EWaitForMoreData, then the observe does nothing.

	if( iState == EAbortMethod )
		{
		// Something has gone wrong - need to tell observer to abort the
		// method.
		// NOTE - this could delete this object, so ensure that accessing data
		// members is not done after this call.
		iObserver.AbortInvoke();
		}
	else if( iState != EWaitForMoreData )
		{
		// Update the remaining message space - remove the remaining space in
		// this SDU.
		if( iLimitedMessageSize )
			iRemainingMessageSpace -= iRemainingSDUSpace;

		// Tell observer to send a primitive
		iObserver.SendInvokePrimitive();
		}
	}

void CWspCOTxData::Cleanup()
	{
	delete iHeaderData;
	iHeaderData		= NULL;
	delete iBodyData;
	iBodyData		= NULL;
	delete iTrailerData;
	iTrailerData	= NULL;
	}

void CWspCOTxData::SetSessionPropertyL(TInt aProperty, THTTPHdrVal aFieldValue)
	{
	RHTTPSession session			= ProtTrans().Transaction().Session();
	RHTTPConnectionInfo	connInfo	= session.ConnectionInfo();
	RStringPool stringPool			= session.StringPool();

	connInfo.SetPropertyL(
						 stringPool.StringF(aProperty, RHTTPSession::GetTable()), 
						 aFieldValue
						 );
	}

TInt CWspCOTxData::CalculateUIntVarLength(TUint32 aUint)
	{
	TUint8 size = 0; // maximum value is 5 with a 32 bit integer
	TUint32 value = aUint;
	do {
		++size;
		value >>= KUIntVarOctetShift; ; // shift by 7 bits.
		} while (value>0);

	return size;
	}

/*
 * Methods from MHTTPDataSupplier
 */

TBool CWspCOTxData::GetNextDataPart(TPtrC8& aDataPart)
	{
	__ASSERT_DEBUG( iGotBodyData, Panic(KWspPanicNoRequestDataReceived) );

	// Set the data part
	aDataPart.Set(iBodyData->Des());

	// Set the return value
	TBool lastChunk = ETrue;
	if( iState != EDone || iHasTrailer )
		{
		// All the current data has not been sent  - this is not the last chunk.
		lastChunk = EFalse;
		}
	return lastChunk;
	}

void CWspCOTxData::ReleaseData()
	{
	__ASSERT_DEBUG( iGotBodyData, Panic(KWspPanicNoRequestDataReceived) );

	// Clear the flag indicating that there is request data
	iGotBodyData = EFalse;

	// Zero the old body data.
	TPtr8 buf = iBodyData->Des();
	buf.Zero();
	}
	
TInt CWspCOTxData::OverallDataSize()
	{
	return iOverallDataSize;
	}

TInt CWspCOTxData::Reset()
	{
	// This does nothing
	return KErrNotFound;
	}

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

#include "chttpmessagecomposer.h"

#include <http/mhttpdatasupplier.h>
#include <inetprottextutils.h>

#include "mhttpmessagecomposerobserver.h"
#include "thttpmessagepanic.h"

const TInt KDefaultBufferSize	= 128;

// 'this' used in base member initializer list, The 'this' pointer being used is a base class pointer.
#pragma warning( disable : 4355 )

CHttpMessageComposer* CHttpMessageComposer::NewL(MHttpMessageComposerObserver& aObserver)
/**
	Factory constructor. 
	@param		aObserver	The observer for the composer.
	@return		A pointer to a fully constructed and initialised object.
*/
	{
	return new (ELeave) CHttpMessageComposer(aObserver);
	}

CHttpMessageComposer::~CHttpMessageComposer()
/**
	Destructor
*/
	{
	Cancel();

	delete iDataBuffer;
	}

CHttpMessageComposer::CHttpMessageComposer(MHttpMessageComposerObserver& aObserver)
: CActive(CActive::EPriorityStandard + 1), iObserver(aObserver), iDataComposer(*this)
/**
	Constructor.
	We want the request/response to be sent/received and processed as early as possible to avoid the latency by keeping 
	it in the framework. We reduced the scheduling of AOs by 1/6th + 2 when composing and 1/5th + 2 for parsing the header. The value 2 
	indicates the EIdle state and the startline composing/parsing.
*/
	{
	CActiveScheduler::Add(this);
	}

void CHttpMessageComposer::MessageInfoAvailable()
/**
	Notifies the composer that more message info is available. The composer can
	continue composing the message.
	@pre		The composer is waiting for more message info.
	@post		The composer continues composing the message.
	@panic		EHttpMessagePanicBadDataState	The composer was not waiting for
												more info.
*/
	{
	__ASSERT_DEBUG( iDataState == EWaitingForInfo, THttpMessagePanic::Panic(THttpMessagePanic::EHttpMessagePanicBadDataState) ); 

	// Message info available - update data state and continue/start composing.
	iDataState = EGotInfo;
	CompleteSelf();
	}

void CHttpMessageComposer::GetMessageData(TPtrC8& aData)
/**
	Get the current message data.
	@panic		EHttpMessagePanicBadDataState	There is no current data buffer 
												ready.
	@pre		The observer has been notified of available message data.
*/
	{
	__ASSERT_DEBUG( iDataState == EWaitingForRelease, THttpMessagePanic::Panic(THttpMessagePanic::EHttpMessagePanicBadDataState) ); 

	aData.Set(iSendBuffer);
	}

void CHttpMessageComposer::ReleaseMessageData()
/**
	Release the message data. The observer has finished with the current message 
	data buffer. The composer can continue composing the message.
	@panic		EHttpMessagePanicBadDataState	There is no current data buffer 
												ready.
	@pre		The observer has been notified of available message data.
	@post		The composer continues composing the message.
*/
	{
	__ASSERT_DEBUG( iDataState == EWaitingForRelease, THttpMessagePanic::Panic(THttpMessagePanic::EHttpMessagePanicBadDataState) );

	// Is there an entity body?
	if (CheckMessagePendingComplete())
		{
		iObserver.MessageComplete();

		//The composer is done.
		iState = EIdle;
		}
	else
		{
		// Update data state and continue composing.
		iDataState = EGotInfo;
		CompleteSelf();
		}
	}

void CHttpMessageComposer::Reset()
/**
	Composer reset request. As the observer can reset the composer during one of
	the callback functions, the composer must check for re-entrancy to avoid 
	releasing resources that are still required. If the composer is either 
	waiting for more message info, waiting for its data part to be released or 
	is waiting to process its state machine, the composer can safely reset 
	immediately. Otherwise the composer is being reset from within its RunL() 
	and so it must defer resetting itself until a safer point - this is the 
	point in the RunL() where the next step is decided.
	@panic		EHttpMessagePanicDoubleReset	The composer has been reset twice
												in one of the callback functions.
*/
	{
	// Check the data state of the composer - the composer cannot be reset if 
	// the Reset() was called in one of the observer callbacks. It is safe to 
	// reset now if - 
	// 1) the data state is WaitingForInfo
	// 2) the data state is WaitingForRelease
	// 3) the composer is active - waiting to for its RunL() to be called.
	if( iDataState == EWaitingForInfo || iDataState == EWaitingForRelease || IsActive() )
		{
		// Cancel and do the reset.
		Cancel();
		DoReset();
		}
	else
		{
		// Debug check for a double Reset() call...
		__ASSERT_DEBUG( iDataState != EReset, THttpMessagePanic::Panic(THttpMessagePanic::EHttpMessagePanicDoubleReset) );

		// The Reset() was called inside a callback - defer reseting the composer
		// until call stack is back in the composer RunL().
		iDataState = EReset;
		}
	}

void CHttpMessageComposer::CompleteSelf()
/**
	Self-complete function. Ensures that the state machine is processed.
*/
	{
	TRequestStatus* pStat = &iStatus;
	User::RequestComplete(pStat, KErrNone);
	SetActive();
	}

void CHttpMessageComposer::DoReset()
/**
	Resets the composer. The composer moves into the Idle state. Allocated 
	resources are also reset. 
*/
	{
	// Reset the composer - composer state should be Idle and the data state 
	// should be WaitingForInfo.
	iState = EIdle;
	iDataState = EWaitingForInfo;
	iLastChunk = EFalse;

	// Reset the data composer
	iDataComposer.Reset();
	}

CHttpMessageComposer::TComposingStatus CHttpMessageComposer::ComposeStartLineL()
/**
	Composes the message start-line. The start-line can be either a Request-Line
	or a Status-Line. Both have three tokens.

		generic-message	=	start-line
							*(message-header CRLF)
							CRLF
							[ message-body ]
		
		start-line		=	Request-Line | Status-Line

		Request-Line	=	Method SP Request-URI SP HTTP-Version CRLF

		Status-Line		=	HTTP-Version SP Status-Code SP Reason-Phrase CRLF

	The observer supplies these tokens.
	@return		A value of ESectionDone indicating that the start-line has been 
				done.
*/
	{
	__START_PERFORMANCE_LOGGER();
	// Get the start-line tokens from the observer.
	TPtrC8 token1, token2, token3;
	iObserver.StartLineL(token1, token2, token3);

	// Add the tokens to the data buffer.
	iDataComposer.AddTokenL(token1, THttpDataComposer::ESpace);
	iDataComposer.AddTokenL(token2, THttpDataComposer::ESpace);
	iDataComposer.AddTokenL(token3, THttpDataComposer::ECRLF);
	__END_PERFORMANCE_LOGGER(_L(",CHttpMessageComposer::ComposeStartLineL()"));

	return ESectionDone;
	}

CHttpMessageComposer::TComposingStatus CHttpMessageComposer::ComposeHeadersL()
    {
    // Compose maximum of 6 headers at one go without AO scheduling
    TComposingStatus status = ComposeSingleHeaderL();
    if(status == ESectionNotDone)
        {
        status = ComposeSingleHeaderL();
        if(status == ESectionNotDone)
            {
            status = ComposeSingleHeaderL();
            if(status == ESectionNotDone)
                {
                status = ComposeSingleHeaderL();                
                if(status == ESectionNotDone)
                    {
                    status = ComposeSingleHeaderL();                
                		if(status == ESectionNotDone)
                    	{
                    	status = ComposeSingleHeaderL();                
                    	}                    
                    }
                }
            }
        }
    return status;
    }

CHttpMessageComposer::TComposingStatus CHttpMessageComposer::ComposeSingleHeaderL()
/**
	Composes a header field or the end of headers marker. Each header field is
	delimited by a CRLF end of line marker.

		generic-message	=	start-line
							*(message-header CRLF)
							CRLF
							[ message-body ]
		
		message-header	=	field-name ":" [ field-value ]

	The observer supplies the field name and value tokens. If there is no more
	header field info then the end of headers marker is added - the composer is
	now ready to send the start-line and headers section of the message. The 
	optional message-body will follow if there is one. The current data buffer
	is set to contain the start-line and message-headers.
	@return		A value of ESectionNotDone indicating that there are more headers
				to add. A value of ESendData indicating that the current message 
				buffer should be sent.
*/
	{
	__START_PERFORMANCE_LOGGER();
	// Get header field info for the next header from the observer - the return
	// value should indicate if there is any more header info.
	TPtrC8 name, value;
	TInt error = iObserver.NextHeaderL(name, value);

	// Was there any header info?
	TComposingStatus status = ESectionNotDone;
	if( error == KErrNone )
		{
		// Add header info to the data buffer - return default status.
		iDataComposer.AddTokenL(name, THttpDataComposer::EColonSpace);
		iDataComposer.AddTokenL(value, THttpDataComposer::ECRLF);
		}
	else
		{
		// No more headers - add an empty line to mark the end of the headers.
		iDataComposer.AddTokenL(KNullDesC8(), THttpDataComposer::ECRLF);

		// Set the send buffer - need to send it so return ESendData status.
		iSendBuffer.Set(*iDataBuffer);
		status = ESendData;
		}
		__END_PERFORMANCE_LOGGER(_L(",CHttpMessageComposer::ComposeSingleHeaderL()"));
	return status;
	}

CHttpMessageComposer::TComposingStatus CHttpMessageComposer::ComposeChunkSizeL()
/**
	Compose the chunk-size component. The chunk-size component indicates the 
	size of the subsequent chunk-data component.

		chunk			=	chunk-size [ chunk-extension ] CRLF
							chunk-data CRLF
		chunk-size		=	1*HEX

	The composer is ready to send the chunk-size component before sending the
	chunk-data. Also, an empty line marker from a previous chunk-data component
	is included in the send buffer if this is not the first chunk-size component.
	Note that if the chunk-data component is zero-length it should be ignored
	since a zero value chunk-size indicates the last-chunk component.
	@return		A value of ESendData indicating that the current message buffer
				should be sent. A value	of ESectionDone if the chunk is of zero-
				length.
*/
	{
	__START_PERFORMANCE_LOGGER();
	// Get the current data chunk from the data supplier
	TPtrC8 chunk;
	iBodyData->GetNextDataPart(chunk);

	TComposingStatus status = ESendData;
	TInt chunkSize = chunk.Length();
	if( chunkSize > 0 )
		{
		// Convert the size to its HEX representation.
		HBufC8* hex = NULL;
		InetProtTextUtils::ConvertHexToDescriptorL(chunkSize, hex);
		CleanupStack::PushL(hex);

		// Add the chunk-size componennt.
		iDataComposer.AddTokenL(*hex, THttpDataComposer::ECRLF);
		CleanupStack::PopAndDestroy(hex);

		// Set the send buffer - need to send it so return ESendData status.
		iSendBuffer.Set(*iDataBuffer);
		}
	else
		{
		// The chunk is zero-length - this section is done.
		status = ESectionDone;
		}
	__END_PERFORMANCE_LOGGER(_L(",CHttpMessageComposer::ComposeChunkSizeL()"));
	return status;
	}

CHttpMessageComposer::TComposingStatus CHttpMessageComposer::ComposeLastChunkL()
/**
	Composes the last-chunk component. The last-chunk component marks the end
	of the chunked data. 

		last-chunk		=	1*("0") [ chunk-extension ] CRLF

	Optional trailers may follow.
	@return		A value of ESectionDone indicating that the last-chunk component
				has	been added to the message.
*/
	{
	// Add zero to mark the last-chunk.
	_LIT8(KZero, "0");
	iDataComposer.AddTokenL(KZero(), THttpDataComposer::ECRLF);

	// The trailers now follow so this section is done.
	return ESectionDone;
	}

CHttpMessageComposer::TComposingStatus CHttpMessageComposer::ComposeTrailerL()
/**
	Composes a trailer header field. Each trailer header field is delimited by 
	a CRLF end of line marker.

		Chunked-Body	=	*chunk
							last-chunk
							trailer
							CRLF

		trailer			=	*(entity-header CRLF)

		entity-header	=	message-header
		
		message-header	=	field-name ":" [ field-value ]

	The observer supplies the field name and value tokens. If there is no more
	trailer header field info then the end of headers marker is added - the 
	composer is now ready to send the trailer headers section of the message. 
	The last-chunk section is also included in the send buffer.
	@return		A value of ESectionNotDone indicating that there are more headers
				to add. A value of ESendData indicating that the current message 
				buffer should be sent.
*/
	{
	__START_PERFORMANCE_LOGGER();
	// Get trailer header field info for the next trailer from the observer - the
	// return value should indicate if there is any more trailer info.
	TPtrC8 name, value;
	TInt error = iObserver.NextTrailerL(name, value);

	// Was there any trailer info?
	TComposingStatus status = ESectionNotDone;
	if( error == KErrNone )
		{
		// Add trailer info to the data buffer - return default status.
		iDataComposer.AddTokenL(name, THttpDataComposer::EColonSpace);
		iDataComposer.AddTokenL(value, THttpDataComposer::ECRLF);
		}
	else
		{
		// No more trailers - add an empty line to mark the end of the trailers.
		iDataComposer.AddTokenL(KNullDesC8(), THttpDataComposer::ECRLF);

		// Set the send buffer - need to send it so return ESendData status.
		iSendBuffer.Set(*iDataBuffer);
		status = ESendData;
		}
	__END_PERFORMANCE_LOGGER(_L(",CHttpMessageComposer::ComposeTrailerL()"));
	return status;
	}

/*
 *	Methods from MHttpBufferSupplier
 */

void CHttpMessageComposer::ReAllocBufferL(TInt aRequiredSize, TPtr8& aBuffer)
/**	
	Reallocates the data buffer. The composer supplies the data buffer to the
	data composer. The data composer needs more space to store the current token.
	If the data buffer has not been created then it is created, otherwise it is 
	reallocated to at least the required size.
	@param		aRequiredSize	The minimum size of buffer required.
	@param		aBuffer			An output argument set to the reallocated buffer.
	@panic		EInvariantFalse	The required size was less then the current max
								size of the buffer.
*/
	{
	if( iDataBuffer == NULL )
		{
		// Create the buffer..
		iDataBuffer = HBufC8::NewL(aRequiredSize + KDefaultBufferSize);
		}
	else
		{
		__ASSERT_DEBUG( aRequiredSize > iDataBuffer->Des().MaxLength(), User::Invariant() );

		iDataBuffer = iDataBuffer->ReAllocL(aRequiredSize + KDefaultBufferSize);
		}
	aBuffer.Set(iDataBuffer->Des());
	}

void CHttpMessageComposer::DeleteBuffer()
/**
	Deletes the data buffer.
*/
	{
	delete iDataBuffer;
	iDataBuffer = NULL;
	}

/*
 *	Methods from CActive
 */
void CHttpMessageComposer::RunL()
/**
	Asynchronous request service handler. The composer state machine is processed
	in this function. Behaviour depends on the state. The composer will self-
	complete if it can continue composing the message. The composer informs its
	observer when it has message data ready to send. It will then suspend 
	processing until the observer notifies it that it has finished with the 
	current data. If the composer requires more message info it will stop 
	processing until it is notified that more message info is available.
	
	If the observer has reset the composer in one of the callback functions then
	the composer will defer resetting itself until it is back in the RunL().
	@leave		KErrCorrupt	The body data supplier indicated that the last data
							part even though it had not supplied all the data it
							specified. Or the body data supplier has supplied 
							all the data it specified but has not indicated a 
							last data part.
	@panic		EHttpMessagePanicBadDataState	The composer has no info to compose
												the message with.
	@panic		EInvarinatFalse	The composer is trying to send entity body data 
								when there is none to send.
	@panic		EHttpMessagePanicBadComposerStata	The composer is in an illegal
													state.
*/
	{
	__ASSERT_DEBUG( iDataState == EGotInfo, THttpMessagePanic::Panic(THttpMessagePanic::EHttpMessagePanicBadDataState) ); 

	TComposingStatus status = ESectionNotDone;
	switch( iState )
		{
	case EIdle:
		{
		status = ESectionDone;
		iState = ECreatingStartLine;
		} 
		//coverity [MISSING_BREAK]
		// Fallthrough is required here as we no longer compose line by line. We compose startline & headers (6) at one go.
	case ECreatingStartLine:
		{
		status = ComposeStartLineL();
		iState = ECreatingHeaders;
        } 
		//coverity [MISSING_BREAK]
		// Fallthrough is required here as we no longer compose line by line. We compose startline & headers (6) at one go.
	case ECreatingHeaders:
		{
		status = ComposeHeadersL();
		// Is the headers section complete?
		if( status != ESectionNotDone )
			{
			iState = EPendingEntityBody;

			// Is there an entity body?
			iBodyData = iObserver.HasBodyL();
			}
		// Stay in this state if there are more headers to compose.
		} break;
	case EPendingEntityBody:
		{
		// Release the current data buffer
		iDataComposer.Release();

		// Set the amount of the entity body data
		if( iBodyData != NULL )
			iDataSizeLeft = iBodyData->OverallDataSize();
		else
			iDataSizeLeft = 0;

		// Determine next action depending on amount of entity data to send.
		if( iDataSizeLeft > 0 )
			{
			// A known amount of data greater than zero. Send entity body with
			// no transfer encoding. 
			iState = ESendEntityData;
			}
		else if( iDataSizeLeft == 0 )
			{
			// No entity body or zero length entity body. The message is 
			// complete. 
			iState = EPendingIdle;
			}
		else
			{
			// An unknown amount of data. Send entity body with chunked transfer
			// encoding. 
			iState = ECreatingChunkSize;
			}
		status = ESectionDone;
		} break;
	case ESendEntityData:
		{
		__ASSERT_DEBUG( !iLastChunk, User::Invariant() );

		// Get the next entity body part
		iLastChunk = iBodyData->GetNextDataPart(iSendBuffer);

		// Only send data if there is any to send.
		TInt dataLength = iSendBuffer.Length();
		if( dataLength > 0 )
			status = ESendData;
		else
			status = ESectionDone;

		// Update how much data is left to send.
		iDataSizeLeft -= dataLength;

		// Check whether the client is sending more/less data than it should.
		if( (iDataSizeLeft < 0) || (iDataSizeLeft > 0 && iLastChunk) )
			User::Leave(KErrCorrupt);

		iState = EPendingReleaseData;
		} break;
	case EPendingReleaseData:
		{
		// Release the current data part.
		iBodyData->ReleaseData();

		// Has the last entity body part been released?
		if( iLastChunk )
			{
			// Yep, message is complete.
			status = ESectionDone;
			iState = EPendingIdle;
			}
		else
			{
			// Stop composing until more data received.
			status = EStop;
			iState = ESendEntityData;
			}
		} break;
	case ECreatingChunkSize:
		{
		status = ComposeChunkSizeL();
		iState = ESendChunkData;
		} break;
	case ESendChunkData:
		{
		// Release the current data buffer
		iDataComposer.Release();

		// Get the chunk-data from the body data supplier.
		iLastChunk = iBodyData->GetNextDataPart(iSendBuffer);

		// Only send data if there is any to send.
		if( iSendBuffer.Length() > 0 )
			status = ESendData;
		else
			status = ESectionDone;
		
		// Move to the PendingReleaseChunk state.
		iState = EPendingReleaseChunk;
		} break;
	case EPendingReleaseChunk:
		{
		// Release the current data part.
		iBodyData->ReleaseData();

		// RFC2616 states - 
		//
		// chunk			=	chunk-size [ chunk-extension ] CRLF
		//						chunk-data CRLF
		// 
		// Therefore need to add CRLF marker.
		iDataComposer.AddTokenL(KNullDesC8(), THttpDataComposer::ECRLF);

		// Has the last entity body part been released?
		  if ( iLastChunk )
			{
			// Yep - mark with last-chunk component
			status = ComposeLastChunkL();
			iState = ECreatingTrailers;
			}
		else
			{
			// Stop composing until more data received.
			status = EStop;
			iState = ECreatingChunkSize;
			}
		} break;
	case ECreatingTrailers:
		{
		status = ComposeTrailerL();

		// Have all the trailers been added?
		if( status != ESectionNotDone )
			{
			iState = EPendingEndOfChunkedBody;
			}
		// Stay in this state if there are more trailers to compose.
		} break;
	case EPendingEndOfChunkedBody:
		{
		// Release the current data buffer.
		iDataComposer.Release();

		// The message is complete.
		status = ESectionDone;
		iState = EPendingIdle;
		} break;
	case EPendingIdle:
		{
		// The message is complete - inform the observer.
		iObserver.MessageComplete();

		// The composer is done.
		iState = EIdle;
		status = EStop;
		} break;
	default:
		THttpMessagePanic::Panic(THttpMessagePanic::EHttpMessagePanicBadComposerState);
		break;
		}
	// Determine what to do next...
	if( iDataState == EReset )
		{
		// The observer has reset the composer during one of the callbacks - it 
		// is now safe to do the reset.
		DoReset();
		}
	else if( status == ESectionDone || status == ESectionNotDone )
		{
		// Need to keep composing - complete-self.
		CompleteSelf();
		}
	else if( status == ESendData )
		{
		// Update data state...
		iDataState = EWaitingForRelease;

		// Inform the observer that there is message data to send.
		iObserver.MessageDataReadyL();
		}
	else
		{
		// Data state is EStop - stop composing and wait for more message data.
		iDataState = EWaitingForInfo;
		}
	}

void CHttpMessageComposer::DoCancel()
/**
	Asynchronous request cancel. This function does nothing as the only asynch
	request that is made is a self-complete request.
*/
	{
	// Do nothing...
	}

TInt CHttpMessageComposer::RunError(TInt aError)
/**
	Asynchronous request service error handler. An error has occured whilst
	processing the state machine. Reset the composer and notify the observer of 
	the error.
	@param		aError	The error code.
	@return		An interger value of KErrNone indicates that the error has been
				handled.
	@post		The composer has been reset.
*/
	{
	// Stop composing and reset.
	DoReset();

	// Notify the observer of the error
	return iObserver.HandleComposeError(aError);
	}

TBool CHttpMessageComposer::CheckMessagePendingComplete()
/**
	Check if the message has been completely sent. 
	i.e. the headers have been composed and sent and there is no body to send.
	Only waiting for a confirmation socket that it has in fact been sent. 

	@return		TBool ETrue if the message is complete.
	@post		The composer has been reset.
*/
	{
	return (iState == EPendingEntityBody && iBodyData==NULL) ? ETrue:EFalse;
	}


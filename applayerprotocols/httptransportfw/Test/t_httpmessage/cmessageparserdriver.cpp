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

#include "cmessageparserdriver.h"

#include "mdriverobserver.h"
#include "cmessagedatasupplier.h"

CMessageParserDriver* CMessageParserDriver::NewL(MDriverObserver& aObserver)
	{
	CMessageParserDriver* self = new(ELeave) CMessageParserDriver(aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CMessageParserDriver::~CMessageParserDriver()
	{
	Cancel();
	delete iDataSupplier;
	iHeaders.Close();
	iTrailers.Close();
	iMessageParser.Close();
	}

CMessageParserDriver::CMessageParserDriver(MDriverObserver& aObserver)
: CActive(CActive::EPriorityStandard), iObserver(aObserver)
	{
	CActiveScheduler::Add(this);
	}

void CMessageParserDriver::ConstructL()
	{
	iMessageParser.OpenL(*this);
	}

void CMessageParserDriver::SetMessageData(const TDesC8& aMessageData)
	{
	iMessageData.Set(aMessageData);
	}

void CMessageParserDriver::SetStartLine(const TDesC8& aStartLine)
	{
	iStartLine.Set(aStartLine);
	}

void CMessageParserDriver::SetHeaderL(const TDesC8& aFieldName, const TDesC8& aFieldValue)
	{
	User::LeaveIfError(iHeaders.Append(THeaderField(aFieldName, aFieldValue)));
	}

void CMessageParserDriver::SetBodyData(const TDesC8& aBodyData, TBool aIsChunked, TBool aIsUnknownLength)
	{
	iEntityBody.Set(aBodyData);
	iIsUnknownLength = aIsUnknownLength;

	// Set the entity body info
	if( iEntityBody.Length() > 0 )
		{
		if( aIsChunked )
			{
			iExpectBodyState = EChunkEncodedBody;
			}
		else
			{
			if( iIsUnknownLength )
				iExpectBodyState = EUnknownBodyLength;
			else
				iExpectBodyState = ENonEncodedBody;
			}
		}
	else
		{
		iExpectBodyState = ENoEntityBody;
		}
	}

void CMessageParserDriver::SetTrailerL(const TDesC8& aFieldName, const TDesC8& aFieldValue)
	{
	User::LeaveIfError(iTrailers.Append(THeaderField(aFieldName, aFieldValue)));
	}

void CMessageParserDriver::Start(TInt aExpectedError, TBool aTestReset)
	{
	iExpectedError	= aExpectedError;
	iDoReset		= aTestReset;

	// Inform the observer that the test has started
	iObserver.NotifyStart();

	// Initially set the buffer size to the length of the message unless doing
	// reset test - use a buffer size of an eighth of the message size
	if( iDoReset )
		{
		iBufferSize = iMessageData.Length()/8;
		if( iBufferSize < 1 )
			iBufferSize = 1;
		}
	else
		iBufferSize = iMessageData.Length();

	// Set the reset state to initial state - PendingStartLine.
	iResetState = EPendingStartLine;

	// Self-complete to kick-off the test.
	CompleteSelf();
	}

void CMessageParserDriver::CompleteSelf()
	{
	TRequestStatus* pStat = &iStatus;
	User::RequestComplete(pStat, KErrNone);
	SetActive();
	}

void CMessageParserDriver::DoReset()
	{
	// Reset the parser and update the reset state.
	iMessageParser.Reset();
	iResetState = iState;

	iObserver.Log(_L("RESET - starting test again!!"));

	// Reset the data supplier
	delete iDataSupplier;
	iDataSupplier = NULL;

	if( iState == EPendingHeaders )
		iResetIndex = iHeaderIndex;
	else if( iState == EPendingTrailers )
		iResetIndex = iTrailerIndex;
	else if( iState == EPendingBodySize )
		iResetIndex = 0;
	else if( iState == EPendingBody )
		iResetIndex = iBodyIndex;

	// Start the test over...
	CompleteSelf();
	}

/*
 *	Methods from CActive
 */

void CMessageParserDriver::RunL()
	{
	// Reset the test case data
	iHeaderIndex	= 0;
	iTrailerIndex	= 0;
	iExpectBodyData.Set(iEntityBody);

	// Set the reset flags
	iReset		= EFalse;
	iBodyIndex	= 0;
	iChunkCount	= 0;

	// Reset the data supplier
	delete iDataSupplier;
	iDataSupplier = NULL;

	iDataSupplier = CMessageDataSupplier::NewL(*this, iMessageData, iBufferSize);

	TBuf<64> comment;
	comment.Format(_L("STARTING PARSING -> buffer size : %d"), iBufferSize);
	iObserver.Log(comment);

	// Start the parser...
	iMessageParser.ReceivedMessageData();

	// Move to PendingStartLine state...
	iState = EPendingStartLine;
	}

void CMessageParserDriver::DoCancel()
	{
	// Do nothing...
	}

TInt CMessageParserDriver::RunError(TInt aError)
	{
	// Inform the observer of the error
	iObserver.NotifyError(aError);
	return KErrNone;
	}

/*
 *	Methods from MHttpMessageParserObserver
 */

void CMessageParserDriver::GetDataPacket(TPtrC8& aData)
	{
	__ASSERT_DEBUG( iTestFailed == EFalse, User::Invariant() );

	if( iReset )
		{
		// Not expecting a header. Test has failed - notify the observer.
		iObserver.Log(_L("FAIL - not expecting GetDataPacket"));
		iObserver.NotifyError(KErrNotFound);
		iTestFailed = ETrue;
		}

	iDataSupplier->GetData(aData);
	}

void CMessageParserDriver::ReleaseDataPacket()
	{
	__ASSERT_DEBUG( iTestFailed == EFalse, User::Invariant() );

	if( iReset )
		{
		// Not expecting a header. Test has failed - notify the observer.
		iObserver.Log(_L("FAIL - not expecting ReleaseDataPacket"));
		iObserver.NotifyError(KErrNotFound);
		iTestFailed = ETrue;
		}

	iDataSupplier->ReleaseData();
	
	// If we have recieved all the data and we were doing a 1.0 request of unknown length
	// tell the parser the data is complete and we are waiting for the body to be complete.
	if( iExpectBodyData.Length() == 0 && iIsUnknownLength && iState==EPendingBodyComplete)
		{
		TRAPD(err, iMessageParser.CompletedBodyDataL() );
		if( err != KErrNone )
			{
			// Not expecting a header. Test has failed - notify the observer.
			iObserver.Log(_L("FAIL - not expecting ReleaseDataPacket"));
			iObserver.NotifyError(err);
			iTestFailed = ETrue;
			}
		}
		
	}

void CMessageParserDriver::StartLineL(const TDesC8& aStartLine)
	{
	__ASSERT_DEBUG( iTestFailed == EFalse, User::Invariant() );

	iObserver.Log(_L("Start-line received"));
	TBuf<256> comment;
	comment.Copy(aStartLine);
	iObserver.Log(comment);

	// Was start-line expected?
	if( iState != EPendingStartLine || iReset )
		{
		// Not expecting start-line. Test has failed - notify the observer.
		iObserver.Log(_L("FAIL - not expecting start-line"));
		iObserver.NotifyError(KErrNotFound);
		iTestFailed = ETrue;
		}
	else
		{
		// Check that the start-line is correct.
		if( aStartLine.Compare(iStartLine) == 0 )
			{
			// Should a reset occur here?
			if( iDoReset && iResetState == iState )
				iReset = ETrue;

			// Match - what is next?
			if( iHeaders.Count() == 0 )
				iState = EPendingBodySize;
			else
				iState = EPendingHeaders;
            // We no longer parse line by line. We parse startline & headers (5) at one go.
            // We cannot reset here
            iReset = EFalse;
			// Reset?
			if( iDoReset && iReset )
				DoReset();
			}
		else
			{
			// The start-line does not match. Test has failed - notify the observer.
			iObserver.Log(_L("FAIL - received incorrect start-line"));
			iObserver.NotifyError(KErrNotFound);
			iTestFailed = ETrue;
			}
		}
	}

void CMessageParserDriver::HeaderL(const TDesC8& aFieldName, TDesC8& aFieldValue)
	{
	__ASSERT_DEBUG( iTestFailed == EFalse, User::Invariant() );

	iObserver.Log(_L("HeaderL received"));

	// Was a header expected? NOTE - this can be a trailer header...
	if( (iState != EPendingHeaders && iState != EPendingTrailers) || iReset )
		{
		// Not expecting a header. Test has failed - notify the observer.
		iObserver.Log(_L("FAIL - not expecting HeaderL"));
		iObserver.NotifyError(KErrNotFound);
		iTestFailed = ETrue;
		}
	else
		{
		// Yep - log the parsed header...
		TBuf<64> name;
		name.Copy(aFieldName);
		TBuf<256> value;
		value.Copy(aFieldValue);
		TBuf<322> comment;
		comment.Format(_L("%S: %S"), &name, &value);
		iObserver.Log(comment);

		// Get the expected header info...
		THeaderField header;
		if( iState == EPendingHeaders )
			{
			// Should a reset occur here?
			if( iDoReset && (iResetState == iState && iResetIndex == iHeaderIndex) )
				iReset = ETrue;

			// Get the current header info...
			header = iHeaders[iHeaderIndex];

			// Loop to next header...
			++iHeaderIndex;

			// Set next state...
			if( iHeaderIndex < iHeaders.Count() )
				iState = EPendingHeaders;
			else
				iState = EPendingBodySize;
			}
		else
			{
			// Should a reset occur here?
			if( iDoReset && (iResetState == iState && iResetIndex == iTrailerIndex) )
				iReset = ETrue;

			// Get the current trailer info...
			header = iTrailers[iTrailerIndex];

			// Loop to next header...
			++iTrailerIndex;

			// Set next state...
			if( iTrailerIndex < iTrailers.Count() )
				iState = EPendingTrailers;
			else
				iState = EPendingMessageComplete;
			}
		TPtrC8 expectedName		= header.iName;
		TPtrC8 expectedValue	= header.iValue;

		// Check that the parsed header info matches expected
		if( expectedName.Compare(aFieldName) != 0 || expectedValue.Compare(aFieldValue) != 0 )
			{
			// The header info does not match. Test has failed - notify the observer.
			iObserver.Log(_L("FAIL - received incorrect header info"));
			iObserver.NotifyError(KErrNotFound);
			iTestFailed = ETrue;
			}
		else if( iDoReset && iReset )
			DoReset();
		}
	}

TInt CMessageParserDriver::BodySizeL()
	{
	__ASSERT_DEBUG( iTestFailed == EFalse, User::Invariant() );

	// Log the headers complete event
	iObserver.Log(_L("BodySizeL received"));

	// Was this expected?
	TInt size = MHttpMessageParserObserver::ENoBody;
	if( iState != EPendingBodySize || iReset )
		{
		// Not expected! Test has failed - notify the observer.
		iObserver.Log(_L("FAIL - not expecting BodySizeL"));
		iObserver.NotifyError(KErrNotFound);
		iTestFailed = ETrue;
		}
	else
		{
		// Should a reset occur here?
		if( iDoReset && iResetState == iState )
			iReset = ETrue;

		// Yep - workout what is next...
		switch( iExpectBodyState )
			{
		case ENoEntityBody:
			{
			// No body - use default value of size
			iState = EPendingMessageComplete;
			} break;
		case EChunkEncodedBody:
			{
			// There is a body that is chunk encoded.
			size = MHttpMessageParserObserver::EChunked;
			iState = EPendingBody;
			} break;
		case ENonEncodedBody:
			{
			// There is a body that is not encoded.
			size = iExpectBodyData.Length();
			iState = EPendingBody;
			} break;
		case EUnknownBodyLength:
			{
			// There is a body of unknown length
			size = MHttpMessageParserObserver::EUnknown;
			iState = EPendingBody;
			} break;
		default:
			User::Invariant();
			break;
			}

		// Reset?
		if( iDoReset && iReset )
			DoReset();
		}
	return size;
	}

void CMessageParserDriver::BodyChunkL(const TDesC8& aData)
	{
	__ASSERT_DEBUG( iTestFailed == EFalse, User::Invariant() );

	iObserver.Log(_L("BodyChunkL received"));

	// Was this expected?
	if( iExpectBodyState != EChunkEncodedBody && (iState != EPendingBody || iReset ))
		{
		// No body data expected. Test has failed - notify the observer.
		iObserver.Log(_L("FAIL - not expecting BodyChunkL"));
		iObserver.NotifyError(KErrNotFound);
		iTestFailed = ETrue;
		}
	else
		{
		// Dump the body data...
		iObserver.Dump(aData);

		// Check to see if it matches the expected data.
		TBool match = EFalse;
		if( aData.Length() <= iExpectBodyData.Length() )
			{
			// Is the content the same
			if( aData.Compare(iExpectBodyData.Left(aData.Length())) == 0 )
				{
				// Data is the same - update the expected data
				iExpectBodyData.Set(iExpectBodyData.Mid(aData.Length()));
				match = ETrue;
				}
			}
		if( match )
			{
			// Should a reset occur here?
			if( iDoReset && (iResetState == iState && iResetIndex == iBodyIndex) )
				iReset = ETrue;

			++iBodyIndex;

			// Has all the data been received?
			if( iExpectBodyData.Length() == 0 )
				iState = EPendingBodyComplete;

			// Reset?
			if( iDoReset && iReset )
				DoReset();
			}
		else
			{
			// The body data does not match. Test has failed - notify the observer.
			iObserver.Log(_L("FAIL - received incorrect body data"));
			iObserver.NotifyError(KErrNotFound);
			iTestFailed = ETrue;
			}
		}
	}

void CMessageParserDriver::BodyCompleteL()
	{
	__ASSERT_DEBUG( iTestFailed == EFalse, User::Invariant() );

	iObserver.Log(_L("BodyCompleteL received"));

	// Was this expected?
	if( iState != EPendingBodyComplete || iReset )
		{
		// Not expecting body complete. Test has failed - notify the observer.
		iObserver.Log(_L("FAIL - not expecting BodyCompleteL"));
		iObserver.NotifyError(KErrNotFound);
		iTestFailed = ETrue;
		}
	else
		{
		// Should a reset occur here?
		if( iDoReset && iResetState == iState )
			iReset = ETrue;

		// What's next?
		if( iExpectBodyState == EChunkEncodedBody && iTrailers.Count() > 0 )
			iState = EPendingTrailers;
		else
			iState = EPendingMessageComplete;

		// Reset?
		if( iDoReset && iReset )
			DoReset();
		}
	}

void CMessageParserDriver::MessageCompleteL(const TPtrC8& /*aExcessData*/)
	{
	__ASSERT_DEBUG( iTestFailed == EFalse, User::Invariant() );

	// Log the message complete event
	iObserver.Log(_L("Message Complete"));

	// Was this expected?
	if( iState != EPendingMessageComplete || iReset )
		{
		// Not expecting message complete. Test has failed - notify the observer.
		iObserver.Log(_L("FAIL - not expecting message complete"));
		iObserver.NotifyError(KErrNotFound);
		iTestFailed = ETrue;
		}
	else
		{
		// Should a reset occur here?
		if( iDoReset && iResetState == iState )
			iReset = ETrue;

		// Next state...
		iState = EDone;

		// Reset? Or is the buffer size small enough?
		if( iDoReset && iReset )
			DoReset();
		else if( !iDoReset && iBufferSize > 1 )
			{
			// Nope - half and do test again!!
			iBufferSize /= 2;
			iResetState = EPendingStartLine;
			CompleteSelf();

			iObserver.Log(_L("RE-PARSE WITH SMALLER BUFFER"));
			}
		else
			{
			// Yep - test done...
			iObserver.NotifyComplete();
			}
		}
	}

TInt CMessageParserDriver::HandleParserError(TInt aError)
	{
	// Is an error expected?
	if( iExpectedError == aError )
		{
		TBuf<64> comment;
		comment.Format(_L("Received expected error : %d"), aError);
		iObserver.Log(comment);

		// Yep and received expected - is the buffer size small enough?
		if( !iDoReset && iBufferSize > 1 )
			{
			// Nope - halve and do test again!!
			iBufferSize /= 2;
			CompleteSelf();

			iObserver.Log(_L("RE-PARSE WITH SMALLER BUFFER"));
			}
		else
			{
			// Yep - test done...
			iObserver.NotifyComplete();
			}
		}
	else
		{
		// Inform the observer of the error
		iObserver.NotifyError(aError);
		}
	return KErrNone;
	}

void CMessageParserDriver::Reserved_MHttpMessageParserObserver()
	{
	User::Invariant();
	}

/*
 *	Methods from MDataSupplierObserver
 */

void CMessageParserDriver::DataReady()
	{
	if( iDoReset && iChunkIndex == iChunkCount )
		{
		// Reset the parser and update the chunk index
		iMessageParser.Reset();
		++iChunkIndex;

		iObserver.Log(_L("RESET - starting test again!!"));

		// Reset the data supplier
		delete iDataSupplier;
		iDataSupplier = NULL;

		// Start the test over...
		CompleteSelf();
		}
	else
		{
		// Inc the chunk count and notify the parser of more data
		++iChunkCount;
		iMessageParser.ReceivedMessageData();
		}
	}

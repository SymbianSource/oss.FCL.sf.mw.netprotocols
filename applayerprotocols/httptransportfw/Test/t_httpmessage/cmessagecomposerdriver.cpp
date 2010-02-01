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

#include "cmessagecomposerdriver.h"

#include "mdriverobserver.h"
#include "cbodydatasupplier.h"

CMessageComposerDriver* CMessageComposerDriver::NewL(MDriverObserver& aObserver)
	{
	CMessageComposerDriver* self = new(ELeave) CMessageComposerDriver(aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CMessageComposerDriver::~CMessageComposerDriver()
	{
	iHeaders.Close();
	iTrailers.Close();
	iMessageComposer.Close();
	delete iBodyData;
	}

CMessageComposerDriver::CMessageComposerDriver(MDriverObserver& aObserver)
: CActive(CActive::EPriorityStandard), iObserver(aObserver)
	{
	CActiveScheduler::Add(this);
	}

void CMessageComposerDriver::ConstructL()
	{
	iMessageComposer.OpenL(*this);
	}

void CMessageComposerDriver::SetMessageData(const TDesC8& aMessageData)
	{
	iMessageData.Set(aMessageData);
	}

void CMessageComposerDriver::SetStartLine(const TDesC8& aToken1, const TDesC8& aToken2, const TDesC8& aToken3)
	{
	iStartLine1.Set(aToken1);
	iStartLine2.Set(aToken2);
	iStartLine3.Set(aToken3);
	}

void CMessageComposerDriver::SetHeaderL(const TDesC8& aFieldName, const TDesC8& aFieldValue)
	{
	User::LeaveIfError(iHeaders.Append(THeaderField(aFieldName, aFieldValue)));
	}

void CMessageComposerDriver::SetBodyDataL(const TDesC8& aBodyPart)
	{
	if( iBodyData == NULL )
		iBodyData = CBodyDataSupplier::NewL(*this);

	iBodyData->AddBodyPartL(aBodyPart);
	}

void CMessageComposerDriver::SetBodySize(TInt aSize)
	{
	__ASSERT_DEBUG( iBodyData, User::Invariant() );
	
	iBodyData->SetBodySize(aSize);
	}

void CMessageComposerDriver::SetTrailerL(const TDesC8& aFieldName, const TDesC8& aFieldValue)
	{
	User::LeaveIfError(iTrailers.Append(THeaderField(aFieldName, aFieldValue)));
	}

void CMessageComposerDriver::Start(TInt aExpectedError, TBool aTestReset)
	{
	iExpectedError	= aExpectedError;
	iDoReset		= aTestReset;

	// Inform the observer that the test has started
	iObserver.NotifyStart();

	// Set the reset state to initial state - PendingStartLine.
	iResetState = EPendingStartLine;

	// Self-complete to kick-off the test.
	CompleteSelf();
	}

void CMessageComposerDriver::CompleteSelf()
	{
	TRequestStatus* pStat = &iStatus;
	User::RequestComplete(pStat, KErrNone);
	SetActive();
	}

void CMessageComposerDriver::DoReset()
	{
	// Reset the composer and update the reset state.
	iMessageComposer.Reset();
	iResetState = iState;

	iObserver.Log(_L("RESET - starting test again!!"));

	// Reset the data supplier
	if( iBodyData != NULL )
		iBodyData->ResetSupplier();

	if( iState == EPendingNextHeader )
		iResetIndex = iHeaderIndex;
	else if( iState == EPendingNextTrailer )
		iResetIndex = iTrailerIndex;
	else if( iState == EPendingHasBody )
		iChunkIndex = 0;

	// Start the test over...
	CompleteSelf();
	}

/*
 *	Methods from CActive
 */

void CMessageComposerDriver::RunL()
	{
	// Reset the indexes
	iHeaderIndex = 0;
	iTrailerIndex = 0;

	// Set the reset flags
	iReset			= EFalse;
	iMessageChunk	= 0;
	iChunkCount		= 0;

	// Reset the expected data..
	iExpectedData.Set(iMessageData);

	// Start the composer...
	iMessageComposer.MessageInfoAvailable();

	// Move to PendingStartLine state...
	iState = EPendingStartLine;
	}

void CMessageComposerDriver::DoCancel()
	{
	// Do nothing...
	}

TInt CMessageComposerDriver::RunError(TInt aError)
	{
	// Inform the observer of the error
	iObserver.NotifyError(aError);

	return KErrNone;
	}

/*
 *	Methods from MHttpMessageComposerObserver
 */

void CMessageComposerDriver::StartLineL(TPtrC8& aToken1, TPtrC8& aToken2, TPtrC8& aToken3)
	{
	__ASSERT_DEBUG( iTestFailed == EFalse, User::Invariant() );

	iObserver.Log(_L("StartLineL received"));

	// Was StartLineL expected?
	if( iState != EPendingStartLine || iReset )
		{
		// Not expecting start-line. Test has failed - notify the observer.
		iObserver.Log(_L("FAIL - not expecting StartLineL request"));
		iObserver.NotifyError(KErrNotFound);
		iTestFailed = ETrue;
		}
	else
		{
		// Should a reset occur here?
		if( iDoReset && iResetState == iState )
			iReset = ETrue;

		// Set the tokens...
		aToken1.Set(iStartLine1);
		aToken2.Set(iStartLine2);
		aToken3.Set(iStartLine3);

		// Set the next state...
		iState = EPendingNextHeader;
        // We no longer compose line by line. We compose startline & headers (6) at one go.
        // We cannot reset here
        iReset = EFalse;
		if( iReset )
			DoReset();
		}
	}

TInt CMessageComposerDriver::NextHeaderL(TPtrC8& aHeaderName, TPtrC8& aHeaderValue)
	{
	__ASSERT_DEBUG( iTestFailed == EFalse, User::Invariant() );

	iObserver.Log(_L("NextHeaderL received"));

	// Was NextHeaderL expected?
	TInt error = ( iHeaderIndex < iHeaders.Count() ) ? KErrNone : KErrNotFound;
	if( iState != EPendingNextHeader || iReset )
		{
		// Not expecting NextHeader. Test has failed - reset the composer and
		// notify the observer.
		iObserver.Log(_L("FAIL - not expecting NextHeaderL request"));
		iObserver.NotifyError(KErrNotFound);
		iTestFailed = ETrue;
		}
	else
		{
		// Should a reset occur here?
		if( iDoReset && (iResetState == iState && iResetIndex == iHeaderIndex) )
			iReset = ETrue;

		// Are there any headers left to give? 
		if( iHeaderIndex < iHeaders.Count() )
			{
			// Give the current header info...
			THeaderField header = iHeaders[iHeaderIndex];

			aHeaderName.Set(header.iName);
			aHeaderValue.Set(header.iValue);

			// Loop to next header...
			++iHeaderIndex;
			}
		else
			{
			// No more headers - change state
			iState = EPendingHasBody;
			}

		if( iReset )
			DoReset();
		}
	// Return KErrNotFound if this is the last header.
	return error;
	}

MHTTPDataSupplier* CMessageComposerDriver::HasBodyL()
	{
	__ASSERT_DEBUG( iTestFailed == EFalse, User::Invariant() );

	iObserver.Log(_L("HasBodyL received"));

	// Was start-line expected?
	if( iState != EPendingHasBody )
		{
		// Not expecting HasBodyL. Test has failed - notify the observer.
		iObserver.Log(_L("FAIL - not expecting HasBodyL request"));
		iObserver.NotifyError(KErrNotFound);
		iTestFailed = ETrue;
		}
	else
		{
		// Should a reset occur here?
//		if( iDoReset && iResetState == iState )
//			iReset = ETrue;

		// Set the next state...
		if( iBodyData != NULL && iBodyData->IsChunked() )
			{
			// Have got a chunked body - expect NextTrailersL...
			iState = EPendingNextTrailer;
			}
		else
			{
			// Either no body or non-encoded body - expect MessageComplete
			iState = EPendingMessageComplete;
			}

//		if( iReset )
// 		DoReset();
		}
	return iBodyData;
	}

TInt CMessageComposerDriver::NextTrailerL(TPtrC8& aHeaderName, TPtrC8& aHeaderValue)
	{
	__ASSERT_DEBUG( iTestFailed == EFalse, User::Invariant() );

	iObserver.Log(_L("NextTrailerL received"));

	// Was NextTrailerL expected?
	TInt error = ( iTrailerIndex < iTrailers.Count() ) ? KErrNone : KErrNotFound;
	if( iState != EPendingNextTrailer || iReset )
		{
		// Not expecting NextHeader. Test has failed - notify the observer.
		iObserver.Log(_L("FAIL - not expecting NextTrailerL request"));
		iObserver.NotifyError(KErrNotFound);
		iTestFailed = ETrue;
		}
	else
		{
		// Should a reset occur here?
		if( iDoReset && (iResetState == iState && iResetIndex == iTrailerIndex) )
			iReset = ETrue;

		// Are there any headers left to give? 
		if( iTrailerIndex < iTrailers.Count() )
			{
			// Give the current header info...
			THeaderField header = iTrailers[iTrailerIndex];

			aHeaderName.Set(header.iName);
			aHeaderValue.Set(header.iValue);

			// Loop to next header...
			++iTrailerIndex;
			}
		else
			{
			// No more trailers - change state
			iState = EPendingMessageComplete;
			}

		if( iReset )
			DoReset();
		}
	// Return KErrNotFound if this is the last header.
	return error;
	}

void CMessageComposerDriver::MessageComplete()
	{
	__ASSERT_DEBUG( iTestFailed == EFalse, User::Invariant() );

	// Log the message complete event
	iObserver.Log(_L("Message Complete received"));

	// Was this expected?
	if( iState != EPendingMessageComplete || iReset )
		{
		// Not expecting message complete. Test has failed - notify the observer
		iObserver.Log(_L("FAIL - not expecting message complete"));
		iObserver.NotifyError(KErrNotFound);
		iTestFailed = ETrue;
		}
	else
		{
		// Has all the data been received?
		if( iExpectedData.Length() == 0 )
			{
			// Should a reset occur here?
			if( iDoReset && iResetState == iState )
				iReset = ETrue;

			iState = EDone;

			if( iReset )
				DoReset();
			else
				{// Test done...
				iObserver.NotifyComplete();
				}
			}
		else
			{
			// Oh dear, not all data has been received - test failed.
			iObserver.Log(_L("FAIL - all message data not received"));
			iObserver.NotifyError(KErrNotFound);
			iTestFailed = ETrue;
			}
		}
	}

void CMessageComposerDriver::MessageDataReadyL()
	{
	__ASSERT_DEBUG( iTestFailed == EFalse, User::Invariant() );

	if( iReset )
		{
		// The composer has been reset - should not be receiving this. 
		// Test has failed - notify the observer.
		iObserver.Log(_L("FAIL - not expecting MessageDataReady"));
		iObserver.NotifyError(KErrNotFound);
		iTestFailed = ETrue;
		}
	else
		{
		// Get data from the composer
		TPtrC8 data;
		iMessageComposer.GetMessageData(data);

		// Dump the message data...
		iObserver.Log(_L("Message data received"));
		iObserver.Dump(data);

		// Check to see if it matches the expected data.
		TBool match = EFalse;
		if( data.Length() <= iExpectedData.Length() )
			{
			// Is the content the same
			if( data.Compare(iExpectedData.Left(data.Length())) == 0 )
				{
				// Data is the same - update the expected data
				iExpectedData.Set(iExpectedData.Mid(data.Length()));
				match = ETrue;
				}
			}
		if( match )
			{
			if( iDoReset && iMessageIndex == iMessageChunk )
				{
				// Reset the composer and update the chunk index
				iMessageComposer.Reset();
				++iMessageIndex;

				iObserver.Log(_L("RESET - starting test again!!"));

				// Reset the data supplier
				if( iBodyData != NULL )
					iBodyData->ResetSupplier();

				// Start the test over...
				CompleteSelf();
				}
			else
				{
				// Inc the message chunk count
				++iMessageChunk;

				// Release the message data...
				iMessageComposer.ReleaseMessageData();
				}
			}
		else
			{
			// The data does not match. Test has failed - notify the observer.
			iObserver.Log(_L("FAIL - received incorrect data"));
			iObserver.NotifyError(KErrNotFound);
			iTestFailed = ETrue;
			}
		}
	}

TInt CMessageComposerDriver::HandleComposeError(TInt aError)
	{
	// Is an error expected?
	if( iExpectedError == aError )
		{
		TBuf<64> comment;
		comment.Format(_L("Received expected error : %d"), aError);
		iObserver.Log(comment);

		// Yep - test done...
		iObserver.NotifyComplete();
		}
	else
		{
		// Inform the observer of the error
		iObserver.NotifyError(aError);
		}
	return KErrNone;
	}

void CMessageComposerDriver::Reserved_MHttpMessageComposerObserver()
	{
	User::Invariant();
	}

/*
 *	Methods from MBodyDataSupplierObserver
 */

void CMessageComposerDriver::NotifyMoreData()
	{
	if( iDoReset && iChunkIndex == iChunkCount )
		{
		// Reset the composer and update the chunk index
		iMessageComposer.Reset();
		++iChunkIndex;

		iObserver.Log(_L("RESET - starting test again!!"));

		// Reset the data supplier
		if( iBodyData != NULL )
			iBodyData->ResetSupplier();

		// Start the test over...
		CompleteSelf();
		}
	else
		{
		// Inc the chunk count and notify the composer of more data
		++iChunkCount;
		iMessageComposer.MessageInfoAvailable();
		}
	}

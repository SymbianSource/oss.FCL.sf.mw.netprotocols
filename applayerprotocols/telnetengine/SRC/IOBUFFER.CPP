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
// Telnet Protocol API
// CIOBufferControl implementation
// 
//

/**
 @file
*/

#include "IOBUFFER.H"
#include "ACTIVEIO.H"
#include "TELDEBUG.H"

CIOBufferControl::CIOBufferControl()
/**
Constructor
*/
	{
	}

CIOBufferControl::~CIOBufferControl()
/**
Destructor
*/
	{
	Reset();
	__FLOG_STATIC(KTelnetLoggingCompnt(),KTelnetLoggingTag(),_L("CIOBufferControl::D'Tor"));
	}

CIOBufferControl* CIOBufferControl::NewL(MIONotifier* aNotifier)
	{
	CIOBufferControl* self = new(ELeave) CIOBufferControl;
	CleanupStack::PushL(self);
	self->ConstructL(aNotifier);
	CleanupStack::Pop();
	return self;
	}

void CIOBufferControl::ConstructL(MIONotifier* aNotifier)
	{
	iNotifier = aNotifier;

	iWriter = NULL;
	iReader = NULL;

	iQueueIndex = 0;
	iLiveIndex = 0;
	
	iUrgent = NULL;
	iWriteBuffers[0] = NULL;
	iWriteBuffers[1] = NULL;
	}

TInt CIOBufferControl::WriteUrgent(HBufC8* aBuffer)
/**
Caller requests to send data as TCP urgent
*/
	{
	TInt err = KErrNone;
	// Check to see if write is already outstanding
	if(iLiveIndex == iQueueIndex)
		{
		// No write outstanding
		// Just set the live index to point to the heap buffer
		iWriteBuffers[iLiveIndex] = aBuffer;
		// Make indexes different so if the next write call comes before completion of this one
		// then it can append to the queue buffer
		(iLiveIndex == 0) ? (iQueueIndex = 1) : (iQueueIndex = 0);
		iWriter->IssueUrgentWrite(*(iWriteBuffers[iLiveIndex]));
		err = KErrNone;
		}
	else
		{
		// Check for existing queued urgent
		if(iUrgent)
			// DON'T permit queued urgents
			err = KErrInUse;
		else
			{
			// Data will be sent on next write completion
			iUrgent = aBuffer;
			err = KErrNone;
			}
		}
	return(err);
	}

TInt CIOBufferControl::Write(HBufC8* aBuffer)
/**
Writes issued into this object send it to the active writer
Ownership of the buffer is transferred
*/
	{
	TInt ret = KErrNone;

	TPtr8 ptr = aBuffer->Des();

	// iLiveIndex = iQueueIndex means there are no outstanding writes on the socket
	if(iLiveIndex == iQueueIndex)
		{			
		iWriteBuffers[iLiveIndex] = aBuffer;
		// Make indexes different so if the next write call comes before completion of this one
		// then it can append to the queue buffer
		(iLiveIndex == 0) ? (iQueueIndex = 1) : (iQueueIndex = 0);
		iWriter->IssueWrite(*(iWriteBuffers[iLiveIndex]));
		}
	else
		{
		__FLOG_STATIC(KTelnetLoggingCompnt(),KTelnetLoggingTag(),_L("CIOBufferControl() Write Queue"));
		// There is already a buffer loaded on the socket
		if(iWriteBuffers[iQueueIndex])
			// Append this data to the iQueueIndex buffer
			// Overflow check first
			{
			HBufC8* qBuf = iWriteBuffers[iQueueIndex];
			TPtr8 qPtr = qBuf->Des();
			if((qPtr.MaxLength() - qPtr.Length()) < ptr.Length())
				// Not enough room to append
				{
				qBuf = iWriteBuffers[iQueueIndex]->ReAlloc(qPtr.Length() + ptr.Length());
				if(qBuf==NULL)
					return KErrNoMemory;		//Return error and don't take ownership of aBuffer
				iWriteBuffers[iQueueIndex] = qBuf;
				qPtr = qBuf->Des();
				}
			qPtr.Append(ptr);
			delete aBuffer;
			}
		else
			// Nothing queued just assign the pointer
			// Data will be sent on next write completion
			{
			iWriteBuffers[iQueueIndex] = aBuffer;
			}
		}
	return(ret);
	}


TInt CIOBufferControl::Read(TDes8& aBuffer)
	{
	return(iReader->IssueRead(aBuffer));
	}

void CIOBufferControl::SetWriter(CActiveWriter* aWriter)
	{
	iWriter = aWriter;
	}

void CIOBufferControl::SetReader(CActiveReader* aReader)
	{
	iReader = aReader;
	}


// MIONotifier 

void CIOBufferControl::Event(TInt aEvent,TInt aEventCode)
	{
	iNotifier->Event(aEvent,aEventCode);
	}

void CIOBufferControl::WriteComplete()
/**
Normal asynchronous completion to a write request to the active writer
The data we transmitted will always be pointed to by iLiveIndex
On completion there could be Urgent or ordinary data queued
Submit any queued urgent first
*/
	{
	// Free the HBufC8* we loaded for transmit the last time
	delete iWriteBuffers[iLiveIndex];
	iWriteBuffers[iLiveIndex] = NULL;
	// First check to see if we have any urgent data queued
	if(iUrgent)
		{
		// Urgent data queued, move the urgent pointer to the live index
		iWriteBuffers[iLiveIndex] = iUrgent;
		// Free the pointer
		iUrgent = NULL;
		// Send the queued urgent data
		iWriter->IssueUrgentWrite(*(iWriteBuffers[iLiveIndex]));
		}
	else if(iWriteBuffers[iQueueIndex])
		// Check to see if any data is on the normal queue
		{
		// Data queued
		// Swap the indexes
		iLiveIndex = iQueueIndex;
		(iLiveIndex == 0) ? (iQueueIndex = 1) : (iQueueIndex = 0); 		
		// Issue the next write
		__FLOG_STATIC(KTelnetLoggingCompnt(),KTelnetLoggingTag(),_L("CIOBufferControl() WriteComplete() Write Queue Data"));
		iWriter->IssueWrite(*(iWriteBuffers[iLiveIndex]));
		// complete the write
		}
	else
		{
		// No data queued, set the transmitter empty condition
		iLiveIndex = iQueueIndex;
		}
	iNotifier->WriteComplete();
	}

void CIOBufferControl::WriteError(TInt aError)
/**
Write completion with an error, just pass the code up in a notification
*/
	{
	Reset();
	iNotifier->WriteError(aError);
	}

void CIOBufferControl::ReadCompleteL()
	{
	iNotifier->ReadCompleteL();
	}

void CIOBufferControl::ReadComplete(TInt aError)
	{
	Reset();
	iNotifier->ReadComplete(aError);
	}

void CIOBufferControl::Reset()
	{
	delete iWriteBuffers[0];
	delete iWriteBuffers[1];
	delete	iUrgent;

	iUrgent = NULL;
	iWriteBuffers[0] = NULL;
	iWriteBuffers[1] = NULL;

	iQueueIndex = iLiveIndex = 0;
	iWriter = NULL;
	iReader = NULL;
	}

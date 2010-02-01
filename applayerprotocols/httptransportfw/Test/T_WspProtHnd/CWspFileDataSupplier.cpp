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
// System includes
// Local includes
// 
//

// Class signatures
#include "cwspfiledatasupplier.h"


// Constants used in this file
_LIT(KFileSystemRoot,"C:\\");
const TInt KChunkSize = 64;

/** Implementation of class 'CWspFileDataSupplier'.
*/


CWspFileDataSupplier* CWspFileDataSupplier::NewL()
	{
	CWspFileDataSupplier* me = new(ELeave)CWspFileDataSupplier();
	CleanupStack::PushL(me);
	me->ConstructL();
	CleanupStack::Pop(me);
	return me;
	}

CWspFileDataSupplier* CWspFileDataSupplier::NewL(const TDesC& aFileName)
	{
	CWspFileDataSupplier* me = new(ELeave)CWspFileDataSupplier();
	CleanupStack::PushL(me);
	me->ConstructL(aFileName);
	CleanupStack::Pop(me);
	return me;
	}

CWspFileDataSupplier::~CWspFileDataSupplier()
	{
	delete iBuffer;
	iFileHnd.Close();
	iFileSrvHnd.Close();
	}

void CWspFileDataSupplier::CleanUp(TAny* aDataSupplierItem)
	{
	delete STATIC_CAST(CWspFileDataSupplier*, aDataSupplierItem);
	}

void CWspFileDataSupplier::SetFileL(const TDesC& aFileName)
	{
	// Close any already open file
	if (iFileOpen)
		{
		iFileHnd.Close();
		iFileOpen = EFalse;
		}

	// Parse the name of the new file and open it
	iParsedFileName.Set(KFileSystemRoot, &aFileName, NULL);
	User::LeaveIfError(iFileHnd.Open(iFileSrvHnd, iParsedFileName.FullName(), EFileRead));
	iFileOpen = ETrue;

	// Read from the request body file
	TPtr8 bufferPtr = iBuffer->Des();

	// Read initially into the dynamic buffer.  If we haven't hit the
	// end of file, then read more into a local buffer and append to the
	// dynamic one, expanding and reallocating where necessary
	TInt err = iFileHnd.Read(bufferPtr);
	TBuf8<KChunkSize> chunk;
	TBool done = EFalse;
	while (!done)
		{
		// If it's not at EOF, then the read should have been successful with no error
		User::LeaveIfError(err);

		// Read some more... detect EOF
		err = iFileHnd.Read(chunk);
		done = (chunk.Length() == 0);
		if ((err == KErrNone) && !done)
			{
			// Determine whether we need to grow the dynamic buffer
			if (chunk.Length() > (bufferPtr.MaxLength() - bufferPtr.Length()))
				{
				HBufC8* oldbuffer = iBuffer;
				iBuffer = oldbuffer->ReAllocL(oldbuffer->Length() + KChunkSize);
				bufferPtr = iBuffer->Des();
				}

			// Append from the local buffer
			bufferPtr.Append(chunk);
			}
		}
	}

TBool CWspFileDataSupplier::GetNextDataPart(TPtrC8& aDataPart)
	{
	__ASSERT_ALWAYS(iFileOpen, User::Invariant());
	// Return the data in one chunk
	aDataPart.Set(*iBuffer);
	return ETrue;
	}

void CWspFileDataSupplier::ReleaseData()
	{
	__ASSERT_ALWAYS(iFileOpen, User::Invariant());
	// Clear out the submit buffer
	TPtr8 bufferPtr = iBuffer->Des();
	bufferPtr.Zero();
	}

TInt CWspFileDataSupplier::OverallDataSize()
	{
	__ASSERT_ALWAYS(iFileOpen, User::Invariant());
	return iBuffer->Length();
	}

TInt CWspFileDataSupplier::Reset()
	{
	__ASSERT_ALWAYS(iFileOpen, User::Invariant());
	return KErrNotSupported;
	}

CWspFileDataSupplier::CWspFileDataSupplier()
	{
	}

void CWspFileDataSupplier::ConstructL()
	{
	// Connect to the fileserver
	User::LeaveIfError(iFileSrvHnd.Connect());

	// Make a buffer to hold chunks taken from the file
	iBuffer = HBufC8::NewMaxL(KChunkSize);
	}

void CWspFileDataSupplier::ConstructL(const TDesC& aFileName)
	{
	// General set-up first
	ConstructL();

	// Open the specified file
	SetFileL(aFileName);
	}

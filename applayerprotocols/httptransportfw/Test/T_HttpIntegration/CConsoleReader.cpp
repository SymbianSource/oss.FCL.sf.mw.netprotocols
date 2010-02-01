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
// rev:	mjdavey, symbian@mjdss.com, July 2002
// for:	Typhoon (7.0s) & JetStream (8.0)
// 
//

#include "CConsoleReader.h"

//-----------------------------------------------------------------------------

CConsoleReader* CConsoleReader::NewL(CConsoleBase& aConsole)
{
CConsoleReader* self = new (ELeave) CConsoleReader(aConsole);
return self;
}

//-----------------------------------------------------------------------------

CConsoleReader::~CConsoleReader()
{
CActive::Cancel();
}

//-----------------------------------------------------------------------------

CConsoleReader::CConsoleReader(CConsoleBase& aConsole)
	: 
	CActive(EPriorityUserInput), 
	iConsole(aConsole)
{
CActiveScheduler::Add(this);
}

//-----------------------------------------------------------------------------

void CConsoleReader::ReadLine(TDes& aData, TRequestStatus& aCallbackStatus)
{
// Set data buffer and call back request status
iData = &aData;
iCallbackStatus = &aCallbackStatus;
*iCallbackStatus = KRequestPending;

// Set mode
iMode = ELineMode;

// Issue character request
IssueRequest();
}

//-----------------------------------------------------------------------------

void CConsoleReader::ReadChar(TDes& aData, TRequestStatus& aCallbackStatus)
{
// Set data buffer and call back request status
iData = &aData;
iCallbackStatus = &aCallbackStatus;
*iCallbackStatus = KRequestPending;

// Set mode
iMode = ECharMode;

// Issue character request
IssueRequest();
}

//-----------------------------------------------------------------------------

void CConsoleReader::ReadCancel()
{
Cancel();
}

//-----------------------------------------------------------------------------

void CConsoleReader::IssueRequest()
{
__ASSERT_DEBUG( !IsActive(), User::Panic(_L("Reader"), KErrCorrupt) );

// Issue read character request
iConsole.Read(iStatus);
// Wait for request to be handled;
SetActive();
}

//-----------------------------------------------------------------------------

void CConsoleReader::RunL()
{
ProcessKeyPress(TChar(iConsole.KeyCode()));
}

//-----------------------------------------------------------------------------

void CConsoleReader::DoCancel()
{
iConsole.ReadCancel();
User::RequestComplete(iCallbackStatus, KErrCancel);
}

//-----------------------------------------------------------------------------

void CConsoleReader::ProcessKeyPress(TChar aKey)
{
TBuf<1> charBuf;
charBuf.Format(_L("%d"), (TInt)aKey);

switch( aKey )
	{
	case EKeyBackspace:
	case EKeyDelete:
		// Reduce the data length and request another key press
		if( iData->Length() > 0 )
			{
			iData->SetLength(iData->Length() - 1);
			iConsole.Write(charBuf);
			}

		if( iMode == ECharMode )
			{
			// Complete the request
			User::RequestComplete(iCallbackStatus, KErrNone);
			iCallbackStatus = NULL;
			}
		else
			{
			// Get the next key press
			IssueRequest();
			}
		break;

	case EKeyEnter:

		// Write the new line to the console and complete the callback request status
		iConsole.Write(KTxtNewLine);
		User::RequestComplete(iCallbackStatus, KErrNone);
		iCallbackStatus = NULL;
		break;

	default:
		// Append to input line, display and request another key press
		iData->Append(aKey);
		iConsole.Write(charBuf);
		if( iMode == ECharMode )
			{
			// Add a carriage return
			iConsole.Write(KTxtNewLine);
			// Complete the request
			User::RequestComplete(iCallbackStatus, KErrNone);
			iCallbackStatus = NULL;
			}
		else
			// Get the next key press
			IssueRequest();
		break;
	}
}

//-----------------------------------------------------------------------------
//	End of File
//-----------------------------------------------------------------------------



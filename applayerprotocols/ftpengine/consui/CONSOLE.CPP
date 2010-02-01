// Copyright (c) 1998-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Author:	Philippe Gabriel
// Console active object
// Buffers input from the keyboard and reports a command line
// terminated by a return key
// 
//

/**
 @file Console.cpp
 @internalComponent
*/

//#include "defs.h"
//#include "debug.h"
#include "CONSOLE.H"
#include <e32base.h>
#include <e32test.h>

//
// Definitions
//

CmdConsole::CmdConsole(MCmdConsoleReport* aNotifier,CConsoleBase* aConsole):CActive(CActive::EPriorityStandard),
iNotifier(aNotifier),iConsole(aConsole),iAcceptInput(TRUE)
{
}

CmdConsole* CmdConsole::NewL(MCmdConsoleReport* aNotifier,CConsoleBase* aConsole)
{
	CmdConsole* self = new(ELeave) CmdConsole(aNotifier,aConsole);
	CActiveScheduler::Add(self);
	return self;
}

CmdConsole::~CmdConsole(void)
{
Cancel();
}

void CmdConsole::Start(void)
{
	iConsole->Read(iStatus);
	SetActive();
}

void CmdConsole::Reset(void)
{
	// Reset buffer
	iCmdBuffer.Zero();
	// Ok to Accept input
	iAcceptInput = TRUE;
	// Display a prompt
	iConsole->Printf(_L("\nftp>"));
}

TDes& CmdConsole::FetchCmd(void)
{
	return iCmdBuffer;
}

void CmdConsole::RunL()
{
// Utput to the screen is done in a weird way
// but could not find the pattern to printf directly
TBuf<5> test;
TPtrC acceptInput(KAlphabet);
TChar c= iConsole->KeyCode();
	Start();
	if ( c == EKeyEscape )
		{
		// Call Notifier
		iNotifier->Escape();
		return;
		}
	if (!iAcceptInput)
		return;
	if ( c == EKeyEnter )
		{
		// Call Notifier
		iAcceptInput = FALSE;
		iConsole->Printf(_L("\n"));
		iNotifier->CmdReady();
		return;
		}
	if (c == EKeyBackspace) 
		{if (iCmdBuffer.Length()==0)
			// If buffer empty bail out
			return;
		// Otherwise remove last char from buffer
		iCmdBuffer.Delete(iCmdBuffer.Length()-1,1);
		test.Zero();
		test.Append(EKeyBackspace);
		test.Append(' ');
		test.Append(EKeyBackspace);
		test.Append(0);

		iConsole->Printf(_L("%S"),&test);
		//	iConsole->Printf(_L("%C"),c);
		}
	else if(KErrNotFound != acceptInput.Locate(c))
		{
		// Is that a char or a cmd?
		// If it's a char append it 
		iCmdBuffer.Append(c);
		// Output it on the screen
		// iConsole->Printf(_L("%S"),&acceptInput.Mid(acceptInput.Locate(c),1));
		// iConsole->Printf(_L("%c"),c);
		test.Zero();
		test.Append(c);
		test.Append(0);

		iConsole->Printf(_L("%S"),&test);
		}
}

void CmdConsole::DoCancel(void)
{
iConsole->ReadCancel();
}

void CmdConsole::ConstructL(void){}


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

#include "cdriverbase.h"

#include "thttptrhndtestpanic.h"

CDriverBase::CDriverBase(MDriverObserver& aObserver)
: CActive(CActive::EPriorityLow), iObserver(aObserver)
/**	
	Constructor.
	@componentInternal		
	@param		aObserver	The test driver observer.
*/
	{
	CActiveScheduler::Add(this);
	}

CDriverBase::~CDriverBase()
/**	
	Destructor.
	@componentInternal		
*/
	{
	Cancel();
	iCmdStack.Close();
	}

void CDriverBase::Start()
/**	
	Starts the driver.
	@componentInternal		
*/
	{
	__ASSERT_DEBUG( iCmdStack.Count() > 0, THttpTrHndTestPanic::Panic(THttpTrHndTestPanic::EEmptyCmdStack) );

	// Notify observer that test is starting
	iObserver.NotifyStart();

	iNextCmd = 0;
	NextCmd();
	}

TBool CDriverBase::TestExpectedCmd(TInt aCmd, TInt aError)
/**	
	Is aCmd the currently expected command. Used by the concrete driver objects
	to see if the invocation that has occured on their API is the expected one.
	A return value of ETrue indicates that it is, otherwise a value of EFalse is
	returned if it is not.
	@componentInternal		
	@param		The ID of the invoked command.
	@return		A value of ETrue it the invoked command was expected otherwise a
				value of EFalse if it was not.
*/
	{
	TBool correctCmd = aCmd == iCurrentCmd;

	if( !correctCmd )
		{
		// Inform the observer that the wrong cmd was received
		iObserver.NotifyError(aError);
		}
	else
		{
		// Go onto next cmd
		NextCmd();
		}
	return correctCmd;
	}

void CDriverBase::AppendDoCmdL(TInt aCmd)
/**	
	Appends an 'Expect' command with ID aCmd to the command stack.
	@componentInternal		
	@param		aCmd	The ID of the 'Expect' command.
*/
	{
	User::LeaveIfError(iCmdStack.Append(TCmdEntry(aCmd, TCmdEntry::EDoCmd)));
	}

void CDriverBase::AppendExpectCmdL(TInt aCmd)
/**	
	Appends a 'Do' command with ID aCmd to the command stack.
	@componentInternal		
	@param		aCmd	The ID of the 'Do' command.
*/
	{
	User::LeaveIfError(iCmdStack.Append(TCmdEntry(aCmd, TCmdEntry::EExpectCmd)));
	}

void CDriverBase::DoLog(const TDesC& aComment)
/**	
	Logs comment to log file, but not the summary file.
	@componentInternal		
	@param		aComment	The comment to be logged.
*/
	{
	iObserver.Log(aComment);
	}

void CDriverBase::NextCmd()
/**	
	Sets the next command from the command stack. If the next command is a 'Do'
	command then the object self-completes in order to perform the command.
	@componentInternal		
*/
	{
	if( iNextCmd < iCmdStack.Count() )
		{
		TCmdEntry cmd = iCmdStack[iNextCmd++];

		iCurrentCmd = cmd.iCmd;

		if( cmd.iType == TCmdEntry::EDoCmd )
			{
			// Next cmd is a Do-type - self complete.
			CompleteSelf();
			}
		}
	else
		{
		// All the cmds have been run - notify the observer
		iObserver.NotifyComplete();
		}
	}

void CDriverBase::CompleteSelf()
/**	
	Self-complete function. Ensures that the active scheduler calls the RunL()
	at the next opportunity.
	@componentInternal		
*/
	{
	TRequestStatus* pStat = &iStatus;
	User::RequestComplete(pStat, KErrNone);
	SetActive();
	}

/*
 *	Methods from CActive
 */

void CDriverBase::RunL()
/**	
	Performs the current 'Do' command.
	@componentInternal		
*/
	{
	// Get derived-class to run the cmd
	RunCmdL(iCurrentCmd);

	// Get the next cmd..
	NextCmd();
	}

void CDriverBase::DoCancel()
/**	
	This does nothing - the only asynchronous request made by this class is to
	complete itself.
	@componentInternal		
*/
	{
	// Nothing to do.
	}

TInt CDriverBase::RunError(TInt aError)
/**	
	Error handler for 'Do' commands that leave.
	@componentInternal		
	@return		A value of KErrNone. The error is always handled.
*/
	{
	iObserver.NotifyError(aError);
	return KErrNone;
	}

/*
 *	Methods from MDriverObserver
 */

void CDriverBase::NotifyError(TInt aError)
/**	
	@see		MDriverObserver
	@componentInternal
*/
	{
	iObserver.NotifyError(aError);
	}

void CDriverBase::NotifyStart()
/**	
	@see		MDriverObserver
	@componentInternal
*/
	{
	iObserver.NotifyStart();
	}

void CDriverBase::NotifyComplete()
/**	
	@see		MDriverObserver
	@componentInternal
*/
	{
	iObserver.NotifyComplete();
	}

void CDriverBase::Log(const TDesC& aComment)
/**	
	@see		MDriverObserver
	@componentInternal
*/
	{
	iObserver.Log(aComment);
	}

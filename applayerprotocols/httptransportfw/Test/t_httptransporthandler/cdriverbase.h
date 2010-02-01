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

#ifndef __CDRIVERBASE_H__
#define __CDRIVERBASE_H__

#include <e32base.h>

#include "mdriverobserver.h"

/**	@class		CDriverBase
	The CDriverBase is an abstract class that provides command running 
	functionality. There are two types of commands - 'Do' and 'Expect' commands.

	A 'Do' command implies that the concrete driver must perform an action. The
	concrete class is notified of the action via its override of the RunCmdL() 
	function.

	Once the 'Do' command has been performed, the next command is extracted from
	the command stack. If it is another 'Do' command, then the driver base self-
	completes and the RunCmdL() function is called again. For an 'Expect' command
	the driver base waits until the concrete class it invoked.

	When a concrete class is invoked on one of its APIs it can check to see if
	that invocation was expected by asking this base class, passing the the 
	command ID of the invoked function in the ExpecetedCmd() API.

	If the invoked command was expected, the next command is extracted from the
	command stack as described earlier.

	A derived class must form the command stack using the AppendDoCmdL() and the
	AppendExpectCmdL() APIs. The commands should be appended in running order.
	@componentInternal		
*/
class CDriverBase	: public CActive,
					  public MDriverObserver
	{
private:	// helper class

/**	@class		TCmdEntry
	The TCmdEntry class is a helper class for CDriverBase. It encapsulates a 
	command ID and its type.
	@componentInternal		
*/
	class TCmdEntry
		{
	public:	// enums

/**	@enum		TCmdType
	An enumeration defining the types of commands.
*/
		enum TCmdType
			{
			/** A 'Do' type command. This type of command implies that the driver
				object must perform an action.
			*/
			EDoCmd			=0,
			/**	An 'Expect' type command. This type of command implies that the
				driver expects an invocation on its API.
			*/
			EExpectCmd
			};

	public:	// methods

		inline TCmdEntry();

		inline TCmdEntry(TInt aCmd, TCmdType aType);

	public:	// attributes

		TInt		iCmd;

		TCmdType	iType;

		};

public:	// methods

	virtual ~CDriverBase();

	void Start();

private:	// methods from CActive

	virtual void RunL();

	virtual void DoCancel();

	virtual TInt RunError(TInt aError);

private:	// methods from MDriverObserver

	virtual void NotifyError(TInt aError);

	virtual void NotifyStart();

	virtual void NotifyComplete();

	virtual void Log(const TDesC& aComment);

protected:	// methods

	CDriverBase(MDriverObserver& aObserver);

	TBool TestExpectedCmd(TInt aCmd, TInt aError);

	void AppendExpectCmdL(TInt aCmd);

	void AppendDoCmdL(TInt aCmd);

	void DoLog(const TDesC& aComment);

private:	// methods

/**	@fn			RunCmdL(TInt aCmd) =0
	Requests the command with ID aCmd is run.
	@componentInternal		
	@param		aCmd	The ID of the command to run.
*/
	virtual void RunCmdL(TInt aCmd) =0;

private:	// methods

	void CompleteSelf();

	void NextCmd();

private:	// attributes

	MDriverObserver&		iObserver;

	TInt					iCurrentCmd;

	RArray<TCmdEntry>		iCmdStack;

	TInt					iNextCmd;

	};

inline  CDriverBase::TCmdEntry::TCmdEntry()
/**	
	Default constructor. Object not initialised.
	@componentInternal		
*/
	{
	}

inline  CDriverBase::TCmdEntry::TCmdEntry(TInt aCmd, TCmdType aType)
: iCmd(aCmd), iType(aType)
/**	
	Default constructor. Object is initialised.
	@componentInternal		
	@param		aCmd	The command ID.
	@param		aType	The command type.
*/
	{
	}

#endif	// __CDRIVERBASE_H__

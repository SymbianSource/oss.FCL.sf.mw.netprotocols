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

/**
 @file MNwssSessionEventHandler.h
 @warning : This file contains Rose Model ID comments - please do not delete
*/

#ifndef __MNWSSSESSIONEVENTHANDLER_H__
#define __MNWSSSESSIONEVENTHANDLER_H__


/**
	@since			7.0
	This interface defines methods to handle each type of session event that can be received
	in the WSP layer of the WAP Stack.
 */
//##ModelId=3C9B094E00E4
class MNwssSessionEventHandler
	{
public:	// Methods

/**Handle the S-Disconnect.ind PDU received from the WAP Stack.	
	@since			7.0
	@leave			System wide errors, e.g. KErrNoMemory
 */
	//##ModelId=3C9B094E0118
	virtual void HandleDisconnectIndL() = 0;

/**Handle the S-Suspend.ind PDU received from the WAP Stack.	
	@since			7.0
	@leave			System wide errors, e.g. KErrNoMemory
 */
	//##ModelId=3C9B094E0117
	virtual void HandleSuspendIndL() = 0;

/**Handle the S-Resume.cnf PDU received from the WAP Stack.	
	@since			7.0
	@leave			System wide errors, e.g. KErrNoMemory
 */
	//##ModelId=3C9B094E0116
	virtual void HandleResumeCnfL() = 0;

/**Handle the S-Connect.cnf PDU received from the WAP Stack.	
	@since			7.0
	@leave			System wide errors, e.g. KErrNoMemory
 */
	//##ModelId=3C9B094E010D
	virtual void HandleConnectCnfL() = 0;

/**Handle the S-Redirect.ind PDU received from the WAP Stack.	
	@since			7.0
	@leave			System wide errors, e.g. KErrNoMemory
 */
	//##ModelId=3C9B094E010C
	virtual void HandleRedirectIndL() = 0;

/**Handle the E-Exception.ind PDU received from the WAP Stack.	
	@since			7.0
	@leave			System wide errors, e.g. KErrNoMemory
 */
	//##ModelId=3C9B094E0102
	virtual void HandleExceptionIndL() = 0;
	};

#endif	// __MNWSSSESSIONEVENTHANDLER_H__

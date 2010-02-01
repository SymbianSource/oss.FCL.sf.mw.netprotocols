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
 @file MNwssTransactionEventHandler.h
 @warning : This file contains Rose Model ID comments - please do not delete
*/

#ifndef __MNWSSTRANSACTIONEVENTHANDLER_H__
#define __MNWSSTRANSACTIONEVENTHANDLER_H__

// System includes
#include <wapcli.h>


/**
	@since			7.0
	This interface defines methods to handle each type of transaction event that can be received
	in the WSP layer of the WAP Stack.  To handle an event, the corresponding transaction ID is
	supplied.
 */
//##ModelId=3C9B094C02EA
class MNwssTransactionEventHandler
	{
public:	// Methods

/**	
	Handle the T-MethodInvoke.cnf PDU received from the WAP Stack.	
	@since			7.0
	@param			aTransId		(in) The ID of the transaction on which the event occurred.
	@leave			System wide errors, e.g. KErrNoMemory
 */
	//##ModelId=3C9B094C031C
	virtual void HandleMethodInvokeCnfL(RWSPCOTrans::TTransID aTransId) = 0;

/**Handle the T-MethodResult.ind PDU received from the WAP Stack.	
	@since			7.0
	@param			aTransId		(in) The ID of the transaction on which the event occurred.
	@leave			System wide errors, e.g. KErrNoMemory
 */	//##ModelId=3C9B094C0314
	virtual void HandleMethodResultIndL(RWSPCOTrans::TTransID aTransId) = 0;

/**Handle the T-MethodAbort.ind PDU received from the WAP Stack.	
	@since			7.0
	@param			aTransId		(in) The ID of the transaction on which the event occurred.
	@leave			System wide errors, e.g. KErrNoMemory
 */	//##ModelId=3C9B094C0312
	virtual void HandleAbortIndL(RWSPCOTrans::TTransID aTransId) = 0;
	};

#endif	// __MNWSSTRANSACTIONEVENTHANDLER_H__

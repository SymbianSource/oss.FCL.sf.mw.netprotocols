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
 @file MNwssOomHandler.h
 @warning : This file contains Rose Model ID comments - please do not delete
*/
#ifndef __MNWSSOOMHANDLER_H__
#define __MNWSSOOMHANDLER_H__

// System includes
#include <wapcli.h>
#include <wsp/mwspcomethodcallback.h>

/**
	@since			7.0
	This interface provides callback mechanisms used to abort methods or disconnect a session in
	in out-of-memory situation in the NWSS WSP Transport Handler.
 */
//##ModelId=3C9B095502F7
class MNwssOomHandler
	{
public:	// Methods

/**Used as an asynchronous callback, in an OOM situation, to cause the method that
					couldn't complete at the moment of OOM, to be cleanly aborted for the client.
	@param			aMethodToAbort	(in) The method which is to be aborted due to the OOM situation
	@param			aAbortOnStack	(in) Flag indicating whether the method must be aborted on the
										 WAP stack as well.
	@since			7.0
 */
	//##ModelId=3C9B09550321
	virtual void SendOomMethodAbort(MWspCOMethodCallback& aMethodToAbort, TBool aAbortOnStack) = 0;

/**Used as an asynchronous callback, in an OOM situation, to cause the method that
					couldn't complete at the moment of OOM, to be cleanly aborted for the client.
	@param			aTransId		(in) A WAP stack transaction ID that identifies the method which
										 is to be aborted due to the OOM situation
	@param			aAbortOnStack	(in) Flag indicating whether the method must be aborted on the
										 WAP stack as well.
	@since			7.0
 */
	//##ModelId=3C9B09550324
	virtual void SendOomMethodAbort(RWSPCOTrans::TTransID aTransId, TBool aAbortOnStack) = 0;

/**Used as an asynchronous callback, in an OOM situation, to cause the session that
					couldn't connect at the moment of OOM, to be cleanly disconnected for the client.
	@param			aDisconnectOnStack	(in) Flag indicating whether the session must be disconnected
											 on the WAP stack as well.
	@since			7.0
 */
	//##ModelId=3C9B0955031F
	virtual void SendOomDisconnect(TBool aDisconnectOnStack) = 0;
	};

#endif	// __MNWSSOOMHANDLER_H__

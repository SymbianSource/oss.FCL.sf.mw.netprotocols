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
//

/**
 @file
 @warning : This file contains Rose Model ID comments - please do not delete
*/

#ifndef	__MHTTPSESSIONEVENTCALLBACK_H__
#define	__MHTTPSESSIONEVENTCALLBACK_H__

// System includes
#include <e32std.h>
#include <http/rhttptransaction.h>
#include <http/thttpevent.h>


//##ModelId=3C4C0F460242
class MHTTPSessionEventCallback
/**
The per-session callback for receiving session event callbacks.
@publishedAll
@released
*/
    {
public:	// Methods

	/** Called when the filters registration conditions are satisfied for events that occur
		on the session. Any leaves must be handled by the appropriate MHFRunError.
		@param aEvent The session event that has occured.
	*/
	//##ModelId=3C4C0F460262
	virtual void MHFSessionRunL(const THTTPSessionEvent& aEvent) =0;

	/** Called when MHFRunL leaves from a session event. This works in the same
		way as CActve::RunError
		If you don't completely handle the error, a panic will occur.
		@param aError The leave code that RunL left with.
		@param aEvent The Event that was being processed.
		@return KErrNone if the error has been cancelled or the code
		of the continuing error otherwise.	
	*/
	//##ModelId=3C4C0F460256
	virtual TInt MHFSessionRunError(TInt aError, const THTTPSessionEvent& aEvent) =0;
    };


#endif //	__MHTTPSESSIONEVENTCALLBACK_H__

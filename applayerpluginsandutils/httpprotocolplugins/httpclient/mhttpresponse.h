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
//

#ifndef __MHTTPRESPONSE_H__
#define __MHTTPRESPONSE_H__

#include <e32std.h>

class RHTTPTransaction;
class MHttpResponse
	{
public:

	virtual void ResponseDataReceived(const TDesC8& aData) = 0;

	virtual void CancelResponse() =0;

	virtual void ConnectionError(TInt aError) =0;
	
	virtual TBool ResponseCompleted () =0;
	
	virtual void OnResponseReceiveTimeOut () =0;
	
	virtual TInt ReceiveTimeOutValue () =0;

	virtual TBool NeedCompletion () =0;
	
	virtual TBool CompleteResponse ( const TDesC8& aData ) = 0;
	
	virtual void FailTransactionL() = 0;
	
	virtual TBool ResponseInformational () =0;
	
	virtual void FlushBodyDataIfNotRead() =0;

	};

#endif	//  __MHTTPRESPONSE_H__

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


#ifndef		__VALIDATIONFILTER_H__
#define		__VALIDATIONFILTER_H__

#include <e32std.h>
#include <e32base.h>
#include <http/mhttpfilter.h>
#include <http/rhttpsession.h>
#include <http/framework/httplogger.h>
#include <http/cecomfilter.h>
	
/** The validation filter checks requests and responses to ensure that they are valid. It 
	also adds either a succeded or failed event. Most clients will only be intrested in this
	simplified reporting of success. Any error codes are still sent to the client.
*/
//##ModelId=3B1E52AC018D
class CValidationFilter : public CEComFilter, public MHTTPFilter
	{
public:
	static CEComFilter* InstallFilterL(TAny* aSession);	
	//##ModelId=3B1E52AC0239
	~CValidationFilter();

public:	// Methods from MHTTPFilterBase

	// @see MHTTPFilterBase::MHFRunL
	//##ModelId=3B1E52AC022F
	virtual void MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent);

	// @see MHTTPFilterBase::MHFSessionRunL
	virtual void MHFSessionRunL(const THTTPSessionEvent& aEvent);

	// @see MHTTPFilterBase::MHFRunError
	//##ModelId=3BA600490296
	virtual TInt MHFRunError(TInt aError, RHTTPTransaction aTransaction,
							 const THTTPEvent& aEvent);

	// @see MHTTPFilterBase::MHFSessionRunError
	virtual TInt MHFSessionRunError(TInt aError, const THTTPSessionEvent& aEvent);

public:	// Methods from MHTTPFilter

	// @see MHTTPFilter::MHFUnload
	//##ModelId=3B1E52AC0225
	virtual void MHFUnload(RHTTPSession aSession, THTTPFilterHandle aHandle);

	// @see MHTTPFilter::MHFLoad
	virtual void MHFLoad(RHTTPSession aSession, THTTPFilterHandle aHandle);

private:
	// Constructor
	//##ModelId=3B1E52AC0224
	 CValidationFilter();
	// For 2 phase construction. Also installs the filter.
	//##ModelId=3B1E52AC0219
	void ConstructL(RHTTPSession aSession);

	//##ModelId=3B1E52AC020F
	void ValidateRequestMethodL(RHTTPTransaction aTransaction);

	//##ModelId=3B1E52AC01FB
	TBool ValidateRequestHeadersL(RHTTPTransaction aTransaction);

	//##ModelId=3B1E52AC01E7
	void ValidateRequestGetL(RHTTPTransaction aTransaction);
	//##ModelId=3B1E52AC01DE
	void ValidateRequestPostL(RHTTPTransaction aTransaction);
	//##ModelId=3B1E52AC01D5
	void ValidateRequestPutL(RHTTPTransaction aTransaction);
	//##ModelId=3B1E52AC01D3
	void ValidateRequestTraceL(RHTTPTransaction aTransaction);
	//##ModelId=3B1E52AC01C9
	void ValidateRequestHeadL(RHTTPTransaction aTransaction);

	void ValidateRequestOptionsL(RHTTPTransaction aTransaction);

	//##ModelId=3B1E52AC0205
	/** Validate status codes. Anything from 400->599 is an error or warning
		@return ETrue if the status code indicates a problem with the transaction
	*/
	//##ModelId=3BA60049020A
	TBool ValidateResponseStatusCode(RHTTPTransaction aTransaction);

	/** Fails a request if it contains a body. 
		@return returns ETrue if the request did contain a body
	*/
	//##ModelId=3BA6004901CE
	TBool RequestFailIfContainsBodyL(RHTTPTransaction aTransaction);

	/** Fails and cancels a transaction
		@param aTransaction The transaction to cancel
		@param aStatus The status or error that caused the transaction to fail
	*/
	//##ModelId=3BA600490174
	void FailAndCancelL(RHTTPTransaction aTransaction, THTTPEvent aStatus);


private:
	//##ModelId=3B1E52AC01C1
	TBool iTransactionFailed;

	//##ModelId=3B1E52AC01A3
	RHTTPMessage iCurrentMessage;   // it's a small object
	//##ModelId=3B1E52AC019B
	RStringPool iStringPool;
	
	const TStringTable&    iStringTable;
	// HTTP logger handle, debug only.
	__DECLARE_LOG
	};


#endif // 	__VALIDATIONFILTER_H__

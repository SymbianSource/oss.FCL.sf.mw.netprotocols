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

#ifndef __TCLIENTFILTER_H__
#define __TCLIENTFILTER_H__

// A filter whose job is to pass events to the client while appearing
// as an ordinary filter to the transaction.
//##ModelId=3C4C187D032D
class TClientFilter : public MHTTPFilter
	{
public:	// Methods from MHTTPFilterBase

	// @see MHTTPFilterBase::MHFRunL
	//##ModelId=3C4C187D0378
	virtual void MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent);

	// @see MHTTPFilterBase::MHFSessionRunL
	//##ModelId=3C4C187D0376
	virtual void MHFSessionRunL(const THTTPSessionEvent& aEvent);

	// @see MHTTPFilterBase::MHFRunError
	//##ModelId=3C4C187D036C
	virtual TInt MHFRunError(TInt aError, RHTTPTransaction aTransaction,
							 const THTTPEvent& aEvent);

	// @see MHTTPFilterBase::MHFSessionRunError
	//##ModelId=3C4C187D0369
	virtual TInt MHFSessionRunError(TInt aError, const THTTPSessionEvent& aEvent);

public:	// Methods from MHTTPFilter

/**
	@fn				MHFUnload(RHTTPSession aSession, THTTPFilterHandle aHandle)
	@see			MHTTFilter
 */
	//##ModelId=3C4C187D0362
	virtual void MHFUnload(RHTTPSession aSession, THTTPFilterHandle aHandle);

/**
	@fn				MHFLoad(RHTTPSession aSession, THTTPFilterHandle aHandle)
	@see			MHTTFilter
 */
	//##ModelId=3C4C187D035F
	virtual void MHFLoad(RHTTPSession aSession, THTTPFilterHandle aHandle);

public:	// Methods

	//##ModelId=3C4C187D0355
	inline void SetSessionEventCallback(MHTTPSessionEventCallback* aSessionEventCallback);

private:	// Attributes

	//##ModelId=3C4C187D034E
	MHTTPSessionEventCallback* iSessionEventCallback;

	};


inline void TClientFilter::SetSessionEventCallback(MHTTPSessionEventCallback* aSessionEventCallback)
	{
	iSessionEventCallback = aSessionEventCallback;
	}

#endif // __TCLIENTFILTER_H__

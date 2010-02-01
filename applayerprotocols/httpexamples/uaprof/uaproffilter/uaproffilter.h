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

#ifndef __UAPROFFILTER_H__
#define __UAPROFFILTER_H__

// System Includes
#include <http/mhttpfilter.h>
#include <http/rhttpsession.h>
#include <http/cuaproffilterinterface.h>

// Forward declaration
class MHTTPCpiManager;


/** CUAProfFilter is used to add CPI data to outgoing session connection requests.
*/
class CUAProfFilter : public CUAProfFilterInterface, public MHTTPFilter
	{
public: // methods

	static CUAProfFilter* InstallFilterL(TAny* aFilterParams);
	
	~CUAProfFilter();


public:	// methods from MHTTPFilterBase

	virtual void MHFSessionRunL(const THTTPSessionEvent& aEvent);
	
	virtual TInt MHFSessionRunError(TInt aError, const THTTPSessionEvent& aEvent);
	
	virtual void MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent);
	
	virtual TInt MHFRunError(TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& aEvent);
	
	virtual void MHFLoad(RHTTPSession aSession, THTTPFilterHandle aHandle);

	virtual void MHFUnload(RHTTPSession aSession, THTTPFilterHandle aHandle);
	

private: // methods

	CUAProfFilter(RHTTPSession aSession, MHTTPCpiManager& aUAProfFilterCallback);
	
	void ConstructL();
	
	void GetCpiAndAppendL();
	

private: // attributes

	/** A count to make sure we delete at the correct time
	*/
	TInt iLoadCount;

	/** String pool to be used
	*/
	RStringPool iStringPool;

	/** The current session
	*/
	RHTTPSession iSession;

	/** The manager used for retrieval of Profile and Profile-Diff information
	*/
	MHTTPCpiManager& iCpiManager;
	
	/** The string table which is used for storage of all strings
	*/
	const TStringTable& iStringTable;
	
	/** Array of Profile information
	*/
	RPointerArray<HBufC8> iProfiles;

	/** Array of Profile-Diff information
	*/
	RPointerArray<HBufC8> iProfileDiffs;
	};
	
#endif // __UAPROFFILTER_H__

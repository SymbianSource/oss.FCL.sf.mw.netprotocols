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

#ifndef		__MHTTPCOOKIEFILTER_H__
#define		__MHTTPCOOKIEFILTER_H__

#include <e32base.h>
#include <http/rhttptransaction.h>

class CCookie;

/** Interface for a cookie manager.
	The StoreCookieL and GetCookiesL functions need to be implemented
*/
class MHttpCookieManager
	{
public: 
	/** Store a cookie. This will get called with each cookie that is encountered in a 
		transaction response. 
		Implementations of this interface may leave with any of KErrNoMemory if there is insufficient
		memory or any other system-wide error code if an attempt to append to an internal list fails.
		If this function leaves then the transaction will be failed. 
		Care should be made to make sure that this function leaves only when necessary.
		@param aCookie. The cookie to be stored. Ownership of aCookie is passed to the 
		instantiation of this class.
		@param aUri The Uri of the request that supplied this cookie.  This is required so
		that the manager can complete any missing attributes of the cookie.
	*/
	virtual void StoreCookieL(CCookie* aCookie, const TUriC8& aUri) =0;

	/** Get the relevant cookies for a transaction
		@param	aTransaction	The transaction which cookies are requested for
		@param	aCookieList		An array which will be filled with the cookies for 
			aTransaction. The array should be empty when this function is called.
		@param	aCookie2Reqd	Flag indicating whether a Cookie2 header should be added to the 
								outgoing request headers
	*/
	virtual void GetCookiesL(RHTTPTransaction aTransaction, RArray<CCookie*>& aCookieList, TBool& aCookie2Reqd)=0;
	};

#endif	// __MHTTPCOOKIEFILTER_H__
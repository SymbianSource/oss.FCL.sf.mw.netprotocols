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

#ifndef __HTTPCOOKIEFILTER_H__
#define __HTTPCOOKIEFILTER_H__

#include <http/mhttpfilter.h>
#include <http/rhttpsession.h>
#include <http/ccookiefilterinterface.h>
class MHttpCookieManager;
class CCookie;

/** CCookieFilter is used to trap outgoing requests, to add cookies to them, and incoming 
	responses, to extract cookies from them.
*/
class CCookieFilter : public CCookieFilterInterface, public MHTTPFilter
	{
 public:
	/** Installs the cookie filter, supplying it with this callback interface
		@param aSession the session to install the filter into.
		@leave KErrNoMemory There was not enough memory.
	*/
	static CCookieFilter* InstallFilterL(TAny* aFilterParams);

	/** Standard destructor
	*/
	~CCookieFilter();

	//From MHttpFilter
	virtual void MHFRunL(RHTTPTransaction aTransaction,
						 const THTTPEvent& aEvent);
	virtual TInt MHFRunError(TInt aError, 
							 RHTTPTransaction aTransaction,
							 const THTTPEvent& aEvent);
	virtual void MHFUnload(RHTTPSession aSession, 
						   THTTPFilterHandle aHandle);
	virtual void MHFLoad(RHTTPSession aSession, 
						 THTTPFilterHandle aHandle);

private:
	/** Standard constructor
	*/
	CCookieFilter(RHTTPSession aSession, MHttpCookieManager& aCookieFilterCallback);

	/** Standard C class second phase construction
	*/
	void ConstructL();

	/** Check the response headers of the specified transaction and extract any cookies
		from them for storage
		@param	aTransaction	The transaction to handle the response headers
	*/
	void HandleResponseHeadersL(RHTTPTransaction aTransaction);

	/** Find any cookies which are appropriate to this request and add them to the
		outgoing request headers.
		@param	aTransaction	The transaction to handle the request headers
	*/
	void HandleRequestHeadersL(RHTTPTransaction aTransaction);

	/** Extracts a cookie from the headers and stores it
		@param	aRequestHeaders	The headers which contain the cookie
		@param	aPartIndex		The part index of the cookie within the headers
		@param	aFieldName		The name of the header field we are storing.  Could be
								SetCookie or SetCookie2
		@param	aUri			The Uri that issued this cookie
	*/
	void StoreOneCookieL(RHTTPHeaders aRequestHeaders, TInt aPartIndex, RStringF aFieldName, const TUriC8& aUri);

	/** Adds a specified cookie to an outgoing request
		@param	aRequestHeaders	The request headers to which the cookie should be added
		@param	aCookie			The cookie to add to the headers
		@param	aPartIndex		The index of the cookie
	*/
	void AddCookieToRequestL(RHTTPHeaders aRequestHeaders, const CCookie& aCookie, TInt aPartIndex);

private:
	/** Count of how many times this filter has been loaded - used to ensure 
		destruction happens at the correct time
	*/
	TInt iLoadCount;

	/** String pool to be used by this cookie
	*/
	RStringPool iStringPool;

	/** The current http session
	*/
	RHTTPSession iSession;

	/** The manager which is used for storage and retrieval of cookies
	*/
	MHttpCookieManager& iCookieManager;

	/** The string table which is used for storage of all strings
	*/
	const TStringTable& iStringTable;
	};

#endif //__HTTPCOOKIEFILTER_H__

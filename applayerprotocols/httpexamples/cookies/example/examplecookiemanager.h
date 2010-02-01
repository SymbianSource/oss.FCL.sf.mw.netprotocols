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

#ifndef __EXAMPLECOOKIEMANAGER_H__
#define __EXAMPLECOOKIEMANAGER_H__

#include <http/mhttpcookiemanager.h>

/** CExampleCookieManager is an example implementation of a class that provides 
	the MHttpCookieManager interface

	It is meant as an example for cookie support developers and to be used to test 
	the MHttpCookieManager interface and the CCookieFilter
*/
class CExampleCookieManager: public CBase, public MHttpCookieManager
	{
public:
	/** Standard CClass construction method
	@param	aStringPool	The stringpool to use for the creation of all strings in this manager
	@return	A pointer to a fully constructed CExampleCookieManager
	*/
	IMPORT_C static CExampleCookieManager* NewL(RStringPool aStringPool);

	/** Standard destructor
	*/
	IMPORT_C ~CExampleCookieManager();

	/** Store a cookie. This will get called with each cookie that is encountered in a 
		transaction response.
		@param	aCookie	The cookie to be stored. Ownership of aCookie is passed to this class.
		@param	aUri	The uri that sent this cookie.  This is required to complete any
		missing attributes of the cookie and to ensure the cookie is valid.
		@leave KErrNoMemory If the cookie could not be appended to the array or if attributes
		could not be added to the cookie during validation.
	*/
	IMPORT_C void StoreCookieL(CCookie* aCookie, const TUriC8& aUri);

	/** Get the relevant cookies for a transaction
		@param	aTransaction	The transaction containing the request for which cookies are required
		@param	aCookieList		An array which will be filled with the cookies for aTransaction. 
								The array should be empty when this function is called.  Ownership of
								the cookies remains with the manager.
		@param	aCookie2Reqd	Flag indicating whether a Cookie2 header should be added to the 
								outgoing request headers
	*/
	IMPORT_C void GetCookiesL(RHTTPTransaction aTransaction, RArray<CCookie*>& aCookieList, TBool& aCookie2Reqd);

	/** Return the number of cookies that the manager currently holds.  This function has been added
		for testing purposes but could be used as part of an API to allow users to view, add, delete 
		cookies etc.
		@return	The number of cookies held by the manager.
	*/
	TInt CookieCount();

private:
	/** Standard constructor
	@param	aStringPool	The stringpool to use for the creation of all strings in this manager
	*/
	CExampleCookieManager(RStringPool aStringPool);

	/** Ensure that a cookie is valid for the uri which submitted it.  Also, add the 
	domain, path and port attributes to the cookie if they are missing.
	@param	aCookie	The cookie to be validated
	@param	aUri	The uri which the cookie came from
	@return	ETrue if the cookie is valid, EFalse if not
	*/
	TBool ValidateCookieL(CCookie& aCookie, const TUriC8& aUri);

	/** Check if the specified cookie should be submitted to the specified uri based
	on the domain attribute only
	@param	aCookie	The cookie to be checked
	@param	aUri	The uri which is being requested
	@return	ETrue if the cookie domain attribute matches the uri, EFalse if not
	*/
	TBool CheckDomainMatch(CCookie& aCookie, const TUriC8& aUri) const;

	/** Check if the specified cookie should be submitted to the specified uri based
	on the path attribute only
	@param	aCookie	The cookie to be checked
	@param	aUri	The uri which is being requested
	@return	ETrue if the cookie path attribute matches the uri, EFalse if not
	*/
	TBool CheckPathMatch(CCookie& aCookie, const TUriC8& aUri) const;

	/** Check if the specified cookie should be submitted to the specified uri based
	on the port attribute only
	@param	aCookie	The cookie to be checked
	@param	aUri	The uri which is being requested
	@return	ETrue if the cookie port attribute matches the uri, EFalse if not
	*/
	TBool CheckPortMatch(CCookie& aCookie, const TUriC8& aUri) const;

	/** Check if the specified cookie should be submitted to the specified uri based
	on the secure attribute only.  If a cookie contains the secure attribute then it
	should only be submitted to a secure uri (ie one with a scheme of https).
	@param	aCookie	The cookie to be checked
	@param	aUri	The uri which is being requested
	@return	ETrue if the cookie secure attribute matches the uri, EFalse if not
	*/
	TBool CheckSecureMatch(CCookie& aCookie, const TUriC8& aUri) const;

	/** Compares two cookies to see which has the most specific path.  Used for inserting
	cookies in order to the array returned from GetCookiesL. A more specific path is one which
	contains more elements. ie /home/images/holiday is more specific than /home/images.
	This comparison is achieved by counting the number of /s in the path.
	The strange parameter type is required because the two parameters must be const 
	references to whatever is contained in the array (in this case CCookie*).  To achieve
	this the format is CCookie* const & aCookie1.
	@param	aCookie1	The first cookie to be compared
	@param	aCookie2	The second cookie to be compared
	@return	Positive if aCookie1 has more /s than aCookie2, 0 if equal, negative 
	if aCookie1 has less /s aCookie2
	*/
	static TInt CompareCookiePaths(CCookie* const & aCookie1, CCookie* const & aCookie2);

	/** Count the number of path separators (/) in a given descriptor.
	@param	aDes	The descriptor to count the separators in
	@return	The number of separators
	*/
	static TInt CountSeparators(const TDesC8& aDes);

	/** Returns a descriptor which is equivalent to the input descriptor without
	double quotes surrounding it. If the input descriptor does not have double quotes
	surrounding it then it is returned as is.  This is required because cookie attributes 
	can optionally be surrounded by double quotes.
	@param	aDes	The descriptor to remove the quotes from
	@return	Descriptor which points to the part of aDes with no quotes
	*/
	TPtrC8 RemoveQuotes(const TDesC8& aDes) const;

private:
	/** Array of cookies for the current session
	*/
	RPointerArray<CCookie> iCookies;

	/** The stringpool which is used for all strings in the manager
	*/
	RStringPool iStringPool;
	};

inline TInt CExampleCookieManager::CookieCount()
	{
	return iCookies.Count();
	}

#endif //__EXAMPLECOOKIEMANAGER_H__


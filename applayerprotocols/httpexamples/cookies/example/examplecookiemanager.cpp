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

#include "examplecookiemanager.h"
#include "httpcookieerr.h"

#include <http/ccookie.h>
#include <http/thttphdrval.h>

EXPORT_C CExampleCookieManager* CExampleCookieManager::NewL(RStringPool aStringPool)
	{
	CExampleCookieManager* cookieManager = new(ELeave)CExampleCookieManager(aStringPool);
	return cookieManager;
	}

EXPORT_C CExampleCookieManager::~CExampleCookieManager()
	{
	// At this point a real cookie manager would go through the list of cookies
	// and destroy any that don't have an Expires or MaxAge attribute (because they 
	// should only last for the session).
	// Any remaining ones should be persisted till the next session.

	// However, we don't persist for this implementation
	iCookies.ResetAndDestroy();
	}

CExampleCookieManager::CExampleCookieManager(RStringPool aStringPool) :
	iStringPool(aStringPool)
	{
	}

EXPORT_C void CExampleCookieManager::StoreCookieL(CCookie* aCookie, const TUriC8& aUri)
	{
	CleanupStack::PushL(aCookie);

	if(ValidateCookieL(*aCookie, aUri))
		{
		User::LeaveIfError(iCookies.Append(aCookie));
		CleanupStack::Pop(aCookie);
		}
	else
		CleanupStack::PopAndDestroy(aCookie);
	}

EXPORT_C void CExampleCookieManager::GetCookiesL(RHTTPTransaction aTransaction, 
												 RArray<CCookie*>& aCookieList,
												 TBool& aCookie2Reqd)
	{
	// For comparing cookies to insert them in the correct order to the returned array
	TLinearOrder<CCookie*> compareFunc(CompareCookiePaths);

	// Ensure the array is empty
	__ASSERT_DEBUG(aCookieList.Count()==0, HTTPCookiePanic::Panic(HTTPCookiePanic::EGetCookiesArrayNotEmpty));

	const TInt numCookies = iCookies.Count();
	for (TInt ii=0; ii<numCookies; ++ii)
		{
		CCookie* cookie = iCookies[ii];
		const TUriC8& uri = aTransaction.Request().URI();

		if(CheckDomainMatch(*cookie, uri) &&
		   CheckPathMatch(*cookie, uri) &&
		   CheckPortMatch(*cookie, uri) &&
		   CheckSecureMatch(*cookie, uri))
			{
			User::LeaveIfError(aCookieList.InsertInOrderAllowRepeats(cookie, compareFunc));

			if(!cookie->FromCookie2())
				aCookie2Reqd = ETrue;
			}
		}
	}

TBool CExampleCookieManager::CheckDomainMatch(CCookie& aCookie, const TUriC8& aUri) const
	{
	TChar domainSep = '.';

	if(aUri.IsPresent(EUriHost))
		{
		THTTPHdrVal attributeVal;
		aCookie.Attribute(CCookie::EDomain, attributeVal);

		const TDesC8& domain = aUri.Extract(EUriHost);
		const TPtrC8 cookieDomain = RemoveQuotes(attributeVal.StrF().DesC());

		// Domain matching rules:
		// if the cookie domain doesn't start with a dot then it must match the uri domain exactly
		// if it does start with a dot and it 
		TInt matchLoc = domain.FindF(cookieDomain);
		if((cookieDomain[0] != TUint(domainSep))		&& 
			(matchLoc == 0)								&& 
			(domain.Length() == cookieDomain.Length()))
			return ETrue;
		else if((matchLoc != KErrNotFound) &&
				(domain.Left(matchLoc).Locate(domainSep) == KErrNotFound))
				return ETrue;
		}

	return EFalse;
	}

TBool CExampleCookieManager::CheckPathMatch(CCookie& aCookie, const TUriC8& aUri) const
	{
	THTTPHdrVal attributeVal;
	aCookie.Attribute(CCookie::EPath, attributeVal);
	TPtrC8 cookiePath = RemoveQuotes(attributeVal.StrF().DesC());

	const TDesC8& uriPath = aUri.Extract(EUriPath);
	if(uriPath.Length() == 0)
		{
		// if the uri has no path then it matches against no cookie path
		// or a cookie path of just a /
		const TInt pathLength = cookiePath.Length();
		if(pathLength == 0 || pathLength == 1)
			return ETrue;
		}
	else if(uriPath.FindF(cookiePath) == 0)
		{
		TChar separator('/');
		// Check that the character after the matched bit is a / otherwise
		// /path would match against /path2
		const TInt uriLength = uriPath.Length();
		const TInt cookieLength = cookiePath.Length();

		if(uriLength == cookieLength)
			return ETrue;
		else if(uriLength > cookieLength)
			{
			if(cookiePath[cookieLength - 1] == TUint(separator))
				return ETrue;
			else if(uriPath[cookieLength] == TUint(separator))
				return ETrue;
			}
		}

	return EFalse;
	}

TBool CExampleCookieManager::CheckPortMatch(CCookie& aCookie, const TUriC8& aUri) const
	{
	THTTPHdrVal val;
	if(aCookie.Attribute(CCookie::EPort, val) == KErrNone)
		{
		TChar portSeparator(',');
		_LIT8(KDefaultPort, "80");

		const TDesC8& port = aUri.IsPresent(EUriPort)? aUri.Extract(EUriPort) : KDefaultPort();

		const TPtrC8& portList = RemoveQuotes(val.StrF().DesC());
		TInt portPos = portList.FindF(port);
		// if we do not find the port in the list then do not match
		if(portPos == KErrNotFound)
			return EFalse;
		// if the number was the last in the list then match
		else if((portPos + port.Length()) == portList.Length())
			return ETrue;
		// check that the number is followed by a separator ie do not match 80 with 8000
		else if(portList[portPos + port.Length()] == TUint(portSeparator))
			return ETrue;
		// we have not found a match
		else
			return EFalse;
		}

	// If the cookie does not have a portlist return ETrue to match any port
	return ETrue;
	}

TBool CExampleCookieManager::CheckSecureMatch(CCookie& aCookie, const TUriC8& aUri) const
	{
	THTTPHdrVal val;
	TBool secureCookie = aCookie.Attribute(CCookie::ESecure, val) == KErrNone;

	// if the cookie is not secure we don't care about the uri
	if(!secureCookie)
		return ETrue;

	// Check if the scheme is https - if there is no scheme then assume not
	if(aUri.IsPresent(EUriScheme))
		{
		_LIT8(KSecureScheme, "https");
		const TDesC8& scheme = aUri.Extract(EUriScheme);
		return scheme.CompareF(KSecureScheme()) == 0;
		}

	// The cookie is secure and we don't have a secure transaction
	return EFalse;
	}

TBool CExampleCookieManager::ValidateCookieL(CCookie& aCookie, const TUriC8& aUri)
	{
	THTTPHdrVal attributeVal;

	if(aCookie.Attribute(CCookie::EPath, attributeVal) == KErrNone)
		{
		// if the path attribute exists check it is a prefix of the path
		// of the uri that issued it (if not reject)
		RStringF cookiePath = attributeVal.StrF();
		const TDesC8& uriPath = aUri.Extract(EUriPath);
		if(uriPath.FindF(RemoveQuotes(cookiePath.DesC())) != 0)
			return EFalse;
		}
	else
		{
		// if the path attribute doesn't exist add it
		THTTPHdrVal val(iStringPool.OpenFStringL(aUri.Extract(EUriPath)));
		aCookie.SetAttributeL(CCookie::EPath, val);
		}

	if(aCookie.Attribute(CCookie::EDomain, attributeVal) == KErrNone)
		{
		const TChar dot('.');
		const TDesC8& cookieDomain = attributeVal.StrF().DesC();
		const TDesC8& uriDomain = aUri.Extract(EUriHost);

		// if the domain does not exactly match the uri and does not begin
		// with a dot then add one
		if((cookieDomain.Compare(uriDomain) != 0) &&
		   (cookieDomain.Locate(dot) != 0))
			{
			_LIT8(KAddDotString, ".%S");
			HBufC8* newDomain = HBufC8::NewLC(cookieDomain.Length() + 1);
			newDomain->Des().AppendFormat(KAddDotString(), &cookieDomain);

			RStringF domain = iStringPool.OpenFStringL(*newDomain);
			CleanupStack::PopAndDestroy(newDomain);

			THTTPHdrVal val(domain);
			aCookie.SetAttributeL(CCookie::EDomain, val);
			domain.Close();
			}

		// if the domain does not contain an embedded dot then reject it
		// ie reject .com or .com.
		// Start by removing one character from each end. ie start at pos 1 and take a length
		// which is 2 shorter than the original descriptor
		TPtrC8 domainMiddle = cookieDomain.Mid(1, cookieDomain.Length() - 2);
		if(domainMiddle.Locate(dot) == KErrNotFound)
			return EFalse;

		// Reject the cookie if the domain differs by two or more levels from the uri
		// ie if uri=www.x.y.com then accept a cookie with .x.y.com but reject .y.com
		TInt pos = uriDomain.FindF(cookieDomain);
		if(pos > 2)
			{
			const TDesC8& domainDiff = uriDomain.Left(pos);

			// Remove one character from each end. ie start at pos 1 and take a length
			// which is 2 shorter than the original descriptor
			const TDesC8& diffMiddle = domainDiff.Mid(1, domainDiff.Length() - 2);
			if(diffMiddle.Locate(dot) != KErrNotFound)
				return EFalse;
			}
		}
	else
		{
		// if the domain attribute is not found add it
		THTTPHdrVal val(iStringPool.OpenFStringL(aUri.Extract(EUriHost)));
		aCookie.SetAttributeL(CCookie::EDomain, val);
		val.StrF().Close();
		}

	if(!CheckPortMatch(aCookie, aUri))
		return EFalse;

	return ETrue;
	}

TInt CExampleCookieManager::CompareCookiePaths(CCookie* const & aCookie1, CCookie* const & aCookie2)
	{
	THTTPHdrVal path1Val;
	THTTPHdrVal path2Val;

	// if the first cookie has no path then it must be smaller
	if(aCookie1->Attribute(CCookie::EPath, path1Val) == KErrNotFound)
		return -1;

	if(aCookie2->Attribute(CCookie::EPath, path2Val) == KErrNotFound)
		return 1;

	TInt path1Count = CountSeparators(path1Val.StrF().DesC());
	TInt path2Count = CountSeparators(path2Val.StrF().DesC());

	return path1Count - path2Count;
	}
TInt CExampleCookieManager::CountSeparators(const TDesC8& aDes)
	{
	const TChar pathSeparator('/');
	TInt numSeps = 0;

	// Get all the descriptor to start with
	TPtrC8 desPtr = aDes.Mid(0);
	TInt sepPos = desPtr.Locate(pathSeparator);
	while(sepPos != KErrNotFound)
		{
		++numSeps;

		// Get the rest of the descriptor without the separator that we have found
		desPtr.Set(desPtr.Mid(sepPos + 1));
		sepPos = desPtr.Locate(pathSeparator);
		}

	return numSeps;
	}

TPtrC8 CExampleCookieManager::RemoveQuotes(const TDesC8& aDes) const
	{
	TChar quote('"');

	if(aDes[0] == TUint(quote))
		{
		return aDes.Mid(1, aDes.Length() - 2);
		}
	else
		return aDes;
	}


// Copyright (c) 2000-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Purpose:		Definition of CUrl class - Url processor based on RFC2396.
// CUrl encapsulates a url and provides access to its components. On creation, the contents of 
// the url is in unescaped mode - excluded characters (as defined by RFC2396) have not been coded into their 
// escape triples. Two NewL() functions are provided. The first takes any descriptor and encapsulates it into
// the CUrl object, and leaves if the the url begins with a ':' (EWapErrCorruptUrl) or if the scheme is 
// 
//

//				corrupted (EWapErrCorruptScheme). The second NewL() creates a url with a file scheme (file://) from a 
//				TParseBase argument, and will leave if the url is invalid (EWapErrCorruptUrl). Two static functions have 
//				been provided that each take a descriptor argument which is then escape encoded/decoded.
//
//				NOTE - maintainer needs detailed knowledge of url parsing (RFC2396)
//

// System includes
//
#include <Utf.h>

// CUrl class signature
//
#include "urlbase.h"

// Static data used in this class
//
const TInt KCUrlInvalidCharPos		= -1;
const TInt KUrlConversionBufferSize = 50;

//	Constants 
_LIT(KHexDigit, "0123456789ABCDEF");
_LIT(KUnwiseData, "{}|\\^[]\'");
_LIT(KDelimsData, "<>#%\"");
_LIT(KReservedData, ";/?:@&=+$,");
_LIT(KReservedDataForPath, ";?@&=+$,");

//	Delimiters and scheme identifiers for urls
_LIT(KUrlPathDelimiter,"/");		
_LIT(KUrlFileSchemeSignature,"file://"); 
_LIT(KUrlLoc,"//");
_LIT(KUrlColon,":");
_LIT(KUrlQMark,"?");
_LIT(KUrlHash,"#");

// Implementation of CUrl
//

EXPORT_C CUrl* CUrl::NewL(const TDesC& aUrl)
//
//	Static factory c'tor.
	{
	CUrl* url = new(ELeave) CUrl();
	CleanupStack::PushL(url);
	url->ConstructL(aUrl);
	CleanupStack::Pop();
	return url;
	}

EXPORT_C CUrl* CUrl::NewL(const TParseBase& aFileName)
//
//	Static factory c'tor. Used for creating CUrl object for a file on local file system
	{
	CUrl* url = new(ELeave) CUrl();
	CleanupStack::PushL(url);
	url->ConstructL(aFileName);
	CleanupStack::Pop();
	return url;	
	}

CUrl::CUrl()
//
//	Default c'tor
	{
	}

EXPORT_C CUrl::~CUrl()
//
//	D'tor
	{
	delete iUrlDes;
	}

void CUrl::ConstructL(const TDesC& aUrl)
//
//	Non-trivial c'tor - can be used for all general urls
	{
	// Stripe any leading whitespace
	TPtrC url = aUrl;
	while( url.Locate(' ') == 0 )
		{
		// Remove the leading whitespace -> set pointer to second character
		url.Set(url.Mid(1));
		}
	iUrlDes = url.AllocL();

	// Check to see if there's ':' at start of aUrl
	TInt colonPos = aUrl.Locate(':');
	if (colonPos == 0)
		User::Leave(EWapErrCorruptUrl);
	TPtrC scheme(Component(EUrlScheme));
	CheckSchemeValidL(scheme);
	}

void CUrl::CheckSchemeValidL(const TDesC& aScheme) const
	{ // Check the scheme, leave if corrupt
	for (TInt i=0; i < aScheme.Length(); i++)
		{
		TChar c = aScheme[i];
		TBool ok =  (c>='a' &&  c<='z') || (c>='A'  && c <='Z');

		if (!ok && i > 0)
			ok = (c>='0' && c<='9') || c == '+' || c == '-' || c == '.';

		if (!ok)
			User::Leave(EWapErrCorruptScheme);
		}
	}

void CUrl::ConstructL(const TParseBase& aFileName)
//
//	Non-trivial c'tor - used to create Urls for local file space only. NO scheme in aFileName
	{
	if( aFileName.IsWild() ) 
		User::Leave(EWapErrCorruptUrl);

	// Convert Unicode file name into UTF8 format
	HBufC8* utf8 = ConvertFromUnicodeToUtf8L(aFileName.FullName());
	CleanupStack::PushL(utf8);

	// Convert to 16-bit format
	HBufC* escapedFullPath = HBufC::NewLC(utf8->Length());
	escapedFullPath->Des().Copy(*utf8);
	
	// Need to escape encode all the components - start with filename, then path and last the drive
	TPtr fullPath = escapedFullPath->Des();
	if ( fullPath.Compare(KNullDesC) )
		{
		// Convert '\' to '/' then escape encode with EUrlPath - this doesn't encode '/' and ':'
		for (TInt i=0; i<fullPath.Length(); ++i)
			{
			if (fullPath[i] == KPathDelimiter)
				fullPath[i] = '/';
			}
		escapedFullPath = EscapeEncodeL(fullPath, EUrlPath);
		CleanupStack::PopAndDestroy();	// escapedFullPath (old version)
		CleanupStack::PushL(escapedFullPath);	// new version

		// Replace ':' with '|'
		fullPath.Set(escapedFullPath->Des());
		TInt colonPos = fullPath.Locate(KDriveDelimiter);
		if( colonPos != KErrNotFound )
			fullPath[colonPos] = '|';
		}
	// Form the url
	TInt size = KUrlFileSchemeSignature().Length() + KUrlPathDelimiter().Length() + fullPath.Length();

	iUrlDes = HBufC::NewL(size);
	iUrlDes->Des().Append(KUrlFileSchemeSignature());
	iUrlDes->Des().Append(KUrlPathDelimiter());
	iUrlDes->Des().Append(fullPath);

	CleanupStack::PopAndDestroy(2);	// escapedFullPath, utf8
	}

EXPORT_C CUrl* CUrl::ResolveL(CUrl& aBaseUrl, CUrl& aRelativeUrl)
//
//	Based on the relative parsing algorithm in RFC2396
	{
	//	Return copy of aRelativeUrl if aBaseUrl is empty
	if (aBaseUrl.UrlDes().Compare(KNullDesC)==0)
		return aRelativeUrl.AllocL();

	TPtrC relativeUrl(aRelativeUrl.UrlDes());

	TPtrC relPath(aRelativeUrl.Component(EUrlPath));
	TPtrC relAuth(aRelativeUrl.Component(EUrlAuthority));
	TPtrC relScheme(aRelativeUrl.Component(EUrlScheme));
	TPtrC relQuery(aRelativeUrl.Component(EUrlQuery));
	TPtrC relFragment(aRelativeUrl.Component(EUrlFragment));

	if (relScheme.Length() > 0)
		{
		// LOOPHOLE in RFC 1630 if schemes match then remove the scheme from the relative url
		if (aBaseUrl.UrlDes().FindF(relScheme) == 0)
			relativeUrl.Set(relativeUrl.Mid(relScheme.Length() + 1)); // remove the ':' as well
		else // the relative url is absolute
			return NewL(relativeUrl);
		}

	TBool useBaseAuth = ETrue;
	TBool useRelQuery = ETrue;
	TPtrC basePath(aBaseUrl.Component(EUrlPath));
	HBufC* resolvedPath = NULL;
	if (relPath.Compare(KNullDesC)==0 && relAuth.Compare(KNullDesC)==0 
		&& relScheme.Compare(KNullDesC)==0 && relQuery.Compare(KNullDesC)==0) // relative URL could just be a fragment
		{
		// Use current document url (assume that it is aBaseUrl), including query, then add relative URL fragment
		useRelQuery = EFalse;
		resolvedPath = basePath.AllocLC();
		}
	else if (relativeUrl.Find(KUrlLoc) == 0) // relative url is a network path
		{
		// Set resolved path to be the relative path
		useBaseAuth = EFalse;
		resolvedPath = relPath.AllocLC();
		}	
	else if (relPath.Locate('/') == 0) // relative url is an absolute path
		{
		resolvedPath = relPath.AllocLC();
		}
	else 
		{
		//	Do path resolution, merge the base path and relative path
		if (relPath.Length() != 0)
			// if the relative path is a query or fragment then shouldn't strip the document from the basePath
			{
			TInt endBasePath = basePath.LocateReverse('/');
			if (endBasePath != KErrNotFound)
				basePath.Set(basePath.Left(endBasePath + 1)); // keep the '/' 
			else
				basePath.Set(_L("/"));	//	Create path of just '/'
			}
		//	Resolve relative path against base path
		resolvedPath = HBufC::NewLC(relPath.Length() + basePath.Length());
		TRelativePaths relativePaths(basePath, relPath, resolvedPath->Des());
		relativePaths.ResolveRelativePaths();
		}

	// put the url together
	TPtrC baseScheme(aBaseUrl.Component(EUrlScheme));
	TPtrC baseAuth(aBaseUrl.Component(EUrlAuthority));
	TPtrC baseQuery(aBaseUrl.Component(EUrlQuery));

	HBufC* resolvedUrl = HBufC::NewLC(aBaseUrl.UrlDes().Length()  
										 + relativeUrl.Length()
										 + KUrlColon().Length() 
										 + KUrlLoc().Length()
										 + KUrlQMark().Length()
										 + KUrlHash().Length()
										 + 1); // this will be long enough - extra 1 just in case basePath was empty
	TPtr resolvedBuf = resolvedUrl->Des();

	if (baseScheme.Length() > 0)
		{
		resolvedBuf.Append(baseScheme);
		resolvedBuf.Append(KUrlColon);
		}

	resolvedBuf.Append(KUrlLoc);	

	if (useBaseAuth && baseAuth.Length() >0)
		{
		resolvedBuf.Append(baseAuth);
		}
	else if (relAuth.Length() > 0)
		{
		resolvedBuf.Append(relAuth);
		}

	resolvedBuf.Append(*resolvedPath);

	if (useRelQuery && relQuery.Length() >0)
		{
		resolvedBuf.Append(KUrlQMark);
		resolvedBuf.Append(relQuery);
		}
	else if (!useRelQuery && baseQuery.Length() >0)
		{
		resolvedBuf.Append(KUrlQMark);
		resolvedBuf.Append(baseQuery);
		}

	if (relFragment.Length() >0)
		{
		resolvedBuf.Append(KUrlHash);
		resolvedBuf.Append(relFragment);
		}

	CUrl * url = CUrl::NewL(*resolvedUrl);
	CleanupStack::PopAndDestroy(2); // resolvedUrl, resolvedPath

	return url;
	}

void CUrl::Panic(TPanicCode aPanicCode) const
	{
	_LIT(KWapCUrl,"Wap - CUrl");
	User::Panic(KWapCUrl, aPanicCode);
	}

void CUrl::Part(TComponent aComponent, const TDesC& aUrl, TInt& aStartPos, TInt& aEndPos) const
//
// Takes a descriptor as a url and parses it for the start and end positions of a particular component. 
// KCUrlInvalidCharPos is used to indicate that component is not in url.
// 
// Based on RFC2396
	{
	aStartPos = aEndPos = KCUrlInvalidCharPos;

	TPtrC url = aUrl;
	// search for first of ':' | '/' | '?' | '#'
	TInt stripped =0;

	TInt colonPos = aUrl.Locate(':');
	__ASSERT_ALWAYS(colonPos != 0,Panic(EInvalidUrl));		// if ':' is first character then it is invalid
	TInt hashPos = aUrl.Locate('#');
	TInt slashPos = aUrl.Locate('/');
	TInt queryPos = aUrl.Locate('?');

	colonPos = colonPos == KErrNotFound ? KMaxTInt : colonPos;
	hashPos = hashPos == KErrNotFound ? KMaxTInt : hashPos;
	slashPos = slashPos == KErrNotFound ? KMaxTInt : slashPos;
	queryPos = queryPos == KErrNotFound ? KMaxTInt : queryPos;

	// if ':' is before others then there is a scheme so extract it
	if (colonPos < hashPos && colonPos < slashPos && colonPos < queryPos)
		{
		if (aComponent == EUrlScheme)
			{
			aStartPos = stripped;
			aEndPos = colonPos -1;
			return;
			}

		if (url.Length() == colonPos + 1) // reached the end of the url
			return;	
		url.Set(url.Mid(colonPos+1));
		stripped += colonPos+1;
		hashPos -= colonPos+1;
		slashPos -= colonPos+1;
		queryPos -= colonPos+1;
		}

	// if first 2 characters are '//' then a host exists, 
	if (url.Find(KUrlLoc) == 0)
		{
		TInt urlLocLength = KUrlLoc().Length();
		if (url.Length() == urlLocLength) // reached the end of the url
			return;
		// extract '//' and rescan for '/'
		url.Set(url.Mid(urlLocLength));
		stripped += urlLocLength;
		hashPos -= urlLocLength;
		queryPos -= urlLocLength;
		slashPos = url.Locate('/');
		slashPos = slashPos == KErrNotFound ? KMaxTInt : slashPos;

		// host is ended by first of '#' | '?' | '/' | end of url
		TInt hostEndCharPos = slashPos;
		hostEndCharPos = hashPos < hostEndCharPos ? hashPos : hostEndCharPos;
		hostEndCharPos = queryPos < hostEndCharPos ? queryPos : hostEndCharPos;
		hostEndCharPos = url.Length() < hostEndCharPos ? url.Length() : hostEndCharPos;
	
		if (aComponent == EUrlAuthority)
			{
			aStartPos = stripped;
			aEndPos = hostEndCharPos+stripped-1;
			return;
			}
		if (aComponent == EUrlLocation ||
			aComponent == EUrlUsername ||
			aComponent == EUrlPassword)
			{
			aStartPos = stripped;
			aEndPos = hostEndCharPos+stripped-1;
			// We need part of the authority. Extract it
			PartOfAuthority(aComponent, aUrl, aStartPos, aEndPos);
			return;
			}
		//	Have we reached the end of the url
		if (url.Length() == hostEndCharPos)
			return; 
		// extract host	
		url.Set(url.Mid(hostEndCharPos));
		stripped += hostEndCharPos;
		slashPos -= hostEndCharPos;
		hashPos -= hostEndCharPos;
		queryPos -= hostEndCharPos;		
		}

	//	If first character is '/' | this is the start of a relative url | there is no authority then path exists
	//	as long as it isn't just a query or fragment
	if ((slashPos == 0 || stripped == 0 || stripped == colonPos +1 ) && hashPos != 0 && queryPos !=0)
		{ 
		TInt pathEndCharPos = queryPos;
		pathEndCharPos = hashPos < pathEndCharPos ? hashPos : pathEndCharPos;
		pathEndCharPos = url.Length() < pathEndCharPos ? url.Length() : pathEndCharPos;
		if (aComponent == EUrlPath)
			{
			aStartPos = stripped;
			aEndPos = pathEndCharPos+stripped-1;
			return;
			}
		if (url.Length() == pathEndCharPos) // reached the end of the url
			return;
		//	extractPath
		url.Set(url.Mid(pathEndCharPos));
		stripped += pathEndCharPos ;
		queryPos -= pathEndCharPos ;
		hashPos -= pathEndCharPos ;
		}

	// if first is '?' then query exists
	if (queryPos == 0) 	
		{
		// extract ?, query is ended by '#' | end of url
		if (url.Length() == 1)	// reached the end of the url
			return;
		url.Set(url.Mid(1));
		stripped += 1;
		hashPos -= 1;
		// extract query
		TInt queryEndCharPos = hashPos;
		queryEndCharPos = url.Length() < queryEndCharPos ? url.Length() : queryEndCharPos;
		if (aComponent == EUrlQuery)
			{
			aStartPos = stripped;
			aEndPos = queryEndCharPos+stripped-1;
			return;
			}
		if (url.Length() == queryEndCharPos) // reached the end of the url
			return;
		url.Set(url.Mid(queryEndCharPos));
		stripped += queryEndCharPos;
		hashPos -= queryEndCharPos;
		}

	if (hashPos == 0)
		{
		if (url.Length() == 1)	// reached the end of the url
			return;
		// extract hash
		url.Set(url.Mid(1));
		stripped += 1;
		// fragment left
		if (aComponent == EUrlFragment)
			{
			aStartPos = stripped;
			aEndPos = stripped + url.Length() -1;
			return;
			}
		}
	__ASSERT_DEBUG(	aStartPos == aEndPos && aEndPos == KCUrlInvalidCharPos, Panic(EInvalidUrl));
	}

void CUrl::PartOfAuthority(TComponent aComponent, const TDesC& aUrl, TInt& aStartPos, TInt& aEndPos) const
	{
	if (aEndPos < aStartPos)
		{
		// We don't have what the user asked for
		aStartPos = KCUrlInvalidCharPos;
		aEndPos = KCUrlInvalidCharPos;
		return;
		}

	TPtrC authority = aUrl.Mid(aStartPos, aEndPos - aStartPos);
	TInt endPos = aEndPos - aStartPos;
	TInt colonPos = authority.Locate(':');
	TInt atPos = authority.Locate('@');
	if (atPos == KErrNotFound)
		{
		// There isn't a username or password.
		if (aComponent != EUrlLocation)
			{
			// We don't have what the user asked for
			aStartPos = KCUrlInvalidCharPos;
			aEndPos = KCUrlInvalidCharPos;
			}
		// Else all we've got is the location, so we can return
		// without doing anything
		return;
		}

	// We have an @.
	if (aComponent == EUrlLocation)
		{
		if (atPos != endPos - 1)
			{
			// We have a location of non-zero length
			aStartPos += atPos + 1;
			return;
			}
		else
			{
			aStartPos = KCUrlInvalidCharPos;
			aEndPos = KCUrlInvalidCharPos;			
			}
		}
	else
		{
		// Either username or password
		if (aComponent == EUrlUsername)
			{
			if (colonPos == KErrNotFound || colonPos > atPos)
				{
				// No password
				aEndPos = aStartPos + atPos - 1;
				return;
				}
			else
				{
				aEndPos = aStartPos + colonPos - 1;
				return;
				}
			}
		else
			{
			// They want the password
			if (colonPos == KErrNotFound || colonPos > atPos)
				{
				// There isn't a password
				aStartPos = KCUrlInvalidCharPos;
				aEndPos = KCUrlInvalidCharPos;
				}
			else
				{
				aEndPos = aStartPos + atPos - 1;
				aStartPos += colonPos + 1;
				}
			}
		}
	}

EXPORT_C const TPtrC CUrl::Component(TComponent aType) const
	{
	TInt start = KCUrlInvalidCharPos;
	TInt end = KCUrlInvalidCharPos;

	TComponent type(aType);
	if (aType == EUrlFileName)
		type = EUrlPath;


	Part(type, *iUrlDes, start,end);
	if  (start == KCUrlInvalidCharPos || end == KCUrlInvalidCharPos)
			return KNullDesC();

	TInt length = end - start +1; 
	if (end == (*iUrlDes).Length())
		length--;

	TPtrC component((*iUrlDes).Mid(start, length));

	if (aType == EUrlFileName)
		{
		TInt slashPos = component.LocateReverse('/');
		if (slashPos == KErrNotFound)
			return KNullDesC();
		else
			component.Set(component.Right(component.Length() - (slashPos +1)));
		}

	return component;
	}


EXPORT_C void CUrl::SetComponentL(TComponent aComponent, const TDesC& aValue)
	{
	TInt start, end;
	// As a crude first step, we always resize the URL descriptor to
	// be longer than it currently is by 4 characters longer than the
	// value. Then at the end, we'll resize it to be the right length.
	// 4 allows :// to be inserted if a scheme is set when none is present
	iUrlDes=iUrlDes->ReAllocL(iUrlDes->Length() + aValue.Length() + 4);

	Part(aComponent, *iUrlDes, start, end);
		
	if (aComponent == EUrlScheme && start == KCUrlInvalidCharPos && end == KCUrlInvalidCharPos)
		{ // We're trying to add a scheme, but there isn't one already. Check if scheme valid
		  // There could already be a // at the start, but there can't be a : at the start
		CheckSchemeValidL(aValue);
		// insert location if required
		TPtr wptr(iUrlDes->Des());
		// insert // at the start
		if(wptr.Find(KUrlLoc)!=0)
			wptr.Insert(0,KUrlLoc);
		// insert scheme and a colon
		wptr.Insert(0,KUrlColon);
		wptr.Insert(0,aValue);
		}
	else if ((aComponent == EUrlUsername || aComponent == EUrlPassword) && 
		(start == KCUrlInvalidCharPos || end == KCUrlInvalidCharPos))
		{
		// We're trying to add a username or password, but there isn't
		// one already, so the surrounding punctuation won't be there.
		if (aComponent == EUrlUsername)
			{
			Part(EUrlAuthority, *iUrlDes, start, end);
			// If there isn't an authority, this is too complex.
			if (start == KCUrlInvalidCharPos)
				User::Leave(KErrNotSupported);

			// We've now found the correct start position. Check if
			// the password exists
			TInt scratch;
			Part(EUrlPassword, *iUrlDes, end, scratch);
			if (end == KCUrlInvalidCharPos)
				{
				// No username or password. This means we need to add the @
				iUrlDes->Des().Insert(start, _L("@"));
				end = start;
				}
			else
				{
				// There is a password, so end currently points after
				// the colon.
				end--;
				}
			}
		else
			{
			// aComponent == EUrlPassword
			TInt scratch;
			Part(EUrlUsername, *iUrlDes, scratch, start);
			if (start == KCUrlInvalidCharPos)
				{
				// No username. We need to find the start of the
				// location, and add :@ after it and insert the
				// password between them.

				
				Part(EUrlLocation, *iUrlDes, start, scratch);
				iUrlDes->Des().Insert(start, _L(":@"));
				}
			else
				{
				// There is a username but no password. Add a colon at
				// the end of it and then add the password afterwards.
				iUrlDes->Des().Insert(++start, KUrlColon);
				}
			start++;
			end = start;
			}
		}
	else if (start == KCUrlInvalidCharPos || end == KCUrlInvalidCharPos)
		{
		// A complex case we can't deal with
		User::Leave(KErrNotSupported);
		}
	else
		{
		// The easy case; end currently points at the last character;
		// we want it to point one after.
		end++;
		}
	if (start != KCUrlInvalidCharPos && end != KCUrlInvalidCharPos)
		{ // this check is necessary to catch the case where
		  // we set a scheme and there wasn't one already
		iUrlDes->Des().Replace(start, end - start, aValue);
		}
	iUrlDes=iUrlDes->ReAllocL(iUrlDes->Length());
	}

EXPORT_C TBool CUrl::operator==(CUrl& aUrl) const
	{
	return !Compare(aUrl,EUrlGenericCompare);
	}

EXPORT_C TInt CUrl::Compare(CUrl& aUrl, TInt aCompareComps) const
//
//	Scheme is case insensitive, rest of url is case sensitive
	{
	TInt result =0;
	if (aCompareComps & EUrlScheme)
		{
		result += Component(EUrlScheme).CompareF(aUrl.Component(EUrlScheme));
		if (result !=0)
			return result;
		}
		
	if (aCompareComps & EUrlLocation)
		{
		result += Component(EUrlLocation).Compare(aUrl.Component(EUrlLocation));
		if (result !=0)
			return result;
		}

	if (aCompareComps & EUrlUsername)
		{
		result += Component(EUrlUsername).Compare(aUrl.Component(EUrlUsername));
		if (result !=0)
			return result;
		}

	if (aCompareComps & EUrlPassword)
		{
		result += Component(EUrlPassword).Compare(aUrl.Component(EUrlPassword));
		if (result !=0)
			return result;
		}

	if (aCompareComps & EUrlPath)
		{
		result += Component(EUrlPath).Compare(aUrl.Component(EUrlPath));
		if (result !=0)
			return result;
		}

	if (aCompareComps & EUrlQuery)
		{
		result += Component(EUrlQuery).Compare(aUrl.Component(EUrlQuery));
		if (result !=0)
			return result;
		}

	if (aCompareComps & EUrlFragment)
		{
		result += Component(EUrlFragment).Compare(aUrl.Component(EUrlFragment));
		if (result !=0)
			return result;
		}
	return result;
	}

EXPORT_C void CUrl::SetL(CUrl& aUrl)
	{
	HBufC* url = aUrl.UrlDes().AllocL();
	delete iUrlDes;
	iUrlDes = url;	
	}

EXPORT_C CUrl* CUrl::AllocL() const
	{
	return CUrl::NewL(*iUrlDes);
	}

EXPORT_C CUrl* CUrl::AllocL(TComponent aComponent) const
	{
	if (aComponent == EUrlGenericCompare)
		return AllocL();
	else if (aComponent == EUrlNoCredentials)
		{
		TInt usernameStart;
		TInt locationStart;
		TInt scratch;
		Part(EUrlUsername, *iUrlDes, usernameStart, scratch);
		Part(EUrlLocation, *iUrlDes, locationStart, scratch);
		// If there isn't a location and a username, we can just
		// return the current URL.
		if (locationStart == KCUrlInvalidCharPos || 
			usernameStart == KCUrlInvalidCharPos)
			return AllocL();
		// Make a CUrl of the right length and then copy the correct
		// data into it.
		TInt lengthOfReturnedUrl = iUrlDes->Length() - locationStart + 
			usernameStart;
		CUrl* result = CUrl::NewL(iUrlDes->Left(lengthOfReturnedUrl));
		result->iUrlDes->Des().Replace(usernameStart,
									   lengthOfReturnedUrl - usernameStart,
									   iUrlDes->Right(lengthOfReturnedUrl - 
													  usernameStart));
		return result;
		}
	else return CUrl::NewL(Component(aComponent));
	}

EXPORT_C CUrl* CUrl::UrlEscapedL() const
//
//	Create and return an escaped version of the current unescaped url
	{
	User::Panic(_L("Function Not Used"), KErrNotSupported);
	return NULL;
	}

EXPORT_C CUrl* CUrl::UrlUnescapedL() const
//
//	Create and return an unescaper version of the current escaped url
	{
	User::Panic(_L("Function Not Used"), KErrNotSupported);
	return NULL;
	}

EXPORT_C HBufC* CUrl::EscapeEncodeL(const TDesC& aString)
//
// Deprecated function - should use overload function.
	{
	return EscapeEncodeL(aString, EUrlGenericCompare);
	}

EXPORT_C HBufC* CUrl::EscapeEncodeL(const TDesC& aString, TInt aEscapeMode)
//
// Encodes any excluded characters in aString as escape triples - excluded characters set by aEscapeMode
	{
	// Need to create an HBufC with our excluded characters
	HBufC* excludedBuf = NULL;

	switch (aEscapeMode)
		{
	case EUrlGenericCompare:
		{
		// This is normal operation - escape all sets of data other than KReservedData
		excludedBuf = HBufC::NewLC(KUnwiseData().Length() + KDelimsData().Length());
		TPtr excluded = excludedBuf->Des();
		excluded.Append(KUnwiseData);
		excluded.Append(KDelimsData);
		} break;
	case EUrlScheme:
		{
		// Escaping data in scheme - reserved chars have no special meaning so escape as well.
		excludedBuf = HBufC::NewLC(KUnwiseData().Length() + KDelimsData().Length() + KReservedData().Length());
		TPtr excluded = excludedBuf->Des();
		excluded.Append(KUnwiseData);
		excluded.Append(KDelimsData);
		excluded.Append(KReservedData);
		} break;
	case EUrlPath:
		{
		// Escaping data in a local file path - same as EUrlScheme but don't escape '/' and ':'
		excludedBuf = HBufC::NewLC(KUnwiseData().Length() + KDelimsData().Length() + KReservedDataForPath().Length());
		TPtr excluded = excludedBuf->Des();
		excluded.Append(KUnwiseData);
		excluded.Append(KDelimsData);
		excluded.Append(KReservedDataForPath);
		} break;
	default:
		// Not supported return NULL
		return NULL;
		break;
		}

	//	Descriptor to hex digits
	const TDesC& HexDigit = KHexDigit;

	//	Allocate space to build escaped url - consider worse case, where all characters are excluded => length x 3
	HBufC* buf = HBufC::NewLC(aString.Length()*3);	//	CS
	TPtr escapedBuf = buf->Des();

	for (TInt i=0; i<aString.Length(); ++i)
		{
		//	Check if current character must be escaped, will leave if non 8-bit character
		TChar currentChar = aString[i];
		if (currentChar > 0xff)
			User::Leave(EWapErrCorruptUrl);
		//	Check if aChar is a member of DelimsData or UnwiseData or a control character
		if (excludedBuf->Locate(currentChar)!=KErrNotFound || (currentChar>=0x00 && currentChar<=0x1F) || currentChar==' ' || currentChar > 0x7E )
			{
			//	Escaped character will be 8-bit
			escapedBuf.Append('%');
			TInt msNibble = (currentChar & 0xf0) >> 4;	//	Get msNibble by masking against 11110000 and dividing by 16 (>>4)
			escapedBuf.Append(HexDigit[msNibble]);
			TInt lsNibble = (currentChar & 0x0f);	//	Get lsNibble by masking against 00001111
			escapedBuf.Append(HexDigit[lsNibble]);
			}
		else
			{
			escapedBuf.Append(currentChar);
			}
		}
	// Create new HBufC object
	HBufC* encodedString = escapedBuf.AllocL();

	CleanupStack::PopAndDestroy(2);	//	buf, excludedBuf
	return encodedString;
	}

EXPORT_C HBufC* CUrl::EscapeDecodeL(const TDesC& aString)
//
// Decodes any escape triples in aString back into excluded characters
	{
	//	Descriptor to hex digits
	const TDesC& HexDigit = KHexDigit;

	//	Allocate space for to build unescaped url
	HBufC* buf = HBufC::NewLC(aString.Length());	//	CS
	TPtr unescapedBuf = buf->Des();

	//	Go through url
	for (TInt i=0; i<aString.Length(); ++i)
		{
		//	See if at start of an escape triple
		TChar currentChar = aString[i];
		if (currentChar == '%')
			{
			//	Check that next two characters are valid
			TChar msNibble = aString[i+1];
			if (msNibble.IsAlpha())
				msNibble.UpperCase();
			TChar lsNibble = aString[i+2];
			if (lsNibble.IsAlpha())
				lsNibble.UpperCase();
			TInt msNibbleValue = HexDigit.Locate(msNibble);
			TInt lsNibbleValue = HexDigit.Locate(lsNibble);
			if (msNibbleValue == KErrNotFound || lsNibbleValue == KErrNotFound)
				{
				User::Leave(EWapErrCorruptUrl);
				}
			else
				{
				//	Convert characters into hex value
				TInt hex = 0x10*msNibbleValue + 0x01*lsNibbleValue;
				unescapedBuf.Append(hex);
				}
			//	Move index to get next character
			i+=2;
			}
		else
			{
			unescapedBuf.Append(currentChar);
			}
		}
	// Create new HBufC object
	HBufC* encodedString = unescapedBuf.AllocL();

	CleanupStack::PopAndDestroy();	//	buf
	return encodedString;
	}

EXPORT_C HBufC8* CUrl::ConvertFromUnicodeToUtf8L(const TDesC& aString)
//
// Converts input string from unicode to UTF8 format - converted string is returned in 8-bit format
	{
	// Return an empty buffer straight-away
	if( aString.Compare(KNullDesC) == 0 )
		return KNullDesC8().AllocL();

	// Convert from Unicode to UTF8
	TPtrC unicode = aString;
	TBuf8<KUrlConversionBufferSize> buf;
	HBufC8* utf8Buffer = HBufC8::NewLC(unicode.Length());
	TPtr8 utf8 = utf8Buffer->Des();

	// Loop until all of the filename is converted
	FOREVER
		{
		const TInt returnValue = CnvUtfConverter::ConvertFromUnicodeToUtf8(buf, unicode);
		if( returnValue == CnvUtfConverter::EErrorIllFormedInput || returnValue < 0)
			User::Leave(EWapErrCorruptUrl);

		// Is escapedFullPath too small?
		if( utf8.Length() + buf.Length() > utf8.MaxLength() )
			{
			utf8Buffer = utf8Buffer->ReAllocL(utf8.Length() + buf.Length());
			CleanupStack::Pop();	// utf8Buffer (old version)
			CleanupStack::PushL(utf8Buffer);	// new version
			utf8.Set(utf8Buffer->Des());
			}
		// Copy converted characters
		utf8.Append(buf);

		if( returnValue==0 )
			break; // All of aUnicodeText has been converted and handled

		// Set input descriptor to remaining characters
		unicode.Set(unicode.Right(returnValue));
		}
	CleanupStack::Pop();	// utf8Buffer
	return utf8Buffer;		// Ownership transfered to caller
	}

EXPORT_C HBufC* CUrl::ConvertToUnicodeFromUtf8L(const TDesC8& aString)
//
// Converts input string from UTF8 to Unicode format - converted string is returned in 16-bit format
	{
	// Return an empty buffer straight-away
	if( aString.Compare(KNullDesC8) == 0 )
		return KNullDesC().AllocL();

	// Convert from Unicode to UTF8
	TPtrC8 utf8 = aString;
	TBuf<KUrlConversionBufferSize> buf;
	HBufC* unicodeBuffer = HBufC::NewLC(utf8.Length());
	TPtr unicode = unicodeBuffer->Des();

	// Loop until all of the filename is converted
	FOREVER
		{
		const TInt returnValue = CnvUtfConverter::ConvertToUnicodeFromUtf8(buf, utf8);
		if( returnValue == CnvUtfConverter::EErrorIllFormedInput || returnValue < 0)
			User::Leave(EWapErrCorruptUrl);

		// Is escapedFullPath too small?
		if( unicode.Length() + buf.Length() > unicode.MaxLength() )
			{
			unicodeBuffer = unicodeBuffer->ReAllocL(unicode.Length() + buf.Length());
			CleanupStack::Pop();	// unicodeBuffer (old version)
			CleanupStack::PushL(unicodeBuffer);	// new version
			unicode.Set(unicodeBuffer->Des());
			}
		// Copy converted characters
		unicode.Append(buf);

		if( returnValue==0 )
			break; // All of utf8 has been converted and handled

		// Set input descriptor to remaining characters
		utf8.Set(utf8.Right(returnValue));
		}
	CleanupStack::Pop();	// unicodeBuffer
	return unicodeBuffer;	// Ownership transfered to caller
	}

//
//	TRelativePaths
//
CUrl::TRelativePaths::TRelativePaths(TPtrC aBasePath, TPtrC aRelativePath, TPtr aResolvedPath)
	: iBasePath(aBasePath), iRelativePath(aRelativePath), iResolvedPath(aResolvedPath)
//
//	Trivial c'tor
	{
	}

void CUrl::TRelativePaths::ResolveRelativePaths()
//
//	Resolves iRelativePath against iBasePath, algorithm based on RFC2396 - result in iResolvedPath
	{
	TBool finished = EFalse;
	while (!finished && iRelativePath.Length()>0)
		{
		//	This applies only in cases such as '..\etc'
		if (iRelativePath.Locate('/')==0)
			iRelativePath.Set(iRelativePath.Mid(1));
		TInt relPathLength = iRelativePath.Length();
		if (relPathLength>0 && iRelativePath[0] == '.')
			{
			//	Case 1 - .<end> -> remove '.'
			if (relPathLength == 1)
				{
				iRelativePath.Set(KNullDesC);
				finished = ETrue;
				}
			//	Length of iRelative MUST be > 1, otherwise would not have got here
			//	Case 2 - ./ -> remove './' and continue
			else if (iRelativePath[1] == '/')
				{
				iRelativePath.Set(iRelativePath.Mid(2));
				}
			//	Case 3 & 4 - ..<end> or ../ -> remove a path from iBasePath
			else if (iRelativePath[1] == '.')
				{
				if (relPathLength == 2 || (relPathLength>2 && iRelativePath[2] == '/'))
					{
					//	Try to remove a directory from iBasePath
					TInt basePathLength = iBasePath.Length();
					//	If base path empty or just '/' -> finish
					if (basePathLength==0 || (basePathLength==1 && iBasePath[0] == '/'))
						{
						finished = ETrue;
						}
					else
						{
						TInt endBasePath = iBasePath.LocateReverse('/');
						if (endBasePath == basePathLength - 1)
							{
							//	Have found / on end of path - remove and rescan for '/'
							iBasePath.Set(iBasePath.Left(endBasePath));
							endBasePath = iBasePath.LocateReverse('/');
							}
						if (endBasePath > KErrNotFound)
							{
							//	Remove a directory, keeping the end '/'
							iBasePath.Set(iBasePath.Left(endBasePath+1));
							iRelativePath.Set(iRelativePath.Mid(2)); //	Remove .. at start
							}
						}
					}
				else
					{
					//	iRelative now of form '..etc'
					finished = ETrue;
					}
				}
			else
				{
				//	iRelative now of form '.etc'
				finished = ETrue;
				}
			}
		else
			{
			//	iRelativePath now of form 'etc'
			finished = ETrue;
			}
		}	//	while (!finished && iRelativePath.Length>0)
	//	Form the resolved path 
	iResolvedPath.Append(iBasePath);
	iResolvedPath.Append(iRelativePath);
	CleanResolvedPath();
	}

void CUrl::TRelativePaths::CleanResolvedPath()
//
//	Deal with any /. and /../ in iResolvedPath - produced if the relative url is of the form g/./h or g/../h
	{
	TInt pos = 0;
	//	Deal with /../ by locating the previous / and removing everything between
	//	Eg /data/.../stuff -> /stuff
	while (pos > KErrNotFound)
		{
		_LIT(KSlashDotDotSlash,"/../");
		pos = iResolvedPath.Find(KSlashDotDotSlash);
		if (pos > KErrNotFound)
			{
			TPtrC leftHandSide(iResolvedPath.Left(pos));
			TInt slashPos = leftHandSide.LocateReverse('/');
			if (slashPos > KErrNotFound)
				iResolvedPath.Delete(slashPos, pos - slashPos +3);
			else 
				pos = KErrNotFound;
			}
		}
	//	Remove any /. in iResolvedPath
	pos = 0;
	while (pos > KErrNotFound)
		{
		_LIT(KSlashDot,"/.");
		pos = iResolvedPath.Find(KSlashDot);
		TInt resolvedLength = iResolvedPath.Length();
		if (pos > KErrNotFound)
			{ 
			if (resolvedLength == pos+2) // just remove the .
				iResolvedPath.Delete(pos+1,1);
			else if  (resolvedLength > pos +1 && iResolvedPath[pos+2] == '/') // is of the form /./
				iResolvedPath.Delete(pos,2);
			else
				pos = KErrNotFound;
			}
		}
	}

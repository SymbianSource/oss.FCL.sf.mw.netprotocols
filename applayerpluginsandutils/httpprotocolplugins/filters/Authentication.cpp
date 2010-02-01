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

// System includes
#include <e32std.h>
#include <e32math.h>
#include <hash.h>
#include <uri8.h>
#include <http/rhttpsession.h>
#include <http/rhttptransaction.h>
#include <http/rhttpheaders.h>
#include <httpstringconstants.h>
#include <tconvbase64.h>
#include <inetprottextutils.h>

// User Includes
#include "Authentication.h"
#include <http/mhttpauthenticationcallback.h>
#include "corefilterspanic.h"
#include <httperr.h>
#include "CTransaction.h"


// Length of a digest hash when represented in hex
const TInt KHashLength = 32;
// Length of a digest hash before converting to hex.
const TInt KRawHashLength = 16;

CAuthenticationFilterInterface* CAuthenticationFilter::InstallFilterL(TAny* aAuthenticationParams)
	{
	TAuthenticationParams* authParams = REINTERPRET_CAST(TAuthenticationParams*, aAuthenticationParams);
	CAuthenticationFilter* filter = new (ELeave) CAuthenticationFilter(authParams->iCallback);
	CleanupStack::PushL(filter);
	filter->ConstructL(*(authParams->iSession));
	CleanupStack::Pop(filter); 
	return filter;
	}

CAuthenticationFilter::CAuthenticationFilter(MHTTPAuthenticationCallback* aCallback) 
	: iCallback(aCallback) 
	{
	}

void CAuthenticationFilter::ConstructL(RHTTPSession aSession)
	{
	iStringPool = aSession.StringPool();

	iMD5Calculator = CMD5::NewL();
	const TStringTable& st = RHTTPSession::GetTable();
	// Register the filter. We only care about 401 status codes where
	// there's a WWW-Authenticate header (which the RFC says MUST be
	// present). If we don't fire (e.g. due to a missing header) then
	// the transaction will fail if we ignore it, which is what we
	// want.

	// Register for submit events, to extract credentials from the URI
	// and to pre-supply credentials for basic
	// authentication. Registered as EStatusCodeHandler+1 so that it
	// isn't offered submit events originating from the 401 handler
	// (which is harmless but slow and confusing)
	aSession.FilterCollection().AddFilterL(*this, 
										   THTTPEvent::ESubmit, 
										   RStringF(), 
										   KAnyStatusCode, 
										   EStatusCodeHandler + 1, 
										   iStringPool.StringF(HTTP::EAuthentication,st));
	
	// And for 401 status codes, for obvious reasons.
	aSession.FilterCollection().AddFilterL(*this, 
										   THTTPEvent::EGotResponseHeaders, 
										   iStringPool.StringF(HTTP::EWWWAuthenticate,st), 
										   401, 
										   EStatusCodeHandler, 
										   iStringPool.StringF(HTTP::EAuthentication,st));

	}

CAuthenticationFilter::~CAuthenticationFilter()
	{
	delete iMD5Calculator;
	TInt credCount = iCredentials.Count();
	for (TInt ii = 0; ii < credCount; ++ii)
		{
		TCredentials& cred = iCredentials[ii];

		// Close all owned strings now while we can get a string pool handle.
		iStringPool.String(cred.iUser).Close();
		iStringPool.String(cred.iPassword).Close();
		iStringPool.StringF(cred.iURI).Close();
		iStringPool.String(cred.iRealm).Close();
		iStringPool.StringF(cred.iAuthType,RHTTPSession::GetTable()).Close();
		}

	iCredentials.Close();
	}

void CAuthenticationFilter::MHFLoad(RHTTPSession, THTTPFilterHandle)
	{
	++iLoadCount;
	}

void CAuthenticationFilter::MHFUnload(RHTTPSession /*aSession*/, THTTPFilterHandle /*aFilterHandler*/)
	{
	__ASSERT_DEBUG(iLoadCount >= 0, TFCoreFiltersPanic::Panic(TFCoreFiltersPanic::EAuthenticationFilterBadState));

	if (--iLoadCount)
		return;


	delete this;
	}


void CAuthenticationFilter::MHFRunL(RHTTPTransaction aTransaction, 
									const THTTPEvent& aEvent)
	{
	switch (aEvent.iStatus)
		{
		case THTTPEvent::ESubmit:
			SubmitL(aTransaction);
			break;
		case THTTPEvent::EGotResponseHeaders:
			HandleTransactionL(aTransaction);
			break;
		default:
			__ASSERT_DEBUG(EFalse, TFCoreFiltersPanic::Panic(TFCoreFiltersPanic::EAuthenticationFilterBadState));
			break;
		}
	}

TInt CAuthenticationFilter::MHFRunError(TInt /*aError*/, 
										RHTTPTransaction aTransaction,
										const THTTPEvent& /*aEvent*/)
	{
	aTransaction.Fail();
	return KErrNone;
	}

void CAuthenticationFilter::MHFSessionRunL(const THTTPSessionEvent& /*aEvent*/)
	{
	}

TInt CAuthenticationFilter::MHFSessionRunError(TInt aError, const THTTPSessionEvent& /*aEvent*/)
	{
	return aError;
	}


void 
CAuthenticationFilter::HandleTransactionL(RHTTPTransaction aTransaction)
	{
	// "WWW- Authenticate: SCHEME REALM" header is the header the server returns
	// to request authorization
	RStringF wwwAuthHeader = iStringPool.StringF(HTTP::EWWWAuthenticate,RHTTPSession::GetTable());
	RHTTPHeaders headers(aTransaction.Response().GetHeaderCollection());

	TInt headerPart = FindHeaderPartToUseL(aTransaction);

	if (headerPart == KErrNotFound)
		return;

	THTTPHdrVal authTypeParam;
	TInt error = headers.GetField(wwwAuthHeader, headerPart, authTypeParam);
	
	// If anything goew wrong, just stop. This will have the effect of
	// returning the error response from the server to the client.
	if (error != KErrNone)
		return;

	TBool	 haveUserPW = EFalse;
	RString  username;
	RString  password;
	TInt cred = KErrNotFound;
	TInt credID = -1;
	RHTTPTransactionPropertySet propSet = aTransaction.PropertySet();

	// Check if we have a valid user/pw in our local creditial list
	RString realm;
	THTTPHdrVal hdrVal;
	if (!headers.GetParam(wwwAuthHeader, iStringPool.StringF(HTTP::ERealm,RHTTPSession::GetTable()), 
						  hdrVal, headerPart))
		{
		realm = hdrVal;

		cred = FindCredentials(realm, authTypeParam.StrF().Index(RHTTPSession::GetTable()), 
							   aTransaction.Request().URI());

		THTTPHdrVal authStatus;
		if (cred != KErrNotFound && 
			propSet.Property(iStringPool.StringF(HTTP::EAuthenticationStatus,RHTTPSession::GetTable()), 
					 authStatus) && authStatus.Int() == iCredentials[cred].iID)
			{
			// If the credentials are in the list, and we've already
			// tried to authenticate once, then the credentials must
			// be wrong. Delete them and ignore them.
			TCredentials& creds = iCredentials[cred];
			iStringPool.String(creds.iUser).Close();
			iStringPool.String(creds.iPassword).Close();
			iStringPool.StringF(creds.iURI).Close();
			iStringPool.String(creds.iRealm).Close();
			iStringPool.StringF(creds.iAuthType,RHTTPSession::GetTable()).Close();
			iCredentials.Remove(cred);
			cred = KErrNotFound;
			}

		// Yes - we have it
		if (cred != KErrNotFound)
			{
			// We copy the strings so that wherever we got some
			// credentials, we also have 2 strings to pop.
			username = iStringPool.String(iCredentials[cred].iUser).Copy();
			CleanupClosePushL(username);
			password = iStringPool.String(iCredentials[cred].iPassword).Copy();
			CleanupClosePushL(password);
			credID = iCredentials[cred].iID;
			haveUserPW = ETrue;
			}
		}

	// Check to see if we're passed the user/pw in the header
	// (supplied by user or URI)
	if (!haveUserPW)
		{
		// If this is a first time run (AuthStatus doesn't exist) and
		// we have name & pw
		THTTPHdrVal authStatus, usernameVal, passwordVal;
		if (!propSet.Property(iStringPool.StringF(HTTP::EAuthenticationStatus,RHTTPSession::GetTable()),
							  authStatus) &&
			propSet.Property(iStringPool.StringF(HTTP::EUsername,RHTTPSession::GetTable()), 
							 usernameVal) &&
			propSet.Property(iStringPool.StringF(HTTP::EPassword,RHTTPSession::GetTable()), 
							 passwordVal) )
			{
			username = usernameVal.Str().Copy();
			CleanupClosePushL(username);
			password = passwordVal.Str().Copy();
			CleanupClosePushL(password);
			haveUserPW = ETrue;
			}
		}										
	
	if (haveUserPW)
		{
		// Forgot old username/password and release their strings
		propSet.RemoveProperty(iStringPool.StringF(HTTP::EUsername,RHTTPSession::GetTable()));
		propSet.RemoveProperty(iStringPool.StringF(HTTP::EPassword,RHTTPSession::GetTable()));
		}
	
	// Can't find a password anywhere. Get user/pw externally
	RStringF authType = authTypeParam.StrF();
	if (!haveUserPW && iCallback!=NULL)
		{
		haveUserPW = iCallback->GetCredentialsL(aTransaction.Request().URI(),
											   realm, authType, username, 
											   password);
		CTransaction* trans = CTransaction::Implementation(aTransaction);
		if(!trans->IsValid())
			{
			return;
			}
		if (haveUserPW)
			{
			CleanupClosePushL(username);
			CleanupClosePushL(password);
			}
		}
	
	// If still no user/pw then give up. Leave the authentication to
	// someone else.
	if (!haveUserPW)
		return;	


	// Encode the Username/Password
	switch (authType.Index(RHTTPSession::GetTable()))
		{
		case HTTP::EBasic:
			EncodeBasicAuthL(username, password, aTransaction);
			break;
		case HTTP::EDigest:
			{
			THTTPHdrVal requestUri;
			propSet.Property(iStringPool.StringF(HTTP::EUri,RHTTPSession::GetTable()), requestUri);
			EncodeDigestAuthL(username, password, aTransaction, headerPart, requestUri);
			}break;
		default:
			__ASSERT_DEBUG(EFalse, TFCoreFiltersPanic::Panic(TFCoreFiltersPanic::EAuthenticationFilterBadState));
			break;
		}

	if (cred == KErrNotFound)
		{
		// We didn't get the username and password from the
		// credentials store, so we need to save them there
		TCredentials newCred;

		newCred.iUser = username;
		newCred.iPassword = password;
			
		const TDesC8& uriDes = aTransaction.Request().URI().Extract(EUriHost);
		newCred.iURI =iStringPool.OpenFStringL(uriDes);
		RStringF uriS = iStringPool.StringF(newCred.iURI);
		CleanupClosePushL(uriS);
		
		newCred.iRealm = realm.Copy();
		newCred.iAuthType = authType.Index(RHTTPSession::GetTable());
		credID = newCred.iID = iNextID++;
		CleanupClosePushL(realm);
		User::LeaveIfError(iCredentials.Append(newCred));
		CleanupStack::Pop(4); // username, password, realm, uri
		}
	else
		{
		CleanupStack::PopAndDestroy(2); // username, password
		}

	// Set AuthStatus.
	propSet.SetPropertyL(iStringPool.StringF(HTTP::EAuthenticationStatus,RHTTPSession::GetTable()), 
						 credID);

	// kill the transaction back to client
	aTransaction.Cancel(THTTPFilterHandle::ECurrentFilter);
	
	// Resubmit the request to the server
	aTransaction.SubmitL(THTTPFilterHandle::ECurrentFilter);
	}


void CAuthenticationFilter::SubmitL(RHTTPTransaction aTransaction)
	{
	const TUriC8& uri = aTransaction.Request().URI();

	if (uri.IsPresent(EUriUserinfo))
		{
		const TPtrC8 userInfo = uri.Extract(EUriUserinfo);
		TInt colonPos = userInfo.Locate(':');
		if (colonPos == KErrNotFound)
			colonPos = userInfo.Length();

		if (colonPos)
			{
			// We have a username
			TPtrC8 username = userInfo.Left(colonPos);
			RString strUsername = iStringPool.OpenStringL(username);
			CleanupClosePushL ( strUsername );
			THTTPHdrVal usernameVal ( strUsername );			
			aTransaction.PropertySet().SetPropertyL(iStringPool.StringF(HTTP::EUsername,RHTTPSession::GetTable()),
												   usernameVal);
			CleanupStack::PopAndDestroy ( &strUsername );
			}
		if (colonPos != userInfo.Length())
			{
			// We have a password
			TPtrC8 password = userInfo.Mid(colonPos + 1);
			RString strPassword = iStringPool.OpenStringL(password);
			CleanupClosePushL ( strPassword );
			THTTPHdrVal passwordVal( strPassword );
			aTransaction.PropertySet().SetPropertyL(iStringPool.StringF(HTTP::EPassword,RHTTPSession::GetTable()),
												   passwordVal);
			CleanupStack::PopAndDestroy ( &strPassword );
			}

		// Now remove the username and password from the URI
		CUri8* newUri = CUri8::NewLC(uri);
		newUri->RemoveComponentL(EUriUserinfo);
		aTransaction.Request().SetURIL(newUri->Uri());
		CleanupStack::PopAndDestroy(newUri); // newUri
		}

	// If we've done a previous request to a URI that is exactly the
	// same as this or is a subset of this, we can supply the previous
	// credentials immediately. Note that we only try this with basic
	// authentication. Adding digest support would involve a lot of
	// issues with reading Authentication-Info headers, keeping track
	// of nonce counts, stale replies and so on. Currently, I don't
	// think we have requirements to do that.
	TInt cred = FindCredentialsForURI(aTransaction.Request().URI());
	if (cred != KErrNotFound && iCredentials[cred].iAuthType == HTTP::EBasic)
		{
		// Note that this does not set the authentication
		// property. This means that if the supplied username and
		// password doesn't work for this URI, we won't immediately
		// remove it from the credential store when we get the 401
		// back.
		EncodeBasicAuthL(iStringPool.String(iCredentials[cred].iUser),
						iStringPool.String(iCredentials[cred].iPassword),
						aTransaction);
		}
	}

TInt CAuthenticationFilter::FindCredentials(const RString& aRealm, 
											TInt aAuthType, 
											const TUriC8& aURI) const
	{
	TInt count = iCredentials.Count();
	for (TInt cred = 0; cred < count; ++cred)
		{
		if (iCredentials[cred].iRealm == aRealm && 
			iCredentials[cred].iAuthType == aAuthType)
			{
			const TDesC8& transDes = aURI.Extract(EUriHost);
			const TDesC8& credDes = iStringPool.StringF(iCredentials[cred].iURI).DesC();

			if (transDes.Compare(credDes) == 0)
				{
				return cred; // The descriptors match. 
				}
			}
		}

	return KErrNotFound;
	}

TInt CAuthenticationFilter::FindCredentialsForURI(const TUriC8& aURI) const
	{
	// Check if any of the stored URIs are the beginning of the URI
	// we're trying now. If they are, we can immediately attempt to
	// re-use the existing credentials.
	TInt count = iCredentials.Count();
	for (TInt cred = 0; cred < count; ++cred)
		{
		const TDesC8& transDes = aURI.Extract(EUriHost);
		const TDesC8& credDes = iStringPool.StringF(iCredentials[cred].iURI).DesC();

		if (transDes.Compare(credDes) == 0)
			{
			return cred; // The descriptors match. 
			}
		}

	return KErrNotFound;
	}

void CAuthenticationFilter::EncodeBasicAuthL(const RString& aUsername, 
										  const RString& aPW, 
										  RHTTPTransaction& aTransaction)
	{
	__START_PERFORMANCE_LOGGER();
	// Standard, plain-text HTTP - Base 64 the name and password
	TBase64 codec;
	HBufC8* nameAndPW = HBufC8::NewMaxLC(aUsername.DesC().Length() + aPW.DesC().Length() + 1);
	_LIT8(KBasicAuthFormatString, "%S:%S");
	nameAndPW->Des().Format(KBasicAuthFormatString, &aUsername.DesC(), &aPW.DesC());
 
	// Conservatively allocate a buffer twice as big as the unencoded
	// buffer for the encoded string.
	HBufC8* encoded = HBufC8::NewMaxLC(nameAndPW->Des().Length() * 2);
	TPtr8 encodedPtr(encoded->Des());
	codec.Encode(*nameAndPW, encodedPtr);
	
	RString encodedString = iStringPool.OpenStringL(*encoded);
	CleanupStack::PopAndDestroy(2, nameAndPW); 

	CleanupClosePushL(encodedString);
	
	RHTTPHeaders requestHeaders(aTransaction.Request().GetHeaderCollection());
	
	requestHeaders.RemoveField(iStringPool.StringF(HTTP::EAuthorization,RHTTPSession::GetTable())); 
	requestHeaders.SetFieldL(iStringPool.StringF(HTTP::EAuthorization,RHTTPSession::GetTable()), 
									THTTPHdrVal(iStringPool.StringF(HTTP::EBasic,RHTTPSession::GetTable())));
	
	requestHeaders.SetFieldL(iStringPool.StringF(HTTP::EAuthorization,RHTTPSession::GetTable()), 
									THTTPHdrVal(encodedString));

	aTransaction.PropertySet().SetPropertyL(iStringPool.StringF(HTTP::EAuthorization,RHTTPSession::GetTable()), 
																THTTPHdrVal(encodedString));
	
	CleanupStack::PopAndDestroy(&encodedString);
	__END_PERFORMANCE_LOGGER(_L(",CAuthenticationFilter::EncodeBasicAuthL()"));
	}

void CAuthenticationFilter::EncodeDigestAuthL(const RString& aUsername, 
											  const RString& aPW, 
											  RHTTPTransaction& aTransaction,
											  TInt headerPart,
											  const THTTPHdrVal& aRequestUri)
	{
	__START_PERFORMANCE_LOGGER();
	// For now, only support RFC2069 format. The digest is
	// MD5(H(A1):nonce:H(A2))
	TBuf8<KHashLength> hash;
	
	THTTPHdrVal nonce;
	RHTTPHeaders headers = aTransaction.Response().GetHeaderCollection();
	TInt error = headers.GetParam(iStringPool.StringF(HTTP::EWWWAuthenticate,RHTTPSession::GetTable()), 
								  iStringPool.StringF(HTTP::ENonce,RHTTPSession::GetTable()), nonce,
								  headerPart);

	THTTPHdrVal realm;
	error = headers.GetParam(iStringPool.StringF(HTTP::EWWWAuthenticate,RHTTPSession::GetTable()),
							 iStringPool.StringF(HTTP::ERealm,RHTTPSession::GetTable()), realm,
							 headerPart);
	THTTPHdrVal qop;
	error = headers.GetParam(iStringPool.StringF(HTTP::EWWWAuthenticate,RHTTPSession::GetTable()),
							 iStringPool.StringF(HTTP::EQop,RHTTPSession::GetTable()), qop,
							 headerPart);
	HBufC8* stringToHash = NULL;
	RStringF auth = iStringPool.StringF(HTTP::EAuth,RHTTPSession::GetTable());
	TBool useNewDigest = (error == KErrNone);

	TBuf8<KHashLength> cnonce;
	if (useNewDigest)
		{
		// As a qop has been specified, we should generate a cnonce
		// and form the digest by calculating the hash of
		// H(A1):nonce:nc-value:cnonce:qop:H(A2) nc-value is the
		// number of times we've responded since we last received a
		// nonce. As we don't remeber nonces, it's always 00000001 So,
		// the length of the buffer needed is 32 for the hashes and
		// the cnonce, 8 for the nc-value, 5 for the colons, plus the
		// length of the nonce and the qop, or 109 plus the lenght of
		// the nonce and the qop
		stringToHash = HBufC8::NewMaxLC(nonce.Str().DesC().Length() + 
										 qop.Str().DesC().Length() +
										 3 * KHashLength + 8 + 5);
		TPtr8 stringMod = stringToHash->Des();
		stringMod.Zero();
		HA1L(aUsername, aPW, realm.Str(), stringMod);
		stringMod.Append(':');
		stringMod.Append(nonce.Str().DesC());
		_LIT8(KNc, ":00000001:");
		stringMod.Append(KNc);
		GenerateCNonce(cnonce);
		stringMod.Append(cnonce);
		stringMod.Append(':');
		stringMod.Append(auth.DesC());
		stringMod.Append(':');
		HA2L(aTransaction.Request().Method(), aRequestUri.Str(), hash);
		stringMod.Append(hash);		
		}
	else
		{
		// The response is the hash of H(A1):nonce:H(A2).
		// Crqeate a buffer for the string to convert into MD5. The
		// length is 32 characters for each of the hashes, 2
		// characters for the 2 colons, plus the length of the nonce
		stringToHash = HBufC8::NewMaxLC(nonce.Str().DesC().Length() + 
												 2 * KHashLength + 2);
		TPtr8 stringMod = stringToHash->Des();
		stringMod.Zero();
		HA1L(aUsername, aPW, realm.Str(), stringMod);
		stringMod.Append(':');
		stringMod.Append(nonce.Str().DesC());
		stringMod.Append(':');
		HA2L(aTransaction.Request().Method(), aRequestUri.Str(), hash);
		stringMod.Append(hash);
		}
	
	Hash(*stringToHash, hash);
	CleanupStack::PopAndDestroy(stringToHash);

	// OK. hash now contains the digest response. Set up the header.
	RHTTPHeaders requestHeaders(aTransaction.Request().GetHeaderCollection());
	RHTTPHeaders responseHeaders(aTransaction.Response().GetHeaderCollection());
	
	requestHeaders.RemoveField(iStringPool.StringF(HTTP::EAuthorization,RHTTPSession::GetTable())); 

	THTTPHdrVal hdrVal(aUsername);
	
	requestHeaders.SetFieldL(iStringPool.StringF(HTTP::EAuthorization,RHTTPSession::GetTable()), 
							 THTTPHdrVal(iStringPool.StringF(HTTP::EDigest,RHTTPSession::GetTable())),
							 iStringPool.StringF(HTTP::EUsername,RHTTPSession::GetTable()),
							 hdrVal);
	responseHeaders.GetParam(iStringPool.StringF(HTTP::EWWWAuthenticate,RHTTPSession::GetTable()), 
							 iStringPool.StringF(HTTP::ERealm,RHTTPSession::GetTable()), hdrVal, 
							 headerPart);
	requestHeaders.SetFieldL(iStringPool.StringF(HTTP::EAuthorization,RHTTPSession::GetTable()), 
							 THTTPHdrVal(iStringPool.StringF(HTTP::EDigest,RHTTPSession::GetTable())),
							 iStringPool.StringF(HTTP::ERealm,RHTTPSession::GetTable()),
							 hdrVal);
	responseHeaders.GetParam(iStringPool.StringF(HTTP::EWWWAuthenticate,RHTTPSession::GetTable()), 
							 iStringPool.StringF(HTTP::ENonce,RHTTPSession::GetTable()), hdrVal,
							 headerPart);
	requestHeaders.SetFieldL(iStringPool.StringF(HTTP::EAuthorization,RHTTPSession::GetTable()), 
							 THTTPHdrVal(iStringPool.StringF(HTTP::EDigest,RHTTPSession::GetTable())),
							 iStringPool.StringF(HTTP::ENonce,RHTTPSession::GetTable()),
							 hdrVal);
	RString hashStr = iStringPool.OpenStringL(hash);
	CleanupClosePushL(hashStr);
	hdrVal.SetStr(hashStr);
	requestHeaders.SetFieldL(iStringPool.StringF(HTTP::EAuthorization,RHTTPSession::GetTable()), 
							 THTTPHdrVal(iStringPool.StringF(HTTP::EDigest,RHTTPSession::GetTable())),
							 iStringPool.StringF(HTTP::EResponse,RHTTPSession::GetTable()),
							 hdrVal);

	requestHeaders.SetFieldL(iStringPool.StringF(HTTP::EAuthorization,RHTTPSession::GetTable()), 
							 THTTPHdrVal(iStringPool.StringF(HTTP::EDigest,RHTTPSession::GetTable())),
							 iStringPool.StringF(HTTP::EUri,RHTTPSession::GetTable()),
							 aRequestUri);
	CleanupStack::PopAndDestroy(&hashStr);
	if (useNewDigest)
		{
		hdrVal.SetStrF(auth);
		requestHeaders.SetFieldL(iStringPool.StringF(HTTP::EAuthorization,RHTTPSession::GetTable()), 
								 THTTPHdrVal(iStringPool.StringF(HTTP::EDigest,RHTTPSession::GetTable())),
								 iStringPool.StringF(HTTP::EQop,RHTTPSession::GetTable()),
								 hdrVal);
		RString cnonceString = iStringPool.OpenStringL(cnonce);
		CleanupClosePushL(cnonceString);
		hdrVal.SetStr(cnonceString);
		requestHeaders.SetFieldL(iStringPool.StringF(HTTP::EAuthorization,RHTTPSession::GetTable()), 
								 THTTPHdrVal(iStringPool.StringF(HTTP::EDigest,RHTTPSession::GetTable())),
								 iStringPool.StringF(HTTP::ECnonce,RHTTPSession::GetTable()),
								 hdrVal);
		CleanupStack::PopAndDestroy(); //cnonceString
		hdrVal.SetStrF(iStringPool.StringF(HTTP::E00000001,RHTTPSession::GetTable()));
		requestHeaders.SetFieldL(iStringPool.StringF(HTTP::EAuthorization,RHTTPSession::GetTable()), 
								 THTTPHdrVal(iStringPool.StringF(HTTP::EDigest,RHTTPSession::GetTable())),
								 iStringPool.StringF(HTTP::ENc,RHTTPSession::GetTable()),
								 hdrVal);
		}
	
	error = responseHeaders.GetParam(iStringPool.StringF(HTTP::EWWWAuthenticate,RHTTPSession::GetTable()), 
									 iStringPool.StringF(HTTP::EOpaque,RHTTPSession::GetTable()), 
									 hdrVal, headerPart);
	if (error != KErrNotFound)
		requestHeaders.SetFieldL(iStringPool.StringF(HTTP::EAuthorization,RHTTPSession::GetTable()), 
								 THTTPHdrVal(iStringPool.StringF(HTTP::EDigest,RHTTPSession::GetTable())),
								 iStringPool.StringF(HTTP::EOpaque,RHTTPSession::GetTable()),
								 hdrVal);	
 __END_PERFORMANCE_LOGGER(_L(",CAuthenticationFilter::EncodeDigestAuthL()"));	
	}

TInt CAuthenticationFilter::HA1L(const RString& aUsername,const RString& aPW,
								 const RString& aRealm, TDes8& aResult)
	{
	// For now, we only support MD5, not MD5-sess. This means A1 is
	// username:realm:password

	// Create a buffer long enough for the username, realm, psassword
	// and 2 colons.
	HBufC8* a1 = HBufC8::NewMaxLC(aUsername.DesC().Length() + 
								  aPW.DesC().Length() + 
								  aRealm.DesC().Length() + 2);
	TPtr8 a1Mod = a1->Des();
	a1Mod.Zero();
	a1Mod.Append(aUsername.DesC());
	a1Mod.Append(':');
	a1Mod.Append(aRealm.DesC());
	a1Mod.Append(':');
	a1Mod.Append(aPW.DesC());

	Hash(*a1, aResult);

	CleanupStack::PopAndDestroy(a1);
	return KErrNone;
	}


TInt CAuthenticationFilter::HA2L(const RStringF& aMethod, 
								 const RString& aRequestUri,
								 TDes8& aResult)
	{
	// In the auth qop, a2 is Method:digest-uri-value
	// Digest-uri-value. We don't support auth-int qop
	// Allocate enough space for the method, the URI and the colon.
	TPtrC8 requestUri = aRequestUri.DesC();
	TPtrC8 method = aMethod.DesC();
	HBufC8* a2 = HBufC8::NewMaxLC(requestUri.Length() + method.Length() + 1);
	TPtr8 a2Mod = a2->Des();
	a2Mod.Zero();
	a2Mod.Append(method);
	a2Mod.Append(':');
	a2Mod.Append(requestUri);

	Hash(*a2, aResult);

	CleanupStack::PopAndDestroy(a2);

	return KErrNone;
	}

void CAuthenticationFilter::Hash(const TDesC8& aMessage, TDes8& aHash)
	{
	__START_PERFORMANCE_LOGGER();
	// Calculate the 128 bit (16 byte) hash
	iMD5Calculator->Reset();
	TPtrC8 hash = iMD5Calculator->Hash(aMessage);
	// Now print it as a 32 byte hex number
	aHash.Zero();
	_LIT8(formatStr, "%02x");
	for (TInt ii = 0; ii < KRawHashLength; ii++)
		{
		TBuf8<2> scratch;
		scratch.Zero();
		scratch.Format(formatStr, hash[ii]);
		aHash.Append(scratch);
		}
		__END_PERFORMANCE_LOGGER(_L(",CAuthenticationFilter::Hash()"));
	}


 void CAuthenticationFilter::GenerateCNonce(TDes8& aNonce)
	{
	__START_PERFORMANCE_LOGGER();
	// 'Inspired by' CObexAuthenticator

	// The purpose of the client nonce is to protect against 'chosen
	// plaintext' attacks where a hostile server tricks the client
	// into supplying a password using a specific server nonce that
	// allows an (as yet undiscovered) flaw in MD5 to recover the
	// password. As such the only important thing about client nonces
	// is that they change and aren't predictable. See section 4.9 of
	// RFC2616

	TTime time;
	time.UniversalTime();
	TInt64 randomNumber = Math::Rand(iSeed);
	randomNumber <<= 32;
	randomNumber += Math::Rand(iSeed);
	TBuf8<33> key;
	key.Zero();
	key.AppendNum(time.Int64(), EHex);
	key.Append(_L8(":"));
	key.AppendNum(randomNumber, EHex);
	
	Hash(key, aNonce);
	__END_PERFORMANCE_LOGGER(_L(",CAuthenticationFilter::GenerateCNonce()"));
	}

TBool CAuthenticationFilter::FindAuth(const TDesC8& aQop) const
	{
	TPtrC8 tokens = aQop;
	TPtrC8 token;

	const TDesC8& auth = iStringPool.StringF(HTTP::EAuth,RHTTPSession::GetTable()).DesC();
	while(InetProtTextUtils::ExtractNextTokenFromList(tokens, token, ',') != 0 && 
		  token.Length() != 0)
		{
		if (token.Compare(auth) == 0)
			return ETrue;
		}

	return EFalse;
	}

TInt CAuthenticationFilter::FindHeaderPartToUseL(RHTTPTransaction aTransaction) const
	{
	// There may be several different authentication fields. We need
	// to chose the strongest one we understnad. Currently, we only
	// support 2, Basic and Digest, and Digest is the strongest
	// assuming there is a qop that we can accept. Therefore, we keep
	// track of the last good basic one we found, and return the
	// moment we find an acceptable digest one.

	// While we're here, we check that all desired fields are there.

	RStringF wwwAuthenticate = iStringPool.StringF(HTTP::EWWWAuthenticate,RHTTPSession::GetTable());
	RStringF realm = iStringPool.StringF(HTTP::ERealm,RHTTPSession::GetTable());
	RStringF nonce = iStringPool.StringF(HTTP::ENonce,RHTTPSession::GetTable());
	RStringF qop   = iStringPool.StringF(HTTP::EQop,RHTTPSession::GetTable());
	TInt lastGoodBasic = KErrNotFound;
	RHTTPHeaders headers = aTransaction.Response().GetHeaderCollection();
	const TInt parts = headers.FieldPartsL(wwwAuthenticate);
	for (TInt ii = 0; ii < parts; ii++)
		{
		THTTPHdrVal fieldVal;// The name of the current field.
		THTTPHdrVal hdrVal;//A scratch hdrVal
		headers.GetField(wwwAuthenticate, ii, fieldVal);
		switch (fieldVal.StrF().Index(RHTTPSession::GetTable()))
			{
		case HTTP::EBasic:
			if (headers.GetParam(wwwAuthenticate, realm, hdrVal, ii) == 
				KErrNone)
				lastGoodBasic = ii;
			break;

		case HTTP::EDigest:
			if (headers.GetParam(wwwAuthenticate, realm, hdrVal, ii) == 
				KErrNone &&
				headers.GetParam(wwwAuthenticate, nonce, hdrVal, ii) == 
				KErrNone)
				{
				// We've got a realm and a nonce. If there's a qop, we
				// need to check it's acceptable.
				if (headers.GetParam(wwwAuthenticate, qop, hdrVal, ii) != 
					KErrNone || FindAuth(hdrVal.Str().DesC()))
					return ii;
				}
			break;

		default:
			// We don't understand this, whatever it is. Ignore it.
			break;
			}
		}

	return lastGoodBasic;
	}



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

#ifndef __AUTHENTICATION_H__
#define __AUTHENTICATION_H__


/** Authentication Filter provides a response the http server challenge
    WWW-Authenticate: challenge realm.

	challenge currently supported is: "basic"
*/

// System includes
#include <http/cauthenticationfilterinterface.h>
#include <http/mhttpfilter.h>
#include "timerlogger.h"

// Forward declarations
class MHTTPAuthenticationCallback;
class CMD5;

//##ModelId=3A3A17590238
class CAuthenticationFilter : public CAuthenticationFilterInterface, public MHTTPFilter
	{
public:
	static CAuthenticationFilterInterface* InstallFilterL(TAny* aAuthenticationParams);	
	// Destructor
	//##ModelId=3B1E68290061
	~CAuthenticationFilter();

public:	// Methods from MHTTPFilterBase

	// @see MHTTPFilterBase::MHFRunL
	//##ModelId=3B1E682801E6
	virtual void MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent);

	// @see MHTTPFilterBase::MHFSessionRunL
	virtual void MHFSessionRunL(const THTTPSessionEvent& aEvent);

	// @see MHTTPFilterBase::MHFRunError
	//##ModelId=3B1E682702AD
	virtual TInt MHFRunError(TInt aError, RHTTPTransaction aTransaction,
							 const THTTPEvent& aEvent);

	// @see MHTTPFilterBase::MHFSessionRunError
	virtual TInt MHFSessionRunError(TInt aError, const THTTPSessionEvent& aEvent);

public:	// Methods from MHTTPFilter

	// @see MHTTPFilter::MHFUnload
	//##ModelId=3B1E68270072
	virtual void MHFUnload(RHTTPSession aSession, THTTPFilterHandle aHandle);

	// @see MHTTPFilter::MHFLoad
	//##ModelId=3B1E68260229
	virtual void MHFLoad(RHTTPSession aSession, THTTPFilterHandle aHandle);

 private:
	// Constructor
	//##ModelId=3B1E68260107
	CAuthenticationFilter(MHTTPAuthenticationCallback* aCallback);
	// For 2 phase construction. Also installs the filter.
	//##ModelId=3B1E682503D6
	void ConstructL(RHTTPSession aSession);
	
	// Function to handle Submit events. We need to remove any
	// username and password from the URI.
	//##ModelId=3B1E6825006F
	void SubmitL(RHTTPTransaction aTransaction);

	// Provides a central point for all Tranaction handling
	//##ModelId=3B1E6824033F
	void HandleTransactionL(RHTTPTransaction aTransaction);

	// Search list for matching credentials
	//##ModelId=3B1E68240186
	TInt FindCredentials(const RString& aRealm, TInt aAuthType, 
						 const TUriC8& aURI) const;

	// Searches list for credentials that can be used with this URI
	// without knowing the realm.
	//##ModelId=3BA6004600A7
	TInt FindCredentialsForURI(const TUriC8& aURI) const;

	// Finds the header part with the stringest authentication that
	// we support.
	//##ModelId=3BA600460007
	TInt FindHeaderPartToUseL(RHTTPTransaction aTransaction) const;

	// Set Basic Auth
	//##ModelId=3B1E6823030B
	void EncodeBasicAuthL(const RString& aUsername,const RString& aPW,
						  RHTTPTransaction& aTransaction);
	
	// Set Digest Auth
	//##ModelId=3B1E682300BC
	void EncodeDigestAuthL(const RString& aUsername,const RString& aPW,
						   RHTTPTransaction& aTransaction, TInt headerPart,
						   const THTTPHdrVal& aRequestUri);

	/**Digest helper functions
		See RFC2617 for details of what these mean */
	//@{
	// Calculate H(A1). See section 3.2.2.2
	//##ModelId=3BA600450237
	TInt HA1L(const RString& aUsername,const RString& aPW,
			 const RString& aRealm, TDes8& aResult);

	
	// Calculate H(A2). See section 3.2.2.3
	//##ModelId=3BA600450178
	TInt HA2L(const RStringF& aMethod, const RString& aRequestUri, TDes8& aResult);

	// Calculate a hash and in the form of a 32 character hex result.
	//##ModelId=3BA6004500C4
	void Hash(const TDesC8& aMessage, TDes8& aHash);

	//##ModelId=3BA600450056
	void GenerateCNonce(TDes8& aNonce);

	//##ModelId=3BA6004403A8
	TBool FindAuth(const TDesC8& aQop) const;
	//@}
	
   	__DECLARE_PERFORMANCE_LOG
	// Container for holding an internal storage of credentials
	class TCredentials
		{
	public:
		RStringToken iRealm;	//< Realm used - there may be multiple for each host
		RStringToken iUser;		//< username
		RStringToken iPassword; //< password
		RStringTokenF iURI;		//< uri
		TInt iAuthType;			//< basic or digest or ??
		TInt iID;				//< to associate credentials with transactions
		};

	// callback for getting username/pw from external source
	//##ModelId=3B1E6823008A
	MHTTPAuthenticationCallback*       iCallback;
	// internal list of valid credentials
	//##ModelId=3B1E6823004E
	RArray<TCredentials>              iCredentials;
	// local copy
	//##ModelId=3B1E68230012
	RStringPool                        iStringPool;
	
	// A reference count to make sure we delete at the right time.
	//##ModelId=3B1E682203D4
	TInt iLoadCount;

	// The next free credential ID
	//##ModelId=3BA600440394
	TInt iNextID;

	// The digest calculator
	//##ModelId=3BA600440380
	CMD5* iMD5Calculator;

	// A seed for random numbers, used for client nonces.
	//##ModelId=3BA600440362
	TInt64 iSeed;
	};


#endif //__AUTHENTICATION_H_

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


#ifndef __TESTFILTER_H_
#define __TESTFILTER_H_
#include "Timer.h"
#include <http/mhttpfilter.h>
#include <http/rhttpsession.h>
#include <http/cecomfilter.h>
#include <http/framework/httplogger.h>
#include <ecom/ecom.h>

class RHTTPResponse;
class RHTTPTransaction;
class CMD5;

const TInt KMaxTypeLength = 255;
const TInt KBsfAddressSize = 128;

class CShutdown : public CTimer
    {
    enum { EShutdownDelay = 0x4000000 }; // approx 2s
    public:
        CShutdown();
        void ConstructL();
        void Start();
        void Stop();
    private:
        void RunL();
    };

class CTestFilterInterface : public CBase
/**
The ECOM interface definition for an HTTP authentication filter.  Implementations
of an authentication filter must inherit this class.
@publishedAll
@released
*/
	{
public: // Methods
	/**
	@fn				InstallFilterL(RHTTPSession& aSession, MHTTPAuthenticationCallback* aCallback)
	Intended Usage:	This method is used to install the authentication filter to the filter queue.
					The returned pointer is not owned as the filters must be self-destroying when
					they are unloaded.
	@param			aSession A handle to the transport session
	@param			aCallback A pointer to the object implementing the callback function
	@return			Pointer to the newly installed plugin
	@pre 			The session had already been setup
	@post			The filter(s) have been installed
	*/
	inline static void InstallFilterL(RHTTPSession& aSession);

	/**
	@fn				~CTestFilterInterface()
	Intended Usage:	Virtual destructor
	@pre 			The object identified by the destructor key in iEcomDtorID exists
	@post			The object is destroyed
	*/
	inline ~CTestFilterInterface();

private: 
	// Attributes
	/// The ECom destructor key identifier
	TUid iEcomDtorID;
	};

//----------------------------------------------------------------------------------------

inline void CTestFilterInterface::InstallFilterL(RHTTPSession& aSession)
	{
	const TUid KUidAuthenticationFilter = {0x2002CE96};

	REComSession::CreateImplementationL(KUidAuthenticationFilter, _FOFF(CTestFilterInterface,iEcomDtorID), &aSession);
	}

inline CTestFilterInterface::~CTestFilterInterface()
	{
	REComSession::DestroyedImplementation(iEcomDtorID);
	}


class CTestFilter : public CTestFilterInterface, public MHTTPFilter, public MTimerObserver
{
    enum
        {
        EAlgUnknown,
        EAlgMd5,
        EAlgMd5Sess
        };

public:

	static CTestFilterInterface* InstallFilterL(TAny* aSession);
 
	virtual ~CTestFilter();

public:	// Methods from MHTTPFilterBase

	/// @see MHTTPFilterBase::MHFRunL
	virtual void MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent);

	/// @see MHTTPFilterBase::MHFSessionRunL
	virtual void MHFSessionRunL(const THTTPSessionEvent& aEvent);

	/// @see MHTTPFilterBase::MHFRunError
	virtual TInt MHFRunError(TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& aEvent);

	/// @see MHTTPFilterBase::MHFSessionRunError
	virtual TInt MHFSessionRunError(TInt aError, const THTTPSessionEvent& aEvent);

public:	// Methods from MHTTPFilter

	/// @see MHTTPFilter::MHFUnload
	virtual void MHFUnload(RHTTPSession aSession, THTTPFilterHandle aHandle);

	/// @see MHTTPFilter::MHFLoad
	virtual void MHFLoad(RHTTPSession aSession, THTTPFilterHandle aHandle);
	
 private:
	
	CTestFilter();

	///ContructL also Installs the filter.
	void ConstructL( RHTTPSession aSession );
	
	void CheckHeadersL( RHTTPTransaction& aTrans );
	
	void CleanupAll();
	
   	TBool FindAuth(const TDesC8& aQop) const;
	
	void DoSubmitL( RHTTPTransaction aTransaction );
	
	void EncodeBasicAuthL(RString& aUsername,RString& aPW, RHTTPTransaction& aTransaction);
	
 	TBool HandleTimerEventL(TTimerEventType aEventType);
 	
	/// A count to make sure we delete at the right time.
	TInt        iLoadCount;
	RStringPool iStringPool;
	RStringF 	iUsernameStr;
	RStringF 	iPasswordStr;
	RStringF 	iUserAgent;
	RStringF    iAuthInfo;
    TInt64      iSeed;
    TInt		iOffset;
	TUint8      iFlags;
	TBuf8<5>    iProtocolIdentifier;
	TInt        iBootstrapCount;
	TBool       iBootstrapPending;
	TBool       iHaveCredentials;
	CActiveSchedulerWait      iBootstrapWait;
    CTimerOut* iTimeOut;   

};

#endif //__TESTFILTER_H_

//EOF

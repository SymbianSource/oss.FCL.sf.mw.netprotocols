// Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// $Header$
// The header file introducing the collection of HTTP command classes
// of the Integration Harness framework.
// 
//

#ifndef __HTTPFRMWRK_H__
#define __HTTPFRMWRK_H__

//-----------------------------------------------------------------------------

class CWspEventDispatcher;

class CFrmwrkSession :	public CBase, 
						public MHTTPSessionEventCallback,
						public MHTTPAuthenticationCallback
	{
	public:

	static CFrmwrkSession *NewL ( const TDesC& aName, 
								  const TDesC& aFramework, 
								  CTEngine *aTestMachine);

	static CFrmwrkSession *NewLC( const TDesC& aName, 
								  const TDesC& aFramework, 
								  CTEngine *aTestMachine);

	virtual ~CFrmwrkSession();

	enum TFrmkSessionState
		{
		ENormal,
		EWSPConnecting,
		EWSPDisconnecting,
		EWSPDisconnected
		};

	private:

	void ConstructL( const TDesC& aName, 
					 const TDesC& aFramework, 
					 CTEngine *aTestMachine );
	private:	
		
	inline CTEngine* Machine() ;

	public:
	virtual void MHFSessionRunL(const THTTPSessionEvent& aEvent);
	virtual TInt MHFSessionRunError(TInt aError, const THTTPSessionEvent& aEvent);
	//	from AuthentificationCallback
	virtual TBool GetCredentialsL(const TUriC8 &aURI,
																RString aRealm,
																RStringF aAuthType,
																RString &aUsername,
																RString &aPassword);
	// TInt id ;
	inline TBool UseWSPProtocol () ;
	CWspEventDispatcher *iEventDispatcher;
	// Get Name.
	TPtrC Name() const ;

	//	simplify logging facility
	void Log(TRefByValue<const TDesC> aFmt, ... );
	inline const RHTTPSession& Session() ;

	TInt OpenL();
	TInt Close();
	TInt SetPropertiesL();
	TInt ConnectL();
	TInt DisconnectL();
	TFrmkSessionState	iState;

	protected:
	CTEngine* iEngine;
	void SetWspProxyPropertiesL(RHTTPSession & aSession, RStringPool &aStrPool) ;
	void SetWspCapabilityPropertiesL( RHTTPSession & aSession, RStringPool &aStrPool) ;

	private:
		TTime iStartTime;
		TBool iuseWSPProtocol;
		RHTTPSession iSession;	
		RConnection iConnection;	
		HBufC* iName;
	};

inline CTEngine* CFrmwrkSession::Machine()
{
	return iEngine ;
}

inline TBool CFrmwrkSession::UseWSPProtocol()
{
	return iuseWSPProtocol ;
}

inline const RHTTPSession& CFrmwrkSession::Session()
{
	return iSession ;
}
//-----------------------------------------------------------------------------

#endif  // __HTTPFRMWRK_H__

//-----------------------------------------------------------------------------
// End of File
//-----------------------------------------------------------------------------


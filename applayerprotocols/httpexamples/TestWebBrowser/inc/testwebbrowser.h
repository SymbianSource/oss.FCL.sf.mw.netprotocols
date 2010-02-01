
// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef _TESTWEBBROWSER_H
#define _TESTWEBBROWSER_H

#include <http/mhttpauthenticationcallback.h>

#include "browsertransaction.h"

class CBrowserTransaction;
class CHttpExampleUtils;

class CTestWebBrowser : public CActive, 
						public MBrowserTransactionObserver,
						public MHTTPAuthenticationCallback
	{
	public:
		
		enum THttpClientState
			{
			EMainMenu,
			EEnterUrl,
			ESelectProxy,
			EStop,
			EStartTransaction,
			ETransactionStarted,
			ETransactionCompleted		
			};
		
		enum TMenuSelection
			{
			EDownloadURL,
			ESetProxy,
			EQuit
			};
		virtual ~CTestWebBrowser ();
		
		static CTestWebBrowser* NewLC ( CHttpExampleUtils& aUtils );
		static CTestWebBrowser* NewL ( CHttpExampleUtils& aUtils );
		
		
		virtual void OnTransactionCreateL ( const TDesC8& aUri, TBool aParseHtml );
		virtual void OnTransactionClose ( CBrowserTransaction* aTrans );

		// methods inherited from MHTTPAuthenticationCallback
		virtual TBool GetCredentialsL ( const TUriC8& aURI, RString aRealm, 
							  		 RStringF aAuthenticationType,
							  		 RString& aUsername, 
								 	 RString& aPassword );
			
	protected:
		CTestWebBrowser ( CHttpExampleUtils& aUtils );
		void ConstructL ();
		void IssueAndCompleteRequest();
		
		void ExecuteMainMenuL ();
		TBool ValidateUri ( const TDesC8& aUri );

		HBufC* GenerateFilePathL ( const TUriC8& aUri );
		
		void AcceptUrlL ();
		void AcceptProxyL ();
		
		void OnTransactionCompleted ();
		
		void CreateTransactionL ( const TDesC8& aUri, TBool aParseHtml = EFalse );
		
		void SetProxyL ( const TDesC8& aProxyAddr );
		
	private:
		// Methods from CActive
		virtual void RunL();
		virtual void DoCancel();
		
	private:
		RPointerArray < CBrowserTransaction > iTransArray;
		CHttpExampleUtils& iTestUtils;
		RHTTPSession iHttpSession;	
		
		THttpClientState iCurrentState;
		
		HBufC8* iBaseUri;	
	};

#endif
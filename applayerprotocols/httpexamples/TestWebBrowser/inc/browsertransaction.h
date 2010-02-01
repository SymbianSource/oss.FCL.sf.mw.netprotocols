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

#ifndef _HTTPTRANSACTION_H
#define _HTTPTRANSACTION_H

#include <http.h>

class CHttpExampleUtils;
class CBrowserTransaction;
class CHtmlHandler;

class MBrowserTransactionObserver 
	{
	public:
		virtual void OnTransactionCreateL ( const TDesC8& aUri, TBool aParseHtml ) = 0;
		virtual void OnTransactionClose ( CBrowserTransaction* aTrans ) = 0;
	};

// -------------------------------------------------------------------
class CBrowserTransaction : public CBase, public MHTTPTransactionCallback
	{
	public:
		
		enum TMimeType
			{
			eXml,
			eHtml,
			eUnknown				
			};
		
		virtual ~CBrowserTransaction();
		
		static CBrowserTransaction* NewLC ( RHTTPSession& aSession, 
											CHttpExampleUtils& aTestUtils,
											MBrowserTransactionObserver* aObserver,
											const TDesC& aFilePath,
											TBool aParseHtml  = EFalse  );
									
		static CBrowserTransaction* NewL ( 	RHTTPSession& aSession, 
											CHttpExampleUtils& aTestUtils,
											MBrowserTransactionObserver* aObserver,	
											const TDesC& aFilePath,
											TBool aParseHtml  = EFalse  );
							
		void CreateTransactionL ( const TDesC8& aUri );
		void StartTransactionL ();
		
		//
		// methods from MHTTPTransactionCallback
		//
		virtual void MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent);
		virtual TInt MHFRunError(TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& aEvent);
			
	protected:
		CBrowserTransaction( RHTTPSession& aSession, CHttpExampleUtils& aTestUtils, MBrowserTransactionObserver* aObserver, TBool aParseHtml  = EFalse  );
		void ConstructL ( const TDesC& aFilePath );

	private:	
		void ExtractContentTypeL ( RHTTPResponse& aResponse, RStringF& aContentTypeValue );
		void LoadPluginL ( TMimeType aType );
		void SetAcceptHdrL ();
		TMimeType GetMimeType ( const RStringF& aType );
	private:
		RHTTPTransaction iTransaction;	
		
		RHTTPSession& iSession;	
		MBrowserTransactionObserver* iTransObserver;	
		CHttpExampleUtils& iTestUtils;
		
		HBufC* iFilePath;
		
		RFs iFileServer;
		RFile iRespBodyFile;
		MHTTPDataSupplier* iRespBody;	
		
		CHtmlHandler *iHtmlHandler;
		
		TBool iParseHtml;
		
		TBool iSavingBody;
		
	};

#endif // _HTTPTRANSACTION_H
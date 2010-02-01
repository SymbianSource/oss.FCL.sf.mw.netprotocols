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

#ifndef HTMLHANDLER_H
#define HTMLHANDLER_H

#include <xml/contenthandler.h>
#include <xml/parser.h>
class MBrowserTransactionObserver;

class CHtmlHandler : public CBase, public Xml::MContentHandler
	{
	public:
	
	static CHtmlHandler* NewL ( MBrowserTransactionObserver& aObserver, const TDesC8& aType );
	static CHtmlHandler* NewLC ( MBrowserTransactionObserver& aObserver, const TDesC8& aType );
	
	~CHtmlHandler ();
	
	// Virtual functions from MContentHandler
	void OnStartDocumentL( const Xml::RDocumentParameters &aDocParam, TInt aErrorCode );
	void OnEndDocumentL( TInt aErrorCode );	
	void OnStartElementL( const Xml::RTagInfo &aElement, const Xml::RAttributeArray &aAttributes, TInt aErrorCode );
	void OnEndElementL( const Xml::RTagInfo &aElement, TInt aErrorCode );
	void OnContentL( const TDesC8 &aBytes, TInt aErrorCode );
	void OnStartPrefixMappingL( const RString &aPrefix, const RString &aUri, TInt aErrorCode );
	void OnEndPrefixMappingL( const RString &aPrefix, TInt aErrorCode );
	void OnIgnorableWhiteSpaceL( const TDesC8 &aBytes, TInt aErrorCode );
	void OnSkippedEntityL( const RString &aName, TInt aErrorCode );
	void OnProcessingInstructionL( const TDesC8 &aTarget, const TDesC8 &aData, TInt aErrorCode );
	void OnError( TInt aErrorCode );
	TAny* GetExtendedInterface( const TInt32 aUid );
	
	void ParseHtmlContentL ( const TDesC8& aContent );
	
	void ParseEndL ();
	protected:		
		CHtmlHandler ( MBrowserTransactionObserver& aObserver ); // constructor.
		void ConstructL ( const TDesC8& aType ); 
		
	private:
		TBool GetAttributeValueL ( RString& aAttributeValue, const RString& aAttribute, const Xml::RAttributeArray& aAttributeArray );
		
	private:		
		RStringPool iStringPool; 
		MBrowserTransactionObserver& iTransObserver;
		Xml::CParser* iHtmlParser;
	};


#endif // HTMLHANDLER_H
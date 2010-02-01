// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// ineturiproperties.h
// 
//

#ifndef __URILISTINITIALIZER_H__
#define __URILISTINITIALIZER_H__

#include <xml/contenthandler.h>
#include <xml/parser.h>

class CUriListInterface;
class CInetUriImpl;

/**
This class initializes the database from a pre-configured XML file. The XML file takes the following form

*/

NONSHARABLE_CLASS ( CUriListInitializer ) : public CBase,
											public Xml::MContentHandler
	{
	public:
	
	enum TParserState
		{
		EInitial,
		EStartOfServicetype,
		EEndOfServicetype,
		EStartOfListtype,
		EEndOfListtype,
		EStartOfRecord,
		EEndOfRecord,
		EStartOfUri,
		EEndOfUri,
		EStartOfPermission,
		EEndOfPermission,
		EStartOfFavouriteName,
		EEndOfFavouriteName
		};
		
	static CUriListInitializer* NewL ( CUriListInterface& aListInterface );
	~CUriListInitializer ();
	
	void ParseDocumentL ( const TDesC& aFile );
	
	// Virtual functions from MContentHandler
	void OnStartDocumentL ( const Xml::RDocumentParameters &aDocParam, TInt aErrorCode );
	void OnEndDocumentL ( TInt aErrorCode );	
	void OnStartElementL ( const Xml::RTagInfo &aElement, const Xml::RAttributeArray &aAttributes, TInt aErrorCode );
	void OnEndElementL ( const Xml::RTagInfo &aElement, TInt aErrorCode );
	void OnContentL ( const TDesC8 &aBytes, TInt aErrorCode );
	inline void OnStartPrefixMappingL ( const RString &aPrefix, const RString &aUri, TInt aErrorCode );
	inline void OnEndPrefixMappingL ( const RString &aPrefix, TInt aErrorCode );
	inline void OnIgnorableWhiteSpaceL ( const TDesC8 &aBytes, TInt aErrorCode );
	inline void OnSkippedEntityL ( const RString &aName, TInt aErrorCode );
	inline void OnProcessingInstructionL ( const TDesC8 &aTarget, const TDesC8 &aData, TInt aErrorCode );
	inline void OnError ( TInt aErrorCode );
	inline TAny* GetExtendedInterface ( const TInt32 aUid );
	
	private:
	CUriListInitializer ( CUriListInterface& aListInterface );
	void ConstructL ();
	
	TInt ConvertToInteger ( const TDesC8& aValue );
	void SetParserState ( TParserState aNewState );
	TParserState ParserStateL ( const RString& aTag, TBool aEndElement =EFalse );
	void SetServiceType ( TInt aIndex );
	void SetListType ( TInt aIndex );
	void SetValueL ( const TDesC8& aValue );
	
	private:		
	CUriListInterface& 	iListInterface;
	CInetUriImpl*		iInetUriImpl;
	RStringPool			iStringPool;
	TParserState		iParserState;
	RBuf8				iContent;
	};

#include "urilistinitializer.inl"

#endif // __URILISTINITIALIZER_H__

// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "urilistinitializer.h"
#include "urischema.h"
#include "urilistinterface.h"
#include "ineturiimpl.h"

using namespace Xml;


CUriListInitializer* CUriListInitializer::NewL ( CUriListInterface& aListInterface )
	{
	CUriListInitializer* self = new ( ELeave ) CUriListInitializer ( aListInterface );
	CleanupStack::PushL ( self );
	self->ConstructL ();
	CleanupStack::Pop ();
	return self;
	}

CUriListInitializer::~CUriListInitializer ()
	{
	iContent.Close ();
	iStringPool.Close ();
	delete iInetUriImpl;
	}


CUriListInitializer::CUriListInitializer ( CUriListInterface& aListInterface )
: iListInterface ( aListInterface ),
iParserState ( EInitial )
	{
		
	}

void CUriListInitializer::ConstructL ()
	{
	const TInt KMaxContentSize = 4 * 1024;
	iContent.CreateL (KMaxContentSize);
	iStringPool.OpenL ( URIXMLTAGS::Table ); // Open the schema table
	iInetUriImpl = CInetUriImpl::NewL (); // Create an empty object
	}

void CUriListInitializer::ParseDocumentL ( const TDesC& aFile )
	{
	_LIT8( KXmlParserDataType, "text/xml" );
	
	RFs myFs;
	CleanupClosePushL ( myFs );
	User::LeaveIfError ( myFs.Connect () );
	
	CParser* myParser = CParser::NewLC ( KXmlParserDataType(), *this );
	ParseL ( *myParser, myFs, aFile );
	CleanupStack::PopAndDestroy ( 2 ); // myParser, myFs
	}

// Virtual functions from MContentHandler
void CUriListInitializer::OnStartDocumentL ( const Xml::RDocumentParameters& /* aDocParam */, TInt aErrorCode )
	{
	User::LeaveIfError ( aErrorCode );
	}
	
void CUriListInitializer::OnEndDocumentL ( TInt aErrorCode )
	{
	User::LeaveIfError ( aErrorCode );		
	}

void CUriListInitializer::OnStartElementL ( const Xml::RTagInfo& aElement, const Xml::RAttributeArray& /* aAttributes */, TInt aErrorCode )
	{	
	User::LeaveIfError ( aErrorCode );		
	SetParserState ( ParserStateL( aElement.LocalName() ) );
	}

void CUriListInitializer::OnEndElementL ( const Xml::RTagInfo& aElement, TInt aErrorCode )
	{
	User::LeaveIfError ( aErrorCode );
	SetParserState ( ParserStateL( aElement.LocalName(), ETrue ) );		
	SetValueL ( KNullDesC8() );
	if (!iInetUriImpl->IsDirty()) 
		{
		if( iParserState == EEndOfRecord )
		    {
		    // Insert the record	
		    iListInterface.InsertL ( *iInetUriImpl );
		    iInetUriImpl->Clear ();
		    }
		}
	}

void CUriListInitializer::OnContentL ( const TDesC8& aBytes, TInt aErrorCode )
	{
	User::LeaveIfError ( aErrorCode );	
	SetValueL ( aBytes );
	}

TInt CUriListInitializer::ConvertToInteger ( const TDesC8& aValue )
	{
	TInt num = -1;
	TLex8 lexRead (aValue);
	lexRead.Val ( num );
	return num;
	}

void CUriListInitializer::SetParserState ( TParserState aNewState )
	{
	switch ( aNewState )
		{
		case EInitial:
		iParserState = EInitial;
		break;
		
		case EStartOfServicetype:
		__ASSERT_DEBUG ( iParserState == EInitial, User::Invariant() );
		iParserState = aNewState;
		break;
		
		case EEndOfServicetype:
		__ASSERT_DEBUG ( iParserState == EEndOfListtype, User::Invariant() );
		iParserState = aNewState;
		break;
		
		case EStartOfListtype:
		__ASSERT_DEBUG ( (iParserState == EStartOfServicetype) || (iParserState == EEndOfListtype), User::Invariant() );
		iParserState = aNewState;
		break;
		
		case EEndOfListtype:
		__ASSERT_DEBUG ( iParserState == EEndOfRecord, User::Invariant() );
		iParserState = aNewState;		
		break;
		
		case EStartOfRecord:
		__ASSERT_DEBUG ( (iParserState == EStartOfListtype) || (iParserState == EEndOfRecord), User::Invariant() );
		iParserState = aNewState;				
		break;
		
		case EEndOfRecord:
		__ASSERT_DEBUG ( (iParserState == EEndOfUri) || (iParserState == EEndOfPermission) || (iParserState == EEndOfFavouriteName), User::Invariant() );
		iParserState = aNewState;				
		break;
		
		case EStartOfUri:
		case EStartOfPermission:
		case EStartOfFavouriteName:		
		iParserState = aNewState;				
		break;
		
		case EEndOfUri:
		__ASSERT_DEBUG ( (iParserState == EStartOfUri), User::Invariant() );
		iParserState = aNewState;
		break;
		
		case EEndOfPermission:
		__ASSERT_DEBUG ( (iParserState == EStartOfPermission), User::Invariant() );			
		iParserState = aNewState;		
		break;
		
		case EEndOfFavouriteName:
		__ASSERT_DEBUG ( (iParserState == EStartOfFavouriteName), User::Invariant() );
		iParserState = aNewState;		
		break;

		default:

		// Do nothing
		break;
		}	
	}

CUriListInitializer::TParserState CUriListInitializer::ParserStateL ( const RString& aTag, TBool aEndElement /* =EFalse */ )
	{	
	// Open a RString from the string pool
	RString myTagString = iStringPool.OpenStringL ( aTag.DesC() );
	TInt tblIndex = myTagString.Index ( URIXMLTAGS::Table );
	TParserState parserState = EInitial;
	switch ( tblIndex )
		{
		case URIXMLTAGS::EWapPush:
		case URIXMLTAGS::EBrowser:
		case URIXMLTAGS::EDevPro:
		case URIXMLTAGS::EPushEMail:
		case URIXMLTAGS::EVoip:
		parserState = aEndElement ? EEndOfServicetype : EStartOfServicetype;
		SetServiceType ( tblIndex );
		break;
		case URIXMLTAGS::EWhitelist:
		case URIXMLTAGS::EBlacklist:
		parserState = aEndElement ? EEndOfListtype : EStartOfListtype;
		SetListType ( tblIndex );
		break;
		
		case URIXMLTAGS::ERecord:
		parserState = aEndElement ? EEndOfRecord : EStartOfRecord;
		break;
		
		case URIXMLTAGS::EUri:
		parserState = aEndElement ? EEndOfUri : EStartOfUri;
		break;

		case URIXMLTAGS::EPermission:
		parserState = aEndElement ? EEndOfPermission : EStartOfPermission;		
		break;

		case URIXMLTAGS::EFavouriteName:
		parserState = aEndElement ? EEndOfFavouriteName : EStartOfFavouriteName;
		break;
		
		default:
		// Do nothing
		break;
		}	
	myTagString.Close ();
	return parserState;	
	}

void CUriListInitializer::SetServiceType ( TInt aIndex )
	{
	switch ( aIndex )
		{
		case URIXMLTAGS::EWapPush:
		iInetUriImpl->Properties().SetServiceType (InetUriList::EWapPush);
		break;
		case URIXMLTAGS::EBrowser:
		iInetUriImpl->Properties().SetServiceType (InetUriList::EBrowser);		
		break;
		case URIXMLTAGS::EDevPro:
		iInetUriImpl->Properties().SetServiceType (InetUriList::EDevProv);		
		break;
		case URIXMLTAGS::EPushEMail:
		iInetUriImpl->Properties().SetServiceType (InetUriList::EPushEMail);
		break;
		case URIXMLTAGS::EVoip:
		iInetUriImpl->Properties().SetServiceType (InetUriList::EVoip);
		break;
		default:
		User::Invariant ();				
		}
	}

void CUriListInitializer::SetListType ( TInt aIndex )
	{
	switch ( aIndex )	
		{
		case URIXMLTAGS::EWhitelist:
		iInetUriImpl->Properties().SetListType (InetUriList::EWhiteList);
		break;
		
		case URIXMLTAGS::EBlacklist:
		iInetUriImpl->Properties().SetListType (InetUriList::EBlackList);
		break;
		
		default:
		User::Invariant ();
		}
	}

void CUriListInitializer::SetValueL ( const TDesC8& aValue )
	{	
	switch ( iParserState )		
		{
		case EStartOfUri:
		case EStartOfFavouriteName:
		iContent.Append ( aValue );
		break;
		
		case EStartOfPermission:
		iInetUriImpl->Properties().SetPermission (static_cast <InetUriList::TPermission> ( ConvertToInteger ( aValue ) ) );
		break;
				
		case EEndOfUri:
		if ( iContent.Length() > 0 )
			{
			iInetUriImpl->SetUriL ( iContent );
			iContent.Zero ();			
			}
		break;
		
		case EEndOfFavouriteName:
		if ( iContent.Length() > 0 )
			{
			iInetUriImpl->Properties().SetFavouriteNameL ( iContent );		
			iContent.Zero ();		
			}
			break;
			
		default:
		break;	// do nothing
		}
	}

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

#include "tldlistinitializer.h"
#include "tldschema.h"
#include "urilistinterface.h"
#include "tldproperties.h"

using namespace Xml;

CTldListInitializer* CTldListInitializer::NewL ( CUriListInterface& aListInterface )
	{
	CTldListInitializer* self = new ( ELeave ) CTldListInitializer ( aListInterface );
	CleanupStack::PushL ( self );
	self->ConstructL ();
	CleanupStack::Pop ();
	return self;
	}

CTldListInitializer::~CTldListInitializer ()
	{
	iContent.Close ();
	iTldName.Close ();
	iStringPool.Close ();
	delete iTldUriImpl;
	}


CTldListInitializer::CTldListInitializer ( CUriListInterface& aListInterface )
: iListInterface ( aListInterface ),
iParserState ( EInitial )
	{
		
	}

void CTldListInitializer::ConstructL ()
	{
	const TInt KMaxContentSize = 4 * 1024;
	iContent.CreateL (KMaxContentSize);
	iStringPool.OpenL ( TLDXMLTAGS::Table ); // Open the schema table
	iTldUriImpl = CTldProperties::NewL (InetUriList::EBlackList); // Create an empty object
	}

void CTldListInitializer::ParseDocumentL ( const TDesC& aFile )
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
void CTldListInitializer::OnStartDocumentL ( const Xml::RDocumentParameters& /* aDocParam */, TInt aErrorCode )
	{
	User::LeaveIfError ( aErrorCode );
	}
	
void CTldListInitializer::OnEndDocumentL ( TInt aErrorCode )
	{
	User::LeaveIfError ( aErrorCode );		
	}

void CTldListInitializer::OnStartElementL ( const Xml::RTagInfo& aElement, const Xml::RAttributeArray&  aAttributes, TInt aErrorCode )
	{	
	User::LeaveIfError ( aErrorCode );		
	SetParserState ( ParserStateL( aElement.LocalName() ) );
	SaveTldName(aAttributes); 
	SetTldNameL ( );
	}

void CTldListInitializer::OnEndElementL ( const Xml::RTagInfo& aElement, TInt aErrorCode )
	{
	User::LeaveIfError ( aErrorCode );
	SetParserState ( ParserStateL( aElement.LocalName(), ETrue ) );		
	SetValueL ( KNullDesC8() );
	if ( iParserState == EEndOfListtype )
		{
		// Insert the record and then clear
		iListInterface.InsertTldPolicyDataL( *iTldUriImpl );
		iTldUriImpl->Clear();
		}
	}

void CTldListInitializer::OnContentL ( const TDesC8& aBytes, TInt aErrorCode )
	{
	User::LeaveIfError ( aErrorCode );	
	SetValueL ( aBytes );
	}

void CTldListInitializer::SetParserState ( TParserState aNewState )
	{
	switch ( aNewState )
		{
		case EInitial:
		iParserState = EInitial;
		break;
		
		case EStartOfPolicyData:
		__ASSERT_DEBUG ( iParserState == EInitial, User::Invariant() );
		iParserState = aNewState;
		break;

		case EEndOfPolicyData:
		__ASSERT_DEBUG ( iParserState == EEndOfTLD, User::Invariant() );
		iParserState = aNewState;
		break;
		
		case EStartOfTLD:
		__ASSERT_DEBUG ( (iParserState == EStartOfPolicyData) || (iParserState == EEndOfTLD ), User::Invariant() );
		iParserState = aNewState;				
		break;
		
		case EEndOfTLD:
		__ASSERT_DEBUG ( iParserState == EEndOfListtype, User::Invariant() );
		iParserState = aNewState;				
		break;
		
		case EStartOfListtype:
		__ASSERT_DEBUG ( (iParserState ==  EStartOfTLD ) || (iParserState == EEndOfListtype), User::Invariant() );
		iParserState = aNewState;
		break;
		
		case EEndOfListtype:
		__ASSERT_DEBUG ( iParserState == EStartOfListtype, User::Invariant() );
		iParserState = aNewState;		
		break;
		
		default:

		// Do nothing
		break;
		}	
	}

CTldListInitializer::TParserState CTldListInitializer::ParserStateL ( const RString& aTag, TBool aEndElement /* =EFalse */ )
	{	
	// Open a RString from the string pool
	RString myTagString = iStringPool.OpenStringL ( aTag.DesC() );
	TInt tblIndex = myTagString.Index ( TLDXMLTAGS::Table );
	TParserState parserState( EInitial );
	switch ( tblIndex )
		{
		case TLDXMLTAGS::EPolicyData:
			{
			parserState = aEndElement ? EEndOfPolicyData : EStartOfPolicyData;
			break;				
			}

		case TLDXMLTAGS::EWhitelist:
		case TLDXMLTAGS::EBlacklist:
			{
			parserState = aEndElement ? EEndOfListtype : EStartOfListtype;
			SetListType ( tblIndex );
			break;				
			}
		
		case TLDXMLTAGS::ETLD:
			{
			parserState = aEndElement ? EEndOfTLD : EStartOfTLD;
			break;	
			}

		default:
			{
			// Do nothing
			break;
			}
		}	
	myTagString.Close ();
	return parserState;	
	}

void CTldListInitializer::SetListType ( TInt aIndex )
	{
	switch ( aIndex )	
		{
		case TLDXMLTAGS::EWhitelist: 
			{
			iTldUriImpl->SetListType(InetUriList::EWhiteList);
			break;		
			}
		case TLDXMLTAGS::EBlacklist:
			{
			iTldUriImpl->SetListType(InetUriList::EBlackList);
			break;	
			}
		default:
			{
			User::Invariant ();	
			}
		}
	}

void CTldListInitializer::SetTldNameL ( )
	{
	switch ( iParserState )	
		{
		case EStartOfListtype:
			{
			iTldUriImpl->SetTldNameL(iTldName);
			break;
			}
		default:
			{
			//Do Nothing
			break;
			}
		}
	}	

void CTldListInitializer::SetValueL ( const TDesC8& aValue )
	{	
	switch ( iParserState )		
		{
		case EStartOfListtype:
			{
			iContent.Append ( aValue );
			break;	
			}
		case EEndOfListtype:
			{
			if ( iContent.Length() > 0 )
				{
				//Set initialize database(Blobs)
				iTldUriImpl->SetCharacterSetL(iContent);
				iContent.Zero ();
				}
			break;
			}
		default: 
			{
			// do nothing
			break;	
			}
		}
	}

//Store the Tld name for later use
void CTldListInitializer::SaveTldName(const Xml::RAttributeArray&  aAttributes)
	{
	_LIT8( KName, "name" );
	TInt nAttributes = aAttributes.Count();
	for(TInt index(0); index < nAttributes; index++)
		{
		const RAttribute& KAttribute = aAttributes[index];
		const RTagInfo& KNameInfo = KAttribute.Attribute();
		const TDesC8& KLocalName8 = KNameInfo.LocalName().DesC();
		if ( iParserState == EStartOfTLD )
			{
			// Store the TLD name for later use
			__ASSERT_ALWAYS(KLocalName8.CompareF( KName ) == 0, User::Invariant() );
			iTldName.Close();
			TInt err = iTldName.Create( KAttribute.Value().DesC() );
			__ASSERT_ALWAYS(KErrNone == err, User::Invariant() );
			break;	
			}
		}
	}
	
	

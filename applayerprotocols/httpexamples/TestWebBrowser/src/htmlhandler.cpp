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


#include <stringpool.h>
#include <xml/documentparameters.h>
#include <xml/taginfo.h>
#include <xml/attribute.h>
#include <e32def.h>


#include "htmlhandler.h"
#include "htmltagstable.h"
#include "htmlattributestable.h"
#include "browsertransaction.h"

using namespace Xml;

_LIT8 ( KStyleSheet, "stylesheet" );

CHtmlHandler::CHtmlHandler  ( MBrowserTransactionObserver& aObserver )
	: iTransObserver ( aObserver )
	{
		
	}

CHtmlHandler::~CHtmlHandler ()
	{
	delete iHtmlParser;
	}

CHtmlHandler* CHtmlHandler::NewLC ( MBrowserTransactionObserver& aObserver, const TDesC8& aType )
	{
	CHtmlHandler* me = new (ELeave) CHtmlHandler ( aObserver );
	CleanupStack::PushL ( me );
	me->ConstructL ( aType );
	return me;
	}

CHtmlHandler* CHtmlHandler::NewL ( MBrowserTransactionObserver& aObserver, const TDesC8& aType )
	{
	CHtmlHandler* me = CHtmlHandler::NewLC ( aObserver, aType );
	CleanupStack::Pop ( me );
	return me;
	}
	
void CHtmlHandler::ConstructL ( const TDesC8& aType )
	{
	iHtmlParser = CParser::NewL ( aType, *this );
	
	iStringPool = iHtmlParser->StringPool();
	iStringPool.OpenL ( HtmlTagsStringTable::Table ); // open tags table
	iStringPool.OpenL ( HtmlAttributesStringTable::Table ); // open attributes table
	
	iHtmlParser->ParseBeginL ();		
	}

void CHtmlHandler::ParseHtmlContentL ( const TDesC8& aContent )
	{
	iHtmlParser->ParseL ( aContent );
	}
	
// Virtual functions from MContentHandler
void CHtmlHandler::OnStartDocumentL( const RDocumentParameters& /* aDocParam */, TInt /* aErrorCode */ )
	{
	return;	
	}

void CHtmlHandler::OnEndDocumentL( TInt /* aErrorCode */ )
	{
	return;		
	}
	
void CHtmlHandler::OnStartElementL( const RTagInfo& aElement, const RAttributeArray& aAttributes, TInt /* aErrorCode */ )
	{			
	TBool parseHtml = EFalse;
	const TDesC8& KDesc = aElement.LocalName ().DesC();
	HBufC8* des = KDesc.AllocLC();
	TPtr8 ptr( des->Des() );
	ptr.LowerCase ();
	RString tagName = iStringPool.OpenStringL ( ptr );
	CleanupClosePushL ( tagName );
	TInt index = tagName.Index ( HtmlTagsStringTable::Table );
	CleanupStack::PopAndDestroy ();
	
	RString attributeValue;
	TInt attributeIndex = KErrNotFound;
	
	// Only body, img, input, frame, script & link tags are handled.
	switch ( index )
		{
		case HtmlTagsStringTable::EBody:
			{
			attributeIndex = HtmlAttributesStringTable::EBackground;			
			}		
		break;
		case HtmlTagsStringTable::EFrame:
		case HtmlTagsStringTable::EIFrame:
			{
			parseHtml = ETrue;				
			}
		case HtmlTagsStringTable::EImg:
		case HtmlTagsStringTable::EInput:		
		case HtmlTagsStringTable::EScript:		
			{
			attributeIndex = HtmlAttributesStringTable::ESrc;	
			}
		break;					
		
		case HtmlTagsStringTable::ELink:
			{
			RString relValue;
			GetAttributeValueL ( relValue, iStringPool.String ( HtmlAttributesStringTable::ERel, HtmlAttributesStringTable::Table ), aAttributes );
			if ( !relValue.DesC().CompareF( KStyleSheet() ) )
				{
				attributeIndex = HtmlAttributesStringTable::EHref;
				}
			}
		break;					
		default:			
		break;		
		}

	// Get the attribute value corresponds to the attribute name. The attribute index is mapped depends on the tag.	
	// Only required tags are handled else the attribute value will stay as KErrNotFound. 
	TBool result = ( attributeIndex != KErrNotFound ) ? GetAttributeValueL ( attributeValue, iStringPool.String ( attributeIndex, HtmlAttributesStringTable::Table ), aAttributes ) : EFalse;
	
	if ( result )
		{
		// There is an included uri that need to be downloaded. Create a new transaction
		// for the uri.
		iTransObserver.OnTransactionCreateL ( attributeValue.DesC (), parseHtml );
		}		
	CleanupStack::PopAndDestroy ();// des
	return;		
	}
	
void CHtmlHandler::OnEndElementL( const RTagInfo& aElement, TInt /* aErrorCode */ )
	{
	return;		
	}

void CHtmlHandler::OnContentL( const TDesC8& aBytes, TInt aErrorCode )
	{
	return;		
	}
	
void CHtmlHandler::OnStartPrefixMappingL( const RString& /* aPrefix */, const RString& /* aUri */, TInt /* aErrorCode */ )
	{
	return; // do nothing	
	}
	
void CHtmlHandler::OnEndPrefixMappingL( const RString& /* aPrefix */, TInt /* aErrorCode */ )
	{
	return; // do nothing		
	}
	
void CHtmlHandler::OnIgnorableWhiteSpaceL( const TDesC8& /* aBytes */, TInt /* aErrorCode */ )
	{
	return; // do nothing		
	}
	
void CHtmlHandler::OnSkippedEntityL( const RString& /* aName */, TInt /* aErrorCode */ )
	{
	return; // do nothing		
	}
	
void CHtmlHandler::OnProcessingInstructionL( const TDesC8& /* aTarget */, const TDesC8& /* aData */, TInt /* aErrorCode */ )
	{
	return; // do nothing		
	}
	
void CHtmlHandler::OnError( TInt /* aErrorCode */ )
	{
	return;		
	}
	
TAny* CHtmlHandler::GetExtendedInterface( const TInt32 /* aUid */ )
	{
	return NULL; 		
	}


TBool CHtmlHandler::GetAttributeValueL ( RString& aAttributeValue, const RString& aAttribute, const Xml::RAttributeArray& aAttributeArray )
	{	
	const TInt count = aAttributeArray.Count ();	
	for ( TInt i = 0; i < count; ++i )
		{
		if ( !aAttribute.DesC().CompareF ( aAttributeArray[i].Attribute().LocalName ().DesC() ) )
			{
			aAttributeValue = aAttributeArray[i].Value ();				
			return ETrue;
			}			
		}		
	return EFalse;
	}

void CHtmlHandler::ParseEndL ()
{
	iHtmlParser->ParseEndL ();
}





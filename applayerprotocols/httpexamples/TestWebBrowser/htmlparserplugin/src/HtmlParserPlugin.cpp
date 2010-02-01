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

#include "HtmlParserPlugin.h"

#include <e32std.h>
#include <ecom/implementationproxy.h>
#include <xml/stringdictionarycollection.h>
#include <xml/contenthandler.h>

#include "HtmlParser.h"
using namespace Xml;

MParser* CHtmlParserPlugin::EcomNewL( TAny* aInitParams )
	{
	// The cast to the MParser interface is a particular to the Xml Framework.  Ecom plug-ins usually
	// return the pointer to the C class.
	return static_cast< MParser* >( CHtmlParserPlugin::NewL( aInitParams ) );
	}

/*
CHtmlParserPlugin::NewL is for testing and can only be accessed by linking directly to XmlParser.o

*/
CHtmlParserPlugin* CHtmlParserPlugin::NewL( TAny* aInitParams )
	{
	TParserInitParams* p = reinterpret_cast< TParserInitParams* >( aInitParams );

	CHtmlParserPlugin* self = new( ELeave ) CHtmlParserPlugin( p->iContentHandler );

	CleanupStack::PushL( self );
	self->ConstructL ( p->iStringDictionaryCollection );
	CleanupStack::Pop( self );
	return self;
	}


CHtmlParserPlugin::CHtmlParserPlugin( MContentHandler* aContentHandler ) :
	iContentHandler( *aContentHandler )
	{
	}

void CHtmlParserPlugin::ConstructL ( Xml::RStringDictionaryCollection* aStringDictionaryCollection )
	{
	iHtmlParser = CHtmlParser::NewL ( iContentHandler, aStringDictionaryCollection->StringPool() );
	}

/*
Used by framework to delete this object
*/
void CHtmlParserPlugin::Release()
	{
	delete this;
	}

/*
Private destructor - only accessible from Release method
*/
CHtmlParserPlugin::~CHtmlParserPlugin()
	{
	delete iHtmlParser;
	}

TInt CHtmlParserPlugin::EnableFeature( TInt /* aParseMode */ )
	{
	return ETrue; 
	}

TInt CHtmlParserPlugin::DisableFeature( TInt /* aParseMode */ )
	{
	return ETrue; 
	}

TBool CHtmlParserPlugin::IsFeatureEnabled( TInt /* aParseMode */ ) const
	{
	return ETrue; 
	}

void CHtmlParserPlugin::SetContentSink( Xml::MContentHandler& aContentHandler )
	{
	iHtmlParser->SetContentSink ( aContentHandler );
	return;	
	}

void CHtmlParserPlugin::ParseChunkL( const TDesC8& aChunk )
	{
	ParseChunkInPartsL ( aChunk );
	}

void CHtmlParserPlugin::ParseLastChunkL( const TDesC8& aFinalChunk )
	{
	ParseChunkInPartsL ( aFinalChunk, ETrue );
	return;
	}
	
void CHtmlParserPlugin::ParseChunkInPartsL( const TDesC8& aChunk, TBool aLast /* = EFalse */ )
	{	
	if ( ( aChunk.Length () > 0 ) || aLast )
		{
		iHtmlParser->ParseL ( aChunk, aLast );
		}	
	}

	
const TInt KUidHtmlParserPlugin = 0x10272787;

const TImplementationProxy ImplementationTable[] = {
	IMPLEMENTATION_PROXY_ENTRY( KUidHtmlParserPlugin, CHtmlParserPlugin::EcomNewL )
};

EXPORT_C const TImplementationProxy* ImplementationGroupProxy( TInt& aTableCount ) 
	{

	aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);

	return ImplementationTable;
	}

#ifndef EKA2
GLDEF_C TInt E32Dll(TDllReason)
	{
	return KErrNone;
	}
#endif






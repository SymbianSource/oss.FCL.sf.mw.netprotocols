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

#ifndef _HTML_PARSER_PLUGIN_H
#define _HTML_PARSER_PLUGIN_H

#include <xml/plugins/parserinterface.h>
#include <xml/plugins/parserinitparams.h>

class CHtmlParser;

class CHtmlParserPlugin : public CBase, public Xml::MParser
{
public:
	/*
	EcomNewL function intended for construction by Ecom of implementation MParser interface.
	*/
	static Xml::MParser* EcomNewL( TAny* aInitParams );

	/*
	This class has a private destructor.  The Xml Framework destroys the parser using this Release method.
	*/
	void Release();

	/*
	Standard NewL function.  This function is used by EcomNewL but has been provided separately for
	testing purposes.
	*/
	static CHtmlParserPlugin* NewL( TAny* aInitParams );

	/*
	Implementation of MParser interface
	*/
	TInt EnableFeature( TInt aParserFeature );
	TInt DisableFeature( TInt aParserFeature );
	TBool IsFeatureEnabled( TInt aParserFeature ) const;
	void ParseChunkL( const TDesC8& aChunk );
	void ParseLastChunkL( const TDesC8& aFinalChunk );

	void SetContentSink( Xml::MContentHandler& aContentHandler );
	
	RStringPool& StringPool();

protected:
	void ConstructL ();
private:
	CHtmlParserPlugin( Xml::MContentHandler* aContentHandler );
	void ConstructL( Xml::RStringDictionaryCollection* aStringDictionaryCollection );
	void ParseChunkInPartsL( const TDesC8& aChunk, TBool aLast = EFalse );
	virtual ~CHtmlParserPlugin();

private:	
	/*
	References to client-provided objects
	*/
	Xml::MContentHandler& iContentHandler;
	
	CHtmlParser* iHtmlParser;
};

#endif // _HTML_PARSER_PLUGIN_H
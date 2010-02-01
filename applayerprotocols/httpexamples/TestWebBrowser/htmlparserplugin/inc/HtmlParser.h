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
// HTML parser header
// @publishedAll
// @released
// 
//

#ifndef _HTMLPARSER_H
#define _HTMLPARSER_H

#include <xml/parserfeature.h>
#include <xml/plugins/parserinitparams.h>

class CDesC8ArrayFlat;
class CStringParser;

class CHtmlParser : public CBase 
	{
	public:
		enum TTagState
			{
			ENoTag,
			EOpeningTag,
			EClosingTag,
			EText
			};

		enum TParserState
			{
			EInitialState,
			ESeeStartOfTag,
			ESeeEndOfTag,
			ESeeClosingTagIndicator,
			ESeeEquals,
			ESeeExclamationMark,
			ESeeEndOfTagWhileReadingJavascript,
			EReadingOpeningTag,
			EReadingClosingTag,
			EReadingAttribute,
			EReadingAttributeValue,
			EReadingAttributeValueWithinApos,
			EReadingAttributeValueWithinQuot,
			EReadingText,
			EReadingJavascript,
			EFinishedReadingTag,
			EFinishedReadingAttribute,
			EFinishedReadingAttributeValue			
			};

		static CHtmlParser* NewL ( Xml::MContentHandler& aContentHandler, RStringPool& aStringPool );	
		~CHtmlParser ();
		
		void ParseL ( const TDesC8& aBuffer, TBool aLastChunk = EFalse );	
		void SetContentSink(class Xml::MContentHandler& );

	private:
		CHtmlParser ( Xml::MContentHandler& aContentHandler, RStringPool& aStringPool );
		void ConstructL ();		

		void CreateTagInfoLC ( Xml::RTagInfo& aTagInfo, const TDesC8& aTag );
		
		void ParseContentL ( const TDesC8& aContent, TBool aLastChunk = EFalse );		

		void ParseTaggedBufferL ( const TPtrC8& aTaggedBuffer, TBool aPartial = EFalse );
		
		void ProcessPartialContentL ( CStringParser& aParser );
		void AddToPartialContentL ( const TPtrC8& aContent, TBool aAppend = EFalse ); 
		
		void ParseStartDocumentL ( CStringParser& aParser );
		
		void ProcessTextL ( const TPtrC8& aText );	
		
		TBool InspectCurrentCharacter( TChar aChar );

		void  SeeWhiteSpaceCharacterInTag( TBool& aBool );
		void  SeeSpecialCharactersInTag( TChar aChar, TBool& aBool );
		void  SeeOtherCharactersInTag();
		void  SeeOtherCharactersNotInTag( TBool& aBool );
		
		TBool CheckAndRemoveTagL ( const TPtrC8& aTag );

		void CallStartElementL ( const TDesC8& aTag );
		void CallEndElementL ( const TDesC8& aTag );
		
		void CallStartDocumentL ( const TDesC8& aCharset );		

		TBool IsOptionalTagL ( const TDesC8& aTag );
		void CheckAndProcessLastOptionalTagL ( const TDesC8& aTag );
		TBool IsForbiddenTagL ( const TDesC8& aTag );
		void CheckAndProcessForbiddenTagL ( const TDesC8& aTag );
		
		void ExtractCharsetL ( const TDesC8& aContent, TPtrC8& aCharset, TBool& aXMLFound );
		void ExtractCharsetValueL ( const TDesC8& aContent, const TDesC8& aSearchValue, TPtrC8& aCharset );
		
	private:	
		Xml::MContentHandler* 	iContentHandler;
		RStringPool& 			iStringPool;
		
		TTagState				iTagState;
		TParserState			iParserState;	
		TBool 					iInTag;
		
		HBufC8*					iPartialContent; // Content which is pending to parse.
		
		CDesC8ArrayFlat*		iTagAttribute;
		CDesC8ArrayFlat* 		iTagAttributeValue;
		CDesC8ArrayFlat*		iTagQueue;
				
		TBool 					iFirst;
		TBool					iEndDocCalled;		
	};

#endif // _HTMLPARSER_H
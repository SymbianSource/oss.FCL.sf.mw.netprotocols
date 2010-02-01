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

#include <xml/xmlparsererrors.h>
#include <xml/taginfo.h>
#include <xml/documentparameters.h>
#include <xml/contenthandler.h>
#include <e32test.h>

#include "HtmlParser.h"
#include "StringParser.h"
#include "badesca.h"
#include "HtmlTagsTable.h"

using namespace Xml;

const TInt KPartialContenttSize = 256;
const TInt KDefaultGranularity = 4;
const TInt KStartTag = '<' ;
const TInt KEndTag = '>';

_LIT8 ( KMetaName, "meta" );
_LIT8 ( KDefaultCharset, "UTF-8" );
_LIT8 ( KXMLName, "?xml" );

LOCAL_C void AttributeArrayDelete( TAny* aPtr );


CHtmlParser::CHtmlParser ( Xml::MContentHandler& aContentHandler, RStringPool& aStringPool )
 :iContentHandler ( &aContentHandler ),
  iStringPool ( aStringPool ),
  iTagState ( ENoTag ),
  iParserState ( EInitialState ),
  iFirst ( ETrue ),
  iEndDocCalled ( EFalse )
	{	
	}

CHtmlParser::~CHtmlParser ()
	{	
	delete iPartialContent;			
	delete iTagAttribute;		
	delete iTagAttributeValue;	
	delete iTagQueue;	
	}

CHtmlParser* CHtmlParser::NewL ( Xml::MContentHandler& aContentHandler, RStringPool& aStringPool )
	{
	CHtmlParser* self = new (ELeave) CHtmlParser ( aContentHandler, aStringPool );
	CleanupStack::PushL ( self );
	
	self->ConstructL ();
	
	CleanupStack::Pop ();
	return self;
	}


void CHtmlParser::ConstructL ()
	{		
	iStringPool.OpenL ( HtmlTagsStringTable::Table );
	iTagAttribute = new (ELeave) CDesC8ArrayFlat ( KDefaultGranularity );
	iTagAttributeValue = new (ELeave) CDesC8ArrayFlat ( KDefaultGranularity );
	iTagQueue = new (ELeave) CDesC8ArrayFlat ( KDefaultGranularity );
	
	iPartialContent = HBufC8::NewL ( KPartialContenttSize );	
	}

void CHtmlParser::ParseL ( const TDesC8&  aBuffer, TBool aLastChunk /* = EFalse */  )
	{
	ParseContentL ( aBuffer, aLastChunk );
	}

void CHtmlParser::CreateTagInfoLC ( RTagInfo& aTagInfo, const TDesC8& aTag )
	{
	RString uriString;
	RString nameString;
	RString prefixString;		
		
	// For HTML there is no uri and prefix. So make it null
	uriString = iStringPool.OpenStringL( KNullDesC8 );
	CleanupClosePushL ( uriString );
	prefixString = iStringPool.OpenStringL( KNullDesC8 );
	CleanupClosePushL ( prefixString );
	
	// name string ( tag )
	nameString = iStringPool.OpenStringL ( aTag );
	CleanupClosePushL ( nameString );
	
	// Create tag info with the uri, localname and prefix values.
	aTagInfo.Open( uriString, prefixString, nameString );	
	
	CleanupStack::Pop ( 3 ); 
		
	CleanupClosePushL( aTagInfo );	
	}

void CHtmlParser::SetContentSink( class Xml::MContentHandler& aContentHandler )
	{
	iContentHandler = &aContentHandler;	
	}

// InspectCurrentCharacterL - updates iParserState, iTagState and iInTag	
TBool CHtmlParser::InspectCurrentCharacter( TChar aChar )
	{
	TBool processCharacter = ETrue;

	if ( iParserState == EReadingJavascript )
		{
		if ( aChar == '>' )
			{
			iParserState = ESeeEndOfTagWhileReadingJavascript;			
			}

		}
	else if ( aChar == '<' )
		{
		iTagState	=EOpeningTag;
	 	iParserState = ESeeStartOfTag;
		iInTag		=ETrue;
		}
	else if ( iInTag )
		{
		if ( aChar == '/' || aChar == '=' || aChar == '"' || aChar == '>' || aChar == '!' || aChar == '\'' )
			{
			SeeSpecialCharactersInTag( aChar, processCharacter );
			}
		else if ( aChar.IsSpace() )
			{
			SeeWhiteSpaceCharacterInTag( processCharacter );
			}
		else if ( iParserState == ESeeExclamationMark && aChar=='-' )
			{
			iParserState = EReadingJavascript;
			processCharacter = EFalse;
			}
		else
			{
			SeeOtherCharactersInTag();
			}
		}
	else
		{
		SeeOtherCharactersNotInTag(processCharacter);
		}

	return processCharacter;	
	}

/**
	Set the new parser state depends on the previous, when the parser finds a 
	space in between the tags. 
	eg: <a href="/AudioVideo/TT/" class="navlink" shape="rect">
	1. Read a -> State EReadingOpeningTag
	2. Read space -> State EFinishedReadingTag
	so on...	
	
	@param aBool& [out]
 */
void  CHtmlParser::SeeWhiteSpaceCharacterInTag( TBool& aBool )
	{
	switch (iParserState)
		{
		case EReadingOpeningTag:
			iParserState = EFinishedReadingTag;
			aBool = EFalse;
			break;
		case EReadingClosingTag:
			iParserState = EFinishedReadingTag;
			aBool = EFalse;
			break;
		case EReadingAttribute:
			iParserState = EFinishedReadingAttribute;
			aBool = EFalse;
			break;
		case EReadingAttributeValue:
			iParserState = EFinishedReadingAttributeValue;
			break;
		case EReadingAttributeValueWithinQuot:
			break;		
		default:
			break;
		}	
	}

void  CHtmlParser::SeeSpecialCharactersInTag( TChar aChar, TBool& aBool )
	{
	switch (aChar)
		{
		case '/':
			if ( ( iParserState == EReadingAttributeValue ) || ( iParserState == EReadingAttributeValueWithinQuot ) ||	( iParserState == EReadingAttributeValueWithinApos ) )
				{
				aBool = ETrue;				
				}
			else if ( iParserState == ESeeStartOfTag )
				{
				iTagState = EClosingTag;
	 			iParserState = ESeeClosingTagIndicator;
				aBool = ETrue;
				}
			else if ( iParserState == ESeeEquals )
				{
				// special case. '/' comming after =. The parser will not understand this situation and 
				// will not parse the element.
				// eg: <form action=/search name=f ... >
				iParserState = EReadingAttributeValue;					
				}	
			else if ( ( iParserState == EFinishedReadingAttributeValue ) ||	( iParserState == EFinishedReadingTag )	)
				{	
				iParserState = ESeeClosingTagIndicator;					
				}			
			break;
		case '=':
			if( ( iParserState != EReadingAttributeValue ) && ( iParserState != EReadingAttributeValueWithinQuot ) && ( iParserState != EReadingAttributeValueWithinApos ) )
				{
				iParserState = ESeeEquals;
				aBool = EFalse;
				}
			else
				{
				aBool = ETrue;
				}
			break;
		case '"':
			if ( iParserState == ESeeEquals )
				{
				iParserState = EReadingAttributeValueWithinQuot;
				aBool = EFalse;
				}
			else
				{
				iParserState = EFinishedReadingAttributeValue;
				}
			break;
		case '\'':
			if ( iParserState == ESeeEquals )
				{
				iParserState = EReadingAttributeValueWithinApos;
				aBool = EFalse;
				}
			else
				{
				iParserState = EFinishedReadingAttributeValue;
				}
			break;
			
		case '>':
			iParserState = ESeeEndOfTag;
			iTagState = EClosingTag;
			iInTag = EFalse;
			break;
		case '!':
			if(iParserState == ESeeStartOfTag)
				{
				iParserState = ESeeExclamationMark;
				aBool = EFalse;
				}
			break;
					
		default:
			break;
		}	
	}

void  CHtmlParser::SeeOtherCharactersInTag()
	{
	switch (iParserState)
		{
		case ESeeStartOfTag:
			iParserState = EReadingOpeningTag;
			break;
		case ESeeClosingTagIndicator:
			iParserState = EReadingClosingTag;
			break;
		case ESeeEquals:
			iParserState = EReadingAttributeValue;
			break;
		case EFinishedReadingTag:
			iParserState = EReadingAttribute;
			break;
		case EFinishedReadingAttribute:
			iParserState = EReadingAttribute;
			break;
		case EFinishedReadingAttributeValue:
			iParserState = EReadingAttribute;
			break;
		default:
			break;
		}
	}

void  CHtmlParser::SeeOtherCharactersNotInTag( TBool& aBool )
	{
	switch (iParserState)
		{
		case EReadingText:
			aBool = EFalse;
			break;
		case ESeeEndOfTag:
			iParserState = EReadingText;
			iTagState = EText;
			break;
		case EInitialState:
			iParserState = EReadingText;
			break;
		default:
			break;
		}
	}

/**
	Parses the content and fetches the buffer in between tags or HTML text. 
	The string can be partial ie unable to find the end tag or unable to find 
	the opening tag in case of HTML text. In this case the pending buffer will be 
	saved and will be parsed during the next call.	
	
	@param aContent [out] Content to be parsed. 
 */
void CHtmlParser::ParseContentL ( const TDesC8& aContent, TBool aLastChunk /* = EFalse */ )
	{
	
	CStringParser* parser = CStringParser::NewL ( aContent );	
	CleanupStack::PushL ( parser );

	if ( iFirst )
		{	
		TBool xmlFound;	
		TPtrC8 charset ( KDefaultCharset );
		TRAPD ( err, ExtractCharsetL ( aContent, charset, xmlFound ) );
		TChar ch;
		parser->GetCurrentCharacter ( ch );
		if ( ( err == KErrNotFound ) && ( ch != KStartTag ) )
			{
			CleanupStack::PopAndDestroy ( parser );
			// Maybe an invalid html document. First character must be '<'
			return;
			}
		
		CallStartDocumentL ( charset );
		iFirst = EFalse;	
		
		if ( xmlFound )		
			{
			// ignore that line ( <?xml ... ?> )
			parser->ParseTill ( charset, KEndTag );
			parser->SkipLength ( 1 ); // skip '>'
			}
		}
	
			
	// Process the partial content which is saved in the last call.
	TRAPD ( err, ProcessPartialContentL ( *parser ) );
	if ( err != KErrNone )  
	{		
		AddToPartialContentL ( aContent, ETrue );
		CleanupStack::PopAndDestroy ( parser );
		return; 
	}
		
	TChar ch;
	while ( parser->GetCurrentCharacter( ch ) )
		{	
		
		if ( InspectCurrentCharacter ( ch ) )
			{
			switch ( iTagState )				
				{
				case EOpeningTag:
					{
					TPtrC8 TaggedBuffer;
					
					if ( ch == KStartTag )
						{
						parser->SkipLength ( 1 ); // skip '<'						
						}
					// The parser relies on the assumption that there will not be any '>' in between the
					// tagged content. But there is a chance that attribute value can contain a '<'. The
					// parsing will not happen properly in that case.
					// eg. <tag name="value>abc" >
					TBool success = parser->ParseTill ( TaggedBuffer, KEndTag );
					ParseTaggedBufferL ( TaggedBuffer, !success ); // handle tagged buffer
					break;					
					}
				case EClosingTag:
					parser->SkipLength ( 1 ); // skip '>'
					break;
				case EText:
					{
					TPtrC8 Text;	
					if ( parser->ParseTill ( Text, KStartTag ) )
						{
						ProcessTextL ( Text ); // Handle text.
						}
					else
						{						
						AddToPartialContentL ( Text ); 
						}
					}
					break;
				default:
					parser->SkipLength ( 1 );
					break;
				}
			}
		else
			{
			parser->SkipLength ( 1 );
			}	
			
		if ( iEndDocCalled )				
			{
			break; // </html> has already been reached. No further processing.				
			}
		}
	
	// This is the last chunk. OnEndDocumentL may have been called when it arrives for </html>
	// In case if it cannot find </html> call OnEndDocumentL here. 
	if ( aLastChunk && !iEndDocCalled )
		{
		iContentHandler->OnEndDocumentL ( KErrNone );			
		}
	CleanupStack::Pop ( parser );
	delete parser;	
	}

/**
	Parses the string inbetween the start tag and end tag. 
	Fetches the HTML tags, attribute, values or java content.	
	
	@param aTaggedBuffer [in] Content inbetween start and end tag. 
	@Param aPartial [in] ETrue for partial tagged content.
 */
void CHtmlParser::ParseTaggedBufferL ( const TPtrC8& aTaggedBuffer, TBool  aPartial  /* = EFalse */ )
	{	
	
	// Will add to the partial content variable and return.
	if ( aPartial )
		{
		AddToPartialContentL ( aTaggedBuffer, ETrue ); 
		return;
		}

	CStringParser* parser = CStringParser::NewL ( aTaggedBuffer );
	CleanupStack::PushL ( parser );			
			
	TPtrC8 htmltag;	
	TBool callEndElement = EFalse;
	TBool callStartElement = EFalse;
	TChar ch;	

	while ( parser->GetCurrentCharacter( ch ) )
		{
		InspectCurrentCharacter ( ch );
		
		switch ( iParserState )
			{		
			case ESeeExclamationMark:
				{
				TPtrC8 text;
				
				TChar ch;
				parser->GetNextCharacter( ch );
				
				if ( ch == '-' )
					{
					parser->SkipLength ( 2 ); // skip --
					parser->ParseTill ( text, _L8 ("/-") ); 
					}
				else
					{
					parser->GetRemainder ( text );
					}
				
				iContentHandler->OnContentL( text, KErrNone ); // This is a java script.				
				
				parser->GetRemainder ( text );
				}
			break;
							
			case ESeeClosingTagIndicator:
				{
				TPtrC8 tag;				
				parser->SkipLength ( 1 ); // skip '/'
				parser->GetRemainder ( tag );			
				// check the tag in the array and remove it.	
				if ( !CheckAndRemoveTagL ( tag ) )
					{
					iContentHandler->OnError( EXmlTagMismatch );	
					}
				else
					{
					if ( tag.Length () )
						{
						htmltag.Set ( tag );						
						}
					callEndElement = ETrue;		
					}
				}
			break;
			
			case EReadingOpeningTag:
				{
				parser->ParseTill( htmltag, _L8 ("\t\n ") );
				
				// check whether it is an optional tag.
				// Only the tags which is defined as optional in the HTML 4.01 specs is 
				// considered. Any other tag will not be considered optional.
				// In case of any other tag ( other than defined in specs ) which has optional behaviour 
				// and if no end tag specified for it, the end tag will can get nested.
				if ( IsOptionalTagL ( htmltag ) )	
					{
					CheckAndProcessLastOptionalTagL ( htmltag );
					}				
								
				iTagQueue->AppendL (  htmltag ); // append to tag array				
				callStartElement = ETrue;			
				}
			break;
			
			case EReadingAttribute:
				{
				TPtrC8 attribute;
				parser->ParseTill( attribute, _L8 (" \n\t\r=") );
				iTagAttribute->AppendL ( attribute );	// append to attribute array.
				
				TChar ch;
				TBool success = parser->GetCurrentCharacter ( ch );
				
				if ( !success || ( ch != '=') ) 
					{
					// This attribute doesn't have an attribute value.
					// The attribute value will be same as attribute name.
					iTagAttributeValue->AppendL ( attribute );
					}				
				}
			break;
		
			case EReadingAttributeValue:
				{
				TPtrC8 attributeval;										
				parser->ParseTill( attributeval, _L8 ( " \t\r\n" ) );
				iTagAttributeValue->AppendL ( attributeval ); // append to attribute value array
				}
			break;	
			
			case EReadingAttributeValueWithinQuot:
				{
				parser->SkipLength ( 1 ); // skip '"' 
				TPtrC8 attributeval;										
				parser->ParseTill( attributeval, '"' );
				iTagAttributeValue->AppendL ( attributeval ); // append to attribute value array									
				}
			break;
			
			case EReadingAttributeValueWithinApos:
				{
				parser->SkipLength ( 1 ); // skip ''' 
				TPtrC8 attributeval;										
				parser->ParseTill( attributeval, '\'' );
				iTagAttributeValue->AppendL ( attributeval ); // append to attribute value array														
				}
			break;
						
			default:
				parser->SkipLength ( 1 );
			break;
			}
		};
	
	// Forbidden tags need to be closed immediatly. Check whether it
	// has already closed otherwise close it.
	if ( !callEndElement && IsForbiddenTagL ( htmltag ) )
		{
		CheckAndRemoveTagL ( htmltag );
		callEndElement = ETrue;
		}
		
	// Tell the XML framework about the tag and attribute/value.
	if ( callStartElement )
		{
		CallStartElementL ( htmltag );
		}
		
	// Calling end element... ( End tag ) </html>
	if ( callEndElement )
		{
		CallEndElementL ( htmltag );
		
		if ( !htmltag.CompareF ( _L8 ("html" ) ) )
			{
			iContentHandler->OnEndDocumentL ( KErrNone );
			iEndDocCalled = ETrue;
			}
		}

	
	iTagAttribute->Delete ( 0, iTagAttribute->Count () );
	iTagAttributeValue->Delete ( 0, iTagAttributeValue->Count () );
	
	CleanupStack::Pop ( parser );
	delete parser;
	}

/**
	Inform the CHtmlParser about the HTML text.	
	
	@param aText [in] Document text. 
 */
void CHtmlParser::ProcessTextL ( const TPtrC8& aText )
	{
	iContentHandler->OnContentL ( aText, KErrNone );
	}

/**
	Process the partial content, which is saved in the last call with the new content.	
	
	@param aParser [in,out] String parser object. 
 */
void CHtmlParser::ProcessPartialContentL ( CStringParser& aParser )
	{
	if ( iPartialContent->Des().Length () > 0 )
		{
		switch ( iTagState )											
			{
			case EOpeningTag:
				{				
				TPtrC8 TaggedBuffer;
				TBool success = aParser.ParseTill ( TaggedBuffer, KEndTag );
				if ( success )
					{
					AddToPartialContentL ( TaggedBuffer, ETrue );
					ParseTaggedBufferL ( iPartialContent->Des(), EFalse );
					}
				else
					{
					User::Leave ( KErrNotFound );
					}
				}			
			break;						
			case EText:
				{
				TPtrC8 Text;	
				if ( aParser.ParseTill ( Text, KStartTag ) )
					{
					AddToPartialContentL ( Text, ETrue );
					ProcessTextL ( iPartialContent->Des() ); // Handle text.
					}
				else
					{						
					User::Leave ( KErrNotFound );
					}					
				}
			break;			
			default:			
			// shouldn't come here.
			User::Panic ( _L ("Html parser - partial content processing error."), KErrUnknown );
			break;
			}
		}		
		iPartialContent->Des().Zero (); // zero the partial content length.		
	}

/**
	Add or append to the partial buffer. 
	
	@param aContent [in] Partial content to save.
	@param aAppend [in] ETrue to append to the existing partial buffer.
 */
void CHtmlParser::AddToPartialContentL ( const TPtrC8& aContent, TBool aAppend /* = EFalse */ )
	{
	if ( aContent.Length () == 0 )
		{
		return;			
		}

	TInt len = aContent.Length () + iPartialContent->Des().Length();
	if ( iPartialContent->Des().MaxSize () < len )
		{
		iPartialContent = iPartialContent->ReAllocL ( len );			
		}

	
	if ( aAppend )
		{
		iPartialContent->Des().Append( aContent );		
		}
	else
		{
		*iPartialContent = aContent; // Partial text. Save the text.		
		}		
	}

/**
	Check the aTag ( if it is not empty ) in the tag array and will	remove it from the array.
	
	@param aTag [in] Tag value need to be checked.

	@return TBool ETrue Found and successfully removed.
 */
TBool CHtmlParser::CheckAndRemoveTagL ( const TPtrC8& aTag )
	{
	
	TInt count = iTagQueue->Count ();	
	if ( !count )
		{
		return EFalse;			
		}
	
	if ( !aTag.Length () )
		{
		// Remove the last tag added. 
		// For eg: <img src="picture.jpg"/> 
		// "img" tag has been added and need to remove it from the array.		
		TPtrC8 temp ( iTagQueue->MdcaPoint ( count - 1 ) );		
		iTagQueue->Delete ( count - 1 );		
		return ETrue;					
		}
		
	// Iterate through the tag array from end till it finds the tag.
	// Remove the elements that is mis-matching from the array. If the tag array 
	// count is zero and if it is unable to find the tag, then there is a tag
	// mis-match.		
	while ( count )
		{
		TBool found = ETrue;
		TPtrC8 temp ( iTagQueue->MdcaPoint ( count - 1 ) );
		if ( aTag.Compare ( temp ) )
			{
			// Tag mis-match. 
			found = EFalse;		
			CallEndElementL ( temp ); // no end tag. Client should know about this.						
			}			
		// Remove the tag.
		iTagQueue->Delete ( count - 1 );		
					
		if ( found )
			{
			return ETrue;					
			}
		count = iTagQueue->Count ();	
		}

	return EFalse; // Tag is not present. 				
	}

/**
	Creates RTagInfo & RAttributeArray and tell XML framework
	
	@param aTag [in] Tag value.
 */
void CHtmlParser::CallStartElementL ( const TDesC8& aTag )
	{	
	// Create tag info with the uri, localname and prefix values.
	RTagInfo tagInfo;	
	CreateTagInfoLC	( tagInfo, aTag );

	RAttributeArray attributes;
	CleanupStack::PushL( TCleanupItem( AttributeArrayDelete, &attributes ) );
	
	for ( TInt i  = 0; i < iTagAttribute->Count(); ++i )
		{
		RString nameString;
		RString valueString;
		RString uriString;
		RString prefixString;
		
		// Create RString's for attribute name/value		
		nameString = iStringPool.OpenStringL ( (*iTagAttribute)[i] );
		CleanupClosePushL ( nameString );
		valueString = iStringPool.OpenStringL ( (*iTagAttributeValue)[i] );
		CleanupClosePushL ( valueString );
		
		// For HTML there is no uri and prefix. So make it null
		uriString = iStringPool.OpenStringL( KNullDesC8 );
		CleanupClosePushL ( uriString );
		prefixString = iStringPool.OpenStringL( KNullDesC8 );
		CleanupClosePushL ( prefixString );
		
		RAttribute attribute;
		attribute.Open( uriString, prefixString, nameString, valueString );		

		CleanupStack::Pop ( 4 ); 
		CleanupClosePushL(attribute);
		
		User::LeaveIfError(attributes.Append(attribute));
		
		CleanupStack::Pop();
		}
	

	// Tell the framework.
	iContentHandler->OnStartElementL ( tagInfo, attributes, KErrNone );
	CleanupStack::PopAndDestroy ( &attributes );
	CleanupStack::PopAndDestroy ( &tagInfo ); 
	}

/**
	Creates RTagInfo and tells the framework.
	
	@param aTag [in] Tag value.
 */
void CHtmlParser::CallEndElementL ( const TDesC8& aTag )
	{
	
	if ( !aTag.Length () )
		{
		return;			
		}

	// Create tag info with the uri, localname and prefix values.
	RTagInfo tagInfo;	

	CreateTagInfoLC( tagInfo, aTag );
	
	// Tell the framework
	iContentHandler->OnEndElementL ( tagInfo, KErrNone );
	
	CleanupStack::PopAndDestroy ( &tagInfo ); 	
	}

/**
	Creates RDocumentParams and tells the framework.	
 */
void CHtmlParser::CallStartDocumentL ( const TDesC8& aCharset )
	{
	RString encodingString = iStringPool.OpenStringL ( aCharset );
	CleanupClosePushL ( encodingString );
	
	RDocumentParameters docparams;	
	
	docparams.Open ( encodingString );
	CleanupStack::Pop (); // pop encodingString
	CleanupClosePushL ( docparams );
	
	// Tell the framework.
	iContentHandler->OnStartDocumentL( docparams, KErrNone );
	
	CleanupStack::PopAndDestroy ();
	}

void CHtmlParser::CheckAndProcessLastOptionalTagL ( const TDesC8& aTag )
	{
	TInt count = iTagQueue->Count();
	// Tag queue is empty. Nothing to do.
	if ( !count )		
		{
		return;
		}
	
	TPtrC8 temp( iTagQueue->MdcaPoint ( count - 1 ) );
	
	if ( !temp.Compare ( aTag ) )
		{		
		CallEndElementL ( aTag );		//  Optional tag without an end tag. Should be closed.
		iTagQueue->Delete( count  - 1 );// delete it from the array.
		}
	}

TBool CHtmlParser::IsOptionalTagL ( const TDesC8& aTag )
	{
	for ( TInt i = HtmlTagsStringTable::EBody; i < HtmlTagsStringTable::EArea; ++i )
		{
		RString string = iStringPool.String( i, HtmlTagsStringTable::Table );
		CleanupClosePushL ( string );
		TInt result = aTag.Compare ( string.DesC() );
		CleanupStack::PopAndDestroy ( 1 );
		if ( !result )
			{
			return ETrue;
			}
		}
	return EFalse;
	}

TBool CHtmlParser::IsForbiddenTagL ( const TDesC8& aTag )
	{

	for ( TInt i = HtmlTagsStringTable::EArea; i <= HtmlTagsStringTable::EParam; ++i )
		{
		RString string = iStringPool.String( i, HtmlTagsStringTable::Table );
		CleanupClosePushL ( string );
		TInt result = aTag.Compare ( string.DesC() );
		CleanupStack::PopAndDestroy ( 1 );
		if ( !result )
			{
			return ETrue;			
			}

		}
	return EFalse;
	}

void CHtmlParser::CheckAndProcessForbiddenTagL ( const TDesC8& aTag )
	{
	TInt count = iTagQueue->Count();
	if ( count )		
		{
		CallEndElementL ( aTag );		//  Forbidden tag without an end tag. Should be closed.
		iTagQueue->Delete( count  - 1 );// 	delete it from the array.	
		}		
	}


void CHtmlParser::ExtractCharsetL ( const TDesC8& aContent, TPtrC8& aCharset, TBool& aXMLFound )
	{
	aXMLFound = EFalse;
	TBool meta = ETrue;
	TInt pos = aContent.FindF ( KMetaName );		
	if ( pos == KErrNotFound )
		{
		pos = aContent.FindF ( KXMLName );
		if ( pos == KErrNotFound )
			{
			User::Leave ( pos );
			}		
		meta = EFalse;
		aXMLFound = ETrue;
		}
				
	TPtrC8 contentPtr ( aContent.Mid ( pos ) );	
	ExtractCharsetValueL ( contentPtr, meta ? _L8 ( "charset" ) : _L8( "encoding" ), aCharset );
	}

/**
	Extract the charset value from the HTML content. This function will be called only
	if the document is of HTML type. Function will leave if is unable to find "charset" or "encoding".
 
 */
void CHtmlParser::ExtractCharsetValueL ( const TDesC8& aContent, const TDesC8& aSearchValue, TPtrC8& aCharset )
	{
	
	TInt pos = aContent.FindF ( aSearchValue );
	if ( pos == KErrNotFound )
		{
		User::Leave ( pos );
		}
		
	TPtrC8 contentPtr ( aContent.Mid ( pos ) );
	CStringParser* parser = CStringParser::NewLC ( contentPtr );
	TPtrC8 value;
	
	if ( parser->ParseTill ( value, '=' ) )	
		{
		parser->SkipLength ( 1 ); // skip '='
		TChar ch;
		parser->GetCurrentCharacter ( ch );
		if ( ch == '\"' )
			{
			parser->SkipLength ( 1 ); // skip '\"'	
			}
		
		parser->ParseTill ( aCharset, _L8 ("\"\r\n" ) );		
		}
	else
		{
		CleanupStack::PopAndDestroy ( parser );
		User::Leave ( pos );
		}				

	CleanupStack::PopAndDestroy ( parser );								
	}	

LOCAL_C void AttributeArrayDelete( TAny* aPtr )
	{
	RAttributeArray& attributes = *( RAttributeArray* )aPtr;

	TInt nAttributes = attributes.Count();
	for( TInt i=0; i < nAttributes; ++i )
		{
		attributes[i].Close();
		}		
	attributes.Close();
	}
	



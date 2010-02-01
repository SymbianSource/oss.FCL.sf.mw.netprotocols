// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <e32test.h>
#include <f32file.h>
#include <stringpool.h>
#include <xml/documentparameters.h>
#include <xml/taginfo.h>
#include <xml/attribute.h>
#include "contenthandlers.h"
#include "htmltagstable.h"

using namespace Xml;

GLREF_D RTest test;


//
// TRebuildingContentHandler
//
TRebuildingContentHandler::TRebuildingContentHandler(RFile& aOut)
	: iError(KErrNone), iOutFile(aOut)
	{
	}

void TRebuildingContentHandler::OnStartDocumentL(const RDocumentParameters& aDocParam, TInt aErrorCode)
	{
	User::LeaveIfError(aErrorCode);

	iOutFile.Write(_L8("<?xml encoding=\""));
	iOutFile.Write(aDocParam.CharacterSetName().DesC());
	iOutFile.Write(_L8("\""));
	iOutFile.Write(_L8("?>\n"));
	}

void TRebuildingContentHandler::OnEndDocumentL(TInt aErrorCode)
	{
	User::LeaveIfError(aErrorCode);
	}
	
void TRebuildingContentHandler::OnStartElementL(const RTagInfo& aElement, const RAttributeArray& aAttributes, TInt aErrorCode)
	{
	User::LeaveIfError(aErrorCode);

	const TDesC8& localPart8 = aElement.LocalName().DesC();
	const TDesC8& prefix8 = aElement.Prefix().DesC();

	if(prefix8.Length())
		{
		iOutFile.Write(_L8("<"));
		iOutFile.Write(prefix8);
		iOutFile.Write(_L8(":"));
		iOutFile.Write(localPart8);
		}
	else
		{
		iOutFile.Write(_L8("<"));
		iOutFile.Write(localPart8);
		}	

	TInt nAttributes = aAttributes.Count();
	for(TInt i=0; i<nAttributes; i++)
		{
		const RAttribute& attribute = aAttributes[i];
		const RTagInfo& nameInfo = attribute.Attribute();

		const TDesC8& localPart8 = nameInfo.LocalName().DesC();
		const TDesC8& prefix8 = nameInfo.Prefix().DesC();
		const TDesC8& value8 = attribute.Value().DesC();

		if(prefix8.Length())
			{
			iOutFile.Write(_L8(" "));
			iOutFile.Write(prefix8);
			iOutFile.Write(_L8(":"));
			iOutFile.Write(localPart8);
			}
		else
			{
			iOutFile.Write(_L8(" "));
			iOutFile.Write(localPart8);
			}	

		iOutFile.Write(_L8("=\""));
		iOutFile.Write(value8);
		iOutFile.Write(_L8("\""));
		}
	
	if ( IsForbiddenTagL ( localPart8 ) )
		{
		iOutFile.Write(_L8(" /"));	
		}
		
	iOutFile.Write(_L8(">"));
	}

void TRebuildingContentHandler::OnEndElementL(const RTagInfo& aElement, TInt aErrorCode)
	{
	User::LeaveIfError(aErrorCode);


	const TDesC8& localPart8 = aElement.LocalName().DesC();
	const TDesC8& prefix8 = aElement.Prefix().DesC();
	
	if ( IsForbiddenTagL ( localPart8 ) )
		{
		return; // The end tag has been already inserted for forbidden tags
		}


	iOutFile.Write(_L8("</"));

	if(prefix8.Length())
		{
		iOutFile.Write(prefix8);
		iOutFile.Write(_L8(":"));
		}
	iOutFile.Write(localPart8);
	iOutFile.Write(_L8(">"));
	}

void TRebuildingContentHandler::OnContentL(const TDesC8& aData8, TInt aErrorCode)
	{
	User::LeaveIfError(aErrorCode);
	
	//_LIT8(KGt, "&gt;");
	//_LIT8(KLt, "&lt;");
	//_LIT8(KAmp, "&amp;");
	//_LIT8(KAPos, "&apos;");
	//_LIT8(KQuot, "&quot;");

	TInt newLength = 0;
	TInt length = aData8.Length();

	TInt i;
	for(i=0; i<length; i++)
		switch(aData8[i])
			{
/*			
			case '>':
				newLength += K_gt8().Length();
				break;
			case '<':
				newLength += K_lt8().Length();
				break;
			case '&':
				newLength += K_amp8().Length();
				break;
			case '\"':
				newLength += K_quot8().Length();
				break;
			case '\'':
				newLength += K_apos8().Length();
				break;
*/
			default:
				newLength++;
			}

	TPtr8 out = HBufC8::NewLC(newLength)->Des();

	TChar c;
	for(i=0; i<length; i++)
		switch(c=aData8[i])
			{
/*
			case '>':
				out.Append(K_gt8);
				break;
			case '<':
				out.Append(K_lt8);
				break;
			case '&':
				out.Append(K_amp8);
				break;
			case '\"':
				out.Append(K_quot8);
				break;
			case '\'':
				out.Append(K_apos8);
				break;
*/
			default:
				out.Append(c);
			}

	iOutFile.Write(out);

	CleanupStack::PopAndDestroy(); // out
	}

void TRebuildingContentHandler::OnStartPrefixMappingL(const RString& /*aPrefix*/, const RString& /*aUri*/, TInt aErrorCode)
	{
	User::LeaveIfError(aErrorCode);
	}

void TRebuildingContentHandler::OnEndPrefixMappingL(const RString& /*aPrefix*/, TInt aErrorCode)
	{
	User::LeaveIfError(aErrorCode);
	}

void TRebuildingContentHandler::OnIgnorableWhiteSpaceL(const TDesC8& /*aBytes*/, TInt /*aErrorCode*/)
	{
	}

void TRebuildingContentHandler::OnSkippedEntityL(const RString& /*aName*/, TInt /*aErrorCode*/)
	{
	}

void TRebuildingContentHandler::OnProcessingInstructionL(const TDesC8& aTarget8, const TDesC8& aData8, TInt aErrorCode)
	{
	User::LeaveIfError(aErrorCode);

	iOutFile.Write(_L8("<?"));
	iOutFile.Write(aTarget8);
	iOutFile.Write(_L8(" "));
	iOutFile.Write(aData8);
	iOutFile.Write(_L8("?>\n"));
	}

void TRebuildingContentHandler::OnExtensionL(const RString& /*aData*/, TInt /*aToken*/, TInt /*aErrorCode*/)
	{
	// Meaningless in XML
	_LIT(KUnsupportedFeature,"Illegal call to TRebuildingContentHandler::OnExtensionL - aborting\n");
	test.Panic(KErrNotSupported, KUnsupportedFeature);
	}

void TRebuildingContentHandler::OnError(TInt aError)
	{
	iError = aError;
	test.Printf(_L("TRebuildingContentHandler::OnError - ERROR: code=%d - Aborting parsing process\n"), aError);
	}

TAny* TRebuildingContentHandler::GetExtendedInterface(const TInt32)
	{
	// do nothing.
	return NULL;
	}

void TRebuildingContentHandler::SetStringPoolL ( RStringPool& aPool )
	{
	iStringPool = &aPool;
	iStringPool->OpenL ( HtmlTagsStringTable::Table );
	}

TBool TRebuildingContentHandler::IsForbiddenTagL ( const TDesC8& aTag )
	{
	for ( TInt i = HtmlTagsStringTable::EArea; i <= HtmlTagsStringTable::EParam; ++i )
		{
		RString string = iStringPool->String( i, HtmlTagsStringTable::Table );
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
//
// TSimpleContentHandler
//
const TInt TSimpleContentHandler::KExpectedLeaveCode = 1234;

TSimpleContentHandler::TSimpleContentHandler()
	: iLeaveOnStartElement(EFalse), iNumElements(0), iNumSkippedEntities(0), iNumPrefixMappings(0), iNumPrefixUnmappings(0), 
		iError(KErrNone)
	{
	}

void TSimpleContentHandler::OnStartDocumentL(const RDocumentParameters&, TInt)
	{
	//test.Printf(_L("<!-- TSimpleContentHandler::OnStartDocumentL -->\n"));

	iNumElements = 0;
	iNumSkippedEntities = 0;
	iNumPrefixMappings = 0;
	iNumPrefixUnmappings = 0;
	}

void TSimpleContentHandler::OnEndDocumentL(TInt)
	{
	//test.Printf(_L("\n<!-- TSimpleContentHandler::OnEndDocumentL -->\n"));

	test(!iLeaveOnStartElement);
	}
	
void TSimpleContentHandler::OnStartElementL(const RTagInfo&, const RAttributeArray&, TInt)
	{
	// test.Printf(_L("<!-- TSimpleContentHandler::OnStartElementL -->\n"));

	if(iLeaveOnStartElement)
		if(iNumElements++ == 0)
			{
			//test.Printf(_L("TSimpleContentHandler::OnStartElementL: Simulating Leave with code %d\n"), KExpectedLeaveCode);
			User::Leave(KExpectedLeaveCode);
			}
		else
			test(0);

	iNumElements++;
	}

void TSimpleContentHandler::OnEndElementL(const RTagInfo&, TInt)
	{
	// test.Printf(_L("\n<!-- TSimpleContentHandler::OnEndElementL -->"));

	test(!iLeaveOnStartElement);
	}

void TSimpleContentHandler::OnContentL(const TDesC8&, TInt)
	{
	// test.Printf(_L("\n<!-- TSimpleContentHandler::OnContentL -->"));

	test(!iLeaveOnStartElement);
	}

void TSimpleContentHandler::OnProcessingInstructionL(const TDesC8&, const TDesC8&, TInt)
	{
	// test.Printf(_L("\n<!-- TSimpleContentHandler::OnProcessingInstructionL -->"));

	test(!iLeaveOnStartElement || iNumElements==0);
	}

void TSimpleContentHandler::OnOutOfData()
	{
	_LIT(KUnexpectedOutOfData,"Unexpected call to TSimpleContentHandler::OnOutOfData - aborting\n");
	test.Panic(KErrAbort, KUnexpectedOutOfData);
	}

void TSimpleContentHandler::OnError(TInt aError)
	{
	iError = aError;
	//test.Printf(_L("TSimpleContentHandler::OnError - ERROR: code=%d\n"), aError);
	}

TAny* TSimpleContentHandler::GetExtendedInterface(const TInt32)
	{
	// do nothing.
	return NULL;
	}
	
void TSimpleContentHandler::OnStartPrefixMappingL(const RString&, const RString&, TInt)
	{
	iNumPrefixMappings++;
	}

void TSimpleContentHandler::OnEndPrefixMappingL(const RString&, TInt)
	{
	test(!iLeaveOnStartElement);
	iNumPrefixUnmappings++;
	}

void TSimpleContentHandler::OnIgnorableWhiteSpaceL(const TDesC8&, TInt)
	{
	}

void TSimpleContentHandler::OnSkippedEntityL(const RString&, TInt)
	{
	iNumSkippedEntities++;
	}

void TSimpleContentHandler::OnExtensionL(const RString&, TInt, TInt)
	{
	// Meaningless in XML
	_LIT(KUnsupportedFeature,"Illegal call to TSimpleContentHandler::OnExtensionL - aborting\n");
	test.Panic(KErrNotSupported, KUnsupportedFeature);
	}

void TSimpleContentHandler::SetStringPoolL ( RStringPool& aPool )
	{
	iStringPool = &aPool;
	iStringPool->OpenL ( HtmlTagsStringTable::Table );
	}

TBool TSimpleContentHandler::IsForbiddenTagL ( const TDesC8& aTag )
	{
	for ( TInt i = HtmlTagsStringTable::EArea; i <= HtmlTagsStringTable::EParam; ++i )
		{
		RString string = iStringPool->String( i, HtmlTagsStringTable::Table );
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

//
// TNamespaceContentHandler
//
TNamespaceContentHandler::TNamespaceContentHandler(const TDesC8& aDefaultUri, const TDesC8& aElementPrefix,
		const TDesC8& aElementUri, const TDesC8& aAttributePrefix, const TDesC8& aAttributeUri)
	: TSimpleContentHandler(), iDefaultUri(aDefaultUri), iElementPrefix(aElementPrefix), iElementUri(aElementUri),
		iAttributePrefix(aAttributePrefix), iAttributeUri(aAttributeUri), iState(KNothingMapped)
	{
	}

void TNamespaceContentHandler::OnStartElementL(const RTagInfo& aElement, const RAttributeArray& aAttributes, TInt aError)
	{
	TSimpleContentHandler::OnStartElementL(aElement, aAttributes, aError);

	if(iState==KNothingMapped)
		test(aElement.Prefix().DesC().Length()==0 && aElement.Uri().DesC().Length()==0);
	else
		 if(aElement.Prefix().DesC().Length()==0)
			test(aElement.Uri().DesC()==iDefaultUri);
		else
			test(aElement.Prefix().DesC()==iElementPrefix && aElement.Uri().DesC()==iElementUri);

	if(aAttributes.Count())
		{
		const RTagInfo& attribute = aAttributes[0].Attribute();

		if(attribute.Prefix().DesC().Length()==0)
			test(attribute.Uri().DesC().Length()==0);
		else
			{
			test(iState!=KNothingMapped);
			test(attribute.Prefix().DesC()==iAttributePrefix && attribute.Uri().DesC()==iAttributeUri);
			}
		}
	}

void TNamespaceContentHandler::OnEndElementL(const RTagInfo& aElement, TInt aError)
	{
	TSimpleContentHandler::OnEndElementL(aElement, aError);

	if(iState==KNothingMapped)
		test(aElement.Prefix().DesC().Length()==0 && aElement.Uri().DesC().Length()==0);
	else
		if(aElement.Prefix().DesC().Length()==0)
			test(aElement.Uri().DesC()==iDefaultUri);
		else
			test(aElement.Prefix().DesC()==iElementPrefix && aElement.Uri().DesC()==iElementUri);
	}

void TNamespaceContentHandler::OnStartPrefixMappingL(const RString& aPrefix, const RString& aUri, TInt aError)
	{
	TSimpleContentHandler::OnStartPrefixMappingL(aPrefix, aUri, aError);

	TPtrC8 prefix = aPrefix.DesC();
	TPtrC8 uri = aUri.DesC();

	switch(iState)
		{
		case KNothingMapped:
			test(prefix.Length()==0 && uri==iDefaultUri);
			iState = KDefaultNsMapped;
			break;

		case KDefaultNsMapped:
			test(prefix==iElementPrefix && uri==iElementUri);
			iState = KElementNsMapped;
			break;

		case KElementNsMapped:
			test(prefix==iAttributePrefix && uri==iAttributeUri);
			iState = KAttributeNsMapped;
			break;

		case KAttributeNsMapped:
			test(0);
		}
	}

void TNamespaceContentHandler::OnEndPrefixMappingL(const RString& aPrefix, TInt aError)
	{
	TSimpleContentHandler::OnEndPrefixMappingL(aPrefix, aError);

	TPtrC8 prefix = aPrefix.DesC();

	switch(iState)
		{
		case KAttributeNsMapped:
			test(prefix==iAttributePrefix);
			iState = KElementNsMapped;
			break;

		case KElementNsMapped:
			test(prefix==iElementPrefix);
			iState = KDefaultNsMapped;
			break;

		case KDefaultNsMapped:
			test(prefix.Length()==0);
			iState = KNothingMapped;
			break;

		case KNothingMapped:
			test(0);
		}
	}



void TCapsContentHandler::OnStartElementL(const RTagInfo& aElement, const RAttributeArray& aAttributes, TInt aError)
	{
	TSimpleContentHandler::OnStartElementL(aElement, aAttributes, aError);

	// Test file should contain the following:
	//		<PREFIX:NAME xlmns:PREFIX="HTTP://UPPERCASE.COM" 
	//		xlmns:ATTPREFIX="HTTP://STILLUPPERCASE.COM" ATTPREFIX:ATTNAME="VALUE"/>

	// Expat should deliver it to us as:
	//		prefix, name, "HTTP://UPPERCASE.COM"
	//		attprefix, attname, "HTTP://STILLUPPERCASE.COM", "VALUE"

	_LIT8(KPrefix, "prefix");
	_LIT8(KName, "name");
	_LIT8(KUri, "HTTP://UPPERCASE.COM");
	_LIT8(KAttprefix, "attprefix");
	_LIT8(KAttname, "attname");
	_LIT8(KAtturi, "HTTP://STILLUPPERCASE.COM");
	_LIT8(KValue, "VALUE");

	test(aElement.Prefix().DesC() == KPrefix);
	test(aElement.LocalName().DesC() == KName);
	test(aElement.Uri().DesC() == KUri);

	test(aAttributes.Count() == 1);
	const RAttribute& attribute = aAttributes[0];
	const RTagInfo& nameInfo = attribute.Attribute();

	test(nameInfo.Prefix().DesC() == KAttprefix);
	test(nameInfo.LocalName().DesC() == KAttname);
	test(nameInfo.Uri().DesC() == KAtturi);
	test(attribute.Value().DesC() == KValue);
	}


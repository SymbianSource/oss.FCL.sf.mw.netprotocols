// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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

// Local includes
//
#include "XmlPars.h"

// System includes
//
#include <attrlut.h>
#include <cbnfnode.h>
#include <xmlelemt.h>
#include <cnode.h>
#include <xmllib.h>	// For the error codes

LOCAL_C TBool Compare(TInt aLength,const TText* aStringA,const TDesC& aStringB)
	{
	if(aLength==aStringB.Length())
		{
		const TText* stringB=aStringB.Ptr();
		while(aLength>=(TInt)(sizeof(TInt32)/sizeof(TText)))
			{
			if(*(TInt32*)stringB!=*(TInt32*)aStringA)
				return EFalse;
			stringB+=(sizeof(TInt32)/sizeof(TText));
			aStringA+=(sizeof(TInt32)/sizeof(TText));
			aLength-=(sizeof(TInt32)/sizeof(TText));
			}
		while(aLength>0)
			{
			if(*stringB++!=*aStringA++)
				return EFalse;
			aLength--;
			}
		return ETrue;
		}
	return EFalse;
	}

CXmlParser::CXmlParser(CAttributeLookupTable& aAttributeLUT) 
: CBNFParser(aAttributeLUT), iErrorCode(KErrNone), iDataGathering(EFalse)
	{
	}

CXmlParser* CXmlParser::NewL(CAttributeLookupTable& aAttributeLUT)
	{
	CXmlParser* self = new(ELeave) CXmlParser(aAttributeLUT);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return(self);
	}

CXmlParser::~CXmlParser()
	{
	__XML_LOG_ENTER(_L("CXmlParser::~CXmlParser()"));
	PreAttribute(*this);	// Clears up attribute variables
	PreExternalID(*this);
	delete iName;
	delete iCData;
	delete iDTDUrl;
	delete iDocType;
	delete iXmlVersionString;
	delete iDocRootName;
	__XML_LOG_RETURN;
	__XML_CLOSE_LOG;
	}

void CXmlParser::ConstructL()
	{
	__XML_OPEN_LOG(__LOG_WAP_FILE_NAME);
	__XML_LOG_ENTER(_L("CXmlParser::CXmlParser()"));
	__XML_LOG_RETURN;
	}

void CXmlParser::ResetL(CXmlElement* aRootNode)
// Reseting method with which the parser can be prepared for parsing another document
	{
	__XML_LOG_ENTER(_L("CXmlParser::ResetL()"));
	iRootNode = aRootNode;
	iCurrentNode = iRootNode;
	ResetL();
	__XML_LOG_RETURN;
	}

void CXmlParser::ResetL()
// Inherited reseting method
	{
	__XML_LOG_ENTER(_L("CXmlParser::ResetL()"));
	CBNFParser::ResetL();

	PreAttribute(*this);
	PreExternalID(*this);
	iRootTagFound = EFalse;
	iDataGathering = EFalse;
	iErrorCode = KErrNone;
	delete iName;
	iName = NULL;
	delete iCData;
	iCData = NULL;
	delete iDTDUrl;
	iDTDUrl = NULL;
	delete iDocType;
	iDocType = NULL;
	delete iDocRootName;
	iDocRootName = NULL;
	delete iXmlVersionString;
	iXmlVersionString = NULL;
	__XML_LOG_RETURN;
	}

void CXmlParser::ContinueL()
// Continue parsing without adding data
	{
	__XML_LOG_ENTER(_L("CXmlParser::ContinueL()"));
	iErrorCode = KErrNone;
	if( !iDataGathering )
		{
		ParseL();
		}
	__XML_LOG_RETURN;
	}

void CXmlParser::ProcessDataL(HBufC8& aData)
// Parse incoming data. Data is copied.
	{
	__XML_LOG_ENTER(_L("CXmlParser::()"));
	HBufC* newData = HBufC::NewL(aData.Length());
	newData->Des().Copy(aData.Des());
	ProcessDataL(newData);
	__XML_LOG_RETURN;
	}

void CXmlParser::ProcessDataL(HBufC* aData)
// Process given data. Parser takes owneship of the data.
	{
	__XML_LOG_ENTER(_L("CXmlParser::ProcessDataL()"));
	EolNormalization(aData->Des());
	
	iString.AddStringL(aData);

	if( iRootNode == NULL )
		{
		iErrorCode = EWapErrXmlLibMissingDocumentRootNode;
		__XML_LOG(_L("ProcessDataL setting error code EWapErrXmlLibMissingDocumentRootNode"));
		__XML_LOG_RETURN;
		return;
		}
	if( !iDataGathering )
		{
		ParseL();
		}
	__XML_LOG_RETURN;
	}

void CXmlParser::CommitL()
// All the data has been passed in. Finish.
	{
	__XML_LOG_ENTER(_L("CXmlParser::CommitL()"));
	if( iDataGathering )
		{
		const HBufC* id = iLUT.Des2IDL(KXmlLibBufferedDocumentAttribute);
		CDataNoDelete* attributeValue = new(ELeave) CDataNoDelete(iString.ContentL());
		CleanupStack::PushL(attributeValue);
		iRootNode->AddAttributeL(id, attributeValue);
		CleanupStack::Pop(attributeValue);
		iErrorCode = EWapErrXmlLibDocumentBuffered;
		__XML_LOG(_L("CommitL setting error code EWapErrXmlLibDocumentBuffered"));
		__XML_LOG_RETURN;
		return;
		}
	CBNFParser::CommitL();
	__XML_LOG_RETURN;
	}

//
//	CALLBACK FUNCTIONS
//

// These methods are bound to specific rules and they are called when ever a rule finishes,
// regardless of the success of the matching of the rule.
// Functions, whose name start with "Post" are executed _after_ the rule and "Pre" rules
// are executed prior to the matching attempt of a rule.


void CXmlParser::PostCharDataL(CBNFParser& aParser)
// Handle "ordinary" character data. This generates #PCDATA nodes for the body text
// of the document. Nodes consisting only of white spaces are ignored.
	{
	__XML_EXT_LOG_ENTER(_L("CXmlParser::PostCharDataL()"));
	CXmlParser& parser = REINTERPRET_CAST(CXmlParser&, aParser);

	if( parser.RuleMatched() )
		{
		HBufC* data = parser.MarkedL();

		TInt i=0;
		TPtr text = data->Des();
		for(; i < data->Length(); i++)
			{
			TInt ch=text[i];
			if( !(ch == 0x9 || ch == 0xA || ch == 0xD || ch == 0x20)  )
				{
				break;
				}
			}
		if( i >= data->Length() )
			{
			delete data;
			parser.DeleteMark();
			__XML_EXT_LOG_RETURN;
			return;
			}

		parser.CharRefReplacement(text);

		CleanupStack::PushL(data);
		const HBufC* id = parser.iLUT.Des2IDL(KPCDataID);
		CXmlElement* newNode = CXmlElement::NewL(id, parser.iCurrentNode);
		parser.iCurrentNode->AppendChildL(newNode);  // Now the node is owned by the parent and would hence get deleted in a leave
		newNode->SetDataL(data);
		CleanupStack::Pop();
		}
	parser.DeleteMark();
	__XML_EXT_LOG_RETURN;
	}

void CXmlParser::PostCDStart(CBNFParser& aParser)
// Initialize matching of CDATA sections
	{
	__XML_EXT_LOG_ENTER(_L("CXmlParser::PostCDStart()"));
	CXmlParser& parser = REINTERPRET_CAST(CXmlParser&, aParser);

	if( parser.RuleMatched() )
		{
		delete parser.iCData;
		parser.iCData = NULL;
		}
	__XML_EXT_LOG_RETURN;
	}

void CXmlParser::PostCDataL(CBNFParser& aParser)
// Store the matched text withing CDATA section
	{
	__XML_EXT_LOG_ENTER(_L("CXmlParser::PostCData()"));
	CXmlParser& parser = REINTERPRET_CAST(CXmlParser&, aParser);

	if( parser.RuleMatched() )
		{
		parser.iCData = parser.MarkedL();
		}
	parser.DeleteMark();
	__XML_EXT_LOG_RETURN;
	}

void CXmlParser::PostCDSectL(CBNFParser& aParser)
// If whole of the CDATA section matches, a CDATA node is generated
// and the data is attached to that node. In other cases the document
// is in error.
	{
	__XML_EXT_LOG_ENTER(_L("CXmlParser::PostCDSectL()"));
	CXmlParser& parser = REINTERPRET_CAST(CXmlParser&, aParser);

	if( parser.RuleMatched() )
		{
		const HBufC* id = parser.iLUT.Des2IDL(KCDataID);
		CXmlElement* newNode = CXmlElement::NewL(id, parser.iCurrentNode);
		parser.iCurrentNode->AppendChildL(newNode);
		newNode->SetDataL(parser.iCData);
		parser.iCData = NULL;
		}
	else if( parser.iCData != NULL )
		{
		delete parser.iCData;
		parser.iCData = NULL;
		parser.iErrorCode = EWapErrXmlLibMissingCDATASectionEndTag;
		parser.SetState(EStopped);
		}
	__XML_EXT_LOG_RETURN;
	}


void CXmlParser::PostNameL(CBNFParser& aParser)
// Store string matching the Name -rule
	{
	__XML_EXT_LOG_ENTER(_L("CXmlParser::PostName()"));
	CXmlParser& parser = REINTERPRET_CAST(CXmlParser&, aParser);

	if( parser.RuleMatched() )
		{
		delete parser.iName;	// Get rid of the possibly errorneous, leftover name!
		parser.iName = parser.MarkedL();
		}
	parser.DeleteMark();
	__XML_EXT_LOG_RETURN;
	}

void CXmlParser::PostNodeNameL(CBNFParser& aParser)
// Wrapper function for storing the matched Name string as a name of a Node.
	{
	__XML_EXT_LOG_ENTER(_L("CXmlParser::PostNodeNameL()"));
	CXmlParser& parser = REINTERPRET_CAST(CXmlParser&, aParser);

	if( parser.RuleMatched() )
		{
		// Validity constraint: root element name must match the Name declared in DOCTYPE
		if( parser.iCurrentNode == parser.iRootNode )
			{
			if( parser.iDocRootName == NULL )
				{
				parser.iErrorCode = EWapErrXmlLibInvalidDocumentStructure;
				parser.SetState(EStopped);
				return;	
				}
			if( parser.iDocRootName->Compare(parser.iName->Des()) != 0 )
				{
				parser.iErrorCode = EWapErrXmlLibRootElementNameMismatch;
				parser.SetState(EStopped);
				return;
				}
			}
		const HBufC* type = parser.iLUT.Des2IDL(parser.iName->Des());
		parser.iCurrentNode->SetType(type);
		delete parser.iName;
		parser.iName = NULL;
		}
	__XML_EXT_LOG_RETURN;
	}

void CXmlParser::PreAttribute(CBNFParser& aParser)
// Prepare for matching an attribute definition (attrName = "attrValue")
	{
	__XML_EXT_LOG_ENTER(_L("CXmlParser::PreAttribute()"));
	CXmlParser& parser = REINTERPRET_CAST(CXmlParser&, aParser);

	delete parser.iAttrName;
	delete parser.iAttValue;
	parser.iAttrName = parser.iAttValue = NULL;
	__XML_EXT_LOG_RETURN;
	}

void CXmlParser::PostAttrName(CBNFParser& aParser)
// Store the matched Name-string as attribute name
	{
	__XML_EXT_LOG_ENTER(_L("CXmlParser::PostAttrName()"));
	CXmlParser& parser = REINTERPRET_CAST(CXmlParser&, aParser);

	if( parser.RuleMatched() )
		{
		parser.iAttrName = parser.iName;
		parser.iName = NULL;
		}
	__XML_EXT_LOG_RETURN;
	}

void CXmlParser::PostPureAttValueL(CBNFParser& aParser)
// Store the matched string as attribute value
	{
	__XML_EXT_LOG_ENTER(_L("CXmlParser::PostPureAttValue()"));
	CXmlParser& parser = REINTERPRET_CAST(CXmlParser&, aParser);

	if( parser.RuleMatched() )
		{
		delete parser.iAttValue;
		parser.iAttValue = parser.MarkedL();
		}
	parser.DeleteMark();
	__XML_EXT_LOG_RETURN;
	}

void CXmlParser::PostAttributeL(CBNFParser& aParser)
// Should all the parts of an attribute definition match, an attribute
// is created for the current node.
	{
	__XML_EXT_LOG_ENTER(_L("CXmlParser::PostAttributeL()"));
	CXmlParser& parser = REINTERPRET_CAST(CXmlParser&, aParser);

	if( parser.RuleMatched() )
		{
		if(parser.iAttrName != NULL)
			{
			// Add the attribute
			if(parser.iAttValue != NULL)
				{
				TPtr attrValText = parser.iAttValue->Des();
				parser.CharRefReplacement(attrValText);
				parser.iCurrentNode->SetAttributeL(*parser.iAttrName, *parser.iAttValue, parser.iLUT);
				}
			else
				parser.iCurrentNode->SetAttributeL(*parser.iAttrName, KNullDesC, parser.iLUT);
			}
		delete parser.iAttValue;
		delete parser.iAttrName;
		parser.iAttrName = parser.iAttValue = NULL;
		}
	else if( parser.iAttValue != NULL || parser.iAttrName != NULL )
		{
		parser.iErrorCode = EWapErrXmlLibInvalidAttributeDeclaration;
		parser.SetStatus(EStopped);
		}
	__XML_EXT_LOG_RETURN;
	}

void CXmlParser::PostTagStartL(CBNFParser& aParser)
// A new tag start ("<") is matched, generate new node and attach it as child for its parent
	{
	__XML_EXT_LOG_ENTER(_L("CXmlParser::PostTagStartL()"));
	CXmlParser& parser = REINTERPRET_CAST(CXmlParser&, aParser);

	if( parser.RuleMatched() )
		{
		if( parser.iRootTagFound )
			{
			CXmlElement* newNode = CXmlElement::NewL(NULL, parser.iCurrentNode);
			CleanupStack::PushL(newNode);
			parser.iCurrentNode->AppendChildL(newNode);
			CleanupStack::Pop();
			parser.iCurrentNode = newNode;
			}
		else
			{
			parser.iRootTagFound = ETrue;
			}
		}
	__XML_EXT_LOG_RETURN;
	}

void CXmlParser::PostEmptyElemClose(CBNFParser& aParser)
// If we match a closing empty element (i.e. <tag /> ) then this tag has no children
// and current node pointer is moved up to the parent
	{
	__XML_EXT_LOG_ENTER(_L("CXmlParser::PostEmptyElemClose()"));
	CXmlParser& parser = REINTERPRET_CAST(CXmlParser&, aParser);

	if( parser.RuleMatched() )
		{
		parser.iCurrentNode = (CXmlElement *)parser.iCurrentNode->Parent();
		}
	__XML_EXT_LOG_RETURN;
	}

void CXmlParser::PostETag(CBNFParser& aParser)
// Post rule for an end tag. We check that the end tag name matches
// the name of the node pointer by iCurrentNode (if everything is fine and
// all the tags are properly opened and closed, the pointer should point
// to the node that opened this tag). 
	{
	__XML_EXT_LOG_ENTER(_L("CXmlParser::PostETag()"));
	CXmlParser& parser = REINTERPRET_CAST(CXmlParser&, aParser);

	if( parser.RuleMatched() )
		{
		TDesC* type = (TDesC*)parser.iCurrentNode->Type();
		if( type->Compare(*parser.iName) )
			{
			parser.iErrorCode = EWapErrXmlLibEndTagMismatch;
			parser.SetState(EStopped);
			}
		else
			{
			// This is the end tag for iCurrentNode
			if( parser.iCurrentNode != parser.iRootNode)
				{
				parser.iCurrentNode = (CXmlElement*)parser.iCurrentNode->Parent();
				}
			// Here we don't need to make a notification of the node being the document closing (e.g. </wml> )
			// tag, since our rule tree won't match any subsequent tags and this will result in
			// EWapErrXmlLibInvalidDocument on call to CommitL.
			}
		delete parser.iName;
		parser.iName = NULL;
		}
	__XML_EXT_LOG_RETURN;
	}

void CXmlParser::PreExternalID(CBNFParser& aParser)
// Prepare for matching an external ID
	{
	__XML_EXT_LOG_ENTER(_L("CXmlParser::PreExternalID()"));
	CXmlParser& parser = REINTERPRET_CAST(CXmlParser&, aParser);

	delete parser.iSystemLiteral;
	delete parser.iPubidLiteral;
	parser.iSystemLiteral = parser.iPubidLiteral = NULL;
	__XML_EXT_LOG_RETURN;
	}

void CXmlParser::PostPureSystemLiteralL(CBNFParser& aParser)
// Store the contents of the SystemLiteral
	{
	__XML_EXT_LOG_ENTER(_L("CXmlParser::PostPureSystemLiteral()"));
	CXmlParser& parser = REINTERPRET_CAST(CXmlParser&, aParser);

	if( parser.RuleMatched() )
		{
		parser.iSystemLiteral = parser.MarkedL();
		}
	parser.DeleteMark();
	__XML_EXT_LOG_RETURN;
	}

void CXmlParser::PostPubidLiteralL(CBNFParser& aParser)
// Post rule for PubidLiteral
	{
	__XML_EXT_LOG_ENTER(_L("CXmlParser::PostPubidLiteral()"));
	CXmlParser& parser = REINTERPRET_CAST(CXmlParser&, aParser);

	if( parser.RuleMatched() )
		{
		HBufC* data = parser.MarkedL();
		// Since this entire rule matched, there must be quotation marks in the beginning
		// and at the end of the string. Those are now removed.
		TPtr pureLiteral = data->Des();
		pureLiteral.Delete(pureLiteral.Length()-1,1);
		pureLiteral.Delete(0,1);
		parser.iPubidLiteral = data;
		}
	parser.DeleteMark();
	__XML_EXT_LOG_RETURN;
	}

void CXmlParser::PostDTDidAndUrl(CBNFParser& aParser)
// If this combination is successfully matched, we have a complete definition
// for a DTD. This is signaled to the client who then can provide us with the
// DTD tree is available.
	{
	__XML_EXT_LOG_ENTER(_L("CXmlParser::PostDTDidAndUrl()"));
	CXmlParser& parser = REINTERPRET_CAST(CXmlParser&, aParser);

	if( parser.RuleMatched() )
		{
		// NOTICE that PostDTDidAndUrl is a wrapper rule for an ExternalID and it is possible
		// for an ExternalID to define just a URL, without the doctype. Here we assign
		// the PubidLiteral to iDocType, which would in such case be NULL. Therefore the client
		// using these values must check whether the doctype is NULL.
		parser.iDocType = parser.iPubidLiteral;
		parser.iDTDUrl = parser.iSystemLiteral;
		parser.iPubidLiteral = parser.iSystemLiteral = NULL;
		parser.iErrorCode = EDTDDefinitionFound;
		parser.SetState(EPaused);
		}
	__XML_EXT_LOG_RETURN;
	}

void CXmlParser::PostCharReferenceValue(CBNFParser& aParser)
// If character reference value rule didn't match, the document is in error
// since, the beginning of a character reference (&#) did match
	{
	__XML_EXT_LOG_ENTER(_L("CXmlParser::PostCharReferenceValue()"));
	CXmlParser& parser = REINTERPRET_CAST(CXmlParser&, aParser);

	if( !parser.RuleMatched() )
		{
		parser.iErrorCode = EWapErrXmlLibInvalidCharacterReference;
		parser.SetStatus(EStopped);
		}
	__XML_EXT_LOG_RETURN;
	}

_LIT(KLitLt,"lt");
_LIT(KLitGt,"gt");
_LIT(KLitAmp,"amp");
_LIT(KLitApos,"apos");
_LIT(KLitQuot,"quot");

void CXmlParser::PostEntityRefL(CBNFParser& aParser)
// After matching the an entity reference we substitute it
// with the corresponding replacement text to the parsing text.
// If the entity isn't one of the XML obligatory values,
// the replacement text is retrieved from the DTD tree.
// The parsing continues from the point where the replacement
// text was inserted, not after it!
	{
	__XML_EXT_LOG_ENTER(_L("CXmlParser::PostEntityRefL()"));
	CXmlParser& parser = REINTERPRET_CAST(CXmlParser&, aParser);

	if( parser.RuleMatched() )
		{
		// The predefined entities (see XML specification, chapter 4.6)
		const TText* stringPtr=parser.iName->Ptr();
		TInt length=parser.iName->Length();
		const TDesC* str=NULL;
		if(Compare(length,stringPtr,KLitLt))
			str=&KPredefEntityLT;
		else if(Compare(length,stringPtr,KLitGt))
			str=&KPredefEntityGT;
		else if(Compare(length,stringPtr,KLitAmp))
			str=&KPredefEntityAMP;
		else if(Compare(length,stringPtr,KLitApos))
			str=&KPredefEntityAPOS;
		else if(Compare(length,stringPtr,KLitQuot))
			str=&KPredefEntityQUOT;
		HBufC* replacementText = NULL;
		if( str )
			replacementText = str->AllocL();
		if( replacementText == NULL )
			{
			const HBufC* id = parser.iLUT.Des2IDL(parser.iName->Des());
			CBNFNode* entityNode = REINTERPRET_CAST(CBNFNode*, parser.iDTD->Attribute(id) );
			if( entityNode != NULL )
				{
				// On entity nodes the only child should be an AND node with an EExact child that
				// that holds the replacement text data.
				CTypedNode<TInt,const TDesC*>* child=entityNode->Child(0);
				if( child != NULL && child->Type() == EExact )
					{
					replacementText = child->Data()->AllocL();
					}
				}
			}

		delete parser.iName;
		parser.iName = NULL;

		if( replacementText == NULL )
			{
			parser.iErrorCode = EWapErrXmlLibUnknownEntityReference;
			parser.SetState(EStopped);
			}

		parser.iString.ReplaceMarkedL(replacementText);
		}
	parser.DeleteMark();
	__XML_EXT_LOG_RETURN;
	}


void CXmlParser::PostVersionNumL(CBNFParser& aParser)
// Store XML version string
	{
	__XML_EXT_LOG_ENTER(_L("CXmlParser::PostVersionNum()"));
	CXmlParser& parser = REINTERPRET_CAST(CXmlParser&, aParser);

	if( parser.RuleMatched() )
		{
		parser.iXmlVersionString = parser.MarkedL();
		}
	parser.DeleteMark();
	__XML_EXT_LOG_RETURN;
	}

void CXmlParser::PostDoctypedecl(CBNFParser& aParser)
// Check after a doctype declaration that there must have been
// a DTD definition.
	{
	__XML_EXT_LOG_ENTER(_L("CXmlParser::PostDoctypedecl()"));
	CXmlParser& parser = REINTERPRET_CAST(CXmlParser&, aParser);

	if( parser.RuleMatched() )
		{
		if( parser.iDTDUrl == NULL )
			{
			parser.iErrorCode = ENoDTDDefined;
			parser.SetState(EPaused);
			}
		}
	__XML_EXT_LOG_RETURN;
	}

void CXmlParser::PostDocRootName(CBNFParser& aParser)
// Store the document name definition in the doctypedecl
// for later check, that the first tag has the same name
	{
	__XML_EXT_LOG_ENTER(_L("CXmlParser::PostDocRootName()"));
	CXmlParser& parser = REINTERPRET_CAST(CXmlParser&, aParser);

	if( parser.RuleMatched() )
		{
		parser.iDocRootName = parser.iName;
		parser.iName = NULL;
		}
	__XML_EXT_LOG_RETURN;
	}

void CXmlParser::PostProlog(CBNFParser& aParser)
// After the prolog part we can determine if we had wrong version
// of Xml defined, if we didn't get a DTD and stuff.
	{
	__XML_EXT_LOG_ENTER(_L("CXmlParser::PostProlog()"));
	CXmlParser& parser = REINTERPRET_CAST(CXmlParser&, aParser);

	if( parser.iDTD == NULL )
		{
		// No DTD, no show. We can not proceed, unless we have a DTD!
		parser.iErrorCode = EWapErrXmlLibNoDTD;
		parser.SetState(EStopped);
		}

	if( parser.iXmlVersionString == NULL ||
		parser.iXmlVersionString->Compare( KSupportedXmlVersion ) != 0 )
		{
		__XML_EXT_LOG1(_L("Looking for supported XML - \"%S\""), &KSupportedXmlVersion);

		if (parser.iXmlVersionString==NULL)
			{
			__XML_EXT_LOG1(_L("But version string is NULL"), NULL);
			}
		else
			{
			TBuf<256> buf;
			buf.Copy(*parser.iXmlVersionString);
			__XML_EXT_LOG1(_L("Found XMLVersionString \"%S\""), &buf);
			}
		parser.iErrorCode = EWapErrXmlLibInvalidXmlVersionDefinition;
		parser.SetState(EStopped);
		}

	delete parser.iXmlVersionString;
	parser.iXmlVersionString = NULL;

	// The mark set in the beginning of the document was to make sure that no data is deleted
	// until we know for sure that we have a DTD and need not to buffer the data.
	// We can now waste the already parsed data and that is allowed by removing the first
	// mark. This point can not be reached if data gathering was active.
	parser.DeleteMark();
	__XML_EXT_LOG_RETURN;
	}


// A macro to define literal strings that are used to name and reference the rule
// USAGE: _STRING(hello)
//		this would result in definition of a literal called Khello that has value "hello"
//		i.e. this equals to _LIT(Khello, "hello"); 
#define _STRING(X) _LIT(K ## X, #X);

CBNFNode* CXmlParser::TreeL()
// Builds the rule tree to parse XML
	{
	__XML_LOG_ENTER(_L("CXmlParser::TreeL()"));

	// save calling the exported function in CBNFNode dozens of times

	const TDesC* rangeStart=CBNFNode::KRangeStart();
	const TDesC* rangeEnd=CBNFNode::KRangeEnd();

	CBNFNode* root = NewBNFL();
	CleanupStack::PushL(root);

	_STRING(document);
	NewComponentL(root, *root, Kdocument);

//
//	Common "help rules"
//
	_STRING(S);
	// Optional whitespace sequence: S?
	_STRING(optS);
	CBNFNode& optS = NewRuleL(root, KoptS, EOptional, NULL, NULL, NULL);
	NewComponentL(root, optS, KS);

	// Single quote: '
	_STRING(singleQuote);
	NewRuleL(root, KsingleQuote, EExact, _L("'"), NULL, NULL);

	// Double quote
	_STRING(doubleQuote);
	NewRuleL(root, KdoubleQuote, EExact, _L("\""), NULL, NULL);

	_STRING(Char);

	// Number ::= [0-9]
	_STRING(Number);
	CBNFNode& Number = NewRuleL(root, KNumber, ERange, NULL, NULL, NULL);
	AddComponentAttributeL(Number, rangeStart, '0');
	AddComponentAttributeL(Number, rangeEnd, '9');

	// HexNumber ::= [0-9a-fA-f]
	_STRING(HexNumber);
	CBNFNode& HexNumber = NewRuleL(root, KHexNumber, EOr, NULL, NULL, NULL);
	NewComponentL(root, HexNumber, KNumber);
	CBNFNode& HexNumber2 = NewComponentL(HexNumber, ERange);
		AddComponentAttributeL(HexNumber2, rangeStart, 'a');
		AddComponentAttributeL(HexNumber2, rangeEnd, 'f');
	CBNFNode& HexNumber3 = NewComponentL(HexNumber, ERange);
		AddComponentAttributeL(HexNumber3, rangeStart, 'A');
		AddComponentAttributeL(HexNumber3, rangeEnd, 'F');

	// HexValue ::= HexNumber+
	_STRING(HexValue);
	CBNFNode& HexValue = NewRuleL(root, KHexValue, ENMore, NULL, NULL, NULL);
		AddComponentAttributeL(HexValue, CBNFNode::KNMoreMinimum(), 1);
	NewComponentL(root, HexValue, KHexNumber);

	// DecNumber ::= Digit+
	_STRING(DecValue);
	CBNFNode& DecValue = NewRuleL(root, KDecValue, ENMore, NULL, NULL, NULL);
		AddComponentAttributeL(DecValue, CBNFNode::KNMoreMinimum(), 1);
	NewComponentL(root, DecValue, KNumber);

	// Latin char: [a-zA-Z]
	_STRING(LatinChar);
	CBNFNode& LatinChar = NewRuleL(root, KLatinChar, EOr, NULL, NULL, NULL);
	CBNFNode& LatinChar1 = NewComponentL(LatinChar, ERange);
		AddComponentAttributeL(LatinChar1, rangeStart, 'A');
		AddComponentAttributeL(LatinChar1, rangeEnd, 'Z');
	CBNFNode& LatinChar2 = NewComponentL(LatinChar, ERange);
		AddComponentAttributeL(LatinChar2, rangeStart, 'a');
		AddComponentAttributeL(LatinChar2, rangeEnd, 'z');

//
//	XML rules
//
	_STRING(prolog);
	_STRING(element);
	_STRING(Misc);
	// XML specs rule [1]
	// [1] document ::= prolog element Misc*
	CBNFNode& document = NewRuleL(root, Kdocument, EAnd, NULL, NULL, NULL);
	NewComponentL( root, document, Kprolog );
	NewComponentL( root, document, Kelement );
	CBNFNode& document3 = NewComponentL( document, ENMore );
		NewComponentL( root, document3, KMisc);

	// XML specs rule [2]
	// [2] Char ::= #x9 | #xA | #xD | [#x20-#xD7FF] | [#xE000-#xFFFD] | [#x10000-#x10FFFF] 
	//
	// Notes:
	//	- Changed order of range 20-d7fff and selection of 9, 0xA and 0xD, since it is
	//	  we probably can make match faster with the first range
	//	- Removed the last range since we only support 16-bit chars
	//  - Implemented this as its own ruletype
	NewXmlRuleL(root, KChar, EChar);

	// XML specs rule [3]
	// [3] S ::= (#x20 | #x9 | #xD | #xA)+
	//  - Implemented this as its own ruletype
	NewXmlRuleL(root, KS, ES);

	_STRING(Letter);
	_STRING(FirstNameChar);
	NewXmlRuleL(root, KFirstNameChar, EFirstNameChar);
	_STRING(NMoreNameChar);
	NewXmlRuleL(root, KNMoreNameChar, ENMoreNameChar);
	// XML specs rule [5]
	// [5] Name ::= (Letter | '_' | ':') (NameChar)*
	_STRING(Name);
	CBNFNode& Name = NewRuleL(root, KName, EAnd, NULL, MarkCallback, PostNameL);
		NewComponentL(root, Name, KFirstNameChar);
		NewComponentL(root, Name, KNMoreNameChar);

	_STRING(Reference);
	// Help rule
	// Attribute value without the quote marks
	// PureAttValue ::= ([^<&"] | Reference)*
	_STRING(PureAttValueDQ);
	CBNFNode& PureAttValueDQ = NewRuleL(root, KPureAttValueDQ, ENMore, NULL, MarkCallback, PostPureAttValueL);
	CBNFNode& PureAttValueDQ1 = NewComponentL(PureAttValueDQ, EOr);
		NewComponentL(PureAttValueDQ1, ESelect, _L("^<&\"") );
		NewComponentL(root, PureAttValueDQ1, KReference);

	_STRING(PureAttValueSQ);
	CBNFNode& PureAttValueSQ = NewRuleL(root, KPureAttValueSQ, ENMore, NULL, MarkCallback, PostPureAttValueL);
	CBNFNode& PureAttValueSQ1 = NewComponentL(PureAttValueSQ, EOr);
		NewComponentL(PureAttValueSQ1, ESelect, _L("^<&'") );
		NewComponentL(root, PureAttValueSQ1, KReference);

		// XML specs rule [10]
	// [10] AttValue ::= '"' ([^<&"] | Reference)* '"' | "'" ([^<&'] | Reference)* "'"
	_STRING(AttValue);
	CBNFNode& AttValue = NewRuleL(root, KAttValue, EOr, NULL, NULL, NULL );
	CBNFNode& AttValue1 = NewComponentL(AttValue, EAnd);
		NewComponentL(root, AttValue1, KdoubleQuote);
		NewComponentL(root, AttValue1, KPureAttValueDQ);
		NewComponentL(root, AttValue1, KdoubleQuote);
	CBNFNode& AttValue2 = NewComponentL(AttValue, EAnd);
		NewComponentL(root, AttValue2, KsingleQuote);
		NewComponentL(root, AttValue2, KPureAttValueSQ);
		NewComponentL(root, AttValue2, KsingleQuote);

	// Help rule
	// System literal without the quotation marks
	// PureSystemLiteral ::= [^"]*
	_STRING(PureSystemLiteralDoubleQuote);
	CBNFNode& PureSystemLiteralDQ = NewRuleL(root, KPureSystemLiteralDoubleQuote, ENMore, NULL, MarkCallback, PostPureSystemLiteralL);
	NewComponentL(PureSystemLiteralDQ, ESelect, _L("^\"") );

	_STRING(PureSystemLiteralSingleQuote);
	CBNFNode& PureSystemLiteralSQ = NewRuleL(root, KPureSystemLiteralSingleQuote, ENMore, NULL, MarkCallback, PostPureSystemLiteralL);
	NewComponentL(PureSystemLiteralSQ, ESelect, _L("^'") );

	// XML specs rule [11]
	// [11] SystemLiteral ::= ('"' [^"]* '"') | ("'" [^']* "'")
	_STRING(SystemLiteral);
	CBNFNode& SystemLiteral = NewRuleL(root, KSystemLiteral, EOr, NULL, NULL, NULL);
	CBNFNode& SystemLiteral1 = NewComponentL(SystemLiteral, EAnd);
		NewComponentL(root, SystemLiteral1, KdoubleQuote);
		NewComponentL(root, SystemLiteral1, KPureSystemLiteralDoubleQuote);
		NewComponentL(root, SystemLiteral1, KdoubleQuote);
	CBNFNode& SystemLiteral2 = NewComponentL(SystemLiteral, EAnd);
		NewComponentL(root, SystemLiteral2, KsingleQuote);
		NewComponentL(root, SystemLiteral2, KPureSystemLiteralSingleQuote);
		NewComponentL(root, SystemLiteral2, KsingleQuote);

	_STRING(PubidChar);
	// XML specs rule [12]
	// [12] PubidLiteral ::= '"' PubidChar* '"' | "'" (PubidChar - "'")* "'"
	_STRING(PubidLiteral);
	CBNFNode& PubidLiteral = NewRuleL(root, KPubidLiteral, EOr, NULL, MarkCallback, PostPubidLiteralL);
	CBNFNode& PubidLiteral1 = NewComponentL(PubidLiteral, EAnd);
		NewComponentL(root, PubidLiteral1, KdoubleQuote);
		CBNFNode& PubidLiteral12 = NewComponentL(PubidLiteral1, ENMore);
			NewComponentL(root, PubidLiteral12, KPubidChar);
		NewComponentL(root, PubidLiteral1, KdoubleQuote);
	CBNFNode& PubidLiteral2 = NewComponentL(PubidLiteral, EAnd);
		NewComponentL(root, PubidLiteral2, KsingleQuote);
		CBNFNode& PubidLiteral22 = NewComponentL(PubidLiteral2, ENMore);
			CBNFNode& PubidLiteral221 = NewComponentL(PubidLiteral22, EWithout);
				NewComponentL(root, PubidLiteral221, KPubidChar);
				NewComponentL(PubidLiteral221, EExact, _L("'") );
		NewComponentL(root, PubidLiteral2, KsingleQuote);

	// XML specs rule [13]
	// [13] PubidChar ::= #x20 | #xD | #xA | [a-zA-Z0-9] | [-'()+,./:=?;!*#@$_%]
	CBNFNode& PubidChar = NewRuleL(root, KPubidChar, EOr, NULL, NULL, NULL);
	NewComponentL(PubidChar, ESelect, _L(" \r\n") );
	NewComponentL(root, PubidChar, KLatinChar);
	NewComponentL(root, PubidChar, KNumber);
	NewComponentL(PubidChar, ESelect, _L("-'()+,./:=?;!*#@$_%") );

	_STRING(CharRef);
	// XML specs rule [14]
	// [14] CharData ::= [^<&]* - ([^<&]* ']]>' [^<&]*)
	// valid CharData shall has length more than zero.
	// Also character references are included to the char data and replaced later.
	// CharData ::= (([^<&]+ - ([^<&]* ']]>' [^<&]*)) | CharRef)+
	//
	// Optimized:
	// CharData ::= ( [^<&]+ | CharRef )+
	_STRING(CharData);
	CBNFNode& CharData = NewRuleL(root, KCharData, ENMore, NULL, MarkCallback, PostCharDataL);
		AddComponentAttributeL(CharData, CBNFNode::KNMoreMinimum(), 1);
	CBNFNode& CharData1 = NewComponentL(CharData, EOr);
		CBNFNode& CharData11 = NewComponentL(CharData1, ENMore);
			AddComponentAttributeL(CharData11, CBNFNode::KNMoreMinimum(), 1);
			NewComponentL(CharData11, ESelect, _L("^<&") );
		NewComponentL(root, CharData1, KCharRef);

	// XML specs rule [15]
	// [15] Comment ::= "<!--" ((Char - '-') | ('-' (Char - '-')))* "-->"
	_STRING(Comment);
	CBNFNode& Comment = NewRuleL(root, KComment, EAnd, NULL, NULL, NULL);
	NewComponentL(Comment, EExact, _L("<!--"));
		CBNFNode& Comment0 = NewComponentL(Comment, ENMore);
			CBNFNode& Comment1 = NewComponentL(Comment0, EOr);
				CBNFNode& Comment11 = NewComponentL(Comment1, EWithout);
					NewComponentL(root, Comment11, KChar);
					NewComponentL(Comment11, EExact, _L("-"));
				CBNFNode& Comment12 = NewComponentL(Comment1, EAnd);
					NewComponentL(Comment12, EExact, _L("-"));
					CBNFNode& Comment122 = NewComponentL(Comment12, EWithout);
						NewComponentL(root, Comment122, KChar);
						NewComponentL(Comment122, EExact, _L("-"));
	NewComponentL(Comment, EExact, _L("-->"));

	_STRING(PITarget);
	// XML specs rule [16]
	// [16] PI ::= '<?' PITarget (S (Char* - (Char* '?>' Char*)))? '?>'
	// PIs are wasted, reformulated the rule:
	// PI ::= '<?' PITarget (Char - '?>')* '?>'
	_STRING(PI);
	CBNFNode& PI = NewRuleL(root, KPI, EAnd, NULL, NULL, NULL);
	NewComponentL(PI, EExact, _L("<?") );
	NewComponentL(root, PI, KPITarget);
	CBNFNode& PI4 = NewComponentL(PI, ENMore);
		CBNFNode& PI41 = NewComponentL(PI4, EWithout);
			NewComponentL(root, PI41, KChar);
			NewComponentL(PI41, EExact, _L("?>") );
	NewComponentL(PI, EExact, _L("?>") );

	// XML specs rule [17]
	// [17] PITarget ::= Name - (('X' | 'x') ('M' | 'm') ('L' | 'l'))
	CBNFNode& PITarget = NewRuleL(root, KPITarget, EWithout, NULL, NULL, NULL);
	NewComponentL(root, PITarget, KName);
	CBNFNode& PITarget2 = NewComponentL(PITarget, EAnd);
		NewComponentL(PITarget2, ESelect, _L("Xx") );
		NewComponentL(PITarget2, ESelect, _L("Mm") );
		NewComponentL(PITarget2, ESelect, _L("Ll") );

	_STRING(CDStart);
	_STRING(CData);
	_STRING(CDEnd);
	// XML specs rule [18]
	// [18] CDSect ::= CDStart CData CDEnd
	_STRING(CDSect);
	CBNFNode& CDSect = NewRuleL(root, KCDSect, EAnd, NULL, NULL, PostCDSectL);
	NewComponentL(root, CDSect, KCDStart);
	NewComponentL(root, CDSect, KCData);
	NewComponentL(root, CDSect, KCDEnd);

	// XML specs rule [19]
	// [19] CDStart ::= '<![CDATA['
	NewRuleL(root, KCDStart, EExact, _L("<![CDATA["), NULL, PostCDStart);

	// XML specs rule [20]
	// [20] CData ::= (Char* - (Char* ']]>' Char*))
	// Modified:
	// CData ::= (Char - ']]>')*
	CBNFNode& CData = NewRuleL(root, KCData, ENMore, NULL, MarkCallback, PostCDataL);
	CBNFNode& CData1 = NewComponentL(CData, EWithout);
		NewComponentL(root, CData1, KChar);
		NewComponentL(CData1, EExact, _L("]]>") );

	// XML specs rule [21]
	// [21] CDEnd ::= ']]>'
	NewRuleL(root, KCDEnd, EExact, _L("]]>"), NULL, NULL);

	_STRING(XMLDecl);
	_STRING(doctypedecl);
	// XML specs rule [22]
	// [22] prolog ::= XMLDecl? Misc* (doctypedecl Misc*)?
	CBNFNode& prolog = NewRuleL(root, Kprolog, EAnd, NULL, MarkCallback, PostProlog);
	CBNFNode& prolog1 = NewComponentL(prolog, EOptional);
		NewComponentL(root, prolog1, KXMLDecl);
	CBNFNode& prolog2 = NewComponentL(prolog, ENMore);
		NewComponentL(root, prolog2, KMisc);
	CBNFNode& prolog3 = NewComponentL(prolog, EOptional);
		CBNFNode& prolog31 = NewComponentL(prolog3, EAnd);
			NewComponentL(root, prolog31, Kdoctypedecl);
			CBNFNode& prolog312 = NewComponentL(prolog31, ENMore);
				NewComponentL(root, prolog312, KMisc);

	_STRING(VersionInfo);
	_STRING(EncodingDecl);
	_STRING(SDDecl);
	// XML specs rule [23]
	// [23] XMLDecl ::= '<?xml' VersionInfo EncodingDecl? SDDecl? S? '?>'
	CBNFNode& XMLDecl = NewRuleL(root, KXMLDecl, EAnd, NULL, NULL, NULL);
	NewComponentL(root, XMLDecl, KoptS);
	NewComponentL(XMLDecl, EExact, _L("<?xml") );
	NewComponentL(root, XMLDecl, KVersionInfo);
	CBNFNode& XMLDecl3 = NewComponentL(XMLDecl, EOptional);
		NewComponentL(root, XMLDecl3, KEncodingDecl);
	CBNFNode& XMLDecl4 = NewComponentL(XMLDecl, EOptional);
		NewComponentL(root, XMLDecl4, KSDDecl);
	NewComponentL(root, XMLDecl, KoptS);
	NewComponentL(XMLDecl, EExact, _L("?>") );

	_STRING(Eq);
	_STRING(VersionNum);
	// XML specs rule [24]
	// [24] VersionInfo ::= S 'version' Eq (' VersionNum ' | " VersionNum ")
	CBNFNode& VersionInfo = NewRuleL(root, KVersionInfo, EAnd, NULL, NULL, NULL);
	NewComponentL(root, VersionInfo, KS);
	NewComponentL(VersionInfo, EExact, _L("version") );
	NewComponentL(root, VersionInfo, KEq);
	CBNFNode& VersionInfo4 = NewComponentL(VersionInfo, EOr);
		CBNFNode& VersionInfo41 = NewComponentL(VersionInfo4, EAnd);
			NewComponentL(root, VersionInfo41, KsingleQuote);
			NewComponentL(root, VersionInfo41, KVersionNum);
			NewComponentL(root, VersionInfo41, KsingleQuote);
		CBNFNode& VersionInfo42 = NewComponentL(VersionInfo4, EAnd);
			NewComponentL(root, VersionInfo42, KdoubleQuote);
			NewComponentL(root, VersionInfo42, KVersionNum);
			NewComponentL(root, VersionInfo42, KdoubleQuote);

	// XML specs rule [25]
	// [25] Eq ::= S? '=' S?
	CBNFNode& Eq = NewRuleL(root, KEq, EAnd, NULL, NULL, NULL);
	NewComponentL(root, Eq, KoptS);
	NewComponentL(Eq, EExact, _L("=") );
	NewComponentL(root, Eq, KoptS);

	// XML specs rule [26]
	// [26] VersionNum ::= ([a-zA-Z0-9_.:] | '-')+
	CBNFNode& VersionNum = NewRuleL(root, KVersionNum, ENMore, NULL, MarkCallback, PostVersionNumL);
		AddComponentAttributeL(VersionNum, CBNFNode::KNMoreMinimum(), 1);
	CBNFNode& VersionNum1 = NewComponentL(VersionNum, EOr);
		CBNFNode& VersionNum11 = NewComponentL(VersionNum1, ERange);
			AddComponentAttributeL(VersionNum11, rangeStart, 'a');
			AddComponentAttributeL(VersionNum11, rangeEnd, 'z');
		CBNFNode& VersionNum12 = NewComponentL(VersionNum1, ERange);
			AddComponentAttributeL(VersionNum12, rangeStart, 'A');
			AddComponentAttributeL(VersionNum12, rangeEnd, 'Z');
		CBNFNode& VersionNum13 = NewComponentL(VersionNum1, ERange);
			AddComponentAttributeL(VersionNum13, rangeStart, '0');
			AddComponentAttributeL(VersionNum13, rangeEnd, '9');
		NewComponentL(VersionNum1, ESelect, _L("-_.:") );

	// XML specs rule [27]
	// [27] Misc ::= Comment | PI | S
	//
	// Notes:
	//	- Put S (whitespace) first, since it is the most probable match here
	CBNFNode& Misc = NewRuleL(root, KMisc, EOr, NULL, NULL, NULL);
	NewComponentL(root, Misc, KS);
	NewComponentL(root, Misc, KComment);
	NewComponentL(root, Misc, KPI);

	// Help rule
	// a rule, that can succesfully ignore a valid inline dtd
	// inlineDTD ::= ( S | Comment | ( <! ("[^"]*" | '[^']*' | Char-'>')* > ) | PI )*
	_STRING(inlineDTD);
	CBNFNode& inlineDTD = NewRuleL(root, KinlineDTD, EOr, NULL, NULL, NULL);
	CBNFNode& inlineDTD0 = NewComponentL(inlineDTD, ENMore);
	CBNFNode& inlineDTD1 = NewComponentL(inlineDTD0, EOr);
		NewComponentL(root, inlineDTD1, KS);
		NewComponentL(root, inlineDTD1, KComment);
		CBNFNode& inlineDTD12 = NewComponentL(inlineDTD1, EAnd);
			NewComponentL(inlineDTD12, EExact, _L("<!") );
			CBNFNode& inlineDTD122 = NewComponentL(inlineDTD12, ENMore);
				CBNFNode& inlineDTD1221 = NewComponentL(inlineDTD122, EOr);
					CBNFNode& inlineDTD12211 = NewComponentL(inlineDTD1221, EAnd);
						NewComponentL(root, inlineDTD12211, KdoubleQuote);
						CBNFNode& inlineDTD122112 = NewComponentL(inlineDTD12211, ENMore);
							NewComponentL(inlineDTD122112, ESelect, _L("^\"") );
						NewComponentL(root, inlineDTD12211, KdoubleQuote);
					CBNFNode& inlineDTD12212 = NewComponentL(inlineDTD1221, EAnd);
						NewComponentL(root, inlineDTD12212, KsingleQuote);
						CBNFNode& inlineDTD122122 = NewComponentL(inlineDTD12212, ENMore);
							NewComponentL(inlineDTD122122, ESelect, _L("^'") );
						NewComponentL(root, inlineDTD12212, KsingleQuote);
					CBNFNode& inlineDTD12213 = NewComponentL(inlineDTD1221, EWithout);
						NewComponentL(root, inlineDTD12213, KChar);
						NewComponentL(inlineDTD12213, EExact, _L(">") );
			NewComponentL(inlineDTD12, EExact, _L(">") );
		NewComponentL(root, inlineDTD1, KPI);

	_STRING(ExternalID);
	// Help rule
	// DTD doctype and url
	_STRING(DTDidAndUrl);
	CBNFNode& DTDidAndUrl = NewRuleL(root, KDTDidAndUrl, EAnd, NULL, NULL, PostDTDidAndUrl);
	NewComponentL(root, DTDidAndUrl, KExternalID);

	// Help rule
	// KDocRootName = Name
	_STRING(DocRootName);
	CBNFNode& docRootName = NewRuleL(root, KDocRootName, EAnd, NULL, NULL, PostDocRootName);
	NewComponentL(root, docRootName, KName);

	// XML specs rule [28]
	// [28] doctypedecl ::= '<!DOCTYPE' S Name (S ExternalID)? S? ('[' (markupdecl | PEReference | S)* ']' S?)? '>'
	// NOTE: part contained inside braces is not handled here, but is interpreted as
	//       inline DTD and shall be handled by the DTDDH. The actual form of this rule here is:
	// [28] doctypedecl ::= '<!DOCTYPE' S DocRootName (S DTDidAndUrl)? S? ('[' inlineDTD ']' S?)? '>'
	CBNFNode& doctypedecl = NewRuleL(root, Kdoctypedecl, EAnd, NULL, NULL, PostDoctypedecl);
	NewComponentL(doctypedecl, EExact, _L("<!DOCTYPE") );
	NewComponentL(root, doctypedecl, KS);
	NewComponentL(root, doctypedecl, KDocRootName);
	CBNFNode& doctypedecl4 = NewComponentL(doctypedecl, EOptional);
		CBNFNode& doctypedecl41 = NewComponentL(doctypedecl4, EAnd);
			NewComponentL(root, doctypedecl41, KS);
			NewComponentL(root, doctypedecl41, KDTDidAndUrl);
	NewComponentL(root, doctypedecl, KoptS);
	CBNFNode& doctypedecl6 = NewComponentL(doctypedecl, EOptional);
		CBNFNode& doctypedecl61 = NewComponentL(doctypedecl6, EAnd);
			NewComponentL(doctypedecl61, EExact, _L("[") );
			NewComponentL(root, doctypedecl61, KinlineDTD);
			NewComponentL(doctypedecl61, EExact, _L("]") );
			NewComponentL(root, doctypedecl61, KoptS);
	NewComponentL(doctypedecl, EExact, _L(">") );

	// Help rule
	// yes | no
	_STRING(YesOrNo);
	CBNFNode& YesOrNo = NewRuleL(root, KYesOrNo, EOr, NULL, NULL, NULL);
	NewComponentL(YesOrNo, EExact, _L("yes") );
	NewComponentL(YesOrNo, EExact, _L("no") );

	// XML specs rule [32]
	// [32] SDDecl ::= S 'standalone' Eq (("'" ('yes' | 'no') "'") | ('"' ('yes' | 'no') '"'))
	CBNFNode& SDDecl = NewRuleL(root, KSDDecl, EAnd, NULL, NULL, NULL);
	NewComponentL(root, SDDecl, KS);
	NewComponentL(SDDecl, EExact, _L("standalone") );
	NewComponentL(root, SDDecl, KEq);
	CBNFNode& SDDecl4 = NewComponentL(SDDecl, EOr);
		CBNFNode& SDDecl41 = NewComponentL(SDDecl4, EAnd);
			NewComponentL(root, SDDecl41, KsingleQuote);
			NewComponentL(root, SDDecl41, KYesOrNo);
			NewComponentL(root, SDDecl41, KsingleQuote);
		CBNFNode& SDDecl42 = NewComponentL(SDDecl4, EAnd);
			NewComponentL(root, SDDecl42, KdoubleQuote);
			NewComponentL(root, SDDecl42, KYesOrNo);
			NewComponentL(root, SDDecl42, KdoubleQuote);


	// Help rule
	// EmptyElemTag closing "/>"
	// EmptyElemClose ::= "/>"
	_STRING(EmptyElemClose);
	NewRuleL(root, KEmptyElemClose, EExact, _L("/>"), NULL, PostEmptyElemClose);

	_STRING(Tag);
	_STRING(content);
	_STRING(ETag);
	// XML specs rule [39]
	// [39] element ::= EmptyElemTag | STag content ETag 
	// For the sake of efficency and to make it possible to consume the matching
	// tokens immediately, we have rearranged this rule as follows:
	// element ::= Tag ( "/>" | '>' content ETag)
	CBNFNode& element = NewRuleL(root, Kelement, EAnd, NULL, NULL, NULL);
	NewComponentL(root, element, KTag);
	CBNFNode& element2 = NewComponentL(element, EOr);
		NewComponentL(root, element2, KEmptyElemClose);
		CBNFNode& element22 = NewComponentL(element2, EAnd);
			NewComponentL(element22, EExact, _L(">") );
			NewComponentL(root, element22, Kcontent);
			NewComponentL(root, element22, KETag);

	// Help rule
	// NodeName ::= Name
	// This is basically a wrapper for Name to be able to attach it to the current node
	_STRING(NodeName);
	CBNFNode& NodeName = NewRuleL(root, KNodeName, EOr, NULL, NULL, PostNodeNameL);
	NewComponentL(root, NodeName, KName);

	// Help rule
	// TagStart ::= '<'
	_STRING(TagStart);
	NewRuleL(root, KTagStart, EExact, _L("<"), NULL, PostTagStartL);

	_STRING(Attribute);
	// XML specs rule [40]
	// [40] STag ::= '<' Name (S Attribute)* S? '>' 
	// The ending '>' is left out to make this a common Tag for both the STag and
	// EmptyElemTag.
	// Tag ::= '<' NodeName (S Attribute)* S?
	CBNFNode& Tag = NewRuleL(root, KTag, EAnd, NULL, NULL, NULL);
	NewComponentL(root, Tag, KTagStart);
	NewComponentL(root, Tag, KNodeName);
	CBNFNode& Tag3 = NewComponentL(Tag, ENMore);
		CBNFNode& Tag31 = NewComponentL(Tag3, EAnd);
			NewComponentL(root, Tag31, KS);
			NewComponentL(root, Tag31, KAttribute);
	NewComponentL(root, Tag, KoptS);

	// Help rule
	// AttrName := Name
	_STRING(AttrName);
	CBNFNode& AttrName = NewRuleL(root, KAttrName, EOr, NULL, NULL, PostAttrName);
	NewComponentL(root, AttrName, KName);

	// XML specs rule [41]
	// [41] Attribute ::= Name Eq AttValue 
	CBNFNode& Attribute = NewRuleL(root, KAttribute, EAnd, NULL, PreAttribute, PostAttributeL);
	NewComponentL(root, Attribute, KAttrName);
	NewComponentL(root, Attribute, KEq);
	NewComponentL(root, Attribute, KAttValue);

	// XML specs rule [42]
	// [42] ETag ::= '</' Name S? '>'
	CBNFNode& ETag = NewRuleL(root, KETag, EAnd, NULL, NULL, PostETag);
	NewComponentL(ETag, EExact, _L("</") );
	NewComponentL(root, ETag, KName);
	NewComponentL(root, ETag, KoptS);
	NewComponentL(ETag, EExact, _L(">") );

	_STRING(EntityRef);
	// XML specs rule [43]
	// [43] content ::= (element | CharData | Reference | CDSect | PI | Comment)*
	// To make it work properly, the rule is rearranged. Now we can immediately consume
	// matching patterns.
	// content ::= (EntityRef | CDSect | PI | Comment | (element - "</") | CharData)*
	CBNFNode& content = NewRuleL(root, Kcontent, ENMore, NULL, NULL, NULL);
	CBNFNode& content1 = NewComponentL(content, EOr);
		NewComponentL(root, content1, KEntityRef);
		NewComponentL(root, content1, KCDSect);
		NewComponentL(root, content1, KPI);
		NewComponentL(root, content1, KComment);
		// This prevents the element matching to ETag!
		CBNFNode& content14 = NewComponentL(content1, EWithout);
			NewComponentL(root, content14, Kelement);
			NewComponentL(content14, EExact, _L("</") );
		NewComponentL(root, content1, KCharData);

	// Help rules
	// For hex and decimal character reference values

	// HexCharRefVal = [0-9a-fA-F]+ ';'
	_STRING(HexCharRefValue);
	CBNFNode& HexCharRefVal = NewRuleL(root, KHexCharRefValue, EAnd, NULL, NULL, PostCharReferenceValue);
		NewComponentL(root, HexCharRefVal, KHexValue);
		NewComponentL(HexCharRefVal, EExact, _L(";") );

	// DecCharRefVal = [0-9]+ ';'
	_STRING(DecCharRefValue);
	CBNFNode& DecCharRefVal = NewRuleL(root, KDecCharRefValue, EAnd, NULL, NULL, PostCharReferenceValue);
		NewComponentL(root, DecCharRefVal, KDecValue);
		NewComponentL(DecCharRefVal, EExact, _L(";") );

	// XML specs rule [66]
	// [66] CharRef ::= '&#' [0-9]+ ';' | '&#x' [0-9a-fA-F]+ ';' 
	// Swapped places for hex and dec reference to improve performance
	// CharRef ::= '&#x' [0-9a-fA-F]+ ';' | '&#' [0-9]+ ';'
	CBNFNode& CharRef = NewRuleL(root, KCharRef, EOr, NULL, NULL, NULL);
	CBNFNode& CharRef2 = NewComponentL(CharRef, EAnd);
		NewComponentL(CharRef2, EExact, _L("&#x") );
		NewComponentL(root, CharRef2, KHexCharRefValue);
	CBNFNode& CharRef1 = NewComponentL(CharRef, EAnd);
		NewComponentL(CharRef1, EExact, _L("&#") );
		NewComponentL(root, CharRef1, KDecCharRefValue);

	// XML specs rule [67]
	// [67] Reference ::= EntityRef | CharRef
	CBNFNode& Reference = NewRuleL(root, KReference, EOr, NULL, NULL, NULL);
	NewComponentL(root, Reference, KEntityRef);
	NewComponentL(root, Reference, KCharRef);

	// XML specs rule [68]
	// [68] EntityRef ::= '&' Name ';'
	CBNFNode& EntityRef = NewRuleL(root, KEntityRef, EAnd, NULL, MarkCallback, PostEntityRefL);
	NewComponentL(EntityRef, EExact, _L("&") );
	NewComponentL(root, EntityRef, KName);
	NewComponentL(EntityRef, EExact, _L(";") );

	// XML specs rule [75]
	// [75] ExternalID ::= 'SYSTEM' S SystemLiteral  | 'PUBLIC' S PubidLiteral S SystemLiteral
	CBNFNode& ExternalID = NewRuleL(root, KExternalID, EOr, NULL, PreExternalID, NULL);
	CBNFNode& ExternalID1 = NewComponentL(ExternalID, EAnd);
		NewComponentL(ExternalID1, EExact, _L("SYSTEM") );
		NewComponentL(root, ExternalID1, KS);
		NewComponentL(root, ExternalID1, KSystemLiteral);
	CBNFNode& ExternalID2 = NewComponentL(ExternalID, EAnd);
		NewComponentL(ExternalID2, EExact, _L("PUBLIC") );
		NewComponentL(root, ExternalID2, KS);
		NewComponentL(root, ExternalID2, KPubidLiteral);
		NewComponentL(root, ExternalID2, KS);
		NewComponentL(root, ExternalID2, KSystemLiteral);

	_STRING(EncName);
	// XML specs rule [80]
	// [80] EncodingDecl ::= S 'encoding' Eq ('"' EncName '"' | "'" EncName "'" )
	CBNFNode& EncodingDecl = NewRuleL(root, KEncodingDecl, EAnd, NULL, NULL, NULL );
	NewComponentL(root, EncodingDecl, KS);
	NewComponentL(EncodingDecl, EExact, _L("encoding") );
	NewComponentL(root, EncodingDecl, KEq);
	CBNFNode& EncodingDecl4 = NewComponentL(EncodingDecl, EOr);
		CBNFNode& EncodingDecl41 = NewComponentL(EncodingDecl4, EAnd);
			NewComponentL(root, EncodingDecl41, KdoubleQuote);
			NewComponentL(root, EncodingDecl41, KEncName);
			NewComponentL(root, EncodingDecl41, KdoubleQuote);
		CBNFNode& EncodingDecl42 = NewComponentL(EncodingDecl4, EAnd);
			NewComponentL(root, EncodingDecl42, KsingleQuote);
			NewComponentL(root, EncodingDecl42, KEncName);
			NewComponentL(root, EncodingDecl42, KsingleQuote);

	// XML specs rule [81]
	// [81] EncName ::= [A-Za-z] ([A-Za-z0-9._] | '-')* 
	CBNFNode& EncName = NewRuleL(root, KEncName, EAnd, NULL, NULL, NULL);
	NewComponentL(root, EncName, KLatinChar);
	CBNFNode& EncName2 = NewComponentL(EncName, ENMore);
		CBNFNode& EncName21 = NewComponentL(EncName2, EOr);
			NewComponentL(root, EncName21, KLatinChar);
			NewComponentL(root, EncName21, KNumber);
			NewComponentL(EncName21, ESelect, _L("-._") );

	// A modified Letter rule
	NewXmlRuleL(root, KLetter, ELetter);

	CleanupStack::Pop();
	__XML_LOG_RETURN;
	return(root);
	}

TBool CXmlParser::PerformRuleL(CBNFNode& aRule, CFragmentedString::TStringMatch& aMatched)
// Overriden perform rule method to implement our own rule types
	{
    TBool performedMatch;
    switch (aRule.Type())
        {
	case EChar:
		performedMatch = CharL(aMatched);
		break;
	case ELetter:
		performedMatch = LetterL(aMatched);
		break;
	case ES:
		performedMatch = SL(aMatched);
		break;
	case EFirstNameChar:
		performedMatch = FirstNameCharL(aMatched);
		break;
	case ENMoreNameChar:
		performedMatch = NMoreNameCharL(aMatched);
		break;
    default:
		performedMatch = CBNFParser::PerformRuleL(aRule, aMatched);
		break;
        }
    return performedMatch;
    }

TBool CXmlParser::CharL(CFragmentedString::TStringMatch& aMatched)
// Implementation of the Char rule as its own type of rule
// [2] Char ::= #x9 | #xA | #xD | [#x20-#xD7FF] | [#xE000-#xFFFD]
    {
	aMatched = iString.MatchRange(0x20, 0xD7FF);
	if( aMatched == CFragmentedString::EMatch)
		{
		iString.ConsumeMatched();
		return ETrue;
		}
	else if( aMatched == CFragmentedString::EInsufficientData )
		return ETrue;

	aMatched = iString.MatchSelect(_L("\t\r\n"));

	if( aMatched != CFragmentedString::EMatch )
		aMatched = iString.MatchRange(0xe000, 0xfffd);

	if( aMatched == CFragmentedString::EMatch)
		iString.ConsumeMatched();

    return ETrue;
    }

TBool CXmlParser::LetterL(CFragmentedString::TStringMatch& aMatched)
// Implementation of the modified letter rule
// Letter ::= [0x41-0x5a] | [0x61-0x7a] | [0xc0-0xd6] | [0xd8-0xf6] | [0xf8-0xff] | [0x100-0xd7a3]
    {
	TInt low[6] = {0x41, 0x61, 0xc0, 0xd8, 0xf8, 0x100};
	TInt high[6] = {0x5a, 0x7a, 0xd6, 0xf6, 0xff, 0xd7a3};
	TInt i;
	for(i=0; i<6; ++i)
		{
		aMatched = iString.MatchRange(low[i], high[i]);
		if( aMatched == CFragmentedString::EMatch )
			{
			iString.ConsumeMatched();
			break;
			}
		else if( aMatched == CFragmentedString::EInsufficientData )
			break;
		}

    return ETrue;
    }

_LIT(KWhiteSpace," \t\n\r");
TBool CXmlParser::SL(CFragmentedString::TStringMatch& aMatched)
// Implementaion if the whitespace rule
// [3] S ::= (#x20 | #x9 | #xD | #xA)+
	{
	TBool foundWhiteSpace = EFalse;
	aMatched = iString.MatchSelect(KWhiteSpace());
	if( aMatched == CFragmentedString::EMatch )
		foundWhiteSpace = ETrue;
	while( aMatched == CFragmentedString::EMatch )
		{
		aMatched = iString.MatchSelect(KWhiteSpace());
		if( aMatched == CFragmentedString::EMatch)
			iString.ConsumeMatched();
		}
	if( foundWhiteSpace )
		{
		aMatched = CFragmentedString::EMatch;
		}

	return ETrue;;
	}

TBool CXmlParser::FirstNameCharL(CFragmentedString::TStringMatch& aMatched)
// Additional rule to speed up the check of the validity of the first
// character for a Name
// FirstNameChar ::= Letter | [_:]
	{
	LetterL(aMatched);
	if( aMatched == CFragmentedString::EInsufficientData )
		return ETrue;
	else if( aMatched != CFragmentedString::EMatch )
		aMatched = iString.MatchSelect(_L("_:"));

	if( aMatched == CFragmentedString::EMatch )
		iString.ConsumeMatched();

	return ETrue;
	}

TBool CXmlParser::NMoreNameCharL(CFragmentedString::TStringMatch& aMatched)
// Implementation of zero or more name characters rule
// NMoreNameChar ::= (Letter | [0-9] | [.-_:\x000B7])*
	{
	aMatched = CFragmentedString::EMatch;
	while( aMatched == CFragmentedString::EMatch )
		{
		LetterL(aMatched);
		if( aMatched == CFragmentedString::EInsufficientData )
			return ETrue;
		else if( aMatched != CFragmentedString::EMatch )
			aMatched = iString.MatchRange('0', '9');

		if( aMatched != CFragmentedString::EMatch )
			aMatched = iString.MatchSelect(_L(".-_:\x00B7"));

		if( aMatched == CFragmentedString::EMatch )
			iString.ConsumeMatched();
		}

	// We return every time EMatch, since if we are here trying to match this
	// rule the first character already matched and even if we don't the name
	// is still valid.
	aMatched = CFragmentedString::EMatch;

	return ETrue;
	}

//
//	UTILITY FUNCTIONS
//

void CXmlParser::CharRefReplacement(TPtr& aString)
// A method to replace character references to the given string with the referenced character
// This method does minimal set of checks since when this method is called the string has
// already been parsed and hence the references it contains are valid (this is ensured by the
// reference parsing rules). Therefore, this method should be called just before the data
// is being attached as an attribute value or as CDATA to a node. The reason why we don't replace
// the values during the parsing (i.e. when the values are checked) is that if the rule would
// later fail and the string would be re-parsed then the replaced characters might cause
// errorneous situations and unintended references to appear.
	{
	__XML_LOG_ENTER(_L("CXmlParser::CharRefReplacement()"));
	_LIT(KCharRefMatchingPattern, "*&#*;*");

	TPtrC examinable = aString;
	TInt examinableOffset = 0;
	TInt referenceOffset = examinable.Match(KCharRefMatchingPattern);
	while( referenceOffset != KErrNotFound )
		{
		TPtrC reference(examinable.Mid(referenceOffset));
		TInt referenceValueOffset = 2;
		TRadix system = EDecimal;
		if( reference[2] == 'x' )
			{
			referenceValueOffset++;
			system = EHex;
			}
		TLex valueString(reference.Mid(referenceValueOffset, reference.Locate(';')-referenceValueOffset) );
		TUint16 referenceValue = 32;	// just in case something fails, we shall insert space
		valueString.Val(referenceValue, system);
		aString.Delete(examinableOffset+referenceOffset, reference.Locate(';') );
		aString[examinableOffset+referenceOffset] = referenceValue;

		examinable.Set(aString.Mid(examinableOffset+referenceOffset+1));
		examinableOffset += referenceOffset+1;
		referenceOffset = examinable.Match(KCharRefMatchingPattern);
		}
	__XML_LOG_RETURN;
	} 

void CXmlParser::EolNormalization(TPtr aString)
// End-of-Line -normalization
// Changes 0xD chars to 0xA, combination of 0xD,0xA is reduced to 0xA
	{
	__XML_LOG_ENTER(_L("CXmlParser::EolNormalization()"));
	TInt offset;
	for( offset = aString.Locate(0xD); offset != KErrNotFound; offset=aString.Locate(0xD) )
		{
		if( aString.Length() > offset+1 && 
			aString[offset+1] == 0xA )
			{
			aString.Delete(offset,1);
			}
		else
			{
			aString[offset] = 0xA;
			}
		}
	__XML_LOG_RETURN;
	}

CBNFNode& CXmlParser::NewXmlRuleL(CBNFNode* aRootRule, const TDesC& aRuleName, TXmlParserNodeTypes aRuleType)
	{
	CBNFNode& newRule = NewRuleL(aRootRule, aRuleName, EAnd, NULL, NULL, NULL);
	newRule.SetType(aRuleType);
	return newRule;
	}

CBNFNode&  CXmlParser::NewXmlRuleL(CBNFNode* aRootRule, const TDesC& aRuleName, TXmlParserNodeTypes aRuleType, HBufC* aData, TRuleCallback* aPreRule, TRuleCallback* aPostRule)
	{
	CBNFNode& newRule = NewRuleL(aRootRule, aRuleName, EAnd, aData, aPreRule, aPostRule);
	newRule.SetType(aRuleType);
	return newRule;
	}

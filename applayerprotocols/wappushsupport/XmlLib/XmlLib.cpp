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
// XmlLib.h
// 
//

// Local includes
//
#include "XmlPars.h"
#include "XmlValid.h"

//	System includes
//
#include <attrlut.h>
#include <xmlelemt.h>
#include <wapattrdf.h>
#include <estatus.h>
#include <mwappluginsp.h>
#include <mframeworksp.h>
#include <xmllib.h>

//	METHOD IMPLEMENTATIONS
//

EXPORT_C CXmlLibrary::CXmlLibrary(MWapPluginSP& aPluginSP, CXmlElement* aRootNode)
: iPluginSP(aPluginSP), iRootNode(aRootNode)
	{
	}

EXPORT_C CXmlLibrary::~CXmlLibrary()
	{
	delete iParser;
	}

EXPORT_C CXmlLibrary* CXmlLibrary::NewL(MWapPluginSP& aPluginSP, CXmlElement* aRootNode)
	{
	CXmlLibrary* self = CXmlLibrary::NewLC(aPluginSP, aRootNode);
	CleanupStack::Pop(self);
	return(self);
	}

EXPORT_C CXmlLibrary* CXmlLibrary::NewLC(MWapPluginSP& aPluginSP, CXmlElement* aRootNode)
	{
	CXmlLibrary* self = new(ELeave) CXmlLibrary(aPluginSP, aRootNode);
	CleanupStack::PushL(self);
	self->ConstructL();
	return(self);
	}

EXPORT_C void CXmlLibrary::ConstructL()
	{
	}

EXPORT_C void CXmlLibrary::PrepareParserL()
// A utlity method to set up the parser for parsing
// Notice, that the iRootNode has to be set before this one is called!
	{
	iParser = CXmlParser::NewL(iPluginSP.AttributeLUT());
	iParser->ResetL(iRootNode);
	}

EXPORT_C void CXmlLibrary::ResetL(CXmlElement* aRootNode)
// Reset the library and parser for processing of new document
	{
	iRootNode = aRootNode;
	iDTD = NULL;
	if( iParser != NULL)
		{
		iParser->ResetL(iRootNode);
		}
	}

// WAP 1.2 Addition!
EXPORT_C TInt CXmlLibrary::ProcessDataL(TDesC8& aData)
	{
	HBufC8* newData = aData.AllocLC();
	TInt ret = ProcessDataL(*newData);
	CleanupStack::PopAndDestroy();	// newData
	return (ret);
	}

EXPORT_C TInt CXmlLibrary::ProcessDataL(HBufC8& aData)
// Method for processing the streaming data
	{
	if( iParser == NULL )
		{
		PrepareParserL();
		}
	iParser->ProcessDataL(aData);

	return( ExecuteDataProcessingL() );
	}

EXPORT_C TInt CXmlLibrary::CommitL()
// Method to inform the library that all the data has been passed in
// Performs finishing tasks for the parsing, checks if all the data was parsed
// correctly and if the DTD tree was available the validation is also performed!
	{
	TInt returnCode = CommitParserL();
	if( returnCode != KErrNone )
		{
		return( returnCode );
		}

	if( iDTD != NULL )
		{
		// iDTD contains the correct dtdRoot, call the protected method
		return( ExecuteValidateL(iDTD) );
		}

	return(KErrNone);
	}

EXPORT_C TInt CXmlLibrary::ValidateL(CBNFNode& aDTDRootNode)
// Service method for validation
	{
	CBNFNode* dtdTree = ExtractDTDTree(&aDTDRootNode);
	if( dtdTree == NULL )
		{
		// Check to see if we have buffered the data.
		const HBufC* id = iPluginSP.AttributeLUT().Des2IDL(KXmlLibBufferedDocumentAttribute);
		if (iRootNode->AttributeExists(id))
			{
			// Attribute exists - delete the data adn attribute
			CDataNoDelete* attribute = (CDataNoDelete*)iRootNode->CNode::Attribute((TAny*)id);
			HBufC* bufferedDocument = (HBufC*)attribute->Data();
			delete bufferedDocument;
			iRootNode->DeleteAttribute(id);
			}
		return( EWapErrXmlLibInvalidDTD );
		}
	return( ExecuteValidateL(dtdTree) );
	}

EXPORT_C TInt CXmlLibrary::ExecuteValidateL(CBNFNode* aDTDRootNode)
// Actual execution of validation. This method can be overridden by
// inheriting classes.
// The aDTDRootNode MUST contain the actual DTD tree, not the engine Root.
	{
	TInt returnValue = KErrNone;

	// Check if the document has already been validated
	const HBufC* validAttrId = iPluginSP.AttributeLUT().Des2IDL(KWAPDocumentValidStatusAttributeName);
	if( iRootNode->AttributeExists(validAttrId) )
		{
		return( EWapErrXmlLibDocumentAlreadyValid );
		}

	// If there was no DTD during the parsing the incoming data was buffered and
	// attached as an attribute to the document root node. Now we have received all
	// the data and have a DTD, so parse the document again!
	const HBufC* id = iPluginSP.AttributeLUT().Des2IDL(KXmlLibBufferedDocumentAttribute);
	if( iRootNode->AttributeExists(id) )
		{
		if( iDTD == NULL )
			{
			iDTD = aDTDRootNode;
			}
		if( iParser != NULL )
			{
			iParser->ResetL(iRootNode);
			}
		CDataNoDelete* attribute = (CDataNoDelete*)iRootNode->CNode::Attribute((TAny*)id);
		HBufC* bufferedDocument = (HBufC*)attribute->Data();
		iRootNode->DeleteAttribute(id);
		returnValue = ProcessDataL(bufferedDocument);
		if( returnValue == KErrNone )
			{
			returnValue = CommitParserL();
			}
		if(returnValue != KErrNone )
			{
			return(returnValue);
			}
		}
	// Validation of the parsed document
	CXmlValidator* validator = CXmlValidator::NewL(*aDTDRootNode, iPluginSP.AttributeLUT());
	CleanupStack::PushL(validator);
	returnValue = validator->ValidateTreeL(iRootNode);
	CleanupStack::PopAndDestroy(validator);

	// Should the validation finish without errors, mark the document validated 
	// by attaching an attribute to the root node
	if( returnValue == KErrNone )
		{
		iRootNode->SetAttributeL(KWAPDocumentValidStatusAttributeName, KNullDesC, iPluginSP.AttributeLUT());
		}	
	return( returnValue );
	}

TInt CXmlLibrary::ProcessDataL(HBufC16* aData)
// Internal utility function. Used when reparsing the buffered document.
// The attributes in the document nodes are 16-bit character data, hence we need a different interface.
	{
	if( iParser == NULL )
		{
		PrepareParserL();
		}
	iParser->ProcessDataL(aData);

	return( ExecuteDataProcessingL() );
	}

TInt CXmlLibrary::CommitParserL()
// Internal utility function for finishing off the parser and checking the errors, if any
	{
	// Finish off the parser
	iParser->SetStatus(EPluginComplete);
	iParser->CommitL();
	if( iParser->ErrorCode() != KErrNone )
		{
		return( iParser->ErrorCode() );
		}
	if( !iParser->Valid() )
		{
		return( EWapErrXmlLibInvalidDocument );
		}
	return( KErrNone );
	}

TInt CXmlLibrary::ExecuteDataProcessingL()
// A method to examine return codes from parser after a single
// parsing pass. Here we also initiate DTD fetched if the parser
// returns appropriate signals or we might activate the data gathering
// in the parser, if no DTD is available.
	{

	if( iParser->ErrorCode() == CXmlParser::EDTDDefinitionFound ||
		iParser->ErrorCode() == CXmlParser::ENoDTDDefined )
		{
		if( iDTD == NULL)
			{
			if( iParser->ErrorCode() == CXmlParser::EDTDDefinitionFound )
				{
				iDTD = (CBNFNode*)iPluginSP.PrepareDTDL(*iParser->DocType(), *iParser->DTDUrl(), (CDocumentNode*)iRootNode);  // DocType might be NULL!
				}
			else
				{
				iDTD = (CBNFNode*)iPluginSP.PrepareDTDL(*iDefaultDoctype, *iDefaultDTDUrl, (CDocumentNode*)iRootNode);
				}

			if( iDTD != NULL )
				{
				iDTD = ExtractDTDTree(iDTD);
				if( iDTD == NULL )
					{
					// The engine returned us its root to DTD tree, but this node didn't
					// contain the actual DTD tree, hence report an error
					return( EWapErrXmlLibInvalidDTD );
					}
				}
			}
		if( iDTD == NULL )
			{
			iParser->SetDataGathering(ETrue);
			}
		else
			{
			iParser->SetDTD(iDTD);
			}
		iParser->ContinueL();
		}
	return( iParser->ErrorCode() );
	}

CBNFNode* CXmlLibrary::ExtractDTDTree(CBNFNode* aDTDRoot)
// Internal utility function for extracting the actual DTD tree
// from the document root node provided by the engine.
	{
	if( aDTDRoot != NULL )
		return( (CBNFNode*)aDTDRoot->NextChild() );
	else
		return( NULL );
	}

// RESERVED FOR FUTURE USE
EXPORT_C void CXmlLibrary::CXmlLibrary_Reserved1()
	{
	}


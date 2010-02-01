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

#ifndef __XMLPARS_H__
#define __XMLPARS_H__

// System includes
//
#include <e32base.h>
#include <cbnfparser.h>

// Local includes
//
#include "XmlLog.h"

//	Forward class declarations
//
class CAttributeLookupTable;
class CBNFNode;
class CXmlElement;

//	DATA STRUCTURES
//

// Replacement strings for predefined XML entities
_LIT(KPredefEntityLT,	"&#60;");
_LIT(KPredefEntityGT,	"&#62;");
_LIT(KPredefEntityAMP,	"&#38;");
_LIT(KPredefEntityAPOS,	"&#39;");
_LIT(KPredefEntityQUOT,	"&#34;");

//	CXmlParser CLASS DEFINITION
//
//##ModelId=3B6679A00264
class CXmlParser : public CBNFParser
	{
public:	// Data type definitions
	// Positive error codes to signal clients about actions
	enum TSignals
		{
		EDTDDefinitionFound = 1,
		ENoDTDDefined
		};

	// Definition of our own rule types
	enum TXmlParserNodeTypes
		{
		EFirstXmlParserNodeType = ELastParserNodeType,
		EChar,
		ELetter,
		ES,
		EFirstNameChar,
		ENMoreNameChar,
		ELastXmlParserNodeType
		};

public:	// Methods

	//##ModelId=3B6679A10374
	static CXmlParser* NewL(CAttributeLookupTable& aAttributeLUT);
	//##ModelId=3B6679A10373
	virtual ~CXmlParser();

	// Ready the parser to take in a document. Must be called before any data passed
	// into ProcessDataL!
	// INPUT:
	// aRootNode - the to be root node of the document we are about to parse
	//##ModelId=3B6679A1035F
	void ResetL(CXmlElement* aRootNode);

	// Perform parsing for the incoming data
	// INPUT:
	// aData - buffer of text to be parsed. Parser makes a copy if the buffer.
	//##ModelId=3B6679A10337
	void ProcessDataL(HBufC8& aData);

	// Perform parsing for the incoming data
	// INPUT:
	// aData - buffer of text to be parser. Parser TAKES OWNERSHIP OF THE BUFFER!
	//##ModelId=3B6679A1034B
	void ProcessDataL(HBufC16* aData);

	// Signal, that all the data has been passed in. CommitL should be called when all
	// the data has been parsed.
	//##ModelId=3B6679A1032E
	void CommitL();

	// Continue parsing after returning with a signal that requires actions from the client.
	//##ModelId=3B6679A1032D
	void ContinueL(); // Just go on parsing after a stop or pause without adding data

	// Get the current error status of the parser
	// OUTPUT:
	// return TInt - an error code defined in XmlLib.h or KErrNone, if everything was Ok.
	//##ModelId=3B6679A10325
	inline TInt ErrorCode();

	// Get document type definitions string (if found)
	// OUTPUT:
	// const HBufC* - the document type
	//##ModelId=3B6679A10324
	inline const HBufC* DocType();

	// OUTPUT:
	// const HBufC* - Url string defining the location of external DTD document
	//##ModelId=3B6679A10323
	inline const HBufC* DTDUrl();

	// Detect status of data gathering in the parser. If no DTD was available when it was
	// requested, the parser enters a data gathering mode, in which it just buffers the incoming
	// data for further parsing.
	// OUTPUT:
	// TBool - ETrue, if data is currently being gathered, EFalse if parsing on normally
	//##ModelId=3B6679A1031A
	inline TBool DataGathering() const;

	// OUTPUT:
	// const CBNFNode* - The DTD tree root node
	//##ModelId=3B6679A10319
	inline const CBNFNode* DTD() const;

	// Setting data gathering mode either active or inactive
	// INPUT:
	// TBool aDataGathering - ETrue sets the data gathering active, EFalse disables it.
	//##ModelId=3B6679A10305
	inline void SetDataGathering(TBool aDataGathering);

	// Set a valid DTD for the parser to use
	// INPUT:
	// CBNFNode* aDTD - the DTD tree root node
	//##ModelId=3B6679A102E8
	inline void SetDTD(CBNFNode* aDTD);

protected:	// Methods

	//##ModelId=3B6679A10369
	virtual void ResetL();
	//##ModelId=3B6679A102E7
	virtual CBNFNode* TreeL();
	//##ModelId=3B6679A102D2
	CXmlParser(CAttributeLookupTable& aALUT);

	// Second phase c'tor. All allocation occurs in this function.
	//
	//##ModelId=3B6679A102C8
	void ConstructL();

	//
	// Tree building rule callback functions
	//
	//##ModelId=3B6679A102B4
	static void PostCharDataL(CBNFParser& aParser);
	//##ModelId=3B6679A102A0
	static void PostCDStart(CBNFParser& aParser);
	//##ModelId=3B6679A1028C
	static void PostCDataL(CBNFParser& aParser);
	//##ModelId=3B6679A10278
	static void PostCDSectL(CBNFParser& aParser);
	//##ModelId=3B6679A10264
	static void PostNameL(CBNFParser& aParser);
	//##ModelId=3B6679A10250
	static void PostNodeNameL(CBNFParser& aParser);
	//##ModelId=3B6679A1023C
	static void PreAttribute(CBNFParser& aParser);
	//##ModelId=3B6679A10228
	static void PostPureAttValueL(CBNFParser& aParser);
	//##ModelId=3B6679A10214
	static void PostAttrName(CBNFParser& aParser);
	//##ModelId=3B6679A101F6
	static void PostAttributeL(CBNFParser& aParser);
	//##ModelId=3B6679A101E2
	static void PostTagStartL(CBNFParser& aParser);
	//##ModelId=3B6679A101CE
	static void PostEmptyElemClose(CBNFParser& aParser);
	//##ModelId=3B6679A101BA
	static void PostETag(CBNFParser& aParser);
	//##ModelId=3B6679A101A6
	static void PostPureSystemLiteralL(CBNFParser& aParser);
	//##ModelId=3B6679A10192
	static void PostPubidLiteralL(CBNFParser& aParser);
	//##ModelId=3B6679A1017E
	static void PreExternalID(CBNFParser& aParser);
	//##ModelId=3B6679A1016A
	static void PostDTDidAndUrl(CBNFParser& aParser);
	//##ModelId=3B6679A10156
	static void PostCharReferenceValue(CBNFParser& aParser);
	//##ModelId=3B6679A10142
	static void PostEntityRefL(CBNFParser& aParser);
	//##ModelId=3B6679A1012E
	static void PostVersionNumL(CBNFParser& aParser);
	//##ModelId=3B6679A1011A
	static void PostDoctypedecl(CBNFParser& aParser);
	//##ModelId=3B6679A100FC
	static void PostDocRootName(CBNFParser& aParser);
	//##ModelId=3B6679A100E8
	static void PostProlog(CBNFParser& aParser);

	//	Internal utility functions
	//
	//##ModelId=3B6679A100D4
	void CharRefReplacement(TPtr& aString);
	//##ModelId=3B6679A100C0
	void EolNormalization(TPtr aString);

	// The following methods were implemented to speed up the parser
	// Some of the parsing rules were implemented as their own types of
	// rules and these methods are the handler functions for those types.	
	//##ModelId=3B6679A100A2
    virtual TBool PerformRuleL(CBNFNode& aRule, CFragmentedString::TStringMatch& aMatched);
	//##ModelId=3B6679A1008E
	TBool CharL(CFragmentedString::TStringMatch& aMatched);
	//##ModelId=3B6679A1007A
	TBool LetterL(CFragmentedString::TStringMatch& aMatched);
	//##ModelId=3B6679A10066
	TBool SL(CFragmentedString::TStringMatch& aMatched);
	//##ModelId=3B6679A10052
	TBool FirstNameCharL(CFragmentedString::TStringMatch& aMatched);
	//##ModelId=3B6679A1003E
	TBool NMoreNameCharL(CFragmentedString::TStringMatch& aMatched);
	//##ModelId=3B6679A0038F
	CBNFNode& NewXmlRuleL(CBNFNode* aRootRule, const TDesC& aRuleName, TXmlParserNodeTypes aRuleType);
	//##ModelId=3B6679A003C1
	CBNFNode& NewXmlRuleL(CBNFNode* aRootRule, const TDesC& aRuleName, TXmlParserNodeTypes aRuleType, HBufC* aData, TRuleCallback* aPreRule, TRuleCallback* aPostRule);

private:	// Attributes

	// Document tree root node from which down the tree shall be built
	//##ModelId=3B6679A0037D
	CXmlElement* iRootNode;	// NOT OWNED

	// Storage pointers for the DocType and DTD url
	//##ModelId=3B6679A00371
	HBufC* iDocType;
	//##ModelId=3B6679A0035F
	HBufC* iDTDUrl;

	// Parsing time pointer to the node that is currently being contstructed
	//##ModelId=3B6679A00353
	CXmlElement* iCurrentNode;

	// A flag denoting whether we have already found the first tag of the document.
	// Enables the first tag Name and attributes to be inserted to the given root node.
	//##ModelId=3B6679A00341
	TBool iRootTagFound;

	// Parsing time data storages. Used to temporarily store the matched components
	//##ModelId=3B6679A00335
	HBufC* iName;
	//##ModelId=3B6679A00323
	HBufC* iAttrName;
	//##ModelId=3B6679A00317
	HBufC* iAttValue;
	//##ModelId=3B6679A0030D
	HBufC* iCData;
	//##ModelId=3B6679A002FB
	HBufC* iSystemLiteral;
	//##ModelId=3B6679A002F1
	HBufC* iPubidLiteral;
	//##ModelId=3B6679A002E7
	HBufC* iXmlVersionString;
	//##ModelId=3B6679A002DB
	HBufC* iDocRootName;

	// An error code defining the failure reason, if something went wrong
	// Positive error code values are used to signal XmlLib about actions that should be taken
	//##ModelId=3B6679A002D1
	TInt iErrorCode;

	// DTD tree for entity replacement
	//##ModelId=3B6679A002BF
	CBNFNode* iDTD; // NOT OWNED

	// Data gathering mode, if we didn't get a DTD, then we need to gather the data in a buffer
	// and perform the parsing in the validation phase
	//##ModelId=3B6679A002B5
	TBool iDataGathering;
	__XML_DECLARE_LOG

	};

//	Inline functions
//
inline TInt CXmlParser::ErrorCode()
	{ 
	return(iErrorCode);
	}

inline const HBufC* CXmlParser::DocType()
	{ 
	return(iDocType);
	}

inline const HBufC* CXmlParser::DTDUrl()
	{ 
	return(iDTDUrl); 
	}

inline TBool CXmlParser::DataGathering() const
	{
	return(iDataGathering);
	}

inline void CXmlParser::SetDataGathering(TBool aDataGathering)
	{ 
	iDataGathering = aDataGathering; 
	}

inline const CBNFNode* CXmlParser::DTD() const
	{ 
	return(iDTD);
	}

inline void CXmlParser::SetDTD(CBNFNode* aDTD)
	{
	iDTD = aDTD; 
	}

#endif // __XMLPARS_H __

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
// CXmlLibrary is an interface class for a client to access the Xml parsing and validation
// services provided by the library. In general, CXmlLibrary should be concidered as a base
// class for a library which implements the language specific bits of an Xml-language parser.
// USAGE:
// - Construction of the class using NewL or NewL
// - Pass data to parsing by calling ProcessDataL.
// - When all data has been parsed, call CommitL
// 
//

#ifndef __XMLLIB_H__
#define __XMLLIB_H__

// System includes
//
#include <e32base.h>
#include <wapengstd.h>

//	CONSTANT DEFINITIONS
//

/**
ID for the root node attribute that contains the buffered document in case no DTD was available
@publishedAll
@deprecated
*/
_LIT(KXmlLibBufferedDocumentAttribute, "XmlLibBufferedDocument");

/**
The Xml version that XML currently parser supports
@publishedAll
@deprecated
*/
_LIT(KSupportedXmlVersion, "1.0");

/**
ID for node containing text data defined in a CDATA section (i.e. within <![CDATA[ .. ]]> )
@publishedAll
@deprecated
*/
_LIT(KCDataID, "CDATA");

/**
ID for "normal" text nodes
@publishedAll
@deprecated
*/
_LIT(KPCDataID, "#PCDATA");


// Forward class declarations
//
class CXmlElement;
class CXmlParser;
class CBNFNode;
class CAttributeLookupTable;
class MWapPluginSP;

//	CXmlLibrary
//
//##ModelId=3B6679A401C8
class CXmlLibrary : public CBase
/**
@publishedAll
@deprecated
*/
	{
public:	// Methods
	// Default constuctor
	// Sets default values values iMIMEType, iDefaultDoctype and iDefaultDTDUrl
	// An inheriting class should override this constructor and set the variables
	// with their correct values.
	//##ModelId=3B6679A402CD
	IMPORT_C virtual ~CXmlLibrary();
	//##ModelId=3B6679A402BA
	IMPORT_C static CXmlLibrary* NewL(MWapPluginSP& aPluginSP, CXmlElement* aRootNode);
	//##ModelId=3B6679A402AF
	IMPORT_C static CXmlLibrary* NewLC(MWapPluginSP& aPluginSP, CXmlElement* aRootNode);

	//##ModelId=3B6679A40287
	IMPORT_C virtual TInt ProcessDataL(TDesC8& aData);

	// Xml Parse given data and generate document tree to the given root node.
	// INPUT:
	//	aData - document text, or a piece of it
	// RETURN:
	//	TInt - an error code defined above, KErrNone if no errors.
	//##ModelId=3B6679A40291
	IMPORT_C virtual TInt ProcessDataL(HBufC8& aData);

	// Signal parser that incoming data stream has finished
	// RETURN:
	//	TInt - an error code defined above, KErrNone if no error occured.
	//		   NOTE: In case no DTD was available, the document got buffered and was attached
	//				 to an attribute in the root node and CommitL returns KErrDocumentBuffered.
	//				 The buffered document will be re-parsed when engine receives the requested
	//				 DTD and calls a data handler (defined by iMIMEType) for validation.
	//##ModelId=3B6679A4027E
	IMPORT_C virtual TInt CommitL();

	// Validate the document according to the given DTD.
	// INPUT:
	//	aDTDRootNode - Root node to the DTD tree which defines the structure of the document.
	//                 This referenced node is the DTD root node created by the engine.
	//                 The actual DTD tree, if any, is attached as a child to this node.
	//				   NO INHERITING CLASS SHOULD OVERRIDE THIS! Override the protected
	//                 ValidateL instead.
	// RETURN:
	//	TInt - an error code defining the reason for validation failure. KErrNone if no errors.
	//##ModelId=3B6679A40274
	IMPORT_C virtual TInt ValidateL(CBNFNode& aDTDRootNode);

	// Set parser and library to its initial state to start parsing of a new document
	// INPUT:
	//	aRootNode - root node for the new document tree
	//##ModelId=3B6679A4026B
	IMPORT_C virtual void ResetL(CXmlElement* aRootNode);

	// -----------------------------------------------------------------------
	// (WAP Push Addition)
	// Wap Push specific method to switch off use of XML validator which doesn't work
	// with push messages.
	//	iIgnoreValidator will be set on class construction to False with complies with
	// current code usage; for push messages ONLY set iIgnoreValidator to True
	// -----------------------------------------------------------------------
//	inline void OmitValidator(TBool aIgnoreValidator) { iIgnoreValidator = aIgnoreValidator; } ;
	//
	// -----------------------------------------------------------------------

protected:
	// A simple, internal utility function to setup the Xml parser
	//##ModelId=3B6679A4026A
	IMPORT_C void PrepareParserL();

	//##ModelId=3B6679A40269
	IMPORT_C void ConstructL();

	// Default constructor
	// SEE NOTES ON iMimeType, iDefaultDocType and iDefaultDTDUrl for inheriting class's constuctor
	IMPORT_C CXmlLibrary(MWapPluginSP& aPluginSP, CXmlElement* aRootNode);

	// Data parsing method for internal usage. Takes ownership of the given data
	// INPUT:
	// aData - Pointer to the UNICODE text data. Parser shall take ownership of this data!
	// RETURN:
	// TInt - Error code, KErrNone if no errors.
	//##ModelId=3B6679A402A5
	TInt ProcessDataL(HBufC16* aData);

	// Internal utility function for finishing with the parser
	// RETURN:
	// TInt - Error code. KErrNone if no errors.
	//##ModelId=3B6679A40260
	TInt CommitParserL();

	// Internal utility function for handling return values from parsing and initiating DTD fetch
	// RETURN:
	// TInt - Error code. KErrNone if no errors.
	//##ModelId=3B6679A4025F
	TInt ExecuteDataProcessingL();

	//##ModelId=3B6679A40255
	CBNFNode* ExtractDTDTree(CBNFNode* aDTDRoot);

	// Internal validation function that actually performs the validation.
	// INPUT:
	// aDTDRootNode - Pointer to the _actual_ dtd tree root node, NOT the root given by angine
	// OUTPUT:
	// TInt - Error code, KErrNone if no errors
	//##ModelId=3B6679A4024B
	IMPORT_C virtual TInt ExecuteValidateL(CBNFNode* aDTDRootNode);

protected:	// Attributes

	/** Plugin Service Provider
	 */
	//##ModelId=3B6679A40239
	MWapPluginSP&		iPluginSP;

	/** Document Root Node
	 */
	//##ModelId=3B6679A40225
	CXmlElement*		iRootNode;

	// The default DTD doctype and Url in case no DTD was defined
	// NOTE: Inheriting class should set these values in its constructor!!
	//##ModelId=3B6679A4021B
	const TDesC*		iDefaultDoctype;

	//##ModelId=3B6679A40211
	const TDesC*		iDefaultDTDUrl;

	/** The actual DTD tree - not owned
	 */
	//##ModelId=3B6679A401FC
	CBNFNode*			iDTD;

private:	// Attributes

	/** The XML parser
	 */
	//##ModelId=3B6679A401F2
	CXmlParser*			iParser;

private:	// BC-proofing

/**
	Intended Usage	:	Reserved for future use
	@since			6.0
 */
	//##ModelId=3B6679A40241
	IMPORT_C virtual void CXmlLibrary_Reserved1();

	/** Reserved for future use
	 */
	//##ModelId=3B6679A401E8
	TAny*				iCXmlLibrary_Reserved;

	};

#endif // __XMLLIB_H__

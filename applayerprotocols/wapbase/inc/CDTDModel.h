// Copyright (c) 2000-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// contains the parser specialisation (BNF grammar) for a DTD parser.
// See CBNFParser for function details
// 
//

#ifndef __CDTDMODEL_H__
#define __CDTDMODEL_H__

#include <cbnfnode.h>
#include <cbnfparser.h>
#include <crulemarkedstack.h>

//##ModelId=3B666BCB001C



class CDTDModel : public CBNFParser
/** Creates a tree that represents a specified DTD.

To use it:

1. call the base class function ProcessDataL() with the filename of the DTD, and call CommitL()

2. call GeneratedTree() to get the tree

	@publishedAll
	@deprecated
*/
	{
public:
    class CDTDElementAttribute : public CBase
	/** Represents an attribute of a DTD element. */
		{
	public:
		/** DTD element attribute value types. */
		enum KValueType
			{
			/** Attribute supplies a default. */
			EDefault, 
			/** Attribute is #REQUIRED. */
			ERequired, 
			/** Attribute is #IMPLIED. */
			EImplied, 
			/** Attribute is #FIXED. */
			EFixed, 
			/** Attribute is a reference. */
			EReference
			};

	public:
		/** Destructor.
		*/
		virtual ~CDTDElementAttribute() 
			{ 
			delete iType; 
			delete iValue; 
			}

	public:
		/** Node that specifies the attribute type. */
		CBNFNode*	iType;
		/** Node that specifies the attribute value. */
        CBNFNode*	iValue;
		/** Attribute value type. */
        KValueType	iValueType;
		};


	enum TRuleMarks 
		{
		ERuleStartMark = 1, 
		EConditionalMark, 
		EGroupMark 
		};

	//##ModelId=3B666BCB0321
	IMPORT_C static CDTDModel* NewL(CAttributeLookupTable& aLUT);
	//##ModelId=3B666BCB0320
	IMPORT_C virtual ~CDTDModel();

	// This method MUST be invoked before data can be processed by the parser.
	//##ModelId=3B666BCB031F
	IMPORT_C virtual void ResetL();

	//##ModelId=3B666BCB0315
	IMPORT_C CBNFNode* GeneratedTree();

	// Building method for a hard coded WML1.1 DTD tree
	// Input:
	//		aPackageRoot - A root node to which the generated tree is attached
	//
	//##ModelId=3B666BCB030B
	IMPORT_C void BuildWml11DTDL(CBNFNode& aPackageRootNode);

protected:
    IMPORT_C CDTDModel(CAttributeLookupTable& aLUT);	

	// construct our BNF rule tree
	//##ModelId=3B666BCB0302
	IMPORT_C virtual CBNFNode* TreeL();
	
	// to control our internal state on conditional rules.
	//##ModelId=3B666BCB02F7
	IMPORT_C virtual void StartConditional(TParserNodeTypes aRuleType); 
	//##ModelId=3B666BCB02E3
	IMPORT_C virtual void EndConditional(TParserNodeTypes aRuleType, TBool aSuccess);

	// rule callbacks to build the DTD Tree
	//##ModelId=3B666BCB02D9
	static void PreRulesL(CBNFParser& aParser);
	//##ModelId=3B666BCB02CF
	static void PostRulesL(CBNFParser& aParser);
	//##ModelId=3B666BCB02C5
	static void PreElementDeclL(CBNFParser& aParser);
	//##ModelId=3B666BCB02B1
	static void PostElementDeclL(CBNFParser& aParser);
	//##ModelId=3B666BCB02A7
	static void PreEntityDeclL(CBNFParser& aParser);
	//##ModelId=3B666BCB0294
	static void PostEntityDeclL(CBNFParser& aParser);
	//##ModelId=3B666BCB0289
	static void PreAttlistLineL(CBNFParser& aParser);
	//##ModelId=3B666BCB027F
	static void PostAttlistLineL(CBNFParser& aParser);
	//##ModelId=3B666BCB0275
	static void PostAttlistReferenceL(CBNFParser& aParser);
	//##ModelId=3B666BCB026B
	static void PreAttValueL(CBNFParser& aParser);
	//##ModelId=3B666BCB0261
	static void PostAttValueL(CBNFParser& aParser);
	//##ModelId=3B666BCB024E
	static void PostRequiredL(CBNFParser& aParser);
	//##ModelId=3B666BCB0243
	static void PostFixedL(CBNFParser& aParser);
	//##ModelId=3B666BCB0239
	static void PostImpliedL(CBNFParser& aParser);
	//##ModelId=3B666BCB022F
	static void PostAndL(CBNFParser& aParser);
	//##ModelId=3B666BCB0225
	static void PostOrL(CBNFParser& aParser);
	//##ModelId=3B666BCB021B
	static void PreGroupL(CBNFParser& aParser);
	//##ModelId=3B666BCB0208
	static void PostGroupL(CBNFParser& aParser);
	//##ModelId=3B666BCB01FD
	static void PostMatch0PlusL(CBNFParser& aParser);
	//##ModelId=3B666BCB01F3
	static void PostMatch1PlusL(CBNFParser& aParser);
	//##ModelId=3B666BCB01E9
	static void PostOptionalL(CBNFParser& aParser);
	//##ModelId=3B666BCB01DF
	static void PostIdentifierL(CBNFParser& aParser);
	//##ModelId=3B666BCB01CC
	static void PostEntityDeclNameL(CBNFParser& aParser);
	//##ModelId=3B666BCB01C1
	static void PostElementDeclNameL(CBNFParser& aParser);
	//##ModelId=3B666BCB01B7
	static void PostAttlistDeclNameL(CBNFParser& aParser);
	//##ModelId=3B666BCB01AD
	static void PostAttfieldNameL(CBNFParser& aParser);
	//##ModelId=3B666BCB01A3
	static void PostReferenceNameL(CBNFParser& aParser);
	//##ModelId=3B666BCB0199
	static void PostStringL(CBNFParser& aParser);
	//##ModelId=3B666BCB0186
	static void PostValidHexCharL(CBNFParser& aParser);
	//##ModelId=3B666BCB017B
	static void PostValidDecCharL(CBNFParser& aParser);
	//##ModelId=3B666BCB0171
	static void PrePEDeclL(CBNFParser& aParser);
	//##ModelId=3B666BCB0167
	static void PostPEDeclL(CBNFParser& aParser);
	//##ModelId=3B666BCB015D
	static void PostGEDeclL(CBNFParser& aParser);
	//##ModelId=3B666BCB0153
	static void PreGEContentL(CBNFParser& aParser);
	//##ModelId=3B666BCB0149
	static void PostGEContentL(CBNFParser& aParser);	

	// utility functions
	//##ModelId=3B666BCB0135
	static void PostValidCharL(CBNFParser& aParser, TRadix aRadix);
	//##ModelId=3B666BCB0117
	static void PostRuleL(CBNFParser& aParser, TParserNodeTypes aType, const TDesC* aAttribute);

	//##ModelId=3B666BCB0104
	void CharRefReplacement(TPtr& aString);

	//##ModelId=3B666BCB0103
	void DoClear();

	// Help methods for the hardcoded WML 1.1 tree
	//##ModelId=3B666BCB00DB
	void AddNewAttributeL(CBNFNode& aNode, const TDesC* aAttributeId, const TDesC& aAttributeType, CDTDElementAttribute::KValueType aAttributeValueType, CBNFNode* aRootNode);
	//##ModelId=3B666BCB00BD
	void AddNewReferenceAttributeL(CBNFNode& aNode, const TDesC& aAttributeType, CBNFNode* aRootNode);
	//##ModelId=3B666BCB0095
	void AddNewAttributeWithValueL(CBNFNode& aNode, const TDesC* aAttributeId, const TDesC& aAttributeType, const TDesC& aAttributeDefaultValue, CBNFNode* aRootNode);


protected:
    typedef CRuleMarkedStack<ETrue> COwnerRuleMarkedStack;

protected:
	//##ModelId=3B666BCB008B
	TBool					iInPEDecl;	
	//##ModelId=3B666BCB0078
	TBool					iCharRefCheck;
	//##ModelId=3B666BCB006E
	HBufC*					iGEContent;

	//##ModelId=3B666BCB0064
	HBufC*					iNewAttfieldName;
	//##ModelId=3B666BCB005A
	CDTDElementAttribute*	iNewElementAttribute;
	//##ModelId=3B666BCB0046
	CBNFNode*				iNewRule;
	//##ModelId=3B666BCB003C
	CBNFNode*				iNewTree;
	//##ModelId=3B666BCB0030
	COwnerRuleMarkedStack	iNewRuleStack;
	};

#endif // __CDTDMODEL_H__

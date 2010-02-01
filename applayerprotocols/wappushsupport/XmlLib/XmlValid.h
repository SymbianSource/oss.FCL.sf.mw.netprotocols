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

#ifndef __XMLVALID_H__
#define __XMLVALID_H__

// System includes
//
#include <e32base.h>
#include <cstack.h>

//	Forward class declarations
//
class CAttributeLookupTable;
class CBNFNode;
class CXmlElement;

//	CLASS DEFINITION
//
//##ModelId=3B6679A20284
class CXmlValidator : public CBase
	{
protected:	// Data types
	enum TDTDNodeMatch
		{
		EMiss,
		EConditionalMiss,
		EMatch
		};

typedef CStack<CBNFNode, EFalse> CDTDNodeStack;
typedef CStack<CXmlElement, EFalse> CChildMarkStack;

public:	// Methods
	//##ModelId=3B6679A300A6
	static CXmlValidator* NewL(CBNFNode& aDTD, CAttributeLookupTable& aALUT);
	//##ModelId=3B6679A300A5
	virtual ~CXmlValidator();
	//##ModelId=3B6679A30091
	TInt ValidateTreeL(CXmlElement* aRootNode);

protected:	// Methods
	//##ModelId=3B6679A3007D
	CXmlValidator(CBNFNode& aDTD, CAttributeLookupTable& aALUT);
	//##ModelId=3B6679A30072
	void ConstructL();

	//##ModelId=3B6679A3005E
	TInt ValidateNodeAttributesL(CXmlElement* aDocumentNode, CBNFNode* aDTDNode);
	//##ModelId=3B6679A3004A
	TInt ValidateNodeChildrenL(CXmlElement* aDocumentNode, CBNFNode* aDTDNode);
	//##ModelId=3B6679A30040
	TInt ValidateNodeL(CXmlElement* aNode);
	//##ModelId=3B6679A30022
	TInt NormalizeAndCheckAttributeValueL(TPtr& aAttributeValue, CBNFNode* aAttributeValueType);

private:	// Methods
	//##ModelId=3B6679A3000E
	TInt HandleReferenceL(CBNFNode* aDTDNode, TBool& aNodeFinished);
	//##ModelId=3B6679A203E2
	TInt HandleAndL(CBNFNode* aDTDNode, TBool& aNodeFinished);
	//##ModelId=3B6679A203CE
	TInt HandleOrL(CBNFNode* aDTDNode, TBool& aNodeFinished);
	//##ModelId=3B6679A203BA
	TInt HandleOptionalL(CBNFNode* aDTDNode, TBool& aNodeFinished);
	//##ModelId=3B6679A2039C
	TInt HandleNMoreL(CBNFNode* aDTDNode, TBool& aNodeFinished);

	//##ModelId=3B6679A20388
	TInt HandleReferenceInAttributeValueTypeL(CBNFNode* aDTDNode, TBool& aNodeFinished);
	//##ModelId=3B6679A20374
	TInt HandleAttributeValueCheckAndNormalize(TPtr& aAttributeValue, const TDesC* aAttributeValueType);

	//##ModelId=3B6679A20360
	const TDesC* NodeName(CBNFNode* aNode);
	//##ModelId=3B6679A20356
	void NormalizeAttributeValueWhiteSpaces(TPtr& aAttributeValue);

protected:	// Attributes
	//##ModelId=3B6679A2034C
	CBNFNode&					iDTD;
	//##ModelId=3B6679A20338
	CAttributeLookupTable&		iALUT;

	// These are pointers and variables for the validation DTD tree traversing
	// They are used to temporarily store pointers to data structures while traversing
	// the tree and hence, data is not owned by these!
	//##ModelId=3B6679A20324
	CXmlElement*				iCurrentDocumentChild;
	//##ModelId=3B6679A20310
	CBNFNode*					iDTDChildNode;
	//##ModelId=3B6679A202FE
	TBool						iNodeMatched;
	//##ModelId=3B6679A202F4
	TDTDNodeMatch				iSubNodeMatch;
	//##ModelId=3B6679A202E0
	CArrayFixFlat<TInt>*		iCounterArray;			// ...except for the counter stack, that owns the TInts
	//##ModelId=3B6679A202B8
	CDTDNodeStack				iDTDNodeStack;
	//##ModelId=3B6679A202AF
	CChildMarkStack				iDocumentChildMarkStack;

	};

#endif // __XMLVALID_H__

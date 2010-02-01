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
//

// Local includes
//
#include "XmlValid.h"

// System includes
//
#include <cdtdmodel.h>
#include <xmlelemt.h>
#include <cbnfnode.h>
#include <cnodeleteattribute.h>
#include <xmllib.h>


CXmlValidator::CXmlValidator(CBNFNode& aDTD, CAttributeLookupTable& aALUT) : iDTD(aDTD), iALUT(aALUT)
	{
	}

CXmlValidator::~CXmlValidator()
	{
	delete iCounterArray;
	}

CXmlValidator* CXmlValidator::NewL(CBNFNode& aDTD, CAttributeLookupTable& aALUT)
	{
	CXmlValidator* self = new(ELeave) CXmlValidator(aDTD, aALUT);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return(self);
	}

void CXmlValidator::ConstructL()
	{
	iCounterArray = new(ELeave) CArrayFixFlat<TInt>(3);
	}

TInt CXmlValidator::ValidateTreeL(CXmlElement* aRootNode)
	{
	if( aRootNode == NULL )
		{
		return( EWapErrXmlLibMissingDocument );
		}
	CNode* currentNode = aRootNode;
	CNode* childNode = NULL;
	TInt returnCode = KErrNone;

	FOREVER		// Hihi :)
		{
		if( childNode == NULL )
			{
			// We entered this node first time, going down the tree

			returnCode = ValidateNodeL((CXmlElement*)currentNode);
			if( returnCode != KErrNone )
				{
				return( returnCode );
				}

			childNode = currentNode->NextChild();  // The first born...
			}
		else
			{
			// We returned from a child node
			childNode = currentNode->NextChild(childNode);
			}

		if( childNode == NULL )
			{
			if( currentNode == aRootNode )
				{
				// The tree is done.
				return( KErrNone );
				}
			// Go up the tree
			childNode = currentNode;
			currentNode = currentNode->Parent();
			}
		else
			{
			currentNode = childNode;
			childNode = NULL;
			}
		}
	}

TInt CXmlValidator::ValidateNodeL(CXmlElement* aNode)
	{
	TInt returnCode = KErrNone;

	CBNFNode* dtdNode = REINTERPRET_CAST(CBNFNode*,iDTD.Attribute((const TDesC*)(aNode->Type())));
	if( dtdNode == NULL )
		{
		return( EWapErrXmlLibIllegalTagName );
		}
	returnCode = ValidateNodeAttributesL(aNode, dtdNode);
	if( returnCode == KErrNone )
		{
		returnCode = ValidateNodeChildrenL(aNode, dtdNode);
		}

	// Here we could perform the validation of Node's data

	return( returnCode );
	}


TInt CXmlValidator::ValidateNodeChildrenL(CXmlElement* aDocumentNode, CBNFNode* aDTDNode)
	{
	iCurrentDocumentChild = (CXmlElement*)aDocumentNode->NextChild();
	if( iCurrentDocumentChild != NULL && iDTD.Attribute((const TDesC*)(iCurrentDocumentChild->Type())) == NULL )
		{
		return( EWapErrXmlLibIllegalTagName );
		}

	TInt returnValue = KErrNone;
	TBool nodeFinished = EFalse;
	iDTDChildNode = NULL;
	CBNFNode* dtdNode;
	iDTDNodeStack.Clear();
	iDTDNodeStack.PushL(aDTDNode);
	iCounterArray->Reset();
	iNodeMatched = EFalse;
	while( iCurrentDocumentChild != NULL && !iDTDNodeStack.IsEmpty() )
		{
		dtdNode = iDTDNodeStack.Head();
		nodeFinished = EFalse;
		switch(dtdNode->Type())
			{
		case EReference:
			returnValue = HandleReferenceL(dtdNode, nodeFinished);
			break;
		case ERoot:
		case EAnd:
			returnValue = HandleAndL(dtdNode, nodeFinished);
			break;
		case EOr:
			returnValue = HandleOrL(dtdNode, nodeFinished);
			break;
		case EOptional:
			returnValue = HandleOptionalL(dtdNode, nodeFinished);
			break;
		case ENMore:
			returnValue = HandleNMoreL(dtdNode, nodeFinished);
			break;
		case EIncomplete:
			nodeFinished = ETrue;
			iNodeMatched = ETrue;
			break;
			}

		if( returnValue != KErrNone )
			{
			return( returnValue );
			}

		if( nodeFinished )
			{
			iDTDChildNode = dtdNode;
			iDTDNodeStack.Pop();
			}
		else
			{
			iDTDChildNode = NULL;
			}

		if( iNodeMatched )
			{
			iCurrentDocumentChild = (CXmlElement*)aDocumentNode->NextChild(iCurrentDocumentChild);
			if( iCurrentDocumentChild != NULL && iDTD.Attribute((const TDesC*)(iCurrentDocumentChild->Type())) == NULL )
				{
				return( EWapErrXmlLibIllegalTagName );
				}
			iNodeMatched = EFalse;
			iSubNodeMatch = EMatch;
			}
		}

	if( iCurrentDocumentChild == NULL )
		{
		return( KErrNone );
		}
	else
		{
		return( EWapErrXmlLibInvalidDocumentStructure );
		}
	}

TInt CXmlValidator::HandleReferenceL(CBNFNode* aDTDNode, TBool& aNodeFinished)
	{	
	if( iDTDChildNode == NULL )
		{
		CNoDeleteAttributeT<CBNFNode*>* attributeNode = REINTERPRET_CAST(CNoDeleteAttributeT<CBNFNode*>*, aDTDNode->Attribute(CBNFNode::KReference()));
		CBNFNode* referencedNode = attributeNode->Attribute();

		const TDesC* dtdNodeName = NodeName(referencedNode);
		if( (*dtdNodeName)[0] != '%' )
			{
			aNodeFinished = ETrue;
			if( iCurrentDocumentChild->Type()->Compare(*dtdNodeName) == 0 || 
				( iCurrentDocumentChild->Type()->Compare(KCDataID) == 0 && 
				  dtdNodeName->Compare(KPCDataID) == 0 )
			  )
				{
				iNodeMatched = ETrue;
				}
			else
				{
				iNodeMatched = EFalse;
				}
			}
		else
			{
			aNodeFinished = EFalse;
			iDTDNodeStack.PushL(referencedNode);
			}
		}
	else
		{
		aNodeFinished = ETrue;
		}
	return( KErrNone );
	}

TInt CXmlValidator::HandleAndL(CBNFNode* aDTDNode, TBool& aNodeFinished)
	{
	if( iDTDChildNode == NULL )
		{
		iSubNodeMatch = EMiss;
		aNodeFinished = EFalse;
		iDTDNodeStack.PushL((CBNFNode*)aDTDNode->NextChild());
		iCounterArray->InsertL(0, 0);
		}
	else
		{
		if( (iSubNodeMatch == EMatch || iSubNodeMatch == EConditionalMiss) && aDTDNode->NextChild(iDTDChildNode)!=NULL )
			{
			if( iSubNodeMatch == EMatch )
				{
				(*iCounterArray)[0]++;
				}
			aNodeFinished = EFalse;
			iSubNodeMatch = EMiss;
			iDTDNodeStack.PushL( (CBNFNode*)aDTDNode->NextChild(iDTDChildNode) );
			}
		else
			{
			if( iCounterArray->At(0) > 0 && iSubNodeMatch == EConditionalMiss )
				iSubNodeMatch = EMatch;
			aNodeFinished = ETrue;
			iCounterArray->Delete(0);
			}
		}
	return( KErrNone );
	}

TInt CXmlValidator::HandleOrL(CBNFNode* aDTDNode, TBool& aNodeFinished)
	{
	if( iDTDChildNode == NULL )
		{
		aNodeFinished = EFalse;
		iSubNodeMatch = EMiss;
		iDTDNodeStack.PushL((CBNFNode*)aDTDNode->NextChild());
		iDocumentChildMarkStack.PushL(iCurrentDocumentChild);
		}
	else
		{
		if( (iSubNodeMatch == EMiss || iSubNodeMatch == EConditionalMiss) && aDTDNode->NextChild(iDTDChildNode)!=NULL )
			{
			aNodeFinished = EFalse;
			iDTDNodeStack.PushL((CBNFNode*)aDTDNode->NextChild(iDTDChildNode) );
			iCurrentDocumentChild = iDocumentChildMarkStack.Head();
			}
		else
			{
			if( iSubNodeMatch != EMatch )
				{
				iCurrentDocumentChild = iDocumentChildMarkStack.Pop();
				}
			else
				{
				iDocumentChildMarkStack.Pop();
				}

			aNodeFinished = ETrue;
			}
		}
	return( KErrNone );
	}

TInt CXmlValidator::HandleOptionalL(CBNFNode* aDTDNode, TBool& aNodeFinished)
	{
	if( iDTDChildNode == NULL )
		{
		aNodeFinished = EFalse;
		iDTDNodeStack.PushL((CBNFNode*)aDTDNode->NextChild());
		}
	else
		{
		aNodeFinished = ETrue;
		if( iSubNodeMatch == EMiss )
			{
			iSubNodeMatch = EConditionalMiss;
			}
		}
	return( KErrNone );
	}

TInt CXmlValidator::HandleNMoreL(CBNFNode* aDTDNode, TBool& aNodeFinished)
	{
	if( iDTDChildNode == NULL )
		{
		iSubNodeMatch = EMiss;
		iDTDNodeStack.PushL((CBNFNode*)aDTDNode->NextChild());
		iCounterArray->InsertL(0, 0);
		aNodeFinished = EFalse;
		}
	else
		{
		if( iSubNodeMatch == EMatch )
			{
			(*iCounterArray)[0]++;
			iDTDNodeStack.PushL((CBNFNode*)aDTDNode->NextChild());
			iSubNodeMatch = EMiss;
			aNodeFinished = EFalse;
			}
		else
			{
			aNodeFinished = ETrue;
			if( iCounterArray->At(0) > 0 )
				{
				iSubNodeMatch = EMatch;
				}
			else if( aDTDNode->AttributeExists(CBNFNode::KNMoreMinimum()) )
				{
				iSubNodeMatch = EMiss;
				}
			else
				{
				iSubNodeMatch = EConditionalMiss;
				}
			iCounterArray->Delete(0);
			}
		}
	return( KErrNone );
	}

const TDesC*  CXmlValidator::NodeName(CBNFNode* aNode)
    {
	TInt count=iDTD.AttributeCount();
	const TDesC* nodeId = NULL;
	for (TInt i = 0; i < count; i++)
		{
		const TDesC* type;
		if (aNode == iDTD.AttributeByIndex(i,type))
			{
			nodeId = type;
			break;
			}
		}
	return nodeId;
    }


TInt CXmlValidator::ValidateNodeAttributesL(CXmlElement* aDocumentNode, CBNFNode* aDTDNode)
// aDTDNode is the node from the DTD tree corresponding to the aDocumentNode
	{

	TInt i;
	TInt count=aDTDNode->AttributeCount();
	for(i=0; i < count; i++)
		{
		const TDesC* attributeName;
		CDTDModel::CDTDElementAttribute* DTDAttribute = REINTERPRET_CAST(CDTDModel::CDTDElementAttribute*, aDTDNode->AttributeByIndex(i,attributeName) );

		if( DTDAttribute->iValueType == CDTDModel::CDTDElementAttribute::EReference )
			{				
			CNoDeleteAttributeT<CBNFNode*>* attributeNode = REINTERPRET_CAST(CNoDeleteAttributeT<CBNFNode*>*,DTDAttribute->iType->Attribute(CBNFNode::KReference()));
			TInt refReturnValue = ValidateNodeAttributesL(aDocumentNode, attributeNode->Attribute());
			if( refReturnValue != KErrNone )
				{
				return(refReturnValue);
				}
			continue;
			}

//		const TDesC* attributeName = aDTDNode->AttributeTypeByIndex(i);
		const TDesC* documentAttributeValue = aDocumentNode->Attribute(*attributeName);
		if( documentAttributeValue != NULL )
			{
			if( DTDAttribute->iValueType == CDTDModel::CDTDElementAttribute::EFixed )
				{
				if( documentAttributeValue->Compare(*(DTDAttribute->iValue->Data()) ) != 0 )
					{
					return( EWapErrXmlLibIllegalFixedAttributeValue );
					}
				}
			else
				{
				TPtr attrPtr(((HBufC*)documentAttributeValue)->Des());
				TInt errorCode = NormalizeAndCheckAttributeValueL(attrPtr, DTDAttribute->iType);
				if( errorCode != KErrNone )
					{
					return( errorCode );
					}
				}
			}
		else
			{
			switch( DTDAttribute->iValueType )
				{
			case CDTDModel::CDTDElementAttribute::ERequired:
				{
				return( EWapErrXmlLibMissingRequiredAttribute );
				} // break;
			case CDTDModel::CDTDElementAttribute::EFixed:
			case CDTDModel::CDTDElementAttribute::EDefault:
				aDocumentNode->SetAttributeL( *attributeName, *(DTDAttribute->iValue->Data()), iALUT);
				break;
			case CDTDModel::CDTDElementAttribute::EImplied:
			default:
				break;
				}
			}
		}
	return(KErrNone);
	}


TInt CXmlValidator::NormalizeAndCheckAttributeValueL(TPtr& aAttributeValue, CBNFNode* aAttributeValueTypeRoot)
	{
	NormalizeAttributeValueWhiteSpaces(aAttributeValue);

	TInt returnValue = KErrNone;
	TBool nodeFinished = EFalse;
	iDTDChildNode = NULL;
	CBNFNode* dtdNode;
	iDTDNodeStack.Clear();
	iDTDNodeStack.PushL(aAttributeValueTypeRoot);
	iCounterArray->Reset();
	iNodeMatched = EFalse;
	while( !iNodeMatched && !iDTDNodeStack.IsEmpty() )
		{
		dtdNode = iDTDNodeStack.Head();
		nodeFinished = EFalse;
		switch(dtdNode->Type())
			{
		case EReference:
			returnValue = HandleReferenceInAttributeValueTypeL(dtdNode, nodeFinished);
			break;
		case ERoot:
		case EAnd:
			returnValue = HandleAndL(dtdNode, nodeFinished);
			break;
		case EOr:
			returnValue = HandleOrL(dtdNode, nodeFinished);
			break;
		case EOptional:
			returnValue = HandleOptionalL(dtdNode, nodeFinished);
			break;
		case ENMore:
			returnValue = HandleNMoreL(dtdNode, nodeFinished);
			break;
		case EIncomplete:
			nodeFinished = ETrue;
			returnValue = HandleAttributeValueCheckAndNormalize( aAttributeValue, NodeName(dtdNode) );
			break;
			}

		if( returnValue != KErrNone )
			{
			return( returnValue );
			}

		if( nodeFinished )
			{
			iDTDChildNode = dtdNode;
			iDTDNodeStack.Pop();
			}
		else
			{
			iDTDChildNode = NULL;
			}
		}

	if( iNodeMatched )
		{
		return( KErrNone );
		}
	else
		{
		return( EWapErrXmlLibIllegalAttributeValue );
		}
	}

TInt CXmlValidator::HandleReferenceInAttributeValueTypeL(CBNFNode* aDTDNode, TBool& aNodeFinished)
	{
	if( iDTDChildNode == NULL )
		{
		CNoDeleteAttributeT<CBNFNode*>* attributeNode = REINTERPRET_CAST(CNoDeleteAttributeT<CBNFNode*>*, aDTDNode->Attribute(CBNFNode::KReference()));
		CBNFNode* referencedNode = attributeNode->Attribute();
		aNodeFinished = EFalse;
		iDTDNodeStack.PushL(referencedNode);
		}
	else
		{
		aNodeFinished = ETrue;
		}
	return( KErrNone );
	}

TInt CXmlValidator::HandleAttributeValueCheckAndNormalize(TPtr& aAttributeValue, const TDesC* aAttributeValueType)
	{
	_LIT( KNMTOKENID, "NMTOKEN");
	_LIT( KIDID, "ID" );

	if( aAttributeValueType->Compare(KCDataID) != 0 )
		{
		// Xml specs 3.3.3, when declared calue is not CDATA, leading and trailing spaces
		// are wasted and space sequences are replaced with single space
		aAttributeValue.TrimAll();
		}
	else
		{
		// The value type is CDATA
		// In future, here we could ran a check for the contents the attribute that it
		// corresponds to CDATA constraints
		iNodeMatched = ETrue;
		return( KErrNone );
		}

	if( aAttributeValueType->Compare(KNMTOKENID) == 0 )
		{
		// Here we could ran a check for the attribute contents to correspond
		// to NMTOKEN rules
		iNodeMatched = ETrue;
		}
	else if( aAttributeValueType->Compare(KIDID) == 0 )
		{
		// Here we could ran a check for the attribute contents to correspond
		// to ID rules
		iNodeMatched = ETrue;
		}
	else
		{
		// The value aAttributeValueType contains an enumerated value. Check if this one matches to the attribute value
		if( aAttributeValue.Compare(*aAttributeValueType) == 0 )
			{
			iNodeMatched = ETrue;
			}
		else
			{
			iNodeMatched = EFalse;
			}
		}

	return( KErrNone );
	}

void CXmlValidator::NormalizeAttributeValueWhiteSpaces(TPtr& aAttributeValue)
	{
	// White space normalization, XML specs 3.3.3
	TInt i;
	TInt length=aAttributeValue.Length();
	TText* stringPtr=(TText*)aAttributeValue.Ptr();
	for(i=0; i < length; i++)
		{
		TText ch=*stringPtr;
		if( ch==0xD && i<length-1 && *(stringPtr+1)==0xA)
			{
			aAttributeValue.Delete(i,1);
			*stringPtr=0x20;
			length--;
			}
		if(	ch==0x9 || ch==0xA || ch==0xD)
			{
			*stringPtr = 0x20;
			}
		++stringPtr;
		}
	}

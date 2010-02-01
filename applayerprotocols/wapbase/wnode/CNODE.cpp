// Copyright (c) 1998-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <cnode.h>

#ifndef __WAP_MONOLITHIC__
// Its a standalone DLL

#ifndef EKA2
GLDEF_C TInt E32Dll(TDllReason /*aReason*/)
	{
	return(KErrNone);
	}
#endif

#endif
//
// Node implementation
//

/** Allocates and constructs a new node.

@return New node
@param aType The type of the node
@param aParent The parent of this node
*/
EXPORT_C CNode* CNode::NewL(TAny* aType,CNode* aParent)
	{
	return (new(ELeave) CNode(aType,aParent));
	}

CNode::CNode(TAny* aType,CNode* aParent)
	: iType(aType), iParent(aParent), iValues(8), iTypes(8)
	{}

/** Destructor.

Data (if owned by the object), child nodes and attributes are deleted.
*/
EXPORT_C CNode::~CNode()
	{
	iValues.ResetAndDestroy();
	delete iDataValue;
	DeleteAllChildNodes();
	}

/** Deletes a specified child node.

@param aNode Node to delete
*/
EXPORT_C void CNode::DeleteChildNode(CNode* aNode)
	{
	__ASSERT_DEBUG(iChildList != NULL && aNode != NULL ,Panic(ENodeNoChildren));
	__ASSERT_DEBUG(aNode != NULL ,Panic(ENodeBadArgument));
	CArrayPtr<CNode> &childList = *iChildList;
	TInt childListCount = iChildList->Count() - 1;
	for(TInt childIter = childListCount;childIter >= 0;childIter--)
		{
		if(aNode == childList[childIter])
			{
			delete childList[childIter];
			iChildList->Delete(childIter);
			break;
			}

		}
	}

/** Deletes all the child nodes of this node.
*/
EXPORT_C void CNode::DeleteAllChildNodes()
	{
	if(iChildList != NULL)
		{
		iChildList->ResetAndDestroy();
		delete iChildList;
		iChildList = NULL;
		}
	}

/** Creates a new child node.

@return The new child node
@param aType Node type
*/
EXPORT_C CNode& CNode::AppendNodeL(TAny* aType)
	{
	CNode* node = CNode::NewL(aType,this);
	CleanupStack::PushL(node);
	AppendNodeToThisNodeL(node);
	CleanupStack::Pop();
	return *node;
	}

/** Adds an existing node as a child.

@param aNode Node to make a child
*/
EXPORT_C void CNode::AppendNodeToThisNodeL(CNode* aNode)
	{
	if(!iChildList)
		iChildList = new (ELeave) CArrayPtrFlat<CNode>(KGranularity);
	iChildList->AppendL(aNode);
	}

/** Gets the first child or the next child after a specified child.

@return First or next child node
@param aNode Child node or NULL to get the first child
*/
EXPORT_C CNode* CNode::NextChild(const CNode* aNode) const
	{
//	__ASSERT_DEBUG(iChildList != NULL,Panic(ENodeNoChildren));
	if(iChildList)	//if there are any children
		{
		if(aNode == NULL)	//if just want the first child
			return (*iChildList)[0];
		else
			{
			TInt index = FindChild(aNode);
			__ASSERT_DEBUG(index != KErrNotFound,Panic(ENodeBadArgument));
//			__ASSERT_DEBUG(!((index + 1) >= iChildList->Count()),Panic(ENodeBadArgument));
			if(index == KErrNotFound  || ((index+1) >= iChildList->Count()) )
				return NULL;
			else
				return (*iChildList)[index + 1];
			}
		}
	return NULL;
	}

TInt CNode::FindChild(const CNode* aNode) const
	{
	__ASSERT_DEBUG(aNode != NULL, Panic(ENodeBadArgument));
	__ASSERT_DEBUG(iChildList != NULL,Panic(ENodeNoChildren));
	TInt count = iChildList->Count();
	for(TInt childIter = 0;childIter < count; childIter++)
		{
		if(aNode == (*iChildList)[childIter])
			return childIter;
		}
	return KErrNotFound;
	}

/** Gets the previous child before a specified child.

@return Previous child node
@param aNode Child node
*/
EXPORT_C CNode* CNode::PrevChild(const CNode& aNode) const
	{
	__ASSERT_DEBUG(iChildList != NULL,Panic(ENodeNoChildren));
	if(iChildList)	//if there are any children
		{
		TInt index = FindChild(&aNode);
		__ASSERT_DEBUG(index != KErrNotFound && index != 0,Panic(ENodeBadArgument));
		return (*iChildList)[index - 1];
		}
	else
		return NULL;
	}

/** Gets the parent of this node.

@return Parent
*/
EXPORT_C CNode* CNode::Parent() const
	{
	return iParent;
	}

/** Changes the parent of the node.

The node is removed from the childlist of its current parent.

@param aParent New parent
*/
EXPORT_C void CNode::ReparentL(CNode* aParent)
	{
//	__ASSERT_DEBUG(iParent != NULL && iParent->iChildList != NULL,Panic(ENodeNoChildren));
	if( iParent != NULL && iParent->iChildList != NULL )
	{
		for(TInt ChildIter = 0;ChildIter < iParent->iChildList->Count(); ChildIter++)
			{
			if(this == (*iParent->iChildList)[ChildIter])
				{
				iParent->iChildList->Delete(ChildIter);
				break;
				}
			}
	}
	if (aParent)
		{
		aParent->AppendNodeToThisNodeL(this);
		iParent = aParent;
		}
	}

/** Gets the next sibling node.

This asks for the next child of its parent.

@return Next sibling node
*/
EXPORT_C CNode* CNode::NextSibling() const
	{
	__ASSERT_DEBUG(iParent != NULL,Panic(ENodeNoChildren));
	if(iParent)
		return Parent()->NextChild(this);
	else 
		return NULL;
	}

/** Gets the previous sibling node.

This asks for the previous child of its parent.

@return Previous sibling node
*/
EXPORT_C CNode* CNode::PrevSibling() const
	{
	__ASSERT_DEBUG(iParent != NULL,Panic(ENodeNoChildren));
	if(iParent)
		return Parent()->PrevChild(*this);
	else 
		return NULL;
	}

/** Gets the number of children of this node.

@return Number of children of this node
*/
EXPORT_C TInt CNode::NumberImmediateChildren() const
	{
	if(iChildList)
		return iChildList->Count();
	else 
		return 0;
	}

/** Gets the absolute root node of the tree.

@return Root node
*/
EXPORT_C const CNode& CNode::Root() const
	{
	const CNode* node = this;
	while(node->Parent() != NULL)
		node = node->Parent();
	return *node;
	}

/** Sets the node data.

The object will delete the data in its destructor.

@param aData Node data
*/
EXPORT_C void CNode::SetDataL(HBufC16* aData)
	{
	delete iDataValue;
	iDataValue=NULL;
	iDataValue = new(ELeave) CDataDelete(aData);
	}

HBufC16* CNode::SetupDeletableOrNonDeleteableDataLC()
	{
	__ASSERT_DEBUG(iDataValue != NULL ,Panic(ENoData));
	HBufC16* aData = iDataValue->SetData(NULL);
	CleanupStack::PushL(aData); // **** eeek !
	delete iDataValue;
	return aData;
	}

/** Sets the object not to delete the node data in its destructor.

Note that the function internally reallocates memory. If it leaves, the data is lost.
 */
EXPORT_C void CNode::SetDataNoDeleteL()
	{
	HBufC16* aData = SetupDeletableOrNonDeleteableDataLC();
	iDataValue = new (ELeave) CDataNoDelete(aData);
	CleanupStack::Pop();
	}

/** Sets the object to delete the node data in its destructor.

Note that the function internally reallocates memory. If it leaves, the data is lost. */
EXPORT_C void CNode::ClearSetDataNoDeleteL()
	{
	HBufC16* aData = SetupDeletableOrNonDeleteableDataLC();
	iDataValue = new (ELeave) CDataDelete(aData);
	CleanupStack::Pop();
	}

/** Sets the node data to be taken from a specified file.

If the data is deleted, the referenced file is also deleted.

@param aFileDataLocationNowNodeOwns Name of the file containing the data
*/
EXPORT_C void CNode::SetFileDataL(HBufC16* aFileDataLocationNowNodeOwns)
	{
	delete iDataValue;
	iDataValue = NULL;
	iDataValue = new(ELeave) CFileDataDelete(aFileDataLocationNowNodeOwns);
	}

/** Resets the node data to a specified pointer.

Existing data owned by the node is deleted.

@param aData Root node
*/
EXPORT_C void CNode::ResetDataPointer(HBufC16* aData)
	{
	if (iDataValue)
		iDataValue->ResetDataPointer(aData);
	}

/** Gets the node data.

@return Node data or NULL if no data is set
*/
EXPORT_C HBufC16* CNode::Data() const
	{
	if(iDataValue)
		return iDataValue->Data();
	else
		return NULL;
	}

/** Adds an attribute.

The node takes ownership of aAttributeValue.

@param aAttributeType Attribute type
@param aAttributeValue Attribute value
*/
EXPORT_C void CNode::AddAttributeL(TAny* aAttributeType, CBase* aAttributeValue)
	{
	TInt count;
	if((count=iValues.Count())>0)
		{
		const TInt32* typesPtr=iTypesBasePtr;
		for(TInt ii=0;ii<count;ii++)
			{
			if(*typesPtr==(TInt32)aAttributeType)
				{
				delete iValues[ii];
				iValues[ii]=aAttributeValue;
				return;
				}
			typesPtr++;
			}
		}
	iTypes.AppendL(aAttributeType);
	TRAPD(err,iValues.AppendL(aAttributeValue));
	if(err)
		iTypes.Delete(iTypes.Count()-1);
	AdjustBasePointers();
	User::LeaveIfError(err);
	}

void CNode::AdjustBasePointers()
	{
	if(iValues.Count())
		iTypesBasePtr=(TInt32*)iTypes.CArrayFixBase::At(0);
	else
		iTypesBasePtr=NULL;
	}

/** Sets node data and adds an attribute.

The node takes ownership of aDataand aAttributeValue. 
Existing node data owned by the node is deleted.

@param aData Node data
@param aAttributeType Attribute type
@param aAttributeValue Attribute value
*/
EXPORT_C void CNode::AddDataAndAttributeL(HBufC16 *aData, TAny* aAttributeType, CBase* aAttributeValue)
	{
	__ASSERT_DEBUG(aData != NULL, Panic(ENodeBadArgument));

	delete iDataValue;
	iDataValue = NULL;
	iDataValue = new(ELeave) CDataDelete(aData);

	AddAttributeL(aAttributeType,aAttributeValue);
	}

/** Gets a child node by index.

@return Child node
@param aByIndex Index of the child node
*/
EXPORT_C CNode* CNode::Child(TInt aByIndex) const
	{
	__ASSERT_DEBUG(iChildList != NULL , Panic(ENodeNoChildren));
	__ASSERT_DEBUG(aByIndex < iChildList->Count() && aByIndex >= 0, Panic(ENodeBadArgument));
	return ((*iChildList)[aByIndex]);
	}

/** Gets an attribute value for a specified attribute type.

@return Attribute value
@param aAttributeType Attribute type
*/
EXPORT_C CBase* CNode::Attribute(TAny* aAttributeType) const
	{
	TInt count;
	if((count=iValues.Count())>0)
		{
		const TInt32* typesPtr=iTypesBasePtr;
		for(TInt ii=0;ii<count;ii++)
			{
			if(*typesPtr==(TInt32)aAttributeType)
				return iValues[ii];
			typesPtr++;
			}
		}
	return NULL;
	}

/** Tests if an attribute of a specified type exists.

@return True if the attribute exists, otherwise false
@param aAttributeType Attribute type
*/
EXPORT_C TBool CNode::AttributeExists(TAny* aAttributeType) const
	{
	TInt count;
	if((count=iValues.Count())>0)
		{
		const TInt32* typesPtr=iTypesBasePtr;
		for(TInt ii=0;ii<count;ii++)
			{
			if(*typesPtr==(TInt32)aAttributeType)
				return ETrue;
			typesPtr++;
			}
		}
	return EFalse;
	}

/** Deletes an attribute of a specified type.

Note that the attribute value will be deleted.

@param aAttributeType Attribute type
*/
EXPORT_C void CNode::DeleteAttribute(TAny* aAttributeType)
	{
	TInt count;
	if((count=iValues.Count())>0)
		{
		const TInt32* typesPtr=iTypesBasePtr;
		for(TInt ii=0;ii<count;ii++)
			{
			if(*typesPtr==(TInt32)aAttributeType)
				{ // delete attribute value
				delete iValues[ii];
				iValues.Delete(ii);
				iTypes.Delete(ii);
				AdjustBasePointers();
				break;
				}
			typesPtr++;
			}
		}
	}

/** Delete all node attributes.

Note that attribute values will be deleted.
*/
EXPORT_C void CNode::DeleteAllAttributes()
	{
	iValues.ResetAndDestroy();
	iTypes.Reset();
	AdjustBasePointers();
	}

/** Removes an attribute of a specified type, but does not delete it.

The caller is now responsible for the destruction of the attribute value.

@param aAttributeType Attribute type
*/
EXPORT_C void CNode::RemoveAttributeNoDelete(TAny* aAttributeType)
	{
	TInt count;
	if((count=iValues.Count())>0)
		{
		const TInt32* typesPtr=iTypesBasePtr;
		for(TInt ii=0;ii<count;ii++)
			{
			if(*typesPtr==(TInt32)aAttributeType)
				{ // do not delete the pointer in iValues
				iValues.Delete(ii);
				iTypes.Delete(ii);
				AdjustBasePointers();
				break;
				}
			typesPtr++;
			}
		}
	}

/** Gets the number of attributes of this node.

@return Number of attributes of this node
*/
EXPORT_C TInt CNode::AttributeCount() const
	{
	return iValues.Count();
	}

/** Gets the attribute value of an attribute at a specified index.

@return Attribute value
@param aIndex Attribute index
*/
EXPORT_C CBase* CNode::AttributeByIndex(TInt aIndex) const
	{
	if(iValues.Count())
		return iValues[aIndex];
	return NULL;
	}

/** Gets the attribute value and type of an attribute at a specified index..

@return Attribute value
@param aIndex Attribute index
@param aType On return, the attribute type
*/
EXPORT_C CBase* CNode::AttributeByIndex(TInt aIndex,TAny*& aType) const
	{
	if(iValues.Count())
		{
		aType=iTypes[aIndex];
		return iValues[aIndex];
		}
	return NULL;
	}

/** Gets the attribute value of an attribute at a specified index

@return Attribute value
@param aIndex Attribute index
*/
EXPORT_C TAny* CNode::AttributeTypeByIndex(TInt aIndex) const
	{
	if(iValues.Count())
		return iTypes[aIndex];
	return NULL;
	}

/** Gets the node type.

@return Node type
*/
EXPORT_C TAny* CNode::Type() const
	{
	return iType;
	}

/** Sets the node type.

@param aType Node type
*/
EXPORT_C void CNode::SetType(TAny* aType)
	{
	iType = aType;
	}

void CNode::Reserved1()
// Reserved for future expansion
	{
	User::Panic(_L("Reserved"),KErrNotSupported);
	}

void CNode::Reserved1() const
// Reserved for future expansion
	{
	User::Panic(_L("Reserved"),KErrNotSupported);
	}

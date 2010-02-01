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

_LIT(KWapBaseNodePanic,"Node-Panic");

#ifndef __WAP_MONOLITHIC__

void Panic(TNodePanic aPanic)
//
//	Panic the client program.
//
	{
	User::Panic(KWapBaseNodePanic,aPanic);
	}
#endif

//
// INLINED Node implementation
// for description of templated api's see CNODE.H
//

//CTOR of non-deletable data
/** Constructor.

@param aData Buffer to wrap
*/
inline CDataNoDelete::CDataNoDelete(HBufC16* aData)
	: iData(aData)
	{
	}

//DTOR doesn't delete the data member
/** Destructor.

The wrapped buffer is not deleted.
*/
inline CDataNoDelete::~CDataNoDelete()
	{
	}

//Accessor method to set the iData pointer to the parameter passed in
//Ownership is taken here
// Returns the previous value
/** Changes the buffer that is wrapped.

@return The previous wrapped buffer
@param aData Buffer to wrap
*/
inline HBufC16* CDataNoDelete::SetData(HBufC16* aData)
	{
	HBufC16* prevVal = iData;
	iData = aData;
	return prevVal;
	}

//Resets data pointer to point to aData, data is not deleted
/** Sets the buffer that is wrapped.

The existing value is forgotten.

@param aData Buffer to wrap
*/
inline void CDataNoDelete::ResetDataPointer(HBufC16 *aData)
	{
	iData = aData;
	}

//Accessor method to get the data
/** Gets the wrapped buffer.

@return The wrapped buffer
*/
inline HBufC16* CDataNoDelete::Data()
	{
	return iData;
	}

//CTOR of deletable data
/** Constructor.

@param aData Buffer to wrap
*/
inline CDataDelete::CDataDelete(HBufC16* aData)
	: CDataNoDelete(aData)
	{
	}

//DTOR of deletable data...DELETES THE DATA
/** Destructor.

The wrapped buffer is deleted.
*/
inline CDataDelete::~CDataDelete()
	{
	delete iData;
	}

/** Sets the buffer that is wrapped.

The existing value is deleted.

@param aData Buffer to wrap
*/
inline void CDataDelete::ResetDataPointer(HBufC16* aData)
	{
	delete iData;
	iData = aData;
	}

//CTOR of deletable file data
/** Constructor.

@param aData Buffer to wrap
*/
inline CFileDataDelete::CFileDataDelete(HBufC16* aData)
	: CDataNoDelete(aData)
	{
	}

// DTOR of deletable file data...
// DELETES THE DATA AFTER REMOVING THE REFERENCED FILE
/** Destructor.

It deletes the filename buffer and the file itself.
*/
inline CFileDataDelete::~CFileDataDelete()
	{
	RemoveFile();
	delete iData;
	}

/** Sets the filename that is wrapped.

The existing filename buffer and the file itself are deleted.

@param aData Buffer to wrap
*/
inline void CFileDataDelete::ResetDataPointer(HBufC16* aData)
	{
	RemoveFile();
	delete iData;
	iData = aData;
	}

inline void CFileDataDelete::RemoveFile()
	{
	// When this panics 
	// Someone somewhere has incorrectly reset this node's data
	__ASSERT_DEBUG(iData,Panic(ENoData));
	RFs fs;
	// If connect fails we can sadly do nothing to remove the file
	// it will be left lying around :-<
	if(fs.Connect() == KErrNone)
		{
		fs.Delete(iData->Des());	
		fs.Close();
		}
	}

//CTOR of wrapper for integer attributes
/** Constructor.

@param aInteger Integer to wrap
*/
inline CIntAttribute::CIntAttribute(TInt aInteger)
	: iInteger(aInteger)
	{
	}

//Accessor method
/** Gets the wrapped integer.

@return The wrapped integer
*/
inline TInt CIntAttribute::Int() const
	{
	return iInteger;
	}


//
//
//TEMPLATED FUNCTIONS SEE CNODE.H FOR DESCRIPTIONS OF API'S
//
//
/** Allocates and constructs a new node.

@return New node
@param aType The type of the node
@param aParent The parent of this node
*/
template <class TNodeType, class TAttributeType>
inline CTypedNode<TNodeType, TAttributeType>* CTypedNode<TNodeType, TAttributeType>::NewL(TNodeType aType, CNode* aParent)
	{
	return (STATIC_CAST(CTypedNode*,CNode::NewL(CONST_CAST(TAny*,REINTERPRET_CAST(const TAny*,aType)),aParent)));
	}


template <class TNodeType, class TAttributeType>
inline CTypedNode<TNodeType, TAttributeType>::CTypedNode(TNodeType aType, CNode* aParent)
	: CNode(CONST_CAST(TAny*,REINTERPRET_CAST(const TAny*,aType)),aParent)
	{}

/** Deletes a specified child node.

@param aNode Node to delete
*/
template <class TNodeType, class TAttributeType>
inline void CTypedNode<TNodeType, TAttributeType>::DeleteChildNode(CNode* aNode)
	{
	CNode::DeleteChildNode(aNode);
	}

/** Deletes all the child nodes of this node.
*/
template <class TNodeType, class TAttributeType>
inline void CTypedNode<TNodeType, TAttributeType>::DeleteAllChildNodes()
	{
	CNode::DeleteAllChildNodes();
	}

/** Creates a new child node.

@return The new child node
@param aType Node type
*/
template <class TNodeType, class TAttributeType>
inline CTypedNode<TNodeType, TAttributeType>& CTypedNode<TNodeType, TAttributeType>::AppendNodeL(TNodeType aType)
	{
	return (STATIC_CAST(CTypedNode& , CNode::AppendNodeL(CONST_CAST(TAny*,REINTERPRET_CAST(TAny*,aType)))));
	}

/** Adds an existing node as a child.

@param aNode Node to make a child
*/
template <class TNodeType, class TAttributeType>
inline void CTypedNode<TNodeType, TAttributeType>::AppendNodeToThisNodeL(CNode* aNode)
	{
	CNode::AppendNodeToThisNodeL(aNode);
	}

/** Gets the first child or the next child after a specified child.

@return First or next child node
@param aNode Child node or NULL to get the first child
*/
template <class TNodeType, class TAttributeType>
inline CTypedNode<TNodeType, TAttributeType>* CTypedNode<TNodeType, TAttributeType>::NextChild(const CNode* aNode) const
	{
	return (STATIC_CAST(CTypedNode*,CNode::NextChild(aNode)));
	}

/** Gets the previous child before a specified child.

@return Previous child node
@param aNode Child node
*/
template <class TNodeType, class TAttributeType>
inline CTypedNode<TNodeType, TAttributeType>* CTypedNode<TNodeType, TAttributeType>::PrevChild(const CNode& aNode) const
	{
	return (STATIC_CAST(CTypedNode*,CNode::PrevChild(aNode)));
	}

/** Gets the parent of this node.

@return Parent
*/
template <class TNodeType, class TAttributeType>
inline CTypedNode<TNodeType, TAttributeType>* CTypedNode<TNodeType, TAttributeType>::Parent() const
	{
	return (STATIC_CAST(CTypedNode*,CNode::Parent()));
	}

/** Changes the parent of the node.

The node is removed from the childlist of its current parent.

@param aParent New parent
*/
template <class TNodeType, class TAttributeType>
inline void CTypedNode<TNodeType, TAttributeType>::ReparentL(CNode* aParent)
	{
	CNode::ReparentL(aParent);
	}

/** Gets the next sibling node.

This asks for the next child of its parent.

@return Next sibling node
*/
template <class TNodeType, class TAttributeType>
inline CTypedNode<TNodeType, TAttributeType>* CTypedNode<TNodeType, TAttributeType>::NextSibling() const
	{
	return (STATIC_CAST(CTypedNode*,CNode::NextSibling()));
	}

/** Gets the previous sibling node.

This asks for the previous child of its parent.

@return Previous sibling node
*/
template <class TNodeType, class TAttributeType>
inline CTypedNode<TNodeType, TAttributeType>* CTypedNode<TNodeType, TAttributeType>::PrevSibling() const
	{
	return (STATIC_CAST(CTypedNode*,CNode::PrevSibling()));
	}

/** Gets the number of children of this node.

@return Number of children of this node
*/
template <class TNodeType, class TAttributeType>
inline TInt CTypedNode<TNodeType, TAttributeType>::NumberImmediateChildren() const
	{
	return (CNode::NumberImmediateChildren());
	}

/** Gets the absolute root node of the tree.

@return Root node
*/
template <class TNodeType, class TAttributeType>
inline const CTypedNode<TNodeType, TAttributeType>& CTypedNode<TNodeType, TAttributeType>::Root() const
	{
	return (STATIC_CAST(const CTypedNode&,CNode::Root()));
	}

/** Sets the node data.

The object will delete the data in its destructor.

@param aData Node data
*/
template <class TNodeType, class TAttributeType>
inline void CTypedNode<TNodeType, TAttributeType>::SetDataL(HBufC16 *aData)
	{
	CNode::SetDataL(aData);
	}

/** Sets the object not to delete the node data in its destructor.

Note that the function internally reallocates memory. If it leaves, the data is lost.
 */
template <class TNodeType, class TAttributeType>
inline void CTypedNode<TNodeType, TAttributeType>::SetDataNoDeleteL()
	{
	CNode::SetDataNoDeleteL();
	}

/** Sets the object to delete the node data in its destructor.

Note that the function internally reallocates memory. If it leaves, the data is lost. */
template <class TNodeType, class TAttributeType>
inline void CTypedNode<TNodeType, TAttributeType>::ClearSetDataNoDeleteL()
	{
	CNode::ClearSetDataNoDeleteL();
	}

/** Sets the node data to be taken from a specified file.

If the data is deleted, the referenced file is also deleted.

@param aData Name of the file containing the data
*/
template <class TNodeType, class TAttributeType>
inline void CTypedNode<TNodeType, TAttributeType>::SetFileDataL(HBufC16 *aData)
	{
	CNode::SetFileDataL(aData);
	}

/** Resets the node data to a specified pointer.

Existing data owned by the node is deleted.

@param aData Root node
*/
template <class TNodeType, class TAttributeType>
inline void CTypedNode<TNodeType, TAttributeType>::ResetDataPointer(HBufC16* aData)
	{
	CNode::ResetDataPointer(aData);
	}

/** Gets the node data.

@return Node data or NULL if no data is set
*/
template <class TNodeType, class TAttributeType>
inline HBufC16* CTypedNode<TNodeType, TAttributeType>::Data() const
	{
	return (CNode::Data());
	}

/** Deletes an attribute of a specified type.

Note that the attribute value will be deleted.

@param aAttributeType Attribute type
*/
template <class TNodeType, class TAttributeType>
inline void CTypedNode<TNodeType, TAttributeType>::DeleteAttribute(TAttributeType aAttributeType)
	{
	CNode::DeleteAttribute(CONST_CAST(TAny*, REINTERPRET_CAST(const TAny*,aAttributeType)));
	}

/** Delete all node attributes.

Note that attribute values will be deleted.
*/
template <class TNodeType, class TAttributeType>
inline void CTypedNode<TNodeType, TAttributeType>::DeleteAllAttributes()
	{
	CNode::DeleteAllAttributes();
	}

/** Removes an attribute of a specified type, but does not delete it.

The caller is now responsible for the destruction of the attribute value.

@param aAttributeType Attribute type
*/
template <class TNodeType, class TAttributeType>
inline void CTypedNode<TNodeType, TAttributeType>::RemoveAttributeNoDelete(TAttributeType aAttributeType)
	{
	CNode::RemoveAttributeNoDelete(CONST_CAST(TAny*,REINTERPRET_CAST(const TAny*,aAttributeType)));
	}

/** Gets the number of attributes of this node.

@return Number of attributes of this node
*/
template <class TNodeType, class TAttributeType>
inline TInt CTypedNode<TNodeType, TAttributeType>::AttributeCount() const
	{
	return(CNode::AttributeCount());
	}

/** Gets the attribute value of an attribute at a specified index

@return Attribute value
@param aIndex Attribute index
*/
template <class TNodeType, class TAttributeType>
inline TAttributeType CTypedNode<TNodeType, TAttributeType>::AttributeTypeByIndex(TInt aIndex) const
	{
	return(REINTERPRET_CAST(TAttributeType, CNode::AttributeTypeByIndex(aIndex)));
	}

/** Gets the attribute value of an attribute at a specified index

@return Attribute value
@param aIndex Attribute index
*/
template <class TNodeType, class TAttributeType>
inline CBase* CTypedNode<TNodeType, TAttributeType>::AttributeByIndex(TInt aIndex) const
	{
	return(CNode::AttributeByIndex(aIndex));
	}

/** Gets the attribute value and type of an attribute at a specified index..

@return Attribute value
@param aIndex Attribute index
@param aType On return, the attribute type
*/
template <class TNodeType, class TAttributeType>
inline CBase* CTypedNode<TNodeType, TAttributeType>::AttributeByIndex(TInt aIndex,TAttributeType& aType) const
	{
	TAny* type;
	CBase* ret=CNode::AttributeByIndex(aIndex,type);
	aType=REINTERPRET_CAST(TAttributeType, type);
	return ret;
	}

/** Adds an attribute.

The node takes ownership of aAttributeValue.

@param aAttributeType Attribute type
@param aAttributeValue Attribute value
*/
template <class TNodeType, class TAttributeType>
inline void CTypedNode<TNodeType, TAttributeType>::AddAttributeL(TAttributeType aAttributeType, CBase* aAttributeValue)
	{
	CNode::AddAttributeL(CONST_CAST(TAny*, REINTERPRET_CAST(const TAny*,aAttributeType)),aAttributeValue);
	}

/** Sets node data and adds an attribute.

The node takes ownership of aDataand aAttributeValue. 
Existing node data owned by the node is deleted.

@param aData Node data
@param aAttributeType Attribute type
@param aAttributeValue Attribute value
*/
template <class TNodeType, class TAttributeType>
inline void CTypedNode<TNodeType, TAttributeType>::AddDataAndAttributeL(HBufC16 *aData, TAttributeType aAttributeType, CBase* aAttributeValue)
	{
	CNode::AddDataAndAttributeL(aData,CONST_CAST(TAny*, REINTERPRET_CAST(const TAny*,aAttributeType)),aAttributeValue);
	}

/** Gets a child node by index.

@return Child node
@param aByIndex Index of the child node
*/
template <class TNodeType, class TAttributeType>
inline CTypedNode<TNodeType, TAttributeType>* CTypedNode<TNodeType, TAttributeType>::Child(TInt aByIndex) const
	{
	return (REINTERPRET_CAST(CTypedNode*,CNode::Child(aByIndex)));
	}

/** Gets an attribute value for a specified attribute type.

@return Attribute value
@param aAttributeType Attribute type
*/
template <class TNodeType, class TAttributeType>
inline CBase* CTypedNode<TNodeType, TAttributeType>::Attribute(TAttributeType aAttributeType) const
	{
	return (CNode::Attribute(CONST_CAST(TAny*, REINTERPRET_CAST(const TAny*,aAttributeType))));
	}

/** Tests if an attribute of a specified type exists.

@return True if the attribute exists, otherwise false
@param aAttributeType Attribute type
*/
template <class TNodeType, class TAttributeType>
inline TBool CTypedNode<TNodeType, TAttributeType>::AttributeExists(TAttributeType aAttributeType) const
	{
	return (CNode::AttributeExists(CONST_CAST(TAny*, REINTERPRET_CAST(const TAny*,aAttributeType))));
	}

/** Gets the node type.

@return Node type
*/
template <class TNodeType, class TAttributeType>
inline TNodeType CTypedNode<TNodeType, TAttributeType>::Type() const
	{
	return (REINTERPRET_CAST(TNodeType,CNode::Type()));
	}

/** Sets the node type.

@param aType Node type
*/
template <class TNodeType, class TAttributeType>
inline void CTypedNode<TNodeType, TAttributeType>::SetType(TNodeType aType)
	{
	CNode::SetType(CONST_CAST(TAny*,REINTERPRET_CAST(const TAny* ,aType)));
	}


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
// This contains the definitions of CDataDelete, CDataNoDelete, CIntAttribute, CNode, and CTypedNode.
// CDataNoDelete is a base class to CDataDelete and are essentially a wrapper around an HBufC16.
// The node owns the data that is added to it and therefore is responsible for deleting all its data,
// hence theses two classes allow a user to have non deletable data. Internally to the node it is also 
// sometimes necessary to change the data into either deletable or non-deletable data api's are provided
// for this.
// CIntAttribute is wrapper around a TInt, this is provided as the nodes attribute value is a CBase* however
// it might be desirable to store integer's in here. Attribute value is owned by the node therefore the node is
// responsible for deleting it.
// CNode is the basis for constructing a tree. It consists of an array of child nodes, node type, a parent node,
// an array of attributes, and a data member. Internally the data member is defined as CDataNoDelete however
// to the user all the exported api's take an HBufC16*. Data is owned by the node and is destroyed by the node,
// However in certain circumstances this is not desirable hence the api's to make the data non-deletable. The
// node type is defined as a TAny* however normal usage would use the templated node - CTypedNode. The node type
// is used to identify groups of nodes. The attribute array is fairy simple and consists of AttributeType and 
// AttributeValue. AttributeType can be defined by using the templated class and should be a 32bit value. AttributeValue
// can be any object derived from CBase and the node takes ownership therefore the node delete's it.
// Basic usage should be to use the templated class in order to make use of the templated types (TNodeType,TAttributeType).
// Create a node for example:
// CTypedNode<CNode*, const TDesC*> *tree = CTypedNode<CNode*, const TDesC*>::NewL(0,0);
// add a new child:
// CNODE *TestChildNode1 = tree->AppendNodeL(aTempNodeForNodeType);
// add some data:
// TestChildNode1->SetDataL(aHBufC16);
// add an attribute:
// TestChildNode1->->AddAttributeL(aTAttributeType,aCBasePointerAttributeValue);
// Explanation of individual api's is documented below.
//

 

#ifndef __CNODE_H__
#define __CNODE_H__

#include <e32base.h>
#include <f32file.h>


/**
	@file
	@publishedAll
	@deprecated
*/

//Granularity of arrays
const TInt KGranularity = 5;

//enum of panic reasons
enum TNodePanic
	{
	ENodeBadArgument,
	ENodeNoChildren,
	ENoData,
	EAttributeFailure
	};

//node panic function
inline void Panic(TNodePanic aPanic);

//Wrapper around an HBufC16 doesn't delete data
//##ModelId=3B666BCC0303
class CDataNoDelete : public CBase
/** Provides a wrapper around an HBufC16: the buffer is not deleted when the object is deleted.
*/
	{
public:
	//##ModelId=3B666BCC032D
	CDataNoDelete(HBufC16* aData);
	//##ModelId=3B666BCC032C
	virtual ~CDataNoDelete();
	//##ModelId=3B666BCC0324
	HBufC16* SetData(HBufC16* aData);
	//##ModelId=3B666BCC0322
	virtual void ResetDataPointer(HBufC16* aData);
	//##ModelId=3B666BCC0321
	HBufC16* Data();

protected:
	/** The wrapped buffer. */
	//##ModelId=3B666BCC0319
	HBufC16* iData;
	};

//Wrapper around an HBufC16 does delete data
//##ModelId=3B666BCC0399
class CDataDelete : public CDataNoDelete
/** Provides a wrapper around an HBufC16: the buffer is deleted when the 
object is deleted.
*/
	{
public:
	//##ModelId=3B666BCC03B7
	CDataDelete(HBufC16* aData);
	//##ModelId=3B666BCC03B0
	virtual ~CDataDelete();
	//##ModelId=3B666BCC03AE
	virtual void ResetDataPointer(HBufC16* aData);
	};

//Wrapper around an HBufC16 does delete data (FileName)
// After Removing referenced File 
//##ModelId=3B666BC7026F
class CFileDataDelete : public CDataNoDelete
/** Provides a wrapper around a filename: the referenced file is deleted when the object is deleted.
*/
	{
public:
	//##ModelId=3B666BC7028F
	CFileDataDelete(HBufC16* aData);
	//##ModelId=3B666BC7028E
	virtual ~CFileDataDelete();
	//##ModelId=3B666BC70285
	virtual void ResetDataPointer(HBufC16* aData);
private:
	//##ModelId=3B666BC70284
	void RemoveFile();
	};

//Wrapper around a TInt used for attribute values in order to allow the node to call a dtor
//##ModelId=3B666BC6023E
class CIntAttribute : public CBase
/** Provides an object wrapper around a TInt value. */
	{
public:
	//##ModelId=3B666BC6025B
	inline CIntAttribute(TInt aInteger);		
	//##ModelId=3B666BC6025A
	inline TInt Int() const;
private:
	//##ModelId=3B666BC60264
	CIntAttribute();
	//##ModelId=3B666BC60253
	TInt iInteger;
	};

//
// Node class
// Normal usage would be to use CTypedNode in order to use specific templated types
//

//##ModelId=3B666BCD02D2
class CNode : public CBase 
	{
public:
	//##ModelId=3B666BCE0139
	IMPORT_C ~CNode();

	//NewL parameters aType to indentify the type of node, CNode* to set the parent of this node
	//##ModelId=3B666BCE0125
	IMPORT_C static CNode* NewL(TAny* aType,CNode* aParent);

	//Deletes a child node which is passed in as a parameter
	//##ModelId=3B666BCE011C
	IMPORT_C void DeleteChildNode(CNode* aNode);

	//Deletes all the child nodes haging of this node
	//##ModelId=3B666BCE011B
	IMPORT_C void DeleteAllChildNodes();

	//Creates a new node and adds it to the childlist, reference to new node is returned. Can leave if it can't allocate memory for a new node
	//Type of node is the parameter
	//##ModelId=3B666BCE0108
	IMPORT_C CNode& AppendNodeL(TAny* aType = 0);

	//Appends a node which is passed in as a parameter to this node, so its added to the childlist
	//##ModelId=3B666BCE00FD
	IMPORT_C void AppendNodeToThisNodeL(CNode* aNode);

	//Sets the data in the node to the parameter that is passed in. It's deletable and the node will delete it in dtor
	//Push aDataNowNodeOwns onto the CleanupStack before calling then pop off on return
	//##ModelId=3B666BCE00F3
	IMPORT_C void SetDataL(HBufC16* aDataNowNodeOwns);

	//returns the data stored in the node, which is returned as an HBufC16*
	//##ModelId=3B666BCE00EB
	IMPORT_C HBufC16* Data() const;

	//WARNING this function can leave as it deletes the wrapper object and then creates
	//a new non deletable wrapper object and sets the data pointer inside the new object
	//IF THIS LEAVES YOU WILL LOSE YOUR DATA
	//##ModelId=3B666BCE00EA
	IMPORT_C void SetDataNoDeleteL();

	//WARNING this function can leave as it deletes the wrapper object and then creates
	//a new deletable wrapper object then sets the pointer in the new object
	//##ModelId=3B666BCE00E9
	IMPORT_C void ClearSetDataNoDeleteL();

	// Sets the data in the node to the FileName parameter that is passed in. 
	// It's deletable and the node will delete it, and the file it refers to in dtor
	// Push aDataLocationNowNodeOwns onto the CleanupStack before calling then pop off on return
	//##ModelId=3B666BCE00DF
	IMPORT_C void SetFileDataL(HBufC16* aFileDataLocationNowNodeOwns);

	//Resets the data pointer to point to aData parameter will delete the data that is owned by the node
	//##ModelId=3B666BCE00D5
	IMPORT_C void ResetDataPointer(HBufC16* aData);

	//returns a reference to the absolute root of the tree
	//##ModelId=3B666BCE00CB
	IMPORT_C const CNode& Root() const;

	//returns a child node which is stored in the array of children. The child node is accessed by the index that is the parameter.
	//##ModelId=3B666BCE00C1
	IMPORT_C CNode* Child(TInt aByIndex) const;

	//Returns either the first child of this node if the parameter is NULL. Or it returns the next chld in the array after
	//the child passed in as a parameter
	//##ModelId=3B666BCE00AE
	IMPORT_C CNode* NextChild(const CNode* aNode = NULL) const;

	//returns the previous child to the child passed in as a parameter. The node parameter is a reference because its the child previous
	//to the node passed in which obviously must exist
	//##ModelId=3B666BCE00A4
	IMPORT_C CNode* PrevChild(const CNode& aNode) const;

	//Returns the parent of this node
	//##ModelId=3B666BCE00A3
	IMPORT_C CNode* Parent() const;

	//WARNING this function can leave as it calls AppendNodeToThisNode. The aParent parameter is the node to which you would like to make
	//the parent of 'this' node.
	//It removes itself from the childlist of it's current parent
	//##ModelId=3B666BCE0099
	IMPORT_C void ReparentL(CNode* aParent);

	//Returns the next sibling which means in effect that it asks for the next child of its parent.
	//Sibling means brother or sister.
	//##ModelId=3B666BCE0090
	IMPORT_C CNode* NextSibling() const;

	//Returns the previous sibling which means in effect that it asks for the previous child of its parent.
	//Sibling means brother or sister.
	//##ModelId=3B666BCE008F
	IMPORT_C CNode* PrevSibling() const;

	//returns the number of children that this node has
	//##ModelId=3B666BCE0085
	IMPORT_C TInt NumberImmediateChildren() const;

	//Deletes the attribute of which is of aAttributeType (the parameter)
	//WARNING Attribute values of nodes will be deleted as the node owns them if you don't want it deleted then use a wrapper
	//##ModelId=3B666BCE007C
	IMPORT_C void DeleteAttribute(TAny* aAttributeType);

	//Deletes all the attributes of this node
	//WARNING Attribute values of nodes will be deleted as the node owns them if you don't want it deleted then use a wrapper
	//##ModelId=3B666BCE007B
	IMPORT_C void DeleteAllAttributes();

	//remove an attribute without deleting it, this is done by simply setting the attributeValue pointer to NULL
	//WARNING you are now responsiblefor the destruction of this attribute value
	//##ModelId=3B666BCE0071
	IMPORT_C void RemoveAttributeNoDelete(TAny* aAttributeType);

	// Returns the number of attributes that this node has
	//##ModelId=3B666BCE0067
	IMPORT_C TInt AttributeCount() const;

	//Returns the type of attribute (AttributeType) at a given index...NOT the attributeValue
	//##ModelId=3B666BCE005D
	IMPORT_C TAny* AttributeTypeByIndex(TInt aIndex) const; 

	//Returns the value of an attribute (AttributeValue) at a given index...NOT the attributeType
	//##ModelId=3B666BCE003F
	IMPORT_C CBase* AttributeByIndex(TInt aIndex) const;
	//##ModelId=3B666BCE0049
	IMPORT_C CBase* AttributeByIndex(TInt aIndex,TAny*& aType) const;

	//Adds an attribute, parameters are the type of attribute and its value
	//WARNING node takes ownership of aAttributeValue
	//Push aAttributeValue onto the CleanupStack before calling then pop off on return
	//##ModelId=3B666BCE002B
	IMPORT_C void AddAttributeL(TAny* AttributeType, CBase* aAttributeValue);

	//Adds data to the node and also adds an attribute, parameters are the Data to be added, the type of attribute and its value
	//WARNING node takes ownership of aData and aAttributeValue
	//Push aAttributeValue and aData onto the CleanupStack before calling then pop off on return
	//##ModelId=3B666BCE000D
	IMPORT_C void AddDataAndAttributeL(HBufC16 *aData, TAny* AttributeType, CBase* aAttributeValue);

	//Returns an attribute value for the given AttributeType(the parameter)
	//##ModelId=3B666BCE0003
	IMPORT_C CBase* Attribute(TAny* AttributeType) const;

	//Returns TRUE if the attribute of the given type exists
	//##ModelId=3B666BCD03E1
	IMPORT_C TBool AttributeExists(TAny* aAttributeType) const;

	//Returns the node type
	//##ModelId=3B666BCD03D7
	IMPORT_C TAny* Type() const;

	//Sets the node type to be aType (the parameter)
	//##ModelId=3B666BCD03CD
	IMPORT_C void SetType(TAny* aType);


protected:
	//ctor
	//##ModelId=3B666BCD03B9
	CNode(TAny* aType, CNode* aParent);

	//internal finds a child which is passed in as a parameter
	//##ModelId=3B666BCD03AF
	TInt FindChild(const CNode* aNode) const;

	//##ModelId=3B666BCD03A7
	HBufC16* SetupDeletableOrNonDeleteableDataLC();
	//##ModelId=3B666BCD03A6
	void AdjustBasePointers();
private:
	// Reserved for future expansion
	//##ModelId=3B666BCD039B
	virtual void Reserved1();			
	//##ModelId=3B666BCD03A5
	virtual void Reserved1() const;

protected:
	//the Type of Node
	//##ModelId=3B666BCD0392
	TAny* iType;

	//This Nodes parent
	//##ModelId=3B666BCD037E
	CNode *iParent;

	// stores attribute type and value. iTypes must be Flat array
	//##ModelId=3B666BCD0372
	CArrayPtrFlat<CBase> iValues;
	//##ModelId=3B666BCD034C
	CArrayPtrFlat<TAny> iTypes;

	//##ModelId=3B666BCD0324
	TInt32* iTypesBasePtr;
	//##ModelId=3B666BCD0310
	CDataNoDelete* iDataValue;
//
	//An array of child nodes
	//##ModelId=3B666BCD02FC
	CArrayPtr<CNode> *iChildList;
private:
	//##ModelId=3B666BCD02DF
	TAny*	iReserved;						// Reserved for future expansion
	};


//CTypedNode is derived from CNode and is a thin template. TNodeType you should define as a 32 bit value
//TAttributeType should be defined as a 32 bit value
//FOR EXPLANATION OF API'S SEE ABOVE 
//##ModelId=3B666BCC01A4
template <class TNodeType, class TAttributeType>
class CTypedNode : public CNode 
/** Template class for a node in a node tree.

The node type is set to the template parameter TNodeType and the attribute type to 
TAttributeType. These parameters should be pointers to the type required to store 
the type value: e.g. for a string, a const TDesC*.

The class is thin template over CNode. */
	{
public:
	//##ModelId=3B666BCC024A
	inline static CTypedNode* NewL(TNodeType aType,CNode* aParent);
	//##ModelId=3B666BCC0248
	inline void DeleteChildNode(CNode* aNode);
	//##ModelId=3B666BCC0247
	inline void DeleteAllChildNodes();
	//##ModelId=3B666BCC0245
	inline CTypedNode<TNodeType,TAttributeType>& AppendNodeL(TNodeType aType);
	//##ModelId=3B666BCC023B
	inline void AppendNodeToThisNodeL(CNode* aNode);
	//##ModelId=3B666BCC0228
	inline void SetDataL(HBufC16* aDataNowNodeOwns);
	//##ModelId=3B666BCC0227
	inline void SetDataNoDeleteL();
	//##ModelId=3B666BCC0221
	inline void ClearSetDataNoDeleteL();
	//##ModelId=3B666BCC021F
	inline void SetFileDataL(HBufC16* aFileDataLocationNowNodeOwns);
	//##ModelId=3B666BCC021D
	inline void ResetDataPointer(HBufC16* aData);
	//##ModelId=3B666BCC0215
	inline HBufC16* Data() const;
	//##ModelId=3B666BCC0214
	inline const CTypedNode& Root() const;
	//##ModelId=3B666BCC020B
	inline CTypedNode* Child(TInt aByIndex) const;
	//##ModelId=3B666BCC0209
	inline CTypedNode* NextChild(const CNode* aNode = NULL) const;
	//##ModelId=3B666BCC0201
	inline CTypedNode* PrevChild( const CNode& aNode) const;
	//##ModelId=3B666BCC0200
	inline CTypedNode* Parent() const;
	//##ModelId=3B666BCC01FE
	inline void ReparentL(CNode* aParent);
	//##ModelId=3B666BCC01F5
	inline CTypedNode* NextSibling() const;
	//##ModelId=3B666BCC01F4
	inline CTypedNode* PrevSibling() const;
	//##ModelId=3B666BCC01EE
	inline TInt NumberImmediateChildren() const;
	//##ModelId=3B666BCC01EC
	inline void DeleteAttribute(TAttributeType aAttributeType);
	//##ModelId=3B666BCC01EB
	inline void DeleteAllAttributes();
	//##ModelId=3B666BCC01E5
	inline void RemoveAttributeNoDelete(TAttributeType aAttributeType);
	//##ModelId=3B666BCC01E4
	inline TInt AttributeCount() const; // Returns the number of attributes
	//##ModelId=3B666BCC01E2
	inline TAttributeType AttributeTypeByIndex(TInt aIndex) const; 
	//##ModelId=3B666BCC01DC
	inline CBase* AttributeByIndex(TInt aIndex) const; 
	//##ModelId=3B666BCC01DE
	inline CBase* AttributeByIndex(TInt aIndex,TAttributeType& aType) const; 
	//##ModelId=3B666BCC01D9
	inline void AddAttributeL(TAttributeType aAttributeType, CBase* aAttributeValue);
	//##ModelId=3B666BCC01D0
	inline void AddDataAndAttributeL(HBufC16 *aData, TAttributeType aAttributeType, CBase* aAttributeValue);
	//##ModelId=3B666BCC01CE
	inline CBase* Attribute(TAttributeType aAttributeType) const;
	//##ModelId=3B666BCC01CC
	inline TBool AttributeExists(TAttributeType aAttributeType) const;
	//##ModelId=3B666BCC01C9
	inline TNodeType Type() const;
	//##ModelId=3B666BCC01C7
	inline void SetType(TNodeType aType);
protected:
	//##ModelId=3B666BCC01C4
	CTypedNode(TNodeType aType, CNode* aParent);
	};
#include <cnode.inl>
#endif

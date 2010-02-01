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
// XmlElement.h
// XML Element class
// Derived from CTypedNode. Adds functionality
// to make the use of the node simpler
// 
//

#ifndef __XMLELEMT_H
#define __XMLELEMT_H

//
//  INCLUDES
//
#include <e32base.h>
#include <cnode.h>

//
//  CONSTANTS  
//
/**
@publishedAll
@deprecated
*/
_LIT(KXmlElementDataItemAttributeName, "__XML_DI");

//
//  DATA TYPES  
//
enum TXmlElementPanic
/**
@publishedAll
@deprecated
*/
{
	EXmlElPanicInvalidDataItem = 1
};

// Type of an attribute in a node
typedef const TDesC* TXmlAttributeType;
// Type of an element (type of node)
typedef const TDesC* TXmlElementType;

//
//  FUNCTION PROTOTYPES  
//
/**
@publishedAll
@deprecated
*/
GLREF_C void Panic(TXmlElementPanic aPanic);

//
//  FORWARD DECLARATIONS
//
class RFile;
class CAttributeLookupTable;

//
//  CLASS DEFINITIONS 
//
//##ModelId=3B66798702FF
class CXmlElement : public CTypedNode<TXmlElementType, TXmlAttributeType>
/**
@publishedAll
@deprecated
*/
{
public:
	// Construction
	//##ModelId=3B6679870364
	static inline CXmlElement* NewL(TXmlElementType aType,CNode* aParent);

	// Attributes
	// Return the value of a particular attribute
	//##ModelId=3B667987035D
	IMPORT_C const TDesC* Attribute(const TDesC& aAttributeName) const;
	// Return the value of an aIndexth attribute
	//##ModelId=3B6679870362
	inline const TDesC* Attribute(TInt aIndex) const;

	// Set attribute value
	//##ModelId=3B6679870359
	IMPORT_C void SetAttributeL(const TDesC& aAttributeName, const TDesC& aAttributeValue, CAttributeLookupTable& aAttributeLUT);

	// Store a unicode descriptor as data value. 
	//##ModelId=3B6679870357
	IMPORT_C void SetTextL(const TDesC& aData);
	// Get data value
	//##ModelId=3B6679870352
	inline const TDesC* Text() const;

	// Children
	//##ModelId=3B6679870350
	inline void AppendChildL(CXmlElement* aElement);

	//##ModelId=3B667987034D
	IMPORT_C void InsertChildL(TInt aIndexToInsertBefore, CXmlElement* aElement);

	//##ModelId=3B6679870347
	inline TInt Count() const;

	//##ModelId=3B6679870345
	inline CXmlElement* Child(TInt aIndex) const;

	// Find out the index of this child
	// INPUT:
	// aChild - Pointer to the child node
	// RETURN:
	// TInt - index to the child, -1 if no child found
	//##ModelId=3B6679870343
	IMPORT_C TInt ChildIndex(CXmlElement* aChild) const;

	// Data item
	//##ModelId=3B667987033C
	IMPORT_C void SetDataItemL(CBase* aDataItem, CAttributeLookupTable& aAttributeLUT); // takes ownership of aDataItem
	//##ModelId=3B667987033B
	IMPORT_C CBase* DataItem() const;
	//##ModelId=3B6679870339
	IMPORT_C void DeleteDataItem(CAttributeLookupTable& aAttributeLUT);

private:
	// Data Item handling
	class CDataItemAttribute : public CBase
	{
	public:
		~CDataItemAttribute() { delete iDataItem; };
		CBase* iDataItem;
	};

	//##ModelId=3B6679870335
	CXmlElement() : CTypedNode<TXmlElementType, TXmlAttributeType>(NULL, NULL) {};

// Debug stuff
//#ifdef _DEBUG
public:
	//##ModelId=3B6679870334
	IMPORT_C void WriteIntoFileL();
	//##ModelId=3B6679870331
	void StartWriteL(RFile& aFile, TInt aIndentation);
private:
	//##ModelId=3B6679870327
	void WriteNameL(RFile& aFile, TInt aIndentation);
//#endif
};

//
//	INLINE METHODS
//
inline CXmlElement* CXmlElement::NewL(TXmlElementType aType,CNode* aParent) 
{
	return (CXmlElement*)CTypedNode<TXmlElementType, TXmlAttributeType>::NewL(aType, aParent);
}

inline const TDesC* CXmlElement::Attribute(TInt aIndex) const
{ 
	return (TDesC*)((CDataDelete*)AttributeByIndex(aIndex))->Data();
}

inline const TDesC* CXmlElement::Text() const
{ 
	return (const TDesC*)CTypedNode<TXmlElementType, TXmlAttributeType>::Data(); 
}

inline void CXmlElement::AppendChildL(CXmlElement* aElement)
{ 
	CTypedNode<TXmlElementType, TXmlAttributeType>::AppendNodeToThisNodeL(aElement); 
}

inline TInt CXmlElement::Count() const
{ 
	return NumberImmediateChildren(); 
}

inline CXmlElement* CXmlElement::Child(TInt aIndex) const
{ 
	return (CXmlElement*)CTypedNode<TXmlElementType, TXmlAttributeType>::Child(aIndex); 
}

#endif //__XMLELEMT_H

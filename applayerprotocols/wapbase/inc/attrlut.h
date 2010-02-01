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
// Attribute Look-Up-Table header
// The Attribute Lookup Table maintains a global set of unique IDs to be defined for document
// node attributes and language tags. It does this by generating a unique ID for any descriptor
// passed to it. This descriptor is in fact a pointer to a copy of the string. Once a string
// has been seen once, any further references to it will map back to the same ID (pointer)
// value.
// 
//

#if !defined(__CWAPENGINE_LUT_H__)
#define __CWAPENGINE_LUT_H__

#include <e32base.h>

class RTest;
class CAttributeLookupTableNode;

//##ModelId=3B666BC70189

class CAttributeLookupTable : public CBase
/** Provides an attribute lookup table, which maintains a global set of unique IDs to be defined 
	for document node attributes and language tags. 

	It does this by generating a unique (in the scope of the lookup table object) ID for any string 
	passed to it. Once a string has been seen once, any further references to it will map back to the 
	same ID value. 

	@publishedAll
	@deprecated
*/
{
public:
	//##ModelId=3B666BC701E3
	IMPORT_C ~CAttributeLookupTable();
	//##ModelId=3B666BC701DC
	IMPORT_C static CAttributeLookupTable* NewL();
	//##ModelId=3B666BC701DA
	IMPORT_C const HBufC* Des2IDL( const TDesC& ); // map descriptor to ID
	//##ModelId=3B666BC701D3
	IMPORT_C const HBufC* KeyExists( const TDesC& ); // check to see if the given descriptor already exists
	//##ModelId=3B666BC701D2
	IMPORT_C void Reset(); // clear the table

	// debug hook
	typedef enum{
		EPrint,
		EPrintWholeTable
	} TDebugSelectorType;
	//##ModelId=3B666BC701C6
	IMPORT_C void Debug( TDebugSelectorType aSelector, TAny* aParam1, TAny* aParam2 );

private:
	//##ModelId=3B666BC701C5
	CAttributeLookupTable();
	//##ModelId=3B666BC701BF
	void ConstructL();
	//##ModelId=3B666BC701BD
	CAttributeLookupTableNode* FindDes( const TDesC& );
	//##ModelId=3B666BC701BB
	TUint Hash( const TDesC& );
	//##ModelId=3B666BC701A9
	CArrayPtrFlat<CArrayPtrSeg<CAttributeLookupTableNode> >* iList;
};


#endif // __CWAPENGINE_LUT_H__

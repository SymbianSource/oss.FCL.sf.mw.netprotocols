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
// Attribute Look-Up-Table
// The Attribute Lookup Table maintains a global set of unique IDs to be defined for document
// node attributes and language tags. It does this by generating a unique ID for any descriptor
// passed to it. This descriptor is in fact a pointer to a copy of the string. Once a string
// has been seen once, any further references to it will map back to the same ID (pointer)
// value.
// 
//

// system includes
// -
#include <e32base.h>
#include <e32test.h>

// our include
//
#include <attrlut.h>

// Hash table size
//
#define KHashModulo 32


//==============================================================
// CAttributeLookupTableNode
//==============================================================

// Internal node class
//
//##ModelId=3B666737014E
class CAttributeLookupTableNode : public CBase
{
public:
	//##ModelId=3B6667370176
	CAttributeLookupTableNode() : CBase()
		{
		//iDes = 0;
		}
	//##ModelId=3B666737016C
	~CAttributeLookupTableNode()
		{
		delete iDes;
		}
	//##ModelId=3B6667370165
	HBufC* iDes;
};


//==============================================================
// CAttributeLookupTable
//==============================================================

CAttributeLookupTable::CAttributeLookupTable() : CBase()
	{
	}

/** Destructor.
*/
EXPORT_C CAttributeLookupTable::~CAttributeLookupTable()
	{
	Reset();
	delete iList;
	}

void CAttributeLookupTable::ConstructL()
	{
	iList = new (ELeave) CArrayPtrFlat<CArrayPtrSeg<CAttributeLookupTableNode> >( KHashModulo );
	iList->ResizeL( KHashModulo );
	for ( TInt i = 0; i < KHashModulo; i++ )
		{
		(*iList)[i] = NULL;
		}
	}

/** Allocates and constructs a new attribute lookup table.

@return New attribute lookup table
*/
EXPORT_C CAttributeLookupTable* CAttributeLookupTable::NewL()
	{
	CAttributeLookupTable* iLUT = new (ELeave) CAttributeLookupTable();
	CleanupStack::PushL( iLUT );
	iLUT->ConstructL();
	CleanupStack::Pop();
	return iLUT;
	}


// Lookup with allocating
//
/** Gets a unique ID for a specified descriptor.

If this is the first time the descriptor has been used, a new ID will be allocated.

@return ID
@param aAttributeName Descriptor to get ID for
*/
EXPORT_C const HBufC* CAttributeLookupTable::Des2IDL( const TDesC& aAttributeName )
	{
	// lookup the attribute
	CAttributeLookupTableNode* node = FindDes( aAttributeName );
	if ( node )
		return node->iDes;

	// create a new node at the end of the appropriate array
	CAttributeLookupTableNode* newnode = new (ELeave) CAttributeLookupTableNode();
	CleanupStack::PushL( newnode );
	newnode->iDes = aAttributeName.AllocL();

	TInt hash = Hash( aAttributeName );
	CArrayPtrSeg<CAttributeLookupTableNode>* list = iList->At( hash );
	if ( !list )
		list = iList->At( hash ) = new (ELeave) CArrayPtrSeg<CAttributeLookupTableNode>( 2 );

	list->AppendL(newnode);

	CleanupStack::Pop(); // newnode

	return newnode->iDes;
	}


// Lookup without allocating
//
/** Tests if an ID has been allocated for a specified descriptor.

@return ID, or NULL if none has been allocated
@param aKey Descriptor to test
*/
EXPORT_C const HBufC* CAttributeLookupTable::KeyExists( const TDesC& aKey )
	{
	CAttributeLookupTableNode* node = FindDes( aKey );
	if ( node )
		return node->iDes;
	return NULL; // 0 is an invalid ID
	}


// Find the given descriptor in the hash table
//
CAttributeLookupTableNode* CAttributeLookupTable::FindDes( const TDesC& aAttributeName )
	{
	CArrayPtrSeg<CAttributeLookupTableNode>* list = iList->At( Hash( aAttributeName ) );
	if ( list )
		{
		TInt length=aAttributeName.Length();
		TInt count = list->Count();
		for ( TInt i = 0; i < count; i++ )
			{
			CAttributeLookupTableNode* node = list->At(i);
			if ( node->iDes->Length()==length && node->iDes->Compare( aAttributeName ) == 0 ) // ** CASE SENSITIVE **
				return node;
			}
		}
	return NULL;
	}


// Generate a hash value
//
TUint CAttributeLookupTable::Hash( const TDesC& aDes )
	{
	// *** CASE SENSITIVE **
	TInt len=aDes.Length();
	TUint hash = 0;
	const TText* ptr=aDes.Ptr();
	for ( TInt i = 0; i < len; i++ )
		hash = 131*hash + *ptr++;
	return hash % KHashModulo;
	}


// Empty the table
//
/** Clears all allocated IDs.
*/
EXPORT_C void CAttributeLookupTable::Reset()
	{
	if ( iList )
		{
		for ( TInt i = 0; i < iList->Count(); i++ )
			{
			CArrayPtrSeg<CAttributeLookupTableNode>* list = iList->At( i );
			if ( list )
				{
				list->ResetAndDestroy();
				delete list;
				iList->At( i ) = 0;
				}
			}
		}
	}


// Debug hook
//
#if defined (_DEBUG)
EXPORT_C void CAttributeLookupTable::Debug(TDebugSelectorType aSelector, TAny* aParam1, TAny* /*aParam2*/)
	{
	switch(aSelector)
		{
		case EPrint:
			{
			RTest* console = REINTERPRET_CAST(RTest*,aParam1);
			for ( TInt i = 0; i < iList->Count(); i++ )
				{
				if ( i == 0 || (i%15) == 0 )
					console->Printf( _L("\n%d: "), i );
				TInt count = 0;
				CArrayPtrSeg<CAttributeLookupTableNode>* list = iList->At(i);
				if ( list )
					count = list->Count();
				console->Printf( _L("%d\t"), count );
				}
			console->Printf( _L("\n") );
			break;
			}
		default:
			break;
		}	
	}
#else
EXPORT_C void CAttributeLookupTable::Debug(TDebugSelectorType /*aSelector*/, TAny* /*aParam1*/, TAny* /*aParam2*/)
	{
	}
#endif

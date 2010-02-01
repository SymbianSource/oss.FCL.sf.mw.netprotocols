// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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

inline TOwnerIdMapping::TOwnerIdMapping()
	: iRepUid(0), iOwner(0)
	{
	}
	
inline TOwnerIdMapping::TOwnerIdMapping(TUint32 aRepUid, TUint32 aOwner)
	: iRepUid(aRepUid), iOwner(aOwner)
	{
	}


inline TInt TServerResources::AddOwnerIdLookupMapping(const TUint32 aRepositoryUid, const TUint32 aOwnerUid) 
	{
	TOwnerIdMapping lookupTableEntry (aRepositoryUid, aOwnerUid);
	return iOwnerIdLookUpTable.InsertInOrder(lookupTableEntry, TLinearOrder<TOwnerIdMapping>(TOwnerIdMapping::CompareUids));
	}
	
inline TInt TServerResources::FindOwnerIdLookupMapping(TUint32 aRepositoryId) 
	{
	TOwnerIdMapping lookupTableEntry ;
	lookupTableEntry.iRepUid = aRepositoryId ;

	return TServerResources::iOwnerIdLookUpTable.FindInOrder(lookupTableEntry, TLinearOrder<TOwnerIdMapping>(TOwnerIdMapping::CompareUids)) ;
	}

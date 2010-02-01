// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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

inline RSettingsArray::RSettingsArray() :
	RArray<TServerSetting>(KGranularity, _FOFF(TServerSetting, iKey))
	{iIsDefault=EFalse;}

inline RSettingsArray::~RSettingsArray()
	{ Reset(); }

inline TServerSetting* RSettingsArray::Find(TInt aKey) const
	{
	TInt i = FindInUnsignedKeyOrder(TServerSetting(aKey));
	return i>=0 ? const_cast<TServerSetting*>(&operator[](i)) : (TServerSetting*)0;
	}

inline TInt RSettingsArray::FindIndex(const TServerSetting& aSetting) const
	{
	return FindInUnsignedKeyOrder(aSetting);
	}

inline void RSettingsArray::OrderedInsertL(const TServerSetting& aEntry)
	{ InsertInUnsignedKeyOrderL(aEntry); }

inline TInt RSettingsArray::OrderedInsert(const TServerSetting& aEntry)
	{ return InsertInUnsignedKeyOrder(aEntry); }

inline TInt RSettingsArray::Count() const
	{ return RArray<TServerSetting>::Count(); }

inline const TServerSetting& RSettingsArray::operator[](TInt aIndex) const
	{
	return RArray<TServerSetting>::operator[](aIndex);
	}

inline TServerSetting& RSettingsArray::operator[](TInt aIndex)
	{
	return RArray<TServerSetting>::operator[](aIndex);
	}

inline void RSettingsArray::Remove(TInt aId)
	{
	TInt i = FindInUnsignedKeyOrder(TServerSetting(aId));
	if(i>=0)
		DeleteElement(i);
	}

inline void RSettingsArray::Reset()
	{
	for(TInt i=Count()-1; i>=0; i--)
		{
		operator[](i).Reset();
		}
	RArray<TServerSetting>::Reset();
	}

inline void RSettingsArray::DeleteElement(TInt aIndex)
	{
	operator[](aIndex).Reset();
	RArray<TServerSetting>::Remove(aIndex);
	}

inline void RSettingsArray::RemoveElement(TInt aIndex)
	{
	RArray<TServerSetting>::Remove(aIndex);
	}

inline void RSettingsArray::AdoptL(RSettingsArray& aSrc)
	{
	Reset();

	for(TInt i=aSrc.Count()-1; i>=0; i--)
		{
		AppendL(aSrc[0]);
		aSrc.RemoveElement(0);
		}
	}

inline void RSettingsArray::ExternalizeL(RWriteStream& aStream) const
	{
	
	TInt32 numElements = Count() ;	
	
	aStream << numElements ;
	for (TInt32 count = 0; count < numElements; count++)
		{
			aStream << (*this)[count] ;
		} 
	}


inline void RSettingsArray::WriteBackupStream(RWriteStream& aStream) const
	{
	
	// Note: Unlike the usual implementation of Externalize()
	// only selected data (settings with the "backup" bit set
	// in metadata) are streamed out!!!
	TInt32 numElementsToExternalize = 0;
	TInt32 numElements = Count() ;	
	TInt32 count ;
		
	for (count = 0; count < numElements; count++)
		{
		if ((*this)[count].Meta() & KMetaBackup) 
			numElementsToExternalize++ ;
		} 
	
	aStream << numElementsToExternalize ;
	for (count = 0; count < numElements; count++)
		{
		if ((*this)[count].Meta() & KMetaBackup) 
			aStream << (*this)[count] ;
		} 
	}

inline void RSettingsArray::InternalizeL(RReadStream& aStream)
	{
 	TInt32 numElements;
	
	aStream >> numElements ;	
	for (TInt32 count = 0; count < numElements; count++)
		{
		TServerSetting serverSetting ;
		aStream >> serverSetting ;
		serverSetting.PushL();
		if(IsDefault())
			{
			serverSetting.SetClean();			
			}
		AppendL(serverSetting) ;
		serverSetting.Pop();
		} 
	}

inline TBool RSettingsArray::IsDefault() const
	{
	return iIsDefault;
	}

inline void RSettingsArray::SetIsDefault(TBool aIsDefault)
	{
	iIsDefault=aIsDefault;
	}

inline RRangePolicyArray::RRangePolicyArray() :
	RArray<TSettingsAccessPolicy>()
	{}

inline RRangePolicyArray::~RRangePolicyArray()
	{ Reset(); }

inline TSettingsAccessPolicy* RRangePolicyArray::Find(TInt aKey) const
	{
	for(TInt ii = Count()-1; ii>=0;ii--)
		{
		if(operator[](ii).IsInRange(aKey))
			return const_cast<TSettingsAccessPolicy*>(&operator[](ii));
		}
	return NULL;
	}

inline void RRangePolicyArray::OrderedInsertL(const TSettingsAccessPolicy& aEntry)
	{ 
	InsertInUnsignedKeyOrderL(aEntry); 
	}

inline void RRangePolicyArray::ExternalizeL(RWriteStream& aStream) const
	{
	TInt32 numElements = Count() ;
	
	aStream << numElements ;
	
	for (TInt32 count = 0; count < numElements; count++)
		{
		aStream << (*this)[count] ;
		} 
	}

inline void RRangePolicyArray::InternalizeL(RReadStream& aStream)
	{
	TInt32 numElements = Count() ;	
	aStream >> numElements ;
	for (TInt32 count = 0; count < numElements; count++)
		{
		TSettingsAccessPolicy newElement ;
		aStream >> newElement ;
		AppendL(newElement) ;
		} 
	}

inline RPointerArray<TSettingsAccessPolicy>& CSharedRepository::SinglePolicyArray()
	{
	return iSinglePolicies;
	}

inline const TSecurityPolicy& CSharedRepository::GetReadAccessPolicy(const TServerSetting& aSetting)
	{
	const TSecurityPolicy* policy = aSetting.GetReadAccessPolicy();
	if (policy)
		{
		return *policy;
		}
	return GetFallbackReadAccessPolicy(aSetting.Key());
	}

inline const TSecurityPolicy& CSharedRepository::GetReadAccessPolicy(TUint32 aId)
	{
	const TServerSetting* s = iSettings.Find(aId);
	if (s)
		{
		const TSecurityPolicy* policy = s->GetReadAccessPolicy();
		if (policy)
			{
			return *policy;
			}
		}
	return GetFallbackReadAccessPolicy(aId);
	}

inline const TSecurityPolicy& CSharedRepository::GetDefaultReadAccessPolicy()
	{
	return iDefaultReadPolicy;
	}

inline const TSecurityPolicy& CSharedRepository::GetWriteAccessPolicy(const TServerSetting& aSetting)
	{
	const TSecurityPolicy* policy = aSetting.GetWriteAccessPolicy();
	if (policy)
		{
		return *policy;
		}
	return GetFallbackWriteAccessPolicy(aSetting.Key());
	}

inline const TSecurityPolicy& CSharedRepository::GetWriteAccessPolicy(TUint32 aId)
	{
	const TServerSetting* s = iSettings.Find(aId);
	if (s)
		{
		const TSecurityPolicy* policy = s->GetWriteAccessPolicy();
		if (policy)
			{
			return *policy;
			}
		}
	return GetFallbackWriteAccessPolicy(aId);
	}
	
inline const TSecurityPolicy& CSharedRepository::GetDefaultWriteAccessPolicy()
	{
	return iDefaultWritePolicy;
	}

inline TBool CSharedRepository::IsTransactionActive()
	{
	return (iPessimisticTransactionLockCount != 0) ||
		(iNumActiveConcurrentReadWriteTransactions > 0);
	}

inline RDefaultMetaArray::RDefaultMetaArray() :
	RArray<TSettingsDefaultMeta>()
	{}

inline RDefaultMetaArray::~RDefaultMetaArray()
	{ Reset(); }

inline TSettingsDefaultMeta* RDefaultMetaArray::Find(TInt aKey) const
	{
	for(TInt ii = Count()-1; ii>=0;ii--)
		{
		if(operator[](ii).IsInRange(aKey))
			return const_cast<TSettingsDefaultMeta*>(&operator[](ii));
		}
	return NULL;
	}

inline void RDefaultMetaArray::OrderedInsertL(const TSettingsDefaultMeta& aEntry)
	{ 
	InsertInUnsignedKeyOrderL(aEntry); 
	}

inline void RDefaultMetaArray::ExternalizeL(RWriteStream& aStream) const
	{
	TInt32 numElements = Count() ;
	
	aStream << numElements ;
	
	for (TInt32 count = 0; count < numElements; count++)
		{
		aStream << (*this)[count] ;
		} 
	}

inline void RDefaultMetaArray::InternalizeL(RReadStream& aStream)
{
	TInt32 numElements = Count() ;	
	aStream >> numElements ;
	for (TInt32 count = 0; count < numElements; count++)
		{
		TSettingsDefaultMeta newElement ;
		aStream >> newElement ;
		AppendL(newElement) ;
		} 

}

inline void CSharedRepository::WriteBackupStream(RWriteStream& aStream) const
	{
	iSettings.WriteBackupStream(aStream) ;
	aStream << iUid ;
	aStream << iOwner ;
	aStream << iTimeStamp.Int64() ;
		
	aStream << iRangePolicies ;
	aStream << iDefaultReadPolicy.Package() ;
	aStream << iDefaultWritePolicy.Package() ;
	
	aStream << iDefaultMeta ;
	aStream << iRangeMeta ;

	// placeholders for 3 booleans that should never have been externalised
	TInt8 dummyBoolValue = 0;
	aStream << dummyBoolValue;
	aStream << dummyBoolValue;
	aStream << dummyBoolValue;
	}
	
inline void CSharedRepository::WriteDeletedSettingsStream(RWriteStream& aStream) const
	{
	TCardinality numDeletedSettings = iDeletedSettings.Count() ;
	aStream << numDeletedSettings ;
	for (TInt i = 0; i < numDeletedSettings; i++)
		{
		aStream << iDeletedSettings[i] ;	
		}
	}	

inline void CSharedRepository::ExternalizeL(RWriteStream& aStream) const
	{
	aStream << iSettings ;
	aStream << iUid ;
	aStream << iOwner ;
	aStream << iTimeStamp.Int64() ;
		
	aStream << iRangePolicies ;
	aStream << iDefaultReadPolicy.Package() ;
	aStream << iDefaultWritePolicy.Package() ;
	
	aStream << iDefaultMeta ;
	aStream << iRangeMeta ;
	
	// placeholders for 3 booleans that should never have been externalised
	TInt8 dummyBoolValue = 0;
	aStream << dummyBoolValue;
	aStream << dummyBoolValue;
	aStream << dummyBoolValue;
	}

inline void CSharedRepository::InternalizeL(RReadStream& aStream)
	{
 	RSettingsArray mergeArray;
 
 	aStream >> mergeArray ;
 	// Only keys will be matched, so all values will be overwritten
 	iSettings.MergeArray(mergeArray, iDeletedSettings, ERestoreMerge, ETrue);
	aStream >> iUid ;
	aStream >> iOwner ;
	
	TInt64 timeStampInt ;
	aStream >> timeStampInt ;
	iTimeStamp = timeStampInt ;

	TInt32 numElements = iSettings.Count();
	for (TInt32 count = 0; count < numElements; count++)
		{
		iSettings[count].SetAccessPolicy(GetFallbackAccessPolicy(count));
		}
	iRangePolicies.Reset();		
	aStream >> iRangePolicies ;
	
	HBufC8* securityPolicyPackage ;
	securityPolicyPackage = HBufC8::NewLC(aStream, 10000) ;
	iDefaultReadPolicy.Set(securityPolicyPackage->Des()) ;
	CleanupStack::PopAndDestroy(securityPolicyPackage) ;
	securityPolicyPackage = HBufC8::NewLC(aStream, 10000) ;
	iDefaultWritePolicy.Set(securityPolicyPackage->Des()) ;
	CleanupStack::PopAndDestroy(securityPolicyPackage) ;
	
	iDefaultPolicy=TSettingsAccessPolicy(iDefaultReadPolicy,iDefaultWritePolicy, KUnspecifiedKey);
	
	aStream >> iDefaultMeta ;
	
	iRangeMeta.Reset();
	aStream >> iRangeMeta ;

	// placeholders for 3 booleans that should never have been externalised
	TInt8 dummyBoolValue;
	aStream >> dummyBoolValue;
	aStream >> dummyBoolValue;
	aStream >> dummyBoolValue;
	}

inline void CSharedRepository::SetSize(TInt aSize) 
	{ 
	iSize = aSize; 
	}

inline TInt CSharedRepository::Size() const 
	{ 
	return iSize; 
	} 

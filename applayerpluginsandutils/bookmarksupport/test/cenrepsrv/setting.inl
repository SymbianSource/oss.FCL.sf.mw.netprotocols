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

using namespace NCentralRepositoryConstants;

inline TServerSetting::TServerSetting()
	: iKey(0),
	  iMeta(0),
	  iAccessPolicy(0)
	{
	Mem::FillZ(&iValue, sizeof(iValue));
	}

inline TServerSetting::TServerSetting(const TUint32 aKey)
	: iKey(aKey),
	  iMeta(0),
	  iAccessPolicy(0)
	{
	Mem::FillZ(&iValue, sizeof(iValue));
	}

/** Ensures this setting has the same type and value of source. Where EString types
are involved, safely removes previously-owned string, and deep copies string taken
from source.
Does not require source and destination type to match. Callers must check if important.
@return	KErrNone if successful or KErrNoMemory if no memory available for copying any string.
@post If error code is not KErrNone, object is guaranteed to be in its original state.
*/
inline TInt TServerSetting::ReplaceTypeAndValue(const TServerSetting& source)
	{
	ASSERT(IsDeleted());
	
	if (source.IsStr())
		{
		const HBufC8* sourceBuf = source.GetStrValue();
		HBufC8* buf = sourceBuf ? sourceBuf->Alloc() : NULL;
		if (sourceBuf && !buf)
			{
			return KErrNoMemory;
			}
		
		ResetValue();
		SetType(EString);
		iValue.s = buf;
		}
	else if (source.IsReal())
		{
		if(!source.iValue.r)
			{
			return KErrCorrupt;
			}
		
		TReal* temp = new TReal(source.GetRealValue());
		if (temp == NULL)
			{
			return KErrNoMemory;
			}
		
		ResetValue();
		SetType(EReal);
		iValue.r = temp;
		}
	else
		{
		ResetValue();
		SetType(source.Type());
		if (source.IsInt())
			{
			iValue.i = source.GetIntValue();
			}
		}
	
	return KErrNone;
	}

/** Replaces the contents of this setting with those of source, safely cleaning up previously-
owned string, and making a deep copy of source string if of string type.
Does not require source and destination type to match. Callers must check if important.
@return	KErrNone if successful or KErrNoMemory if no memory available for copying any string.
@post If error code is not KErrNone, object is guaranteed to be in its original state.
*/
inline TInt TServerSetting::Replace(const TServerSetting& aSetting)
	{
	if (aSetting.IsStr())
		{
		const HBufC8* sourceBuf = aSetting.GetStrValue();
		HBufC8* targetBuf = sourceBuf ? sourceBuf->Alloc() : NULL;
		if (sourceBuf && !targetBuf)
			{
			return KErrNoMemory;
			}
		Reset();
		*this = aSetting;
		iValue.s = targetBuf;
		}
	else if (aSetting.IsReal())
		{
		TReal* temp = new TReal(aSetting.GetRealValue());
		if(temp == NULL)
			{
			return KErrNoMemory;
			}

		Reset();
		*this = aSetting;
		iValue.r = temp;
		temp = NULL;
		}
	else
		{
		Reset();
		*this = aSetting;
		}
	return KErrNone;
	}

/** Transfer ownership of the contents of this setting with those of source, safely cleaning up previously-
owned string, and making a deep copy of source string if of string type.
Does not require source and destination type to match. Callers must check if important.
@return	KErrNone if successful or KErrNoMemory if no memory available for copying any string.
@post If error code is not KErrNone, object is guaranteed to be in its original state.
*/
inline TInt TServerSetting::Transfer(TServerSetting& aSetting)
	{
	Reset();
	*this = aSetting;
	
	Mem::FillZ(&aSetting.iValue, sizeof(aSetting.iValue));
	aSetting.SetType(EDeleted);

	return KErrNone;
	}


inline TUint32 TServerSetting::Key() const
	{
	return iKey;
	}

inline void TServerSetting::SetKey(TUint32 aKey)
	{
	iKey = aKey;
	}

inline TUint32 TServerSetting::Meta() const
	{
	return iMeta & ~KMetaType;
	}

inline void TServerSetting::SetMeta(const TUint32 aMeta)
	{
	iMeta = (iMeta & KMetaType) | (aMeta & ~KMetaType);
	}
	
inline TUint32 TServerSetting::Type() const
	{
	return iMeta & KMetaType;
	}

inline void TServerSetting::SetType(const TUint32 aType)
	{
	iMeta = (iMeta & ~KMetaType) | (aType & KMetaType);
	}
	
inline TInt TServerSetting::GetIntValue() const
	{
	return iValue.i;
	}

inline const TReal& TServerSetting::GetRealValue() const
	{
	return *(iValue.r);
	}

inline const HBufC8* TServerSetting::GetStrValue() const
	{
	return iValue.s;
	}

inline void TServerSetting::SetIntValue(TInt aVal)
	{
	SetType(EInt);
	iValue.i = aVal;
	}

inline void TServerSetting::SetRealValue(const TReal* aVal)
	{
	ASSERT(aVal);
	SetType(EReal);
	iValue.r = const_cast<TReal*>(aVal);
	}


inline void TServerSetting::SetStrValue(const HBufC8* aVal)
	{
	SetType(EString);
	if(aVal && !aVal->Length())
		{
		delete aVal;
		iValue.s = NULL;
		}
	else
		{
		iValue.s = const_cast<HBufC8*>(aVal);
		}
	}

inline void TServerSetting::SetDeleted()
	{
	SetType(EDeleted);
	}
	
inline void TServerSetting::CopyValueL(TInt aVal)
	{
	Reset();
	SetIntValue(aVal);
	}

inline void TServerSetting::CopyValueL(const TReal& aVal)
	{
	Reset();
	TReal* temp = new(ELeave)TReal(aVal);
	SetRealValue(temp);
	temp = NULL;
	}

inline void TServerSetting::CopyValueL(const TDesC8& aVal)
	{
	Reset();
	if(aVal.Length())
		{
		SetStrValue(aVal.AllocL());
		}
	else
		{
		SetStrValue(NULL);
		}
	}

inline TInt TServerSetting::AssignValueTo(TInt& aVal) const
	{
	return IsInt() ? (aVal=iValue.i, KErrNone) : KErrArgument;
	}

inline TInt TServerSetting::AssignValueTo(TReal& aVal) const
	{
	return IsReal() ? 
		(iValue.r ? (aVal=*(iValue.r), KErrNone) : KErrCorrupt)
		: KErrArgument;
	}

inline TInt TServerSetting::AssignValueTo(TDes8& aVal) const
	{
	return IsStr() ?
		(iValue.s ? (aVal=iValue.s->Des(), KErrNone) : (aVal=KNullDesC8, KErrNone))
		: KErrArgument;
	}

inline TInt TServerSetting::AssignValueFromL(TInt aVal)
	{
	return IsInt() ? (iValue.i=aVal, KErrNone) : KErrArgument;
	}

inline TInt TServerSetting::AssignValueFromL(const TReal& aVal)
	{
	if(!IsReal())
		{
		return KErrArgument;
		}
	TReal* temp = new(ELeave)TReal(aVal);
	delete iValue.r;
	iValue.r = temp;
	temp = NULL;
	
	return KErrNone;
	}

inline TInt TServerSetting::AssignValueFromL(const TDesC8& aVal)
	{
	if(!IsStr())
		return KErrArgument;
	
	if(aVal.Length())
		{
		HBufC8* buf = aVal.AllocL();
		delete iValue.s;
		iValue.s = buf;
		}
	else
		{
		delete iValue.s;
		iValue.s = NULL;
		}


	return KErrNone;
	}
	
inline void TServerSetting::Reset()
	{
	ResetValue();
	iAccessPolicy = NULL;
	}

inline void TServerSetting::ResetValue()
	{
	if(IsStr())
		{
		if(iValue.s)
			{
			delete iValue.s;
			}
		}
	if(IsReal())
		{
		ASSERT(iValue.r);
		delete iValue.r;
		}
	iValue.i = 0;
	}

inline void TServerSetting::PushL() const
	{
	if(IsStr() && iValue.s)
		{
		CleanupStack::PushL(iValue.s);
		}
	else if(IsReal())
		{
		CleanupStack::PushL(iValue.r);
		}
	}

inline void TServerSetting::Pop() const
	{
	if(IsStr() && iValue.s)
		{
		CleanupStack::Pop(iValue.s);
		}
	else if(IsReal())
		{
		CleanupStack::Pop(iValue.r);
		}
	}

inline TInt TServerSetting::operator==(const TServerSetting& aSetting) const
	{
	return	aSetting.IsInt() && *this==aSetting.iValue.i ||
		aSetting.IsReal() && aSetting.iValue.r && *this==*(aSetting.iValue.r) ||
		aSetting.IsStr() && !aSetting.iValue.s && !iValue.s ||
		aSetting.IsStr() && aSetting.iValue.s && *this==aSetting.iValue.s->Des();
	}

inline TInt TServerSetting::operator==(TInt aVal) const
	{
	return IsInt() && iValue.i==aVal;
	}

inline TInt TServerSetting::operator==(const TReal& aVal) const
	{
	return IsReal() && iValue.r && *(iValue.r)==aVal;
	}

inline TInt TServerSetting::operator==(const TDesC8& aVal) const
	{
	return IsStr() && iValue.s && iValue.s->Des()==aVal;
	}

inline TSettingsAccessPolicy* TServerSetting::AccessPolicy() const
	{
	return iAccessPolicy;
	}

inline void TServerSetting::SetAccessPolicy(TSettingsAccessPolicy* aPolicy)
	{
	iAccessPolicy = aPolicy;
	}

//inline const RArray<TSecurityPolicy>& TServerSetting::GetReadAccessPolicy()
inline const TSecurityPolicy* TServerSetting::GetReadAccessPolicy() const
	{
	return iAccessPolicy ? iAccessPolicy->GetReadAccessPolicy() : NULL;
	}

//inline const RArray<TSecurityPolicy>& TServerSetting::GetWriteAccessPolicy()
inline const TSecurityPolicy* TServerSetting::GetWriteAccessPolicy() const 
	{
	return iAccessPolicy ? iAccessPolicy->GetWriteAccessPolicy() : NULL;
	}

inline TBool TServerSetting::HasAccessPolicy() const
	{
	return NULL != iAccessPolicy;
	}

inline void TServerSetting::ExternalizeMetaL(RWriteStream& aStream) const
	{
	//Historically, CRE file stores type and meta information seperately for 
	//TServerSetting. To save memory, this has been changed for the 
	//in-memory representation but we must store and retrieve the meta in the
	//same way for compatibility with existing backup and cre files.
	aStream << Meta();
	TUint32 type = Type();
	TUint8 temp = static_cast<TUint8>(type >> 28) ;
	aStream << temp ;
	}

inline void TServerSetting::ExternalizeL(RWriteStream& aStream) const
	{
	aStream << iKey ;
	ExternalizeMetaL(aStream);
	switch (Type())
		{
		case EInt:
			{
			TInt32 integerValue = iValue.i ;
			aStream << integerValue ;
			break ;
			}		
		case EReal :
			ASSERT(iValue.r);
			aStream << *(iValue.r) ;
			break ;
			
		case EString :
			if(iValue.s)
				{
				aStream << *(iValue.s) ;
				}
			else
				{
				aStream << KNullDesC8 ;
				}
			break ;
			
		case EDeleted :
			// Deleted settings should never be in a settings list being made
			// persistent. Hence, fail assert if attempting to externalize them:
			ASSERT(EFalse);
			break ;
		}	
	}


inline void TServerSetting::InternalizeMetaL(RReadStream& aStream)
	{
	//Historically, CRE file stores type and meta information seperately for 
	//TServerSetting. To save memory, this has been changed for the 
	//in-memory representation but we must store and retrieve the meta in the
	//same way for compatibility with existing backup and cre files.
	TUint32 meta;
	aStream >> meta;
	SetMeta(meta) ;
	
	TUint8 temp ;
	aStream >> temp ;
	TUint32 type = (static_cast<TUint32>(temp)) << 28;
	SetType(type);
	}

inline void TServerSetting::InternalizeL(RReadStream& aStream) 
{
	aStream >> iKey ;
	InternalizeMetaL(aStream);
	switch (Type())
		{
		case EInt:
			{
			TInt32 integerValue;
			aStream >> integerValue ;
			iValue.i = integerValue ;
			break ;
			}			
		case EReal:
			iValue.r = new(ELeave)TReal;
			aStream >> *(iValue.r);
			break ;
			
		case EString :
			{
			HBufC8* string = HBufC8::NewL (aStream, KMaxBinaryLength) ;
			if(string->Length())
				{
				iValue.s =	string;	
				}
			else
				{
				delete string;
				iValue.s = NULL;
				}
			break ;
			}

		case EDeleted :
			// Deleted settings should never be in a persistent settings list.
			// Hence, fail assert if attempting to internalize them:
			ASSERT(EFalse);
			break ;
		}	
	}

inline TBool TServerSetting::IsType(const TInt&) const
	{ 
	return Type() == EInt; 
	}
	
inline TBool TServerSetting::IsType(const TReal&) const
	{ 
	return Type() == EReal; 
	}
	
inline TBool TServerSetting::IsType(const TDesC8&) const
	{ 
	return Type() == EString; 
	}

inline TBool TServerSetting::IsType(const TDesC16&) const
	{ 
	return Type() == EString; 
	}

inline TBool TServerSetting::IsInt() const
	{
	return (iMeta & KMetaType) == EInt;
	}
	
inline TBool TServerSetting::IsReal() const
	{
	return (iMeta & KMetaType) == EReal;
	}
	
inline TBool TServerSetting::IsStr() const
	{
	return (iMeta & KMetaType) == EString;
	}
	
inline TBool TServerSetting::IsDeleted() const
	{
	return (iMeta & KMetaType) == EDeleted;
	}

inline TSettingsAccessPolicy::TSettingsAccessPolicy(TSecurityPolicy& aReadPolicy,
													TSecurityPolicy& aWritePolicy,
													TUint32 aLowKey, TUint32 aHighKey,
													TUint32 aKeyMask)
	{
	iLowKey = aLowKey; 
	iHighKey = aHighKey;
	iKeyMask = aKeyMask;
	iReadAccessPolicy.Set(aReadPolicy.Package());
	iWriteAccessPolicy.Set(aWritePolicy.Package());
	}

inline TSettingsAccessPolicy::TSettingsAccessPolicy(TSecurityPolicy& aReadPolicy,
													TSecurityPolicy& aWritePolicy,
													TUint32 aKey)
	{
	iLowKey = aKey; 
	iHighKey = 0;
	iKeyMask = 0;
	iReadAccessPolicy.Set(aReadPolicy.Package());
	iWriteAccessPolicy.Set(aWritePolicy.Package());
	}

inline TSettingsAccessPolicy::TSettingsAccessPolicy(TUint32 key)
	{
	iLowKey = key; 
	iHighKey = 0;
	iKeyMask = 0;
	}
		
inline TSettingsAccessPolicy::TSettingsAccessPolicy()
	{
	iLowKey = 0; 
	iHighKey = 0;
	iKeyMask = 0;
	}
	
inline TBool TSettingsAccessPolicy::IsInRange(TUint32 aKey) const
	{
	if((iLowKey<=aKey)&&(aKey<=iHighKey))
		return ETrue;
	else if((0 != iKeyMask)&&((iKeyMask&aKey)==iLowKey))
		return ETrue;
		
	return EFalse;
	}

inline void TSettingsAccessPolicy::ExternalizeL(RWriteStream& aStream) const
{
	aStream << iLowKey ;
	aStream << iHighKey ;
	aStream << iKeyMask ;
	
	// Externalize TSecurityPolicy objects as descriptors
	aStream << (iReadAccessPolicy.Package()) ;
	aStream << (iWriteAccessPolicy.Package()) ;
}

inline void TSettingsAccessPolicy::InternalizeL(RReadStream& aStream) 
{
	aStream >> iLowKey ;
	aStream >> iHighKey ;
	aStream >> iKeyMask ;
	
	// Internalize TSecurityPolicy objects as descriptors and then use them
	// to set the actual TSecurityPolicy member data.
	HBufC8* securityPolicyPackage ;
	securityPolicyPackage = HBufC8::NewLC(aStream, 10000);
	iReadAccessPolicy.Set(securityPolicyPackage->Des()) ;
	CleanupStack::PopAndDestroy(securityPolicyPackage) ;
	
	securityPolicyPackage = HBufC8::NewLC(aStream, 10000);
	iWriteAccessPolicy.Set(securityPolicyPackage->Des()) ;
	CleanupStack::PopAndDestroy(securityPolicyPackage) ;

	}

inline TUint32 TSettingsAccessPolicy::LowKey() const
	{
	return iLowKey;
	}
inline TUint32 TSettingsAccessPolicy::HighKey() const
	{
	return iHighKey;
	}
inline TUint32 TSettingsAccessPolicy::KeyMask() const
	{
	return iKeyMask;
	}

/**
@internalTechnology
It is responsility of client to check the key,
this simply returns the TSecurityPolicy - it is set to EAlwaysFail if uninitialised
*/
inline const TSecurityPolicy* TSettingsAccessPolicy::GetReadAccessPolicy() const
	{
	return &iReadAccessPolicy;
	}
	

/**
@internalTechnology
It is responsility of client to check the key,
this simply returns the TSecurityPolicy - it is set to EAlwaysFail if uninitialised
*/	
inline const TSecurityPolicy* TSettingsAccessPolicy::GetWriteAccessPolicy() const
	{
	return &iWriteAccessPolicy;
	}
	
inline TSettingsDefaultMeta::TSettingsDefaultMeta(TUint32 aValue, TUint32 aLowKey,
												  TUint32 aHighKey, TUint32 aKeyMask)
	{
	iLowKey = aLowKey; 
	iHighKey = aHighKey;
	iKeyMask = aKeyMask;
	iDefaultMetaData = aValue;
	}

inline TSettingsDefaultMeta::TSettingsDefaultMeta()
	{
	iLowKey = 0; 
	iHighKey = 0;
	iKeyMask = 0;
	iDefaultMetaData = 0;
	}

inline TUint32 TSettingsDefaultMeta::LowKey() const
	{
	return iLowKey;
	}
	
inline TUint32 TSettingsDefaultMeta::HighKey() const
	{
	return iHighKey;
	}
	
inline TUint32 TSettingsDefaultMeta::KeyMask() const
	{
	return iKeyMask;
	}
	
inline TBool TSettingsDefaultMeta::IsInRange(TUint32 aKey) const
	{
	if((iLowKey<=aKey)&&(aKey<=iHighKey))
		return ETrue;
	else if((0 != iKeyMask)&&((iKeyMask&aKey)==iLowKey))
		return ETrue;
		
	return EFalse;
	}



	
inline void TSettingsDefaultMeta::ExternalizeL(RWriteStream& aStream) const
{
	aStream << iLowKey ;
	aStream << iHighKey ;
	aStream << iKeyMask ;
	aStream << iDefaultMetaData ;
}

inline void TSettingsDefaultMeta::InternalizeL(RReadStream& aStream)
{
	aStream >> iLowKey ;
	aStream >> iHighKey ;
	aStream >> iKeyMask ;
	aStream >> iDefaultMetaData;
}

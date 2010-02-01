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

#ifndef SETTING_H
#define SETTING_H

#include <e32std.h>
#include <s32strm.h>

#include <centralrepository.h>
#include "srvdefs.h"



class TSettingsAccessPolicy;

class TServerSetting
	{
	friend class RSettingsArray;
public:
	enum TType { EInt	  = 0x00000000, 
				 EReal	  = 0x10000000, 
				 EString  = 0x20000000, 
				 EDeleted = 0x30000000 };

	inline TServerSetting();
	inline TServerSetting(TUint32 aKey);

	inline TInt ReplaceTypeAndValue(const TServerSetting& source);
	inline TInt Replace(const TServerSetting& source);
	inline TInt Transfer(TServerSetting& source);

	inline TUint32 Key() const;
	inline void SetKey(TUint32 aKey);
	inline TUint32 Meta() const;
	inline void SetMeta(const TUint32 aMeta);
	inline TBool IsClean(){return(iMeta & KMetaDefaultValue);};
	inline void SetClean(){iMeta |= KMetaDefaultValue;};
	inline TUint32 Type() const;
	inline void SetType(const TUint32 aType);

	inline TInt GetIntValue() const;
	inline const TReal& GetRealValue() const;
	inline const HBufC8* GetStrValue() const;

	inline void SetIntValue(TInt aVal);
	inline void SetRealValue(const TReal* aVal);
	inline void SetStrValue(const HBufC8* aVal);

	inline void SetDeleted();

	inline void CopyValueL(TInt aVal);
	inline void CopyValueL(const TReal& aVal);
	inline void CopyValueL(const TDesC8& aVal);

	inline TInt AssignValueTo(TInt& aVal) const;
	inline TInt AssignValueTo(TReal& aVal) const;
	inline TInt AssignValueTo(TDes8& aVal) const;

	inline TInt AssignValueFromL(TInt aVal);
	inline TInt AssignValueFromL(const TReal& aVal);
	inline TInt AssignValueFromL(const TDesC8& aVal);

	inline void PushL() const;
	inline void Pop() const;

	inline void Reset();
	inline void ResetValue();

	inline TInt operator==(const TServerSetting& aSetting) const;
	inline TInt operator==(TInt aVal) const;
	inline TInt operator==(const TReal& aVal) const;
	inline TInt operator==(const TDesC8& aVal) const;

	template <class T>
	inline TInt operator!=(const T& aVal) const
		{
		return !operator==(aVal);
		}

	inline TSettingsAccessPolicy* AccessPolicy() const;
	inline void SetAccessPolicy(TSettingsAccessPolicy* aPolicy);
	inline const TSecurityPolicy* GetReadAccessPolicy() const;
	inline const TSecurityPolicy* GetWriteAccessPolicy() const;
	inline TBool HasAccessPolicy() const;
	
	inline void ExternalizeL(RWriteStream& aStream) const;
	inline void InternalizeL(RReadStream& aStream);
	inline void ExternalizeMetaL(RWriteStream& aStream) const;
	inline void InternalizeMetaL(RReadStream& aStream);
	
	inline TBool IsType(const TInt&) const;
	inline TBool IsType(const TReal&) const;
	inline TBool IsType(const TDesC8&) const;
	inline TBool IsType(const TDesC16&) const;
	
	inline TBool IsInt() const;
	inline TBool IsReal() const;
	inline TBool IsStr() const;
	inline TBool IsDeleted() const;
	
private:
	TUint32 iKey;
	TUint32 iMeta;
	union TValue { TInt i; TReal* r; HBufC8* s; } iValue;

	// A bit of dilemma how to store TSecurePolicy for read/write access...
	// If we were implementing a list of policies per an entry then RArray would be an obvious choice,
	// but even then when no policy is specified each entry would have RArray (in compressed state 24B)
	// Or we could have two instances of TSecurePolicy (6B each)
	// so lets go for a link to a separate array of policies allocated only when needed

	TSettingsAccessPolicy* iAccessPolicy;
	};


class TSettingsAccessPolicy
	{
	friend class CSharedRepository;
	friend class CIniFileOut;

public:
	// Single entry type is maintained in a separate array
	// then the range type... to avoid duplicities
	enum {ESingleSettingPolicy,ERangeSettingsPolicy};

	inline TSettingsAccessPolicy(TSecurityPolicy& aReadPolicy,TSecurityPolicy& aWritePolicy,
								TUint32 aLowKey, TUint32 aHighKey, TUint32 aKeyMask=0);
	inline TSettingsAccessPolicy(TSecurityPolicy& aReadPolicy,TSecurityPolicy& aWritePolicy,
								TUint32 aKey);

	
	inline TBool IsInRange(TUint32 aKey) const;							
	inline const TSecurityPolicy* GetReadAccessPolicy() const;
	inline const TSecurityPolicy* GetWriteAccessPolicy() const;
	
	inline void ExternalizeL(RWriteStream& aStream) const;
	inline void InternalizeL(RReadStream& aStream) ;
	inline TSettingsAccessPolicy();
	inline TUint32 LowKey() const;
	inline TUint32 HighKey() const;
	inline TUint32 KeyMask() const;
		
private:
	inline TSettingsAccessPolicy(TUint32 key);
private:
	// we can have 'duplicites' where a single policy would have the same key
	// as 'range' policy with the low key... 
	TUint32 iLowKey; // used as a key for single policy as well
	TUint32 iHighKey; // only used for Range
	TUint32 iKeyMask; // only used for Range

	
	TSecurityPolicy iReadAccessPolicy;
	TSecurityPolicy iWriteAccessPolicy;

	};
	
class TSettingsDefaultMeta
	{
	friend class CSharedRepository;
public:
	inline TSettingsDefaultMeta(TUint32 aValue, TUint32 aLowKey,
								TUint32 aHighKey, TUint32 aKeyMask=0);

	inline TBool IsInRange(TUint32 aKey) const;	
	inline TSettingsDefaultMeta();
						
	inline TUint32 GetDefaultMetadata() const 
	{
		return iDefaultMetaData ;
	};

	inline void ExternalizeL(RWriteStream& aStream) const;
	inline void InternalizeL(RReadStream& aStream) ;	

	inline TUint32 LowKey() const;
	inline TUint32 HighKey() const;
	inline TUint32 KeyMask() const;

private:
	TUint32 iLowKey;
	TUint32 iHighKey;
	TUint32 iKeyMask;
	TUint32 iDefaultMetaData;
	};

#include "setting.inl"

#endif // SETTING_H

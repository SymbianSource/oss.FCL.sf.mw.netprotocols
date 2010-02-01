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
// Internal class for manipulating bookmark extended properties.
// @internalComponent
// 
//

#include "bkmrk.h"
#include <cauthentication.h>

CBkmrkExtendedProperties* CBkmrkExtendedProperties::NewL(CBookmarkBase& aParent, CBookmarkDb& aDb, CRepository& aRepository)
	{
	CBkmrkExtendedProperties* self = new (ELeave) CBkmrkExtendedProperties(aParent, aDb, aRepository);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CBkmrkExtendedProperties::CBkmrkExtendedProperties(CBookmarkBase& aParent, CBookmarkDb& aDb, CRepository& aRepository) 
	: CBkmrkProperties(aParent, aDb, aRepository)
	{
	}

CBkmrkExtendedProperties::~CBkmrkExtendedProperties()
	{
	delete iAuthentication;
	}

void CBkmrkExtendedProperties::ConstructL()
	{
	CBkmrkProperties::ConstructL();
	}

void CBkmrkExtendedProperties::SetAuthenticationL(const CAuthentication& aAuthentication)
	{
	iParent.LeaveIfNotWritableL();
	delete iAuthentication;
	iAuthentication = NULL;
	iAuthentication = CAuthentication::NewL(aAuthentication.Name(), aAuthentication.Password(), aAuthentication.Method());
	SetDirty();
	}

const CAuthentication& CBkmrkExtendedProperties::AuthenticationL()
	{
	if (!iAuthentication)
		{
		// Create an empty authentication object
		iAuthentication = CAuthentication::NewL(KNullDesC8, KNullDesC8);
		}
		
	return *iAuthentication;
	}
	
TUint32 CBkmrkExtendedProperties::Proxy() const
	{
	return iProxy;
	}
	
void CBkmrkExtendedProperties::SetProxyL(TUint32 aProxy)
	{
	iParent.LeaveIfNotWritableL();
	iProxy = aProxy;
	SetDirty();
	}

TUint32 CBkmrkExtendedProperties::Nap() const
	{
	return iNap;
	}
	
void CBkmrkExtendedProperties::SetNapL(TUint32 aNap)
	{
	iParent.LeaveIfNotWritableL();
	iNap = aNap;
	SetDirty();
	}
	
void CBkmrkExtendedProperties::TransNewL()
	{
	CBkmrkProperties::TransNewL();
	TUint32 indexBase = IndexBase();
	// create entries in the repository
	iRepository->Create(indexBase + KBkmrkAuthNameIndex, AuthenticationL().Name());
	iRepository->Create(indexBase + KBkmrkAuthPasswordIndex, AuthenticationL().Password());
	iRepository->Create(indexBase + KBkmrkAuthMethodIndex, AuthenticationL().Method());
	iRepository->Create(indexBase + KBkmrkProxyIndex, static_cast<TInt>(iProxy));
	iRepository->Create(indexBase + KBkmrkNapIndex, static_cast<TInt>(iNap));
	}

void CBkmrkExtendedProperties::TransSaveL()
	{
	CBkmrkProperties::TransSaveL();
	TUint32 indexBase = IndexBase();
	iRepository->Set(indexBase + KBkmrkAuthNameIndex, AuthenticationL().Name());
	iRepository->Set(indexBase + KBkmrkAuthPasswordIndex, AuthenticationL().Password());
	iRepository->Set(indexBase + KBkmrkAuthMethodIndex, AuthenticationL().Method());
	iRepository->Set(indexBase + KBkmrkProxyIndex, static_cast<TInt>(iProxy));
	iRepository->Set(indexBase + KBkmrkNapIndex, static_cast<TInt>(iNap));
	}

void CBkmrkExtendedProperties::TransLoadL()
	{
	CBkmrkProperties::TransLoadL();
	
	TUint32 indexBase = IndexBase();
	HBufC8* buffer = HBufC8::NewL(Bookmark::KMaxDataLength);
	CleanupStack::PushL(buffer);
	TPtr8 bufPtr = buffer->Des();
	if (!iAuthentication)
		{
		// Create an empty authentication object
		iAuthentication = CAuthentication::NewL(KNullDesC8, KNullDesC8);
		}
	User::LeaveIfError(iRepository->Get(indexBase + KBkmrkAuthNameIndex, bufPtr));
	iAuthentication->SetNameL(bufPtr);
	User::LeaveIfError(iRepository->Get(indexBase + KBkmrkAuthPasswordIndex, bufPtr));
	iAuthentication->SetPasswordL(bufPtr);
	TInt method;
	User::LeaveIfError(iRepository->Get(indexBase + KBkmrkAuthMethodIndex, method));
	iAuthentication->SetMethod(static_cast<CAuthentication::TMethod>(method));
	CleanupStack::PopAndDestroy(buffer);
	
	TInt retVal;
	User::LeaveIfError(iRepository->Get(indexBase + KBkmrkProxyIndex, retVal));
	iProxy = static_cast<TUint32>(retVal);
	User::LeaveIfError(iRepository->Get(indexBase + KBkmrkNapIndex, retVal));
	iNap = static_cast<TUint32>(retVal);
	SetClean();
	}

void CBkmrkExtendedProperties::TransRemoveL()
	{
	CBkmrkProperties::TransRemoveL();
	
	TUint32 indexBase = IndexBase();
	iRepository->Delete(indexBase + KBkmrkAuthNameIndex);
	iRepository->Delete(indexBase + KBkmrkAuthPasswordIndex);
	iRepository->Delete(indexBase + KBkmrkAuthMethodIndex);
	iRepository->Delete(indexBase + KBkmrkProxyIndex);
	iRepository->Delete(indexBase + KBkmrkNapIndex);
	iCustomProperties->TransRemoveL();
	}
	

// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "CWspCapabilityInfo.h"

#include <wsp/WspTypes.h>
#include "CWspAliasAddresses.h"
#include "CWspExtendedMethods.h"
#include "CWspHeaderCodePages.h"
#include "CWspUnknownCapabilities.h"

const TUint32 KWspMethodMORDefault			= 1;
const TUint32 KWspPushMORDefault			= 1;
const TUint32 KWspClientSDUSizeDefault		= 1400;
const TUint32 KWspServerSDUSizeDefault		= 1400;
const TUint32 KWspClientMessageSizeDefault	= 1400;
const TUint32 KWspServerMessageSizeDefault	= 1400;
const TUint8  KWspProtocolOptionsDefault	= 0x00;


CWspCapabilityInfo* CWspCapabilityInfo::NewL()
	{
	CWspCapabilityInfo* self = new (ELeave) CWspCapabilityInfo();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CWspCapabilityInfo::~CWspCapabilityInfo()
	{
	delete iAliasAddresses;
	delete iExtendedMethods;
	delete iHeaderCodePages;
	delete iUnknownCapabilities;
	}

CWspCapabilityInfo::CWspCapabilityInfo()
: CBase(), iMethodMOR(KWspMethodMORDefault),
		   iPushMOR(KWspPushMORDefault),
		   iProtocolOptions(KWspProtocolOptionsDefault),
		   iClientSDUSize(KWspClientSDUSizeDefault),
		   iServerSDUSize(KWspServerSDUSizeDefault),
		   iClientMessageSize(KWspClientMessageSizeDefault),
		   iServerMessageSize(KWspServerMessageSizeDefault)
	{
	}

void CWspCapabilityInfo::ConstructL()
	{
	iAliasAddresses = CWspAliasAddresses::NewL();
	iExtendedMethods = CWspExtendedMethods::NewL();
	iHeaderCodePages = CWspHeaderCodePages::NewL();
	iUnknownCapabilities = CWspUnknownCapabilities::NewL();
	}

/*
 * Methods from MWspCapabilityViewer
 */

const MWspAliasAddresses& CWspCapabilityInfo::GetAliasAddresses() const
	{
	return *iAliasAddresses;
	}

const MWspExtendedMethods& CWspCapabilityInfo::GetExtendedMethods() const
	{
	return *iExtendedMethods;
	}

const MWspHeaderCodePages& CWspCapabilityInfo::GetHeaderCodePages() const
	{
	return *iHeaderCodePages;
	}

TUint8 CWspCapabilityInfo::GetMethodMOR() const
	{
	return iMethodMOR;
	}

TUint8 CWspCapabilityInfo::GetPushMOR() const
	{
	return iPushMOR;
	}

TUint8 CWspCapabilityInfo::GetProtocolOptions() const
	{
	return iProtocolOptions;
	}

TUint32 CWspCapabilityInfo::GetClientSDUSize() const
	{
	return iClientSDUSize;
	}

TUint32 CWspCapabilityInfo::GetServerSDUSize() const
	{
	return iServerSDUSize;
	}

TUint32 CWspCapabilityInfo::GetClientMessageSize() const
	{
	return iClientMessageSize;
	}

TUint32 CWspCapabilityInfo::GetServerMessageSize() const
	{
	return iServerMessageSize;
	}

const MWspUnknownCapabilities& CWspCapabilityInfo::GetUnknownCapabilities() const
	{
	return *iUnknownCapabilities;
	}

/*
 * Methods from MWspCapabilitiesSetter
 */

void CWspCapabilityInfo::Reset(TWspCapability aCapability)
	{
	switch(aCapability)
		{
	case EAliasAddresses:
		{
		iAliasAddresses->Reset();
		} break;
	case EExtendedMethods:
		{
		iExtendedMethods->Reset();
		} break;
	case EHeaderCodePages:
		{
		iHeaderCodePages->Reset();
		} break;
	case EMethodMOR:
		{
		iMethodMOR = KWspMethodMORDefault;
		} break;
	case EPushMOR:
		{
		iPushMOR = KWspPushMORDefault;
		} break;
	case EProtocolOptions:
		{
		iProtocolOptions = KWspProtocolOptionsDefault;
		} break;
	case EClientSDUSize:
		{
		iClientSDUSize = KWspClientSDUSizeDefault;
		} break;
	case EServerSDUSize:
		{
		iServerSDUSize = KWspServerSDUSizeDefault;
		} break;
	case EClientMessageSize:
		{
		iClientMessageSize = KWspClientMessageSizeDefault;
		} break;
	case EServerMessageSize:
		{
		iServerMessageSize = KWspServerMessageSizeDefault;
		} break;
	case EUnknownCapabilities:
		{
		iUnknownCapabilities->Reset();
		} break;
	case EAllCapabilities:
		{
		// Recurse into this function using a different value of TWspCapability
		for(TInt capability = 0; capability<EAllCapabilities; ++capability)
			{
			// Reset the given capability
			Reset(STATIC_CAST(TWspCapability, capability));
			}
		} break;
	default:
		// Ignore any unknown capability
		break;
		}
	}

void CWspCapabilityInfo::AddAliasAddressL(TWspBearer aBearer, TUint16 aPort, const TDesC8& aAddress)
	{
	iAliasAddresses->AddAddressL(aBearer, aPort, aAddress);
	}

void CWspCapabilityInfo::AddExtendedMethodL(TUint8 aPDUType, const TDesC8& aMethodName)
	{
	iExtendedMethods->AddExtendedMethodL(aPDUType, aMethodName);
	}

void CWspCapabilityInfo::AddHeaderCodePageL(TUint8 aPageCode, const TDesC8& aPageName)
	{
	iHeaderCodePages->AddHeaderCodePageL(aPageCode, aPageName);
	}

void CWspCapabilityInfo::SetMethodMOR(TUint8 aMOR)
	{
	iMethodMOR = aMOR;
	}

void CWspCapabilityInfo::SetPushMOR(TUint8 aMOR)
	{
	iPushMOR = aMOR;
	}

void CWspCapabilityInfo::SetProtocolOptions(TUint8 aProtocolOptions)
	{
	iProtocolOptions = aProtocolOptions;
	}

void CWspCapabilityInfo::SetClientSDUSize(TUint32 aSize)
	{
	iClientSDUSize = aSize;
	}

void CWspCapabilityInfo::SetServerSDUSize(TUint32 aSize)
	{
	iServerSDUSize = aSize;
	}

void CWspCapabilityInfo::SetClientMessageSize(TUint32 aSize)
	{
	iClientMessageSize = aSize;
	}

void CWspCapabilityInfo::SetServerMessageSize(TUint32 aSize)
	{
	iServerMessageSize = aSize;
	}

void CWspCapabilityInfo::AddUnknownCapabilityL(const TDesC8& aIdentifier, const TDesC8& aValue)
	{
	iUnknownCapabilities->AddUnknownCapabilityL(aIdentifier, aValue);
	}

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
// testhook.cpp - Outbound plugin test
// When the ip.etesthook.esk is put in c:/system/data this
// plugin is bound to the IP. All outgoing messages will pass
// through this. The name "ip.etesthook.esk" is important, since this
// must be processed before any other ip.*.esk files.
// Based on Nokia's exout plugin
// 
//


#include <e32svr.h>	
#include "testhook.h"
#include <httpsocketconstants.h>

const TUint16 KTestProtocol	= 0xfb5;	//likewise

//
// Entrypoint
//
#ifdef __SECURE_API__
GLDEF_C TInt E32Dll()
#else
GLDEF_C TInt E32Dll(TDllReason /*aReason*/)
#endif
{
	return KErrNone;
}

class CTestProtocolFamily : public CProtocolFamilyBase
{
public:
	CTestProtocolFamily();
	~CTestProtocolFamily();
	TInt Install();
	TInt Remove();
	TUint ProtocolList(TServerProtocolDesc *& aProtocolList);
	CProtocolBase* NewProtocolL(TUint aSockType, TUint aProtocol);
};

extern "C" { IMPORT_C CProtocolFamilyBase* Install(void); }
EXPORT_C CProtocolFamilyBase* Install()
{
	return new (ELeave) CTestProtocolFamily;
}

//----------------------------------------------------------------------
// Dummy protocol family declaration. Required for DLL's
//

CTestProtocolFamily::CTestProtocolFamily()
	{
	__DECLARE_NAME(_S("CTestProtocolFamily"));
	}

CTestProtocolFamily::~CTestProtocolFamily()
	{
	}

TInt CTestProtocolFamily::Install()
	{
	return KErrNone;
	}

TInt CTestProtocolFamily::Remove()
	{
	return KErrNone;
	}

TUint CTestProtocolFamily::ProtocolList(TServerProtocolDesc *& aProtocolList)
	{
	// This function should be a leaving fn
	// apparently it is OK for it to leave
	TServerProtocolDesc *p = new (ELeave) TServerProtocolDesc[1]; // Esock catches this leave
	
	CTestProtocol::FillIdentification(p[0]);
	aProtocolList = p;
	return 1;
	}

CProtocolBase* CTestProtocolFamily::NewProtocolL(TUint /*aSockType*/, TUint aProtocol)
	{
	if (aProtocol != KTestProtocol)
		{
		User::Leave(KErrNotSupported);
		}

	return new (ELeave) CTestProtocol;
	}


//-------------------------------------

const TInt CTestFlowInfo::iOffset = _FOFF(CTestFlowInfo, iDLink);

CTestProtocol::CTestProtocol() : iFlowList(CTestFlowInfo::iOffset), iSessionId(KErrNotFound)
	{
	}

void CTestProtocol::InitL(TDesC& aTag)
	{
	CProtocolBase::InitL(aTag);
	}

void CTestProtocol::StartL()
{
//	__ASSERT_DEBUG(iProtocol != NULL, User::Leave(KErrGeneral));
}

CTestProtocol::~CTestProtocol()
	{
	if (iIPProtocol != NULL)
		{
		iIPProtocol->Unbind(this);
		}
	}

void CTestProtocol::BindL(CProtocolBase* aProtocol, TUint aId)
	{
	if ((aId != KProtocolInet6Ip) || (aProtocol == this))
		{
		User::Leave(KErrGeneral);
		}
	
	TUint ourId;
	TServerProtocolDesc info;
	Identify(&info);
	ourId = info.iProtocol;
	
	if (aId == ourId)
		{
		User::Leave(KErrGeneral);
		}

	aProtocol->BindL((CProtocolBase*)this, BindFlowHook());
	
	iIPProtocol = (CProtocolInet6Binder*)aProtocol;
	}

void CTestProtocol::Unbind(CProtocolBase* aProtocol, TUint /*aId*/)
	{
	if (iIPProtocol == aProtocol) 
		{
		iIPProtocol = 0;
		}

	// All the FlowContexts in iFlowList must be deleted as well
	if(!iFlowList.IsEmpty())
		{
		CTestFlowInfo* temp = iFlowList.First();
		while (temp != NULL)
			{
			temp->iDLink.Deque();
			delete temp;
			temp = iFlowList.First();
			}
		}
	}

void CTestProtocol::FillIdentification(TServerProtocolDesc& anEntry)
	{
	anEntry.iName=_S("TestHook");
	anEntry.iAddrFamily=KAfInet;
	anEntry.iSockType=KSockDatagram;
	anEntry.iProtocol=KTestProtocol;
	anEntry.iVersion=TVersion(1, 0, 0);
	anEntry.iByteOrder=EBigEndian;
	anEntry.iServiceInfo=KSIDatagram | KSIConnectionLess;
	anEntry.iNamingServices=0;
	anEntry.iSecurity=KSocketNoSecurity;
	anEntry.iMessageSize=0xffff;
	anEntry.iServiceTypeInfo=0;
	anEntry.iNumSockets=KUnlimitedSockets;
	}

void CTestProtocol::Identify(TServerProtocolDesc *aDesc) const
	{
	FillIdentification(*aDesc);
	}

void CTestProtocol::BindToL(CProtocolBase* /*aProtocol*/)
	{
	User::Panic(_L("TestHook"), 0);
	}


//----------------------------------------------------------------------
MFlowHook *CTestProtocol::OpenL(TPacketHead& /*aHead*/, CFlowContext* aFlow)
	{
	CTestFlowInfo* info = new (ELeave) CTestFlowInfo;
	info->iMgr = this;
	iFlowList.AddLast(*info);	//Save the flow info for later use

	if( iSessionId > 0 )
		{
		RDebug::Print(_L("Opening session with id = %i\n"),iSessionId);
		}

	info->iFlow = aFlow;

	return info;
	}



TInt CTestProtocol::SetFlowOption(TUint aLevel, TUint aName, const TDesC8 &aOption, CFlowContext& aFlow)
	{
	(void) aFlow;
	if((aLevel== KSOLHttpSessionInfo) && (aName == KSOHttpSessionId)) 
		{
		TPckgBuf<TInt> option; 
		option.Copy(aOption);
		iSessionId = option();
		return KErrNone;
		}
	return KErrNotSupported;
	}
		
TInt CTestFlowInfo::ReadyL(TPacketHead& /*aHead*/)
{
	return EFlow_READY;
}

//
// Applies all the remaining modifications needed for every outgoing packet
//
TInt CTestFlowInfo::ApplyL(RMBufSendPacket& /*aPacket*/, RMBufSendInfo& /*aInfo*/)
	{
	return 0;
	}

void CTestFlowInfo::Close()
	{
	if (--iRefs < 0)
		{
		iDLink.Deque();
		delete this;
		}
	}

void CTestFlowInfo::Open()
	{
	iRefs++;
	}

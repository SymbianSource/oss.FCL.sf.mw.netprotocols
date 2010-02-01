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
// testhook.h - hook plugin for outbound packets
// 
//

#ifndef __TESTHOOK_H
#define __TESTHOOK_H

#include <in_bind.h>


class CTestFlowInfo;

class CTestProtocol : public CIp6Hook
{
	public:
		CTestProtocol();
		virtual ~CTestProtocol();

		//CIp6Hook::CProtocolBase
		virtual void InitL(TDesC& aTag);
		virtual void StartL();
		virtual void BindToL(CProtocolBase* aProtocol);
		virtual void BindL(CProtocolBase* /*aProtocol*/, TUint /*aId*/);
		virtual void Unbind(CProtocolBase* /*aProtocol*/, TUint /*aId*/);
		virtual void Identify(TServerProtocolDesc *aDesc) const;
		virtual TInt SetFlowOption(TUint aLevel, TUint aName, const TDesC8 &aOption, CFlowContext &aFlow);
		static void FillIdentification(TServerProtocolDesc& aDesc);
		
		//CIp6Hook::MIp6Hook 
		// never called because binded only as outbound
		virtual TInt ApplyL(RMBufHookPacket& /*aPacket*/, RMBufRecvInfo& /*aInfo*/){
			return KIp6Hook_PASS;}

		//Outbound Flow
		virtual MFlowHook *OpenL(TPacketHead& /*aHead*/, CFlowContext *aFlow);

	private:
		CProtocolInet6Binder*		iIPProtocol;
		TDblQue<CTestFlowInfo>	iFlowList;
		TInt iSessionId;
};

class CTestFlowInfo : public CBase, public MFlowHook
	{
	public:
		void Open();
		TInt ReadyL(TPacketHead &);
		TInt ApplyL(RMBufSendPacket &, RMBufSendInfo &);
		void Close();

		CTestProtocol		*iMgr;
		TInt				iRefs;
		TDblQueLink			iDLink;

		static const TInt iOffset;

		CFlowContext*	iFlow;
	};

#endif

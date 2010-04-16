// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __CHTTPSECURESOCKET_H__
#define __CHTTPSECURESOCKET_H__

// System includes
#include <e32base.h>
#include <ecom/ecom.h>
#include <http/framework/csecuritypolicy.h>

// User includes
#include "mcommsinfoprovider.h"

class CX509Certificate;

/**	The ECOM interface definition for a secure socket class.
 */
class CHttpSecureSocket : public CBase
	{
public:		// methods

	inline static CHttpSecureSocket* NewL(RSocket& aSocket, MCommsInfoProvider& aCommsInfoProvider);

	inline virtual ~CHttpSecureSocket();

	virtual void StartSecureHandshakeL(TRequestStatus& aStatus, const TDesC8& aHostName,const  TDesC& aProtocolVersion) =0;

	virtual void RecvOneOrMore(TDes8& aBuffer, TRequestStatus& aStatus, TSockXfrLength& aLength) =0;

	virtual void CancelRecv() =0;

	virtual void Send(const TDesC8& aBuffer, TRequestStatus& aStatus) =0;

	virtual void CancelSend() =0;

	virtual const CX509Certificate* ServerCert() = 0; 

	virtual void CancelHandshake() =0;
	
	virtual TInt CipherSuite(TDes8& aCipherSuite) =0;

    virtual TInt PendingBytesToRead () =0;

protected:
	struct THttpSecureSocketParams
		{
		RSocket* iSocket;
		MCommsInfoProvider* iCommsInfoProvider;
		};


private:	// Attributes

	/**	The ECom destructor key identifier
	*/
	TUid iEcomDtorID;
	};
	
	
//-----------------------------------------------------------------------------


inline CHttpSecureSocket* CHttpSecureSocket::NewL(RSocket& aSocket, MCommsInfoProvider& aCommsInfoProvider)
	{
	_LIT8(KDataTypeName, "http-secure-socket");

	const TUid KUidSecSockInterface = {0x101F55F8};

	TEComResolverParams resParams;
	resParams.SetDataType(KDataTypeName());

	THttpSecureSocketParams initParams;
	initParams.iSocket = &aSocket;
	initParams.iCommsInfoProvider = &aCommsInfoProvider;

	TAny* ptr = REComSession::CreateImplementationL(KUidSecSockInterface, 
									_FOFF(CHttpSecureSocket, iEcomDtorID),
									&initParams,
									resParams);

	return REINTERPRET_CAST(CHttpSecureSocket*, ptr);
	}
	
inline CHttpSecureSocket::~CHttpSecureSocket()
	{
	REComSession::DestroyedImplementation(iEcomDtorID);
	} 	
	
#endif // __CHTTPSECURESOCKET_H__

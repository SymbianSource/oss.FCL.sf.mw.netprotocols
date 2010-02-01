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

#ifndef __CSOCKETCONTROLLER_H__
#define __CSOCKETCONTROLLER_H__

#include <e32base.h>
#include <in_sock.h>
#include "msocketcontroller.h"

class CSocket;
class MInputStream;
class MOutputStream;
class CSocketReader;
class CSocketWriter;
class MSocketControllerStore;
class MConnectionPrefsProvider;


class CSocketController: public CBase,
						 public MSocketController
/**	
	The CSocketController class owns a connected socket. It provides input and
	output stream objects that use that socket via the CSocketReader and 
	CSocketWriter attributes.

	A CSocketController object will self-delete once the socket it owns has been 
	closed. The socket can be closed either by the remote client, the input stream
	observer or the output stream observer.
	@internalComponent
*/
	{
public:	// methods

	static CSocketController* NewL(CSocket* aSocket, MConnectionPrefsProvider& aConnectionPrefsProvider,TBool aPriority);

	virtual ~CSocketController();

	void NotifyInStore(MSocketControllerStore& aStore);

	MInputStream& InputStream() const;

	MOutputStream& OutputStream() const;

   TBool  HostAndPortMatches(const TDesC8& aHost, TUint16 aPort);
    
    inline TInetAddr& RemoteAddress();
    
    void AssignRemoteHostInfoL(const TDesC& aHost, TUint16 aPort, const TInetAddr& aAddr);

private:	// methods from MSocketController

	virtual void StreamClosed(TInt aError, MSocketController::TStreamType aStreamType);

	virtual void StreamSuspend(MSocketController::TStreamType aStreamType);

	virtual void StreamResume(MSocketController::TStreamType aStreamType);

	virtual void ConnectionInfo(TDes8& aRemoteHost, TUint16& aRemotePort, TUint16& aLocalPort);

private:	//	methods

	CSocketController(CSocket* aSocket, MConnectionPrefsProvider& aConnectionPrefsProvider, TBool aPriority);

	void ConstructL( TInt aRecvBufferSize );

private:
    TUint16                     iPort;
     
    HBufC8*                     iHost;
  
/** The socket controller store. This is not owned.
*/
	MSocketControllerStore*	iStore;

/**	The connected socket. The ownership of this object is given by the iOwnSocket
	flag.
*/
	CSocket*				iSocket;

/**	The socket reader.
*/
	CSocketReader*			iSocketReader;

/**	The socket writer.
*/
	CSocketWriter*			iSocketWriter;
	
	TBool 					iPriority;

/** The connection preferences provider
*/
	MConnectionPrefsProvider& iConnectionPrefsProvider;

    TInetAddr                   iRemoteAddress;       
	};

inline TInetAddr& CSocketController::RemoteAddress()
    {
    return iRemoteAddress;
    }

#endif	// __CSOCKETCONTROLLER_H__

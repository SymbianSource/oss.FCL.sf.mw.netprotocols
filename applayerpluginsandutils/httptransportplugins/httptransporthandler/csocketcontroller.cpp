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

#include "csocketcontroller.h"

#include "csocket.h"
#include "csocketreader.h"
#include "csocketwriter.h"
#include "msocketcontrollerstore.h"
#include "mconnectionprefsprovider.h"
#include "httptransporthandlercommon.h"
#include "thttptrlayerpanic.h"
#include <escapeutils.h>

CSocketController* CSocketController::NewL(CSocket* aSocket, MConnectionPrefsProvider& aConnectionPrefsProvider,TBool aPriority)
/**	
	The factory constructor. Ownership of the socket is transferred on calling 
	the factory constructor.
	@param		aSocket		The connected socket.
	@return		A pointer to a fully constructed object.
*/
	{
	// Ownership of socket has been transferred - place on cleanup stack.
	CleanupStack::PushL(aSocket);
	
	// Transfer ownership of the socket to the socket controller.
	CSocketController* self = new (ELeave) CSocketController(aSocket, aConnectionPrefsProvider, aPriority);

	// Ownership of socket with the socket controller - pop-off the cleanup stack
	CleanupStack::Pop(aSocket);

	// Continue with initialisation of the socket controller
	CleanupStack::PushL(self);
	self->ConstructL( aConnectionPrefsProvider.GetRecvBufferSize() );
	CleanupStack::Pop(self);
	return self;
	}

CSocketController::~CSocketController()
/**	
	Destructor.
*/
	{
	// Clean-up...
	delete iSocketReader;
	delete iSocketWriter;
	delete iSocket;
	delete iHost;
	}

CSocketController::CSocketController(CSocket* aSocket, MConnectionPrefsProvider& aConnectionPrefsProvider, TBool aPriority)
: CBase(), iSocket(aSocket), iPriority(aPriority), iConnectionPrefsProvider(aConnectionPrefsProvider) 
/**	
	Constructor.
	@param		aStore		The socket controller store.
	@param		aSocket		The connected socket.
*/
	{
	__ASSERT_DEBUG( iSocket, User::Invariant() );
	}

void CSocketController::ConstructL( TInt aRecvBufferSize )
/**	
	Second phase constructor. Initialises the object. Ownership of the connected
	socket is passed to this class if this function completes.
*/
	{
	// Create the socket reader and writer
	// Writer is created (and added to AS) first so that it has a 'higher priority' in AS.
	// Must keep this order since it guarantees that we receive the acknowledgement of data being
	// sent out before receiving the response, which is something we rely on.
	iSocketWriter = CSocketWriter::NewL(*iSocket, *this, iPriority);	
	iSocketReader = CSocketReader::NewL(*iSocket, *this, aRecvBufferSize, iPriority);
	}

void CSocketController::NotifyInStore(MSocketControllerStore& aStore)
/**	
	Notifier that this object is now owned by the socket controller store.
	@param		aStore		The socket controller store.
*/
	{
	iStore = &aStore;
	}

MInputStream& CSocketController::InputStream() const
/**	
	The access function for the input stream.
	@return		The input stream object.
*/
	{
	return *iSocketReader;
	}

MOutputStream& CSocketController::OutputStream() const
/**	
	The access function for the output stream.
	@return		The output stream object.
*/
	{
	return *iSocketWriter;
	}

/*
 *	Methods from MSocketController
 */

void CSocketController::StreamClosed(TInt aError, MSocketController::TStreamType aStreamType)
/**	
	@see		MSocketController
*/
	{
	// Notify the other stream...
	switch( aStreamType )
		{
	case EInputStream:
		{
		// The input stream has closed the socket - inform the socket writer
		iSocketWriter->SocketClosed(aError);
		} break;
	case EOutputStream:
		{
		// The output stream has closed the socket - inform the socket reader
		iSocketReader->SocketClosed(aError);
		} break;
	default:
		User::Invariant();
		break;
		}

	// Both the input and output streams should be in the Closed state - this
	// socket controller is no longer useful.
	if( iStore )
		{
		// Socket is closing, check if an immediate (abortive) socket close is required
		if(iConnectionPrefsProvider.ImmediateSocketShutdown())
			{
			// This will have no effect on secure connections, secure connections will
			// be closed with default behaviour when iScoket is destroyed in the d'tor
			iSocket->ShutdownImmediate();
			}
		
		// This socket controller is in the socket controller store - need to 
		// remove it from there.
		iStore->SocketControllerShutdown(*this);
		
		// This object is now ownerless - it should be deleted.
		delete this;
		}
	// This socket controller is not owned by the socket controller store - the
	// owner (probably the cleanup stack) will delete it.
	}

void CSocketController::StreamSuspend(MSocketController::TStreamType aStreamType)
/**	
	@see		MSocketController
*/
	{
	// Suspend the other stream...
	switch( aStreamType )
		{
	case EInputStream:
		{
		// This is not supported yet!
		User::Invariant();
		} break;
	case EOutputStream:
		{
		// The output stream has suspended the socket - inform the socket reader
		iSocketReader->Suspend();
		} break;
	default:
		User::Invariant();
		break;
		}
	}

void CSocketController::StreamResume(MSocketController::TStreamType aStreamType)
/**	
	@see		MSocketController
*/
	{
	// Resume the other stream...
	switch( aStreamType )
		{
	case EInputStream:
		{
		// This is not supported yet!
		User::Invariant();
		} break;
	case EOutputStream:
		{
		// The output stream has resumed the socket - inform the socket reader
		iSocketReader->Resume();
		} break;
	default:
		User::Invariant();
		break;
		}
	}

#ifdef	_DEBUG
void CSocketController::ConnectionInfo(TDes8& aRemoteHost, TUint16& aRemotePort, TUint16& aLocalPort)
#else
void CSocketController::ConnectionInfo(TDes8& /*aRemoteHost*/, TUint16& /*aRemotePort*/, TUint16& /*aLocalPort*/)
#endif
/**	
	@see		MSocketController
*/	{
#ifdef _DEBUG
	__ASSERT_DEBUG( aRemoteHost.MaxLength() >= KIpv6MaxAddrSize, User::Invariant() );

	TInetAddr addr;
	iSocket->RemoteName(addr);
	
	TBuf<KIpv6MaxAddrSize> ip16bit;
	addr.Output(ip16bit);

	aRemoteHost.Copy(ip16bit);
	aRemotePort = static_cast<TUint16>(addr.Port());

	TInetAddr local;
	iSocket->LocalName(local);
	aLocalPort = static_cast<TUint16>(local.Port());
#else
	User::Invariant();
#endif
	}

TBool  CSocketController::HostAndPortMatches(const TDesC8& aHost, TUint16 aPort)
    {
    if(iHost == NULL)
        return EFalse;
    
    if(aHost.Compare(*iHost) == 0 && aPort == iPort)
        return ETrue;
    
    return EFalse;    
    }

void CSocketController::AssignRemoteHostInfoL(const TDesC& aHost, TUint16 aPort, const TInetAddr& aAddr)
    {
    iHost = EscapeUtils::ConvertFromUnicodeToUtf8L(aHost);
    iPort = aPort;
    iRemoteAddress = aAddr;
    }



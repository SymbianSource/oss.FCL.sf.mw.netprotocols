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
// Telnet Protocol API
// CTelnetResolver implementation
// 
//

/**
 @file 
*/


#include "TELRESOL.H"
#include "IOBUFFER.H"
#include "TELCTRL.H"
#include "ACTIVEIO.H"
#include "TELDEBUG.H"


CTelnetResolver::CTelnetResolver() : CActive(EPriorityStandard)
/**
Constructor
*/
    {
    CActiveScheduler::Add(this);
    }

CTelnetResolver::~CTelnetResolver()
/**
Destructor
*/
    {
	__FLOG_STATIC(KTelnetLoggingCompnt(),KTelnetLoggingTag(),_L("CTelnetResolver::D'Tor"));
	iSocket.Close();
	iResolver.Close();
    iSocketServ.Close();
	Cancel();
    }

CTelnetResolver* CTelnetResolver::NewL(MTelnetResolver* aNotifier)
    {
    CTelnetResolver* self = new(ELeave) CTelnetResolver;
    CleanupStack::PushL(self);
    self->ConstructL(aNotifier);
    CleanupStack::Pop();
    return self;
    }

void CTelnetResolver::ConstructL(MTelnetResolver* aNotifier)
    {
    iState = EDisconnected;
	iEvent = ENone;

    User::LeaveIfError(iSocketServ.Connect());

    iNotify = aNotifier;
    }

void CTelnetResolver::TriggerActive(const TEvent aEvent)
/**
Helper method that triggers our active object RunL() and sets the cause
iEvent member which is interrogated in RunL() in the appropriate state
Currently NOT called from RunL() it'self
*/
	{
	iEvent = aEvent;
	SetActive();
	TRequestStatus *pS=&iStatus;
	User::RequestComplete(pS,KErrNone);
	}

TInt CTelnetResolver::IssueConnect(const TDesC& aServerName, TUint aPort)
/**
Called as a result of an API Connect.
Needs to use name resolution
*/

	{
	// Check for valid state
	if(iState != EDisconnected)
		return(KErrInUse);
	// Set member variable for the host name and the port
    TInt err;
	if((err = iResolver.Open(iSocketServ, KAfInet, KProtocolInetTcp)) != KErrNone)
		return(err);
	
	// Copy host name and port into our object
	iHostAddr.SetPort(aPort);
	iServerName = aServerName;
	// Make the async call to the resolver
	iResolver.GetByName(iServerName,iNameEntry,iStatus);
	// EDisconnected is the top of the state machine so set the state for Event() completion
	iState = ELookingUp;
	
	SetActive();
	return(KErrNone);
	}

TInt CTelnetResolver::IssueConnect(const TInetAddr& aInetAddr, TUint aPort)
/**
Called as a result of an API Connect.
IP address supplied
*/
	{
	if(iState != EDisconnected)
		return(KErrInUse);

	// copy IP and port to our object
	iHostAddr.SetPort(aPort);
	if (aInetAddr.Family() == KAfInet)
		iHostAddr.SetAddress(aInetAddr.Address());
	else
		iHostAddr.SetAddress(aInetAddr.Ip6Address());

	// Synchronous Open followed by Asynchronous Connect
	TInt err;
	if((err = iSocket.Open(iSocketServ, KAfInet, KSockStream, KProtocolInetTcp)) == KErrNone)
		{
		iSocket.Connect(iHostAddr, iStatus);
		// EDisconnected is top of the state machine so set the state for Event() completion
		iState = EConnecting;
		SetActive();
		}
	else
		{
		return(err);
		}

	return(KErrNone);
	}

TInt CTelnetResolver::IssueDisconnect()
/**
Called as a result of API Disconnect
*/
	{
	TInt ret;
	if(iState == EConnected)
		{
		// Requires action in the state machine so trigger RunL with the appropriate event
		// providing the we are connected.
		TriggerActive(EDoDisconnect);
		ret = KErrNone;
		}
	else if(iState == ELookingUp)
		{
		// Still looking up, cancel and reset states
		iResolver.Cancel();
		iResolver.Close();
		iState = EDisconnected;
		iEvent = ENone;
		ret = KErrServerBusy;
		__FLOG_STATIC(KTelnetLoggingCompnt(),KTelnetLoggingTag(),_L("CTelnetResolver::IssueDisconnect() Cancel ELookingUp"));
		}
	else if(iState == EConnecting)
		{
		// Opening the socket, cancel and reset states
		iState = EDisconnected;
		iEvent = ENone;
		iSocket.Close();
		ret = KErrServerBusy;
		__FLOG_STATIC(KTelnetLoggingCompnt(),KTelnetLoggingTag(),_L("CTelnetResolver::IssueDisconnect() Cancel EConnecting"));
		}
	else
		// Already Disconnecting
		ret = KErrDisconnected;

	return(ret);
	}

void CTelnetResolver::HandleEof()
/**
Called from FSM when it reads Eof
*/
	{
	// Requires action in the state machine so trigger RunL with the appropriate event
	TriggerActive(EEofDetected);
	}


void CTelnetResolver::DoCancel()
    {
    }

void CTelnetResolver::RunL()
    {
	// ONLY called from here
	Event();
    }

void CTelnetResolver::Event()
/**
State machine for the Telnet Resolver class.
Called ONLY from CTelnetResolver::RunL()
Main switch on the current state with checks for iEvent and/or iStatus depending on the state.
EDisconnected state is not in the machine as iState is ALWAYS set to ELookingUp or EConnecting
as a result of a client app connect request.
TODO Need to implement client app EDoDisconnect event in the EConnecting and ELookingUp states 
*/
	{
	// Switch on the state. Only certain events are legal in certain states
	switch(iState)
		{
	case	ELookingUp	:		
		__FLOG_STATIC(KTelnetLoggingCompnt(),KTelnetLoggingTag(),_L("CTelnetResolver::Event() ELookingUp"));
		// COMPLETION from host lookup (transitory state)
		// Finished with the resolver
		iResolver.Close();		
		// Completion of GetByName()
		if(iStatus == KErrNone)
			{
			// Get the first IP address from the list returned and copy it to our object
			iNameRecord = iNameEntry();
			if (TInetAddr::Cast(iNameRecord.iAddr).Family() == KAfInet)
				iHostAddr.SetAddress(TInetAddr::Cast(iNameRecord.iAddr).Address());
			else
				iHostAddr.SetAddress(TInetAddr::Cast(iNameRecord.iAddr).Ip6Address());

			// Get a socket and make async connection call
			if(iSocket.Open(iSocketServ, KAfInet, KSockStream, KProtocolInetTcp) == KErrNone)
				{
				iSocket.Connect(iHostAddr, iStatus);
				iState = EConnecting;
				SetActive();
				}				
			else
				{
				// ERROR
				__FLOG_STATIC(KTelnetLoggingCompnt(),KTelnetLoggingTag(),_L("CTelnetResolver::Event() ELookingUp ERROR 1"));
				iState = EDisconnected;
				iEvent = ENone;
				iNotify->ResolverDisconnected();
				}
			}
		else
			{
			__FLOG_STATIC(KTelnetLoggingCompnt(),KTelnetLoggingTag(),_L("CTelnetResolver::Event() ELookingUp ERROR 2"));
			iState = EDisconnected;
			iEvent = ENone;
			iNotify->ResolverDisconnected();
			}
		break;
								
	case	EConnecting	:
		__FLOG_STATIC(KTelnetLoggingCompnt(),KTelnetLoggingTag(),_L("CTelnetResolver::Event() EConnecting"));
		// COMPLETION from Connect (Transitory state)
		// TODO :- check iEvent for client EDoDisconnect
		// Just check the iStatus, the event code is not relevant
		if(iStatus == KErrNone)
			{
			// set the state and notify client we have connected
			iState = EConnected;
			TRAPD(err, iNotify->ResolverConnectedL());
			if(err!=KErrNone)
				{
				__FLOG_STATIC1(KTelnetLoggingCompnt(),KTelnetLoggingTag(),_L("CTelnetResolver::Event() EConnecting, ResolverConnectedL left with error = %d"),err);	
				iState = EDisconnected;
				iEvent = ENone;
				iSocket.Close();
				iNotify->ResolverDisconnected();
				break;
				}
			}
		else
			{
			// ERROR
			__FLOG_STATIC1(KTelnetLoggingCompnt(),KTelnetLoggingTag(),_L("CTelnetResolver::Event() EConnecting ERROR = %d"),iStatus.Int());
			iState = EDisconnected;
			iEvent = ENone;
			iSocket.Close();
			iNotify->ResolverDisconnected();
			}
		break;
				
	case	EConnected	:
		// ONLY NON transitory state
		// from the client app
		if(iEvent == EEofDetected)
			{
			// EOF from the line as a result of a Read (TCP connection being closed by remote end)
			// Reset the state, close the socket and notify the client app
			iState = EDisconnected;
			iEvent = ENone;
			iSocket.Close();
			iNotify->ResolverDisconnected();
			}
		else if(iEvent == EDoDisconnect)
			{
			// Client requested Disconnect
			// Shutdown output (Sends TCP FIN) , set the state
			// Asynchronous call
			iEvent = ENone;
			iSocket.Shutdown(RSocket::EStopOutput,iStatus);
			iState = EShuttingDown;
			SetActive();
			}
		else
			{
			// ERROR
			// BUG only transition here is on the above 2 events
			__FLOG_STATIC(KTelnetLoggingCompnt(),KTelnetLoggingTag(),_L("CTelnetResolver::Event() EConnected ERROR"));

			}

		break;

	case	EShuttingDown	:
		if(iStatus != KErrNone)
			{
			}
		iState = EDisconnecting;
		break;

	case	EDisconnecting	:
		// COMPLETION from client DoDisconnect shuddown()
		// EOF from the line is all we can expect here
		if(iEvent == EEofDetected)
			{
			iState = EDisconnected;
			iEvent = ENone;
			iSocket.Close();
			iNotify->ResolverDisconnected();
			}
		else
			{
			}
		break;
		
	default	:
		// ERROR
		break;
		}
	}

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
// Symbian Telnet Control class definition
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
#include "TELFSM.H"

CTelnetControl::CTelnetControl()
/**
Constructor
*/
	{
	}

CTelnetControl::~CTelnetControl()
/**
Destructor
*/
	{
	__FLOG_STATIC(KTelnetLoggingCompnt(),KTelnetLoggingTag(),_L("CTelnetControl::D'Tor"));

	// Active Object
	// It's Destructor :-
	//   Close() resolver
	//   Close() socket
	//   Close() socket server
	//   Cancel's()
	delete iTelnetResolver;

	// Active Objects
	// All Socket stuff is already closed therefore :-
	//   Their Destructors
	//   Cancel() only
	// 
	delete iPortReader;
	delete iPortWriter;

	// Not active and no new'd resource
	delete iPortIOControl;

	// Not Active
	// deletes all the RFC objects it new'd
	delete iProto;

	}

CTelnetControl* CTelnetControl::NewL(const TTelnetConfig& aConfig,MTelnetNotification* aTelnetNotification)
	{
	CTelnetControl* self = new(ELeave) CTelnetControl;
	CleanupStack::PushL(self);
	self->ConstructL(aConfig,aTelnetNotification);
	CleanupStack::Pop();
	return self;
	}

void CTelnetControl::ConstructL(const TTelnetConfig& aConfig,MTelnetNotification* aTelnetNotification)
	{
	iTelnetNotification = aTelnetNotification;

	iPortReadBuffer.SetLength(0);
	iClientReadBuffer.SetLength(0);
	iClientWriteOutstanding = FALSE;
	// Create support objects

	// Create the socket handler, pass in MTelnetResolver
	iTelnetResolver = CTelnetResolver::NewL(this);

	// Create the port i/o cooperating objects
	// IO control talks to us so pass in our MIONotification
	iPortIOControl = CIOBufferControl::NewL(this);
	// Port reader and write Active objects talk to PortIOControl so pass in it's MIONotification
	iPortWriter = CActiveWriter::NewL(iPortIOControl);
	iPortReader = CActiveReader::NewL(iPortIOControl);

	// Create the Telnet Protocol handler
	iProto = CProto::NewL(aConfig,this);
	}

// Two overloaded connect methods 

TInt CTelnetControl::Connect(const TDesC& aServerName, TUint aPort)
// Use host name resolver
	{
	if (iTelnetResolver->State() != CTelnetResolver::EDisconnected)
		return KErrInUse;
	Reset();
	TInt err;
	err = iTelnetResolver->IssueConnect(aServerName, aPort);
	return err;
	}

TInt CTelnetControl::Connect(const TInetAddr& aInetAddr, TUint aPort)
// Straight IP address
	{
	if (iTelnetResolver->State() != CTelnetResolver::EDisconnected)
		return KErrInUse;
	Reset();
	TInt err;
	err = iTelnetResolver->IssueConnect(aInetAddr, aPort);
	return err;
	}

void CTelnetControl::ResolverConnectedL()
	{
	__FLOG_STATIC(KTelnetLoggingCompnt(),KTelnetLoggingTag(),_L("CTelnetControl::ResolverConnected()"));

	// Resolver has called us back to say we're connected
	// Give the read and write active objects a pointer to the socket
	iPortWriter->SetSocket(iTelnetResolver->Socket());
	iPortReader->SetSocket(iTelnetResolver->Socket());
	iPortIOControl->SetWriter(iPortWriter);
	iPortIOControl->SetReader(iPortReader);

	TBuf8<128> tempBuffer;
	// Call into CProto to see if we need to issue any Telnet Protocol requests following connection
	iProto->GetInitOptions(tempBuffer);
	if(tempBuffer.Length())
		{
		// Allocate a heap buffer of the correct size, copy data into it and send it to buffer
		// Control class.
		// Ownership of the heap buffer is passed to Buffer Control
		HBufC8* actionBuffer = HBufC8::New(tempBuffer.Length());
		if(actionBuffer==NULL)
			{
			iTelnetNotification->Error(KErrNoMemory);
			User::Leave(KErrNoMemory);
			}
		TPtr8 ptr = actionBuffer->Des();
		ptr = tempBuffer;
		if(iPortIOControl->Write(actionBuffer) != KErrNone)
			delete actionBuffer;
		}

	// Notify the client that we've connected
	// Client should issue first read from here
	iTelnetNotification->Connected();
	}

void CTelnetControl::ResolverDisconnected()
/**
Socket handler has notified us that the TCP connection is closed
*/
	{
	__FLOG_STATIC(KTelnetLoggingCompnt(),KTelnetLoggingTag(),_L("CTelnetControl::ResolverDisconnected()"));

	// M interface call to the client app
	iTelnetNotification->ConnectionClosed();

	}

TInt CTelnetControl::Disconnect()
/**
Client app request to close the connection
*/
	{
	__FLOG_STATIC(KTelnetLoggingCompnt(),KTelnetLoggingTag(),_L("CTelnetControl::Disconnect()"));
	// Let the socket handler do this
	return(iTelnetResolver->IssueDisconnect());
	}

TInt CTelnetControl::Read()
/**
Client app has issued a read
*/
	{
	if (iTelnetResolver->State() == CTelnetResolver::EDisconnected)
		return KErrDisconnected;

	iPortReadBuffer.SetLength(0);
	return(iPortIOControl->Read(iPortReadBuffer));
	}


// Two overloaded write methods

TInt CTelnetControl::Write(TTelnetUserControl& aControlCode)
/**
Sends one of the Telnet control codes defined in Telsess.h
Certain of these characters require data to be sent as urgent
*/
	{
	TInt err = KErrNone;

	if (iTelnetResolver->State() != CTelnetResolver::EConnected)
		return KErrDisconnected;


	TBuf8<20> tempBuffer;
	TBuf8<20> tempUrgentBuffer;
	// Pass an ordinary buffer and a buffer for urgent data
	if((err = iProto->ProtoWrite(aControlCode,tempBuffer,tempUrgentBuffer)) == KErrNone)
		{
		if(tempUrgentBuffer.Length())
			// Urgent data to send
			// Get a HBuf and copy
			{
			// Allocate a heap buffer of the correct size, copy data into it and send it to buffer
			// Control class.
			// Ownership of the heap buffer is passed to Buffer Control
			HBufC8* urgentBuffer = HBufC8::New(tempUrgentBuffer.Length());
			if(urgentBuffer==NULL)
				return KErrNoMemory;
			TPtr8 urgentPtr = urgentBuffer->Des();
			urgentPtr = tempUrgentBuffer;
			if((err = iPortIOControl->WriteUrgent(urgentBuffer)) != KErrNone)
				delete urgentBuffer;
			}
		if(err == KErrNone && tempBuffer.Length())
			{
			// Allocate a heap buffer of the correct size, copy data into it and send it to buffer
			// Control class.
			// Ownership of the heap buffer is passed to Buffer Control
			HBufC8* buffer = HBufC8::New(tempBuffer.Length());
			if(buffer==NULL)
				return KErrNoMemory;
			TPtr8 ptr = buffer->Des();
			ptr = tempBuffer;
			if((err= iPortIOControl->Write(buffer)) != KErrNone)
				delete buffer;
			else
				iClientWriteOutstanding = TRUE;
			}
		}
	return(err);
	}

TInt CTelnetControl::Write(const TDesC8& aBuffer)
/**
Ordinary Client application write
We allocate a heap buffer of adequate size then pass it to I/O Buffer control
Ownership is also transferred
*/
	{
	
	if(iTelnetResolver->State() != CTelnetResolver::EConnected)
		return(KErrDisconnected);
	if(iClientWriteOutstanding)
		return KErrInUse;

	TInt err=KErrNone;

	// Create room for escapes + possible protocol appends
	HBufC8* buffer = HBufC8::New((aBuffer.Length() * 2) + 128);
	if(buffer==NULL)
		return KErrNoMemory;
	TPtr8 ptr = buffer->Des();
	
	// Proto method performs escapes etc
	err = iProto->ProtoWrite(aBuffer,ptr);

	if(err != KErrNone || !ptr.Length())
		{
		delete buffer;
		return err;
		}
	// Send the data to the server
	if((err = iPortIOControl->Write(buffer)) != KErrNone)
		delete buffer;
	else
		iClientWriteOutstanding = TRUE;
	return err;
	}

TInt CTelnetControl::SetConfig(const TTelnetConfig& aConfig)
/**
Client call to modify Telnet Configuration
In connected state we pass a buffer, because CProto may decide to negotiate an option
*/
	{
	TInt err = KErrNone;
	if (iTelnetResolver->State() == CTelnetResolver::EConnected)
		{
		TBuf8<128> tempBuffer;
		iProto->ModifyConfig(aConfig,&tempBuffer);
		if(tempBuffer.Length())
			{
			// Allocate a heap buffer of the correct size, copy data into it and send it to buffer
			// Control class.
			// Ownership of the heap buffer is passed to Buffer Control
			HBufC8* actionBuffer = HBufC8::New(tempBuffer.Length());
			if(actionBuffer==NULL)
				return KErrNoMemory;
			TPtr8 ptr = actionBuffer->Des();
			ptr = tempBuffer;
			if((err = iPortIOControl->Write(actionBuffer)) != KErrNone)
				delete actionBuffer;
			}
		}
	else
		// Config change with connection down
		iProto->ModifyConfig(aConfig,NULL);
	return err;
	}


TInt CTelnetControl::OptionStatus(TOptionStatus& aStatus)
/**
Client call to read the Telnet RFC options state
*/
	{
	if (iTelnetResolver->State() != CTelnetResolver::EConnected)
		return KErrDisconnected;
	TInt err = KErrNone;
	// Synchronous call into the CProto object
	iProto->OptionStatus(aStatus);
	// If RFC 859 is enabled for the Server, we can retrieve the server's perceived state of the options

	TBuf8<128> tempBuffer;
	iProto->ServerOptionStatus(tempBuffer);
	if(tempBuffer.Length())
		{
		// Allocate a heap buffer of the correct size, copy data into it and send it to buffer
		// Control class.
		// Ownership of the heap buffer is passed to Buffer Control
		HBufC8* actionBuffer = HBufC8::New(tempBuffer.Length());
		if(actionBuffer==NULL)
			return KErrNoMemory;
		TPtr8 ptr = actionBuffer->Des();
		ptr = tempBuffer;
		// Tell server to send it's perceived state of the RFC options
		// Currently we do nothing with them when the server returns them
		if((err = iPortIOControl->Write(actionBuffer)) != KErrNone)
			delete actionBuffer;
		}
	return err;
	}

TInt CTelnetControl::SetOption(const TInt aOption)
/**
Client trying to enable an RFC option 
*/
	{
	if (iTelnetResolver->State() != CTelnetResolver::EConnected)
		return KErrDisconnected;
	TInt err = KErrNone;

	TBuf8<128> tempBuffer;
	TInt32 event;
	// Call straight into CProto
	iProto->ClientRequestOption(aOption,tempBuffer,event);
	if(tempBuffer.Length())
		{
		// Allocate a heap buffer of the correct size, copy data into it and send it to buffer
		// Control class.
		// Ownership of the heap buffer is passed to Buffer Control
		HBufC8* actionBuffer = HBufC8::New(tempBuffer.Length());
		if(actionBuffer==NULL)
			return KErrNoMemory;
		TPtr8 ptr = actionBuffer->Des();
		ptr = tempBuffer;
		// Resultant write to the server
		if((err = iPortIOControl->Write(actionBuffer)) != KErrNone)
			delete actionBuffer;
		}
	return(err);
	}

void CTelnetControl::Reset()
/**
Called before connection attempts
Clears states and zeros buffers
Calls subordinate objects to reset
*/
	{
	iPortReadBuffer.SetLength(0);
	iClientReadBuffer.SetLength(0);
	iClientWriteOutstanding = FALSE;

	iPortIOControl->Reset();
	iProto->Reset();
	}



//
// M Interface callbacks from CIOBufferControl

void CTelnetControl::WriteComplete()
/**
Propogates the completion to the client if it has a write outstanding
*/
	{
	if(iClientWriteOutstanding)
		{
		iClientWriteOutstanding = FALSE;
		iTelnetNotification->WriteComplete();
		}
	}

void CTelnetControl::Event(TInt aEvent,TInt aEventCode)
/**
Urgent data event is the only one we're intersted in
aEventCode will be the byte that the TCP urgent pointed to
*/
	{
	if(aEvent == CProto::EEventUrgentData)
		{
		iProto->ReceiveUrgent(aEventCode);
		}
	}


void CTelnetControl::ReadCompleteL()
	{
	iClientReadBuffer.SetLength(0);
	// Call into the protocol object
	// ClientReadBuffer will have it's length set if there is client data
	TBuf8<256> tempBuffer;
	// Call the protocol object which will
	iProto->ProtoRead(iPortReadBuffer,iClientReadBuffer,tempBuffer);

	if(tempBuffer.Length())
		{
		// Allocate a heap buffer of the correct size, copy data into it and send it to buffer
		// Control class.
		// Ownership of the heap buffer is passed to Buffer Control
		HBufC8* actionBuffer = HBufC8::New(tempBuffer.Length());
		if(actionBuffer==NULL)
			{
			iTelnetNotification->Error(KErrNoMemory);
			User::Leave(KErrNoMemory);
			}
		TPtr8 ptr = actionBuffer->Des();
		// Copy the protocol data from stack copy
		ptr = tempBuffer;
		if(iPortIOControl->Write(actionBuffer) != KErrNone)
			delete actionBuffer;
		}
	if(iClientReadBuffer.Length())
		{
		iTelnetNotification->ReadComplete(iClientReadBuffer);
		}
	else
		{
		// Re-request a read if there was no data for the client
		iPortReadBuffer.SetLength(0);
		iPortIOControl->Read(iPortReadBuffer);
		}
	}


void CTelnetControl::ReadComplete(TInt aError)
/**
Will be a Read Error on the socket
We could pass the reason up to the client but is it worth it ?
the majority of the time it will be an ordinary disconnect.
Log the error and start close of the socket
*/
	{
	aError = aError;
	__FLOG_STATIC1(KTelnetLoggingCompnt(),KTelnetLoggingTag(),_L("CTelnetControl::ReadComplete() Disconnect Reason = %d"),aError);
	iTelnetResolver->HandleEof();
	}

void CTelnetControl::WriteError(TInt aError)
/**
Will be a Write error on the socket
*/
	{
	iTelnetNotification->Error(aError);
	}

void CTelnetControl::ResolverError(TInt aError)
/**
Client not interested at the moment so ignore
*/
	{
	iTelnetNotification->Error(aError);
	}

void CTelnetControl::ProtoError(TInt aError)
/**
Error from a RFC Option request
*/
	{
	iTelnetNotification->Error(aError);
	}

void CTelnetControl::ProtoEvent()
/**
RFC Option state has changed to Enabled/Disabled
Tell the Client
*/
	{
	iTelnetNotification->OptionsChanged();	
	}

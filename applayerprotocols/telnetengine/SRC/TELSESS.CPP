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
// CTelnetSession implementation
// 
//

/**
 @file
*/

#include "TELDEBUG.H"
#include "TELSESS.H"
#include "TELRESOL.H"
#include "IOBUFFER.H"
#include "TELCTRL.H"
#include "TELFSM.H"



// CTelnetSession implementation 

CTelnetSession::CTelnetSession()
/**
Constructor
*/
    {
    }

CTelnetSession::~CTelnetSession()
	/** Destructor. */
    {
	delete iTelnetProtocol;

	// DEBUG Close
	__FLOG_STATIC(KTelnetLoggingCompnt(),KTelnetLoggingTag(),_L("CTelnetSession D'Tor"));

    }

EXPORT_C CTelnetSession* CTelnetSession::NewL(const TTelnetConfig& aConfig,const MTelnetNotification* aNotifier)
	/** Allocates and constructs a new telnet session object.
	*
	* @param aConfig	The TTelnetConfig object to use. The client should derive from
	*					the TTelnetConfig class and pass this pointer into NewL().
	* @param aNotifier	The MTelnetNotification object to use. The client should derive from
	*					the MTelnetNotification class and pass this pointer into NewL().
	* @return			The newly created telnet session.
	*/
    {

    CTelnetSession* self=new (ELeave) CTelnetSession();
    CleanupStack::PushL(self);
    self->ConstructL(aConfig,aNotifier);
    CleanupStack::Pop();
    return self;
    }


EXPORT_C void CTelnetSession::ConstructL(const TTelnetConfig& aConfig,const MTelnetNotification* aNotifier)
	/** Second-phase protected constructor, called by NewL(). 
	* 
	* @see CTelnetSession::NewL() 
	*/
    {


	__FLOG_STATIC(KTelnetLoggingCompnt(),KTelnetLoggingTag(),_L("CTelnetSession::ConstructL"));

	iNotifier = CONST_CAST(MTelnetNotification*, aNotifier);
	iTelnetProtocol = CTelnetControl::NewL(aConfig,iNotifier);
    }

EXPORT_C TInt CTelnetSession::Connect(const TInetAddr& aNetAddr)
	/** Initiates connection to the specified IP address on the standard telnet port (port 23).
	* 
	* The client is asynchronously notified of successful connection by the MTelnetNotification::Connected() function.
	*
	* @param aNetAddr	The IP address to which connection is to be made.
	* @return			@li KErrNone,	if successful 
	*					@li KErrInUse,	if already connected or if an attempt to connect is already in progress.
	*/
    {
    TBuf<20> aBuf;
    aNetAddr.Output(aBuf);
    return (iTelnetProtocol->Connect(aNetAddr, 23));
    }

EXPORT_C TInt CTelnetSession::Connect(const TInetAddr& aNetAddr, TUint aPort)
	/** Initiates connection to the specified IP address and telnet port.
	*
	* The client is asynchronously notified of successful connection by the MTelnetNotification::Connected() 
	* function.
	* 
	* @param aNetAddr	The address to which connection is to be made.
	* @param aPort		The port through which connection is to be made.
	* @return			@li KErrNone,	if successful
	*					@li KErrInUse,	if already connected or if an attempt to connect is already in progress. 
	*/
    {
    TBuf<20> aBuf;
    aNetAddr.Output(aBuf);
    return (iTelnetProtocol->Connect(aNetAddr, aPort));
    }

EXPORT_C TInt CTelnetSession::Connect(const TDesC& aServerName)
	/** Connects to the specified server on the standard telnet port (port 23).
	*
	* The client is asynchronously notified of successful connection by the 
	* MTelnetNotification::Connected() function.
	*
	* @param aServerName	The hostname of the server to which connection is to be made.
	* @return				@li KErrNone,	if successful
	*						@li KErrInUse,	if already connected or an attempt to connect is already in progress. 
	*/
    {
    return (iTelnetProtocol->Connect(aServerName, 23));
    }

EXPORT_C TInt CTelnetSession::Connect(const TDesC& aServerName, TUint aPort)
	/** Connects to the specified server on the specified port.
	* 
	* The client is asynchronously notified of successful connection by the MTelnetNotification::Connected() 
	* function.
	*
	* @param aServerName	The hostname of the server to which connection is to be made.
	* @param aPort			The port through which connection is to be made.
	* @return				@li KErrNone,	if successful
	*						@li KErrInUse,	if already connected or if an attempt to connect is already in progress. 
	*/
    {
    return (iTelnetProtocol->Connect(aServerName, aPort));
    }

EXPORT_C TInt CTelnetSession::Disconnect()
	/** Requests disconnection from the server.
	* 
	* @return	@li KErrNone,			if successful; this will be followed by a Disconnected() notification
	* 									if the client previously received a Connected() notification 
	* 			@li KErrServerBusy,		if the socket is in a connecting state; in this case the client will
	*									not receive a ConnectionClosed() notification 
			@li KErrDisconnected,	if already already disconnecting. 
	*/
    {
    // Send Disconnect request to Server.
    __FLOG_STATIC(KTelnetLoggingCompnt(),KTelnetLoggingTag(),_L("TTelnetSession()::Disconnect()"));
    return iTelnetProtocol->Disconnect();
    }

EXPORT_C TInt CTelnetSession::Write(const TDesC8& aBuffer)
	/** Writes a byte stream over the open telnet connection to the telnet server.
	*
	* The client is notified asynchronously by the: 
	* @li MTelnetNotification::WriteComplete() function, if the write is successful 
	* @li MTelnetNotification::Error() function, if the write is not successful.
	* 
	* @param aBuffer	The data to write. 
	* @return			@li KErrNone,			if successful
	* 					@li KErrOverflow,		if a write buffer overflow occurred
	* 					@li KErrGeneral,		if an invalid control code was issued 
	* 					@li KErrDisconnected,	if disconnected
	* 					@li KErrNotSupported,	when in binary mode. 
	*/    
	{
    return (iTelnetProtocol->Write(aBuffer));
    }

EXPORT_C TInt CTelnetSession::Write(TTelnetUserControl& aControlCode)
	/** Writes telnet-defined code over the open telnet connection to the telnet server.
	* 
	* The client is asynchronously notified by the: 
	* @li MTelnetNotification::WriteComplete() function, if the write is successful 
	* @li MTelnetNotification::Error() function, if the write is not successful.
	* 
	* @param aControlCode	The telnet control code to write. See TTelnetUserControl enum.
	* @return				@li KErrNone,			if successful
	* 						@li KErrOverflow,		if a write buffer overflow occurred
	* 						@li KErrGeneral,		if an invalid control code was issued
	* 						@li KErrDisconnected,	if disconnected
	* 						@li KErrNotSupported,	when in binary mode. 
	*/    
	{
    // Should return eg KErrNotSupported? when in Binary_Mode
	return(iTelnetProtocol->Write(aControlCode));
    }

EXPORT_C TInt CTelnetSession::Read()
	/** Reads the data received over the telnet buffer. 
	* 
	* The client is asynchronously notified by the:* 
	* @li MTelnetNotification::ReadComplete() function, if the read is successful* 
	* @li MTelnetNotification::Error() function, if the read is not successful.
	* 
	* @return	@li KErrNone,			if successful
	*			@li KErrDisconnected,	if disconnected. 
	*/
    {
    return (iTelnetProtocol->Read());
    }

EXPORT_C TInt CTelnetSession::DoForceLogout()
	/** Forces a logout from the server.
	* 
	* @return	@li KErrNone,			if successful
	*			@li KErrDisconnected,	if disconnected. 
	*/
	{
	return(iTelnetProtocol->SetOption(KTelnetProtOptionLogoff));
	}

EXPORT_C TInt CTelnetSession::DoModifyConfig(TTelnetConfig& aConfig)
	/** Sets the telnet connection configuration.
	* 
	* If any options change status as a result, the client is notified through an OptionsChanged() call. 
	* 
	* @param aConfig	Telnet connection configuration.
	* @return			KErrNone, if successful; otherwise, one of the system wide error codes. 
	*/	
	{
	return(iTelnetProtocol->SetConfig(aConfig));
	}

EXPORT_C TInt CTelnetSession::OptionStatus(TOptionStatus& aStatus)
	/** Gets the status of the supported RFC-defined options.
	* 
	* See TOptionStatus for a description of options.
	* 
	* @param aStatus	On return, the status of the RFC-defined options.
	* @return			@li KErrNone,			if successful 
	* 					@li KErrDisconnected,	if disconnected. 
	*/
	{
	return(iTelnetProtocol->OptionStatus(aStatus));
	}


EXPORT_C void ClientStubOrdinal1() 
	{
	_LIT(KStubPanic, "TelSess.dll Stub");
	User::Panic(KStubPanic, KErrNotSupported);
	}

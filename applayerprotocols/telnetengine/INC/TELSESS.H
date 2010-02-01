/**
* Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:
* Telnet Session API
* 
*
*/



/**
 @file TELSESS.H
*/

#ifndef _TELSESS_H_
#define _TELSESS_H_

#include <in_sock.h>
#include <e32std.h>
#include <e32cons.h>
#include <f32file.h>

#define __TELNETSESSDEBUG__ // Put in Debug mode

class CTelnetControl;

// Used by client to retrieve RFC option state
// Flags for Telnet negotiated options
// 1 = Enabled 0 = Disabled

/**
Defines the supported, RFC-defined, option status.

@publishedAll
@released 
*/
typedef struct
    {
	/** If ETrue, the server is sending in binary (RFC 856). */
    TBool    iServerBinary; ///< RFC 856  (Server Sending in Binary)
	/** If ETrue, the client is sending in binary (RFC 856). */
    TBool    iClientBinary; ///< RFC 856  (Client Sending in Binary)
	/** If ETrue, the server is echoing data back to the client (RFC 857). */
    TBool    iEcho;         ///< RFC 857  (Server Echoing Data from Client)
	/** If ETrue, the client will negotiate about window size (RFC 1073). */
    TBool    iNAWS;         ///< RFC 1073 (Client Providing Window Size Information)
	/** If ETrue, the terminal speed option is enabled (RFC 1079). */
    TBool    iTerminalSpeed;///< RFC 1079 (Client Providing Terminal Speed Information)
	/** If ETrue, the client is providing terminal type information (RFC 1091). */
    TBool    iTerminalType; ///< RFC 1091 (Client Providing Terminal Type Information)
	/** If ETrue, if the server is providing status information (RFC 859). */
    TBool    iServerStatus; ///< RFC 859  (Server Providing Status Information)
	/** If ETrue, if the client is providing status information (RFC 859). */
    TBool    iClientStatus; ///< RFC 859  (Client Providing Status Information)
    }TOptionStatus;

// Used by client to configure the Symbian Telnet
class TTelnetConfig
/**
* Specifies telnet session configuration information.
* 
* The client must provide a reference to an instance of this class as a parameter 
* to the CTelnetSession::NewL() function. 
* 
* Configuration options can be modified, when the session is in progress, using 
* the CTelnetSession::DoModifyConfig() function. 
* 
* Configuration option state can be obtained using the CTelnetSession::OptionStatus() 
* function.
* @publishedAll
* @released 
*/
    {
 public:

	/** Constructor. */
	 TTelnetConfig() {
		              iAllowLogout = TRUE;        // RFC 727  - Default TRUE = Server Can Logout Client
					  iWindowSize.x = (TInt16)80; // RFC 1073 - Default 80 x 24
					  iWindowSize.y = (TInt16)24;
					  iTermSpeed = _L8("38400");  // RFC 1079 - Default to 38400 Baud Terminal
					  iTermType  = _L8("dumb");   // RFC 1091 - Default to base NVT Terminal
					}
	/** Destructor. */
	 ~TTelnetConfig(){}

	typedef struct
		/** 
		* Specifies the client's window size.
		* 
		* The default is 80 columns by 24 rows which constructs to x=80, y=24. 
		*/
		{
		/** The client window's width (number of columns). */
		TInt16 x;
		/** The client window's height (number of rows). */
		TInt16 y;
		}TWindowSize;

	/** 
	* Specifies the client's window size.
	* 
	* The default is 80 columns by 24 rows. 
	* 
	* (RFC 1073 -- Set to Configure Window Size) 
	*/
	TWindowSize	iWindowSize; 
	
	/**
	* Specifies the speed of the telnet connection.
	* 
	* The default is "38400" (in ASCII characters). Must be set to a server-supported rate.
	* 
	* (RFC 1079 -- Set to Configure Terminal Speed) 
	*/
	TBuf8<20>	iTermSpeed;  
	
	/**
	* Specifies the terminal type.
	* 
	* The default is "dumb" (in ASCII characters). 
	* 
	* (RFC 1091 -- Set to Configure Terminal Type) 
	*/
	TBuf8<20>	iTermType;
	
	/**
	* Specifies whether the telnet server can logout the client.
	* 
	* The default, ETrue, enables the server to logout the client. If EFalse any 
	* logout request from the telnet server is rejected by the client.
	*
	* (RFC 727 -- Set/Clear to Enable/Disable Server Logout) 
	*/
	TBool		iAllowLogout;
	
	/**
	* Enables or disables telnet server echo to client.
	* 
	* The default, ETrue, enables server echo. Note that although this is normal 
	* telnet server behaviour, it is not a formal telnet requirement. It is therefore 
	* recommended that the client implement an optional local echo. 
	*
	* (RFC 857 -- Set/Clear to Enable/Disable Server Echo to client) 
	*/
	TBool		iServerEcho;
    };

// Client can pass these in overloaded CTelnetSession::Write(TTelnetUserControl& aControlCode);

/**
* Specifies the telnet control code to send to the server.
*
* This is done using the overloaded CTelnetSession::Write() function.
* @publishedAll
* @released 
*/
enum TTelnetUserControl 
    {
	/** Control code not supported. */
    ENotSupported = 0,
	
	/**
	* Break (the NVT 129th ASCII break code).
	* The default is 243. This is not a valid control in Binary mode. 
	*/
    EBrk	= 243,
	
	/**
	* Interupt Process.
	* The default is 244.
	*/
    EIp		= 244,
	
	/** 
	* Abort Output.
	* The default is 245. 
	*/
    EAo		= 245,
	
	/**
	Are you there?
	* The default is 246. 
	*/
    EAyt	= 246,
	
	/**
	* Erase Character. 
	* The default is 247. This is not a valid control in Binary mode. 
	*/
    EEc		= 247,
	
	/** 
	* Erase Line.
	* The default is 248. This is not a valid control in Binary mode. 
	*/
    EEl		= 248,
    };

// Client MUST override this class
// Client MUST provide a pointer to an instance of this class as a parameter to CTelnetSession::NewL()
class MTelnetNotification
/**
* Symbian telnet notification interface.
* 
* Informs the client when an error occurs, when reads and writes have completed, 
* when a connection is made or closed and when configuration options change.
* @publishedAll
* @released 
*/
    {
 public:
	/** 
	* Reports errors on the Telnet connection.
	*
	* The error code is supplied by the server. If this function is called, it is 
	* likely that the connection has failed. Therefore it is recommended that the 
	* client disconnects and resets its state.
	* 
	* @param aError	Error code from server. 
	 */
    virtual void Error(TInt aError) = 0; ///< Miscellaneous Error callback
	
	/** 
	* Passes the data that was received from the server to the client.
	* 
	* It is recommended that the client copies the data from this buffer, as it 
	* is owned by the Symbian telnet.
	* 
	* @param aBuffer	Data received from server. 
	*/
    virtual void ReadComplete(const TDesC8& aBuffer) = 0; ///< Last Read to Server completion callback
	
	/** Notifies the client that a write to the server from the client has completed. */
    virtual void WriteComplete() = 0; ///< Last Write to Server completion callback
	
	/** Notifies the client that the connection to the server has been closed. */
    virtual void ConnectionClosed() = 0; ///< Telnet Connection Closed callback
	
	/** Notifies the client that a connection to the server has been made. */
    virtual void Connected() = 0; ///< Telnet Connected callback
	
	/** Notifies the client that telnet configuration options have changed. */
    virtual void OptionsChanged() = 0;                    ///< Telnet RFC Options Changed callback
    };

class CTelnetControl;

class CTelnetSession : public CBase
/**
* The main Symbian telnet class.
* 
* Consists of a connection interface and a protocol interface. The connection 
* interface comprises member functions that: connect to, read from, write to 
* and disconnect from a telnet server.
* 
* The protocol interface includes member functions that get and set telnet configuration.
*
* @publishedAll
* @released 
*/
    {
    //public:
    // Data Structures
 public:
    ~CTelnetSession();
    // Construction
    // Need to add max buffer size to constructor [check]
    IMPORT_C static CTelnetSession* NewL(const TTelnetConfig& aConfig,const MTelnetNotification* aNotifier);
 protected:
    IMPORT_C void ConstructL(const TTelnetConfig& aConfig,const MTelnetNotification* aNotifier);
    //
    // Connection Interface
    //
 public:
    IMPORT_C TInt Connect(const TInetAddr& aNetAddr); ///< Connects to aNetAddr on the standard telnet port (port 23)
    IMPORT_C TInt Connect(const TInetAddr& aNetAddr, TUint aPort); ///< Connects to aNetAddr on user specified port
    IMPORT_C TInt Connect(const TDesC& aServerName); ///< Connects to aSeverName on the standard telnet port (port 23)
    IMPORT_C TInt Connect(const TDesC& aServerName, TUint aPort); ///< Connects to aServerName on user specified port
    IMPORT_C TInt Disconnect(); ///< Disconnects current connection 
    IMPORT_C TInt Write(const TDesC8& aBuffer); ///< Writes a byte stream over the open telnet connection.
    IMPORT_C TInt Write(TTelnetUserControl& aControlCode); ///< Writes NVT code to stream. Prepends \<IAC\> code (255)
    IMPORT_C TInt Read(); ///< Read data received over telnet buffer.
    
	//
	// Protocol Interface 
	//
	IMPORT_C TInt DoForceLogout();
	IMPORT_C TInt DoModifyConfig(TTelnetConfig& aConfig);
	IMPORT_C TInt OptionStatus(TOptionStatus& aStatus);
 private:
    CTelnetSession();

 private:
    MTelnetNotification* iNotifier;
    CTelnetControl* iTelnetProtocol;

    };


#endif // _TELNETPROTOCOLAPI_H_

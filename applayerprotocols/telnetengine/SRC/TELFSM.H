/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies).
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
*
*/


#ifndef _TELFSM_
#define _TELFSM_

#include <telsess.h>

// RFC defined codes/commands 

/**
End of subnegotiation
*/
static const TUint8 KTelnetSE	= 240;
	
/**
No Operation
*/
static const TUint8 KTelnetNOP	= 241;
	
/**
Data Mark
*/
static const TUint8 KTelnetDM	= 242;	
/**
NVT character Break
*/
static const TUint8 KTelnetBRK	= 243;
	
/**
NVT Interrupt Process
*/
static const TUint8 KTelnetIP	= 244;	

/**
NVT Abort Output
*/
static const TUint8 KTelnetAO	= 245;	

/**
NVT Are You There
*/
static const TUint8 KTelnetAYT	= 246;	

/**
NVT Erase Character
*/
static const TUint8 KTelnetEC	= 247;	

/**
NVT Erase Line
*/
static const TUint8 KTelnetEL	= 248;
	
/**
NVT Go Ahead Signal
*/
static const TUint8 KTelnetGA	= 249;	

/**
Start of subnegotiation
*/
static const TUint8 KTelnetSB	= 250;	

/**
Indicates the desire to begin/confirm option
*/
static const TUint8 KTelnetWILL	= 251;
	
/**
Indicates the refusal to begin/continue option
*/
static const TUint8 KTelnetWONT	= 252;
	
/**
Indicates the request that the other party begin/confirmation of option
*/
static const TUint8 KTelnetDO	= 253;	

/**
Indicates the demand to stop/confirm that the option is no longer expected
*/
static const TUint8 KTelnetDONT	= 254;
	
/**
NVT Interpret As Command escape character
*/
static const TUint8 KTelnetIAC	= 255;	

static const TUint8 KTelnetCommandSEND	= 1;
static const TUint8 KTelnetCommandIS	= 0;


// Supported RFC defined options 

static const TUint8 KTelnetProtOptionEcho			= 1;
static const TUint8 KTelnetProtOptionSuppressGA		= 3;
static const TUint8 KTelnetProtOptionBinary			= 0;
static const TUint8 KTelnetProtOptionTerminalSpeed	= 32;
static const TUint8 KTelnetProtOptionTerminalType	= 24;
static const TUint8 KTelnetProtOptionWindowSize		= 31;
static const TUint8 KTelnetProtOptionLogoff			= 18;
static const TUint8 KTelnetProtOptionStatus			= 5;

// RFC defined NVT Character constants 

static const TUint8 KTelnetCR			= 0x0D;
static const TUint8 KTelnetLF			= 0x0A;
static const TUint8 KTelnetNULL			= 0x00;

class MProtoEvent;

class TOptionFSM
/**
RFC 1143 Bidirectional RFC option negotiation
This class contains the core Us/Him state machine as defined in RFC 1143

@internalComponent
*/
{
public:

	/** Option Negotiation requests for both directions */
	enum TRequests
		{
		ENoOption,EClientDo = 1,EClientWill = 2,EServerDo = 4, EServerWill = 8,
		EClientWont,EClientDont,EServerWont,EServerDont
		};

	/** Return values from Request */
	enum TRequestReturns
		{
		EErrNone,EProtocolError,EPermissionsError
		};

	/** Actions as a result of Request */
	enum TActions
		{
		ENoAction,ESendDo,ESendWill,ESendDont,ESendWont
		};

	/** Events as a result of Request */
	enum TEvents
		{
		ENoEvent,EServerEnabled = 1,EServerDisabled = 2,EClientEnabled = 4,EClientDisabled = 8
		};
	
	// Key RFC 1143 method that takes requests from client or server side
	TInt32	Request(const TInt32 aRequest,TInt32& aAction,TInt32& aEvent);
	// Set the permissission on an option, maximum of :-
	// EClientDo | EClientWill | EServerDo | EServerWill
	void	SetPermission(const TInt32 aRequestPermissions){Allowed = aRequestPermissions;}
	
	// There are intermmediate WANT states but we only return enabled/disabled
	TBool	ServerEnabled(){return(Him == EYes);}
	TBool	ClientEnabled(){return(Us == EYes);}
	// Reset the state machine
	void	Reset();

	TOptionFSM();
	~TOptionFSM(){}

private:
	// Permissions, maximum of :-
	// EClientDo | EClientWill | EServerDo | EServerWill
	TInt32	Allowed;
	// Defined in RFC 1143
	TInt8	Us;
	TInt8	UsQ;
	TInt8	Him;
	TInt8	HimQ;
	
	// values for HimQ,UsQ
	enum TQueueStates
		{
		ENone,EEmpty,EOpposite
		};
											
	// Values for Us,Him
	enum TOptionStates
		{
		ENo,EYes,EWantYes,EWantNo
		};
};


NONSHARABLE_CLASS(CUnknownOption)	:	public CBase
/**
Handles any unsupported RFC options

@internalComponent
*/
	{
public:
	CUnknownOption(){}
	~CUnknownOption();
	void RequestUnknown(TInt8 aOption,TInt32 aRequest,TDes8& aAction,TInt32& aEvent);
	void Reset(){iFSM.Reset();}

	static CUnknownOption* NewL();

protected:
	void ConstructL();

	TOptionFSM iFSM;
	};


NONSHARABLE_CLASS(CRFCOptionBase)	:	public	CBase
/**
@internalComponent
*/
	{
public:
	CRFCOptionBase(){}
	~CRFCOptionBase();

	static void CreateOptionResponse(TInt8 aOption,TInt32 aAction,TDes8& aBuffer);
	void Reset(){iFSM.Reset();}
	void SetRequestPermission(const TInt32 aAllowed){iFSM.SetPermission(aAllowed);}	
protected:

	virtual TInt32  RequestOption(const TInt32 aRequest,TDes8& aAction,TInt32& aEvent) = 0;
	virtual void	GetTelnetOptionStatus(TDes8& aCurrentStatus) = 0;

	TOptionFSM iFSM;

	};

NONSHARABLE_CLASS(CSuppressGAOption) :		public CRFCOptionBase
/**
TELNET Suppress Go Ahead RFC 858

@internalComponent
*/
	{
public:
	CSuppressGAOption(){}
	~CSuppressGAOption(){}

	static  CSuppressGAOption* NewL();
	TInt32  RequestOption(const TInt32 aRequest,TDes8& aAction,TInt32& aEvent);
	void	GetTelnetOptionStatus(TDes8& aCurrentStatus);

protected:
	void ConstructL();

	};

NONSHARABLE_CLASS(CStatusOption)	:	public CRFCOptionBase
/**
TELNET Status Option RFC 859

@internalComponent
*/
	{
public:
	CStatusOption(){}
	~CStatusOption(){}

	static CStatusOption* NewL();
	TInt32  RequestOption(const TInt32 aRequest,TDes8& aAction,TInt32& aEvent);
	void	GetTelnetOptionStatus(TDes8& aCurrentStatus);
	TBool	ReceiveStatus(){return(iFSM.ServerEnabled());}
	TBool	SendStatus(){return(iFSM.ClientEnabled());}
protected:
	void ConstructL();

	};

NONSHARABLE_CLASS(CSpeedOption)	:	public CRFCOptionBase
/**
TELNET Terminal Speed Option RFC 1079

@internalComponent
*/
	{
public:
	CSpeedOption(){}
	~CSpeedOption(){}

	static CSpeedOption* NewL();
	TInt32  RequestOption(const TInt32 aRequest,TDes8& aAction,TInt32& aEvent);
	void	GetTelnetOptionStatus(TDes8& aCurrentStatus);
	void	GetTelnetSubOption(TDes8& aOutBuffer);
	TBool	Set(const TDesC8& aSpeed);
	TBool	SendSpeed(){return(iFSM.ClientEnabled());}

protected:
	void ConstructL();

private:
	TBuf8<20>	iTermSpeed;
	};

NONSHARABLE_CLASS(CLogoutOption)	:	public CRFCOptionBase
/**
TELNET Logout Option RFC 727

@internalComponent
*/
	{
public:
	CLogoutOption(){}
	~CLogoutOption(){}

	static  CLogoutOption* NewL();
	TInt32  RequestOption(const TInt32 aRequest,TDes8& aAction,TInt32& aEvent);
	void	GetTelnetOptionStatus(TDes8& aCurrentStatus);

protected:
	void ConstructL();
	};


NONSHARABLE_CLASS(CTerminalTypeOption)	:	public CRFCOptionBase
/**
TELNET Terminal Type Option RFC 1091

@internalComponent
*/
	{
public:
	CTerminalTypeOption(){}
	~CTerminalTypeOption(){}

	static CTerminalTypeOption* NewL();
	TInt32  RequestOption(const TInt32 aRequest,TDes8& aAction,TInt32& aEvent);
	void	GetTelnetOptionStatus(TDes8& aCurrentStatus);
	void	GetTelnetSubOption(TDes8& aOutBuffer);
	TBool	Set(const TDesC8& aType);
	TBool	SendTerminalType(){return(iFSM.ClientEnabled());}
	const TDesC8& TerminalType(){return(iTermType);}   

protected:
	void ConstructL();

private:
	TBuf8<40>	iTermType;
	};

NONSHARABLE_CLASS(CWindowSizeOption)	:	public CRFCOptionBase
/**
TELNET Window Size Option RFC 1073

@internalComponent
*/
	{
public:
	CWindowSizeOption(){}
	~CWindowSizeOption(){}

	static CWindowSizeOption* NewL();
	TInt32  RequestOption(const TInt32 aRequest,TDes8& aAction,TInt32& aEvent);
	void	GetTelnetOptionStatus(TDes8& aCurrentStatus);
	void	GetTelnetSubOption(TDes8& aOutBuffer);
	TBool	Set(const TTelnetConfig::TWindowSize& aSize);
	TBool	SendWindowSize(){return(iFSM.ClientEnabled());}

protected:
	void ConstructL();

private:
	TTelnetConfig::TWindowSize iWindowSize;
	};


NONSHARABLE_CLASS(CEchoOption)	:	public CRFCOptionBase
/**
TELNET Echo Option RFC 857

@internalComponent
*/
	{
public:
	CEchoOption(){}
	~CEchoOption(){}

	static CEchoOption* NewL();
	TInt32  RequestOption(const TInt32 aRequest,TDes8& aAction,TInt32& aEvent);
	void	GetTelnetOptionStatus(TDes8& aCurrentStatus);
	TBool	ReceiveEcho(){return(iFSM.ServerEnabled());}
protected:
	void ConstructL();

	};

NONSHARABLE_CLASS(CBinaryOption)	:	public CRFCOptionBase
/**
TELNET Binary Option RFC 856

@internalComponent
*/
	{
public:
	CBinaryOption(){}
	~CBinaryOption(){}

	static CBinaryOption* NewL();
	TInt32  RequestOption(const TInt32 aRequest,TDes8& aAction,TInt32& aEvent);
	void	GetTelnetOptionStatus(TDes8& aCurrentStatus);
	TBool	ReceiveBinary(){return(iFSM.ServerEnabled());}
	TBool	SendBinary(){return(iFSM.ClientEnabled());}

protected:
	void ConstructL();
	};

NONSHARABLE_CLASS(CProto) : public CBase
/**
The main TELNET RFC 854 coordinating class

@internalComponent
*/
	{
public:
	CProto();
	~CProto();
	static CProto* NewL(const TTelnetConfig& aConfig,MProtoEvent* aNotifier);
	TInt ProtoWrite(const TDesC8& aInBuffer,TDes8& aOutBuffer);
	TInt ProtoWrite(const TTelnetUserControl& aControlCode,TDes8& aBuffer,TDes8& aUrgentBuffer);
	TInt ProtoRead(const TDesC8& aPortReadBuffer,TDes8& aClientReadBuffer,TDes8& aProtoBuffer);
	TInt ClientRequestOption(TInt32 aOptionRequest,TDes8& aAction,TInt32& aEvent);
	void ModifyConfig(const TTelnetConfig& aConfig,TDes8* aAction);
	void ReceiveUrgent(TInt aUrgentData);
	void OptionStatus(TOptionStatus& aStatus);
	void ServerOptionStatus(TDes8& aActionBuffer);

	void GetInitOptions(TDes8& aActionBuffer);
	void Reset();

	enum TLineEvents
		{
		EEventNone,
		EEventUrgentData
		};
	
private:
	enum TReceiveState
		{
		ENormal,EIAC,ECommand,ESubOpt
		};

	enum TSubOptState
		{
		ESB,EOption,ERequest,EEndIAC
		};

	TBool					iUrgentFlag;
	TInt					iUrgentData;

	TReceiveState			iReceiveState;
	TSubOptState			iSubOptState;

	TBuf8<128>				iProtReadBuffer;
	TBuf8<128>				iServerStatus;

	CBinaryOption*			iBinary;
	CEchoOption*			iEcho;
	CWindowSizeOption*		iWindowSize;
	CTerminalTypeOption*	iTerminalType;
	CLogoutOption*			iLogout;
	CSpeedOption*			iSpeed;
	CSuppressGAOption*		iGA;
	CStatusOption*			iStatus;
	CUnknownOption*			iUnknown;

	void GetOptionStatus(TDes8& aOutBuffer);

	MProtoEvent* iNotifier;
protected :
	void ConstructL(const TTelnetConfig& aConfig,MProtoEvent* aNotifier);
	};

class MProtoEvent
/**
@internalComponent
*/
	{
public:
	virtual void ProtoError(TInt aError) = 0;
	virtual void ProtoEvent() = 0;
	};

#endif

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
// Supported RFC class implementations
// 
//

/**
 @file 
*/

#include	"TELFSM.H"
#include	"TELDEBUG.H"

// The option negotiation class
// Independent of the option 
// All option classes contain one of these state machine classes

TOptionFSM::TOptionFSM()
/**
Constructor
Set up the defaults
*/
	{
	Allowed = ENoOption;
	Us = ENo;
	UsQ = EEmpty;
	Him = ENo;
	HimQ = EEmpty;
	}

void TOptionFSM::Reset()
/**
Reset restores the state machine to default but leaves the permissions
*/
	{
	Us = ENo;
	UsQ = EEmpty;
	Him = ENo;
	HimQ = EEmpty;
	}

TInt32	TOptionFSM::Request(const TInt32 aRequest,TInt32& aAction,TInt32& aEvent)
/**
Implementation of RFC 1143 Q Method bidirectional Telnet option state machine
Can be called from the client side (aRequest = EClientXX) or the line side (aRequest = EServerXX).

errors are returned for illegal requests
aEvent is set when an option transits to enabled or disabled
aAction's are the Telnet defined DO, WILL, WONT, DONT
*/
	{
	TInt32 Ret = EErrNone;
	aAction = ENoAction;
	aEvent = ENoEvent;

	switch(aRequest)
		{
//
		case	EServerWill	:
		if(Him == ENo)
			{
			if(Allowed & EServerWill)
				{
				Him = EYes;				
				Ret = EErrNone;
				aEvent = EServerEnabled;
				aAction = ESendDo;
				}
			else
				{
				Ret = EErrNone;
				aEvent = ENoEvent;
				aAction = ESendDont;
				}
			}
		else if(Him == EYes)
			{
			Ret = EErrNone;
			aEvent = ENoEvent;
			aAction = ENoAction;
			}
		else if(Him == EWantNo && HimQ == EEmpty)
			{
			Him = ENo;
			Ret = EProtocolError;
			aEvent = EServerDisabled;
			aAction = ENoAction;
			}
		else if(Him == EWantNo && HimQ == EOpposite)
			{
			Him = EYes;
			HimQ = EEmpty;
			Ret = EProtocolError;
			aEvent = EServerEnabled;
			aAction = ENoAction;
			}
		else if(Him == EWantYes && HimQ == EEmpty)
			{
			Him = EYes;
			Ret = EErrNone;
			aEvent = EServerEnabled;
			aAction = ENoAction;
			}
		else if(Him == EWantYes && HimQ == EOpposite)
			{
			Him = EWantNo;
			HimQ = EEmpty;
			Ret = EErrNone;
			aEvent = ENoEvent;
			aAction = ESendDont;
			}
		break;
//
		case	EServerDo	:
		if(Us == ENo)
			{
			if(Allowed & EServerDo)
				{
				Us = EYes;				
				Ret = EErrNone;
				aEvent = EClientEnabled;
				aAction = ESendWill;
				}
			else
				{
				Ret = EErrNone;
				aEvent = ENoEvent;
				aAction = ESendWont;
				}
			}
		else if(Us == EYes)
			{
			Ret = EErrNone;
			aEvent = ENoEvent;
			aAction = ENoAction;
			}
		else if(Us == EWantNo && UsQ == EEmpty)
			{
			Us = ENo;
			Ret = EProtocolError;
			aEvent = EClientDisabled;
			aAction = ENoAction;
			}
		else if(Us == EWantNo && UsQ == EOpposite)
			{
			Us = EYes;
			UsQ = EEmpty;
			Ret = EProtocolError;
			aEvent = EClientEnabled;
			aAction = ENoAction;
			}
		else if(Us == EWantYes && UsQ == EEmpty)
			{
			Us = EYes;
			Ret = EErrNone;
			aEvent = EClientEnabled;
			aAction = ENoAction;
			}
		else if(Us == EWantYes && UsQ == EOpposite)
			{
			Us = EWantNo;
			UsQ = EEmpty;
			Ret = EErrNone;
			aEvent = ENoEvent;
			aAction = ESendWont;
			}
		break;
//

		case	EServerWont	:
		if(Him == ENo)
			{
			Ret = EErrNone;
			aEvent = ENoEvent;
			aAction = ENoAction;
			}
		else if(Him == EYes)
			{
			Him = ENo;
			Ret = EErrNone;
			aEvent = EServerDisabled;
			aAction = ESendDont;
			}
		else if(Him == EWantNo && HimQ == EEmpty)
			{
			Him = ENo;
			Ret = EErrNone;
			aEvent = EServerDisabled;
			aAction = ENoAction;
			}
		else if(Him == EWantNo && HimQ == EOpposite)
			{
			Him = EWantYes;
			HimQ = ENone;
			Ret = EErrNone;
			aEvent = ENoEvent;
			aAction = ESendDo;
			}
		else if(Him == EWantYes && HimQ == EEmpty)
			{
			Him = ENo;
			Ret = EErrNone;
			aEvent = EServerDisabled;
			aAction = ENoAction;
			}
		else if(Him == EWantYes && HimQ == EOpposite)
			{
			Him = ENo;
			HimQ = ENone;
			Ret = EErrNone;
			aEvent = EServerDisabled;
			aAction = ENoAction;
			}
		break;
//
		case	EServerDont	:
		if(Us == ENo)
			{
			Ret = EErrNone;
			aEvent = ENoEvent;
			aAction = ENoAction;
			}
		else if(Us == EYes)
			{
			Us = ENo;
			Ret = EErrNone;
			aEvent = EClientDisabled;
			aAction = ESendWont;
			}
		else if(Us == EWantNo && UsQ == EEmpty)
			{
			Us = ENo;
			Ret = EErrNone;
			aEvent = EClientDisabled;
			aAction = ENoAction;
			}
		else if(Us == EWantNo && UsQ == EOpposite)
			{
			Us = EWantYes;
			UsQ = ENone;
			Ret = EErrNone;
			aEvent = ENoEvent;
			aAction = ESendWill;
			}
		else if(Us == EWantYes && UsQ == EEmpty)
			{
			Us = ENo;
			Ret = EErrNone;
			aEvent = EClientDisabled;
			aAction = ENoAction;
			}
		else if(Us == EWantYes && UsQ == EOpposite)
			{
			Us = ENo;
			UsQ = ENone;
			Ret = EErrNone;
			aEvent = EClientDisabled;
			aAction = ENoAction;
			}
		break;
//
		case	EClientDo	:
		if(!(Allowed & EClientDo))
			{
			Ret = EPermissionsError;
			aEvent = ENoEvent;
			aAction = ENoAction;
			}
		else if(Him == ENo)
			{
			Him = EWantYes;
			Ret = EErrNone;
			aEvent = ENoEvent;
			aAction = ESendDo;
			}
		else if(Him == EYes)
			{
			Ret = EProtocolError;
			aEvent = ENoEvent;
			aAction = ENoAction;
			}
		else if(Him == EWantNo && HimQ == EEmpty)
			{
			if(UsQ == EOpposite)
				{
				Ret = EErrNone;
				HimQ = EOpposite;
				}
			else
				Ret = EProtocolError;
			aEvent = ENoEvent;
			aAction = ENoAction;
			}
		else if(Him == EWantNo && HimQ == EOpposite)
			{
			Ret = EProtocolError;
			aEvent = ENoEvent;
			aAction = ENoAction;
			}
		else if(Him == EWantYes && HimQ == EEmpty)
			{
			Ret = EProtocolError;
			aEvent = ENoEvent;
			aAction = ENoAction;
			}
		else if(Him == EWantYes && HimQ == EOpposite)
			{
			HimQ = EEmpty;
			Ret = EErrNone;
			aEvent = ENoEvent;
			aAction = ENoAction;
			}
		break;
//
		case	EClientWill	:
		if(!(Allowed & EClientWill))
			{
			Ret = EPermissionsError;
			aEvent = ENoEvent;
			aAction = ENoAction;
			}
		else if(Us == ENo)
			{
			Us = EWantYes;
			Ret = EErrNone;
			aEvent = ENoEvent;
			aAction = ESendWill;
			}
		else if(Us == EYes)
			{
			Ret = EProtocolError;
			aEvent = ENoEvent;
			aAction = ENoAction;
			}
		else if(Us == EWantNo && UsQ == EEmpty)
			{
			if(HimQ == EOpposite)
				{
				Ret = EErrNone;
				UsQ = EOpposite;
				}
			else
				Ret = EProtocolError;
			aEvent = ENoEvent;
			aAction = ENoAction;
			}
		else if(Us == EWantNo && UsQ == EOpposite)
			{
			Ret = EProtocolError;
			aEvent = ENoEvent;
			aAction = ENoAction;
			}
		else if(Us == EWantYes && UsQ == EEmpty)
			{
			Ret = EProtocolError;
			aEvent = ENoEvent;
			aAction = ENoAction;
			}
		else if(Us == EWantYes && UsQ == EOpposite)
			{
			UsQ = EEmpty;
			Ret = EErrNone;
			aEvent = ENoEvent;
			aAction = ENoAction;
			}
		break;
//
		case	EClientDont	:
		if(Him == ENo)
			{
			Ret = EProtocolError;
			aEvent = ENoEvent;
			aAction = ENoAction;
			}
		else if(Him == EYes)
			{
			Him = EWantNo;
			Ret = EErrNone;
			aEvent = ENoEvent;
			aAction = ESendDont;
			}
		else if(Him == EWantNo && HimQ == EEmpty)
			{
			Ret = EProtocolError;
			aEvent = ENoEvent;
			aAction = ENoAction;
			}
		else if(Him == EWantNo && HimQ == EOpposite)
			{
			HimQ = EEmpty;
			Ret = EErrNone;
			aEvent = ENoEvent;
			aAction = ENoAction;
			}
		else if(Him == EWantYes && HimQ == EEmpty)
			{
			if(UsQ == EOpposite)
				{
				HimQ = EOpposite;
				Ret = EErrNone;
				}
			else
				Ret = EProtocolError;
			aEvent = ENoEvent;
			aAction = ENoAction;
			}
		else if(Him == EWantYes && HimQ == EOpposite)
			{
			Ret = EProtocolError;
			aEvent = ENoEvent;
			aAction = ENoAction;
			}
		break;
//
		case	EClientWont	:
		if(Us == ENo)
			{
			Ret = EProtocolError;
			aEvent = ENoEvent;
			aAction = ENoAction;
			}
		else if(Us == EYes)
			{
			Us = EWantNo;
			Ret = EErrNone;
			aEvent = ENoEvent;
			aAction = ESendWont;
			}
		else if(Us == EWantNo && UsQ == EEmpty)
			{
			Ret = EProtocolError;
			aEvent = ENoEvent;
			aAction = ENoAction;
			}
		else if(Us == EWantNo && UsQ == EOpposite)
			{
			UsQ = EEmpty;
			Ret = EErrNone;
			aEvent = ENoEvent;
			aAction = ENoAction;
			}
		else if(Us == EWantYes && UsQ == EEmpty)
			{
			if(HimQ == EOpposite)
				{
				UsQ = EOpposite;
				Ret = EErrNone;
				}
			else
				Ret = EProtocolError;

			aEvent = ENoEvent;
			aAction = ENoAction;
			}
		else if(Us == EWantYes && UsQ == EOpposite)
			{
			Ret = EProtocolError;
			aEvent = ENoEvent;
			aAction = ENoAction;
			}
		break;

		default	:
		break;

	
		}
	return(Ret);
	}

// The base class for all RFC option classes 

CRFCOptionBase::~CRFCOptionBase()
/**
Destructor
*/
	{
	__FLOG_STATIC(KTelnetLoggingCompnt(),KTelnetLoggingTag(),_L("CRFCOptionBase::D'Tor"));
	}

void CRFCOptionBase::CreateOptionResponse(TInt8 aOption,TInt32 aAction,TDes8& aBuffer)
/**
Create a Telnet protocol request or response
*/
	{
	aBuffer.Append(KTelnetIAC);

	// MAP our local codes to the genuine telnet ones
	if(aAction == TOptionFSM::ESendWill)
		aBuffer.Append(KTelnetWILL);
	else if(aAction == TOptionFSM::ESendDo)
		aBuffer.Append(KTelnetDO);
	else if(aAction == TOptionFSM::ESendDont)
		aBuffer.Append(KTelnetDONT);
	else if(aAction == TOptionFSM::ESendWont)
		aBuffer.Append(KTelnetWONT);

	aBuffer.Append(aOption);
	};


void CUnknownOption::RequestUnknown(TInt8 aOption,const TInt32 aRequest, TDes8& aAction,TInt32& aEvent)
/**
For tidy code we keep a class that handles unknown option requests from the server.
The result of a call into this object will always be WONT or DONT
*/
	{
	TInt32 action;
	iFSM.Request(aRequest,action,aEvent);
	if(action != TOptionFSM::ENoAction)
		CRFCOptionBase::CreateOptionResponse(aOption,action,aAction);
	}

CUnknownOption* CUnknownOption::NewL()
	{
	CUnknownOption* self = new(ELeave) CUnknownOption;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

void CUnknownOption::ConstructL()
	{
	}

CUnknownOption::~CUnknownOption()
/**
Destructor
*/
	{
	__FLOG_STATIC(KTelnetLoggingCompnt(),KTelnetLoggingTag(),_L("CUnknownOption::D'Tor"));
	}

CSuppressGAOption* CSuppressGAOption::NewL()
	{
	CSuppressGAOption* self = new(ELeave) CSuppressGAOption;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

TInt32 CSuppressGAOption::RequestOption(const TInt32 aRequest, TDes8& aAction,TInt32& aEvent)
/**
Virtual overide
Should always be a request for DO or WILL as we always suppress the Go Ahead signal
*/
	{
	TInt32 action;

	iFSM.Request(aRequest,action,aEvent);
	if(action != TOptionFSM::ENoAction)
		CreateOptionResponse(KTelnetProtOptionSuppressGA,action,aAction);
	return(KErrNone);
	}

void CSuppressGAOption::ConstructL()
	{
	}

void CSuppressGAOption::GetTelnetOptionStatus(TDes8& aCurrentStatus)
/**
Virtual overide
Returns a Telnet Protocol option status , see RFC 859
*/
	{
	if(iFSM.ClientEnabled())
		{
		aCurrentStatus.Append(KTelnetDO);
		aCurrentStatus.Append(KTelnetProtOptionSuppressGA);
		}
	}


CStatusOption* CStatusOption::NewL()
	{
	CStatusOption* self = new(ELeave) CStatusOption;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

TInt32 CStatusOption::RequestOption(const TInt32 aRequest, TDes8& aAction,TInt32& aEvent)
/**
Virtual overide
*/
	{
	TInt32 action;

	iFSM.Request(aRequest,action,aEvent);
	if(action != TOptionFSM::ENoAction)
		CreateOptionResponse(KTelnetProtOptionStatus,action,aAction);

	return(KErrNone);
	}

void CStatusOption::ConstructL()
	{
	}

void CStatusOption::GetTelnetOptionStatus(TDes8& aCurrentStatus)
/**
Virtual overide
Returns a Telnet Protocol option status , see RFC 859
*/
	{
	if(iFSM.ClientEnabled())
		{
		aCurrentStatus.Append(KTelnetWILL);
		aCurrentStatus.Append(KTelnetProtOptionStatus);
		}
	if(iFSM.ServerEnabled())
		{
		aCurrentStatus.Append(KTelnetDO);
		aCurrentStatus.Append(KTelnetProtOptionStatus);
		}
	}

CSpeedOption* CSpeedOption::NewL()
	{
	CSpeedOption* self = new(ELeave) CSpeedOption;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

TInt32 CSpeedOption::RequestOption(const TInt32 aRequest, TDes8& aAction,TInt32& aEvent)
/**
Virtual overide
*/
	{
	TInt32 action;
	TInt32 err;
	err = iFSM.Request(aRequest,action,aEvent);
	if(action != TOptionFSM::ENoAction)
		CreateOptionResponse(KTelnetProtOptionTerminalSpeed,action,aAction);
	return(err);
	}

void CSpeedOption::GetTelnetSubOption(TDes8& aOutBuffer)
/**
Creates the sub option for telling the server our speed
*/
	{
	aOutBuffer.Append(KTelnetIAC); // Interpret As Command
	aOutBuffer.Append(KTelnetSB);  // Suboption start
	aOutBuffer.Append(KTelnetProtOptionTerminalSpeed); //Terminal Speed
	aOutBuffer.Append(KTelnetCommandIS); // IS

	aOutBuffer.Append(iTermSpeed); // Receive Speed
	aOutBuffer.Append(',');
	aOutBuffer.Append(iTermSpeed); // Send Speed

	aOutBuffer.Append(KTelnetIAC); // Interpret As Command
	aOutBuffer.Append(KTelnetSE);  // Suboption End
	}

void CSpeedOption::ConstructL()
	{
	iTermSpeed = _L8("38400");
	}

void CSpeedOption::GetTelnetOptionStatus(TDes8& aCurrentStatus)
/**
Virtual overide
Returns a Telnet Protocol option status , see RFC 859
*/
	{
	if(iFSM.ClientEnabled())
		{
		aCurrentStatus.Append(KTelnetWILL);
		aCurrentStatus.Append(KTelnetProtOptionTerminalSpeed);
		}
	}

TBool CSpeedOption::Set(const TDesC8& aSpeed)
/**
Sets the speed option, return TRUE if it's altered
*/
	{
	TBool ret;
	(iTermSpeed == aSpeed) ? (ret = FALSE) : (ret = TRUE);
	iTermSpeed = aSpeed;
	return ret;
	}

CLogoutOption* CLogoutOption::NewL()
	{
	CLogoutOption* self = new(ELeave) CLogoutOption;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

TInt32 CLogoutOption::RequestOption(const TInt32 aRequest, TDes8& aAction,TInt32& aEvent)
/**
Virtual overide
*/
	{
	TInt32 action;
	TInt32 err;
	err = iFSM.Request(aRequest,action,aEvent);
	if(action != TOptionFSM::ENoAction)
		CreateOptionResponse(KTelnetProtOptionLogoff,action,aAction);
	return(err);
	}

void CLogoutOption::ConstructL()
	{
	}

void CLogoutOption::GetTelnetOptionStatus(TDes8& aCurrentStatus)
/**
I don't think this method is relevant for status reporting but for consistency and is
never called to report it's status.
*/
	{
	if(iFSM.ServerEnabled())
		{
		aCurrentStatus.Append(KTelnetDO);
		aCurrentStatus.Append(KTelnetProtOptionLogoff);
		}
	}

CTerminalTypeOption* CTerminalTypeOption::NewL()
	{
	CTerminalTypeOption* self = new(ELeave) CTerminalTypeOption;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

TInt32 CTerminalTypeOption::RequestOption(const TInt32 aRequest, TDes8& aAction,TInt32& aEvent)
/**
Virtual overide
*/
	{
	TInt32 action;
	TInt32 err;
	err = iFSM.Request(aRequest,action,aEvent);
	if(action != TOptionFSM::ENoAction)
		CreateOptionResponse(KTelnetProtOptionTerminalType,action,aAction);
	return(err);
	}

void CTerminalTypeOption::ConstructL()
	{
	iTermType  = _L8("dumb");
	}

void CTerminalTypeOption::GetTelnetSubOption(TDes8& aOutBuffer)
	{
	aOutBuffer.Append(KTelnetIAC); // Interpret As Command
	aOutBuffer.Append(KTelnetSB);  // Suboption Start
	aOutBuffer.Append(KTelnetProtOptionTerminalType); // Terminal Type
	aOutBuffer.Append(KTelnetCommandIS); // IS
	aOutBuffer.Append(iTermType); // Terminal Type eg "dumb"
	aOutBuffer.Append(KTelnetIAC);// Interpret As Command
	aOutBuffer.Append(KTelnetSE); // Suboption End
	}

void CTerminalTypeOption::GetTelnetOptionStatus(TDes8& aCurrentStatus)
/**
Virtual overide
Returns a Telnet Protocol option status , see RFC 859
*/
	{
	if(iFSM.ClientEnabled())
		{
		aCurrentStatus.Append(KTelnetWILL);
		aCurrentStatus.Append(KTelnetProtOptionTerminalType);
		}
	}

TBool CTerminalTypeOption::Set(const TDesC8& aType)
/**
Sets the terminal type, returns TRUE if the type has altered
*/
	{
	TBool ret;
	(aType == iTermType) ? (ret = FALSE) : (ret = TRUE);
	iTermType = aType;
	return ret;
	}

CWindowSizeOption* CWindowSizeOption::NewL()
	{
	CWindowSizeOption* self = new(ELeave) CWindowSizeOption;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

TInt32 CWindowSizeOption::RequestOption(const TInt32 aRequest, TDes8& aAction,TInt32& aEvent)
	{
	TInt32 action;
	TInt32 err;
	err = iFSM.Request(aRequest,action,aEvent);
	if(action != TOptionFSM::ENoAction)
		CreateOptionResponse(KTelnetProtOptionWindowSize,action,aAction);
	return(err);
	}

void CWindowSizeOption::ConstructL()
	{
	iWindowSize.x = 80;
	iWindowSize.y = 24;
	}

TBool CWindowSizeOption::Set(const TTelnetConfig::TWindowSize& aSize)
/**
Sets the window size, returns TRUE if it's altered
*/
	{
	TBool ret;
	(iWindowSize.x == aSize.x && iWindowSize.y == aSize.y) ? (ret = FALSE) : (ret = TRUE);
	iWindowSize.x = aSize.x;
	iWindowSize.y = aSize.y;
	return ret;
	}

void CWindowSizeOption::GetTelnetSubOption(TDes8& aOutBuffer)
/**
Creates the window size suboption in short's
Make sure we escape any Interpret As Command 255's (unlikely to occur)
*/
	{
	aOutBuffer.Append(KTelnetIAC);                  // Interpret As Command
	aOutBuffer.Append(KTelnetSB);                   // Suboption Start
	aOutBuffer.Append(KTelnetProtOptionWindowSize); // Window Size (NAWS)

	TUint8 nibble;

	nibble = (TUint8)(iWindowSize.x >> 8);  // High Byte of x
	aOutBuffer.Append(nibble);              
	if(nibble == KTelnetIAC)                // Escape check
		aOutBuffer.Append(KTelnetIAC);
	nibble = (TUint8)iWindowSize.x;         // Low byte of x
	aOutBuffer.Append(nibble); 
	if(nibble == KTelnetIAC)                // Escape check          
		aOutBuffer.Append(KTelnetIAC);

	nibble = (TUint8)(iWindowSize.y >> 8);  // High byte of y
	aOutBuffer.Append(nibble);
	if(nibble == KTelnetIAC)                // Escape check
		aOutBuffer.Append(KTelnetIAC);
	nibble = (TUint8)iWindowSize.y;         // Low byte of y
	aOutBuffer.Append(nibble);
	if(nibble == KTelnetIAC)                // Escape check
		aOutBuffer.Append(KTelnetIAC);

	aOutBuffer.Append(KTelnetIAC);          // Interpret As Command
	aOutBuffer.Append(KTelnetSE);           // Suboption End
	}

void CWindowSizeOption::GetTelnetOptionStatus(TDes8& aCurrentStatus)
/**
Virtual overide
Returns a Telnet Protocol option status , see RFC 859
*/
	{
	if(iFSM.ClientEnabled())
		{
		aCurrentStatus.Append(KTelnetWILL);
		aCurrentStatus.Append(KTelnetProtOptionWindowSize);
		}
	}


CEchoOption* CEchoOption::NewL()
	{
	CEchoOption* self = new(ELeave) CEchoOption;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

TInt32 CEchoOption::RequestOption(const TInt32 aRequest,TDes8& aAction,TInt32& aEvent)
	{
	TInt32 action;
	TInt32 err;
	err = iFSM.Request(aRequest,action,aEvent);
	if(action != TOptionFSM::ENoAction)
		CreateOptionResponse(KTelnetProtOptionEcho,action,aAction);
	return(err);
	}

void CEchoOption::ConstructL()
	{
	}

void CEchoOption::GetTelnetOptionStatus(TDes8& aCurrentStatus)
/**
Virtual overide
Returns a Telnet Protocol option status , see RFC 859
*/
	{
	if(iFSM.ClientEnabled())
		{
		aCurrentStatus.Append(KTelnetWILL);
		aCurrentStatus.Append(KTelnetProtOptionEcho);
		}
	}


CBinaryOption* CBinaryOption::NewL()
	{
	CBinaryOption* self = new(ELeave) CBinaryOption;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

TInt32 CBinaryOption::RequestOption(const TInt32 aRequest,TDes8& aAction,TInt32& aEvent)
	{
	TInt32 action;
	TInt32 err;

	err = iFSM.Request(aRequest,action,aEvent);
	if(action != TOptionFSM::ENoAction)
		CreateOptionResponse(KTelnetProtOptionBinary,action,aAction);
	return(err);
	}

void CBinaryOption::ConstructL()
	{
	}

void CBinaryOption::GetTelnetOptionStatus(TDes8& aCurrentStatus)
/**
Virtual overide
Returns a Telnet Protocol option status , see RFC 859
*/
	{
	if(iFSM.ClientEnabled())
		{
		aCurrentStatus.Append(KTelnetWILL);
		aCurrentStatus.Append(KTelnetProtOptionBinary);
		}
	}

CProto* CProto::NewL(const TTelnetConfig& aConfig,MProtoEvent* aNotifier)
	{
	CProto* self = new(ELeave) CProto;
	CleanupStack::PushL(self);
	self->ConstructL(aConfig,aNotifier);
	CleanupStack::Pop();
	return self;
	}

void CProto::ConstructL(const TTelnetConfig& aConfig,MProtoEvent* aNotifier)
/**
Initialises the protocol object and creates the RFC objects
*/
	{
	iNotifier =				aNotifier;

	iReceiveState =			ENormal;
	iUrgentFlag =			FALSE;

	iBinary =				CBinaryOption::NewL();
	iEcho =					CEchoOption::NewL();
	iWindowSize =			CWindowSizeOption::NewL();
	iTerminalType =			CTerminalTypeOption::NewL();
	iLogout =				CLogoutOption::NewL();
	iSpeed =				CSpeedOption::NewL();
	iGA =					CSuppressGAOption::NewL();
	iStatus =				CStatusOption::NewL();
	iUnknown =				CUnknownOption::NewL();

	// Set the permitted requests for all the RFC objects
	// Logout permissions are set from the aConfig
	// Echo permissions are set from aConfig
	iWindowSize->SetRequestPermission
					(
					TOptionFSM::EServerDo | TOptionFSM::EClientWill
					);

	iStatus->SetRequestPermission
					(
					TOptionFSM::EServerWill | TOptionFSM::EClientDo |
					TOptionFSM::EClientWill | TOptionFSM::EServerDo
					);
	
	iSpeed->SetRequestPermission
					(
					TOptionFSM::EServerDo | TOptionFSM::EClientWill
					);
	
	iTerminalType->SetRequestPermission
					(
					TOptionFSM::EServerDo | TOptionFSM::EClientWill 
					);
	
	iBinary->SetRequestPermission
					(
					TOptionFSM::EClientDo | TOptionFSM::EServerWill |
					TOptionFSM::EServerDo | TOptionFSM::EClientWill
					);

	iGA->SetRequestPermission
					(
					TOptionFSM::EClientDo | TOptionFSM::EServerWill |
					TOptionFSM::EServerDo | TOptionFSM::EClientWill
					);
	// NULL means we don't want a protocol string returned to send to the server
	ModifyConfig(aConfig,NULL);
	}

void CProto::ReceiveUrgent(TInt aUrgentData)
/**
We've received an urgent notification
aUrgent will be the urgent data byte, currently we're not interested in what it is 
*/
	{
	iUrgentFlag = TRUE;
	iUrgentData = aUrgentData;
	}

void CProto::GetOptionStatus(TDes8& aOutBuffer)\
/**
Called when the server wants to know our perceived state of the Telnet option RFC's
RFC member methods add WILL/DO KTelnetProtOptionXXXX, if its currently enabled for the client
*/
	{
	aOutBuffer.Append(KTelnetIAC);                    // Interpret As Command
	aOutBuffer.Append(KTelnetSB);                     // Suboption Start
 	aOutBuffer.Append(KTelnetProtOptionStatus);       // KTelnetProtOptionStatus
	aOutBuffer.Append(KTelnetCommandIS);              // IS

	iBinary->GetTelnetOptionStatus(aOutBuffer);       // ? WILL Binary
	iWindowSize->GetTelnetOptionStatus(aOutBuffer);   // ? WILL NAWS
	iTerminalType->GetTelnetOptionStatus(aOutBuffer); // ? WILL Terminal Type
	iSpeed->GetTelnetOptionStatus(aOutBuffer);        // ? WILL Terminal Speed
	iGA->GetTelnetOptionStatus(aOutBuffer);           // ? DO Suppress Go Ahead
	iStatus->GetTelnetOptionStatus(aOutBuffer);       // ? WILL and/or DO Status  

	aOutBuffer.Append(KTelnetIAC);                    // Interpret As Command
	aOutBuffer.Append(KTelnetSE);                     // Suboption End
	}

void CProto::OptionStatus(TOptionStatus& aStatus)
/**
Client has requested the state of the RFC options
*/
	{
	(iBinary->ReceiveBinary() == TRUE) ?			(aStatus.iServerBinary = TRUE)	: (aStatus.iServerBinary = FALSE);
	(iBinary->SendBinary() == TRUE) ?				(aStatus.iClientBinary = TRUE)	: (aStatus.iClientBinary = FALSE);
	(iEcho->ReceiveEcho() == TRUE) ?				(aStatus.iEcho = TRUE)			: (aStatus.iEcho = FALSE);
	(iWindowSize->SendWindowSize()) ?				(aStatus.iNAWS = TRUE)			: (aStatus.iNAWS = FALSE);
	(iSpeed->SendSpeed() == TRUE) ?					(aStatus.iTerminalSpeed = TRUE) : (aStatus.iTerminalSpeed = FALSE);
	(iTerminalType->SendTerminalType() == TRUE) ?	(aStatus.iTerminalType = TRUE)	: (aStatus.iTerminalType = FALSE);
	(iStatus->ReceiveStatus() == TRUE) ?			(aStatus.iServerStatus = TRUE)	: (aStatus.iServerStatus = FALSE);
	(iStatus->SendStatus() == TRUE) ?				(aStatus.iClientStatus = TRUE)	: (aStatus.iClientStatus = FALSE);
	}

void CProto::ServerOptionStatus(TDes8& aActionBuffer)
	{
	if(iStatus->ReceiveStatus())
		{
		aActionBuffer.Append(KTelnetIAC);             // Interpret As Command
		aActionBuffer.Append(KTelnetSB);              // Suboption Start
		aActionBuffer.Append(KTelnetProtOptionStatus);// Status
		aActionBuffer.Append(KTelnetCommandSEND);     // Send 
		aActionBuffer.Append(KTelnetIAC);             // Interpret As Command
		aActionBuffer.Append(KTelnetSE);              // Suboption End    
		}
	}

void CProto::GetInitOptions(TDes8& aActionBuffer)
/**
Should be called following connection
Place here any calls to enable options at connection time
*/
	{
	TInt32 event;
	// Switch on binary if the terminal is not "dumb"
	if(iTerminalType->TerminalType() != _L8("dumb"))
		ClientRequestOption(KTelnetProtOptionBinary,aActionBuffer,event);					
	// Always suppress the Go Ahead signal
	ClientRequestOption(KTelnetProtOptionSuppressGA,aActionBuffer,event);					
	}


void CProto::ModifyConfig(const TTelnetConfig& aConfig,TDes8 * aActionBuffer)
/**
Modify the configurable options
aActionBuffer is set to NULL if the caller does not want to notify the server of option changes
*/
	{
	// If the speed has changed
	if(iSpeed->Set(aConfig.iTermSpeed))
		{
		// If speed option is not enabled and caller wants to notify server
		if(!iSpeed->SendSpeed() && aActionBuffer)
			{
			TInt32 event;
			// If caller wants to notify server
			// Switch the option on
			ClientRequestOption(KTelnetProtOptionTerminalSpeed,*aActionBuffer,event);
			}
		}
	// Terminal Type
	// We don't currently support lists of terminal types.
	// All telnet servers request the terminal type first thing so, just modify the terminal type
	// If the connection is up, then the client will have to close the connection
	iTerminalType->Set(aConfig.iTermType);
	// NAWS
	// If window size has changed
	if(iWindowSize->Set(aConfig.iWindowSize))
		{
		// If NAWS is not enabled and the user wants to notify the server
		if(!iWindowSize->SendWindowSize() && aActionBuffer)
			{
			TInt32 event;
			// Switch on NAWS
			ClientRequestOption(KTelnetProtOptionWindowSize,*aActionBuffer,event);
			}
		else
			// Window size is already enabled
			// If the caller wants to notify the server
			if(aActionBuffer)
				// return IAC SB NAWS IS X Y IAC SE
				iWindowSize->GetTelnetSubOption(*aActionBuffer);
		}

	// Enable/disable server logout is a passive set action
	// If the Client wants to force a Telnet defined logout then it should call DoForceLogout()
	if(aConfig.iAllowLogout)
		iLogout->SetRequestPermission
					(
					TOptionFSM::EServerWill | TOptionFSM::EClientDo
					);
	else
		iLogout->SetRequestPermission
					(
					TOptionFSM::ENoOption
					);
	// Echo
	// First set the permissions for the Server depending on the member boolean
	if(aConfig.iServerEcho)
		iEcho->SetRequestPermission(TOptionFSM::EServerWill | TOptionFSM::EClientDo);
	else
		iEcho->SetRequestPermission(TOptionFSM::ENoOption);

	// Echo is a "toggler"
	// If we are required to supply an action and the client wants to change the echo state
	if(aActionBuffer && iEcho->ReceiveEcho() != aConfig.iServerEcho)
		{
		TInt32 event;
		// Call the routine to create the request as the client is attempting to toggle
		// Server echo
		ClientRequestOption(KTelnetProtOptionEcho,*aActionBuffer,event);
		}
	}


TInt CProto::ClientRequestOption(TInt32 aOptionRequest,TDes8& aAction,TInt32& aEvent)
/**
Creates client side option requests
Currently we only switch on options
Ignore the errors as we can pre-check the state of the option before we call this.
*/
	{
	TInt32 err = KErrNone;
	TInt32 event;
	TBuf8<10> action;

	aEvent = TOptionFSM::ENoEvent;
	
	// If binary
	if(aOptionRequest == KTelnetProtOptionBinary)
		{
		// Request the server to transmit in Binary
		err = iBinary->RequestOption(TOptionFSM::EClientDo,action,event);
		aEvent |= event;
		aAction.Append(action);

		// Tell the server we are prepared to transmit in binary
		action.SetLength(0);
		err = iBinary->RequestOption(TOptionFSM::EClientWill,action,event);
		aEvent |= event;
		aAction.Append(action);
		}
	else if(aOptionRequest == KTelnetProtOptionSuppressGA)
		{
		// Request the server to suppress the go ahead signal
		err = iGA->RequestOption(TOptionFSM::EClientDo,action,event);
		aEvent |= event;
		aAction.Append(action);

		// Tell the server we are prepared to suppress the go ahead signal
		action.SetLength(0);
		err = iGA->RequestOption(TOptionFSM::EClientWill,action,event);
		aEvent |= event;
		aAction.Append(action);
		}
	else if(aOptionRequest == KTelnetProtOptionStatus)
		{
		// Tell the server we are prepared to send status information
		err = iStatus->RequestOption(TOptionFSM::EClientWill,action,event);
		aEvent = event;
		aAction.Append(action);
		}
	else if(aOptionRequest == KTelnetProtOptionLogoff)
		{
		// Client is forcing a logout
		iLogout->SetRequestPermission
					(
					TOptionFSM::EServerWill | TOptionFSM::EClientDo
					);
		err = iLogout->RequestOption(TOptionFSM::EClientDo,action,event);
		aEvent = event;
		aAction.Append(action);
		}
	else if(aOptionRequest == KTelnetProtOptionWindowSize)
		{
		// Tell the Server we are prepared to send window size information
		err = iWindowSize->RequestOption(TOptionFSM::EClientWill,action,event);
		aEvent = event;
		aAction.Append(action);
		}
	else if(aOptionRequest == KTelnetProtOptionTerminalType)
		{
		// Tell the server we are prepared to send terminal type information
		err = iTerminalType->RequestOption(TOptionFSM::EClientWill,action,event);
		aEvent = event;
		aAction.Append(action);
		}
	else if(aOptionRequest == KTelnetProtOptionTerminalSpeed)
		{
		// Tell the server we are prepared to send terminal speed information
		err = iSpeed->RequestOption(TOptionFSM::EClientWill,action,event);
		aEvent = event;
		aAction.Append(action);
		}
	else if(aOptionRequest == KTelnetProtOptionEcho)
		{
		// Echo is a special case where we toggle
		if(iEcho->ReceiveEcho())
			// Server Echo is currently enabled on so tell server DONT
			err = iEcho->RequestOption(TOptionFSM::EClientDont,action,event);
		else
			// Server Echo is currently disabled so tell server DO 
			err = iEcho->RequestOption(TOptionFSM::EClientDo,action,event);
		aEvent = event;
		aAction.Append(action);
		}


	return(err);
	}

TInt CProto::ProtoWrite(const TDesC8& aInBuffer,TDes8& aOutBuffer)
/**
Modifies the output stream :-
Escape EIAC in Binary mode.
PAD CR to CR NULL when not in binary mode
*/
	{
	if(iBinary->SendBinary())
		{
		for(TInt i=0;i<aInBuffer.Length();i++)
			{
			if(aInBuffer[i] == KTelnetIAC)
				aOutBuffer.Append(KTelnetIAC);
			aOutBuffer.Append(aInBuffer[i]);
			}
		}
	else
		{
		for(TInt i=0;i<aInBuffer.Length();i++)
			{
			aOutBuffer.Append(aInBuffer[i]);
			if(aInBuffer[i] == KTelnetCR && (i == (aInBuffer.Length() - 1) || aInBuffer[i+1] != KTelnetLF))
				{
				aOutBuffer.Append(KTelnetNULL);
				}
			}
		}
	return(KErrNone);
	}

TInt CProto::ProtoWrite(const TTelnetUserControl& aControlCode,TDes8& aBuffer,TDes8& aUrgentBuffer)
/**
Writes a Telnet 854 defined control code to the server
In the case of an Interrupt Process we need to send urgent data 
*/
	{
	TInt err;
	if(aControlCode == KTelnetIP)
		// Interrupt Process, supported in Binary and NVT mode
		{
		aUrgentBuffer.Append(KTelnetIAC); // Interpret As Command *** URGENT ***
		aUrgentBuffer.Append(KTelnetIP);  // Interrupt Process    *** URGENT ***
		aUrgentBuffer.Append(KTelnetIAC); // Interpret As Command *** URGENT ***
		aBuffer.Append(KTelnetDM);        // Data Mark
		err = KErrNone;
		}
	else
		{
		// All codes valid when not in binary
		if(!iBinary->SendBinary())
			{
			__FLOG_STATIC1(KTelnetLoggingCompnt(),KTelnetLoggingTag(),_L("CProto::ProtoWrite() Normal Send Control = %d"),aControlCode);
			aBuffer.Append(KTelnetIAC);           // Interpret As Command
			aBuffer.Append((TInt8)aControlCode);  // Code 
			err = KErrNone;
			}
		else if(aControlCode == KTelnetAYT || aControlCode == KTelnetAO)
			// In binary Are You There and Abort Output only
			{
			__FLOG_STATIC1(KTelnetLoggingCompnt(),KTelnetLoggingTag(),_L("CProto::ProtoWrite() Binary Send Control = %d"),aControlCode);
			aBuffer.Append(KTelnetIAC);          // Interpret As Command
			aBuffer.Append((TInt8)aControlCode); // Code
			err = KErrNone;
			}
		else
			{
			err = KErrGeneral;
			__FLOG_STATIC(KTelnetLoggingCompnt(),KTelnetLoggingTag(),_L("CProto::ProtoWrite() ERROR Illegal Code in Binary"));
			}
		}
	return(err);
	}


TInt CProto::ProtoRead(const TDesC8& aInBuffer,TDes8& aClientOutBuffer,TDes8& aProtoOutBuffer)
/**
Process data received from the Telnet Server
Contains receive state machine plus sub-state machine for suboptions
*/
	{
//	TBuf8<64> action;
	TBuf8<128> subOptions;
	TInt32 numEvents = 0;
	// Loop through the input buffer byte by byte
	for(TInt i=0;i<aInBuffer.Length();i++)
		{
		switch(iReceiveState)
			{
		case	ENormal	:
			// Check for Interpret as command
			if(aInBuffer[i] == KTelnetIAC)
				{
				// Reset the object protocol receive buffer
				iProtReadBuffer.SetLength(0);
				// Set the state
				iReceiveState = EIAC;
				__FLOG_STATIC(KTelnetLoggingCompnt(),KTelnetLoggingTag(),_L("CProto::ProtoRead() IAC"));
				}
			// Check for first character of a RFC 854 Sync
			else if(aInBuffer[i] == KTelnetIP)
				{
				__FLOG_STATIC(KTelnetLoggingCompnt(),KTelnetLoggingTag(),_L("CProto::ProtoRead() IP"));
				}
			// Check for Data Mark that follows Sync and ends urgent processing
			else if(aInBuffer[i] == KTelnetDM && iUrgentFlag)
				{
				iUrgentFlag = FALSE;
				__FLOG_STATIC(KTelnetLoggingCompnt(),KTelnetLoggingTag(),_L("CProto::ProtoRead()  DM"));
				}
			// Check we are not discarding during urgent receive waiting for DM
			else if(!iUrgentFlag)
				{
				// Pass to client
				// Normal data received, just write it to the output buffer
				aClientOutBuffer.Append(aInBuffer[i]);
				}
			else
				{
				// Discard
				__FLOG_STATIC(KTelnetLoggingCompnt(),KTelnetLoggingTag(),_L("CProto::ProtoRead() Discard During Urgent"));
				}
			break;

		case	EIAC	:
			// Last character received was an Interpret As Command
			// Check for DO WILL WONT DONT
			if(aInBuffer[i] == KTelnetDO || aInBuffer[i] == KTelnetWILL || aInBuffer[i] == KTelnetDONT || aInBuffer[i] == KTelnetWONT)
				{
				// Add it to the protocol buffer
				iProtReadBuffer.Append(aInBuffer[i]);
				iReceiveState = ECommand;
				}
			else if(aInBuffer[i] == KTelnetSB)
				{
				// Suboption Start
				iReceiveState = ESubOpt;
				iSubOptState = ESB;
				}
			else
				{
				// Interpret As Command followed by Interpret As Command
				if(aInBuffer[i] == KTelnetIAC)
					{
					// Previous Interpret As Command is Escaping a genuine 255 , legal in Binary Mode
					if(iBinary->ReceiveBinary() && !iUrgentFlag)
						{
						// Pass to client
						aClientOutBuffer.Append(aInBuffer[i]);
						}
					else
						{
						// Discard whilst in urgent or illegal in NVT mode
						__FLOG_STATIC1(KTelnetLoggingCompnt(),KTelnetLoggingTag(),_L("CProto::ProtoRead() Illegal IAC Urgent = %d"),iUrgentFlag);
						}
					}
				else
					{
					// Unsupported command
					__FLOG_STATIC1(KTelnetLoggingCompnt(),KTelnetLoggingTag(),_L("CProto::ProtoRead() Unsupported Command = %d"),aInBuffer[i]);
					}
				// Back to normal state
				iReceiveState = ENormal;
				}
			break;

		case	ESubOpt		:
			// Receiving a Suboption sequence
			if(iSubOptState == ESB)
				{
				// TERMINAL-TYPE,TERMINAL-SPEED,STATUS
				iProtReadBuffer.Append(aInBuffer[i]);
				iSubOptState = EOption;
				}
			else if(iSubOptState == EOption)
				{
				// SEND or IS
				iProtReadBuffer.Append(aInBuffer[i]);
				iSubOptState = ERequest;
				}
			else if(iSubOptState == ERequest)
				{
				// IAC or STATUS info from the server
				if(aInBuffer[i] == KTelnetIAC)
					{
					iSubOptState = EEndIAC;
					}
				iProtReadBuffer.Append(aInBuffer[i]);
				}
			else if(iSubOptState == EEndIAC && aInBuffer[i] == KTelnetSE)
				{
				// SE Suboption End
				// Scan through the sequence we have just received
				// Currently we support sending Window Size ,Terminal Speed and Status Information
				if(iProtReadBuffer[0] == KTelnetProtOptionTerminalType)
					{
					// Get our window size
					iTerminalType->GetTelnetSubOption(aProtoOutBuffer);
					__FLOG_STATIC(KTelnetLoggingCompnt(),KTelnetLoggingTag(),_L("CProto::ProtoRead() SE Term Type"));
					}
				else if(iProtReadBuffer[0] == KTelnetProtOptionTerminalSpeed)
					{
					// Get our speed
					iSpeed->GetTelnetSubOption(aProtoOutBuffer);
					__FLOG_STATIC(KTelnetLoggingCompnt(),KTelnetLoggingTag(),_L("CProto::ProtoRead() SE Term Speed"));
					}
				else if(iProtReadBuffer[0] == KTelnetProtOptionStatus)
					{
					// Check whether it's a request for us to send or it's status info from the server
					if(iProtReadBuffer[1] == KTelnetCommandSEND)
						{
						// Server wants our status
						GetOptionStatus(aProtoOutBuffer);
						__FLOG_STATIC(KTelnetLoggingCompnt(),KTelnetLoggingTag(),_L("CProto::ProtoRead() SE STATUS SEND"));
						}
					else if(iProtReadBuffer[1] == KTelnetCommandIS)
						{
						// Status info from the server
						// We are currently doing nothing with it but in case
						// the client wants to know the server's perceived state of the connection
						// store it in the object
						iServerStatus = iProtReadBuffer;
						__FLOG_STATIC(KTelnetLoggingCompnt(),KTelnetLoggingTag(),_L("CProto::ProtoRead() SE STATUS IS"));
						}
					else
						{
						// Corruption
						__FLOG_STATIC(KTelnetLoggingCompnt(),KTelnetLoggingTag(),_L("CProto::ProtoRead() STATUS SubOption ERROR"));
						}
					}
					iReceiveState = ENormal;
				}
			else
				{
				iReceiveState = ENormal;
				__FLOG_STATIC(KTelnetLoggingCompnt(),KTelnetLoggingTag(),_L("CProto::ProtoRead() SubOption Unexpected Receive ERROR"));
				}

			break;

		case	ECommand	:
			// Last byte received was a DO, WILL, WONT or DONT
			{
			TInt32 event = TOptionFSM::ENoEvent;
			TOptionFSM::TRequests	request = TOptionFSM::ENoOption;
			TBuf8<64> action;

			// Map to our internal command code
			if(iProtReadBuffer[0] == KTelnetDO)
				request = TOptionFSM::EServerDo;
			else if(iProtReadBuffer[0] == KTelnetWILL)
				request = TOptionFSM::EServerWill;
			else if(iProtReadBuffer[0] == KTelnetWONT)
				request = TOptionFSM::EServerWont;
			else if(iProtReadBuffer[0] == KTelnetDONT)
				request = TOptionFSM::EServerDont;

			// switch on the RFC option
			// Most cases get a possible action and a possible event
			switch(aInBuffer[i])
				{
			case	KTelnetProtOptionEcho			:
				iEcho->RequestOption(request,action,event);
				if(action.Length())
					aProtoOutBuffer.Append(action);
				__FLOG_STATIC2(KTelnetLoggingCompnt(),KTelnetLoggingTag(),_L("CProto::ProtoRead() Echo Event = %d Request = %d"),event,request);
				if(event != TOptionFSM::ENoEvent)
					numEvents++;
				break;

			case	KTelnetProtOptionSuppressGA		:
				iGA->RequestOption(request,action,event);
				if(action.Length())
					aProtoOutBuffer.Append(action);
				__FLOG_STATIC1(KTelnetLoggingCompnt(),KTelnetLoggingTag(),_L("CProto::ProtoRead() GA Event = %d"),event);
				break;

			case	KTelnetProtOptionBinary			:
				iBinary->RequestOption(request,action,event);
				if(action.Length())
					aProtoOutBuffer.Append(action);
				__FLOG_STATIC2(KTelnetLoggingCompnt(),KTelnetLoggingTag(),_L("CProto::ProtoRead() Binary Event = %d request = %d"),event,request);
				if(event != TOptionFSM::ENoEvent)
					numEvents++;
				break;

			case	KTelnetProtOptionTerminalSpeed	:
				iSpeed->RequestOption(request,action,event);
				if(action.Length())
					aProtoOutBuffer.Append(action);
				__FLOG_STATIC1(KTelnetLoggingCompnt(),KTelnetLoggingTag(),_L("CProto::ProtoRead() Terminal Speed Event = %d"),event);
				if(event != TOptionFSM::ENoEvent)
					numEvents++;
				break;

			case	KTelnetProtOptionTerminalType	:
				iTerminalType->RequestOption(request,action,event);
				if(action.Length())
					aProtoOutBuffer.Append(action);
				__FLOG_STATIC1(KTelnetLoggingCompnt(),KTelnetLoggingTag(),_L("CProto::ProtoRead() Terminal Type Event = %d"),event);
				if(event != TOptionFSM::ENoEvent)
					numEvents++;
				break;

			case	KTelnetProtOptionWindowSize		:
				// Winbdow size, send our window size if it's enabled
				iWindowSize->RequestOption(request,action,event);
				if(action.Length())
					aProtoOutBuffer.Append(action);
				if(event == TOptionFSM::EClientEnabled)
					{
					iWindowSize->GetTelnetSubOption(subOptions);
					}
				__FLOG_STATIC1(KTelnetLoggingCompnt(),KTelnetLoggingTag(),_L("CProto::ProtoRead() Window Size Event = %d"),event);
				if(event != TOptionFSM::ENoEvent)
					numEvents++;
				break;

			case	KTelnetProtOptionLogoff			:
				// Server may want to log us out
				iLogout->RequestOption(request,action,event);
				if(action.Length())
					aProtoOutBuffer.Append(action);
				__FLOG_STATIC1(KTelnetLoggingCompnt(),KTelnetLoggingTag(),_L("CProto::ProtoRead() Logout Event = %d"),event);
				break;

			case	KTelnetProtOptionStatus			:
				iStatus->RequestOption(request,action,event);
				if(action.Length())
					aProtoOutBuffer.Append(action);
				__FLOG_STATIC2(KTelnetLoggingCompnt(),KTelnetLoggingTag(),_L("CProto::ProtoRead() Status Event = %d request = %d"),event,request);
				if(event != TOptionFSM::ENoEvent)
					numEvents++;
				break;

			default									:
				iUnknown->RequestUnknown(aInBuffer[i],request,action,event);
				if(action.Length())
					aProtoOutBuffer.Append(action);
				__FLOG_STATIC1(KTelnetLoggingCompnt(),KTelnetLoggingTag(),_L("CProto::ProtoRead() Unsupported Option = %d"),aInBuffer[i]);
				break;

				}
			}
			iReceiveState = ENormal;
			break;

		default			:
			break;
			}
		}
	// If any option events have occured then tell the client side so it can retrieve the status
	// if it's interested
	if(numEvents)
		iNotifier->ProtoEvent();

	if(subOptions.Length())
		{
		aProtoOutBuffer.Append(subOptions);
		__FLOG_STATIC1(KTelnetLoggingCompnt(),KTelnetLoggingTag(),_L("CProto::ProtoRead() Add SubOpt Length = %d"),subOptions.Length());
		}
	return(KErrNone);
	}

void CProto::Reset()
/**
Clear states
Don't bother to zero buffers as state machine does them
*/
	{
	iReceiveState =			ENormal;
	iUrgentFlag =			FALSE;

	iBinary->Reset();
	iLogout->Reset();
	iWindowSize->Reset();
	iSpeed->Reset();
	iStatus->Reset();
	iTerminalType->Reset();
	iEcho->Reset();
	iGA->Reset();
	}


CProto::CProto()
/**
Constructor
*/
	{
	}


CProto::~CProto()
/**
Destructor
*/
	{
	__FLOG_STATIC(KTelnetLoggingCompnt(),KTelnetLoggingTag(),_L("CProto::D'Tor"));

	delete iUnknown;
	delete iStatus;
	delete iGA;
	delete iSpeed;
	delete iLogout;
	delete iTerminalType;
	delete iWindowSize;
	delete iEcho;
	delete iBinary;
	}

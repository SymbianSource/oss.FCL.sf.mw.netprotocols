// Copyright (c) 1998-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// TTelnet.cpp
// Tiny Telnet - A simple telnet client to test CTelnetSession API
// 
//

#include <e32cons.h>
#include <in_sock.h>
#include <es_sock.h>
#include <nifman.h>
#include <e32test.h>
#include <TELSESS.H>
#include "TUIEDIT.H"
//#include <debug.h>

// ActiveConsole Declaration

class CActiveConsole : public CActive
/**
@internalComponent
*/
	{
public:
	~CActiveConsole();
	void RequestCharacter();
	void RunL();
	virtual void ProcessKeyPress(TChar aChar) = 0;
	void DoCancel();

protected:
	void ConstructL(CConsoleBase* aConsole);
	inline CActiveConsole(TInt aPriority) : CActive(aPriority){};

protected:
	CConsoleBase* iConsole;
	};

class CTinyTelnet : public CActiveConsole, public MTelnetNotification 
/**
@internalComponent
*/
	{
enum TState 
	{
	EConnecting,EConnected, EDisconnecting,EDisconnected,EComplete
	};

public:
	CTinyTelnet();
	~CTinyTelnet();

	static CTinyTelnet* NewLC(CConsoleBase* aConsole);
	void ConstructL(CConsoleBase* aConsole);

	void DisplayOptionStates();

	void Error(TInt aError);
	void ReadComplete(const TDesC8& aBuffer);
	void WriteComplete();
	void ConnectionClosed();
	void OptionsChanged();
	void Connected();

	void ProcessKeyPress(TChar aChar);
	void DoCancel();


	TBool DoCommand();
	void Read();
	TInt Write(TChar aChar);

private:
	void Open(TDesC& aCommandArg);
	void OpenIP(TDesC& aCommandArg);
	void Close(TDesC& aCommandArg);
	void Help(TDesC& aCommandArg);
	void LocalEcho(TDesC& aCommandArg);
	void Binary(TDesC& aCommandArg);
	void Logout(TDesC& aCommandArg);

private:
	TTelnetConfig iConfig;
	CTelnetSession* iTelnetSession;
	TInetAddr iAddress;
	TBuf<30> iBuffer;
	CLineEdit* iCommandLine; //[remove]
	TBuf<80> iCommand;
	TBuf<80> iCommandArg;
	TState iState;
	TBool iLocalEcho;
	TBool iBinaryOnOff;
};




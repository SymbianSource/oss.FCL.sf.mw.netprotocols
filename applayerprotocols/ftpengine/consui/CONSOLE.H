/**
* Copyright (c) 1998-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Author:	Philippe Gabriel
* Console active object
* Buffers input from the keyboard and reports a command line
* terminated by a return key
* 
*
*/



/**
 @file Console.h
 @internalComponent
*/

#if !defined(__CONSOLE_H__)
#define __CONSOLE_H__
#include <e32base.h>
#include <e32test.h>

//////////////////////////////////////////////////////////////
// Definitions
//////////////////////////////////////////////////////////////

/**
The set of chars we accept as an input
@internalComponent
*/
// character '£' is recognized as invalid multibyte character sequence on Linux build => replaced with '\xA3'
_LIT(KAlphabet," abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789<>\\\",./?:@~;'#!\xA3$%^&*()_+-=");
class MCmdConsoleReport
/**
@internalComponent
*/
{
public:
virtual void CmdReady(void)=0;
virtual void Escape(void)=0;
};
class CmdConsole : public CActive
/**
@internalComponent
*/
{
public:
	CmdConsole(MCmdConsoleReport*,CConsoleBase*);
	static CmdConsole* NewL(MCmdConsoleReport*,CConsoleBase*);
	~CmdConsole(void);
	void Start(void);
	void Reset(void);
	TDes& FetchCmd(void);
	
protected:
	void RunL();
    void DoCancel(void);
private:
	void ConstructL(void);
	MCmdConsoleReport* iNotifier;
	CConsoleBase* iConsole;
	TBuf<512> iCmdBuffer;
	TBool	iAcceptInput;
};
#endif

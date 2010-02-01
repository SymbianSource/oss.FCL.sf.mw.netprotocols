// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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

// System includes
#include <httpstringconstants.h>
#include <wsperror.h>

// Local includes
#include "cwspfiledatasupplier.h"
#include "t_wspeventdriver/cwspeventdriver.h"
#include "t_wspeventdriver/tdriverpanic.h"
#include "testconsole/ccommandlineprocessor.h"
#include "testconsole/ccommandline.h"

// Class signature
//
#include "cwspcmdlineprocessor.h"


// Constants used in this file
//
_LIT(KWspThStubIniFile,			"stub_tests.ini");
_LIT(KMsgUnknownCmd,			"\nError - unknown command. Try 'help'");
_LIT(KHelp,						"help");
_LIT(KScriptCmd,				"script");
_LIT(KConnectCmd,				"connect");
_LIT(KDisconnectCmd,			"disconnect");
_LIT(KSetCmd,					"set");
_LIT(KClientMessageSizeCmd,		"climsgsize");
_LIT(KServerMessageSizeCmd,		"svrmsgsize");
_LIT(KUseAcknowledgementsCmd,	"useacks");
_LIT(KUseSuspendResumeCmd,		"usesuspres");
_LIT(KMethodMORCmd,				"methodmor");
_LIT(KAddSessHeaderCmd,			"addsesshdr");
_LIT(KAddSessHeaderWithParamCmd,"addsesshdrparam");
_LIT(KDumpSessionHeadersCmd,	"dumpsesshdrs");

_LIT(KConnectionPropertiesCmd,	"connprops");
_LIT(KProxyAddress,				"proxyaddr");
_LIT(KBearer,					"bearer");
_LIT(KIp,						"ip");
_LIT(KSms,						"sms");
_LIT(KLocalPort,				"locport");
_LIT(KRemotePort,				"remport");
_LIT(KServiceNum,				"svcnum");
_LIT(KSessionType,				"sesstype");
_LIT(KConnOriented,				"co");
_LIT(KConnLess,					"cl");
_LIT(KSecurity,					"security");
_LIT(KUseWtls,					"on");
_LIT(KDoNotUseWtls,				"off");
_LIT(KTimeOut,					"timeout");

_LIT(KOpenTransactionCmd,		"opentrans");
_LIT(KAddHeaderCmd,				"addhdr");
_LIT(KAddHeaderWithParamCmd,	"addhdrparam");
_LIT(KAddBodyCmd,				"addbody");
_LIT(KSubmitCmd,				"submit");
_LIT(KCancelCmd,				"cancel");
_LIT(KCloseCmd,					"close");
_LIT(KDumpResponseHeadersCmd,	"dumpresphdrs");
_LIT(KDumpResponseBodyCmd,		"dumprespbody");
_LIT(KInt,						"int");
_LIT(KDate,						"date");
_LIT(KStr,						"str");
_LIT(KStrF,						"strf");

_LIT(KMsgServiceNumberNotDefined, "KWspErrAccessPointNotSetUpForCSD");
_LIT(KMsgAccessPointNotSetUp, "KWspErrAccessPointNotSetUp");
_LIT(KMsgBadRedirectedAddressData, "KWspErrBadRedirectedAddressData");
_LIT(KMsgBearerNotSupported, "KWspErrBearerNotSupported");
_LIT(KMsgRequestTooBig, "KWspErrRequestTooBig");
_LIT(KMsgStreamedRequestDataNotSupported, "KWspErrStreamedRequestDataNotSupported");
_LIT(KMsgUnsupportedSendPrimitive, "KWspErrUnsupportedSendPrimitive");
_LIT(KMsgSessionClosingOrSuspending, "KWspErrSessionClosingOrSuspending");
_LIT(KMsgExpectingBodyData, "KWspErrExpectingBodyData");
_LIT(KMsgTrailerHeadersNotExpected, "KWspErrTrailerHeadersNotExpected");
_LIT(KMsgMissingHeader, "KWspErrMissingHeader");
_LIT(KMsgSMSBearerNotInCommsDB, "KWspErrSMSBearerNotInCommsDB");
_LIT(KMsgCouldNotCreateTransportHandler, "KWspErrCouldNotCreateTransportHandler");

/*
** Implementation of class CWspCmdLineProcessor
*/


CWspCmdLineProcessor* CWspCmdLineProcessor::NewL(CConsoleBase& aConsole,
												 const TDesC& aInitialScript)
	{
	CWspCmdLineProcessor* self = new (ELeave) CWspCmdLineProcessor(aConsole);
	CleanupStack::PushL(self);
	self->ConstructL(aInitialScript);
	CleanupStack::Pop(self);
	return self;
	}

CWspCmdLineProcessor::~CWspCmdLineProcessor()
	{
	delete iCmdLineProcessor;
	delete iEventDriver;
	}

CWspCmdLineProcessor::CWspCmdLineProcessor(CConsoleBase& aConsole)
: CEventCallbackFilter(), iConsole(aConsole)
	{
	}

void CWspCmdLineProcessor::ConstructL(const TDesC& aInitialScript)
	{
	iCmdLineProcessor = CCommandLineProcessor::NewL(iConsole, *this);
	iEventDriver = CWspEventDriver::NewL(*this, KWspThStubIniFile());
	iStrP = iEventDriver->StringPool();

	// If there's initial script, then read commands from it to begin with
	if (aInitialScript.Length() > 0)
		iCmdLineProcessor->StartScriptL(aInitialScript);
	}

void CWspCmdLineProcessor::Start()
	{
	iCmdLineProcessor->GetNextCommand();
	}

void CWspCmdLineProcessor::CommandDone()
	{
	iCmdLineProcessor->GetNextCommand();
	}

void CWspCmdLineProcessor::HandleCommandLine(CCommandLine* aCmdLine)
	{
	if( aCmdLine->Count() != 0 )
		{
		TPtrC command = *aCmdLine->At(0);
		TInt err = KErrNone;
		if (command.CompareF(KHelp) == 0)
			{
			// Display help...
			TRAP(err, Do_HelpL(aCmdLine));
			}
		else if (command.CompareF(KScriptCmd) == 0)
			{
			// Execute command script...
			TRAP(err, Do_ScriptL(aCmdLine));
			}
		else if (command.CompareF(KConnectCmd) == 0)
			{
			// Connect...
			TRAP(err, Do_SessionConnectL(aCmdLine));
			}
		else if (command.CompareF(KDisconnectCmd) == 0)
			{
			// Disconnect...
			TRAP(err, Do_SessionDisconnectL(aCmdLine));
			}
		else if (command.CompareF(KSetCmd) == 0)
			{
			// Set capabilities and connection properties...
			TRAP(err, Do_SetL(aCmdLine));
			}
		else if (command.CompareF(KAddSessHeaderCmd) == 0)
			{
			// Add session headers...
			TRAP(err, Do_AddSessHeaderL(aCmdLine));
			}
		else if (command.CompareF(KAddSessHeaderWithParamCmd) == 0)
			{
			// Add session headers with parameters...
			TRAP(err, Do_AddSessHeaderWithParamL(aCmdLine));
			}
		else if (command.CompareF(KDumpSessionHeadersCmd) == 0)
			{
			// Add session headers with parameters...
			TRAP(err, Do_DumpSessionHeadersL(aCmdLine));
			}
		else if (command.CompareF(KOpenTransactionCmd) == 0)
			{
			// Open a transaction...
			TRAP(err, Do_OpenTransactionL(aCmdLine));
			}
		else if (command.CompareF(KAddHeaderCmd) == 0)
			{
			// Add a header field to a transaction...
			TRAP(err, Do_AddHeaderL(aCmdLine));
			}
		else if (command.CompareF(KAddHeaderWithParamCmd) == 0)
			{
			// Add a header field with a parameter to a tranasaction...
			TRAP(err, Do_AddHeaderWithParamL(aCmdLine));
			}
		else if (command.CompareF(KAddBodyCmd) == 0)
			{
			// Add a body to a transaction...
			TRAP(err, Do_AddBodyL(aCmdLine));
			}
		else if (command.CompareF(KSubmitCmd) == 0)
			{
			// Submit a transaction...
			TRAP(err, Do_SubmitL(aCmdLine));
			}
		else if (command.CompareF(KCancelCmd) == 0)
			{
			// Cancel a transaction...
			TRAP(err, Do_CancelL(aCmdLine));
			}
		else if (command.CompareF(KCloseCmd) == 0)
			{
			// Close a transaction...
			TRAP(err, Do_CloseL(aCmdLine));
			}
		else if (command.CompareF(KDumpResponseHeadersCmd) == 0)
			{
			// Dump the response headers from a transaction...
			TRAP(err, Do_DumpRespHdrsL(aCmdLine));
			}
		else if (command.CompareF(KDumpResponseBodyCmd) == 0)
			{
			// Dump the response body from a transaction...
			TRAP(err, Do_DumpRespBodyL(aCmdLine));
			}
		else
			{
			// Command not recognised
			iConsole.Write(KMsgUnknownCmd);
			}

		// Display a message if any command left during execution
		if (err != KErrNone)
			{
			_LIT(KCommandFailedMsg, "\n%S: command failed with error %d");
			iConsole.Printf(KCommandFailedMsg, &command, err);
			}
		}

	// Get next command...
	CommandDone();
	}

void CWspCmdLineProcessor::StartDump()
	{
	if (!iIsDumping)
		{
		iIsDumping = ETrue;
		iDumpLineCount = 0;
		iConsole.Printf(_L("\n"));
		}
	}

/** 
*/
void CWspCmdLineProcessor::WriteData(const TDesC16& aData)
	{
	if (iIsDumping)
		{
		TSize scrSize = iConsole.ScreenSize();
		TInt lineCount = 0;
		for (TInt ii = 0; ii < aData.Length(); ii += scrSize.iWidth)
			{
			TInt range = ((aData.Length() - ii) > scrSize.iWidth)?
							scrSize.iWidth:
							(aData.Length() - ii);
			const TDesC& text = aData.Mid(ii, range);
			iConsole.Printf(_L("%S"), &text);
			++lineCount;
			if (lineCount % (scrSize.iHeight - 5) == 0)
				{
				iConsole.Printf(_L("Press a key..."));
				(void)iConsole.Getch();
				TPoint pos = iConsole.CursorPos();
				iConsole.SetCursorPosAbs(TPoint(0, pos.iY));
				iConsole.ClearToEndOfLine();
				}
			}
		}
	}

/** 
*/
void CWspCmdLineProcessor::WriteData(const TDesC8& aData)
	{
	TBuf<KMaxFileName> tempBuffer;
	if (iIsDumping)
		{
		TSize scrSize = iConsole.ScreenSize();
		TInt lineCount = 0;
		for (TInt ii = 0; ii < aData.Length(); ii += scrSize.iWidth)
			{
			TInt range = ((aData.Length() - ii) > scrSize.iWidth)?
							scrSize.iWidth:
							(aData.Length() - ii);
			tempBuffer.Copy(aData.Mid(ii, range));
			iConsole.Printf(_L("%S"), &tempBuffer);
			++lineCount;
			if (lineCount % (scrSize.iHeight - 5) == 0)
				{
				iConsole.Printf(_L("Press a key..."));
				(void)iConsole.Getch();
				TPoint pos = iConsole.CursorPos();
				iConsole.SetCursorPosAbs(TPoint(0, pos.iY));
				iConsole.ClearToEndOfLine();
				}
			}
		}
	}

/** 
*/
void CWspCmdLineProcessor::DumpData(const TDesC8& aData)
	{
	if (iIsDumping)
		{
		TSize scrSize = iConsole.ScreenSize();
		TInt lineCount = 0;

		// Iterate the supplied block of data in blocks of cols=80 bytes
		const TInt cols=16;
		TInt pos = 0;
		TBuf<KMaxFileName> dumpLine;
		TBuf<KMaxFileName> anEntry;
		while (pos < aData.Length())
			{
			//start-line exadecimal( a 4 digit number)
			anEntry.Format(_L("%04x : "), pos);
			dumpLine.Append(anEntry);

			// Hex output
			TInt offset;
			for (offset = 0; offset < cols; offset++)
				{
				if (pos + offset < aData.Length())
					{
					TInt nextByte = aData[pos + offset];
					anEntry.Format(_L("%02x "), nextByte);
					dumpLine.Append(anEntry);
					}
				else
					{
					//fill the remaining spaces with blanks untill the cols-th Hex number 
					anEntry.Format(_L("   "));
					dumpLine.Append(anEntry);
					}
				}
				anEntry.Format(_L(": "));
				dumpLine.Append(anEntry);

			// Char output
			for (offset = 0; offset < cols; offset++)
				{
				if (pos + offset < aData.Length())
					{
					TInt nextByte = aData[pos + offset];
					if ((nextByte >= 32) && (nextByte <= 127))
						{
						anEntry.Format(_L("%c"), nextByte);
						dumpLine.Append(anEntry);
						}
					else
						{
						anEntry.Format(_L("."));
						dumpLine.Append(anEntry);
						}
					}
				else
					{
					anEntry.Format(_L(" "));
					dumpLine.Append(anEntry);
					}
				}
			iConsole.Printf(_L("%S\n"), &dumpLine);	
			dumpLine.Zero();
			++lineCount;
			if (lineCount % (scrSize.iHeight - 5) == 0)
				{
				iConsole.Printf(_L("Press a key..."));
				(void)iConsole.Getch();
				TPoint pos = iConsole.CursorPos();
				iConsole.SetCursorPosAbs(TPoint(0, pos.iY));
				iConsole.ClearToEndOfLine();
				}

			// Advance to next  byte segment (1 seg= cols)
			pos += cols;
			}
		}
	}

/** 
*/
void CWspCmdLineProcessor::EndDump()
	{
	iIsDumping = EFalse;
	}

void CWspCmdLineProcessor::Do_HelpL(CCommandLine* /*aCmdLine*/)
	{
	// Maybe add some command-specific help another time...
	_LIT(KHelpMsg1, "\nSupported commands:\n");
	_LIT(KHelpMsg2, "  connect,  disconnect,  set climsgsize,  set svrmsgsize,\n");
	_LIT(KHelpMsg3, "  set useacks,  set usesuspres,  set methodmor,\n");
	_LIT(KHelpMsg4, "  set connprops,  addsesshdr, addsesshdrparam, dumpsesshdrs,\n");
	_LIT(KHelpMsg5, "  opentrans, submit, cancel, close, dumpresphdrs, dumprespbody,\n");
	_LIT(KHelpMsg6, "  help, script, quit");
	iConsole.Printf(KHelpMsg1);
	iConsole.Printf(KHelpMsg2);
	iConsole.Printf(KHelpMsg3);
	iConsole.Printf(KHelpMsg4);
	iConsole.Printf(KHelpMsg5);
	iConsole.Printf(KHelpMsg6);
	}

void CWspCmdLineProcessor::Do_ScriptL(CCommandLine* aCmdLine)
	{
	// Check syntax
	if (aCmdLine->Count() != 2)
		{
		_LIT(KCommandFailedMsg, "\nSyntax: script <filename>");
		iConsole.Printf(KCommandFailedMsg);
		return;
		}

	// Get the command processor to read from the script file
	TPtrC fileName = *aCmdLine->At(1);
	TRAPD(err, iCmdLineProcessor->StartScriptL(fileName));
	if (err != KErrNone)
		{
		_LIT(KCommandFailedMsg, "\nError: Script file '%S' cannot be read!");
		iConsole.Printf(KCommandFailedMsg, aCmdLine->At(1));
		return;
		}

	}

void CWspCmdLineProcessor::Do_SessionConnectL(CCommandLine* aCmdLine)
	{
	// Check syntax
	if (aCmdLine->Count() != 1)
		{
		_LIT(KCommandFailedMsg, "\nSyntax: connect <no args>");
		iConsole.Printf(KCommandFailedMsg);
		return;
		}

	iEventDriver->SessionConnectL();
	}

void CWspCmdLineProcessor::Do_SessionDisconnectL(CCommandLine* aCmdLine)
	{
	// Check syntax
	if (aCmdLine->Count() != 1)
		{
		_LIT(KCommandFailedMsg, "\nSyntax: disconnect <no args>");
		iConsole.Printf(KCommandFailedMsg);
		return;
		}

	iEventDriver->SessionDisconnectL();
	}

void CWspCmdLineProcessor::Do_SetL(CCommandLine* aCmdLine)
	{
	// Check syntax
	if (aCmdLine->Count() == 1)
		{
		_LIT(KCommandFailedMsg, "\nSyntax: set <option> (<option value>)");
		iConsole.Printf(KCommandFailedMsg);
		return;
		}

	// Find the specific thing being set
	TPtrC command = *aCmdLine->At(1);
	if (command.CompareF(KClientMessageSizeCmd) == 0)
		{
		// setclientmessagesize
		Do_SetClientMessageSizeL(aCmdLine);
		}
	else if (command.CompareF(KServerMessageSizeCmd) == 0)
		{
		// setservermessagesize
		Do_SetServerMessageSizeL(aCmdLine);
		}
	else if (command.CompareF(KUseAcknowledgementsCmd) == 0)
		{
		// setuseacknowledgements
		Do_SetUseAcknowledgementsL(aCmdLine);
		}
	else if (command.CompareF(KUseSuspendResumeCmd) == 0)
		{
		// setusesuspendresume
		Do_SetUseSuspendResumeL(aCmdLine);
		}
	else if (command.CompareF(KMethodMORCmd) == 0)
		{
		// setmethodmor
		Do_SetMethodMORL(aCmdLine);
		}
	else if (command.CompareF(KConnectionPropertiesCmd) == 0)
		{
		// setconnectionproperties
		Do_SetConnectionPropertiesL(aCmdLine);
		}
	else
		{
		// Command not recognised
		iConsole.Write(KMsgUnknownCmd);
		}
	}


void CWspCmdLineProcessor::Do_SetClientMessageSizeL(CCommandLine* aCmdLine)
	{
	// Check syntax
	if (aCmdLine->Count() != 3)
		{
		_LIT(KCommandFailedMsg, "\nSyntax: set climsgsize <unsigned integer>");
		iConsole.Printf(KCommandFailedMsg);
		return;
		}

	// Get the argument and convert to an integer
	TPtrC command = *aCmdLine->At(2);
	TInt size = 0;
	TLex lexer(command);
	if ((lexer.Val(size) != KErrNone) || (size <= 0))
		{
		_LIT(KCommandFailedMsg, "\nset climsgsize: invalid integer value supplied");
		iConsole.Printf(KCommandFailedMsg);
		return;
		}

	// Execute the command
	iEventDriver->SetClientMessageSizeL(size);
	}

void CWspCmdLineProcessor::Do_SetServerMessageSizeL(CCommandLine* aCmdLine)
	{
	// Check syntax
	if (aCmdLine->Count() != 3)
		{
		_LIT(KCommandFailedMsg, "\nSyntax: set svrmsgsize <unsigned integer>");
		iConsole.Printf(KCommandFailedMsg);
		return;
		}

	// Get the argument and convert to an integer
	TPtrC command = *aCmdLine->At(2);
	TInt size = 0;
	TLex lexer(command);
	if ((lexer.Val(size) != KErrNone) || (size <= 0))
		{
		_LIT(KCommandFailedMsg, "\nset svrmsgsize: invalid integer value supplied");
		iConsole.Printf(KCommandFailedMsg);
		return;
		}

	// Execute the command
	iEventDriver->SetServerMessageSizeL(size);
	}

void CWspCmdLineProcessor::Do_SetUseAcknowledgementsL(CCommandLine* aCmdLine)
	{
	// Check syntax
	if (aCmdLine->Count() != 2)
		{
		_LIT(KCommandFailedMsg, "\nSyntax: set useacks <no args>");
		iConsole.Printf(KCommandFailedMsg);
		return;
		}

	iEventDriver->SetUseAcknowledgementsL();
	}

void CWspCmdLineProcessor::Do_SetUseSuspendResumeL(CCommandLine* aCmdLine)
	{
	// Check syntax
	if (aCmdLine->Count() != 2)
		{
		_LIT(KCommandFailedMsg, "\nSyntax: set usesuspres <no args>");
		iConsole.Printf(KCommandFailedMsg);
		return;
		}

	iEventDriver->SetUseSuspendResumeL();
	}

void CWspCmdLineProcessor::Do_SetMethodMORL(CCommandLine* aCmdLine)
	{
	// Check syntax
	if (aCmdLine->Count() != 3)
		{
		_LIT(KCommandFailedMsg, "\nSyntax: set methodmor <unsigned integer>");
		iConsole.Printf(KCommandFailedMsg);
		return;
		}

	// Get the argument and convert to an integer
	TPtrC command = *aCmdLine->At(2);
	TInt mor = 0;
	TLex lexer(command);
	if ((lexer.Val(mor) != KErrNone) || (mor <= 0))
		{
		_LIT(KCommandFailedMsg, "\nsetmethodmor: invalid integer value supplied");
		iConsole.Printf(KCommandFailedMsg);
		return;
		}

	// Execute the command
	iEventDriver->SetMethodMORL(mor);
	}

void CWspCmdLineProcessor::Do_SetConnectionPropertiesL(CCommandLine* aCmdLine)
	{
	// Check syntax
	if (aCmdLine->Count() != 4)
		{
		_LIT(KCommandFailedMsg, "\nSyntax: set connprops (proxyaddr|bearer|locport|remport|svcnum|sesstype|security|timeout) <value>");
		iConsole.Printf(KCommandFailedMsg);
		return;
		}

	// Find the specific connection property being set
	TPtrC command = *aCmdLine->At(2);
	if (command.CompareF(KProxyAddress) == 0)
		{
		// setclientmessagesize
		Do_SetProxyAddressL(aCmdLine);
		}
	else if (command.CompareF(KBearer) == 0)
		{
		// setservermessagesize
		Do_SetBearerL(aCmdLine);
		}
	else if (command.CompareF(KLocalPort) == 0)
		{
		// setuseacknowledgements
		Do_SetLocalPortL(aCmdLine);
		}
	else if (command.CompareF(KRemotePort) == 0)
		{
		// setusesuspendresume
		Do_SetRemotePortL(aCmdLine);
		}
	else if (command.CompareF(KServiceNum) == 0)
		{
		// setmethodmor
		Do_SetServiceNumL(aCmdLine);
		}
	else if (command.CompareF(KSessionType) == 0)
		{
		// setconnectionproperties
		Do_SetSessionTypeL(aCmdLine);
		}
	else if (command.CompareF(KSecurity) == 0)
		{
		// setconnectionproperties
		Do_SetSecurityL(aCmdLine);
		}
	else if (command.CompareF(KTimeOut) == 0)
		{
		// setconnectionproperties
		Do_SetTimeOutL(aCmdLine);
		}
	else
		{
		// Command not recognised
		iConsole.Write(KMsgUnknownCmd);
		}
	}

void CWspCmdLineProcessor::Do_SetProxyAddressL(CCommandLine* aCmdLine)
	{
	// Get the address argument and convert to narrow then to RStringF
	TPtrC arg = *aCmdLine->At(3);
	HBufC8* addrArg8 = HBufC8::NewL(arg.Length());
	CleanupStack::PushL(addrArg8);
	addrArg8->Des().Copy(arg);

	RStringF addr = iStrP.OpenFStringL(*addrArg8);
	CleanupClosePushL(addr);

	// Execute the command
	iEventDriver->SetConnectionPropertiesL(addr, RStringF(), -1, -1, -1, RStringF(), RStringF(), -1);
	CleanupStack::PopAndDestroy(2, addrArg8);
	}

void CWspCmdLineProcessor::Do_SetBearerL(CCommandLine* aCmdLine)
	{
	// Get the address argument and convert to narrow then to RStringF
	TPtrC arg = *aCmdLine->At(3);
	RStringF bearer;
	if (arg.CompareF(KIp) == 0)
		bearer = iStrP.StringF(HTTP::EWspIp, RHTTPSession::GetTable());
	else if (arg.CompareF(KSms) == 0)
		bearer = iStrP.StringF(HTTP::EWspSMS, RHTTPSession::GetTable());
	else
		{
		_LIT(KCommandFailedMsg, "\nset connprops bearer: invalid bearer value supplied (must be 'ip' or 'sms')");
		iConsole.Printf(KCommandFailedMsg);
		return;
		}

	// Execute the command
	iEventDriver->SetConnectionPropertiesL(RStringF(), bearer, -1, -1, -1, RStringF(), RStringF(), -1);
	}

void CWspCmdLineProcessor::Do_SetLocalPortL(CCommandLine* aCmdLine)
	{
	// Get the argument and convert to an integer
	TPtrC arg = *aCmdLine->At(3);
	TInt locPort = -1;
	TLex lexer(arg);
	if ((lexer.Val(locPort) != KErrNone) || (locPort < 0) || (locPort > 65535))
		{
		_LIT(KCommandFailedMsg, "\nset connprops locport: invalid integer value supplied (must be between 0 and 65535)");
		iConsole.Printf(KCommandFailedMsg);
		return;
		}

	// Execute the command
	iEventDriver->SetConnectionPropertiesL(RStringF(), RStringF(), locPort, -1, -1, RStringF(), RStringF(), -1);
	}

void CWspCmdLineProcessor::Do_SetRemotePortL(CCommandLine* aCmdLine)
	{
	// Get the argument and convert to an integer
	TPtrC arg = *aCmdLine->At(3);
	TInt remPort = -1;
	TLex lexer(arg);
	if ((lexer.Val(remPort) != KErrNone) || (remPort < 0) || (remPort > 65535))
		{
		_LIT(KCommandFailedMsg, "\nset connprops remPort: invalid integer value supplied (must be between 0 and 65535)");
		iConsole.Printf(KCommandFailedMsg);
		return;
		}

	// Execute the command
	iEventDriver->SetConnectionPropertiesL(RStringF(), RStringF(), -1, remPort, -1, RStringF(), RStringF(), -1);
	}

void CWspCmdLineProcessor::Do_SetServiceNumL(CCommandLine* aCmdLine)
	{
	// Get the argument and convert to an integer
	TPtrC arg = *aCmdLine->At(3);
	TInt svcNum = -1;
	TLex lexer(arg);
	if ((lexer.Val(svcNum) != KErrNone) || (svcNum < 0) || (svcNum > 65535))
		{
		_LIT(KCommandFailedMsg, "\nset connprops svcNum: invalid integer value supplied (must be between 0 and 65535)");
		iConsole.Printf(KCommandFailedMsg);
		return;
		}

	// Execute the command
	iEventDriver->SetConnectionPropertiesL(RStringF(), RStringF(), -1, -1, svcNum, RStringF(), RStringF(), -1);
	}

void CWspCmdLineProcessor::Do_SetSessionTypeL(CCommandLine* aCmdLine)
	{
	// Get the address argument and convert to narrow then to RStringF
	TPtrC arg = *aCmdLine->At(3);
	RStringF sessType;
	if (arg.CompareF(KConnOriented) == 0)
		sessType = iStrP.StringF(HTTP::EWspConnectionOriented, RHTTPSession::GetTable());
	else if (arg.CompareF(KConnLess) == 0)
		sessType = iStrP.StringF(HTTP::EWspConnectionLess, RHTTPSession::GetTable());
	else
		{
		_LIT(KCommandFailedMsg, "\nset connprops sesstype: invalid session type value supplied (must be 'co' or 'cl')");
		iConsole.Printf(KCommandFailedMsg);
		return;
		}

	// Execute the command
	iEventDriver->SetConnectionPropertiesL(RStringF(), RStringF(), -1, -1, -1, sessType, RStringF(), -1);
	}

void CWspCmdLineProcessor::Do_SetSecurityL(CCommandLine* aCmdLine)
	{
	// Get the address argument and convert to narrow then to RStringF
	TPtrC arg = *aCmdLine->At(3);
	RStringF security;
	if (arg.CompareF(KUseWtls) == 0)
		security = iStrP.StringF(HTTP::EWspUseWtls, RHTTPSession::GetTable());
	else if (arg.CompareF(KDoNotUseWtls) == 0)
		security = iStrP.StringF(HTTP::EWspDoNotUseWtls, RHTTPSession::GetTable());
	else
		{
		_LIT(KCommandFailedMsg, "\nset connprops security: invalid security value supplied (must be 'on' or 'off')");
		iConsole.Printf(KCommandFailedMsg);
		return;
		}

	// Execute the command
	iEventDriver->SetConnectionPropertiesL(RStringF(), RStringF(), -1, -1, -1, RStringF(), security, -1);
	}

void CWspCmdLineProcessor::Do_SetTimeOutL(CCommandLine* aCmdLine)
	{
	// Get the argument and convert to an integer
	TPtrC arg = *aCmdLine->At(3);
	TInt timeOut = -1;
	TLex lexer(arg);
	if ((lexer.Val(timeOut) != KErrNone) || (timeOut < 0))
		{
		_LIT(KCommandFailedMsg, "\nset connprops timeOut: invalid integer value supplied (must be positive)");
		iConsole.Printf(KCommandFailedMsg);
		return;
		}

	// Execute the command
	iEventDriver->SetConnectionPropertiesL(RStringF(), RStringF(), -1, -1, -1, RStringF(), RStringF(), timeOut);
	}

void CWspCmdLineProcessor::Do_OpenTransactionL(CCommandLine* aCmdLine)
	{
	// Check syntax
	if (aCmdLine->Count() != 4)
		{
		_LIT(KCommandFailedMsg, "\nSyntax: opentrans <transaction ID> <method> <uri>");
		iConsole.Printf(KCommandFailedMsg);
		return;
		}

	// Check the supplied identified is not already in use
	TPtrC trId = *aCmdLine->At(1);
	TStoredItemBase item(NULL, NULL, NULL);
	TBool exists = FetchFromItemStore(trId, item);
	if (exists)
		{
		_LIT(KCommandFailedMsg, "\nError: Transaction with identifier '%S' already exists.");
		iConsole.Printf(KCommandFailedMsg, &trId);
		return;
		}

	// Get the two arguments and convert to narrow
	TPtrC methodArg = *aCmdLine->At(2);
	HBufC8* methodArg8 = HBufC8::NewL(methodArg.Length());
	CleanupStack::PushL(methodArg8);
	methodArg8->Des().Copy(methodArg);
	TPtrC uriArg = *aCmdLine->At(3);
	HBufC8* uriArg8 = HBufC8::NewL(uriArg.Length());
	CleanupStack::PushL(uriArg8);
	uriArg8->Des().Copy(uriArg);

	RStringF method = iStrP.OpenFStringL(*methodArg8);
	CleanupClosePushL(method);
	TUriParser8 uri;
	uri.Parse(*uriArg8);
	
	// Execute the command
	CDriverTrans* tr = iEventDriver->OpenTransactionL(method, uri, trId);
	CleanupStack::PopAndDestroy(3, methodArg8);

	// Store the transaction in the item store, with the supplied identifier
	TStoredItem<CDriverTrans> storeItem(*tr);
	StoreItemL(tr->TransId(), storeItem);	

	// Confirm
	_LIT(KCommandSucceededMsg, "\nTransaction with identifier '%S' created.");
	iConsole.Printf(KCommandSucceededMsg, &trId);
	}

void CWspCmdLineProcessor::Do_AddHeaderL(CCommandLine* aCmdLine)
	{
	// Check syntax
	if (aCmdLine->Count() != 5)
		{
		_LIT(KCommandFailedMsg, "\nSyntax: addhdr <transaction ID> <field name> <field type> <field val>");
		iConsole.Printf(KCommandFailedMsg);
		return;
		}

	// Check the supplied transaction identifier exists
	TPtrC trId = *aCmdLine->At(1);
	TStoredItem<CDriverTrans> item;
	TBool exists = FetchFromItemStore(trId, item);
	if (!exists)
		{
		_LIT(KCommandFailedMsg, "\nError: Transaction with identifier '%S' does not exist!");
		iConsole.Printf(KCommandFailedMsg, &trId);
		return;
		}

	// Get the field name, type and value
	RStringF fieldName;
	CDriverTrans::THdrValType fieldType = CDriverTrans::ETInt;
	RString fieldVal;
	if (GetHeaderFieldNameTypeValueLC(aCmdLine, 2, fieldName, fieldType, fieldVal))
		{
		// Execute the command
		item.StoredItem().AddHeaderL(fieldName, fieldType, fieldVal);
		CleanupStack::PopAndDestroy(2, &fieldName);
		}
	}

void CWspCmdLineProcessor::Do_AddHeaderWithParamL(CCommandLine* /*aCmdLine*/)
	{
	// @todo finish this method
	}

void CWspCmdLineProcessor::Do_AddSessHeaderL(CCommandLine* aCmdLine)
	{
	// Check syntax
	if (aCmdLine->Count() != 4)
		{
		_LIT(KCommandFailedMsg, "\nSyntax: addsesshdr <field name> <field type> <field val>");
		iConsole.Printf(KCommandFailedMsg);
		return;
		}

	// Get the field name, type and value
	RStringF fieldName;
	CDriverTrans::THdrValType fieldType = CDriverTrans::ETInt;
	RString fieldVal;
	if (GetHeaderFieldNameTypeValueLC(aCmdLine, 1, fieldName, fieldType, fieldVal))
		{
		// Execute the command
		iEventDriver->AddRequestSessionHeaderL(fieldName, fieldType, fieldVal);
		CleanupStack::PopAndDestroy(2, &fieldName);
		}
	}

void CWspCmdLineProcessor::Do_AddSessHeaderWithParamL(CCommandLine* /*aCmdLine*/)
	{
	// @todo finish this method
	}

void CWspCmdLineProcessor::Do_DumpSessionHeadersL(CCommandLine* aCmdLine)
	{
	// Check syntax
	if (aCmdLine->Count() != 1)
		{
		_LIT(KCommandFailedMsg, "\nSyntax: dumpsesshdrs <no args>");
		iConsole.Printf(KCommandFailedMsg);
		return;
		}

	iEventDriver->DumpSessionHeadersL(*this);
	}


TBool CWspCmdLineProcessor::GetHeaderFieldNameTypeValueLC(CCommandLine* aCmdLine,
														 TInt aFieldNameIdx,
														 RStringF& aFieldName,
														 CDriverTrans::THdrValType& aFieldType,
														 RString& aFieldVal)
	{
	// Extract field name, field type and field value
	TPtrC fieldNameArg = *aCmdLine->At(aFieldNameIdx);
	HBufC8* fieldNameArg8 = HBufC8::NewL(fieldNameArg.Length());
	CleanupStack::PushL(fieldNameArg8);
	fieldNameArg8->Des().Copy(fieldNameArg);
	aFieldName = iStrP.OpenFStringL(*fieldNameArg8);
	CleanupStack::PopAndDestroy(fieldNameArg8);
	CleanupClosePushL(aFieldName);
	//
	TPtrC fieldTypeArg = *aCmdLine->At(aFieldNameIdx + 1);
	if (fieldTypeArg.Compare(KInt) == 0)
		aFieldType = CDriverTrans::ETInt;
	else if (fieldTypeArg.Compare(KDate) == 0)
		aFieldType = CDriverTrans::ETDateTime;
	else if (fieldTypeArg.Compare(KStr) == 0)
		aFieldType = CDriverTrans::ERString;
	else if (fieldTypeArg.Compare(KStrF) == 0)
		aFieldType = CDriverTrans::ERStringF;
	else 
		{
		_LIT(KCommandFailedMsg, "\nError: Field type must be (int|date|str|strf)");
		iConsole.Printf(KCommandFailedMsg);
		CleanupStack::PopAndDestroy(&aFieldName);
		return EFalse;
		}
	//
	TPtrC fieldValArg = *aCmdLine->At(aFieldNameIdx + 2);
	HBufC8* fieldValArg8 = HBufC8::NewL(fieldValArg.Length());
	CleanupStack::PushL(fieldValArg8);
	fieldValArg8->Des().Copy(fieldValArg);
	aFieldVal = iStrP.OpenStringL(*fieldValArg8);
	CleanupStack::PopAndDestroy(fieldValArg8);
	CleanupClosePushL(aFieldVal);
	return ETrue;
	}

void CWspCmdLineProcessor::Do_AddBodyL(CCommandLine* aCmdLine)
	{
	// Check syntax
	if (aCmdLine->Count() != 3)
		{
		_LIT(KCommandFailedMsg, "\nSyntax: addbody <transaction ID> <body file name>");
		iConsole.Printf(KCommandFailedMsg);
		return;
		}

	// Check the supplied transaction identifier exists
	TPtrC trId = *aCmdLine->At(1);
	TStoredItem<CDriverTrans> item;
	TBool exists = FetchFromItemStore(trId, item);
	if (!exists)
		{
		_LIT(KCommandFailedMsg, "\nError: Transaction with identifier '%S' does not exist!");
		iConsole.Printf(KCommandFailedMsg, &trId);
		return;
		}

	// Create a file reading data supplier in the item store, which automatically
	// takes ownership.  Tag the object with an ID derived from time now, since it is
	// never going to be looked up, but should stay unique anyway
	TTime now;
	now.UniversalTime();
	TBuf<32> tag;
	now.FormatL(tag, _L("%D%M%Y%/0%3%/1%1%/2%2%/3 %-B%:0%J%:1%T%:2%S%.%*C4%:3%+B"));
	TCreatedItem<MHTTPDataSupplier, CWspFileDataSupplier> createdItem =
		CreateItemL(tag, _L("CWspFileDataSupplier"));

	// Read the file
	TPtrC fileName = *aCmdLine->At(2);
	TRAPD(err, createdItem.CreatedItem().SetFileL(fileName));
	if (err != KErrNone)
		{
		_LIT(KCommandFailedMsg, "\nError: File '%S' cannot be read!");
		iConsole.Printf(KCommandFailedMsg, aCmdLine->At(2));
		return;
		}

	// Invoke on the driver transaction found in the item store
	item.StoredItem().AddBody( createdItem.CreatedItem() );
	}

void CWspCmdLineProcessor::Do_SubmitL(CCommandLine* aCmdLine)
	{
	// Check syntax
	if (aCmdLine->Count() != 2)
		{
		_LIT(KCommandFailedMsg, "\nSyntax: submit <transaction ID>");
		iConsole.Printf(KCommandFailedMsg);
		return;
		}

	// Check the supplied transaction identifier exists
	TPtrC trId = *aCmdLine->At(1);
	TStoredItem<CDriverTrans> item;
	TBool exists = FetchFromItemStore(trId, item);
	if (!exists)
		{
		_LIT(KCommandFailedMsg, "\nError: Transaction with identifier '%S' does not exist!");
		iConsole.Printf(KCommandFailedMsg, &trId);
		return;
		}

	// Invoke on the driver transaction found in the item store
	item.StoredItem().SubmitL();
	}

void CWspCmdLineProcessor::Do_CancelL(CCommandLine* aCmdLine)
	{
	// Check syntax
	if (aCmdLine->Count() != 2)
		{
		_LIT(KCommandFailedMsg, "\nSyntax: cancel <trId>");
		iConsole.Printf(KCommandFailedMsg);
		return;
		}

	// Check the supplied transaction identifier exists
	TPtrC trId = *aCmdLine->At(1);
	TStoredItem<CDriverTrans> item;
	TBool exists = FetchFromItemStore(trId, item);
	if (!exists)
		{
		_LIT(KCommandFailedMsg, "\nError: Transaction with identifier '%S' does not exist!");
		iConsole.Printf(KCommandFailedMsg, &trId);
		return;
		}

	// Invoke on the driver transaction found in the item store
	item.StoredItem().Cancel();
	}

void CWspCmdLineProcessor::Do_CloseL(CCommandLine* aCmdLine)
	{
	// Check syntax
	if (aCmdLine->Count() != 2)
		{
		_LIT(KCommandFailedMsg, "\nSyntax: close <trId>");
		iConsole.Printf(KCommandFailedMsg);
		return;
		}

	// Check the supplied transaction identifier exists
	TPtrC trId = *aCmdLine->At(1);
	TStoredItem<CDriverTrans> item;
	TBool exists = FetchFromItemStore(trId, item);
	if (!exists)
		{
		_LIT(KCommandFailedMsg, "\nError: Transaction with identifier '%S' does not exist!");
		iConsole.Printf(KCommandFailedMsg, &trId);
		return;
		}

	// Invoke on the driver transaction found in the item store
	item.StoredItem().Close();
	}

void CWspCmdLineProcessor::Do_DumpRespHdrsL(CCommandLine* aCmdLine)
	{
	// Check syntax
	if (aCmdLine->Count() != 2)
		{
		_LIT(KCommandFailedMsg, "\nSyntax: dumpresphdrs <trId>");
		iConsole.Printf(KCommandFailedMsg);
		return;
		}

	// Check the supplied transaction identifier exists
	TPtrC trId = *aCmdLine->At(1);
	TStoredItem<CDriverTrans> item;
	TBool exists = FetchFromItemStore(trId, item);
	if (!exists)
		{
		_LIT(KCommandFailedMsg, "\nError: Transaction with identifier '%S' does not exist!");
		iConsole.Printf(KCommandFailedMsg, &trId);
		return;
		}

	// Invoke on the driver transaction found in the item store
	item.StoredItem().DumpResponseHeadersL(*this);
	}

void CWspCmdLineProcessor::Do_DumpRespBodyL(CCommandLine* aCmdLine)
	{
	// Check syntax
	if (aCmdLine->Count() != 2)
		{
		_LIT(KCommandFailedMsg, "\nSyntax: dumprespbody <trId>");
		iConsole.Printf(KCommandFailedMsg);
		return;
		}

	// Check the supplied transaction identifier exists
	TPtrC trId = *aCmdLine->At(1);
	TStoredItem<CDriverTrans> item;
	TBool exists = FetchFromItemStore(trId, item);
	if (!exists)
		{
		_LIT(KCommandFailedMsg, "\nError: Transaction with identifier '%S' does not exist!");
		iConsole.Printf(KCommandFailedMsg, &trId);
		return;
		}

	// Invoke on the driver transaction found in the item store
	item.StoredItem().DumpResponseBody(*this);
	}

void CWspCmdLineProcessor::TransactionRunL(CDriverTrans& aTransaction, const THTTPEvent& aEvent)
	{
	// Cancel the commandline processor
	iCmdLineProcessor->CancelGetCommand();

	// Check the event
	switch( aEvent.iStatus )
		{
	case THTTPEvent::EGotResponseHeaders:
		{
		_LIT(KMsg, "\nTrans event: (%S) Got Response Headers.");
		iConsole.Printf(KMsg, &(aTransaction.TransId()));
		} break;
	case THTTPEvent::EGotResponseBodyData:
		{
		_LIT(KMsg, "\nTrans event: (%S) Got Response Body Data.");
		iConsole.Printf(KMsg, &(aTransaction.TransId()));
		} break;
	case THTTPEvent::EResponseComplete:
		{
		_LIT(KMsg, "\nTrans event: (%S) Response Complete.");
		iConsole.Printf(KMsg, &(aTransaction.TransId()));
		} break;
	case THTTPEvent::ESucceeded:
		{
		_LIT(KMsg, "\nTrans event: (%S) Transaction Succeeded.");
		iConsole.Printf(KMsg, &(aTransaction.TransId()));
		} break;
	case THTTPEvent::EFailed:
		{
		_LIT(KMsg, "\nTrans event: (%S) Transaction Failed.");
		iConsole.Printf(KMsg, &(aTransaction.TransId()));
		} break;
	case THTTPEvent::EUnrecoverableError:
		{
		_LIT(KMsg, "\nTrans event: (%S) Unrecoverable Error.");
		iConsole.Printf(KMsg, &(aTransaction.TransId()));
		} break;
	case THTTPEvent::ERedirectedPermanently:
		{
		_LIT(KMsg, "\nTrans event: (%S) Request Redirected Permanently.");
		iConsole.Printf(KMsg, &(aTransaction.TransId()));
		} break;
	case THTTPEvent::ERedirectedTemporarily:
		{
		_LIT(KMsg, "\nTrans event: (%S) Request Redirected Temporarily.");
		iConsole.Printf(KMsg, &(aTransaction.TransId()));
		} break;
	case THTTPEvent::EMoreDataReceivedThanExpected:
		{
		_LIT(KMsg, "\nTrans event: (%S) More Data Received Than Expected.");
		iConsole.Printf(KMsg, &(aTransaction.TransId()));
		} break;
	default:

		break;
		}
	// Start-up the commandline processor
	iCmdLineProcessor->GetNextCommand();

	}

TInt CWspCmdLineProcessor::TransactionRunError(TInt aError, CDriverTrans& aTransaction, const THTTPEvent& aEvent)
	{
	// Cancel the commandline processor
	iCmdLineProcessor->CancelGetCommand();

	// Display a message
	_LIT(KMsg, "\nTrans (%S) event (%d):  Unhandled error, code = %d.");
	iConsole.Printf(KMsg, &(aTransaction.TransId()), aEvent.iStatus, aError);

	// Start-up the commandline processor
	iCmdLineProcessor->GetNextCommand();
	return KErrNone;
	}

void CWspCmdLineProcessor::SessionRunL(const THTTPSessionEvent& aEvent)
	{
	// Cancel the commandline processor
	iCmdLineProcessor->CancelGetCommand();

	// Check the event
	switch( aEvent.iStatus )
		{
	case THTTPSessionEvent::EConnectedOK:
		{
		_LIT(KWspConnectedOKMsg, "\nWSP Session connected. No reduced capabilities.");
		iConsole.Write(KWspConnectedOKMsg);
		} break;
	case THTTPSessionEvent::EConnectedWithReducedCapabilities:
		{
		_LIT(KWspConnectedReducedCapsMsg, "\nWSP Session connected. Capabilities reduced.");
		iConsole.Write(KWspConnectedReducedCapsMsg);
		} break;
	case THTTPSessionEvent::EDisconnected:
		{
		_LIT(KWspDisconnectedMsg, "\nWSP Session disconnected.");
		iConsole.Write(KWspDisconnectedMsg);
		} break;
	case THTTPSessionEvent::EAuthenticatedOK:
		{
		_LIT(KWspAuthOKMsg, "\nWSP/WTLS Authenticated OK!");
		iConsole.Write(KWspAuthOKMsg);
		} break;
	case THTTPSessionEvent::EAuthenticationFailure:
		{
		_LIT(KWspAuthFailedMsg, "\nWSP/WTLS Authenticated Failure");
		iConsole.Write(KWspAuthFailedMsg);
		} break;
	case THTTPSessionEvent::EConnectionTimedOut:
		{
		_LIT(KWspTimeOutMsg, "\nTimed out when attempting to connect to WAP Proxy");
		iConsole.Write(KWspTimeOutMsg);
		} break;
	case THTTPSessionEvent::ENotConnected:
		{
		/** The client has requested a transaction event that requires a session
			to be connected or the connection to be initiated, but neither is
			currently true. The transaction event will be left pending until the
			session is connected. An incoming event.
		*/
		_LIT(KWspMsg, "\nWarning: Not Connected");
		iConsole.Write(KWspMsg);
		} break;
	case THTTPSessionEvent::EExceptionInfo:
		{
		/** The proxy has sent some information that is not related to a 
			transaction and has no effect on the state of the session. The 
			information from the proxy is in the EProxyExceptionInfo property.
		*/
		_LIT(KWspMsg, "\nWarning: Exception Info");
		iConsole.Write(KWspMsg);
		} break;
	case THTTPSessionEvent::ERedirected:
		{
		/** The client connection request was (permanently) redirected to a new
			WAP proxy address. The client should check the EWspProxyAddress 
			property for the new address. The client's access-point database can
			then be updated with this address. No notification is given of a 
			temporary redirection.
		*/
		_LIT(KWspMsg, "\nWarning: (Permanently) Redirected");
		iConsole.Write(KWspMsg);
		} break;
	case THTTPSessionEvent::EAlreadyConnecting:
		{
		/** The client has requested a session event that is not valid whilst
			the WSP session is trying to establish a connection.
		*/
		_LIT(KWspMsg, "\nWarning: Already Connecting");
		iConsole.Write(KWspMsg);
		} break;
	case THTTPSessionEvent::EAlreadyConnected:
		{
		/** The client has requested a session event that is not valid whilst
			the WSP session is in the Connected state.
		*/
		_LIT(KWspMsg, "\nWarning: Already Connected");
		iConsole.Write(KWspMsg);
		} break;
	case THTTPSessionEvent::EAlreadyDisconnecting:
		{
		/** The client has requested a session event that is not valid whilst
			the WSP session is trying to close the connection.
		*/
		_LIT(KWspMsg, "\nWarning: Already Disconnecting");
		iConsole.Write(KWspMsg);
		} break;
	case THTTPSessionEvent::EAlreadyDisconnected:
		{
		/** The client has requested a session event that is not valid whilst
			the WSP session is in the Null (or disconnected) state.
		*/
		_LIT(KWspMsg, "\nWarning: Already Disconnected.");
		iConsole.Write(KWspMsg);
		} break;
	default:
		{
		// Try to get a string for this error
		const TDesC& errStr = CWspCmdLineProcessor::ErrCodeToString(aEvent.iStatus);
		if (errStr.Length() > 0)
			{
			_LIT(KErrMsg, "\nSession event error: %S.");
			iConsole.Printf(KErrMsg, &errStr);
			}
		else
			{
			_LIT(KDefMsg, "\nReceived unrecognised session event %d.");
			iConsole.Printf(KDefMsg, aEvent.iStatus);
			}
		} break;
		}
	// Start-up the commandline processor
	iCmdLineProcessor->GetNextCommand();
	}

TInt CWspCmdLineProcessor::SessionRunError(TInt aError, const THTTPSessionEvent& aEvent)
	{
	// Cancel the commandline processor
	iCmdLineProcessor->CancelGetCommand();

	// Display a message
	_LIT(KMsg, "\nSession event (%d):  Unhandled error, code = %d.");
	iConsole.Printf(KMsg, aEvent.iStatus, aError);

	// Start-up the commandline processor
	iCmdLineProcessor->GetNextCommand();
	return KErrNone;
	}

TStoredItemBase CWspCmdLineProcessor::DoCreateItemLC(const TDesC& aDriverClass)
	{
	TStoredItemBase item(NULL, NULL, NULL);
	if (!aDriverClass.Compare(_L("CWspFileDataSupplier")))
		{
		CWspFileDataSupplier* ds = CWspFileDataSupplier::NewL();
		CleanupStack::PushL(ds);

		// Create a 'created item' that holds the created item with a reference to its clean-up function.
		item = TCreatedItem<MHTTPDataSupplier, CWspFileDataSupplier>(*ds, CWspFileDataSupplier::CleanUp);
		}
	else
		TDriverPanic::Panic(TDriverPanic::ECannotCreateItemInStore);
	return item;
	}

const TDesC& CWspCmdLineProcessor::ErrCodeToString(TInt aError)
	{
	switch (aError)
		{
		case KWspErrServiceNumberNotDefined:
			return KMsgServiceNumberNotDefined();
		case KWspErrAccessPointNotSetUp:
			return KMsgAccessPointNotSetUp();
		case KWspErrBadRedirectedAddressData:
			return KMsgBadRedirectedAddressData();
		case KWspErrBearerNotSupported:
			return KMsgBearerNotSupported();
		case KWspErrRequestTooBig:
			return KMsgRequestTooBig();
		case KWspErrStreamedRequestDataNotSupported:
			return KMsgStreamedRequestDataNotSupported();
		case KWspErrUnsupportedSendPrimitive:
			return KMsgUnsupportedSendPrimitive();
		case KWspErrSessionClosingOrSuspending:
			return KMsgSessionClosingOrSuspending();
		case KWspErrExpectingBodyData:
			return KMsgExpectingBodyData();
		case KWspErrTrailerHeadersNotExpected:
			return KMsgTrailerHeadersNotExpected();
		case KWspErrMissingHeader:
			return KMsgMissingHeader();
		case KWspErrSMSBearerNotInCommsDB:
			return KMsgSMSBearerNotInCommsDB();
		case KWspErrCouldNotCreateTransportHandler:
			return KMsgCouldNotCreateTransportHandler();
		default:
			return KNullDesC();
		}
	}

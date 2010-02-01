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

#ifndef __CWSPCMDLINEPROCESSOR_H__
#define __CWSPCMDLINEPROCESSOR_H__

#include <e32base.h>
#include <e32cons.h>

#include "testconsole/mcommandhandler.h"
#include "t_wspeventdriver/ceventcallbackfilter.h"
#include "t_wspeventdriver/cwspeventdriver.h"
#include "t_wspeventdriver/mconsoledumper.h"

class CCommandLine;
class CConsoleBase;
class CCommandLineProcessor;

class CWspCmdLineProcessor : public CEventCallbackFilter,
							 public MCommandHandler,
							 public MConsoleDumper
	{
public:	// Methods

	static CWspCmdLineProcessor* NewL(CConsoleBase& aConsole, const TDesC& aInitialScript);

	~CWspCmdLineProcessor();

	void Start();

private:	// Methods from MCommandHandler

	virtual void HandleCommandLine(CCommandLine* aCmdLine);

private: // methods from MConsoleDumper

	/** 
	*/
	virtual void StartDump();

	/** 
	*/
	virtual void WriteData(const TDesC16& aData);

	/** 
	*/
	virtual void WriteData(const TDesC8& aData);

	/** 
	*/
	virtual void DumpData(const TDesC8& aData);

	/** 
	*/
	virtual void EndDump();

private:	// Methods from CEventCallbackFilter

/**
	@fn				SessionRunL(const THTTPSessionEvent& aEvent)
	@see			CEventCallbackFilter
 */
	virtual void SessionRunL(const THTTPSessionEvent& aEvent);

/**
	@fn				SessionRunError(TInt aError, const THTTPSessionEvent& aEvent)
	@see			CEventCallbackFilter
 */
	virtual TInt SessionRunError(TInt aError, const THTTPSessionEvent& aEvent);

/**
	@fn				TransactionRunL(CDriverTrans& aTransaction, const THTTPEvent& aEvent)
	@see			CEventCallbackFilter
 */
	virtual void TransactionRunL(CDriverTrans& aTransaction, const THTTPEvent& aEvent);

/**
	@fn				TransactionRunError(TInt aError, CDriverTrans& aTransaction, const THTTPEvent& aEvent)
	@see			CEventCallbackFilter
 */
	virtual TInt TransactionRunError(TInt aError, CDriverTrans& aTransaction, const THTTPEvent& aEvent);

	virtual TStoredItemBase DoCreateItemLC(const TDesC& aStubClass);

private:	// Methods

	CWspCmdLineProcessor(CConsoleBase& aConsole);

	void ConstructL(const TDesC& aInitialScript);

	void CommandDone();

private: // helpers for methods that can be invoked from the command line

	void Do_HelpL(CCommandLine* aCmdLine);
	void Do_ScriptL(CCommandLine* aCmdLine);
	void Do_SessionConnectL(CCommandLine* aCmdLine);
	void Do_SessionDisconnectL(CCommandLine* aCmdLine);
	void Do_AddSessHeaderL(CCommandLine* aCmdLine);
	void Do_AddSessHeaderWithParamL(CCommandLine* aCmdLine);
	void Do_DumpSessionHeadersL(CCommandLine* aCmdLine);
	void Do_SetL(CCommandLine* aCmdLine);
	void Do_SetClientMessageSizeL(CCommandLine* aCmdLine);
	void Do_SetServerMessageSizeL(CCommandLine* aCmdLine);
	void Do_SetUseAcknowledgementsL(CCommandLine* aCmdLine);
	void Do_SetUseSuspendResumeL(CCommandLine* aCmdLine);
	void Do_SetMethodMORL(CCommandLine* aCmdLine);
	void Do_SetConnectionPropertiesL(CCommandLine* aCmdLine);
	void Do_SetProxyAddressL(CCommandLine* aCmdLine);
	void Do_SetBearerL(CCommandLine* aCmdLine);
	void Do_SetLocalPortL(CCommandLine* aCmdLine);
	void Do_SetRemotePortL(CCommandLine* aCmdLine);
	void Do_SetServiceNumL(CCommandLine* aCmdLine);
	void Do_SetSessionTypeL(CCommandLine* aCmdLine);
	void Do_SetSecurityL(CCommandLine* aCmdLine);
	void Do_SetTimeOutL(CCommandLine* aCmdLine);
	void Do_OpenTransactionL(CCommandLine* aCmdLine);
	void Do_AddHeaderL(CCommandLine* aCmdLine);
	void Do_AddHeaderWithParamL(CCommandLine* aCmdLine);
	void Do_AddBodyL(CCommandLine* aCmdLine);
	void Do_SubmitL(CCommandLine* aCmdLine);
	void Do_CancelL(CCommandLine* aCmdLine);
	void Do_CloseL(CCommandLine* aCmdLine);
	void Do_DumpRespHdrsL(CCommandLine* aCmdLine);
	void Do_DumpRespBodyL(CCommandLine* aCmdLine);

	// Helper for Do_AddHeaderL and Do_AddSessHeaderL
	TBool GetHeaderFieldNameTypeValueLC(CCommandLine* aCmdLine,
										 TInt aFieldNameIdx,
										 RStringF& aFieldName,
										 CDriverTrans::THdrValType& aFieldType,
										 RString& aFieldVal);

	// Translate error codes into strings
	static const TDesC& ErrCodeToString(TInt aError);

private:	// Attributes

	CConsoleBase&				iConsole;

	CCommandLineProcessor*		iCmdLineProcessor;

	CWspEventDriver*			iEventDriver;

	/** The HTTP string pool, owned by the session in iEventDriver
	*/
	RStringPool			iStrP;

	// Flag that indicates if the cmdlineprocessor has been asked to dump data
	TBool iIsDumping;

	// Counter of the lines written since dumping started
	TInt iDumpLineCount;
	};

#endif	// __CWSPCMDLINEPROCESSOR_H__

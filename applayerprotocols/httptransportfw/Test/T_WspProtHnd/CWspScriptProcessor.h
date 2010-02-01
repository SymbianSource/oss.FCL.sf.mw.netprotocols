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

#ifndef __CWSPSCRIPTPROCESSOR_H__
#define __CWSPSCRIPTPROCESSOR_H__

#include <f32file.h>
#include <flogger.h>
#include <e32base.h>
#include <e32cons.h>
#include "cchatengine.h"
#include "chattriggers.h"
#include "testconsole/mcommandhandler.h"
#include "t_wspeventdriver/ceventcallbackfilter.h"
#include "t_wspeventdriver/cwspeventdriver.h"


class CConsoleBase;
class CWspEventDriver;
class CHTTPTestUtils;
class CScriptFile;
class CDriverScriptStarter;
class CWspScriptProcessor : public CEventCallbackFilter, public MChatEngineObserver,
							public MChatSendTrigger
	{
friend class CDriverScriptStarter;
public:	// Methods

	static CWspScriptProcessor* NewL(CScriptFile& aDriverIniFile, CHTTPTestUtils& aTestUtils);

	~CWspScriptProcessor();

	void Start();

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

private:	// Methods MChatEngineObserver

	virtual void ConsoleLog(const TDesC& aLogText);
	virtual void ScriptError(TInt aErrorCode, const TDesC& aErrorText);
	virtual void SetCompletionCode(TInt aCompletionCode);
	virtual void MCEO_Reserved();

private:	// Methods from MChatSendTrigger

	virtual void SendInvokeL(CChatMethodInvocation& aMethodInvocation);
	virtual void MCST_Reserved();

private: // Methods

	CWspScriptProcessor(CScriptFile& aDriverIniFile, CHTTPTestUtils& aTestUtils);
	void ConstructL();
	
	TInt TransactionRunErrorL(TInt aError, CDriverTrans& aTransaction, const THTTPEvent& aEvent);
	TInt SessionRunErrorL(TInt aError, const THTTPSessionEvent& aEvent);

private: // helpers for methods that can be invoked by the script

	/** CWspEventDriver API methods
	*/
	void Do_SessionConnectL(CChatMethodInvocation& aMethodInvocation);
	void Do_SessionDisconnectL(CChatMethodInvocation& aMethodInvocation);
	void Do_SetClientMessageSizeL(CChatMethodInvocation& aMethodInvocation);
	void Do_SetServerMessageSizeL(CChatMethodInvocation& aMethodInvocation);
	void Do_SetUseAcknowledgementsL(CChatMethodInvocation& aMethodInvocation);
	void Do_SetUseSuspendResumeL(CChatMethodInvocation& aMethodInvocation);
	void Do_SetMethodMORL(CChatMethodInvocation& aMethodInvocation);
	void Do_SetConnectionPropertiesLL(CChatMethodInvocation& aMethodInvocation);
	void Do_GetConnectionPropertiesL(CChatMethodInvocation& aMethodInvocation);
	void Do_OpenTransactionLL(CChatMethodInvocation& aMethodInvocation);
	void Do_AddRequestSessionHeaderLL(CChatMethodInvocation& aMethodInvocation);
	void Do_AddRequestSessionHeaderWithParamLL(CChatMethodInvocation& aMethodInvocation);

	/** CDriverTrans API methods
	*/
	void Do_AddHeaderL(CChatMethodInvocation& aMethodInvocation);
	void Do_AddHeaderWithParamL(CChatMethodInvocation& aMethodInvocation);
	void Do_AddBodyL(CChatMethodInvocation& aMethodInvocation);
	void Do_GetHeaderLL(CChatMethodInvocation& aMethodInvocation);
	void Do_GetBodyL(CChatMethodInvocation& aMethodInvocation);
	void Do_GetStatusCodeL(CChatMethodInvocation& aMethodInvocation);
	void Do_SubmitLL(CChatMethodInvocation& aMethodInvocation);
	void Do_CancelL(CChatMethodInvocation& aMethodInvocation);
	void Do_CloseL(CChatMethodInvocation& aMethodInvocation);
	void Do_MoreRequestBodyDataL(CChatMethodInvocation& aMethodInvocation);

	/** CDriverDataSupplier API methods
	*/
	void Do_DriverDataSupplier_OverallDataSizeL(CChatMethodInvocation& aMethodInvocation);
	void Do_DriverDataSupplier_GetNextDataPartL(CChatMethodInvocation& aMethodInvocation);
	void Do_DriverDataSupplier_ReleaseDataL(CChatMethodInvocation& aMethodInvocation);
	void Do_DriverDataSupplier_ResetL(CChatMethodInvocation& aMethodInvocation);

private: // helpers for the helpers...

	void Do_AddHeader_PreliminariesLC(CChatMethodInvocation& aMethodInvocation, RStringF& aFieldName,
									  CDriverTrans::THdrValType& aFieldType, RString& aFieldValue);

	void Do_AddHeaderWithParam_PreliminariesLC(CChatMethodInvocation& aMethodInvocation,
											 RStringF& aFieldName, CDriverTrans::THdrValType& aFieldType,
											 RString& aFieldValue, RStringF& aParamName,
											 CDriverTrans::THdrValType& aParamType, RString& aParamValue);

private:	// Attributes

	/** File server session, used by chat engine
	*/
	RFs iFileSrv;

	/** Logger session, used by chat engine
	*/
	RFileLogger iLogger;

	/** The initial logging level done by the driver's chat engine
	*/
	CChatEngine::TLogLevel iLoggingLevel;

	/** The test utilities, providing a console window and summary logger used by the driver
	*/
	CHTTPTestUtils& iTestUtils;

	/** Reader for the driver's .ini file, which contains links to the individual chat scripts.  This is owned
		and opened elsewhere.
	*/
	CScriptFile&		iDriverIniFile;

	/** Helper that reads from the driver's .ini file, and creates the chat engine with each individual scripts
	*/
	CDriverScriptStarter*	iScriptStarter;

	/** The engine used to parse 'chat' scripts, and stimulate the event driver according to their commands
	*/
	CChatEngine*		iChatEngine;

	/** The driver which sends events to the WSP Protocol Handler and receives callback events
	*/
	CWspEventDriver*	iEventDriver;

	/** The HTTP string pool, owned by the session in iEventDriver
	*/
	RStringPool			iStrP;

	/** 
	*/
	TInt iLeakTests;

	/**
	*/
	TInt iFirstLeakIteration;

	/**
	*/
	TInt iFailureError;
	};


class CScriptSection;

/** An active object utility used by the WSP Script Processor to read the driver .ini file and start each
	new script in a new chat engine.
*/
class CDriverScriptStarter : public CActive
	{
public: // Methods

	/** Factory construction method
	*/
	static CDriverScriptStarter* NewL(CWspScriptProcessor& aDriver);

	/** Destructor
	*/
	virtual ~CDriverScriptStarter();

	/** Request the start of the next script found in the .ini file
	*/
	void StartNextScript();

private: // Methods

	/** Run the script starter
	*/
	virtual void RunL();

	/** Handle errors that caused RunL to leave
	*/
	virtual TInt RunError(TInt aError);

	/** Active-object cancellation protocol
	*/
	virtual void DoCancel();

	/** Normal constructor
	*/
	CDriverScriptStarter(CWspScriptProcessor& aDriver);

	/** Second-phase construction
	*/
	void ConstructL();

	/** Helper for RunL - finds the next script file identified in the .ini file and creates a new
		chat engine in the driver to handle it.
	*/
	void DoRunL();

	/** Helper for RunL used when doing iterative leak-testing
	*/
	void DoLeakTestsRunL();

private: // Attributes

	/** The driver this object is helping
	*/
	CWspScriptProcessor& iDriver;

	/** Index of the section in the .ini file from which the current chat script is being executed
	*/
	TInt iSectionIdx;

	/** The .ini file section which defines the current chat script being run
	*/
	CScriptSection* iSection;
	};


#endif	// __CWSPSCRIPTPROCESSOR_H__

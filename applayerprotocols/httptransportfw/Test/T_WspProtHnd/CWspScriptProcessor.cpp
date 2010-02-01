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
//
#include <httpstringconstants.h>

// Local includes
//
#include "t_wspeventdriver/cwspeventdriver.h"
#include "t_wspeventdriver/tdriverpanic.h"
#include "t_wspeventdriver/cdriverdatasupplier.h"
#include "drivercustomvalidators.h"
#include "testscripts.h"
#include "cchatmethodinvocation.h"
#include "cchatitemstorevalidator.h"
#include "cchattintvalidator.h"
#include "cchattboolvalidator.h"
#include "cchattdesc8validator.h"

// Class signature
//
#include "cwspscriptprocessor.h"




// Constants used in this file
//
_LIT(KWspThStubIniFile, "StubIni");
_LIT(KStubTestCount, "StubTestCount");
_LIT(KChatLogsDir, "ChatLogsDir");
_LIT(KLogLevel, "LogLevel");
_LIT(KDefaultSectionName, "Defaults");
_LIT(KTestSectionName, "Test");
_LIT(KScriptFile, "ScriptFile");
_LIT(KReturnCode, ":RETURN:");
_LIT(KLeakTestCount, "LeakTestCount");
_LIT(KLeakTestFirstIter, "LeakTestFirstIter");
_LIT(KSessRunErrInOOM, "MHFSessionRunError called in OOM situation: err = %d, event = %d");
//
_LIT(KSessionConnectL, "SessionConnectL");
_LIT(KSessionDisconnectL, "SessionDisconnectL");
_LIT(KSetClientMessageSizeL, "SetClientMessageSizeL");
_LIT(KSetServerMessageSizeL, "SetServerMessageSizeL");
_LIT(KSetUseAcknowledgementsL, "SetUseAcknowledgementsL");
_LIT(KSetUseSuspendResumeL, "SetUseSuspendResumeL");
_LIT(KSetMethodMORL, "SetMethodMORL");
_LIT(KSetConnectionPropertiesL, "SetConnectionPropertiesL");
_LIT(KGetConnectionProperties, "GetConnectionProperties");
_LIT(KOpenTransactionL, "OpenTransactionL");
_LIT(KAddRequestSessionHeaderL, "AddRequestSessionHeaderL");
_LIT(KAddRequestSessionHeaderWithParamL, "AddRequestSessionHeaderWithParamL");
_LIT(KAddHeaderL, "AddHeaderL");
_LIT(KAddHeaderWithParamL, "AddHeaderWithParamL");
_LIT(KAddBody, "AddBody");
_LIT(KGetBody, "GetBody");
_LIT(KGetHeaderL, "GetHeaderL");
_LIT(KGetStatusCode, "GetStatusCode");
_LIT(KSubmitL, "SubmitL");
_LIT(KCancel, "Cancel");
_LIT(KClose, "Close");
_LIT(KMoreRequestBodyDataL, "MoreRequestBodyDataL");
_LIT(KDriverDataSupplier_OverallDataSize, "MHTTPDataSupplier::OverallDataSize");
_LIT(KDriverDataSupplier_GetNextDataPart, "MHTTPDataSupplier::GetNextDataPart");
_LIT(KDriverDataSupplier_ReleaseData, "MHTTPDataSupplier::ReleaseData");

CWspScriptProcessor* CWspScriptProcessor::NewL(CScriptFile& aDriverIniFile, CHTTPTestUtils& aTestUtils)
	{
	CWspScriptProcessor* self = new (ELeave) CWspScriptProcessor(aDriverIniFile, aTestUtils);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CWspScriptProcessor::~CWspScriptProcessor()
	{
	delete iChatEngine;
	delete iEventDriver;
	delete iScriptStarter;
	iLogger.Close();
	iFileSrv.Close();
	}

CWspScriptProcessor::CWspScriptProcessor(CScriptFile& aDriverIniFile, CHTTPTestUtils& aTestUtils)
: CEventCallbackFilter(), iTestUtils(aTestUtils), iDriverIniFile(aDriverIniFile)
	{
	}

void CWspScriptProcessor::ConstructL()
	{
	// Make server connections (without actually Open()ing) to the file server and file logger
	User::LeaveIfError(iFileSrv.Connect());
	User::LeaveIfError(iLogger.Connect());

	// Read the name of the stub .ini file from the driver .ini file
	const TDesC& iniFileName = iDriverIniFile.ItemValue(KDefaultSectionName(),
														KWspThStubIniFile(),
														_L("stub_tests.ini"));

	// Determine whether the stub is to do test counting using RTest in HTTPTESTUTILS - should be
	// turned off when doing leaking testing in the driver
	TBool enableStubTestCounting = iDriverIniFile.ItemValue(KDefaultSectionName(),
														KStubTestCount(),
														1);

	HBufC* iniFileNameAndFlag = HBufC::NewLC(iniFileName.Length() + 2);
	iniFileNameAndFlag->Des().Copy(iniFileName);
	_LIT(KStubTestCountingOn, " 1");
	_LIT(KStubTestCountingOff, " 0");
	iniFileNameAndFlag->Des().Append(enableStubTestCounting?KStubTestCountingOn:KStubTestCountingOff);

	// Create the event driver and get it's string pool ref
	iEventDriver = CWspEventDriver::NewL(*this, *iniFileNameAndFlag);
  	iStrP = iEventDriver->StringPool();

	// Clean up
	CleanupStack::PopAndDestroy(iniFileNameAndFlag);

	// Create the script starter
	iScriptStarter = CDriverScriptStarter::NewL(*this);
	}

void CWspScriptProcessor::Start()
	{
 	// Reset the event driver before running the first test
	(void)iEventDriver->Reset();

	iScriptStarter->StartNextScript();
	}

void CWspScriptProcessor::TransactionRunL(CDriverTrans& aTransaction, const THTTPEvent& aEvent)
	{
	// Represent this method invocation
	CChatMethodInvocation* methodCalled = CChatMethodInvocation::NewL(*iChatEngine, _L("TransactionRunL"));
	CleanupStack::PushL(methodCalled);

	// validate 'aTransaction'
	TStoredItem<CDriverTrans> item1(aTransaction);
	CChatItemStoreValidator* vtor1 = CChatItemStoreValidator::NewL(_L("aTransaction"), item1);
	CleanupStack::PushL(vtor1);
	methodCalled->AdoptValidator(vtor1);
	CleanupStack::Pop(vtor1);

	// validate 'aEvent'
	CChatTHTTPEventValidator* vtor2 = CChatTHTTPEventValidator::NewL(_L("aEvent"), aEvent);
	CleanupStack::PushL(vtor2);
	methodCalled->AdoptValidator(vtor2);
	CleanupStack::Pop(vtor2);

	iChatEngine->TestExpectation(*methodCalled);
	CleanupStack::PopAndDestroy(methodCalled);
	}

TInt CWspScriptProcessor::TransactionRunError(TInt aError, CDriverTrans& aTransaction, const THTTPEvent& aEvent)
	{
	TInt retVal = KErrNone;
	TRAPD(err, retVal = TransactionRunErrorL(aError, aTransaction, aEvent));
	if(err!=KErrNone)
		return err;
		
	return retVal;
	}
	
TInt CWspScriptProcessor::TransactionRunErrorL(TInt aError, CDriverTrans& aTransaction, const THTTPEvent& aEvent)
	{
	// Represent this method invocation
	CChatMethodInvocation* methodCalled = CChatMethodInvocation::NewL(*iChatEngine, _L("TransactionRunError"));
	CleanupStack::PushL(methodCalled);

	// validate 'aError'
	CChatTIntValidator* vtor1 = CChatTIntValidator::NewL(_L("aError"), aError);
	CleanupStack::PushL(vtor1);
	methodCalled->AdoptValidator(vtor1);
	CleanupStack::Pop(vtor1);

	// validate 'aTransaction'
	TStoredItem<CDriverTrans> item1(aTransaction);
	CChatItemStoreValidator* vtor2 = CChatItemStoreValidator::NewL(_L("aTransaction"), item1);
	CleanupStack::PushL(vtor2);
	methodCalled->AdoptValidator(vtor2);
	CleanupStack::Pop(vtor2);

	// validate 'aEvent'
	CChatTHTTPEventValidator* vtor3 = CChatTHTTPEventValidator::NewL(_L("aEvent"), aEvent);
	CleanupStack::PushL(vtor3);
	methodCalled->AdoptValidator(vtor3);
	CleanupStack::Pop(vtor3);

	// Test against the script
	iChatEngine->TestExpectation(*methodCalled);

	// Extract the return code
	const CChatParameter* retParam = methodCalled->GetParameter(KReturnCode());
	__ASSERT_DEBUG(retParam != NULL, TDriverPanic::Panic(TDriverPanic::ENullPointer));
	CChatTIntValidator* retVtor = CChatTIntValidator::NewL(*retParam);
	CleanupStack::PushL(retVtor);
	TInt retCode = retVtor->IntValue();

	// Clean up and finish
	CleanupStack::PopAndDestroy(2, methodCalled);	
	return retCode;
	}

void CWspScriptProcessor::SessionRunL(const THTTPSessionEvent& aEvent)
	{
	// Special case used for OOM testing - if aEvent's value is KErrNoMemory, then leave
	// right away, since the remainder of the script will not be valid in the test
	if ((aEvent.iStatus == KErrNoMemory) && (iLeakTests > 0))
		User::Leave(aEvent.iStatus);

	// Represent this method invocation
	CChatMethodInvocation* method = CChatMethodInvocation::NewL(*iChatEngine, _L("SessionRunL"));
	CleanupStack::PushL(method);

	// validate 'aEvent'
	CChatTHTTPEventValidator* vtor = CChatTHTTPEventValidator::NewL(_L("aEvent"), aEvent);
	CleanupStack::PushL(vtor);
	method->AdoptValidator(vtor);
	CleanupStack::Pop(vtor);

	// Test against the script
	iChatEngine->TestExpectation(*method);
	CleanupStack::PopAndDestroy(method);
	}

TInt CWspScriptProcessor::SessionRunError(TInt aError, const THTTPSessionEvent& aEvent)
	{
	TInt retVal = KErrNone;
	TRAPD(err, retVal = SessionRunErrorL(aError, aEvent));
	if(err!=KErrNone)
		return err;
		
	return retVal;
	}

TInt CWspScriptProcessor::SessionRunErrorL(TInt aError, const THTTPSessionEvent& aEvent)
	{
	// Special case used for OOM testing - if aError is KErrNoMemory, then fail the
	// test
	if ((aError == KErrNoMemory) && (iLeakTests > 0))
		{
		TBuf<KMaxFileName> errText;
		errText.Format(KSessRunErrInOOM(), aError, aEvent.iStatus);
		ScriptError(aError, errText);
		return KErrNone;
		}

	// Represent this method invocation
	CChatMethodInvocation* method = CChatMethodInvocation::NewL(*iChatEngine, _L("SessionRunError"));
	CleanupStack::PushL(method);

	// validate 'aError'
	CChatTIntValidator* vtor1 = CChatTIntValidator::NewL(_L("aError"), aError);
	CleanupStack::PushL(vtor1);
	method->AdoptValidator(vtor1);
	CleanupStack::Pop(vtor1);

	// validate 'aEvent'
	CChatTHTTPEventValidator* vtor2 = CChatTHTTPEventValidator::NewL(_L("aEvent"), aEvent);
	CleanupStack::PushL(vtor2);
	method->AdoptValidator(vtor2);
	CleanupStack::Pop(vtor2);

	// Test against the script
	iChatEngine->TestExpectation(*method);

	// Extract the return code
	const CChatParameter* retParam = method->GetParameter(KReturnCode());
	__ASSERT_DEBUG(retParam != NULL, TDriverPanic::Panic(TDriverPanic::ENullPointer));
	CChatTIntValidator* retVtor = CChatTIntValidator::NewL(*retParam);
	CleanupStack::PushL(retVtor);
	TInt retCode = retVtor->IntValue();

	// Clean up and finish
	CleanupStack::PopAndDestroy(2, method);	
	return retCode;
	}

TStoredItemBase CWspScriptProcessor::DoCreateItemLC(const TDesC& aDriverClass)
	{
	TStoredItemBase item(NULL, NULL, NULL);
	if (!aDriverClass.Compare(_L("CDriverDataSupplier")))
		{
		CDriverDataSupplier* ds = CDriverDataSupplier::NewL(*iChatEngine);
		CleanupStack::PushL(ds);

		// Create a 'stored item' that holds the created item with a reference to its clean-up function.
		item = TCreatedItem<MHTTPDataSupplier, CDriverDataSupplier>(*ds, CDriverDataSupplier::CleanUp);
		}
	else
		TDriverPanic::Panic(TDriverPanic::ECannotCreateItemInStore);
	return item;
	}

void CWspScriptProcessor::SendInvokeL(CChatMethodInvocation& aMethodInvocation)
	{
	if (aMethodInvocation.MethodName().Compare(KSessionConnectL()) == 0)
		{
		Do_SessionConnectL(aMethodInvocation);
		}
	else if (aMethodInvocation.MethodName().Compare(KSessionDisconnectL()) == 0)
		{
		Do_SessionDisconnectL(aMethodInvocation);
		}
	else if (aMethodInvocation.MethodName().Compare(KSetClientMessageSizeL()) == 0)
		{
		Do_SetClientMessageSizeL(aMethodInvocation);
		}
	else if (aMethodInvocation.MethodName().Compare(KSetServerMessageSizeL()) == 0)
		{
		Do_SetServerMessageSizeL(aMethodInvocation);
		}
	else if (aMethodInvocation.MethodName().Compare(KSetUseAcknowledgementsL()) == 0)
		{
		Do_SetUseAcknowledgementsL(aMethodInvocation);
		}
	else if (aMethodInvocation.MethodName().Compare(KSetUseSuspendResumeL()) == 0)
		{
		Do_SetUseSuspendResumeL(aMethodInvocation);
		}
	else if (aMethodInvocation.MethodName().Compare(KSetMethodMORL()) == 0)
		{
		Do_SetMethodMORL(aMethodInvocation);
		}
	else if (aMethodInvocation.MethodName().Compare(KSetConnectionPropertiesL()) == 0)
		{
		Do_SetConnectionPropertiesLL(aMethodInvocation);
		}
	else if (aMethodInvocation.MethodName().Compare(KGetConnectionProperties()) == 0)
		{
		Do_GetConnectionPropertiesL(aMethodInvocation);
		}
	else if (aMethodInvocation.MethodName().Compare(KOpenTransactionL()) == 0)
		{
		Do_OpenTransactionLL(aMethodInvocation);
		}
	else if (aMethodInvocation.MethodName().Compare(KAddRequestSessionHeaderL()) == 0)
		{
		Do_AddRequestSessionHeaderLL(aMethodInvocation);
		}
	else if (aMethodInvocation.MethodName().Compare(KAddRequestSessionHeaderWithParamL()) == 0)
		{
		Do_AddRequestSessionHeaderWithParamLL(aMethodInvocation);
		}
	else if (aMethodInvocation.MethodName().Compare(KAddHeaderL()) == 0)
		{
		Do_AddHeaderL(aMethodInvocation);
		}
	else if (aMethodInvocation.MethodName().Compare(KAddHeaderWithParamL()) == 0)
		{
		Do_AddHeaderWithParamL(aMethodInvocation);
		}
	else if (aMethodInvocation.MethodName().Compare(KAddBody()) == 0)
		{
		Do_AddBodyL(aMethodInvocation);
		}
	else if (aMethodInvocation.MethodName().Compare(KGetHeaderL()) == 0)
		{
		Do_GetHeaderLL(aMethodInvocation);
		}
	else if (aMethodInvocation.MethodName().Compare(KGetBody()) == 0)
		{
		Do_GetBodyL(aMethodInvocation);
		}
	else if (aMethodInvocation.MethodName().Compare(KGetStatusCode()) == 0)
		{
		Do_GetStatusCodeL(aMethodInvocation);
		}
	else if (aMethodInvocation.MethodName().Compare(KSubmitL()) == 0)
		{
		Do_SubmitLL(aMethodInvocation);
		}
	else if (aMethodInvocation.MethodName().Compare(KCancel()) == 0)
		{
		Do_CancelL(aMethodInvocation);
		}
	else if (aMethodInvocation.MethodName().Compare(KClose()) == 0)
		{
		Do_CloseL(aMethodInvocation);
		}
	else if (aMethodInvocation.MethodName().Compare(KMoreRequestBodyDataL()) == 0)
		{
		Do_MoreRequestBodyDataL(aMethodInvocation);
		}
	else if (aMethodInvocation.MethodName().Compare(KDriverDataSupplier_OverallDataSize()) == 0)
		{
		Do_DriverDataSupplier_OverallDataSizeL(aMethodInvocation);
		}
	else if (aMethodInvocation.MethodName().Compare(KDriverDataSupplier_GetNextDataPart()) == 0)
		{
		Do_DriverDataSupplier_GetNextDataPartL(aMethodInvocation);
		}
	else if (aMethodInvocation.MethodName().Compare(KDriverDataSupplier_ReleaseData()) == 0)
		{
		Do_DriverDataSupplier_ReleaseDataL(aMethodInvocation);
		}
	else
		TDriverPanic::Panic(TDriverPanic::EUnrecognisedMethod);	
	}

void CWspScriptProcessor::MCST_Reserved()
	{
	}

void CWspScriptProcessor::ConsoleLog(const TDesC& aLogText)
	{
	iTestUtils.Test().Printf(_L("%S\n"), &aLogText);
	}

void CWspScriptProcessor::ScriptError(TInt aErrorCode, const TDesC& aErrorText)
	{
	iTestUtils.Test().Printf(_L(">> Driver Script Error: %d (%S)\n"), aErrorCode, &aErrorText);
	iFailureError = aErrorCode;

	// Stop the scheduler in order that the script starter can move onto the next
	// leak iteration or script if not leak testing.
	CActiveScheduler::Stop();
	}

void CWspScriptProcessor::SetCompletionCode(TInt aCompletionCode)
	{
	// Just log to the console if we're doing leak testing
	iTestUtils.Test().Printf(_L(">> Driver Script Completed with code: %d\n"), aCompletionCode);

	// Record the completion code
	iFailureError = aCompletionCode;

	// Ensure the test utils log a test end, if leak testing isn't being used.
	if (iLeakTests == 0)
		iTestUtils.EndTest(aCompletionCode);

	// Purge stuff in the item store so that it's not full of defunct pointers when the next script runs!
	FlushItemStore();

	// Stop the scheduler in order that the script starter can move onto the next script
	CActiveScheduler::Stop();
	}

void CWspScriptProcessor::MCEO_Reserved()
	{
	}

void CWspScriptProcessor::Do_SessionConnectL(CChatMethodInvocation& /*aMethodInvocation*/)
	{
	iEventDriver->SessionConnectL();
	}

void CWspScriptProcessor::Do_SessionDisconnectL(CChatMethodInvocation& /*aMethodInvocation*/)
	{
	iEventDriver->SessionDisconnectL();
	}

void CWspScriptProcessor::Do_SetClientMessageSizeL(CChatMethodInvocation& aMethodInvocation)
	{
	// TUint32 aSize
	_LIT(KASize, "aSize");
	const CChatParameter* param_aSize = aMethodInvocation.GetParameter(KASize());
	__ASSERT_DEBUG(param_aSize != NULL, TDriverPanic::Panic(TDriverPanic::ENullPointer));
	CChatTIntValidator* vtor_aSize = CChatTIntValidator::NewL(*param_aSize);
	CleanupStack::PushL(vtor_aSize);

	// Invoke the method
	iEventDriver->SetClientMessageSizeL(vtor_aSize->IntValue());
	CleanupStack::PopAndDestroy(vtor_aSize);
	}

void CWspScriptProcessor::Do_SetServerMessageSizeL(CChatMethodInvocation& aMethodInvocation)
	{
	// TUint32 aSize
	_LIT(KASize, "aSize");
	const CChatParameter* param_aSize = aMethodInvocation.GetParameter(KASize());
	__ASSERT_DEBUG(param_aSize != NULL, TDriverPanic::Panic(TDriverPanic::ENullPointer));
	CChatTIntValidator* vtor_aSize = CChatTIntValidator::NewL(*param_aSize);
	CleanupStack::PushL(vtor_aSize);

	// Invoke the method
	iEventDriver->SetServerMessageSizeL(vtor_aSize->IntValue());
	CleanupStack::PopAndDestroy(vtor_aSize);
	}

void CWspScriptProcessor::Do_SetUseAcknowledgementsL(CChatMethodInvocation& /*aMethodInvocation*/)
	{
	iEventDriver->SetUseAcknowledgementsL();
	}

void CWspScriptProcessor::Do_SetUseSuspendResumeL(CChatMethodInvocation& /*aMethodInvocation*/)
	{
	iEventDriver->SetUseSuspendResumeL();
	}

void CWspScriptProcessor::Do_SetMethodMORL(CChatMethodInvocation& aMethodInvocation)
	{
	// TUint32 aMOR
	_LIT(KAMOR, "aMOR");
	const CChatParameter* param_aMOR = aMethodInvocation.GetParameter(KAMOR());
	__ASSERT_DEBUG(param_aMOR != NULL, TDriverPanic::Panic(TDriverPanic::ENullPointer));
	CChatTIntValidator* vtor_aMOR = CChatTIntValidator::NewL(*param_aMOR);
	CleanupStack::PushL(vtor_aMOR);

	// Invoke the method
	iEventDriver->SetMethodMORL(vtor_aMOR->IntValue());
	CleanupStack::PopAndDestroy(vtor_aMOR);
	}

void CWspScriptProcessor::Do_SetConnectionPropertiesLL(CChatMethodInvocation& aMethodInvocation)
	{
	// RStringF aProxyAddress
	// RStringF aBearer
	// TInt aLocalPort
	// TInt aRemotePort
	// TInt aServiceNum
	// RStringF aSessionType
	// RStringF aIsSecure
	// TInt aTimeOut

	_LIT(KAProxyAddress, "aProxyAddress");
	_LIT(KABearer, "aBearer");
	_LIT(KALocalPort, "aLocalPort");
	_LIT(KARemotePort, "aRemotePort");
	_LIT(KAServiceNum, "aServiceNum");
	_LIT(KASessionType, "aSessionType");
	_LIT(KAIsSecure, "aIsSecure");
	_LIT(KATimeOut, "aTimeOut");
	TInt numStringsOpened = 0;
	//
	const CChatParameter* param_aProxyAddress = aMethodInvocation.GetParameter(KAProxyAddress());
	RStringF str_aProxyAddress;
	if (param_aProxyAddress)
		{
		CChatTDesC8Validator* vtor_aProxyAddress = CChatTDesC8Validator::NewL(*param_aProxyAddress);
		CleanupStack::PushL(vtor_aProxyAddress);
		str_aProxyAddress = iStrP.OpenFStringL(vtor_aProxyAddress->DesC8Value());
		CleanupStack::PopAndDestroy(vtor_aProxyAddress);
		CleanupClosePushL(str_aProxyAddress);
		++numStringsOpened;
		}
	//
	const CChatParameter* param_aBearer = aMethodInvocation.GetParameter(KABearer());
	RStringF str_aBearer;
	if (param_aBearer)
		{
		CChatTDesC8Validator* vtor_aBearer = CChatTDesC8Validator::NewL(*param_aBearer);
		CleanupStack::PushL(vtor_aBearer);
		str_aBearer = iStrP.OpenFStringL(vtor_aBearer->DesC8Value());
		CleanupStack::PopAndDestroy(vtor_aBearer);
		CleanupClosePushL(str_aBearer);
		++numStringsOpened;
		}
	//
	const CChatParameter* param_aLocalPort = aMethodInvocation.GetParameter(KALocalPort());
	TInt int_aLocalPort = KErrNotFound;
	if (param_aLocalPort != NULL)
		{
		CChatTIntValidator* vtor_aLocalPort = CChatTIntValidator::NewL(*param_aLocalPort);
		int_aLocalPort = vtor_aLocalPort->IntValue();
		delete vtor_aLocalPort;
		}
	//
	const CChatParameter* param_aRemotePort = aMethodInvocation.GetParameter(KARemotePort());
	TInt int_aRemotePort = KErrNotFound;
	if (param_aRemotePort != NULL)
		{
		CChatTIntValidator* vtor_aRemotePort = CChatTIntValidator::NewL(*param_aRemotePort);
		int_aRemotePort = vtor_aRemotePort->IntValue();
		delete vtor_aRemotePort;
		}
	//
	const CChatParameter* param_aServiceNum = aMethodInvocation.GetParameter(KAServiceNum());
	TInt int_aServiceNum = KErrNotFound;
	if (param_aServiceNum != NULL)
		{
		CChatTIntValidator* vtor_aServiceNum = CChatTIntValidator::NewL(*param_aServiceNum);
		int_aServiceNum = vtor_aServiceNum->IntValue();
		delete vtor_aServiceNum;
		}
	//
	const CChatParameter* param_aSessionType = aMethodInvocation.GetParameter(KASessionType());
	RStringF str_aSessionType;
	if (param_aSessionType)
		{
		CChatTDesC8Validator* vtor_aSessionType = CChatTDesC8Validator::NewL(*param_aSessionType);
		CleanupStack::PushL(vtor_aSessionType);
		str_aSessionType = iStrP.OpenFStringL(vtor_aSessionType->DesC8Value());
		CleanupStack::PopAndDestroy(vtor_aSessionType);
		CleanupClosePushL(str_aSessionType);
		++numStringsOpened;
		}
	//
	const CChatParameter* param_aIsSecure = aMethodInvocation.GetParameter(KAIsSecure());
	RStringF str_aIsSecure;
	if (param_aIsSecure)
		{
		CChatTBoolValidator* vtor_aIsSecure = CChatTBoolValidator::NewL(*param_aIsSecure);
		CleanupStack::PushL(vtor_aIsSecure);
		str_aIsSecure = iStrP.StringF(vtor_aIsSecure->BooleanValue()?HTTP::EWspUseWtls:HTTP::EWspDoNotUseWtls, RHTTPSession::GetTable());
		CleanupStack::PopAndDestroy(vtor_aIsSecure);
		}
	//
	const CChatParameter* param_aTimeOut = aMethodInvocation.GetParameter(KATimeOut());
	TInt int_aTimeOut = KErrNotFound;
	if (param_aTimeOut != NULL)
		{
		CChatTIntValidator* vtor_aTimeOut = CChatTIntValidator::NewL(*param_aTimeOut);
		int_aTimeOut = vtor_aTimeOut->IntValue();
		delete vtor_aTimeOut;
		}

	// Invoke the method
	iEventDriver->SetConnectionPropertiesL(str_aProxyAddress, str_aBearer, int_aLocalPort, int_aRemotePort,
										   int_aServiceNum, str_aSessionType, str_aIsSecure, int_aTimeOut);

	// Close the opened strings
	CleanupStack::PopAndDestroy(numStringsOpened);
	}

void CWspScriptProcessor::Do_GetConnectionPropertiesL(CChatMethodInvocation& aMethodInvocation)
	{
	RStringF proxyAddress;
	RStringF bearer;
	TInt localPort = -1;
	TInt remotePort = -1;
	TInt serviceNum = -1;
	RStringF sessionType;
	RStringF isSecure;
	TInt timeOut = -1;

	// Invoke the method
	iEventDriver->GetConnectionProperties(proxyAddress, bearer, localPort, remotePort,
										  serviceNum, sessionType, isSecure, timeOut);

	// Create validators for the parameters that have been set.
	if (proxyAddress != RStringF())
		{
		_LIT(KProxyAddress, "aProxyAddress");
		CChatTDesC8Validator* vtor = CChatTDesC8Validator::NewL(KProxyAddress(), proxyAddress.DesC());
		CleanupStack::PushL(vtor);
		aMethodInvocation.AdoptValidator(vtor);
		CleanupStack::Pop(vtor);
		}
	if (bearer != RStringF())
		{
		_LIT(KBearer, "aBearer");
		CChatTDesC8Validator* vtor = CChatTDesC8Validator::NewL(KBearer(), bearer.DesC());
		CleanupStack::PushL(vtor);
		aMethodInvocation.AdoptValidator(vtor);
		CleanupStack::Pop(vtor);
		}
	if (localPort >= 0)
		{
		_LIT(KLocalPort, "aLocalPort");
		CChatTIntValidator* vtor = CChatTIntValidator::NewL(KLocalPort(), localPort);
		CleanupStack::PushL(vtor);
		aMethodInvocation.AdoptValidator(vtor);
		CleanupStack::Pop(vtor);
		}
	if (remotePort >= 0)
		{
		_LIT(KRemotePort, "aRemotePort");
		CChatTIntValidator* vtor = CChatTIntValidator::NewL(KRemotePort(), remotePort);
		CleanupStack::PushL(vtor);
		aMethodInvocation.AdoptValidator(vtor);
		CleanupStack::Pop(vtor);
		}
	if (serviceNum >= 0)
		{
		_LIT(KServiceNum, "aServiceNum");
		CChatTIntValidator* vtor = CChatTIntValidator::NewL(KServiceNum(), serviceNum);
		CleanupStack::PushL(vtor);
		aMethodInvocation.AdoptValidator(vtor);
		CleanupStack::Pop(vtor);
		}
	if (sessionType != RStringF())
		{
		_LIT(KSessionType, "aSessionType");
		CChatTDesC8Validator* vtor = CChatTDesC8Validator::NewL(KSessionType(), sessionType.DesC());
		CleanupStack::PushL(vtor);
		aMethodInvocation.AdoptValidator(vtor);
		CleanupStack::Pop(vtor);
		}
	if (isSecure != RStringF())
		{
		_LIT(KIsSecure, "aIsSecure");
		CChatTDesC8Validator* vtor = CChatTDesC8Validator::NewL(KIsSecure(), isSecure.DesC());
		CleanupStack::PushL(vtor);
		aMethodInvocation.AdoptValidator(vtor);
		CleanupStack::Pop(vtor);
		}
	if (timeOut >= 0)
		{
		_LIT(KTimeOut, "aTimeOut");
		CChatTIntValidator* vtor = CChatTIntValidator::NewL(KTimeOut(), timeOut);
		CleanupStack::PushL(vtor);
		aMethodInvocation.AdoptValidator(vtor);
		CleanupStack::Pop(vtor);
		}
	}

void CWspScriptProcessor::Do_OpenTransactionLL(CChatMethodInvocation& aMethodInvocation)
	{
	// RStringF aMethod
	// TUriC8 aUri
	_LIT(KAMethod, "aMethod");
	_LIT(KAUri, "aUri");
	const CChatParameter* param_aMethod = aMethodInvocation.GetParameter(KAMethod());
	__ASSERT_DEBUG(param_aMethod != NULL, TDriverPanic::Panic(TDriverPanic::ENullPointer));
	CChatTDesC8Validator* vtor_aMethod = CChatTDesC8Validator::NewL(*param_aMethod);
	CleanupStack::PushL(vtor_aMethod);

	const CChatParameter* param_aUri = aMethodInvocation.GetParameter(KAUri());
	__ASSERT_DEBUG(param_aUri != NULL, TDriverPanic::Panic(TDriverPanic::ENullPointer));
	CChatTDesC8Validator* vtor_aUri = CChatTDesC8Validator::NewL(*param_aUri);
	CleanupStack::PushL(vtor_aUri);

	const CChatParameter* stparam_retCode = aMethodInvocation.GetParameter(KReturnCode());
	__ASSERT_DEBUG(stparam_retCode != NULL, TDriverPanic::Panic(TDriverPanic::ENullPointer));
	CChatTDesC8Validator* vtor_retCode = CChatTDesC8Validator::NewL(*stparam_retCode);
	CleanupStack::PushL(vtor_retCode);

	RStringF method = iStrP.OpenFStringL(vtor_aMethod->DesC8Value());
	CleanupClosePushL(method);
	TUriParser8 uri;
	uri.Parse(vtor_aUri->DesC8Value());
	HBufC16* trId16 = HBufC16::NewL(vtor_retCode->DesC8Value().Length());	
	CleanupStack::PushL(trId16);
	trId16->Des().Copy(vtor_retCode->DesC8Value());

	// Invoke the method
	CDriverTrans* tr = iEventDriver->OpenTransactionL(method, uri, *trId16);
	CleanupStack::PopAndDestroy(5, vtor_aMethod);

	// Bundle up the returned object for the item store
	TStoredItem<CDriverTrans> item(*tr);
	CChatItemStoreValidator* vtor_retVal = CChatItemStoreValidator::NewL(KReturnCode(), item);
	CleanupStack::PushL(vtor_retVal);
	aMethodInvocation.AdoptValidator(vtor_retVal);
	CleanupStack::Pop(vtor_retVal);
	}

void CWspScriptProcessor::Do_AddRequestSessionHeaderLL(CChatMethodInvocation& aMethodInvocation)
	{
	RStringF fieldName;
	CDriverTrans::THdrValType fieldType;
	RString fieldValue;
	Do_AddHeader_PreliminariesLC(aMethodInvocation, fieldName, fieldType, fieldValue);

	// Call the method
	iEventDriver->AddRequestSessionHeaderL(fieldName, fieldType, fieldValue);
	CleanupStack::PopAndDestroy(5);
	}

void CWspScriptProcessor::Do_AddRequestSessionHeaderWithParamLL(CChatMethodInvocation& aMethodInvocation)
	{
	// Create all the parameters for the call
	RStringF fieldName;
	CDriverTrans::THdrValType fieldType;
	RString fieldValue;
	RStringF paramName;
	CDriverTrans::THdrValType paramType;
	RString paramValue;
	Do_AddHeaderWithParam_PreliminariesLC(aMethodInvocation,
										fieldName, fieldType, fieldValue,
										paramName, paramType, paramValue);

	// Call the method
	iEventDriver->AddRequestSessionHeaderWithParamL(fieldName, fieldType, fieldValue,
													paramName, paramType, paramValue);
	CleanupStack::PopAndDestroy(10);
	}

void CWspScriptProcessor::Do_AddHeaderL(CChatMethodInvocation& aMethodInvocation)
	{
	RStringF fieldName;
	CDriverTrans::THdrValType fieldType;
	RString fieldValue;
	Do_AddHeader_PreliminariesLC(aMethodInvocation, fieldName, fieldType, fieldValue);

	// Get the object that this method must be invoked on, from the item store
	const TDesC& objectId = aMethodInvocation.InvocationItem();
	TStoredItem<CDriverTrans> object;
	if (!FetchFromItemStore(objectId, object))
		TDriverPanic::Panic(TDriverPanic::EItemNotFoundInStore);

	// Call the method
	object.StoredItem().AddHeaderL(fieldName, fieldType, fieldValue);
	CleanupStack::PopAndDestroy(5);
	}

void CWspScriptProcessor::Do_AddHeaderWithParamL(CChatMethodInvocation& aMethodInvocation)
	{
	// Create all the parameters for the call
	RStringF fieldName;
	CDriverTrans::THdrValType fieldType;
	RString fieldValue;
	RStringF paramName;
	CDriverTrans::THdrValType paramType;
	RString paramValue;
	Do_AddHeaderWithParam_PreliminariesLC(aMethodInvocation,
										fieldName, fieldType, fieldValue,
										paramName, paramType, paramValue);

	// Get the object that this method must be invoked on, from the item store
	const TDesC& objectId = aMethodInvocation.InvocationItem();
	TStoredItem<CDriverTrans> object;
	if (!FetchFromItemStore(objectId, object))
		TDriverPanic::Panic(TDriverPanic::EItemNotFoundInStore);

	// Call the method
	object.StoredItem().AddHeaderWithParamL(fieldName, fieldType, fieldValue, paramName, paramType, paramValue);
	CleanupStack::PopAndDestroy(10);
	}

void CWspScriptProcessor::Do_AddHeader_PreliminariesLC(CChatMethodInvocation& aMethodInvocation,
													   RStringF& aFieldName, CDriverTrans::THdrValType& aFieldType,
													   RString& aFieldValue)
	{
	_LIT(KAFieldName, "aFieldName");
	_LIT(KAFieldType, "aFieldType");
	_LIT(KAFieldValue, "aFieldValue");

	const CChatParameter* param_aFieldName = aMethodInvocation.GetParameter(KAFieldName());
	__ASSERT_DEBUG(param_aFieldName != NULL, TDriverPanic::Panic(TDriverPanic::ENullPointer));
	CChatTDesC8Validator* vtor_aFieldName = CChatTDesC8Validator::NewL(*param_aFieldName);
	CleanupStack::PushL(vtor_aFieldName);

	const CChatParameter* param_aFieldType = aMethodInvocation.GetParameter(KAFieldType());
	__ASSERT_DEBUG(param_aFieldType != NULL, TDriverPanic::Panic(TDriverPanic::ENullPointer));
	CChatTHdrValTypeValidator* vtor_aFieldType = CChatTHdrValTypeValidator::NewL(*param_aFieldType);
	CleanupStack::PushL(vtor_aFieldType);

	const CChatParameter* param_aFieldValue = aMethodInvocation.GetParameter(KAFieldValue());
	__ASSERT_DEBUG(param_aFieldValue != NULL, TDriverPanic::Panic(TDriverPanic::ENullPointer));
	CChatTDesC8Validator* vtor_aFieldValue = CChatTDesC8Validator::NewL(*param_aFieldValue);
	CleanupStack::PushL(vtor_aFieldValue);

	aFieldName = iStrP.OpenFStringL(vtor_aFieldName->DesC8Value());
	CleanupClosePushL(aFieldName);
	aFieldType = vtor_aFieldType->TypeValue();
	aFieldValue = iStrP.OpenStringL(vtor_aFieldValue->DesC8Value());
	CleanupClosePushL(aFieldValue);
	}

void CWspScriptProcessor::Do_AddHeaderWithParam_PreliminariesLC(CChatMethodInvocation& aMethodInvocation,
																RStringF& aFieldName, CDriverTrans::THdrValType& aFieldType,
																RString& aFieldValue, RStringF& aParamName,
																CDriverTrans::THdrValType& aParamType, RString& aParamValue)
	{
	_LIT(KAFieldName, "aFieldName");
	_LIT(KAFieldType, "aFieldType");
	_LIT(KAFieldValue, "aFieldValue");
	_LIT(KAParamName, "aParamName");
	_LIT(KAParamType, "aParamType");
	_LIT(KAParamValue, "aParamValue");

	const CChatParameter* param_aFieldName = aMethodInvocation.GetParameter(KAFieldName());
	__ASSERT_DEBUG(param_aFieldName != NULL, TDriverPanic::Panic(TDriverPanic::ENullPointer));
	CChatTDesC8Validator* vtor_aFieldName = CChatTDesC8Validator::NewL(*param_aFieldName);
	CleanupStack::PushL(vtor_aFieldName);

	const CChatParameter* param_aFieldType = aMethodInvocation.GetParameter(KAFieldType());
	__ASSERT_DEBUG(param_aFieldType != NULL, TDriverPanic::Panic(TDriverPanic::ENullPointer));
	CChatTHdrValTypeValidator* vtor_aFieldType = CChatTHdrValTypeValidator::NewL(*param_aFieldType);
	CleanupStack::PushL(vtor_aFieldType);

	const CChatParameter* param_aFieldValue = aMethodInvocation.GetParameter(KAFieldValue());
	__ASSERT_DEBUG(param_aFieldValue != NULL, TDriverPanic::Panic(TDriverPanic::ENullPointer));
	CChatTDesC8Validator* vtor_aFieldValue = CChatTDesC8Validator::NewL(*param_aFieldValue);
	CleanupStack::PushL(vtor_aFieldValue);

	const CChatParameter* param_aParamName = aMethodInvocation.GetParameter(KAParamName());
	__ASSERT_DEBUG(param_aParamName != NULL, TDriverPanic::Panic(TDriverPanic::ENullPointer));
	CChatTDesC8Validator* vtor_aParamName = CChatTDesC8Validator::NewL(*param_aParamName);
	CleanupStack::PushL(vtor_aParamName);

	const CChatParameter* param_aParamType = aMethodInvocation.GetParameter(KAParamType());
	__ASSERT_DEBUG(param_aParamType != NULL, TDriverPanic::Panic(TDriverPanic::ENullPointer));
	CChatTHdrValTypeValidator* vtor_aParamType = CChatTHdrValTypeValidator::NewL(*param_aParamType);
	CleanupStack::PushL(vtor_aParamType);

	const CChatParameter* param_aParamValue = aMethodInvocation.GetParameter(KAParamValue());
	__ASSERT_DEBUG(param_aParamValue != NULL, TDriverPanic::Panic(TDriverPanic::ENullPointer));
	CChatTDesC8Validator* vtor_aParamValue = CChatTDesC8Validator::NewL(*param_aParamValue);
	CleanupStack::PushL(vtor_aParamValue);

	aFieldName = iStrP.OpenFStringL(vtor_aFieldName->DesC8Value());
	CleanupClosePushL(aFieldName);
	aFieldType = vtor_aFieldType->TypeValue();
	aFieldValue = iStrP.OpenStringL(vtor_aFieldValue->DesC8Value());
	CleanupClosePushL(aFieldValue);

	aParamName = iStrP.OpenFStringL(vtor_aParamName->DesC8Value());
	CleanupClosePushL(aParamName);
	aParamType = vtor_aParamType->TypeValue();
	aParamValue = iStrP.OpenStringL(vtor_aParamValue->DesC8Value());
	CleanupClosePushL(aParamValue);
	}

void CWspScriptProcessor::Do_AddBodyL(CChatMethodInvocation& aMethodInvocation)
	{
	// Obtain the item store name for CDriverDataSupplier parameter, in order to obtain it
	// from the item store before passing it to CDriverTrans::AddBody().
	_LIT(KABody, "aBody");
	const CChatParameter* param_aBody = aMethodInvocation.GetParameter(KABody());
	__ASSERT_DEBUG(param_aBody != NULL, TDriverPanic::Panic(TDriverPanic::ENullPointer));
	const TDesC& bodyObjId = param_aBody->ParamStringValue();
	TStoredItem<MHTTPDataSupplier> body;
	if (!FetchFromItemStore(bodyObjId, body))
		TDriverPanic::Panic(TDriverPanic::EItemNotFoundInStore);

	// Get the object that this method must be invoked on, from the item store
	const TDesC& objectId = aMethodInvocation.InvocationItem();
	TStoredItem<CDriverTrans> object;
	if (!FetchFromItemStore(objectId, object))
		TDriverPanic::Panic(TDriverPanic::EItemNotFoundInStore);

	// Call the method
	object.StoredItem().AddBody(body.StoredItem());
	}

void CWspScriptProcessor::Do_GetHeaderLL(CChatMethodInvocation& aMethodInvocation)
	{
	_LIT(KAFieldName, "aFieldName");
	_LIT(KAFieldType, "aFieldType");
	_LIT(KAFieldValue, "aFieldValue");
	_LIT(KAPart, "aPart");

	// Get the parameters
	RStringF fieldName;
	const CChatParameter* param_aFieldName = aMethodInvocation.GetParameter(KAFieldName());
	__ASSERT_DEBUG(param_aFieldName != NULL, TDriverPanic::Panic(TDriverPanic::ENullPointer));
	CChatTDesC8Validator* vtor_aFieldName = CChatTDesC8Validator::NewL(*param_aFieldName);
	CleanupStack::PushL(vtor_aFieldName);

	fieldName = iStrP.OpenFStringL(vtor_aFieldName->DesC8Value());
	CleanupClosePushL(fieldName);

	const CChatParameter* param_aPart = aMethodInvocation.GetParameter(KAPart());
	__ASSERT_DEBUG(param_aPart != NULL, TDriverPanic::Panic(TDriverPanic::ENullPointer));
	CChatTIntValidator* vtor_aPart = CChatTIntValidator::NewL(*param_aPart);
	CleanupStack::PushL(vtor_aPart);

	// Get the object that this method must be invoked on, from the item store
	CDriverTrans::THdrValType fieldType;
	RString fieldValue;
	const TDesC& objectId = aMethodInvocation.InvocationItem();
	TStoredItem<CDriverTrans> object;
	if (!FetchFromItemStore(objectId, object))
		TDriverPanic::Panic(TDriverPanic::EItemNotFoundInStore);

	// Call the method
	TBool hasHeader = object.StoredItem().GetHeaderL(fieldName, fieldType, fieldValue, vtor_aPart->IntValue());
	
	// Add the string value to the cleanup stack
	CleanupClosePushL(fieldValue);

	// Add validators for the out parameters and the return value
	CChatTHdrValTypeValidator* vtor_aFieldType = CChatTHdrValTypeValidator::NewL(KAFieldType(), fieldType);
	CleanupStack::PushL(vtor_aFieldType);
	aMethodInvocation.AdoptValidator(vtor_aFieldType);
	CleanupStack::Pop(vtor_aFieldType);

	CChatTDesC8Validator* vtor_aFieldValue = CChatTDesC8Validator::NewL(KAFieldValue(), fieldValue.DesC());
	CleanupStack::PushL(vtor_aFieldValue);
	aMethodInvocation.AdoptValidator(vtor_aFieldValue);
	CleanupStack::Pop(vtor_aFieldValue);

	CChatTBoolValidator* retVtor = CChatTBoolValidator::NewL(KReturnCode(), hasHeader);
	CleanupStack::PushL(retVtor);
	aMethodInvocation.AdoptValidator(retVtor);
	CleanupStack::Pop(retVtor);

	// Cleanup...
	CleanupStack::PopAndDestroy(4, vtor_aFieldName);
	}

void CWspScriptProcessor::Do_GetBodyL(CChatMethodInvocation& aMethodInvocation)
	{
	// Get the object that this method must be invoked on, from the item store
	const TDesC& objectId = aMethodInvocation.InvocationItem();
	TStoredItem<CDriverTrans> object;
	if (!FetchFromItemStore(objectId, object))
		TDriverPanic::Panic(TDriverPanic::EItemNotFoundInStore);

	// Call the method
	MHTTPDataSupplier& data = object.StoredItem().GetBody();

	// Store the data supplier
	TStoredItem<MHTTPDataSupplier> item(data);
	CChatItemStoreValidator* vtor = CChatItemStoreValidator::NewL(KReturnCode(), item);
	CleanupStack::PushL(vtor);
	aMethodInvocation.AdoptValidator(vtor);
	CleanupStack::Pop(vtor);
	}

void CWspScriptProcessor::Do_GetStatusCodeL(CChatMethodInvocation& aMethodInvocation)
	{
	// Get the object that this method must be invoked on, from the item store
	const TDesC& objectId = aMethodInvocation.InvocationItem();
	TStoredItem<CDriverTrans> object;
	if (!FetchFromItemStore(objectId, object))
		TDriverPanic::Panic(TDriverPanic::EItemNotFoundInStore);

	// Call the method
	TInt statusCode = object.StoredItem().GetStatusCode();

	// Create a validator for the return value
	CChatTIntValidator* vtor = CChatTIntValidator::NewL(KReturnCode(), statusCode);
	CleanupStack::PushL(vtor);
	aMethodInvocation.AdoptValidator(vtor);
	CleanupStack::Pop(vtor);
	}

void CWspScriptProcessor::Do_SubmitLL(CChatMethodInvocation& aMethodInvocation)
	{
	// Get the object that this method must be invoked on, from the item store
	const TDesC& objectId = aMethodInvocation.InvocationItem();
	TStoredItem<CDriverTrans> object;
	if (!FetchFromItemStore(objectId, object))
		TDriverPanic::Panic(TDriverPanic::EItemNotFoundInStore);

	// Call the method
	object.StoredItem().SubmitL();
	}

void CWspScriptProcessor::Do_CancelL(CChatMethodInvocation& aMethodInvocation)
	{
	// Get the object that this method must be invoked on, from the item store
	const TDesC& objectId = aMethodInvocation.InvocationItem();
	TStoredItem<CDriverTrans> object;
	if (!FetchFromItemStore(objectId, object))
		TDriverPanic::Panic(TDriverPanic::EItemNotFoundInStore);

	// Call the method
	object.StoredItem().Cancel();
	}

void CWspScriptProcessor::Do_CloseL(CChatMethodInvocation& aMethodInvocation)
	{
	// Get the object that this method must be invoked on, from the item store
	const TDesC& objectId = aMethodInvocation.InvocationItem();
	TStoredItem<CDriverTrans> object;
	if (!FetchFromItemStore(objectId, object))
		TDriverPanic::Panic(TDriverPanic::EItemNotFoundInStore);

	// Call the method
	object.StoredItem().Close();
	}

void CWspScriptProcessor::Do_MoreRequestBodyDataL(CChatMethodInvocation& aMethodInvocation)
	{
	// Get the object that this method must be invoked on, from the item store
	const TDesC& objectId = aMethodInvocation.InvocationItem();
	TStoredItem<CDriverTrans> object;
	if (!FetchFromItemStore(objectId, object))
		TDriverPanic::Panic(TDriverPanic::EItemNotFoundInStore);

	// Call the method
	object.StoredItem().MoreRequestBodyDataL();
	}

void CWspScriptProcessor::Do_DriverDataSupplier_OverallDataSizeL(CChatMethodInvocation& aMethodInvocation)
	{
	// Get the object that this method must be invoked on, from the item store
	const TDesC& objectId = aMethodInvocation.InvocationItem();

	TStoredItem<MHTTPDataSupplier> object;
	if (!FetchFromItemStore(objectId, object))
		TDriverPanic::Panic(TDriverPanic::EItemNotFoundInStore);

	// Call the method
	TInt size = object.StoredItem().OverallDataSize();

	// Add validators for the return value
	CChatTIntValidator* sizeVtor = CChatTIntValidator::NewL(KReturnCode(), size);
	CleanupStack::PushL(sizeVtor);
	aMethodInvocation.AdoptValidator(sizeVtor);
	CleanupStack::Pop(sizeVtor);
	}

void CWspScriptProcessor::Do_DriverDataSupplier_GetNextDataPartL(CChatMethodInvocation& aMethodInvocation)
	{
	// Get the object that this method must be invoked on, from the item store
	const TDesC& objectId = aMethodInvocation.InvocationItem();

	TStoredItem<MHTTPDataSupplier> object;
	if (!FetchFromItemStore(objectId, object))
		TDriverPanic::Panic(TDriverPanic::EItemNotFoundInStore);

	// Call the method
	TPtrC8 buffer;
	TBool ret = object.StoredItem().GetNextDataPart(buffer);

	// Add validators for the return and out parameters
	_LIT(KABuffer, "aDataPart");
	CChatTDesC8Validator* buffVtor = CChatTDesC8Validator::NewL(KABuffer(), buffer);
	CleanupStack::PushL(buffVtor);
	aMethodInvocation.AdoptValidator(buffVtor);
	CleanupStack::Pop(buffVtor);
	CChatTBoolValidator* retVtor = CChatTBoolValidator::NewL(KReturnCode(), ret);
	CleanupStack::PushL(retVtor);
	aMethodInvocation.AdoptValidator(retVtor);
	CleanupStack::Pop(retVtor);
	}

void CWspScriptProcessor::Do_DriverDataSupplier_ReleaseDataL(CChatMethodInvocation& aMethodInvocation)
	{
	// Get the object that this method must be invoked on, from the item store
	const TDesC& objectId = aMethodInvocation.InvocationItem();

	TStoredItem<MHTTPDataSupplier> object;
	if (!FetchFromItemStore(objectId, object))
		TDriverPanic::Panic(TDriverPanic::EItemNotFoundInStore);

	// Call the method
	object.StoredItem().ReleaseData();
	}

void CWspScriptProcessor::Do_DriverDataSupplier_ResetL(CChatMethodInvocation& aMethodInvocation)
	{
	// Get the object that this method must be invoked on, from the item store
	const TDesC& objectId = aMethodInvocation.InvocationItem();

	TStoredItem<MHTTPDataSupplier> object;
	if (!FetchFromItemStore(objectId, object))
		TDriverPanic::Panic(TDriverPanic::EItemNotFoundInStore);

	// Call the method
	object.StoredItem().Reset();
	}


/** Implementation of CDriverScriptStarter - an active object utility used by the WSP Script Processor
	to read the driver .ini file and start each new script in a new chat engine.
*/


CDriverScriptStarter* CDriverScriptStarter::NewL(CWspScriptProcessor& aDriver)
	{
	CDriverScriptStarter* me = new(ELeave)CDriverScriptStarter(aDriver);
	CleanupStack::PushL(me);
	me->ConstructL();
	CleanupStack::Pop(me);
	return me;
	}

CDriverScriptStarter::~CDriverScriptStarter()
	{
	}

void CDriverScriptStarter::StartNextScript()
	{
	// First, identify the section from the .ini file that the next script is in
	CArrayPtrFlat<CScriptSection>& sections = iDriver.iDriverIniFile.Sections();

	// Find from the previous position (iSectionIdx) the next section marked [Test]
	TBool found = EFalse;
	iSection = NULL;
	while (!found && (iSectionIdx < sections.Count()))
		{
		// Check the title for a match with [Test]
		iSection = sections[iSectionIdx];
		if (iSection ->SectionName().CompareF(KTestSectionName) == 0)
			found = ETrue;

		++iSectionIdx;
		}
	if (found)
		{
		// Determine whether we are leak testing, and if so, how many iterations
		iDriver.iLeakTests = iSection->ItemValue(KLeakTestCount(), 0); // do not leak test by default
		iDriver.iFirstLeakIteration = iSection->ItemValue(KLeakTestFirstIter(), 0); // start at the first by default
		
		// Kick off in the next AO iteration
		TRequestStatus* stat = &iStatus;
		User::RequestComplete(stat, KErrNone);
		SetActive();
		}
	else // finished the scripts - stop the scheduler
		CActiveScheduler::Stop();
	}

void CDriverScriptStarter::RunL()
	{
	// Either do leak tests or regular tests
	TInt err = KErrNone;
	if (iDriver.iLeakTests > 0)
		{
		TRAP(err, DoLeakTestsRunL());
		}
	else
		{
		TRAP(err,DoRunL());
		}

	switch(err)
		{
	case KErrNone:
		{
		iDriver.iTestUtils.LogIt(_L("\nTest case finished"));
		}
		break;
	default:// any error
		{
		iDriver.iTestUtils.LogIt(_L("Test case failed (caught a Leave) ... skipping to the next test case"));
		}
		break;
		}
	}

void CDriverScriptStarter::DoRunL()
	{
	__ASSERT_DEBUG(iSection != NULL, TDriverPanic::Panic(TDriverPanic::ENullPointer));

	// Reset the protocol handler for the next test
	User::LeaveIfError(iDriver.iEventDriver->Reset());

	// Get the script name and (re-)create the chat engine
	const TDesC& scriptFileName = iSection->ItemValue(KScriptFile(), KNullDesC()); 
	__ASSERT_DEBUG(scriptFileName.Length() > 0, TDriverPanic::Panic(TDriverPanic::EInvalidScriptFilename));

	// Read the location for the 'chat' script logfile and the initial logging level
	const TDesC& logFileCompName = iDriver.iDriverIniFile.ItemValue(KDefaultSectionName(), KChatLogsDir(), _L("chat"));
	TInt loggingLevel = iDriver.iDriverIniFile.ItemValue(KDefaultSectionName(), KLogLevel(), 2);

	// Log test start, unless we're doing leak testing - in which case the HTTPTestUtils
	// change the memory state
	if (iDriver.iLeakTests == 0)
		iDriver.iTestUtils.StartTestL(scriptFileName);

	// Create chat engine used to validate API calls and make callbacks, and set its default logging level
	iDriver.iChatEngine = CChatEngine::NewL(scriptFileName, logFileCompName, (CChatEngine::TLogLevel)loggingLevel,
								iDriver.iFileSrv, iDriver.iLogger, iDriver, iDriver, iDriver);
	CleanupStack::PushL(iDriver.iChatEngine);

	// Kick off the script
	iDriver.iChatEngine->Start();

	// Run scheduler until the script is completed or it has left with an error
	iDriver.iFailureError = KErrNone;
	CActiveScheduler::Start();

	// If there was a failure detected, then leave
	if (iDriver.iFailureError)
		User::Leave(iDriver.iFailureError);

	CleanupStack::PopAndDestroy(iDriver.iChatEngine);
	iDriver.iChatEngine = NULL;

	// Advance to the next test script, and start the (new) chat engine again
	StartNextScript();
	}

void CDriverScriptStarter::DoLeakTestsRunL()
	{
	TInt count = iDriver.iFirstLeakIteration;
	TBool done = EFalse;
	while( !done )
		{
		iDriver.iTestUtils.LogIt(_L("Memory Leak Testing on allocation %d\n"), count);

		__UHEAP_MARK;
		__UHEAP_FAILNEXT(count);
		TRAPD(err,DoRunL());
		__UHEAP_RESET;

		// Reset the event driver
		TInt err2 = iDriver.iEventDriver->Reset();
		if (err == KErrNone)
			err = err2;

		__UHEAP_MARKEND;
		User::Heap().Check();

		// Break out if the test passes successfully; allow only memory failure or test failure codes to proceed.
		if (err == KErrNone)
			done = ETrue;
		else if (err < 0 && err != KErrNoMemory)
			User::Leave(err);
		else
			++count;
		}
	}

TInt CDriverScriptStarter::RunError(TInt /*aError*/)
	{
	// Not much we can do - ignore
	return KErrNone;
	}

void CDriverScriptStarter::DoCancel()
	{
	// Not much we can do - ignore
	}

CDriverScriptStarter::CDriverScriptStarter(CWspScriptProcessor& aDriver)
	: CActive(CActive::EPriorityStandard), iDriver(aDriver)
	{
	CActiveScheduler::Add(this);
	}

void CDriverScriptStarter::ConstructL()
	{
	}

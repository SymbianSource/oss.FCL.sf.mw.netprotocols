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
// System includes
// 
//

// Local includes
//
#include "cchatengine.h"
#include "cchatmethodinvocation.h"
#include "cchattdesc8validator.h"
#include "cchattboolvalidator.h"
#include "cchattintvalidator.h"
#include "t_wspeventdriver/tdriverpanic.h"

// Class signatures
//
#include "cdriverdatasupplier.h"


// Constants used in this file
//


/** Implementation of class 'CDriverDataSupplier'.  This class validates TWspReason parameter values,
	in method invocations from the unit under test.
*/


EXPORT_C
CDriverDataSupplier* CDriverDataSupplier::NewL(CChatEngine& aChatEngine)
	{
	CDriverDataSupplier* me = new(ELeave)CDriverDataSupplier(aChatEngine);
	CleanupStack::PushL(me);
	me->ConstructL();
	CleanupStack::Pop(me);
	return me;
	}

CDriverDataSupplier::~CDriverDataSupplier()
	{
	delete iBuffer;
	}

EXPORT_C
void CDriverDataSupplier::CleanUp(TAny* aDataSupplierItem)
	{
	delete STATIC_CAST(CDriverDataSupplier*, aDataSupplierItem);
	}

TBool CDriverDataSupplier::GetNextDataPart(TPtrC8& aDataPart)
	{
	TBool retVal = EFalse;
	TRAPD(err, retVal = GetNextDataPartL(aDataPart))
	if(err!=KErrNone)
		TDriverPanic::Panic(TDriverPanic::EMemoryAllocError);
		
	return retVal;
	}

void CDriverDataSupplier::ReleaseData()
	{
	TRAPD(err, ReleaseDataL())
	if(err!=KErrNone)
		TDriverPanic::Panic(TDriverPanic::EMemoryAllocError);
	}

TInt CDriverDataSupplier::OverallDataSize()
	{
	TInt retVal = KErrNone;
	TRAPD(err, retVal = OverallDataSizeL())
	if(err!=KErrNone)
		TDriverPanic::Panic(TDriverPanic::EMemoryAllocError);
		
	return retVal;
	}

TInt CDriverDataSupplier::Reset()
	{
	TInt retVal = KErrNone;
	TRAPD(err, retVal = ResetL())
	if(err!=KErrNone)
		TDriverPanic::Panic(TDriverPanic::EMemoryAllocError);
		
	return retVal;
	}

TBool CDriverDataSupplier::GetNextDataPartL(TPtrC8& aDataPart)
	{
	// represent this method invocation
	_LIT(KThisMethod, "MHTTPDataSupplier::GetNextDataPart");
	CChatMethodInvocation* methodCalled = CChatMethodInvocation::NewL(iChatEngine, KThisMethod());
	CleanupStack::PushL(methodCalled);

	// test the invocation against what's expected by the chat script
	iChatEngine.TestExpectation(*methodCalled);

	// get parameters for the 'out' parameter and the return code
	_LIT(KParamADataPart, "aDataPart");
	const CChatParameter* param_aDataPart = methodCalled->GetParameter(KParamADataPart());
	if (!param_aDataPart)
		{
		iChatEngine.WriteComment(CChatEngine::ELogTerse, _L("PANIC: Invocation on MHTTPDataSupplier::GetNextDataPart() failed because required parameter 'aDataPart' was not provided in the script."));
		TDriverPanic::Panic(TDriverPanic::EParameterMissingInScript);
		}

	_LIT(KReturnCode, ":RETURN:");
	const CChatParameter* param_retCode = methodCalled->GetParameter(KReturnCode());
	if (!param_retCode)
		{
		iChatEngine.WriteComment(CChatEngine::ELogTerse, _L("PANIC: Invocation on MHTTPDataSupplier::GetNextDataPart() failed because required parameter ':RETURN:' was not provided in the script."));
		TDriverPanic::Panic(TDriverPanic::EParameterMissingInScript);
		}

	// create validators for these parameters in order to convert them
	CChatTDesC8Validator* vtor_aDataPart = CChatTDesC8Validator::NewL(*param_aDataPart);
	CleanupStack::PushL(vtor_aDataPart);
	CChatTBoolValidator* vtor_retCode = CChatTBoolValidator::NewL(*param_retCode);
	CleanupStack::PushL(vtor_retCode);

	// Keep a copy of the out parameter and keep the return code
	iBuffer = vtor_aDataPart->DesC8Value().AllocL();
	aDataPart.Set(*iBuffer);
	TBool retVal = vtor_retCode->BooleanValue();

	// clean up unwanted validators
	CleanupStack::PopAndDestroy(3, methodCalled);

	return retVal;
	}
	
void CDriverDataSupplier::ReleaseDataL()
	{
	// represent this method invocation
	_LIT(KThisMethod, "MHTTPDataSupplier::ReleaseData");
	CChatMethodInvocation* methodCalled = CChatMethodInvocation::NewL(iChatEngine, KThisMethod());

	// test the invocation against what's expected by the chat script
	iChatEngine.TestExpectation(*methodCalled);

	// release the buffer we held in GetNextDataPart, and clean up
	delete iBuffer;
	iBuffer = NULL;
	delete methodCalled;
	}
	
TInt CDriverDataSupplier::OverallDataSizeL()
	{
	// represent this method invocation
	_LIT(KThisMethod, "MHTTPDataSupplier::OverallDataSize");
	CChatMethodInvocation* methodCalled = CChatMethodInvocation::NewL(iChatEngine, KThisMethod());
	CleanupStack::PushL(methodCalled);
	// test the invocation against what's expected by the chat script
	iChatEngine.TestExpectation(*methodCalled);

	// get parameters for the 'out' parameter and the return code
	_LIT(KReturnCode, ":RETURN:");
	const CChatParameter* param_retCode = methodCalled->GetParameter(KReturnCode());
	if (!param_retCode)
		{
		iChatEngine.WriteComment(CChatEngine::ELogTerse, _L("PANIC: Invocation on MHTTPDataSupplier::OverallDataSize() failed because required parameter ':RETURN:' was not provided in the script."));
		TDriverPanic::Panic(TDriverPanic::EParameterMissingInScript);
		}

	// create validators for these parameters in order to convert them
	CChatTIntValidator* vtor_retCode = CChatTIntValidator::NewL(*param_retCode);
	CleanupStack::PushL(vtor_retCode);

	// Keep the return code
	TInt retVal = vtor_retCode->IntValue();

	// clean up unwanted validators
	CleanupStack::PopAndDestroy(vtor_retCode);
	CleanupStack::PopAndDestroy(methodCalled);
	return retVal;
	}
	
TInt CDriverDataSupplier::ResetL()
	{
	// represent this method invocation
	_LIT(KThisMethod, "MHTTPDataSupplier::Reset");
	CChatMethodInvocation* methodCalled = CChatMethodInvocation::NewL(iChatEngine, KThisMethod());
	CleanupStack::PushL(methodCalled);

	// test the invocation against what's expected by the chat script
	iChatEngine.TestExpectation(*methodCalled);

	// get parameters for the 'out' parameter and the return code
	_LIT(KReturnCode, ":RETURN:");
	const CChatParameter* param_retCode = methodCalled->GetParameter(KReturnCode());
	if (!param_retCode)
		{
		iChatEngine.WriteComment(CChatEngine::ELogTerse, _L("PANIC: Invocation on MHTTPDataSupplier::Reset() failed because required parameter ':RETURN:' was not provided in the script."));
		TDriverPanic::Panic(TDriverPanic::EParameterMissingInScript);
		}

	// create validators for these parameters in order to convert them
	CChatTIntValidator* vtor_retCode = CChatTIntValidator::NewL(*param_retCode);
	CleanupStack::PushL(vtor_retCode);

	// Keep the return code
	TInt retVal = vtor_retCode->IntValue();

	// clean up unwanted validators
	CleanupStack::PopAndDestroy(2, methodCalled);
	return retVal;
	}

CDriverDataSupplier::CDriverDataSupplier(CChatEngine& aChatEngine)
	: iChatEngine(aChatEngine)
	{
	}

void CDriverDataSupplier::ConstructL()
	{
	}



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
#include "t_wspeventdriver/tdriverpanic.h"

// Class signatures
//
#include "drivercustomvalidators.h"


// Constants used in this file
//
_LIT(KEAll, "EAll");
_LIT(KEAnyTransactionEvent, "EAnyTransactionEvent");
_LIT(KEAnySessionEvent, "EAnySessionEvent");
_LIT(KESubmit, "ESubmit");
_LIT(KECancel, "ECancel");
_LIT(KENotifyNewRequestBodyPart, "ENotifyNewRequestBodyPart");
_LIT(KEClosed, "EClosed");
_LIT(KEGotResponseHeaders, "EGotResponseHeaders");
_LIT(KEGotResponseBodyData, "EGotResponseBodyData");
_LIT(KEResponseComplete, "EResponseComplete");
_LIT(KESucceeded, "ESucceeded");
_LIT(KEFailed, "EFailed");
_LIT(KEUnrecoverableError, "EUnrecoverableError");
_LIT(KETooMuchRequestData, "ETooMuchRequestData");
_LIT(KEGotResponseTrailerHeaders, "EGotResponseTrailerHeaders");
_LIT(KEConnect, "EConnect");
_LIT(KEDisconnect, "EDisconnect");
_LIT(KEConnectedOK, "EConnectedOK");
_LIT(KEConnectedWithReducedCapabilities, "EConnectedWithReducedCapabilities");
_LIT(KEDisconnected, "EDisconnected");
_LIT(KEAuthenticatedOK, "EAuthenticatedOK");
_LIT(KEAuthenticationFailuer, "EAuthenticationFailuer");
_LIT(KEConnectionTimedOut, "EConnectionTimedOut");
_LIT(KERedirectedPermanently, "ERedirectedPermanently");
_LIT(KERedirectedTemporarily, "ERedirectedTemporarily");
_LIT(KEMoreDataReceivedThanExpected, "EMoreDataReceivedThanExpected");
_LIT(KENotConnected, "ENotConnected");
_LIT(KEExceptionInfo, "EExceptionInfo");
_LIT(KERedirected, "ERedirected");
_LIT(KEAlreadyConnecting, "EAlreadyConnecting");
_LIT(KEAlreadyConnected, "EAlreadyConnected");
_LIT(KEAlreadyDisconnecting, "EAlreadyDisconnecting");
_LIT(KEAlreadyDisconnected, "EAlreadyDisconnected");
_LIT(KETInt, "TInt");
_LIT(KERString, "RString");
_LIT(KERStringF, "RStringF");
_LIT(KETDateTime, "TDateTime");


/** Implementation of class 'CChatTHTTPEventValidator'.  This class validates THTTPEvent parameter values
	in method invocations from the unit under test.
*/

CChatTHTTPEventValidator* CChatTHTTPEventValidator::NewL(const TDesC& aParamName, THTTPEvent aEvent)
	{
	CChatTHTTPEventValidator* me = new(ELeave)CChatTHTTPEventValidator(aEvent);
	CleanupStack::PushL(me);
	me->ConstructL(aParamName);
	CleanupStack::Pop(me);
	return me;
	}

CChatTHTTPEventValidator* CChatTHTTPEventValidator::NewL(const CChatParameter& aScriptParam)
	{
	CChatTHTTPEventValidator* me = new(ELeave)CChatTHTTPEventValidator();
	CleanupStack::PushL(me);
	me->ConstructL(aScriptParam);
	CleanupStack::Pop(me);
	return me;
	}

CChatTHTTPEventValidator::~CChatTHTTPEventValidator()
	{
	}
const TDesC& CChatTHTTPEventValidator::ClassName() const
	{
	_LIT(KMyClassName, "CChatTHTTPEventValidator");
	return KMyClassName();
	}

TBool CChatTHTTPEventValidator::IsValid(const CChatParameter& aParamToCompare, TDes& aErrorMessage) const
	{
	THTTPEvent paramVal = CChatTHTTPEventValidator::Convert(aParamToCompare);
	if (iEventVal != paramVal)
		{
		_LIT(KErrMsg, "Expected value of %d; Driver received value of %d.");
		aErrorMessage.Format(KErrMsg(), paramVal.iStatus, iEventVal.iStatus);
		}
	return (iEventVal.iStatus == paramVal.iStatus);
	}

THTTPEvent CChatTHTTPEventValidator::EventValue() const
	{
	return iEventVal;
	}

CChatTHTTPEventValidator::CChatTHTTPEventValidator(THTTPEvent aEvent)
	: CChatValidator(), iEventVal(aEvent)
	{
	}

CChatTHTTPEventValidator::CChatTHTTPEventValidator()
	: CChatValidator()
	{
	}

void CChatTHTTPEventValidator::ConstructL(const TDesC& aParamName)
	{
	CChatValidator::ConstructL(aParamName);
	}

void CChatTHTTPEventValidator::ConstructL(const CChatParameter& aScriptParam)
	{
	CChatValidator::ConstructL(aScriptParam.ParamName());
	iEventVal.iStatus = Convert(aScriptParam);
	}

TInt CChatTHTTPEventValidator::Convert(const CChatParameter& aParamToConvert)
	{
	TInt retVal = THTTPEvent::EAll;
	const TDesC& paramValStr = aParamToConvert.ParamStringValue();
	if (!paramValStr.Compare(KEAll()))
		retVal = THTTPEvent::EAll;
	else if (!paramValStr.Compare(KEAnyTransactionEvent()))
		retVal = THTTPEvent::EAnyTransactionEvent;
	else if (!paramValStr.Compare(KEAnySessionEvent()))
		retVal = THTTPEvent::EAnySessionEvent;
	else if (!paramValStr.Compare(KESubmit()))
		retVal = THTTPEvent::ESubmit;
	else if (!paramValStr.Compare(KECancel()))
		retVal = THTTPEvent::ECancel;
	else if (!paramValStr.Compare(KENotifyNewRequestBodyPart()))
		retVal = THTTPEvent::ENotifyNewRequestBodyPart;
	else if (!paramValStr.Compare(KEClosed()))
		retVal = THTTPEvent::EClosed;
	else if (!paramValStr.Compare(KEGotResponseHeaders()))
		retVal = THTTPEvent::EGotResponseHeaders;
	else if (!paramValStr.Compare(KEGotResponseBodyData()))
		retVal = THTTPEvent::EGotResponseBodyData;
	else if (!paramValStr.Compare(KEResponseComplete()))
		retVal = THTTPEvent::EResponseComplete;
	else if (!paramValStr.Compare(KESucceeded()))
		retVal = THTTPEvent::ESucceeded;
	else if (!paramValStr.Compare(KEFailed()))
		retVal = THTTPEvent::EFailed;
	else if (!paramValStr.Compare(KEUnrecoverableError()))
		retVal = THTTPEvent::EUnrecoverableError;
	else if (!paramValStr.Compare(KETooMuchRequestData()))
		retVal = THTTPEvent::ETooMuchRequestData;
	else if (!paramValStr.Compare(KEGotResponseTrailerHeaders()))
		retVal = THTTPEvent::EGotResponseTrailerHeaders;	
	else if (!paramValStr.Compare(KEConnect()))
		retVal = THTTPSessionEvent::EConnect;
	else if (!paramValStr.Compare(KEDisconnect()))
		retVal = THTTPSessionEvent::EDisconnect;
	else if (!paramValStr.Compare(KEConnectedOK()))
		retVal = THTTPSessionEvent::EConnectedOK;
	else if (!paramValStr.Compare(KEConnectedWithReducedCapabilities()))
		retVal = THTTPSessionEvent::EConnectedWithReducedCapabilities;
	else if (!paramValStr.Compare(KEDisconnected()))
		retVal = THTTPSessionEvent::EDisconnected;
	else if (!paramValStr.Compare(KEAuthenticatedOK()))
		retVal = THTTPSessionEvent::EAuthenticatedOK;
	else if (!paramValStr.Compare(KEAuthenticationFailuer()))
		retVal = THTTPSessionEvent::EAuthenticationFailure;
	else if (!paramValStr.Compare(KEConnectionTimedOut()))
		retVal = THTTPSessionEvent::EConnectionTimedOut;
	else if (!paramValStr.Compare(KERedirectedPermanently()))
		retVal = THTTPEvent::ERedirectedPermanently;
	else if (!paramValStr.Compare(KERedirectedTemporarily()))
		retVal = THTTPEvent::ERedirectedTemporarily;
	else if (!paramValStr.Compare(KEMoreDataReceivedThanExpected()))
		retVal = THTTPEvent::EMoreDataReceivedThanExpected;
	else if (!paramValStr.Compare(KENotConnected()))
		retVal = THTTPSessionEvent::ENotConnected;
	else if (!paramValStr.Compare(KEExceptionInfo()))
		retVal = THTTPSessionEvent::EExceptionInfo;
	else if (!paramValStr.Compare(KERedirected()))
		retVal = THTTPSessionEvent::ERedirected;
	else if (!paramValStr.Compare(KEAlreadyConnecting()))
		retVal = THTTPSessionEvent::EAlreadyConnecting;
	else if (!paramValStr.Compare(KEAlreadyConnected()))
		retVal = THTTPSessionEvent::EAlreadyConnected;
	else if (!paramValStr.Compare(KEAlreadyDisconnecting()))
		retVal = THTTPSessionEvent::EAlreadyDisconnecting;
	else if (!paramValStr.Compare(KEAlreadyDisconnected()))
		retVal = THTTPSessionEvent::EAlreadyDisconnected;
	else // perhaps its an integer event value
		retVal = ConvertInteger(aParamToConvert);

	return retVal;
	}

TInt CChatTHTTPEventValidator::ConvertInteger(const CChatParameter& aParamToConvert)
	{
	const TDesC& paramValStr = aParamToConvert.ParamStringValue();
	TLex lexer(paramValStr);
	TInt paramVal = 0;
	if (lexer.Val(paramVal) != KErrNone)
		TDriverPanic::Panic(TDriverPanic::EBadIntegerValueInScript);
	return paramVal;
	}

/** Implementation of class 'CChatTHdrValTypeValidator'.  This class validates THdrValType parameter values
	in method invocations from the unit under test.
*/

CChatTHdrValTypeValidator* CChatTHdrValTypeValidator::NewL(const TDesC& aParamName, CDriverTrans::THdrValType aType)
	{
	CChatTHdrValTypeValidator* me = new(ELeave)CChatTHdrValTypeValidator(aType);
	CleanupStack::PushL(me);
	me->ConstructL(aParamName);
	CleanupStack::Pop(me);
	return me;
	}

CChatTHdrValTypeValidator* CChatTHdrValTypeValidator::NewL(const CChatParameter& aScriptParam)
	{
	CChatTHdrValTypeValidator* me = new(ELeave)CChatTHdrValTypeValidator();
	CleanupStack::PushL(me);
	me->ConstructL(aScriptParam);
	CleanupStack::Pop(me);
	return me;
	}

CChatTHdrValTypeValidator::~CChatTHdrValTypeValidator()
	{
	}
const TDesC& CChatTHdrValTypeValidator::ClassName() const
	{
	_LIT(KMyClassName, "CChatTHdrValTypeValidator");
	return KMyClassName();
	}

TBool CChatTHdrValTypeValidator::IsValid(const CChatParameter& aParamToCompare, TDes& aErrorMessage) const
	{
	CDriverTrans::THdrValType paramVal = CChatTHdrValTypeValidator::Convert(aParamToCompare);
	if (iType != paramVal)
		{
		_LIT(KErrMsg, "Expected value of %d; Driver received value of %d.");
		aErrorMessage.Format(KErrMsg(), paramVal, iType);
		}
	return (iType == paramVal);
	}

CDriverTrans::THdrValType CChatTHdrValTypeValidator::TypeValue() const
	{
	return iType;
	}

CChatTHdrValTypeValidator::CChatTHdrValTypeValidator(CDriverTrans::THdrValType aType)
	: CChatValidator(), iType(aType)
	{
	}

CChatTHdrValTypeValidator::CChatTHdrValTypeValidator()
	: CChatValidator()
	{
	}

void CChatTHdrValTypeValidator::ConstructL(const TDesC& aParamName)
	{
	CChatValidator::ConstructL(aParamName);
	}

void CChatTHdrValTypeValidator::ConstructL(const CChatParameter& aScriptParam)
	{
	CChatValidator::ConstructL(aScriptParam.ParamName());
	iType = Convert(aScriptParam);
	}

CDriverTrans::THdrValType CChatTHdrValTypeValidator::Convert(const CChatParameter& aParamToConvert)
	{
	CDriverTrans::THdrValType retVal = CDriverTrans::ETInt;
	const TDesC& paramValStr = aParamToConvert.ParamStringValue();
	if (!paramValStr.Compare(KETInt()))
		retVal = CDriverTrans::ETInt;
	else if (!paramValStr.Compare(KERString()))
		retVal = CDriverTrans::ERString;
	else if (!paramValStr.Compare(KERStringF()))
		retVal = CDriverTrans::ERStringF;
	else if (!paramValStr.Compare(KETDateTime()))
		retVal = CDriverTrans::ETDateTime;
	else
		TDriverPanic::Panic(TDriverPanic::EBadTHdrValTypeInScript);
	return retVal;
	}

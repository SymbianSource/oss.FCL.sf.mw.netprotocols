// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "cpipeliningconfigtest.h"
#include <uri8.h>

CPipeliningConfigTest* CPipeliningConfigTest::NewLC(const TDesC& aTestName)
	{
	CPipeliningConfigTest* me = new (ELeave) CPipeliningConfigTest();
	CleanupStack::PushL(me);
	me->ConstructL(aTestName);
	return me;
	}
	
CPipeliningConfigTest::CPipeliningConfigTest()
	{
	}	

void CPipeliningConfigTest::ConstructL(const TDesC& aTestName)
	{
	iTestName=aTestName.AllocL();
	}

CPipeliningConfigTest::~CPipeliningConfigTest()
	{
	iTransactionUris.ResetAndDestroy();
	delete iTestName;
	}



const TDesC& CPipeliningConfigTest::TestCaseName() const
	{
	return *iTestName;
	}
	
	
TInt CPipeliningConfigTest::TotalTransactionCount() const
	{
	return iTransactionUris.Count();
	}
	
	
TInt CPipeliningConfigTest::TransactionCount(TInt /*aConnectionIndex*/) const
	{
	return 0;
	}
	
	
RHTTPTransaction CPipeliningConfigTest::GetTransactionL(TInt aIndex, 
														RHTTPSession aSession, 
											 			MHTTPTransactionCallback& aClient)
	{
	
	if (aIndex >= iTransactionUris.Count())
		{
		User::Invariant();
		}
	
	CheckAndSetSessionSettingsL(aSession);
	
	RStringF method = aSession.StringPool().StringF(HTTP::EGET,RHTTPSession::GetTable());
	TUriParser8 uri; 
	uri.Parse(*(iTransactionUris[aIndex]));
	return aSession.OpenTransactionL(uri, aClient, method);
	}
	
	
TBool CPipeliningConfigTest::BatchTransactions() const
	{
	return ETrue;
	}
	

TInt CPipeliningConfigTest::ExpectedError(RHTTPTransaction /*aTrans*/)
	{
	return KErrNone;
	}
	
	
const TDesC8& CPipeliningConfigTest::GetRawRequest(TInt /*aConnectionIndex*/, 
												   TInt /*aTransIndex*/)
	{
	User::Invariant();
	return KNullDesC8();
	}
	
	
const TDesC8& CPipeliningConfigTest::GetRawResponse(TInt /*aConnectionIndex*/, 
													TInt /*aTransIndex*/)
	{
	User::Invariant();
	return KNullDesC8();
	}
	
	
TInt CPipeliningConfigTest::ConnectionCount() const
	{
	return 0;
	}
	
TInt CPipeliningConfigTest::RealExpectedConnectionCount() const
	{
	return iExpectedNumberConnections;
	}
	

TBool CPipeliningConfigTest::EnableBatching() const
	{
	return ETrue;
	}	
	
	
void CPipeliningConfigTest::ProcessHeadersL(RHTTPTransaction /*aTrans*/)
	{
	}
	
	
void CPipeliningConfigTest::CheckAndSetSessionSettingsL(RHTTPSession aSession)	
	{
	RStringPool stringPool = aSession.StringPool();
	RHTTPConnectionInfo sessionSettings = aSession.ConnectionInfo();
	
	if (iMaxNumberTransactionsToPipeline > 0)
		{
		RStringF maxToPipelineSetting = stringPool.StringF(HTTP::EMaxNumTransactionsToPipeline,
														   aSession.GetTable());
		THTTPHdrVal value;
		if (sessionSettings.Property(maxToPipelineSetting, value) == EFalse)
			{
			value.SetInt(iMaxNumberTransactionsToPipeline);
			sessionSettings.SetPropertyL(maxToPipelineSetting,value); 		
			}
		}
	
	
	if (iMaxNumberTransportHandlers > 0)
		{
		RStringF maxTransportHandlers = stringPool.StringF(HTTP::EMaxNumTransportHandlers,
														   aSession.GetTable());
		THTTPHdrVal value;
		if (sessionSettings.Property(maxTransportHandlers, value) == EFalse)
			{
			value.SetInt(iMaxNumberTransportHandlers);
			sessionSettings.SetPropertyL(maxTransportHandlers,value); 		
			}
		}
	}
	
	
void CPipeliningConfigTest::SetMaxNumberTransactionsToPipeline(TInt aMaxNumberTransactionsToPipeline)
	{	
	iMaxNumberTransactionsToPipeline = aMaxNumberTransactionsToPipeline;
	}
	
	
void CPipeliningConfigTest::SetMaxNumberTransportHandlers(TInt aMaxNumberTransportHandlers)
	{
	iMaxNumberTransportHandlers = aMaxNumberTransportHandlers;
	}
	
	
void CPipeliningConfigTest::AddTransactionUrisL(const TDesC8& aUri, TInt aRepeatNumber)
	{
	for (TInt ii = 0; ii< aRepeatNumber; ++ii)
		{
		HBufC8* uri = aUri.AllocLC();
		User::LeaveIfError(iTransactionUris.Append(uri));
		CleanupStack::Pop(uri);
		} 
	}	
	
	
	
void CPipeliningConfigTest::ClearTransactionUris()
	{
	iTransactionUris.ResetAndDestroy();
	}
	
	
void CPipeliningConfigTest::SetExpectedNumberConnections(TInt aExpectedNumberConnections)
	{
	iExpectedNumberConnections = aExpectedNumberConnections;
	}

TBool CPipeliningConfigTest::TransportHandlerPriority() const	
	{
	return EFalse;		
	}
	


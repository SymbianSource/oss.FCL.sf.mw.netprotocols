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
// defines test classes: CFormEncoderTest
// 
//

#include "t_formencoder.h"
#include "t_formencoder_tests.h"
#include <chttpformencoder.h>

// CSomeOfflineTest-----------------------------------

void CFormEncoderTest::DoRunL()
	{
	iExpectedStatusCode = 0;
	// insert test code here, allow it to leave if the test fails
	TestSinglesL();
	TestPairsL();
	TestEmptyNameL();
	}
	
TInt CFormEncoderTest::RunError(TInt aErr)
	{
	iEngine->Utils().LogIt(_L("\nTest failed with error code %d\n"), aErr);
	return KErrNone;
	}

void CFormEncoderTest::DoCancel()
	{
	}

CFormEncoderTest::~CFormEncoderTest()
	{
	}	

const TDesC& CFormEncoderTest::TestName()
	{
	_LIT(KHeaderTestName,"CFormEncoderTest");
	return KHeaderTestName;
	}

void CFormEncoderTest::TestL(TBool aTestPass)
	{
	if (!aTestPass)
		User::Leave(KErrArgument);

	}


void CFormEncoderTest::TestSinglesL()
	{
	TestSinglePairL(KName,KValue1, KNameEncodedLength+KValue1EncodedLength+1);
	TestSinglePairL(KName,KValue2, KNameEncodedLength+KValue2EncodedLength+1);
	TestSinglePairL(KName,KValue3, KNameEncodedLength+KValue3EncodedLength+1);
	TestSinglePairL(KName,KValue4, KNameEncodedLength+KValue4EncodedLength+1);
	TestSinglePairL(KName,KValue5, KNameEncodedLength+KValue5EncodedLength+1);
	TestSinglePairL(KName,KValue6, KNameEncodedLength+KValue6EncodedLength+1);
	TestSinglePairL(KName,KValue7, KNameEncodedLength+KValue7EncodedLength+1);
	TestSinglePairL(KName,KValue8, KNameEncodedLength+KValue8EncodedLength+1);
	TestSinglePairL(KName,KValue9, KNameEncodedLength+KValue9EncodedLength+1);
	TestSinglePairL(KName,KValue10,  KNameEncodedLength+KValue10EncodedLength+1);
	}

void CFormEncoderTest::TestSinglePairL(const TDesC8& aName, const TDesC8& aValue, TInt aExpectedEncodedLength)
	{
	CHTTPFormEncoder* formEncoder = CHTTPFormEncoder::NewL();
	CleanupStack::PushL(formEncoder);

	formEncoder->AddFieldL(aName, aValue);
	
	TPtrC8 data;
	formEncoder->GetNextDataPart(data);
	iEngine->Utils().DumpData(data);
	TestL(data.Length() == aExpectedEncodedLength);
	formEncoder->ReleaseData();
	CleanupStack::PopAndDestroy();
	}


void CFormEncoderTest::TestEmptyNameL()
	{
	CHTTPFormEncoder* formEncoder = CHTTPFormEncoder::NewL();
	CleanupStack::PushL(formEncoder);
	TRAPD(err,formEncoder->AddFieldL(KNullDesC8, KValue1));
	if (err == KErrNoMemory)
		User::Leave(err);
	TestL(err == KErrArgument);
	CleanupStack::PopAndDestroy();
	}

void CFormEncoderTest::TestPairsL()
	{
	CHTTPFormEncoder* formEncoder = CHTTPFormEncoder::NewL();
	CleanupStack::PushL(formEncoder);
	formEncoder->AddFieldL(KName, KValue1);
	formEncoder->AddFieldL(KName, KValue2);
	TPtrC8 data;
	formEncoder->GetNextDataPart(data);
	iEngine->Utils().DumpData(data);
	TestL(data.Length() == (KNameEncodedLength * 2) + KValue1EncodedLength + KValue2EncodedLength + 3);
	formEncoder->ReleaseData();
	CleanupStack::PopAndDestroy();

	formEncoder = CHTTPFormEncoder::NewL();
	CleanupStack::PushL(formEncoder);
	formEncoder->AddFieldL(KName, KValue2);
	formEncoder->AddFieldL(KName, KValue3);
	formEncoder->GetNextDataPart(data);
	iEngine->Utils().DumpData(data);
	TestL(data.Length() == (KNameEncodedLength * 2) + KValue2EncodedLength + KValue3EncodedLength + 3);
	formEncoder->ReleaseData();
	CleanupStack::PopAndDestroy();

	formEncoder = CHTTPFormEncoder::NewL();
	CleanupStack::PushL(formEncoder);
	formEncoder->AddFieldL(KName, KValue3);
	formEncoder->AddFieldL(KName, KValue4);
	formEncoder->GetNextDataPart(data);
	iEngine->Utils().DumpData(data);
	TestL(data.Length() == (KNameEncodedLength * 2) + KValue3EncodedLength + KValue4EncodedLength + 3);
	formEncoder->ReleaseData();
	CleanupStack::PopAndDestroy();

	formEncoder = CHTTPFormEncoder::NewL();
	CleanupStack::PushL(formEncoder);
	formEncoder->AddFieldL(KName, KValue4);
	formEncoder->AddFieldL(KName, KValue5);
	formEncoder->GetNextDataPart(data);
	iEngine->Utils().DumpData(data);
	TestL(data.Length() == (KNameEncodedLength * 2) + KValue4EncodedLength + KValue5EncodedLength + 3);
	formEncoder->ReleaseData();
	CleanupStack::PopAndDestroy();

	formEncoder = CHTTPFormEncoder::NewL();
	CleanupStack::PushL(formEncoder);
	formEncoder->AddFieldL(KName, KValue5);
	formEncoder->AddFieldL(KName, KValue6);
	formEncoder->GetNextDataPart(data);
	iEngine->Utils().DumpData(data);
	TestL(data.Length() == (KNameEncodedLength * 2) + KValue5EncodedLength + KValue6EncodedLength + 3);
	formEncoder->ReleaseData();
	CleanupStack::PopAndDestroy();


	formEncoder = CHTTPFormEncoder::NewL();
	CleanupStack::PushL(formEncoder);
	formEncoder->AddFieldL(KName, KValue6);
	formEncoder->AddFieldL(KName, KValue7);
	formEncoder->GetNextDataPart(data);
	iEngine->Utils().DumpData(data);
	TestL(data.Length() == (KNameEncodedLength * 2) + KValue6EncodedLength + KValue7EncodedLength + 3);
	formEncoder->ReleaseData();
	CleanupStack::PopAndDestroy();


	formEncoder = CHTTPFormEncoder::NewL();
	CleanupStack::PushL(formEncoder);
	formEncoder->AddFieldL(KName, KValue7);
	formEncoder->AddFieldL(KName, KValue8);
	formEncoder->GetNextDataPart(data);
	iEngine->Utils().DumpData(data);
	TestL(data.Length() == (KNameEncodedLength * 2) + KValue7EncodedLength + KValue8EncodedLength + 3);
	formEncoder->ReleaseData();
	CleanupStack::PopAndDestroy();


	formEncoder = CHTTPFormEncoder::NewL();
	CleanupStack::PushL(formEncoder);
	formEncoder->AddFieldL(KName, KValue8);
	formEncoder->AddFieldL(KName, KValue9);
	formEncoder->GetNextDataPart(data);
	iEngine->Utils().DumpData(data);
	TestL(data.Length() == (KNameEncodedLength * 2) + KValue8EncodedLength + KValue9EncodedLength + 3);
	formEncoder->ReleaseData();
	CleanupStack::PopAndDestroy();


	formEncoder = CHTTPFormEncoder::NewL();
	CleanupStack::PushL(formEncoder);
	formEncoder->AddFieldL(KName, KValue9);
	formEncoder->AddFieldL(KName, KValue10);
	formEncoder->GetNextDataPart(data);
	iEngine->Utils().DumpData(data);
	TestL(data.Length() == (KNameEncodedLength * 2) + KValue9EncodedLength + KValue10EncodedLength + 3);
	formEncoder->ReleaseData();
	CleanupStack::PopAndDestroy();


	formEncoder = CHTTPFormEncoder::NewL();
	CleanupStack::PushL(formEncoder);
	formEncoder->AddFieldL(KName, KValue1);
	formEncoder->AddFieldL(KName, KValue2);
	formEncoder->AddFieldL(KName, KValue3);
	formEncoder->GetNextDataPart(data);
	iEngine->Utils().DumpData(data);
	TestL(data.Length() == (KNameEncodedLength * 3) + KValue1EncodedLength + KValue2EncodedLength + KValue3EncodedLength +5);
	formEncoder->ReleaseData();
	CleanupStack::PopAndDestroy();

	formEncoder = CHTTPFormEncoder::NewL();
	CleanupStack::PushL(formEncoder);
	formEncoder->AddFieldL(KName, KValue3);
	formEncoder->AddFieldL(KName, KValue4);
	formEncoder->AddFieldL(KName, KValue5);
	formEncoder->GetNextDataPart(data);
	iEngine->Utils().DumpData(data);
	formEncoder->ReleaseData();
	TestL(data.Length() == (KNameEncodedLength * 3) + KValue3EncodedLength + KValue4EncodedLength + KValue5EncodedLength +5);

	CleanupStack::PopAndDestroy();


	formEncoder = CHTTPFormEncoder::NewL();
	CleanupStack::PushL(formEncoder);
	formEncoder->AddFieldL(KName, KValue5);
	formEncoder->AddFieldL(KName, KValue6);
	formEncoder->AddFieldL(KName, KValue7);
	formEncoder->GetNextDataPart(data);
	iEngine->Utils().DumpData(data);
	TestL(data.Length() == (KNameEncodedLength * 3) + KValue5EncodedLength + KValue6EncodedLength + KValue7EncodedLength +5);
	formEncoder->ReleaseData();
	CleanupStack::PopAndDestroy();

	formEncoder = CHTTPFormEncoder::NewL();
	CleanupStack::PushL(formEncoder);
	formEncoder->AddFieldL(KName, KValue7);
	formEncoder->AddFieldL(KName, KValue8);
	formEncoder->AddFieldL(KName, KValue9);
	formEncoder->GetNextDataPart(data);
	iEngine->Utils().DumpData(data);
	TestL(data.Length() == (KNameEncodedLength * 3) + KValue7EncodedLength + KValue8EncodedLength + KValue9EncodedLength +5);
	formEncoder->ReleaseData();
	CleanupStack::PopAndDestroy();
	

	formEncoder = CHTTPFormEncoder::NewL();
	CleanupStack::PushL(formEncoder);
	formEncoder->AddFieldL(KName, KValue1);
	formEncoder->AddFieldL(KName, KValue2);
	formEncoder->AddFieldL(KName, KValue3);
	formEncoder->AddFieldL(KName, KValue4);
	formEncoder->GetNextDataPart(data);
	iEngine->Utils().DumpData(data);
	TestL(data.Length() == (KNameEncodedLength * 4) + KValue1EncodedLength + KValue2EncodedLength + KValue3EncodedLength + 
		KValue4EncodedLength +7);

	formEncoder->ReleaseData();
	CleanupStack::PopAndDestroy();

	formEncoder = CHTTPFormEncoder::NewL();
	CleanupStack::PushL(formEncoder);
	formEncoder->AddFieldL(KName, KValue4);
	formEncoder->AddFieldL(KName, KValue5);
	formEncoder->AddFieldL(KName, KValue6);
	formEncoder->AddFieldL(KName, KValue7);
	formEncoder->GetNextDataPart(data);
	iEngine->Utils().DumpData(data);
	TestL(data.Length() == (KNameEncodedLength * 4) + KValue4EncodedLength + KValue5EncodedLength + KValue6EncodedLength + 
		KValue7EncodedLength +7);

	formEncoder->ReleaseData();
	CleanupStack::PopAndDestroy();

	formEncoder = CHTTPFormEncoder::NewL();
	CleanupStack::PushL(formEncoder);
	formEncoder->AddFieldL(KName, KValue7);
	formEncoder->AddFieldL(KName, KValue8);
	formEncoder->AddFieldL(KName, KValue9);
	formEncoder->AddFieldL(KName, KValue10);
	formEncoder->GetNextDataPart(data);
	iEngine->Utils().DumpData(data);
	TestL(data.Length() == (KNameEncodedLength * 4) + KValue7EncodedLength + KValue8EncodedLength + KValue9EncodedLength + 
		KValue10EncodedLength +7);

	formEncoder->ReleaseData();
	CleanupStack::PopAndDestroy();


	formEncoder = CHTTPFormEncoder::NewL();
	CleanupStack::PushL(formEncoder);
	formEncoder->AddFieldL(KName, KValue1);
	formEncoder->AddFieldL(KName, KValue2);
	formEncoder->AddFieldL(KName, KValue3);
	formEncoder->AddFieldL(KName, KValue4);
	formEncoder->AddFieldL(KName, KValue5);
	formEncoder->GetNextDataPart(data);
	iEngine->Utils().DumpData(data);
	TestL(data.Length() == (KNameEncodedLength * 5) + KValue1EncodedLength + KValue2EncodedLength + KValue3EncodedLength + 
		KValue4EncodedLength + KValue5EncodedLength +9);

	formEncoder->ReleaseData();
	CleanupStack::PopAndDestroy();


	formEncoder = CHTTPFormEncoder::NewL();
	CleanupStack::PushL(formEncoder);
	formEncoder->AddFieldL(KName, KValue6);
	formEncoder->AddFieldL(KName, KValue7);
	formEncoder->AddFieldL(KName, KValue8);
	formEncoder->AddFieldL(KName, KValue9);
	formEncoder->AddFieldL(KName, KValue10);
	formEncoder->GetNextDataPart(data);
	iEngine->Utils().DumpData(data);
	TestL(data.Length() == (KNameEncodedLength * 5) + KValue6EncodedLength + KValue7EncodedLength + KValue8EncodedLength + 
		KValue9EncodedLength + KValue10EncodedLength +9);

	formEncoder->ReleaseData();
	CleanupStack::PopAndDestroy();

	formEncoder = CHTTPFormEncoder::NewL();
	CleanupStack::PushL(formEncoder);
	formEncoder->AddFieldL(KName, KValue1);
	formEncoder->AddFieldL(KName, KValue2);
	formEncoder->AddFieldL(KName, KValue3);
	formEncoder->AddFieldL(KName, KValue4);
	formEncoder->AddFieldL(KName, KValue5);
	formEncoder->AddFieldL(KName, KValue6);
	formEncoder->AddFieldL(KName, KValue7);
	formEncoder->AddFieldL(KName, KValue8);
	formEncoder->AddFieldL(KName, KValue9);
	formEncoder->AddFieldL(KName, KValue10);
	formEncoder->GetNextDataPart(data);
	iEngine->Utils().DumpData(data);
	TestL(data.Length() == (KNameEncodedLength * 10) + KValue1EncodedLength + KValue2EncodedLength + KValue3EncodedLength + 
		KValue4EncodedLength + KValue5EncodedLength + KValue6EncodedLength + KValue7EncodedLength + KValue8EncodedLength + 
		KValue9EncodedLength + KValue10EncodedLength +19);

	formEncoder->ReleaseData();
	CleanupStack::PopAndDestroy();
}



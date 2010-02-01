// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "CPipeliningConfigDriver.h"
#include "TestScripts.h"
#include "T_PipeliningConfigFilter.h"
#include <http/cecomfilter.h>
#include <uri8.h>

// Constants for ini file
_LIT(KName, "name");
_LIT(KUri, "uri");
_LIT(KVersion, "httpversion");
_LIT(KTransCount, "transcount");
_LIT(KPipeline, "pipeline");
_LIT(KUsername, "username");
_LIT(KPassword, "password");

_LIT(KHttp11, "1.1");


CPipeliningConfigDriver* CPipeliningConfigDriver::NewL(CScriptFile& aScriptFile, TInt aSectionIndex)
	{
	CPipeliningConfigDriver* self = new(ELeave) CPipeliningConfigDriver(aScriptFile, aSectionIndex);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}
	
CPipeliningConfigDriver::~CPipeliningConfigDriver()
	{
	iTransArray.Reset();
	iTransArray.Close();
	iSession.Close();
	delete iUri;
	delete iUsername;
	delete iPassword;
	}

CPipeliningConfigDriver::CPipeliningConfigDriver(CScriptFile& aScriptFile, TInt aSectionIndex)
	: iScriptFile(aScriptFile), iSectionIndex(aSectionIndex)
	{
	}
	
void CPipeliningConfigDriver::ConstructL()
	{
	ReadScriptL();
	ResetHttpSessionL();
	}

const TDesC& CPipeliningConfigDriver::TestName()
	{
	return iTestName;
	}

void CPipeliningConfigDriver::DoCancel()
	{
	}

void CPipeliningConfigDriver::DoRunL()
	{
	CreateTransactionsL();
	// Submit the first transaction
	++iCurrentTrans;
	iTransArray[0].SubmitL();
	CActiveScheduler::Start();
	if (iTestFail==1)
		{
		User::Leave(-1);
		}
	}
	
void CPipeliningConfigDriver::ResetHttpSessionL()
	{
	iSession.Close();
	iSession.OpenL();
	
	SetHttpVersionL();
	
	// Install the authentication filter
	InstallAuthenticationL(iSession);
	
	// Install the pipelining config filter
	const TUid KPipeliningConfigUid = { 0x101F5601 };
	CEComFilter::InstallFilterL(iSession, KPipeliningConfigUid);
	
	// Install the pipelining test validation filter
	iFilter = CPipeliningConfigFilterTest::NewL(iSession, iTotalTrans, iShouldPipeline);
	}
	
TBool CPipeliningConfigDriver::GetCredentialsL( const TUriC8& /*aURI*/, RString /*aRealm*/, 
								  				RStringF /*aAuthenticationType*/,
												RString& aUsername, 
								  				RString& aPassword)
	{
	if(iUsername!=NULL && iPassword!=NULL )
		{
		// Ownership of the RStrings are transferred
		aUsername = iSession.StringPool().OpenStringL(*iUsername);
		aPassword = iSession.StringPool().OpenStringL(*iPassword);
		return ETrue;
		}

	return EFalse;
	}

HBufC8* CPipeliningConfigDriver::ConvertStringTo8BitL(const TDesC& aDes)
	{
	TPtrC des( aDes );
	HBufC8* heapDes = HBufC8::NewL(aDes.Length());
	TPtr8 desPtr = heapDes->Des();
	desPtr.Copy(des);
	return heapDes;
	}
	
TInt CPipeliningConfigDriver::ConvertDesToInt(const TDesC& aDes)
	{
	TLex intLex(aDes);
	TInt number;
	intLex.Val(number);
	return number;
	}

void CPipeliningConfigDriver::ReadScriptL()
	{
	// Create the transactions as specified in the ini file
	CScriptSection& section = iScriptFile.Section(iSectionIndex);
	
	// Read the test case name
	iTestName.Set( section.ItemL(KName()).Value() );
	
	// Read the Uri
	iUri = ConvertStringTo8BitL( section.ItemL(KUri()).Value() );
	
	// Read the username and password if any
	CScriptSectionItem* usernameItem = section.Item(KUsername());
	CScriptSectionItem* passwordItem = section.Item(KPassword());
	if(usernameItem!=NULL && passwordItem!=NULL )
		{
		iUsername = ConvertStringTo8BitL( usernameItem->Value() );
		iPassword = ConvertStringTo8BitL( passwordItem->Value() );
		}
	
	// Get the number of times the transaction should be repeated
	iTotalTrans = ConvertDesToInt(section.ItemL(KTransCount()).Value());
	
	// Get the flag to indicate whether the transaction should be pipelined
	iShouldPipeline = ConvertDesToInt(section.ItemL(KPipeline()).Value());
	
	// Get the HTTP version;
	iVersion.Set(section.ItemValue(KVersion(), KHttp11)); // Default is 1.1
	}	

void CPipeliningConfigDriver::CreateTransactionsL()
	{
	TUriParser8 uri;
	uri.Parse(*iUri);
	
	// Create the transactions
	for( TInt ii=0; ii<iTotalTrans; ++ii)
		{
		RStringF getMethod = iSession.StringPool().StringF(HTTP::EGET, iSession.GetTable());
		RHTTPTransaction trans = iSession.OpenTransactionL(uri, *this, getMethod);
		CleanupClosePushL(trans);
		User::LeaveIfError(iTransArray.Append(trans));
		CleanupStack::Pop(&trans);
		}
	}
	
void CPipeliningConfigDriver::SetHttpVersionL()
	{
	THTTPHdrVal httpVerProp(iSession.StringPool().StringF(HTTP::EHTTPVersion,iSession.GetTable()));
	if (iVersion.CompareF(KHttp11) == 0)
		{
		iSession.ConnectionInfo().SetPropertyL(httpVerProp, iSession.StringPool().StringF(HTTP::EHttp11, iSession.GetTable()));
		}
	else // apart from 1.1 only 1.0 is supported
		iSession.ConnectionInfo().SetPropertyL(httpVerProp, iSession.StringPool().StringF(HTTP::EHttp10, iSession.GetTable()));
	}
	
void CPipeliningConfigDriver::MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent)
	{
	switch(aEvent.iStatus)
		{
		case THTTPEvent::EGotResponseHeaders:
			{
			iEngine->Utils().LogIt(_L("<EGotResponseHeaders (%d)\n"),aEvent.iStatus);
			} break;
		case THTTPEvent::EGotResponseBodyData:
			{
			iEngine->Utils().LogIt(_L("<EGotResponseBodyData received (%d)>\n"),aEvent.iStatus);
			MHTTPDataSupplier* body = aTransaction.Response().Body();
			body->ReleaseData();
			} break;
		case THTTPEvent::EResponseComplete:
			{
			// The transaction's response is complete
			iEngine->Utils().LogIt(_L("<EResponseComplete received (%d)>\n"),aEvent.iStatus);
			} break;	
		case THTTPEvent::ESucceeded:
			{
				if(iCurrentTrans==iTotalTrans)
				{
				if( iFilter->GetTestResult() )
					{
					iEngine->Utils().LogIt(_L("Failed validation\n"));
					iTestFail = 1;
					}
				CActiveScheduler::Stop();
				}
			else
				{
				iTransArray[iCurrentTrans].SubmitL();
				++iCurrentTrans;
				}
			} break;
		case THTTPEvent::EFailed:
			{
			iEngine->Utils().LogIt(_L("<EFailed received, ending tests>\n"));
			CActiveScheduler::Stop();
			} break;
		default:
			{
			if( aEvent.iStatus < KErrNone )
				{
				// Error has occured
				iEngine->Utils().LogIt(_L("<Error occured: Error: %d>\n"),aEvent.iStatus);
				CActiveScheduler::Stop();
				}
			} break;
		}
	}

TInt CPipeliningConfigDriver::MHFRunError(TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& /*aEvent*/)
	{
	iEngine->Utils().LogIt(_L("MHFRunError: Errorcode: %d.\n"), aError);
	aTransaction.Fail();
	return KErrNone;
	}

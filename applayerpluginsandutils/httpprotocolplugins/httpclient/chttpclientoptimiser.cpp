// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "chttpclientoptimiser.h"

CHttpClientOptimiser* CHttpClientOptimiser::NewL(MOutputStream& aOutputStream, MInputStreamObserver& aInputStreamObserver)
	{
	CHttpClientOptimiser* me = new(ELeave) CHttpClientOptimiser(aOutputStream, aInputStreamObserver);
	return me;
	}

CHttpClientOptimiser::CHttpClientOptimiser(MOutputStream& aOutputStream, MInputStreamObserver& aInputStreamObserver)
:iOutputStream(&aOutputStream), 
	iInputStreamObserver(&aInputStreamObserver)
	{
	}

CHttpClientOptimiser::~CHttpClientOptimiser()
	{
	delete iDecodedBuffer;
	delete iEncodedBuffer;
	}

void CHttpClientOptimiser::BindOptimiser(MHttpDataOptimiser& aOptimiser)
	{
	iDataOptimiser = &aOptimiser;
	}

// methods from MOutputStream
void CHttpClientOptimiser::SendDataReqL(const TDesC8& aBuffer)
	{
	delete iEncodedBuffer;
	iEncodedBuffer = NULL;
	iDataOptimiser->EncodeL(aBuffer, iEncodedBuffer);
	__ASSERT_ALWAYS(iEncodedBuffer != NULL, User::Invariant() ); // Need to add panic code
	iOutputStream->SendDataReqL(iEncodedBuffer->Des()); // Calls CSocketWriter::SendDataReq()
	}

void CHttpClientOptimiser::Bind(MOutputStreamObserver& aObserver)
	{
	iOutputStream->Bind(aObserver);
	}

void CHttpClientOptimiser::ShutdownReq()
	{
	iOutputStream->ShutdownReq();
	}

void CHttpClientOptimiser::SecureClientReq(const TDesC8& aHostName)
	{
	__ASSERT_ALWAYS((aHostName.Length() > 0), User::Invariant());
	iOutputStream->SecureClientReq(aHostName);
	}

void CHttpClientOptimiser::Close()
	{
	iOutputStream->Close();
	}

const CX509Certificate* CHttpClientOptimiser::ServerCert()
	{
	return (iOutputStream->ServerCert());
	}

TInt CHttpClientOptimiser::CipherSuite(TDes8& aCipherSuite)
	{
	__ASSERT_ALWAYS((aCipherSuite.Length() > 0), User::Invariant());
	return (iOutputStream->CipherSuite(aCipherSuite));
	}

void CHttpClientOptimiser::Reset ()
	{
	iOutputStream->Reset();
	}

void CHttpClientOptimiser::MOutputStream_Reserved()
	{
	User::Invariant();
	}
	
// methods from MInputStreamObserver
void CHttpClientOptimiser::ReceivedDataIndL(const TDesC8& aBuffer)
	{
	delete iDecodedBuffer;	
	iDecodedBuffer = NULL;
	  	
	TBool transFail = EFalse;	
	iDataOptimiser->DecodeL(aBuffer, iDecodedBuffer, transFail);
		
	if(transFail)
		{
		CurrentResponse()->FailTransactionL();
		}
		
	else
		{
		__ASSERT_ALWAYS(iDecodedBuffer != NULL, User::Invariant());	
		iInputStreamObserver->ReceivedDataIndL(iDecodedBuffer->Des());		
		}
	}
	
void CHttpClientOptimiser::SecureServerCnf()
	{
	iInputStreamObserver->SecureServerCnf();
	}
	
void CHttpClientOptimiser::InputStreamCloseInd(TInt aError)
	{
	iInputStreamObserver->InputStreamCloseInd(aError);
	}

void CHttpClientOptimiser::MInputStreamObserver_Reserved()
	{
	User::Invariant();
	}
	
void CHttpClientOptimiser::OnReceiveTimeOut()
	{
	
	}
MHttpResponse* CHttpClientOptimiser::CurrentResponse()
	{
	return (iInputStreamObserver->CurrentResponse());
	}

void CHttpClientOptimiser::SetTCPCorking(TBool /* aValue */)
    {
    
    }


	
	

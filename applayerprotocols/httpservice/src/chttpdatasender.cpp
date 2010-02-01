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

#include "chttpdatasender.h"
#include "chttpcontentreader.h"
#include "chttpclienttransaction.h"
#include <thttpevent.h>

CHttpDataSender* CHttpDataSender::New(RHTTPTransaction aTrans, CHttpClientTransaction& aClientTrans, MHttpContentSource* aSource)
    {
    return new CHttpDataSender(aTrans, aClientTrans, aSource);
    }

CHttpDataSender* CHttpDataSender::New(RHTTPTransaction aTrans, CHttpClientTransaction& aClientTrans, RFile& aFile)
    {
    CHttpDataSender* self = new CHttpDataSender(aTrans, aClientTrans);
    if(self)
        {
        if(self->Construct(aFile) != KErrNone)
            {
            delete self;
            self = NULL;
            }
        }
    return self;
    }


CHttpDataSender::CHttpDataSender(RHTTPTransaction aTrans, CHttpClientTransaction& aClientTrans)
: iTransaction(aTrans),iClientTrans(aClientTrans),
iDynamicContentLen(KErrNotFound)
	{
	}

CHttpDataSender::CHttpDataSender(RHTTPTransaction aTrans, CHttpClientTransaction& aClientTrans, MHttpContentSource* aSource)
: iTransaction(aTrans),iDynamicSource(aSource),
iClientTrans(aClientTrans)
    {
    
    }

CHttpDataSender::~CHttpDataSender()
    {
    delete iFileReader;
    }

TInt CHttpDataSender::Construct(RFile& aFile)
    {
    iFileReader = CHttpFileReader::New(aFile, *this);
    return iFileReader ? KErrNone : KErrNoMemory;
    }

// From MHttpDataSender
void CHttpDataSender::Notify(const TDesC8& aData, TBool aLast)
	{
	__ASSERT_DEBUG(iDataPtr.Length() == 0, User::Invariant());
	iDataPtr.Set(aData.Ptr(), aData.Length());
	iLastPart = aLast;
	if(!iInCallback)
	    {
	    iTransaction.SendEvent(THTTPEvent::ENotifyNewRequestBodyPart, THTTPEvent::EOutgoing, THTTPFilterHandle::EClient);
	    }
	}

void CHttpDataSender::Error(TInt aError)
	{
	THTTPEvent evt(aError);
	iTransaction.SendEvent(evt, THTTPEvent::EIncoming, THTTPFilterHandle::EClient);
	}
	
// From MHTTPDataSupplier
TBool CHttpDataSender::GetNextDataPart(TPtrC8& aDataPart)
	{
	aDataPart.Set(KNullDesC8());
	iInCallback = ETrue;
	if(iDataPtr.Length() == 0)
	    {
	    RequestData();
	    }
	
	if(iDataPtr.Length() > 0)
	    {
	    iTransfferedDataSize += iDataPtr.Length();
	    iClientTrans.OnTransferProgress(iTransaction.Request().Body()->OverallDataSize(), iTransfferedDataSize);
	    aDataPart.Set(iDataPtr);
	    iWaitingForRelease = ETrue;
	    }
	iInCallback = EFalse;
	return iLastPart;
	}
	
void CHttpDataSender::ReleaseData()
	{
	if(iWaitingForRelease)
	    {
	    iDataPtr.Set(NULL, 0);
	    if(!iLastPart)
	        RequestData();
	    iWaitingForRelease = EFalse;
	    }
	}
	
TInt CHttpDataSender::OverallDataSize()
	{
	if(iFileReader)
	    return iFileReader->Size();
	return iDynamicContentLen;
	}
	
TInt CHttpDataSender::Reset()
	{
	return KErrNone;
	}

void CHttpDataSender::RequestData()
    {
    if(iDynamicSource)
        {
        THttpContentSourceOp op(this);
        iDynamicSource->OnData(op);
        }
    else if(iFileReader)
        {
        iFileReader->Read();
        }
    }

void CHttpDataSender::SetDynamicContentLen(TInt aLen)
    {
    iDynamicContentLen = aLen;
    }

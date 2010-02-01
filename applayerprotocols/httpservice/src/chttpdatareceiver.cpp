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


#include "chttpdatareceiver.h"
#include "chttpclienttransaction.h"
#include "chttpcontentwriter.h"

CHttpDataReceiver::CHttpDataReceiver(RHTTPTransaction aTrans, CHttpClientTransaction& aClientTrans)
: iTransaction(aTrans),iClientTrans(aClientTrans)
    {
    }

CHttpDataReceiver::~CHttpDataReceiver()
    {
    delete iFileWriter;
    }

TInt CHttpDataReceiver::SetFile(RFile& aFile)
    {
    iFileWriter = CHttpFileWriter::New(aFile, *this);
    return iFileWriter ? KErrNone : KErrNoMemory;
    }

void CHttpDataReceiver::DataAvailable()
    {
    if(iDynamicSink)
        {
        THttpContentSinkOp op(*this);
        iDynamicSink->OnData(op);
        }
    else 
        {
        Store();
        }
    }

TBool CHttpDataReceiver::GetData(TPtrC8& aData)
    {
    return iDataSupplier->GetNextDataPart(aData);
    }

void CHttpDataReceiver::Store()
    {
    if(iFileWriter)
        {
        TPtrC8 ptr;
        iDataSupplier->GetNextDataPart(ptr);
        iFileWriter->Write(ptr);
        if(!iDynamicSink)
            {
            iTransfferedDataSize += ptr.Length();
            iClientTrans.OnTransferProgress(iTransaction.Response().Body()->OverallDataSize(), iTransfferedDataSize);
            }
        }
    else
        {
        Release();
        }
    }

void CHttpDataReceiver::Release()
    {
    iDataSupplier->ReleaseData();
    }


void CHttpDataReceiver::Error(TInt aError)
    {
    THTTPEvent evt(aError);
    iTransaction.SendEvent(evt, THTTPEvent::EIncoming, THTTPFilterHandle::EClient);  
    }

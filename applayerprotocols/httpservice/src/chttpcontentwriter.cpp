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

#include "chttpcontentwriter.h"
#include "mhttpdatareceiver.h"


CHttpFileWriter* CHttpFileWriter::New(RFile aFile, MHttpDataReceiver& aReceiver)
    {
    CHttpFileWriter* self = new CHttpFileWriter(aFile, aReceiver);
    return self;
    }


CHttpFileWriter::CHttpFileWriter(RFile aFile, MHttpDataReceiver& aReceiver)
: CActive(EPriorityStandard),
iFile(aFile),
iDataReceiver(aReceiver)
    {
    CActiveScheduler::Add(this);    
    }
CHttpFileWriter::~CHttpFileWriter()
    {
    Cancel();    
    }

void CHttpFileWriter::Write(const TDesC8& aData)
    {
    iFile.Write(aData, iStatus);
    SetActive();
    }

// From CActive
void CHttpFileWriter::RunL()
    {
    User::LeaveIfError(iStatus.Int());
    iDataReceiver.Release();
    }

void CHttpFileWriter::DoCancel()
    {
    }

TInt CHttpFileWriter::RunError(TInt aError)
    {
    iDataReceiver.Error(aError);
    return KErrNone;
    }    


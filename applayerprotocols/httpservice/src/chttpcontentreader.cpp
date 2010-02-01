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

#include "chttpcontentreader.h"
#include "mhttpdatasender.h"

const TInt KFileReadSize = 16 * 1024;

CHttpFileReader* CHttpFileReader::New(RFile aFile, MHttpDataSender& aReceiver)
    {
    CHttpFileReader* self = new CHttpFileReader(aFile, aReceiver);
    if(self)
        {
        if(self->Construct() != KErrNone)
            {
            delete self;
            self = NULL;
            }
        }
    return self;
    }

TInt CHttpFileReader::Construct()
    {   
    iFile.Size(iTotalSize);
    return iContentBuffer.Create(KFileReadSize);
    }

CHttpFileReader::CHttpFileReader(RFile aFile, MHttpDataSender& aReceiver)
: CActive(EPriorityStandard),
iFile(aFile),
iDataSender(aReceiver)
    {
    CActiveScheduler::Add(this);
    }
CHttpFileReader::~CHttpFileReader()
    {
    Cancel();
    iContentBuffer.Close();
    }

void CHttpFileReader::Read()
    {
    iContentBuffer.Zero();
    iFile.Read(iContentBuffer, iStatus);
    SetActive();
    }

// From CActive
void CHttpFileReader::RunL()
    {
    User::LeaveIfError(iStatus.Int());
    iReadBytes += iContentBuffer.Length();
    iDataSender.Notify(iContentBuffer, iReadBytes == iTotalSize);
    }

void CHttpFileReader::DoCancel()
    {
    iFile.ReadCancel(iStatus);
    }

TInt CHttpFileReader::RunError(TInt aError)
    {
    iDataSender.Error(aError);
    return KErrNone;
    }    


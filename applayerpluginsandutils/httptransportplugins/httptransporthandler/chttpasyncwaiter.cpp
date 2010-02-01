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

#include "chttpasyncwaiter.h"
 
CHttpAsyncWaiter* CHttpAsyncWaiter::New()
    {
     return ( new CHttpAsyncWaiter() );
    }
 
 CHttpAsyncWaiter::CHttpAsyncWaiter()
     : CActive(EPriorityStandard)	
     {
     CActiveScheduler::Add(this);
     }   
 
 CHttpAsyncWaiter::~CHttpAsyncWaiter()
     {
     Cancel();
    }
     
 void CHttpAsyncWaiter::StartAndWait()
     {
     SetActive();
     CActiveScheduler::Start();
     }
     
 TInt CHttpAsyncWaiter::Result() const
     {
     return iError;
     }
     
 void CHttpAsyncWaiter::RunL()
     {
     iError = iStatus.Int();
     CActiveScheduler::Stop();
     }
     
 void CHttpAsyncWaiter::DoCancel()
     {
     iError = KErrCancel;
     CActiveScheduler::Stop();
     }
 

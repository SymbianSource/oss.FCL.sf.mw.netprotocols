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

#ifndef __CHTTPASYNCWAITER_H__
#define __CHTTPASYNCWAITER_H__
 
 #include <e32base.h>
 
 class CHttpAsyncWaiter : public CActive
     {
public:
     static CHttpAsyncWaiter* New();
     ~CHttpAsyncWaiter();
     
     void StartAndWait();
     TInt Result() const;
     
 private:
     CHttpAsyncWaiter();
     
     // from CActive
     virtual void RunL();
     virtual void DoCancel();
     
 private:
     TInt iError;
     };
 
 #endif // __CHTTPASYNCWAITER_H__ 


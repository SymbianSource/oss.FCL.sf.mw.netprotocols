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

#ifndef __CHTTPDATARECEIVER_H__
#define __CHTTPDATARECEIVER_H__

#include  <e32base.h>
#include <http.h>
#include "mhttpdatareceiver.h"

class MHttpContentSink;
class CHttpFileWriter;
class CHttpClientTransaction;

class CHttpDataReceiver : public CBase, public MHttpDataReceiver
    {
    public:
    CHttpDataReceiver(RHTTPTransaction aTrans, CHttpClientTransaction& aClientTrans);
    ~CHttpDataReceiver();
    
    inline void SetDataSupplier(MHTTPDataSupplier* aSupplier);
    inline void SetDynamicSink(MHttpContentSink*  aSink);
    TInt SetFile(RFile& aFile);
    void DataAvailable();
    
    private:
    // From MHttpDataReceiver
    TBool GetData(TPtrC8& aData);
    void Store();
    void Release();
    void Error(TInt aError);
    
    private:
    RHTTPTransaction        	iTransaction;
    MHTTPDataSupplier*      	iDataSupplier;
    MHttpContentSink*       	iDynamicSink;
    CHttpFileWriter*        	iFileWriter;
    CHttpClientTransaction&     iClientTrans;
    TInt                        iTransfferedDataSize;
    };

inline void CHttpDataReceiver::SetDataSupplier(MHTTPDataSupplier* aSupplier)
    {
    iDataSupplier = aSupplier;
    }

inline void CHttpDataReceiver::SetDynamicSink(MHttpContentSink*  aSink)
    {
    iDynamicSink = aSink;
    }
#endif // __CHTTPDATARECEIVER_H__

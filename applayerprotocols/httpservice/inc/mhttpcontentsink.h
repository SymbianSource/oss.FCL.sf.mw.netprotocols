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


#ifndef __MHTTPCONTENTSINK_H__
#define __MHTTPCONTENTSINK_H__

#include <e32base.h>

class MHttpDataReceiver;
/**
 * THttpContentSinkOp is an operator to retrieve/store/release the HTTP
 * content. The data consuming can be an asynchronous operation. After 
 * consuming the data Release/Store function must be called.
 * 
 * @publishedAll
 * @released
 */
class THttpContentSinkOp
    {
    friend class CHttpDataReceiver;
    public:
    IMPORT_C THttpContentSinkOp();
    IMPORT_C TBool GetData(TPtrC8& aData);
    IMPORT_C void Release();
    IMPORT_C void Store(); // Store into a file if a sink is provided.
    
    private:
        THttpContentSinkOp(MHttpDataReceiver& aSupplier);
    private:
    MHttpDataReceiver* iDataReceiver;
    };

/**
 * The implementation of MHttpContentSink can process the response body data  If the application wants to 
 * process/sink the data in different means then the client can insulate sink to a different sink
 * 
 * @publishedAll
 * @prototype
 */

class MHttpContentSink
	{
	public:
    /**
     * Supply a part of the response body data. The data will survive till the THttpContentSinkOp::Release/Store 
     * is called. application should process the data or keep a separate copy if it has to process the data at a 
     * later stage. This function will be called when part of the body data is available
     * 
     * @param   aData  Sink Operator
     */
	virtual void OnData(THttpContentSinkOp& aData) =0;
	
	private:
   // Reserved for future use.
    inline virtual void Reserved();
    inline virtual void Reserved2();        
    };

inline void MHttpContentSink::Reserved()
    {
    
    }

inline void MHttpContentSink::Reserved2()
    {
    
    }	

#endif // __MHTTPCONTENTSINK_H__

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


#ifndef __MHTTPCONTENTSOURCE_H__
#define __MHTTPCONTENTSOURCE_H__

#include <e32base.h>

class MHttpDataSender;

/**
 * THttpContentSourceOp is an operator to notify the HTTP Service
 * when content is available. 
 *  
 * @publishedAll
 * @prototype
 */
class THttpContentSourceOp
    {
    friend class CHttpDataSender;
public:
    IMPORT_C THttpContentSourceOp();
    IMPORT_C void Notify(const TDesC8& aData, TBool aLast = EFalse);
    private:
        THttpContentSourceOp(MHttpDataSender* aSender);
    MHttpDataSender* iDataSender;
    };

/**
 * The implementation of MHttpContentSink can supply the request body data.
 *  
 * @publishedAll
 * @prototype
 */
class MHttpContentSource
	{
	public:	
	/**
	 * Obtain a part of request body data from the source. The data should be guranteed to survive till 
	 * another call to OnData or a call to CHttpClientTransaction::OnCompletion/OnError
	 * 
	 * @param aData Data source operator
	 */
	    	    
	virtual void OnData(THttpContentSourceOp& aData) =0;
	
	private:
    // Reserved for future use.
    inline virtual void Reserved();
    inline virtual void Reserved2();	    
	};

inline void MHttpContentSource::Reserved()
    {
    
    }

inline void MHttpContentSource::Reserved2()
    {
    
    }

#endif // __MHTTPCONTENTSOURCE_H__

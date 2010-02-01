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

#ifndef __CHTTPDATASENDER_H__
#define __CHTTPDATASENDER_H__

#include <e32base.h>
#include <http.h>
#include "mhttpdatasender.h"

class MHttpContentSource;
class CHttpFileReader;
class CHttpClientTransaction;

class CHttpDataSender : public CBase, 
						public MHttpDataSender, 
						public MHTTPDataSupplier
	{
	public:
	static CHttpDataSender* New(RHTTPTransaction aTrans, CHttpClientTransaction& aClientTrans, MHttpContentSource* aSource);
	static CHttpDataSender* New(RHTTPTransaction aTrans, CHttpClientTransaction& aClientTrans, RFile& aFile);

	~CHttpDataSender();
	
	void SetDynamicContentLen(TInt aLen);
	private:
	CHttpDataSender(RHTTPTransaction aTrans, CHttpClientTransaction& aClientTrans); 
	CHttpDataSender(RHTTPTransaction aTrans, CHttpClientTransaction& aClientTrans, MHttpContentSource* aSource);
	TInt Construct(RFile& aFile);
	// From MHttpDataSender
	void Notify(const TDesC8& aData, TBool aLast);
	void Error(TInt aError);
	
	// From MHTTPDataSupplier
	TBool GetNextDataPart(TPtrC8& aDataPart);
	void ReleaseData();
	TInt OverallDataSize();
	TInt Reset();
	
	void RequestData();
	
	private:
	RHTTPTransaction 			iTransaction;	
	MHttpContentSource* 		iDynamicSource;	
	CHttpFileReader*    		iFileReader;
	CHttpClientTransaction&     iClientTrans;
	TPtrC8              		iDataPtr;
	TBool               		iWaitingForRelease;	
	TBool               		iLastPart;
	TBool               		iInCallback;
	TInt                		iDynamicContentLen;
	TInt                        iTransfferedDataSize;
	};

#endif // __CHTTPDATASENDER_H__

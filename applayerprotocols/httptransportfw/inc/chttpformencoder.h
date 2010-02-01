// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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

/**
 @file CHTTPFormEncoder.h
 @warning : This file contains Rose Model ID comments - please do not delete
*/

#ifndef __CHTTPFORMENCODER__
#define __CHTTPFORMENCODER__

// System includes
#include <http/mhttpdatasupplier.h>

//##ModelId=3C4C187902EB
class CHTTPFormEncoder : public CBase, public MHTTPDataSupplier
/** 
A data supplier class that is used to build up data that is to be encoded to application/x-www-form-urlencoded.

A client will create an instance of this class and add name/value pairs. They then use this as the data supplier for the 
body of an http request that is a forms submission.  

The name and value must both be supplied in the correct character encoding that you want to send to the server. This then 
gets url encoded.
@publishedAll
@released
*/
	{
public:
	//##ModelId=3C4C18790347
	IMPORT_C static CHTTPFormEncoder* NewL();
	//##ModelId=3C4C18790346
	IMPORT_C  ~CHTTPFormEncoder();

	// from MHTTPDataSupplier
	//##ModelId=3C4C1879033D
	virtual TBool GetNextDataPart(TPtrC8& aDataPart);
	//##ModelId=3C4C1879033C
	virtual void ReleaseData();
	//##ModelId=3C4C1879033B
	virtual TInt OverallDataSize();
	//
	//##ModelId=3C4C18790335
	virtual TInt Reset();

	/** Adds a field to the data.
	A field consists a name and value pair
	@param aFieldName A name. This must not be empty
	@param aFieldValue A value. This may be an empty descriptor
	@leave KErrNoMemory There was not enough memory
	@leave KErrArgument aFieldName was empty
	@panic Will panic if called after the first data has been supplied or OverallDataSize has been called
	*/
	//##ModelId=3C4C18790332
	IMPORT_C void AddFieldL(const TDesC8& aFieldName, const TDesC8& aFieldValue);
	
	
private:
	//##ModelId=3C4C18790331
	CHTTPFormEncoder();
	//##ModelId=3C4C1879032A
	void ConstructL();
	
	//##ModelId=3C4C18790328
	TBool IsAlphaNumeric(TUint8 aChar);
	//##ModelId=3C4C1879031F
	void AppendSpecialChar(TUint8 aChar);
	//##ModelId=3C4C1879031D
	void AppendData(const TDesC8& aData);

private:
	enum THTTPFormEncoderState
		{
		EAllowAppendData, // when first constructed 
		ESuppliedSize,
		ESupplyingData,    // GetDataChunk has been called but ReleaseData has not
		EDataReleased
		};

	//##ModelId=3C4C18790315
	THTTPFormEncoderState iState;
	//##ModelId=3C4C1879030D
	HBufC8* iData;		// data is ALWAYS kept in the form-url-encoded format
	};

#endif // __CHTTPFORMENCODER__

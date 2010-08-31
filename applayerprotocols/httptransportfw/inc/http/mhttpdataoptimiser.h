// Copyright (c) 2007-2010 Nokia Corporation and/or its subsidiary(-ies).
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
 @file
 @warning : This file contains Rose Model ID comments - please do not delete
*/

#ifndef	__MHTTPDATAOPTIMISER_H__
#define	__MHTTPDATAOPTIMISER_H__


/**
@publishedAll
@released
*/
class MHttpDataOptimiser
{
	public:
	/**
	This method converts the HTTP message data to the proprietary protocol format. 
	It is the responsibility of the implementation to allocate the required buffer for the encoded data. 
	It is the responsibility of the caller to free the allocated memory.
	@param	aHTTPData Contains the message data in HTTP format.
	@param	aEncodedData On return it contains the encoded message data.
	@leave		KErrNoMemory On insuffcient memory conditions.
	*/
	virtual void EncodeL (const TDesC8& aHttpData, HBufC8*& aEncodedData) = 0;

	/**
	This method converts the data from the proprietary protocol format to the HTTP format. 
	It is the responsibility of the implementation to allocate the required buffer for the encoded data. 
	It is the responsibility of the caller to free the allocated memory.
	@param	aData Is the raw data received over the TCP connection.
	@param	aHTTPData On return contains the data in HTTP format.
	@param	aTransFail If set to ETrue, sends the KErrHTTPOptimiserFailsTrans event to the client. 
	@leave		KErrNoMemory On insuffcient memory conditions.
	*/
	virtual void DecodeL (const TDesC8& aData, HBufC8*& aHttpData, TBool& aTransFail) = 0;
};


#endif //	__MHTTPDATAOPTIMISER_H__

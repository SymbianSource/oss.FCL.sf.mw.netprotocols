// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Definition of the data provider observer abstract class
// It is anticipated that this class will be used as a mixin for
// those objects which wish to provide services for an MDataProvider
// plugin.
// 
//


#ifndef __MDATAPROVIDEROBSERVER_H__
#define __MDATAPROVIDEROBSERVER_H__

// System includes
//
#include <e32std.h>

//##ModelId=3B712B2B01C2
class MDataProviderObserver
/**
MDataProviderObserver provides a virtual interface for any class to observe
any MDataProvider, and provide data receiving services. 
@publishedAll
@deprecated
*/
	{
public:	// Methods

/**
	Intended Usage	:	Called by the data provider to request processing of 
	the data within the buffer.
	@since			6.0
	@param			aBuffer	A reference to the buffer with the data to process
 */
	//##ModelId=3B712B2B0209
	virtual void ProcessDataL(HBufC8& aBuffer) = 0;

/**
	Intended Usage	:	Called by the data provider to report its status to 
	its observer. A status code which is either a standard error condition, 
	(i.e. < 0), a percentage complete value, ( 0 > aStatus < 100), OR a 
	TPluginStatus value.
	@since			6.0
	@param			aStatusCode	The status of the data provider
 */
	//##ModelId=3B712B2B0203
	virtual void SetStatus(TInt aStatusCode = KErrNone) = 0;

/**
	Intended Usage	:	The request to construct a CDataHandler to process data 
	of aDataType, where aDataType is a string to match against some plugins 
	declared data handling ability.
	@since			6.0
	@param			aDataType A reference to a descriptor with the data type
 */
	//##ModelId=3B712B2B01FE
	virtual void SetDocumentTypeL(const TDesC& aDataType) = 0;

/**
	Intended Usage	:	The request to construct a CDataHandler for aDataType with
	non default document encoding where aDataType is a string to match against 
	some plugins declared data handling ability.
	@since			6.0
	@param			aDataType		A reference to a descriptor with the data type
	@param			aContentType	A reference to a descriptor with the content type
 */
	//##ModelId=3B712B2B0200
	virtual void SetDocumentTypeL(const TDesC& aDataType, const TDesC& aContentType) = 0;

/**
	Intended Usage	:	The report of how many bytes of data is expected to be 
	passed to ProcessDataL, enabling %age reporting capabilities.
	@since			6.0
	@param			aAmountExpected	An integer value of the expected amount
 */
	//##ModelId=3B712B2B01EC
	virtual void SetDataExpected(TInt aAmountExpected) = 0;

/**
	Intended Usage	:	Set the URI that the DataProvider considers the base 
	URI for the current data.
	@since			6.0
	@param			aBaseUri	A const pointer to the uri
 */
	//##ModelId=3B712B2B01EA
	virtual void SetBaseUriL(const TDesC* aBaseUri) = 0;

private:

/**
	Intended Usage	:	Reserved for future expansion.
	@since			6.0
 */
	//##ModelId=3B712B2B01E1
	virtual void MDataProviderObserverReserved1() =0;

/**
	Intended Usage	:	Reserved for future expansion.
	@since			6.0
 */
	//##ModelId=3B712B2B01E0
	virtual void MDataProviderObserverReserved2() =0;

	};  // MDataProviderObserver

#endif    // __MDATAPROVIDEROBSERVER_H__

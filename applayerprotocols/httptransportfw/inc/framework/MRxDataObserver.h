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
 @file MRxDataObserver.h
 @warning : This file contains Rose Model ID comments - please do not delete
*/

#ifndef	__MRXDATAOBSERVER_H__
#define __MRXDATAOBSERVER_H__

// System includes
#include <e32std.h>

// Forward declarations
class CRxData;


//##ModelId=3A914E570296
class MRxDataObserver
/**
An interface to be implemented by classes that wish to receive status information
from an implementation of the CRxData class.
@publishedAll
@released
*/
	{
public:
/** @fn				void SetStatusL(CRxData& aRxData, TInt aStatus)
	Intended Usage:	Receive a status message from the object that processes received
					data in a protocol handler.
	@param			aRxData		The object which is processing the data.
	@param			aStatus		The status (or error) code.
*/
	//##ModelId=3B1E6B5800E5
	virtual void SetStatusL(CRxData& aRxData, TInt aStatus) =0;	

private: 
    // methods
    /**    @fn             Reserved1()
   Intended Usage: Reserve a slot in the v-table to preserve future BC
   */
   //##ModelId=3C4C37CE035D
   inline virtual void MRDO_Reserved1();
    

/**	@fn				Reserved2()
	Intended Usage:	Reserve a slot in the v-table to preserve future BC
 */
	//##ModelId=3C4C37CE0349
	inline virtual void MRDO_Reserved2();
	};

inline void MRxDataObserver::MRDO_Reserved1()
    {}


inline void MRxDataObserver::MRDO_Reserved2()
	{}

#endif // __MRXDATAOBSERVER_H__

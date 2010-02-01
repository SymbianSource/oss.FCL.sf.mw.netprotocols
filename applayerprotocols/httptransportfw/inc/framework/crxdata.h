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
 @file CRxData.h
 @warning : This file contains Rose Model ID comments - please do not delete
*/

#ifndef __CRXDATA_H__
#define __CRXDATA_H__

// System includes
#include <e32base.h>
#include <http/framework/cprottransaction.h>
#include <http/mhttpdatasupplier.h>
#include <http/rhttpsession.h> // this include is needed to ensure that the <rhttp.inl> is compiled by gcc...

// Forward declarations
class MRxDataObserver;


//##ModelId=3B1E52AB0087
class CRxData : public CBase, public MHTTPDataSupplier
/**
An abstract base class for the decoding of response data received by a
protocol handler.
@publishedAll
@released
*/
	{
public:

/** Intended Usage:	Destructor - cleans up and releases resources to the system	
*/
	IMPORT_C
	//##ModelId=3B1E52AB00ED
	virtual ~CRxData();

/** Intended Usage:	Obtain the protocol transaction whose request part is represented
					as encoded data by this object.
	@return The protocol transaction object
*/
	IMPORT_C
	//##ModelId=3B1E52AB00EC
	CProtTransaction& ProtTrans();

public:

/** Intended Usage:	Resets the received data object to its initial state following
					construction.  This method must be implemented for specific sub-
					classes of received data.
	@post			The object is reset.
*/
	//##ModelId=3BA60048014B
	virtual void ResetRxData() = 0;

protected:

/** Intended Usage:	Default constructor	- creates an Rx data object not associated
					with any protocol transaction.
*/
	IMPORT_C
	//##ModelId=3A914E5F008F
	CRxData();

/** Intended Usage:	Normal constructor, of a CRxData that corresponds to the supplied
					protocol transaction, and which sends status updates to the supplied
					observer interface,
	@param			aProtTrans			(in) The transaction for which this object
											 represents the transmitted data.
	@param			aObserver			(in) The object's observer.
*/
	IMPORT_C CRxData(CProtTransaction& aProtTrans, MRxDataObserver& aObserver);

/** Intended Usage:	Second phase construction, default
*/
	IMPORT_C
	//##ModelId=3B1E52AB00BA
	void ConstructL();

protected:

	/** The protocol transaction for which this object represents the encoded received data
	*/
	//##ModelId=3B1E52AB00B2
	CProtTransaction* iProtTrans;

	/** This object's observer, to whom we forward status information
	*/
	//##ModelId=3B1E7738028B
	MRxDataObserver* iObserver;

private: // methods

/**	Intended Usage:	Reserve a slot in the v-table to preserve future BC
 */
	//##ModelId=3C4C37DA0007
	inline virtual void Reserved1();

/**	Intended Usage:	Reserve a slot in the v-table to preserve future BC
 */
	//##ModelId=3C4C37D903DB
	inline virtual void Reserved2();
	};


inline void CRxData::Reserved1()
	{}
inline void CRxData::Reserved2()
	{}


#endif // __CRXDATA_H__ 


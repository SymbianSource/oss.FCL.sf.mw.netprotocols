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
 @file CTxData.h
 @warning : This file contains Rose Model ID comments - please do not delete
*/

#ifndef __CTXDATA_H__
#define __CTXDATA_H__

// System includes
#include <e32base.h>
#include <http/framework/cprottransaction.h>
#include <http/mhttpdatasupplier.h>
#include <http/rhttpsession.h> // this include is needed to ensure that the <rhttp.inl> is compiled by gcc...


//##ModelId=3B1E52A8001F
class CTxData : public CBase, public MHTTPDataSupplier
/**
An abstract base class for the encoding of request data to be transmitted by a
protocol handler.
@publishedAll
@released
*/
	{
public: // methods

/** @fn				~CTxData()
	Intended Usage:	Destructor - cleans up and releases resources to the system	
*/
	IMPORT_C
	//##ModelId=3B1E52A80051
	virtual ~CTxData();

/** @fn				CProtTransaction& ProtTrans()
	Intended Usage:	Obtain the protocol transaction whose request part is represented
					as encoded data by this object.
	@return The protocol transaction object
*/
	IMPORT_C
	//##ModelId=3B1E52A80049
	CProtTransaction& ProtTrans();

protected:

/** @fn				CTxData()
	Intended Usage:	Default constructor	- creates a Tx data object not associated
					with any protocol transaction.
*/
	IMPORT_C
	//##ModelId=3A925A44019E
	CTxData();
	
/** @fn				CTxData(CProtTransaction& aProtTransaction)
	Intended Usage:	Normal constructor, of a CTxData that corresponds to the supplied
					protocol transaction.
	@param			aProtTransaction	(in) The transaction for which this object
											 represents the transmitted data.
*/
	IMPORT_C CTxData(CProtTransaction& aProtTransaction);

/** @fn				ConstructL()
	Intended Usage:	Second phase construction, default
*/
	IMPORT_C
	//##ModelId=3B1E52A8003D
	void ConstructL();

protected: // attributes

	/** The HTTP transaction from whose request this OTA transmission data is built.
		Note that the transaction is not owned by this object.
	*/
	//##ModelId=3B1E52A80035
	CProtTransaction* iProtTrans;

private: // methods

/**	@fn				Reserved1()
	Intended Usage:	Reserve a slot in the v-table to preserve future BC
 */
	//##ModelId=3C4C37D6010F
	inline virtual void Reserved1();

/**	@fn				Reserved2()
	Intended Usage:	Reserve a slot in the v-table to preserve future BC
 */
	//##ModelId=3C4C37D600FB
	inline virtual void Reserved2();
	};


inline void CTxData::Reserved1()
	{}
inline void CTxData::Reserved2()
	{}


#endif // __CTXDATA_H__

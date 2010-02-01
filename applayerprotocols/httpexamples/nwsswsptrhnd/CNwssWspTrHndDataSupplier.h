// Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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
 @file CNwssWspTrHndDataSupplier.h
 @warning : This file contains Rose Model ID comments - please do not delete
*/

#ifndef __CNWSSWSPTRHNDDATASUPPLIER_H__
#define __CNWSSWSPTRHNDDATASUPPLIER_H__

// System includes
#include <e32base.h>
#include <http/mhttpdatasupplier.h>

// User includes
#include "mnwssoomhandler.h"
#include <http/framework/logging.h>

// Constants
const TInt KNwssResponseDataBufferSize = 2048;

// Forward class declarations
class RWSPCOTrans;

/**
	@since			7.0
	This class acts as a data supplier for WSP response bodies.  It is created when the
	T-MethodResult.ind PDU is received, and is passed to the client to allow them to extract
	the result data from the WAP Stack.
 */

//##ModelId=3C4C46D00163
class CNwssWspTrHndDataSupplier : public CBase, public MHTTPDataSupplier
	{
public: // methods

/**Factory construction.  Creates a new data supplier, associated with the
					supplied WAP stack transaction handle.
	@since			7.0
	@param			aStackTrans			(in) The WAP Stack transaction from which the response data is
											 to be extracted.
	@param			aOomHandler			(in) A handler used in OOM situations that occur when
											 invocations on the WAP stack fail with KErrNoMemory.
	@leave			System wide errors, e.g. KErrNoMemory.
 */
	//##ModelId=3C4C46D001C8
	static CNwssWspTrHndDataSupplier* NewL(RWSPCOTrans& aStackTrans, MNwssOomHandler& aOomHandler);

/**Destructor.
	@since			7.0
 */
	//##ModelId=3C4C46D001C7
	virtual ~CNwssWspTrHndDataSupplier();

private: // methods inherited from MHTTPDataSupplier

/**Obtain a data part from the supplier.  The data is guaranteed to survive until a call
					is made to ReleaseData().
	@since			7.0
	@param			aDataPart	(in) the data part
	@return			ETrue if this is the last part. EFalse otherwise
 */
	//##ModelId=3C4C46D001BE
	virtual TBool GetNextDataPart(TPtrC8& aDataPart);

/**Release the current data part being held at the data
					supplier.  This call indicates to the supplier that the part
					is no longer needed, and another one can be supplied, if
					appropriate.
	@since			7.0
 */
	//##ModelId=3C4C46D001BD
	virtual void ReleaseData();

/**Obtain the overall size of the data being supplied, if known
					to the supplier.  Where a body of data is supplied in several
					parts this size will be the sum of all the part sizes. If
					the size is not known, KErrNotFound is returned; in this case
					the client must use the return code of GetNextDataPart to find
					out when the data is complete.
	@since			7.0
	@return			A size in bytes, or KErrNotFound if the size is not known.
 */
	//##ModelId=3C4C46D001B4
	virtual TInt OverallDataSize();

/**Reset the data supplier.  This indicates to the data supplier that it should
					return to the first part of the data.  This could be used in a situation where
					the data consumer has encountered an error and needs the data to be supplied
					afresh.  Even if the last part has been supplied (i.e. GetNextDataPart has
					returned ETrue), the data supplier should reset to the first part.
				
					If the supplier cannot reset it should return an error code; otherwise it should
					return KErrNone, where the reset will be assumed to have succeeded
 	@since			7.0
	@return			KErrNone, if reset is supported and was sucessfull; or KErrNotSupported otherwise.
 */
	//##ModelId=3C4C46D001B3
	virtual TInt Reset();

private: // methods

/**Normal constructor.
	@since			7.0
	@param			aStackTrans			(in) The WAP Stack transaction from which the response data is
											 to be extracted.
	@param			aOomHandler			(in) A handler used in OOM situations that occur when
											 invocations on the WAP stack fail with KErrNoMemory.
 */
	//##ModelId=3C4C46D001AA
	CNwssWspTrHndDataSupplier(RWSPCOTrans& aStackTrans, MNwssOomHandler& aOomHandler);

/**Second phase construction, allocates the buffer to be used by the data supplier.
	@since			7.0
 */
	//##ModelId=3C4C46D001A9
	void ConstructL();

/**Extracts the response body data from the transaction.
	@since			7.0
	@return			An error code from the WAP stack.
*/
	//##ModelId=3C9B094D0100
	TInt ExtractBodyData();

private:

	/** Logger handle
	*/
	__DECLARE_LOG

	/** The WAP stack transaction from which response data is being extracted
	*/
	//##ModelId=3C4C46D00197
	RWSPCOTrans& iStackTrans;

	/** The handler provided for use in out-of-memory situations
	*/
	//##ModelId=3C9B094D00F6
	MNwssOomHandler& iOomHandler;

	/** The transaction ID for iStackTrans
	*/
	//##ModelId=3C9B094D00E2
	RWSPCOTrans::TTransID iStackTransId;

	/** A buffer used to hold the data received from the WAP proxy
	*/
	//##ModelId=3C4DA474022E
	TBuf8<KNwssResponseDataBufferSize> iBuffer;

	/** The overall size of the response.
	*/
	//##ModelId=3C4DA4630297
	TInt iOverallDataSize;

	/** The number of bytes remaining to be extracted from the body response in the
		WAP stack
	*/
	//##ModelId=3C4DA44F0375
	TInt iRemainingBytes;
	};


#endif // __CNWSSWSPTRHNDDATASUPPLIER_H__

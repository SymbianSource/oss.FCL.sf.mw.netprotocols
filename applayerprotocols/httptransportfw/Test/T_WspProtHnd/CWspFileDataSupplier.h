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

#ifndef __CWSPFILEDATASUPPLIER_H__
#define __CWSPFILEDATASUPPLIER_H__

// System includes
#include <f32file.h>
#include <http/mhttpdatasupplier.h>


/** Definition of class 'CWspFileDataSupplier'.
*/
class CWspFileDataSupplier : public CBase, public MHTTPDataSupplier
	{
public: // methods

	/** Factory construction: create an uninitialised data supplier
	*/
	static CWspFileDataSupplier* NewL();

	/** Factory construction: create a data supplier, to read from the specified file
	*/
	static CWspFileDataSupplier* NewL(const TDesC& aFileName);

	/** Destructor
	*/
	virtual ~CWspFileDataSupplier();

	/** Clean-up support required by the client's item store
	*/
	static void CleanUp(TAny* aDataSupplierItem);

	/** Open the specified file.  Must be called straight after construction.  If a
		previous file was open, it is closed first.
	*/
	void SetFileL(const TDesC& aFileName);

	/** Get the next data part. The supplied buffer is set to point at the data.  The returned flag indicates ETrue if no more
		data is to come, ie. aDataPart is the final buffer.
	*/
	virtual TBool GetNextDataPart(TPtrC8& aDataPart);

	/** Release the current data part being held at the data
		supplier.  This call indicates to the supplier that the part
		is no longer needed, and another one can be supplied, if
		appropriate.
	*/
	virtual void ReleaseData();

	/** Obtain the overall size of the data being supplied, if known
		to the supplier.  Where a body of data is supplied in several
		parts this size will be the sum of all the part sizes. If
		the size is not known, KErrNotFound is returned; in this case
		the client must use the return code of GetNextDataPart to find
		out when the data is complete.

		@return A size in bytes, or KErrNotFound if the size is not known.  */
	virtual TInt OverallDataSize();

	/** Reset the data supplier.  This indicates to the data supplier that it should
		return to the first part of the data.  This could be used in a situation where
		the data consumer has encountered an error and needs the data to be supplied
		afresh.  Even if the last part has been supplied (i.e. GetNextDataPart has
		returned ETrue), the data supplier should reset to the first part.
	
		If the supplier cannot reset it should return an error code; otherwise it should
		return KErrNone, where the reset will be assumed to have succeeded*/
	virtual TInt Reset();

private:

	/** Normal constructor
	*/
	CWspFileDataSupplier();

	/** Second phase construction, default.
	*/
	void ConstructL();

	/** Second phase construction, opens the named file
	*/
	void ConstructL(const TDesC& aFileName);

private:

	/** A buffer used to hold the data specified in the script, which is copied to the invoker of this API
	*/
	HBufC8* iBuffer;

	/** Handle to the file server
	*/
	RFs iFileSrvHnd;

	/** Parsed version of the filename to be read
	*/
	TParse iParsedFileName;

	/** Handle to the file being supplied by this data supplier
	*/
	RFile iFileHnd;

	/** Flag that indicates whether a file is currently open
	*/
	TBool iFileOpen;
	};


#endif // __CWSPFILEDATASUPPLIER_H__

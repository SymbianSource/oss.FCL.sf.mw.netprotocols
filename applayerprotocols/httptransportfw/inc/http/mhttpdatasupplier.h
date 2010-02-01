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
 @file
 @warning : This file contains Rose Model ID comments - please do not delete
*/

#ifndef	__MHTTPDATASUPPLIER_H__
#define	__MHTTPDATASUPPLIER_H__

// System includes
#include <e32base.h>


//##ModelId=3C4C187903E5
class MHTTPDataSupplier
/** 
A data supplier - This class is used to deliver the response data
to the client, and is also used by the client to supply request
body data to HTTP in POST transactions. Data is supplied in a
number of parts. When a part is available it can be retreived
with GetNextDataPart. The returned descriptor will remain valid until
ReleaseData is called. 

To use this class to supply POST data, you have a number of
options. If the post data needs to be URL Form encoded, you should
use CHTTPFormEncoder, which will do the encoding and interface to
the MHTTPDataSupplier for you. If you have all the data available,
return its length in OverallDataSize, and pass back all the data
from GetNextDataPart, returning ETrue to indicate that this is the
last part. 

If you don't want to form all the data at once, but know how much
you'll eventualy have, return the total length from
OverallDataSize. When GetNextDataPart is first called, return the
first part. If GetNextDataPart is called again before ReleaseData,
you should still return the first part. Only when ReleaseData is
called should you move to the second part. If you don't know the
total size of the data, the procedure is the same but you should
return KErrNotFound from OverallDataSize.

When the next part is available, clients should call
RHTTPTransaction::NotifyNewRequestBodyPartL to inform HTTP that
the new data is available. They can do this from ReleaseData if
more data is instantly available, or in some applications they may
need to call it some time later when the next part has been
assembled.

Filter writers should note that the MHTTPDataSupplier interface is
designed to be used by 1 client, as 1 component needs to know when
to call ReleaseData(). However, filters can be written to
transform the data in some way. For instance, a filter could be
written to automaticaly handle a particular content encoding. When
this filter first receives a GotResponseBodyData, it should take a
copy of the response's body and replace the body with a
MHTTPDataSupplier supplied by the filter. The filter should then
receive the data from HTTP via the saved data supplier and give it
to the client via its own data supplier.
@publishedAll
@released
*/
	{
 public:
	/** Obtain a data part from the supplier.  The data is guaranteed
		to survive until a call is made to ReleaseData().
		@param aDataPart - the data part
		@return ETrue if this is the last part. EFalse otherwise */
	//##ModelId=3C4C187A0026
	 virtual TBool GetNextDataPart(TPtrC8& aDataPart) = 0;

	/** Release the current data part being held at the data
		supplier.  This call indicates to the supplier that the part
		is no longer needed, and another one can be supplied, if
		appropriate.  */
	//##ModelId=3C4C187A0025
	virtual void ReleaseData() = 0;

	/** Obtain the overall size of the data being supplied, if known
		to the supplier.  Where a body of data is supplied in several
		parts this size will be the sum of all the part sizes. If
		the size is not known, KErrNotFound is returned; in this case
		the client must use the return code of GetNextDataPart to find
		out when the data is complete.

		@return A size in bytes, or KErrNotFound if the size is not known.  */
	//##ModelId=3C4C187A001D
	virtual TInt OverallDataSize() = 0;

	/** Reset the data supplier.  This indicates to the data supplier that it should
		return to the first part of the data.  This could be used in a situation where
		the data consumer has encountered an error and needs the data to be supplied
		afresh.  Even if the last part has been supplied (i.e. GetNextDataPart has
		returned ETrue), the data supplier should reset to the first part.
	
		If the supplier cannot reset it should return an error code; otherwise it should
		return KErrNone, where the reset will be assumed to have succeeded*/
	//##ModelId=3C4C187A001C
	virtual TInt Reset() = 0;

private:
	// Some reserved methods for future expansion (e.g. better support
	// for ZCD)
	//##ModelId=3C4C187A001B
	inline virtual void MHDS_Reserved1();
	//##ModelId=3C4C187A0012
	inline virtual void MHDS_Reserved2();
	//##ModelId=3C4C187A0011
	inline virtual void MHDS_Reserved3();
	};

inline void MHTTPDataSupplier::MHDS_Reserved1()
	{}

inline void MHTTPDataSupplier::MHDS_Reserved2()
	{}

inline void MHTTPDataSupplier::MHDS_Reserved3()
	{}

#endif // __MHTTPDATASUPPLIER_H__ 

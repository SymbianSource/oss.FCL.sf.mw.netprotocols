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

#ifndef	__THTTPFILTERHANDLE_H__
#define	__THTTPFILTERHANDLE_H__

// System includes
#include <e32std.h>


//##ModelId=3A914DF50317
class THTTPFilterHandle
/** 
A Filter registration handle. This identifies a particular filter
registration, and is passed to the filter in MHFLoad. It is used
to identify filters for deletion and for locating the start point
for sending events. It is an opaque class; all you need to be able
to do with it is to compare them for equality.
@publishedAll
@released
*/
	{
 public:
	/* Note. These are negative as values for particular filters use
       positive values.*/
	/** Alisases for the client, and the filters you're most likely to want
		to refer to.
	*/
	enum TStandardValues
		{
		EUndefined = 0, //< Undefined default value.
		EClient = -1, //< Placeholder for the client.
		EProtocolHandler = -2, //< Constant value for the protocol handler.
		// When sending an event from a MHFRunL, indicates the current filter.
		ECurrentFilter = -3 
		};
 public:
	//##ModelId=3A914DF50336
	inline THTTPFilterHandle(TStandardValues aVal = EUndefined);
	//##ModelId=3A914DF5032D
	inline TBool operator==(THTTPFilterHandle aThat) const;
	//##ModelId=3A914DF5032B
	inline TBool operator!=(THTTPFilterHandle aThat) const;
 private:
	//##ModelId=3A914DF50323
	TInt iValue;
	friend class RHTTPSession;
	friend class RHTTPFilterCollection;
	friend class CHTTPSession;
	friend class CTransaction;
	};  

inline THTTPFilterHandle::THTTPFilterHandle(TStandardValues aVal)
		: iValue(aVal)
	{
	}

inline TBool THTTPFilterHandle::operator==(THTTPFilterHandle aThat) const
	{
	return iValue == aThat.iValue;
	}

inline TBool THTTPFilterHandle::operator!=(THTTPFilterHandle aThat) const
	{
	return !(*this == aThat);
	}


#endif // __THTTPFILTERHANDLE_H__

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

#ifndef	__RHTTPFILTERCOLLECTION_H__
#define	__RHTTPFILTERCOLLECTION_H__ 

// System includes
#include <e32std.h>
#include <http/thttpevent.h>
#include <stringpool.h>

// Forward declarations
class CHTTPSession;
class RHTTPSession;
class MHTTPFilter;
class THTTPFilterIterator;


//##ModelId=3C4C187C02B3
class RHTTPFilterCollection
/** 
A Handle on the filter collection in a session. It provides
facilities for adding and deleting filters and for querying what
filters are installed. 
@publishedAll
@released
@see RHTTPSession::FilterCollection 
@see MHTTPFilter::TPositions
*/
	{
 public:
	//##ModelId=3C4C187C02F9
	inline RHTTPFilterCollection();

	//##ModelId=3C4C187C02E5
	IMPORT_C void AddFilterL(MHTTPFilter& aFilter, THTTPEvent aEvent,
							 RStringF aHeader, TInt aStatusCode, 
 							 TInt aPosition, RStringF aName);


	//##ModelId=3C4C187C02F1
	IMPORT_C void AddFilterL(MHTTPFilter& aFilter, THTTPEvent aEvent, 
 							 TInt aPosition, RStringF aName);

	//##ModelId=3C4C187C02DF
	IMPORT_C TBool CanChangeFilters() const;

	//##ModelId=3C4C187C02DD
	IMPORT_C void RemoveFilter(RStringF aFilter);

	//##ModelId=3C4C187C02DB
	IMPORT_C THTTPFilterIterator Query(RStringF aName = RStringF());

	//##ModelId=3C4C187C02D1
	inline RHTTPSession Session() const;

 private:
	friend class CHTTPSession;
	friend class RHTTPSession;

	//##ModelId=3C4C187C02C9
	CHTTPSession* iImplementation;
	};

/** Constructor
  */
inline RHTTPFilterCollection::RHTTPFilterCollection()
		: iImplementation(NULL)
	{
	}

#endif //	__RHTTPFILTERCOLLECTION_H__

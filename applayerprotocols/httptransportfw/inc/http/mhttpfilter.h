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

#ifndef __MHTTPFILTER_H__
#define __MHTTPFILTER_H__

// System includes
#include <e32std.h>
#include <http/mhttpfilterbase.h>
#include <http/rhttpsession.h>
#include <http/thttpfilterhandle.h>


//##ModelId=3C4C187D03E1
class MHTTPFilter : public MHTTPFilterBase
/** 
A HTTP filter 
@publishedAll
@released
*/
	{
 public:
	/** Called when the filter is being removed from a session's filter queue.
		@param aSession The session it's being removed from
		@param aHandle The filter handle. Complex filters may need to 
		refer to this to keep track of which particular registration is
		being unloaded.
	*/
	//##ModelId=3C4C187E0011
	IMPORT_C virtual void MHFUnload(RHTTPSession aSession, 
									THTTPFilterHandle aHandle);

	/** Called when the filter is being added to the session's filter queue.
		@param aSession The session it's being added to.
		@param aHandle The filter handle. Complex filters may need to keep
		track of this, for instance if generating events in response to
		external stimuli
	*/
	//##ModelId=3C4C187E000E
	IMPORT_C virtual void MHFLoad(RHTTPSession aSession,
								  THTTPFilterHandle aHandle);
 public:
	/** 
		@enum TPositions
		Default filter positions
	 */
	enum TPositions
		{
		/** The Protocol Handler :- At the base of the filter chain
			and nearest to the transport layer */
		EProtocolHandler	= 0,
		// Filters that should be downstream of any status-code related processing
		/** Filters to handle caching behaviours.*/
		ECache				= 100,
		/** Filters to handle particular status codes.
			e.g.Redirection and authentication. */
		EStatusCodeHandler	= 200,
		/**	The UAProf filter. */
		EUAProf				= 250,
		/** The cookie filter, and other filters that don't need to be
			concerned with resubmitting of transactions due to things
			like redirection. */
		ECookies			= 300,
		/** The validation filter and any other final tidy up or
            initial preprocessing of transactions*/
		ETidyUp = 400,
		/** Client filters that assume all standard behaviour is
            already there.*/
		EClientFilters		= 500,
		/** The client :- At the head of the filter chain : 
			No position above this must be used. */
		EClient				= 1000
		};
	}; 

#endif // __MHTTPFILTER_H__

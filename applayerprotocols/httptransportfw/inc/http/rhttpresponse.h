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
 @file RHTTPResponse.h
 @warning : This file contains Rose Model ID comments - please do not delete
*/

#ifndef __RHTTPRESPONSE_H__
#define __RHTTPRESPONSE_H__

// System includes
#include <e32std.h>
#include <http/rhttpmessage.h>
#include <stringpool.h>


//##ModelId=3A375D1203B5
class RHTTPResponse : public RHTTPMessage
/** 
An HTTP Response.  This class extends the abstract HTTP Message to add an HTTP
version, a status code and status text.
@publishedAll
@released
*/
	{
public:
	/**Get Methods
		Methods for reading information from the response.
	*/
	//@{
	/** 
		Returns the status code
	*/
	//##ModelId=3A914DF801B3
	IMPORT_C TInt StatusCode() const;
	/** Returns the status text, that is the text after the number on
        the first line of the response. */
	//##ModelId=3A914DF80195
	IMPORT_C RStringF StatusText() const;
	/**
		Returns the version of HTTP used by the server
	*/
	//##ModelId=3A914DF80163
	IMPORT_C TVersion Version() const;
	//@}
	/** Set Methods 

		Methods for setting information. These methods will not be of
		use to the client; they are intended for the use of advanced
		filters
	*/
	//@{
	/** Sets the status code.
	 */
	//##ModelId=3A3765310382
	IMPORT_C void SetStatusCode(TInt aStatus);
	// Sets the status string
	//##ModelId=3A914DF8019F
	IMPORT_C void SetStatusText(RStringF aStatusString);
	// Sets the HTTP version
	//##ModelId=3A914DF8016D
	IMPORT_C void SetVersion(TVersion aVersion);
	//@}

	friend class CResponse;
	};



#endif // __RHTTPRESPONSE_H__

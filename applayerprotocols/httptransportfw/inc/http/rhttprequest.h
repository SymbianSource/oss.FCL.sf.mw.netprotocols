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

#ifndef __RHTTPREQUEST_H__
#define __RHTTPREQUEST_H__ 

// System includes
#include <uri8.h>
#include <stringpool.h>
#include <http/rhttpmessage.h>


//##ModelId=3C4C186E0191
class RHTTPRequest : public RHTTPMessage
/** 
An HTTP Request.  This class extends the abstract HTTP Message to add an HTTP
method to be invoked on the resource at the remote HTTP server, and the URL that
identifies that resource.
@publishedAll
@released
*/
	{
 public:
	/** Get Methods
		Methods for reading information from the response.
	*/
	//@{
	// Gets the method name
	// @return The method to be used in the HTTP request.
	//##ModelId=3C4C186E01B3
	IMPORT_C RStringF Method() const;
	// Gets the URI
	// @return The URI to be used in the HTTP request.
	//##ModelId=3C4C186E01B2
	IMPORT_C const TUriC8& URI() const;
	//@}
	/**Set Methods
		
		Methods for setting information. These methods will not be of
		use to the client; they are intended for the use of advanced
		filters
	*/
	//@{
	// Sets the method name
	// @param aMethod The method name to be used in the HTTP request.
	//##ModelId=3C4C186E01B0
	IMPORT_C void  SetMethod(RStringF aMethod);
	// Sets the URI
	// @param aURI The URI to be used in the HTTP request.
	//##ModelId=3C4C186E01A6
	IMPORT_C void SetURIL(const TUriC8& aURI);
	//@}

	friend class CRequest;
	};


#endif //	__RHTTPREQUEST_H__  

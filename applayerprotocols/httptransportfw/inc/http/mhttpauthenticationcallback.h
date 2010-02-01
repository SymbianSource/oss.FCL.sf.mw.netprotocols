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
 @file MHTTPAuthenticationCallback.h
 @warning : This file contains Rose Model ID comments - please do not delete
*/

#ifndef	__MHTTPAUTHENTICATIONCALLBACK_H__
#define	__MHTTPAUTHENTICATIONCALLBACK_H__ 

// System includes
#include <uri8.h>
#include <stringpool.h>
#include <http/rhttpsession.h>
#include <http/cauthenticationfilterinterface.h>


//##ModelId=3B1E52A50151
class MHTTPAuthenticationCallback
/** 
The mixin that needs to be implemented if authentication is to be
supported. This supplies the username and password when they are
needed for authentication.
To use, subclass off this class and implement GetCredentialsL. To install
the class into a session, call InstallAuthenticationL.
@publishedAll
@released
*/
	{
 public:
	/** Gets some credentials. Note that the first 3 parameters are
		informational and many clients may not have a use for
		them. Clients may want to display some or all of these
		parameters to the user, so that the user knows who they are
		giving their credentials to, and how securely they will be
		transmitted.
		@param aURI The URI being requested
		@param aRealm The realm being requested
		@param aAuthenticationType The type of authentication (basic or digest)
		@param aUsername The username is returned in this parameter. 
		The caller will close the returned string.
		@param aPassword The password is returned in this parameter
		The caller will close the returned string.
		@return True if credentials have been obtained. 
		@leave Anything. If this function leaves the transaction will be 
		failed. 
	*/
	//##ModelId=3B1E52A5015D
	virtual TBool GetCredentialsL(const TUriC8& aURI, RString aRealm, 
								  RStringF aAuthenticationType,
								  RString& aUsername, 
								  RString& aPassword) = 0;

	/** Installs the callback into the session.
		Typicaly called from the implementation class's ConstructL().
		@pre The session is opened, but no transactions have been created on it
		@post The session supports authentication, and uses this API to
		get its credentials.
		@param aSession the session to install the filter into.
		@leave KErrNoMemory There was not enough memory.
	 */
	//##ModelId=3B1E52A5015B
	inline void InstallAuthenticationL(RHTTPSession aSession);
	};

inline void MHTTPAuthenticationCallback::InstallAuthenticationL(RHTTPSession aSession)
	{
	CAuthenticationFilterInterface::InstallFilterL(aSession, this);
	}


#endif //	__MHTTPAUTHENTICATIONCALLBACK_H__

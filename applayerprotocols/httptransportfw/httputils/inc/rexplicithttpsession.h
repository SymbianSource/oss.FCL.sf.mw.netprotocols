// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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
 @publishedPartner
 @released
*/

#ifndef REXPLICITHTTPSESSION_H_
#define REXPLICITHTTPSESSION_H_

#include <http.h>
#include <es_sock.h>


/**
RExplicitHTTPSession is a wrapper around the RHTTPSession. 
This handle provides features which allows the application to prompt the user only once 
for the selection of an IAP during muliple connections within a session.

@publishedPartner
@released
*/
class RExplicitHTTPSession
	{
public:
    IMPORT_C RExplicitHTTPSession();
    IMPORT_C void OpenL(const TUriC8& aUri, TUint32 aIapNumber = 0, TBool aStartConnection = ETrue);
    IMPORT_C void Close();

    IMPORT_C void StartConnection(TRequestStatus& aStatus);
    IMPORT_C void CancelStart();
 
    IMPORT_C RHTTPSession& HTTPSession();
    inline RHTTPSession& operator()();

private:
    void BindConnectionL(TUint32 aIap, TBool aStartConnection);
    // Returns ETrue if there is a proxy to use for the given scheme; 
    // aProxyServer gets set
    TBool UseProxyL(const TDesC8& aScheme, HBufC8*& aProxyServer);
     
private:
    RSocketServ iSocketServ;
    RConnection iConnection;
    RHTTPSession iHTTPSession;
    TUint32 iIap;
    };

// inline methods
inline RHTTPSession& RExplicitHTTPSession::operator()()
    {
    return HTTPSession();
    }

#endif /*REXPLICITHTTPSESSION_H_*/

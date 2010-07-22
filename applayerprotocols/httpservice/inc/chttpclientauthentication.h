// Copyright (c) 2003-2010 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef __CHTTPCLIENTAUTHENTICATION_H__
#define __CHTTPCLIENTAUTHENTICATION_H__

#include <e32base.h>
#include <http/mhttpauthenticationcallback.h>
class MHTTPServiceAuthentication;

NONSHARABLE_CLASS(CHttpClientAuthentication) : public CBase, public MHTTPAuthenticationCallback
    {
public:
    static CHttpClientAuthentication* New(RHTTPSession aSesion, MHTTPServiceAuthentication* aCallback);

// methods inherited from MHTTPAuthenticationCallback
    virtual TBool GetCredentialsL(const TUriC8& aURI, RString aRealm, 
                                 RStringF aAuthenticationType,
                                 RString& aUsername, 
                                 RString& aPassword);

protected:
    TInt Construct(RHTTPSession aSesion, MHTTPServiceAuthentication* aCallback);
    void FillCredentialsL(RString& aRealm, RString& aUsername, RString& aPassword, RBuf8& aClientUsername, RBuf8& aClientPassword);
private:
    MHTTPServiceAuthentication* iHTTPServiceAuthentication;    
    };

#endif  __CHTTPCLIENTAUTHENTICATION_H__

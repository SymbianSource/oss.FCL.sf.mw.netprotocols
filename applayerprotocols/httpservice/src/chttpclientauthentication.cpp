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

#include "chttpclientauthentication.h"
#include "mhttpserviceauthentication.h"

CHttpClientAuthentication* CHttpClientAuthentication::New(RHTTPSession aSesion, MHTTPServiceAuthentication* aCallback)
    {
    CHttpClientAuthentication* self = new CHttpClientAuthentication;
    TInt err = self->Construct(aSesion, aCallback);
    if(err != KErrNone)
        {
        delete self;
        self = NULL;
        }
    return self;
    }

TInt CHttpClientAuthentication::Construct(RHTTPSession aSession, MHTTPServiceAuthentication* aCallback)
    {      
    // Install this class as the callback for authentication requests
    TRAPD(err, InstallAuthenticationL(aSession));
    iHTTPServiceAuthentication = aCallback;
    return err;
    }

void CHttpClientAuthentication::FillCredentialsL(RString& aRealm, RString& aUsername, RString& aPassword, RBuf8& aClientUsername, RBuf8& aClientPassword)
    {  
    aUsername = aRealm.Pool().OpenStringL(aClientUsername);
    aPassword = aRealm.Pool().OpenStringL(aClientPassword);
    }

TBool CHttpClientAuthentication::GetCredentialsL(const TUriC8& aURI, RString aRealm, 
                                   RStringF aAuthenticationType,
                                   RString& aUsername, 
                                   RString& aPassword)
    {
    //User credentials
    RBuf8 clientUsername;
    RBuf8 clientPassword;    
     
    User::LeaveIfError(iHTTPServiceAuthentication->OnAuthentication(aURI.UriDes(), aRealm.DesC(), aAuthenticationType.DesC(), clientUsername, clientPassword));
    
    FillCredentialsL(aRealm, aUsername, aPassword, clientUsername, clientPassword);
    
    clientUsername.Close();
    clientPassword.Close();    
    
    return ETrue;    
    }

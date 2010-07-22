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
#include <e32base.h>
#include <f32file.h>  
#include "ctesthttpserviceauthentication.h"

_LIT(KAuthURL, "Passing credentials for URL %S, realm %S\n");
_LIT(KAuthType, "Using %S authentication\n");
  
CTestHttpServiceAuthentication::~CTestHttpServiceAuthentication()
    {  
    }
    
CTestHttpServiceAuthentication::CTestHttpServiceAuthentication()
    {    
    }
    

// From MHTTPServiceAuthentication
TInt CTestHttpServiceAuthentication::OnAuthentication(const TDesC8& /*aUri*/, const TDesC8& /*aRealm*/, const TDesC8& /*aType*/, RBuf8& aUsername, RBuf8& aPassword)
    {
    //User credentials
    _LIT8(KUserName,  "username");
    _LIT8(KPassword,  "password");
   
    aUsername.Create(KUserName);
    aPassword.Create(KPassword);
    return KErrNone;
    }
 

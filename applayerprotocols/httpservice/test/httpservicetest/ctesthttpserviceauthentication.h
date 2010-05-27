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

#ifndef __CTESTHTTPSERVICEAUTHENTICATION_H__
#define __CTESTHTTPSERVICEAUTHENTICATION_H__

#include <e32base.h>
#include "mhttpserviceauthentication.h"

class CTestHttpServiceAuthentication : public CBase, public MHTTPServiceAuthentication
    {
public:
    CTestHttpServiceAuthentication();
    ~CTestHttpServiceAuthentication();
      
    // inherited from MHTTPServiceAuthentication
    TInt OnAuthentication(const TDesC8& aUri, const TDesC8& aRealm, const TDesC8& aType, RBuf8& aUsername, RBuf8& aPassword);
     
private:

    };

#endif // __CTESTHTTPSERVICEAUTHENTICATION_H__

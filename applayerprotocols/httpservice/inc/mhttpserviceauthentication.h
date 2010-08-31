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

/**
 @file MHTTPServiceAuthentication.h
 */

#ifndef __MHTTPSERVICEAUTHENTICATION_H__
#define __MHTTPSERVICEAUTHENTICATION_H__ 

// System includes
#include <e32base.h>

class MHTTPServiceAuthentication
/** 
The mixin that needs to be implemented if authentication is to be
supported. This supplies the username and password when they are
needed for authentication.

@publishedAll
@released
*/
    {
 public:
     virtual TInt OnAuthentication(const TDesC8& aUri, const TDesC8& aRealm, const TDesC8& aType, RBuf8& aUsername, RBuf8& aPassword) = 0;
    };

#endif //   __MHTTPSERVICEAUTHENTICATION_H__

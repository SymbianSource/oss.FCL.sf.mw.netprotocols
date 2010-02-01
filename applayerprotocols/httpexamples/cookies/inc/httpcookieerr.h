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

#ifndef __HTTPCOOKIEERR_H__
#define __HTTPCOOKIEERR_H__

#include <e32std.h>
#include <e32base.h>

//##ModelId=3B1E52A5010B
class HTTPCookiePanic
	{
public:
	enum THTTPCookiePanic
		{
		EInvalidFilterState,
		EGetCookiesArrayNotEmpty
		};

	//##ModelId=3B1E52A50115
	static void Panic(THTTPCookiePanic aPanic);
	};

inline void HTTPCookiePanic::Panic(HTTPCookiePanic::THTTPCookiePanic aPanic)
	{
	_LIT(KHTTPCookiePanic,"HTTP-COOKIE");
	User::Panic(KHTTPCookiePanic,aPanic);
	}

#endif // __HTTPCOOKEIERR_H__


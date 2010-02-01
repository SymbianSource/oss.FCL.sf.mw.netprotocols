// Copyright (c) 2000-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef	 	__CURL_H__
#define		__CURL_H__

#include <e32base.h>

class CUrl
	{
public:

	//	Enum of component parts of the Url.
	//
	enum TComponent
		{	
		EUrlScheme = 0x01,
		EUrlLocation = 0x02,
		EUrlPath = 0x04,
		EUrlQuery = 0x08,
		EUrlFragment = 0x10,
		EUrlFileName = 0x20, // the end of the path
		EUrlUsername = 0x40,
		EUrlPassword = 0x80,
		EUrlAuthority = EUrlLocation | EUrlUsername | EUrlPassword,
		EUrlNoCredentials = EUrlScheme | EUrlLocation | EUrlPath | EUrlQuery |
			EUrlFragment,
		EUrlGenericCompare = EUrlScheme | EUrlAuthority | EUrlPath | EUrlQuery
		};

public:	
	IMPORT_C static HBufC* EscapeDecodeL(const TDesC& aString);
	IMPORT_C static HBufC* EscapeEncodeL(const TDesC& aString, TInt aEscapeMode);
	
private:	
	IMPORT_C void Stub7();
	IMPORT_C void Stub8();
	IMPORT_C void Stub9();
	IMPORT_C void Stub10();
	IMPORT_C void Stub11();
	IMPORT_C void Stub12();
	IMPORT_C void Stub13();
	IMPORT_C void Stub14();
	IMPORT_C void Stub15();
	IMPORT_C void Stub16();
	IMPORT_C void Stub18();
	IMPORT_C void Stub19();
	IMPORT_C void Stub20();
	IMPORT_C void Stub21();
	};




#endif 		 // __CURL_H__

// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __HTTPCLIENTUTILS_H__
#define __HTTPCLIENTUTILS_H__

#include "thttpheadervaluevariant.h"
#include <thttphdrval.h>
class RStringPool;
class RStringF;

class HttpClientUtils
	{
public:
  static THttpHeaderValueVariant CopyHttpHdrVal(const THTTPHdrVal& aVal);
  static THTTPHdrVal CopyHttpHdrVal(const THttpHeaderValueVariant& aVal, RStringPool& aPool);
  static TInt OpenStringF(const TDesC8& aDesc, RStringPool& aPool, RStringF& aStringF);
  static void CloseString(const THTTPHdrVal& aVal);	
	};
#endif // __HTTPCLIENTUTILS_H__

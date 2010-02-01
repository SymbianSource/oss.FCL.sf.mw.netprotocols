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

#ifndef __CWSPHEADERUTILS_H__
#define __CWSPHEADERUTILS_H__

#include <e32base.h>
#include <stringpool.h>

#include <http/rhttpheaders.h>
#include "cwspheadercodec.h"

// @todo docing

//##ModelId=3C022B5D02CD
class CWspHeaderUtils : public CBase
	{
public:	// Methods

	//##ModelId=3C4C438502DE
	static CWspHeaderUtils* NewL(CWspHeaderCodec& aCodec);

	//##ModelId=3C4C438502DD
	~CWspHeaderUtils();

	//##ModelId=3C022B6E01CD
	HBufC8* EncodeHeadersL(RStringPool aStringPool, RHTTPHeaders aHeaders);

	//##ModelId=3C4C438502B5
	HBufC8* EncodeNoTrailerHeadersL(RStringPool aStringPool, RHTTPHeaders aHeaders, HBufC8*& iTrailerData);

	//##ModelId=3C4C4385028D
	void DecodeReplyHeadersL(RStringPool aStringPool, const TDesC8& aEncodedData, RHTTPHeaders& aHeaders);

	//##ModelId=3C4C43850265
	void DecodeHeadersL(RStringPool aStringPool, const TDesC8& aEncodedData, RHTTPHeaders& aHeaders);

private:	// Methods

	//##ModelId=3C4C43850252
	CWspHeaderUtils(CWspHeaderCodec& aCodec);

	//##ModelId=3C4C43850233
	TBool EncodeContentTypeL(RStringPool aStringPool, RHTTPHeaders aHeaders, HBufC8*& aBuf);

	//##ModelId=3C4C43850215
	void EncodeHeaderFieldL(RHTTPHeaders aHeaders, RStringF aHeaderField, HBufC8*& aBuf);

	//##ModelId=3C4C43850201
	TBool IsTrailerHeader(RArray<RStringF> aTrailerHeaders, RStringF aHeaderField);

private:	// Attributes

	//##ModelId=3C4C438501F9
	CWspHeaderCodec& iCodec;
	};

#endif	// __CWSPHEADERUTILS_H__

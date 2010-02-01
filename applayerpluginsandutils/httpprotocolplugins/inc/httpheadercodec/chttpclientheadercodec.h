// Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __CHTTPCLIENTHEADERCODEC_H__
#define __CHTTPCLIENTHEADERCODEC_H__


#include <stringpool.h>
#include <http/framework/cheadercodecplugin.h>


/**	
	Codec for Client header fields, as defined in RFC2616 : HTTP/1.1 
	In general this means encoding the Request fields and decoding the Response fields.	
	@internalComponent
	@todo
*/
class CHttpClientHeaderCodec : public CHeaderCodecPlugin
	{
public:	// methods

	static CHttpClientHeaderCodec* NewL(TAny* aStringPool);
	
	virtual ~CHttpClientHeaderCodec();

private:	// methods from CHeaderCodec

	virtual TBool CanEncode(RStringF aHeaderField) const;

	virtual TBool CanDecode(RStringF aHeaderField) const;

	virtual CHeaderCodec* FindDelegateCodecL(RStringF aHeaderField) const;

private:	// methods

	CHttpClientHeaderCodec(RStringPool aStringPool);
	
	void ConstructL();

private:	// attributes

	RStringPool		iStringPool;
	const TStringTable& iStringTable;
	};

#endif	// __CHTTPCLIENTHEADERCODEC_H__

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

#ifndef __CHTTPSERVERHEADERCODEC_H__
#define __CHTTPSERVERHEADERCODEC_H__

#include <e32base.h>
#include <http/framework/cheadercodecplugin.h>

/**	
	Codec for Server header fields, as defined in RFC2616 : HTTP/1.1
	In general this means encoding the Response fields and decoding the Request fields.
	@internalComponent
*/
class CHttpServerHeaderCodec : public CHeaderCodecPlugin
	{
public: // methods

	static CHttpServerHeaderCodec* NewL(TAny* aStringPool);

	virtual ~CHttpServerHeaderCodec();

private:	// methods from CHeaderCodec

	virtual TBool CanEncode(RStringF aHeaderField) const;

	virtual TBool CanDecode(RStringF aHeaderField) const;

	virtual CHeaderCodec* FindDelegateCodecL(RStringF aHeaderField) const;

private:	// methods

	CHttpServerHeaderCodec(RStringPool aStringPool);

	void ConstructL();

private:	// attributes

	RStringPool		iStringPool;

	};

#endif	// __CHTTPSERVERHEADERCODEC_H__

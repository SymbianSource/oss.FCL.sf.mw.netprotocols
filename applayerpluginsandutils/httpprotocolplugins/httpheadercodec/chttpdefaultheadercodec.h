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

#ifndef __CHTTPDEFAULTHEADERCODEC_H__
#define __CHTTPDEFAULTHEADERCODEC_H__

#include <http/framework/cheadercodecplugin.h>

/** A default codec that will be instantiated as a delegate codec to handle any unrecognised
	header field by copying directly between raw data or string header field values.
	@todo
*/
class CHttpDefaultHeaderCodec : public CHeaderCodecPlugin
	{
public:

	static CHttpDefaultHeaderCodec* NewL(TAny* aStringPool);

	virtual ~CHttpDefaultHeaderCodec();

private:	// methods from CHeaderCodec

	virtual TBool CanEncode(RStringF aHeaderField) const;

	virtual TBool CanDecode(RStringF aHeaderField) const;

	virtual CHeaderCodec* FindDelegateCodecL(RStringF aHeaderField) const;

private:	// methods

	CHttpDefaultHeaderCodec(RStringPool aStrPool);

	void ConstructL();

private:	// attributes

	RStringPool		iStringPool;

	};

#endif	// __CHTTPDEFAULTHEADERCODEC_H__

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

#ifndef __CHTTPGENERALHEADERCODEC_H__
#define __CHTTPGENERALHEADERCODEC_H__

#include <e32base.h>
#include <stringpool.h>
#include <http/framework/cheadercodecplugin.h>

/**	
	The CHttpGeneralHeaderCodec class implements the encoding and decoding of 
	the HTTP General and Entity headers as specified in RFC 2616.
	@internalComponent
	@see		CHeaderCodec
*/
class CHttpGeneralHeaderCodec : public CHeaderCodecPlugin
	{
public:	// methods

	static CHttpGeneralHeaderCodec* NewL(TAny* aStringPool);

	virtual ~CHttpGeneralHeaderCodec();

private:	// methods from CHeaderCodec

	virtual TBool CanEncode(RStringF aHeaderField) const;

	/** CanDecode()
		@param aHeaderField	header field being checked
		@return ETrue if able to decode, else EFalse. 
		Query function checking capability of this codec.
	*/
	virtual TBool CanDecode(RStringF aHeaderField) const;

	/** FindDelegateCodecL()
		@param aHeaderField	header field name passed in
		@return CHeaderCodec* pointer to the delegate codec if one, else NULL. 
	*/
	virtual CHeaderCodec* FindDelegateCodecL(RStringF aHeaderField) const;

private:	// methods

	CHttpGeneralHeaderCodec(RStringPool aStringPool);

	void ConstructL();

private:	// attributes

	RStringPool		iStringPool;

	const TStringTable&        iStringTable;
	};

#endif	// __CHTTPGENERALHEADERCODEC_H__

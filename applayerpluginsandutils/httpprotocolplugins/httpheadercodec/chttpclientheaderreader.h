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

#ifndef __CHTTPCLIENTHEADERREADER_H__
#define __CHTTPCLIENTHEADERREADER_H__

#include "chttpheaderreader.h"

/**	@class			CClientHeaderReader
	@internalComponent
	@todo
*/
class CHttpClientHeaderReader : public CHttpHeaderReader
	{
public: // methods

	static CHttpClientHeaderReader* NewL(RStringPool aStrPool);
	
	virtual ~CHttpClientHeaderReader();

private:	// methods for CHeaderReader

	virtual void DecodeHeaderL(RHeaderField& aHeader);

private: // methods

	CHttpClientHeaderReader(RStringPool aStrPool);

	void DecodeAge(RHeaderField& aHeader) const;
	
	void DecodeEtag(RHeaderField& aHeader) const;
	
	void DecodeVary(RHeaderField& aHeader) const;
	
	void DecodeExpires(RHeaderField& aHeader) const;
	
	void DecodeLastModified(RHeaderField& aHeader) const;
	
	void DecodeWWWAuthenticateL(RHeaderField& aHeader) const;

	void DecodeSetCookieL(RHeaderField& aHeader) const;
	
	void DecodeOneCookieL(RHeaderField& aHeader, const TDesC8& aCookieData, TInt aCookieNumber) const;
	
	void SetCookieNameAndValueL(CHeaderFieldPart& aCookie, const TDesC8& aNameValue) const;
	
	};

#endif	// __CHTTPCLIENTHEADERREADER_H__

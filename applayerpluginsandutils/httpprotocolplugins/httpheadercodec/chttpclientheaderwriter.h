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
// CHttpClientHeaderWriter.cpp
// 
//

#ifndef __CHTTPCLIENTHEADERWRITER_H__
#define __CHTTPCLIENTHEADERWRITER_H__

#include <e32base.h>
#include <stringpool.h>
#include "timerlogger.h"

#include "chttpheaderwriter.h"

class CHeaderFieldParam;

/**	@class			CHttpClientHeaderWriter
	@internalComponent

*/
class CHttpClientHeaderWriter : public CHttpHeaderWriter
	{
public:	// methods

	static CHttpClientHeaderWriter* NewL(RStringPool aStringPool);

	virtual ~CHttpClientHeaderWriter();

private:	// methods from CHeaderWriter

	virtual void EncodeHeaderL(RHeaderField& aHeader);

private:	// methods

	CHttpClientHeaderWriter(RStringPool aStringPool);

	void EncodeAcceptL(RHeaderField& aHeader) const;

	void EncodeAcceptCharsetL(RHeaderField& aHeader) const;
	
	void EncodeAcceptEncoding(RHeaderField& aHeader) const;
	
	void EncodeAcceptLanguageL(RHeaderField& aHeader) const;
	
	void EncodeAuthorizationL(RHeaderField& aHeader) const;
	
	void EncodeExpectL(RHeaderField& aHeader) const;
	
	void EncodeFromL(RHeaderField& aHeader) const;
	
	void EncodeHostL(RHeaderField& aHeader) const;
	
	void EncodeRangeL(RHeaderField& aHeader) const;
	
	void EncodeRefererL(RHeaderField& aHeader) const;
	
	void EncodeTEL(RHeaderField& aHeader) const;
	
	void EncodeUserAgentL(RHeaderField& aHeader) const;
	
	void EncodeCookieL(RHeaderField& aHeader) const;
	
	void EncodeOneCookieAttributeL(RHeaderField& aHeader, const CHeaderFieldParam& aAttribute) const;
	
	void EncodeOneCookieL(RHeaderField& aHeader, const CHeaderFieldPart& aCookieFieldPart, TBool aIsNetscapeStyleCookie) const;

	__DECLARE_PERFORMANCE_LOG
	};

#endif	// __CHTTPCLIENTHEADERWRITER_H__

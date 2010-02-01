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

#ifndef __CHTTPSERVERHEADERREADER_H__
#define __CHTTPSERVERHEADERREADER_H__

#include <e32base.h>

#include "chttpheaderreader.h"

/**	
	@internalComponent
*/
class CHttpServerHeaderReader : public CHttpHeaderReader
	{
public:	// methods

	static CHttpServerHeaderReader* NewL(RStringPool aStringPool);
	virtual ~CHttpServerHeaderReader();

private:	// methods from CHeaderReader

	virtual void DecodeHeaderL(RHeaderField& aHeader);

private: // methods

	CHttpServerHeaderReader(RStringPool aStringPool);
	
	void DecodeAuthorizationL(RHeaderField& aHeader) const;
	void DecodeExpectL(RHeaderField& aHeader) const;
	void DecodeFromL(RHeaderField& aHeader) const;
	void DecodeHostL(RHeaderField& aHeader) const;
	void DecodeRangeL(RHeaderField& aHeader) const;
	void DecodeRefererL(RHeaderField& aHeader) const;
	void DecodeUserAgentL(RHeaderField& aHeader) const;
	void DecodeTEL(RHeaderField& aHeader) const;
	void DecodeGenericAcceptL(RHeaderField& aHeader, TInt aErrorCode) const;
	};

#endif	// __CHTTPSERVERHEADERREADER_H__

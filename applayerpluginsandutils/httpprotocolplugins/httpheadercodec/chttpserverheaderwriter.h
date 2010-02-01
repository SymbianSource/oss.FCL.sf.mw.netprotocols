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

#ifndef __CHTTPSERVERHEADERWRITER_H__
#define __CHTTPSERVERHEADERWRITER_H__

#include <e32base.h>

#include "chttpheaderwriter.h"

/**	
	@internalComponent
*/
class CHttpServerHeaderWriter : public CHttpHeaderWriter
	{
public:	// methods

	static CHttpServerHeaderWriter* NewL(RStringPool aStringPool);

	virtual ~CHttpServerHeaderWriter();

private:	// methods from CHeaderWriter

	virtual void EncodeHeaderL(RHeaderField& aHeader);

private:	// methods

	CHttpServerHeaderWriter(RStringPool aStringPool);

	void EncodeWWWAuthenticateL(RHeaderField& aHeader) const;
	};

#endif	// __CHTTPSERVERHEADERWRITER_H__

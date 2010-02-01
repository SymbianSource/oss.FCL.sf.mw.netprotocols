// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __WBTOKEN_H
#define __WBTOKEN_H

#include <e32std.h>
#include <s32stor.h>

//
// CWbxmlToken
// WBXML token to WML ascii string
//
//##ModelId=3B6678E00090
class CWbxmlToken : public CBase
{
public :
	//##ModelId=3B6678E000B1
	CWbxmlToken();
	//##ModelId=3B6678E000B0
	~CWbxmlToken();
	//##ModelId=3B6678E000AE
	void SetTextL(const HBufC8& aText);
public :
	//##ModelId=3B6678E000A6
	HBufC8* iBuf;
};

#endif // __WBTOKEN_H

//
// End of file
//

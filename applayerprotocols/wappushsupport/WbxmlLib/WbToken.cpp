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

#include "WbToken.h"

//
// class CWbxmlToken : public CBase
// Helper for bytecode to ascii converter
//

CWbxmlToken::CWbxmlToken()
{
	__DECLARE_NAME(_S("CWbxmlToken"));
}

CWbxmlToken::~CWbxmlToken()
{
	delete iBuf;
}

void CWbxmlToken::SetTextL(const HBufC8& aText)
{
	if (iBuf)
	{
		delete iBuf;
		iBuf = NULL;
	}

	iBuf = aText.AllocL();
}

//
// End of file
//

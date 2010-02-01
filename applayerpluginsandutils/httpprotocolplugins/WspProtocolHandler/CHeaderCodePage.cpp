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

#include "CHeaderCodePage.h"

CHeaderCodePage* CHeaderCodePage::NewL(TUint8 aPageCode, const TDesC8& aPageName)
	{
	CHeaderCodePage* self = new (ELeave) CHeaderCodePage(aPageCode);
	CleanupStack::PushL(self);
	self->ConstructL(aPageName);
	CleanupStack::Pop(self);
	return self;
	}

CHeaderCodePage::~CHeaderCodePage()
	{
	delete iName;
	}

CHeaderCodePage::CHeaderCodePage(TUint8 aPageCode)
: CBase(), iCode(aPageCode)
	{
	}

void CHeaderCodePage::ConstructL(const TDesC8& aPageName)
	{
	iName = aPageName.AllocL();
	}

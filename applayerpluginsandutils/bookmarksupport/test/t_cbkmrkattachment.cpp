// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "bkmrkattachment.h"
#include "repository.h"
#include "testutils.h"

LOCAL_D CTestWrapper* gTestWrapper;

_LIT(KFilename, "t_cbkmrkattachment.cpp");
_LIT8(KDataIcon,  "123456789");
_LIT8(KDataIcon2, "different4436");

LOCAL_D void AttachmentTestsL()
	{
	CRepository* iconRepository = CRepository::NewL(KUidIconRepository);
	CBkmrkAttachment* newIcon = CBkmrkAttachment::NewL(*iconRepository, KDataIcon);
	
	gTestWrapper->TEST(newIcon->Size() == KDataIcon().Size());
	gTestWrapper->TEST(newIcon->Data().Compare(KDataIcon) == 0);
	gTestWrapper->TEST(newIcon->Size() != KDataIcon2().Size());
	gTestWrapper->TEST(newIcon->Data().Compare(KDataIcon2) != 0);
	
	newIcon->SetDataL(KDataIcon2);
	
	gTestWrapper->TEST(newIcon->Size() == KDataIcon2().Size());
	gTestWrapper->TEST(newIcon->Data().Compare(KDataIcon2) == 0);
	gTestWrapper->TEST(newIcon->Size() != KDataIcon().Size());
	gTestWrapper->TEST(newIcon->Data().Compare(KDataIcon) != 0);
	
	delete newIcon;
	delete iconRepository;
	}

void DoAttachmentUnitL(CTestWrapper* aTest)
	{
	gTestWrapper = aTest;
	gTestWrapper->SetFile(KFilename);
	gTestWrapper->Next(_L("*** Attachment tests ***"));
	
	AttachmentTestsL();
	}

// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "tbmoomtester.h"
#include "tbmtests.h"
#include "testutils.h"
#include <e32test.h>
#include <centralrepository.h>

const TUid KUidBookmarkDatabaseRepository   = { 0x10205AF8 };
const TUid KUidFolderRepository             = { 0x10205AF9 };
const TUid KUidBookmarkRepository           = { 0x10205AFA };
const TUid KUidIconRepository               = { 0x10205AFB };


CBmOomTester* CBmOomTester::NewL ( CTestWrapper& aTest )
	{
	CBmOomTester* self = new (ELeave) CBmOomTester ( aTest );
	CleanupStack::PushL ( self );
	self->ConstructL ();
	CleanupStack::Pop ( self );
	return self;
	}

CBmOomTester::CBmOomTester ( CTestWrapper& aTest ) 
: iTestWrapper ( aTest )
	{
	
	}
	
CBmOomTester::~CBmOomTester ()
	{
	delete iBmTester;
	}

void CBmOomTester::TestL ()
	{
	iBmTester->OpenDbL ();
	iBmTester->SetBkmrkDbPropertiesL ();
	iBmTester->CreateCustomPropertiesL ();
	iBmTester->CreateTreeL ();
	iBmTester->CommitDbL  ();
	iBmTester->CloseDb ();	
	iBmTester->OpenDbL ();
	iBmTester->SetCustomPropertiesValueL ();
	iBmTester->UpdateBkmrkDbPropertiesL ();	
	iBmTester->ModifyTreeL ();
	iBmTester->CommitDbL  ();
	iBmTester->CloseDb ();
	iBmTester->OpenDbL ();
	iBmTester->ReadCustomPropertiesValueL ();
	iBmTester->UpdateTreeL ();	
	iBmTester->DeleteAllL ();
	iBmTester->CommitDbL  ();
	iBmTester->UpdateTreeL ();	
	iBmTester->CloseDb ();
	}

void CBmOomTester::ResetBkmrkDbL ()
	{
	ResetRepositoryL ( KUidBookmarkDatabaseRepository );
	ResetRepositoryL ( KUidFolderRepository );
	ResetRepositoryL ( KUidBookmarkRepository );
	ResetRepositoryL ( KUidIconRepository );
	}
	
void CBmOomTester::ConstructL ()
	{
	ResetBkmrkDbL ();
	iBmTester = CBmTests::NewL ( iTestWrapper );
	}

void CBmOomTester::ResetRepositoryL ( TUid aUid )
	{
	CRepository* repository = CRepository::NewL ( aUid );
	CleanupStack::PushL ( repository );
	User::LeaveIfError ( repository->Reset() );	
	CleanupStack::PopAndDestroy ( repository );
	}


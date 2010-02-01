// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "sessmgr.h"   // CSessionManager
#include "srvparams.h" // KServerName
#include "srvres.h"	   // TServerResources
#include "srvrepos.h"  // CServerRepository
#include <bautils.h>   // BaflUtils::FileExists()
#include <bacline.h>   // CCommandLineArguments
#include "backup.h"	   // CRepositoryBackupClient
#include "install.h"   // SWI watcher

enum
	{
	ESoftReset = 0x01,
	} TStartupOptions;

_LIT(KSoftReset, "--SoftReset");

static void ParseCmdLineOptions(TInt& aOptions)
	{
	CCommandLineArguments* args = CCommandLineArguments::NewLC();

	for (TInt i = 1; i < args->Count(); ++i)
		{
		if (args->Arg(i).Compare(KSoftReset) == 0)
			{
			aOptions |= ESoftReset;
			}
		}

	CleanupStack::PopAndDestroy(args);
	}

static void CloseTServerResources(TAny*)
	{
	// Ready to exit.
	TServerResources::Close();
	}

//
// Perform all server initialisation, in particular creation of the
// scheduler and server and then run the scheduler
//
static void RunServerL()
	{
	TInt options = 0;

	ParseCmdLineOptions(options);

	// NOTE: Insert TraceHeap install here,
	//		 when RAllocator is available.
	//
	// naming the server thread after the server helps to debug panics
	User::LeaveIfError(User::RenameThread(KServerName));

	// create and install the active scheduler we need
	CActiveScheduler* s=new(ELeave) CActiveScheduler;
	CleanupStack::PushL(s);
	CActiveScheduler::Install(s);

	TServerResources::InitialiseL();

	CleanupStack::PushL(TCleanupItem(CloseTServerResources, 0));

	CSessionManager::NewLC();

	CCentRepSWIWatcher* swiWatcher = 0;
		
	if( TServerResources::iInstallDirectory)
		{		
		swiWatcher = CCentRepSWIWatcher::NewL(TServerResources::iFs);	
		CleanupStack::PushL(swiWatcher) ;
		swiWatcher->Start();
		}

	CRepositoryBackupClient* backupClient =
		CRepositoryBackupClient::NewL(TServerResources::iFs);

	CleanupStack::PushL(backupClient) ;
	backupClient->StartL();

	// check command line options
	if (options & ESoftReset)
		{
		CServerRepository::ResetRepositoriesL();
		}

	// Initialisation complete, now signal the client
#ifdef __MYSERVER_NO_PROCESSES__
	RThread::Rendezvous(KErrNone);
#else
	RProcess::Rendezvous(KErrNone);
#endif

	// Ready to run
	CActiveScheduler::Start();

	// Delete backup client if it exists
	if(backupClient)
		CleanupStack::PopAndDestroy(backupClient);

	// Delete file watcher if it exists
	if(swiWatcher)
		CleanupStack::PopAndDestroy(swiWatcher);

	// Ready to exit.

	TServerResources::Close();

	// Cleanup the server and scheduler
	CleanupStack::PopAndDestroy(2);//CSessionManager, s
	}

// Entry point for the server
TInt E32Main()
	{
	__UHEAP_MARK;

	CTrapCleanup* cleanup = CTrapCleanup::New();
	TInt r = KErrNoMemory;
	if(cleanup)
		{
		TRAP(r, RunServerL());
		delete cleanup;
		}

	__UHEAP_MARKEND;

	return r;
	}

#ifdef __MYSERVER_NO_PROCESSES__

// WINS thread entry-point function.
static TInt ThreadFunction(TAny*)
	{
	return E32Main();
	}

//WINS DLL entry-point, returns the real thread function cast to TInt
IMPORT_C TInt WinsMain();
EXPORT_C TInt WinsMain()
	{
	return reinterpret_cast<TInt>(&ThreadFunction);
	}

TInt E32Dll(TDllReason)
	{
	return KErrNone;
	}

#endif //__MYSERVER_NO_PROCESSES__

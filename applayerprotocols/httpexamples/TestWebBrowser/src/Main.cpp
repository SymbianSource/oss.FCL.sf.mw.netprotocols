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

#include <e32base.h>

#include <http.h>
#include <chttpformencoder.h>
#include "httpexampleutils.h"
#include "testwebbrowser.h"

// Create a test object, invoke the tests using it and remove
LOCAL_D void TestL()
	{
	// Start C32 and initalize some device drivers. This is necessary when running a test console as these won't 
	// have been started
	CHttpExampleUtils::InitCommsL();
	
	CHttpExampleUtils* httpUtils = CHttpExampleUtils::NewL ( _L ( "Test web browser" ) );
	CleanupStack::PushL ( httpUtils );

		
	// create an active scheduler to use
	CActiveScheduler* scheduler = new(ELeave) CActiveScheduler();
	CleanupStack::PushL( scheduler );
	CActiveScheduler::Install( scheduler );
	
	
	// Create and start the web browser	
	CTestWebBrowser* webBrowser = CTestWebBrowser::NewLC ( *httpUtils );
	CActiveScheduler::Start ();
	

	CleanupStack::Pop ( webBrowser ); // Pop and destroy webBrowser
	delete webBrowser;
	CleanupStack::Pop ( scheduler );  // Pop and destroy scheduler
	delete scheduler;
	CleanupStack::Pop ( httpUtils );  // pop and destroy httpUtils
	delete httpUtils;
	}


// Main program - run the tests within a TRAP harness, reporting any errors that
// occur via the panic mechanism. Test for memory leaks using heap marking.
GLDEF_C TInt E32Main()
	{

	__UHEAP_MARK;
	CTrapCleanup* tc = CTrapCleanup::New();
	TRAPD( err,TestL() );
	if ( err != KErrNone )
		User::Panic( _L( "Test failed with error code: %i" ), err );
	delete tc;
	__UHEAP_MARKEND;
	return KErrNone;
	}







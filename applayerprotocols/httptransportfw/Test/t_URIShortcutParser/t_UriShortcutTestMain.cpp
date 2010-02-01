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
// This is the main file to select between TestShortcutParser(Manual Test) or the
// t_URIShortcutParser (Automated Test). This has a main function DoTestsL which will 
// perform the Manual tests or automated tests according to the macro MANUAL. For running 
// the Manual tests leave the macro as it is while for running the Automated tests ,comment
// the macro in the  main.h file. 
// 
//

#include <e32base.h>
#include <e32cons.h>
#include <coecntrl.h>
#include <e32test.h>
#include <ecom/ecom.h>

#include "t_UriShortcutTestMain.h"


GLDEF_C TInt E32Main()
	{
	__UHEAP_MARK;
	CTrapCleanup* tc=CTrapCleanup::New();
	
	TInt err;
	TRAP(err,DoTestsL());
	
	
	delete tc;
	
	__UHEAP_MARKEND;
	
	return KErrNone;
	}
 


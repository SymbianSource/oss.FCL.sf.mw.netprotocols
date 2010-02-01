/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Cookie Manager's panic handling at server-side.
*
*/


// INCLUDE FILES
	// User includes
#include "CookieServerPanic.h"

/**
* Global panic function
*/
GLDEF_C void PanicServer( TCookieServerPanic aPanic )
	{
	User::Panic( KCookieServerPanicString, aPanic);
	}

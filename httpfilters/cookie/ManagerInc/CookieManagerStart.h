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
* Description:  Declaration of StartCookieServer method, the main entry point
*               of the Cookie Manager
*         
*
*/


#ifndef __COOKIEMANAGERSTART_H__
#define __COOKIEMANAGERSTART_H__

// INCLUDE FILES
	// System includes
#include <e32base.h>

// Single export used to start the message server
IMPORT_C TInt StartCookieServer( TAny* );

#endif	// __COOKIEMANAGERSTART_H__
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
* Description:  Server starting for CookieManager.
*
*/


// INCLUDE FILES
	// System includes
#include <e32std.h>
#include <basched.h>

	// User includes
#include "CookieManagerStart.h"
#include "CookieManagerServer.h"
#include "CookieLogger.h"
#include "CookieServerDef.h"

// CONSTANTS

//**********************************
// Single exported function
//**********************************

/**
* An exported function on the global name space that runs the server.
*/
EXPORT_C TInt StartCookieServer( TAny* /*anArg*/ )
	{
    CLOG( ( EServerStart, 0, _L( "" ) ) );
    CLOG( ( EServerStart, 0, _L( "*****************" ) ) );
    CLOG( ( EServerStart, 0, _L( "-> StartCookieServerStart" ) ) );

	//__UHEAP_MARK;

	TBuf<32> name( KCookieServerThreadName );

    RThread().RenameMe( name );

	TInt result = KErrNone;

	CTrapCleanup* cleanup = CTrapCleanup::New();
	if ( cleanup == NULL )
		{
		result = KErrNoMemory;
        CLOG( ( EServerStart, 0, _L( "TrapCleanup failure" ) ) );
		}
	else
		{
		CBaActiveScheduler* scheduler = new CBaActiveScheduler;
		if ( scheduler == NULL )
			{
			result = KErrNoMemory;
            CLOG( ( EServerStart, 0, _L( "CBaActiveScheduler failure" ) ) );
			}
		else
			{
			CActiveScheduler::Install( scheduler );
    
			CLOG( ( EServerStart, 0, _L( "ActiveScheduler installed OK" ) ) );

			CCookieManagerServer* server = NULL;
			TRAP( result, server = CCookieManagerServer::NewL() );
			if ( result == KErrNone )
				{
                RProcess::Rendezvous( result );
				CLOG( ( EServerStart, 0, _L( "Starting ActiveScheduler" ) ) );

				// It starts an infinite loop from which we can break out
				// only if we stop the active scheduler
				CActiveScheduler::Start();

				CLOG( ( EServerStart, 0, _L( "ActiveScheduler stopped" ) ) );

				// finished
				delete server;
				}
            else
                {
                CLOG( ( EServerStart, 0,
                    _L( "Cookie Server start-up failure %d" ), result ) );
                }
			delete scheduler;
			}
		delete cleanup;
		}

	//__UHEAP_MARKEND;

    CLOG( ( EServerStart, 0, _L( "<- StartCookieServerStart err%d" ),
                            result ) );

	return result;
	}

// End of file

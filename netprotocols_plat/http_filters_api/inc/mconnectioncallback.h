/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  MConnectionCallback
*
*/



#ifndef MCONNECTIONCALLBACK_H
#define MCONNECTIONCALLBACK_H

//  INCLUDES
#include <e32base.h>
#include <ApEngineConsts.h>

// CONSTANTS

// MACROS

// DATA TYPES
enum TConnHandlerErr
{
   EErrNetUnreach,
   EErrWaitTimeout

};

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
*   Client Callback interface to encapsulate a Connetion Handler
*/
class MConnectionCallback 
{
public:  // Constructors and destructor

    /**
    * This function Creates connection if it doesn't exist. Returns status of created connection.
    * @param A reference to RConnection, typecasted as TInt
    * @param A reference to he socket server, typecasted as TInt
    * @param A reference to flag of a new connection
    * @param A reference to bearer type
    **/

	IMPORT_C virtual TInt CreateConnection(TInt* aConnectionPtr, TInt* aSockSvrHandle, TBool* aNewConn, TApBearerType* aBearerType) = 0;
  
    /**
    * This function cancel outstanding transactions and notify a user with the message "Out of coverage"
    **/

  IMPORT_C virtual void CoverageEvent(TInt aError) = 0; 
									
};

#endif      // MCONNECTIONCALLBACK_H

// End of File

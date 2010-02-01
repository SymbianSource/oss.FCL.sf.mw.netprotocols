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
* Description:  HTTP Connection Handler filter
*
*/



#ifndef CHTTPFILTERCONNHANDLER_H
#define CHTTPFILTERCONNHANDLER_H

//  INCLUDES
#include <http/mhttpfilter.h>
#include "httpfilterconnhandlerinterface.h"
#include <httpstringconstants.h>
#include <ApEngineConsts.h>

class MConnectionCallback;
class CHttpFilterConnHandlerObserver;

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
*   Http Filter ConnHandler implementation class.
*
*  @lib cannot link with Ecom plugins
*  @since 2.0
*/

class CHttpFilterConnHandler : public CHttpFilterConnHandlerInterface, public MHTTPFilter
{
public:  // Constructors and destructor

    /**
    * Two-phased constructor. This function replaces NewL
    */
    static CHttpFilterConnHandler* InstallFilterL(TAny* aConnParams);

    /**
    * Destructor.
    */
    virtual ~CHttpFilterConnHandler();

public: // Functions from base classes


    /**
    * From MHTTPFilterBase Process a transaction event.
    * @since 2.0
    * @param aTransaction The transaction for which an event occured
    * @param aEvent The event to be processed
    * @return void
    */
    virtual void MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent);

    /**
    * From MHTTPFilterBase Process a session event.
    * @since 2.0
    * @param aEvent The event to be processed
    * @return void
    */
    virtual void MHFSessionRunL(const THTTPSessionEvent& aEvent);

    /**
    * From MHTTPFilterBase Process an error that occured while processing the transaction.
    * @since 2.0
    * @param aError The error that occured
    * @param aTransaction The transaction for which the error occured
    * @param aEvent The event during which the error occured
    * @return KErrNone
    */
    virtual TInt MHFRunError(TInt aError,
                             RHTTPTransaction aTransaction,
                             const THTTPEvent& aEvent);

    /**
    * From MHTTPFilterBase Called when MHFRunL leaves from a session event.
    * @since 2.0
    * @param aError The error that occured
    * @param aEvent The event during which the error occured
    * @return KErrNone
    */
    virtual TInt MHFSessionRunError(TInt aError, const THTTPSessionEvent& aEvent);

    /**
    * From MHTTPFilter Called when the filter is being removed from a session's filter queue.
    * @since 2.0
    * @param aSession The session it's being removed from
    * @param aHandle The filter handle.
    * @return void
    */
    virtual void MHFUnload(RHTTPSession aSession, THTTPFilterHandle aHandle);

    /**
    * From MHTTPFilter Called when the filter is being added to the session's filter queue.
    * @since 2.0
    * @param aSession The session it's being added to.
    * @param aHandle The filter handle.
    * @return void
    */
    virtual void MHFLoad(RHTTPSession aSession, THTTPFilterHandle aHandle);

public:
    /**
    * access function
    */
    void GetObserver(CHttpFilterConnHandlerObserver*& aObserver);

private:

    /**
    * C++ default constructor.
    */
    CHttpFilterConnHandler(RHTTPSession* aSession, MConnectionCallback* aConnCallback);

    /**
    * By default Symbian 2nd phase constructor is private.
    */
    void ConstructL();

    /**
    * Function to handle Submit events. Creates connection if it doesn't exist.
     **/
    void CreateConnectionL(TApBearerType* aBearerType, MConnectionCallback* aCallbackPtr );

private:    // Data

    // Counter for how many times the filter was loaded
    TInt            iLoadCount;

    // The HTTP Session for which the filter was loaded
    RHTTPSession*   iSession;

    // The ConnectionCallback
    MConnectionCallback* iConnCallback;

    // The pointer to the CHttpFilterConnHandlerObserver object
    CHttpFilterConnHandlerObserver* iObserver;

};

#endif      // CHTTPFILTERCONNHANDLER_H

// End of File

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
* Description:   Proxy HTTP filter
*
*/



#ifndef CHTTPFILTERPROXY_H
#define CHTTPFILTERPROXY_H

//  INCLUDES
#include <http/mhttpfilter.h>
#include "httpfilterproxyinterface.h"
#include <httpstringconstants.h>
#include <es_sock.h>


// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
*   Proxy Filter implementation class.
*
*  @lib cannot link with Ecom plugins
*  @since 2.0
*/
class CHttpFilterProxy : public CHttpFilterProxyInterface, public MHTTPFilter
{
public:  // Constructors and destructor

    /**
    * Two-phased constructor. This function replaces NewL
    */
    static CHttpFilterProxy* InstallFilterL(TAny* aSession);

    /**
    * Destructor.
    */
    virtual ~CHttpFilterProxy();

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

    /**
    * From CFilterBase reserved
    * @since 2.0
    * @param 
    * @return 0
    */
    virtual TInt Reserved(){return 0;}

private:

    /**
    * C++ default constructor.
    */
    CHttpFilterProxy(RHTTPSession* aSession);

    /**
    * By default Symbian 2nd phase constructor is private.
    */
    void ConstructL();

    /**
    * Function to handle Submit events. Set proxy properties (EUseProxy and EProxyAddress) 
    * in order to get connected throught proxy when a new HTTP session will be in effect.  
    * The Proxy data will be taken from the CommDb.
    */
    void SetProxyL(const RHTTPTransaction aTransaction);

    /**
    * Purpose	Reads the Proxy information from the comms database. 
    * Gives the Proxy usage, a proxy address name (including a proxy port number), 
    * and proxy exceptions through the output arguments.
    */
    void ReadProxyInfoL(const RStringPool aStringPool,
                        TBool& aUsage,
                        RStringF& aProxyAddress,
                        RStringF& aExceptions);


    /**
    * Get exception from the list of exceptions
    */
    TBool GetException(TPtr8& aException, TInt& aStartIndex);

    /**
    * Compare each exception from exception list against current uri
    */
    void ExcptionsCompare(const RHTTPTransaction aTransaction);

private:    // Data

    // Counter for how many times the filter was loaded
    TInt            iLoadCount;

    // The HTTP Session for which the filter was loaded
    RHTTPSession*   iSession;

    ///////////////////////////////////////////////////
    // data read from the Proxy Table
    ///////////////////////////////////////////////////
    TBool           iUsage;
    RStringF        iProxyAddress;
    RStringF        iExceptions;

	// ESOCK handles used to start link-layer
	RConnection iConnection;
	RSocketServ iSocketServ;

	TBool iFilterOwnsConnection;

	RStringPool iStringPool;
	RHTTPConnectionInfo	iConnInfo;

};

#endif      // CHTTPFILTERPROXY_H

// End of File

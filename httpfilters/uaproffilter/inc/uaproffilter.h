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
* Description:  Declaration of class CHttpUAProfFilter.
*
*
*/


#ifndef HTTP_UAPROF_FILTER_H
#define HTTP_UAPROF_FILTER_H

// INCLUDES

#include <e32std.h>
#include <e32base.h>
#include <etelmm.h>

#include <http/mhttpfilter.h>
#include <http/rhttpsession.h>
#include <httpstringconstants.h>
#include <centralrepository.h>
#include <cenrepnotifyhandler.h>

#include "uaproffilter_interface.h"

// CLASS DECLARATION

/**
* Implementation of User Agent Profile HTTP filter.
*/
class CHttpUAProfFilter
: public CHttpUAProfFilterInterface,
  public MHTTPFilter,
  public MCenRepNotifyHandlerCallback
    {
    public:     // Constructors and destructor.

        /**
        * Constructor.
        * @param aSession Install on this session (RHTTPSession as TAny*).
        */      
        static CHttpUAProfFilterInterface* InstallFilterL( TAny* aSession );

        /**
        * Destructor.
        */      
        virtual ~CHttpUAProfFilter();

    public: // From MHTTPFilterBase

        /** 
        * Handle transaction event.
        * @param aTransaction The transaction that the event has occurred on.
        * @param aEvent The event that has occurred.
        */
        void MHFRunL( RHTTPTransaction aTransaction, const THTTPEvent& aEvent );

        /** 
        * Handle error in MHFRunL.
        * @param aError The leave code that MHFRunL left with.
        * @param aTransaction The transaction that was being processed.
        * @param aEvent The Event that was being processed.
        * @return KErrNone.
        */
        TInt MHFRunError
            (
            TInt aError,
            RHTTPTransaction aTransaction,
            const THTTPEvent& aEvent
            );

        /** 
        * Handle session event.
        * @ param aEvent The session event that has occured.
        * @ exception Any leaves must be handled by MHFSessionRunError.
        */
        void MHFSessionRunL( const THTTPSessionEvent& aEvent );

        /** 
        * Called when MHFSessionRunL leaves from a session event. This works 
        * in the same way as CActive::RunError.
        * If you don't completely handle the error, a panic will occur.
        * @param aError The leave code that RunL left with.
        * @param aEvent The Event that was being processed.
        * @return KErrNone if the error has been cancelled or the code
        *   of the continuing error otherwise.    
        */
        TInt MHFSessionRunError( TInt aError, const THTTPSessionEvent& aEvent );

    public:     // From MHTTPFilter

        /** 
        * Unload filter.
        * @param aSession The session it's being removed from.
        * @param aHandle The filter handle.
        */
        virtual void MHFUnload
            ( RHTTPSession aSession, THTTPFilterHandle aHandle );

        /** 
        * Load filter.
        * @param aSession The session it's being added to.
        * @param aHandle The filter handle.
        */
        virtual void MHFLoad
            ( RHTTPSession aSession, THTTPFilterHandle aHandle );

        // Comment out when available.

        public:     // From CCenRepNotifyHandler

            /**
            * Central Repository key change.
            * @param aId Keyword of which value was changed.
            * @param aValue The new value involved with the keyword.
            */
            void HandleNotifyInt(TUint32 aId, TInt aValue);

    private:    // Constructors and destructor.

        /**
        * Constructor.
        * @param aSession Session to install on.
        */
        CHttpUAProfFilter( RHTTPSession aSession );

        /**
        * Second phase constructor. Leaves on failure.
        */      
        void ConstructL();

    private:    // Constructors and destructor

        /**
        * Copy constructor - unimplemented, forbidden.
        */
        CHttpUAProfFilter( const CHttpUAProfFilter& aUAProfFilter );

        /**
        * Assignment operator - unimplemented, forbidden.
        */
        CHttpUAProfFilter& operator=( const CHttpUAProfFilter& aUAProfFilter );

    private:    // New methods

        /**
        * Setup IMEI-notify feature related things.
        */
        void SetupImeiNotifyL();

        /**
        * Adding UserAgentProfile information to transaction's header.
        * @param aTransaction Which transaction is used.
        */
        void SubmitL( RHTTPTransaction aTransaction );

        /**
        * Add this header to the header collection. If already exists, replace.
        * @param aHeaders Header collection.
        * @param aName Header name.
        * @param aValue Header value.
        */
        void AddOrReplaceHeaderL
            (
            RHTTPHeaders& aHeaders,
            const RStringF& aName,
            const RStringF& aValue
            );

        /**
        * Open iUaName and iUaNameWithImei strings.
        */
        void OpenUserAgentNameStringsL();

        /**
        * Open iUaProf and iUaProf3G strings.
        * Note this method uses the iSdClient.
        */
        void OpenProfileStringsL();

        /**
        * Convert string from 16-bit to 8-bit.
        * @param aSrc Source descriptor.
        * @return Converted 8-bit buffer. Owner is the caller.
        */
        HBufC8* ConvertL( const TDesC16& aSrc );

        /**
        * Read string from Central Repository. Leave on any error.
        * iSdCli must be connected and assigned.
        * @param aKey Which key should be read.
        * @param aBuf Allocated string returned here. Not pushed, owner is the
        * caller. Will be NULL on error, non-NULL on KErrNone.
        * @return Error code.
        */
        TInt ReadSdString( const TUint32  aKey, HBufC*& aBuf );

        /**
        * Get a shared string from the session stringpool.
        * @param aId Id of string.
        * @return Shared string from HTTP string table. No need to close.
        */
        inline RStringF StringF( HTTP::TStrings aId );
        
        /**
        * Get the phone's IMEI code. (Initial part that fits into aBuf.)
        * Gives back a fake IMEI on wins.
        * @param aBuf Receives IMEI code.
        */
        void GetImeiL( TDes& aBuf ) const;

        /**
        * Query if the phone is a 3G network.
        * @return ETrue if the phone is in 3G network.
        */
        TBool Is3gNetworkL() const;
        
        void AddImeiToUserAgentL(TPtr8 &aUAStringPtr, const TDesC8& aImei);

    private:  // data

        TInt iLoadCount;                    ///< Load count.
        RStringF iUaProf;                   ///< UAProf value.
        TBool iUaProfOpen;                  ///< ETrue if iUaProf is open.
        RStringF iUaProf3G;                 ///< UAProf 3G value.
        TBool iUaProf3GOpen;                ///< ETrue if iUaProf3G is open.
        RStringF iUaName;                   ///< User Agent name.
        TBool iUaNameOpen;                  ///< ETrue if iUaName is open.
        RStringF iUaNameWithImei;           ///< User Agent name with IMEI.
        TBool iUaNameWithImeiOpen;          ///< ETrue if iUaNameWithImei open.
        RStringF iUaNameMms;                ///< User Agent name - MMS client.
        TBool iUaNameMmsOpen;               ///< ETrue if iUaNameMms is open.
        RStringF iUaNameMmsWithImei;        ///< User Agent MMS name with IMEI.
        TBool iUaNameMmsWithImeiOpen;       ///< iUaNameMmsWithImei open?
        RHTTPSession iSession;              ///< Session.
        RStringPool iStringPool;            ///< String pool (of the session).
        const TStringTable& iStringTable;   ///< String table.
        TBool iImeiFeatureEnabled;          ///< IMEI-Notify feature enabled?
        TBool iImeiSendingOn;               ///< IMEI sending on?
        RTelServer iTelServer;              ///< Telephony Server session.
        RMobilePhone iMobilePhone;          ///< Mobile phone.

        CRepository* iRepositoryBrowser;      // CentRep client. Owned.
        CRepository* iRepositoryCommonEngine; // CentRep client. Owned.
        CCenRepNotifyHandler* iNotifyHandler; // Handle changes in CenRep.
    };

#include "uaproffilter.inl"

#endif  // HTTP_UAPROF_FILTER_H

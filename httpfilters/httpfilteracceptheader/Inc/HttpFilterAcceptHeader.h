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
* Description:  Http filter for handling dynamic accept headers
*
*/



#ifndef HTTPFILTERACCEPTHEADER_H
#define HTTPFILTERACCEPTHEADER_H

//  INCLUDES

#include <apgcli.h>


#include "httpfilteracceptheaderinterface.h"

const TUint KLanguageBufSize = 256;

class RSharedDataClient;

// CLASS DECLARATION

/**
*  Http filter for handling dynamic accept headers.
*  This filter reads the supported MIME types from the system
*  and constructs the accept headers accordingly. Some optimization
*  is also provided. The filter stores the list of the types in memory 
*  to avoid unnecessary client-server communication to the system.
*
*  The client can also define which subset of the MIME types
*  it needs in the request headers. The following enum defines
*  the parameters that the client can deliver in the transactino
*  properties.
*  <pre>
*
*  typedef enum
*      {
*      EAcceptAll,     
*      EAcceptImages,  
*      EAcceptCss,     
*      EAcceptNone     
*      } ELoadTypes;
*
*  </pre>
*  The filter can be constructed like this:
*  <pre>
*
*    CHttpFilterAcceptHeader* filter = 
*        (CHttpFilterAcceptHeader*) REComSession::CreateImplementationL(
*			 KUidHttpFilterAcceptHeader, 
*			_FOFF(CHttpFilterAcceptHeader, iEcomDtorID),
*			&iSess );
*
*  </pre>
*
*
*  @lib ECOM plugins are not meant to be linked against.
*  @since 2.0
*/

class CHttpFilterAcceptHeader: public CHttpFilterAcceptHeaderInterface, private MHTTPFilter
    {

    public:   // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CHttpFilterAcceptHeader* InstallFilterL(TAny* aSession);

        /**
        * Destructor.
        */
        virtual ~CHttpFilterAcceptHeader();

    public:  // Functions from base classes

        // Methods from MHTTPFilterBase

        /**
        * From MHTTPFilterBase. Leaves if error occures. The only possible
        * error can be no memory situation.
        * @since 2.0
        * @param aTransaction An instance of HTTP transaction
        * @param aEvent An instance of THTTPEvent.
        */
        virtual void MHFRunL(
            RHTTPTransaction aTransaction, 
            const THTTPEvent& aEvent);

        /**
        * From MHTTPFilterBase. Empty implementation.
        * @since 2.0
        * @param aEvent An instance of THTTPEvent.
        */
        virtual void MHFSessionRunL(const THTTPSessionEvent& aEvent);

        /**
        * From MHTTPFilterBase. Empty implementation. Not a fatal error so the
        * transaction can continue.
        * @since 2.0
        * @param aTransaction An instance of HTTP transaction
        * @param aEvent An instance of THTTPEvent.
        */
        virtual TInt MHFRunError(TInt aError, RHTTPTransaction aTransaction,
                                 const THTTPEvent& aEvent);

        /**
        * From MHTTPFilterBase. Empty implementation.
        * @since 2.0
        * @param aEvent An instance of THTTPEvent.
        */
        virtual TInt MHFSessionRunError(TInt aError, 
                                        const THTTPSessionEvent& aEvent);

        // Methods from MHTTPFilter

        /**
        * From MHTTPFilter. Deletes itself if the reference 
        * counter reaches zero.
        * @since 2.0
        * @param aEvent An instance of THTTPEvent.
        */
        virtual void MHFUnload(RHTTPSession aSession, 
                               THTTPFilterHandle aHandle);


        /**
        * From MHTTPFilter. Increments the reference counter by one.
        * @since 2.0
        * @param aEvent An instance of THTTPEvent.
        */
        virtual void MHFLoad(RHTTPSession aSession, 
                             THTTPFilterHandle aHandle);


    protected:   // New functions

    protected:   // Functions from base classes

    private:

        /**
        * C++ constructor.
        */
        CHttpFilterAcceptHeader(RHTTPSession* aSession);

        /**
        * Prohibit copy contructor
        */
        CHttpFilterAcceptHeader(const CHttpFilterAcceptHeader&) {};


        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();


        /**
        * Decide which accept headers must be added to the transaction
        * @since 2.0
        * @param aTransaction An instance of HTTP transaction
        */
        void AddHeadersL(RHTTPTransaction aTransaction);

		/**
		* Add transaction property to inform if we already added
		* all accept headers. This is to avoid adding headers 
		* multiple times in case of redirect.
		*/
		void AllHeadersSetL(RHTTPTransactionPropertySet& aProperties);

        /**
        * Add all supported MIME types
        * @since 2.0
        * @param aHeaders An instance of RHTTPHeaders from the current 
        *        transaction.
        * @param aAcceptStr Ready to use string representing "accept".
        */
        void AddAllHeadersL( RHTTPHeaders& aHeaders, RStringF aAcceptStr );

        /**
        * Add all image MIME types
        * @since 2.5
        * @param aHeaders An instance of RHTTPHeaders from the current 
        *        transaction.
        * @param aAcceptStr Ready to use string representing "accept".
        */
        void AddNpiHeadersL( RHTTPHeaders& aHeaders, RStringF aAcceptStr );

        /**
        * Add all image MIME types
        * @since 2.0
        * @param aHeaders An instance of RHTTPHeaders from the current 
        *        transaction.
        * @param aAcceptStr Ready to use string representing "accept".
        */
        void AddImageHeadersL( RHTTPHeaders& aHeaders, RStringF aAcceptStr );

        /**
        * Add text/css
        * @since 2.0
        * @param aHeaders An instance of RHTTPHeaders from the current 
        *        transaction.
        * @param aAcceptStr Ready to use string representing "accept".
        */
        void AddCssHeadersL( RHTTPHeaders& aHeaders, RStringF aAcceptStr );

        /**
        * CHttpFilterAcceptHeader::AddJavaScriptHeadersL
        * Add "application/x-javascript"
        */
        void AddJavaScriptHeadersL(RHTTPHeaders& aHeaders,RStringF aAcceptStr );

        /**
        * Utility class to actually add a key/value pair to the headers
        * @since 2.0
        * @param aKey RString containing the key sting. E.g. "accept"
        * @param aValue A value for the aKey.
        */

        void SetHeaderL( RHTTPHeaders aHeaders,
                         const RStringF aKey,
                         const TPtrC8& aValue );

        /**
        * Add Language Header based on KHttpAcceptMultipleAcceptLanguagesSupport feature
        * @since 2.6
        * @param aTransaction An instance of HTTP transaction
        */
		void AddLanguageHeaderL(RHTTPTransaction aTransaction);

        /**
        * Add Compose Multiple Languages String
        * @since 2.6
        */
		void ComposeMultipleLanguagesStringL();

        /**
        * Read the featurebitmask from Shared Data
        * @since 2.6
        * @param aSDClient Initialized SharedData client
        */
        void ReadFeatureBitmaskL();

        /**
        * Add hardcoded MIME types that the browser supports itself
        * @since 2.6
        */
        void AddBrowserSupportedTypesL();

        /**
        * Read MIME types for external applications from Shared Data
        * @since 2.6
        * @param aSDClient Initialized SharedData client
        */
        void ReadMimeTypesL();

        /**
        * Add individual entries to MIME type array. Performs
        * white space clean up before adding.
        * @since 2.6
        * @param aPtr A pointer to descriptor containing a mime type
        */
        void AddToArrayL( TPtr8& aPtr );

        /**
        * Parse comma separated list of MIME types and places them to 
        * MIME type array
        * @since 2.6
        * @param aPtr A pointer to descriptor containing a mime type list
        */
        void ParseAndStoreMimeListL( TPtr8& aPtr );


    public:      // Data

    protected:   // Data

    private:     // Data

        TInt iLoadCount;

        RHTTPSession* iSession;

        CDataTypeArray* iDataTypes;

        RStringF iLoadTypeStr;

        RStringF iAcceptStr;

		RStringF iDone;

        TInt     iFeatureBitmask; // Refer HttpFilterAcceptHeaderVariant.hrh for mask definition

		TLanguage iUserSelectedLang;
		RStringF  iUserSelectedLangString;

		TBuf8<KLanguageBufSize> iInstalledLangBuf;
    };

#endif      // CHTTPFILTERACCEPTHEADER_H

// End of File

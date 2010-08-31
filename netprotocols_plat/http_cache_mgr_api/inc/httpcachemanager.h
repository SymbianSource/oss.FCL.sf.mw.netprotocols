/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   Definition of CHttpCacheManager
*
*/

#ifndef CHTTPCACHEMANAGER_H
#define CHTTPCACHEMANAGER_H

//  INCLUDES
#include <e32base.h>
#include <cenrepnotifyhandler.h>
#include <BrCtlDefs.h>
#include <http/rhttpsession.h>

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS
class CHttpCacheEntry;
class CHttpCacheHandler;
class CCenRepNotifyHandler;
class CRepository;
class RHTTPTransaction;
class MHTTPDataSupplier;
class CHttpCacheFileWriteHandler;
class CHttpCacheFileHash;
class THttpCachePostponeParameters;

// CLASS DECLARATION

/**
*
*  @lib
*  @since 3.1
*/
class THttpCacheEntry
    {
    public:

        //
        CHttpCacheHandler*  iCacheHandler;

        //
        CHttpCacheEntry*    iCacheEntry;
    };

/**
*
*  @lib
*  @since 3.1
*/
class CHttpCacheManager : public CBase, public MCenRepNotifyHandlerCallback
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * @since 3.1
        * @param
        * @param
        * @return CacheHandler object.
        */
        IMPORT_C static CHttpCacheManager* NewL();

        /**
        * Destructor.
        */
        virtual ~CHttpCacheManager();

    public: // new functions

        /**
        *
        * @since 3.1
        * @param
        * @return
        */
        IMPORT_C TInt RequestL( RHTTPTransaction& aTrans, TBrCtlDefs::TBrCtlCacheMode aCacheMode,
            THttpCacheEntry& aCacheEntry );

        /**
        *
        * @since 3.1
        * @param
        * @return
        */
        IMPORT_C TInt RequestHeadersL( RHTTPTransaction& aTrans, THttpCacheEntry& aCacheEntry );

        /**
        *
        * @since 3.1
        * @param
        * @return
        */
        IMPORT_C HBufC8* RequestNextChunkL( RHTTPTransaction& aTrans, TBool& aLastChunk, THttpCacheEntry& aCacheEntry );

        /**
        *
        * @since 3.1
        * @param
        * @return
        */
        IMPORT_C void ReceivedResponseHeadersL( RHTTPTransaction& aTrans, THttpCacheEntry& aCacheEntry );

        /**
        *
        * @since 3.1
        * @param
        * @return
        */
    IMPORT_C void ReceivedResponseBodyDataL( RHTTPTransaction& aTrans, MHTTPDataSupplier& aBodyDataSupplier, THttpCacheEntry& aCacheEntry );

        /**
        *
        * @since 3.1
        * @param
        * @return
        */
    IMPORT_C void ResponseComplete( RHTTPTransaction& aTrans, THttpCacheEntry& aCacheEntry );

        /**
        *
        * @since 3.1
        * @param
        * @return
        */
    IMPORT_C void RequestClosed( RHTTPTransaction* aTrans, THttpCacheEntry& aCacheEntry );

        /**
        *
        * @since 3.1
        * @param
        * @return
        */
        IMPORT_C TInt RemoveAllL();

        /**
        *
        * @since 3.1
        * @param
        * @return
        */
        IMPORT_C TInt RemoveL( const TDesC8& aUrl );

        /**
        *
        * @since 3.1
        * @param
        * @return
        */
        IMPORT_C TBool Find( const TDesC8& aUrl );

        /**
        *
        * @since 3.1
        * @param
        * @return
        */
        IMPORT_C TBool SaveL( const TDesC8& aUrl, const TDesC8& aHeader,
                     const TDesC8& aContent, const TDesC8&  aHttpHeaderString );

        /**
        *
        * @since 3.1
        * @param
        * @return
        */
        IMPORT_C TInt AddHeaderL( const TDesC8& aUrl, const TDesC8& aName, const TDesC8& aValue );

    public: // from MCenRepNotifyHandlerCallback

        /**
        *
        * @since 3.1
        * @param
        * @return
        */
        void HandleNotifyString( const TUint32 aKeyId, const TDesC& aValue );

    private:

        /**
        * Construct.
        * @since 3.1
        * @param
        * @param
        * @return CacheHandler object.
        */
        CHttpCacheManager();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private:

        /**
        *
        * @since 3.1
        * @param
        * @return
        */
        void CreateCacheHandlersL();

        /**
        *
        * @since 7.1
        * @param
        * @return
        */
        void RemoveOrphanedFilesL();    // remove all orphan files.
        /**
        *
        * @since 7.1
        * @param
        * @return
        */
        // perform phase 2 of orphan file removal. Mark any files 
        void MarkAllCacheContentAsNoDelete( CHttpCacheFileHash* aOnDiskFilesMap );
        /**
        *
        * @since 7.1
        * @param
        * @return
        */
        // perform phase 3 of orphan file removal.
        // Write an 'empty cache' index file over the top of any unknown index files to let any running clients
        // prepare for their data to disappear.
        void WipeAllOtherIndexFilesL( CHttpCacheFileHash* aOnDiskFilesMap, RFs& aRfs );
        /**
        *
        * @since 7.1
        * @param
        * @return
        */
        // perform phase 4 of orphan file removal. Remove any files not referenced in the loaded caches.
        void DeleteMarkedFilesL( CHttpCacheFileHash* aOnDiskFilesMap, RFs& aRfs );
        /**
        *
        * @since 7.1
        * @param
        * @return
        */
        // utility fn for phase 3 of orphan file removal.
        void GenerateEmptyIndexFileL(const TDesC& aIndexFile, RFs& aRfs );

        /**
         * @since 7.1
         * override cache parameters according to application using DLL and any config in Centrep
         */
        void ApplyCacheOverridesL(CRepository& aRepository, const TUint32& aSecIdInt, TBool& aCacheEnabled, TInt& aCacheSize, TBool& aOpCacheEnabled, TBool& aVssCacheEnabled, TDes& aPath, const TDesC& aDefaultDrive);

        /**
        *
        * @since 3.1
        * @param
        * @return
        */
        CHttpCacheHandler* CacheHandler( const TDesC8& aUrl,  TDes8* aHttpHeaderString ) const;


    /**
        *
        * @since 3.1
        * @param
        * @return
        */
        TBool VSSRequestCheckL( const RHTTPTransaction& aTrans,
                                const RHTTPHeaders& aHttpHeader, const TDesC8& aUrl );
    /**
        *
        * @since 3.1
        * @param
        * @return
        */

        TBool VSSHeaderCheck(  TDes8*  aHttpHeaderString ) const ;

        /**
         * @since 7.1
         * 
         */
        void GetPostponeParamsL( TBool& aNewCentrepPresent, THttpCachePostponeParameters& aParams, CRepository* aRepo );

        /**
         * @since 7.1
         * 
         */
        void GetHttpCacheConfigL( CRepository& aRepository, TBool& aCacheEnabled, TInt& aCacheSize, TDes& aCacheFolder );

        /**
         * @since 7.1
         * 
         */
        void GetOperatorCacheConfigL( CRepository& aRepository, TBool& aOpCacheEnabled, TInt& aOpCacheSize, TDes& aOpCacheFolder );

        /**
         * @since 7.1
         * 
         */
        void GetVSSCacheConfigL( CRepository& aRepository, TBool& aVSSCacheEnabled, TInt& aVssCacheSize, TDes& aVssCacheFolder );
        
        /**
         * @since 7.1
         * 
         */
        void GetCriticalDriveLevelsL( CRepository& aRepository, const TDesC& aCacheFolder, TInt& aCriticalLevel );

        /**
         * @since 7.1
         * 
         */
        void CreateHttpCacheL( const TInt& aSecIdInt, const TInt& aCacheSize, const TInt& aCriticalLevel, const THttpCachePostponeParameters& aPostpone );

        /**
         * @since 7.1
         * 
         */
        void CreateOperatorCacheL( CRepository& aRepository, const TDesC& aOpCacheFolder, const TInt& aOpCacheSize, const TInt& aCriticalLevel, const THttpCachePostponeParameters& aPostpone );

        /**
         * @since 7.1
         * 
         */
        void CreateVssCacheL( CRepository& aRepository, const TDesC& aVssCacheFolder, const TInt& aVssCacheSize, const TInt& aCriticalLevel, const THttpCachePostponeParameters& aPostpone );

        /**
         * @since 7.1
         * 
         */
        void CrashCheckL( const TInt& aSecIdInt );


    private:    // Data

        CHttpCacheHandler*                         iCache;      // owned

        TPath                                      iCacheFolder;

        CHttpCacheHandler*                         iOperatorCache;      // owned
        //
        CHttpCacheHandler*                         iphoneSpecificCache;      // owned
        //
        CHttpCacheFileWriteHandler*                iFileWriteHandler; // owned

        HBufC8*                                    iOpDomain;       // owned
        //
        CCenRepNotifyHandler*                      iOfflineNotifyHandler; // owned
        //
        CRepository*                               iOfflineRepository; // owned
        //
        TBool                                      iOfflineMode;
        //
        TBool                                      iCacheEnabled;
        //
        HBufC8*                                    iVSSWhiteList;       // owned
        //
        TBool                                      iVSSCacheEnabled;
        TBool                                      iVSSTransaction;
    };

#endif      // CHttpCacheManager_H

// End of File

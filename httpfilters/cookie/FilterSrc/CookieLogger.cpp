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
* Description: 
*      Implementation of class CookieLogger.   
*      
*
*/


// INCLUDE FILES

#ifdef __TEST_COOKIE_LOG__

    #include "CookieLogger.h"
    #include <flogger.h>

    // ================= CONSTANTS =======================

    /// Cookie logging directory.
    _LIT( KCookieLogDir, "Cookie" );
    /// Cookie log file name.
    _LIT( KCookieLogFile, "Cookie.txt" );

    // ================= MEMBER FUNCTIONS =======================

    // ---------------------------------------------------------
    // CookieLogger::Write()
    // ---------------------------------------------------------
    //
    void CookieLogger::Write
    ( TInt32 aMask, TInt aLevel, TRefByValue<const TDesC16> aFmt, ... )
        {
        if( (aMask & COOKIE_LOG_MASK) && (aLevel <= COOKIE_LOG_LEVEL) )
            {
            VA_LIST list;
            VA_START( list, aFmt );
            RFileLogger::WriteFormat
                (
                KCookieLogDir,
                KCookieLogFile,
                EFileLoggingModeAppend,
                aFmt,
                list
                );
            VA_END( list );
            }
        }

    // ---------------------------------------------------------
    // CookieLogger::Write()
    // ---------------------------------------------------------
    //
    void CookieLogger::Write
    ( TInt32 aMask, TInt aLevel, TRefByValue<const TDesC16> aFmt, VA_LIST& aList )
        {
        if( (aMask & COOKIE_LOG_MASK) && (aLevel <= COOKIE_LOG_LEVEL) )
            {
            RFileLogger::WriteFormat
                (
                KCookieLogDir,
                KCookieLogFile,
                EFileLoggingModeAppend,
                aFmt,
                aList
                );
            }
        }

    // ---------------------------------------------------------
    // CookieLogger::Write()
    // ---------------------------------------------------------
    //
    void CookieLogger::Write
    ( TInt32 aMask, TInt aLevel, TRefByValue<const TDesC8> aFmt, ... )
        {
        if( (aMask & COOKIE_LOG_MASK) && (aLevel <= COOKIE_LOG_LEVEL) )
            {
            VA_LIST list;
            VA_START( list, aFmt );
            RFileLogger::WriteFormat
                (
                KCookieLogDir,
                KCookieLogFile,
                EFileLoggingModeAppend,
                aFmt,
                list
                );
            VA_END( list );
            }
        }

    // ---------------------------------------------------------
    // CookieLogger::Write()
    // ---------------------------------------------------------
    //
    void CookieLogger::Write
    ( TInt32 aMask, TInt aLevel, TRefByValue<const TDesC8> aFmt, VA_LIST& aList )
        {
        if( (aMask & COOKIE_LOG_MASK) && (aLevel <= COOKIE_LOG_LEVEL) )
            {
            RFileLogger::WriteFormat
                (
                KCookieLogDir,
                KCookieLogFile,
                EFileLoggingModeAppend,
                aFmt,
                aList
                );
            }
        }

    // ---------------------------------------------------------
    // CookieLogger::HexDump()
    // ---------------------------------------------------------
    //
    void CookieLogger::HexDump
            (
            TInt32 aMask,
            TInt aLevel, 
            const TText* aHeader,
            const TText* aMargin,
            const TUint8* aPtr,
            TInt aLen
            )
        {
        if( (aMask & COOKIE_LOG_MASK) && (aLevel <= COOKIE_LOG_LEVEL) )
            {
            RFileLogger::HexDump
                (
                KCookieLogDir,
                KCookieLogFile,
                EFileLoggingModeAppend,
                aHeader,
                aMargin,
                aPtr,
                aLen
                );
            }
        }

#endif // __TEST_COOKIE_LOG__

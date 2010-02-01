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
*      Logging macros for Cookie Manager.   
*      
*
*/


#ifndef COOKIE_LOGGER_H
#define COOKIE_LOGGER_H

// INCLUDES

#include <e32std.h>
#include <e32def.h>
#include <flogger.h>

// TYPES

enum TCookieLogMask             ///< Log mask bits.
    {
    ELogOff         = 0x00000000,   ///< Don't log.
    EFilter         = 0x00000001,   ///< Log filter activity.
    EClient         = 0x00000002,   ///< Log client side activity.
    EServer         = 0x00000004,   ///< Log server activity.
    EServerSession  = 0x00000008,   ///< Log server session activity.
    ECookieArray    = 0x0000000F,   ///< Log CCookieArray activity.
    ELogAll         = 0xFFFFFFFF    ///< Log all.
    };

// MACROS

/// Determines what to log. Construct this from TCookieLogMask values.
#define COOKIE_LOG_MASK ELogAll
/// Determines log detail (0==basic level).
#define COOKIE_LOG_LEVEL 5

// CLASS DECLARATION

/**
* Logger class.
*/
class CookieLogger
    {
    public:     // new methods

    /**
    * Write formatted log.
    * @param aMask Log mask.
    * @param aLevel Log level.
    * @param aFmt Format string.
    */
    static void Write
        ( TInt32 aMask, TInt aLevel, TRefByValue<const TDesC16> aFmt, ... );

    /**
    * Write formatted log.
    * @param aMask Log mask.
    * @param aLevel Log level.
    * @param aFmt Format string.
    * @param aList Variable argument list.
    */
    static void Write
        (
        TInt32 aMask,
        TInt aLevel,
        TRefByValue<const TDesC16> aFmt,
        VA_LIST& aList
        );

    /**
    * Write formatted log.
    * @param aMask Log mask.
    * @param aLevel Log level.
    * @param aFmt Format string.
    */
    static void Write
        ( TInt32 aMask, TInt aLevel, TRefByValue<const TDesC8> aFmt, ... );

    /**
    * Write formatted log.
    * @param aMask Log mask.
    * @param aLevel Log level.
    * @param aFmt Format string.
    * @param aList Variable argument list.
    */
    static void Write
        (
        TInt32 aMask,
        TInt aLevel,
        TRefByValue<const TDesC8> aFmt,
        VA_LIST& aList
        );

    /**
    * Write hex dump.
    * @param aMask Log mask.
    * @param aLevel Log level.
    * @param aHeader Header string.
    * @param aMargin Margin.
    * @param aPtr Data.
    * @param aLen Data length.
    */
    static void HexDump
        (
        TInt32 aMask,
        TInt aLevel, 
        const TText* aHeader,
        const TText* aMargin,
        const TUint8* aPtr,
        TInt aLen
        );
    };

#ifdef __TEST_COOKIE_LOG__

    /// Write formatted to log.
    #define CLOG( body ) CookieLogger::Write body
    /// Write hex dump.
    #define CDUMP( body ) CookieLogger::HexDump body
    /// Guard "log-only" argument name with this (expands to argument).
    #define LOG_ONLY( argName ) argName

#else /* not defined __TEST_COOKIE_LOG__ */

    /// Do nothing (log disabled).
    #define CLOG( body )
    /// Do nothing (log disabled).
    #define CDUMP( body )
    /// Guard "log-only" argument name with this (expands to nothing).
    #define LOG_ONLY( argName )


#endif /* def __TEST_COOKIE_LOG__ */

#endif /* def COOKIE_LOGGER_H */

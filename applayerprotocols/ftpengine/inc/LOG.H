//
// LOG.H
/**
* Copyright (c) 1996-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Logging
* Written by AdamG, Oct 1996
* 
*
*/



/**
 @file LOG.H
 @internalComponent
*/

#if !defined(__LOG_H__)
#define __LOG_H__

#include <e32std.h>
#include <e32base.h>

#if defined(_MKLOG)
/**
@internalComponent
*/
# define _LOG
#endif

#if defined(_LOG)


# if defined(__WINS__)
/**
@internalComponent
*/
#  define LOG(a) a 
# elif defined (__EPOC32__)
/** No equivelent for EPOC32 yet */
#  define LOG(a)
//#  undef _LOG
# endif
#else
# define LOG(a)
# undef _LOG
#endif	// _LOG

#ifdef _LOG

class Log
/**
@internalComponent
*/
	{
public:
	IMPORT_C static void Write(const TDesC& aDes);
	IMPORT_C static void Printf(TRefByValue<const TDesC> aFmt, ...);
	IMPORT_C static void HexDump(const TText* aHeader, const TText* aMargin, const TUint8* aPtr, TInt aLen);
	};
#endif

#endif

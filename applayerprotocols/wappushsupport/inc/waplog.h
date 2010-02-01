// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
// WAP Logging public header, defines the name of the 
// file to use for WAP activity logging messages
// 
//

#if !defined(__WAPLOG_H__)
#define __WAPLOG_H__

#if !defined(__E32STD_H__)
#include <e32std.h>
#endif

#ifdef _DEBUG
/**
@publishedAll
@deprecated
*/
_LIT(KWapLogFileName,"WAPLog.html");
#define __LOG_WAP_FILE_NAME KWapLogFileName
#else
#define __LOG_WAP_FILE_NAME
#endif

#endif

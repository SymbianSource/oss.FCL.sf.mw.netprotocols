/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
*
*/

#ifndef HTTPCACHEMANAGERINTERNALCRKEYS_H
#define HTTPCACHEMANAGERINTERNALCRKEYS_H

const TUid KCRUidCacheManager = {0x101F8557};

const TUint32 KCacheManagerHttpCacheEnabled = 0x00000001;
const TUint32 KCacheManagerHttpCacheSize = 0x00000002;
const TUint32 KCacheManagerHttpCacheFolder = 0x00000003;
const TUint32 KOperatorCacheFolder = 0x00000004;
const TUint32 KOperatorCacheSize = 0x00000005;
const TUint32 KOperatorDomainUrl = 0x00000006;
const TUint32 KPhoneSpecificCacheEnabled = 0x00000007;
const TUint32 KPhoneSpecificCacheFolder  = 0x00000008;
const TUint32 KPhoneSpecificCacheSize    = 0x00000009;
const TUint32 KPhoneSpecificCacheDomainUrl    = 0x0000000A;
const TUint32 KCacheWritePostponeEnabled = 0x0000000B;
const TUint32 KCacheWritePostponeImmediateWriteThreshold = 0x0000000C;
const TUint32 KCacheWritePostponeFreeRAMThreshold = 0x0000000D;
const TUint32 KCacheWritePostponeWriteTimeout = 0x0000000E;
const TUint32 KCacheManagerHttpCacheProcessOverride = 0x0000000F;

#endif      // HTTPCACHEMANAGERINTERNALCRKEYS_H


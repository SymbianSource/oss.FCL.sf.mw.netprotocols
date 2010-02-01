// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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
//

#ifndef SRVPARAMS_H
#define SRVPARAMS_H

#include <e32std.h>


_LIT(KServerName, "!CentralRepository");
_LIT(KServerImg,  "centralrepositorysrv");
const TUid KServerUid3  = { 0x10202BE9 };

const TInt KServerMajorVersion = 1;
const TInt KServerMinorVersion = 0;
const TInt KServerBuildVersion = 0;

#ifdef __MYSERVER_NO_PROCESSES__
const TInt KServerStackSize=0x2000; //  8KB
const TInt KServerInitHeapSize=0x1000; //  4KB
const TInt KServerMaxHeapSize=0x1000000; // 16MB
#endif

const TInt KServerPriority = 0;

#endif // SRVPARAMS_H

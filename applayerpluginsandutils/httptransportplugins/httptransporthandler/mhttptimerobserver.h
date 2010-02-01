// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __MHTTPTIMEROBSERVER_H__
#define __MHTTPTIMEROBSERVER_H__

#include <e32std.h>

class MHttpTimerObserver
	{
public:
	
	virtual void TimeOut() =0;
	
	};

#endif /*__MHTTPTIMEROBSERVER_H__*/

// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef CENTREP_LOG_H
#define CENTREP_LOG_H

#include <e32debug.h>

//
// MACROs for trace statements in client/server code.
//

#ifdef CENTREP_TRACE

#define __CENTREP_TRACE(_text)			RDebug::Print(_L(_text))
#define __CENTREP_TRACE1(_text, _a1)		RDebug::Print(_L(_text), (_a1))
#define __CENTREP_TRACE2(_text, _a1, _a2)		RDebug::Print(_L(_text), (_a1), (_a2))
#define __CENTREP_TRACE3(_text, _a1, _a2, _a3)	RDebug::Print(_L(_text), (_a1), (_a2), (_a3))

#else

#define __CENTREP_TRACE(_text)			
#define __CENTREP_TRACE1(_text, _a1)		
#define __CENTREP_TRACE2(_text, _a1, _a2)
#define __CENTREP_TRACE3(_text, _a1, _a2, _a3)	

#endif



#endif // CENTREP_LOG_H


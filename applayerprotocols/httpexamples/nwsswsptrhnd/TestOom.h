// Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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

/**
 @file TestOom.h
*/

#ifndef	__TESTOOM_H__
#define __TESTOOM_H__

// System includes
#include <e32std.h>


// Testing macro, used as a wrapper to calls made to the WAP Stack.
// The macro simulates allocation failures in the WAP Stack, and returns
// KErrNoMemory in OOM situations when used in a heap-failure testing environment.
// Also define the test-leave macro, which should be place before calls to leaving functions
// in order to simulate the control paths followed should their leaves occur.
#if defined (_DEBUG) && defined(__UNIT_TESTING__)

#define __TESTOOM(_r, _x) _r = KErrNone;{HBufC* _a=HBufC::New(4);delete _a;if (_a==NULL)_r=KErrNoMemory;else _r=_x;}
#define __TESTOOMD(_r, _x) TInt _r = KErrNone;{HBufC* _a=HBufC::New(4);delete _a;if (_a==NULL)_r=KErrNoMemory;else _r=_x;}
#define __DEBUGTESTLEAVE {HBufC* _a=HBufC::NewL(4);delete _a;}

#else

#define __TESTOOM(_r, _x) _r = _x
#define __TESTOOMD(_r, _x) TInt _r = _x
#define __DEBUGTESTLEAVE

#endif


#endif // __TESTOOM_H__

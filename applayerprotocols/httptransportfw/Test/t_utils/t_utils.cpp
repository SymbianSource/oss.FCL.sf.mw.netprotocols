// Copyright (c) 2001-2010 Nokia Corporation and/or its subsidiary(-ies).
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
// contains utilities such as a global comms init function for starting c32 and loading device drives for test code
// 
//

#include "t_utils.h"

#if !defined __E32BASE_H__
#include <e32base.h>
#endif

// for StartC32()
#if !defined __C32COMM_H__
#include <c32comm.h>
#endif


#if defined (__WINS__)
#define PDD_NAME		_L("ECDRV")
#else
#define PDD_NAME		_L("EUART1")
#define PDD2_NAME		_L("EUART2")
#define PDD3_NAME		_L("EUART3")
#define PDD4_NAME		_L("EUART4")
#endif

#define LDD_NAME		_L("ECOMM")

void RHttpTestUtils::InitCommsL()
	{
#ifndef SIROCCO_CODE_MIGRATION
	TInt ret = User::LoadPhysicalDevice(PDD_NAME);
	User::LeaveIfError(ret);

#ifndef __WINS__
	User::LoadPhysicalDevice(PDD2_NAME);
	User::LoadPhysicalDevice(PDD3_NAME);
	User::LoadPhysicalDevice(PDD4_NAME);
#endif

	ret = User::LoadLogicalDevice(LDD_NAME);
	User::LeaveIfError(ret);

	ret = StartC32();
	User::LeaveIfError(ret);
#endif //SIROCCO_CODE_MIGRATION
	}


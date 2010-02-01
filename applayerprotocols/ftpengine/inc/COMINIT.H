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
* Initialise Comm Server
* Written by AdamG, October 1996
* 
*
*/



/**
 @file COMINIT.CPP
 @internalComponent
*/

#include <e32hal.h>

/**
@internalComponent
*/
#if defined (__WINS__)
#define PDD_NAME		_L("ECDRV")
#define LDD_NAME		_L("ECOMM")
#define LDD_FNAME		_L("ECOMM")
#endif

/**
@internalComponent
*/
GLDEF_C void CommInitL()
	{
#if defined (__WINS__)

	TInt err=User::LoadPhysicalDevice(PDD_NAME);
	if (err!=KErrNone && err!=KErrAlreadyExists)
		User::Leave(err);
	err=User::LoadLogicalDevice(LDD_NAME);
	if (err!=KErrNone && err!=KErrAlreadyExists)
		User::Leave(err);
#endif
	}


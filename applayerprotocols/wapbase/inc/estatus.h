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
// Created at: 04-Jan-99 10:35:54 AM
// Status enumeration for Data Collectors, Data Providers and Data Handlers.
// 
//

/**
 @file EStatus.h 
 @publishedAll
 @deprecated
*/
 
#if !defined(__ESTATUS_H__)
#define __ESTATUS_H__

#ifndef __E32DEF_H__
#include <e32def.h> // for TInt
#endif

// 
// values < 0 are error codes
//			0 - 100 are % processed
//		  > 100 are the enum values below...
//
enum TPluginStatus {	
					EPluginComplete=100,
					EPluginInitialised,
					EPluginWaitingForChild,
					EPluginProcessing,
					EPluginSuspend,
					EPluginCancelled,
					EConnectionChanged
				   };

#endif    // __ESTATUS_H__

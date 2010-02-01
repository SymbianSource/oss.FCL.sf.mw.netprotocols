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

#ifndef SRVREQS_H
#define SRVREQS_H

enum TServerRequest
	{
	EInitialise,
	ECreateInt,
	ECreateReal,
	ECreateString,
	EDelete,
	EGetInt,
	ESetInt,
	EGetReal,
	ESetReal,
	EGetString,
	ESetString,
	EFind,
	EFindEqInt,
	EFindEqReal,
	EFindEqString,
	EFindNeqInt,
	EFindNeqReal,
	EFindNeqString,
	EGetFindResult,
	ENotifyRequestCheck,
	ENotifyRequest,
	ENotifyCancel,
	ENotifyCancelAll,
	EGroupNotifyRequest,
	EGroupNotifyCancel,
	EReset,
	EResetAll,
	ETransactionStart,
	ETransactionCommit,
	ETransactionCancel,
	EMove,
	ETransactionState,
	ETransactionFail,
	EDeleteRange,
	ELastInTable=EDeleteRange,

	EGetSetParameters
	};

#if defined(__CENTREP_SERVER_PERFTEST__) || defined(__CENTREP_SERVER_MEMTEST__) || defined(__CENTREP_SERVER_CACHETEST__)

enum TServerGetSetParametersSubCmd
	{
	//Subcommands for CentRep server performance test which is run when __CENTREP_SERVER_PERFTEST__ macro is defined
	EGetPerfResults,
	ERestartPerfTests,
	EStopPerfTests,	
	//Subcommands for CentRep server memory test which is run when __CENTREP_SERVER_MEMTEST__ macro is defined
	EGetMemResults,
	//Subcommands for CentRep server cache test which is run when __CENTREP_SERVER_CACHETEST__ macro is defined
	EEnableCache,
	EDisableCache,
	// The identifier for last command. Should always remain at the end
	ELast
	};
#endif // __CENTREP_SERVER_PERFTEST__ || __CENTREP_SERVER_MEMTEST__ || __CENTREP_SERVER_CACHETEST__

#endif // SRVREQS_H

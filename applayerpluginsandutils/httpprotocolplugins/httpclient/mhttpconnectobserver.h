// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __MHTTPCONNECTOBSERVER_H__
#define __MHTTPCONNECTOBSERVER_H__

#include <e32std.h>

class MHttpConnectObserver
	{
public:

	virtual void ConnectCompleteL(TInt aConnectId) =0;

	virtual void ConnectErrorL(TInt aConnectId) =0;

	virtual void FailPendingTransactions(TInt aConnectId) =0;

	};

#endif	// __MHTTPCONNECTOBSERVER_H__

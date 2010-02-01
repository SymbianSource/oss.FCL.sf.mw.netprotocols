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

#ifndef __MBODYDATASUPPLIEROBSERVER_H__
#define __MBODYDATASUPPLIEROBSERVER_H__

#include <e32base.h>

class MBodyDataSupplierObserver
	{
public:	// methods

	virtual void NotifyMoreData() =0;

	};

#endif	// __MBODYDATASUPPLIEROBSERVER_H__

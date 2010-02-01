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

#ifndef __MDATASUPPLIEROBSERVER_H__
#define __MDATASUPPLIEROBSERVER_H__

#include <e32base.h>

class MDataSupplierObserver
	{
public:	// methods

	virtual void DataReady() =0;

	};

#endif	// __MDATASUPPLIEROBSERVER_H__

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

#ifndef __CDOMAINLIST_H__
#define __CDOMAINLIST_H__

#include <e32base.h>
#include <stringpool.h>
#include "MDomainList.h"

class CDomainList : public CBase,
					public MDomainList
	{
public:
	static CDomainList* NewL(TInt aListSize);
	~CDomainList();
	
	// From MDomainList
	virtual TInt IsDomainInList(const RStringF& aDomain) const;
	virtual void AddDomainToListL(RStringF aDomain);
	virtual TInt Count() const;
	
private:
	CDomainList(TInt aListSize);

private:
	TInt iListSize;
	RArray<RStringF> iDomainList;
	};
	
#endif // __CDOMAINLIST_H__

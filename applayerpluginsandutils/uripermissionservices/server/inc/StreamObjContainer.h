// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// SqlSrvObjContainer.h
// 
//

#ifndef __INETSRVSTREAMOBJCONTAINER_H__
#define __INETSRVSTREAMOBJCONTAINER_H__

#include <e32std.h>

/**
Indexed storage for server side URI stream objects.


@internalComponent
*/

template <class T> class RStreamObjContainer
	{
public:
	RStreamObjContainer();
	void Close();
	void AllocL();
	TInt Add(T* aObj);
	void Remove(TInt aHandle);
	T* Find(TInt aHandle) const;
	TInt Count() const;

private:	
	TInt MakeHandle(TInt aIndex) const;
	TInt MakeIndex(TInt aHandle) const;
	
private:	
	struct TEntry
		{
		T*		iObj;
		TInt	iNext;
		};
	enum {KGranularity = 16};
	//KMaxSize value depends on how many bits are used for handles - see
	//KStmtHandleMask and KStreamHandleMask in SqlUtil.h file.
	enum {KMaxSize = 0x7FF};
	TEntry*	iEntries;
	TInt	iSize;
	TInt 	iFree;
	};

#include "StreamObjContainer.inl"

#endif//__INETSRVSTREAMOBJCONTAINER_H__

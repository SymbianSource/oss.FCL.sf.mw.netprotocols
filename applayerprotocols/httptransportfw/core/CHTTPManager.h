// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __CHTTPMANAGER_H__
#define __CHTTPMANAGER_H__

#include <e32base.h>
#include <stringpool.h>


/** A singleton manager for HTTP Client.  The purpose of this class is to ensure that
	only a single instance of a string pool is used throughout HTTP - necessary to ensure
	that string comparisons in requests and responses succeed.  The singleton pattern is
	achieved by caching a pointer to the manager object in DLL's Thread-Local Storage.
*/

//##ModelId=3C4C188001C9
class CHTTPManager : public CBase
	{
public:

	/** Creates the object, or returns a pointer to the existing singleton.
		Note that the manager has a reference count; calls to this must be
		matched with a call to Release()
	*/
	//##ModelId=3C4C188001FE
	static CHTTPManager* CreateOrFindL();

	/** Decrements the reference count, and deletes if it is 0. (opposite of 
		CreateOrFindL())
	 */
	//##ModelId=3C4C188001FD
	void Release();

	/** Returns the string pool
	 */
	//##ModelId=3C4C188001FB
	RStringPool& StringPool();

	/** Returns the string pool
	 */
	//##ModelId=3C4C188001FC
	const RStringPool& StringPool()const ;

 private:

	/** Constructor
	 */
	//##ModelId=3C4C188001F3
	CHTTPManager();

	/** 2 phase construction
	 */
	//##ModelId=3C4C188001F2
	void ConstructL();

 	/** Destructor
	 */
	//##ModelId=3C4C188001F1
	~CHTTPManager();

private:

	/** The string pool
	 */
	//##ModelId=3C4C188001E9
	RStringPool iPool;

	/** The reference count
	 */
	//##ModelId=3C4C188001E0
	TUint iUseCount;
};

#include "CHTTPManager.inl"

#endif // __CHTTPMANAGER_H__


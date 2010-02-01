// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Classes for manipulating bookmarks and bookmark folders.
// @internalComponent
// 
//

#ifndef __BKMRKATTACHMENT_H__
#define __BKMRKATTACHMENT_H__

#include <centralrepository.h>
#include <bookmarkdef.h>
#include "repository.h"

/**
Internal representation of a bookmark attachment. Currently the only attachments are icons.
An attachment is made up of 8bit raw data plus and ID
*/
class CBkmrkAttachment: public CRepositoryAccessor
	{
public:
	static CBkmrkAttachment* NewL(CRepository& aRepository, const TDesC8& aData);
	~CBkmrkAttachment();

	Bookmark::TAttachmentId Id() const;
	void SetIdL();

	TInt Size() const;
	const TDesC8& Data() const;
	void SetDataL(const TDesC8& aData);

	// From MRepositoryAccessor
	TUint32 IndexBase();
	void SetIdFromIndexBase(TUint32 aIndexBase);
	void TransSaveL();
	void TransNewL();
	void TransLoadL();
	void TransRemoveL();

private:
	// From MRepositoryAccessor
	CBkmrkAttachment();
	void ConstructL(CRepository& aRepository, const TDesC8& aData);

private:
	Bookmark::TAttachmentId iId;
	HBufC8* 				iData;
	};

#endif //__BKMRKATTACHMENT_H__

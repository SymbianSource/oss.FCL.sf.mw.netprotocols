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
// Classes for Bookmark extended properties.
// @internalComponent
// 
//

#ifndef __BKMRKPROPERTIES_H__
#define __BKMRKPROPERTIES_H__

#include <e32base.h>
#include "bkmrkdb.h"
#include "propertyreg.h"

/**
Internal representation of additional properties. This class is onlt created and loaded form the repository 
when it's member data is first required.
*/
class CBkmrkProperties: public CRepositoryAccessor
	{
public:
	static CBkmrkProperties* NewL(CBookmarkBase& iParent, CBookmarkDb& aDb, CRepository& aRepository);
	virtual ~CBkmrkProperties();

	const TDesC& Description() const;
	void SetDescriptionL(const TDesC& aDescription);

	Bookmark::TAttachmentId IconId() const;
	void SetIconIdL(Bookmark::TAttachmentId aIconId);

	// From MRepositoryAccessor
	void CommitL();
	void SetDirty();
	TUint32 IndexBase();
	void SetIdFromIndexBase(TUint32 aIndexBase);
	virtual void TransSaveL();
	virtual void TransNewL();
	virtual void TransLoadL();
	virtual void TransRemoveL();

	// custom properties
	CPropertyList& CustomProperties();

	TTime LastModified () const;
	void SetLastModified ( const TTime& aTime );

protected:
	CBkmrkProperties(CBookmarkBase& aParent, CBookmarkDb& aDb, CRepository& aRepository);
	void ConstructL();
	
private:
	void SetModified ();
	
protected:
	CBookmarkBase&			iParent;
	CBookmarkDb&			iDatabase;
	CPropertyList*			iCustomProperties;
	HBufC* 					iDescription;
	Bookmark::TAttachmentId	iIconId;
	TTime 					iLastModified;
	TBool					iUpdateModifiedOnCommit;
	};

/**
Internal representation of extended properties. It is derived from CBkmrkProperties and so contains the same 
set of member data. In addition is holds further data that is specific to bookmarks and not folders.
*/
class CBkmrkExtendedProperties: public CBkmrkProperties
	{
public:
	static CBkmrkExtendedProperties* NewL(CBookmarkBase& aParent, CBookmarkDb& aDb, CRepository& aRepository);
	~CBkmrkExtendedProperties();

	const CAuthentication& AuthenticationL();
	void SetAuthenticationL(const CAuthentication& aAuthentication);

	TUint32 Proxy() const;
	void SetProxyL(TUint32 aProxy);

	TUint32 Nap() const;
	void SetNapL(TUint32 aNap);

	// From MRepositoryAccessor
	void TransSaveL();
	void TransNewL();
	void TransLoadL();
	void TransRemoveL();
	
private:
	CBkmrkExtendedProperties(CBookmarkBase& aParent, CBookmarkDb& aDb, CRepository& aRepository);
	void ConstructL();

private:
	CAuthentication*	iAuthentication;
	TUint32				iProxy;
	TUint32				iNap;
	};

#endif //__BKMRKPROPERTIES_H__

// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// DTD node header
// 
//

#if !defined(__CDTDNODE_H__)
#define __CDTDNODE_H__

#include <e32base.h>
#include <cbnfnode.h>

class CDTDNode : public CBNFNode
/**
Document Type Definition (DTD) node that forms part of a DTD tree.
@publishedAll
@deprecated
*/
{
public:
	//##ModelId=3B66675300CE
	inline ~CDTDNode();

	//##ModelId=3B66675300CD
	static inline CDTDNode* NewL();
	//##ModelId=3B66675300CC
	inline void AddCompletedAttributeL();

	//##ModelId=3B66675300C4
	const TDesC* CompletedAttrID();

	enum{ // attribute IDs start at -6000
		KCompletedAttrID = -6000
	};
private:
	//##ModelId=3B66675300C3
	inline CDTDNode();
};


#include <dtdnode.inl>

#endif // __CDTDNODE_H__

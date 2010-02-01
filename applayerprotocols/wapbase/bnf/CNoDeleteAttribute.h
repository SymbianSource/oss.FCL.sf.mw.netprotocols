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
//

#ifndef __CNODELETEATTRIBUTE_H__
#define __CNODELETEATTRIBUTE_H__

// System includes
//
#include <e32base.h>



template <class T>
class CNoDeleteAttributeT : public CBase
/**
    @publishedAll
    @deprecated
*/
	{
public:

	CNoDeleteAttributeT(T aAttribute) : iAttribute(aAttribute) {};
	~CNoDeleteAttributeT() {};

	T Attribute() { return iAttribute; }
	void SetAttribute(T aAttribute) { iAttribute = aAttribute; }

protected:

	T iAttribute;

	};

#endif	// __CNODELETEATTRIBUTE_H__

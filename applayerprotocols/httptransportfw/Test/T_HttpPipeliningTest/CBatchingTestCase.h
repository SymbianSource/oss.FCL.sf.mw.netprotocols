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

#ifndef __CBATCHINGTESTCASE_H__
#define __CBATCHINGTESTCASE_H__

#include <e32base.h>
#include <http.h>

#include "CPipeliningTestCase.h"

/*
The CBatchingTestCase is a simple class to inherit from CPipeliningTestCase to provide
default implementations from the pipelining test cases with the differences required for
batching test cases. This only affects the EnableBatching() method. All other default 
implementations are inherited from the CPipeliningTestCase class.
*/
class CBatchingTestCase : public CPipeliningTestCase
	{
public:	
	~CBatchingTestCase();
	
	/*
	This method will indicate whether to enable batching (request buffering). The batching
	test cases will probably require batching enables so the default implementation is to
	return ETrue.
	*/
	virtual TBool EnableBatching() const;
	
protected:
	CBatchingTestCase();

	};
	
#endif // __CBATCHINGTESTCASE_H__

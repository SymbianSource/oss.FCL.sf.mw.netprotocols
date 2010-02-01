// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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

/**
 @file thttptable.h 
 @internalAll
 @released
*/

#ifndef	__THTTPTABLE_H__
#define	__THTTPTABLE_H__

// System includes
#include <e32std.h>
#include <stringpool.h>
#include <httpstringconstants.h>

class THTTPTable
{
private:
	THTTPTable();
		
public:
	IMPORT_C static const TStringTable& Table();	
};

#endif //__THTTPTABLE_H__


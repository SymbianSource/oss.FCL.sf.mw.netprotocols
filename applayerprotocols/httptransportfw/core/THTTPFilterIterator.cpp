// Copyright (c) 2000-2009 Nokia Corporation and/or its subsidiary(-ies).
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

// System includes
#include <e32std.h>
#include <http/thttpfilteriterator.h>// Class signature

// Local includes
#include "CHTTPSession.h"

EXPORT_C TBool THTTPFilterIterator::AtEnd() const
	{
	return (iPos == iImplementation->FilterQueue().Count());
	}

EXPORT_C THTTPFilterIterator& THTTPFilterIterator::operator++()
	{
	if (iName.IsNull())
		{
		iPos++;
		}
	else
		{
		for (iPos++; iPos < iImplementation->FilterQueue().Count() &&
				 iName != iImplementation->FilterQueue()[iPos].iName; iPos++)
			;
		}
	return *this;
	}

EXPORT_C const THTTPFilterRegistration& THTTPFilterIterator::operator()() const
	{
	return iImplementation->FilterQueue()[iPos];
	}

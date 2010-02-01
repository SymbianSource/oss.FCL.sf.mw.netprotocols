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

#ifndef __MWSPCOMETHODOBSERVER_H__
#define __MWSPCOMETHODOBSERVER_H__

// System includes
#include <e32std.h>
#include <http/framework/mrxdataobserver.h>

// Forward declarations
class CWspCOTransaction;

// @todo docing

//##ModelId=3C4C41AE01BD
class MWspCOMethodObserver : public MRxDataObserver
	{
public:	// Methods

/**	@fn				HandleMethodAbort(CWspCOTransaction& aTransaction) =0

	@since			7.0
	@param			aTransaction	A reference to the aborted method transaction.
	@post			The aborted method transaction has been dealt with.
*/
	//##ModelId=3C4C41AE01D2
	virtual void HandleMethodAbort(CWspCOTransaction& aTransaction) =0;

/**	@fn				NotifyPendingCompletingMethod() =0

	@since			7.0
*/
	virtual void NotifyPendingCompletingMethod() =0;

/**	@fn				NotifyMethodComplete() =0

	@since			7.0
*/
	virtual void NotifyMethodComplete() =0;
	
	};

#endif	// __MWSPCOMETHODOBSERVER_H__

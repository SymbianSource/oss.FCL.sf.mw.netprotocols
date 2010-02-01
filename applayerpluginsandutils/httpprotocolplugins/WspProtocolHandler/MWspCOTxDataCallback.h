// Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __MWSPCOTXDATACALLBACK_H__
#define __MWSPCOTXDATACALLBACK_H__

#include <e32std.h>

//##ModelId=3C4C419A0151
class MWspCOTxDataCallback
	{
public:	// Methods

/**	@fn				SendInvokePrimitive() =0
	Informs the Tx Data object observer that it should send a S-MethodInvoke or
	S-MethodInvokeData primitive.
	@since			7.0
*/
	virtual void SendInvokePrimitive() =0;

/**	@fn				AbortInvoke() =0
	Informs the Tx Data object observer that the method should be aborted.
	@see			7.0
*/
	virtual void AbortInvoke() =0;

	};

#endif	// __MWSPCOTXDATACALLBACK_H__

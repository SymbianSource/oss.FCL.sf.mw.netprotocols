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

#ifndef __MWSPCORXDATACALLBACK_H__
#define __MWSPCORXDATACALLBACK_H__

#include <e32std.h>

//##ModelId=3C4C41B40021
class MWspCORxDataCallback
	{
public:	// Methods

/**	@fn				AbortResponse() =0
	Informs the Rx Data object observer that the method should be aborted.
	@since			7.0
*/
	//##ModelId=3C4C41B40037
	virtual void AbortResponse() =0;

/**	@fn				SendResponsePrimitive() =0
	Informs the Tx Data object observer that it should send the response to the
	S-MethodResult or S-MethodResultData primitive.
	@since			7.0
*/
	virtual void SendResponsePrimitive() =0;

	};

#endif	// __MWSPCORXDATACALLBACK_H__

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

#ifndef __MWSPPRIMITIVESENDERCALLBACK_H__
#define __MWSPPRIMITIVESENDERCALLBACK_H__

#include <e32std.h>
#include <wsp/wsptypes.h>

// Types used for arguments in this file (e.g. TWspBearer) are from the Wap namespace
using namespace Wap;

/** The MWspPrimitiveSenderCallback is used to perform an automatic session 
	connect. An active object uses this callback to send a WSP primitive.
*/
//##ModelId=3C4C419D0213
class MWspPrimitiveSenderCallback
	{
public:	// Methods

/**	@fn				SendPrimitiveL(TWspPrimitive aPrimitive) =0
	Intended Usage	:
	@since			7.0
	@post			The specified primitive has been sent.
*/
	//##ModelId=3C4C419D0229
	virtual void SendPrimitiveL(TWspPrimitive aPrimitive) =0;

/** @fn				WspPrimitiveSenderCallbackError(TInt aError) =0
	Intended Usage	:	Error handling function for when SendPrimitiveL() 
						leaves.
	@since			7.0
	@param			aError	The error with which SendPrimitiveL() left.
	@return			An error code. If the input error value could be handled, 
					then KErrNone is returned, otherwise the input error code
					is returned.
*/
	//##ModelId=3C4C419D0227
	virtual TInt WspPrimitiveSenderCallbackError(TInt aError) =0;

	};

#endif	// __MWSPPRIMITIVESENDERCALLBACK_H__

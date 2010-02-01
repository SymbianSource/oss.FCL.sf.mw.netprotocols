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

#ifndef __CWSPPRIMITIVESENDER_H__
#define __CWSPPRIMITIVESENDER_H__

#include <e32base.h>
#include <wsp/wsptypes.h>

class MWspPrimitiveSenderCallback;

// Types used for arguments in this file (e.g. TWspBearer) are from the Wap namespace
using namespace Wap;

// @todo docing
//##ModelId=3C4C41B4025C
class CWspPrimitiveSender : public CActive
	{
public:	// Methods

/**	@fn				NewL(MWspPrimitiveSenderCallback& aCallback)
	Intended Usage	:	Static factory constructor.
	@since			7.0
	@param			aCallback	A reference to the callback object that will 
								send the WSP primitive.
	@return			A pointer to a fully initialised object.
*/
	//##ModelId=3C4C41B402A5
	static CWspPrimitiveSender* NewL(MWspPrimitiveSenderCallback& aCallback);

/**	@fn				~CWspPrimitiveSender()
	Intended Usage	:	Destructor.
	@since			7.0
*/
	//##ModelId=3C4C41B402A4
	~CWspPrimitiveSender();

/**	@fn					InitiateSend(TWspPrimitive aPrimitive)
	Intended Usage	:	Sets this object active and completes it self. This 
						ensures that the RunL() is entered as soon as possible
						after this function ends. The RunL() does the session 
						connect.
	@since			7.0
	@post			The embeddded active object has been set active and 
					completed.
*/
	//##ModelId=3C4C41B402A2
	void InitiateSend(TWspPrimitive aPrimitive);

private:	// Methods from CActive

/**	@fn				RunL()
	Intended Usage	:	Called by active schedule when the request is complete.
						This will invoke the callback object to send the WSP
						primitive.
	@since			7.0
	@todo			adds leaves
	@pre			The request has been completed
	@post			The WSP primitive has been sent.
*/
	//##ModelId=3C4C41B4029A
	virtual void RunL();

/**	@fn				RunError(TInt aError)
	Intended Usage	:	Error handling function for when RunL() leaves. Defers 
						action to callback object.
	@since			7.0
	@param			aError	The error with which RunL() left.
	@return			An error code. If the input error value could be handled, 
					then KErrNone is returned, otherwise the input error code
					is returned. This will cause a panic.
*/
	//##ModelId=3C4C41B40298
	virtual TInt RunError(TInt aError);

/**	@fn				DoCancel()
	Intended Usage	:	Part of Active Objects framework. For this active object
						this function does nothing.
	@since			7.0
*/
	//##ModelId=3C4C41B40291
	virtual void DoCancel();

private:	// Methods

/**	@fn				CWspPrimitiveSender(MWspPrimitiveSenderCallback& aCallback)
	Intended Usage	:	First phase constructor.
	@since			7.0
	@param			aCallback	A reference to the callback object that will 
								send the WSP primitive.
*/
	//##ModelId=3C4C41B4028F
	CWspPrimitiveSender(MWspPrimitiveSenderCallback& aCallback);

/**	@fn				ContructL()
	Intended Usage	: Second phase constructor. Does any necessary allocations.
	@since			7.0
	@pre			First phase construction has been done.
	@post			The object is fully initialised.
*/
	//##ModelId=3C4C41B4028E
	void ConstructL();
	
private:	// Attributes

	/** The callback object that will send the WSP primitive.
	*/
	//##ModelId=3C4C41B4027C
	MWspPrimitiveSenderCallback&	iCallback;

	/** The primitive to be sent.
	*/
	//##ModelId=3C4C41B40273
	TWspPrimitive					iPrimitive;

	};

#endif	// __CWSPPRIMITIVESENDER_H__

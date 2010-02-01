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

#ifndef __CCONNECTIONTIMER_H__
#define __CCONNECTIONTIMER_H__

#include <e32base.h>

class MConnectionTimerCallback;

/** The class CConnectionTimer is used by the Wsp Protocol Handler to ensure
	 that a WSP session establishment times-out.
*/
//##ModelId=3C4C41AE014F
class CConnectionTimer : public CTimer
	{
public:	// Methods

/**	@fn				NewL(MConnectionTimerCallback& aCallback)
	Intended Usage	:	Static factory constructor.
	@since			7.0
	@param			aCallback	A reference to the callback object that handles
								the timer complete event.
	@return			A pointer to a fully initialised object.
*/
	//##ModelId=3C4C41AE018D
	static CConnectionTimer* NewL(MConnectionTimerCallback& aCallback);

/**	@fn				~CConnectionTimer()
	Intended Usage	:	Standard destructor.
	@since			7.0
*/
	//##ModelId=3C4C41AE018C
	~CConnectionTimer();

/**	@fn				Start(TInt aDelayTime)
	Intended Usage	:	Starts the timer.
	@since			7.0
	@param			aDelayTime	The time delay, in microseconds, by when the 
								timer should complete.
	@post			The object is active.
*/
	//##ModelId=3C4C41AE0184
	void Start(TInt aDelayTime);

private:	// Methods from CActive

/**	@fn				RunL()
	Intended Usage	:	Called by active schedule when the timer is complete.
	@since			7.0
	@todo			add leaves
	@pre			The timer has completed.
	@post			The connection time-out has been handled.
*/
	//##ModelId=3C4C41AE0183
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
	//##ModelId=3C4C41AE0181
	virtual TInt RunError(TInt aError);

private:	// Methods

/**	@fn				CConnectionTimer(MConnectionTimerCallback& aCallback)
	Intended Usage	:	First phase constructor.
	@since			7.0
	@param			aCallback	A reference to the callback object that handles
								the timer complete event.
*/
	//##ModelId=3C4C41AE0178
	CConnectionTimer(MConnectionTimerCallback& aCallback);

/**	@fn				ContructL()
	Intended Usage	: Second phase constructor. Does any necessary allocations.
	@since			7.0
	@pre			First phase construction has been done.
	@post			The object is fully initialised.
*/
	//##ModelId=3C4C41AE0177
	void ConstructL();

private:	// Attributes

	/** A reference to the callback object.
	*/
	//##ModelId=3C4C41AE016F
	MConnectionTimerCallback&		iCallback;

	};

#endif	// __CCONNECTIONTIMER_H__

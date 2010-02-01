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

#ifndef __TEVENTREGISTRATION_H__
#define __TEVENTREGISTRATION_H__

#include <http/thttpevent.h>

// An event's registration in the filter queue
//##ModelId=3C4C1885025C
class TEventRegistration
	{
 public:
	//##ModelId=3C4C18850298
	inline TEventRegistration(THTTPEvent aEvent, TInt aDirection, 
							  TInt aStartFilter);
	//##ModelId=3C4C188502A4
	inline TEventRegistration();

	// The event
	//##ModelId=3C4C18850290
	THTTPEvent iEvent;
	// The direction it's going in
	//##ModelId=3C4C18850286
	TInt iDirection;
	// The filter it starts at
	//##ModelId=3C4C1885027C
	TInt iStartFilter;
	};

inline TEventRegistration::TEventRegistration(THTTPEvent aEvent, 
											  TInt aDirection, 
											  TInt aStartFilter)
		: iEvent(aEvent), iDirection(aDirection), iStartFilter(aStartFilter)
	{
	};

inline TEventRegistration::TEventRegistration()
	{
	};

#endif

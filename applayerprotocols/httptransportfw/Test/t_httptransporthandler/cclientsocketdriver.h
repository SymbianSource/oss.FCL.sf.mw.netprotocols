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

#ifndef __CCLIENTSOCKETDRIVER_H__
#define __CCLIENTSOCKETDRIVER_H__

#include <e32base.h>

#include "csocketdriver.h"
#include "msocketdriverobserver.h"

class MInputStream;
class MOutputStream;
class MClientDriverObserver;

/**	@class		CClientSocketDriver
	The CClientSocketDriver class is a specilisation of the CSocketDriver class.
	Its behaviour is that of a client - it sends a data, mimicking a request, 
	and then expects to receive some data, i.e. the response. After it has 
	received the response the connetion is closed.
	@componentInternal		
	@see		CSocketDriver
*/
class CClientSocketDriver : public CSocketDriver,
							public MSocketDriverObserver
	{
public:	// methods

	static CClientSocketDriver* NewL(
									MDriverObserver&		aDriverObserver,
									MClientDriverObserver&	aObserver, 
									MInputStream&			aInputStream, 
									MOutputStream&			aOutputStream,
									TBool					aSecure
									);

	virtual ~CClientSocketDriver();

private:	// methods from CSocketDriver

	virtual void SetCmdsL();

	virtual const TDesC8& SendData();

	virtual const TDesC8& ReceiveData();

private:	// methods from MSocketDriverObserver

	virtual void SocketClosed();

private:	// methods

	CClientSocketDriver(MDriverObserver& aDriverObserver, MClientDriverObserver& aObserver, TBool aSecure); 

private:	// attributes

/**	The client socket driver observer.
*/
	MClientDriverObserver&	iObserver;
	};

#endif	// __CCLIENTSOCKETDRIVER_H__

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

#ifndef __CSERVERSOCKETDRIVER_H__
#define __CSERVERSOCKETDRIVER_H__

#include <e32base.h>

#include "csocketdriver.h"
#include "msocketdriverobserver.h"

class MInputStream;
class MOutputStream;
class MServerDriverObserver;

/**	@class		CServerSocketDriver
	The CServerSocketDriver class is a specilisation of the CSocketDriver class.
	Its behaviour is that of a server - it expects receive some data, i.e. a 
	request, and then it sends some data, i.e. the response. After it has sent 
	the data it expects the connection to be closed.
	@componentInternal		
	@see		CSocketDriver
*/
class CServerSocketDriver : public CSocketDriver,
							public MSocketDriverObserver
	{
public:	// methods

	static CServerSocketDriver* NewL(
									MDriverObserver&		aDriverObserver,
									MServerDriverObserver&	aObserver, 
									MInputStream&			aInputStream, 
									MOutputStream&			aOutputStream
									);

	virtual ~CServerSocketDriver();

private:	// methods from CSocketDriver

	virtual void SetCmdsL();

	virtual const TDesC8& SendData();

	virtual const TDesC8& ReceiveData();

private:	// methods from MSocketDriverObserver

	virtual void SocketClosed();

private:	// methods

	CServerSocketDriver(MDriverObserver& aDriverObserver, MServerDriverObserver& aObserver); 

private:	// attributes

/**	The server socket driver observer.
*/
	MServerDriverObserver&	iObserver;

	};

#endif	// __CSERVERSOCKETDRIVER_H__

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

/**
 @file CNwssMethodInfo.h
*/

#ifndef __CNWSSCONNECTGUARD_H__
#define __CNWSSCONNECTGUARD_H__

// System includes
#include <e32base.h>
#include <stringpool.h>
#include <wsp/wsptypes.h>

class CUri8;
class TUriC8;
class MWspCOMethodInvoker;
class MWspCOMethodCallback;
class MWspCOSessionCallback;

/**
	@class			CNwssConnectGuard
	@since			7.0
	This class encapsulates the functionality to ensure that the Wsp CO Session 
	states tables are honoured by the NWSS WAP Stack. Specifically, during WTLS
	connection, there is a gap from when the S-Connect.req is received from the 
	protocol handler to when the RWSPCOConn receives that primitive. During this 
	time the protocol handler thinks that the Wsp CO Session state is Connecting 
	and will method invocation and allow the S-Disconnect.req primitive to be sent.

	If a T-MethodInvoke.req is received by the transport handler, this class will
	store the information and send the primitive once it is told that the 
	S-Connect.req has been sent to the WAP Stack.

	Also, if the S-Disconnect.req is received by the transport handler, it can use
	this class to send the T-MethodAbort.ind (if necessary) and the S-Disconnect.ind.

	Similarly, if the transport handler	receives T-MethodAbort.req.
 */
//##ModelId=3C9B094D0363
class CNwssConnectGuard : public CActive
	{
public:	// Methods

/**	@fn				NewL(MWspCOMethodInvoker& aMethodInvoker, MWspCOSessionCallback& aSessionCallback)
	Static factory c'tor.
	@since			7.0
	@param			aMethodInvoker		A reference to the Wsp CO method invoker.
	@param			aSessionCallback	A reference to the Wsp CO session callback.
*/
	//##ModelId=3C9B094E0094
	static CNwssConnectGuard* NewL(MWspCOMethodInvoker& aMethodInvoker, MWspCOSessionCallback& aSessionCallback);

/**	@fn				~CNwssConnectGuard()
	Standard destructor.
	@since			7.0
*/
	//##ModelId=3C9B094E008A
	~CNwssConnectGuard();

/**	@fn				SendDisconnectInd()
	Sends the S-Disconnect.ind primitive to the Wsp CO Session callback. If a
	T-MethodInvoke.req was received, then the T-MethodAbort.ind primitive is sent
	to the Wsp CO Method callback.
	@since			7.0
	@param			aReason		A parameter that indicates the reason for the 
								disconnection.
*/
	//##ModelId=3C9B094E0082
	void SendDisconnectInd(Wap::TWspReason aReason);

/**	@fn				SendMethodInvokeReq()
	Sends the S-MethodInvoke.req primitive.
	@since			7.0
*/
	//##ModelId=3C9B094E0081
	void SendMethodInvokeReq();

/**	@fn				SendMethodAbortInd()
	Sends the T-MethodAbort.ind primitive to the Wsp CO Method callback.
	@since			7.0
*/
	//##ModelId=3C9B094E0080
	void SendMethodAbortInd();

/**	@fn				ReceivedMethodInvokeReqL(MWspCOMethodCallback& aMethodCallback, RStringF aMethod, const TUriC8& aRequestUri, const TDesC8& aRequestHeaders, const TDesC8& aRequestBody)
	Stores the method invocation information.
	@warning		Will panic with EMORExceeded if there already is method info stored.
	@param			aMethodCallback		A reference to the Wsp CO method callback
										object for this method.
	@param			aMethod				The method type.
	@param			aRequestUri			A reference to the request Uri.
	@param			aRequestHeaders		A reference to a buffer with the request
										header data.
	@param			aRequestBody		A reference to a buffer with the request
*/
	//##ModelId=3C9B094E004E
	void ReceivedMethodInvokeReqL(
								 MWspCOMethodCallback&		aMethodCallback, 
								 RStringF					aMethod, 
								 const TUriC8&				aRequestUri, 
								 const TDesC8&				aRequestHeaders, 
								 const TDesC8&				aRequestBody
								 );

private:	// Methods from CActive

	//##ModelId=3C9B094E0045
	virtual void RunL();

	//##ModelId=3C9B094E0044
	virtual void DoCancel();

private:	// Methods

/**	@fn				CNwssConnectGuard(MWspCOMethodInvoker& aMethodInvoker, MWspCOSessionCallback& aSessionCallback)
	Constuctor. 
	@since			7.0
	@param			aMethodInvoker		A reference to the Wsp CO method invoker.
	@param			aSessionCallback	A reference to the Wsp CO session callback.
*/
	//##ModelId=3C9B094E0031
	CNwssConnectGuard(MWspCOMethodInvoker& aMethodInvoker, MWspCOSessionCallback& aSessionCallback);

/**	@fn				ResetMethodInfo()
	Resets the method info. Releases resources.
	@since			7.0
*/
	//##ModelId=3C9B094E0030
	void ResetMethodInfo();

/**	@fn				CompleteSelf()
	Completes the request.
	@since			7.0
*/
	//##ModelId=3C9B094E0026
	void CompleteSelf();

private:	// Attributes

	/** A reference to the Wsp CO method invoker object.
	*/
	//##ModelId=3C9B094E001E
	MWspCOMethodInvoker&			iMethodInvoker;

	/** A reference to the Wsp CO session callback.
	*/
	//##ModelId=3C9B094D03E6
	MWspCOSessionCallback&			iSessionCallback;

	/** A reference to the Wsp CO method callback object for this method.
	*/
	//##ModelId=3C9B094D03D3
	MWspCOMethodCallback*			iMethodCallback;

	/** The method type.
	*/
	//##ModelId=3C9B094D03C9
	RStringF						iMethod;

	/** The request Uri.
	*/
	//##ModelId=3C9B094D03BD
	CUri8*							iRequestUri;

	/** The request header data.
	*/
	//##ModelId=3C9B094D03AB
	HBufC8*							iRequestHeaders;

	/** The request body data.
	*/
	//##ModelId=3C9B094D03A1
	HBufC8*							iRequestBody;

	/** A flag indicating that an S-Disconnect.ind be sent
	*/
	//##ModelId=3C9B094D0397
	TBool							iSendDisconnect;

	/** A parameter that indicates the reason for the S-Disconnect.ind
	*/
	//##ModelId=3C9B094D038D
	Wap::TWspReason					iDisconnectReason;

	};

#endif	// __CNWSSCONNECTGUARD_H__

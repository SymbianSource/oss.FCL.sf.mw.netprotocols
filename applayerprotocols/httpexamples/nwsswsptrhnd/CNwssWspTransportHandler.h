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

#ifndef __CNWSSWSPTRANSPORTHANDLER_H__
#define __CNWSSWSPTRANSPORTHANDLER_H__

// System includes
#include <wsp/cwsptransporthandler.h>
#include <wsp/mwspcosessioninvoker.h>
#include <wsp/mwspcomethodinvoker.h>
#include <wapcli.h>

// Local includes
#include "cnwsswspsession.h"
#include "mnwsswapserver.h"
#include "mconnectioninfoprovider.h"
#include "cnwsswspcosession.h"

// Forward declarations
class MWspCOPushInvoker;
class MWspCLMethodInvoker;
class MWspCLPushInvoker;

/**
	@since			7.0
	This class provides a concrete implementation of the CWspTransportHandler
	interface, specifically for the NWSS Rainbow 2.1 WAP Stack, which is used on
	the Hurricane release of Symbian OS.  The present implementation supports only
	the Connection-Oriented Session and Method services.  In the future,
	Connectionless and Push may be added.

	There is no real behaviour in this class.  It acts mainly as a container for
	the objects that provide the various services.  Also, it implements a set of
	additional Mix-in interfaces, which provide internal services to other classes
	in the transport handler.

	By implementing MSecurityPolicy, it can supply default policy values when the
	WTLS handshake takes place, if a security policy plug-in is not provided by the
	client.

	The class owns the main WAP protocol stack server handle, RWAPServ.  It
	provides access to this for other classes in the transport handler by
	implementing the narrow interface MNwssWapServer.  It provides access to the
	client's Capability Info, Session Headers and Proxy Info services by
	implementing the MConnectionInfoProvider interface and offering it to other
	classes in the transport handler.
 */

//##ModelId=3BBD8B17018B
class CNwssWspTransportHandler : public CWspTransportHandler,
								 public MNwssWapServer,
								 public MConnectionInfoProvider
	{
public: // creation/deletion

/**Factory construction of the transport handler.
	@since			7.0
	@param			aInstantiationParams	(in) Points to a structure containing
												 the parameters sent to the parent
												 class constructor.
	@return			The newly-created transport handler object.
	@leave			KErrNoMemory.
 */
	//##ModelId=3C4C49C0033B
	static CNwssWspTransportHandler* NewL(TAny* aInstantiationParams);

/** Destructor.
	@since			7.0
 */
	//##ModelId=3C4C49C00286
	virtual ~CNwssWspTransportHandler();

public: // API methods inherited from CWspTransportHandler

/** Query the services supported by this instantiation of the WSP
					transport handler plug-in.
	@since			7.0
	@return			A bit-field set of flags indicating the service supported.
	@see			CWspTransportHandler
 */
	//##ModelId=3C4C49BF0112
	virtual CWspTransportHandler::TWspSupportedServices SupportedServices() const;

/**Obtain the connection-oriented session invocation API.
	@since			7.0
	@return			The interface to be used by a client to make invocations on the
					connection-oriented session service.
	@see MWspCOSessionInvoker
 */
	//##ModelId=3C4C49BF005E
	virtual MWspCOSessionInvoker& COSessionInvoker();

/** Obtain the connection-oriented method invocation API.
	@since			7.0
	@return			The interface to be used by a client to make invocations on the
					connection-oriented transaction service.
	@see MWspCOMethodInvoker
 */
	//##ModelId=3C4C49BE0392
	virtual MWspCOMethodInvoker& COTransactionInvoker();

/** Obtain the connection-oriented push invocation API.
	@since			7.0
	@return			The interface to be used by a client to make invocations on the
					connection-oriented push service.
	@see MWspCOPushInvoker
 */
	//##ModelId=3C4C49BE02DE
	virtual MWspCOPushInvoker& COPushInvoker();

/** Obtain the connectionless method invocation API.
	@since			7.0
	@return			The interface to be used by a client to make invocations on the
					connectionless transaction service.
	@see MWspCLMethodInvoker
 */
	//##ModelId=3C4C49BE0229
	virtual MWspCLMethodInvoker& CLMethodInvoker();

/** Obtain the connectionless push invocation API.
	@since			7.0
	@return			The interface to be used by a client to make invocations on the
					connectionless push service.
	@see MWspCLPushInvoker
 */
	//##ModelId=3C4C49BE0175
	virtual MWspCLPushInvoker& CLPushInvoker();

	/** Get the Server Certificate for the current session.
	@param	aServerCert A TCertInfo which will be filled with the certificate information
	@return	An error code.  KErrNone if aServerCert has been completed, otherwise one of 
			the system wide error codes
	*/
	//##ModelId=3C9B0947024C
	virtual TInt ServerCert(TCertInfo& aCertInfo) const;

private: // methods inherited from MNwssWapServer

/**Obtain a handle for the WAP Stack server.
	@since			7.0
	@return			The WAP Stack server session handle.
 */
	//##ModelId=3C4C49BE00AD
	virtual RWAPServ& WapStack();

private: // methods inherited from MConnectionInfoProvider

/** Obtain the client's provider of proxy information.
	@since			7.0
	@return			The interface supplied by the client through which proxy
					information is obtained.
 */
	//##ModelId=3C4C49BD03C2
	virtual MWspProxyInfoProvider& ProxyInfoProvider() const;

/** Obtain the client's provider of capability information.
	@since			7.0
	@return			The interface supplied by the client through which capability
					information is obtained.
 */
	//##ModelId=3C4C49BD0304
	virtual MWspCapabilityProvider& CapabilityProvider() const;

/** Obtain the client's provider of session headers.
	@since			7.0
	@return			The interface supplied by the client through which client
					session headers are obtained, and to which server (i.e. proxy)
					session headers are returned.
 */
	//##ModelId=3C4C49BD023C
	virtual MWspSessionHeadersProvider& SessionHeadersProvider() const;

private: // methods inherited from MSecurityPolicy

/** Ask the client to accept or reject a certificate. Not
					applicable to TLS, as the SSL layer does this itself.
					The call is asynchronous, returning the result via the TRequestStatus.
	@since			7.0
	@param			aServerCert		(in) A structure containing the certificate details.
	@param			aStatus			(out) A status flag used to indicate errors. A value of 0 is used to indicate
									that the cert is untrusted and 1 that it is trusted
 */
	//##ModelId=3C4C49BC03DF
	virtual void ValidateUntrustedServerCert(TCertInfo& aServerCert, TRequestStatus& aStatus) const;

/** Cancel a previous request to ask the client to accept or reject
					an untrusted WTLS server certificate.
	@since			7.0
 */
	//##ModelId=3C4C49BC032B
	virtual void CancelValidateUntrustedServerCert();

/** Obtain an array of WTLS cipher suites.  Not applicable to TLS clients.
	@since			7.0
	@return			An array of cipher suites, consisting of paired bulk encryption
					algorithms and MAC algorithms.  The array must be ordered with
					strongest cipher suites at the start.
 */
	//##ModelId=3C4C49BC026D
	virtual const RArray<TWtlsCipherSuite>& GetWtlsCipherSuites();

/** Obtain a list of TLS cipher suites.  Not applicable to WTLS
					clients.
	@since			7.0
	@return			An descriptor containing a list of the TLS cipher suites. Each cipher suite is a 2 byte pair
 */
	//##ModelId=3C4C49BC01B8
	virtual const TDesC8& GetTlsCipherSuites();

/** Obtain the WTLS key exchange suites.  Not applicable
					to TLS clients.
	@since			7.0
	@return			An array of the WTLS key exchange suites.  The array must be ordered
					with authenticating key exchange suites at the start followed by
					anonymous key exchange suites if desired.  Within each group of keys
					longer keys should precede shorter ones.
 */
	//##ModelId=3C4C49BC010E
	virtual const RArray<TWtlsKeyExchangeSuite>& GetWtlsKeyExchangeSuites();

/** Obtain arbitrary, named security policy properties, in a generic
					'stringified' form.
	@since			7.0
	@param			aPropertyName	(in) The policy property name.
	@param			aPropertyValue	(out) The property value. Parsing of the value is
									left to the caller. The caller must close the
									RString.
	@return			An error code: KErrNone if the property exists or KErrNotFound if
					it doesn't.
 */
	//##ModelId=3C4C49BB02E3
	virtual TInt GetNamedPolicyProperty(RStringF aPropertyName, RString& aPropertyValue);

/**	Reserve a slot in the v-table to preserve future BC
	@since			7.0
 */
	//##ModelId=3C4C49BB0239
	virtual void Reserved1();

/**Reserve a slot in the v-table to preserve future BC
	@since			7.0
 */
	//##ModelId=3C4C49BB0199
	virtual void Reserved2();

private: // methods

/** Normal constructor.  The bundled instantiation params are
					passed to the parent class constructor. Ownership of the
					parameters is _not_ passed across.
	@since			7.0
	@param			aInstantiationParams	(in) The set of instantiation parameters.
 */
	//##ModelId=3C4C49BB0080
	CNwssWspTransportHandler(TAny* aInstantiationParams);

/** Second phase construction.  Initial allocation of class data
					members is done here.
	@since			7.0
	@leave			System error codes, eg. KErrNoMemory.
 */
	//##ModelId=3C4C49BB001C
	void ConstructL();

private: // attributes

	/** Session with the WAP Stack server
	*/
	//##ModelId=3C4C49BB0012
	RWAPServ iWapStackHnd;

	/** Adapter for Connection-Oriented WSP sessions
	*/
	//##ModelId=3C4C49BA03E6
	CNwssWspCOSession* iWspCOSession;

	/** Default security policy values for WTLS cipher suites
	*/
	//##ModelId=3C4C49BA03D2
	RArray<TWtlsCipherSuite> iDefSecPolCipherSuites;

	/** Default security policy values for WTLS key exchange suites
	*/
	//##ModelId=3C4C49BA03BE
	RArray<TWtlsKeyExchangeSuite> iDefSecPolKeyExchSuite;
	};


#endif // __CNWSSWSPTRANSPORTHANDLER_H__

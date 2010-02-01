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

#ifndef __CWSPPROXYINFOPROVIDER_H__
#define __CWSPPROXYINFOPROVIDER_H__

// System includes
#include <e32base.h>
#include <wsp/mwspproxyinfoprovider.h>
#include <wsp/wsptypes.h>
#include <http/rhttpsession.h>

// User includes
#include "tproxyinfo.h"

/** The CWspProxyInfoProvider class implements the MWspProxyInfoProvider API. 
	The proxy information that is provided is obtained either from the HTTP
	session properties or from the Symbian OS Comms Database.

	The EWspProxyAddress property must be set in the HTTP session for the proxy
	info not to be taken from the Comms Database. The other properties 
	(described below) are optional. If these have been not set then the 
	following defaults will bw applied; the session will be unsecured CO over IP.

	The following HTTP properties define the proxy info;

	1) EWspProxyAddress	- a property of type RStringF that contains a bearer
	independent address.

	2) EWspBearer		- a property of type RStringF which should have a value
	of EWspIp for an IP (v4) bearer or a value of EWspSMS for an 8-bit WAP SMS
	bearer

	3) EWspSecurity		- a property of type RStringF which should contain the
	values EWspUseWtls, for a WTLS secured WSP session, or EWspDoNotUseWtls, for
	a non-secured WSP session.

	4) EWspSessionType	- a property of type RStringF that can have either the
	value EWspConnectionOriented for a CO session and a value of 
	EWspConnectionless for a CL session.

	For the case of an IP bearer, the following additional properties are 
	required to be defined;

	a) EWspLocalPort	- a property of type TInt that contains the local port
	number. Defaulted at 0, which means that the transport handler will allocate
	an appropriate value.

	b) EWspRemotePort	- a property of type TInt that contains the remote port
	number. Default value depends of the security status of the session.

	For the case of an SMS bearer, the following additional property is 
	required to be defined;

	a) EWspServiceNumber	- a property of type RStringF that should contain 
	the service centre number for the SMS bearer.
*/
//##ModelId=3BBDAFC80354
class CWspProxyInfoProvider : public CBase, public MWspProxyInfoProvider
	{
public:	// Methods

/**	
	Static factory constructor.
	@since			7.0
	@param			aSession	The HTTP session in use.
	@return			A pointer to a fully initialised object.
*/
	//##ModelId=3C4C419D0014
	static CWspProxyInfoProvider* NewL(RHTTPSession aSession);

/**Standard destructor.
	@since	7.0
*/
	//##ModelId=3C4C419D0000
	~CWspProxyInfoProvider();

/**Updates the proxy info. If the client has specified 
						proxy info in the session properties then that is the 
						proxy info that is used. If no proxy info has been 
						defined in the session properties, then the default WAP
						proxy in the Comms Database is used.
	@since			7.0
	@leave		The function will leave if the proxy info cannot be obtained
					from either the Comms DB or from the HTTP session properties.
	@post			The proxy info has been updated.
*/
	//##ModelId=3C4C419C03CA
	void UpdateProxyInfoL();

/**Updates the proxy info to use the redirected proxy
						address. If more than one address is given in 
						aRedirectedAddresses, then only the first one is used.
						A flag is set indicating that the proxy info should not
						be updated on the next call to UpdateProxyInfoL.
	@since			7.0
	@leave		The function will leave if the redirected address info cannot
					be extracted from the supplied data.
	@param			aRedirectedAddresses	A reference to a structure containing
											the parsed alternate addresses for the 
											redirected proxy.
	@post			The proxy information has been updated to use the redirected
					proxy address. The next call to UpdateProxyInfoL will not 
					cause the proxy info to be updated.
*/									
	//##ModelId=3C4C419C0384
	void SetTemporaryRedirectedProxyL(TWspRedirectedAddress& aRedirectAddress);

/**Updates the proxy info to use the redirected proxy
						address. If more than one address is given in 
						aRedirectedAddresses, then only the first one is used.
						A flag is set indicating that the proxy info should not
						be updated on the next call to UpdateProxyInfoL. Also,
						the session properties are updated with the new proxy 
						info.
	@since			7.0
	@leave		The function will leave if the redirected address info cannot
					be extracted from the supplied data. It will also leave if
					the redirected proxy info cannot be set in the HTTP session
					properties.
	@param			aRedirectedAddresses	A reference to a structure containing
											the parsed alternate address for the 
											redirected proxy.
	@post			The proxy information has been updated to use the redirected
					proxy address. The next call to UpdateProxyInfoL will not 
					cause the proxy info to be updated. The session properties 
					are updated with the new proxy info.
*/									
	//##ModelId=3C4C419C0348
	void SetPermanentRedirectedProxyL(TWspRedirectedAddress& aRedirectAddress);

/**Checks the current proxy info against the proxy info 
						specified by the client either through the HTTP session
						properties or the Comms Database. Also updates the 
						current proxy info if there has been a change. A flag is
						set indicating that the proxy info should not be updated
						on the next call to UpdateProxyInfoL.
	@since			7.0
	@leave		The function will leave if the proxy info cannot be obtained
					from either the Comms DB or from the HTTP session properties.
	@return			A boolean value of ETrue if the current proxy info defers 
					from the client specified proxy info.
	@post			The proxy info has been updated. The next call to 
					UpdateProxyInfoL will not cause the proxy info to be updated.
*/
	//##ModelId=3C4C419C032A
	TBool ProxyInfoChangedAndUpdateL();

/**Ensures that the next call to UpdateProxiInfoL actually 
						does an update.
	@since			7.0
	@post			The proxy info is unlocked so that the next call to 
					UpdateProxiInfoL actually does an update.
*/
	//##ModelId=3C4C419C0316
	void UnlockProxyInfo();

/**Removes all proxy information and closes the related resources.
	@since			7.0
	@post			The proxy info is removed; strings associated with it are closed.
*/
	//##ModelId=3C4C419C0302
	void ResetProxyInfo();

private:	// Methods from MWspProxyInfoProvider

/**@see			MWspProxyInfoProvider
*/
	//##ModelId=3C4C419C02D0
	virtual const TDesC8& ProxyAddress() const;

/**@see			MWspProxyInfoProvider
*/
	//##ModelId=3C4C419C028A
	virtual TUint16 RemotePort() const;

/**@see			MWspProxyInfoProvider
*/
	//##ModelId=3C4C419C0262
	virtual TUint16 LocalPort() const;

/**@see			MWspProxyInfoProvider
*/
	//##ModelId=3C4C419C0230
	virtual TWspBearer Bearer() const;

/**@see			MWspProxyInfoProvider
*/
	//##ModelId=3C4C419C0207
	virtual const TDesC8& ServiceCentreNumber() const;

/**@see			MWspProxyInfoProvider
*/
	//##ModelId=3C4C419C01DF
	virtual TWspSession WspSessionService() const;

/**@see			MWspProxyInfoProvider
*/
	//##ModelId=3C4C419C01C1
	virtual TBool SecureConnection() const;

private:	// Methods

/**First phase constructor.
	@since			7.0
*/
	//##ModelId=3C4C419C018F
	CWspProxyInfoProvider(RHTTPSession aSession);

/**Sets the currect proxy info to be that given the redirected
						address. Any fields that are not set in the redirected
						address are kept from the current proxy info.
	@since			7.0
	@leave		The function will leave if the redirected address info cannot
					be extracted from the supplied data.
	@param			aRedirectedAddress		A structure containing a single parsed
											alternate address for the redirected proxy.
	@post			The current proxy info contains the redirected proxt info.
*/
	//##ModelId=3C4C419C015D
	void ChangeToRedirectedProxyInfoL(TWspRedirectedAddress& aRedirectAddress);
	
/**Extracts the specified proxy info. Checks the HTTP 
						session properties for proxy info. If the proxy info has
						not been completely specified in the properties then the
						default WAP proxy in the Comms Database is used.
	@since			7.0
	@leave		The function will leave with KErrNotSupported if the
					Connectionless session service has been specified. It will 
					also leave with KWspErrServiceNumberNotDefined if an SMS 
					bearer has been specified and there is no Service Centre
					number defined. The function will leave if the proxy info
					cannot be extracted from the Comms DB.
	@return			A TProxyInfo object that contains the proxy info.
*/
	//##ModelId=3C4C419C013F
	TProxyInfo GetProxyInfoL() const;

private:	// Static methods

/**Extracts the WSP session type from the HTTP session properties set. A default
	value of EWspConnectionOriented is given if the connection type property is 
	not given.
	@since			7.0
	@param			aConnInfo		The session properties for the current HTTP 
									session.
	@param			aStringPool		The current string pool.
	@return			The WSP session type specified in the properties set.
*/
	//##ModelId=3C4C419C00F9
	static TWspSession ExtractSessionTypeFromSession(RHTTPConnectionInfo aConnInfo, RStringPool aStringPool);

/**Extracts the bearer type from the HTTP session properties set. A default 
	value of EIP is given if the bearer property is not present.
	@since			7.0
	@param			aConnInfo		The session properties for the current HTTP 
									session.
	@param			aStringPool		The current string pool.
	@return			The bearer type specified in the properties set.
*/
	//##ModelId=3C4C419C00B3
	static TWspBearer ExtractBearerInfoFromSession(RHTTPConnectionInfo aConnInfo, RStringPool aStringPool);

/**Extracts the security info from the HTTP session properties set. A default 
	value of EFalse is given if the security info property is not present.
	@since			7.0
	@param			aConnInfo		The session properties for the current HTTP 
									session.
	@param			aStringPool		The current string pool.
	@return			A boolean value of ETrue of a security has been specified 
					in the properties set.
*/
	//##ModelId=3C4C419C0077
	static TBool ExtractSecurityInfoFromSession(RHTTPConnectionInfo aConnInfo, RStringPool aStringPool);

/**Extracts the specified port number from the HTTP session properties set. Can
	be either HTTP::ELocalPort or HTTP::ERemotePort. The default value of zero is
	given if the specified port property is not present.
	@since			7.0
	@param			aPortType		The specified port. 
	@param			aConnInfo		The session properties for the current HTTP 
									session.
	@param			aStringPool		The current string pool.
	@return			A 16-bit unsigned integer with the port number extracted 
					from the properties set.
*/
	//##ModelId=3C4C419C0031
	static TUint16 ExtractPortInfoFromSession(TInt aPortType, RHTTPConnectionInfo aConnInfo, RStringPool aStringPool);

/**Gives the default remote port number given the security status and connection
	type, as specified by the WAP specs.
	@since			7.0
	@param			aConnInfo		The session properties for the current HTTP 
									session.
	@param			aStringPool		The current string pool.
	@return			A 16-bit unsigned integer with the default remote port 
					number.
*/
	//##ModelId=3C4C419C0009
	static TUint16 DefaultRemotePort(TBool aSecureConnection, TBool aCOConnection);

/**Reads the connection information from the comms database file. Gives the 
	bearer type, proxy address, security info, service centre number and 
	connection type through the output arguments.
	@since			7.0
	@param			aStringPool				The current string pool.
	@param			aBearer					An output argument for the bearer 
											type.
	@param			aProxyAddress			An output argument for the proxy
											address.
	@param			aSecureConnection		An output argument for the security
											info.
	@param			aServiceCentreNumber	An output argument for the service
											centre number.
	@param			aSessionServiceType		An output argument for the		
											connection type.
*/
	//##ModelId=3C4C419B038D
	static void ReadWapAccessPointInfoL(
									   RStringPool		aStringPool,
									   TWspBearer&		aBearer, 
									   RStringF&		aProxyAddress, 
									   TBool&			aSecureConnection, 
									   RStringF&		aServiceCentreNumber,
									   TWspSession&		aSessionServiceType
									   );

/**Extracts a redirected address data structure from the given data. The 
	redirected address data structure is defined by the WSP Specification, July
	2001. The extracted information is returned in the TWspRedirectedAddress 
	object. If there is more than one address in the data, then only the first
	redirected address is extracted. The remaining data is ignored.
	@since			7.0
	@leave		The function will leave with KWspErrBadRedirectedAddressData
					if the data is not correctly formed. If an unsupported bearer
					has been specified then the function will leave with
					KWspErrBearerNotSupported. The function will also leave if 
					the RStringF for the proxy address fails to open.
	@param			aStringPool				The current string pool.
	@param			aRedirectedAddresses	A reference to the buffer containing 
											the redirected addresses data.
*/
	//##ModelId=3C4C419B036F
	static TWspRedirectedAddress ExtractRedirectedProxyInfoL(RStringPool aStringPool, const TDesC8& aRedirectedAddresses);

/**Extracts the (SMS) service centre number from the comms database.
	@since			7.0
	@param			aStringPool				The current string pool.
	@param			aServiceCentreNumber	An output argument for the service
											centre number.
*/
	//##ModelId=3C4C419B0351
	static void ReadServiceCentreNumberL(RStringPool aStringPool, RStringF& aServiceCentreNumber);

private:	// Attributes

	/** The HTTP sessiom in use. This is not owned.
	*/
	//##ModelId=3C4C419B0346
	RHTTPSession		iSession;

	/** The proxy info.
	*/
	//##ModelId=3C4C419B033C
	TProxyInfo			iProxyInfo;

	/** A flag to indicate that the proxy info has already been updated and 
		should not be changed on the next UpdateProcyInfoL call.
	*/
	//##ModelId=3C4C419B032A
	TBool				iProxyInfoLocked;

	};

#endif	// __CWSPPROXYINFOPROVIDER_H__

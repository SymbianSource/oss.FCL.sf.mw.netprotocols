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
 @file csecuritypolicy.h
 @warning : This file may contain Rose Model ID comments - please do not delete
*/

#ifndef __CSECURITYPOLICY_H__
#define __CSECURITYPOLICY_H__

// System includes
#include <e32base.h>
#include <ecom/ecom.h>
#include <stringpool.h>
#include <ssl.h>
#include <wsp/wsptypes.h>


//##ModelId=3C4C37E401CE
class MSecurityPolicy
/**
The Security Policy Interface.  This interface class must be the base 
class of a security policy plug-in.
@publishedAll
@released
*/
	{
public:

/** Intended Usage:	Ask the client to accept or reject a certificate. Not
					applicable to TLS, as the SSL layer does this itself.
					The call is asynchronous, returning the result via the TRequestStatus.
	@param			aServerCert		(in) A structure containing the certificate details.
	@param			aStatus			(out) A status flag used to indicate errors. A value of 0 is used to indicate
									that the cert is untrusted and 1 that it is trusted
*/
	//##ModelId=3C4C37E4020B
	virtual void ValidateUntrustedServerCert(TCertInfo& aServerCert, TRequestStatus& aStatus) const = 0;

/** Intended Usage:	Cancel a previous request to ask the client to accept or reject
					an untrusted WTLS server certificate.
*/
	//##ModelId=3C4C37E4020A
	virtual void CancelValidateUntrustedServerCert() = 0;


/** Intended Usage:	Obtain an array of WTLS cipher suites.  Not applicable to TLS clients.
	@return			An array of cipher suites, consisting of paired bulk encryption
					algorithms and MAC algorithms.  The array must be ordered with
					strongest cipher suites at the start.
*/
	//##ModelId=3C4C37E40201
	virtual const RArray<TWtlsCipherSuite>& GetWtlsCipherSuites() =0;

/** Intended Usage:	Obtain a list of TLS cipher suites.  Not applicable to WTLS
					clients.
	@return			An descriptor containing a list of the TLS cipher suites. Each cipher suite is a 2 byte pair
*/
	//##ModelId=3C4C37E40200
	virtual const TDesC8& GetTlsCipherSuites() = 0;

/** Intended Usage:	Obtain the WTLS key exchange suites.  Not applicable
					to TLS clients.
	@return			An array of the WTLS key exchange suites.  The array must be ordered
					with authenticating key exchange suites at the start followed by
					anonymous key exchange suites if desired.  Within each group of keys
					longer keys should precede shorter ones.
*/
	//##ModelId=3C4C37E401FA
	virtual const RArray<TWtlsKeyExchangeSuite>& GetWtlsKeyExchangeSuites() = 0;

/** Intended Usage:	Obtain arbitrary, named security policy properties, in a generic
					'stringified' form.
	@param			aPropertyName	(in) The policy property name.
	@param			aPropertyValue	(out) The property value. Parsing of the value is
									left to the caller. The caller must close the
									RString.
	@return			An error code: KErrNone if the property exists or KErrNotFound if
					it doesn't.
*/
	//##ModelId=3C4C37E401F7
	virtual TInt GetNamedPolicyProperty(RStringF aPropertyName, RString& aPropertyValue) = 0;

protected:
/**	Intended Usage:	Reserve a slot in the v-table to preserve future BC
 */
	//##ModelId=3C4C37E401F6
	virtual void Reserved1() =0;

/**	Intended Usage:	Reserve a slot in the v-table to preserve future BC
 */
	//##ModelId=3C4C37E401EC
	virtual void Reserved2() =0;
	};

//##ModelId=3C4C37C50300
class CSecurityPolicy : public CBase, public MSecurityPolicy
/** 
The Security Policy Plug-in API.  This ECOM interface class must be implemented
by each instance of a security policy plug-in.
@publishedAll
@released
*/
	{
public: // methods

/** Intended Usage:	Factory construction - instantiates the plug-in.
	@param			aStringPool		(in) An open string pool reference that can be used by the plugin.
	@return			The constructed plug-in.
*/
	//##ModelId=3C4C37C503C8
	inline static CSecurityPolicy* NewL(RStringPool aStringPool);

/** Intended Usage:	Class destructor.
*/
	//##ModelId=3C4C37C503BE
	inline virtual ~CSecurityPolicy();


protected: // methods

/** Intended Usage:	Normal constructor. Creates an instance of the plug-in base.
	@param			aStrPool		(in) An open string pool reference that can be used by the plugin.
	@return			The constructed plug-in base.
*/
	//##ModelId=3C4C37C503B5
	inline CSecurityPolicy(RStringPool aStrPool);


/**	Intended Usage:	Reserve a slot in the v-table to preserve future BC
 */
	//##ModelId=3C4C37C503B4
	inline virtual void Reserved1();

/**	Intended Usage:	Reserve a slot in the v-table to preserve future BC
 */
	//##ModelId=3C4C37C503AD
	inline virtual void Reserved2();

protected: // attributes

	/** The stringpool reference
	*/
	//##ModelId=3C4C37C503AA
	RStringPool iStrPool;

private: // attributes
	#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
	/** D'tor Key identification required for ECOM
	*/
	#else
	/** D'tor Key identification required for ECOM
		@internalAll
	*/
	#endif	//SYMBIAN_ENABLE_SPLIT_HEADERS

	//##ModelId=3C4C37C50396
	TUid iDtor_ID_Key;
	};

#include <http/framework/csecuritypolicy.inl>

#endif // __CSECURITYPOLICY_H__

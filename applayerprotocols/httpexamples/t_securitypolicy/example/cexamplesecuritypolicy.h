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
 @file cexamplesecuritypolicy.h
*/

#ifndef		__CEXAMPLESECURITYPOLICY_H__
#define		__CEXAMPLESECURITYPOLICY_H__

#include <e32std.h>
#include <http/framework/csecuritypolicy.h>
#include <wsp/wsptypes.h>
#include "cuntrustedcertdialog.h"

/** 
	@class			CExampleSecurityPolicy
	@since			7.0
	An example security policy plug-in. This is intended as a basic example of a security policy plugin. It
	is an Ecom plugin. It is NOT suitable for real use in a platform.
*/

class CExampleSecurityPolicy : public CSecurityPolicy, public MDialogCallback
	{
public:
	/** @fn InstallL
	This is the function called by Ecom to create the security policy plugin
	*/
	static CSecurityPolicy* InstallL(TAny* aStringPool);

	~CExampleSecurityPolicy();

	// @see CSecurityPolicy::ValidateUntrustedServerCert
	void ValidateUntrustedServerCert(TCertInfo& aServerCert, TRequestStatus& aStatus) const;

	// @see CSecurityPolicy::CancelValidateUntrustedServerCert
	void CancelValidateUntrustedServerCert();

	// @see CSecurityPolicy::GetWtlsCipherSuites
	const RArray<TWtlsCipherSuite>& GetWtlsCipherSuites();
	
	// @see CSecurityPolicy::GetTlsCipherSuites
	const TDesC8& GetTlsCipherSuites();

	// @see CSecurityPolicy::GetWtlsKeyExchangeSuites
	const RArray<TWtlsKeyExchangeSuite>& GetWtlsKeyExchangeSuites();

	// @see CSecurityPolicy::GetNamedPolicyProperty
	TInt GetNamedPolicyProperty(RStringF aPropertyName, RString& aPropertyValue);

private:
	CExampleSecurityPolicy(RStringPool aStringpool);
	void ConstructL();

private: // from MDialogCallback
	virtual void DialogDismissed(TBool aChoice);

private:
	TBuf8<128> iTlsCipherSuites;

	RArray<TWtlsCipherSuite> iWtlsCipherSuites;
	RArray<TWtlsKeyExchangeSuite> iWtlsKeyExchangeSuites;

	mutable CUntrustedCertDialog* iUntrustedCertDialog;
	mutable TRequestStatus* iClientStatus;

	RString iKeyRefreshRate;
	};


#endif //	__CEXAMPLESECURITYPOLICY_H__
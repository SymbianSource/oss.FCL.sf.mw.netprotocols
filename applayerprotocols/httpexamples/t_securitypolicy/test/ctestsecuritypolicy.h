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
 @file ctestsecuritypolicy.h
*/

#ifndef		__CTESTSECURITYPOLICY_H__
#define		__CTESTSECURITYPOLICY_H__

#include <e32std.h>
#include <http/framework/csecuritypolicy.h>
#include <wsp/wsptypes.h>
#include <stringpool.h>
#include "TestScripts.h"

/** 
	@class			CTestSecurityPolicy
	@since			7.0
	An test security policy plug-in. This is intended for testing purposes only. It
	is an ecom plugin. It is NOT suitable for real use in a platform.It initalizes values from a t_securitypolicy.ini
	file in c\httptest\
*/
//##ModelId=3C4C4C3F0054
class CTestSecurityPolicy : public CSecurityPolicy
	{
public:
	//##ModelId=3C4C4C3F016E
	static CSecurityPolicy* InstallL(TAny* aStringPool);

	//##ModelId=3C4C4C3F016D
	~CTestSecurityPolicy();

	// from CSecurityPolicy
	// @see CSecurityPolicy::ValidateUntrustedServerCert
	//##ModelId=3C4C4C3F015B
	void ValidateUntrustedServerCert(TCertInfo& aServerCert,
									 TRequestStatus& aStatus) const;

	// @see CSecurityPolicy::CancelValidateUntrustedServerCert
	//##ModelId=3C4C4C3F015A
	void CancelValidateUntrustedServerCert();

	// @see CSecurityPolicy::GetWtlsCipherSuites
	//##ModelId=3C4C4C3F0159
	const RArray<TWtlsCipherSuite>& GetWtlsCipherSuites();
	
	// @see CSecurityPolicy::GetTlsCipherSuites
	//##ModelId=3C4C4C3F0151
	const TDesC8& GetTlsCipherSuites();

	// @see CSecurityPolicy::GetWtlsKeyExchangeSuites
	//##ModelId=3C4C4C3F0150
	const RArray<TWtlsKeyExchangeSuite>& GetWtlsKeyExchangeSuites();

	// @see CSecurityPolicy::GetNamedPolicyProperty
	//##ModelId=3C4C4C3F0145
	TInt GetNamedPolicyProperty(RStringF aPropertyName, RString& aPropertyValue);

private:
	//##ModelId=3C4C4C3F013C
	CTestSecurityPolicy(RStringPool aStringpool);
	//##ModelId=3C4C4C3F013B
	void ConstructL();
	//##ModelId=3C4C4C3F0132
	void ReadIniFileL();
	//##ModelId=3C4C4C3F0128
	void ReadSectionL(CScriptSection& aScriptSection);
	//##ModelId=3C4C4C3F011F
	void ReadSingleItemL(CScriptSectionItem& aScriptItem);
	//##ModelId=3C4C4C3F011D
	void ParseWtlsCipherSuiteListL(const TDesC& aCipherSuiteList);
	//##ModelId=3C4C4C3F0113
	void ParseWtlsKeyExchangeSuiteListL(const TDesC& aKeyExchangeSuiteList);
	//##ModelId=3C4C4C3F0100
	void ParseTlsCipherSuiteListL(const TDesC& aCipherSuiteList, RArray<TInt>& aCipherSuites);
	//##ModelId=3C4C4C3F00F5
	void ParseNamedPolicyL(const TDesC& aNameValuePolicyPair);

private:
	//##ModelId=3C4C4C3F00EE
	TBool iAutoValidateServerCert;
	//##ModelId=3C4C4C3F00E3
	RArray<TWtlsCipherSuite> iWtlsCipherSuites;
	//##ModelId=3C4C4C3F00C4
	RArray<TWtlsKeyExchangeSuite> iWtlsKeyExchangeSuites;

	//##ModelId=3C4C4C3F00A6
	TBuf8<128> iTlsCipherSuites;

	//##ModelId=3C4C4C3F0088
	RArray<RStringF> iPolicyNames;
	//##ModelId=3C4C4C3F006A
	RArray<RString> iPolicyValues;
	};


#endif //	__CTESTSECURITYPOLICY_H__

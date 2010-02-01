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


#include <e32std.h>
#include "cexamplesecuritypolicy.h"
#include "securesocket.h"
#include "securitypolicypluginstrings.h"


CSecurityPolicy* CExampleSecurityPolicy::InstallL(TAny* aStringPool)
	{
	RStringPool* stringPool = REINTERPRET_CAST(RStringPool*, aStringPool);
	CExampleSecurityPolicy* self = new (ELeave) CExampleSecurityPolicy(*stringPool);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

void CExampleSecurityPolicy::ConstructL()
	{
	iUntrustedCertDialog = CUntrustedCertDialog::NewL(*this);

	// Populate the WTLS cipher suites
	User::LeaveIfError( iWtlsCipherSuites.Append(TWtlsCipherSuite(
													TWtlsCipherSuite::ERC5_CBC,
													TWtlsCipherSuite::ESHA)) );
	User::LeaveIfError( iWtlsCipherSuites.Append(TWtlsCipherSuite(
													TWtlsCipherSuite::ERC5_CBC,
													TWtlsCipherSuite::ESHA_80)) );
	User::LeaveIfError( iWtlsCipherSuites.Append(TWtlsCipherSuite(
													TWtlsCipherSuite::ERC5_CBC,
													TWtlsCipherSuite::ESHA_40)) );
	User::LeaveIfError( iWtlsCipherSuites.Append(TWtlsCipherSuite(
													TWtlsCipherSuite::ERC5_CBC_56,
													TWtlsCipherSuite::ESHA)) );
	User::LeaveIfError( iWtlsCipherSuites.Append(TWtlsCipherSuite(
													TWtlsCipherSuite::ERC5_CBC_56,
													TWtlsCipherSuite::ESHA_80)) );
	User::LeaveIfError( iWtlsCipherSuites.Append(TWtlsCipherSuite(
													TWtlsCipherSuite::ERC5_CBC_40,
													TWtlsCipherSuite::ESHA)) );
	User::LeaveIfError( iWtlsCipherSuites.Append(TWtlsCipherSuite(
													TWtlsCipherSuite::ERC5_CBC_40,
													TWtlsCipherSuite::ESHA_80)) );

	// Populate the WTLS key exchange suites.  In order to avoid blowing out the
	// stack, we must re-use the same TWtlsKeyExchangeSuite for each one, since it
	// contains a 512-byte buffer.
	TWtlsKeyExchangeSuite keyExSuite(
									TWtlsKeyExchangeSuite::ERSA,
									TWtlsKeyExchangeSuite::EIdNull,
									KNullDesC8()
									);
	User::LeaveIfError( iWtlsKeyExchangeSuites.Append(keyExSuite) );
	keyExSuite.iKeyExchangeSuite = TWtlsKeyExchangeSuite::ERSA_768;
	keyExSuite.iKeyIdType			 = TWtlsKeyExchangeSuite::EIdNull;
	User::LeaveIfError( iWtlsKeyExchangeSuites.Append(keyExSuite) );
	keyExSuite.iKeyExchangeSuite = TWtlsKeyExchangeSuite::ERSA_512;
	keyExSuite.iKeyIdType			 = TWtlsKeyExchangeSuite::EIdNull;
	User::LeaveIfError( iWtlsKeyExchangeSuites.Append(keyExSuite) );
	keyExSuite.iKeyExchangeSuite = TWtlsKeyExchangeSuite::ERSA_anon;
	keyExSuite.iKeyIdType			 = TWtlsKeyExchangeSuite::EIdNull;
	User::LeaveIfError( iWtlsKeyExchangeSuites.Append(keyExSuite) );
	keyExSuite.iKeyExchangeSuite = TWtlsKeyExchangeSuite::ERSA_anon_768;
	keyExSuite.iKeyIdType			 = TWtlsKeyExchangeSuite::EIdNull;
	User::LeaveIfError( iWtlsKeyExchangeSuites.Append(keyExSuite) );
	keyExSuite.iKeyExchangeSuite = TWtlsKeyExchangeSuite::ERSA_anon_512;
	keyExSuite.iKeyIdType			 = TWtlsKeyExchangeSuite::EIdNull;
	User::LeaveIfError( iWtlsKeyExchangeSuites.Append(keyExSuite) );

	// Create some named policies
	_LIT8(KKeyRefreshRate, "3");
	iKeyRefreshRate = iStrPool.OpenStringL(KKeyRefreshRate());
	}

CExampleSecurityPolicy::CExampleSecurityPolicy(RStringPool aStringPool) :
	CSecurityPolicy(aStringPool)
	{
	}

CExampleSecurityPolicy::~CExampleSecurityPolicy()
	{
	iWtlsCipherSuites.Close();
	iWtlsKeyExchangeSuites.Close();
	delete iUntrustedCertDialog;
	iKeyRefreshRate.Close();
	}

void CExampleSecurityPolicy::ValidateUntrustedServerCert(TCertInfo& /*aServerCert*/,
														 TRequestStatus& aStatus) const
	{
	iClientStatus = &aStatus;
	aStatus = KRequestPending;
	iUntrustedCertDialog->DisplayUntrustedCertDialog();
	}

void CExampleSecurityPolicy::DialogDismissed(TBool aChoice)
	{
	User::RequestComplete(iClientStatus, aChoice);
	iClientStatus = NULL;
	}

void CExampleSecurityPolicy::CancelValidateUntrustedServerCert()
	{
	iUntrustedCertDialog->Cancel();
	}


const RArray<TWtlsCipherSuite>& CExampleSecurityPolicy::GetWtlsCipherSuites()
	{
	return iWtlsCipherSuites;
	}

	
const RArray<TWtlsKeyExchangeSuite>& CExampleSecurityPolicy::GetWtlsKeyExchangeSuites()
	{
	return iWtlsKeyExchangeSuites;
	}


const TDesC8& CExampleSecurityPolicy::GetTlsCipherSuites()
	{
	// This is an example, since this is actually just giving back the default set of ciphers
	// that are available. You don't actually need to set these when using a CSecureSocket. 

	// It is advisable that unless you wish to limit the cipher suites to not include weak cipher suites that this 
	// function returns an empty descriptor.
	if (iTlsCipherSuites.Length() == 0)
		{
		RSocket socket;
		_LIT(KTls1_0, "tls1.0");
		CSecureSocket* secureSocket = NULL;
		TRAPD(err, secureSocket = CSecureSocket::NewL(socket, KTls1_0));
		if (err == KErrNone)
			secureSocket->AvailableCipherSuites(iTlsCipherSuites);	// ignore any errors and just pass back an empty descriptor
																	// to indicate use available ciphers 
		delete secureSocket;
		}

	return iTlsCipherSuites;
	}


TInt CExampleSecurityPolicy::GetNamedPolicyProperty(RStringF aPropertyName, RString& aPropertyValue)
	{
	// Work around 'feature' of string table, in which RStringF values opened from a case-sensitive
	// string table don't have index values that may be compared with their RString counterparts
	RString propName;
	TRAPD(err, propName = iStrPool.OpenStringL(aPropertyName.DesC()));
	if (err)
		return KErrNotFound;

	TInt retVal;
	switch (propName.Index(SecurityPolicyProperties::Table))
		{
	case SecurityPolicyProperties::ESequenceNumberMode:
		{
		aPropertyValue = iStrPool.String(SecurityPolicyProperties::EExplicit,
										 SecurityPolicyProperties::Table);
		retVal = KErrNone;
		} break;
	case SecurityPolicyProperties::EKeyRefreshRate:
		{
		aPropertyValue = iKeyRefreshRate.Copy();
		retVal = KErrNone;
		} break;
	case SecurityPolicyProperties::ESharedSecret:
		{
		retVal = KErrNotSupported;
		} break;
	case SecurityPolicyProperties::ERecordLengthUsage:
		{
		aPropertyValue = iStrPool.String(SecurityPolicyProperties::EUsed,
										 SecurityPolicyProperties::Table);
		retVal = KErrNone;
		} break;
	case SecurityPolicyProperties::EAnonymousAllowed:
		{
		aPropertyValue = iStrPool.String(SecurityPolicyProperties::EAllowed,
										 SecurityPolicyProperties::Table);
		retVal = KErrNone;
		} break;
	default:
		{
		retVal = KErrNotFound;
		} break;
		}
	propName.Close();
	return retVal;
	}




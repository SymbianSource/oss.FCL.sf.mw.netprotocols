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
#include "ctestsecuritypolicy.h"
#include "securesocket.h"


#include "httptestutils.h"
#include "HttpTestCore.h"
#include "TestScripts.h"
#include <inetprottextutils.h>

// Location of INI file 
_LIT(KSecurityPolicy, "securitypolicy");
_LIT(KSecurityPolicyFileName, "securitypolicy.ini");


_LIT(KValidateUntrustedServerCert, "ValidateUntrustedServerCert");
_LIT(KWTLSCipherSuites, "WTLSCipherSuites");
_LIT(KWTLSKeyExchangeSuites, "WTLSKeyExchangeSuites");
_LIT(KTLSCipherSuites, "TLSCipherSuites");
_LIT(KPolicy, "Policy");
_LIT(KReject, "Reject");
_LIT(KAccept, "Accept");

CSecurityPolicy* CTestSecurityPolicy::InstallL(TAny* aStringPool)
	{
	RStringPool* stringPool = REINTERPRET_CAST(RStringPool*, aStringPool);
	CTestSecurityPolicy* self = new (ELeave) CTestSecurityPolicy(*stringPool);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

void CTestSecurityPolicy::ConstructL()
	{
	ReadIniFileL();

	// If WTLS settings not read from .ini file then leave WTLS cipher suite/key exchange suite
	// arrays empty, as the WAP stack will provide its own defaults.
	}


void CTestSecurityPolicy::ReadIniFileL()
	{
	CHttpTestEngine* engine= CHttpTestEngine::NewL(KSecurityPolicy(), EFalse);
	CleanupStack::PushL(engine);
	CScriptFile* iniFile = CScriptFile::NewL(engine->Utils(), KSecurityPolicy, KSecurityPolicyFileName());
	CleanupStack::PushL(iniFile);
	iniFile->ReadScriptL(KSecurityPolicyFileName);

	CArrayPtrFlat<CScriptSection>& sections=iniFile->Sections();
	const TInt secCount = sections.Count();
	if (secCount != 1)
		User::Leave(KErrCorrupt);
	
	ReadSectionL(*sections[0]);

	CleanupStack::PopAndDestroy(2,engine); // iniFile, engine
	}

void CTestSecurityPolicy::ReadSectionL(CScriptSection& aScriptSection)
	{
	CArrayPtrFlat<CScriptSectionItem>& items = aScriptSection.Items();
	const TInt numItems = items.Count();
	for (TInt ii=0; ii<numItems ; ++ii)
		{
		ReadSingleItemL(*items[ii]);
		}
	}

void CTestSecurityPolicy::ReadSingleItemL(CScriptSectionItem& aScriptItem)
	{
	TPtrC name(aScriptItem.Item());
	TPtrC value(aScriptItem.Value());
	if (name.CompareF(KValidateUntrustedServerCert) == 0)
		{
		if (value.CompareF(KReject) == 0)
			iAutoValidateServerCert = EFalse;
		else if (value.CompareF(KAccept) == 0)
			iAutoValidateServerCert = ETrue;
		else
			User::Leave(KErrCorrupt);
		}

	else if (name.CompareF(KWTLSCipherSuites) == 0)
		{
		ParseWtlsCipherSuiteListL(value);
		}
	
	else if (name.CompareF(KWTLSKeyExchangeSuites) == 0)
		{
		ParseWtlsKeyExchangeSuiteListL(value);
		}

	else if (name.CompareF(KTLSCipherSuites) == 0)
		{
		RArray<TInt> tlsCipherSuites;
		CleanupClosePushL(tlsCipherSuites);
		ParseTlsCipherSuiteListL(value, tlsCipherSuites);

		const TInt numCipherSuites = tlsCipherSuites.Count();
		for (TInt ii=0; ii<numCipherSuites && ii < 128; ++ii)
			{
			TInt cipherSuiteNumber = tlsCipherSuites[ii];
			if (cipherSuiteNumber < 127)
				iTlsCipherSuites.Append(TChar(cipherSuiteNumber));
			}

		CleanupStack::PopAndDestroy(&tlsCipherSuites);
		}
	else if (name.Find(KPolicy) == 0) // matches 'Policy1', 'Policy2', etc.
		ParseNamedPolicyL(value);
	}


void CTestSecurityPolicy::ParseWtlsCipherSuiteListL(const TDesC& aCipherSuiteList)
	{
	TPtrC cipherList(aCipherSuiteList);
	TInt commaLoc = KErrNotFound;
	do
		{
		TPtrC suite(KNullDesC);
		commaLoc = cipherList.Locate(',');
		if (commaLoc <= 0)
			{
			if (cipherList.Length() > 0)
				suite.Set(cipherList);
			}
		else
			{
			suite.Set(cipherList.Left(commaLoc));
			cipherList.Set(cipherList.Right(cipherList.Length() - (commaLoc +1)));
			}

		if (suite.Length() > 0)
			{
			InetProtTextUtils::RemoveWhiteSpace(suite, InetProtTextUtils::ERemoveBoth);
	
			// suite will now contain '{0;0}' where the first digit is an EWtlsBulkEncryptionAlgorithm enumeration index,
			// and the second digit is an EWtlsMacAlgorithm index.  Validate this format as follows:
			const TInt suiteLen = suite.Length();
			TInt semiColonLoc = suite.Locate(';');
			if ( (semiColonLoc == KErrNotFound) ||	// corrupt if no ; found
				 (suiteLen < 5) ||					// corrupt if the length is less than the minimum for two 1 digit no's
				 (suite[0] != '{') ||				// corrupt if the suite doesn't start with {
				 (suite[suiteLen - 1] != '}') ||	// corrupt if the suite doesn't end with }
				 (semiColonLoc == 1) ||				// corrupt if the { and ; are adjacent (no first digit present)
				 (semiColonLoc == suiteLen - 2) )	// corrupt if the ; and } are adjacent (no second digit present)
				User::Leave(KErrCorrupt);

			// Now we can get substrings containing the two integers
			TPtrC bulkAlgEnum = suite.Mid(1, semiColonLoc - 1);
			InetProtTextUtils::RemoveWhiteSpace(bulkAlgEnum, InetProtTextUtils::ERemoveBoth);
			TPtrC macAlgEnum  = suite.Mid(semiColonLoc + 1, suiteLen - semiColonLoc - 2);
			InetProtTextUtils::RemoveWhiteSpace(macAlgEnum, InetProtTextUtils::ERemoveBoth);
			TInt bulkAlg = 0;
			TInt macAlg = 0;
			User::LeaveIfError(InetProtTextUtils::ConvertDescriptorToInt(bulkAlgEnum,bulkAlg));
			User::LeaveIfError(InetProtTextUtils::ConvertDescriptorToInt(macAlgEnum,macAlg));
			
			// Finally, cast these integers to the actual enumerated types and append a pair to the list.
			if ( (bulkAlg >= TWtlsCipherSuite::ENULL) &&
				 (bulkAlg <= TWtlsCipherSuite::EIDEA_CBC_64) &&
				 (macAlg  >= TWtlsCipherSuite::ESHA_0) &&
				 (macAlg  <= TWtlsCipherSuite::EMD5) )
				{
				TWtlsCipherSuite wtlsCipherSuite(STATIC_CAST(TWtlsCipherSuite::TBulkEncryptionAlgorithm, bulkAlg),
												 STATIC_CAST(TWtlsCipherSuite::TMacAlgorithm, macAlg));
				User::LeaveIfError( iWtlsCipherSuites.Append(wtlsCipherSuite) );
				}
			else 
				User::Leave(KErrCorrupt);
			}
		}
	while (commaLoc != KErrNotFound);
	}


void CTestSecurityPolicy::ParseWtlsKeyExchangeSuiteListL(const TDesC& aKeyExchangeSuiteList)
	{
	TPtrC keyExchList(aKeyExchangeSuiteList);
	TInt commaLoc = KErrNotFound;
	do
		{
		TPtrC suite(KNullDesC);
		commaLoc = keyExchList.Locate(',');
		if (commaLoc <= 0)
			{
			if (keyExchList.Length() > 0)
				suite.Set(keyExchList);
			}
		else
			{
			suite.Set(keyExchList.Left(commaLoc));
			keyExchList.Set(keyExchList.Right(keyExchList.Length() - (commaLoc +1)));
			}

		if (suite.Length() > 0)
			{
			InetProtTextUtils::RemoveWhiteSpace(suite, InetProtTextUtils::ERemoveBoth);
	
			// 'suite' will now contain '{0;0;""}' where the first digit is an
			// TKeyExchangeSuite enumeration index, the second digit is an TIdType index
			// and the string is a key ID.  Validate this format as follows:
			const TInt suiteLen = suite.Length();
			TInt semiColonLoc1 = suite.Locate(';');
			TInt semiColonLoc2 = KErrNotFound;
			if (semiColonLoc1 < suiteLen - 1)
				semiColonLoc2 = suite.Mid(semiColonLoc1 + 1).Locate(';');
			if (semiColonLoc2 != KErrNotFound)
				semiColonLoc2 += (semiColonLoc1 + 1);
			TInt quoteLoc1 = suite.Locate('\"');
			TInt quoteLoc2 = KErrNotFound;
			if (quoteLoc1 < suiteLen - 1)
				quoteLoc2 = suite.Mid(quoteLoc1 + 1).Locate('\"');
			if (quoteLoc2 != KErrNotFound)
				quoteLoc2 += (quoteLoc1 + 1);
			if ( (semiColonLoc1 == KErrNotFound) ||		// corrupt if no first ; found
				 (semiColonLoc2 == KErrNotFound) ||		// corrupt if no second ; found
				 (suiteLen < 8) ||						// corrupt if the length is less than the minimum for two 1 digit no's and an empty string
				 (suite[0] != '{') ||					// corrupt if the suite doesn't start with {
				 (suite[suiteLen - 1] != '}') ||		// corrupt if the suite doesn't end with }
				 (semiColonLoc1 == 1) ||				// corrupt if the { and ; are adjacent (no first digit present)
				 (semiColonLoc1 == semiColonLoc2 - 1) ||// corrupt if the first and second ;s are adjacent (no second digit present)
				 (quoteLoc1 == KErrNotFound) ||			// corrupt if no first " found
				 (quoteLoc1 < semiColonLoc2) ||			// corrupt if the first " precedes the second ;
				 (quoteLoc2 == KErrNotFound) )			// corrupt if no second " found
				User::Leave(KErrCorrupt);

			// Now we can get substrings containing the two integers and the string
			TPtrC keyExchEnum = suite.Mid(1, semiColonLoc1 - 1);
			TPtrC idTypeEnum  = suite.Mid(semiColonLoc1 + 1, semiColonLoc2 - semiColonLoc1 - 1);
			TPtrC keyIdString = suite.Mid(quoteLoc1 + 1, quoteLoc2 - quoteLoc1 - 1);
			if (keyIdString.Length() > KWtlsMaxKeyIdLength)
				User::Leave(KErrCorrupt);
			TInt keyExch = 0;
			TInt idType = 0;
			User::LeaveIfError(InetProtTextUtils::ConvertDescriptorToInt(keyExchEnum,keyExch));
			User::LeaveIfError(InetProtTextUtils::ConvertDescriptorToInt(idTypeEnum,idType));
			
			// Finally, cast these integers to the actual enumerated types and append a pair to the list.
			if ( (keyExch >= TWtlsKeyExchangeSuite::ENULL) &&
				 (keyExch <= TWtlsKeyExchangeSuite::EECDH_ECDSA_uncomp) &&
				 (idType  >= TWtlsKeyExchangeSuite::EIdNull) &&
				 (idType  <= TWtlsKeyExchangeSuite::EX509Name) )
				{
				TBuf8<KWtlsMaxKeyIdLength> keyId;
				keyId.Copy(keyIdString);
				TWtlsKeyExchangeSuite wtlskeyExchSuite(STATIC_CAST(TWtlsKeyExchangeSuite::TKeyExchangeSuite, keyExch),
													   STATIC_CAST(TWtlsKeyExchangeSuite::TKeyIdType, idType),
													   keyId);
				User::LeaveIfError( iWtlsKeyExchangeSuites.Append(wtlskeyExchSuite) );
				}
			else
				User::Leave(KErrCorrupt);
			}
		}
	while (commaLoc != KErrNotFound);
	}


void CTestSecurityPolicy::ParseTlsCipherSuiteListL(const TDesC& aCipherSuiteList, RArray<TInt>& aCipherSuites)
	{
	TPtrC cipherList(aCipherSuiteList);
	TInt commaLoc = KErrNotFound;
	do
		{
		TPtrC number(KNullDesC);
		commaLoc = cipherList.Locate(',');
		if (commaLoc <= 0)
			{
			if (cipherList.Length() > 0)
				number.Set(cipherList);
			else
				return;
			}
		else
			{
			number.Set(cipherList.Left(commaLoc));
			cipherList.Set(cipherList.Right(cipherList.Length() - (commaLoc +1)));
			}

		if (number.Length() > 0)
			{
			InetProtTextUtils::RemoveWhiteSpace(number, InetProtTextUtils::ERemoveBoth);
			TInt cipherSuite;
			User::LeaveIfError(InetProtTextUtils::ConvertDescriptorToInt(number,cipherSuite));
			User::LeaveIfError(aCipherSuites.Append(cipherSuite));
			}
		else
			User::Leave(KErrCorrupt);
		}
	while (commaLoc != KErrNotFound);
	}


void CTestSecurityPolicy::ParseNamedPolicyL(const TDesC& aNameValuePolicyPair)
	{
	TInt equalsLoc = aNameValuePolicyPair.Locate('=');
	User::LeaveIfError(equalsLoc);

	TPtrC name(aNameValuePolicyPair.Left(equalsLoc));
	InetProtTextUtils::RemoveWhiteSpace(name, InetProtTextUtils::ERemoveBoth);

	HBufC8* name8 = HBufC8::NewLC(name.Length());
	TPtr8  name8Ptr= name8->Des();
	name8Ptr.Copy(name);

	TPtrC value(KNullDesC);
	if (equalsLoc < aNameValuePolicyPair.Length())
		{
		value.Set(aNameValuePolicyPair.Right(aNameValuePolicyPair.Length() - (equalsLoc+1)));
		InetProtTextUtils::RemoveWhiteSpace(value, InetProtTextUtils::ERemoveBoth);
		}

	HBufC8* value8 = HBufC8::NewLC(value.Length());
	TPtr8  value8Ptr= value8->Des();
	value8Ptr.Copy(value);

	RStringF nameStr=iStrPool.OpenFStringL(*name8);
	CleanupClosePushL(nameStr);
	User::LeaveIfError(iPolicyNames.Append(nameStr));
	CleanupStack::Pop(); //nameStr
	RString valueStr=iStrPool.OpenStringL(*value8);
	CleanupClosePushL(valueStr);

	User::LeaveIfError(iPolicyValues.Append(valueStr));
	CleanupStack::Pop(); //valueStr
	CleanupStack::PopAndDestroy(2); //value8, name8
	}

CTestSecurityPolicy::CTestSecurityPolicy(RStringPool aStringPool) :
	CSecurityPolicy(aStringPool)
	{
	}

CTestSecurityPolicy::~CTestSecurityPolicy()
	{
	iWtlsCipherSuites.Close();
	iWtlsKeyExchangeSuites.Close();

	const TInt numPolicyNames = iPolicyNames.Count();
	const TInt numPolicyValues = iPolicyValues.Count();
	for (TInt ii = 0; ii < numPolicyNames; ++ii)
		{
		iPolicyNames[ii].Close();
		}
	for (TInt ii = 0; ii < numPolicyValues; ++ii)
		{
		iPolicyValues[ii].Close();
		}
	iPolicyNames.Close();
	iPolicyValues.Close();
	}

void CTestSecurityPolicy::ValidateUntrustedServerCert(TCertInfo& /*aServerCert*/,
													  TRequestStatus& aStatus) const
	{
	TRequestStatus* stat = &aStatus;
	User::RequestComplete(stat, iAutoValidateServerCert);
	}


void CTestSecurityPolicy::CancelValidateUntrustedServerCert()
	{
	}


const RArray<TWtlsCipherSuite>& CTestSecurityPolicy::GetWtlsCipherSuites()
	{
	return iWtlsCipherSuites;
	}

	
const TDesC8& CTestSecurityPolicy::GetTlsCipherSuites()
	{
	// just return an empty string if its not set in the ini file
	return iTlsCipherSuites;
	}


const RArray<TWtlsKeyExchangeSuite>& CTestSecurityPolicy::GetWtlsKeyExchangeSuites()
	{
	return iWtlsKeyExchangeSuites;
	}

TInt CTestSecurityPolicy::GetNamedPolicyProperty(RStringF aPropertyName, RString& aPropertyValue)
	{
	const TInt numPolicies = iPolicyNames.Count();
	for (TInt ii = 0; ii < numPolicies; ++ii)
		{
		const TDesC8& myName = iPolicyNames[ii].DesC();
		const TDesC8& theirName = aPropertyName.DesC();
		if (myName.Compare(theirName) == 0)
			{
			aPropertyValue = iPolicyValues[ii].Copy();
			return KErrNone;
			}
		}	
	return KErrNotFound;
	}




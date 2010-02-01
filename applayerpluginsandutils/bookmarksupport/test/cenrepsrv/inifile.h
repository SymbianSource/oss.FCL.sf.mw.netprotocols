// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __INIFILE_H__
#define __INIFILE_H__

#include <s32file.h>
#include <centralrepository.h>
#include "srvres.h"


using namespace NCentralRepositoryConstants;
using namespace NCentralRepositoryResources;

//Forward declarations
class RRangePolicyArray;
class RDefaultMetaArray;
class TServerSetting;
class TSettingsAccessPolicy;
class CSharedRepository;

//Max length of one setting statement
const TInt KMaxLengthWithPolicyInfo = KMaxBinaryLength * 2 + 64;

//
// CIniFileIn class

class CIniFileIn : public CBase
	{
	enum {KReadPolicy,KWritePolicy};

public:
	enum TIniFileOpenMode {ERomOnly,EInstallOnly,EAuto};

public:
	static TInt NewLC(CIniFileIn*& aIniFile,HBufC* aFullFileName,TCentRepLocation aLocation);
	~CIniFileIn();

	TInt ReadSettingL(TServerSetting& aSetting,TSecurityPolicy& aSingleReadPolicy,TSecurityPolicy& aSingleWritePolicy, TBool& aSingleReadPolicyFound, TBool& aSingleWritePolicyFound, TBool& aSingleMetaFound);

	TInt ReadPlatSecSectionL(TSecurityPolicy& aDefaultReadPolicy, TBool& aGotDefaultReadPolicy,
							TSecurityPolicy& aDefaultWritePolicy, TBool& aGotDefaultWritePolicy,
							RRangePolicyArray& aRangePolicies);
	TInt ReadRdPolicyL(TSecurityPolicy& aReadPolicy);
	TInt ReadWrPolicyL(TSecurityPolicy& aWritePolicy);
	TInt ReadOwnerSectionL(TUint32 &aOwnerUID) ;
	TInt ReadTimeStampSectionL(TTime &aTimeStamp) ;
	TInt ReadDefaultMetaSecSectionL(TUint32& aDefaultMeta,
	                                RDefaultMetaArray& aDefaultMetaRanges) ;
	//
	TInt SkipPlatSecSectionL();
	TInt SkipOwnerSectionL() ;
	TInt SkipDefaultMetaSectionL() ;
	TInt SkipTimeStampSectionL() ;

	TInt FindMainSectionL(void) ;
	
	TInt GetAccessPolicyTextL(TUint aKey,HBufC*& aText);
	HBufC* FullName();
	
private:
	TInt ReadHeaderL();
	TInt ReadSettingOnlyL(TServerSetting& aSetting, TBool& aSingleMetaFound);
	TInt ReadStringL(HBufC8*& aString);
	TInt ReadString16To8L(HBufC8*& aString);
	TInt ReadBinaryL(HBufC8*& aString);
	void SkipComments();
	void SkipEqualSign();
	//
	TInt ReadNumber(TUint32& aVal);
	TInt ReadCapabilityL(TCapability& aCapability);
	void CheckForAlwaysPassOrFailL(TBool& aAlwaysPass,TBool& aAlwaysFail);
	TInt ReadPolicyL(TSecurityPolicy& aPolicy,TInt aPolicyType);
	TInt ReadCapabilitiesL(TSecurityPolicy& aPolicy);
	TInt ReadSidAndCapabilitiesL(TSecurityPolicy& aPolicy,const TDesC& aPolicyType,
								TSecureId& aSid);
	TInt ReadRangePoliciesL(const TSecurityPolicy& aDefaultReadPolicy,const TSecurityPolicy& aDefaultWritePolicy,
				RRangePolicyArray& aRangePolicies);
	TInt ReadRangeMetaDefaultsL(RDefaultMetaArray& aDefaultMetaRanges);


	//
	TInt GetPlatSecSectionLC(HBufC*& aSection);
	TInt GetTimeStampSectionLC(HBufC*& aSection);
	TInt GetOwnerSectionLC(HBufC*& aSection) ;
	TInt GetDefaultMetaSectionLC(HBufC*& aSection) ;
	TInt GetSectionLC(const TDesC16& aSectionId, HBufC*& aSection) ;

private:
	HBufC* iBuf;
	HBufC* iFullName;
	TLex iLex;
	TLexMark iMainSectionMark;
	//
	};

//
// CIniFileOut class
enum TCapAccessMode
	{
	ECapReadAccess, 
	ECapWriteAccess
	};
		
class CIniFileOut : public CBase
	{
public:
	static CIniFileOut* NewLC();
	~CIniFileOut();
    void CommitL(const TDesC& aOutFileName);

	void WriteHeaderL();
	void WriteOwnerSectionL(TUid aOwner);
	void WriteTimeStampL(const TTime& aTime);
	void WriteMetaDataL(TUint32 aDefaultMeta,
	                    const RDefaultMetaArray& aDefaultMetaRanges);
	void WritePlatSecL(const TSecurityPolicy& aDefaultReadPolicy, 
					   const TSecurityPolicy& aDefaultWritePolicy,
					   const RRangePolicyArray& aRangePolicies);
	void WriteMainSectionHeaderL();
	void WriteSettingL(const TServerSetting& aSetting);
	void WriteSettingL(const TServerSetting& aSetting,
					   const TSettingsAccessPolicy& aAccessPolicy);

private:
	CIniFileOut();
	void ConstructL();
	void WriteLineL(const TDesC& aData);
	void AppendSecurityPolicyL(const TSecurityPolicy& aPolicy,
							   TCapAccessMode aRdWrMode);
	void DoSettingL(const TServerSetting& aSetting);

private:
	TBool		iCommited;
	RFile 		iTransFile;			//Transaction file
	RFileBuf	iTransFileBuf;		//Transaction file buffer
	TParse		iTransFilePath;		//Transaction file name.
	TBuf<KMaxLengthWithPolicyInfo> iBuf;//General purpose buffer, used internally
	};

#endif // __INIFILE_H__

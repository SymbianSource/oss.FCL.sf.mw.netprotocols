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

#define __INCLUDE_CAPABILITY_NAMES__
#include <utf.h>
#include "inifile.h"
#include "srvres.h"
#include "shrepos.h"

#define MAX(a,b)        ((a)<(b)?(b):(a))
#define MAX3(a,b,c)     MAX(MAX(a,b),c)
#define MAX4(a,b,c,d)   MAX(MAX(a,b),MAX(c,d))

_LIT(KTransactFileName, "transact");

//Unicode text file prefix - FE,FF bytes.
static const TUint16 KUcs2Bom = 0xfeff;

//Repository (ini) file - signature
_LIT(KSignature, "cenrep");
static const TInt KSignatureLen = 6;

//Repository (ini) file  - version string and version number
_LIT(KVersion, "version");
static const TInt KVersionLen = 7;
static const TUint KCurrentVersion = 1;

//Repository (ini) file - supported types names
_LIT(KTypeInt, "int");
_LIT(KTypeReal, "real");
_LIT(KTypeString, "string");
_LIT(KTypeString8, "string8");
_LIT(KTypeBinary, "binary");

//Max type name length
static const TInt KMaxTypeLen = 9;

//The symbol used in repository (ini) files to note null data
static const TChar KNullDataIndicator = '-';

// Section identifiers in the repository (ini) file
_LIT(KPlatSecSection, "[platsec]");
static const TInt KPlatSecSectionLen = 9;

_LIT(KOwnerSection, "[owner]");
static const TInt KOwnerSectionLen = 7;

_LIT(KTimeStampSection, "[timestamp]");
static const TInt KTimeStampSectionLen = 11;

_LIT(KMainSection, "[main]");
static const TInt KMainSectionLen = 6;

_LIT(KDefaultMetaSection, "[defaultmeta]");
static const TInt KDefaultMetaSectionLen = 13 ;

static const TInt KIniFileSectionLen = MAX4(KPlatSecSectionLen,KMainSectionLen,KTimeStampSectionLen, KDefaultMetaSectionLen);

// Other useful string constants
_LIT(KMaskString, "mask");
static const TInt KMaskLen = 4;

_LIT(KReadAccessSidString, "sid_rd");
_LIT(KReadAccessCapString, "cap_rd");
_LIT(KWriteAccessSidString, "sid_wr");
_LIT(KWriteAccessCapString, "cap_wr");
_LIT(KAccessAlwaysPass, "alwayspass");
_LIT(KAccessAlwaysFail, "alwaysfail");
// could do max of _LITs above
static const TInt KMaxAccessTypeLen = 6; 

// longest capability string from CapabilityNames is 15
static const TInt KMaxCapabilityStringLen = 20; 

static const TInt KBufLen = MAX3(KVersionLen, KSignatureLen, KIniFileSectionLen);

_LIT(KCrNl,	"\r\n");
const TUint KCr = '\r';
const TUint KTab = '\t';
const TUint KSpace = ' ';

_LIT(KHexIntFormat, "0x%X");
_LIT(KUidFormat, "0x%08X");
_LIT(KRangeMetaFmt, "0x%X 0x%X 0x%08X");
_LIT(KMaskMetaFmt, "0x%X mask = 0x%X 0x%08X");

_LIT(KRangePrefix, "0x%X 0x%X");
_LIT(KMaskPrefix, "0x%08X mask = 0x%08X");

//
// Local functions

/**
The function checks if the file with aFile name exists.
@param aFile File name, including the full path.
@return 0, if the file does not exist, non-zero value otherwise.
@internalComponent
*/
static TBool FileExists(const TDesC& aFile)
	{
	TEntry entry;
	return TServerResources::iFs.Entry(aFile, entry) == KErrNone;
	}

/**
@internalComponent
*/
static TInt ReadFileL(RFile aFile, HBufC16*& aBuf)
	{
	TInt size;
	TInt r = aFile.Size(size);
	if(r!=KErrNone)
		return r;
	if(size<2)
		return KErrCorrupt;

	TInt len = size/2-1;
	aBuf = HBufC16::NewL(len);
	TPtr16 ptr16 = aBuf->Des();
	TPtr8 ptr8((TUint8*)ptr16.Ptr(), 0, 2);
	r = aFile.Read(ptr8, 2);
	if(r!=KErrNone)
		return r;

	if(*ptr16.Ptr()!=KUcs2Bom)
		return KErrCorrupt;

	ptr8.Set((TUint8*)ptr16.Ptr(), 0, size-2);
	r = aFile.Read(ptr8);
	if(r!=KErrNone)
		return r;
	ptr16.SetLength(len);

	return KErrNone;
	}

static TBool IsNegativeNumber(TLex& aLex)	
    {
    if (aLex.Peek()=='-')
        return ETrue;
    else
        return EFalse;
    }

/**
@internalComponent
*/
static TInt ReadNumberL(TLex& aLex, TUint32& aVal)
	{
	TRadix radix = EDecimal;
	if(aLex.Peek()=='0')
		{
		aLex.Inc();
		if(aLex.Peek().GetLowerCase()=='x')
			{
			aLex.Inc();
			radix = EHex;
			}
		else
			aLex.UnGet();
		}

	if(aLex.Val(aVal, radix)!=KErrNone)
		return KErrCorrupt;
	
	return KErrNone;
	}

/**
@internalComponent
*/
static void WriteBinary(TDes& aBuf, const HBufC8* aString)
	{
	if(aString)
		{
		TInt len = aString->Length();
		if(len==0)
			aBuf.Append(KNullDataIndicator);
		else
			{
			TPtr8 ptr8 = const_cast<HBufC8*>(aString)->Des();
			for(TInt i=0;i<len;i++)
				aBuf.AppendNumFixedWidth(ptr8[i], EHex, 2);
			}
		}
	else
		{
		aBuf.Append(KNullDataIndicator);
		}
	}

/**
The function writes setting value into the supplied buffer (aBuf).
@param aBuf The buffer where the setting value will be appended.
@param aSetting Reference to the setting object
@leave KErrGeneral If the supplied setting object has unknown type.
@internalComponent
*/
static void AddSettingValueL(TDes& aBuf, const TServerSetting& aSetting)
	{
	switch(aSetting.Type())
		{
		case TServerSetting::EInt:
			aBuf.Append(KTypeInt);
			aBuf.Append(KSpace);
			aBuf.AppendNum(aSetting.GetIntValue());
			break;
		case TServerSetting::EReal:
			aBuf.Append(KTypeReal);
			aBuf.Append(KSpace);
			aBuf.AppendNum(aSetting.GetRealValue(), TRealFormat());
			break;
		case TServerSetting::EString:
			aBuf.Append(KTypeBinary);
			aBuf.Append(KSpace);
			WriteBinary(aBuf, aSetting.GetStrValue());
			break;
		default:
			User::Leave(KErrGeneral); //unknown setting type
			break;
		}
	}

//
// CIniFileIn class

TInt CIniFileIn::NewLC(CIniFileIn*& aIniFile,HBufC* aFullFileName,TCentRepLocation aLocation)
	{
	aIniFile = new(ELeave) CIniFileIn();
	CleanupStack::PushL(aIniFile);
	RFile file;
	CleanupClosePushL(file);
	TInt r = file.Open(TServerResources::iFs, *aFullFileName, EFileRead|EFileStreamText);	
	if(r==KErrNone)
		{
		TInt rReadFile = ReadFileL(file,aIniFile->iBuf);
		CleanupStack::PopAndDestroy(); //file
		TInt rReadHeader=KErrNone;
		if(rReadFile==KErrNone)
			{
			aIniFile->iLex.Assign(aIniFile->iBuf->Des());
			rReadHeader=aIniFile->ReadHeaderL();
			}
		
		// Delete corrupt file if it's not in ROM
		if((rReadFile==KErrCorrupt) || ( rReadHeader==KErrCorrupt))
			{
			if(aLocation!=ERom)
				User::LeaveIfError(TServerResources::iFs.Delete(*aFullFileName));
			return KErrCorrupt;
			}
		}
	else
		{
		CleanupStack::Pop();//file 
		}
	return r;
	
	
	}		
	
CIniFileIn::~CIniFileIn()
	{
	delete iBuf;
	delete iFullName;
	}


TInt CIniFileIn::ReadHeaderL()
	{
	TBuf<KBufLen> buf;

	//
	// Check file signature
	//

	SkipComments();

	iLex.Mark();
	iLex.SkipCharacters();

	if(iLex.TokenLength()>KSignatureLen)
		return(KErrCorrupt);

	buf.CopyLC(iLex.MarkedToken());
	if(buf.Compare(KSignature)!=0)
		return(KErrCorrupt);

	//
	// Check file version
	//

	SkipComments();

	iLex.Mark();
	iLex.SkipCharacters();

	if(iLex.TokenLength()>KVersionLen)
		return(KErrCorrupt);

	buf.CopyLC(iLex.MarkedToken());
	if(buf.Compare(KVersion)!=0)
		return(KErrCorrupt);

	iLex.SkipSpace();

	TUint version;
	iLex.Val(version);
	if(version>KCurrentVersion)
		return(KErrNotSupported);
	return( KErrNone);
	}

void CIniFileIn::SkipComments()
	{
	for(;;)
		{
		iLex.SkipSpace();

		if(iLex.Peek()!='#')
			break;

		while(iLex.Get()!='\n' && !iLex.Eos()) {}
		}
	}
	
void CIniFileIn::SkipEqualSign()
	{
	iLex.SkipSpace();
	if(iLex.Peek()=='=')
		iLex.Get();
		
	iLex.SkipSpace();
	}

TInt CIniFileIn::ReadSettingOnlyL(TServerSetting& aSetting,TBool& aSingleMetaFound)
	{
	TInt ret = KErrNone;
	
	aSingleMetaFound=EFalse;
	
	SkipComments();
	iLex.SkipSpace();

	if(iLex.Eos())
		return KErrNotFound;

	TUint32 key;
	TInt r=ReadNumberL(iLex, key);
	if(r!=KErrNone)
		return r;
	aSetting.SetKey(key);

	iLex.SkipSpaceAndMark();
	iLex.SkipCharacters();

	if(iLex.TokenLength()>KMaxTypeLen)
		return KErrCorrupt;

	TBuf<KMaxTypeLen> type;
	type.CopyLC(iLex.MarkedToken());

	iLex.SkipSpace();

	if(type.Compare(KTypeInt)==0)
		{
        if (IsNegativeNumber(iLex))
            {
            TInt i;
            if(iLex.Val(i)!=KErrNone)
                return(KErrCorrupt);
            aSetting.SetIntValue(i);  
            }
        else
            {
            TUint32 i;
            TInt r=ReadNumberL(iLex, i);
            if(r!=KErrNone)
                return r;
            aSetting.SetIntValue(i);  
            }
		}
	else if(type.Compare(KTypeReal)==0)
		{
		TReal r;
		ret=iLex.Val(r,'.');
		//iLex.Val with TReal can return KErrNoMemory
		if (ret!=KErrNone)
			{
			if (ret==KErrNoMemory)	
				User::LeaveNoMemory();
			else
				return KErrCorrupt;
			}
		TReal* temp = new(ELeave)TReal(r);
		aSetting.SetRealValue(temp);
		temp = NULL;
		}
	else if(type.Compare(KTypeString)==0)
		{
		HBufC8* s;
		ret = ReadStringL(s);
		if(ret != KErrNone)
			return KErrCorrupt;
		aSetting.SetStrValue(s);
		}
		
	else if(type.Compare(KTypeString8)==0)
		{
		HBufC8* s;
		ret = ReadString16To8L(s);
		if(ret != KErrNone)
			return KErrCorrupt;
		aSetting.SetStrValue(s);
		}	
		
	else if(type.Compare(KTypeBinary)==0)
		{
		HBufC8* s;
		ret = ReadBinaryL(s);
		if(ret != KErrNone)
			return KErrCorrupt;
		aSetting.SetStrValue(s);
		}
	else
		return KErrCorrupt;

	//skip any spaces or tabs
	while(iLex.Peek()==KSpace || iLex.Peek()==KTab)
		{
		iLex.Inc();
		}
    
    TUint32 meta;
    
    /**
    carriage return reached which means that there is no meta AND capabilities
    defined for this key. Thus setting meta to NULL to be able to set a value
    from default section later. 
    */ 
    if (iLex.Peek()==KCr)
        {   
        meta = NULL;   
        }
    else
        {
        r=ReadNumberL(iLex, meta);
	    /**
	    If meta can not be read, it is not neccessary an error.
	    It might be not present for an individual key and it will be taken
	    from a default section. 
	    If single meta is present, we need to remember so we can recognise a single
	    meta of 0 as distinct from no meta ( also sets meta to 0 ).
	    */ 
	    if(r!=KErrNone)
		    meta = NULL;
	    else
	    	aSingleMetaFound=ETrue;
        }
        
	aSetting.SetMeta(meta);

	return KErrNone;
	}
	
/**
Read an entire DefaultMeta section from ini file 
and create FDefault metadata entries

@internalTechnology
@return KErrNone, KErrCorrupt or KErrNotFound
*/
TInt CIniFileIn::ReadDefaultMetaSecSectionL(TUint32& aDefaultMeta,  RDefaultMetaArray& aDefaultMetaRanges)
	{
	TBuf<KBufLen> buf;

	//
	// Check if a DefaultMeta section is present
	//

	SkipComments();
	
	// we will need this section later to write the out file...
	iLex.Mark(iMainSectionMark);
	
	iLex.Mark();
	iLex.SkipCharacters();
	
	if( iLex.TokenLength()!=KDefaultMetaSectionLen || 
		(buf.CopyLC( iLex.MarkedToken() ), buf.Compare( KDefaultMetaSection )!=0) )
		{
		// Meta not available
		iLex.UnGetToMark();
		return KErrNotFound;
		}

	//
	// Lets read Meta settings
	//

	SkipComments();

	// we might have a default Meta section first
	if(KErrNone != ReadNumber(aDefaultMeta))
		{
		// should we log that no default read policy?
		}

		
	// now lets try range policies
	TInt r=ReadRangeMetaDefaultsL(aDefaultMetaRanges);
	if(r!=KErrNone)
		return r;	
	iLex.Mark(iMainSectionMark);
	return KErrNone;
	}


/**
Reads Meta defaults as defined for range of indexes

@internalTechnology
@return KErrNone, KErrCorrupt
*/
TInt CIniFileIn::ReadRangeMetaDefaultsL(RDefaultMetaArray& aDefaultMetaRanges)
{
	TUint32 lowKey = 0;
	TBuf<KBufLen> buf;
	
	SkipComments();
	while(KErrNone == ReadNumber(lowKey))
		{
		// highKey and mask needs to be zero'd every cycle...
		TUint32 highKey = 0;
		TUint32 mask = 0;
		TUint32 defaultMeta = 0 ;
		
		iLex.SkipSpace();
		// may be not range but key & mask so lets check 'mask' keyword
		if(!iLex.Peek().IsDigit())
			{
			//so should be mask then...
			iLex.Mark();
			while((iLex.Peek()!='=')&&(!iLex.Eos()))
				{
				iLex.Inc();

				if(iLex.TokenLength() >= KMaskLen)
					{
					// so no '=' there
					// not necessarily bad thing... could be space there first
					break;			
					}
			
				}
			
			// check if KMaskLen is < buf length?
			buf.CopyLC(iLex.MarkedToken());
			if(buf.Compare(KMaskString)!=0)
				{
				return KErrCorrupt;
				}	
				
			iLex.SkipSpace();
			if('=' != iLex.Get())
				return KErrCorrupt;
			
			iLex.SkipSpace();
			TInt r=ReadNumberL(iLex,mask);
			if(r!=KErrNone)
				return KErrCorrupt;
			}
		else
			{
			TInt r = ReadNumberL(iLex,highKey);
			if(r!=KErrNone)
				return KErrCorrupt;
			}
			
			
		if(KErrNone == 	ReadNumber(defaultMeta))
			{
			TSettingsDefaultMeta metaDefault(defaultMeta,lowKey, highKey, mask);
			aDefaultMetaRanges.AppendL(metaDefault);
			}
		else
			{
			// range specified with no default Meta!
			return KErrCorrupt;
			}
		SkipComments();
		}
	return KErrNone;
}

/**
Read Owner section from ini file and extract owner UID 

@internalTechnology
@return KErrNone, KErrCorrupt or KErrNotFound
*/
TInt CIniFileIn::ReadOwnerSectionL(TUint32 &aOwnerUID)
	{
	TBuf<KBufLen> buf;



	SkipComments();
	
	// we will need this section later to write the out file...
	iLex.Mark(iMainSectionMark);
	
	iLex.Mark();
	iLex.SkipCharacters();
	
	if( iLex.TokenLength()!=KOwnerSectionLen || 
		(buf.CopyLC( iLex.MarkedToken() ), buf.Compare( KOwnerSection )!=0) )
		{
		// Owner section not available
		iLex.UnGetToMark();
		return KErrNotFound;
		}
	else
		{
		// Found an "owner" section - must be followed by a UID (hex number
		// in format 0xnnnnn) to be valid!
		iLex.SkipSpace() ;
		if(iLex.Peek()=='0')
			{
			iLex.Inc();
			if(iLex.Peek().GetLowerCase()=='x')
				{
				iLex.Inc();
				if(iLex.Val(aOwnerUID, EHex)!=KErrNone)
					{
					return KErrCorrupt;
					}
				}
			else
				{
				return KErrCorrupt;
				}
			}
		else
			return KErrCorrupt;	
		}

	iLex.Mark(iMainSectionMark);

	return KErrNone;
	}

/**
Read Timestamp section from ini file and extract value as a TTime

@internalTechnology
@return KErrNone, KErrCorrupt or KErrNotFound
*/
TInt CIniFileIn::ReadTimeStampSectionL(TTime &aTimeStamp)
	{
	TBuf<25> buf;
	SkipComments();
	
	// we will need this section later to write the out file...
	iLex.Mark(iMainSectionMark);
	
	iLex.Mark();
	iLex.SkipCharacters();
	
	buf.CopyLC( iLex.MarkedToken());
	
	if( iLex.TokenLength()!=KTimeStampSectionLen || 
		(buf.Compare( KTimeStampSection )!=0) )
		{
		// Timestamp section not available
		iLex.UnGetToMark();
		return KErrNotFound;
		}
	else
		{
		// Found a "timestamp" section - must be followed by a a timestamp 
		// either in format...
		//
		// 		YYYYMMDD:HHMMSS.MMMMMM where: 
		// 			YYYY = 4 digit year
		//		  	  MM = 2 digit numeric month
		//        	  DD = 2 digit numeric date
		//		  	  HH = 2 digit hour
		//		  	  MM = 2 digit minute
		//        	  SS = 2 digit second
		//        MMMMMM = 6 digit microseconds
		//	Note that this is the format used for constructing/initialising
		//  a TTime	from a string.
		// 
		//                     ...or a 64-bit integer which can be converted to
		// to a TTime to be considered valid!
		//
		iLex.SkipSpace();
		iLex.Mark();
		iLex.SkipCharacters() ;

		buf.CopyLC(iLex.MarkedToken()) ;
		if (aTimeStamp.Set(buf) !=KErrNone)
			{
			TInt64 intTimeStamp ;
			iLex.UnGetToMark();
			if (iLex.Val(intTimeStamp) != KErrNone)
				{
				return KErrCorrupt;	
				}
			else
				{
				aTimeStamp = intTimeStamp;	
				}			
			}
		}
	iLex.Mark(iMainSectionMark);
	return KErrNone;
	}

/**
Read a setting and it's single policy ( if it exists )

@internalTechnology
@return KErrNone, KErrCorrupt
		aSetting setting read from ini file
		aSingleReadPolicy single read policy if any
		aSingleWritePolicy single write policy if any
		aSingleReadPolicyFound ETrue if single read policy found with this key, EFalse if not
		aSingleWritePolicyFound ETrue if single write policy found with this key, EFalse if not
		aSingleMetaFound ETrue if single metadata found with this key, EFalse if not
*/
TInt CIniFileIn::ReadSettingL(TServerSetting& aSetting,TSecurityPolicy& aSingleReadPolicy,TSecurityPolicy& aSingleWritePolicy, TBool& aSingleReadPolicyFound, TBool& aSingleWritePolicyFound, TBool& aSingleMetaFound)
	{
	aSingleReadPolicyFound = EFalse;
	aSingleWritePolicyFound = EFalse;
	
	TInt error = ReadSettingOnlyL(aSetting, aSingleMetaFound);
	if(KErrNone == error)
		{
		//Need to push into cleanupstack for string setting
		aSetting.PushL();
		// when multiple policies enabled then read in a loop
		
		if (iLex.Peek() !=KCr)
			{
			// if neither read/write policy found we do not create TSettingsAccessPolicy at all...
			TInt err=ReadRdPolicyL(aSingleReadPolicy);
			if (err==KErrNone)
				aSingleReadPolicyFound=ETrue;
			else
				{
				//we need to return error code rather than assuming no single policy is found
				if (err==KErrCorrupt || err==KErrNoMemory)
					{
					aSetting.Pop();
					return err;
					}
				//else if ret!=KErrNone very likely it is KErrNotFound so leave
				//the state of the writePolicyFound to EFalse
				}
				
			err=ReadWrPolicyL(aSingleWritePolicy);
			if (err==KErrNone)
				aSingleWritePolicyFound=ETrue;
			else
				{
				//we need to return error code rather than assuming no single policy is found
				if (err==KErrCorrupt || err==KErrNoMemory)
					{
					aSetting.Pop();
					return err;
					}
				//else if ret!=KErrNone very likely it is KErrNotFound so leave
				//the state of the writePolicyFound to EFalse
				}
			}
							
		//Need to pop back the setting
		aSetting.Pop();
		}
	return error;
	}

TInt CIniFileIn::SkipPlatSecSectionL()
	{
	HBufC* platSecSection;
	TInt r=GetPlatSecSectionLC(platSecSection);
	if(platSecSection)		
		CleanupStack::PopAndDestroy(platSecSection);
	return r;
	}
	
TInt CIniFileIn::SkipOwnerSectionL()
	{
	HBufC* ownerSection;
	TInt r=GetOwnerSectionLC(ownerSection);
	if(ownerSection)	
		CleanupStack::PopAndDestroy(ownerSection);
	return r;
	}
	
TInt CIniFileIn::SkipDefaultMetaSectionL()
	{
	HBufC* section;
	TInt r=GetDefaultMetaSectionLC(section);
	if(section)	
		CleanupStack::PopAndDestroy(section);
	return r;
	}
	
TInt CIniFileIn::SkipTimeStampSectionL()
	{
	HBufC* timeStampSection;
	TInt r=GetTimeStampSectionLC(timeStampSection);
	if(timeStampSection)	
		CleanupStack::PopAndDestroy(timeStampSection);
	return r;
	}	
	
TInt CIniFileIn::GetAccessPolicyTextL(TUint aKey,HBufC*& aText)
	{
	TInt error = KErrNotFound;
	TServerSetting setting;
	iLex.UnGetToMark(iMainSectionMark);
	TBool dummy;
	while (KErrNone == ReadSettingOnlyL(setting, dummy))
		{
		//Need to push string setting as the AllocL can leave
		setting.PushL();
		if(setting.Key() == aKey)
			{
			iLex.Mark();
			// simply read until eol... not ideal but should do for now
			while(('\n' != iLex.Get())&&(!iLex.Eos())) {};
			aText = iLex.MarkedToken().AllocL();
			error = KErrNone;
			setting.Pop();
			setting.Reset();
			break;
			}
		else
			// finish this line in case there is some PlatSec info there...
			while(('\n' != iLex.Get())&&(!iLex.Eos())) {};
		setting.Pop();
		setting.Reset();
		}
	return error;
	}

/**
Read an entire PlatSec section from ini file 
and create TSecurityPolicy for default access and for range of indexes

@internalTechnology
@return KErrNone, KErrCorrupt or KErrNotFound
*/
TInt CIniFileIn::ReadPlatSecSectionL(TSecurityPolicy& aDefaultReadPolicy, TBool& aGotDefaultReadPolicy,
							         TSecurityPolicy& aDefaultWritePolicy, TBool& aGotDefaultWritePolicy,
							         RRangePolicyArray& aRangePolicies)

	{
	TBuf<KBufLen> buf;
	
	aGotDefaultReadPolicy = EFalse ;
	aGotDefaultWritePolicy = EFalse ;
	
	//
	// Check if the PlatSec section is present
	//
	SkipComments();
	
	// we will need this section later to write the out file...
	iLex.Mark(iMainSectionMark);
	
	iLex.Mark();
	iLex.SkipCharacters();
	
	if( iLex.TokenLength()!=KPlatSecSectionLen || 
		(buf.CopyLC( iLex.MarkedToken() ), buf.Compare( KPlatSecSection )!=0) )
		{
		// PlatSec section not available
		iLex.UnGetToMark();
		return KErrNotFound;
		}

	//
	// Lets read the policies
	//

	SkipComments();
	TInt r=KErrNone;
	// we might have a default policies first
	// check for default read policy
	r=ReadRdPolicyL(aDefaultReadPolicy);
	if (r==KErrNone)
		aGotDefaultReadPolicy=ETrue;
	else
		{
		//we need to return error code rather than assuming no default policy is found
		if (r==KErrCorrupt || r==KErrNoMemory)
			return r;
		//else if ret!=KErrNone very likely it is KErrNotFound so leave
		//the state of the writePolicyFound to EFalse
		}
	// check for default write policy
	r=ReadWrPolicyL(aDefaultWritePolicy);
	if (r==KErrNone)
		aGotDefaultWritePolicy=ETrue;
	else
		{
		//we need to return error code rather than assuming no default policy is found
		if (r==KErrCorrupt || r==KErrNoMemory)
			return r;
		//else if ret!=KErrNone very likely it is KErrNotFound so leave
		//the state of the writePolicyFound to EFalse
		}
	// now lets try range policies
	r = ReadRangePoliciesL(aDefaultReadPolicy,aDefaultWritePolicy,aRangePolicies);
	if(r!=KErrNone)
		return KErrCorrupt;
	
	// it must be the main section now so lets check
	SkipComments();
	iLex.Mark();
	iLex.SkipCharacters();
	
	if(iLex.TokenLength()>KBufLen)
		return KErrCorrupt;
		
	buf.CopyLC(iLex.MarkedToken());
	if(buf.Compare(KMainSection)!=0)
		{
		return KErrCorrupt;
		}	

	iLex.Mark(iMainSectionMark);
		
	return KErrNone;
	}

/**
Reads TSecurityPolicy as defined for range of indexes

@internalTechnology
@return KErrNone, KErrCorrupt
@leave KErrNotFound
*/
TInt CIniFileIn::ReadRangePoliciesL(const TSecurityPolicy& aDefaultReadPolicy,
									const TSecurityPolicy& aDefaultWritePolicy,
									RRangePolicyArray& aRangePolicies)
{
	TUint32 lowKey = 0;
	TBuf<KBufLen> buf;
	
	SkipComments();
	while(KErrNone == ReadNumber(lowKey))
		{
		// highKey and mask needs to be zero'd every cycle...
		TUint32 highKey = 0;
		TUint32 mask = 0;
		iLex.SkipSpace();
		// may be not range but key & mask so lets check 'mask' keyword
		if(!iLex.Peek().IsDigit())
			{
			//so should be mask then...
			iLex.Mark();
			while((iLex.Peek()!='=')&&(!iLex.Eos()))
				{
				iLex.Inc();

				if(iLex.TokenLength() >= KMaskLen)
					{
					// so no '=' there
					// not necessarily bad thing... could be space there first
					break;			
					}
			
				}
			
			// check if KMaskLen is < buf length?
			buf.CopyLC(iLex.MarkedToken());
			if(buf.Compare(KMaskString)!=0)
				{
				return KErrCorrupt;
				}	
				
			iLex.SkipSpace();
			if('=' != iLex.Get())
				return KErrCorrupt;
			
			iLex.SkipSpace();
			TInt r = ReadNumberL(iLex,mask);
			if(r!=KErrNone)
				return KErrCorrupt;
			}
		else
			{
			TInt r = ReadNumberL(iLex,highKey);
			if(r!=KErrNone)
				return KErrCorrupt;
			}
		TBool writePolicyFound = EFalse;
		TBool readPolicyFound= EFalse;
		TSecurityPolicy readPolicy;
		TSecurityPolicy writePolicy;	
		
		TInt ret=KErrNone;
		ret=ReadRdPolicyL(readPolicy);
		if (ret==KErrNone)
			readPolicyFound=ETrue;
		else
			{
			if (ret==KErrCorrupt || ret==KErrNoMemory)
				return ret;
			//else if ret!=KErrNone very likely it is KErrNotFound so leave
			//the state of the writePolicyFound to EFalse
			}
		ret=ReadWrPolicyL(writePolicy);
		if (ret==KErrNone)
			writePolicyFound=ETrue;
		else
			{
			if (ret==KErrCorrupt || ret==KErrNoMemory)
				return ret;
			//else if ret!=KErrNone very likely it is KErrNotFound so leave
			//the state of the writePolicyFound to EFalse
			}
		//If only one of the policy is specified,need to set the other one to default value
		//to prevent it from being uninitialized
		if(readPolicyFound || writePolicyFound)
			{
			if (!readPolicyFound)
				readPolicy=aDefaultReadPolicy;		
			if (!writePolicyFound)
				writePolicy=aDefaultWritePolicy;
			TSettingsAccessPolicy settingsPolicy(readPolicy,writePolicy,
												lowKey, highKey, mask);
			aRangePolicies.AppendL(settingsPolicy);
			}
		else
			{
			// range specified with no policies!
			return KErrCorrupt;
			}
		SkipComments();	
		}
	return KErrNone;
}

/**
@internalTechnology
@return TCapability as converted from string description
@leave KErrNotFound
*/
TInt CIniFileIn::ReadCapabilityL(TCapability& aCapability)
	{
	iLex.SkipSpace();
	
	if(iLex.Eos())
		User::Leave(KErrNotFound);
	
	// check if '=' still there and skip
	SkipEqualSign();
		
	iLex.Mark();
	
	// potentially comma separated list of capabilities
	// we read just one at the time
	while(!iLex.Peek().IsSpace() && (iLex.Peek() != ',') && !iLex.Eos())
		{
		iLex.Inc();
		if(iLex.TokenLength()>KMaxCapabilityStringLen)
			return KErrCorrupt;
		}

	TBuf<KMaxCapabilityStringLen> string;
	string.CopyLC(iLex.MarkedToken());
	
	// lets check against list of capabilities
	TInt capability;
	
	// descriptors...desriptors - we need it for conversion from const char[] to TPtr
	HBufC *cap = HBufC::NewLC(KMaxCapabilityStringLen);
	TPtr capPtr = cap->Des() ;	
	HBufC8 *capNarrow = HBufC8::NewLC(KMaxCapabilityStringLen) ;
	for(capability=0; capability<ECapability_Limit; capability++)
		{
		// CapabilityNames is const char[]
		*capNarrow = (const TUint8 *)CapabilityNames[capability];
	
		capPtr.Copy(*capNarrow);
		capPtr.LowerCase();
		if(0 == string.Compare(capPtr))
			{
			aCapability=static_cast<TCapability>(capability);
			CleanupStack::PopAndDestroy(capNarrow);
			CleanupStack::PopAndDestroy(cap);
			return KErrNone;
			}
		}
	CleanupStack::PopAndDestroy(capNarrow);
	CleanupStack::PopAndDestroy(cap);

	// to satisfy compiler
	aCapability=ECapability_Limit;

	// we didn't find anything
	return KErrCorrupt;
	
	}
	
/**
@internalTechnology
@param aAlwaysPass will be true if the first capability is AlwaysPass
	   aAlwaysFail will be true if the first capability is AlwaysFail
*/
void CIniFileIn::CheckForAlwaysPassOrFailL(TBool& aAlwaysPass,TBool& aAlwaysFail)
	{
	iLex.SkipSpace();
	
	if(iLex.Eos())
		User::Leave(KErrNotFound);
	
	// check if '=' still there and skip
	SkipEqualSign();
		
	iLex.Mark();
	// we are just checking if AlwaysPass has been set
	while(!iLex.Peek().IsSpace() && !iLex.Eos())
		{
		iLex.Inc();
		if(iLex.TokenLength()>KMaxCapabilityStringLen)
			{
			iLex.UnGetToMark();
			return;
			}
			
		}

	TBuf<KMaxCapabilityStringLen> string;
	string.CopyLC(iLex.MarkedToken());
	
	aAlwaysPass=(string.Compare(KAccessAlwaysPass)==0);
	aAlwaysFail=(string.Compare(KAccessAlwaysFail)==0);
	//if not either AlwaysPass or AlwaysFail reset the Lex position to Mark
	if(!(aAlwaysPass || aAlwaysFail))
		iLex.UnGetToMark();
	}
	
TInt CIniFileIn::ReadCapabilitiesL(TSecurityPolicy& aPolicy)
	{
	// we can have 0-7 capabilities
	const TInt maxCapWithoutSid = 7;
	TCapability capabilities[maxCapWithoutSid];
	TInt index = 0;
	// initialise 
	for(index=0;index<maxCapWithoutSid;index++)
		capabilities[index] = ECapability_None;

	index = 0;
	
	// lets read the first capability... there must be at least one!
	TBool isAlwaysPass=EFalse;
	TBool isAlwaysFail=EFalse;
	CheckForAlwaysPassOrFailL(isAlwaysPass,isAlwaysFail);
	if(isAlwaysPass || isAlwaysFail)
		{
		//default is set to EAlwaysFail
		TSecurityPolicy policy;
		if (isAlwaysPass)
			policy=TSecurityPolicy(TSecurityPolicy::EAlwaysPass);
		aPolicy.Set(policy.Package());
		}
	else
		{
		TInt r=ReadCapabilityL(capabilities[index]);
		if(r!=KErrNone)
			return r;
	
		// do we have more?
		iLex.SkipSpace();
		index++;
		while((iLex.Peek() == ','))
			{
			//if capabilities supplied is more than allowed return KErrCorrupt
			if (index>=maxCapWithoutSid)
				return KErrCorrupt;
			// skip comma
			iLex.SkipAndMark(1);
			r=ReadCapabilityL(capabilities[index]);
			if(r!=KErrNone)
				return r;
			// do we have yet more?
			iLex.SkipSpace();
			index++;
			}
		TSecurityPolicy policy(static_cast<TCapability>(capabilities[0]),
								static_cast<TCapability>(capabilities[1]),
								static_cast<TCapability>(capabilities[2]),
								static_cast<TCapability>(capabilities[3]),
								static_cast<TCapability>(capabilities[4]),
								static_cast<TCapability>(capabilities[5]),
								static_cast<TCapability>(capabilities[6]));
		aPolicy.Set(policy.Package());
		}
	return KErrNone;
	}

TInt CIniFileIn::ReadSidAndCapabilitiesL(TSecurityPolicy& aPolicy,const TDesC& aPolicyType,
										 TSecureId& aSid)
	{
	//SID was specified we can have 0-3 capabilities
	const TInt maxCapWithSid = 3;
	
	TCapability capabilities[maxCapWithSid];
	TInt index = 0;
	for(index=0;index<maxCapWithSid;index++)
		capabilities[index] = ECapability_None;

	// lets see what we have here...
	iLex.SkipSpaceAndMark();
	
	// we are looking for a string terminated by '='
	// up to a certain length....
	while((iLex.Peek()!='=')&&(!iLex.Eos()))
		{
		iLex.Inc();

		if(iLex.TokenLength() >= KMaxAccessTypeLen)
			{
			// so no '=' there
			// not necessarily bad thing... could be space there first
			break;
			}
			
		}
		
	TBuf<KMaxAccessTypeLen> string;
	string.CopyLC(iLex.MarkedToken());
	
	index = 0;
	// lets check if there are any capabilities specified and if of correct type
	if(0 == string.Compare(aPolicyType))
		{
		//Need to check for AlwaysPass or AlwaysFail
		TBool isAlwaysPass=EFalse;
		TBool isAlwaysFail=EFalse;
		CheckForAlwaysPassOrFailL(isAlwaysPass,isAlwaysFail);
		if(isAlwaysPass || isAlwaysFail)
			{
			//default is set to EAlwaysFail
			TSecurityPolicy policy;
			if (isAlwaysPass)
				policy=TSecurityPolicy(TSecurityPolicy::EAlwaysPass);
			aPolicy.Set(policy.Package());
			}
		else
			{
			// so we have some capabilities to read
			TInt r = ReadCapabilityL(capabilities[index]);
			if(r!=KErrNone)
				return r;
			// do we have more?
			iLex.SkipSpace();
			index++;
			while((iLex.Peek() == ','))
				{
				//cannot permit more than 3 capabilities when followed by a SID
				if (index>=maxCapWithSid)
					return KErrCorrupt;
				// skip comma
				iLex.SkipAndMark(1);
				TInt r= ReadCapabilityL(capabilities[index]);
				if(r!=KErrNone)
					return r;
				// do we have yet more?
				iLex.SkipSpace();
				index++;
				}
		TSecurityPolicy policy(aSid,static_cast<TCapability>(capabilities[0]),
								static_cast<TCapability>(capabilities[1]),
								static_cast<TCapability>(capabilities[2]));
		aPolicy.Set(policy.Package());
			}
		}
	else
		{
		// so no capabilities just SID
		// and the token wasn't for us either
		iLex.UnGetToMark();
		TSecurityPolicy policy(aSid);
		aPolicy.Set(policy.Package());
		}
	return KErrNone;
	}


TInt CIniFileIn::ReadPolicyL(TSecurityPolicy& aPolicy,TInt aPolicyType)
	{
	
	// lets check if there a SID is specified
	iLex.SkipSpaceAndMark();
	
	if(iLex.Eos())
		return KErrNotFound;
	
	while((iLex.Peek()!='=')&&(!iLex.Eos()))
		{
		iLex.Inc();

		if(iLex.TokenLength() >= KMaxAccessTypeLen)
			{
			// so no '=' there
			// not necessarily bad thing... could be space there first
			break;
			}
			
		}
	
	// we are looking for either KReadAccessSid, KReadAccessCap, KWriteAccessSid,KWriteAccessCap
	TBuf<KMaxAccessTypeLen> accessString;
	accessString.CopyLC(iLex.MarkedToken());
	iLex.SkipSpace();
	TInt returnCode = KErrNotFound;
	// we expect a combination of sid_rd1 cap_rd1 sid_wr1 cap_wr1
	if(accessString.Compare(KReadAccessSidString)==0)
		{
		// we've got read - either SID or SID+CAP! Are we expecting read?
		if(KReadPolicy == aPolicyType)
			{
			TUint32 sid;
			SkipEqualSign();
			if (ReadNumber(sid) != KErrNone)
				return KErrCorrupt;
			
			TSecureId sidId(sid);
			// so we read sid and now we expect cap_rd1=cap1,cap2,..
			// lets assume it's a SID+CAP for now
			returnCode= ReadSidAndCapabilitiesL(aPolicy,KReadAccessCapString,sidId);
			}
		}
	else if(accessString.Compare(KReadAccessCapString)==0)
		{
		// we've got read CAP only! Are we expecting read?
		if(KReadPolicy == aPolicyType)
			{
			returnCode=ReadCapabilitiesL(aPolicy);
			}
		}
	else if(accessString.Compare(KWriteAccessSidString)==0)
		{
		// we've got write - either SID or SID+CAP! Are we expecting read?
		if(KWritePolicy == aPolicyType)
			{
			TUint32 sid;
			SkipEqualSign();
			if(ReadNumber(sid)!=KErrNone)
				return KErrCorrupt;
			
			TSecureId sidId(sid);
			// lets assume SID+CAP for now
			returnCode=	ReadSidAndCapabilitiesL(aPolicy,KWriteAccessCapString,sidId);
			}
		}
	else if(accessString.Compare(KWriteAccessCapString)==0)
		{
		// we've got write CAP only! Are we expecting write?
		if(KWritePolicy == aPolicyType)
			{
			returnCode=ReadCapabilitiesL(aPolicy);
			}
		}
	if(KErrNone != returnCode)
		iLex.UnGetToMark();

	return returnCode;
	}

TInt CIniFileIn::ReadRdPolicyL(TSecurityPolicy& aReadPolicy)
	{
	return ReadPolicyL(aReadPolicy,KReadPolicy);
	}
	
TInt CIniFileIn::ReadWrPolicyL(TSecurityPolicy& aReadPolicy)
	{
	return ReadPolicyL(aReadPolicy,KWritePolicy);
	}

TInt CIniFileIn::ReadStringL(HBufC8*& aString)
	{
	iLex.Mark();

	TChar c = iLex.Peek();
	TChar quote = 0;
	if(c=='\'' || c=='\"')
		{
		iLex.SkipAndMark(1);
		quote = c;
		}

	TBool complete = EFalse;

	TInt len;
	for(len=0;!iLex.Eos();len++)
		{
		c = iLex.Get();

		if(quote ? c==quote : c.IsSpace())
			{
			complete = ETrue;
			break;
			}

		if(c=='\\')
			iLex.Get();
		}

	if(!complete || len>KMaxUnicodeStringLength)
		return KErrCorrupt;

	aString = HBufC8::NewL(len*2);
	TPtr8 ptr8 = aString->Des();
	ptr8.SetLength(len*2);
	TPtr16 ptr16((TUint16*)ptr8.Ptr(), len, len);
	
	
	iLex.UnGetToMark();

	_LIT(KSpecialChars, "abfnrvt0");
	static TUint8 specialChars[] = { '\a', '\b', '\f', '\n', '\r', '\v', '\t', '\0' };
	for(TInt i=0;i<len;i++)				
		{
		c = iLex.Get();

		if(c=='\\')
			{
			c = iLex.Get();
			TInt i = KSpecialChars().Locate(c);
			if(i>=0)
				c = specialChars[i];
			}

		ptr16[i] = (TUint16)c;
		
		}	

	if(quote)
		iLex.Inc();  // trailing quote
	
	return KErrNone;
	}



TInt CIniFileIn::ReadString16To8L(HBufC8*& aString)
	{
		iLex.Mark();

	TChar c = iLex.Peek();
	TChar quote = 0;
	if(c=='\'' || c=='\"')
		{
		iLex.SkipAndMark(1);
		quote = c;
		}

	TBool complete = EFalse;

	TInt len;
	for(len=0;!iLex.Eos();len++)
		{
		c = iLex.Get();

		if(quote ? c==quote : c.IsSpace())
			{
			complete = ETrue;
			break;
			}

		if(c=='\\')
			iLex.Get();
		}

	if(!complete || len>KMaxUnicodeStringLength)
		return KErrCorrupt;

	aString = HBufC8::NewLC(len*2);

	HBufC16* tempBuffer = HBufC16::NewLC(len);
	
	TPtr16 ptr16 = tempBuffer->Des();
	TPtr8 ptr8 = aString->Des();
	ptr8.SetLength(len*2);
	
	
	
	iLex.UnGetToMark();

	_LIT(KSpecialChars, "abfnrvt0");
	static TUint8 specialChars[] = { '\a', '\b', '\f', '\n', '\r', '\v', '\t', '\0' };
	for(TInt i=0;i<len;i++)				
		{
		c = iLex.Get();

		if(c=='\\')
			{
			c = iLex.Get();
			TInt i = KSpecialChars().Locate(c);
			if(i>=0)
				c = specialChars[i];
			}

		ptr16.Append(c);
		
		}
		
	const TInt returnValue = CnvUtfConverter::ConvertFromUnicodeToUtf8(ptr8, ptr16);
	if (returnValue==CnvUtfConverter::EErrorIllFormedInput)
		{
		CleanupStack::PopAndDestroy(tempBuffer);
		CleanupStack::PopAndDestroy(aString);			
        return KErrCorrupt;
		}
    else if(returnValue<0)
        User::Leave(KErrGeneral);
    
	CleanupStack::PopAndDestroy(tempBuffer);
	CleanupStack::Pop(aString);	

	if(quote)
		iLex.Inc();  // trailing quote
	
	return KErrNone;
	}

TInt CIniFileIn::ReadBinaryL(HBufC8*& aString)
	{
	iLex.Mark();
	iLex.SkipCharacters();
	TInt len = iLex.TokenLength();
	iLex.UnGetToMark();

	if(len==1 && iLex.Peek()==KNullDataIndicator)
		{
		iLex.Get();
		aString = HBufC8::NewL(0);
		TPtr8 ptr8 = aString->Des();
		ptr8.SetLength(0);
		return KErrNone;
		}

	if(len>KMaxBinaryLength*2 || len%2)
		{
		delete aString;
		return KErrCorrupt;
		}
		
	len /= 2;
	aString = HBufC8::NewL(len);
	TPtr8 ptr8 = aString->Des();
	ptr8.SetLength(len);

	TBuf<2> buf(2);
	for(TInt i=0;i<len;i++)
		{
		buf[0] = (TUint8)iLex.Get();
		buf[1] = (TUint8)iLex.Get();
		TLex lex(buf);
		if(lex.Val(ptr8[i], EHex)!=KErrNone)
			{
			delete aString;	
			return KErrCorrupt;
			}
		}
	return KErrNone;
	}
	
TInt CIniFileIn::ReadNumber(TUint32& aVal)
	{
	iLex.SkipSpace();
	
	if(iLex.Eos())
		return KErrNotFound;
	
	TRadix radix = EDecimal;
	if(iLex.Peek()=='0')
		{
		iLex.Inc();
		if(iLex.Peek().GetLowerCase()=='x')
			{
			iLex.Inc();
			radix = EHex;
			}
		else
			iLex.UnGet();
		}

	return iLex.Val(aVal, radix);
	}


TInt CIniFileIn::GetOwnerSectionLC(HBufC*& aSection)
	{
	return( GetSectionLC(KOwnerSection(), aSection));
	}

TInt CIniFileIn::GetDefaultMetaSectionLC(HBufC*& aSection)
	{
	return( GetSectionLC(KDefaultMetaSection(), aSection));
	}	
	
TInt CIniFileIn::GetTimeStampSectionLC(HBufC*& aSection)
	{
	return( GetSectionLC(KTimeStampSection(), aSection));
	}
	
TInt CIniFileIn::GetPlatSecSectionLC(HBufC*& aSection)
	{
	return( GetSectionLC(KPlatSecSection(), aSection));
	}
	
TInt CIniFileIn::GetSectionLC(const TDesC16& aSectionId, HBufC*& aSection)
	{
	TBuf<KBufLen> buf;
	TLexMark sectionMark;
	aSection=NULL;
	
	SkipComments();

	iLex.Mark(sectionMark);
	
	iLex.Mark();
	iLex.SkipCharacters();	
	
	if( (iLex.TokenLength() != aSectionId.Length()) && 
	     (buf.CopyLC(iLex.MarkedToken()), buf.Compare( aSectionId )!=0) )
		{
		// Expected section not found at this point in the file
		// Note that this is not an error
		iLex.UnGetToMark();
		return KErrNone;
		}

	//
	// Read in the section by grabbing text until we reach
	// the start of another section.
	//
	while(!iLex.Eos())
		{
		// Wait for any other section marker
		SkipComments();
		
		iLex.Mark();

		iLex.SkipCharacters();
		
		if(iLex.TokenLength() <= KBufLen)
			{
			buf.CopyLC(iLex.MarkedToken());
			if((buf.Compare(KMainSection) == 0) ||
			   (buf.Compare(KOwnerSection) == 0) ||
			   (buf.Compare(KPlatSecSection) == 0) ||
			   (buf.Compare(KTimeStampSection) == 0) ||
			   (buf.Compare(KDefaultMetaSection) == 0))
				{
				iLex.Mark(iMainSectionMark);
				iLex.UnGetToMark() ;
				TPtrC lex = iLex.MarkedToken(sectionMark);
				HBufC* section = HBufC::NewMaxLC(lex.Length()); //'\n'
				TPtr ptr = section->Des();
				ptr.Copy(lex);
				aSection=section;
				return KErrNone;
				}	
			}
		}
	return KErrCorrupt;
	}	
	
TInt CIniFileIn::FindMainSectionL(void)
	{
	TBuf<KBufLen> buf;

	//
	// Check if a Main section is present
	//

	SkipComments();
	
	// we will need this section later to write the out file...
	iLex.Mark(iMainSectionMark);
	
	iLex.Mark();
	iLex.SkipCharacters();
	
	if( iLex.TokenLength()!=KMainSectionLen || 
		(buf.CopyLC( iLex.MarkedToken() ), buf.Compare( KMainSection )!=0) )
		{
		// Meta not available
		iLex.UnGetToMark();
		return KErrNotFound;
		}
		
	iLex.Mark(iMainSectionMark);
	return KErrNone ;
	}

HBufC* CIniFileIn::FullName()
	{
	return(iFullName);
	}
	
//===================================================================
// TCompiledSecurityPolicy class
// Used for accessing private data members of TSecurityPolicy. It
// uses the fact that TSecurityPolicy class has a friend whose name
// is TCompiledSecurityPolicy.
// See dbms/tdbms/securitypolicy.h for similar strategy.
// 

// The longest possible security string is one that has 7 capabilities.
static const TInt KSecPolicyStrSize = KMaxCapabilityStringLen * 7 + 10;

class TCompiledSecurityPolicy
	{
public:
	TCompiledSecurityPolicy(const TSecurityPolicy& aSecurityPolicy) :
		iSecurityPolicy(aSecurityPolicy) { }
	const TDesC& TextualizePolicyL(TCapAccessMode aMode);

private:
	enum THeaderType
		{
		EHdrSecureId,
		EHdrCapability
		};

	TCapability CapabilityAt(TInt aIndex) const;
	void DoCapabilitySection(TInt aMaxNumCaps);
	void AppendModeHeader(TCapAccessMode aAccessMode, THeaderType aType);

private:
	const TSecurityPolicy&  iSecurityPolicy;
	TBuf<KSecPolicyStrSize> iBuf;
	};

//
// CIniFileOut class
/**
Standard, phase-one CIniFileOut instance creation method.
The created CIniFileOut instance will use a temporary text file to store the repository settings.
CIniFileOut::CommitL() should be called at the end to finalize the changes.
@return A pointer to a fully constructed CIniFileOut instance.
@leave System-wide error codes, including KErrNoMemory.
*/
CIniFileOut* CIniFileOut::NewLC()
	{
	CIniFileOut* inifile = new(ELeave) CIniFileOut();
	CleanupStack::PushL(inifile);
	inifile->ConstructL();
	return inifile;
	}

CIniFileOut::CIniFileOut()
	: iCommited(EFalse), iTransFileBuf(4 * 1024)// 4K buffer size
    {
    }

/**
Standard, phase-two CIniFileOut instance creation method.
Creates the transaction file.
Initializes transaction file buffer - iTransFileBuf instance.
@leave System-wide error codes, including KErrNoMemory.
*/
void CIniFileOut::ConstructL()
	{
	User::LeaveIfError(iTransFilePath.Set(KTransactFileName, TServerResources::iDataDirectory, TServerResources::iTrnsExt));
	User::LeaveIfError(iTransFile.Replace(TServerResources::iFs, iTransFilePath.FullName(), EFileWrite | EFileStreamText));
	iTransFileBuf.Attach(iTransFile, 0);	
	}

/**
Closes and deletes the transaction file.
If CIniFileOut::CommitL() has not been called prior the destructor call, all the changes
will be lost.
*/
CIniFileOut::~CIniFileOut()
	{
	if (!iCommited)
		{
	    iTransFileBuf.Close();
    	TServerResources::iFs.Delete(iTransFilePath.FullName());
		}
	}

/**
The method writes supplied setting value to the output file.
@param aSetting Setting instance, which value has to be written to the output file.
@param accessPolicies A string descriptor, referencing related to aSetting access policies.
@leave System-wide error codes, including KErrNoMemory.
*/
void CIniFileOut::WriteSettingL(const TServerSetting& aSetting)
	{
	iBuf.Zero();
	DoSettingL(aSetting);
	WriteLineL(iBuf);
    }

void CIniFileOut::WriteSettingL(const TServerSetting& aSetting,
								const TSettingsAccessPolicy& aAccessPolicy)
	{
	iBuf.Zero();
	DoSettingL(aSetting);
	iBuf.Append(KSpace);

	AppendSecurityPolicyL(aAccessPolicy.iReadAccessPolicy, ECapReadAccess);
	iBuf.Append(KSpace);
	AppendSecurityPolicyL(aAccessPolicy.iWriteAccessPolicy, ECapWriteAccess);
	WriteLineL(iBuf);
	}

void CIniFileOut::DoSettingL(const TServerSetting& aSetting)
	{
	iBuf.AppendNum(aSetting.Key(), EDecimal);
	iBuf.Append(KSpace);
	
	::AddSettingValueL(iBuf, aSetting);

	iBuf.Append(KSpace);

	if (!aSetting.Meta())
		{
		iBuf.AppendNum(0, EDecimal);
		}
	else
		{
		iBuf.AppendFormat(KHexIntFormat, aSetting.Meta());
		}
	}

/**
The method commits settings file changes.
If the commit operation fails, the existing settings file will stay unchanged.
@leave System-wide error codes.
*/
void CIniFileOut::CommitL(const TDesC& aOutFileName)
    {
    iTransFileBuf.SynchL();
    iTransFileBuf.Close();

    if(::FileExists(aOutFileName))
        {
	    User::LeaveIfError(TServerResources::iFs.Replace(iTransFilePath.FullName(), aOutFileName));
        }
    else
        {
        User::LeaveIfError(TServerResources::iFs.Rename(iTransFilePath.FullName(), aOutFileName));
        }
    iCommited = ETrue;    
    }

void CIniFileOut::WriteMainSectionHeaderL()
	{
	WriteLineL(KMainSection());
	}

/**
Writes a text line to the repository file.
@param aData The string which will be written to the file as a single text line
@leave System-wide error codes
*/
void CIniFileOut::WriteLineL(const TDesC& aData)
	{
	iTransFileBuf.WriteL(reinterpret_cast <const TUint8*> (aData.Ptr()), aData.Size());
	iTransFileBuf.WriteL(reinterpret_cast <const TUint8*> (KCrNl().Ptr()), KCrNl().Size());
	}

/**
Writes repository file header.
@leave System-wide error codes
*/
void CIniFileOut::WriteHeaderL()
	{
	TBuf<64> buf;
	
	buf.Append(KUcs2Bom);
	buf.Append(KSignature);
	WriteLineL(buf);
	
	buf.Zero();
	buf.Append(KVersion);
	buf.Append(KSpace);
	buf.AppendNum(KCurrentVersion);
	buf.Append(KCrNl);
	WriteLineL(buf);
	}
	
/**
Writes owner section to repository file.
*/
void CIniFileOut::WriteOwnerSectionL(TUid aOwner)
	{
	if (aOwner.iUid != 0)
		{
		WriteLineL(KOwnerSection());
		TBuf<32> buf;
		buf.Format(KUidFormat, aOwner.iUid);
		buf.Append(KCrNl);
		WriteLineL(buf);
		}
	}
	
/**
Writes time stamp to repository file.
@param aTime Time stamp
@leave System-wide error codes
*/
void CIniFileOut::WriteTimeStampL(const TTime& aTime)
	{
	if(aTime.Int64() != 0)
		{
		WriteLineL(KTimeStampSection());
		TBuf<32> buf;
		buf.Num(aTime.Int64());
		buf.Append(KCrNl);
		WriteLineL(buf);
		}
	}
	
/**
Writes meta data to repository file.
@param aFileIn Input repository file
@leave System-wide error codes
*/
void CIniFileOut::WriteMetaDataL(TUint32 aDefaultMeta,
								 const RDefaultMetaArray& aDefaultMetaRanges)
	{
	if (!aDefaultMeta && !aDefaultMetaRanges.Count())
		{
		return;
		}

	WriteLineL(KDefaultMetaSection);

	if (aDefaultMeta)
		{
		iBuf.Format(KHexIntFormat, aDefaultMeta);
		WriteLineL(iBuf);
		}

	for (TInt i = 0; i<aDefaultMetaRanges.Count(); i++)
		{
		const TSettingsDefaultMeta& entry = aDefaultMetaRanges[i];
		if (entry.HighKey())
			{
			iBuf.Format(KRangeMetaFmt, entry.LowKey(), entry.HighKey(),
				entry.GetDefaultMetadata());
			}
		else
			{
			iBuf.Format(KMaskMetaFmt, entry.LowKey(), entry.KeyMask(),
				entry.GetDefaultMetadata());
			}
		WriteLineL(iBuf);
		}

	WriteLineL(KCrNl());
	}
	
/**
Writes platsec info to repository file.
@param aFileIn Input repository file
@leave System-wide error codes
*/
void CIniFileOut::WritePlatSecL(const TSecurityPolicy& aDefaultReadPolicy,
								const TSecurityPolicy& aDefaultWritePolicy,
								const RRangePolicyArray& aRangePolicies)
	{
	WriteLineL(KPlatSecSection);

	iBuf.Zero();
	AppendSecurityPolicyL(aDefaultReadPolicy, ECapReadAccess);
	iBuf.Append(KSpace);
	AppendSecurityPolicyL(aDefaultWritePolicy, ECapWriteAccess);
	WriteLineL(iBuf);

	for(TInt i=0; i < aRangePolicies.Count(); i++)
		{
		const TSettingsAccessPolicy& e = aRangePolicies[i];
		if (e.iHighKey != 0)
			{
			iBuf.Format(KRangePrefix, e.iLowKey, e.iHighKey);
			}
		else
			{
			iBuf.Format(KMaskPrefix, e.iLowKey, e.iKeyMask);
			}

		iBuf.Append(KSpace);

		AppendSecurityPolicyL(e.iReadAccessPolicy, ECapReadAccess);
		iBuf.Append(KSpace);
		AppendSecurityPolicyL(e.iWriteAccessPolicy, ECapWriteAccess);
		WriteLineL(iBuf);
		}

	WriteLineL(KCrNl());
	}

void CIniFileOut::AppendSecurityPolicyL(const TSecurityPolicy& aPolicy,
										TCapAccessMode aRdWrMode)
	{
	TCompiledSecurityPolicy policy(aPolicy);
	iBuf.Append(policy.TextualizePolicyL(aRdWrMode));
	}

//
const TDesC& TCompiledSecurityPolicy::TextualizePolicyL(TCapAccessMode aMode)
	{
	iBuf.Zero();
	AppendModeHeader(aMode,  EHdrCapability);

	switch (static_cast<TSecurityPolicy::TType>(iSecurityPolicy.iType))
		{
		case TSecurityPolicy::ETypeFail:
			iBuf.Append(KAccessAlwaysFail);
			break;
		case TSecurityPolicy::ETypePass:
			iBuf.Append(KAccessAlwaysPass);
			break;
		case TSecurityPolicy::ETypeC3:
			DoCapabilitySection(3);
			break;
		case TSecurityPolicy::ETypeC7:
			DoCapabilitySection(7);
			break;
		case TSecurityPolicy::ETypeS3:
			iBuf.Zero(); // erase the "cap_rd", replace with sid_rd
			AppendModeHeader(aMode,  EHdrSecureId);
			iBuf.AppendNum(iSecurityPolicy.iSecureId);

			if (ECapability_HardLimit != iSecurityPolicy.iCaps[0])
				{
				iBuf.Append(KSpace);
				AppendModeHeader(aMode,  EHdrCapability);
				DoCapabilitySection(3);
				}
			break;
			
		default:
			User::Leave(KErrCorrupt);
		} // switch

	return iBuf;
	}

TCapability TCompiledSecurityPolicy::CapabilityAt(TInt aIndex) const
	{
	if (aIndex < 3)
		{
		return static_cast <TCapability> (iSecurityPolicy.iCaps[aIndex]);
		}
	else if(aIndex < 7)
		{
		return static_cast <TCapability> (iSecurityPolicy.iExtraCaps[aIndex - 3]);
		}
		return ECapability_None;
	}

// 
void TCompiledSecurityPolicy::DoCapabilitySection(TInt aMaxNumCaps)
	{
	for (TInt i = 0; i < aMaxNumCaps; i++)
		{
		TCapability cap = CapabilityAt(i);

		if (ECapability_None == cap || ECapability_HardLimit == cap)
			{
			return;
			}
		if (i > 0)
			{
			iBuf.Append(',');
			}

		for (const char* p=CapabilityNames[cap]; *p; p++)
			{
			iBuf.Append((TUint16)*p);
			}
		} // for i
	}

void TCompiledSecurityPolicy::AppendModeHeader(TCapAccessMode aAccessMode, 
											   THeaderType aType)
	{
	if (aAccessMode == ECapReadAccess)
		{
		if (aType == EHdrSecureId)
			{
			iBuf.Append(KReadAccessSidString); // "sid_rd"
			}
		else
			{
			iBuf.Append(KReadAccessCapString); // "cap_rd"
			}
		}
	else
		{
		if (aType == EHdrSecureId)
			{
			iBuf.Append(KWriteAccessSidString); // "sid_wr"
			}
		else
			{
			iBuf.Append(KWriteAccessCapString); // "cap_wr"
			}
		}
	iBuf.Append('=');
	}

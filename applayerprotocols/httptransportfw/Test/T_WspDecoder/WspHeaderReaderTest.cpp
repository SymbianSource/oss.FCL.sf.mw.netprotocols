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
// Demonstrates a simple set of derived class implementations to
// test the CWspHeaderReader class using the test bed.
// It may be used as a basis to develop a full test bed dll.
// For support and comment please contact the authors.
// 
//

/**
 @file WspHeaderReaderTest.cpp
*/
#include "WspHeaderReaderUnitTest.h"
#include "ComponentTester.h"

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_ComponentTest
	@Depends CWspHeaderReader & CComponentTester 

	Comments : Test the CWspHeaderReader class.
 */
class CWspHeaderReader_ComponentTest : public CComponentTester
	{
public:
	/**
		@fn				NewLC(CDataLogger& aDataLogger,
							 MComponentTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this component test.
		@return			CWspHeaderReader_ComponentTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_ComponentTest is fully constructed.
	*/
	static CWspHeaderReader_ComponentTest* NewLC(CDataLogger& aDataLogger, 
							MComponentTestObserver& aObserver);

private:
	/**
		@fn				CWspHeaderReader_ComponentTest(CDataLogger& aDataLogger,
											MComponentTestObserver& aObserver)
		Intended Usage	: Standard c'tor method.
		Error Condition	: None.
		@since			7.0
		@param			aDataLogger The logging object.
		@param			aObserver The observer of this component test.
		@pre 			None.
		@post			CWspHeaderReader_ComponentTest is fully constructed.
	*/
	inline CWspHeaderReader_ComponentTest(CDataLogger& aDataLogger,
												MComponentTestObserver& aObserver);
	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
						to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory.
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_ComponentTest is fully constructed.
		@post			CWspHeaderReader_ComponentTest is fully initialized.
	*/
	inline void ConstructL();

	};	// CWspHeaderReader_ComponentTest

// ______________________________________________________________________________
//
inline CWspHeaderReader_ComponentTest* CWspHeaderReader_ComponentTest::NewLC(CDataLogger& aDataLogger, MComponentTestObserver& aObserver)
	{
	CWspHeaderReader_ComponentTest* self = new (ELeave) CWspHeaderReader_ComponentTest(aDataLogger, aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

inline CWspHeaderReader_ComponentTest::CWspHeaderReader_ComponentTest(CDataLogger& aDataLogger,
									MComponentTestObserver& aObserver)
: CComponentTester(aDataLogger, aObserver)
	{
	// Do nothing here.
	}

inline void CWspHeaderReader_ComponentTest::ConstructL()
	{
	// Perform base class initialization
	ComponentTesterConstructL();

	AddUnitTestL(CWspHeaderReader_CreateAndDestroy_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_ContentTypeShortToken_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_ContentTypeExtensionMedia_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_ContentTypeNull_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_ContentTypeWellKnownMediaInt_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_ContentTypeMediaTypeExtension_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_ContentTypeWith1Param_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_ContentTypeWith2Params_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_Date_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_LastModified_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_PragmaNoCache_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_PragmaTokenParam_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_PragmaTokenParamNoValue_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_VaryShortIntToken_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_VaryTokenText_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_WWWAuthenticateBasic_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_WWWAuthenticateBasicWithoutRealm_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_WWWAuthenticateAuthScheme_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_WWWAuthenticateAuthSchemeNoRealm_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_WWWAuthenticateAuthScheme2Params_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_WWWAuthenticateAuthSchemeNoRealm2Params_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_SetCookieShortIntVersion_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_SetCookieMajorVersionNullName1Param_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_SetCookieTextVersionNullValue_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_SetCookieNullVersion_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_ContentLocation_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_LocationWithoutQuote_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_EncodingVersionShortInt_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_EncodingVersionNullVersion_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_EncodingVersionTextVersion_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_EncodingVersionCodePage_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_EncodingVersionCodePageIntVersion_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_EncodingVersionCodePageTextVersion_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_EncodingVersionCodePageNullVersion_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_AcceptRangesNone_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_AcceptRangesBytes_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_AcceptRangesTokenText_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_ContentEncodingGZip_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_ContentEncodingCompress_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_ContentEncodingDeflate_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_ContentEncodingTokenText_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_ContentLanguageAnyLang_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_ContentLanguageShortInt_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_ContentLanguageLongInt_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_ContentLanguageTokenText_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_ContentLanguageNotExist_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_ContentMD5_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_ContentMD5TooShort_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_ProxyAuthenticateBasic_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_ServerText_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_ServerNullText_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_RetryAfterAbsoluteDate_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_RetryAfterRelativeTime_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_ViaText_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_ViaNullText_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_ViaTextWithoutTerminator_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_TrailerShortInt_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_TrailerNotExistToken_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_TrailerTokenText_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_ContentRangeUnknownLength_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_ContentRangeKnownLength_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_ContentRangeV1_2_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_XWapAppIdUriValue_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_XWapAppIdNullUri_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_XWapAppIdShortInt_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_XWapAppIdLongInt_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_XWapInitUriTextValue_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_XWapInitUriNullValue_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_PushFlagShortInt_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_PushFlagLongInt_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_PushFlagTextValue_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_AllowGet_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_AllowTrace_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_AllowLongInt_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_AllowInvalidShortInt_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_UpgradeText_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_UpgradeNullText_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_WarningShortInt_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_WarningFullValue_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_WarningShortWarnNullAgent_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_WarningShortIntNullText_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_WarningShortIntNullAgentNullText_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_ContentEncodingMultiHeader_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_ContentLanguageMultiHeader_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_ViaMultiHeader_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_TrailerMultiHeader_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_UpgradeMultiHeader_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_AllowMultiHeader_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_UnknownHeader_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_UnknownHeaderNull_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_MultiUnknownHeader_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_ProfileWarningWarnCode_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_ProfileWarningWarnCodeTextWarnTarget_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_ProfileWarningWarnCodeHostWarnTarget_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_ProfileWarningLongFullValue_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_ProfileWarningLongWarnCodeHostWarnTarget_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderReader_ProfileWarningLongWarnCodeHostWarnTargetDateValue_UnitTest::NewL(iDataLogger, *this));
	}

// ______________________________________________________________________________
//
EXPORT_C CComponentTester* NewComponentTestLC(CDataLogger& aDataLogger,
									MComponentTestObserver&	aComponentTestObserver)
	{
	return CWspHeaderReader_ComponentTest::NewLC(aDataLogger, aComponentTestObserver);
	}

// ___________________________________________________________________________
//
// This section of the module simply includes the exported test harness template which 
// makes this a "whole" CPP file with a E32Main entry point below. The test MMP 
// project file can then produce a EXE for the test project instead of a DLL.

#include "ECom\Test_Bed\TestHarnessTemplate.h"

GLDEF_C TInt E32Main()
    {
	return E32Main_TestHarness(NewComponentTestLC);
	}


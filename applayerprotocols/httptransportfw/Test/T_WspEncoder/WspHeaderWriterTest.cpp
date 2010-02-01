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
// test the CWspHeaderWriter class using the test bed.
// It may be used as a basis to develop a full test bed dll.
// For support and comment please contact the authors.
// 
//

/**
 @file WspHeaderWriterTest.cpp
*/
#include "WspHeaderWriterUnitTest.h"
#include "ComponentTester.h"

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_ComponentTest
	@Depends CWspHeaderWriter & CComponentTester 

	Comments : Test the CWspHeaderWriter class.
 */
class CWspHeaderWriter_ComponentTest : public CComponentTester
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
		@return			CWspHeaderWriter_ComponentTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_ComponentTest is fully constructed.
	*/
	static CWspHeaderWriter_ComponentTest* NewLC(CDataLogger& aDataLogger, 
							MComponentTestObserver& aObserver);

private:
	/**
		@fn				CWspHeaderWriter_ComponentTest(CDataLogger& aDataLogger,
											MComponentTestObserver& aObserver)
		Intended Usage	: Standard c'tor method.
		Error Condition	: None.
		@since			7.0
		@param			aDataLogger The logging object.
		@param			aObserver The observer of this component test.
		@pre 			None.
		@post			CWspHeaderWriter_ComponentTest is fully constructed.
	*/
	inline CWspHeaderWriter_ComponentTest(CDataLogger& aDataLogger,
												MComponentTestObserver& aObserver);
	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
						to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory.
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_ComponentTest is fully constructed.
		@post			CWspHeaderWriter_ComponentTest is fully initialized.
	*/
	inline void ConstructL();

	};	// CWspHeaderWriter_ComponentTest

// ______________________________________________________________________________
//
inline CWspHeaderWriter_ComponentTest* CWspHeaderWriter_ComponentTest::NewLC(CDataLogger& aDataLogger, MComponentTestObserver& aObserver)
	{
	CWspHeaderWriter_ComponentTest* self = new (ELeave) CWspHeaderWriter_ComponentTest(aDataLogger, aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

inline CWspHeaderWriter_ComponentTest::CWspHeaderWriter_ComponentTest(CDataLogger& aDataLogger,
									MComponentTestObserver& aObserver)
: CComponentTester(aDataLogger, aObserver)
	{
	// Do nothing here.
	}

inline void CWspHeaderWriter_ComponentTest::ConstructL()
	{
	// Perform base class initialization
	ComponentTesterConstructL();

	AddUnitTestL(CWspHeaderWriter_CreateAndDestroy_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_UserAgent_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_UserAgentNullText_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_AuthorizationBasic_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_AuthorizationBasicNullUser_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_BasicNullPassword_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_BasicNullUserNullPass_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_AuthorizationSchemeNoParam_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_AuthorizationInvalidSchemeToken_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_AuthorizationScheme2Params_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_AuthorizationScheme2ParamV1_2_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_ContentLocation_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_ContentLocationNullText_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_ContentTypeShortInt_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_ContentTypeConstrainedText_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_ContentTypeConstrainedNullText_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_ContentTypeGeneralShortInt1Param_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_ContentTypeGeneralLongInt_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_ContentTypeGeneralLongInt2Params_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_ContentTypeGeneralText_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_ContentTypeGeneralNullText_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_AcceptCharsetShortInt_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_AcceptCharsetText_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_AcceptCharsetAnyCharset_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_AcceptCharsetGeneralShortIntWithQVal_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_AcceptCharsetGeneralLongIntQVal_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_AcceptCharsetGeneralTextQVal_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_AcceptCharsetAnyCharsetV1_2_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_AcceptLanguageShortInt_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_AcceptLanguageAnyLang_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_AcceptLanguageGeneralShortQVal_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_ContentLanguageGeneralLongInt_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_AcceptLanguageGeneralTextQVal_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_AcceptLanguageText_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_ContentEncodingGZip_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_ContentEncodingCompress_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_ContentEncodingDeflate_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_ContentEncodingText_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_ContentLanguageShortInt_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_ContentLanguageLongInt_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_ContentLanguageText_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_ContentMD516ByteData_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_ContentMD531ByteData_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_Date_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_FromText_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_FromNullText_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_LastModified_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_RangeByteRangeNoLastPos_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_RangeByteRangeWithLastPos_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_RangeSuffixRange_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_RefererUri_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_RefererNullText_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_ViaUri_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_ViaNullText_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_XWapAppIdUri_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_XWapAppIdNullText_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_XWapAppIdShortInt_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_XWapAppIdLongInt_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_EncodingVersion_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_EncodingVersionCodePageInt_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_EncodingVersionCodePageIntWithVersion_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_Expect100Continue_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_ExpectExpressionTokenText_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_ExpectExpressionQuotedText_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_ExpectExpressionTokenTextParam_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_ExpectExpressionQuotedTextParam_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_ExpectExpression2Params_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_TETrailers_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_TEChunked_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_TEIdentitiyWithQValue_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_TEGzip_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_TECompressWithQValue_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_TETokenText_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_TETokenTextWithQValue_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_TrailerShortInt_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_Trailerv1_4NewShortInt_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_TrailerTokenText_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_AcceptEncodingGzip_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_AcceptEncodingCompress_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_AcceptEncodingDeflate_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_AcceptEncodingTokenText_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_AcceptEncodingGeneralAny_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_AcceptEncodingGeneralAnyQVal_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_AcceptEncodingGeneralGzipQVal_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_AcceptEncodingGeneralAnyQValVersion1_2_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_ContentRange_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_ContentRangeUnknownLength_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_AllowGet_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_AllowPost_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_AllowInvalid_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_PragmaNoCache_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_PragmaParameter_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_UpgradeText_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_UpgradeNullText_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_Warning110_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_Warning214_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_WarningWarnValue100_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_WarningWarnValue110NullAgent_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_AcceptConstrainedToken_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_AcceptConstrainedText_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_AcceptGeneralTokenQVal_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_AcceptGeneralLongInt2Params_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_AcceptGeneralNullToken3Params_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_CookieVersion_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_CookieVersionNameVal_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_CookieVersionNameNullValPath_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_CookieVaerionNameValNullPathDomain_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_AcceptMultiHeader_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_AcceptCharsetMultiHeader_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_RangeMultiHeader_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_TEMultiHeader_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_TrailerMultiHeader_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_ViaMultiHeader_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_AllowMultiHeader_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_ContentEncodingMultiHeader_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_UnknownHeader_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_UnknownNullHeader_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_MultiUnknownHeader_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_Profile_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_ProfileNullText_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_ProfileMalformed_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_ProfileDiffShort_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_ProfileDiffLong_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_ProfileDiffMultiHeader_UnitTest::NewL(iDataLogger, *this));
	// Test cases for Defect Fix HOL-595DR6
	AddUnitTestL(CWspHeaderWriter_DefectFix_HOL595DR6_1_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_DefectFix_HOL595DR6_2_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_DefectFix_HOL595DR6_3_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_DefectFix_HOL595DR6_4_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_DefectFix_HOL595DR6_5_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_DefectFix_HOL595DR6_6_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CWspHeaderWriter_DefectFix_HOL595DR6_7_UnitTest::NewL(iDataLogger, *this));
	}

// ______________________________________________________________________________
//
EXPORT_C CComponentTester* NewComponentTestLC(CDataLogger& aDataLogger,
									MComponentTestObserver&	aComponentTestObserver)
	{
	return CWspHeaderWriter_ComponentTest::NewLC(aDataLogger, aComponentTestObserver);
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

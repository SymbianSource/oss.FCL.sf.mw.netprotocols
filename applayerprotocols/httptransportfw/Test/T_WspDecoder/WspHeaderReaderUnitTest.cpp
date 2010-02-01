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
// The unit test class implementations for the CWspHeaderReader class.
// 
//

/**
 @file WspHeaderReaderUnitTest.cpp
*/

#include "WspHeaderReaderUnitTest.h"

// System includes
#include <wspstringconstants.h>

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderCreateAndDestroyUnitTest,"CWspHeaderReader_CreateAndDestroy_UnitTest");

CWspHeaderReader_CreateAndDestroy_UnitTest* CWspHeaderReader_CreateAndDestroy_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_CreateAndDestroy_UnitTest* self = 
					new(ELeave) CWspHeaderReader_CreateAndDestroy_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_CreateAndDestroy_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_CreateAndDestroy_UnitTest::~CWspHeaderReader_CreateAndDestroy_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_CreateAndDestroy_UnitTest::CWspHeaderReader_CreateAndDestroy_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderCreateAndDestroyUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_CreateAndDestroy_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EAccept);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderContentTypeShortTokenUnitTest,"CWspHeaderReader_ContentTypeShortToken_UnitTest");

CWspHeaderReader_ContentTypeShortToken_UnitTest* CWspHeaderReader_ContentTypeShortToken_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_ContentTypeShortToken_UnitTest* self = 
					new(ELeave) CWspHeaderReader_ContentTypeShortToken_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_ContentTypeShortToken_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iContentTypeShortTokenValidator;
	iContentTypeShortTokenValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_ContentTypeShortToken_UnitTest::~CWspHeaderReader_ContentTypeShortToken_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iContentTypeShortTokenValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_ContentTypeShortToken_UnitTest::CWspHeaderReader_ContentTypeShortToken_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderContentTypeShortTokenUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_ContentTypeShortToken_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EContentType);
	iUTContext->iTestHeader->BeginRawDataL();
	TUint8 octet81 = 0x81; // Should be token value for text/*
	iUTContext->iTestHeader->WriteRawDataL(octet81);
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "text/*");
	iUTContext->SetExpectedL(KTxtExpected());
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iContentTypeShortTokenValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iContentTypeShortTokenValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderContentTypeExtensionMediaUnitTest,"CWspHeaderReader_ContentTypeExtensionMedia_UnitTest");

CWspHeaderReader_ContentTypeExtensionMedia_UnitTest* CWspHeaderReader_ContentTypeExtensionMedia_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_ContentTypeExtensionMedia_UnitTest* self = 
					new(ELeave) CWspHeaderReader_ContentTypeExtensionMedia_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_ContentTypeExtensionMedia_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iContentTypeExtensionMediaValidator;
	iContentTypeExtensionMediaValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_ContentTypeExtensionMedia_UnitTest::~CWspHeaderReader_ContentTypeExtensionMedia_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iContentTypeExtensionMediaValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_ContentTypeExtensionMedia_UnitTest::CWspHeaderReader_ContentTypeExtensionMedia_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderContentTypeExtensionMediaUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_ContentTypeExtensionMedia_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EContentType);
	iUTContext->iTestHeader->BeginRawDataL();
	_LIT8(KTxtData, "extension/media\0");
	iUTContext->iTestHeader->WriteRawDataL(KTxtData());
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "extension/media");
	iUTContext->SetExpectedL(KTxtExpected());
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iContentTypeExtensionMediaValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iContentTypeExtensionMediaValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderContentTypeNullUnitTest,"CWspHeaderReader_ContentTypeNull_UnitTest");

CWspHeaderReader_ContentTypeNull_UnitTest* CWspHeaderReader_ContentTypeNull_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_ContentTypeNull_UnitTest* self = 
					new(ELeave) CWspHeaderReader_ContentTypeNull_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_ContentTypeNull_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iContentTypeNullValidator;
	iContentTypeNullValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_ContentTypeNull_UnitTest::~CWspHeaderReader_ContentTypeNull_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iContentTypeNullValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_ContentTypeNull_UnitTest::CWspHeaderReader_ContentTypeNull_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderContentTypeNullUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_ContentTypeNull_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EContentType);
	iUTContext->iTestHeader->BeginRawDataL();
	TUint8 octet00 = 0x00;
	iUTContext->iTestHeader->WriteRawDataL(octet00);
	iUTContext->iTestHeader->CommitRawData();
	iUTContext->SetExpectedL(KNullDesC8());
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iContentTypeNullValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iContentTypeNullValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderContentTypeWellKnownMediaIntUnitTest,"CWspHeaderReader_ContentTypeWellKnownMediaInt_UnitTest");

CWspHeaderReader_ContentTypeWellKnownMediaInt_UnitTest* CWspHeaderReader_ContentTypeWellKnownMediaInt_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_ContentTypeWellKnownMediaInt_UnitTest* self = 
					new(ELeave) CWspHeaderReader_ContentTypeWellKnownMediaInt_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_ContentTypeWellKnownMediaInt_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iContentTypeWellKnownMediaIntValidator;
	iContentTypeWellKnownMediaIntValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_ContentTypeWellKnownMediaInt_UnitTest::~CWspHeaderReader_ContentTypeWellKnownMediaInt_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iContentTypeWellKnownMediaIntValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_ContentTypeWellKnownMediaInt_UnitTest::CWspHeaderReader_ContentTypeWellKnownMediaInt_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderContentTypeWellKnownMediaIntUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_ContentTypeWellKnownMediaInt_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EContentType);
	iUTContext->iTestHeader->BeginRawDataL();
	TUint8 octet02 = 0x02;
	TUint8 octet03 = 0x03;
	TUint8 octet0B = 0x0B;
	iUTContext->iTestHeader->WriteRawDataL(octet03);
	iUTContext->iTestHeader->WriteRawDataL(octet02);
	iUTContext->iTestHeader->WriteRawDataL(octet02);
	iUTContext->iTestHeader->WriteRawDataL(octet0B);
	
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "application/vnd.nokia.syncset+wbxml");
	iUTContext->SetExpectedL(KTxtExpected());
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iContentTypeWellKnownMediaIntValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iContentTypeWellKnownMediaIntValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderContentTypeMediaTypeExtensionUnitTest,"CWspHeaderReader_ContentTypeMediaTypeExtension_UnitTest");

CWspHeaderReader_ContentTypeMediaTypeExtension_UnitTest* CWspHeaderReader_ContentTypeMediaTypeExtension_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_ContentTypeMediaTypeExtension_UnitTest* self = 
					new(ELeave) CWspHeaderReader_ContentTypeMediaTypeExtension_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_ContentTypeMediaTypeExtension_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iContentTypeMediaTypeExtensionValidator;
	iContentTypeMediaTypeExtensionValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_ContentTypeMediaTypeExtension_UnitTest::~CWspHeaderReader_ContentTypeMediaTypeExtension_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iContentTypeMediaTypeExtensionValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_ContentTypeMediaTypeExtension_UnitTest::CWspHeaderReader_ContentTypeMediaTypeExtension_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderContentTypeMediaTypeExtensionUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_ContentTypeMediaTypeExtension_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EContentType);
	iUTContext->iTestHeader->BeginRawDataL();
	
	TUint8 octet1F = 0x1F; // <Octet 31> for length quote
	TUint8 octet82 = 0x82;
	TUint8 octet2D = 0x2D;
	TUint8 octet00 = 0x00; // NULL terminator
	iUTContext->iTestHeader->WriteRawDataL(octet1F);
	iUTContext->iTestHeader->WriteRawDataL(octet82);
	iUTContext->iTestHeader->WriteRawDataL(octet2D);
	_LIT8(KTxt10ByteData, "tenbyteapp");
	for( TInt i=0; i<30; ++i )
		{
		iUTContext->iTestHeader->WriteRawDataL(KTxt10ByteData());
		}
	iUTContext->iTestHeader->WriteRawDataL(octet00);
	iUTContext->iTestHeader->CommitRawData();

	HBufC8* buf = HBufC8::NewL(300);
	CleanupStack::PushL(buf);
	TPtr8 appendBuf(buf->Des());
	for( TInt i=0; i<30; ++i )
		{
		appendBuf.Append(KTxt10ByteData());
		}
	iUTContext->SetExpectedL(*buf);
	CleanupStack::PopAndDestroy(buf);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iContentTypeMediaTypeExtensionValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iContentTypeMediaTypeExtensionValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderContentTypeWith1ParamUnitTest,"CWspHeaderReader_ContentTypeWith1Param_UnitTest");

CWspHeaderReader_ContentTypeWith1Param_UnitTest* CWspHeaderReader_ContentTypeWith1Param_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_ContentTypeWith1Param_UnitTest* self = 
					new(ELeave) CWspHeaderReader_ContentTypeWith1Param_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_ContentTypeWith1Param_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iContentTypeWith1ParamValidator;
	iContentTypeWith1ParamValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_ContentTypeWith1Param_UnitTest::~CWspHeaderReader_ContentTypeWith1Param_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iContentTypeWith1ParamValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_ContentTypeWith1Param_UnitTest::CWspHeaderReader_ContentTypeWith1Param_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderContentTypeWith1ParamUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_ContentTypeWith1Param_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EContentType);
	iUTContext->iTestHeader->BeginRawDataL();
	TUint8 octet11 = 0x11; // short int length of 17 bytes
	TUint8 octetA3 = 0xA3; // token for content type
	TUint8 octet97 = 0x97; // token for well-known param
	_LIT8(KTxtParamVal, "testparamvalue\0"); // param value

	iUTContext->iTestHeader->WriteRawDataL(octet11);
	iUTContext->iTestHeader->WriteRawDataL(octetA3);
	iUTContext->iTestHeader->WriteRawDataL(octet97);
	iUTContext->iTestHeader->WriteRawDataL(KTxtParamVal());
	iUTContext->iTestHeader->CommitRawData();

	_LIT8(KTxtExpected, "application/vnd.wap.multipart.mixed;Name=testparamvalue");
	iUTContext->SetExpectedL(KTxtExpected());
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iContentTypeWith1ParamValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iContentTypeWith1ParamValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderContentTypeWith2ParamsUnitTest,"CWspHeaderReader_ContentTypeWith2Params_UnitTest");

CWspHeaderReader_ContentTypeWith2Params_UnitTest* CWspHeaderReader_ContentTypeWith2Params_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_ContentTypeWith2Params_UnitTest* self = 
					new(ELeave) CWspHeaderReader_ContentTypeWith2Params_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_ContentTypeWith2Params_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iContentTypeWith2ParamsValidator;
	iContentTypeWith2ParamsValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_ContentTypeWith2Params_UnitTest::~CWspHeaderReader_ContentTypeWith2Params_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iContentTypeWith2ParamsValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_ContentTypeWith2Params_UnitTest::CWspHeaderReader_ContentTypeWith2Params_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderContentTypeWith2ParamsUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_ContentTypeWith2Params_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EContentType);
	iUTContext->iTestHeader->BeginRawDataL();
	TUint8 octet11 = 0x11; // short int length of 17 bytes
	TUint8 octetA3 = 0xA3; // token for content type
	TUint8 octet97 = 0x97; // token for well-known param Name
	TUint8 octet8E = 0x8E; // token for well-known param Max-age
	TUint8 octet03 = 0x03; // short length of 3
	TUint8 octet255 = 0xFF;
	TUint8 octet254 = 0xFE;
	
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtParamVal, "testparamvalue\0"); // param value

	iUTContext->iTestHeader->WriteRawDataL(octet11);
	iUTContext->iTestHeader->WriteRawDataL(octetA3);
	iUTContext->iTestHeader->WriteRawDataL(octet97);
	iUTContext->iTestHeader->WriteRawDataL(KTxtParamVal());
	iUTContext->iTestHeader->WriteRawDataL(octet8E);
	iUTContext->iTestHeader->WriteRawDataL(octet03);
	iUTContext->iTestHeader->WriteRawDataL(octet255);
	iUTContext->iTestHeader->WriteRawDataL(octet255);
	iUTContext->iTestHeader->WriteRawDataL(octet254);
	iUTContext->iTestHeader->CommitRawData();

	_LIT8(KTxtExpected, "application/vnd.wap.multipart.mixed;Name=testparamvalue;Max-age=16777214");
	iUTContext->SetExpectedL(KTxtExpected());
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iContentTypeWith2ParamsValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iContentTypeWith2ParamsValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderDateUnitTest,"CWspHeaderReader_Date_UnitTest");

CWspHeaderReader_Date_UnitTest* CWspHeaderReader_Date_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_Date_UnitTest* self = 
					new(ELeave) CWspHeaderReader_Date_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_Date_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iDateValidator;
	iDateValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_Date_UnitTest::~CWspHeaderReader_Date_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iDateValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_Date_UnitTest::CWspHeaderReader_Date_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderDateUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_Date_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EDate);
	iUTContext->iTestHeader->BeginRawDataL();
	TUint8 octet03 = 3;
	TUint8 octet255 = 0xFF;
	TUint8 octet254 = 0xFE;
	iUTContext->iTestHeader->WriteRawDataL(octet03);
	iUTContext->iTestHeader->WriteRawDataL(octet255);
	iUTContext->iTestHeader->WriteRawDataL(octet255);
	iUTContext->iTestHeader->WriteRawDataL(octet254);
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "Tue, 14 Jul 1970 04:20:14 GMT");
	iUTContext->SetExpectedL(KTxtExpected());
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iDateValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iDateValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}




// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderLastModifiedUnitTest,"CWspHeaderReader_LastModified_UnitTest");

CWspHeaderReader_LastModified_UnitTest* CWspHeaderReader_LastModified_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_LastModified_UnitTest* self = 
					new(ELeave) CWspHeaderReader_LastModified_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_LastModified_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iLastModifiedValidator;
	iLastModifiedValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_LastModified_UnitTest::~CWspHeaderReader_LastModified_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iLastModifiedValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_LastModified_UnitTest::CWspHeaderReader_LastModified_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderLastModifiedUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_LastModified_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::ELastModified);
	iUTContext->iTestHeader->BeginRawDataL();
	TUint8 octet03 = 3;
	TUint8 octet255 = 0xFF;
	TUint8 octet254 = 0xFE;
	iUTContext->iTestHeader->WriteRawDataL(octet03);
	iUTContext->iTestHeader->WriteRawDataL(octet255);
	iUTContext->iTestHeader->WriteRawDataL(octet255);
	iUTContext->iTestHeader->WriteRawDataL(octet254);
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "Tue, 14 Jul 1970 04:20:14 GMT");
	iUTContext->SetExpectedL(KTxtExpected());
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iLastModifiedValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iLastModifiedValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderPragmaNoCacheUnitTest,"CWspHeaderReader_PragmaNoCache_UnitTest");

CWspHeaderReader_PragmaNoCache_UnitTest* CWspHeaderReader_PragmaNoCache_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_PragmaNoCache_UnitTest* self = 
					new(ELeave) CWspHeaderReader_PragmaNoCache_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_PragmaNoCache_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iPragmaNoCacheValidator;
	iPragmaNoCacheValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_PragmaNoCache_UnitTest::~CWspHeaderReader_PragmaNoCache_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iPragmaNoCacheValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_PragmaNoCache_UnitTest::CWspHeaderReader_PragmaNoCache_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderPragmaNoCacheUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_PragmaNoCache_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EPragma);
	iUTContext->iTestHeader->BeginRawDataL();
	TUint8 octet80 = 0x80;
	iUTContext->iTestHeader->WriteRawDataL(octet80);
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "No-cache");
	iUTContext->SetExpectedL(KTxtExpected());
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iPragmaNoCacheValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iPragmaNoCacheValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderPragmaTokenParamUnitTest,"CWspHeaderReader_PragmaTokenParam_UnitTest");

CWspHeaderReader_PragmaTokenParam_UnitTest* CWspHeaderReader_PragmaTokenParam_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_PragmaTokenParam_UnitTest* self = 
					new(ELeave) CWspHeaderReader_PragmaTokenParam_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_PragmaTokenParam_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iPragmaTokenParamValidator;
	iPragmaTokenParamValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_PragmaTokenParam_UnitTest::~CWspHeaderReader_PragmaTokenParam_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iPragmaTokenParamValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_PragmaTokenParam_UnitTest::CWspHeaderReader_PragmaTokenParam_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderPragmaTokenParamUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_PragmaTokenParam_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EPragma);
	iUTContext->iTestHeader->BeginRawDataL();
	TUint8 octet18 = 0x18; // short int for the length of the data
	_LIT8(KTxtParamName, "ExampleParam\0");
	_LIT8(KTxtParamValue, "paramvalue\0");
	iUTContext->iTestHeader->WriteRawDataL(octet18);
	iUTContext->iTestHeader->WriteRawDataL(KTxtParamName());
	iUTContext->iTestHeader->WriteRawDataL(KTxtParamValue());
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, ";ExampleParam=paramvalue");
	iUTContext->SetExpectedL(KTxtExpected());
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iPragmaTokenParamValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iPragmaTokenParamValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderPragmaTokenParamNoValueUnitTest,"CWspHeaderReader_PragmaTokenParamNoValue_UnitTest");

CWspHeaderReader_PragmaTokenParamNoValue_UnitTest* CWspHeaderReader_PragmaTokenParamNoValue_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_PragmaTokenParamNoValue_UnitTest* self = 
					new(ELeave) CWspHeaderReader_PragmaTokenParamNoValue_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_PragmaTokenParamNoValue_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iPragmaTokenParamNoValueValidator;
	iPragmaTokenParamNoValueValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_PragmaTokenParamNoValue_UnitTest::~CWspHeaderReader_PragmaTokenParamNoValue_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iPragmaTokenParamNoValueValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_PragmaTokenParamNoValue_UnitTest::CWspHeaderReader_PragmaTokenParamNoValue_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderPragmaTokenParamNoValueUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_PragmaTokenParamNoValue_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EPragma);
	iUTContext->iTestHeader->BeginRawDataL();
	TUint8 octet0E = 0x0E; // short int for the length of the data
	_LIT8(KTxtParamName, "ExampleParam\0");
	TUint8 octet00 = 0x00; // null terminator
	iUTContext->iTestHeader->WriteRawDataL(octet0E);
	iUTContext->iTestHeader->WriteRawDataL(KTxtParamName());
	iUTContext->iTestHeader->WriteRawDataL(octet00);
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, ";ExampleParam");
	iUTContext->SetExpectedL(KTxtExpected());
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iPragmaTokenParamNoValueValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iPragmaTokenParamNoValueValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}


// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderVaryShortIntTokenUnitTest,"CWspHeaderReader_VaryShortIntToken_UnitTest");

CWspHeaderReader_VaryShortIntToken_UnitTest* CWspHeaderReader_VaryShortIntToken_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_VaryShortIntToken_UnitTest* self = 
					new(ELeave) CWspHeaderReader_VaryShortIntToken_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_VaryShortIntToken_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iVaryShortIntTokenValidator;
	iVaryShortIntTokenValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_VaryShortIntToken_UnitTest::~CWspHeaderReader_VaryShortIntToken_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iVaryShortIntTokenValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_VaryShortIntToken_UnitTest::CWspHeaderReader_VaryShortIntToken_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderVaryShortIntTokenUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_VaryShortIntToken_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EVary);
	iUTContext->iTestHeader->BeginRawDataL();
	TUint8 octet91 = 0x91;
	iUTContext->iTestHeader->WriteRawDataL(octet91);
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "Content-type");
	iUTContext->SetExpectedL(KTxtExpected());
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iVaryShortIntTokenValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iVaryShortIntTokenValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderVaryTokenTextUnitTest,"CWspHeaderReader_VaryTokenText_UnitTest");

CWspHeaderReader_VaryTokenText_UnitTest* CWspHeaderReader_VaryTokenText_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_VaryTokenText_UnitTest* self = 
					new(ELeave) CWspHeaderReader_VaryTokenText_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_VaryTokenText_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iVaryTokenTextValidator;
	iVaryTokenTextValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_VaryTokenText_UnitTest::~CWspHeaderReader_VaryTokenText_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iVaryTokenTextValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_VaryTokenText_UnitTest::CWspHeaderReader_VaryTokenText_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderVaryTokenTextUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_VaryTokenText_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EVary);
	iUTContext->iTestHeader->BeginRawDataL();
	_LIT8(KTxtTokenText, "Vary-Token-Text\0");
	iUTContext->iTestHeader->WriteRawDataL(KTxtTokenText());
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "Vary-Token-Text");
	iUTContext->SetExpectedL(KTxtExpected());
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iVaryTokenTextValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iVaryTokenTextValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderWWWAuthenticateBasicUnitTest,"CWspHeaderReader_WWWAuthenticateBasic_UnitTest");

CWspHeaderReader_WWWAuthenticateBasic_UnitTest* CWspHeaderReader_WWWAuthenticateBasic_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_WWWAuthenticateBasic_UnitTest* self = 
					new(ELeave) CWspHeaderReader_WWWAuthenticateBasic_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_WWWAuthenticateBasic_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iWWWAuthenticateBasicValidator;
	iWWWAuthenticateBasicValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_WWWAuthenticateBasic_UnitTest::~CWspHeaderReader_WWWAuthenticateBasic_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iWWWAuthenticateBasicValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_WWWAuthenticateBasic_UnitTest::CWspHeaderReader_WWWAuthenticateBasic_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderWWWAuthenticateBasicUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_WWWAuthenticateBasic_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EWWWAuthenticate);
	iUTContext->iTestHeader->BeginRawDataL();
	TUint8 octet0D = 0x0D; // short length octet for 13 bytes
	TUint8 octet80 = 0x80; // <octet 128> token for basic
	_LIT8(KTxtRealm, "Realm-value\0");
	iUTContext->iTestHeader->WriteRawDataL(octet0D);
	iUTContext->iTestHeader->WriteRawDataL(octet80);
	iUTContext->iTestHeader->WriteRawDataL(KTxtRealm());
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "Basic,Realm-value");
	iUTContext->SetExpectedL(KTxtExpected());
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iWWWAuthenticateBasicValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iWWWAuthenticateBasicValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderWWWAuthenticateBasicWithoutRealmUnitTest,"CWspHeaderReader_WWWAuthenticateBasicWithoutRealm_UnitTest");

CWspHeaderReader_WWWAuthenticateBasicWithoutRealm_UnitTest* CWspHeaderReader_WWWAuthenticateBasicWithoutRealm_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_WWWAuthenticateBasicWithoutRealm_UnitTest* self = 
					new(ELeave) CWspHeaderReader_WWWAuthenticateBasicWithoutRealm_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_WWWAuthenticateBasicWithoutRealm_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iWWWAuthenticateBasicWithoutRealmValidator;
	iWWWAuthenticateBasicWithoutRealmValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_WWWAuthenticateBasicWithoutRealm_UnitTest::~CWspHeaderReader_WWWAuthenticateBasicWithoutRealm_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iWWWAuthenticateBasicWithoutRealmValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_WWWAuthenticateBasicWithoutRealm_UnitTest::CWspHeaderReader_WWWAuthenticateBasicWithoutRealm_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderWWWAuthenticateBasicWithoutRealmUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_WWWAuthenticateBasicWithoutRealm_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EWWWAuthenticate);
	iUTContext->iTestHeader->BeginRawDataL();
	TUint8 octet02 = 0x02; // short length octet for 13 bytes
	TUint8 octet80 = 0x80; // <octet 128> token for basic
	TUint8 octet00 = 0x00; // <octet 0> for null terminate
	iUTContext->iTestHeader->WriteRawDataL(octet02);
	iUTContext->iTestHeader->WriteRawDataL(octet80);
	iUTContext->iTestHeader->WriteRawDataL(octet00);
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "Basic,");
	iUTContext->SetExpectedL(KTxtExpected());
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iWWWAuthenticateBasicWithoutRealmValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iWWWAuthenticateBasicWithoutRealmValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderWWWAuthenticateAuthSchemeUnitTest,"CWspHeaderReader_WWWAuthenticateAuthScheme_UnitTest");

CWspHeaderReader_WWWAuthenticateAuthScheme_UnitTest* CWspHeaderReader_WWWAuthenticateAuthScheme_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_WWWAuthenticateAuthScheme_UnitTest* self = 
					new(ELeave) CWspHeaderReader_WWWAuthenticateAuthScheme_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_WWWAuthenticateAuthScheme_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iWWWAuthenticateAuthSchemeValidator;
	iWWWAuthenticateAuthSchemeValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_WWWAuthenticateAuthScheme_UnitTest::~CWspHeaderReader_WWWAuthenticateAuthScheme_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iWWWAuthenticateAuthSchemeValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_WWWAuthenticateAuthScheme_UnitTest::CWspHeaderReader_WWWAuthenticateAuthScheme_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderWWWAuthenticateAuthSchemeUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_WWWAuthenticateAuthScheme_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EWWWAuthenticate);
	iUTContext->iTestHeader->BeginRawDataL();
	TUint8 octet1F = 0x1F; // length quote <octet 31>
	_LIT8(KTxtScheme, "Authenicate-scheme\0");
	_LIT8(KTxtRealm, "Realm-value\0");
	iUTContext->iTestHeader->WriteRawDataL(octet1F);
	iUTContext->iTestHeader->WriteRawDataL(octet1F); // 31 bytes uintvar-value
	iUTContext->iTestHeader->WriteRawDataL(KTxtScheme);
	iUTContext->iTestHeader->WriteRawDataL(KTxtRealm);
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "Authenicate-scheme,Realm-value");
	iUTContext->SetExpectedL(KTxtExpected());
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iWWWAuthenticateAuthSchemeValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iWWWAuthenticateAuthSchemeValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderWWWAuthenticateAuthSchemeNoRealmUnitTest,"CWspHeaderReader_WWWAuthenticateAuthSchemeNoRealm_UnitTest");

CWspHeaderReader_WWWAuthenticateAuthSchemeNoRealm_UnitTest* CWspHeaderReader_WWWAuthenticateAuthSchemeNoRealm_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_WWWAuthenticateAuthSchemeNoRealm_UnitTest* self = 
					new(ELeave) CWspHeaderReader_WWWAuthenticateAuthSchemeNoRealm_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_WWWAuthenticateAuthSchemeNoRealm_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iWWWAuthenticateAuthSchemeNoRealmValidator;
	iWWWAuthenticateAuthSchemeNoRealmValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_WWWAuthenticateAuthSchemeNoRealm_UnitTest::~CWspHeaderReader_WWWAuthenticateAuthSchemeNoRealm_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iWWWAuthenticateAuthSchemeNoRealmValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_WWWAuthenticateAuthSchemeNoRealm_UnitTest::CWspHeaderReader_WWWAuthenticateAuthSchemeNoRealm_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderWWWAuthenticateAuthSchemeNoRealmUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_WWWAuthenticateAuthSchemeNoRealm_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EWWWAuthenticate);
	iUTContext->iTestHeader->BeginRawDataL();
	TUint8 octet14 = 0x14; // short length of 20 bytes
	_LIT8(KTxtScheme, "Authenicate-scheme\0");
	TUint8 octet00 = 0x00; // null terminate <octet 0>
	iUTContext->iTestHeader->WriteRawDataL(octet14);
	iUTContext->iTestHeader->WriteRawDataL(KTxtScheme);
	iUTContext->iTestHeader->WriteRawDataL(octet00);
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "Authenicate-scheme,");
	iUTContext->SetExpectedL(KTxtExpected());
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iWWWAuthenticateAuthSchemeNoRealmValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iWWWAuthenticateAuthSchemeNoRealmValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderWWWAuthenticateAuthScheme2ParamsUnitTest,"CWspHeaderReader_WWWAuthenticateAuthScheme2Params_UnitTest");

CWspHeaderReader_WWWAuthenticateAuthScheme2Params_UnitTest* CWspHeaderReader_WWWAuthenticateAuthScheme2Params_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_WWWAuthenticateAuthScheme2Params_UnitTest* self = 
					new(ELeave) CWspHeaderReader_WWWAuthenticateAuthScheme2Params_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_WWWAuthenticateAuthScheme2Params_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iWWWAuthenticateAuthScheme2ParamsValidator;
	iWWWAuthenticateAuthScheme2ParamsValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_WWWAuthenticateAuthScheme2Params_UnitTest::~CWspHeaderReader_WWWAuthenticateAuthScheme2Params_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iWWWAuthenticateAuthScheme2ParamsValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_WWWAuthenticateAuthScheme2Params_UnitTest::CWspHeaderReader_WWWAuthenticateAuthScheme2Params_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderWWWAuthenticateAuthScheme2ParamsUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_WWWAuthenticateAuthScheme2Params_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EWWWAuthenticate);
	iUTContext->iTestHeader->BeginRawDataL();
	TUint8 octet1F = 0x1F; // length quote <octet 31>
	TUint8 octet3B = 0x3B; // uintvar of 59 bytes
	_LIT8(KTxtScheme, "Authenicate-scheme\0");
	_LIT8(KTxtRealm, "Realm-value\0");
	_LIT8(KTxtParam1Name, "Param1-name\0");
	_LIT8(KTxtParam1Val, "value\0");
	TUint8 octet9C = 0x9C;		// Token for domain
	_LIT8(KTxtDomain, "mydomain\0");
	iUTContext->iTestHeader->WriteRawDataL(octet1F);
	iUTContext->iTestHeader->WriteRawDataL(octet3B);
	iUTContext->iTestHeader->WriteRawDataL(KTxtScheme);
	iUTContext->iTestHeader->WriteRawDataL(KTxtRealm);
	iUTContext->iTestHeader->WriteRawDataL(KTxtParam1Name);
	iUTContext->iTestHeader->WriteRawDataL(KTxtParam1Val);
	iUTContext->iTestHeader->WriteRawDataL(octet9C);
	iUTContext->iTestHeader->WriteRawDataL(KTxtDomain);
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "Authenicate-scheme,Realm-value;Param1-name=value;Domain=mydomain");
	iUTContext->SetExpectedL(KTxtExpected());
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iWWWAuthenticateAuthScheme2ParamsValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iWWWAuthenticateAuthScheme2ParamsValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderWWWAuthenticateAuthSchemeNoRealm2ParamsUnitTest,"CWspHeaderReader_WWWAuthenticateAuthSchemeNoRealm2Params_UnitTest");

CWspHeaderReader_WWWAuthenticateAuthSchemeNoRealm2Params_UnitTest* CWspHeaderReader_WWWAuthenticateAuthSchemeNoRealm2Params_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_WWWAuthenticateAuthSchemeNoRealm2Params_UnitTest* self = 
					new(ELeave) CWspHeaderReader_WWWAuthenticateAuthSchemeNoRealm2Params_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_WWWAuthenticateAuthSchemeNoRealm2Params_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iWWWAuthenticateAuthSchemeNoRealm2ParamsValidator;
	iWWWAuthenticateAuthSchemeNoRealm2ParamsValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_WWWAuthenticateAuthSchemeNoRealm2Params_UnitTest::~CWspHeaderReader_WWWAuthenticateAuthSchemeNoRealm2Params_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iWWWAuthenticateAuthSchemeNoRealm2ParamsValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_WWWAuthenticateAuthSchemeNoRealm2Params_UnitTest::CWspHeaderReader_WWWAuthenticateAuthSchemeNoRealm2Params_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderWWWAuthenticateAuthSchemeNoRealm2ParamsUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_WWWAuthenticateAuthSchemeNoRealm2Params_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EWWWAuthenticate);
	iUTContext->iTestHeader->BeginRawDataL();
	TUint8 octet1F = 0x1F; // length quote <octet 31>
	TUint8 octet28 = 0x28; // uintvar of 40 bytes
	_LIT8(KTxtScheme, "Authenicate-scheme\0");
	TUint8 octet00 = 0x00; // null terminate
	_LIT8(KTxtParam1Name, "Param1-name\0");
	_LIT8(KTxtParam1Val, "value\0");
	TUint8 octet9C = 0x9C;		// Token for domain
	iUTContext->iTestHeader->WriteRawDataL(octet1F);
	iUTContext->iTestHeader->WriteRawDataL(octet28);
	iUTContext->iTestHeader->WriteRawDataL(KTxtScheme);
	iUTContext->iTestHeader->WriteRawDataL(octet00);
	iUTContext->iTestHeader->WriteRawDataL(KTxtParam1Name);
	iUTContext->iTestHeader->WriteRawDataL(KTxtParam1Val);
	iUTContext->iTestHeader->WriteRawDataL(octet9C);
	iUTContext->iTestHeader->WriteRawDataL(octet00);
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "Authenicate-scheme,;Param1-name=value;Domain");
	iUTContext->SetExpectedL(KTxtExpected());
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iWWWAuthenticateAuthSchemeNoRealm2ParamsValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iWWWAuthenticateAuthSchemeNoRealm2ParamsValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderSetCookieShortIntVersionUnitTest,"CWspHeaderReader_SetCookieShortIntVersion_UnitTest");

CWspHeaderReader_SetCookieShortIntVersion_UnitTest* CWspHeaderReader_SetCookieShortIntVersion_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_SetCookieShortIntVersion_UnitTest* self = 
					new(ELeave) CWspHeaderReader_SetCookieShortIntVersion_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_SetCookieShortIntVersion_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iSetCookieShortIntVersionValidator;
	iSetCookieShortIntVersionValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_SetCookieShortIntVersion_UnitTest::~CWspHeaderReader_SetCookieShortIntVersion_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iSetCookieShortIntVersionValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_SetCookieShortIntVersion_UnitTest::CWspHeaderReader_SetCookieShortIntVersion_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderSetCookieShortIntVersionUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_SetCookieShortIntVersion_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::ESetCookie);
	iUTContext->iTestHeader->BeginRawDataL();
	TUint8 octet18 = 0x18; // short value for 24 bytes
	TUint8 version = 0x91; // version 1.1
	_LIT8(KTxtCookieName, "My-cookie\0");
	_LIT8(KTxtCookieVal, "Cookie-value\0");
	iUTContext->iTestHeader->WriteRawDataL(octet18);
	iUTContext->iTestHeader->WriteRawDataL(version);
	iUTContext->iTestHeader->WriteRawDataL(KTxtCookieName);
	iUTContext->iTestHeader->WriteRawDataL(KTxtCookieVal);
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "1.1,My-cookie,Cookie-value");
	iUTContext->SetExpectedL(KTxtExpected());
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iSetCookieShortIntVersionValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iSetCookieShortIntVersionValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderSetCookieMajorVersionNullName1ParamUnitTest,"CWspHeaderReader_SetCookieMajorVersionNullName1Param_UnitTest");

CWspHeaderReader_SetCookieMajorVersionNullName1Param_UnitTest* CWspHeaderReader_SetCookieMajorVersionNullName1Param_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_SetCookieMajorVersionNullName1Param_UnitTest* self = 
					new(ELeave) CWspHeaderReader_SetCookieMajorVersionNullName1Param_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_SetCookieMajorVersionNullName1Param_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iSetCookieMajorVersionNullName1ParamValidator;
	iSetCookieMajorVersionNullName1ParamValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_SetCookieMajorVersionNullName1Param_UnitTest::~CWspHeaderReader_SetCookieMajorVersionNullName1Param_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iSetCookieMajorVersionNullName1ParamValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_SetCookieMajorVersionNullName1Param_UnitTest::CWspHeaderReader_SetCookieMajorVersionNullName1Param_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderSetCookieMajorVersionNullName1ParamUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_SetCookieMajorVersionNullName1Param_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::ESetCookie);
	iUTContext->iTestHeader->BeginRawDataL();
	TUint8 octet18 = 0x11; // short value for 17 bytes
	TUint8 version = 0x9F; // version 1
	TUint8 cookieName = 0x00;
	_LIT8(KTxtCookieVal, "Cookie-value\0");
	TUint8 paramToken = 0x87;
	TUint8 paramValue = 0xC7;
	iUTContext->iTestHeader->WriteRawDataL(octet18);
	iUTContext->iTestHeader->WriteRawDataL(version);
	iUTContext->iTestHeader->WriteRawDataL(cookieName);
	iUTContext->iTestHeader->WriteRawDataL(KTxtCookieVal);
	iUTContext->iTestHeader->WriteRawDataL(paramToken);
	iUTContext->iTestHeader->WriteRawDataL(paramValue);
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "1,,Cookie-value;Differences=Cache-control");
	iUTContext->SetExpectedL(KTxtExpected());
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iSetCookieMajorVersionNullName1ParamValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iSetCookieMajorVersionNullName1ParamValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderSetCookieTextVersionNullValueUnitTest,"CWspHeaderReader_SetCookieTextVersionNullValue_UnitTest");

CWspHeaderReader_SetCookieTextVersionNullValue_UnitTest* CWspHeaderReader_SetCookieTextVersionNullValue_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_SetCookieTextVersionNullValue_UnitTest* self = 
					new(ELeave) CWspHeaderReader_SetCookieTextVersionNullValue_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_SetCookieTextVersionNullValue_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iSetCookieTextVersionNullValueValidator;
	iSetCookieTextVersionNullValueValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_SetCookieTextVersionNullValue_UnitTest::~CWspHeaderReader_SetCookieTextVersionNullValue_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iSetCookieTextVersionNullValueValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_SetCookieTextVersionNullValue_UnitTest::CWspHeaderReader_SetCookieTextVersionNullValue_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderSetCookieTextVersionNullValueUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_SetCookieTextVersionNullValue_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::ESetCookie);
	iUTContext->iTestHeader->BeginRawDataL();
	TUint8 length = 0x12; // short value for 18 bytes
	_LIT8(KTxtVersion, "13.6\0"); // version
	_LIT8(KTxtCookieName, "My-cookie\0");
	TUint8 cookieVal = 0x00;
	TUint8 paramValue1 = 0x83;
	TUint8 paramValue2 = 0x31;
	iUTContext->iTestHeader->WriteRawDataL(length);
	iUTContext->iTestHeader->WriteRawDataL(KTxtVersion);
	iUTContext->iTestHeader->WriteRawDataL(KTxtCookieName);
	iUTContext->iTestHeader->WriteRawDataL(cookieVal);
	iUTContext->iTestHeader->WriteRawDataL(cookieVal);
	iUTContext->iTestHeader->WriteRawDataL(paramValue1);
	iUTContext->iTestHeader->WriteRawDataL(paramValue2);
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "13.6,My-cookie,;Q=0.333");
	iUTContext->SetExpectedL(KTxtExpected());
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iSetCookieTextVersionNullValueValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iSetCookieTextVersionNullValueValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderSetCookieNullVersionUnitTest,"CWspHeaderReader_SetCookieNullVersion_UnitTest");

CWspHeaderReader_SetCookieNullVersion_UnitTest* CWspHeaderReader_SetCookieNullVersion_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_SetCookieNullVersion_UnitTest* self = 
					new(ELeave) CWspHeaderReader_SetCookieNullVersion_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_SetCookieNullVersion_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iSetCookieNullVersionValidator;
	iSetCookieNullVersionValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_SetCookieNullVersion_UnitTest::~CWspHeaderReader_SetCookieNullVersion_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iSetCookieNullVersionValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_SetCookieNullVersion_UnitTest::CWspHeaderReader_SetCookieNullVersion_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderSetCookieNullVersionUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_SetCookieNullVersion_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::ESetCookie);
	iUTContext->iTestHeader->BeginRawDataL();
	TUint8 length = 0x1A; // short value for 26 bytes
	TUint8 version = 0x00;
	_LIT8(KTxtCookieName, "My-cookie\0");
	_LIT8(KTxtCookieValue, "cookie-value\0");
	TUint8 paramName = 0x82;
	TUint8 paramValue = 0x85;
	iUTContext->iTestHeader->WriteRawDataL(length);
	iUTContext->iTestHeader->WriteRawDataL(version);
	iUTContext->iTestHeader->WriteRawDataL(KTxtCookieName);
	iUTContext->iTestHeader->WriteRawDataL(KTxtCookieValue);
	iUTContext->iTestHeader->WriteRawDataL(paramName);
	iUTContext->iTestHeader->WriteRawDataL(paramValue);
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, ",My-cookie,cookie-value;Level=0.5");
	iUTContext->SetExpectedL(KTxtExpected());
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iSetCookieNullVersionValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iSetCookieNullVersionValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderContentLocationUnitTest,"CWspHeaderReader_ContentLocation_UnitTest");

CWspHeaderReader_ContentLocation_UnitTest* CWspHeaderReader_ContentLocation_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_ContentLocation_UnitTest* self = 
					new(ELeave) CWspHeaderReader_ContentLocation_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_ContentLocation_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iContentLocationValidator;
	iContentLocationValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_ContentLocation_UnitTest::~CWspHeaderReader_ContentLocation_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iContentLocationValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_ContentLocation_UnitTest::CWspHeaderReader_ContentLocation_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderContentLocationUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_ContentLocation_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EContentLocation);
	iUTContext->iTestHeader->BeginRawDataL();
	TUint8 wspQuote = 0x7F; // WSP quote char
	_LIT8(KTxtString, "http://www.symbian.com\0");
	iUTContext->iTestHeader->WriteRawDataL(wspQuote);
	iUTContext->iTestHeader->WriteRawDataL(KTxtString);
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "http://www.symbian.com");
	iUTContext->SetExpectedL(KTxtExpected);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iContentLocationValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iContentLocationValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderLocationWithoutQuoteUnitTest,"CWspHeaderReader_LocationWithoutQuote_UnitTest");

CWspHeaderReader_LocationWithoutQuote_UnitTest* CWspHeaderReader_LocationWithoutQuote_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_LocationWithoutQuote_UnitTest* self = 
					new(ELeave) CWspHeaderReader_LocationWithoutQuote_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_LocationWithoutQuote_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iLocationWithoutQuoteValidator;
	iLocationWithoutQuoteValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_LocationWithoutQuote_UnitTest::~CWspHeaderReader_LocationWithoutQuote_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iLocationWithoutQuoteValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_LocationWithoutQuote_UnitTest::CWspHeaderReader_LocationWithoutQuote_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderLocationWithoutQuoteUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_LocationWithoutQuote_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EContentLocation);
	iUTContext->iTestHeader->BeginRawDataL();
	_LIT8(KTxtString, "http://www.symbian.com\0");
	iUTContext->iTestHeader->WriteRawDataL(KTxtString);
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "http://www.symbian.com");
	iUTContext->SetExpectedL(KTxtExpected);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iLocationWithoutQuoteValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iLocationWithoutQuoteValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderEncodingVersionShortIntUnitTest,"CWspHeaderReader_EncodingVersionShortInt_UnitTest");

CWspHeaderReader_EncodingVersionShortInt_UnitTest* CWspHeaderReader_EncodingVersionShortInt_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_EncodingVersionShortInt_UnitTest* self = 
					new(ELeave) CWspHeaderReader_EncodingVersionShortInt_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_EncodingVersionShortInt_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iEncodingVersionShortIntValidator;
	iEncodingVersionShortIntValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_EncodingVersionShortInt_UnitTest::~CWspHeaderReader_EncodingVersionShortInt_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iEncodingVersionShortIntValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_EncodingVersionShortInt_UnitTest::CWspHeaderReader_EncodingVersionShortInt_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderEncodingVersionShortIntUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_EncodingVersionShortInt_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EEncodingVersion);
	iUTContext->iTestHeader->BeginRawDataL();
	TUint8 version = 0x91;
	iUTContext->iTestHeader->WriteRawDataL(version);
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "1.1");
	iUTContext->SetExpectedL(KTxtExpected);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iEncodingVersionShortIntValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iEncodingVersionShortIntValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderEncodingVersionNullVersionUnitTest,"CWspHeaderReader_EncodingVersionNullVersion_UnitTest");

CWspHeaderReader_EncodingVersionNullVersion_UnitTest* CWspHeaderReader_EncodingVersionNullVersion_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_EncodingVersionNullVersion_UnitTest* self = 
					new(ELeave) CWspHeaderReader_EncodingVersionNullVersion_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_EncodingVersionNullVersion_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iEncodingVersionNullVersionValidator;
	iEncodingVersionNullVersionValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_EncodingVersionNullVersion_UnitTest::~CWspHeaderReader_EncodingVersionNullVersion_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iEncodingVersionNullVersionValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_EncodingVersionNullVersion_UnitTest::CWspHeaderReader_EncodingVersionNullVersion_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderEncodingVersionNullVersionUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_EncodingVersionNullVersion_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EEncodingVersion);
	iUTContext->iTestHeader->BeginRawDataL();
	TUint8 version = 0x00;
	iUTContext->iTestHeader->WriteRawDataL(version);
	iUTContext->iTestHeader->CommitRawData();
	iUTContext->SetExpectedL(KNullDesC8());
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iEncodingVersionNullVersionValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iEncodingVersionNullVersionValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderEncodingVersionTextVersionUnitTest,"CWspHeaderReader_EncodingVersionTextVersion_UnitTest");

CWspHeaderReader_EncodingVersionTextVersion_UnitTest* CWspHeaderReader_EncodingVersionTextVersion_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_EncodingVersionTextVersion_UnitTest* self = 
					new(ELeave) CWspHeaderReader_EncodingVersionTextVersion_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_EncodingVersionTextVersion_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iEncodingVersionTextVersionValidator;
	iEncodingVersionTextVersionValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_EncodingVersionTextVersion_UnitTest::~CWspHeaderReader_EncodingVersionTextVersion_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iEncodingVersionTextVersionValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_EncodingVersionTextVersion_UnitTest::CWspHeaderReader_EncodingVersionTextVersion_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderEncodingVersionTextVersionUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_EncodingVersionTextVersion_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EEncodingVersion);
	iUTContext->iTestHeader->BeginRawDataL();
	_LIT8(KTxtVersion, "31.5\0");
	iUTContext->iTestHeader->WriteRawDataL(KTxtVersion);
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "31.5");
	iUTContext->SetExpectedL(KTxtExpected);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iEncodingVersionTextVersionValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iEncodingVersionTextVersionValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderEncodingVersionCodePageUnitTest,"CWspHeaderReader_EncodingVersionCodePage_UnitTest");

CWspHeaderReader_EncodingVersionCodePage_UnitTest* CWspHeaderReader_EncodingVersionCodePage_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_EncodingVersionCodePage_UnitTest* self = 
					new(ELeave) CWspHeaderReader_EncodingVersionCodePage_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_EncodingVersionCodePage_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iEncodingVersionCodePageValidator;
	iEncodingVersionCodePageValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_EncodingVersionCodePage_UnitTest::~CWspHeaderReader_EncodingVersionCodePage_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iEncodingVersionCodePageValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_EncodingVersionCodePage_UnitTest::CWspHeaderReader_EncodingVersionCodePage_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderEncodingVersionCodePageUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_EncodingVersionCodePage_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EEncodingVersion);
	iUTContext->iTestHeader->BeginRawDataL();
	TUint8 valLength = 0x01;
	TUint8 codePage = 0x81;
	iUTContext->iTestHeader->WriteRawDataL(valLength);
	iUTContext->iTestHeader->WriteRawDataL(codePage);
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "1");
	iUTContext->SetExpectedL(KTxtExpected);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iEncodingVersionCodePageValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iEncodingVersionCodePageValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderEncodingVersionCodePageIntVersionUnitTest,"CWspHeaderReader_EncodingVersionCodePageIntVersion_UnitTest");

CWspHeaderReader_EncodingVersionCodePageIntVersion_UnitTest* CWspHeaderReader_EncodingVersionCodePageIntVersion_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_EncodingVersionCodePageIntVersion_UnitTest* self = 
					new(ELeave) CWspHeaderReader_EncodingVersionCodePageIntVersion_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_EncodingVersionCodePageIntVersion_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iEncodingVersionCodePageIntVersionValidator;
	iEncodingVersionCodePageIntVersionValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_EncodingVersionCodePageIntVersion_UnitTest::~CWspHeaderReader_EncodingVersionCodePageIntVersion_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iEncodingVersionCodePageIntVersionValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_EncodingVersionCodePageIntVersion_UnitTest::CWspHeaderReader_EncodingVersionCodePageIntVersion_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderEncodingVersionCodePageIntVersionUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_EncodingVersionCodePageIntVersion_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EEncodingVersion);
	iUTContext->iTestHeader->BeginRawDataL();
	TUint8 valLength = 0x02;
	TUint8 codePage = 0x81;
	TUint8 version = 0x91;
	iUTContext->iTestHeader->WriteRawDataL(valLength);
	iUTContext->iTestHeader->WriteRawDataL(codePage);
	iUTContext->iTestHeader->WriteRawDataL(version);
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "1,1.1");
	iUTContext->SetExpectedL(KTxtExpected);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iEncodingVersionCodePageIntVersionValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iEncodingVersionCodePageIntVersionValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderEncodingVersionCodePageTextVersionUnitTest,"CWspHeaderReader_EncodingVersionCodePageTextVersion_UnitTest");

CWspHeaderReader_EncodingVersionCodePageTextVersion_UnitTest* CWspHeaderReader_EncodingVersionCodePageTextVersion_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_EncodingVersionCodePageTextVersion_UnitTest* self = 
					new(ELeave) CWspHeaderReader_EncodingVersionCodePageTextVersion_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_EncodingVersionCodePageTextVersion_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iEncodingVersionCodePageTextVersionValidator;
	iEncodingVersionCodePageTextVersionValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_EncodingVersionCodePageTextVersion_UnitTest::~CWspHeaderReader_EncodingVersionCodePageTextVersion_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iEncodingVersionCodePageTextVersionValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_EncodingVersionCodePageTextVersion_UnitTest::CWspHeaderReader_EncodingVersionCodePageTextVersion_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderEncodingVersionCodePageTextVersionUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_EncodingVersionCodePageTextVersion_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EEncodingVersion);
	iUTContext->iTestHeader->BeginRawDataL();
	TUint8 valLength = 0x06;
	TUint8 codePage = 0x81;
	_LIT8(KTxtVersion, "31.5\0");
	iUTContext->iTestHeader->WriteRawDataL(valLength);
	iUTContext->iTestHeader->WriteRawDataL(codePage);
	iUTContext->iTestHeader->WriteRawDataL(KTxtVersion);
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "1,31.5");
	iUTContext->SetExpectedL(KTxtExpected);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iEncodingVersionCodePageTextVersionValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iEncodingVersionCodePageTextVersionValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderEncodingVersionCodePageNullVersionUnitTest,"CWspHeaderReader_EncodingVersionCodePageNullVersion_UnitTest");

CWspHeaderReader_EncodingVersionCodePageNullVersion_UnitTest* CWspHeaderReader_EncodingVersionCodePageNullVersion_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_EncodingVersionCodePageNullVersion_UnitTest* self = 
					new(ELeave) CWspHeaderReader_EncodingVersionCodePageNullVersion_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_EncodingVersionCodePageNullVersion_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iEncodingVersionCodePageNullVersionValidator;
	iEncodingVersionCodePageNullVersionValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_EncodingVersionCodePageNullVersion_UnitTest::~CWspHeaderReader_EncodingVersionCodePageNullVersion_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iEncodingVersionCodePageNullVersionValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_EncodingVersionCodePageNullVersion_UnitTest::CWspHeaderReader_EncodingVersionCodePageNullVersion_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderEncodingVersionCodePageNullVersionUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_EncodingVersionCodePageNullVersion_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EEncodingVersion);
	iUTContext->iTestHeader->BeginRawDataL();
	TUint8 valLength = 0x06;
	TUint8 codePage = 0x81;
	TUint8 version = 0x00;
	iUTContext->iTestHeader->WriteRawDataL(valLength);
	iUTContext->iTestHeader->WriteRawDataL(codePage);
	iUTContext->iTestHeader->WriteRawDataL(version);
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "1,");
	iUTContext->SetExpectedL(KTxtExpected);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iEncodingVersionCodePageNullVersionValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iEncodingVersionCodePageNullVersionValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderAcceptRangesNoneUnitTest,"CWspHeaderReader_AcceptRangesNone_UnitTest");

CWspHeaderReader_AcceptRangesNone_UnitTest* CWspHeaderReader_AcceptRangesNone_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_AcceptRangesNone_UnitTest* self = 
					new(ELeave) CWspHeaderReader_AcceptRangesNone_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_AcceptRangesNone_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iAcceptRangesNoneValidator;
	iAcceptRangesNoneValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_AcceptRangesNone_UnitTest::~CWspHeaderReader_AcceptRangesNone_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iAcceptRangesNoneValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_AcceptRangesNone_UnitTest::CWspHeaderReader_AcceptRangesNone_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderAcceptRangesNoneUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_AcceptRangesNone_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EAcceptRanges);
	iUTContext->iTestHeader->BeginRawDataL();
	TUint8 noneToken = 0x80; // token value for 'none'
	iUTContext->iTestHeader->WriteRawDataL(noneToken);
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "None");
	iUTContext->SetExpectedL(KTxtExpected);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iAcceptRangesNoneValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iAcceptRangesNoneValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderAcceptRangesBytesUnitTest,"CWspHeaderReader_AcceptRangesBytes_UnitTest");

CWspHeaderReader_AcceptRangesBytes_UnitTest* CWspHeaderReader_AcceptRangesBytes_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_AcceptRangesBytes_UnitTest* self = 
					new(ELeave) CWspHeaderReader_AcceptRangesBytes_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_AcceptRangesBytes_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iAcceptRangesBytesValidator;
	iAcceptRangesBytesValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_AcceptRangesBytes_UnitTest::~CWspHeaderReader_AcceptRangesBytes_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iAcceptRangesBytesValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_AcceptRangesBytes_UnitTest::CWspHeaderReader_AcceptRangesBytes_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderAcceptRangesBytesUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_AcceptRangesBytes_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EAcceptRanges);
	iUTContext->iTestHeader->BeginRawDataL();
	TUint8 bytesToken = 0x81; // token value for 'bytes'
	iUTContext->iTestHeader->WriteRawDataL(bytesToken);
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "Bytes");
	iUTContext->SetExpectedL(KTxtExpected);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iAcceptRangesBytesValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iAcceptRangesBytesValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderAcceptRangesTokenTextUnitTest,"CWspHeaderReader_AcceptRangesTokenText_UnitTest");

CWspHeaderReader_AcceptRangesTokenText_UnitTest* CWspHeaderReader_AcceptRangesTokenText_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_AcceptRangesTokenText_UnitTest* self = 
					new(ELeave) CWspHeaderReader_AcceptRangesTokenText_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_AcceptRangesTokenText_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iAcceptRangesTokenTextValidator;
	iAcceptRangesTokenTextValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_AcceptRangesTokenText_UnitTest::~CWspHeaderReader_AcceptRangesTokenText_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iAcceptRangesTokenTextValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_AcceptRangesTokenText_UnitTest::CWspHeaderReader_AcceptRangesTokenText_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderAcceptRangesTokenTextUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_AcceptRangesTokenText_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EAcceptRanges);
	iUTContext->iTestHeader->BeginRawDataL();
	_LIT8(KTxtToken, "accept-ranges-token-text\0"); // token text
	iUTContext->iTestHeader->WriteRawDataL(KTxtToken);
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "accept-ranges-token-text");
	iUTContext->SetExpectedL(KTxtExpected);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iAcceptRangesTokenTextValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iAcceptRangesTokenTextValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderContentEncodingGZipUnitTest,"CWspHeaderReader_ContentEncodingGZip_UnitTest");

CWspHeaderReader_ContentEncodingGZip_UnitTest* CWspHeaderReader_ContentEncodingGZip_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_ContentEncodingGZip_UnitTest* self = 
					new(ELeave) CWspHeaderReader_ContentEncodingGZip_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_ContentEncodingGZip_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iContentEncodingGZipValidator;
	iContentEncodingGZipValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_ContentEncodingGZip_UnitTest::~CWspHeaderReader_ContentEncodingGZip_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iContentEncodingGZipValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_ContentEncodingGZip_UnitTest::CWspHeaderReader_ContentEncodingGZip_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderContentEncodingGZipUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_ContentEncodingGZip_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EContentEncoding);
	iUTContext->iTestHeader->BeginRawDataL();
	TUint8 gzipToken = 0x80;
	iUTContext->iTestHeader->WriteRawDataL(gzipToken);
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "GZip");
	iUTContext->SetExpectedL(KTxtExpected);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iContentEncodingGZipValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iContentEncodingGZipValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderContentEncodingCompressUnitTest,"CWspHeaderReader_ContentEncodingCompress_UnitTest");

CWspHeaderReader_ContentEncodingCompress_UnitTest* CWspHeaderReader_ContentEncodingCompress_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_ContentEncodingCompress_UnitTest* self = 
					new(ELeave) CWspHeaderReader_ContentEncodingCompress_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_ContentEncodingCompress_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iContentEncodingCompressValidator;
	iContentEncodingCompressValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_ContentEncodingCompress_UnitTest::~CWspHeaderReader_ContentEncodingCompress_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iContentEncodingCompressValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_ContentEncodingCompress_UnitTest::CWspHeaderReader_ContentEncodingCompress_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderContentEncodingCompressUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_ContentEncodingCompress_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EContentEncoding);
	iUTContext->iTestHeader->BeginRawDataL();
	TUint8 compressToken = 0x81;
	iUTContext->iTestHeader->WriteRawDataL(compressToken);
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "Compress");
	iUTContext->SetExpectedL(KTxtExpected);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iContentEncodingCompressValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iContentEncodingCompressValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderContentEncodingDeflateUnitTest,"CWspHeaderReader_ContentEncodingDeflate_UnitTest");

CWspHeaderReader_ContentEncodingDeflate_UnitTest* CWspHeaderReader_ContentEncodingDeflate_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_ContentEncodingDeflate_UnitTest* self = 
					new(ELeave) CWspHeaderReader_ContentEncodingDeflate_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_ContentEncodingDeflate_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iContentEncodingDeflateValidator;
	iContentEncodingDeflateValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_ContentEncodingDeflate_UnitTest::~CWspHeaderReader_ContentEncodingDeflate_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iContentEncodingDeflateValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_ContentEncodingDeflate_UnitTest::CWspHeaderReader_ContentEncodingDeflate_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderContentEncodingDeflateUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_ContentEncodingDeflate_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EContentEncoding);
	iUTContext->iTestHeader->BeginRawDataL();
	TUint8 deflateToken = 0x82;
	iUTContext->iTestHeader->WriteRawDataL(deflateToken);
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "Deflate");
	iUTContext->SetExpectedL(KTxtExpected);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iContentEncodingDeflateValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iContentEncodingDeflateValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderContentEncodingTokenTextUnitTest,"CWspHeaderReader_ContentEncodingTokenText_UnitTest");

CWspHeaderReader_ContentEncodingTokenText_UnitTest* CWspHeaderReader_ContentEncodingTokenText_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_ContentEncodingTokenText_UnitTest* self = 
					new(ELeave) CWspHeaderReader_ContentEncodingTokenText_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_ContentEncodingTokenText_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iContentEncodingTokenTextValidator;
	iContentEncodingTokenTextValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_ContentEncodingTokenText_UnitTest::~CWspHeaderReader_ContentEncodingTokenText_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iContentEncodingTokenTextValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_ContentEncodingTokenText_UnitTest::CWspHeaderReader_ContentEncodingTokenText_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderContentEncodingTokenTextUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_ContentEncodingTokenText_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EContentEncoding);
	iUTContext->iTestHeader->BeginRawDataL();
	_LIT8(KTxtTokenText, "token-text\0");
	iUTContext->iTestHeader->WriteRawDataL(KTxtTokenText);
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "Token-text");
	iUTContext->SetExpectedL(KTxtExpected);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iContentEncodingTokenTextValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iContentEncodingTokenTextValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderContentLanguageAnyLangUnitTest,"CWspHeaderReader_ContentLanguageAnyLang_UnitTest");

CWspHeaderReader_ContentLanguageAnyLang_UnitTest* CWspHeaderReader_ContentLanguageAnyLang_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_ContentLanguageAnyLang_UnitTest* self = 
					new(ELeave) CWspHeaderReader_ContentLanguageAnyLang_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_ContentLanguageAnyLang_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iContentLanguageAnyLangValidator;
	iContentLanguageAnyLangValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_ContentLanguageAnyLang_UnitTest::~CWspHeaderReader_ContentLanguageAnyLang_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iContentLanguageAnyLangValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_ContentLanguageAnyLang_UnitTest::CWspHeaderReader_ContentLanguageAnyLang_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderContentLanguageAnyLangUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_ContentLanguageAnyLang_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EContentLanguage);
	iUTContext->iTestHeader->BeginRawDataL();
	TUint8 anyLang = 0x80;
	iUTContext->iTestHeader->WriteRawDataL(anyLang);
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "*");
	iUTContext->SetExpectedL(KTxtExpected);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iContentLanguageAnyLangValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iContentLanguageAnyLangValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderContentLanguageShortIntUnitTest,"CWspHeaderReader_ContentLanguageShortInt_UnitTest");

CWspHeaderReader_ContentLanguageShortInt_UnitTest* CWspHeaderReader_ContentLanguageShortInt_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_ContentLanguageShortInt_UnitTest* self = 
					new(ELeave) CWspHeaderReader_ContentLanguageShortInt_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_ContentLanguageShortInt_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iContentLanguageShortIntValidator;
	iContentLanguageShortIntValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_ContentLanguageShortInt_UnitTest::~CWspHeaderReader_ContentLanguageShortInt_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iContentLanguageShortIntValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_ContentLanguageShortInt_UnitTest::CWspHeaderReader_ContentLanguageShortInt_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderContentLanguageShortIntUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_ContentLanguageShortInt_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EContentLanguage);
	iUTContext->iTestHeader->BeginRawDataL();
	TUint8 spanish = 0x9B;	// token for spanish 'es' language with top bit set
	iUTContext->iTestHeader->WriteRawDataL(spanish);
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "es");
	iUTContext->SetExpectedL(KTxtExpected);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iContentLanguageShortIntValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iContentLanguageShortIntValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderContentLanguageLongIntUnitTest,"CWspHeaderReader_ContentLanguageLongInt_UnitTest");

CWspHeaderReader_ContentLanguageLongInt_UnitTest* CWspHeaderReader_ContentLanguageLongInt_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_ContentLanguageLongInt_UnitTest* self = 
					new(ELeave) CWspHeaderReader_ContentLanguageLongInt_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_ContentLanguageLongInt_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iContentLanguageLongIntValidator;
	iContentLanguageLongIntValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_ContentLanguageLongInt_UnitTest::~CWspHeaderReader_ContentLanguageLongInt_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iContentLanguageLongIntValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_ContentLanguageLongInt_UnitTest::CWspHeaderReader_ContentLanguageLongInt_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderContentLanguageLongIntUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_ContentLanguageLongInt_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EContentLanguage);
	iUTContext->iTestHeader->BeginRawDataL();
	TUint8 length = 0x01;
	TUint8 greenlandic = 0x8A; // token for grrenlanic 'kl'
	iUTContext->iTestHeader->WriteRawDataL(length);
	iUTContext->iTestHeader->WriteRawDataL(greenlandic);
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "kl");
	iUTContext->SetExpectedL(KTxtExpected);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iContentLanguageLongIntValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iContentLanguageLongIntValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderContentLanguageTokenTextUnitTest,"CWspHeaderReader_ContentLanguageTokenText_UnitTest");

CWspHeaderReader_ContentLanguageTokenText_UnitTest* CWspHeaderReader_ContentLanguageTokenText_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_ContentLanguageTokenText_UnitTest* self = 
					new(ELeave) CWspHeaderReader_ContentLanguageTokenText_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_ContentLanguageTokenText_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iContentLanguageTokenTextValidator;
	iContentLanguageTokenTextValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_ContentLanguageTokenText_UnitTest::~CWspHeaderReader_ContentLanguageTokenText_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iContentLanguageTokenTextValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_ContentLanguageTokenText_UnitTest::CWspHeaderReader_ContentLanguageTokenText_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderContentLanguageTokenTextUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_ContentLanguageTokenText_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EContentLanguage);
	iUTContext->iTestHeader->BeginRawDataL();
	_LIT8(KTxtTokenText, "language-in-text\0");
	iUTContext->iTestHeader->WriteRawDataL(KTxtTokenText);
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "language-in-text");
	iUTContext->SetExpectedL(KTxtExpected);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iContentLanguageTokenTextValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iContentLanguageTokenTextValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderContentLanguageNotExistUnitTest,"CWspHeaderReader_ContentLanguageNotExist_UnitTest");

CWspHeaderReader_ContentLanguageNotExist_UnitTest* CWspHeaderReader_ContentLanguageNotExist_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_ContentLanguageNotExist_UnitTest* self = 
					new(ELeave) CWspHeaderReader_ContentLanguageNotExist_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_ContentLanguageNotExist_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iContentLanguageNotExistValidator;
	iContentLanguageNotExistValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_ContentLanguageNotExist_UnitTest::~CWspHeaderReader_ContentLanguageNotExist_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iContentLanguageNotExistValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_ContentLanguageNotExist_UnitTest::CWspHeaderReader_ContentLanguageNotExist_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderContentLanguageNotExistUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_ContentLanguageNotExist_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EContentLanguage);
	TUint8 length = 0x02;
	TUint8 notExist1 = 0x8A;
	TUint8 notExist2 = 0x81;
	iUTContext->iTestHeader->BeginRawDataL();
	iUTContext->iTestHeader->WriteRawDataL(length);
	iUTContext->iTestHeader->WriteRawDataL(notExist1);
	iUTContext->iTestHeader->WriteRawDataL(notExist2);
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "-20");
	iUTContext->SetExpectedL(KTxtExpected);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iContentLanguageNotExistValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderLTrap_Transition(*iUTContext,*iContentLanguageNotExistValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderContentMD5UnitTest,"CWspHeaderReader_ContentMD5_UnitTest");

CWspHeaderReader_ContentMD5_UnitTest* CWspHeaderReader_ContentMD5_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_ContentMD5_UnitTest* self = 
					new(ELeave) CWspHeaderReader_ContentMD5_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_ContentMD5_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iContentMD5Validator;
	iContentMD5Validator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_ContentMD5_UnitTest::~CWspHeaderReader_ContentMD5_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iContentMD5Validator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_ContentMD5_UnitTest::CWspHeaderReader_ContentMD5_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderContentMD5UnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_ContentMD5_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EContentMD5);
	TUint8 length = 0x10;
	iUTContext->iTestHeader->BeginRawDataL();
	_LIT8(KTxtMD5Data, "ThisIsContentMD5");
	iUTContext->iTestHeader->WriteRawDataL(length);
	iUTContext->iTestHeader->WriteRawDataL(KTxtMD5Data);
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "ThisIsContentMD5");
	iUTContext->SetExpectedL(KTxtExpected);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iContentMD5Validator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iContentMD5Validator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderContentMD5TooShortUnitTest,"CWspHeaderReader_ContentMD5TooShort_UnitTest");

CWspHeaderReader_ContentMD5TooShort_UnitTest* CWspHeaderReader_ContentMD5TooShort_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_ContentMD5TooShort_UnitTest* self = 
					new(ELeave) CWspHeaderReader_ContentMD5TooShort_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_ContentMD5TooShort_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iContentMD5TooShortValidator;
	iContentMD5TooShortValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_ContentMD5TooShort_UnitTest::~CWspHeaderReader_ContentMD5TooShort_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iContentMD5TooShortValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_ContentMD5TooShort_UnitTest::CWspHeaderReader_ContentMD5TooShort_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderContentMD5TooShortUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_ContentMD5TooShort_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EContentMD5);
	TUint8 length = 0x10; // length of 16, but actual data is less
	iUTContext->iTestHeader->BeginRawDataL();
	_LIT8(KTxtMD5Data, "ThisIsTooShort");
	iUTContext->iTestHeader->WriteRawDataL(length);
	iUTContext->iTestHeader->WriteRawDataL(KTxtMD5Data);
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "-20");
	iUTContext->SetExpectedL(KTxtExpected);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iContentMD5TooShortValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderLTrap_Transition(*iUTContext,*iContentMD5TooShortValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderProxyAuthenticateBasicUnitTest,"CWspHeaderReader_ProxyAuthenticateBasic_UnitTest");

CWspHeaderReader_ProxyAuthenticateBasic_UnitTest* CWspHeaderReader_ProxyAuthenticateBasic_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_ProxyAuthenticateBasic_UnitTest* self = 
					new(ELeave) CWspHeaderReader_ProxyAuthenticateBasic_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_ProxyAuthenticateBasic_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iProxyAuthenticateBasicValidator;
	iProxyAuthenticateBasicValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_ProxyAuthenticateBasic_UnitTest::~CWspHeaderReader_ProxyAuthenticateBasic_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iProxyAuthenticateBasicValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_ProxyAuthenticateBasic_UnitTest::CWspHeaderReader_ProxyAuthenticateBasic_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderProxyAuthenticateBasicUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_ProxyAuthenticateBasic_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EProxyAuthenticate);
	iUTContext->iTestHeader->BeginRawDataL();
	TUint8 octet0D = 0x0D; // short length octet for 13 bytes
	TUint8 octet80 = 0x80; // <octet 128> token for basic
	_LIT8(KTxtRealm, "Realm-value\0");
	iUTContext->iTestHeader->WriteRawDataL(octet0D);
	iUTContext->iTestHeader->WriteRawDataL(octet80);
	iUTContext->iTestHeader->WriteRawDataL(KTxtRealm());
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "Basic,Realm-value");
	iUTContext->SetExpectedL(KTxtExpected());
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iProxyAuthenticateBasicValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iProxyAuthenticateBasicValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderServerTextUnitTest,"CWspHeaderReader_ServerText_UnitTest");

CWspHeaderReader_ServerText_UnitTest* CWspHeaderReader_ServerText_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_ServerText_UnitTest* self = 
					new(ELeave) CWspHeaderReader_ServerText_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_ServerText_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iServerTextValidator;
	iServerTextValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_ServerText_UnitTest::~CWspHeaderReader_ServerText_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iServerTextValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_ServerText_UnitTest::CWspHeaderReader_ServerText_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderServerTextUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_ServerText_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EServer);
	iUTContext->iTestHeader->BeginRawDataL();
	_LIT8(KTxtText, "www.myservervalue.com\0");
	iUTContext->iTestHeader->WriteRawDataL(KTxtText);
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "www.myservervalue.com");
	iUTContext->SetExpectedL(KTxtExpected());
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iServerTextValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iServerTextValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderServerNullTextUnitTest,"CWspHeaderReader_ServerNullText_UnitTest");

CWspHeaderReader_ServerNullText_UnitTest* CWspHeaderReader_ServerNullText_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_ServerNullText_UnitTest* self = 
					new(ELeave) CWspHeaderReader_ServerNullText_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_ServerNullText_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iServerNullTextValidator;
	iServerNullTextValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_ServerNullText_UnitTest::~CWspHeaderReader_ServerNullText_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iServerNullTextValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_ServerNullText_UnitTest::CWspHeaderReader_ServerNullText_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderServerNullTextUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_ServerNullText_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EServer);
	iUTContext->iTestHeader->BeginRawDataL();
	TUint8 nullString = 0x00;
	iUTContext->iTestHeader->WriteRawDataL(nullString);
	iUTContext->iTestHeader->CommitRawData();
	iUTContext->SetExpectedL(KNullDesC8());
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iServerNullTextValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iServerNullTextValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderRetryAfterAbsoluteDateUnitTest,"CWspHeaderReader_RetryAfterAbsoluteDate_UnitTest");

CWspHeaderReader_RetryAfterAbsoluteDate_UnitTest* CWspHeaderReader_RetryAfterAbsoluteDate_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_RetryAfterAbsoluteDate_UnitTest* self = 
					new(ELeave) CWspHeaderReader_RetryAfterAbsoluteDate_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_RetryAfterAbsoluteDate_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iRetryAfterAbsoluteDateValidator;
	iRetryAfterAbsoluteDateValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_RetryAfterAbsoluteDate_UnitTest::~CWspHeaderReader_RetryAfterAbsoluteDate_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iRetryAfterAbsoluteDateValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_RetryAfterAbsoluteDate_UnitTest::CWspHeaderReader_RetryAfterAbsoluteDate_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderRetryAfterAbsoluteDateUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_RetryAfterAbsoluteDate_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::ERetryAfter);
	iUTContext->iTestHeader->BeginRawDataL();
	TUint8 valueLength = 0x05; // short int value length
	TUint8 absoluteToken = 0x80; // token for absolute value
	TUint8 length = 0x03; // short int length for date
	TUint8 octet255 = 0xFF; // example date value
	TUint8 octet254 = 0xFE; // example date value
	iUTContext->iTestHeader->WriteRawDataL(valueLength);
	iUTContext->iTestHeader->WriteRawDataL(absoluteToken);
	iUTContext->iTestHeader->WriteRawDataL(length);
	iUTContext->iTestHeader->WriteRawDataL(octet255);
	iUTContext->iTestHeader->WriteRawDataL(octet255);
	iUTContext->iTestHeader->WriteRawDataL(octet254);
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "Tue, 14 Jul 1970 04:20:14 GMT");
	iUTContext->SetExpectedL(KTxtExpected);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iRetryAfterAbsoluteDateValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iRetryAfterAbsoluteDateValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderRetryAfterRelativeTimeUnitTest,"CWspHeaderReader_RetryAfterRelativeTime_UnitTest");

CWspHeaderReader_RetryAfterRelativeTime_UnitTest* CWspHeaderReader_RetryAfterRelativeTime_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_RetryAfterRelativeTime_UnitTest* self = 
					new(ELeave) CWspHeaderReader_RetryAfterRelativeTime_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_RetryAfterRelativeTime_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iRetryAfterRelativeTimeValidator;
	iRetryAfterRelativeTimeValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_RetryAfterRelativeTime_UnitTest::~CWspHeaderReader_RetryAfterRelativeTime_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iRetryAfterRelativeTimeValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_RetryAfterRelativeTime_UnitTest::CWspHeaderReader_RetryAfterRelativeTime_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderRetryAfterRelativeTimeUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_RetryAfterRelativeTime_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::ERetryAfter);
	iUTContext->iTestHeader->BeginRawDataL();
	TUint8 valueLength = 0x05; // short int value length
	TUint8 relativeToken = 0x81; // token for relative value
	TUint8 length = 0x03; // short int length for date
	TUint8 octet255 = 0xFF; // example date value
	TUint8 octet254 = 0xFE; // example date value
	iUTContext->iTestHeader->WriteRawDataL(valueLength);
	iUTContext->iTestHeader->WriteRawDataL(relativeToken);
	iUTContext->iTestHeader->WriteRawDataL(length);
	iUTContext->iTestHeader->WriteRawDataL(octet255);
	iUTContext->iTestHeader->WriteRawDataL(octet255);
	iUTContext->iTestHeader->WriteRawDataL(octet254);
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "16777214");
	iUTContext->SetExpectedL(KTxtExpected);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iRetryAfterRelativeTimeValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iRetryAfterRelativeTimeValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderViaTextUnitTest,"CWspHeaderReader_ViaText_UnitTest");

CWspHeaderReader_ViaText_UnitTest* CWspHeaderReader_ViaText_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_ViaText_UnitTest* self = 
					new(ELeave) CWspHeaderReader_ViaText_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_ViaText_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iViaTextValidator;
	iViaTextValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_ViaText_UnitTest::~CWspHeaderReader_ViaText_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iViaTextValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_ViaText_UnitTest::CWspHeaderReader_ViaText_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderViaTextUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_ViaText_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EVia);
	iUTContext->iTestHeader->BeginRawDataL();
	_LIT8(KTxtString, "Via-text\0");
	iUTContext->iTestHeader->WriteRawDataL(KTxtString);
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "Via-text");
	iUTContext->SetExpectedL(KTxtExpected);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iViaTextValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iViaTextValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderViaNullTextUnitTest,"CWspHeaderReader_ViaNullText_UnitTest");

CWspHeaderReader_ViaNullText_UnitTest* CWspHeaderReader_ViaNullText_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_ViaNullText_UnitTest* self = 
					new(ELeave) CWspHeaderReader_ViaNullText_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_ViaNullText_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iViaNullTextValidator;
	iViaNullTextValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_ViaNullText_UnitTest::~CWspHeaderReader_ViaNullText_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iViaNullTextValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_ViaNullText_UnitTest::CWspHeaderReader_ViaNullText_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderViaNullTextUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_ViaNullText_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EVia);
	iUTContext->iTestHeader->BeginRawDataL();
	TUint8 nullTerminate = 0x00;
	iUTContext->iTestHeader->WriteRawDataL(nullTerminate);
	iUTContext->iTestHeader->CommitRawData();
	iUTContext->SetExpectedL(KNullDesC8());
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iViaNullTextValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iViaNullTextValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderViaTextWithoutTerminatorUnitTest,"CWspHeaderReader_ViaTextWithoutTerminator_UnitTest");

CWspHeaderReader_ViaTextWithoutTerminator_UnitTest* CWspHeaderReader_ViaTextWithoutTerminator_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_ViaTextWithoutTerminator_UnitTest* self = 
					new(ELeave) CWspHeaderReader_ViaTextWithoutTerminator_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_ViaTextWithoutTerminator_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iViaTextWithoutTerminatorValidator;
	iViaTextWithoutTerminatorValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_ViaTextWithoutTerminator_UnitTest::~CWspHeaderReader_ViaTextWithoutTerminator_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iViaTextWithoutTerminatorValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_ViaTextWithoutTerminator_UnitTest::CWspHeaderReader_ViaTextWithoutTerminator_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderViaTextWithoutTerminatorUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_ViaTextWithoutTerminator_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EVia);
	iUTContext->iTestHeader->BeginRawDataL();
	_LIT8(KTxtStringNoNull, "Via-text");
	iUTContext->iTestHeader->WriteRawDataL(KTxtStringNoNull);
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "-20");
	iUTContext->SetExpectedL(KTxtExpected);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iViaTextWithoutTerminatorValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderLTrap_Transition(*iUTContext,*iViaTextWithoutTerminatorValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderTrailerShortIntUnitTest,"CWspHeaderReader_TrailerShortInt_UnitTest");

CWspHeaderReader_TrailerShortInt_UnitTest* CWspHeaderReader_TrailerShortInt_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_TrailerShortInt_UnitTest* self = 
					new(ELeave) CWspHeaderReader_TrailerShortInt_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_TrailerShortInt_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iTrailerShortIntValidator;
	iTrailerShortIntValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_TrailerShortInt_UnitTest::~CWspHeaderReader_TrailerShortInt_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iTrailerShortIntValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_TrailerShortInt_UnitTest::CWspHeaderReader_TrailerShortInt_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderTrailerShortIntUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_TrailerShortInt_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::ETrailer);
	iUTContext->iTestHeader->BeginRawDataL();
	TUint8 headerToken = 0xA7; // with top bit set
	iUTContext->iTestHeader->WriteRawDataL(headerToken);
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "Transfer-Encoding");
	iUTContext->SetExpectedL(KTxtExpected);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iTrailerShortIntValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iTrailerShortIntValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderTrailerNotExistTokenUnitTest,"CWspHeaderReader_TrailerNotExistToken_UnitTest");

CWspHeaderReader_TrailerNotExistToken_UnitTest* CWspHeaderReader_TrailerNotExistToken_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_TrailerNotExistToken_UnitTest* self = 
					new(ELeave) CWspHeaderReader_TrailerNotExistToken_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_TrailerNotExistToken_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iTrailerNotExistTokenValidator;
	iTrailerNotExistTokenValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_TrailerNotExistToken_UnitTest::~CWspHeaderReader_TrailerNotExistToken_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iTrailerNotExistTokenValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_TrailerNotExistToken_UnitTest::CWspHeaderReader_TrailerNotExistToken_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderTrailerNotExistTokenUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_TrailerNotExistToken_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::ETrailer);
	iUTContext->iTestHeader->BeginRawDataL();
	TUint8 headerToken = 0x50;
	iUTContext->iTestHeader->WriteRawDataL(headerToken);
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "-20");
	iUTContext->SetExpectedL(KTxtExpected);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iTrailerNotExistTokenValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderLTrap_Transition(*iUTContext,*iTrailerNotExistTokenValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderTrailerTokenTextUnitTest,"CWspHeaderReader_TrailerTokenText_UnitTest");

CWspHeaderReader_TrailerTokenText_UnitTest* CWspHeaderReader_TrailerTokenText_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_TrailerTokenText_UnitTest* self = 
					new(ELeave) CWspHeaderReader_TrailerTokenText_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_TrailerTokenText_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iTrailerTokenTextValidator;
	iTrailerTokenTextValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_TrailerTokenText_UnitTest::~CWspHeaderReader_TrailerTokenText_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iTrailerTokenTextValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_TrailerTokenText_UnitTest::CWspHeaderReader_TrailerTokenText_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderTrailerTokenTextUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_TrailerTokenText_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::ETrailer);
	iUTContext->iTestHeader->BeginRawDataL();
	_LIT8(KTxtTokenText, "text-header-name\0");
	iUTContext->iTestHeader->WriteRawDataL(KTxtTokenText);
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "text-header-name");
	iUTContext->SetExpectedL(KTxtExpected);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iTrailerTokenTextValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iTrailerTokenTextValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderContentRangeUnknownLengthUnitTest,"CWspHeaderReader_ContentRangeUnknownLength_UnitTest");

CWspHeaderReader_ContentRangeUnknownLength_UnitTest* CWspHeaderReader_ContentRangeUnknownLength_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_ContentRangeUnknownLength_UnitTest* self = 
					new(ELeave) CWspHeaderReader_ContentRangeUnknownLength_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_ContentRangeUnknownLength_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iContentRangeUnknownLengthValidator;
	iContentRangeUnknownLengthValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_ContentRangeUnknownLength_UnitTest::~CWspHeaderReader_ContentRangeUnknownLength_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iContentRangeUnknownLengthValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_ContentRangeUnknownLength_UnitTest::CWspHeaderReader_ContentRangeUnknownLength_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderContentRangeUnknownLengthUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_ContentRangeUnknownLength_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EContentRange);
	iUTContext->iTestHeader->BeginRawDataL();
	TUint8 length = 0x03; // length
	TUint8 firstBytePos1 = 0x82; // UIntVar pt1
	TUint8 firstBytePos2 = 0x05; // UIntVar pt2
	TUint8 unknownToken = 0x80; // Token for '*'
	iUTContext->iTestHeader->WriteRawDataL(length);
	iUTContext->iTestHeader->WriteRawDataL(firstBytePos1);
	iUTContext->iTestHeader->WriteRawDataL(firstBytePos2);
	iUTContext->iTestHeader->WriteRawDataL(unknownToken);
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "261,*");
	iUTContext->SetExpectedL(KTxtExpected);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iContentRangeUnknownLengthValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iContentRangeUnknownLengthValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderContentRangeKnownLengthUnitTest,"CWspHeaderReader_ContentRangeKnownLength_UnitTest");

CWspHeaderReader_ContentRangeKnownLength_UnitTest* CWspHeaderReader_ContentRangeKnownLength_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_ContentRangeKnownLength_UnitTest* self = 
					new(ELeave) CWspHeaderReader_ContentRangeKnownLength_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_ContentRangeKnownLength_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iContentRangeKnownLengthValidator;
	iContentRangeKnownLengthValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_ContentRangeKnownLength_UnitTest::~CWspHeaderReader_ContentRangeKnownLength_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iContentRangeKnownLengthValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_ContentRangeKnownLength_UnitTest::CWspHeaderReader_ContentRangeKnownLength_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderContentRangeKnownLengthUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_ContentRangeKnownLength_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EContentRange);
	iUTContext->iTestHeader->BeginRawDataL();
	TUint8 length = 0x05; // length
	TUint8 firstBytePos1 = 0x82; // UIntVar1 pt1
	TUint8 firstBytePos2 = 0x05; // UIntVar1 pt2
	TUint8 entityLen1 = 0x83; // UIntVar2 pt1
	TUint8 entityLen2 = 0xA5; // UIntVar2 pt2
	TUint8 entityLen3 = 0x01; // UIntVar2 pt3
	iUTContext->iTestHeader->WriteRawDataL(length);
	iUTContext->iTestHeader->WriteRawDataL(firstBytePos1);
	iUTContext->iTestHeader->WriteRawDataL(firstBytePos2);
	iUTContext->iTestHeader->WriteRawDataL(entityLen1);
	iUTContext->iTestHeader->WriteRawDataL(entityLen2);
	iUTContext->iTestHeader->WriteRawDataL(entityLen3);
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "261,53889");
	iUTContext->SetExpectedL(KTxtExpected);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iContentRangeKnownLengthValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iContentRangeKnownLengthValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderContentRangeV1_2UnitTest,"CWspHeaderReader_ContentRangeV1_2_UnitTest");

CWspHeaderReader_ContentRangeV1_2_UnitTest* CWspHeaderReader_ContentRangeV1_2_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_ContentRangeV1_2_UnitTest* self = 
					new(ELeave) CWspHeaderReader_ContentRangeV1_2_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_ContentRangeV1_2_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iContentRangeV1_2Validator;
	iContentRangeV1_2Validator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_ContentRangeV1_2_UnitTest::~CWspHeaderReader_ContentRangeV1_2_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iContentRangeV1_2Validator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_ContentRangeV1_2_UnitTest::CWspHeaderReader_ContentRangeV1_2_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderContentRangeV1_2UnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_ContentRangeV1_2_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EContentRange);
	(REINTERPRET_CAST(CWspHeaderCodec*, iUTContext->iCodec))->SetWspVersion(CWspHeaderCodec::EVersion1_2);
	iUTContext->iTestHeader->BeginRawDataL();
	TUint8 length = 0x05; // length
	TUint8 firstBytePos1 = 0x82; // UIntVar1 pt1
	TUint8 firstBytePos2 = 0x05; // UIntVar1 pt2
	TUint8 entityLen1 = 0x83; // UIntVar2 pt1
	TUint8 entityLen2 = 0xA5; // UIntVar2 pt2
	TUint8 entityLen3 = 0x01; // UIntVar2 pt3
	iUTContext->iTestHeader->WriteRawDataL(length);
	iUTContext->iTestHeader->WriteRawDataL(firstBytePos1);
	iUTContext->iTestHeader->WriteRawDataL(firstBytePos2);
	iUTContext->iTestHeader->WriteRawDataL(entityLen1);
	iUTContext->iTestHeader->WriteRawDataL(entityLen2);
	iUTContext->iTestHeader->WriteRawDataL(entityLen3);
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "261,53889");
	iUTContext->SetExpectedL(KTxtExpected);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iContentRangeV1_2Validator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iContentRangeV1_2Validator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderXWapAppIdUriValueUnitTest,"CWspHeaderReader_XWapAppIdUriValue_UnitTest");

CWspHeaderReader_XWapAppIdUriValue_UnitTest* CWspHeaderReader_XWapAppIdUriValue_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_XWapAppIdUriValue_UnitTest* self = 
					new(ELeave) CWspHeaderReader_XWapAppIdUriValue_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_XWapAppIdUriValue_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iXWapAppIdUriValueValidator;
	iXWapAppIdUriValueValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_XWapAppIdUriValue_UnitTest::~CWspHeaderReader_XWapAppIdUriValue_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iXWapAppIdUriValueValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_XWapAppIdUriValue_UnitTest::CWspHeaderReader_XWapAppIdUriValue_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderXWapAppIdUriValueUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_XWapAppIdUriValue_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EXWapApplicationId);
	iUTContext->iTestHeader->BeginRawDataL();
	_LIT8(KTxtUri, "http://www.symbian.com\0");
	iUTContext->iTestHeader->WriteRawDataL(KTxtUri);
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "http://www.symbian.com");
	iUTContext->SetExpectedL(KTxtExpected);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iXWapAppIdUriValueValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iXWapAppIdUriValueValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderXWapAppIdNullUriUnitTest,"CWspHeaderReader_XWapAppIdNullUri_UnitTest");

CWspHeaderReader_XWapAppIdNullUri_UnitTest* CWspHeaderReader_XWapAppIdNullUri_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_XWapAppIdNullUri_UnitTest* self = 
					new(ELeave) CWspHeaderReader_XWapAppIdNullUri_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_XWapAppIdNullUri_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iXWapAppIdNullUriValidator;
	iXWapAppIdNullUriValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_XWapAppIdNullUri_UnitTest::~CWspHeaderReader_XWapAppIdNullUri_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iXWapAppIdNullUriValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_XWapAppIdNullUri_UnitTest::CWspHeaderReader_XWapAppIdNullUri_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderXWapAppIdNullUriUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_XWapAppIdNullUri_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EXWapApplicationId);
	iUTContext->iTestHeader->BeginRawDataL();
	TUint8 nullTerminate = 0x00;
	iUTContext->iTestHeader->WriteRawDataL(nullTerminate);
	iUTContext->iTestHeader->CommitRawData();
	iUTContext->SetExpectedL(KNullDesC8());
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iXWapAppIdNullUriValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iXWapAppIdNullUriValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderXWapAppIdShortIntUnitTest,"CWspHeaderReader_XWapAppIdShortInt_UnitTest");

CWspHeaderReader_XWapAppIdShortInt_UnitTest* CWspHeaderReader_XWapAppIdShortInt_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_XWapAppIdShortInt_UnitTest* self = 
					new(ELeave) CWspHeaderReader_XWapAppIdShortInt_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_XWapAppIdShortInt_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iXWapAppIdShortIntValidator;
	iXWapAppIdShortIntValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_XWapAppIdShortInt_UnitTest::~CWspHeaderReader_XWapAppIdShortInt_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iXWapAppIdShortIntValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_XWapAppIdShortInt_UnitTest::CWspHeaderReader_XWapAppIdShortInt_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderXWapAppIdShortIntUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_XWapAppIdShortInt_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EXWapApplicationId);
	iUTContext->iTestHeader->BeginRawDataL();
	TUint8 shortInt = 0x87; // Short int with top bit set
	iUTContext->iTestHeader->WriteRawDataL(shortInt);
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "7");
	iUTContext->SetExpectedL(KTxtExpected);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iXWapAppIdShortIntValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iXWapAppIdShortIntValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderXWapAppIdLongIntUnitTest,"CWspHeaderReader_XWapAppIdLongInt_UnitTest");

CWspHeaderReader_XWapAppIdLongInt_UnitTest* CWspHeaderReader_XWapAppIdLongInt_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_XWapAppIdLongInt_UnitTest* self = 
					new(ELeave) CWspHeaderReader_XWapAppIdLongInt_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_XWapAppIdLongInt_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iXWapAppIdLongIntValidator;
	iXWapAppIdLongIntValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_XWapAppIdLongInt_UnitTest::~CWspHeaderReader_XWapAppIdLongInt_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iXWapAppIdLongIntValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_XWapAppIdLongInt_UnitTest::CWspHeaderReader_XWapAppIdLongInt_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderXWapAppIdLongIntUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_XWapAppIdLongInt_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EXWapApplicationId);
	iUTContext->iTestHeader->BeginRawDataL();
	TUint8 shortlength = 0x02; // length
	TUint8 longInt1 = 0x4A;
	TUint8 longInt2 = 0x80;
	iUTContext->iTestHeader->WriteRawDataL(shortlength);
	iUTContext->iTestHeader->WriteRawDataL(longInt1);
	iUTContext->iTestHeader->WriteRawDataL(longInt2);
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "19072");
	iUTContext->SetExpectedL(KTxtExpected);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iXWapAppIdLongIntValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iXWapAppIdLongIntValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderXWapInitUriTextValueUnitTest,"CWspHeaderReader_XWapInitUriTextValue_UnitTest");

CWspHeaderReader_XWapInitUriTextValue_UnitTest* CWspHeaderReader_XWapInitUriTextValue_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_XWapInitUriTextValue_UnitTest* self = 
					new(ELeave) CWspHeaderReader_XWapInitUriTextValue_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_XWapInitUriTextValue_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iXWapInitUriTextValueValidator;
	iXWapInitUriTextValueValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_XWapInitUriTextValue_UnitTest::~CWspHeaderReader_XWapInitUriTextValue_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iXWapInitUriTextValueValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_XWapInitUriTextValue_UnitTest::CWspHeaderReader_XWapInitUriTextValue_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderXWapInitUriTextValueUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_XWapInitUriTextValue_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EXWapInitiatorURI);
	iUTContext->iTestHeader->BeginRawDataL();
	_LIT8(KTxtUri, "http://www.symbian.com\0");
	iUTContext->iTestHeader->WriteRawDataL(KTxtUri);
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "http://www.symbian.com");
	iUTContext->SetExpectedL(KTxtExpected);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iXWapInitUriTextValueValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iXWapInitUriTextValueValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderXWapInitUriNullValueUnitTest,"CWspHeaderReader_XWapInitUriNullValue_UnitTest");

CWspHeaderReader_XWapInitUriNullValue_UnitTest* CWspHeaderReader_XWapInitUriNullValue_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_XWapInitUriNullValue_UnitTest* self = 
					new(ELeave) CWspHeaderReader_XWapInitUriNullValue_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_XWapInitUriNullValue_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iXWapInitUriNullValueValidator;
	iXWapInitUriNullValueValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_XWapInitUriNullValue_UnitTest::~CWspHeaderReader_XWapInitUriNullValue_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iXWapInitUriNullValueValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_XWapInitUriNullValue_UnitTest::CWspHeaderReader_XWapInitUriNullValue_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderXWapInitUriNullValueUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_XWapInitUriNullValue_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EXWapInitiatorURI);
	iUTContext->iTestHeader->BeginRawDataL();
	TUint8 nullTerminate = 0x00;
	iUTContext->iTestHeader->WriteRawDataL(nullTerminate);
	iUTContext->iTestHeader->CommitRawData();
	iUTContext->SetExpectedL(KNullDesC8());
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iXWapInitUriNullValueValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iXWapInitUriNullValueValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderPushFlagShortIntUnitTest,"CWspHeaderReader_PushFlagShortInt_UnitTest");

CWspHeaderReader_PushFlagShortInt_UnitTest* CWspHeaderReader_PushFlagShortInt_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_PushFlagShortInt_UnitTest* self = 
					new(ELeave) CWspHeaderReader_PushFlagShortInt_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_PushFlagShortInt_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iPushFlagShortIntValidator;
	iPushFlagShortIntValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_PushFlagShortInt_UnitTest::~CWspHeaderReader_PushFlagShortInt_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iPushFlagShortIntValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_PushFlagShortInt_UnitTest::CWspHeaderReader_PushFlagShortInt_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderPushFlagShortIntUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_PushFlagShortInt_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EPushFlag);
	iUTContext->iTestHeader->BeginRawDataL();
	TUint8 shortInt = 0xA9; // short int with top bit set
	iUTContext->iTestHeader->WriteRawDataL(shortInt);
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "41");
	iUTContext->SetExpectedL(KTxtExpected);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iPushFlagShortIntValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iPushFlagShortIntValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderPushFlagLongIntUnitTest,"CWspHeaderReader_PushFlagLongInt_UnitTest");

CWspHeaderReader_PushFlagLongInt_UnitTest* CWspHeaderReader_PushFlagLongInt_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_PushFlagLongInt_UnitTest* self = 
					new(ELeave) CWspHeaderReader_PushFlagLongInt_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_PushFlagLongInt_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iPushFlagLongIntValidator;
	iPushFlagLongIntValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_PushFlagLongInt_UnitTest::~CWspHeaderReader_PushFlagLongInt_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iPushFlagLongIntValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_PushFlagLongInt_UnitTest::CWspHeaderReader_PushFlagLongInt_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderPushFlagLongIntUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_PushFlagLongInt_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EPushFlag);
	iUTContext->iTestHeader->BeginRawDataL();
	TUint8 length = 0x02;
	TUint8 longInt1 = 0xA8;
	TUint8 longInt2 = 0x05;
	iUTContext->iTestHeader->WriteRawDataL(length);
	iUTContext->iTestHeader->WriteRawDataL(longInt1);
	iUTContext->iTestHeader->WriteRawDataL(longInt2);
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "-20");
	iUTContext->SetExpectedL(KTxtExpected);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iPushFlagLongIntValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderLTrap_Transition(*iUTContext,*iPushFlagLongIntValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderPushFlagTextValueUnitTest,"CWspHeaderReader_PushFlagTextValue_UnitTest");

CWspHeaderReader_PushFlagTextValue_UnitTest* CWspHeaderReader_PushFlagTextValue_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_PushFlagTextValue_UnitTest* self = 
					new(ELeave) CWspHeaderReader_PushFlagTextValue_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_PushFlagTextValue_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iPushFlagTextValueValidator;
	iPushFlagTextValueValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_PushFlagTextValue_UnitTest::~CWspHeaderReader_PushFlagTextValue_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iPushFlagTextValueValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_PushFlagTextValue_UnitTest::CWspHeaderReader_PushFlagTextValue_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderPushFlagTextValueUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_PushFlagTextValue_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EPushFlag);
	iUTContext->iTestHeader->BeginRawDataL();
	_LIT8(KTxtValue, "test-text-value\0");
	iUTContext->iTestHeader->WriteRawDataL(KTxtValue);
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "-20");
	iUTContext->SetExpectedL(KTxtExpected);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iPushFlagTextValueValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderLTrap_Transition(*iUTContext,*iPushFlagTextValueValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderAllowGetUnitTest,"CWspHeaderReader_AllowGet_UnitTest");

CWspHeaderReader_AllowGet_UnitTest* CWspHeaderReader_AllowGet_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_AllowGet_UnitTest* self = 
					new(ELeave) CWspHeaderReader_AllowGet_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_AllowGet_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iAllowGetValidator;
	iAllowGetValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_AllowGet_UnitTest::~CWspHeaderReader_AllowGet_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iAllowGetValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_AllowGet_UnitTest::CWspHeaderReader_AllowGet_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderAllowGetUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_AllowGet_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EAllow);
	iUTContext->iTestHeader->BeginRawDataL();
	TUint8 getToken = 0xC0; // short int Get token with top bit set
	iUTContext->iTestHeader->WriteRawDataL(getToken);
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "Get");
	iUTContext->SetExpectedL(KTxtExpected);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iAllowGetValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iAllowGetValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderAllowTraceUnitTest,"CWspHeaderReader_AllowTrace_UnitTest");

CWspHeaderReader_AllowTrace_UnitTest* CWspHeaderReader_AllowTrace_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_AllowTrace_UnitTest* self = 
					new(ELeave) CWspHeaderReader_AllowTrace_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_AllowTrace_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iAllowTraceValidator;
	iAllowTraceValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_AllowTrace_UnitTest::~CWspHeaderReader_AllowTrace_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iAllowTraceValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_AllowTrace_UnitTest::CWspHeaderReader_AllowTrace_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderAllowTraceUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_AllowTrace_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EAllow);
	iUTContext->iTestHeader->BeginRawDataL();
	TUint8 traceToken = 0xC4; // short int Trace token with top bit set
	iUTContext->iTestHeader->WriteRawDataL(traceToken);
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "Trace");
	iUTContext->SetExpectedL(KTxtExpected);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iAllowTraceValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iAllowTraceValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderAllowLongIntUnitTest,"CWspHeaderReader_AllowLongInt_UnitTest");

CWspHeaderReader_AllowLongInt_UnitTest* CWspHeaderReader_AllowLongInt_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_AllowLongInt_UnitTest* self = 
					new(ELeave) CWspHeaderReader_AllowLongInt_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_AllowLongInt_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iAllowLongIntValidator;
	iAllowLongIntValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_AllowLongInt_UnitTest::~CWspHeaderReader_AllowLongInt_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iAllowLongIntValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_AllowLongInt_UnitTest::CWspHeaderReader_AllowLongInt_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderAllowLongIntUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_AllowLongInt_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EAllow);
	iUTContext->iTestHeader->BeginRawDataL();
	TUint8 length = 0x02; // long int length
	TUint8 longInt1 = 0x45;
	TUint8 longInt2 = 0xA1;
	iUTContext->iTestHeader->WriteRawDataL(length);
	iUTContext->iTestHeader->WriteRawDataL(longInt1);
	iUTContext->iTestHeader->WriteRawDataL(longInt2);
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "-20");
	iUTContext->SetExpectedL(KTxtExpected);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iAllowLongIntValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderLTrap_Transition(*iUTContext,*iAllowLongIntValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderAllowInvalidShortIntUnitTest,"CWspHeaderReader_AllowInvalidShortInt_UnitTest");

CWspHeaderReader_AllowInvalidShortInt_UnitTest* CWspHeaderReader_AllowInvalidShortInt_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_AllowInvalidShortInt_UnitTest* self = 
					new(ELeave) CWspHeaderReader_AllowInvalidShortInt_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_AllowInvalidShortInt_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iAllowInvalidShortIntValidator;
	// iAllowInvalidShortIntValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_AllowInvalidShortInt_UnitTest::~CWspHeaderReader_AllowInvalidShortInt_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iAllowInvalidShortIntValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_AllowInvalidShortInt_UnitTest::CWspHeaderReader_AllowInvalidShortInt_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderAllowInvalidShortIntUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_AllowInvalidShortInt_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EAllow);
	iUTContext->iTestHeader->BeginRawDataL();
	TUint8 shortInt = 0xF0; // invalid short int token
	iUTContext->iTestHeader->WriteRawDataL(shortInt);
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "-5");
	iUTContext->SetExpectedL(KTxtExpected);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iAllowInvalidShortIntValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderLTrap_Transition(*iUTContext,*iAllowInvalidShortIntValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderUpgradeTextUnitTest,"CWspHeaderReader_UpgradeText_UnitTest");

CWspHeaderReader_UpgradeText_UnitTest* CWspHeaderReader_UpgradeText_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_UpgradeText_UnitTest* self = 
					new(ELeave) CWspHeaderReader_UpgradeText_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_UpgradeText_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iUpgradeTextValidator;
	iUpgradeTextValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_UpgradeText_UnitTest::~CWspHeaderReader_UpgradeText_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iUpgradeTextValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_UpgradeText_UnitTest::CWspHeaderReader_UpgradeText_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderUpgradeTextUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_UpgradeText_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EUpgrade);
	iUTContext->iTestHeader->BeginRawDataL();
	_LIT8(KTxtUpgrade, "HTTP/1.2\0");
	iUTContext->iTestHeader->WriteRawDataL(KTxtUpgrade);
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "HTTP/1.2");
	iUTContext->SetExpectedL(KTxtExpected);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iUpgradeTextValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iUpgradeTextValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderUpgradeNullTextUnitTest,"CWspHeaderReader_UpgradeNullText_UnitTest");

CWspHeaderReader_UpgradeNullText_UnitTest* CWspHeaderReader_UpgradeNullText_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_UpgradeNullText_UnitTest* self = 
					new(ELeave) CWspHeaderReader_UpgradeNullText_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_UpgradeNullText_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iUpgradeNullTextValidator;
	iUpgradeNullTextValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_UpgradeNullText_UnitTest::~CWspHeaderReader_UpgradeNullText_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iUpgradeNullTextValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_UpgradeNullText_UnitTest::CWspHeaderReader_UpgradeNullText_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderUpgradeNullTextUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_UpgradeNullText_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EUpgrade);
	iUTContext->iTestHeader->BeginRawDataL();
	TUint8 nullTerminate = 0x00;
	iUTContext->iTestHeader->WriteRawDataL(nullTerminate);
	iUTContext->iTestHeader->CommitRawData();
	iUTContext->SetExpectedL(KNullDesC8());
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iUpgradeNullTextValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iUpgradeNullTextValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderWarningShortIntUnitTest,"CWspHeaderReader_WarningShortInt_UnitTest");

CWspHeaderReader_WarningShortInt_UnitTest* CWspHeaderReader_WarningShortInt_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_WarningShortInt_UnitTest* self = 
					new(ELeave) CWspHeaderReader_WarningShortInt_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_WarningShortInt_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iWarningShortIntValidator;
	iWarningShortIntValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_WarningShortInt_UnitTest::~CWspHeaderReader_WarningShortInt_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iWarningShortIntValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_WarningShortInt_UnitTest::CWspHeaderReader_WarningShortInt_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderWarningShortIntUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_WarningShortInt_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EWarning);
	iUTContext->iTestHeader->BeginRawDataL();
	TUint8 warning = 0x8D; // Short int token for 13 (113)
	iUTContext->iTestHeader->WriteRawDataL(warning);
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "113");
	iUTContext->SetExpectedL(KTxtExpected);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iWarningShortIntValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iWarningShortIntValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderWarningFullValueUnitTest,"CWspHeaderReader_WarningFullValue_UnitTest");

CWspHeaderReader_WarningFullValue_UnitTest* CWspHeaderReader_WarningFullValue_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_WarningFullValue_UnitTest* self = 
					new(ELeave) CWspHeaderReader_WarningFullValue_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_WarningFullValue_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iWarningFullValueValidator;
	iWarningFullValueValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_WarningFullValue_UnitTest::~CWspHeaderReader_WarningFullValue_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iWarningFullValueValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_WarningFullValue_UnitTest::CWspHeaderReader_WarningFullValue_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderWarningFullValueUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_WarningFullValue_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EWarning);
	iUTContext->iTestHeader->BeginRawDataL();
	TUint8 length = 0x1D; // length for 29 bytes
	TUint8 warning = 0x8A; // Short int token for 10 (110)
	_LIT8(KTxtWarnAgent, "my-warn-agent\0");
	_LIT8(KTxtWarnText, "my-warn-value\0");
	iUTContext->iTestHeader->WriteRawDataL(length);
	iUTContext->iTestHeader->WriteRawDataL(warning);
	iUTContext->iTestHeader->WriteRawDataL(KTxtWarnAgent);
	iUTContext->iTestHeader->WriteRawDataL(KTxtWarnText);
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "110,my-warn-agent,my-warn-value");
	iUTContext->SetExpectedL(KTxtExpected);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iWarningFullValueValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iWarningFullValueValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderWarningShortWarnNullAgentUnitTest,"CWspHeaderReader_WarningShortWarnNullAgent_UnitTest");

CWspHeaderReader_WarningShortWarnNullAgent_UnitTest* CWspHeaderReader_WarningShortWarnNullAgent_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_WarningShortWarnNullAgent_UnitTest* self = 
					new(ELeave) CWspHeaderReader_WarningShortWarnNullAgent_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_WarningShortWarnNullAgent_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iWarningShortWarnNullAgentValidator;
	iWarningShortWarnNullAgentValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_WarningShortWarnNullAgent_UnitTest::~CWspHeaderReader_WarningShortWarnNullAgent_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iWarningShortWarnNullAgentValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_WarningShortWarnNullAgent_UnitTest::CWspHeaderReader_WarningShortWarnNullAgent_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderWarningShortWarnNullAgentUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_WarningShortWarnNullAgent_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EWarning);
	iUTContext->iTestHeader->BeginRawDataL();
	TUint8 length = 0x10; // length for 16 bytes
	TUint8 warning = 0xA5; // Short int token for 10 (110)
	TUint8 KTxtWarnAgent = 0x00;
	_LIT8(KTxtWarnText, "my-warn-value\0");
	iUTContext->iTestHeader->WriteRawDataL(length);
	iUTContext->iTestHeader->WriteRawDataL(warning);
	iUTContext->iTestHeader->WriteRawDataL(KTxtWarnAgent);
	iUTContext->iTestHeader->WriteRawDataL(KTxtWarnText);
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "37,,my-warn-value");
	iUTContext->SetExpectedL(KTxtExpected);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iWarningShortWarnNullAgentValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iWarningShortWarnNullAgentValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderWarningShortIntNullTextUnitTest,"CWspHeaderReader_WarningShortIntNullText_UnitTest");

CWspHeaderReader_WarningShortIntNullText_UnitTest* CWspHeaderReader_WarningShortIntNullText_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_WarningShortIntNullText_UnitTest* self = 
					new(ELeave) CWspHeaderReader_WarningShortIntNullText_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_WarningShortIntNullText_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iWarningShortIntNullTextValidator;
	iWarningShortIntNullTextValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_WarningShortIntNullText_UnitTest::~CWspHeaderReader_WarningShortIntNullText_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iWarningShortIntNullTextValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_WarningShortIntNullText_UnitTest::CWspHeaderReader_WarningShortIntNullText_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderWarningShortIntNullTextUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_WarningShortIntNullText_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EWarning);
	iUTContext->iTestHeader->BeginRawDataL();
	TUint8 length = 0x10; // length for 16 bytes
	TUint8 warning = 0xE3; // Short int token for 99 (199)
	_LIT8(KTxtWarnAgent, "my-warn-agent\0");
	TUint8 KTxtWarnText = 0x00;
	iUTContext->iTestHeader->WriteRawDataL(length);
	iUTContext->iTestHeader->WriteRawDataL(warning);
	iUTContext->iTestHeader->WriteRawDataL(KTxtWarnAgent);
	iUTContext->iTestHeader->WriteRawDataL(KTxtWarnText);
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "199,my-warn-agent,");
	iUTContext->SetExpectedL(KTxtExpected);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iWarningShortIntNullTextValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iWarningShortIntNullTextValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderWarningShortIntNullAgentNullTextUnitTest,"CWspHeaderReader_WarningShortIntNullAgentNullText_UnitTest");

CWspHeaderReader_WarningShortIntNullAgentNullText_UnitTest* CWspHeaderReader_WarningShortIntNullAgentNullText_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_WarningShortIntNullAgentNullText_UnitTest* self = 
					new(ELeave) CWspHeaderReader_WarningShortIntNullAgentNullText_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_WarningShortIntNullAgentNullText_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iWarningShortIntNullAgentNullTextValidator;
	iWarningShortIntNullAgentNullTextValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_WarningShortIntNullAgentNullText_UnitTest::~CWspHeaderReader_WarningShortIntNullAgentNullText_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iWarningShortIntNullAgentNullTextValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_WarningShortIntNullAgentNullText_UnitTest::CWspHeaderReader_WarningShortIntNullAgentNullText_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderWarningShortIntNullAgentNullTextUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_WarningShortIntNullAgentNullText_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EWarning);
	iUTContext->iTestHeader->BeginRawDataL();
	TUint8 length = 0x03; // length for 3 bytes
	TUint8 warning = 0x8E; // Short int token for 14 (214)
	TUint8 KTxtWarnAgent = 0x00;
	TUint8 KTxtWarnText = 0x00;
	iUTContext->iTestHeader->WriteRawDataL(length);
	iUTContext->iTestHeader->WriteRawDataL(warning);
	iUTContext->iTestHeader->WriteRawDataL(KTxtWarnAgent);
	iUTContext->iTestHeader->WriteRawDataL(KTxtWarnText);
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "214,,");
	iUTContext->SetExpectedL(KTxtExpected);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iWarningShortIntNullAgentNullTextValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iWarningShortIntNullAgentNullTextValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderContentEncodingMultiHeaderUnitTest,"CWspHeaderReader_ContentEncodingMultiHeader_UnitTest");

CWspHeaderReader_ContentEncodingMultiHeader_UnitTest* CWspHeaderReader_ContentEncodingMultiHeader_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_ContentEncodingMultiHeader_UnitTest* self = 
					new(ELeave) CWspHeaderReader_ContentEncodingMultiHeader_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_ContentEncodingMultiHeader_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iContentEncodingMultiHeaderValidator;
	iContentEncodingMultiHeaderValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_ContentEncodingMultiHeader_UnitTest::~CWspHeaderReader_ContentEncodingMultiHeader_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iContentEncodingMultiHeaderValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_ContentEncodingMultiHeader_UnitTest::CWspHeaderReader_ContentEncodingMultiHeader_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderContentEncodingMultiHeaderUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_ContentEncodingMultiHeader_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EContentEncoding);
	iUTContext->iTestHeader->BeginRawDataL();
	iUTContext->iTestHeader->WriteRawDataL(0x80); // token for Gzip
	iUTContext->iTestHeader->WriteRawDataL(0x8B); // separator
	iUTContext->iTestHeader->WriteRawDataL(0x81); // token for Compress
	iUTContext->iTestHeader->WriteRawDataL(0x8B); // separator
	_LIT8(KTxtToken, "my-encoding\0");
	iUTContext->iTestHeader->WriteRawDataL(KTxtToken);
	iUTContext->iTestHeader->CommitRawData();
	
	_LIT8(KTxtExpected, "GZip,Compress,my-encoding");
	iUTContext->SetExpectedL(KTxtExpected);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iContentEncodingMultiHeaderValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iContentEncodingMultiHeaderValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderContentLanguageMultiHeaderUnitTest,"CWspHeaderReader_ContentLanguageMultiHeader_UnitTest");

CWspHeaderReader_ContentLanguageMultiHeader_UnitTest* CWspHeaderReader_ContentLanguageMultiHeader_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_ContentLanguageMultiHeader_UnitTest* self = 
					new(ELeave) CWspHeaderReader_ContentLanguageMultiHeader_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_ContentLanguageMultiHeader_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iContentLanguageMultiHeaderValidator;
	iContentLanguageMultiHeaderValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_ContentLanguageMultiHeader_UnitTest::~CWspHeaderReader_ContentLanguageMultiHeader_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iContentLanguageMultiHeaderValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_ContentLanguageMultiHeader_UnitTest::CWspHeaderReader_ContentLanguageMultiHeader_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderContentLanguageMultiHeaderUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_ContentLanguageMultiHeader_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EContentLanguage);
	iUTContext->iTestHeader->BeginRawDataL();
	iUTContext->iTestHeader->WriteRawDataL(0x80); // token '*' any
	iUTContext->iTestHeader->WriteRawDataL(0x8C); // separator
	_LIT8(KTxtToken, "my-language\0");
	iUTContext->iTestHeader->WriteRawDataL(KTxtToken);
	iUTContext->iTestHeader->WriteRawDataL(0x8C); // separator
	iUTContext->iTestHeader->WriteRawDataL(0x99); // token 'en'
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "*,my-language,en");
	iUTContext->SetExpectedL(KTxtExpected);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iContentLanguageMultiHeaderValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iContentLanguageMultiHeaderValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderViaMultiHeaderUnitTest,"CWspHeaderReader_ViaMultiHeader_UnitTest");

CWspHeaderReader_ViaMultiHeader_UnitTest* CWspHeaderReader_ViaMultiHeader_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_ViaMultiHeader_UnitTest* self = 
					new(ELeave) CWspHeaderReader_ViaMultiHeader_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_ViaMultiHeader_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iViaMultiHeaderValidator;
	iViaMultiHeaderValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_ViaMultiHeader_UnitTest::~CWspHeaderReader_ViaMultiHeader_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iViaMultiHeaderValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_ViaMultiHeader_UnitTest::CWspHeaderReader_ViaMultiHeader_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderViaMultiHeaderUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_ViaMultiHeader_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EVia);
	iUTContext->iTestHeader->BeginRawDataL();
	_LIT8(KTxtVia1, "symbian.com\0");
	iUTContext->iTestHeader->WriteRawDataL(KTxtVia1);
	iUTContext->iTestHeader->WriteRawDataL(0xAB); // separator
	iUTContext->iTestHeader->WriteRawDataL(0x00);
	iUTContext->iTestHeader->WriteRawDataL(0xAB); // separator
	_LIT8(KTxtVia2, "1.1 devnet.org.uk\0");
	iUTContext->iTestHeader->WriteRawDataL(KTxtVia2);
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "symbian.com,,1.1 devnet.org.uk");
	iUTContext->SetExpectedL(KTxtExpected);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iViaMultiHeaderValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iViaMultiHeaderValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderTrailerMultiHeaderUnitTest,"CWspHeaderReader_TrailerMultiHeader_UnitTest");

CWspHeaderReader_TrailerMultiHeader_UnitTest* CWspHeaderReader_TrailerMultiHeader_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_TrailerMultiHeader_UnitTest* self = 
					new(ELeave) CWspHeaderReader_TrailerMultiHeader_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_TrailerMultiHeader_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iTrailerMultiHeaderValidator;
	iTrailerMultiHeaderValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_TrailerMultiHeader_UnitTest::~CWspHeaderReader_TrailerMultiHeader_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iTrailerMultiHeaderValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_TrailerMultiHeader_UnitTest::CWspHeaderReader_TrailerMultiHeader_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderTrailerMultiHeaderUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_TrailerMultiHeader_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::ETrailer);
	iUTContext->iTestHeader->BeginRawDataL();
	iUTContext->iTestHeader->WriteRawDataL(0x95); // 'From' token
	iUTContext->iTestHeader->WriteRawDataL(0xBA); // separator
	iUTContext->iTestHeader->WriteRawDataL(0xC3); // 'Encoding-version' token
	iUTContext->iTestHeader->WriteRawDataL(0xBA); // separator
	_LIT8(KTxtTrailer, "my-header\0");
	iUTContext->iTestHeader->WriteRawDataL(KTxtTrailer);
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "From,Encoding-version,my-header");
	iUTContext->SetExpectedL(KTxtExpected);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iTrailerMultiHeaderValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iTrailerMultiHeaderValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderUpgradeMultiHeaderUnitTest,"CWspHeaderReader_UpgradeMultiHeader_UnitTest");

CWspHeaderReader_UpgradeMultiHeader_UnitTest* CWspHeaderReader_UpgradeMultiHeader_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_UpgradeMultiHeader_UnitTest* self = 
					new(ELeave) CWspHeaderReader_UpgradeMultiHeader_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_UpgradeMultiHeader_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iUpgradeMultiHeaderValidator;
	iUpgradeMultiHeaderValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_UpgradeMultiHeader_UnitTest::~CWspHeaderReader_UpgradeMultiHeader_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iUpgradeMultiHeaderValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_UpgradeMultiHeader_UnitTest::CWspHeaderReader_UpgradeMultiHeader_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderUpgradeMultiHeaderUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_UpgradeMultiHeader_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EUpgrade);
	iUTContext->iTestHeader->BeginRawDataL();
	_LIT8(KTxtUpgrade1, "symbian.com\0");
	iUTContext->iTestHeader->WriteRawDataL(KTxtUpgrade1);
	iUTContext->iTestHeader->WriteRawDataL(0xA8); // separator
	iUTContext->iTestHeader->WriteRawDataL(0x00);
	iUTContext->iTestHeader->WriteRawDataL(0xA8); // separator
	_LIT8(KTxtUpgrade2, "devnet.org.uk\0");
	iUTContext->iTestHeader->WriteRawDataL(KTxtUpgrade2);
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "symbian.com,,devnet.org.uk");
	iUTContext->SetExpectedL(KTxtExpected);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iUpgradeMultiHeaderValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iUpgradeMultiHeaderValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderAllowMultiHeaderUnitTest,"CWspHeaderReader_AllowMultiHeader_UnitTest");

CWspHeaderReader_AllowMultiHeader_UnitTest* CWspHeaderReader_AllowMultiHeader_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_AllowMultiHeader_UnitTest* self = 
					new(ELeave) CWspHeaderReader_AllowMultiHeader_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_AllowMultiHeader_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iAllowMultiHeaderValidator;
	iAllowMultiHeaderValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_AllowMultiHeader_UnitTest::~CWspHeaderReader_AllowMultiHeader_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iAllowMultiHeaderValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_AllowMultiHeader_UnitTest::CWspHeaderReader_AllowMultiHeader_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderAllowMultiHeaderUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_AllowMultiHeader_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EAllow);
	iUTContext->iTestHeader->BeginRawDataL();
	iUTContext->iTestHeader->WriteRawDataL(0xC0);
	iUTContext->iTestHeader->WriteRawDataL(0x86); // separator
	iUTContext->iTestHeader->WriteRawDataL(0xE0);
	iUTContext->iTestHeader->WriteRawDataL(0x86); // separator
	iUTContext->iTestHeader->WriteRawDataL(0xC4);
	iUTContext->iTestHeader->CommitRawData();

	_LIT8(KTxtExpected, "Get,Post,Trace");
	iUTContext->SetExpectedL(KTxtExpected);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iAllowMultiHeaderValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iAllowMultiHeaderValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderUnknownHeaderUnitTest,"CWspHeaderReader_UnknownHeader_UnitTest");

CWspHeaderReader_UnknownHeader_UnitTest* CWspHeaderReader_UnknownHeader_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_UnknownHeader_UnitTest* self = 
					new(ELeave) CWspHeaderReader_UnknownHeader_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_UnknownHeader_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iUnknownHeaderValidator;
	iUnknownHeaderValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_UnknownHeader_UnitTest::~CWspHeaderReader_UnknownHeader_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iUnknownHeaderValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_UnknownHeader_UnitTest::CWspHeaderReader_UnknownHeader_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderUnknownHeaderUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_UnknownHeader_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	// Passing KErrNotFound will create a header field called TestHeader which is not recognised by the
	// WSP header codec and the default WSP header codec will be used instead
	iUTContext->ConstructL(KErrNotFound);
	_LIT8(KTxtSampleData, "This is some sample header field data\0");
	iUTContext->iTestHeader->BeginRawDataL();
	iUTContext->iTestHeader->WriteRawDataL(KTxtSampleData());
	iUTContext->iTestHeader->CommitRawData();

	_LIT8(KTxtExpected, "This is some sample header field data");
	iUTContext->SetExpectedL(KTxtExpected);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iUnknownHeaderValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iUnknownHeaderValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderUnknownHeaderNullUnitTest,"CWspHeaderReader_UnknownHeaderNull_UnitTest");

CWspHeaderReader_UnknownHeaderNull_UnitTest* CWspHeaderReader_UnknownHeaderNull_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_UnknownHeaderNull_UnitTest* self = 
					new(ELeave) CWspHeaderReader_UnknownHeaderNull_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_UnknownHeaderNull_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iUnknownHeaderNullValidator;
	iUnknownHeaderNullValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_UnknownHeaderNull_UnitTest::~CWspHeaderReader_UnknownHeaderNull_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iUnknownHeaderNullValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_UnknownHeaderNull_UnitTest::CWspHeaderReader_UnknownHeaderNull_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderUnknownHeaderNullUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_UnknownHeaderNull_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(KErrNotFound);
	iUTContext->iTestHeader->BeginRawDataL();
	TUint8 octet00 = 0x00;
	iUTContext->iTestHeader->WriteRawDataL(octet00);
	iUTContext->iTestHeader->CommitRawData();
	iUTContext->SetExpectedL(KNullDesC8());
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iUnknownHeaderNullValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iUnknownHeaderNullValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderMultiUnknownHeaderUnitTest,"CWspHeaderReader_MultiUnknownHeader_UnitTest");

CWspHeaderReader_MultiUnknownHeader_UnitTest* CWspHeaderReader_MultiUnknownHeader_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_MultiUnknownHeader_UnitTest* self = 
					new(ELeave) CWspHeaderReader_MultiUnknownHeader_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_MultiUnknownHeader_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iMultiUnknownHeaderValidator;
	iMultiUnknownHeaderValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_MultiUnknownHeader_UnitTest::~CWspHeaderReader_MultiUnknownHeader_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iMultiUnknownHeaderValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_MultiUnknownHeader_UnitTest::CWspHeaderReader_MultiUnknownHeader_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderMultiUnknownHeaderUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_MultiUnknownHeader_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(KErrNotFound);
	_LIT8(KTxtTestData1, "First test data\0");
	_LIT8(KTxtTestData2, "Second test data\0");
	_LIT8(KTxtTestData3, "Third test data\0");
	_LIT8(KTxtHeaderField, "TestHeader\0");
	iUTContext->iTestHeader->BeginRawDataL();
	iUTContext->iTestHeader->WriteRawDataL(KTxtTestData1());
	iUTContext->iTestHeader->WriteRawDataL(KTxtHeaderField()); // separator
	iUTContext->iTestHeader->WriteRawDataL(KTxtTestData2());
	iUTContext->iTestHeader->WriteRawDataL(KTxtHeaderField()); // separator
	iUTContext->iTestHeader->WriteRawDataL(KTxtTestData3());
	iUTContext->iTestHeader->CommitRawData();

	_LIT8(KTxtExpected, "First test data,Second test data,Third test data");
	iUTContext->SetExpectedL(KTxtExpected);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iMultiUnknownHeaderValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iMultiUnknownHeaderValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderProfileWarningWarnCodeUnitTest,"CWspHeaderReader_ProfileWarningWarnCode_UnitTest");

CWspHeaderReader_ProfileWarningWarnCode_UnitTest* CWspHeaderReader_ProfileWarningWarnCode_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_ProfileWarningWarnCode_UnitTest* self = 
					new(ELeave) CWspHeaderReader_ProfileWarningWarnCode_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_ProfileWarningWarnCode_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iProfileWarningWarnCodeValidator;
	iProfileWarningWarnCodeValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_ProfileWarningWarnCode_UnitTest::~CWspHeaderReader_ProfileWarningWarnCode_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iProfileWarningWarnCodeValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_ProfileWarningWarnCode_UnitTest::CWspHeaderReader_ProfileWarningWarnCode_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderProfileWarningWarnCodeUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_ProfileWarningWarnCode_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EProfileWarning);
	
	TUint8 warnCode = 0x90; // Short int token for 16 (100)
	iUTContext->iTestHeader->BeginRawDataL();
	iUTContext->iTestHeader->WriteRawDataL(warnCode);
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "100");
	iUTContext->SetExpectedL(KTxtExpected);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iProfileWarningWarnCodeValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iProfileWarningWarnCodeValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderProfileWarningWarnCodeTextWarnTargetUnitTest,"CWspHeaderReader_ProfileWarningWarnCodeTextWarnTarget_UnitTest");

CWspHeaderReader_ProfileWarningWarnCodeTextWarnTarget_UnitTest* CWspHeaderReader_ProfileWarningWarnCodeTextWarnTarget_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_ProfileWarningWarnCodeTextWarnTarget_UnitTest* self = 
					new(ELeave) CWspHeaderReader_ProfileWarningWarnCodeTextWarnTarget_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_ProfileWarningWarnCodeTextWarnTarget_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iProfileWarningWarnCodeTextWarnTargetValidator;
	iProfileWarningWarnCodeTextWarnTargetValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_ProfileWarningWarnCodeTextWarnTarget_UnitTest::~CWspHeaderReader_ProfileWarningWarnCodeTextWarnTarget_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iProfileWarningWarnCodeTextWarnTargetValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_ProfileWarningWarnCodeTextWarnTarget_UnitTest::CWspHeaderReader_ProfileWarningWarnCodeTextWarnTarget_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderProfileWarningWarnCodeTextWarnTargetUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_ProfileWarningWarnCodeTextWarnTarget_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EProfileWarning);
	
	TUint8 valueLength = 0x18; // length for 24 bytes
	TUint8 warnCode = 0x92; // Short int token for 18 (102)
	_LIT8(KTxtWarnTarget, "http://www.symbian.com\0");
	iUTContext->iTestHeader->BeginRawDataL();
	iUTContext->iTestHeader->WriteRawDataL(valueLength);
	iUTContext->iTestHeader->WriteRawDataL(warnCode);
	iUTContext->iTestHeader->WriteRawDataL(KTxtWarnTarget());
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "102,http://www.symbian.com");
	iUTContext->SetExpectedL(KTxtExpected);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iProfileWarningWarnCodeTextWarnTargetValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iProfileWarningWarnCodeTextWarnTargetValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderProfileWarningWarnCodeHostWarnTargetUnitTest,"CWspHeaderReader_ProfileWarningWarnCodeHostWarnTarget_UnitTest");

CWspHeaderReader_ProfileWarningWarnCodeHostWarnTarget_UnitTest* CWspHeaderReader_ProfileWarningWarnCodeHostWarnTarget_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_ProfileWarningWarnCodeHostWarnTarget_UnitTest* self = 
					new(ELeave) CWspHeaderReader_ProfileWarningWarnCodeHostWarnTarget_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_ProfileWarningWarnCodeHostWarnTarget_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iProfileWarningWarnCodeHostWarnTargetValidator;
	iProfileWarningWarnCodeHostWarnTargetValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_ProfileWarningWarnCodeHostWarnTarget_UnitTest::~CWspHeaderReader_ProfileWarningWarnCodeHostWarnTarget_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iProfileWarningWarnCodeHostWarnTargetValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_ProfileWarningWarnCodeHostWarnTarget_UnitTest::CWspHeaderReader_ProfileWarningWarnCodeHostWarnTarget_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderProfileWarningWarnCodeHostWarnTargetUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_ProfileWarningWarnCodeHostWarnTarget_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EProfileWarning);

	TUint8 valueLength = 0x17; // length for 23 bytes
	TUint8 warnCode = 0x91; // Short int token for 17 (101)
	_LIT8(KTxtHostWarnTarget, "http://www.symbian.com\0");

	iUTContext->iTestHeader->BeginRawDataL();
	iUTContext->iTestHeader->WriteRawDataL(valueLength);
	iUTContext->iTestHeader->WriteRawDataL(warnCode);
	iUTContext->iTestHeader->WriteRawDataL(KTxtHostWarnTarget());
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "101,http://www.symbian.com");
	iUTContext->SetExpectedL(KTxtExpected);
	
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iProfileWarningWarnCodeHostWarnTargetValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iProfileWarningWarnCodeHostWarnTargetValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderProfileWarningLongFullValueUnitTest,"CWspHeaderReader_ProfileWarningLongFullValue_UnitTest");

CWspHeaderReader_ProfileWarningLongFullValue_UnitTest* CWspHeaderReader_ProfileWarningLongFullValue_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_ProfileWarningLongFullValue_UnitTest* self = 
					new(ELeave) CWspHeaderReader_ProfileWarningLongFullValue_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_ProfileWarningLongFullValue_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iProfileWarningLongFullValueValidator;
	iProfileWarningLongFullValueValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_ProfileWarningLongFullValue_UnitTest::~CWspHeaderReader_ProfileWarningLongFullValue_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iProfileWarningLongFullValueValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_ProfileWarningLongFullValue_UnitTest::CWspHeaderReader_ProfileWarningLongFullValue_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderProfileWarningLongFullValueUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_ProfileWarningLongFullValue_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EProfileWarning);
	
	TUint8 octet1F = 0x1F; // <Octet 31> for length quote
	TUint8 valueLength = 0x27; // length for 39 bytes
	TUint8 warnCode = 0xA0; // Short int token for 32 (200)
	_LIT8(KTxtWarnTarget, "http://www.longaddresswebsite.com\0");
	TUint8 octet03 = 0x03; // short length of 3 for date value
	TUint8 octet255 = 0xFF; // example date value
	TUint8 octet254 = 0xFE; // example date value

	iUTContext->iTestHeader->BeginRawDataL();
	iUTContext->iTestHeader->WriteRawDataL(octet1F);
	iUTContext->iTestHeader->WriteRawDataL(valueLength);
	iUTContext->iTestHeader->WriteRawDataL(warnCode);
	iUTContext->iTestHeader->WriteRawDataL(KTxtWarnTarget());
	iUTContext->iTestHeader->WriteRawDataL(octet03);
	iUTContext->iTestHeader->WriteRawDataL(octet255);
	iUTContext->iTestHeader->WriteRawDataL(octet255);
	iUTContext->iTestHeader->WriteRawDataL(octet254);
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "200,http://www.longaddresswebsite.com,Tue, 14 Jul 1970 04:20:14 GMT");
	iUTContext->SetExpectedL(KTxtExpected);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iProfileWarningLongFullValueValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iProfileWarningLongFullValueValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderProfileWarningLongWarnCodeHostWarnTargetUnitTest,"CWspHeaderReader_ProfileWarningLongWarnCodeHostWarnTarget_UnitTest");

CWspHeaderReader_ProfileWarningLongWarnCodeHostWarnTarget_UnitTest* CWspHeaderReader_ProfileWarningLongWarnCodeHostWarnTarget_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_ProfileWarningLongWarnCodeHostWarnTarget_UnitTest* self = 
					new(ELeave) CWspHeaderReader_ProfileWarningLongWarnCodeHostWarnTarget_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_ProfileWarningLongWarnCodeHostWarnTarget_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iProfileWarningLongWarnCodeHostWarnTargetValidator;
	iProfileWarningLongWarnCodeHostWarnTargetValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_ProfileWarningLongWarnCodeHostWarnTarget_UnitTest::~CWspHeaderReader_ProfileWarningLongWarnCodeHostWarnTarget_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iProfileWarningLongWarnCodeHostWarnTargetValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_ProfileWarningLongWarnCodeHostWarnTarget_UnitTest::CWspHeaderReader_ProfileWarningLongWarnCodeHostWarnTarget_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderProfileWarningLongWarnCodeHostWarnTargetUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_ProfileWarningLongWarnCodeHostWarnTarget_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EProfileWarning);

	TUint8 octet1F = 0x1F; // <Octet 31> for length quote
	TUint8 valueLength = 0x27; // length for 39 bytes
	TUint8 warnCode = 0x90; // Short int token for 16 (100)
	_LIT8(KTxtHostWarnTarget, "http://www.longaddresswebsite.com:9090\0");

	iUTContext->iTestHeader->BeginRawDataL();
	iUTContext->iTestHeader->WriteRawDataL(octet1F);
	iUTContext->iTestHeader->WriteRawDataL(valueLength);
	iUTContext->iTestHeader->WriteRawDataL(warnCode);
	iUTContext->iTestHeader->WriteRawDataL(KTxtHostWarnTarget());
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "100,http://www.longaddresswebsite.com:9090");
	iUTContext->SetExpectedL(KTxtExpected);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iProfileWarningLongWarnCodeHostWarnTargetValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iProfileWarningLongWarnCodeHostWarnTargetValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderProfileWarningLongWarnCodeHostWarnTargetDateValueUnitTest,"CWspHeaderReader_ProfileWarningLongWarnCodeHostWarnTargetDateValue_UnitTest");

CWspHeaderReader_ProfileWarningLongWarnCodeHostWarnTargetDateValue_UnitTest* CWspHeaderReader_ProfileWarningLongWarnCodeHostWarnTargetDateValue_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderReader_ProfileWarningLongWarnCodeHostWarnTargetDateValue_UnitTest* self = 
					new(ELeave) CWspHeaderReader_ProfileWarningLongWarnCodeHostWarnTargetDateValue_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderReader_ProfileWarningLongWarnCodeHostWarnTargetDateValue_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iProfileWarningLongWarnCodeHostWarnTargetDateValueValidator;
	iProfileWarningLongWarnCodeHostWarnTargetDateValueValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderReader_ProfileWarningLongWarnCodeHostWarnTargetDateValue_UnitTest::~CWspHeaderReader_ProfileWarningLongWarnCodeHostWarnTargetDateValue_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iProfileWarningLongWarnCodeHostWarnTargetDateValueValidator;
	delete iDtorValidator;
	}

inline CWspHeaderReader_ProfileWarningLongWarnCodeHostWarnTargetDateValue_UnitTest::CWspHeaderReader_ProfileWarningLongWarnCodeHostWarnTargetDateValue_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderReaderProfileWarningLongWarnCodeHostWarnTargetDateValueUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderReader_ProfileWarningLongWarnCodeHostWarnTargetDateValue_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderReader_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderReader_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EProfileWarning);

	TUint8 octet1F = 0x1F; // <Octet 31> for length quote
	TUint8 valueLength = 0x27; // length for 39 bytes
	TUint8 warnCode = 0xA1; // Short int token for 33 (201)
	_LIT8(KTxtHostWarnTarget, "http://www.longaddresswebsite.com:80\0");
	TUint8 octet03 = 0x03; // short length of 3 for date value
	TUint8 octet255 = 0xFF; // example date value
	TUint8 octet254 = 0xFE; // example date value

	iUTContext->iTestHeader->BeginRawDataL();
	iUTContext->iTestHeader->WriteRawDataL(octet1F);
	iUTContext->iTestHeader->WriteRawDataL(valueLength);
	iUTContext->iTestHeader->WriteRawDataL(warnCode);
	iUTContext->iTestHeader->WriteRawDataL(KTxtHostWarnTarget());
	iUTContext->iTestHeader->WriteRawDataL(octet03);
	iUTContext->iTestHeader->WriteRawDataL(octet255);
	iUTContext->iTestHeader->WriteRawDataL(octet255);
	iUTContext->iTestHeader->WriteRawDataL(octet254);
	iUTContext->iTestHeader->CommitRawData();
	_LIT8(KTxtExpected, "201,http://www.longaddresswebsite.com:80,Tue, 14 Jul 1970 04:20:14 GMT");
	iUTContext->SetExpectedL(KTxtExpected);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderReader class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderReader_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_NewL_Transition(*iUTContext,*iCtorValidator));
	iProfileWarningLongWarnCodeHostWarnTargetDateValueValidator = new(ELeave) TWspHeaderReader_DecodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_DecodeHeaderL_Transition(*iUTContext,*iProfileWarningLongWarnCodeHostWarnTargetDateValueValidator));
	iDtorValidator = new(ELeave) TWspHeaderReader_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderReader_Dtor_Transition(*iUTContext,*iDtorValidator));
	}


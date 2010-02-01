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
// The unit test class implementations for the TFilterConfigurationIterator class.
// 
//

/**
 @file FilterConfigurationIteratorUnitTest.cpp
*/

#include "FilterConfigurationIteratorUnitTest.h"

// ______________________________________________________________________________
//
_LIT(KFilterConfigurationIteratorCreateAndDestroyUnitTest,"CFilterConfigurationIterator_CreateAndDestroy_UnitTest");

CFilterConfigurationIterator_CreateAndDestroy_UnitTest* CFilterConfigurationIterator_CreateAndDestroy_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CFilterConfigurationIterator_CreateAndDestroy_UnitTest* self = 
					new(ELeave) CFilterConfigurationIterator_CreateAndDestroy_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CFilterConfigurationIterator_CreateAndDestroy_UnitTest::RunError(TInt aError)
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
	// delete iCreateAndDestroyValidator;
	// iCreateAndDestroyValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CFilterConfigurationIterator_CreateAndDestroy_UnitTest::~CFilterConfigurationIterator_CreateAndDestroy_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	// delete iCreateAndDestroyValidator;
	delete iDtorValidator;
	}

inline CFilterConfigurationIterator_CreateAndDestroy_UnitTest::CFilterConfigurationIterator_CreateAndDestroy_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KFilterConfigurationIteratorCreateAndDestroyUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CFilterConfigurationIterator_CreateAndDestroy_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TFilterConfigurationIterator_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CFilterConfigurationIterator_UnitTestContext(iDataLogger, *iStateAccessor, *this);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on TFilterConfigurationIterator class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TFilterConfigurationIterator_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CFilterConfigurationIterator_NewL_Transition(*iUTContext,*iCtorValidator));
	// iCreateAndDestroyValidator = new(ELeave) TFilterConfigurationIterator_CreateAndDestroy_TransitionValidator(*iUTContext);
	// AddTransitionL(new(ELeave)CFilterConfigurationIterator_CreateAndDestroy_Transition(*iUTContext,*iCreateAndDestroyValidator));
	iDtorValidator = new(ELeave) TFilterConfigurationIterator_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CFilterConfigurationIterator_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KFilterConfigurationIteratorSuccessfulAtStartUnitTest,"CFilterConfigurationIterator_SuccessfulAtStart_UnitTest");

CFilterConfigurationIterator_SuccessfulAtStart_UnitTest* CFilterConfigurationIterator_SuccessfulAtStart_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CFilterConfigurationIterator_SuccessfulAtStart_UnitTest* self = 
					new(ELeave) CFilterConfigurationIterator_SuccessfulAtStart_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CFilterConfigurationIterator_SuccessfulAtStart_UnitTest::RunError(TInt aError)
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
	delete iSuccessfulFirstValidator;
	iSuccessfulFirstValidator = NULL;
	delete iSuccessfulAtStartValidator;
	iSuccessfulAtStartValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CFilterConfigurationIterator_SuccessfulAtStart_UnitTest::~CFilterConfigurationIterator_SuccessfulAtStart_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iSuccessfulFirstValidator;
	delete iSuccessfulAtStartValidator;
	delete iDtorValidator;
	}

inline CFilterConfigurationIterator_SuccessfulAtStart_UnitTest::CFilterConfigurationIterator_SuccessfulAtStart_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KFilterConfigurationIteratorSuccessfulAtStartUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CFilterConfigurationIterator_SuccessfulAtStart_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TFilterConfigurationIterator_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CFilterConfigurationIterator_UnitTestContext(iDataLogger, *iStateAccessor, *this);
	iUTContext->iAtStartExpected = ETrue;
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CFilterConfigurationIterator class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TFilterConfigurationIterator_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CFilterConfigurationIterator_NewL_Transition(*iUTContext,*iCtorValidator));
	iSuccessfulFirstValidator = new(ELeave) TFilterConfigurationIterator_SuccessfulFirst_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CFilterConfigurationIterator_First_Transition(*iUTContext,*iSuccessfulFirstValidator));
	iSuccessfulAtStartValidator = new(ELeave) TFilterConfigurationIterator_SuccessfulAtStart_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CFilterConfigurationIterator_AtStart_Transition(*iUTContext,*iSuccessfulAtStartValidator));
	iDtorValidator = new(ELeave) TFilterConfigurationIterator_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CFilterConfigurationIterator_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KFilterConfigurationIteratorUnsuccessfulAtStartUnitTest,"CFilterConfigurationIterator_UnsuccessfulAtStart_UnitTest");

CFilterConfigurationIterator_UnsuccessfulAtStart_UnitTest* CFilterConfigurationIterator_UnsuccessfulAtStart_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CFilterConfigurationIterator_UnsuccessfulAtStart_UnitTest* self = 
					new(ELeave) CFilterConfigurationIterator_UnsuccessfulAtStart_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CFilterConfigurationIterator_UnsuccessfulAtStart_UnitTest::RunError(TInt aError)
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
	delete iSuccessfulFirstValidator;
	iSuccessfulFirstValidator = NULL;
	delete iSuccessfulNextValidator;
	iSuccessfulNextValidator = NULL;
	delete iAtStartValidator;
	iAtStartValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CFilterConfigurationIterator_UnsuccessfulAtStart_UnitTest::~CFilterConfigurationIterator_UnsuccessfulAtStart_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iSuccessfulFirstValidator;
	delete iSuccessfulNextValidator;
	delete iAtStartValidator;
	delete iDtorValidator;
	}

inline CFilterConfigurationIterator_UnsuccessfulAtStart_UnitTest::CFilterConfigurationIterator_UnsuccessfulAtStart_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KFilterConfigurationIteratorUnsuccessfulAtStartUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CFilterConfigurationIterator_UnsuccessfulAtStart_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TFilterConfigurationIterator_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CFilterConfigurationIterator_UnitTestContext(iDataLogger, *iStateAccessor, *this);
	iUTContext->iAtStartExpected = EFalse;
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CFilterConfigurationIterator class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TFilterConfigurationIterator_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CFilterConfigurationIterator_NewL_Transition(*iUTContext,*iCtorValidator));
	iSuccessfulFirstValidator = new(ELeave) TFilterConfigurationIterator_SuccessfulFirst_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CFilterConfigurationIterator_First_Transition(*iUTContext,*iSuccessfulFirstValidator));
	iSuccessfulNextValidator = new(ELeave) TFilterConfigurationIterator_SuccessfulNext_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CFilterConfigurationIterator_Next_Transition(*iUTContext,*iSuccessfulNextValidator));
	iAtStartValidator = new(ELeave) TFilterConfigurationIterator_SuccessfulAtStart_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CFilterConfigurationIterator_AtStart_Transition(*iUTContext,*iAtStartValidator));
	iDtorValidator = new(ELeave) TFilterConfigurationIterator_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CFilterConfigurationIterator_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KFilterConfigurationIteratorUnsuccessfulFindByDataTypeUnitTest,"CFilterConfigurationIterator_UnsuccessfulFindByDataType_UnitTest");

CFilterConfigurationIterator_UnsuccessfulFindByDataType_UnitTest* CFilterConfigurationIterator_UnsuccessfulFindByDataType_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CFilterConfigurationIterator_UnsuccessfulFindByDataType_UnitTest* self = 
					new(ELeave) CFilterConfigurationIterator_UnsuccessfulFindByDataType_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CFilterConfigurationIterator_UnsuccessfulFindByDataType_UnitTest::RunError(TInt aError)
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
	delete iFindByDataTypeValidator;
	iFindByDataTypeValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CFilterConfigurationIterator_UnsuccessfulFindByDataType_UnitTest::~CFilterConfigurationIterator_UnsuccessfulFindByDataType_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iFindByDataTypeValidator;
	delete iDtorValidator;
	}

inline CFilterConfigurationIterator_UnsuccessfulFindByDataType_UnitTest::CFilterConfigurationIterator_UnsuccessfulFindByDataType_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KFilterConfigurationIteratorUnsuccessfulFindByDataTypeUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CFilterConfigurationIterator_UnsuccessfulFindByDataType_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TFilterConfigurationIterator_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CFilterConfigurationIterator_UnitTestContext(iDataLogger, *iStateAccessor, *this);
	iUTContext->iFindByDataTypeExpected = KErrNotFound;
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CFilterConfigurationIterator class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TFilterConfigurationIterator_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CFilterConfigurationIterator_NewL_Transition(*iUTContext,*iCtorValidator));
	iFindByDataTypeValidator = new(ELeave) TFilterConfigurationIterator_FindByDataType_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CFilterConfigurationIterator_FindByDataType_Transition(*iUTContext,*iFindByDataTypeValidator));
	iDtorValidator = new(ELeave) TFilterConfigurationIterator_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CFilterConfigurationIterator_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KFilterConfigurationIteratorProtocolList_DEFECT_EXT5BEGTYUnitTest,"CFilterConfigurationIterator_ProtocolList_DEFECT_EXT5BEGTY_UnitTest");

CFilterConfigurationIterator_ProtocolList_DEFECT_EXT5BEGTY_UnitTest* CFilterConfigurationIterator_ProtocolList_DEFECT_EXT5BEGTY_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CFilterConfigurationIterator_ProtocolList_DEFECT_EXT5BEGTY_UnitTest* self = 
					new(ELeave) CFilterConfigurationIterator_ProtocolList_DEFECT_EXT5BEGTY_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CFilterConfigurationIterator_ProtocolList_DEFECT_EXT5BEGTY_UnitTest::RunError(TInt aError)
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
	delete iProtocolList_DEFECT_EXT5BEGTYValidator;
	iProtocolList_DEFECT_EXT5BEGTYValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CFilterConfigurationIterator_ProtocolList_DEFECT_EXT5BEGTY_UnitTest::~CFilterConfigurationIterator_ProtocolList_DEFECT_EXT5BEGTY_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iProtocolList_DEFECT_EXT5BEGTYValidator;
	delete iDtorValidator;
	}

inline CFilterConfigurationIterator_ProtocolList_DEFECT_EXT5BEGTY_UnitTest::CFilterConfigurationIterator_ProtocolList_DEFECT_EXT5BEGTY_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KFilterConfigurationIteratorProtocolList_DEFECT_EXT5BEGTYUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CFilterConfigurationIterator_ProtocolList_DEFECT_EXT5BEGTY_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TFilterConfigurationIterator_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CFilterConfigurationIterator_UnitTestContext(iDataLogger, *iStateAccessor, *this);
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CFilterConfigurationIterator class.
		using constructor and destuctor validators */
	iProtocolList_DEFECT_EXT5BEGTYValidator = new(ELeave) TFilterConfigurationIterator_SessionListProtocols_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CFilterConfigurationIterator_ListProtocols_Transition(*iUTContext,*iProtocolList_DEFECT_EXT5BEGTYValidator));
	}

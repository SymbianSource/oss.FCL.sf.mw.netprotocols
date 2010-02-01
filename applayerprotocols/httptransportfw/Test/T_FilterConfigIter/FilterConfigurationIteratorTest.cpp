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
// test the TFilterConfigurationIterator class using the test bed.
// It may be used as a basis to develop a full test bed dll.
// For support and comment please contact the authors.
// 
//

/**
 @file FilterConfigurationIteratorTest.cpp
*/
#include "FilterConfigurationIteratorUnitTest.h"
#include "ComponentTester.h"

// ______________________________________________________________________________
//
/**
	@internal
	@class CFilterConfigurationIterator_ComponentTest
	@Depends TFilterConfigurationIterator & CComponentTester 

	Comments : Test the TFilterConfigurationIterator class.
 */
class CFilterConfigurationIterator_ComponentTest : public CComponentTester
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
		@return			CFilterConfigurationIterator_ComponentTest* The constructed object.
		@pre 			None.
		@post			CFilterConfigurationIterator_ComponentTest is fully constructed.
	*/
	static CFilterConfigurationIterator_ComponentTest* NewLC(CDataLogger& aDataLogger, 
							MComponentTestObserver& aObserver);

private:
	/**
		@fn				CFilterConfigurationIterator_ComponentTest(CDataLogger& aDataLogger,
											MComponentTestObserver& aObserver)
		Intended Usage	: Standard c'tor method.
		Error Condition	: None.
		@since			7.0
		@param			aDataLogger The logging object.
		@param			aObserver The observer of this component test.
		@pre 			None.
		@post			CFilterConfigurationIterator_ComponentTest is fully constructed.
	*/
	inline CFilterConfigurationIterator_ComponentTest(CDataLogger& aDataLogger,
												MComponentTestObserver& aObserver);
	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
						to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory.
		@since			7.0
		@return			None 
		@pre 			CFilterConfigurationIterator_ComponentTest is fully constructed.
		@post			CFilterConfigurationIterator_ComponentTest is fully initialized.
	*/
	inline void ConstructL();

	};	// CFilterConfigurationIterator_ComponentTest

// ______________________________________________________________________________
//
inline CFilterConfigurationIterator_ComponentTest* CFilterConfigurationIterator_ComponentTest::NewLC(CDataLogger& aDataLogger, MComponentTestObserver& aObserver)
	{
	CFilterConfigurationIterator_ComponentTest* self = new (ELeave) CFilterConfigurationIterator_ComponentTest(aDataLogger, aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

inline CFilterConfigurationIterator_ComponentTest::CFilterConfigurationIterator_ComponentTest(CDataLogger& aDataLogger,
									MComponentTestObserver& aObserver)
: CComponentTester(aDataLogger, aObserver)
	{
	// Do nothing here.
	}

inline void CFilterConfigurationIterator_ComponentTest::ConstructL()
	{
	// Perform base class initialization
	ComponentTesterConstructL();

	AddUnitTestL(CFilterConfigurationIterator_CreateAndDestroy_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CFilterConfigurationIterator_SuccessfulAtStart_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CFilterConfigurationIterator_UnsuccessfulAtStart_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CFilterConfigurationIterator_UnsuccessfulFindByDataType_UnitTest::NewL(iDataLogger, *this));
	AddUnitTestL(CFilterConfigurationIterator_ProtocolList_DEFECT_EXT5BEGTY_UnitTest::NewL(iDataLogger, *this));
	}

// ______________________________________________________________________________
//
EXPORT_C CComponentTester* NewComponentTestLC(CDataLogger& aDataLogger,
									MComponentTestObserver&	aComponentTestObserver)
	{
	return CFilterConfigurationIterator_ComponentTest::NewLC(aDataLogger, aComponentTestObserver);
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


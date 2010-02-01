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
// The definition of the accessor class upon the TFilterConfigurationIterator
// 
//

/**
 @file FilterConfigurationIteratorStateAccessor.h
*/

#ifndef __FILTERCONFIGURATIONITERATORSTATEACCESSOR_H__
#define __FILTERCONFIGURATIONITERATORSTATEACCESSOR_H__

#include <e32base.h>
#include <f32file.h>
#include <s32stor.h>
#include <s32file.h>

#include "TestUtilities.h"
#include "TestBedDefinitions.h"
#include "StateAccessor.h"
#include <http/tfilterconfigurationiter.h>

// ______________________________________________________________________________
//
/**
	@internal
	@class TFilterConfigurationIterator_StateAccessor
	@Depends TFilterConfigurationIterator & MStateAccessor 

	Comments : State accessor for the TFilterConfigurationIterator object under test.
			 Please Note : There are a number of overloads for the Internalize and Externalize methods.
			 This allows other classes to aggregate state accessors for their internal classes, providing
			 the ability to capture the complete state of an instantiation hierarchy.
 */
class TFilterConfigurationIterator_StateAccessor : public MStateAccessor
	{
public:
	/**
		@fn				InvariantTest(TAny* aTestObject)
		Intended Usage	: To check TFilterConfigurationIterator for invariant violation.
		@since			7.0
		@param			aTestObject A pointer to the object under test.
		@return			KErrNone if the invariants were valid, error code otherwise.
		@pre 			TFilterConfigurationIterator_StateAccessor is fully constructed.
		@post			The TFilterConfigurationIterator class under test has been checked for
						for invariance.
	 */
	inline TInt InvariantTest(TAny* aTestObject);

	/**
		@fn				Internalize(TAny* aTestObject)
		Intended Usage	: @todo Implemented by the developer to restore the state of the object under test.
		@since			7.0
		@param			aTestObject The object being tested
		@return			KErrNone if the internalize was successful.
		@return			One of the EPOC standard error codes indicating the reason
						for the failure	to load the object state.
		@pre 			TFilterConfigurationIterator_StateAccessor is fully constructed, and
						valid test object must be passed in.
		@post			The TFilterConfigurationIterator class under test has internalized its
						persistent stream data.
	 */
	inline virtual TInt Internalize(TAny* aTestObject);

	/**
		@fn				Externalize(TAny* aTestObject)
		Intended Usage	: @todo Implemented by the developer to persist the state of the 
						TFilterConfigurationIterator object under test.
		@since			7.0
		@param			aTestObject The object being tested
		@return			KErrNone if the externalize was successful.
		@return			One of the EPOC standard error codes indicating the reason
						for the failure	to externalize the object.
		@pre 			TFilterConfigurationIterator_StateAccessor is fully constructed, and
						valid test object must be passed in.
		@post			The TFilterConfigurationIterator class under test has externalized its
						persistent stream data.
	 */
	inline virtual TInt Externalize(TAny* aTestObject);

	/**
		@fn				InternalizeL(RFileReadStream& aStream, TFilterConfigurationIterator* aFilterConfigurationIterator)
		Intended Usage	: Implemented by the developer to set the state of the 
						TFilterConfigurationIterator object under test.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory
		@exception		@see RFileReadStream
		@since			7.0
		@param			aStream The filestream to read from .
		@param			aFilterConfigurationIterator A pointer to the TFilterConfigurationIterator object under test.
		@return			None.
		@pre 			TFilterConfigurationIterator_StateAccessor is fully constructed.
		@post			The TFilterConfigurationIterator class under test has internalized its persisted state.
	 */
		inline void InternalizeL(RFileReadStream& aStream, TFilterConfigurationIterator* aFilterConfigurationIterator);

	/**
		@fn				ExternalizeL(RFileWriteStream& aStream, TFilterConfigurationIterator* aFilterConfigurationIterator)
		Intended Usage	: Implemented by the developer to persist the state of the 
						TFilterConfigurationIterator object under test.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory
		@exception		@see RFileWriteStream
		@since			7.0
		@param			aStream The filestream to persist into.
		@param			aFilterConfigurationIterator A pointer to the TFilterConfigurationIterator object under test.
		@return			None.
		@pre 			TFilterConfigurationIterator_StateAccessor is fully constructed.
		@post			No change to the TFilterConfigurationIterator class under test, its internal state.
						is persisted.
	*/
	inline void ExternalizeL(RFileWriteStream& aStream, TFilterConfigurationIterator* aFilterConfigurationIterator);

private:
	/**
		@fn				InternalizeL(TFilterConfigurationIterator* aFilterConfigurationIterator)
		Intended Usage	: Implemented by the developer to set the state of the 
						TFilterConfigurationIterator object under test.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory
		@exception		@see RFileReadStream
		@since			7.0
		@param			aFilterConfigurationIterator A pointer to the TFilterConfigurationIterator object under test.
		@pre 			TFilterConfigurationIterator_StateAccessor is fully constructed.
		@post			The TFilterConfigurationIterator class under test has internalized its persisted state.
	*/
	inline void InternalizeL(TFilterConfigurationIterator* aFilterConfigurationIterator);

	/**
		@fn				ExternalizeL(TFilterConfigurationIterator* aFilterConfigurationIterator)
		Intended Usage	: Implemented by the developer to persist the state of the 
						TFilterConfigurationIterator object under test.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory
		@exception		@see RFileWriteStream
		@since			7.0
		@param			aFilterConfigurationIterator A pointer to the TFilterConfigurationIterator object under test.
		@pre 			TFilterConfigurationIterator_StateAccessor is fully constructed.
		@post			No change to the TFilterConfigurationIterator class under test, its internal state.
						is persisted.
	*/
	inline void ExternalizeL(TFilterConfigurationIterator* aFilterConfigurationIterator);
	};	// TFilterConfigurationIterator_StateAccessor

#include "FilterConfigurationIteratorStateAccessor.inl"

#endif // __FILTERCONFIGURATIONITERATORSTATEACCESSOR_H__


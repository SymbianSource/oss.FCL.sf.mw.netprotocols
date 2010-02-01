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
// The implementation of the accessor class upon the TFilterConfigurationIterator
// 
//

/**
 @file FilterConfigurationIteratorStateAccessor.inl
*/

// ______________________________________________________________________________
//
_LIT(KTFilterConfigurationIterator_DumpName,"C:\\System\\Data\\Logs\\TFilterConfigurationIterator_StateDump.bin");

inline TInt TFilterConfigurationIterator_StateAccessor::InvariantTest(TAny* aTestObject)
	{
	if(aTestObject)
		{
		}
	return KErrNone;
	}

inline TInt TFilterConfigurationIterator_StateAccessor::Internalize(TAny* aTestObject)
	{
	TInt error = KErrNone;
	if(aTestObject != NULL)
		{
		TFilterConfigurationIterator* theFilterConfigurationIterator = REINTERPRET_CAST(TFilterConfigurationIterator*, aTestObject);
		TRAP(error,InternalizeL(theFilterConfigurationIterator));
		}
	return error;
	}

inline TInt TFilterConfigurationIterator_StateAccessor::Externalize(TAny* aTestObject)
	{
	TInt error = KErrNone;
	if(aTestObject != NULL)
		{
		TFilterConfigurationIterator* theFilterConfigurationIterator = REINTERPRET_CAST(TFilterConfigurationIterator*, aTestObject);
		TRAP(error,ExternalizeL(theFilterConfigurationIterator));
		}
	return error;
	}

inline void TFilterConfigurationIterator_StateAccessor::InternalizeL(RFileReadStream& /* aStream */, TFilterConfigurationIterator* /* aFilterConfigurationIterator */)
	{
	}

inline void TFilterConfigurationIterator_StateAccessor::ExternalizeL(RFileWriteStream& /* aStream */, TFilterConfigurationIterator* /* aFilterConfigurationIterator */)
	{
	}

// internal helpers
inline void TFilterConfigurationIterator_StateAccessor::InternalizeL(TFilterConfigurationIterator* aFilterConfigurationIterator)
	{
	// Read in the dumped data
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);
	// Read the object dump
	TParse filename;
	filename.Set(KTFilterConfigurationIterator_DumpName(),NULL,NULL);
	RFileReadStream stream;
	User::LeaveIfError(stream.Open(fs,filename.FullName(), EFileRead));
	CleanupClosePushL(stream);
	InternalizeL(stream, aFilterConfigurationIterator);
	CleanupStack::PopAndDestroy(2);	// stream & fs
	}

inline void TFilterConfigurationIterator_StateAccessor::ExternalizeL(TFilterConfigurationIterator* aFilterConfigurationIterator)
	{
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);
	// Write out the object dump
	TParse filename;
	filename.Set(KTFilterConfigurationIterator_DumpName(),NULL,NULL);
	RFileWriteStream stream;
	User::LeaveIfError(stream.Replace(fs,filename.FullName(), EFileWrite));
	CleanupClosePushL(stream);
	ExternalizeL(stream, aFilterConfigurationIterator);
	stream.CommitL();
	CleanupStack::PopAndDestroy(2);	// stream & fs
	}


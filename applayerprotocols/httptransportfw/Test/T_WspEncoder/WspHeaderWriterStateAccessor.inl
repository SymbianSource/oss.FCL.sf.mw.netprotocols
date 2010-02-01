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
// The implementation of the accessor class upon the CWspHeaderWriter
// 
//

/**
 @file WspHeaderWriterStateAccessor.inl
*/

// ______________________________________________________________________________
//
_LIT(KTWspHeaderWriter_DumpName,"C:\\System\\Data\\Logs\\TWspHeaderWriter_StateDump.bin");

inline TInt TWspHeaderWriter_StateAccessor::InvariantTest(TAny* aTestObject)
	{
	if(aTestObject)
		{
		// CWspHeaderWriter* theWspHeaderWriter = REINTERPRET_CAST(CWspHeaderWriter*, aTestObject);
		/** @todo Implement the correct Invariant test for CWspHeaderWriter. */
#pragma message( __FILE__LINE__ "TODO : Implement the correct Invariant test for CWspHeaderWriter.")
		}
	return KErrNone;
	}

inline TInt TWspHeaderWriter_StateAccessor::Internalize(TAny* aTestObject)
	{
	TInt error = KErrNone;
	if(aTestObject != NULL)
		{
		CWspHeaderWriter* theWspHeaderWriter = REINTERPRET_CAST(CWspHeaderWriter*, aTestObject);
		TRAP(error,InternalizeL(theWspHeaderWriter));
		}
	return error;
	}

inline TInt TWspHeaderWriter_StateAccessor::Externalize(TAny* aTestObject)
	{
	TInt error = KErrNone;
	if(aTestObject != NULL)
		{
		CWspHeaderWriter* theWspHeaderWriter = REINTERPRET_CAST(CWspHeaderWriter*, aTestObject);
		TRAP(error,ExternalizeL(theWspHeaderWriter));
		}
	return error;
	}

inline void TWspHeaderWriter_StateAccessor::InternalizeL(RFileReadStream& /* aStream */, CWspHeaderWriter* /* aWspHeaderWriter */)
	{
	/** @todo Implement the correct state reload for CWspHeaderWriter. */
#pragma message( __FILE__LINE__ "TODO : Implement the correct state reload for CWspHeaderWriter.")
	}

inline void TWspHeaderWriter_StateAccessor::ExternalizeL(RFileWriteStream& /* aStream */, CWspHeaderWriter* /* aWspHeaderWriter */)
	{
	/** @todo Implement the correct state persistence for CWspHeaderWriter. */
#pragma message( __FILE__LINE__ "TODO : Implement the correct state persistence for CWspHeaderWriter.")
	}

// internal helpers
inline void TWspHeaderWriter_StateAccessor::InternalizeL(CWspHeaderWriter* aWspHeaderWriter)
	{
	// Read in the dumped data
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);
	// Read the object dump
	TParse filename;
	filename.Set(KTWspHeaderWriter_DumpName(),NULL,NULL);
	RFileReadStream stream;
	User::LeaveIfError(stream.Open(fs,filename.FullName(), EFileRead));
	CleanupClosePushL(stream);
	InternalizeL(stream, aWspHeaderWriter);
	CleanupStack::PopAndDestroy(2);	// stream & fs
	}

inline void TWspHeaderWriter_StateAccessor::ExternalizeL(CWspHeaderWriter* aWspHeaderWriter)
	{
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);
	// Write out the object dump
	TParse filename;
	filename.Set(KTWspHeaderWriter_DumpName(),NULL,NULL);
	RFileWriteStream stream;
	User::LeaveIfError(stream.Replace(fs,filename.FullName(), EFileWrite));
	CleanupClosePushL(stream);
	ExternalizeL(stream, aWspHeaderWriter);
	stream.CommitL();
	CleanupStack::PopAndDestroy(2);	// stream & fs
	}


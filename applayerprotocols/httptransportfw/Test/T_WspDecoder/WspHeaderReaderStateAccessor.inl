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
// The implementation of the accessor class upon the CWspHeaderReader
// 
//

/**
 @file WspHeaderReaderStateAccessor.inl
*/

// ______________________________________________________________________________
//
_LIT(KTWspHeaderReader_DumpName,"C:\\System\\Data\\Logs\\TWspHeaderReader_StateDump.bin");

inline TInt TWspHeaderReader_StateAccessor::InvariantTest(TAny* aTestObject)
	{
	if(aTestObject)
		{
		// CWspHeaderReader* theWspHeaderReader = REINTERPRET_CAST(CWspHeaderReader*, aTestObject);
		/** @todo Implement the correct Invariant test for CWspHeaderReader. */
#pragma message( __FILE__LINE__ "TODO : Implement the correct Invariant test for CWspHeaderReader.")
		}
	return KErrNone;
	}

inline TInt TWspHeaderReader_StateAccessor::Internalize(TAny* aTestObject)
	{
	TInt error = KErrNone;
	if(aTestObject != NULL)
		{
		CWspHeaderReader* theWspHeaderReader = REINTERPRET_CAST(CWspHeaderReader*, aTestObject);
		TRAP(error,InternalizeL(theWspHeaderReader));
		}
	return error;
	}

inline TInt TWspHeaderReader_StateAccessor::Externalize(TAny* aTestObject)
	{
	TInt error = KErrNone;
	if(aTestObject != NULL)
		{
		CWspHeaderReader* theWspHeaderReader = REINTERPRET_CAST(CWspHeaderReader*, aTestObject);
		TRAP(error,ExternalizeL(theWspHeaderReader));
		}
	return error;
	}

inline void TWspHeaderReader_StateAccessor::InternalizeL(RFileReadStream& /* aStream */, CWspHeaderReader* /* aWspHeaderReader */)
	{
	/** @todo Implement the correct state reload for CWspHeaderReader. */
#pragma message( __FILE__LINE__ "TODO : Implement the correct state reload for CWspHeaderReader.")
	}

inline void TWspHeaderReader_StateAccessor::ExternalizeL(RFileWriteStream& /* aStream */, CWspHeaderReader* /* aWspHeaderReader */)
	{
	/** @todo Implement the correct state persistence for CWspHeaderReader. */
#pragma message( __FILE__LINE__ "TODO : Implement the correct state persistence for CWspHeaderReader.")
	}

// internal helpers
inline void TWspHeaderReader_StateAccessor::InternalizeL(CWspHeaderReader* aWspHeaderReader)
	{
	// Read in the dumped data
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);
	// Read the object dump
	TParse filename;
	filename.Set(KTWspHeaderReader_DumpName(),NULL,NULL);
	RFileReadStream stream;
	User::LeaveIfError(stream.Open(fs,filename.FullName(), EFileRead));
	CleanupClosePushL(stream);
	InternalizeL(stream, aWspHeaderReader);
	CleanupStack::PopAndDestroy(2);	// stream & fs
	}

inline void TWspHeaderReader_StateAccessor::ExternalizeL(CWspHeaderReader* aWspHeaderReader)
	{
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);
	// Write out the object dump
	TParse filename;
	filename.Set(KTWspHeaderReader_DumpName(),NULL,NULL);
	RFileWriteStream stream;
	User::LeaveIfError(stream.Replace(fs,filename.FullName(), EFileWrite));
	CleanupClosePushL(stream);
	ExternalizeL(stream, aWspHeaderReader);
	stream.CommitL();
	CleanupStack::PopAndDestroy(2);	// stream & fs
	}


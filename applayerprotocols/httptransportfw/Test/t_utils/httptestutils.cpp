// Copyright (c) 2001-2010 Nokia Corporation and/or its subsidiary(-ies).
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

#include "httptestutils.h"
#include "HttpTestCore.h"

#if !defined __E32BASE_H__
#include <e32base.h>
#endif

// for StartC32()
#if !defined __C32COMM_H__
#include <c32comm.h>
#endif
#if !defined __IN_SOCK_H__
#include <in_sock.h>
#endif


//
//	Constants
_LIT(KTestPanic, "HttpTestUtils");
const TInt KFailedTestsGranularity = 10;
const TInt KMaxLogEntrySize = 256;
#if defined (__WINS__)
#define PDD_NAME		_L("ECDRV")
#else
#define PDD_NAME		_L("EUART1")
#define PDD2_NAME		_L("EUART2")
#define PDD3_NAME		_L("EUART3")
#define PDD4_NAME		_L("EUART4")
#endif
#define LDD_NAME		_L("ECOMM")

const TInt KMaxTempBuffize = 256;


_LIT(KTestFileInputBase, "HttpTest");
_LIT(KPathSep,"\\");

//
//	CHTTPTestUtils
//
CHTTPTestUtils::CHTTPTestUtils(const TDesC& aTitle)
	: iTest(aTitle)
//
//	Default c'tor
	{
	}

CHTTPTestUtils::~CHTTPTestUtils()
//
//	D'tor
	{
	TTime endtime;
	endtime.UniversalTime();

	// Do resource handle leak test?
	if (iDoResourceLeakTest)
		ResourceLeakTest();

	//	End of tests - see if failed or ok
	if ( iFailedTests )
		{
		if (iFailedTests->Count())
			{
			TestHarnessFailed();
			}
		else
			{
			TestHarnessComplete();
			}
		}
		
	//	Log finish time
	TDateTime t = endtime.DateTime();
	LogIt(_L("Ended @ %d:%d:%d:%d"),t.Hour(),t.Minute(),t.Second(),t.MicroSecond());
	TTime difftime(endtime.Int64() - iStartTime.Int64());
	t = difftime.DateTime();
	LogIt(_L("Execution time %d:%d:%d:%d"),t.Hour(),t.Minute(),t.Second(),t.MicroSecond());

	//	Close test harness
	iTest.Close();

	CleanupFailedTestsArray ();
	// Close fileserver connection
	iFs.Close();
	}
EXPORT_C void CHTTPTestUtils::CreateFailedTestsArrayL ()
	{
	if ( iFailedTests )
		{
		CleanupFailedTestsArray ();
		}
	//	Create iFailedTests
	iFailedTests = new (ELeave) CArrayPtrFlat<CTestInfo> (KFailedTestsGranularity);	
	}
EXPORT_C void CHTTPTestUtils::CleanupFailedTestsArray ()
	{
	//	Cleanup
	if ( iFailedTests )
		{
	iFailedTests->ResetAndDestroy();
		iFailedTests->Reset ();
		}
	delete iFailedTests;

	// Close fileserver connection
	iFailedTests = NULL;
	}

EXPORT_C CHTTPTestUtils* CHTTPTestUtils::NewLC(const TDesC& aTitle)
//
//	Static factory c'tor
	{
	CHTTPTestUtils* self = new (ELeave) CHTTPTestUtils(aTitle);
	CleanupStack::PushL(self);
	self->ConstructL(aTitle);
	return self;
	}

EXPORT_C CHTTPTestUtils* CHTTPTestUtils::NewL(const TDesC& aTitle)
//
//	Static factiry c'tor
	{
	CHTTPTestUtils* self = CHTTPTestUtils::NewLC(aTitle);
	CleanupStack::Pop();
	return self;
	}


void CHTTPTestUtils::CreateLogFiles(const TDesC& aTestName)
	{
	TBuf<128> logName(aTestName);
	DefaultLogFileName(logName);
	__OPEN_LOG2(logName);
	logName.Append(_L(".SUMMARY"));

	TInt error = iTestSummaryLogger.Connect();
	if(error == KErrNone)
		{
		TParse p;
		p.Set(logName, NULL, NULL);
		iTestSummaryLogger.CreateLog(KHttpLogsDirName, p.NameAndExt(), EFileLoggingModeOverwrite);
		iTestSummaryLogger.SetDateAndTime(ETrue, ETrue);
		iTestSummaryLogger.Write(KTestHeader);
		}
	else
		User::InfoPrint(_L("Log file create failed"));
	}

void CHTTPTestUtils::ConstructL(const TDesC& aTitle)
//
//	Non-trivial c'tor
	{
	CConsoleBase* console = Console::NewL ( aTitle, TSize(KConsFullScreen, KConsFullScreen) );
	iTest.SetConsole ( console );
	iTest.Title();
	iCanStartTest = ETrue;
	iCurrentTest=NULL;
	//	Create iFailedTests
	CreateFailedTestsArrayL ();

	//	Start up logging server connection
	CreateLogFiles(aTitle);

	iStartTime.UniversalTime();
	TDateTime t = iStartTime.DateTime();
	LogIt(_L("Started @ %d:%d:%d:%d"),t.Hour(),t.Minute(),t.Second(),t.MicroSecond());

	// Find number of open resource handles
	TInt processHandleCount=0;
	RThread().HandleCount(processHandleCount,iStartHandleCount);

	// Connect to file server for test scripts, ini-files etc.
	User::LeaveIfError(iFs.Connect());
	}

EXPORT_C void CHTTPTestUtils::JustStartTestL(const TDesC& aName)
	{
	if (iCanStartTest)
		{
		iCurrentTest= CTestInfo::NewL(aName, iTestCount, 0);
		//	Stop new test being started until this one has ended
		iCanStartTest = EFalse;
		}
	else
		{
		//	Panic client - bad usage - not allowed to nest tests
		Panic(EBadStartTest);
		}

	}
EXPORT_C void CHTTPTestUtils::StartTestL(const TDesC& aName)
//
//	Logs start of test aName
	{
	if (iCanStartTest)
		{
		// Add this test to failed test list - set errorcode to zero
		CTestInfo* temp = CTestInfo::NewLC(aName, iTestCount + 1, 0);
		iFailedTests->AppendL(temp);
		CleanupStack::Pop();	//	temp
		iCurrentTest= temp;

		// Increase RTest's test depth by Start-ing - increment test count
		iTest.Start(aName);
		++iTestCount;

		// Stop new test being started until this one has ended
		iCanStartTest = EFalse;

		TBuf<KMaxFileName + 4> buf;
		buf.Format(KTestStartingWithDesc, iTestCount, &aName);
		WriteComment(buf);

		// Reset iStepNumber - start at 1
		iStepNumber = 1;
		}
	else
		{
		// Panic client - bad usage - not allowed to nest tests
		Panic(EBadStartTest);
		}
	}

EXPORT_C void CHTTPTestUtils::NextStep(const TDesC& aStepName)
//
//	Logs the next step in a test - for informative use.
	{
	if (!iCanStartTest)
		{
		TBuf<KMaxFileName + 4> buf;
		buf.Format(KNextTestStepWithDesc, iTestCount, iStepNumber, &aStepName);
		WriteComment(buf);
		iTest.Next(aStepName);
		++iStepNumber;
		}
	else
		{
		//	Panic client - bad usage - test not started
		Panic(EBadStartTest);
		}
	}

EXPORT_C void CHTTPTestUtils::JustEndTest()
	{
	if ( !iCanStartTest )
		{
		iTest.End ();
		}
	//	Allow new test to start
	iCurrentTest = NULL;
	iCanStartTest = ETrue;
	}

EXPORT_C void CHTTPTestUtils::EndTest(TInt aErrorCode)
//
//	Logs end of test
	{
	if (!iCanStartTest)
		{
		//	Get ptr to this test's entry in failed list - will be the last entry
		TBuf<KMaxFileName + 4> buf;
		TInt index = iFailedTests->Count();
	    CTestInfo* ptr = iFailedTests->At(--index);
		if (aErrorCode)
			{
			//	Set the error code
			ptr->SetErrorCode(aErrorCode);
			buf.Format(KTestFailed, iTestCount, aErrorCode);
			iTest.Printf(KRTestFailed, aErrorCode);
			}
		else
			{
			//	Remove entry from list of failed tests
			delete ptr;
			iFailedTests->Delete(index);
			buf.Format(KTestPassed, iTestCount);
			}
		WriteComment(buf);

		// Decrease RTest's test depth by End-ing
		iTest.End();

		//	Allow new test to start. There's no iCurrentTest until StartTestL is invoked again
		iCurrentTest = NULL;
		iCanStartTest = ETrue;
		}
	else
		{
		//	Panic client - bad usage - test not started
		Panic(EBadEndTest);
		}
	}

EXPORT_C void CHTTPTestUtils::LogIt(TRefByValue<const TDesC> aFmt, ...)

//	Messages to the front end emulator and to the WAP log
	{
	VA_LIST list;
	VA_START(list,aFmt);
	TBuf<2*KMaxFileName> buf;
	buf.Zero();
	if(iCurrentTest)
		buf.Append(iCurrentTest->Name());
	buf.Append(_L(">  "));
	buf.AppendFormatList(aFmt,list,this);
	VA_END(list);
	//write to the console and file, if not silent
	if (!iSilent)
		WriteComment(buf);
	else	// otherwise log to the file only
		__LOG(buf);
	}

EXPORT_C void CHTTPTestUtils::Overflow(TDes& aDes)
	{
	_LIT(KErrOverflowMsg, "...");
	if( aDes.MaxLength() >= KErrOverflowMsg().Length() + aDes.Length() )
		aDes.Append(KErrOverflowMsg);
	}

EXPORT_C void CHTTPTestUtils::operator()(TInt aResult,TInt aLineNum)
//
//	Overload operator ()
	{
	iTest(aResult, aLineNum);
	}

EXPORT_C void CHTTPTestUtils::operator()(TInt aResult)
//
//	Overload operator ()
	{
	iTest(aResult);
	}

EXPORT_C void CHTTPTestUtils::PressAnyKey()
//
//	Request a key press from user and wait - unless we are running a script or we are being silent
	{
	if (!iScriptRunning && !iSilent)
		{
		iTest.Printf(TRefByValue<const TDesC>_L("\nPress a key"));	
		iTest.Getch();
		}
	}

EXPORT_C void CHTTPTestUtils::DumpData(const TDesC8& aData, TBool logIt)
//
//	Do a formatted dump of binary data, optionally logging it
	{
	// Iterate the supplied block of data in blocks of 16 bytes
	TInt pos = 0;
	TBuf<KMaxLogEntrySize> logLine;
	TBuf<KMaxLogEntrySize> anEntry;
	while (pos < aData.Length())
		{
		anEntry.Format(TRefByValue<const TDesC>_L("%04x : "), pos);
		logLine.Append(anEntry);

		// Hex output
		TInt offset;
		for (offset = 0; offset < 16; offset++)
			{
			if (pos + offset < aData.Length())
				{
				TInt nextByte = aData[pos + offset];
				anEntry.Format(TRefByValue<const TDesC>_L("%02x "), nextByte);
				logLine.Append(anEntry);
				}
			else
				{
				anEntry.Format(TRefByValue<const TDesC>_L("   "));
				logLine.Append(anEntry);
				}
			}
			anEntry.Format(TRefByValue<const TDesC>_L(": "));
			logLine.Append(anEntry);

		// Char output
		for (offset = 0; offset < 16; offset++)
			{
			if (pos + offset < aData.Length())
				{
				TInt nextByte = aData[pos + offset];
				if ((nextByte >= 32) && (nextByte <= 127))
					{
					anEntry.Format(TRefByValue<const TDesC>_L("%c"), nextByte);
					logLine.Append(anEntry);
					}
				else
					{
					anEntry.Format(TRefByValue<const TDesC>_L("."));
					logLine.Append(anEntry);
					}
				}
			else
				{
				anEntry.Format(TRefByValue<const TDesC>_L(" "));
				logLine.Append(anEntry);
				}
			}
			if (logIt)
				{
				LogIt(TRefByValue<const TDesC>_L("%S"), &logLine);
				}
			else if (!iSilent)
				{
				iTest.Printf(TRefByValue<const TDesC>_L("%S\n"), &logLine);	
				}
			logLine.Zero();

		// Advance to next 16 byte segment
		pos += 16;
		}
	}

EXPORT_C void CHTTPTestUtils::GetAnEntry(const TDesC& ourPrompt, TDes& currentstring)
//
//	Get an input string from the user, displaying a supplied prompt and default string value
	{
	// If we're scripting, try reading from script first
	TInt readScriptErr = KErrNotFound;
	if (iScriptRunning)
		{
		readScriptErr = ReadLineFromScript(currentstring);
		}
	if (!readScriptErr)
		return;

	// Either not scripting, or hit end of script - continue with user input
	TBuf16<KMaxUserEntrySize> ourLine;
	TBuf<KMaxUserEntrySize> tempstring;				//tempstring is a unicode descriptor
										//create a temporary buffer where the
										//unicode strings are stored in order to 
										//be displayed
	ourLine.Zero ();
	tempstring.Copy(currentstring);		//Copy current string to Unicode buffer
	TKeyCode key = EKeyNull;						//current string buffer is 8 bits wide.
										//Unicode string bufffer (tempstring) is 16 bits wide.
	for (;;)
		{
		if (ourLine.Length () == 0)
			{
			iTest.Console()->SetPos (0, iTest.Console()->WhereY ());
			iTest.Console()->Printf (_L ("%S"), &ourPrompt);
			if (tempstring.Length () != 0)						//get tempstring's number of items
				iTest.Console()->Printf (_L (" = %S"), &tempstring);	//if not zero print them to iTest.Console()
			iTest.Console()->Printf (_L (" : "));
			iTest.Console()->ClearToEndOfLine ();
			}
		key = iTest.Getch();
		
		  if (key == EKeyBackspace)
				{
					if (ourLine.Length() !=0)
					{
						ourLine.SetLength(ourLine.Length()-1);
						iTest.Console()->Printf (_L ("%c"), key);
						iTest.Console()->SetPos(iTest.Console()->WhereX(),iTest.Console()->WhereY());
						iTest.Console()->ClearToEndOfLine();
					}	// end if (ourLine.Length() !=0)
				}	// end if (key == KeyBackSpace)
		  
		  		  
		  if (key == EKeyDelete) 			
				{
					ourLine.Zero();
					iTest.Console()->SetPos (0, iTest.Console()->WhereY ());
					iTest.Console()->ClearToEndOfLine ();
					tempstring.Copy(ourLine);
					break;
				}
		  
		  if (key == EKeyEnter)
			break;
		
		  if (key < 32)
			{
			continue;
			}
		
		ourLine.Append (key);
		iTest.Console()->Printf (_L ("%c"), key);
		iTest.Console()->SetPos(iTest.Console()->WhereX(),iTest.Console()->WhereY());
		iTest.Console()->ClearToEndOfLine();
		if (ourLine.Length () == ourLine.MaxLength ())
			break;
		}	// end of for statement

	if ((key == EKeyEnter) && (ourLine.Length () == 0))
		tempstring.Copy (currentstring);				//copy contents of 8 bit "ourLine" descriptor
	
	iTest.Console()->SetPos (0, iTest.Console()->WhereY ());		
	iTest.Console()->ClearToEndOfLine ();
	iTest.Console()->Printf (_L ("%S"), &ourPrompt);
	
	if ((key == EKeyEnter) && (ourLine.Length() !=0))
		tempstring.Copy(ourLine);
	if (tempstring.Length () != 0)						//if temstring length is not zero
		{
		iTest.Console()->Printf (_L (" = %S\n"), &tempstring);	//print the contents to iTest.Console()
		LogIt(_L ("%S = %S\n"), &ourPrompt, &tempstring);
		}

	else
		//iTest.Console()->Printf (_L (" is empty"));
	iTest.Console()->Printf (_L ("\n"));
	currentstring.Copy(tempstring);						//copy 16 bit tempstring descriptor back 
	}


EXPORT_C TInt CHTTPTestUtils::GetSelection(const TDesC& ourPrompt, const TDesC& validChoices)
//
//	Present the user with a list of options, and get their selection
	{
	// If we're scripting, try reading from script first
	TInt readScriptErr = KErrNotFound;
	if (iScriptRunning)
		{
		TBuf<1> oneCharBuf;
		readScriptErr = ReadLineFromScript(oneCharBuf);
		if (!readScriptErr)
			{
			return validChoices.Locate((TChar)oneCharBuf[0]);
			}
		}

	// Either not scripting, or hit end of script - continue with user input
	TKeyCode key = EKeyNull;
	iTest.Console()->SetPos (0, iTest.Console()->WhereY ());
	Print(ourPrompt);
	iTest.Console()->Printf(_L("[%S] :"), &validChoices);
	TInt retVal = KErrNotFound;
	while (retVal == KErrNotFound)
		{
		key = iTest.Getch();

		// Check that key is in the list of valid choices
		retVal = validChoices.Locate((TChar)key);
		}
	iTest.Console()->Printf(_L("%c\n\n"), key);
	return retVal;
	}


EXPORT_C void CHTTPTestUtils::SetScript(RFile& scriptFile)
//
//	Sets the file to be used for a test script - ie. a file that contains commands used by
//  GetEntry() and GetSelection()
	{
	iScriptFile = &scriptFile;
	iScriptRunning = ETrue;
	LogIt(_L("***SCRIPT STARTING***\n"));
	}

EXPORT_C void CHTTPTestUtils::InitCommsL()
	{
#ifndef SIROCCO_CODE_MIGRATION  
	TInt ret = User::LoadPhysicalDevice(PDD_NAME);
	User::LeaveIfError(ret == KErrAlreadyExists?KErrNone:ret);

#ifndef __WINS__
	ret = User::LoadPhysicalDevice(PDD2_NAME);
	ret = User::LoadPhysicalDevice(PDD3_NAME);
	ret = User::LoadPhysicalDevice(PDD4_NAME);
#endif

	ret = User::LoadLogicalDevice(LDD_NAME);
	User::LeaveIfError(ret == KErrAlreadyExists?KErrNone:ret);
	ret = StartC32();
	User::LeaveIfError(ret == KErrAlreadyExists?KErrNone:ret);
#endif //SIROCCO_CODE_MIGRATION
	}

EXPORT_C void CHTTPTestUtils::HoldOpenConnectionL()
	{
	RSocketServ sockSrvHnd;
	TInt err = sockSrvHnd.Connect();
	if (err != KErrNone)
		{
		User::Leave(err);
		}

	// Start UDP protocol stack
	TProtocolDesc desc;
	_LIT(KUdp, "udp");
	err = sockSrvHnd.FindProtocol(KUdp(), desc);
	User::LeaveIfError(err);
	TRequestStatus stat;
	sockSrvHnd.StartProtocol(desc.iAddrFamily, desc.iSockType, desc.iProtocol, stat);
	User::WaitForRequest(stat);
	}

EXPORT_C void CHTTPTestUtils::ReleaseConnectionL()
	{
	RSocketServ sockSrvHnd;
	TInt err = sockSrvHnd.Connect();
	if (err != KErrNone)
		{
		User::Leave(err);
		}

	// Stop UDP protocol stack
	TProtocolDesc desc;
	_LIT(KUdp, "udp");
	err = sockSrvHnd.FindProtocol(KUdp(), desc);
	User::LeaveIfError(err);
	TRequestStatus stat;
	sockSrvHnd.StopProtocol(desc.iAddrFamily, desc.iSockType, desc.iProtocol, stat);
	User::WaitForRequest(stat);
	}

TInt CHTTPTestUtils::ReadLineFromScript(TDes& aBuffer)
//
// Reads the next line from the script file, and sets the passed-in descriptor with its contents.
// Returns KErrNone if reading succeeded; KErrNotFound if the EOF was reached. When EOF is reached,
// the file is closed.
	{
	// *********************************
	// Assume script is 8-bit text file
	// *********************************
	TBool isAComment = ETrue;
	TInt err = KErrNone;
	TBuf<512> line;
	while (isAComment && !err)
		{
		TFileText text;
		text.Set(*iScriptFile);
		line.SetLength(0);
		for(;;)
			{
			TBuf8<2> c;
			err = iScriptFile->Read(c,1);
			if (err && err != KErrEof)
				{
				iTest.Printf(_L("Error reading file: %d\n"), err);
				break;
				}
			if (c.Length() == 0)
				{
				err = KErrEof;
				break;
				}
			else
				{
				if (c[0] == '\n') // break out if it is CR
					break;
				else if (c[0] != 0x0d) // otherwise append the char, _unless_ it is a LF
					line.Append(c[0]);
				}
			}
		if (err == KErrNone && line.Locate('/') != 0) // comment (only works if it's the first character)
			{
			isAComment = EFalse;
			}
		}

	// The line read is not a comment, or have hit end of file
	if (!err)
		{
		// copy to passed in descriptor, but do not allow an overflow
		aBuffer.Copy(line.Left(aBuffer.MaxLength()));
		LogIt(_L("***SCRIPT : read command '%S' ***\n"), &aBuffer);
		}
	else
		{
		iScriptFile->Close();
		err = KErrNotFound;
		iScriptRunning = EFalse;
		LogIt(_L("***SCRIPT ENDED***\n"));
		}
	return err;
	}

void CHTTPTestUtils::Panic(TInt aPanic)
//
//	Panic the client program.
	{
	User::Panic(KTestPanic,aPanic);
	}

void CHTTPTestUtils::TestHarnessComplete()
//
//	Test harness completed without failures
	{
	WriteComment(KTestHarnessCompleted);
	}

void CHTTPTestUtils::TestHarnessFailed()
//
//	Test harness has a failure - log information
	{
	TBuf<KMaxFileName + 4> buf;
	buf.Format(KTestHarnessFailed, iFailedTests->Count());
	WriteComment(buf);
	//	Log fialed tests' information
	for (TInt ii=0; ii<iFailedTests->Count(); ++ii)
		{
		CTestInfo* failed = iFailedTests->At(ii);
		TPtrC name = failed->Name();
		LogIt(KTestFailInfo, failed->Number(), &name, failed->ErrorCode());
		}
	}

void CHTTPTestUtils::ResourceLeakTest()
//
// Creates a new test that fails if any there are any leaked resource handles
	{
	// Start new test
	_LIT(KResourceTestName, "Resource Handle Leak Test");
	TRAPD(error,StartTestL(KResourceTestName));

	if (error !=KErrNone)
		Panic(EBadStartTest);

	//	Find number of opened handles
	TInt processHandleCount=0;
	TInt threadHandleCount=0;
	RThread().HandleCount(processHandleCount,threadHandleCount);
	TInt err = KErrNone;
	TInt openHandleCount = iStartHandleCount-threadHandleCount;
	if ( openHandleCount !=0)
		{
		err = KErrGeneral;
		LogIt(_L("Number leaked handles is %D"), openHandleCount);
		}
	EndTest(err);
	}


EXPORT_C
TInt CHTTPTestUtils::ResolveFile(const TDesC& aComponent, const TDesC& aFileName, TParse& aParseOut)
	{
	TFileName savedPath;
	TFileName fileName;
	
	fileName.Append(KPathSep);
	fileName.Append(KTestFileInputBase);
	fileName.Append(KPathSep);
	fileName.Append(aComponent);
	fileName.Append(KPathSep);
	fileName.Append(aFileName);
	
	// file finder will look in the session drive first, then Y->A,Z
	// so set session drive to Y (save old and restore it afterwards)
	iFs.SessionPath(savedPath);
	_LIT(KTopDrive,"Y:\\");
	iFs.SetSessionPath(KTopDrive);
    TFindFile file_finder(iFs);
    TInt err = file_finder.FindByDir(fileName,KNullDesC);
	if(err==KErrNone)
		aParseOut.Set(file_finder.File(),NULL,NULL);
	iFs.SetSessionPath(savedPath);
	return(err);
	}

EXPORT_C
HBufC8* CHTTPTestUtils::ReadFileLC(const TDesC& aFile)
	{
	//Returns a HBufC8 with the contents of aFile
	RFile file;
	User::LeaveIfError(file.Open(iFs, aFile, EFileShareAny | EFileRead));
	CleanupClosePushL(file);

	TInt size = 0;
	TInt err = file.Size(size);

	if (err)
		{
		iTest.Printf(_L("ReadFileLC(%S) leaving with %d. Unable to size file\n"), &aFile, err);
		User::Leave(err);
		}

	HBufC8* contents = HBufC8::NewLC(size + 4);
	TPtr8 contentsPtr(contents->Des());
	err = file.Read(0, contentsPtr);

	if (err)
		{
		iTest.Printf(_L("ReadFileLC(%S) leaving with %d. Unable to read file\n"), &aFile, err);
		User::Leave(err);
		}

	CleanupStack::Pop(); //contents
	CleanupStack::PopAndDestroy(); //file
	CleanupStack::PushL(contents);

	return contents;
	}




//
//	CTestInfo
//
CHTTPTestUtils::CTestInfo::CTestInfo()
//
//	Default c'tor
	{
	}

CHTTPTestUtils::CTestInfo::~CTestInfo()
//
//	D'tor
	{
	delete iName;
	}

CHTTPTestUtils::CTestInfo* CHTTPTestUtils::CTestInfo::NewLC(const TDesC& aName, TInt aNumber, TInt aErrorCode)
//
//	Static factory c'tor
	{
	CTestInfo* self = new (ELeave) CTestInfo();
	CleanupStack::PushL(self);
	self->ConstructL(aName, aNumber, aErrorCode);
	return self;
	}

CHTTPTestUtils::CTestInfo* CHTTPTestUtils::CTestInfo::NewL(const TDesC& aName, TInt aNumber, TInt aErrorCode)
//
//	Static factory c'tor
	{
	CTestInfo* self = NewLC(aName, aNumber, aErrorCode);
	CleanupStack::Pop();	//	self
	return self;
	}

void CHTTPTestUtils::CTestInfo::ConstructL(const TDesC& aName, TInt aNumber, TInt aErrorCode)
//
//	Non-trivial c'tor
	{
	iName = aName.AllocLC();
	CleanupStack::Pop();	//	iName

	iNumber = aNumber;
	iErrorCode = aErrorCode;
	}

void CHTTPTestUtils::CTestInfo::SetNameL(const TDesC& aName)
//
//	Sets iName
	{
	HBufC* temp = aName.AllocLC();
	CleanupStack::Pop();	//	temp
	delete iName;
	iName = temp;
	}

void CHTTPTestUtils::CTestInfo::SetNumber(TInt aNumber)
//
//	Sets iNumber
	{
	iNumber = aNumber;
	}

void CHTTPTestUtils::CTestInfo::SetErrorCode(TInt aErrorCode)
//
//	Sets iErrorCode
	{
	iErrorCode = aErrorCode;
	}

#ifndef EKA2
GLDEF_C TInt E32Dll(TDllReason /*aReason*/)
//
//	Main Dll entry point
	{
	return(KErrNone);
	}
#endif // EKA2

void CHTTPTestUtils::Print(const TDesC& aDescriptor)
	{
	TBuf<KMaxTempBuffize> chunkOfPrompt;
	TInt length = aDescriptor.Length();
	TInt num=length/KMaxTempBuffize;
	TInt rest=length%KMaxTempBuffize;
	TInt iter;
	if(num>0)
		for(iter=0;iter<num;++iter)
			{
			chunkOfPrompt.Copy(aDescriptor.Mid(iter*KMaxTempBuffize,KMaxTempBuffize-1));
			iTest.Console()->Printf(_L("%S "), & chunkOfPrompt);
			}
	chunkOfPrompt.Copy(aDescriptor.Mid(length-rest,rest));
	iTest.Console()->Printf(_L("%S "), & chunkOfPrompt);	
	}

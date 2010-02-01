// Copyright (c) 1998-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Author: Philippe Gabriel
// A simple test program which uses the statically linked dll  "ftpprot.dll"
// 
//

// standard example header
#include "DEBUG.H"
#include <e32base.h>
#include <es_sock.h>
#include <in_sock.h>
#include <ftpsess.h>
#include "FTPTEST.H"

TBuf8<955> myBuffer8;
TBuf<2000> myBuffer;
TInt	total;

#define __CONSOLE_OUTPUT__

#define __FILERETR_TESTS__
//#define __IPV4_TESTS__
//#define __IPV6_TESTS__

class TFtpTest02Params
	{
public:
	void SetParams(const TDesC& aInetAddr, TUint aPort, const TDesC& aHostName,
		const TDesC8& aUserName, const TDesC8& aPassword, const TDesC8& aDirectoryPath,
		const TDesC8& aRemoteFileName, const TDesC& aLocalFileName);
public:
	TInetAddr iInetAddr;
	TUint iPort;
	THostName iHostName;
	TPtrC8 iUserName;
	TPtrC8 iPassword;
	TPtrC8 iDirectoryPath;
	TPtrC8 iRemoteFileName;
	TPtrC iLocalFileName;
	};

void TFtpTest02Params::SetParams(const TDesC& aInetAddr, TUint aPort, const TDesC& aHostName,
		const TDesC8& aUserName, const TDesC8& aPassword, const TDesC8& aDirectoryPath,
		const TDesC8& aRemoteFileName, const TDesC& aLocalFileName)
	{
	iInetAddr.Input(aInetAddr);
	iInetAddr.SetPort(aPort);
	iPort=aPort;
	iHostName=aHostName;
	iUserName.Set(aUserName);
	iDirectoryPath.Set(aDirectoryPath);
	iPassword.Set(aPassword);
	iRemoteFileName.Set(aRemoteFileName);
	iLocalFileName.Set(aLocalFileName);
	}

//

class CFtpTest02 : public CBase, public MFtpSessionNotifier
	{
public:
	static CFtpTest02* NewL();
	~CFtpTest02();
	void DoTest();
	void TestMain();
	void StartScheduler();
	void StopSchedulerIfNecessary();

	void Complete(void);
	// Operation completed, more data to follow
	void MoreData(void);
	// Transfer Progress report
	void TransferProgress(TUint aProgress);
	// User canceled ongoing operation
	void Cancel(void);
	// Peer reset the connection
	void ConnReset(void);
	// Connection error
	void ConnectionError(TOpComp aTConnectionError);
	// FTP server does not implement the operation requested
	void OperationNotSupported(void);
	// Local File system error
	void LocalFileSystemError(TOpComp aTLocalFileSystemError);
	// Remote File system error
	void RemoteFileSystemError(TOpComp aTRemoteFileSystemError);
	// Not specified yet
	void EUnknownError();
	// Message reported by server
	void ServerMessage(const TDesC8&);

	void ListTest();
	void RestartTest();
	void FailedRetrievalTest();
    void FailedRetrievalTestOverwrite();
	void RetriveveTestOverwrite();
	void RenameTest();
	void DeleteFileTest();
	void StoreTest();
	void DirectoryTest();

protected:
	CFtpTest02();
private:
	CFTPSession* iCFTPSession;
	TFtpTest02Params iTestParams;
	TBool iSchedulerStarted;
	};	

CFtpTest02* CFtpTest02::NewL()
	{
	CFtpTest02* self = new(ELeave) CFtpTest02();
	CleanupStack::PushL(self);
	self->iCFTPSession = CFTPSession::NewL(self);
	CleanupStack::Pop(self);
	return self;
	}

CFtpTest02::CFtpTest02()
{}

CFtpTest02::~CFtpTest02()
	{
	delete iCFTPSession;
	}

void CFtpTest02::StartScheduler()
	{
	iSchedulerStarted=ETrue;
	CActiveScheduler::Start();
	}

void CFtpTest02::StopSchedulerIfNecessary()
	{
	if(iSchedulerStarted)
		CActiveScheduler::Stop();
	iSchedulerStarted=EFalse;
	}

void CFtpTest02::ServerMessage(const TDesC8& aMessage)
	{
	myBuffer.Copy(aMessage);
#ifdef __CONSOLE_OUTPUT__
	__FTPDebugConsole->Write(myBuffer);
#endif

	FTPPROTDEBUG(0xffff,myBuffer);
	}

void CFtpTest02::TransferProgress(TUint aProgress)
	{
	total += aProgress;
#ifdef __CONSOLE_OUTPUT__
	__FTPDebugConsole->Printf(_L("CFtpTest02::TransferProgress total:>%d< Packet:>%d<\n"),total,aProgress);
#endif

	FTPPROTDEBUG3(0xffff, _L("CFtpTest02::TransferProgress total:>%d< Packet:>%d<\n"),total,aProgress);

	}

void CFtpTest02::Complete(void)
	{
#ifdef __CONSOLE_OUTPUT__
	__FTPDebugConsole->Write(_L("CFtpTest02::Complete\n"));
#endif

	FTPPROTDEBUG(0xffff, _L("CFtpTest02::Complete\n"));
	StopSchedulerIfNecessary();
	}

void CFtpTest02::Cancel(void){}

void CFtpTest02::ConnReset(void)
	{
#ifdef __CONSOLE_OUTPUT__
	__FTPDebugConsole->Write(_L("CFtpTest02::ConnReset\n"));
#endif

	FTPPROTDEBUG(0xffff, _L("CFtpTest02::ConnReset\n"));
	StopSchedulerIfNecessary();
	}

void CFtpTest02::ConnectionError(TOpComp /*aTConnectionError*/)
	{
#ifdef __CONSOLE_OUTPUT__
	__FTPDebugConsole->Write(_L("CFtpTest02::ConnectionError\n"));
#endif

	FTPPROTDEBUG(0xffff, _L("CFtpTest02::ConnectionError\n"));
	StopSchedulerIfNecessary();
	}

void CFtpTest02::OperationNotSupported(void){}

void CFtpTest02::LocalFileSystemError(TOpComp /*aTLocalFileSystemError*/)
	{
#ifdef __CONSOLE_OUTPUT__
	__FTPDebugConsole->Write(_L("CFtpTest02::LocalFileSystemError\n"));
#endif

	FTPPROTDEBUG(0xffff, _L("CFtpTest02::LocalFileSystemError\n"));
	StopSchedulerIfNecessary();
	}

void CFtpTest02::RemoteFileSystemError(TOpComp /*aTRemoteFileSystemError*/)
	{
	StopSchedulerIfNecessary();
#ifdef __CONSOLE_OUTPUT__
	__FTPDebugConsole->Write(_L("CFtpTest02::RemoteFileSystemError\n"));
#endif

	FTPPROTDEBUG(0xffff, _L("CFtpTest02::RemoteFileSystemError\n"));
	}

void CFtpTest02::EUnknownError(void)
	{
#ifdef __CONSOLE_OUTPUT__
	__FTPDebugConsole->Write(_L("Test02Notifier::EUnknownError\n"));
#endif

	FTPPROTDEBUG(0xffff, _L("Test02Notifier::EUnknownError\n"));
	StopSchedulerIfNecessary();
	}

void CFtpTest02::MoreData(void)	
	{
#ifdef __CONSOLE_OUTPUT__
	__FTPDebugConsole->Write(_L("CFtpTest02::Notification called EPacketReceived\n"));
	__FTPDebugConsole->Write(_L("----------Packet from DTP Channel---------\n"));
	__FTPDebugConsole->Write(myBuffer);
	__FTPDebugConsole->Write(_L("\n----------End of Packet from DTP Channel---------\n"));
#endif

	FTPPROTDEBUG(0xffff,_L("CFtpTest02::Notification called EPacketReceived\n"));
	FTPPROTDEBUG(0xffff,_L("----------Packet from DTP Channel---------\n"));
	FTPPROTDEBUG(0xffff,myBuffer);
	FTPPROTDEBUG(0xffff,_L("\n----------End of Packet from DTP Channel---------\n"));

	iCFTPSession->ListDirectory(_L8(""),myBuffer8);
	return;
	}



LOCAL_C void doExampleL()
    {

	CActiveScheduler* exampleScheduler=new (ELeave) CActiveScheduler;
    CleanupStack::PushL(exampleScheduler);
    CActiveScheduler::Install(exampleScheduler);

	CFtpTest02* myFtpTest02 = CFtpTest02::NewL();
	CleanupStack::PushL(myFtpTest02);

#ifdef __IPV4_TESTS__
	__FTPDebugConsole->Printf(_L("FTP Test 02 Ipv4 \n\n"));
#else
#ifdef __FILERETR_TESTS__
	__FTPDebugConsole->Printf(_L("FTP Test 02 File retreive \n\n"));
#else
	__FTPDebugConsole->Printf(_L("FTP Test 02 Ipv6 \n\n"));
#endif
#endif

	__FTPDebugConsole->Printf(_L("FTP Session client \n\n"));

	myFtpTest02->TestMain();

	__FTPDebugConsole->Printf(_L("Finished test\n"));

	CleanupStack::PopAndDestroy(myFtpTest02);
	CleanupStack::PopAndDestroy(exampleScheduler);
	}

void CFtpTest02::TestMain()
	{
#ifdef __IPV4_TESTS__
	iTestParams.SetParams(_L("207.46.133.140"), 21, _L("ftp.microsoft.com"),		//params for microsoft ftp site
		_L8("anonymous"), _L8("philippe@symbian.com"), _L8("misc"), _L8("index.txt"),
		_L("index.txt"));
	DoTest();

	iTestParams.SetParams(_L("192.18.99.73"), 21, _L("ftp.sun.com"),
		_L8("anonymous"), _L8("karl.mcdowall@symbian.com"), _L8("/"), _L8("swec.dat"),
		_L("swec.dat"));
	DoTest();
#endif

#ifdef __FILERETR_TESTS__
	iTestParams.SetParams(_L("10.16.113.6"), 21, _L("ftp-images"),
		_L8("roms"), _L8("integrator"), _L8("/"), _L8("psdagt_udeb.htm"),
		_L("psdagt_udeb.htm"));

	iCFTPSession->Connect(iTestParams.iInetAddr,iTestParams.iUserName,	//Login using passive mode
		iTestParams.iPassword,CFTPSession::Epassive);
	StartScheduler();

	RetriveveTestOverwrite();
	FailedRetrievalTestOverwrite();
	FailedRetrievalTest();

    iCFTPSession->Close();
	StartScheduler();
#endif

#ifdef __IPV6_TESTS__
	iTestParams.SetParams(_L("2001:618:400:6a:2c0:4fff:fe8a:a918"),	//params for snus6
		21, _L("snus6.intra6"),  _L8("karlm"), _L8("karlm"),
		_L8("/usr/home/karlm"), _L8("test.cpp"), _L("test.cpp"));
	DoTest();

	iTestParams.SetParams(_L("2001:618:400:6a:210:5aff:febf:531"),	//params for 6pack6
		21, _L("6pack6.intra6"), _L8("anonymous"), _L8("karlm"),
		_L8("/usr"), _L8("anything"), _L("anything"));
	DoTest();
#endif
	}

void CFtpTest02::DoTest()
	{
//Removed teporarily due to problem with host resolution
//	iCFTPSession->Connect(iTestParams.iHostName,iTestParams.iUserName,	//Login using hostname
//		iTestParams.iPassword,CFTPSession::EActive,iTestParams.iPort);	
//	StartScheduler();
//	iCFTPSession->Close();
//	StartScheduler();

	iCFTPSession->Connect(iTestParams.iInetAddr,iTestParams.iUserName,	//Login using Active mode
		iTestParams.iPassword,CFTPSession::EActive);
	StartScheduler();
	iCFTPSession->Close();
	StartScheduler();

	iCFTPSession->Connect(iTestParams.iInetAddr,iTestParams.iUserName,	//Login using passive mode
		iTestParams.iPassword,CFTPSession::Epassive);
	StartScheduler();

	iCFTPSession->ChangeDirectory(iTestParams.iDirectoryPath);			//Move to appropriate directory.
	StartScheduler();
	
	ListTest();
	RetriveveTestOverwrite();
	FailedRetrievalTest();
	FailedRetrievalTestOverwrite();
	RenameTest();
	DeleteFileTest();
	StoreTest();
	RestartTest();
	DirectoryTest();


    iCFTPSession->Close();
	StartScheduler();

	}

void CFtpTest02::ListTest()
	{
	iCFTPSession->ListDirectory(_L8("."),myBuffer8);
	StartScheduler();
	}

void CFtpTest02::RestartTest()
	{	
//	Note: Restart is a synchronous function
	iCFTPSession->Restart(50000);
	}

void CFtpTest02::RetriveveTestOverwrite()
	{
	total =0;
	iCFTPSession->Retrieve(iTestParams.iRemoteFileName,iTestParams.iLocalFileName,
		CFTPSession::EOverwrite,CFTPSession::EBinary,CFTPSession::EStream);
	StartScheduler();
	}

void CFtpTest02::FailedRetrievalTest()
	{
	iCFTPSession->Retrieve(_L8("NoExist"),_L("NoExist"),CFTPSession::ENoOverwrite,CFTPSession::EBinary,CFTPSession::EStream);
	StartScheduler();
	}

void CFtpTest02::FailedRetrievalTestOverwrite()
	{
	iCFTPSession->Retrieve(_L8("NoExist"),iTestParams.iLocalFileName,CFTPSession::EOverwrite,CFTPSession::EBinary,CFTPSession::EStream);
	StartScheduler();
	}

void CFtpTest02::DeleteFileTest()
	{
	iCFTPSession->DeleteFile(iTestParams.iRemoteFileName);
	StartScheduler();
	}

void CFtpTest02::StoreTest()
	{
	iCFTPSession->Store(iTestParams.iLocalFileName,iTestParams.iRemoteFileName,
		EFalse, CFTPSession::EBinary,CFTPSession::EStream);
	StartScheduler();
	}

void CFtpTest02::RenameTest()
	{
	iCFTPSession->RenameFile(iTestParams.iRemoteFileName, _L8("tempName"));
	StartScheduler();
	iCFTPSession->RenameFile(_L8("tempName"), iTestParams.iRemoteFileName);
	StartScheduler();
	}

void CFtpTest02::DirectoryTest()
	{
	iCFTPSession->CreateDirectory(_L8("TestDir0"));
	StartScheduler();
	iCFTPSession->DeleteDirectory(_L8("TestDir1"));
	StartScheduler();
	}

//Old IP4 ftp sites. Might be worth keeping as a record in case the ones currently being used 
//stop working.
//
//	TInetAddr myaddr(INET_ADDR(207,200,71,59),21);// ftp.netscape.com
//	TPtrC	DNSName(_L("ftp.netscape.com"));
//	TPtrC	userName(_L("anonymous"));
//	TPtrC	passwd(_L("philippe@symbian.com"));
//	TPtrC	directoryPath(_L("pub/communicator/extras/import/eudora"));
//	TPtrC	fileName(_L("Readme.txt"));
//
//	TInetAddr myaddr(INET_ADDR(209,77,154,25),21);// ftp.insignia.com (home of the braves)
//	TPtrC	DNSName(_L("ftp.insignia.com"));
//	TPtrC	userName(_L("anonymous"));
//	TPtrC	passwd(_L("philippe@symbian.com"));
//	TPtrC	directoryPath(_L("special/ntrigue/"));
//	TPtrC	fileName(_L("read4a.txt"));
//
//	TInetAddr myaddr(INET_ADDR(194,129,2,201),21);// sparky
//	TPtrC	DNSName(_L("sparky"));
//	TPtrC	userName(_L("philippe"));
//	TPtrC	passwd(_L("massilia"));
//	TPtrC	directoryPath(_L("ftptest"));
//	TPtrC	fileName(_L("testfile.bin"));
//	TPtrC	fileName(_L("map.gif"));
//	TPtrC	localFile(_L("motodemo.zip"));
//

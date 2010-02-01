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
#include <e32base.h>
#include <f32file.h>
#include <in_sock.h>
#include <ftpprot.h>
#include "FTPTEST.H"
#include "DEBUG.H"

TBuf8<1500> myBuffer;

//#define __IPV6_TESTS__
#define __IPV4_TESTS__

class TTest01Params
	{
public:
	void SetParams(const TDesC& aInetAddr, TUint aPort, const TDesC& aHostName, 
		const TPtrC8& aUserName, const TPtrC8& aPassword,
		const TPtrC8& aDirectoryPath, const TPtrC8& aFileName);
public:
	TInetAddr iInetAddr;
	THostName iHostName;
	TUint iPort;
	TPtrC8 iUserName;
	TPtrC8 iPassword;
	TPtrC8 iDirectoryPath;
	TPtrC8 iFileName;
	};

void TTest01Params::SetParams(const TDesC& aInetAddr, TUint aPort, const TDesC& aHostName, 
		const TPtrC8& aUserName, const TPtrC8& aPassword, const TPtrC8& aDirectoryPath,
		const TPtrC8& aFileName)
	{
	iInetAddr.Input(aInetAddr);
	iInetAddr.SetPort(aPort);
	iPort = aPort;
	iHostName = aHostName;
	iUserName.Set(aUserName);
	iPassword.Set(aPassword);
	iDirectoryPath.Set(aDirectoryPath);
	iFileName.Set(aFileName);
	}

class CFtpTest01 : public CBase, public MFtpProtocolNotifier
	{
public:
	static CFtpTest01* NewL();
	~CFtpTest01();
	void DoTest();
	void TestMain();
	void ServerPositiveAnswerNotification(const TOpComp aStatus);
	void ServerXFerNotification(const TOpComp aStatus);
	void ServerNegativeAnswerNotification(const TOpComp aStatus);
	void ErrorNotification(const TOpComp aStatus);
	void ServerMessage(const class TDesC8&);
	
private:
	void PasvTest();
	void ListTest();
	void RetrTest();
	void StorTest();
	void DeleTest();
	void MkdTest();

	void StartScheduler();
	void StopSchedulerIfNecessary();
private:
	TBool iSchedulerStarted;
	TTest01Params iParams;
	CFtpProtocol* iCFtpProtocol;
	TBool iErrorOccured;
	};

CFtpTest01* CFtpTest01::NewL()
	{
	CFtpTest01* self = new(ELeave) CFtpTest01;
	CleanupStack::PushL(self);
	self->iCFtpProtocol = CFtpProtocol::NewL(self);
	CleanupStack::Pop(self);
	return self;
	}

CFtpTest01::~CFtpTest01()
	{
	delete iCFtpProtocol;
	}

void CFtpTest01::StartScheduler()
	{
	iSchedulerStarted=ETrue;
	CActiveScheduler::Start();
	}

void CFtpTest01::StopSchedulerIfNecessary()
	{
	if(iSchedulerStarted)
		CActiveScheduler::Stop();
	iSchedulerStarted=EFalse;
	}

void CFtpTest01::ErrorNotification(const TOpComp aStatus)
	{
	FTPPROTDEBUG1(0xffff,_L("CFtpTest01::ErrorNotification called aStatus:%d\n"),aStatus);
	(void) aStatus;
	iErrorOccured = ETrue;
	StopSchedulerIfNecessary();
	}

void CFtpTest01::ServerPositiveAnswerNotification(const TOpComp aStatus)
	{
	FTPPROTDEBUG1(0xffff,_L("CFtpTest01::ServerPositiveAnswerNotification called aStatus:%d\n"),aStatus);
	(void) aStatus;
	StopSchedulerIfNecessary();
	}

void CFtpTest01::ServerNegativeAnswerNotification(const TOpComp aStatus)
	{
	FTPPROTDEBUG1(0xffff,_L("CFtpTest01::ServerNegativeAnswerNotification called aStatus:%d\n"),aStatus);
	(void) aStatus;
	StopSchedulerIfNecessary();
	}

void CFtpTest01::ServerXFerNotification(const TOpComp aStatus)
	{
	FTPPROTDEBUG1(0xffff,_L("CFtpTest01::ServerXFerNotification called aStatus:%d\n"),aStatus);
	(void) aStatus;
	StopSchedulerIfNecessary();
	}

void CFtpTest01::ServerMessage(const class TDesC8& /*aMessage*/)
	{
	FTPPROTDEBUG(0xffff,_L("CFtpTest01::ServerMessage called\n"));
	}

LOCAL_C void doExampleL()
    {
	CActiveScheduler* exampleScheduler=new (ELeave) CActiveScheduler;
    CleanupStack::PushL(exampleScheduler);
    CActiveScheduler::Install(exampleScheduler);

#ifdef __IPV4_TESTS__
	__FTPDebugConsole->Printf(_L("FTP Test 01 (Ipv4) \n\n"));
#else
	__FTPDebugConsole->Printf(_L("FTP Test 01 (Ipv6) \n\n"));
#endif

	__FTPDebugConsole->Printf(_L("statically linked DLL example \n\n"));

	CFtpTest01* myTest01 = CFtpTest01::NewL();
	CleanupStack::PushL(myTest01);
	myTest01->TestMain();
	CleanupStack::PopAndDestroy(myTest01);

	CleanupStack::PopAndDestroy(exampleScheduler);
	}

void CFtpTest01::TestMain()
	{
#ifdef __IPV6_TESTS__
	iParams.SetParams(_L("2001:618:400:6a:2c0:4fff:fe8a:a918"),	//params for snus6
		21, _L("snus6.intra6"), _L8("karlm"), _L8("karlm"),
		_L8("/usr/home/karlm"), _L8("test.cpp"));
	DoTest();

	iParams.SetParams(_L("2001:618:400:6a:210:5aff:febf:531"),	//params for 6pack6
		21, _L("6pack6.intra6"), _L8("anonymous"), _L8("karlm"),
		_L8("/usr"), _L8("anything"));
	DoTest();
#endif

#ifdef __IPV4_TESTS__
	iParams.SetParams(_L("207.46.133.140"), 21, _L("ftp.microsoft.com"),	//params for ftp.micorsoft.com
		_L8("anonymous"), _L8("philippe@symbian.com"), _L8("/peropsys"),
		_L8("readme.txt"));
	DoTest();

	iParams.SetParams(_L("192.18.99.73"), 21, _L("ftp.sun.com"),	//params for ftp.sun.com
		_L8("anonymous"), _L8("philippe@symbian.com"), _L8("/pub"),
		_L8("index.html"));
	DoTest();

#endif
//	TPtrC	DNSName(_L("philippeg_nt"));
//	TPtrC8	userName(_L8("phil"));

//	TPtrC8	passwd(_L8("phil"));


//	TInetAddr myaddr(INET_ADDR(64,12,168,249),21);// ftp.netscape.com
//	TPtrC	DNSName(_L("ftp.netscape.com"));
//	TPtrC	userName(_L("anonymous"));
//	TPtrC	passwd(_L("philippe@symbian.com"));
//	TInetAddr myaddr(INET_ADDR(209,77,154,25),21);// ftp.insignia.com (home of the braves)
//	TPtrC	DNSName(_L("ftp.insignia.com"));
//	TPtrC8	userName(_L8("anonymous"));
//	TPtrC8	passwd(_L8("philippe@symbian.com"));
//	TPtrC	directoryPath(_L("special/ntrigue/"));
//	TPtrC	fileName(_L("read5.txt"));
//
//	TInetAddr myaddr(INET_ADDR(155,198,143,16),21);// ftp.radian.ee.ic.ac.uk
//	TPtrC	DNSName(_L("radian.ee.ic.ac.uk"));
//
	__FTPDebugConsole->Printf(_L("Finished tests\n"));
	}

void CFtpTest01::DoTest()
	{
	iErrorOccured = EFalse;
	iCFtpProtocol->Connect(iParams.iInetAddr);		//Just do a normal connect.
	StartScheduler();
	if(iErrorOccured)
		return;

//Removed temporarily due to problems with host resolution
//	iCFtpProtocol->Quit();
//	StartScheduler();

//	iCFtpProtocol->Connect(iParams.iHostName, iParams.iPort);	//Now try using a host name to test resolver.
//	StartScheduler();
//	if(iErrorOccured)
//		return;

	iCFtpProtocol->Help();
	StartScheduler();
    iCFtpProtocol->User(iParams.iUserName);
	StartScheduler();
    iCFtpProtocol->Pass(iParams.iPassword);
	StartScheduler();

	PasvTest();
	__FTPDebugConsole->Printf(_L("FTP PasvTest \n"));

	ListTest();
	__FTPDebugConsole->Printf(_L("FTP ListTest \n"));

	RetrTest();
	__FTPDebugConsole->Printf(_L("FTP RetrTest \n"));

	StorTest();
	__FTPDebugConsole->Printf(_L("FTP StorTest \n"));

	DeleTest();
	__FTPDebugConsole->Printf(_L("FTP DeleTest \n"));

	MkdTest();
	__FTPDebugConsole->Printf(_L("FTP MkdTest \n"));

//QUIT//

	iCFtpProtocol->Quit();
	StartScheduler();
	}


void CFtpTest01::PasvTest()
	{
	iCFtpProtocol->Pasv();
	StartScheduler();
	}

void CFtpTest01::ListTest()
	{
	iCFtpProtocol->Port();
	StartScheduler();
	iCFtpProtocol->RecvBuffer(&myBuffer);
	iCFtpProtocol->List(iParams.iDirectoryPath);
	StartScheduler();
	}

void CFtpTest01::RetrTest()
	{
	iCFtpProtocol->Port();
	StartScheduler();
	iCFtpProtocol->Cwd(iParams.iDirectoryPath);
	StartScheduler();
	iCFtpProtocol->RecvBuffer(&myBuffer);
	iCFtpProtocol->Retr(iParams.iFileName);
	StartScheduler();
	}

void CFtpTest01::StorTest()
	{
	myBuffer = _L8("This is a test file:\n\
				   Sous le Pont Mirabeau coule la Seine\n\
				   et nos amours, faut il qu'il m'en souvienne\n\
				   la joie venait toujours apres la peine\n");
	iCFtpProtocol->Port();
	StartScheduler();
	iCFtpProtocol->SendBuffer(&myBuffer);
	iCFtpProtocol->Stor(_L8("ftptst01.txt"));
	StartScheduler();
	}

void CFtpTest01::DeleTest()
	{
	iCFtpProtocol->Dele(_L8("ftptst01.txt"));
	StartScheduler();
	}

void CFtpTest01::MkdTest()
	{
	iCFtpProtocol->Cwd(iParams.iDirectoryPath);
	StartScheduler();
	iCFtpProtocol->Mkd(_L8("test"));
	StartScheduler();
	}

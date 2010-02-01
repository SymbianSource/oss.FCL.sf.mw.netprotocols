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
//

#include <e32std.h>
#include "t_filters.h"
#include <uri8.h>

_LIT8(KPost, "POST");

// Numbers in brackets are indexes (Filter result numbers). Other
// numbers are event numbers.

TInt sessionCallOrder[] = {	100005, 100003, 100002, //(0) 100005 UID 2 Outgoing
							100003, 100001,			//(3) 100005 UID 1 Outgoing
							100004, 100003, 100001,	//(5) 100004 UID 1 Outgoing
							100004,	100003, 100002,	//(8) 100004 UID 2 Outgoing

							100002, 100003, 100005, //(11) 100005 UID 2 Incoming
							100001, 100003,			//(14) 100005 UID 1 Incoming
							100001, 100003, 100004,  //(16) 100004 UID 1 Incoming
							100002, 100003, 100004  //(19) 100004 UID 2 Incoming
							};

TInt callOrder[] = {4,3,2,1,//(0) 33 Outgoing
					4,2,1, //(4) 22 Outgoing
					4,2,1,//(7). 22,KHTTPMatchAnyEventUid Outgoing
					4,3,2,1, //(10) 33 Outgoing
					2,3,4,5,//(14). Sent from 4th call to Filter 1.33 Incoming
					2,4,5, //(18) 22 Incoming
					2,4,5,//(21) 22,KHTTPMatchAnyEventUid Incoming
					4,2,1, // (24) Cancel (This doesn't happen but it should)
					4,2,1, //(27) Delete ***********
					6,4,3,2,1,//(30) 33 Outgoing Client
					6,// (35)33 Outgoing. Filter 6 deletes itself
					6,4,2,1,//(36) Cancel
					6,4,2,1, // (40) Delete
					6, // (44) Delete from RunError.
					6,4,2,1, // (45) Cancel
					6,4,2,1, // (49) Delete
					6,4,3,2,1, // (53) 33 Status match: response absent.
					6,7,4,3,2,1, // (58) Filter matching due to status
					6,4,3,2,1,// (64) Status match. Wrong status
					6,4,2,1,// (69) Cancel
					6,4,2,1,// (73) Delete
					6,4,3,2,1,// (77) Header absent
					6,8,4,3,2,1,// (82)
					6,8,4,2,1,// 88
					6,4,2,1, 6,4,2,1}; // 93 Cancel, close

TInt callCount = 0;
TInt sessionCallCount = 0;

class CQuitObject : public CActive
	{
public:
	static CQuitObject* NewL(TInt aCount,CHTTPFilterTest* aTest, TBool aTestingSessionEvents = EFalse);
	~CQuitObject();
protected:
	virtual void RunL();
	virtual void DoCancel();
private:
	inline CQuitObject();

	TInt iCount;
	CHTTPFilterTest* iTest;
	TBool iTestingSessionEvents;
	};

CQuitObject* CQuitObject::NewL(TInt aCount, CHTTPFilterTest* aTest, TBool aTestingSessionEvents)
	{
	CQuitObject* that = new (ELeave) CQuitObject;
	that->iTestingSessionEvents = aTestingSessionEvents;
	that->iCount = aCount;
	that->iTest = aTest;
	CActiveScheduler::Add(that);
	that->SetActive();
	TRequestStatus* r = &that->iStatus;
	User::RequestComplete(r, 0);
	return that;
	}

CQuitObject::~CQuitObject()
	{
	Cancel();
	}

CQuitObject::CQuitObject()
		: CActive(EPriorityIdle)
	{
	}

void CQuitObject::RunL()
	{
	TInt count = iTestingSessionEvents ? sessionCallCount : callCount;

	iTest->Utils().LogIt(_L("Finishing when count = %d should be %d"), 
					   count, iCount);
	iTest->EndTest(count != iCount);
	CActiveScheduler::Stop();
	}
void CQuitObject::DoCancel()
	{
	}

class CFTFilter : public CBase, public MHTTPFilter
	{
public:
	static CFTFilter* NewL(TInt aIndex, CHTTPFilterTest* aTest);
	virtual void MHFRunL(RHTTPTransaction aTransaction,
						 const THTTPEvent& aEvent);
	virtual TInt MHFRunError(TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& aEvent);
	virtual void MHFSessionRunL(const THTTPSessionEvent& aEvent);
	virtual TInt MHFSessionRunError(TInt aError, const THTTPSessionEvent& aEvent);

	virtual void MHFLoad(RHTTPSession aSession, THTTPFilterHandle);
	virtual void MHFUnload(RHTTPSession aSession, THTTPFilterHandle);

	TInt iIndex;
	CHTTPFilterTest* iTest;
private:
	CFTFilter(TInt aIndex, CHTTPFilterTest* aTest) { iIndex = aIndex; iTest = aTest;}
	};

CFTFilter* CFTFilter::NewL(TInt aIndex, CHTTPFilterTest* aTest)
	{
	return new (ELeave) CFTFilter(aIndex, aTest);
	}

void CFTFilter::MHFLoad(RHTTPSession, THTTPFilterHandle)
	{
	}

void CFTFilter::MHFUnload(RHTTPSession, THTTPFilterHandle)
	{
	delete this;
	}

void CFTFilter::MHFRunL(RHTTPTransaction aTransaction,
						const THTTPEvent& /*aEvent*/)
	{
	iTest->Utils().LogIt(_L("Filter result %d is %d should be %d"), 
					   callCount, iIndex, callOrder[callCount]);
	iTest->EndTest(callOrder[callCount++]!=iIndex);
	iTest->Utils().LogIt(_L("filtering..."));

	if (callCount == 4)
		{
		aTransaction.SendEventL(THTTPEvent(33), THTTPEvent::EIncoming,
								THTTPFilterHandle(
									THTTPFilterHandle::ECurrentFilter));
		aTransaction.SendEventL(THTTPEvent(22), THTTPEvent::EIncoming,
								THTTPFilterHandle(
									THTTPFilterHandle::ECurrentFilter));
		aTransaction.SendEventL(THTTPEvent(22,KHTTPMatchAnyEventUid), THTTPEvent::EIncoming,
								THTTPFilterHandle(
									THTTPFilterHandle::ECurrentFilter));
		}
	}

void CFTFilter::MHFSessionRunL(const THTTPSessionEvent& /*aEvent*/)
	{
	}

TInt CFTFilter::MHFRunError(TInt aError, RHTTPTransaction /*aTransaction*/, const THTTPEvent& /*aEvent*/)
	{
	return aError;
	}

TInt CFTFilter::MHFSessionRunError(TInt aError, const THTTPSessionEvent& /*aEvent*/)
	{
	return aError;
	}



class CMyErrorFilter : public CBase, public MHTTPFilter
	{
public:
	static CMyErrorFilter* NewL(TInt aIndex, CHTTPFilterTest* aTest);
	virtual void MHFRunL(RHTTPTransaction aTransaction,
						 const THTTPEvent& aEvent);
	virtual TInt MHFRunError(TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& aEvent);
	virtual void MHFSessionRunL(const THTTPSessionEvent& aEvent);
	virtual TInt MHFSessionRunError(TInt aError, const THTTPSessionEvent& aEvent);

	virtual void MHFLoad(RHTTPSession aSession, THTTPFilterHandle);
	virtual void MHFUnload(RHTTPSession aSession, THTTPFilterHandle);

	TInt iIndex;
	CHTTPFilterTest* iTest;
private:
	CMyErrorFilter(TInt aIndex, CHTTPFilterTest* aTest) { iIndex = aIndex; iTest = aTest;}
	};

CMyErrorFilter* CMyErrorFilter::NewL(TInt aIndex, CHTTPFilterTest* aTest)
	{
	return new (ELeave) CMyErrorFilter(aIndex, aTest);
	}
void CMyErrorFilter::MHFRunL(RHTTPTransaction aTransaction,
							 const THTTPEvent& aEvent)
	{
	iTest->Utils().LogIt(_L("Filter result %d is %d should be %d"), 
					   callCount, iIndex, callOrder[callCount]);
	iTest->EndTest(callOrder[callCount++]!=iIndex);
	iTest->Utils().LogIt(_L("filtering..."));


	if (aEvent == THTTPEvent::ECancel || aEvent == THTTPEvent::EClosed)
		return;

	if (callCount < 32)
		User::Leave(-1);
	if (callCount == 36)
		{
		aTransaction.Close();
		User::Leave(-1);
		}
	if (callCount == 45)
		User::Leave(-1);
	}

TInt CMyErrorFilter::MHFRunError(TInt /*aError*/, RHTTPTransaction aTransaction,
								 const THTTPEvent& /*aEvent*/)
	{
	if (callCount == 45)
		aTransaction.Close();
	return 0;
	}

void CMyErrorFilter::MHFUnload(RHTTPSession /*aSession*/, THTTPFilterHandle)
	{
	delete this;
	};

void CMyErrorFilter::MHFLoad(RHTTPSession, THTTPFilterHandle)
	{
	}

void CMyErrorFilter::MHFSessionRunL(const THTTPSessionEvent& /*aEvent*/)
	{
	}

TInt CMyErrorFilter::MHFSessionRunError(TInt aError, const THTTPSessionEvent& /*aEvent*/)
	{
	return aError;
	}


/*------------------------------------------------------------------------------*/

class CSessionEventFilter : public MHTTPFilter
	{	
public:
	static CSessionEventFilter* NewL(TInt aIndex, CHTTPFilterTest* aTest);
	virtual void MHFRunL(RHTTPTransaction aTransaction,
						 const THTTPEvent& aEvent);
	virtual TInt MHFRunError(TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& aEvent);
	virtual void MHFSessionRunL(const THTTPSessionEvent& aEvent);
	virtual TInt MHFSessionRunError(TInt aError, const THTTPSessionEvent& aEvent);

	virtual void MHFLoad(RHTTPSession aSession, THTTPFilterHandle);
	virtual void MHFUnload(RHTTPSession aSession, THTTPFilterHandle);

	TInt iIndex;
	CHTTPFilterTest* iTest;
private:
	CSessionEventFilter(TInt aIndex, CHTTPFilterTest* aTest) { iIndex = aIndex; iTest = aTest;}
	};


CSessionEventFilter* CSessionEventFilter::NewL(TInt aIndex, CHTTPFilterTest* aTest)
	{
	return new (ELeave) CSessionEventFilter(aIndex, aTest);
	}

void CSessionEventFilter::MHFSessionRunL(const THTTPSessionEvent& /*aEvent*/)
	{
	iTest->Utils().LogIt(_L("Filter result %d is %d should be %d"), 
					   sessionCallCount, iIndex, sessionCallOrder[sessionCallCount]);
	iTest->EndTest(sessionCallOrder[sessionCallCount++]!=iIndex);
	iTest->Utils().LogIt(_L("filtering..."));
	}

TInt CSessionEventFilter::MHFSessionRunError(TInt /*aError*/, const THTTPSessionEvent& /*aEvent*/)
	{
	return 0;
	}

void CSessionEventFilter::MHFUnload(RHTTPSession /*aSession*/, THTTPFilterHandle)
	{
	delete this;
	};

void CSessionEventFilter::MHFLoad(RHTTPSession, THTTPFilterHandle)
	{
	}

TInt CSessionEventFilter::MHFRunError(TInt aError, RHTTPTransaction /*aTransaction*/,
								 const THTTPEvent& /*aEvent*/)
	{
	return aError;
	}

void CSessionEventFilter::MHFRunL(RHTTPTransaction /*aTransaction*/, const THTTPEvent& /*aEvent*/)
	{
	}


/*-------------------------------------------------------------------------------*/
 
CHTTPFilterTest* CHTTPFilterTest::NewL()
	{
	return new (ELeave) CHTTPFilterTest;
	}

void CHTTPFilterTest::DoRunSessionEventTestsL()
	{
	sessionCallCount = 0;
	RHTTPSession session;
	session.OpenL();
	CleanupClosePushL(session);

	TInt KEventUid1 = 1;
	TInt KEventUid2 = 2;

	CSessionEventFilter* sessionFilter1 = CSessionEventFilter::NewL(100001, this);
	CleanupStack::PushL(sessionFilter1);
	session.FilterCollection().AddFilterL(*sessionFilter1, THTTPEvent(THTTPEvent::EAnySessionEvent, KEventUid1),
											1, RStringF());
	CleanupStack::Pop(sessionFilter1);
	CSessionEventFilter* sessionFilter2 = CSessionEventFilter::NewL(100002, this);
	CleanupStack::PushL(sessionFilter2);
	session.FilterCollection().AddFilterL(*sessionFilter2, THTTPEvent(THTTPEvent::EAnySessionEvent, KEventUid2),
											2, RStringF());
	CleanupStack::Pop(sessionFilter2);

	CSessionEventFilter* sessionFilter3 = CSessionEventFilter::NewL(100003, this);
	CleanupStack::PushL(sessionFilter3);
	session.FilterCollection().AddFilterL(*sessionFilter3, THTTPEvent(THTTPEvent::EAnySessionEvent, KHTTPMatchAnyEventUid),
											3, RStringF());
	CleanupStack::Pop(sessionFilter3);

	CSessionEventFilter* sessionFilter4 = CSessionEventFilter::NewL(100004, this);
	CleanupStack::PushL(sessionFilter4);
	session.FilterCollection().AddFilterL(*sessionFilter4, THTTPEvent(100004, KHTTPMatchAnyEventUid),
											4, RStringF());
	CleanupStack::Pop(sessionFilter4);

	CSessionEventFilter* sessionFilter5 = CSessionEventFilter::NewL(100005, this);
	CleanupStack::PushL(sessionFilter5);
	session.FilterCollection().AddFilterL(*sessionFilter5, THTTPEvent(100005, KEventUid2),
											5, RStringF());
	CleanupStack::Pop(sessionFilter5);

	// Test outgoing session events
	session.SendSessionEventL(THTTPSessionEvent(100005, KEventUid2),
								THTTPSessionEvent::EOutgoing, THTTPFilterHandle(THTTPFilterHandle::EClient));
	session.SendSessionEventL(THTTPSessionEvent(100005, KEventUid1 ),
								THTTPSessionEvent::EOutgoing, THTTPFilterHandle(THTTPFilterHandle::EClient));
	
	session.SendSessionEventL(THTTPSessionEvent(100004, KEventUid1),
								THTTPSessionEvent::EOutgoing, THTTPFilterHandle(THTTPFilterHandle::EClient));
	session.SendSessionEventL(THTTPSessionEvent(100004, KEventUid2),
								THTTPSessionEvent::EOutgoing, THTTPFilterHandle(THTTPFilterHandle::EClient));

	
	// Test incoming session events
	session.SendSessionEventL(THTTPSessionEvent(100005, KEventUid2),
								THTTPSessionEvent::EIncoming, THTTPFilterHandle(THTTPFilterHandle::EProtocolHandler));
	session.SendSessionEventL(THTTPSessionEvent(100005, KEventUid1 ),
								THTTPSessionEvent::EIncoming, THTTPFilterHandle(THTTPFilterHandle::EProtocolHandler));
	
	session.SendSessionEventL(THTTPSessionEvent(100004, KEventUid1),
								THTTPSessionEvent::EIncoming, THTTPFilterHandle(THTTPFilterHandle::EProtocolHandler));
	session.SendSessionEventL(THTTPSessionEvent(100004, KEventUid2),
								THTTPSessionEvent::EIncoming, THTTPFilterHandle(THTTPFilterHandle::EProtocolHandler));

	CQuitObject* quit = CQuitObject::NewL(22, this, ETrue);
	CleanupStack::PushL(quit);
	Utils().LogIt(_L("Creating transaction"));
	
	StartSchedulerL();

	CleanupStack::PopAndDestroy(2); //quit, session
	}

void CHTTPFilterTest::DoRunL()
	{
	iExpectedStatusCode = 0;
	iResult = 0;
	callCount = 0;
	TUriParser8 uri;
	uri.Parse(_L8("http://www.symbian.com"));

	// Do Session Event Tests first
	DoRunSessionEventTestsL();

	
	Utils().LogIt(_L("Creating session"));
	RHTTPSession a;
	CleanupClosePushL(a);
	a.OpenL();
	


	Utils().LogIt(_L("Adding filters"));
	CFTFilter* filter1 = CFTFilter::NewL(1, this);
	CleanupStack::PushL(filter1);
	a.FilterCollection().AddFilterL(*filter1, THTTPEvent(THTTPEvent::EAnyTransactionEvent),
				 RStringF(), KAnyStatusCode, 1, RStringF());
	CleanupStack::Pop(filter1);

	CFTFilter* filter2 = CFTFilter::NewL(2, this);
	CleanupStack::PushL(filter2);
	a.FilterCollection().AddFilterL(*filter2, THTTPEvent(THTTPEvent::EAnyTransactionEvent),
				 RStringF(), KAnyStatusCode, 4, RStringF());
	CleanupStack::Pop(filter2);

	CFTFilter* filter3 = CFTFilter::NewL(3, this);
	CleanupStack::PushL(filter3);
	a.FilterCollection().AddFilterL(*filter3, THTTPEvent(33),
				 RStringF(), KAnyStatusCode, 6, RStringF());
	CleanupStack::Pop(filter3);

	CFTFilter* filter4 = CFTFilter::NewL(4, this);
	CleanupStack::PushL(filter4);
	a.FilterCollection().AddFilterL(*filter4, THTTPEvent(THTTPEvent::EAnyTransactionEvent),
				 RStringF(), KAnyStatusCode, 9, RStringF());
	CleanupStack::Pop(filter4);

	CQuitObject* quit = CQuitObject::NewL(24, this);
	CleanupStack::PushL(quit);
	Utils().LogIt(_L("Creating transaction"));
	RHTTPTransaction b;
	b = a.OpenTransactionL(uri, *this, 
							 a.StringPool().OpenFStringL(KPost));
	Utils().LogIt(_L("starting events"));
	b.SendEventL(THTTPEvent(33), THTTPEvent::EOutgoing,
				 THTTPFilterHandle(THTTPFilterHandle::EClient));
	b.SendEventL(THTTPEvent(22), THTTPEvent::EOutgoing,
				 THTTPFilterHandle(THTTPFilterHandle::EClient));
	b.SendEventL(THTTPEvent(22,KHTTPMatchAnyEventUid), THTTPEvent::EOutgoing,
				 THTTPFilterHandle(THTTPFilterHandle::EClient));
	b.SendEventL(THTTPEvent(33), THTTPEvent::EOutgoing,
				 THTTPFilterHandle(THTTPFilterHandle::EClient));
	Utils().LogIt(_L("Starting filtering"));
	StartSchedulerL();
	Utils().LogIt(_L("Adding filter/event for error tests"));
	b.Close();

	CleanupStack::PopAndDestroy(quit);

	CMyErrorFilter* filter5 = CMyErrorFilter::NewL(6, this);
	
	CleanupStack::PushL(filter5);
	a.FilterCollection().AddFilterL(*filter5, THTTPEvent(THTTPEvent::EAnyTransactionEvent),
				 RStringF(), KAnyStatusCode, 11, RStringF());
	CleanupStack::Pop(filter5);

	quit = CQuitObject::NewL(35, this);
	CleanupStack::PushL(quit);

	b = a.OpenTransactionL(uri, *this, 
							 a.StringPool().OpenFStringL(KPost));
	b.SendEventL(THTTPEvent(33), THTTPEvent::EOutgoing,
				 THTTPFilterHandle(THTTPFilterHandle::EClient));
	Utils().LogIt(_L("filtering..."));
	StartSchedulerL();
	Utils().LogIt(_L("Testing session delete/leave interaction"));
	b.SendEventL(THTTPEvent(33), THTTPEvent::EOutgoing,
				 THTTPFilterHandle(THTTPFilterHandle::EClient));

	CleanupStack::PopAndDestroy(quit);
	quit = CQuitObject::NewL(44, this);
	CleanupStack::PushL(quit);

	StartSchedulerL();

	Utils().LogIt(_L("Deleting from a RunError"));
	RHTTPTransaction c;
	c = a.OpenTransactionL(uri, *this, 
						   a.StringPool().OpenFStringL(KPost));
	c.SendEventL(THTTPEvent(33), THTTPEvent::EOutgoing,
				 THTTPFilterHandle(THTTPFilterHandle::EClient));

	CleanupStack::PopAndDestroy(quit);
	quit = CQuitObject::NewL(53, this);
	CleanupStack::PushL(quit);

	StartSchedulerL();

	Utils().LogIt(_L("Testing status code matching with no response"));
	CFTFilter* filter6 = CFTFilter::NewL(7, this);
	CleanupStack::PushL(filter6);
	a.FilterCollection().AddFilterL(*filter6, THTTPEvent(THTTPEvent::EAnyTransactionEvent),
				 RStringF(), 200, 10, RStringF());
	CleanupStack::Pop(filter6);
	RHTTPTransaction d;
	d = a.OpenTransactionL(uri, *this, 
						   a.StringPool().OpenFStringL(KPost));
	d.SendEventL(THTTPEvent(33), THTTPEvent::EOutgoing,
				 THTTPFilterHandle(THTTPFilterHandle::EClient));

	CleanupStack::PopAndDestroy(quit);
	quit = CQuitObject::NewL(58, this);
	CleanupStack::PushL(quit);

	StartSchedulerL();
	Utils().LogIt(_L("Testing status code matching - codes do match"));
	d.Response().SetStatusCode(200);
	d.SendEventL(THTTPEvent(33), THTTPEvent::EOutgoing,
				 THTTPFilterHandle(THTTPFilterHandle::EClient));

	CleanupStack::PopAndDestroy(quit);
	quit = CQuitObject::NewL(64, this);
	CleanupStack::PushL(quit);

	StartSchedulerL();
	Utils().LogIt(_L("Testing status code matching - codes dont match"));
	d.Response().SetStatusCode(201);
	d.SendEventL(THTTPEvent(33), THTTPEvent::EOutgoing,
				 THTTPFilterHandle(THTTPFilterHandle::EClient));

	CleanupStack::PopAndDestroy(quit);
	quit = CQuitObject::NewL(69, this);
	CleanupStack::PushL(quit);

	StartSchedulerL();
	Utils().LogIt(_L("Testing header matching - correct header not present"));
	CFTFilter* filter7 = CFTFilter::NewL(8, this);
	CleanupStack::PushL(filter7);
	RStringF wibble = a.StringPool().OpenFStringL(_L8("wibble"));
	CleanupClosePushL(wibble);
	RStringF wobble = a.StringPool().OpenFStringL(_L8("wobble"));
	CleanupClosePushL(wobble);
	d.Close();
	a.FilterCollection().AddFilterL(*filter7, THTTPEvent(THTTPEvent::EAnyTransactionEvent),
				 wibble, KAnyStatusCode, 10, RStringF());

	CleanupStack::Pop(2); // wibble, wobble
	CleanupStack::Pop(filter7);
	CleanupClosePushL(wibble);
	CleanupClosePushL(wobble);

	d = a.OpenTransactionL(uri, *this, 
						   a.StringPool().OpenFStringL(KPost));
	THTTPHdrVal val(wobble);
	d.Response().GetHeaderCollection().SetFieldL(wobble, val);
	d.SendEventL(THTTPEvent(33), THTTPEvent::EOutgoing,
				 THTTPFilterHandle(THTTPFilterHandle::EClient));

	CleanupStack::Pop(2); // wibble, wobble
	CleanupStack::PopAndDestroy(quit);
	CleanupClosePushL(wibble);
	CleanupClosePushL(wobble);
	quit = CQuitObject::NewL(82, this);
	CleanupStack::PushL(quit);	

	StartSchedulerL();
	Utils().LogIt(_L("Testing header matching - correct header present"));
	d.Response().GetHeaderCollection().SetFieldL(wibble, val);
	d.SendEventL(THTTPEvent(33), THTTPEvent::EOutgoing,
				 THTTPFilterHandle(THTTPFilterHandle::EClient));

	CleanupStack::PopAndDestroy(quit);
	quit = CQuitObject::NewL(88, this);
	CleanupStack::PushL(quit);	

	StartSchedulerL();
	Utils().LogIt(_L("deleting session"));
	CleanupStack::PopAndDestroy(quit);
	wibble.Close();
	wobble.Close();
	CleanupStack::Pop(2); // wibble, wobble
	CleanupStack::Pop(); // a
	a.Close();
	}

void CHTTPFilterTest::MHFRunL(RHTTPTransaction /*aTransaction*/,
						const THTTPEvent& /*aEvent*/)
	{
	Utils().LogIt(_L("Filter result %d is 5 (client) should be %d"), 
					   callCount, callOrder[callCount]);
	EndTest(callOrder[callCount++]!=5);
	Utils().LogIt(_L("filtering..."));
	}

TInt CHTTPFilterTest::MHFRunError(TInt aError, RHTTPTransaction /*aTransaction*/, const THTTPEvent& /*aEvent*/)
	{
	return aError;
	}

CHTTPTestUtils& CHTTPFilterTest::Utils()
	{
	return iEngine->Utils();
	}

void CHTTPFilterTest::EndTest(TInt aResult)
	{
	if (aResult)
		{
		Utils().LogIt(_L("TEST FAILED WITH RESULT %d"), aResult);
		iResult = -KErrTestFailed;
		if (iSchedulerRunning)
			CActiveScheduler::Stop();
		}
	}

const TDesC& CHTTPFilterTest::TestName()
	{
	_LIT(KHeaderTestName,"CHttpFilterTest");
	return KHeaderTestName;
	}
void CHTTPFilterTest::DoCancel()
	{
	}
	

void CHTTPFilterTest::StartSchedulerL()
	{
	iSchedulerRunning = ETrue;
	CActiveScheduler::Start();
	iSchedulerRunning = EFalse;
	if (iResult)
		User::Leave(-iResult);
	}

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

#ifndef __CWSPEVENTDRIVER_H__
#define __CWSPEVENTDRIVER_H__

#include <e32base.h>
#include <http/rhttpsession.h>
#include <http/rhttptransaction.h>

#include "t_wspeventdriver/CEventCallbackFilter.h"

class CDriverTrans;
class MDriverTransObs
	{
public:
	virtual void Closed(CDriverTrans& aDrTr) = 0;
	};

class CDriverDataSupplier;
class MConsoleDumper;
class CDriverTrans : public CBase
	{
public: // enums
	enum THdrValType
		{
		ETInt = 0,
		ERString = 1,
		ERStringF = 2,
		ETDateTime = 3
		};

public:
	IMPORT_C static CDriverTrans* NewL(RHTTPSession aSession, RStringF aMethod, TUriC8 aUri,
									   MDriverTransObs& aObs, MHTTPTransactionCallback& aCB,
									   const TDesC& aId);
	IMPORT_C virtual ~CDriverTrans();

	IMPORT_C RHTTPTransaction Trans() const;

	IMPORT_C const TDesC& TransId() const;

public: // scriptable/command-driven methods
	IMPORT_C void AddHeaderL(RStringF aFieldName, THdrValType aFieldType, RString aFieldValue);

	IMPORT_C void AddHeaderWithParamL(RStringF aFieldName, THdrValType aFieldType, RString aFieldValue,
									 RStringF aParamName, THdrValType aParamType, RString aParamValue);

	IMPORT_C void AddBody(MHTTPDataSupplier& aBody);

	IMPORT_C void SubmitL();

	IMPORT_C void Cancel();

	IMPORT_C void Close();

	IMPORT_C TBool GetHeaderL(RStringF aFieldName, THdrValType& aFieldType, RString& aFieldValue, TInt aPart=0);
	
	IMPORT_C TInt GetStatusCode();

	IMPORT_C MHTTPDataSupplier& GetBody();

	IMPORT_C void MoreRequestBodyDataL();

	static void ConvertHeaderValueL(RString aInValue, THdrValType aType, THTTPHdrVal& aOutValue);

	static void DumpHeaderCollectionL(RStringPool aStrP, RHTTPHeaders aHdr, TDes& aWorkingBuffer, MConsoleDumper& aDumper);

public: // command-driven methods (not expected to be scripted)
	IMPORT_C void DumpResponseHeadersL(MConsoleDumper& aConsole);
	IMPORT_C void DumpResponseBody(MConsoleDumper& aConsole);

private:
	CDriverTrans(RHTTPSession aSession, MDriverTransObs& aObs);
	void ConstructL(RStringF aMethod, TUriC8 aUri, MHTTPTransactionCallback& aCB, const TDesC& aId);
private:
	RHTTPSession iSession;
	RHTTPTransaction iTransaction;
	MDriverTransObs& iObs;
	HBufC* iTransId;
	};


class CWspEventDriver : public CBase, public MDriverTransObs, public MTransConverter
	{
public:	// Methods

	IMPORT_C static CWspEventDriver* NewL(CEventCallbackFilter& aCallbackFilter,
										  const TDesC& aTrHndIniFile);

	IMPORT_C ~CWspEventDriver();

	IMPORT_C RStringPool StringPool();

	IMPORT_C TInt Reset();

// Scriptable methods

	IMPORT_C void SessionConnectL();

	IMPORT_C void SessionDisconnectL();

	IMPORT_C void SetClientMessageSizeL(TUint32 aSize);

	IMPORT_C void SetServerMessageSizeL(TUint32 aSize);

	IMPORT_C void SetUseAcknowledgementsL();

	IMPORT_C void SetUseSuspendResumeL();

	IMPORT_C void SetMethodMORL(TUint32 aMOR);

	IMPORT_C void SetConnectionPropertiesL(RStringF aProxyAddress, RStringF aBearer, TInt aLocalPort,
										   TInt aRemotePort, TInt aServiceNum, RStringF aSessionType,
										   RStringF aIsSecure, TInt aTimeOut);

	IMPORT_C void GetConnectionProperties(RStringF& aProxyAddress, RStringF& aBearer, TInt& aLocalPort,
										  TInt& aRemotePort, TInt& aServiceNum, RStringF& aSessionType,
										  RStringF& aIsSecure, TInt& aTimeOut);

	IMPORT_C CDriverTrans* OpenTransactionL(RStringF aMethod, TUriC8 aUri, const TDesC& aId);

	IMPORT_C void AddRequestSessionHeaderL(RStringF aFieldName, CDriverTrans::THdrValType aFieldType, RString aFieldValue);

	IMPORT_C void AddRequestSessionHeaderWithParamL(RStringF aFieldName, CDriverTrans::THdrValType aFieldType, RString aFieldValue,
													RStringF aParamName, CDriverTrans::THdrValType aParamType, RString aParamValue);

// Command-driven methods

	IMPORT_C void DumpSessionHeadersL(MConsoleDumper& aConsole);

private:	// Methods inherited from MDriverTransObs

	virtual void Closed(CDriverTrans& aDrTr);

private:	// Methods inherited from MTransConverter

	virtual CDriverTrans& LookUpTransaction(RHTTPTransaction aTrans);

private:	// Methods

	CWspEventDriver(CEventCallbackFilter& aCallbackFilter);

	void ConstructL(const TDesC& aTrHndIniFile);

private:	// Attributes

	RHTTPSession				iSession;
	RStringPool					iStrP;
	CEventCallbackFilter&		iCallbackFilter;
	RPointerArray<CDriverTrans>	iTransactionList;
	};

#endif	// __CWSPEVENTDRIVER_H__
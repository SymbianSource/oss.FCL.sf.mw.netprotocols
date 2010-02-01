// Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// $Header$
// The header file introducing the collection of HTTP command classes
// of the Integration Harness framework.
// on:	aug 2002,
// 
//

#ifndef __HTTPTRANSACTIONCMDS_H__
#define __HTTPTRANSACTIONCMDS_H__

//-----------------------------------------------------------------------------
//	Command:	The 'TRANSACTION' command which creates a transaction in the named session etc

class CTestTransaction;

class CCmdTransaction : public CCmdBase 
{
public:

	static CCmdTransaction *NewL (TInt, const TDesC& aKeyphrase, const TDesC& aHelpPhrase);
	static CCmdTransaction *NewLC(TInt, const TDesC& aKeyphrase, const TDesC& aHelpPhrase);
	virtual ~CCmdTransaction();

	//	helper defs and methods for Transaction processing
	enum TTransactionModes
		{
		EIdleTransaction,
		EGetTransaction,
		EPostTransaction,
		EHeadTransaction,
		ETraceTransaction,

		//	add new modes above this line
		EMaxTransactionModes
		};

protected:

	void ConstructL(TInt aCommandId, const TDesC& aKeyphrase, const TDesC& aHelpPhrase);
	CCmdTransaction() {};		//	ensure a concrete class!

public:
	TInt  ProcessL (const TDesC& aCommand);

//	private methods for our own personal use!
private:

	TBool checkConnectionExists(TPtrC &);
	TBool checkMethodExists(TPtrC &);
	TInt  ParseCmdArgs( const TDesC& aCommand, TPtrC& aTransName, TPtrC& aConName, TPtrC& aTransMthd,
						TPtrC& aTransURI, TPtrC& aPostData);
	void  ShowTransactions();


	void CloseTransaction(RHTTPTransaction &);

private:

	TInt iExpectedError;
	TBool iExpectError;
	TInt iTestFail;
	TInt iExpectedStatusCode;
	THTTPEvent iReceivedError;
	TInt iTransactionIndex;
	HTTP::TStrings eStrIndex;
	TBool iHasARequestBody;
	TInt iExpectedNumberRedirects;

	TInt iSessionEventIndex;
	TInt iTransactionEventIndex;
};

class CCmdEndTransaction : public CCmdBase 
{
public:

	static CCmdEndTransaction *NewL (TInt, const TDesC& aKeyphrase, const TDesC& aHelpPhrase);
	static CCmdEndTransaction *NewLC(TInt, const TDesC& aKeyphrase, const TDesC& aHelpPhrase);
	virtual ~CCmdEndTransaction();

protected:

	void ConstructL(TInt aCommandId, const TDesC& aKeyphrase, const TDesC& aHelpPhrase);
	CCmdEndTransaction() {};		//	ensure a concrete class!

public:
	TInt  ProcessL (const TDesC& aCommand);

//	private methods for our own personal use!
private:

	TInt  ParseCmdArgs( const TDesC& aCommand, TPtrC& aTransName);
};

class CCmdShowTransaction : public CCmdBase 
{
public:

	static CCmdShowTransaction *NewL (TInt, const TDesC& aKeyphrase, const TDesC& aHelpPhrase);
	static CCmdShowTransaction *NewLC(TInt, const TDesC& aKeyphrase, const TDesC& aHelpPhrase);
	virtual ~CCmdShowTransaction();

protected:

	void ConstructL(TInt aCommandId, const TDesC& aKeyphrase, const TDesC& aHelpPhrase);
	CCmdShowTransaction() {};		//	ensure a concrete class!

public:
	TInt  ProcessL (const TDesC& aCommand);
};

//-----------------------------------------------------------------------------

#endif  // __HTTPTRANSACTIONCMDS_H__

//-----------------------------------------------------------------------------
// End of File
//-----------------------------------------------------------------------------


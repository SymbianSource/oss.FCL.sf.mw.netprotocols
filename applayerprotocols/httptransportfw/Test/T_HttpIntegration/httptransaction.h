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

#ifndef __HTTPTRANSACTION_H__
#define __HTTPTRANSACTION_H__

//-----------------------------------------------------------------------------

class CTestTransaction : public CBase,
						 public MHTTPDataSupplier,
						 public MHTTPTransactionCallback
								
{
public:

	static CTestTransaction *NewL ( RHTTPSession& aSession,
									HTTP::TStrings aTransactiontype,
									const TDesC& aUri,
									const TDesC &aFilename,
									const TDesC &aTransactionName,
									CTEngine *aTestMachine);
							    
   	static CTestTransaction *NewLC ( RHTTPSession& aSession,
									HTTP::TStrings aTransactiontype,
									const TDesC& aUri,
									const TDesC &aFilename,
									const TDesC &aTransactionName,
									CTEngine *aTestMachine);

	virtual ~CTestTransaction();
	virtual void MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent);
	virtual TInt MHFRunError(TInt aError,RHTTPTransaction aTransaction, const THTTPEvent& aEvent);

	//	helper defs and methods for Transaction processing
	enum TestTransactionStates
		{
		EUnInitialised,
		EActive,
		EClosed
		};
		
	void SubmitL();
	TInt SetFileNameL(const TDesC &aName) ;
	void  CloseTransaction();

	
	//	from DataSupplier
	TBool GetNextDataPart(TPtrC8 &aDataPart);
	TInt OverallDataSize();
	void ReleaseData();
	TInt  Reset();

	// Set command family (the known commands).
//	inline void SetTransaction(RHTTPTransaction &);
	TBuf<32> TransactionName() const {return iTransactionName;}
	const TDesC8&  Uri() const {return iUri;}
		
	void SetTransaction(RHTTPTransaction &);
//	inline const RHTTPTransaction &Transaction() const;
	const RHTTPTransaction &Transaction() const;
	TestTransactionStates State() {return iState;} 

private: 							       

	void ConstructL( RHTTPSession& aSession);

    CTestTransaction( HTTP::TStrings aTransactiontype,
					  const TDesC& aUri,
					  const TDesC &aFilename,
					  const TDesC &aTransactionName,
					  CTEngine *aTestMachine);
					  
	inline CTEngine* Machine() ;
	TInt Error( TInt aError, TRefByValue<const TDesC> aFmt, ... );
	void Log(TRefByValue<const TDesC> aFmt, ... );
	void WriteDateStamp();
	void WriteDateStamp(const TDateTime &aDate);
	TBool AddRequestBodyL(RHTTPHeaders &aHeaders, RStringPool &aStrPool);
	TBool AddAcceptHeaderL(RHTTPHeaders &aHeaders, RStringPool &aStrPool);
	TBool AcceptCharSetL(RHTTPHeaders &aHeaders, RStringPool &aStrPool);
	TBool AcceptEncodingL(RHTTPHeaders &aHeaders, RStringPool &aStrPool);
	TBool SetUserAgentL(RHTTPHeaders &aHeaders, RStringPool &aStrPool);
	TBool SetContentLengthL(RHTTPHeaders &aHeaders, RStringPool &aStrPool);
	void  DoneWaitEvents();
	void  DumpResponseBody(RHTTPTransaction &aTrans);
	void  DumpRespHeaders(RHTTPTransaction &aTrans);
	
private:
	enum TransactionFilePanics
		{
		EBufferPanic,
		EBufferNotAllocated,
		EInvalidFileSizeError
		};
		
	RHTTPTransaction iTransaction ;

	TBuf<32>  iTransactionName;
	TBuf8<256> iUri;
	TBuf<256> iFilename;
	CTEngine* iEngine;
	HTTP::TStrings iTransactionType;
	
	RFile 	iReqFile;
	HBufC8 	*iFileBuffer;
	TPtr8 	iFileBufferPtr;
	TInt    iDataChunkCount;
	TBool   iReleaseData;
	
	TestTransactionStates iState;
};

/*
inline const RHTTPTransaction &CTestTransaction::Transaction() const
	{
    return (iTransaction);
    }
  */
/*    
inline void  CTestTransaction::SetTransaction(RHTTPTransaction &aTransaction)
	{
	iTransaction = aTransaction;
	}
*/
inline CTEngine* CTestTransaction::Machine()
	{
	return iEngine ;
	}


#endif  // __HTTPTRANSACTION_H__

//-----------------------------------------------------------------------------
// End of File
//-----------------------------------------------------------------------------


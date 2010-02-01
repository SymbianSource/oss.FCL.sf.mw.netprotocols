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

#ifndef __CSESSION_H__
#define __CSESSION_H__

#include <e32base.h>
#include <ecom/implementationinformation.h>
#include <http/rhttpconnectioninfo.h>
#include <http/rhttpsession.h>
#include <http/mhttptransactioncallback.h>
#include <http/framework/cprotocolhandler.h>
#include "TClientFilter.h"
#include "timerlogger.h"
#include <http/mhttpdataoptimiser.h>

class CHTTPManager;
class CTransaction;
class CHTTPTransactionCodec;
class THTTPFilterRegistration;
class CHeaders;
class MHTTPFilterCreationCallback;
class TFilterConfigurationIterator;
class CEComFilter;

//##ModelId=3C4C0F4800BF
class TDummyTransactionCallback: public MHTTPTransactionCallback
	{
public:	// Methods from MHTTPTransactionCallback
	
/**
	@see			MHTTPFilterBase
 */
	//##ModelId=3C4C0F4800D8
	virtual void MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent);

/**
	@see			MHTTPFilterBase
 */
	//##ModelId=3C4C0F4800D4
	virtual TInt MHFRunError(TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& aEvent);

	};

// Struct for containing a session's filter information
//##ModelId=3C4C0F37027D
class TSessionFilterInfo
	{
public: // enumerations
	// Enum defining the filter category
	enum TFilterCategory
		{
		EMandatory, // Mandatory filter
		EImplicit,  // Implicit filter
		EExplicit	// Explicit filter
		};
public: // data members
	//##ModelId=3C4C0F3702B1
	CImplementationInformation* iFilterInfo;	// ECom filter implementation info
	//##ModelId=3C4C0F3702A7
	TFilterCategory iCategory;					// The filter category
	//##ModelId=3C4C0F37029D
	CEComFilter* iFilterPlugin;					// Pointer to the filter plugin if installed
	};

// Implementation class for the session @see RHTTPSession
//##ModelId=3C4C187700D5
class CHTTPSession : public CBase
	{
 public:
	// Constructs a session with default parameters
	//##ModelId=3C4C187802AD
	static CHTTPSession* NewL();
	// Constructs a session with the specifed protocol
	//##ModelId=3C4C187802B7
	static CHTTPSession* NewL(const TDesC8& aProtocol);
	/**
	Constructs a session that allows filter configuration. This requires the callback
	interface of MHTTPFilterCreationCallback that is called during construction, so
	the client can determine the filters that are installed.
	*/
	//##ModelId=3C4C187802CB
	static CHTTPSession* NewL(const TDesC8& aProtocol, MHTTPFilterCreationCallback* aSessionCallback);
	/**
	This methods lists all of the available protocols on the device. These are identified by
	using the list of protocol handler plugins discovered by ECom and appending the data type
	descriptors to the array. Any existing data in the array is deleted.
	*/
	//##ModelId=3C4C187702E8
	static void ListAvailableProtocolsL(RPointerArray<HBufC8>& aProtocolArray);
	// Destructor
	//##ModelId=3C4C187702E0
	~CHTTPSession();
	// Returns the manager. Does not transfer ownership
	//##ModelId=3C4C187702DE
	CHTTPManager& Manager();
	// Returns the manager. Does not transfer ownership
	//##ModelId=3C4C187702DF
	const CHTTPManager& Manager() const;
	// Returns the filter queue
	//##ModelId=3C4C187702D5
	RArray<THTTPFilterRegistration>& FilterQueue();
	// Returns the filter queue
	//##ModelId=3C4C187702D6
	const RArray<THTTPFilterRegistration>& FilterQueue() const;
	// Returns the next free filter handle
	//##ModelId=3C4C187702D4
	TInt NextFilterHandle();
	// Returns the next transaction ID
	//##ModelId=3C4C187702CA
	TInt NextTransactionID();

	// Adds a transaction to the session
	//##ModelId=3C4C187702C0
	void AddTransactionL(CTransaction* aTransaction);
	// Removes a transaction from the session
	//##ModelId=3C4C187702AC
	void RemoveTransaction(CTransaction* aTransaction);

	// Returns the codec
	//##ModelId=3C4C187702A5
	CHeaderCodec* Codec() const;
	// Returns the proxy/connection details
	//##ModelId=3C4C187702A4
	RHTTPConnectionInfo ConnectionInfo() const;

	//##ModelId=3C4C187702A3
	inline RHTTPSession Handle();

	/** Returns the request session headers. 
	This will create the protocol handler if it doesn't already exist
	 @leave Leaves if protocol handler doesn't exist and fails to be created*/
	//##ModelId=3C4C187702A2
	RHTTPHeaders RequestSessionHeadersL();

	/** Returns the response session headers. 
	This will create the protocol handler if it doesn't already exist
	 @leave KErrNoMemory if attempt to allocate memory fails*/
	//##ModelId=3C4C18770298
	RHTTPHeaders ResponseSessionHeadersL();
	
	// Creates the Protocol handler if necessary, called in transaction creation to allow delayed creation
	//##ModelId=3C4C18770285
	void CreateProtocolHandlerL();

	//##ModelId=3C4C18770284
	inline TBool CanAddFilters() const;

	// Returns the filter configuration iterator if one exists, otherwise will return NULL
	//##ModelId=3C4C1877027A
	TFilterConfigurationIterator* FilterConfigIterator() const;

	// Sets the session event callback
	//##ModelId=3C4C18770270
	void SetSessionEventCallback(MHTTPSessionEventCallback* aSessionEventCallback);

	// Send a session event
	//##ModelId=3C4C1877023E
	void SendSessionEventL(THTTPSessionEvent aStatus, THTTPSessionEvent::TDirection aDirection, 
							 THTTPFilterHandle aStart = THTTPFilterHandle::EClient);

	// Fail the session event
	void FailSessionEvent(THTTPFilterHandle aStart = THTTPFilterHandle::EClient);

	/** 
		When 1 transaction runs, the session is blocked using the
		Block function. It is unblocked at the end with the Unblock
		function. 
		
		If a transaction has run, it must block, and must inform the
		session of this by calling TransactionHasBlocked
		
		If at least 1 transaction has blocked then when Unblock is
		called on the session, CTransaction::Unblock will be called on
		all the transactions. Any transaction that had blocked must
		then unblock and other transactions must do nothing.

		The purpose of this implementation is to be as simple as
		possible, especialy in terms of storage, but to minimise the
		ammount of extra code that gets executed in the normal case
		(when no transactions fire while the session is blocked). This
		is why the session does not remember which transactions have
		blocked (which would add complex storage requirements) but it
		does remember if any have (meaning that all transactions don't
		need to be unblocked in the normal case)
	*/
	//@{
	// Returns ETrue if any transaction that fires should block
	//##ModelId=3C4C18770236
	inline TBool IsBlocking() const;
	// Used to inform the session that a transaction has blocked.
	//##ModelId=3C4C18770235
	inline void TransactionHasBlocked();
	// Called by transaction to indicate that session should now block.
	//##ModelId=3C4C18770234
	inline void Block();
	/** Called by transaction to indicate that other transactions
		should stop blocking (and if transaction have blocked, they
		should be awakened) */
	//##ModelId=3C4C1877022B
	void Unblock();
	//@}

	/** Get the Server Certificate for this session.
	@param	aServerCert A TCertInfo which will be filled with the certificate information
	@return	An error code.  KErrNone if aServerCert has been completed, otherwise one of 
			the system wide error codes
	*/
	TInt SessionServerCert(TCertInfo& aServerCert);

	/** Get the Server Certificate for the current session.
	@return	A pointer to CCertificate, or NULL if not found.
	*/
	const CCertificate*  SessionServerCert();

	/** Get the Server Certificate for the specified transaction.
	@param	aServerCert A TCertInfo which will be filled with the certificate information
	@param	aTransaction The transaction for which the certificate is requested
	@return	An error code.  KErrNone if aServerCert has been completed, otherwise one of 
			the system wide error codes
	*/
	TInt TransactionServerCert(TCertInfo& aServerCert, RHTTPTransaction aTransaction);
	/** Get the default IAP from CommsDat. For this IAP retrieve Sevrice Id & Service Type.
	Using these retrieve the default proxy server name & port number. Add these to the properties set
	of current Http session.
	*/
	void SetupDefaultProxyFromCommsDatL();


	/** Get the Server Certificate for the current session.
	@param	aTransaction The transaction for which the certificate is requested
	@return	A pointer to CCertificate, or NULL if not found.
	*/
	const CCertificate*  TransactionServerCert(RHTTPTransaction aTransaction);

	inline TInt FindTransaction(CTransaction* aTransaction);

	void SetupHttpDataOptimiser (MHttpDataOptimiser& aHttpOptimiser);
 
 	MHttpDataOptimiser* HttpDataOptimiser ();

 private:
	// Constructor
	//##ModelId=3C4C1877022A
	CHTTPSession();

	// 2-phase construction
	//##ModelId=3C4C1877020C
	void ConstructL(const TDesC8& aProtocol, MHTTPFilterCreationCallback* aSessionCallback);

	// Method to construct and build filter lists and install the mandatory and implicits filters
	//##ModelId=3C4C187701F8
	void InitializeFiltersL(const TDesC8& aProtocol);

	// Creates the Protocol handler if necessary, called in transaction creation to allow delayed creation
	//##ModelId=3C4C1877028E
	void CreateProtocolHandlerL(const TDesC8& aProtocol);

	// Method used to cleanup the ECom implementation array for TCleanupItem
	static void CleanListImplementationArray(TAny* aArray);
     
 private:
	// Friend class
	friend class TFilterConfigurationIterator;

	enum TBlockingStates
		{
		ENormal = 0,	// Filters can run
		EShouldBlock,	// Blocking (Filters should not run)
		EHasBlocked		// Blocking, and a transaction has failed to run due to the blocked state.
		};
	
	// The proxy/connection details.
	//##ModelId=3C4C187701EE
	CHeaderFieldPart* iConnectionInfo;
	// The manager (singleton)
	//##ModelId=3C4C187701DA
	CHTTPManager* iManager;
	// The filter queue
	//##ModelId=3C4C187701C8
	RArray<THTTPFilterRegistration> iFilterQueue;
	// A list of all the open transactions
	//##ModelId=3C4C18770194
	RArray<CTransaction*> iTransactions;
	// The protocol handler
	//##ModelId=3C4C18770178
	CProtocolHandler* iProtocolHandler;
	// The next free filter handle
	//##ModelId=3C4C1877016E
	TInt iNextFilterHandle;
	// The transaction counter, used to derive next transaction ID. Only ever incremented.
	//##ModelId=3C4C18770162
	TInt iTransCount;
	// The blocking state of the session
	//##ModelId=3C4C18770158
	TBlockingStates iBlockingState;

	// The 'client filter'
	//##ModelId=3C4C18770145
	TClientFilter iClientFilter;

	// Session request headers that will get added to each request
	//##ModelId=3C4C1877013B
	CHeaders* iRequestSessionHeaders;
	
	// Session response headers that will get added to each response
	//##ModelId=3C4C18770131
	CHeaders* iResponseSessionHeaders;
	
	// A dummy transaction used to send and receive session events 
	//##ModelId=3C4C1877011D
	CTransaction* iSessionEventTransaction;

	// A dummy transaction callback
	//##ModelId=3C4C18770113
	TDummyTransactionCallback iDummyTransactionCallback;

	// ECom implementation info array (RPointerArray), contains the plugin information
	//##ModelId=3C4C18770109
	RImplInfoPtrArray iEComFilterInfoArray;

	// Array to extend the information on the filters used for easier managing
	//##ModelId=3C4C187700FF
	RPointerArray<TSessionFilterInfo> iFilterInfoList;

	// A filter configuration iterator that can be passed to clients to congiure the
	// installed filters
	//##ModelId=3C4C187700EC
	TFilterConfigurationIterator* iFilterConfigIter;

	// Flag to indicate that the session is closing
	TBool iSessionClosing;
	
	__DECLARE_PERFORMANCE_LOG
	
	MHttpDataOptimiser* iHttpDataOptimiser;
	};

#include "CHTTPSession.inl"

#endif // __CSESSION_H

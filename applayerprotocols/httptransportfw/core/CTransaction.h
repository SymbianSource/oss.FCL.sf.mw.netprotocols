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

#ifndef CTRANSACTION_H
#define CTRANSACTION_H

#include <e32base.h>
#include "TEventRegistration.h"
#include "CHTTPSession.h"
#include <http/framework/httplogger.h>
#include <http/framework/cprotocolhandler.h>
#include <http/mhttpdataoptimiser.h>

class THTTPProxy;
class CRequest;
class CResponse;

#if defined (_DEBUG) && defined (_LOGGING)
#define __LOGEVENT(ID,EVENT,DIRECTION,FILTER) LogEvent(ID,EVENT,DIRECTION,FILTER);
#elif !defined (_DEBUG)
#define __LOGEVENT(ID,EVENT,DIRECTION,FILTER)
#endif


// Implementation class for the transaction @see RHTTPTransaction
// Private inheritance as this class should be deleted via the Close function.
//##ModelId=3C4C18840174
class CTransaction : private CActive
	{
	friend class CProtocolHandler;
 public:
	// Constructs a transaction
	//##ModelId=3C4C188403B9
	static CTransaction* NewL(MHTTPTransactionCallback& aCallback, 
							  const TUriC8& aURI,
							  CHTTPSession& aSession, 
							  RStringF aMethod);

	// @see RHTTPTransaction::SendEventL
	//##ModelId=3C4C18840391
	void SendEventL(THTTPEvent& aEvent, TInt aDirection, 
					THTTPFilterHandle aStart);
	
	TInt SendEvent(THTTPEvent& aEvent, TInt aDirection, THTTPFilterHandle aStart);
	/** Sends an event syncronously. Only callable when there are no
		outstanding events. @see SendEventL
		__ASSERT_DEBUG will panic if there are outstanding events.
	 */
	//##ModelId=3C4C18840369
	void SynchronousSendEvent(THTTPEvent aEvent, TInt aDirection, 
					THTTPFilterHandle aStart);
	// Returns the transaction's session
	//##ModelId=3C4C18840360
	RHTTPSession Session() const;

	// Returns the transaction's codec
	//##ModelId=3C4C1884035F
	CHeaderCodec& Codec() const;
	
	// Returns the request object
	//##ModelId=3C4C18840357
	inline CRequest& Request() const;

	// Returns the response object
	//##ModelId=3C4C18840356
	inline CResponse& Response() const;

	// Returns the transactions ID
	//##ModelId=3C4C18840355
	inline TInt Id() const;

	// Returns the transaction's property set
	//##ModelId=3C4C1884034B
	inline RHTTPTransactionPropertySet PropertySet() const;

	/** Cancels the transaction. This removes all outstanding events
		and sends a cancel event outward.
	*/
	//##ModelId=3C4C18840341
	void CancelTransaction(THTTPFilterHandle aStart);

	/** 'Closes the transaction'. This is essentialy the way other
        objects destroy the transaction. They don't directly call the
        destructor in order to cope with filters closing the
        transaction, in which situation the transaction will zombify
        itself, deleting itself when the stack is unwound. 
	*/
	//##ModelId=3C4C18840337
	void Close();

	/** Fails the transaction. That is to say, cancel the transaction
		and send the synchronous events 'EUnrecoverableError' and
		'EFailed'.  It implements the RHTTPTransaction::Fail
		function. */
	//##ModelId=3C4C1884032D
	inline void Fail(THTTPFilterHandle aStart);

	// Extracts the implementation from a RHTTPTransaction
	//##ModelId=3C4C1884031A
	inline static CTransaction* Implementation(RHTTPTransaction aHandle);

	// Returns the callback for this transaction.
	//##ModelId=3C4C18840319
	inline MHTTPTransactionCallback& Callback() const;

	/** If the transaction has previously blocked, set itself active
        and continue running. */
	//##ModelId=3C4C18840311
	inline void Unblock();

	// Returns a RHTTPTransaction for this CTransaction
	//##ModelId=3C4C18840310
	inline RHTTPTransaction Handle();

	//##ModelId=3C4C1884030F
	inline TBool SyncMode() const;

	/** Get the Server Certificate for this transaction.
	@param	aServerCert A TCertInfo which will be filled with the certificate information
	@return	An error code.  KErrNone if aServerCert has been completed, otherwise one of 
			the system wide error codes
	*/
	TInt ServerCert(TCertInfo& aServerCert);
 	
	/** Get the Server Certificate for this transaction.
	@return	The certificate information or NULL if it is not available
	*/
	const CCertificate*  ServerCert();
	
	/** Get the cipher suite for this transaction.
	*/
	RString CipherSuite();
	
	inline TBool IsValid();

	void SetupHttpDataOptimiser (MHttpDataOptimiser& aHttpOptimiser);

	MHttpDataOptimiser* HttpDataOptimiser ();

 private: 

	// Active object RunL; executes 1 filter
	//##ModelId=3C4C18840306
	virtual void RunL();

	// From CActive. Does nothing
	//##ModelId=3C4C18840305
	virtual void DoCancel();

	// From CActive. Offers the error to the current filter.
	//##ModelId=3C4C188402F1
	virtual TInt RunError(TInt aError);

#if defined (_DEBUG)

	void LogEvent(TInt aTransactionId, const THTTPEvent& aEvent, 
							  TInt aDirection, const TDesC8& aFilterName);

#endif



 private:
	// States the transaction can be in
	enum TTransactionStates 
		{
		EPassive = 0,//< No events are occuring
		EFilter,//< There is a 'next filter' to run.
		EInFilter,//< In filter code
		/** The transaction has been closed from within a filter's
			MHFRunL (detectable as the state is either EInFilter or
			ECancelled at the time of the close) and so is 'zombified'
			until the filter returns from its MHFRunL, at which point
			the transaction will delete itself.  */
		EExiting,
		/** The transaction's RunL has fired from within an active
            scheduler started from within a filter on another
            transaction. This transaction must be rescheduled once the
            other transaction has finished. */
		EBlocked,
		ECancelled,//< The transaction has been cancelled from within a filter
		ESuspended 
		};

	// Constructor
	//##ModelId=3C4C188402D3
	CTransaction(MHTTPTransactionCallback& aCallback, CHTTPSession& aSession);

	// 2 phase construcion
	//##ModelId=3C4C188402B5
	void ConstructL(const TUriC8& aURI, RStringF aMethod);

	// Adds an event to the queue (but doesn't start it)
	//##ModelId=3C4C1884028F
	TInt AddEvent(THTTPEvent& aEvent, TInt aDirection, 
					THTTPFilterHandle aStart);

	/** Finds the next filter for the current event.
		@return EFilter if a filter was found, EPassive otherwise */
	//##ModelId=3C4C1884028E
	TTransactionStates FindFilterForThisEvent();

	/** Finds the next filter (possibly in another event).  
		@return The new state*/
	//  ##ModelId=3A914E0202C6
	//##ModelId=3C4C1884028D
	TTransactionStates FindNextFilter();

	// Sets the AO active and enters the specified state
	//##ModelId=3C4C1884027A
	void Activate(TTransactionStates aStatus);

	// Runs the filter that's waiting to run
	//##ModelId=3C4C18840279
	TBool RunOneFilterL();

	// Moves on to the next filter.
	//##ModelId=3C4C18840271
	inline void NextFilter();

	// Returns the current event
	//##ModelId=3C4C18840270
	inline const TEventRegistration& CurrentEvent() const;
	
	// Returns TRUE if there is an event
	//##ModelId=3C4C1884026F
	inline TBool EventAvailable() const;
	
	// Appends the selected event
	//##ModelId=3C4C1884025C
	TInt AppendEvent(TEventRegistration& aEvent);

	// Removes the current event
	//##ModelId=3C4C1884025B
	inline void RemoveEvent();
	
	inline CProtTransaction* ProtocolTransaction();
	inline void SetProtocolTransaction(CProtTransaction* aTrans);
 private:
	// The request
	//##ModelId=3C4C18840251
	CRequest* iRequest;
	// The response
	//##ModelId=3C4C18840235
	CResponse* iResponse;
	// The codec
	//##ModelId=3C4C18840229
	CHTTPTransactionCodec* iCodec;
	// The client
	//##ModelId=3C4C18840216
	MHTTPTransactionCallback& iCallback;
	// The session
	//##ModelId=3C4C18840202
	CHTTPSession& iSession;
	// The transaction's ID, unique within the session
	//##ModelId=3C4C188401F8
	TInt iId;
	// The filter currently running
	//##ModelId=3C4C188401E4
	TInt iCurrentFilter;

	// It will keep a copy of the event  as it is before leave caused by a syncronous event occours
	// This will be used to handle RunErrorL() with the right parameters 
	//##ModelId=3C4C188401D8
	THTTPEvent iASyncEvent;

	// It will keep a copy of the the iNextFilter variable before a leave caused by a syncronous event  occours
	// This will be used to handle RunErrorL() with the right parameters 
	//##ModelId=3C4C188401CE
	TInt iCurrentASyncFilter;

	// The next filter to be run 
	//##ModelId=3C4C188401BC
	TInt iNextFilter;

	// True when processing sync events
	//##ModelId=3C4C188401B2
	TBool iProcessingSyncEvent;

	// A slot for async events
	//##ModelId=3C4C18840194
	TEventRegistration iCurrentSyncEvent;

	//##ModelId=3C4C1884018B
	RHTTPTransactionPropertySet iPropertySet;

     MHttpDataOptimiser* iHttpDataOptimiser;
     
     CProtTransaction* iProtocolTransaction;
     TDblQueLink        iLink;
     __DECLARE_LOG
     // The event queue
     //##ModelId=3C4C188401A7
     RArray<TEventRegistration> iEventQueue;
	};

#include "CTransaction.inl"

#endif // CSESSION_H

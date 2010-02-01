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

/**
 @file
 @warning : This file contains Rose Model ID comments - please do not delete
*/

#ifndef __RHTTPTRANSACTION_H__
#define __RHTTPTRANSACTION_H__ 

// System includes
#include <http/thttpevent.h>
#include <http/rhttpresponse.h>
#include <http/rhttprequest.h>
#include <http/rhttptransactionpropertyset.h>
#include <http/thttpfilterhandle.h>
#include <http/mhttpdataoptimiser.h>

// Forward declarations
class CHTTPTransaction;
class CTransaction;
class MHTTPTransactionCallback;
class RHTTPSession;
class TCertInfo;
class CCertificate;


//##ModelId=3C4C18860091
class RHTTPTransaction 
/**
A HTTP Transaction. This encapsulates 1 HTTP request and
response. A Transaction is associated with a session, and must be
created using the session's CreateTransactionL method.
@publishedAll
@released
@see RHTTPSession
@see RHTTPSession::CreateTransactionL
*/
	{
 public:
	/** 
		Default (uninitialised) constructor
	*/
	//##ModelId=3C4C188600F5
	inline RHTTPTransaction();

	/** Submits a transaction. Once all the headers and other details
		have been set up, this call actualy causes the request to be
		made.
		@leave KErrNoMemory There was not enough memory.
	*/
	//##ModelId=3C4C188600ED
	IMPORT_C void SubmitL(THTTPFilterHandle aStart = 
						  THTTPFilterHandle::EClient);
	
	IMPORT_C TInt Submit(THTTPFilterHandle aStart = THTTPFilterHandle::EClient);
	/** Notify HTTP of the availability of more request body data,
		when submitting body data in several parts.
		
		@param aStart The filter supplying the new data. This will almost always be the client (default value)
		@leave KErrNoMemory There was not enough memory.
	*/
	//##ModelId=3C4C188600EB
	IMPORT_C void NotifyNewRequestBodyPartL(THTTPFilterHandle aStart = 
											 THTTPFilterHandle::EClient);

	/** Sends a status message to all relevant filters. This function
		is predominantly used by filters, rather than by the client.
		@param aStatus The status message to send.  
		@param aDirection The direction down the filter queue that this event 
		will be propogated.
		@leave KErrNoMemory There was not enough memory.
	*/
	//##ModelId=3C4C188600E2
	IMPORT_C void SendEventL(THTTPEvent aStatus, 
							 THTTPEvent::TDirection aDirection, 
							 THTTPFilterHandle aStart);
	
	IMPORT_C TInt SendEvent(THTTPEvent aStatus, THTTPEvent::TDirection aDirection, THTTPFilterHandle aStart);
	/** Gets the response. Note that the returned response may not be
		valid if it hasn't been created yet.
		@see RHTTPMessage::IsValid()
	*/
//##ModelId=3C4C188600E1
	IMPORT_C RHTTPResponse Response() const;

	// Gets the request.
	//##ModelId=3C4C188600DA
	IMPORT_C RHTTPRequest Request() const;

	// Returns the session associated with the transaction.
	//##ModelId=3C4C188600D9
	IMPORT_C RHTTPSession Session() const;

	/** Returns the transaction's property set. This is used by filters
		to store transaction-specific information, and by clients to
		specify things like reload or no cache behaviour.
	*/
	//##ModelId=3C4C188600D8
	IMPORT_C RHTTPTransactionPropertySet PropertySet() const;

	/** Cancels the transaction.
	
		@param aStart The entity that is initiating the cancel (defaults
		to the client). See THTTPFilterHandle for the values this can take.
	*/
	//##ModelId=3C4C188600CF
	IMPORT_C void Cancel(THTTPFilterHandle aStart = 
						 THTTPFilterHandle::EClient);

	/** Closes the transaction and frees all owned resources.
		Transactions must be opened using RHTTPSession::OpenTransactionL.
		It is also closed when you close the session.
	 */
	//##ModelId=3C4C188600CE
	IMPORT_C void Close();

	/** This function should be called by filters when they have
	 failed due to running out of memory. It cancels the transaction
	 and sends the synchronous events EUnrecoverableError and EFailed
	 to the client to inform it of the failure. For instance in a
	 filter that attempts to send an event to the client from a
	 MHFRunError to inform the client of a failure, if the call to
	 SendEventL leaves, Fail() may be used to 'give up'. */
	//##ModelId=3C4C188600C7
	IMPORT_C void Fail(THTTPFilterHandle aStart = 
					   THTTPFilterHandle::ECurrentFilter);

	/** Equality operator to check if this transaction is the same as that one.
		@param aTrans The transaction to compare this one to.
	*/
	//##ModelId=3C4C188600C5
	TBool operator==(RHTTPTransaction aTrans) const;
	/** Inequality operator
		@param aTrans The transaction to compare this one to.
	*/
	//##ModelId=3C4C188600C3
	TBool operator!=(RHTTPTransaction aTrans) const;
	/** Obtain this transaction's ID, which is unique within its
		session.  This is mostly used for producing a
		slightly-meaningful way of identifying transactions in logging
		code.
		@return The transaction ID.  
	*/
	//##ModelId=3C4C188600BB
	IMPORT_C TInt Id() const;

	/** Obtain the server certificate information for this transaction.  This function
		should only be used for text-mode, for WSP use RHttpSession::ServerCert.
		@see RHttpSession::ServerCert
		@param	aServerCert A client supplied object into which the certificate
		information will be placed.
		@return KErrNone if certificate has been completed, KErrNotSupported if
		this function is called for WSP.
		@deprecated v9.2 onwards - maintained for compatibility with v9.1 and before
					TCertInfo has been deprecated since v9.2. CCertificate may be used as an alternative.
	*/
	//##ModelId=3C4C188600B9
	IMPORT_C TInt ServerCert(TCertInfo& aServerCert);

#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
	/** Obtain the server certificate information for this transaction.  This function
		should only be used for HTTP. WSP should use RHttpSession::ServerCert.
		@see RHttpSession::ServerCert
		@prototype
		@return	a CCertificate pointer to an CX509Certificate object.
		Calling code can safely cast to CX509Certificate if using  "HTTP/TCP".
		NULL returned if certificate information not found.
	*/
#else
	/** Obtain the server certificate information for this transaction.  This function
		should only be used for HTTP. WSP should use RHttpSession::ServerCert.
		@see RHttpSession::ServerCert
		@internalAll
		@prototype
		@return	a CCertificate pointer to an CX509Certificate object.
		Calling code can safely cast to CX509Certificate if using  "HTTP/TCP".
		NULL returned if certificate information not found.
	*/
#endif	//SYMBIAN_ENABLE_SPLIT_HEADERS
	IMPORT_C const CCertificate* ServerCert();


	/** Obtain the cipher suite information for this transaction.
		@return RString containing the cipher suite as per RFC2246.
	*/
	IMPORT_C RString CipherSuite();

#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS		
	/**Sets the HTTP data optimiser for the transaction.
	@param aHttpOptimiser An object of the implementation of interface, MHttpDataOptimiser, supplied by the client.
	*/
#else
	/**Sets the HTTP data optimiser for the transaction.
	@param aHttpOptimiser An object of the implementation of interface, MHttpDataOptimiser, supplied by the client.
	@publishedPartner
	@released
	*/
#endif	//SYMBIAN_ENABLE_SPLIT_HEADERS
 	IMPORT_C void SetupHttpDataOptimiser (MHttpDataOptimiser& aHttpOptimiser);
 
 #ifdef SYMBIAN_ENABLE_SPLIT_HEADERS		
 	/**Returns the object of the MHttpDataOptimiser implementation class.
	*/
#else
 	/**Returns the object of the MHttpDataOptimiser implementation class.
	@internalTechnology
	*/
#endif	//SYMBIAN_ENABLE_SPLIT_HEADERS
 	IMPORT_C MHttpDataOptimiser* HttpDataOptimiser ();

private:
	friend class RHTTPSession;
	friend class CTransaction;
	friend class CProtocolHandler;
	
	inline CTransaction* Implementation();
 private:
	//##ModelId=3C4C188600A7
	CTransaction* iImplementation;
	}; 

inline TBool RHTTPTransaction::operator==(RHTTPTransaction aTrans) const
	{
	return (iImplementation == aTrans.iImplementation);
	};
inline TBool RHTTPTransaction::operator!=(RHTTPTransaction aTrans) const
	{
	return !(*this == aTrans);
	};

inline RHTTPTransaction::RHTTPTransaction()
		: iImplementation(NULL)
	{
	}

inline CTransaction* RHTTPTransaction::Implementation()
    {
    return iImplementation;
    }

#endif // __RHTTPTRANSACTION_H__

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

#ifndef __RHTTPSESSION_H__
#define __RHTTPSESSION_H__ 

// System includes
#include <e32std.h>
#include <uri8.h>
#include <http/rhttpconnectioninfo.h>
#include <http/rhttpfiltercollection.h>
#include <http/rhttptransaction.h>
#include <http/mhttpdataoptimiser.h>

// Forward declarations
class MHTTPTransactionCallback;
class CHTTPSession;
class MHTTPSessionEventCallback;
class MHTTPFilterCreationCallback;
class TCertInfo;
class CCertificate;

/**
Constant to represent any HTTP Status code when registering filters.
@publishedAll
@released
*/
const TInt KAnyStatusCode = -1;


//##ModelId=3C4C187B0280
class RHTTPSession 
/** 
A session handle. A session is a set of HTTP transactions using
the same connection settings (e.g. proxy) and the same set of filters.
    Multi-homing support provided by HTTP framework enables clients to
    specify Connection preferences through the session property 
    EHttpSocketConnection.

Note that RHTTPSession (and consequently the whole of HTTP)
depends on the active scheduler; a scheduler must be installed
when the session is opened and it must be running if a transaction
is actually to do anything.
@publishedAll
@released
@see RHTTPTransaction
*/
	{
 public:
	/** Constructor.
		@post The session is closed. (It must be opened with OpenL)
		@see OpenL */
	//##ModelId=3C4C187B02EF
	inline RHTTPSession();

	/** Sets the session event callback.
		Until it is set all incoming session events are consumed
	*/
	//##ModelId=3C4C187B02EA
	IMPORT_C void SetSessionEventCallback(MHTTPSessionEventCallback* aSessionEventCallback);

	/** Send an session event
		@param aEvent. The session event to sent.
	*/
	//##ModelId=3C4C187B02E6
	IMPORT_C void SendSessionEventL(THTTPSessionEvent aStatus, THTTPSessionEvent::TDirection aDirection, 
							 THTTPFilterHandle aStart = THTTPFilterHandle::EClient);

	/** Fail the session event
	*/
	IMPORT_C void FailSessionEvent(THTTPFilterHandle aStart = THTTPFilterHandle::EClient);

	//##ModelId=3A49F701013F
	/** Opens the session using the default protocol HTTP/TCP. 
		This function leaves with an apropriate code if the open failed.
		@pre The session is closed. An active scheduler is installed.
		@post The session is open.
	*/
	//##ModelId=3C4C187B02DA
 	IMPORT_C void OpenL();

	/** Opens the session using the protocol indicated by the parameter passed in.
		This function leaves with an apropriate code if the open failed.
		@param aProtocol The protocol required
		@pre The session is closed. An active scheduler is installed.
		@post The session is open.
	*/
	//##ModelId=3C4C187B02DB
	IMPORT_C void OpenL(const TDesC8& aProtocol);

	/** Opens the session using the protocol indicated by the parameter passed in.
		This method should be used if configuring the installed filters is required.
		Once construction if the session is complete the callback method passed in is
		called passing in a TFilterConfigurationIterator to allow the filters that
		are installed to be configured.
		This function leaves with an apropriate code if the open failed.
		@param aProtocol The protocol required
		@param aSessionCallback The pointer to the object providing the call back method
		@pre The session is closed. An active scheduler is installed.
		@post The session is open.
	*/
	//##ModelId=3C4C187B02DD
	IMPORT_C void OpenL(const TDesC8& aProtocol, MHTTPFilterCreationCallback* aSessionCallback);

	/**	This is a static methods that lists all the available protocols by adding the 
		prorocol descriptors to the descriptor array passed in. Any existing data in the
		descriptor array passed in is deleted.
		This function leaves if the descriptor array can not be updated with the appropriate
		array code.
		@param aProtocolArray A pointer array to heap based descriptors. The data in this
		array will be deleted and filled with descriptors containing the available protocols
		@see OpenL
	*/
	//##ModelId=3C4C187B02D4
	IMPORT_C static void ListAvailableProtocolsL(RPointerArray<HBufC8>& aProtocolArray);

	/** Creates a transaction. 
		@pre The session is open
		@param aURI The URI that the request will be sent to.
		@param aCallback A callback for all status events relating to
		this transaction.
		@param aMethod The HTTP method to use (default: GET)
		@leave KErrNoMemory There was not enough memory.
	*/
	//##ModelId=3C4C187B02D0
	IMPORT_C RHTTPTransaction OpenTransactionL(const TUriC8& aURI, 
									  MHTTPTransactionCallback& aCallback, 
									  RStringF aMethod = RStringF());

	/** Closes the session. All transactions will be stopped and closed 
		@post The session and all open transactions in it are closed.
	 */
	//##ModelId=3C4C187B02C8
	IMPORT_C void Close();


	//##ModelId=3C4C187B02C7
	IMPORT_C  static const TStringTable& GetTable();

	/** Accessor for the HTTP string pool. 
		@return A handle for the string pool used in HTTP.
	 */
	//##ModelId=3C4C187B02C6
	IMPORT_C RStringPool StringPool() const;

	/** Accessor for the connection info for this session, and which
		is used for all transactions in the session. Note that by and
		large, the connection info should be set up before the first
		transaction is created.  @return A handle to the connection
		inf.  */
	//##ModelId=3C4C187B02BE
	IMPORT_C RHTTPConnectionInfo ConnectionInfo() const;

	/** Accessor for the filter collection. Note that the filter
        collection can't be edited after the first transaction has
        been created. */
	//##ModelId=3C4C187B02BD
	inline RHTTPFilterCollection FilterCollection() const;


	/** Accessor for the session headers. These are headers that will get added to each request
		They only become part of the transaction headers when a transaction is submitted and they will not 
		replace headers that already exist in the transaction request 
	*/
	//##ModelId=3C4C187B02BC
	IMPORT_C RHTTPHeaders RequestSessionHeadersL();


	/** Accessor for the response session headers. These are headers that will get added to each response if the 
	corresponding header doesn't already exist
	*/
	//##ModelId=3C4C187B02B6
	IMPORT_C RHTTPHeaders ResponseSessionHeadersL();

	/** Equality operator.
		@param aTrans The session to compare this one to.
	*/
	//##ModelId=3C4C187B02B4
	TBool operator==(RHTTPSession aTrans) const;
	/** Inequality operator
		@param aTrans The session to compare this one to.
	*/

	//##ModelId=3C4C187B02B2
	TBool operator!=(RHTTPSession aTrans) const;

	/** Obtain the server certificate information for this session.  This function
		should only be used for WSP, for text-mode use RHttpTransaction::ServerCert.
		@see RHttpTransaction::ServerCert
		@param	aServerCert A client supplied object into which the certificate
		information will be placed.
		@return KErrNone if certificate has been completed, KErrNotSupported if
		this function is called for text-mode.
		@deprecated v9.2 onwards - maintained for compatibility with v9.1 and before
					TCertInfo has been deprecated since v9.2. CCertificate may be used as an alternative.
	*/
	//##ModelId=3C4C187B02AA
	IMPORT_C TInt ServerCert(TCertInfo& aServerCert);

	#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
	/** Obtain the server certificate information for this session.  This function
		should only be used for WSP. HTTP should use RHttpTransaction::ServerCert.
		@see RHttpSession::ServerCert
		@prototype
		@return	a CCertificate pointer to an CWTLSCertificate object.
		Calling code can safely cast to CWTLSCertificate if using  "WSP/WSP".
		NULL returned if certificate information not found.
	*/
	#else
	/** Obtain the server certificate information for this session.  This function
		should only be used for WSP. HTTP should use RHttpTransaction::ServerCert.
		@see RHttpSession::ServerCert
		@internalAll
		@prototype
		@return	a CCertificate pointer to an CWTLSCertificate object.
		Calling code can safely cast to CWTLSCertificate if using  "WSP/WSP".
		NULL returned if certificate information not found.
	*/
	#endif	//SYMBIAN_ENABLE_SPLIT_HEADERS
	IMPORT_C const CCertificate* ServerCert();

	/** Connect this WSP session. This function does nothing when called for text-mode.
	*/
	//##ModelId=3C4C187B02A9
	IMPORT_C void ConnectL();

	/** Disconnect this WSP session. This function does nothing when called for text-mode.
	*/
	//##ModelId=3C4C187B02A8
	IMPORT_C void DisconnectL();

	#ifndef SYMBIAN_ENABLE_SPLIT_HEADERS
	/**Set the default Proxy for Http Session. This function reads default proxy information,if available, from CommsDat
	and uses it for the current Session.
	@internalAll
	*/
	IMPORT_C void SetupDefaultProxyFromCommsDatL();

	#endif	//SYMBIAN_ENABLE_SPLIT_HEADERS

	#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS	
	/**Sets the HTTP data optimiser for the session.
	@param aHttpOptimiser An object of the implementation of interface, MHttpDataOptimiser, supplied by the client.
	*/
	#else
	/**Sets the HTTP data optimiser for the session.
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
	friend class RHTTPFilterCollection;
	friend class CHTTPSession;

	//##ModelId=3C4C187B0296
	CHTTPSession* iImplementation;
};

inline RHTTPSession::RHTTPSession()
		: iImplementation(0)
	{
	}


inline RHTTPFilterCollection RHTTPSession::FilterCollection() const
	{
	RHTTPFilterCollection c;
	c.iImplementation = iImplementation;
	return c;
	}

inline TBool RHTTPSession::operator==(RHTTPSession aTrans) const
	{
	return (iImplementation == aTrans.iImplementation);
	};
inline TBool RHTTPSession::operator!=(RHTTPSession aTrans) const
	{
	return !(*this == aTrans);
	};



inline RHTTPSession RHTTPFilterCollection::Session() const
	{
	RHTTPSession s;
	s.iImplementation = iImplementation;
	return s;
	}




#endif // __RHTTPSESSION_H__

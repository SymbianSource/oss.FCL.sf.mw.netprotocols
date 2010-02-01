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

#ifndef	__THTTPEVENT_H__
#define	__THTTPEVENT_H__

// System includes
#include <e32std.h>

/**
The HTTP UID. This UID is used for all events defined here.
@publishedAll
@released
*/
const TUint KHTTPUid				= 0x1000A441;

/**
Wildcard Matching UID. When specified as part of an event then the UID event
part of the match will be ignored.
@publishedAll
@released
*/
const TUint KHTTPMatchAnyEventUid	=  0x1000A44C;

/**
The base status code for transaction events. Any number above this but below
KSessionEventBaseStatus is a transaction event.
@publishedAll
@released
*/
const TInt KTransactionEventBaseStatus		= 0;

/**
The base status code for transaction warning events.
@publishedAll
@released
*/
const TInt KTransactionWarningBaseStatus	= 10000;

/**
The base status code for session events. Any number above this is a session 
event.
@publishedAll
@released
*/
const TInt KSessionEventBaseStatus			= 100000;

/**
The base status code for session warning events.
@publishedAll
@released
*/
const TInt KSessionWarningBaseStatus		= 110000;


//##ModelId=3C4C18740294
class THTTPEvent
/**
A HTTP status message. Status messages consist of a UID and a
status code within that UID. Extension dlls that needs to create
new status messages should use their own UID and create status codes
within that UID.
@publishedAll
@released
*/
	{
public:	

	/** The TStandardEvent type is used to specify a family of event types. Any
		negative event number is used to convey error codes. All events are 
		incoming (originate with the origin server) unless otherwise indicated. 
		Outgoing messages will not be seen by the MHTTPTransactionCallback's 
		MHFRunL.
	 */
	enum TStandardEvent
		{
		/** Used when registering filter to indicate the filter is instrested 
			in ALL events, both transaction and session events.
		 */
		EAll					= KRequestPending + 1,
		/** Used when registering filters to indicate the filter is interested
			in any transaction event, from any direction.
		 */
		EAnyTransactionEvent	= KRequestPending,
		/** Used when registering filters to indicate the filter is interested 
			in any session event.
		 */
		EAnySessionEvent		= KSessionEventBaseStatus
		};

	/** The TTransactionEvents type defines the events that correspond to 
		transactions. Outgoing events originate from the client or from filters.
		The clients do not send these explicitly since the API methods of 
		RHTTPTransaction do it on their behalf. Incoming events originate from 
		the protocol handler or from filters, and clients should handle these.
		The ESucceeded and EFailed events are mutually exclusive, but one will
		always be sent to the client as the final event for a transaction.
	 */
	enum TTransactionEvent
		{
		/** The transaction is being submitted. An outgoing event.
		*/
		ESubmit						= KTransactionEventBaseStatus,
		/** The transaction is being cancelled. An outgoing event. 
		*/
		ECancel						= KTransactionEventBaseStatus + 1,
		/** A new part of request body data is available for transmission. An
			outgoing event
		*/
		ENotifyNewRequestBodyPart	= KTransactionEventBaseStatus + 2,
		/** The transaction is being closed. An outgoing event. 
		*/
		EClosed						= KTransactionEventBaseStatus + 3,
		/** All the response headers have been received. An incoming event. 
		*/
		EGotResponseHeaders			= KTransactionEventBaseStatus + 4, 
		/** Some (more) body data has been received (in the HTTP response). An 
			incoming event.
		*/
		EGotResponseBodyData		= KTransactionEventBaseStatus + 5,
		/** The transaction's response is complete. An incoming event. 
		*/
		EResponseComplete			= KTransactionEventBaseStatus + 6,
		/** Some trailer headers have been received. An incoming event.
		*/
		EGotResponseTrailerHeaders	= KTransactionEventBaseStatus + 7,
		/** The transaction has succeeded. An incoming event.
		*/
		ESucceeded					= KTransactionEventBaseStatus + 8,
		/** The transaction has failed. This is a 'catch-all' for communicating
			failures, and more details of the failure should have been notified
			in previous messages or status codes. If the client could process 
			these then it is possible that it should not consider the transaction
			a failure. For instance a browser that displays the body of 404 
			responses to the user would not consider a 404 response to be a 
			failure, as it would display it just like a 200 response even though
			a 404 will be flagged as a 'failure'. An incoming event.
		*/
		EFailed						= KTransactionEventBaseStatus + 9, 
		/** Generated from RHTTPTransaction::Fail(). A filter has failed in a 
			way that prevents it from using the normal event mechanism to inform
			the client of the error. This probably means it's run out of memory.
			An incoming event.
		*/
		EUnrecoverableError			= KTransactionEventBaseStatus + 10,
		/** An event that indicates that there is too much request data to be 
			sent. The transaction will subsequently be cancelled. An incoming 
			event.
		*/
		ETooMuchRequestData			= KTransactionEventBaseStatus + 11,

		
		/** If the returned status code for a transaciton is either 301, 302 or 307
            and the requested method is NOT a GET or HEAD, then the filter sends the 
            client an event ERedirectRequiresConfirmation as stated in RFC2616.

            On receiving this event, the transaction is already setup with the redirected URI
            and the client is required to make the decision to whether to submit the 
            transaction or close the transaction.

            If the requested method is GET or HEAD the transaction is automatically 
            redirected and this event is not used.
        */
		ERedirectRequiresConfirmation = KTransactionEventBaseStatus + 12,

		/** A transaction has been specified to use a proxy and the request
			requires a tunnel to be establised to the origin server.
		*/
		ENeedTunnel					= KTransactionEventBaseStatus + 13,
		
		/** The client wishes to view the current cipher suite.
		*/
		EGetCipherSuite				= KTransactionEventBaseStatus + 14,
		
		/** The transaction's request is complete. An incoming event. 
		*/
		ERequestComplete			= KTransactionEventBaseStatus + 15,
		
		/**An event to signal that 100 Continue response has been received.
		*/
		EReceived100Continue		= KTransactionEventBaseStatus + 16,
		
		/**An event to cancel the wait for a 100-Continue event.
		*/
		ECancelWaitFor100Continue	= KTransactionEventBaseStatus + 17,
		
		/**An event of Send Timeout for a Request.
		*/
		ESendTimeOut	= KTransactionEventBaseStatus + 18,

		/**An event of Receive Timeout for a Response.
		*/
		EReceiveTimeOut	= KTransactionEventBaseStatus + 19,
		
		/**An event that indicate that transaction Suspended.
		*/
		ESuspend = KTransactionEventBaseStatus +20,
		
		/**An event that indicate that transaction Resume.
		*/
		EResume = KTransactionEventBaseStatus +21

		};

	/** The TDirection type defines the direction of an event. An outgoing event
		originates from the client or from filters. The clients do not send these
		explicitly since the API methods of RHTTPTransaction or RHTTPSession do 
		it on their behalf. Incoming events originate from the protocol handler 
		or from filters, and clients should handle these.
	*/
	enum TDirection
		{
		/** An event originating with the client (e.g. start transaction).
		*/
		EOutgoing,
		/** An event originating with the server (e.g. something received). 
		*/
		EIncoming
		};

	/** The TTransactionWarning type indicates that something in a transaction 
		may be incorrect but the transaction may continue. It may also indicate 
		that something (e.g. a filter) may have performed an action that has 
		changed the transaction and that the client should be informed of this.
	*/
	enum TTransactionWarning
		{
		/** An event indicating that the transaction has been redirected and the
			original origin server indicated that it was a permanent redirection.
			The URI for the transaction is now the redirected location. A 
			permanent redirection may require further client behavior if the 
			request came from a stored URI. This is to avoid further redirections 
			on subsequent requests for this resource.
		*/
		ERedirectedPermanently			= KTransactionWarningBaseStatus,
		/** An event indicating that the transaction has been redirected and the
			original server indicated that it was a temporary redirection. 
		*/
		ERedirectedTemporarily			= KTransactionWarningBaseStatus + 1,
		/** An event generated by the Protocol Handler when it receives a Content-
			Length value that does not match the actual length of the body data.
		*/
		EMoreDataReceivedThanExpected	= KTransactionWarningBaseStatus + 2
		};

public:	// Methods

	/** Constructor
		@param aStatus	The status value.
		@param aUID		The UID.
	*/
	//##ModelId=3C4C187402FB
	inline THTTPEvent(TInt aStatus, TUint aUID = KHTTPUid);

	/** Constructor (using a standard event and the HTTP UID)
		@param aStatus	The standard event to use.
	*/
	//##ModelId=3C4C18740304
	inline THTTPEvent(TStandardEvent aStatus = EAnyTransactionEvent);

	/** Constructor (using a transaction event and the HTTP UID)
		@param aStatus	The transaction event to use.
	*/
	//##ModelId=3C4C1874030C
	inline THTTPEvent(TTransactionEvent aStatus);

	/** Assigns a standard event code to an event object
		@param aStatus	The standard event.
		@return The HTTP event object.
	*/
	//##ModelId=3C4C187402EF
	inline THTTPEvent& operator=(TStandardEvent aStatus);

	/** Assigns a transaction event code to an event object
		@param aStatus	The transaction event.
		@return The HTTP event object.
	*/
	//##ModelId=3C4C187402F1
	inline THTTPEvent& operator=(TTransactionEvent aStatus);

	/** Equality operator
		@param The HTTP event object to compare.
		@return ETrue if the HTTP event objects are equal. 
	*/
	//##ModelId=3C4C187402DA
	inline TBool operator==(THTTPEvent aThat) const;

	/** Inequality operator
		@param The HTTP event object to compare.
		@return ETrue if the HTTP event objects are not equal. 
	*/
	//##ModelId=3C4C187402BD
	inline TBool operator!=(THTTPEvent aThat) const;

	/** Equality operator (compares with a standard event)
		@param The standard event object to compare.
		@return ETrue if the standard event objects are equal.
	*/
	//##ModelId=3C4C187402DC
	inline TBool operator==(TStandardEvent aStatus) const;

	/** Inequality operator (compares with a standard event)
		@param The standard event object to compare.
		@return ETrue if the standard event objects are not equal. 
	*/
	//##ModelId=3C4C187402C7
	inline TBool operator!=(TStandardEvent aStatus) const;

	/** Equality operator (compares with a transaction event)
		@param The transaction event object to compare.
		@return ETrue if the transaction event objects are equal. 
	*/
	//##ModelId=3C4C187402E4
	inline TBool operator==(TTransactionEvent aStatus) const;

	/** Inequality operator (compares with a transaction event)
		@param The transaction event object to compare.
		@return ETrue if the transaction event objects are not equal. 
	*/
	//##ModelId=3C4C187402D0
	inline TBool operator!=(TTransactionEvent aStatus) const;
	
	/** @return ETrue if the event is a session event
	*/
	//##ModelId=3C4C187402BC
	inline TBool IsSessionEvent() const;

public:	// Attributes

	/** The status value.
	 */
	//##ModelId=3C4C187402B4
	TInt iStatus;

	/** The UID.
	 */
	//##ModelId=3C4C187402AA
	TUint iUID;

protected: // Attributes
	/** Flag to indicate whether the event is a session event
	*/
	TBool iIsSessionEventFlag;

	};


class THTTPSessionEvent : public THTTPEvent
/**
A HTTP session status message. Status messages consist of a UID and a
status code within that UID. Extension dlls that needs to create
new status messages should use their own UID and create status codes
within that UID.
@publishedAll
@released
*/
	{
public: // Enumerations
	/** The TSessionEvents type defines the evenst that correspond to the 
		of a session entity. Outgoing events originate from the client or from
		filters. Incoming events originate from the protocol handler or from 
		filters, and clients should handle these. 
	*/
	enum TSessionEvent
		{
		/** A session connection should be initiated. An outgoing event.
		*/
		EConnect							= KSessionEventBaseStatus,
		/** The session should be disconnected. An outgoing event.
		 */
		EDisconnect							= KSessionEventBaseStatus + 1,
		/** The session has been successfully connected. None of the client 
			requested capabilities were denied or reduced by the proxy. An 
			incoming event.
		*/
		EConnectedOK						= KSessionEventBaseStatus + 2,
		/** The session has been connected, but with one or more of the client 
			requested capabilities denied or reduced by the proxy. An incoming 
			event.
		*/
		EConnectedWithReducedCapabilities	= KSessionEventBaseStatus + 3,
		/** The session has been disconnected. This either confirms an earlier 
			EDisconnect event or indicates a forced disconnection by the proxy. 
			An incoming event.
		*/
		EDisconnected						= KSessionEventBaseStatus + 4,
		/** The authentication handshake succeeded with the automatic validation
			of the (proxy) server certificate.
		 */
		EAuthenticatedOK					= KSessionEventBaseStatus + 5,
		/** The authentication handshake failed.
		*/
		EAuthenticationFailure				= KSessionEventBaseStatus + 6,
		/** The connection attempt to the proxy timed out.
		*/
		EConnectionTimedOut					= KSessionEventBaseStatus + 7
		};
	
	/**
		HTTP session warning events.
	 */
	enum TSessionWarning
		{
		/** The client has requested a transaction event that requires a session
			to be connected or the connection to be initiated, but neither is
			currently true. The transaction event will be left pending until the
			session is connected. An incoming event.
		*/
		ENotConnected						= KSessionWarningBaseStatus,
		/** The proxy has sent some information that is not related to a 
			transaction and has no effect on the state of the session. The 
			information from the proxy is in the EProxyExceptionInfo property.
		*/
		EExceptionInfo						= KSessionWarningBaseStatus + 1,
		/** The client connection request was (permanently) redirected to a new
			WAP proxy address. The client should check the EWspProxyAddress 
			property for the new address. The client's access-point database can
			then be updated with this address. No notification is given of a 
			temporary redirection.
		*/
		ERedirected							= KSessionWarningBaseStatus + 2,
		/** The client has requested a session event that is not valid whilst
			the WSP session is trying to establish a connection.
		*/
		EAlreadyConnecting					= KSessionWarningBaseStatus + 3,
		/** The client has requested a session event that is not valid whilst
			the WSP session is in the Connected state.
		*/
		EAlreadyConnected					= KSessionWarningBaseStatus + 4,
		/** The client has requested a session event that is not valid whilst
			the WSP session is trying to close the connection.
		*/
		EAlreadyDisconnecting				= KSessionWarningBaseStatus + 5,
		/** The client has requested a session event that is not valid whilst
			the WSP session is in the Null (or disconnected) state.
		*/
		EAlreadyDisconnected				= KSessionWarningBaseStatus + 6
		};

public:
	/** Constructor
		@param aStatus	The status value.
		@param aUID		The UID.
	*/
	inline THTTPSessionEvent(TInt aStatus, TUint aUID = KHTTPUid);

	/** Constructor (using a standard event and the HTTP UID)
		@param aStatus	The standard event to use.
	*/
	inline THTTPSessionEvent(TStandardEvent aStatus = EAnySessionEvent);

	/** Constructor (using a session event and the HTTP UID)
		@param aStatus	The session event to use.
	*/
	inline THTTPSessionEvent(TSessionEvent aStatus);

	/** Assigns a session event code to an event object
		@param aStatus	The session event.
	*/
	//##ModelId=3C4C187402F9
	inline THTTPSessionEvent& operator=(TSessionEvent aStatus);

	// Equality operator (compares with a session event)
	//##ModelId=3C4C187402E6
	inline TBool operator==(TSessionEvent aStatus) const;

	// Inequality operator (compares with a session event)
	//##ModelId=3C4C187402D2
	inline TBool operator!=(TSessionEvent aStatus) const;
	};

#include <http/thttpevent.inl>

#endif // __THTTPEVENT_H__

// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "chttpconnectionmanager.h"

#include <authority8.h>
#include <httperr.h>
#include <x509cert.h>
#include <sslerr.h>

#include "msocketfactory.h"
#include "msocketconnector.h"
#include "moutputstream.h"
#include "minputstream.h"
#include "mhttprequest.h"
#include "mhttpresponse.h"
#include "chttpconnectioninfo.h"
#include "chttprequestbatcher.h"


CHttpConnectionManager* CHttpConnectionManager::NewL(MSocketFactory& aSocketFactory, 
													 MHttpBatchingPropertiesCallback& aCallback,
													 CHttpPipelineFallback& aPipelineFallback, 
													 TInt aMaxTransactionsToPipeline,
													 TBool aEnableOptimalPipeline)
	{
	CHttpConnectionManager* self = new (ELeave) CHttpConnectionManager(aSocketFactory, aCallback, aPipelineFallback, aMaxTransactionsToPipeline, aEnableOptimalPipeline);
	return self;
	}
	
CHttpConnectionManager::~CHttpConnectionManager()
	{
	iPendingRequests.Reset();
	iPendingResponses.Reset();
	
	// Close down connect, if exists
	if( iSocketConnector )
		{
		iSocketConnector->StopConnect();
		}
	CloseConnection();
		
	delete iConnectionInfo;
	iTunnelHost.Close();

	delete iRequestBatcher;
	delete iOptimiser;
	}
	
CHttpConnectionManager::CHttpConnectionManager(MSocketFactory& aSocketFactory, 
											   MHttpBatchingPropertiesCallback& aCallback,
											   CHttpPipelineFallback& aPipelineFallback,
											   TInt aMaxTransactionsToPipeline,
											   TBool aEnableOptimalPipeline): 
	iEnableOptimalPipeline(aEnableOptimalPipeline), iMaxTransactionsToPipeline(aMaxTransactionsToPipeline),
	iSocketFactory(aSocketFactory), iCallback(aCallback), iPipelineFallback(aPipelineFallback)
	{
	}
	
void CHttpConnectionManager::SubmitL(CHttpConnectionInfo& aConnectionInfo, MHttpRequest& aRequest, MHttpResponse& aResponse)
	{
	// Check state - may need to close connection
	TBool secure = EFalse;
	if( iState == EIdleConnected )
		{
		secure = iConnectionInfo->IsSecure();
		// Can the current connection be re-used?
		if( !iConnectionInfo->HostAndPortMatches(aConnectionInfo) ||
			secure && !aConnectionInfo.IsSecure() )
			{
			__ASSERT_DEBUG( iPendingResponses.Count() == 0, User::Invariant() );

			// Nope - either the current connection is to the wrong host or the
			// current connection is secure and need a non-secure connection.
			CloseConnection();
			}
		}
	else if( iState == EClosing )
		{
		__ASSERT_DEBUG( iConnectionInfo->IsNonPersistent(), User::Invariant() );
		__ASSERT_DEBUG( iPendingResponses.Count() == 0, User::Invariant() );

		// The connection was non-persistent and the remote host has yet to 
		// close the connection - close it now.
		CloseConnection();
		}
	if ( iState == EIdleConnected || iState == EConnected )	
		{
		__ASSERT_DEBUG( iInputStream, User::Invariant() );
		iInputStream->Restart ();			
		}

	// Cleanup old connection info and take ownership of the new connection info.
	delete iConnectionInfo;
	iConnectionInfo = &aConnectionInfo;

	// Store the request and response in appropriate place
	if( iCurrentRequest == NULL )
		{
		// Make this the current request
		iCurrentRequest = &aRequest;
		}
	else
		{
		__ASSERT_DEBUG( !CannotPipeline(), User::Invariant() );

		// Append to the pending request queue
		User::LeaveIfError(iPendingRequests.Append(&aRequest));
		}

	if( iCurrentResponse == NULL )
		{
		// Make this the current response
		iCurrentResponse = &aResponse;
		}
	else
		{
		__ASSERT_DEBUG( !CannotPipeline(), User::Invariant() );

		// Append to the pending response queue		
		TInt err = iPendingResponses.Append( &aResponse );
		
		// The request is already added into the array. Remove it from the array
		// in case of any error and leave with the error code. 
		// This is required because a checking is going on in CancelSubmission fn.
		// That function expects: if a request object is present in the array there "must" be a 
		// corresponding response object.
		if ( err != KErrNone )
			{
			TInt requestIndex = FindRequest( aRequest );
			if( requestIndex != KErrNotFound )
				{
				iPendingRequests.Remove( requestIndex );
				}
			User::Leave ( err );
			}

		}
	
	switch( iState )
		{
	case EIdle:
		{
		__FLOG_4(
				_T8("Start connection to host %S, remote port %d (secure : %d, nonpersistent : %d)"), 
				&iConnectionInfo->Host(), 
				iConnectionInfo->Port(), 
				iConnectionInfo->IsSecure(), 
				iConnectionInfo->IsNonPersistent()
				);

		// Need to start connection to the appropriate host
		iSocketConnector = &iSocketFactory.ConnectL(
												   *this, 
												   iConnectionInfo->Host(), 
												   iConnectionInfo->Port()
												   );

		if (!CannotPipeline())
			{
			//  we are pipelining so set this flag
			iFlags.Set(EFirstTransaction);
			}
		// Move to Connecting state
		iState = EConnecting;
		} break;
	case EIdleConnected:
		{
		// A connection is already established with the appropriate host and it
		// is currently not being used by any other transaction. Check to see if
		// an upgrade to a secure connection is required.
		if( iConnectionInfo->IsSecure() && !secure )
			{
			// Upgrade the connection to be secure.
			UpgradeConnectionL();
			}
		else 
			{
			// Move to Connected state and notify the current request to start.
			// Specifies that this transaction is waiting to write its
			// request to the supposedly connected connection. Due timing issues
			// the server may have closed connection but connection manager has
			// not been notified yet.
			iState = EConnected;
			iFlags.Set(EPendingWriteInConnectedState);
			iCurrentRequest->StartRequest();
			}
		} break;
	case EConnecting:
		{
		__ASSERT_DEBUG( !CannotPipeline(), User::Invariant() );

		// Can only submit in this state is a transaction is being pipelined and 
		// pipelining is enabled. A connection is being established with the 
		// appropriate host for an earlier request. Do nothing.
		} break;
	case EConnected:
		{
		__ASSERT_DEBUG( !CannotPipeline(), User::Invariant() );

		// Can only submit in this state is a transaction is being pipelined and 
		// pipelining is enabled. A connection has already been establised with 
		// the appropriate host but is currently being used by other transactions.
		// Check to see if this request is now the current request.
		if( iCurrentRequest == &aRequest )
			{
			// It is - start the request...
			iCurrentRequest->StartRequest();
			}			
		} break;
	case EUpgrading:
		{
		__ASSERT_DEBUG( !CannotPipeline(), User::Invariant() );

		// Can only submit in this state is a transaction is being pipelined and 
		// pipelining is enabled. A connection has already been established with
		// the appropriate host and is currently being upgraded to be secure. Do 
		// nothing. 
		} break;
	default:
		User::Invariant();
		break;
		}
	}
	
CHttpConnectionManager::TConnectionStatus CHttpConnectionManager::Status() const
	{
	// Check the state for correct status value.
	CHttpConnectionManager::TConnectionStatus status;

	switch( iState )
		{
	case EIdle:
	case EClosing:
		{
		status = ENotConnected;
		} break;
	case EIdleConnected:
		{
		status = EConnectedAndAvailable;
		} break;
	case EConnecting:
	    {
	    if(iEnableOptimalPipeline)
	        {
	        status = EConnectingNotAvailable;
	        break;
	        }
	    }
	case EUpgrading:
	case EConnected:
	default:
		{
		// Number of requests is the number of Pending Responses + the current response
		TInt numberResponses = iPendingResponses.Count();
		if (iCurrentResponse != NULL)
			{
			numberResponses +=1;
			}
			
		if( (!CannotPipeline() && !iConnectionInfo->IsNonPersistent()) &&
			numberResponses < iMaxTransactionsToPipeline)
			{
			// The connection can be used to pipeline requests - connect and busy.
			status = EConnectedAndBusy;
			}
		else 
			{
			// Other states, connected and not available.
			status = EConnectedNotAvailable;
			}
		}
		break;
		}
	return status;
	}
	
const CHttpConnectionInfo& CHttpConnectionManager::ConnectionInfo() const
	{
	return *iConnectionInfo;
	}


void CHttpConnectionManager::CancelSubmission(MHttpRequest& aRequest, MHttpResponse& aResponse)
	{
	__FLOG_0(_T8("!! Cancel submission"));

	// If the request has not yet been sent, then just remove the request and
	// response from the pending queues. Everything can carry on as normal. But
	// the request is in the process of being sent or has already been sent and
	// is waiting for a response, then need to close the connection, notifying
	// the other responses of the connection closure.

	TInt requestIndex = FindRequest(aRequest);
	TInt responseIndex = FindResponse(aResponse);
	TBool stopConnection = EFalse;

	if( requestIndex != KErrNotFound )
		{
		__ASSERT_DEBUG( responseIndex != KErrNotFound, User::Invariant() );

		// The request has not been made - just need to remove the request and
		// response objects from the pending queues.
		iPendingRequests.Remove(requestIndex);

		if (KErrNotFound != responseIndex)
			iPendingResponses.Remove(responseIndex);
		}
	else if( responseIndex != KErrNotFound )
		{
		// The request has been sent but the response has not yet been received
		// and therefore need to stop the connection and remove response from 
		// the pending queue.
		iPendingResponses.Remove(responseIndex);
		stopConnection = ETrue;
		}

	// Is this request/response active?
	if( stopConnection || iCurrentRequest == &aRequest || iCurrentResponse == &aResponse )
		{
		// Yep - need to stop the connection. Check state as could be still
		// trying to connect.
		if( iState == EConnecting )
			{
			__FLOG_4(
					_T8("-> stopping connection to host %S, remote port %d (secure : %d, nonpersistent : %d)"), 
					&iConnectionInfo->Host(), 
					iConnectionInfo->Port(), 
					iConnectionInfo->IsSecure(), 
					iConnectionInfo->IsNonPersistent()
					);

			// Stop the connection - move to Idle state
			iSocketConnector->StopConnect();
			iSocketConnector = NULL;
			
			SetIdleState();				
			}
		else
			{
			// As the connection had been established, then the request/response 
			// could be in progress - cancel them.
		   if( iCurrentRequest && iCurrentRequest == &aRequest)
				{
				__FLOG_0(_T8("-> cancelling request"));
				iCurrentRequest->CancelRequest();
				}
				
		   if( iCurrentResponse == NULL && iPendingResponses.Count() == 0 )
				{
				iCurrentResponse = &aResponse;	
				}

		   if( iCurrentResponse && iCurrentResponse == &aResponse )
				{
			   __FLOG_0(_T8("-> completing response"));
				DoResponseCompletion ();
				__FLOG_0(_T8("-> cancelling response"));
				iCurrentResponse->CancelResponse();	
				
				// Need connection closure?
				if ( stopConnection || 
					( iCurrentResponse && iCurrentResponse == &aResponse && !iCurrentResponse->ResponseCompleted () ) )
					{
					__FLOG_4(
							_T8("-> closing connection to host %S, remote port %d (secure : %d, nonpersistent : %d)"), 
							&iConnectionInfo->Host(), 
							iConnectionInfo->Port(), 
							iConnectionInfo->IsSecure(), 
							iConnectionInfo->IsNonPersistent()
							);
					CloseConnection ();												
					}
				else
					{
					ResponseDataParsed ();						
					}
				}				
			}
			
			if (  iCurrentResponse && iState == EIdle )
				{
					if ( stopConnection )
						{
						__FLOG_0(_T8("-> Notifying current and pending transactions"));

						// The current transaction was not the one to cancel - need to
						// notify it. This request has already sent and no response has been received.
						// Need to resend the request to maintain the correct request/response state
						// which is needed by the request/response composer/parser.
						iCurrentResponse->ConnectionError(KErrHttpNonPipeliningError);
						}
						
						NotifyPendingTransactions(KErrHttpNonPipeliningError);
						
						// Current request and response are no longer valid				
						iCurrentRequest = NULL;
						iCurrentResponse = NULL;				
													
					return; 				
				}
				
			if ( iState == EIdle && iPendingRequests.Count() > 0 )
				{				
				// connection is closed explicitly by the connection manager or not active. If we are 
				// having pending requests to be sent need to reconnect here.
				__FLOG_0(_T8("-> Reconnecting and sending the first pending request."));
				// Set the current request & response
				iCurrentRequest = iPendingRequests[0];
				iCurrentResponse = iPendingResponses[0];
				// Remove it from the pending queue
				iPendingRequests.Remove (0);
				iPendingResponses.Remove (0);			
				
				// Now do a reconnect
				TRAPD ( err, ReconnectSocketL () );
				if ( err != KErrNone )
					{
					ReportSocketError ( err );
					}
				return; 
				}
			if ( iCurrentRequest && iCurrentRequest == &aRequest )					
				{
				iCurrentRequest = NULL;
				}
			if ( iCurrentResponse && iCurrentResponse == &aResponse )					
				{
				iCurrentResponse = NULL;			
				}				
		}
	else if( CannotPipeline() && !iCurrentRequest && !iCurrentResponse )
 	    {
			// Closing the connection since pipelining has been disabled and both the request and 
		    // response have been completed.
	   __FLOG_4(
			   _T8("-> closing connection to host %S, remote port %d (secure : %d, nonpersistent : %d)"), 
			   &iConnectionInfo->Host(), 
			   iConnectionInfo->Port(), 
			   iConnectionInfo->IsSecure(), 
			   iConnectionInfo->IsNonPersistent()
			   );
		 CloseConnection();
		}	
	// Otherwise - do nothing. This submission has already completed.
#if defined (_DEBUG) && defined (_LOGGING)
	__FLOG_0(_T8("-> This submission has already completed. Do nothing"));
#endif
	}

const CX509Certificate* CHttpConnectionManager::ServerCert()
	{
	__ASSERT_DEBUG( iOutputStream != NULL, User::Invariant() );

	return iOutputStream->ServerCert();
	}

TInt CHttpConnectionManager::CipherSuite(TDes8& aCipherSuite)
	{
	__ASSERT_DEBUG( iOutputStream != NULL, User::Invariant() );
	
	return iOutputStream->CipherSuite(aCipherSuite);
	}

void CHttpConnectionManager::TunnelConnection(RStringF aHost)
	{
	__ASSERT_DEBUG( iState == EConnected, User::Invariant() );

	// Copy the host information (includes port info) for where the tunnel leads.
	iTunnelHost.Close();
	iTunnelHost = aHost.Copy();
	iTunnel = ETrue;

	__FLOG_0(_T8("!! Tunnel establised"));
	__FLOG_5(
			_T8("-> tunnel to %S via host %S, remote port %d (secure : %d, nonpersistent : %d)"),
			&iTunnelHost.DesC(), 
			&iConnectionInfo->Host(), 
			iConnectionInfo->Port(), 
			iConnectionInfo->IsSecure(), 
			iConnectionInfo->IsNonPersistent()
			);
	}

TBool CHttpConnectionManager::TunnelMatches(RStringF aHost) const
	{
	return (iTunnel && iTunnelHost == aHost);
	}

void CHttpConnectionManager::CloseConnection()
	{
	if( iInputStream )
		{
		// Flag that connection
		iState = EClosing;

		// Close the input stream	
		iInputStream->Close();
		iInputStream = NULL;
		}
	// NOTE - there is no need to close the output stream as by closing the
	// input stream it would have been notified and closed anyway. Also, the
	// state should have moved to Idle.

	__ASSERT_DEBUG( iState == EIdle, User::Invariant() );
	}

void CHttpConnectionManager::UpgradeConnectionL()
	{
	__ASSERT_DEBUG( iOutputStream != NULL, User::Invariant() );

	__FLOG_0(_T8("!! Upgrading to secure"));

	// Request upgrade to a secure connection - move to Upgrading state.
	TPtrC8 host;
	if( iTunnel )
		{
		__FLOG_5(
				_T8("-> tunnel to %S via host %S, remote port %d (secure : %d, nonpersistent : %d)"), 
				&iTunnelHost.DesC(), 
				&iConnectionInfo->Host(), 
				iConnectionInfo->Port(), 
				iConnectionInfo->IsSecure(), 
				iConnectionInfo->IsNonPersistent()
				);

		// Host info is in the tunnel host name - this is an authority form data
		// and so needs to be parsed.
		TAuthorityParser8 authority;
		User::LeaveIfError(authority.Parse(iTunnelHost.DesC()));
		host.Set(authority.Extract(EAuthorityHost));
		}
	else
		{
		__FLOG_4(
				_T8("-> connection to host %S, remote port %d (secure : %d, nonpersistent : %d)"), 
				&iConnectionInfo->Host(), 
				iConnectionInfo->Port(), 
				iConnectionInfo->IsSecure(), 
				iConnectionInfo->IsNonPersistent()
				);

		// Just a normal connection - host info is in the connection info.
		host.Set(iConnectionInfo->Host());
		}
	iOutputStream->SecureClientReq(host);
	iState = EUpgrading;
	}

void CHttpConnectionManager::HandleSocketError(TInt aError)
	{
	// Is this due to the connection manager closing the connection?
	if( iState != EClosing )
		{
#if defined (_DEBUG) && defined (_LOGGING)
		if( iState == EConnecting )
			{
			__FLOG_1(_T8("!! Error : %d"), aError);
			__FLOG_4(
					_T8("-> could not connect to host %S, remote port %d (secure : %d, nonpersistent : %d)"), 
					&iConnectionInfo->Host(), 
					iConnectionInfo->Port(), 
					iConnectionInfo->IsSecure(), 
					iConnectionInfo->IsNonPersistent()
					);
			}
		else 
			{
			__FLOG_1(_T8("!! Error : %d"), aError);
			__FLOG_4(
					_T8("-> connection closed to host %S, remote port %d (secure : %d, nonpersistent : %d)"), 
					&iConnectionInfo->Host(), 
					iConnectionInfo->Port(), 
					iConnectionInfo->IsSecure(), 
					iConnectionInfo->IsNonPersistent()
					);
			}
#endif
		if ((aError == KErrSSLAlertUnexpectedMessage || aError == KErrSSLAlertHandshakeFailure) && !iSecureRetry)
 		    {
 		    //TSW error id - TKOO-7YUCA3
 		    //some websites dont support tls1.0 & retry secure negotiation with ssl3.0 & error value modified to KErrEof for retrying
		    iSecureRetry = ETrue; 
            aError = KErrEof;
 		    }
 		if (  aError == KErrEof || aError == KErrCancel  ) 
 			{
			if ( IsPendingWriteInConnectedState() && iCurrentRequest && !iCurrentRequest->NeedDisconnectNotification() )
 				{
				// Server shut down the connect before the current transaction had 
 				// a chance to send any of its data - attempt re-connect to server.	
 				// Cancel the current request
 				iCurrentRequest->CancelRequest ();		
 				TRAPD(err, ReconnectSocketL());
 				if(err != KErrNone)
 					{
 					// Re-connection to server failed at 1st hurdle - give up and
 					// report the error.
 					ReportSocketError(aError);
 					}
 				else
 					return;	// need early exit to maintain correct state - doh!		
 				}
			else
 				{
 				// Report the socket error
 				ReportSocketError(aError);					
 				}
 			}
		else
 			{
			// Report the socket error 
			ReportSocketError(aError);			
 			}
		}
#if defined (_DEBUG) && defined (_LOGGING)
	else
		{
		__FLOG_4(_T8("!! Connection closed to host %S, remote port %d (secure : %d, nonpersistent : %d)"), 
				&iConnectionInfo->Host(), 
				iConnectionInfo->Port(), 
				iConnectionInfo->IsSecure(), 
				iConnectionInfo->IsNonPersistent()
				);
		if( iConnectionInfo->IsNonPersistent() && aError == KErrEof )
			{
			__FLOG_0(_T8("-> non-persistent connection : expected shutdown"));
			}
		}
#endif

	// Move to Idle state...
	SetIdleState();
	}


void CHttpConnectionManager::ReportSocketError(TInt aError)
	{
	if( iCurrentResponse != NULL )
		{
		__FLOG_0(_T8("-> notifying waiting response(s)"));

		TInt error = aError;
	
		
		// If we have a current request/response, cancel them
		if( iCurrentRequest )
			{
			__FLOG_0(_T8("-> cancelling request"));
			iCurrentRequest->CancelRequest();
			}
				
		if( iCurrentResponse )
			{
			__FLOG_0(_T8("-> cancelling response"));
			iCurrentResponse->CancelResponse();
			}
	
	
		iCurrentResponse->ConnectionError(error);

		if( iState != EConnecting && iPendingResponses.Count() > 0 )
			{
			__FLOG_0(_T8("-> there are pipelined transactions - re-submit without pipelining"));
			__FLOG_2(_T8("-> reporting %d (KErrHttpPipeliningError) instead of %d"), KErrHttpPipeliningError, aError);

			// Change reported error as KErrHttpPipeliningError - this 
			// indicates that those transaction can be re-submitted but 
			// should not be pipelined.
			error = KErrHttpPipeliningError;
			
			// Insert the host to probable pipeline list
			if(iConnectionInfo)
			    {
			    __FLOG_1(_T8("-> Insert host: %S to probable pipelined host list"), &iConnectionInfo->Host());
			    iPipelineFallback.InsertPipelineFailedHost(iConnectionInfo->Host());
			    }
			
			}
		
		// Notify any pending (pipelined) transactions and the current
		// transaction of the error.
		NotifyPendingTransactions(error);
	
		// The current response and request are now no longer valid.
		iCurrentRequest = NULL;
		iCurrentResponse = NULL;
		}
#if defined (_DEBUG) && defined (_LOGGING)
	else
		{
		__FLOG_0(_T8("-> connection not being used - no waiting transactions"));
		}
#endif
	}

void CHttpConnectionManager::ReconnectSocketL()
	{
	__FLOG_0(_T8("-> Attempting to re-connect to server"));

	// Need to re-connect to the server. Clear the flags
	iFlags.Clear(EPendingWriteInConnectedState);

	__FLOG_4(
			_T8("Re-connecting to host %S, remote port %d (secure : %d, nonpersistent : %d)"), 
			&iConnectionInfo->Host(), 
			iConnectionInfo->Port(), 
			iConnectionInfo->IsSecure(), 
			iConnectionInfo->IsNonPersistent()
			);

	// Need to start connection to the appropriate host
	iSocketConnector = &iSocketFactory.ConnectL(
											   *this, 
											   iConnectionInfo->Host(), 
											   iConnectionInfo->Port()
											   );
	// Move to Connecting state...
	iState = EConnecting;
	}

void CHttpConnectionManager::MakeConnectionNonPersistent()
	{
	// Is the connection already non-persistent?
	if( !iConnectionInfo->IsNonPersistent() )
		{
		// Nope - change to be non-persistent...
		iConnectionInfo->SetPersistentState(CHttpConnectionInfo::ENonPersistent);

		__FLOG_0(_T8("!! Converting to non-persistent connection"));
		__FLOG_4(
				_T8("-> connection to host %S, remote port %d (secure : %d, nonpersistent : %d)"), 
				&iConnectionInfo->Host(), 
				iConnectionInfo->Port(), 
				iConnectionInfo->IsSecure(), 
				iConnectionInfo->IsNonPersistent()
				);

		// Flag that this connection now cannot pipeline - cancel any pending 
		// trasactions with a non-pipelining error code. This indicates that 
		// those transaction can be re-submitted and still be pipelined.
		iFlags.Set(ECannotPipeline);
		NotifyPendingTransactions(KErrHttpPipeliningError);

		iCurrentRequest = NULL;
		}
	}

void CHttpConnectionManager::CheckRequestComplete(MHttpRequest& aRequest)
	{
	// Check to see if the current request matches the request to be stopped.
	if( !IsFirstTransaction() && iCurrentRequest == &aRequest)
		{
		__FLOG_0(_T8("!! Incomplete request"));
		__FLOG_0(_T8("-> cancelling request and moving to non-persistent connection"));
		
		// Ok, this request is not complete - cancel it.

		if (aRequest.CheckRequestPendingComplete())
			{
			// Request was done anyway. Just make sure to discard the next confirmation of 
			// data sent.
			iFlags.Set(EDiscardSndDataCnf);
			}
		else
			{
			// Change connection to non-persistent - this should take care of any
			// pipelined requests.
			MakeConnectionNonPersistent();
			}

		if(iCurrentRequest)
			{
			iCurrentRequest->CancelRequest();
			iCurrentRequest = NULL;
			}
		}
	}

TInt CHttpConnectionManager::FindRequest(MHttpRequest& aRequest)
	{
	TInt ii = iPendingRequests.Count();

	while( ii > 0)
		{
		if( &aRequest == iPendingRequests[--ii] )
			{
			// Found the request - pass the appropriate index value.
			return ii;
			}
		}

	return KErrNotFound;
	}

TInt CHttpConnectionManager::FindResponse(MHttpResponse& aResponse)
	{
	TInt ii = iPendingResponses.Count();

	while( ii > 0)
		{
		if( &aResponse == iPendingResponses[--ii] )
			{
			// Found the response - pass the appropriate index value.
			return ii;
			}
		}

	return KErrNotFound;
	}

void CHttpConnectionManager::NotifyPendingTransactions(TInt aError)
	{
	TInt ii = iPendingResponses.Count();

	while( ii > 0 )
		{
		// Notify the waiting transaction of the error 
		iPendingResponses[--ii]->ConnectionError(aError);
		}

	// Reset the pending request and response queues.
	iPendingRequests.Reset();
	iPendingResponses.Reset();
	}

void CHttpConnectionManager::DisablePipelining()
	{
	__ASSERT_DEBUG( iState == EIdle || iState == EIdleConnected || iState == EClosing, User::Invariant() );
	__ASSERT_DEBUG( !CannotPipeline(), User::Invariant() );

	iFlags.Set(ECannotPipeline);
	}

void CHttpConnectionManager::SetIdleState()
	{
	// The connection is now Idle - reset tunnel and pipeline flags.
	iState = EIdle;
	iTunnel = EFalse;
	iFlags.Clear(ECannotPipeline);
	iFlags.Clear(EPendingWriteInConnectedState);
	}

/*
 *	Methods from MHttpRequestObserver
 */

void CHttpConnectionManager::SendRequestDataL(const TDesC8& aData)
	{
	__ASSERT_DEBUG( iState == EConnected, User::Invariant() );

	iOutputStream->SendDataReqL(aData);	
	}
	
void CHttpConnectionManager::RequestComplete()
	{
	__ASSERT_DEBUG( iState == EConnected, User::Invariant() );
    
	//Start the Receive Timer
	StartRecvTimer();

	// Current request is complete.
	if( !IsFirstTransaction() )
		{
		iCurrentRequest = NULL;

		if( iPendingRequests.Count() > 0 )
			{
			// More requests are pending - start the next one. Remove it from the
			// pending queue.
			iCurrentRequest = iPendingRequests[0];
			iPendingRequests.Remove(0);

			iCurrentRequest->StartRequest();
			}
		}
	}

void CHttpConnectionManager::SendingBodyData(TBool aValue)
    {
    if(iOutputStream)
        {
        iOutputStream->SetTCPCorking(aValue);
        }
    }

/*
 *	Methods from MHttpResponseObserver
 */

void CHttpConnectionManager::ResponseDataParsed()
	{
	__ASSERT_DEBUG( iState == EConnected, User::Invariant() );
	
	if( iCurrentResponse == NULL )
		{
		// The current response has finished with the connection - was it a 
		// non-persistent connection?
		if( IsFirstTransaction() )
			{
			iFlags.Clear(EFirstTransaction);
			iCurrentRequest = NULL;
			}

		// More requests are pending - start the next one, 
		// and remove it from the pending queue.
		if ( iPendingRequests.Count() > 0 )
			{
			iCurrentRequest = iPendingRequests[0];
			iPendingRequests.Remove(0);
			if( iPendingResponses.Count() > 0 )
				{
				iCurrentResponse = iPendingResponses[0];
				iPendingResponses.Remove(0);
				}
				iState = EConnected;
				iFlags.Set(EPendingWriteInConnectedState);
				iCurrentRequest->StartRequest();
			}
		else if( iConnectionInfo->IsNonPersistent() )
			{
			__FLOG_0(_T8("!! Non-persistent connection : expect server to close connection"));
			__FLOG_4(
					_T8("-> moving to Closing state on connection to host %S, remote port %d (secure : %d, nonpersistent : %d)"), 
					&iConnectionInfo->Host(), 
					iConnectionInfo->Port(), 
					iConnectionInfo->IsSecure(), 
					iConnectionInfo->IsNonPersistent()
					);

			__ASSERT_DEBUG( iPendingResponses.Count() == 0, User::Invariant() );
			
			iFlags.Clear(ECannotPipeline);
			iState = EClosing;		
			}
		else
			{
			// Connection is persistent and no longer waiting for any more
			// response data - going to IdleConnected state. Also, allow 
			// pipelining to occur on this connection.
			iState = EIdleConnected;
			iFlags.Clear(ECannotPipeline);
			}
			
        //Start the Receive Timer
		StartRecvTimer();

		// Update the state and notify the input stream that the received data 
		// is no longer needed.
		iInputStream->ReceivedDataRes();
		}
	else
		{
		// Check to see if there is any excess data.
		if( iExcessData.Length() > 0 )
			{
			// Spoof receiving this data.
			__FLOG_5(_T8("Received %d bytes of data on connection to host %S, remote port %d (secure : %d, nonpersistent : %d)"),
					iExcessData.Length(),
					&iConnectionInfo->Host(), 
					iConnectionInfo->Port(), 
					iConnectionInfo->IsSecure(), 
					iConnectionInfo->IsNonPersistent()
					);

			// The current transaction should be given the data. Once passed to 
			// the response, reset the excess data.
			iCurrentResponse->ResponseDataReceived(iExcessData);	
			iExcessData.Set(KNullDesC8());
			}
		else
			{
			//Start the Receive Timer
			if(!iCurrentResponse->ResponseInformational())
				{
				StartRecvTimer();
				}
			// Notify the input stream that the received data is no longer needed
			iInputStream->ReceivedDataRes();
			}
		}
	}
	
void CHttpConnectionManager::ResponseComplete(const TDesC8& aExcessData)
	{
	// Remove the current response.
	iCurrentResponse = NULL;
	__FLOG_1(_T8("Received  %d bytes of excess data"), aExcessData.Length());

	// Need to store the excess data.
	if( iPendingResponses.Count() > 0  )
		{
		iExcessData.Set(aExcessData);

		if( !IsFirstTransaction())
			{
			// More responses are pending - get the next one. Remove it from the
			// pending queue.
			iCurrentResponse = iPendingResponses[0];
			iPendingResponses.Remove(0);
			}
		}
	}

/*
 *	Methods from MSocketConnectObserver
 */
 
void CHttpConnectionManager::ConnectionMadeL(MInputStream& aInputStream, MOutputStream& aOutputStream)
	{
	// Connector object no longer valid
	iSocketConnector = NULL;
	
	// Bind to the input stream
	iInputStream = &aInputStream;
	
	iOutputStream = &aOutputStream;

	if(iOptimiser)
		{
		delete iOptimiser;
		iOptimiser = NULL;
		}
			
	__FLOG_4(
			_T8("Connected to host %S, remote port %d (secure : %d, nonpersistent : %d)"), 
			&iConnectionInfo->Host(), 
			iConnectionInfo->Port(), 
			iConnectionInfo->IsSecure(), 
			iConnectionInfo->IsNonPersistent()
			);
			
	TBool batchingEnabled = EFalse;
	MHttpDataOptimiser* dataOptimiser = iCurrentRequest->HttpDataOptimiser( batchingEnabled );
	
	if (!CannotPipeline())
		{
		TInt bufferSize = 0;
		bufferSize = iCallback.GetMaxBatchingBufferSize();

		if (bufferSize > 0)	// Therefore, implicitly, batching is supported
			{
			if (iRequestBatcher)
				{
				delete iRequestBatcher;
				iRequestBatcher = NULL;
				}
			
			if( dataOptimiser && batchingEnabled )
				{
				// dataOptimiser has been set for the current session
				iOptimiser = CHttpClientOptimiser::NewL(*iOutputStream, *this);
				iOptimiser->BindOptimiser(*dataOptimiser);
				iRequestBatcher = CHttpRequestBatcher::NewL(*iOptimiser, bufferSize);
				__FLOG_0(_T8("-> Created request batcher"));
				__FLOG_0(_T8("-> HTTP Data Optimiser has been set"));
				iInputStream->Bind(*iOptimiser);
				}

			else
				{
				iRequestBatcher = CHttpRequestBatcher::NewL(*iOutputStream, bufferSize);
				__FLOG_0(_T8("-> Created request batcher"));
				iOutputStream->Bind(*iRequestBatcher);
				iInputStream->Bind(*this);	
				}
			iRequestBatcher->Bind(*this);
			iOutputStream = iRequestBatcher;
			}
		else
			{
			// We are pipelining but not batching
			__FLOG_0(_T8("-> Did not create request batcher as batching disabled"));			

			CreateOptimiserL( dataOptimiser );
			}
		}
	else
		{
		// We are not pipelining
		__FLOG_0(_T8("-> Did not create request batcher as pipelining disabled"));
		
		CreateOptimiserL( dataOptimiser );
		}

	if( iConnectionInfo->IsSecure() )
		{
		// Upgrade the connection to be secure.
		UpgradeConnectionL();
		}
	else
		{
		// Move to Connected state and notify the current request to start.
		iState = EConnected;
		iCurrentRequest->StartRequest();
		}
	}

TInt CHttpConnectionManager::HandleConnectError(TInt aError)
	{
	// Ok, the connector object is now invalid
	iSocketConnector = NULL;

	// Handle the error...
	HandleSocketError(aError);

	__ASSERT_DEBUG( iState == EIdle, User::Invariant() );

	return KErrNone;
	}

void CHttpConnectionManager::MSocketConnectObserver_Reserved()
	{
	User::Invariant();
	}

/*
 * Methods from MInputStreamObserver
 */

void CHttpConnectionManager::ReceivedDataIndL(const TDesC8& aBuffer)
	{
	if( iCurrentResponse != NULL )
		{
		__FLOG_5(_T8("Received %d bytes of data on connection to host %S, remote port %d (secure : %d, nonpersistent : %d)"),
				aBuffer.Length(),
				&iConnectionInfo->Host(), 
				iConnectionInfo->Port(), 
				iConnectionInfo->IsSecure(), 
				iConnectionInfo->IsNonPersistent()
				);
		
		// The current transaction should be given the data.
		iCurrentResponse->ResponseDataReceived(aBuffer);
		}
	else
		{
		__FLOG_1(_T8("!! Spurious data : %d bytes of data ignored"), aBuffer.Length());
		__FLOG_4(
				_T8("-> data received on connection to host %S, remote port %d (secure : %d, nonpersistent : %d)"), 
				&iConnectionInfo->Host(), 
				iConnectionInfo->Port(), 
				iConnectionInfo->IsSecure(), 
				iConnectionInfo->IsNonPersistent()
				);
        
		//Start the Receive Timer
		StartRecvTimer();

		// This data does not belong to any transaction - ignore it.
		iInputStream->ReceivedDataRes();
		}
	}

void CHttpConnectionManager::SecureServerCnf()
	{
	User::Invariant();
	}

void CHttpConnectionManager::InputStreamCloseInd(TInt aError)
	{
	// The input stream is no longer of any use as the connection has closed.
	iInputStream = NULL;

	// Handle the error only if the output stream observer has not done so 
	// already.
	if( iOutputStream != NULL )
		{
		HandleSocketError(aError);
		}
	}

void CHttpConnectionManager::MInputStreamObserver_Reserved()
	{
	User::Invariant();	
	}

/*
 * Methods from MOutputStreamObserver
 */

void CHttpConnectionManager::SendDataCnfL()
	{
	if (DiscardSndDataCnf())
		{
		__FLOG_0(_T8("-> Disgarding Confirmation that data was sent"));
		iFlags.Clear(EDiscardSndDataCnf);
		if (iState == EConnected)
			{
			RequestComplete();
			}
		else
			{
			__FLOG_0(_T8("-> Initialising iCurrentRequest"));
			iCurrentRequest=NULL;
			}
		}
	// Notify the current request that its data has been sent.
	else if( iCurrentRequest)
		{
		iCurrentRequest->RequestDataSent();

		// The request has now sent data - reset the pending write flag.
		iFlags.Clear(EPendingWriteInConnectedState);
		}
	}

void CHttpConnectionManager::SecureClientCnf()
	{
	__ASSERT_DEBUG( iState == EUpgrading, User::Invariant() );
	
	iSecureRetry = EFalse; //reset the flag


#if defined (_DEBUG) && defined (_LOGGING)
	__FLOG_0(_T8("!! Secure connection establised"));

	if( iTunnel )
		{
		__FLOG_5(
				_T8("-> secure tunnel to %S via host %S, remote port %d (secure : %d, nonpersistent : %d)"), 
				&iTunnelHost.DesC(), 
				&iConnectionInfo->Host(), 
				iConnectionInfo->Port(), 
				iConnectionInfo->IsSecure(), 
				iConnectionInfo->IsNonPersistent()
				);
		}
	else
		{
		__FLOG_4(
				_T8("-> secure connection to host %S, remote port %d (secure : %d, nonpersistent : %d)"), 
				&iConnectionInfo->Host(), 
				iConnectionInfo->Port(), 
				iConnectionInfo->IsSecure(), 
				iConnectionInfo->IsNonPersistent()
				);
		}
#endif

	// Move to Connected state and notify the current request to start.
	iState = EConnected;
	iCurrentRequest->StartRequest();		
	}

void CHttpConnectionManager::OutputStreamCloseInd(TInt aError)
	{
	// The output stream is no longer of any use as the connection has closed.
	iOutputStream = NULL;

	// Handle the error only if the input stream observer has not done so 
	// already.
	if( iInputStream != NULL )
		{
		HandleSocketError(aError);
		}
	}

void CHttpConnectionManager::DoResponseCompletion ()
	{
	__ASSERT_DEBUG( iCurrentResponse != NULL, User::Invariant() );
	// We do these operations only for 3xx responses and the response parsing
	// hasn't happened completely
	if ( iCurrentResponse->NeedCompletion() && !iCurrentResponse->ResponseCompleted () )
		{
		// Try to complete the response. Parser may have the data but not processed.
		TBool responseCompleted = iCurrentResponse->CompleteResponse ( KNullDesC8() );		
		while ( !responseCompleted )
			{
			// Read and try complete the response
			TPtrC8 data;
			__FLOG_0(_T8("!! doing an immediate socket read"));					
			TInt ret = iInputStream->ImmediateRead ( data );
			if ( ( iPendingResponses.Count() == 0 ) || ret <= KErrNone )
				{
				__FLOG_0(_T8("!! Breaking from the loop"));
				// no further data is expected or there is a socket error
				break;					
				}
			responseCompleted = iCurrentResponse->CompleteResponse ( data );
			}
		}
	}

void CHttpConnectionManager::MOutputStreamObserver_Reserved()
	{
	User::Invariant();
	}
	
MHttpResponse* CHttpConnectionManager::CurrentResponse()
	{
	return iCurrentResponse;	
	}

void CHttpConnectionManager::CreateOptimiserL(MHttpDataOptimiser* aDataOptimiser)
	{
	if( aDataOptimiser )
		{
		// The dataOptimiser has been set, it does not matter if it is for session or transaction
		iOptimiser = CHttpClientOptimiser::NewL(*iOutputStream, *this);
		iOptimiser->BindOptimiser(*aDataOptimiser);
		__FLOG_0(_T8("-> HTTP Data Optimiser has been set"));
		iInputStream->Bind(*iOptimiser);
		iOutputStream = iOptimiser;
		iOutputStream->Bind(*this);
		}
	
	else
		{
		iInputStream->Bind(*this);
		iOutputStream->Bind(*this);	
		}
	}

void CHttpConnectionManager::OnReceiveTimeOut()
	{
	if(iCurrentResponse)
		{
		iCurrentResponse->OnResponseReceiveTimeOut();
		}
	}

void CHttpConnectionManager::OnSendTimeOut()
	{
	if(iCurrentRequest)
		{
		iCurrentRequest->OnRequestSendTimeOut();
		}
	}

TInt CHttpConnectionManager::SendTimeOutVal()
	{
	if(iCurrentRequest)
		{
		return iCurrentRequest->SendTimeOutValue();
		}
	return 0;
	}

void CHttpConnectionManager::StartRecvTimer()
	{
	if(iCurrentResponse)
		{
		TInt timeoutValue = iCurrentResponse->ReceiveTimeOutValue();
		if(iInputStream)
			{
			iInputStream->StartReceieveTimer(timeoutValue);
			}
		}
	}

void CHttpConnectionManager::AppendPipelineFailedHost(const TDesC8& aHost)
	{
 	iPipelineFallback.AppendPipelineFailedHost(aHost);
	}


CHttpHostElement* CHttpHostElement::New(const TDesC8& aHost)
    {
    CHttpHostElement* self = new CHttpHostElement;
    if(self && self->Construct(aHost))
        {
        return self;
        }
    delete self;
    return NULL;
    }

CHttpHostElement::~CHttpHostElement()
    {
    delete iHost;
    }

TBool CHttpHostElement::Construct(const TDesC8& aHost)
    {
    iHost = aHost.Alloc();
    return (iHost != NULL);
    }

CHttpPipelineFallback::CHttpPipelineFallback()  
: iPipelineFailedHosts(16), // With granularity 16
iProbablePipelineFailedHosts(CHttpHostElement::LinkOffset())
    {
    
    }

CHttpPipelineFallback* CHttpPipelineFallback::NewL()
    {
    return new(ELeave)CHttpPipelineFallback;
    }

CHttpPipelineFallback::~CHttpPipelineFallback()
    {
    iPipelineFailedHosts.ResetAndDestroy();
    // Free up the objects pointed by the list
    CHttpHostElement* element;
    TSglQueIter<CHttpHostElement> it(iProbablePipelineFailedHosts);
    while((element = it++) != NULL)
        {
        iProbablePipelineFailedHosts.Remove(*element);
        delete element;
        }    
    }

TBool CHttpPipelineFallback::NeedPipelineFallback(const TDesC8& aHost)
    {    
    TInt count = iPipelineFailedHosts.Count();
    for(TInt i = count - 1; i >= 0; --i)
        {
        if(aHost.CompareF(*(iPipelineFailedHosts[i])) == 0)
            return ETrue;
        }
    return EFalse;
    }

void  CHttpPipelineFallback::InsertPipelineFailedHost(const TDesC8& aHost)
    {
    // Already failed. no need to check further.
    if(NeedPipelineFallback(aHost))
        {
        return;
        }
    CHttpHostElement* element = NULL;
    TSglQueIter<CHttpHostElement> it(iProbablePipelineFailedHosts);
    while((element = it++) != NULL)
        {
        if(element->Host().CompareF(aHost) == 0)
            {                                     
            // Remove from the list and push into the pipeline failed array
            iProbablePipelineFailedHosts.Remove(*element);
            iPipelineFailedHosts.Append(element->AcquireHost()); // No need to check the return code 
                                                                     // as, a failure does not matter here.
            delete element; // Delete the host element.
            return;
            }
        }
    // A new host has failed on the pipelining. Add into the list
    element = CHttpHostElement::New(aHost);
    if(element)
        {        
        iProbablePipelineFailedHosts.AddLast(*element);
        }
    
    }
	
	void CHttpPipelineFallback::AppendPipelineFailedHost(const TDesC8& aHost)
	{
 		// Already failed. no need to check further.
     if(NeedPipelineFallback(aHost))
         {
         return;
         }
 		
 		// Failure doesn't matter here.
 		HBufC8* host = aHost.Alloc();
 		if(host == NULL)
 			{
 			return;
 			}
 		
 		iPipelineFailedHosts.Append(host); // no error checking as failure does not matter
 																			 // we will keep going.
 }





	


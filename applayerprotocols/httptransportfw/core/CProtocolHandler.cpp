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

// System includes



#include <ecom/ecom.h>
#include <ecom/ecomerrorcodes.h>
#include <http/framework/cheadercodec.h>
#include <http/framework/cprottransaction.h>
#include <http/framework/csecuritypolicy.h>
#include <http/thttpevent.h>
#include <http/rhttprequest.h>
#include <http/rhttptransaction.h>
#include <http/rhttpheaders.h>
#include <http/mhttpdatasupplier.h>
#include <httperr.h>
#include <http/framework/cprotocolhandler.h>// Class signature
#include "CHTTPSession.h"
#include "CTransaction.h"
// Local includes


/** Implementation of CProtocolHandler class *****************************************************
*/

CProtocolHandler* CProtocolHandler::NewL(const TDesC8& aProtocol, RHTTPSession aSession)
	{
	// Set resolving parameters
	TEComResolverParams resolverParams;
	resolverParams.SetDataType(aProtocol);
	resolverParams.SetWildcardMatch(ETrue);

	// Get the instantiation
	TAny* ptr = REComSession::CreateImplementationL(KUidProtocolHandlerPluginInterface,
								 _FOFF(CProtocolHandler, iDtor_ID_Key),
								 &aSession, resolverParams);

	CProtocolHandler* protHandler = REINTERPRET_CAST(CProtocolHandler*, ptr);

	return protHandler;
	}

EXPORT_C
CProtocolHandler::~CProtocolHandler()
	{
	Cancel();
	iTransactions.ResetAndDestroy();
	delete iCodec;
	delete iSecurityPolicy;
	REComSession::DestroyedImplementation(iDtor_ID_Key);
	}

EXPORT_C
void CProtocolHandler::MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent)
	{
	// Handle the event
	switch (aEvent.iStatus)
		{
	case THTTPEvent::ESubmit:
		{
#if defined (_DEBUG) && defined (_LOGGING)
		RHTTPRequest rq = aTransaction.Request();
		RStringF method = rq.Method();
		const TDesC8& method8 = method.DesC();
		HBufC16* method16 = HBufC16::NewLC(method8.Length());
		TPtr16 method16_ptr = method16->Des();
		method16_ptr.Copy(method8);
		const TDesC8& uri8 = rq.URI().UriDes();
		HBufC16* uri16 = HBufC16::NewLC(uri8.Length());
		TPtr16 uri16_ptr = uri16->Des();
		uri16_ptr.Copy(uri8);

		__LOG3(_L("Transaction (id=%d) submitted, method=%S, URI=%S"),
				aTransaction.Id(), &method16_ptr, &uri16_ptr);

		CleanupStack::PopAndDestroy(2);  //uri16, method16
#endif
		SubmitTransactionL(aTransaction);
		}
		break;
	case THTTPEvent::ENotifyNewRequestBodyPart:
		{
		__LOG1(_L("Transaction (id=%d) has new request body data ready to transmit"), aTransaction.Id());
		// Is there a CProtTransaction object for this transaction?
		CProtTransaction* trans = FindProtocolTransaction(aTransaction);
		if(trans != NULL)
			{
			// Handle this new body data
			NotifyNewRequestBodyPart(*trans);
			}
		}
		break;
	case THTTPEvent::ECancel:
		{
		__LOG1(_L("Transaction (id=%d) cancelled"), aTransaction.Id());
		// Cancel the transaction. Get the specific protocol handler to do its
		// own clean up. 
		HandleCancelTransaction(aTransaction);
		}
		break;
	case THTTPEvent::EClosed:
		{
		__LOG1(_L("Transaction (id=%d) closed"), aTransaction.Id());
		// Close the transaction. Get the specific protocol handler to do its
		// own clean up.
		HandleClosedTransaction(aTransaction);
		}
		break;
	default:
		// Silently ignore anything else sent our way.  We expect another event 
		// describing 'transaction cancelled' in the future.
		__LOG2(_L("Transaction (id=%d) : unrecognised event %d"), aTransaction.Id(), aEvent.iStatus);
		break;
		}
	}

EXPORT_C
void CProtocolHandler::MHFSessionRunL(const THTTPSessionEvent& /*aEvent*/)
	{
	}

#ifdef _DEBUG
EXPORT_C
TInt CProtocolHandler::MHFRunError(TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& aEvent)
#else
EXPORT_C
TInt CProtocolHandler::MHFRunError(TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& /*aEvent*/)
#endif // _DEBUG
	{
	__LOG3(_L("Transaction (id=%d) : MHFRunError (error=%d) for event %d"), aTransaction.Id(), aError, aEvent.iStatus);
	// Attempt to send the error back as an event to the client; if this fails, give up.
	TInt err = aTransaction.SendEvent(aError, THTTPEvent::EIncoming, THTTPFilterHandle(THTTPFilterHandle::EProtocolHandler));
	if (err) // If we can't send events, we'll have to fail the transaction
		aTransaction.Fail(THTTPFilterHandle::EProtocolHandler);
		
	return KErrNone;
	}

EXPORT_C
TInt CProtocolHandler::MHFSessionRunError(TInt aError, const THTTPSessionEvent& /*aEvent*/)
	{
	return aError;
	}

EXPORT_C
void CProtocolHandler::MHFUnload(RHTTPSession /*aSession*/, THTTPFilterHandle /*aHandle*/)
	{
	}

EXPORT_C
void CProtocolHandler::MHFLoad(RHTTPSession /*aSession*/, THTTPFilterHandle /*aHandle*/)
	{
	}

EXPORT_C
CHeaderCodec* CProtocolHandler::Codec() const
	{
	return iCodec;
	}



EXPORT_C const CCertificate* CProtocolHandler::SessionServerCert()
{
	MProtHandlerInterface* interfacePtr = NULL;
	GetInterfaceL( TUid::Uid(KProtHandlerSessionServerCertUid), interfacePtr);
	if (interfacePtr)
		return interfacePtr->SessionServerCert();
	else
		return NULL;
}

EXPORT_C const CCertificate* CProtocolHandler::TransactionServerCert( RHTTPTransaction aTransaction)
{
	MProtHandlerInterface* interfacePtr = NULL;
	GetInterfaceL( TUid::Uid(KProtHandlerTransactionServerCertUid), interfacePtr);
	if (interfacePtr)
		return interfacePtr->TransactionServerCert(aTransaction);
	else
		return NULL;
}

void CProtocolHandler::SubmitTransactionL(RHTTPTransaction aTransaction)
	{
	CProtTransaction* protTrans = FindProtocolTransaction(aTransaction);
	if(protTrans != NULL)
		{
		// would have been a cancelled protocol transaction object		
		// Make sure that CProtTransaction object is in the cancelled state
		if( protTrans->TransactionState() == CProtTransaction::ECancelled )
			{
			// Reset the request data
			MHTTPDataSupplier* dataSupplier = aTransaction.Request().Body();
			if( dataSupplier != NULL && dataSupplier->Reset() != KErrNone )
				{
				// The data supplier needs to be reset but it doesn't
				// support reset.
				aTransaction.Cancel(THTTPFilterHandle::EProtocolHandler);
				User::Leave(KErrHttpCantResetRequestBody);
				}

			// Reset the Tx & Rx data
			protTrans->ResetTxData();
			protTrans->ResetRxData();

			// Set the CProtTransaction state to EPending
			protTrans->SetTransactionState(CProtTransaction::EPending);
			

			// Go active again, to process new transaction
			CompleteSelf();
			}
		}
	else
		{
		// Create an appropriate CProtTransaction object
		protTrans = CreateProtTransactionL(aTransaction);
		
		// Add the transaction to the queue
		TInt error=iTransactions.Append(protTrans);

		if(error)
		{
			delete protTrans;
			User::Leave(error);
		}
		
		aTransaction.Implementation()->SetProtocolTransaction(protTrans);
		// Go active again, to process new transaction
		CompleteSelf();
		}
		
	}

//##ModelId=3A3E152C0398
void CProtocolHandler::HandleCancelTransaction(RHTTPTransaction aTrans)
	{
	// There are three possibilities:
	// 1) if the transaction has been serviced, but is not yet complete, it will be in the active queue.
	// 2) if the transaction has been serviced, and has completed, it will be in the completed queue
	// 3) if the transaction has not yet been serviced, it will be in the pending queue
	// For three possibilities, need to set the transaction state to ECancelled 

	// Cleanup the transaction
	aTrans.Response().GetHeaderCollection().RemoveAllFields();

	CProtTransaction* protTrans = FindProtocolTransaction(aTrans);
	if(protTrans != NULL)
		{
		// If the current state of the transaction is EActive, CompleteSelf() as
		// an active slot has been freed up
		if(protTrans->TransactionState() == CProtTransaction::EActive)
			{
			CompleteSelf();
			}
		
		// Change the transaction state
		protTrans->SetTransactionState(CProtTransaction::ECancelled);

		// Get the derived class to do any necessary cleanup
		CancelTransactionHook(*protTrans);
		}
	}

void CProtocolHandler::HandleClosedTransaction(RHTTPTransaction aTrans)
	{
	// The client has closed the transaction
	TInt pos = FindTransaction(aTrans);
	if( pos != KErrNotFound )
		{
		// Get the transaction wrapper 
		CProtTransaction* trans = iTransactions[pos];

		// If the current state of the transaction is EActive, CompleteSelf() as
		// an active slot has been freed up
		if( trans->TransactionState() == CProtTransaction::EActive )
			{
			CompleteSelf();
			}

		// De-queue the CProtTransaction
		iTransactions.Remove(pos);

		// Get the derived class to do any necessary cleanup - transfer ownership
		ClosedTransactionHook(trans);
		}
	}

TInt CProtocolHandler::FindTransaction(RHTTPTransaction aTransaction) const
	{
	const TUint count = iTransactions.Count();
	TInt ret = KErrNotFound;
	for(TUint i = 0; i < count; ++i)
		{
		if( iTransactions[i]->Transaction() == aTransaction )
			{
			ret = i;
			i = count;
			}
		}
	return ret;
	}

EXPORT_C
void CProtocolHandler::CompleteSelf()
	{
	// Get ready to service another transaction, if we're not active already
	if (!IsActive())
		{
		TRequestStatus* pStat = &iStatus;
		User::RequestComplete(pStat, KErrNone);
		SetActive();
		}
	}

EXPORT_C
void CProtocolHandler::RunL()
	{
	// Search for a pending transaction
	TInt count = iTransactions.Count();
	for( TInt ii = 0; ii < count; ++ii )
		{
		if( iTransactions[ii]->TransactionState() == CProtTransaction::EPending )
			{
			// Found a pending transaction - can it be serviced?
			CProtTransaction* trans = iTransactions[ii];

			// Remember the transaction in case ServiceL leaves
			iCurrentTransaction	= trans->Transaction();

			if( ServiceL(*trans) )
				{
				__LOG1(_L("Transaction (id=%d) can be serviced - adding to active queue"), trans->Transaction().Id());
				
				// Change transaction state to active
				trans->SetTransactionState(CProtTransaction::EActive);
				}
   			}
		}
	Cancel();
	// Next time we go active will be either when a new transaction is submitted
	// (done in MHFRunL above), or when an active transaction completes (handled
	// in subclasses, but effected by calling transaction complete).
	}

EXPORT_C
TInt CProtocolHandler::RunError(TInt aError)
	{
	// Any error caught here is due to ServiceL leaving. For now, pass these all
	// back to the client. Later they might be dealt with individually. The 
	// transaction that caused RunL() to leave is remembered by 
	// iCurrentTransaction. Following the error the transaction must be cancelled.
	// Cleanup occurs later when the transaction is closed by the client.
	__LOG1(_L("CProtocolHandler::RunError %d"), aError);

	// Send the error to the client
	TInt err = iCurrentTransaction.SendEvent(aError, 
											  THTTPEvent::EIncoming, 
											  THTTPFilterHandle(THTTPFilterHandle::EProtocolHandler)
											  );
	// Handle the cancellation - this will get the specific protocol handler
	// to do its clean up too.
	HandleCancelTransaction(iCurrentTransaction);
	// If the event could not be sent - fail the transaction
	if( err )
		iCurrentTransaction.Fail();
	return KErrNone;
	}

EXPORT_C
void CProtocolHandler::DoCancel()
	{
	}

EXPORT_C 
void CProtocolHandler::TransactionCompletedL(RHTTPTransaction aTrans, THTTPEvent aEventStatus)
	{
	User::LeaveIfError(TransactionCompleted(aTrans, aEventStatus));
	}

EXPORT_C 
TInt CProtocolHandler::TransactionCompleted(RHTTPTransaction aTrans, THTTPEvent aEventStatus)
    {
    __LOG1(_L("Transaction (id=%d) completed"), aTrans.Id());
    // Send the 'complete' status messages for this RHTTPTransaction
    TInt err = aTrans.SendEvent(aEventStatus, THTTPEvent::EIncoming, THTTPFilterHandle(THTTPFilterHandle::EProtocolHandler));
    if(err != KErrNone)
        {
        return err;
        }

    // Change the transaction state
    CProtTransaction* protTrans = FindProtocolTransaction(aTrans);
    if(protTrans != NULL)
        {
        // If the current state of the transaction is EActive, CompleteSelf() as
        // an active slot has been freed up
        if(protTrans->TransactionState() == CProtTransaction::EActive)
            {
            CompleteSelf();
            }

        // Change the transaction state
        protTrans->SetTransactionState(CProtTransaction::ECompleted);
        }
    return err;
    }

EXPORT_C
void CProtocolHandler::TransactionFailed(RHTTPTransaction aTrans)
	{
	__LOG1(_L("Transaction (id=%d) completed"), aTrans.Id());

	// Cleanup the transaction
	aTrans.Response().GetHeaderCollection().RemoveAllFields();

	CProtTransaction* protTrans = FindProtocolTransaction(aTrans);
	if(protTrans != NULL)
		{

		// If the current state of the transaction is EActive, CompleteSelf() as
		// an active slot has been freed up
		if(protTrans->TransactionState() == CProtTransaction::EActive)
			{
			CompleteSelf();
			}
		
		// Change the transaction state
		protTrans->SetTransactionState(CProtTransaction::ECancelled);

		// Reset the Rx and Tx data - ok to reset the Rx data object since as
		// the transaction has been failed the client should not be pulling 
		// response data from it. 
		protTrans->ResetTxData();
		protTrans->ResetRxData();
		}
	}

EXPORT_C
CProtocolHandler::CProtocolHandler(RHTTPSession aSession)
	: CActive(KProtocolHandlerActivePriority), iSession(aSession), iTransactions(32)
	{
	CActiveScheduler::Add(this);
	}

EXPORT_C
void CProtocolHandler::ConstructL(RHTTPSession aSession)
	{
	TRAPD(err, iSecurityPolicy = CSecurityPolicy::NewL(aSession.StringPool()));
	if (err != KErrNone && err != KErrNotFound && err != KEComErrNoInterfaceIdentified)
		User::Leave(err);

	// Invoke sub-class method to get a protocol-specific codec
	CreateCodecL();
	}


EXPORT_C TInt CProtocolHandler::NumActiveTransactions() const
	{
	TInt activeTrans = 0;
	TInt count = iTransactions.Count();
	while( count != 0)
		{
		if( iTransactions[--count]->TransactionState() == CProtTransaction::EActive)
			++activeTrans;
		if(count != 0 && iTransactions[--count]->TransactionState() == CProtTransaction::EActive)
			++activeTrans;
		}
	return activeTrans;
	}

EXPORT_C TInt CProtocolHandler::FindTransaction(RHTTPTransaction aTransaction, 
												const CProtTransaction*& aProtTransaction) const
	{
	const TInt count = iTransactions.Count();
	TInt result = KErrNotFound;
	// Search for CProtTransaction wrapper around aTransaction
	for(TInt index = 0; (index<count) && (result==KErrNotFound); ++index)
		{
		if( iTransactions[index]->Transaction() == aTransaction )
			{
			// found it - done
			result = index;
			aProtTransaction = iTransactions[index];
			}
		}
	return result;
	}

EXPORT_C CProtTransaction* CProtocolHandler::FindProtocolTransaction(RHTTPTransaction aTransaction) const
    {
    return aTransaction.Implementation()->ProtocolTransaction();
    }


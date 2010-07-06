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
#include <e32std.h>


#include <http/mhttptransactioncallback.h>
#include <http/thttpfilterregistration.h>
#include <httpstringconstants.h>
#include <httperr.h>

// Local includes
#include "CHTTPSession.h"
#include "CRequest.h"
#include "CResponse.h"
#include "CHTTPManager.h"
#include "CHeaderField.h"

// Class signature
#include "CTransaction.h"

#define LOGFILE "events.csv"

void CTransaction::CancelTransaction(THTTPFilterHandle aStart)
	{
	// Delete all current events.
	iEventQueue.Reset();
	// Cancel the active object
	Cancel();
	// And send the cancel event
	SynchronousSendEvent(THTTPEvent::ECancel, THTTPEvent::EOutgoing, aStart);
	if (iStatus != EInFilter && iStatus != ECancelled)
		iStatus = EPassive;
	else
		iStatus = ECancelled;
	}

void CTransaction::Close()
	{
	// If the request and the response don't exist yet, the object
	// isn't fully constructed and so shouldn't send any cancel or
	// close messages as its part-constructed state could confuse
	// filters.
	if (!iResponse)
		{
		CHeaderFieldPart::ClosePropertySet(iPropertySet);
		delete iRequest;
		delete this;
		return;
		}

	// If we're currently processing a close event we should stop
	// right now, to prevent infinite loops.
	if (iProcessingSyncEvent && 
		iCurrentSyncEvent.iEvent.iStatus == THTTPEvent::EClosed)
		return;

	// Cancel the transaction
	CancelTransaction(THTTPFilterHandle::EClient);
	// Remember the status
	TInt status = iStatus.Int();
	// And send out the 'close' message.
	SynchronousSendEvent(THTTPEvent(THTTPEvent::EClosed), 
						 THTTPEvent::EOutgoing, THTTPFilterHandle::EClient);
	iStatus = status;
	// The state will now be cancelled if and only if we were in a filter

	iSession.RemoveTransaction(this);
	CHeaderFieldPart::ClosePropertySet(iPropertySet);
	delete iRequest;
	delete iResponse;
	
	// Cancel the active object
	Cancel();

	if (status == CTransaction::ECancelled)
		{
		// If we're our transaction's RunL (which we probably are),
		// wait until we're back in the CTransaction before we delete it.
		iStatus = CTransaction::EExiting;
		}
	else
		{
		// We can safeley close straight away
		delete this;
		}

	}

void CTransaction::ConstructL(const TUriC8& aURI, RStringF aMethod)
	{
	RStringPool strP = iSession.Manager().StringPool();
	// If the string is the empty string, this means the default value
	// (of GET) should be used.
	if (aMethod == RStringF())
		aMethod = strP.StringF(HTTP::EGET,RHTTPSession::GetTable());
	aMethod.Copy();
	// The 0 header val in the property set has no meaning; the value
	// of the part is not used by property sets.
	CHeaderFieldPart* fieldPart = CHeaderFieldPart::NewL(THTTPHdrVal(0));
	fieldPart->SetPropertySet(iPropertySet);
	iRequest = CRequest::NewL(aMethod, aURI, this);
	iResponse = CResponse::NewL(this);
	iId = iSession.NextTransactionID();
	CActiveScheduler::Add(this);
	iSession.AddTransactionL(this);
	__OPEN_LOG(LOGFILE);
	__LOG(_L(",Transaction ID, Event, Direction, Filter"));
	}

void CTransaction::RunL()
	{
	// If we're in another transaction's RunL (which has called a
	// filter that has in turn run a nested active scheduler) we
	// should block now and try again when that transaction has
	// finished. This is to prevent obscure re-entrancy problems in
	// client code. See the comments on the blocking functions in the
	// session's header file for more details.
	THTTPEvent currentEvent = CurrentEvent().iEvent;	
	if(currentEvent == THTTPEvent::ESuspend)
		{	
		__ASSERT_DEBUG(iStatus != ESuspended, HTTPPanic::Panic(HTTPPanic::EInvalidEvent));				
		iStatus = ESuspended;		
		iSession.TransactionHasBlocked();
		RemoveEvent();
		return;
		}
	if(currentEvent == THTTPEvent::EResume)
		{		
		if(iStatus != ESuspended)
		HTTPPanic::Panic(HTTPPanic::EInvalidEvent);
		RemoveEvent();	
		iSession.Unblock();
		return;
		}	
	if (iSession.IsBlocking())
		{
		if(iStatus.Int() != ESuspended)
			{
			iStatus = EBlocked;
			iSession.TransactionHasBlocked();
			}				
		return;
		}	
	iSession.Block();
	TBool runNext = RunOneFilterL();	
	iSession.Unblock();	
	if (iStatus.Int() == EExiting)
		{
		iStatus = EPassive;
		delete this;
		}
	else if (runNext)
		{
		// Move on to the next filter and find out what to do next.
		Activate(FindNextFilter());
		}
	}



TInt CTransaction::RunError(TInt aError)
	{
	TInt error = KErrNone;
	RHTTPTransaction t;
	t.iImplementation = this;
	switch (iStatus.Int())
		{
	case EInFilter:
	case ECancelled:
		{
		// Get the right event, according to whether it's being sent synchronously or not
		THTTPEvent currentEvent;
		if (iProcessingSyncEvent==1)
			currentEvent = CurrentEvent().iEvent;
		else
			currentEvent = iASyncEvent;

		// Is it a session event
		TBool isSessionEvent = currentEvent.IsSessionEvent();

		// Now, offer to the appropriate handler - MHFRunError for transaction events
		// or MHFSessionRunError for session events
		if (iProcessingSyncEvent==1)
			{
			MHTTPFilter* filter = iSession.FilterQueue()[iNextFilter].iFilter;
			if (isSessionEvent)
				error = filter->MHFSessionRunError(aError, STATIC_CAST(THTTPSessionEvent&, currentEvent));
			else
				error = filter->MHFRunError(aError, t, currentEvent);
			}
		else
			{
			MHTTPFilter* filter = iSession.FilterQueue()[iCurrentASyncFilter].iFilter;
			if (isSessionEvent)
				error = filter->MHFSessionRunError(aError, STATIC_CAST(THTTPSessionEvent&, currentEvent));
			else
				error = filter->MHFRunError(aError, t, currentEvent);
			}

		// Unless we've been deleted, move on to the next filter and
		// find out what to do next.
		if (iStatus.Int() != EExiting)
			Activate(FindNextFilter());
		} break;
	case EExiting:
		break;
	default:
		__ASSERT_DEBUG(EFalse, HTTPPanic::Panic(HTTPPanic::ETransactionUnhandledError));
		}

	if (!iProcessingSyncEvent)
		iSession.Unblock();

	// Panic if MHFRunError hasn't handled the error
	if (error)
		HTTPPanic::Panic(HTTPPanic::ETransactionUnhandledError);

	if (iStatus.Int() == EExiting)
		{
		delete this;
		}

	return KErrNone;
	}

void CTransaction::DoCancel()
	{
	// Nothing to do here. This only happens when the transaction is
	// cancelled or deleted.
	}




	
#if defined (_DEBUG)

void CTransaction::LogEvent(TInt aTransactionId, const THTTPEvent& aEvent, 
						   TInt aDirection, const TDesC8& aFilterName)
	/* 	creates a single line comma separated list comment of
		Transaction Number, Event, Direction, Filter Name
	*/
	{
	_LIT(KSubmit,"Submit");
	_LIT(KCancel,"Cancel");
	_LIT(KNotifyNewRequestBodyPart,"NotifyNewRequestBodyPart");
	_LIT(KClosed,"Closed");
	_LIT(KGotResponseHeaders,"GotResponseHeaders");
	_LIT(KGotResponseBodyData,"GotResponseBodyData");
	_LIT(KResponseComplete, "ResponseComplete");
	_LIT(KGotResponseTrailerHeaders,"GotResponseTrailerHeaders");
	_LIT(KSucceeded,"Succeeded");
	_LIT(KFailed, "Failed");
	_LIT(KUnrecoverableError, "Unrecoverable Error");
	_LIT(KTooMuchRequestData, "TooMuchRequestData");
	_LIT(KRedirectRequiresConfirmation, "RedirectRequiresConfirmation");
	_LIT(KNeedTunnel, "NeedTunnel");
	_LIT(KGetCipherSuite, "GetCipherSuite");
	
	_LIT(KUnknownEvent, "UnknownEvent");

	TPtrC event(KNullDesC);
	
	switch (aEvent.iStatus)
		{
	case THTTPEvent::ESubmit:
		event.Set(KSubmit);
		break;
	case THTTPEvent::ECancel:
		event.Set(KCancel);
		break;
	case THTTPEvent::ENotifyNewRequestBodyPart:
		event.Set(KNotifyNewRequestBodyPart);
		break;
	case THTTPEvent::EClosed:
		event.Set(KClosed);
		break;
	case THTTPEvent::EGotResponseHeaders:
		event.Set(KGotResponseHeaders);
		break;
	case THTTPEvent::EGotResponseBodyData:
		event.Set(KGotResponseBodyData);
		break;
	case THTTPEvent::EResponseComplete:
		event.Set(KResponseComplete);
		break;
	case THTTPEvent::EGotResponseTrailerHeaders:
		event.Set(KGotResponseTrailerHeaders);
		break;
	case THTTPEvent::ESucceeded:
		event.Set(KSucceeded);
		break;
	case THTTPEvent::EFailed:
		event.Set(KFailed);
		break;
	case THTTPEvent::EUnrecoverableError:
		event.Set(KUnrecoverableError);
		break;
	case THTTPEvent::ETooMuchRequestData:
		event.Set(KTooMuchRequestData);
		break;
	case THTTPEvent::ERedirectRequiresConfirmation:
		event.Set(KRedirectRequiresConfirmation);
		break;
	case THTTPEvent::ENeedTunnel:
		event.Set(KNeedTunnel);
		break;
	case THTTPEvent::EGetCipherSuite:
		event.Set(KGetCipherSuite);
		break;
	default:
		event.Set(KUnknownEvent);
	};
	
	_LIT(KHandled, "Handled");	
	_LIT(KOutgoing,"Outgoing");
	_LIT(KIncoming,"Incoming");
	TPtrC direction(KHandled);

	switch (aDirection)
		{
	case THTTPEvent::EIncoming:
		direction.Set(KIncoming);
		break;
	case THTTPEvent::EOutgoing:
		direction.Set(KOutgoing);
		break;	
		};
	TBuf<64> filterName;
	filterName.Copy(aFilterName);	
	TBuf<256> logBuffer; // if its longer than 256 then http logging code will fall over anyway
	_LIT(KLogFormat, ", Transaction %d, %S, %S, %S");
	logBuffer.AppendFormat(KLogFormat, aTransactionId, &event, &direction, &filterName);
	__LOG(logBuffer);
	}

#endif



TBool CTransaction::RunOneFilterL()
	{
	// The active scheduler is only really used to split up filtering
	// in order to make things more interactive. By now, the next
	// filter to run should have been worked out, so run it and work
	// out what to do next time.
	RHTTPTransaction t;
	t.iImplementation = this;
	TBool processNext = EFalse;
	iCurrentFilter = iNextFilter;
	iStatus = EInFilter;
	__LOGEVENT(iId, CurrentEvent().iEvent,CurrentEvent().iDirection,
		iSession.Manager().StringPool().StringF(
		iSession.FilterQueue()[iNextFilter].iName).DesC());
	
	// Run it.

	MHTTPFilter* filter = iSession.FilterQueue()[iNextFilter].iFilter;
	THTTPEvent currentEvent = CurrentEvent().iEvent;
	//I need to keep a copy of the event and if the  event is asyncronous I need to keep the event 

	if(iProcessingSyncEvent==0 ) 
		{
		iASyncEvent= iEventQueue[0].iEvent;
		iCurrentASyncFilter=iNextFilter;
		}
	if (currentEvent.IsSessionEvent())
		{
		filter->MHFSessionRunL(STATIC_CAST(THTTPSessionEvent&, currentEvent));
		}
	else
		filter->MHFRunL(t, currentEvent);
	
	switch (iStatus.Int())
		{
	case EInFilter:
		processNext = ETrue;
		iStatus = EFilter;
		break;
	case ECancelled:
		iStatus = EPassive;
		processNext = ETrue;
		break;
	default:
		break;
		}
	return processNext;
	}
	
		
	

TInt CTransaction::AddEvent(THTTPEvent& aEvent, TInt aDirection,
							  THTTPFilterHandle aStart)
	{
	// API for adding new events to the queue.
	TInt startFilter = 0;
	// Look at the start filter handle to decide where to start the filter.
	switch (aStart.iValue)
		{
	case THTTPFilterHandle::EUndefined:
		HTTPPanic::Panic(HTTPPanic::EInvalidFilterHandle);
		break;
	case THTTPFilterHandle::EClient:
		startFilter = iSession.FilterQueue().Count() - 1;
		__LOGEVENT(iId,aEvent,aDirection,_L8("Client"));
		break;
	case THTTPFilterHandle::EProtocolHandler:
		startFilter = 0;
		__LOGEVENT(iId,aEvent,aDirection,_L8("Protocol Handler"));
		break;
	case THTTPFilterHandle::ECurrentFilter:
		if (iStatus.Int() != EInFilter && iStatus.Int() != ECancelled && iStatus.Int() != ESuspended)
			HTTPPanic::Panic(HTTPPanic::EInvalidFilterHandle);
		startFilter = iCurrentFilter;
		
		__LOGEVENT(iId,aEvent,aDirection,iSession.Manager().StringPool().StringF(
			iSession.FilterQueue()[iCurrentFilter].iName).DesC());
		break;
	default:
		// Assume any other value is a filter handle.
		{
		TInt i;
		TInt count = iSession.FilterQueue().Count();
		// If there aren't any filters, you shouldn't be specifying a
		// filter handle. Return an error.
		if (count == 0)
			HTTPPanic::Panic(HTTPPanic::EInvalidFilterHandle);
		for (i = 0; i < count; ++i)
			{
			if (iSession.FilterQueue()[i].iHandle == aStart.iValue)
				break;
			}
		if (i == count)
			{
			// We haven't found that filter. We may as well leave
			HTTPPanic::Panic(HTTPPanic::EInvalidFilterHandle);
			}
		startFilter = i;
		__LOGEVENT(iId,aEvent,aDirection,iSession.Manager().StringPool().
			  StringF(iSession.FilterQueue()[i].iName).DesC());
		break;
		}
		}

	TEventRegistration r(aEvent, aDirection, startFilter);
	return AppendEvent(r);
	}

TInt CTransaction::SendEvent(THTTPEvent& aEvent, TInt aDirection, THTTPFilterHandle aStart)
    {
    TInt err = AddEvent(aEvent, aDirection, aStart);
    if(err != KErrNone)
        {
        return err;
        }
        
    // If necessary, process the event having added it to the
    // queue. It's not necessary if we're inside a MHFRunL (in which
    // case the event will be processed when we return to
    // CTransaction::RunL) or if we've already added an event that
    // hasn't been run yet (in which case we'll be active)
    if (iStatus.Int() == EPassive)
        {
        Activate(FindNextFilter());
        }    
    return err;
    }

void CTransaction::SendEventL(THTTPEvent& aEvent, TInt aDirection,
							  THTTPFilterHandle aStart)
	{
	User::LeaveIfError(SendEvent(aEvent, aDirection, aStart));
	}


void CTransaction::SynchronousSendEvent(THTTPEvent aEvent, TInt aDirection, 
										 THTTPFilterHandle aStart)
	{
	// Remember the current async event, in case this is a nested async event.
	TEventRegistration storedSyncEvent = iCurrentSyncEvent;
	iProcessingSyncEvent = ETrue;

	// As we're in sync mode, this call must succeed.
	AddEvent(aEvent, aDirection, aStart);

	TInt s = iStatus.Int();
	// Remember the current filter so that events fired after a cancel
	// start from the right place.
	TInt rememberedCurrentFilter = iCurrentFilter;
	iStatus = EPassive;
	do 
		{
		iStatus = FindNextFilter();
		if (iStatus.Int() != EPassive)
			{
			TRAPD(error, RunOneFilterL());
			if (error)
				RunError(error);
			}
		}
	while (iStatus.Int() != EPassive && iStatus.Int() != EExiting);

	iCurrentFilter = rememberedCurrentFilter;
	if ( iStatus.Int() != EExiting )
		{
			iStatus = s;
		}
	iProcessingSyncEvent = EFalse;
	iCurrentSyncEvent = storedSyncEvent;
	}

CTransaction::TTransactionStates CTransaction::FindNextFilter()
	{
	if (!EventAvailable())
		return EPassive;
	NextFilter();
	if (iStatus.Int() == EPassive)
		{
		// Starts processing of the event on the front of the queue.
		// The event starts at the filter after the one that generated it,
		// in the direction it's going.
		iNextFilter = CurrentEvent().iStartFilter;
		NextFilter();
		}
	do
		{
		// See if there's another filter to process this event.
		iStatus = FindFilterForThisEvent();
		// If not, move on to the next event
		if (iStatus.Int() == EPassive && EventAvailable())
			{
			// Starts processing of the event on the front of the queue.
			// The event starts at the filter after the one that generated it,
			// in the direction it's going.
			iNextFilter = CurrentEvent().iStartFilter;
			NextFilter();
			// Having identified the first filter to be considered for this
			// event, identify the first filter that actualy matches the
			// event.
			}
		}
	while (iStatus.Int() == EPassive && EventAvailable());
	return static_cast<TTransactionStates>(iStatus.Int());
	}

CTransaction::TTransactionStates CTransaction::FindFilterForThisEvent()
	{
	// Identify the next filter to run.
	TBool found = EFalse;

	do
		{
		// If we've run out of filters, this event is finished.
		if (iNextFilter < 0 || 
			iNextFilter >= iSession.FilterQueue().Count())
			break;
		if (iNextFilter < iSession.FilterQueue().Count())
			{
			THTTPFilterRegistration& filter = 
				iSession.FilterQueue()[iNextFilter];
			// Is this filter interested in this event?
			RStringF hdr = 
				iSession.Manager().StringPool().StringF(filter.iHeader);

			// If a header has been specified in the filter, check if
			// it's present by searching for part 0 of that header.
			THTTPHdrVal headerValue;
			TBool hasHeader = hdr == RStringF() ||
				!iResponse->Headers().GetField(hdr,0,headerValue);

			// Does this filter match the event? That is to say:
			//  The event matches, or the filter matches any event and
			//  The filter matches any status code or the status matches and
			//  The filter matches any header or the header matches.

			// first check UID's
			if (CurrentEvent().iEvent.iUID == KHTTPMatchAnyEventUid || 
				filter.iEvent.iUID == KHTTPMatchAnyEventUid ||
				filter.iEvent.iUID == CurrentEvent().iEvent.iUID)
				{
				if (CurrentEvent().iEvent.IsSessionEvent())
					{
					if (filter.iEvent.iStatus == THTTPEvent::EAnySessionEvent ||
						filter.iEvent.iStatus == THTTPEvent::EAll ||
						filter.iEvent.iStatus == CurrentEvent().iEvent.iStatus)
						found = ETrue;
					}
				else if (
					(filter.iEvent.iStatus == CurrentEvent().iEvent.iStatus || 
						filter.iEvent.iStatus == THTTPEvent::EAnyTransactionEvent ||
						filter.iEvent.iStatus == THTTPEvent::EAll) &&
					(filter.iStatus == KAnyStatusCode || filter.iStatus == iResponse->Status()) &&
					(filter.iHeader == RStringF() || hasHeader))
						found = ETrue;
				}
			}

		if (!found)
			{
			// If not, move on to the next filter.
			NextFilter();
			}
		}while (!found);

	if (!found)
		{
		// We haven't found a filter interested in this message, so we
		// must have finised processing it. Move on to the next event
		// if there is one.
		RemoveEvent();
		if (EventAvailable())
			{
			iNextFilter = CurrentEvent().iStartFilter;
			NextFilter();
			}
		return EPassive;
		}
	else
		{
		// We have found a filter. 
		return EFilter;
		}
	}

// Activate and complete ourself.
void CTransaction::Activate(TTransactionStates aStatus)
	{
	if (aStatus == EPassive)
		{
		iStatus = aStatus;
		return;
		}
	SetActive();
	TRequestStatus* r = &iStatus;
	User::RequestComplete(r, aStatus);	
	}


// This function is not allowed to fail in async mode.
TInt CTransaction::AppendEvent(TEventRegistration& aEvent)
	{	
	if (iProcessingSyncEvent)
		{
		iCurrentSyncEvent = aEvent;
		return KErrNone;
		}
	else
		{
		if(aEvent.iEvent == THTTPEvent::EResume)
			{			
			TInt pos(0);
			return iEventQueue.Insert(aEvent,pos);
			}
		return iEventQueue.Append(aEvent);
		}
	}
	
RString CTransaction::CipherSuite()
	{
	RHTTPTransactionPropertySet properties(PropertySet());
	RStringPool strPool(Session().StringPool());
	THTTPHdrVal cipherSuiteValue;
	TBool foundCipherSuiteProperty = properties.Property(strPool.StringF(HTTP::ECipherSuiteValue,
														RHTTPSession::GetTable()), cipherSuiteValue);
						
	if(!foundCipherSuiteProperty)
		{
		// Send synchronous event down to Protocol Handler to ask it to query the connection for
		// the cipher suite used and store it as a property of the transaction.
		SynchronousSendEvent(THTTPEvent::EGetCipherSuite, THTTPEvent::EOutgoing, THTTPFilterHandle::EClient);
		foundCipherSuiteProperty = properties.Property(strPool.StringF(HTTP::ECipherSuiteValue,
														RHTTPSession::GetTable()), cipherSuiteValue);
		}																	
	
	// Make sure property is set to correct type.
	__ASSERT_DEBUG(foundCipherSuiteProperty && cipherSuiteValue.Type() == THTTPHdrVal::KStrVal, 
					HTTPPanic::Panic(HTTPPanic::EHeaderInvalidType));				
	return cipherSuiteValue.Str();
	}

void CTransaction::SetupHttpDataOptimiser (MHttpDataOptimiser& aHttpOptimiser)
 	{
 	iHttpDataOptimiser = &aHttpOptimiser;
 	} 

MHttpDataOptimiser* CTransaction::HttpDataOptimiser ()
 	{
 	return iHttpDataOptimiser;
 	}




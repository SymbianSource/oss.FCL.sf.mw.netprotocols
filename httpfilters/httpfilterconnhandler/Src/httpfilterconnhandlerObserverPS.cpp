/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Observer of System Agent events
*
*/



// INCLUDE FILES
#include <es_sock.h>
#include <PSVariables.h>  // Publish & Subscribe
#include <mmtsy_names.h>

// User Includes
#include "HttpFilterConnHandlerObserverPS.h"
#include "mconnectioncallback.h"
// #include <flogger.h>
//_LIT(KDir, "connFilter");
//_LIT(KFile, "filtererr.txt");

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES

// CONSTANTS
const TInt KMaxOutOfCoverageTime   = 60*1000000;  //60 sec - Updated to 60 secs as part of error fix JSIN-7JSE6H

// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// ============================= LOCAL FUNCTIONS ===============================


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CHttpFilterConnHandlerObserver::CHttpFilterConnHandlerObserver
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------

CHttpFilterConnHandlerObserver::CHttpFilterConnHandlerObserver(MConnectionCallback* aConnCallback):
        CActive(CActive::EPriorityHigh),iConnCallback(aConnCallback)
{
    iObserverState = EIdle;
    // hardcode iUidValue = KPSUidGprsStatusValue for GPRS
    // The iUidValue and iBearerType will be defined after connection is done
    iUidValue = 0;  // no default value, as it will be set on Network mode check
	iPrevUidValue = 0; // no default value, as it will be set on Network mode check
	iBearerType = EApBearerTypeGPRS; // it is hardcoded in Connection manager, and it does not depend on GPRS nor 3G/WCDMA

}

// -----------------------------------------------------------------------------
// CHttpFilterConnHandlerObserver::ConstructL
// ------------------------------------------------------------------------------
void CHttpFilterConnHandlerObserver::ConstructL()
{
    // Note: the CHttpFilterConnHandlerObserver object s.b. created after connection is set for particular bearer
    User::LeaveIfError(iGPRSStateProperty.Attach(KUidSystemCategory, KPSUidGprsStatusValue));
    User::LeaveIfError(iWCDMAStateProperty.Attach(KUidSystemCategory, KPSUidWcdmaStatusValue)); 

    CActiveScheduler::Add(this);
    iSuspendTimer = CPeriodic::NewL(CActive::EPriorityLow );
    if( !iTelServer.Handle() )	
        {
        // this initialization needed for identifying 3G networks
        //
        User::LeaveIfError( iTelServer.Connect() );
        User::LeaveIfError( iTelServer.LoadPhoneModule( KMmTsyModuleName ) );

        TInt numPhones;
        User::LeaveIfError( iTelServer.EnumeratePhones( numPhones ) );
        if( numPhones <= 0 )
            {
            // Huh???
            User::Leave( KErrCorrupt );
            }

        RTelServer::TPhoneInfo phoneInfo;
        User::LeaveIfError( iTelServer.GetPhoneInfo( 0, phoneInfo ) );
        User::LeaveIfError( iMobilePhone.Open( iTelServer, phoneInfo.iName ) );
        User::LeaveIfError( iMobilePhone.Initialise() );

		RMobilePhone::TMobilePhoneNetworkMode networkMode;
		User::LeaveIfError( iMobilePhone.GetCurrentMode( networkMode ) );

		if( networkMode == RMobilePhone::ENetworkModeWcdma ||
			networkMode == RMobilePhone::ENetworkModeTdcdma )
			{ // 3G/WCDMA mode
			iUidValue = KPSUidWcdmaStatusValue;
			}
		else if ( networkMode == RMobilePhone::ENetworkModeGsm ||
				  networkMode == RMobilePhone::ENetworkModeUnknown ||	
				  networkMode == RMobilePhone::ENetworkModeUnregistered )
			{
			iUidValue = KPSUidGprsStatusValue;
			}
		}

    // initial subscription and process current property value
    RunL();
}

// -----------------------------------------------------------------------------
// CHttpFilterConnHandlerObserver::NewL
//
// Creats an instance of the CHttpFilterConnHandlerObserver
// ------------------------------------------------------------------------------
CHttpFilterConnHandlerObserver* CHttpFilterConnHandlerObserver::NewL(MConnectionCallback* aConnCallback)
{
    CHttpFilterConnHandlerObserver* self = new( ELeave ) CHttpFilterConnHandlerObserver(aConnCallback);
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
}

// -----------------------------------------------------------------------------
// CHttpFilterConnHandlerObserver::~CHttpFilterConnHandlerObserver
//
// Destructor
// ------------------------------------------------------------------------------
CHttpFilterConnHandlerObserver::~CHttpFilterConnHandlerObserver()
{
    Cancel();
    iGPRSStateProperty.Close();
    iWCDMAStateProperty.Close();
    iMobilePhone.Close();
    iTelServer.Close();
    if ( iSuspendTimer )
        iSuspendTimer->Cancel();
    delete iSuspendTimer;
}

// -----------------------------------------------------------------------------
// CHttpFilterConnHandlerObserver::DoCancel
//
// Cancellation of notify.
// ------------------------------------------------------------------------------
void CHttpFilterConnHandlerObserver::DoCancel()
{
	if (iPrevUidValue == KPSUidGprsStatusValue)
		iGPRSStateProperty.Cancel();
	else if (iPrevUidValue == KPSUidWcdmaStatusValue)
		iWCDMAStateProperty.Cancel();
}

// -----------------------------------------------------------------------------
// CHttpFilterConnHandlerObserver::RunL
//
// This method will be called when the iGPRSStateProperty or iWCDMAStateProperty is updated
// ------------------------------------------------------------------------------
void CHttpFilterConnHandlerObserver::RunL()
{
    TInt state;
	TInt err = KErrNone;
    // resubscribe before processing new value

	RMobilePhone::TMobilePhoneNetworkMode networkMode;
	User::LeaveIfError( iMobilePhone.GetCurrentMode( networkMode ) );

	if( networkMode == RMobilePhone::ENetworkModeWcdma ||
		networkMode == RMobilePhone::ENetworkModeTdcdma )
		{ 
		iUidValue = KPSUidWcdmaStatusValue;
		// Subscribing to 3G/WCDMA status events
		iWCDMAStateProperty.Subscribe(iStatus);

		SetActive();

		if (iPrevUidValue == KPSUidGprsStatusValue)
			{
			err = iGPRSStateProperty.Get(state);
			}
		else
			{
			err = iWCDMAStateProperty.Get(state);
			}
		iPrevUidValue = KPSUidWcdmaStatusValue;
		}
	else if ( networkMode == RMobilePhone::ENetworkModeGsm ||
			  networkMode == RMobilePhone::ENetworkModeUnknown ||	
			  networkMode == RMobilePhone::ENetworkModeUnregistered )
		{
		iUidValue = KPSUidGprsStatusValue;
		// Subscribing to 2G/GPRS status events
		iGPRSStateProperty.Subscribe(iStatus);

		SetActive();

		if (iPrevUidValue == KPSUidWcdmaStatusValue)
			{
			err = iWCDMAStateProperty.Get(state);
			}
		else
			{
			err = iGPRSStateProperty.Get(state);
			}
		iPrevUidValue = KPSUidGprsStatusValue;
		}


    if(err == KErrNone)
    {
      HandleStatePropertyL(state);      
    }
}

// -----------------------------------------------------------------------------
// CHttpFilterConnHandlerObserver::GetStateL
//
// Get state property
// ------------------------------------------------------------------------------
void CHttpFilterConnHandlerObserver::GetStateL(TInt iUidValue, TInt& aState)
{
    TInt ret = KErrNone;
    switch( iUidValue )
        {
        case KPSUidGprsStatusValue:
            {                
            ret = iGPRSStateProperty.Get(KUidSystemCategory, iUidValue, aState);
            }         
            break;

        case KPSUidWcdmaStatusValue:
            {                
            ret = iWCDMAStateProperty.Get(KUidSystemCategory, iUidValue, aState);
            }    
            break;

        default:
	        break;
        }
    if ( ret != KErrNotFound && ret != KErrNone )
        {
        User::Leave( ret );
        }
}

// -----------------------------------------------------------------------------
// CHttpFilterConnHandlerObserver::SetObserverState
//
// Set iObserverState data member (observer state)
// ------------------------------------------------------------------------------
void CHttpFilterConnHandlerObserver::SetObserverState(TInt aState)
{
    iObserverState = aState;
}

// -----------------------------------------------------------------------------
// CHttpFilterConnHandlerObserver::SetBearerTypeAndUid
//
// Set iBearerType
// ------------------------------------------------------------------------------
void CHttpFilterConnHandlerObserver::SetBearerTypeAndUid(TApBearerType aBearerType)
{
	TInt state;
    iBearerType = aBearerType;
    // set uid according to iBearerType
    switch( iBearerType )
    {
      case EApBearerTypeGPRS:
        {
		RMobilePhone::TMobilePhoneNetworkMode networkMode;
		User::LeaveIfError( iMobilePhone.GetCurrentMode( networkMode ) );

		if( networkMode == RMobilePhone::ENetworkModeWcdma ||
			networkMode == RMobilePhone::ENetworkModeTdcdma )
			{
			User::LeaveIfError(iWCDMAStateProperty.Get(KUidSystemCategory, KPSUidWcdmaStatusValue, state));
			iUidValue = KPSUidWcdmaStatusValue;
			}
		else if ( networkMode == RMobilePhone::ENetworkModeGsm ||
				  networkMode == RMobilePhone::ENetworkModeUnknown ||	
				  networkMode == RMobilePhone::ENetworkModeUnregistered )
			{
			User::LeaveIfError(iGPRSStateProperty.Get(KUidSystemCategory, KPSUidGprsStatusValue, state));
			iUidValue = KPSUidGprsStatusValue;
			}
        }         
        break;

      case EApBearerTypeCDMA:
        {                
           iUidValue = KPSUidWcdmaStatusValue;
        }    
        break;

	default:
		break;
    }
}


// -----------------------------------------------------------------------------
// CHttpFilterConnHandlerObserver::GetObserverState
//
// Set iRequested data member
// ------------------------------------------------------------------------------
void CHttpFilterConnHandlerObserver::GetObserverState(TInt& aState)
{
    aState = iObserverState;
}

// -----------------------------------------------------------------------------
// CHttpFilterConnHandlerObserver::HandleStatePropertyL
//
// This method will be called when the StateProperty will be updated
// In case of GPRS:
//	EPSGprsUnattached,
//	EPSGprsAttach,
//	EPSGprsContextActive,
//	EPSGprsSuspend,
//	EPSGprsContextActivating,
//	EPSGprsMultibleContextActive
// Handle changed stateProperty
// ------------------------------------------------------------------------------
void CHttpFilterConnHandlerObserver::HandleStatePropertyL(TInt aState )
{
//    RFileLogger::WriteFormat(KDir, KFile, EFileLoggingModeAppend, _L8("GPRS %d"), aState);
//    RFileLogger::WriteFormat(KDir, KFile, EFileLoggingModeAppend, _L8("Obs state %d"), iObserverState);
    // handle GPRS bearer type
     if (iUidValue == KPSUidGprsStatusValue || iUidValue == KPSUidWcdmaStatusValue)
     {
       switch (aState)
        {
        case EPSGprsSuspend: // including case EPSWcdmaSuspend: 

            if (iObserverState == EActiveReady)
            {
                // start the timer. If the ESAGprsContextActive event will not occure
                // before the timer expired the
                // current transaction will be canceled and the user will be notified.
                iSuspendTimer->Cancel();
                iSuspendTimer->Start(
                    KMaxOutOfCoverageTime,
                    0,  // No interval
                    TCallBack( TimerSuspendCallback, this ) );

                iObserverState = ESuspended;
            }
            break;
			
        case EPSGprsUnattached: // including case EPSWcdmaUnattached:
            if (iObserverState == ESuspended)
            {
                iSuspendTimer->Cancel();
                iConnCallback->CoverageEvent(EErrWaitTimeout);
                iObserverState = EIdle;
            }
            break;

        case EPSGprsContextActive: // case EPSWcdmaContextActive:
        case EPSGprsMultibleContextActive: // case EPSWcdmaMultipleContextActive:
            if (iObserverState == ESuspended)
            {
                // Signal came back, so just stop a timer
                iSuspendTimer->Cancel();
                iObserverState = EActiveReady;
            }
            break;

        default:
            break;

        } //switch
	 }
}

// -----------------------------------------------------------------------------
// CHttpFilterConnHandlerObserver::DoTimerSuspend
// callback function for the TimerSuspended
// -----------------------------------------------------------------------------
//
void CHttpFilterConnHandlerObserver::DoTimerSuspend()
{
    iSuspendTimer->Cancel();

    TInt gprsState = EPSGprsSuspend;
    TInt wcdmaState = EPSWcdmaSuspend;
    
    iGPRSStateProperty.Get(KUidSystemCategory, KPSUidGprsStatusValue, gprsState);
    iWCDMAStateProperty.Get(KUidSystemCategory, KPSUidWcdmaStatusValue, wcdmaState);

    if (gprsState == EPSGprsSuspend && wcdmaState == EPSWcdmaSuspend)
       {
       // Note that this will be run also if there was an error when getting Gprs/Wcdma status
       // because they were initialized to 'suspend'
		if (iObserverState == ESuspended)
			{
			iObserverState = EIdle;
			iConnCallback->CoverageEvent(EErrWaitTimeout);
	//        RFileLogger::WriteFormat(KDir, KFile, EFileLoggingModeAppend, _L8("CTimer:Event"));
			}
		}
	else
       {
       iObserverState = EActiveReady;         
       }
}

// -----------------------------------------------------------------------------
// CHttpFilterConnHandlerObserver::TimerSuspendCallback
// callback function for the TimerSuspended
// -----------------------------------------------------------------------------
//
TInt CHttpFilterConnHandlerObserver::TimerSuspendCallback(TAny* aAny )
{
    ((CHttpFilterConnHandlerObserver*)aAny)->DoTimerSuspend();
    return KErrNone;
}

//  End of File


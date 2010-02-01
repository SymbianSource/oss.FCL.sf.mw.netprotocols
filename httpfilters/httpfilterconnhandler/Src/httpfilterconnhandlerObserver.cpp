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
#include <syevdef.h>

// User Includes
#include "httpfilterConnHandlerObserver.h"
#include "mconnectioncallback.h"
#include <flogger.h>
_LIT(KDir, "connFilter");
_LIT(KFile, "filtererr.txt");

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
    CActiveScheduler::Add(this);
    iState = EIdle;
}

// -----------------------------------------------------------------------------
// CHttpFilterConnHandlerObserver::ConstructL
// ------------------------------------------------------------------------------
void CHttpFilterConnHandlerObserver::ConstructL()
{
    iSuspendTimer = CPeriodic::NewL(CActive::EPriorityLow );
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
    iSysAgent.Close();
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
    iSysAgent.NotifyEventCancel();
}

// -----------------------------------------------------------------------------
// CHttpFilterConnHandlerObserver::RunL
//
// This method will be called when registred System Agent event occures
// ------------------------------------------------------------------------------
void CHttpFilterConnHandlerObserver::RunL()
{
    TInt state = iSysAgentEvent.State();
    TUid uid   = iSysAgentEvent.Uid();

    if(iStatus.Int() == KErrNone)
    {
        IssueRequest();
        HandleSystemAgentEventL( uid, state );
    }
}

// -----------------------------------------------------------------------------
// CHttpFilterConnHandlerObserver::IssueRequest
//
// Issue notify request to the System agent.
// ------------------------------------------------------------------------------
void CHttpFilterConnHandlerObserver::IssueRequest()
{
    iSysAgent.NotifyOnEvent(iSysAgentEvent);
    SetActive();
}

// -----------------------------------------------------------------------------
// CHttpFilterConnHandlerObserver::SetRequestStatus
//
// Sets the object's request status to refer to the specified request status object
// ------------------------------------------------------------------------------
void CHttpFilterConnHandlerObserver::SetRequestStatus()
{
    iSysAgentEvent.SetRequestStatus(iStatus);
}

// -----------------------------------------------------------------------------
// CHttpFilterConnHandlerObserver::SysAgentConnectL
//
// Connect to System Agent
// ------------------------------------------------------------------------------
void CHttpFilterConnHandlerObserver::SysAgentConnectL()
{
    User::LeaveIfError(iSysAgent.Connect());
}

// -----------------------------------------------------------------------------
// CHttpFilterConnHandlerObserver::SysAgentSetUid
//
// Set System Agent Uid
// ------------------------------------------------------------------------------
void CHttpFilterConnHandlerObserver::SysAgentSetUid(TUid aUid)
{
    iSysAgentEvent.SetUid(aUid);
}

// -----------------------------------------------------------------------------
// CHttpFilterConnHandlerObserver::SysAgentGetState
//
// Get state
// ------------------------------------------------------------------------------
void CHttpFilterConnHandlerObserver::SysAgentGetState(TUid aUid, TInt& aState)
{
    TInt state = iSysAgent.GetState(aUid);
    aState = state;
}
// -----------------------------------------------------------------------------
// CHttpFilterConnHandlerObserver::SetState
//
// Set iRequested data member
// ------------------------------------------------------------------------------
void CHttpFilterConnHandlerObserver::SetState(TInt aState)
{
    iState = aState;
}

// -----------------------------------------------------------------------------
// CHttpFilterConnHandlerObserver::GetState
//
// Set iRequested data member
// ------------------------------------------------------------------------------
void CHttpFilterConnHandlerObserver::GetState(TInt& aState)
{
    aState = iState;
}

// -----------------------------------------------------------------------------
// CHttpFilterConnHandlerObserver::HandleSystemAgentEventL
//
// This method will be called when registred System Agent event occures
// In case of GPRS:
//	ESAGprsUnattached,
//	ESAGprsAttach,
//	ESAGprsContextActive,
//	ESAGprsSuspend,
//	ESAGprsContextActivating,
//	ESAGprsMultibleContextActive
// Handle System Agent events
// ------------------------------------------------------------------------------
void CHttpFilterConnHandlerObserver::HandleSystemAgentEventL( TUid aUid, TInt aState )
{
    RFileLogger::WriteFormat(KDir, KFile, EFileLoggingModeAppend, _L8("GPRS %d"), aState);
    RFileLogger::WriteFormat(KDir, KFile, EFileLoggingModeAppend, _L8("Obs state %d"), iState);
    // Request should be issued in order to get notification for the next event

    // handle GPRS bearer type
    if (aUid == KUidGprsStatus)
    {
        switch (aState)
        {
        case ESAGprsSuspend:
            if (iState == EActiveReady)
            {
                // start the timer. If the ESAGprsContextActive event will not occure
                // before the timer expired the
                // current transaction will be canceled and the user will be notified.
                iSuspendTimer->Cancel();
                iSuspendTimer->Start(
                    KMaxOutOfCoverageTime,
                    0,  // No interval
                    TCallBack( TimerSuspendCallback, this ) );

                iState = ESuspended;
            }
            break;

        case ESAGprsUnattached:
            if (iState == ESuspended)
            {
                iSuspendTimer->Cancel();
                iConnCallback->CoverageEvent(EErrWaitTimeout);
                iState = EIdle;
            }
            break;

        case ESAGprsContextActive:
            if (iState == ESuspended)
            {
                // Signal came back, so just stop a timer
                iSuspendTimer->Cancel();
                iState = EActiveReady;
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
    if (iState == ESuspended)
    {
        iState = EIdle;
        iConnCallback->CoverageEvent(EErrWaitTimeout);
        RFileLogger::WriteFormat(KDir, KFile, EFileLoggingModeAppend, _L8("CTimer:Event"));
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


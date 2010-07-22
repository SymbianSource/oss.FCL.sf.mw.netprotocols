// Copyright (c) 2003-2010 Nokia Corporation and/or its subsidiary(-ies).
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
#include <commdbconnpref.h>
#include <es_sock.h>
#include "CConnectionMonitor.h"
#include "chttpnetworkconnectioninfo.h"

CConnectionMonitor* CConnectionMonitor::NewL(CHttpNetworkConnection* aNetworkConn )
    {
    CConnectionMonitor* self = NewLC( aNetworkConn );
    CleanupStack::Pop( self );
    return self;
    }

CConnectionMonitor* CConnectionMonitor::NewLC( CHttpNetworkConnection* aNetworkConn )
    {
    CConnectionMonitor* self = new (ELeave) CConnectionMonitor( aNetworkConn );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

CConnectionMonitor::CConnectionMonitor( CHttpNetworkConnection* aNetworkConnInfo )
: CActive(CActive::EPriorityStandard)
    {
    iNetworkConnInfo = aNetworkConnInfo;
    CActiveScheduler::Add(this);        
    }

CConnectionMonitor::~CConnectionMonitor()
    {
    Cancel();
    }

RConnection CConnectionMonitor::Connection() 
    {
    return iNetworkConnInfo->Connection();
    }
    
void CConnectionMonitor::ConstructL()
    {
    iNetworkConnInfo->Connection().ProgressNotification(iProgress, iStatus);
    SetActive();
    }

void CConnectionMonitor::DoCancel()
    {
    iNetworkConnInfo->Connection().CancelProgressNotification();
    }

void CConnectionMonitor::RunL()
    {
  
    
    switch ( iProgress().iStage )
        {
        
        // Connection closed
        case KConnectionClosed:
        case KLinkLayerClosed:
            if (iNetworkConnInfo)
                {
                iNetworkConnInfo->HandleConnectionEvent( iProgress().iStage );
                }
            break;

        case KConnectionUninitialised:
            break;

        case KStartingSelection:
            break;

        case KFinishedSelection:
            if (iProgress().iError == KErrNone)
                {
                // The user successfully selected an IAP to be used
                }
            else
                {
                // The user pressed e.g. "Cancel" and did not select an IAP
                }
            break;

        case KConnectionFailure:
            break;

        // Prepearing connection (e.g. dialing)
        case KPsdStartingConfiguration:
        case KPsdFinishedConfiguration:
        case KCsdFinishedDialling:
        case KCsdScanningScript:
        case KCsdGettingLoginInfo:
        case KCsdGotLoginInfo:
            break;
        
        // Creating connection (e.g. GPRS activation)
        case KCsdStartingConnect:
        case KCsdFinishedConnect:
            break;

        // Starting log in
        case KCsdStartingLogIn:
            break;

        // Finished login
        case KCsdFinishedLogIn:
            break;

        // Connection open
        case KConnectionOpen:
        case KLinkLayerOpen:
            break;
            
        // Connection blocked or suspended
        case KDataTransferTemporarilyBlocked:
            break;

        // Hangup or GRPS deactivation
        case KConnectionStartingClose:
            break;

        // Unhandled state
        default:
            break;
        }

    Connection().ProgressNotification(iProgress, iStatus);
    SetActive();
    }

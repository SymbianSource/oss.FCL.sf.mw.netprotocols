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
#include  <httpservice.h>
#include <es_enum.h>
#include "CConnectionMonitor.h"


CHttpNetworkConnection::CHttpNetworkConnection()
    {
    }

CHttpNetworkConnection::~CHttpNetworkConnection()
    {
    if (iConnInfo)
        {
        iConnInfo->iConnection.Close();
        iConnInfo->iSocketServ.Close();
        delete iConnInfo;
        }
    if (iConnectionMonitor)
        {
        delete iConnectionMonitor;
        }
    }
CHttpNetworkConnection* CHttpNetworkConnection::New( )
    {
    CHttpNetworkConnection* self = new (ELeave)  CHttpNetworkConnection() ;
    self->ConstructL();
    return self;
    }

void CHttpNetworkConnection::ConstructL()
    {
    iConnInfo = new (ELeave) CConnectionInfo();
    }

void CHttpNetworkConnection::SetHttpService(CHttpService *aService)
    {
    iHttpService = aService;
    }

/**
 * Start the network connection for  HTTP transactions.
 * Default connection prefernces will be used.
 * 
 * @return KErrNone if connection opening is successful, otherwise relevant errors.
 */
EXPORT_C TInt CHttpNetworkConnection::Start()
    {
    TRequestStatus requestStatus;
    TInt status;
    if (! iConnInfo->iConnectionStatus)
        {
        status = SetupConnection();
        if (status == KErrNone)
            {
            iConnInfo->iConnection.Start( requestStatus);
            User::WaitForRequest(requestStatus);
            status = requestStatus.Int();
            }
        }
    else
        {
        status = KErrAlreadyExists;
        }
    if (status == KErrNone)
        {
        AssociateConnectionWithHttpSession();
        }
    return status;
    }

/**
 * Start the network connection for  HTTP transactions.
 * 
 * @param aConnPref - connection prefernces can be specified.
 * @param aAttachType - EFalse 
 * 
 * @return KErrNoMemory incase of failure otherwise KErrNone for success
 * 
 */
EXPORT_C TInt CHttpNetworkConnection::Start(TCommDbConnPref aConnPref, TBool aAttachType)
    {
    TInt status;
    iConnInfo->iConnPref = aConnPref;

    if (aAttachType && !iConnInfo->iConnectionStatus )
        {
        TUint count;
        TConnectionInfoBuf connInfoPckg;
        
        status = SetupConnection();
                
        if ( status == KErrNone && iConnInfo->iConnection.EnumerateConnections(count) == KErrNone )
            {
            for (TUint i=1; i<=count; i++)
                {
                // Note  GetConnectionInfo expects 1-based index
                if ( iConnInfo->iConnection.GetConnectionInfo( i, connInfoPckg ) == KErrNone )
                    {
                    if ( connInfoPckg().iIapId == iConnInfo->iConnPref.IapId() )
                        {
                        status= iConnInfo->iConnection.Attach(iConnInfo->iConnPref, RConnection::EAttachTypeNormal);
                        return status;
                        }
                    }   
                }
            }
        return KErrNotFound;
        }
    else
        {
        if (!iConnInfo->iConnectionStatus)
            {
            status = SetupConnection();
            if (status == KErrNone)
                {
                TRequestStatus requestStatus;
                iConnInfo->iConnection.Start(iConnInfo->iConnPref,requestStatus);
                User::WaitForRequest(requestStatus);
                status = requestStatus.Int();
                }
          
            }
        else
            {
            status = KErrAlreadyExists;
            }
        if (status == KErrNone)
            {
            AssociateConnectionWithHttpSession();
            }
        
        return status;
        }
    }


/**
 * To Stop the network connection 
 * 
  */
EXPORT_C void CHttpNetworkConnection::Stop()
    {
    if (iConnInfo->iConnectionStatus)
        {
        iConnInfo->iConnectionStatus = EFalse;
        iConnInfo->iConnection.Close();
        iConnInfo->iSocketServ.Close();
        }
    }

void CHttpNetworkConnection::AssociateConnectionWithHttpSession()
    {
    RStringPool strP = iHttpService->Session().StringPool();
    RHTTPConnectionInfo connInfo = iHttpService->Session().ConnectionInfo();
    connInfo.SetProperty(strP.StringF(HTTP::EHttpSocketServ,RHTTPSession::GetTable()), THTTPHdrVal(iConnInfo->iSocketServ.Handle()));
    TInt connPtr = reinterpret_cast<TInt>(&(iConnInfo->iConnection));
    connInfo.SetProperty(strP.StringF(HTTP::EHttpSocketConnection,RHTTPSession::GetTable()), THTTPHdrVal(connPtr));
    }

TInt CHttpNetworkConnection::SetupConnection()
    {
    TInt status = KErrAlreadyExists;
    if (!iConnInfo->iConnectionStatus )
        {
        status = iConnInfo->iSocketServ.Connect() ;
        if (status == KErrNone)
            {
            status = iConnInfo->iConnection.Open ( iConnInfo->iSocketServ ) ;
            if (status != KErrNone)
                {
                iConnInfo->iSocketServ.Close();
                }
            else
                {
                iConnectionMonitor = CConnectionMonitor::NewL(this); 
                iConnInfo->iConnectionStatus = ETrue;
                }
            }
        }
    return status;
    }

void CHttpNetworkConnection::HandleConnectionEvent(TInt aError)
    {
    if (aError == KLinkLayerClosed || aError == KLinkLayerClosed)
        {
        iConnInfo->iConnectionStatus = EFalse;
        }
    }
    

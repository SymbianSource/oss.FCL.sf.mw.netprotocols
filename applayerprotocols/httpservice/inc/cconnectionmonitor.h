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
#ifndef CCONNECTIONMONITOR_H_
#define CCONNECTIONMONITOR_H_
class CHttpNetworkConnection;

NONSHARABLE_CLASS ( CConnectionMonitor ): public CActive
    {
    friend class CHttpNetworkConnection;
    public:
        static CConnectionMonitor* NewL( CHttpNetworkConnection* aNetworkConn );
        static CConnectionMonitor* NewLC(CHttpNetworkConnection* aNetworkConn );
        ~CConnectionMonitor();
        void HandleConnectionEvent(TInt aEvent);


    protected: // from CActive
        void DoCancel();
        void RunL();
    
    private:
        CConnectionMonitor( CHttpNetworkConnection* aNetworkConn );
        void ConstructL();
        RConnection Connection() ;
         
    private:
        TNifProgressBuf iProgress;
        CHttpNetworkConnection* iNetworkConnInfo;
    
    
    };

#endif /* CCONNECTIONMONITOR_H_ */

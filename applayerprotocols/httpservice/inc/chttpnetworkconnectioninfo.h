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

#ifndef CHTTPNETWORKCONNECTIONINFO_H_
#define CHTTPNETWORKCONNECTIONINFO_H_

#include <commdbconnpref.h>
#include <es_sock.h>
#include "chttpservice.h"


class CConnectionMonitor;

class CHttpNetworkConnection: public CBase
    {
    friend class CHttpClientTransactionImpl;    
    friend class CConnectionMonitor;
    friend class CHttpService;
    
    public:
        static CHttpNetworkConnection* New();
        IMPORT_C TInt Start();//start on default connection
        IMPORT_C TInt Start(TCommDbConnPref aConnPref, TBool aAttachType=EFalse);

        IMPORT_C void Stop();
        


    private:
        TInt SetupConnection();
        void AssociateConnectionWithHttpSession();
        void HandleConnectionEvent(TInt aEvent); //Called whenever there is a connection event
        void SetHttpService(CHttpService* aService);
        RConnection Connection()
            {
            return iConnInfo->iConnection;
            }
        CHttpNetworkConnection();
        void ConstructL();
    
        ~CHttpNetworkConnection();

    
    private:
        class CConnectionInfo: public CBase
            {
            public:
            TBool           iConnectionStatus;
            TBool           iAttach;
            TCommDbConnPref iConnPref;
            RConnection     iConnection;
            RSocketServ     iSocketServ;
            };
        CConnectionMonitor* iConnectionMonitor;
        CConnectionInfo*    iConnInfo;
        CHttpService*       iHttpService;
    };

#endif /* CHTTPNETWORKCONNECTIONINFO_H_ */

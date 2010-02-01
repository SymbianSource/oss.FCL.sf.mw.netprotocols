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



#ifndef CHTTPFILTERCONNHANDLEROBSERVER_H
#define CHTTPFILTERCONNHANDLEROBSERVER_H

//  INCLUDES
#include <e32property.h> 
#include <etel.h>
#include <etelmm.h>
#include <ApEngineConsts.h>          
class MConnectionCallback;

// CONSTANTS

// MACROS

// DATA TYPES

enum TObserverState
{
    EIdle = 1,         // browser is not active or suspend timer has been expired
    EActiveReady,      // transaction request has been sent & GPRS is in READY state 
    ESuspended         // GPRS suspended in aftre request has been sent
};

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION

class CHttpFilterConnHandlerObserver : public CActive
{
public:  // Constructors and destructor

    //
    // Two-phased constructor.
    //
    static CHttpFilterConnHandlerObserver* NewL(MConnectionCallback* aConnCallback);

    //
    //Destructor.
    //
    ~CHttpFilterConnHandlerObserver();


public: // Functions from base classes

    void DoCancel();
    void RunL();

public:

    // Get state property
    void GetStateL(TInt iUidValue, TInt& aState);

    // Set observer state data member
    void SetObserverState(TInt aState);

    // Get observer state data member
    void GetObserverState(TInt& aState);

    // Set iBearerType and UidValue
    void SetBearerTypeAndUid(TApBearerType aBearerType);

    // Handle state property
    void HandleStatePropertyL(TInt aState );

protected:  // Functions from base classes

private:

    //C++ default constructor.
    CHttpFilterConnHandlerObserver(MConnectionCallback* aConnCallback);

    // By default Symbian 2nd phase constructor is private.
    void ConstructL();

    // callback function for the TimerSuspended
    void DoTimerSuspend();

    // callback function for the TimerSuspended
    static TInt TimerSuspendCallback(TAny* aPtr);

public:     // Data

protected:  // Data

private:    // Data

    // Current observer state, see TObserverState definition.
    TInt iObserverState;
    // property
    RProperty iGPRSStateProperty;
    RProperty iWCDMAStateProperty;

    // bearerType
    TApBearerType iBearerType;
    // Uid
    TInt iUidValue;
	TInt iPrevUidValue;
    // The ConnectionCallback
    MConnectionCallback*     iConnCallback;
    // CSuspendTimer object
    CPeriodic*     iSuspendTimer;

    RTelServer  iTelServer;
    RMobilePhone    iMobilePhone;

public:     // Friend classes

protected:  // Friend classes

private:    // Friend classes

};

#endif      // CHTTPFILTERCONNHANDLEROBSERVER_H

// End of File

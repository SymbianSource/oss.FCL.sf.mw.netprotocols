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
#include <saclient.h>           // for system agent connection

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

    // Issue notify request to the System agent
    void IssueRequest();

    // Sets the object's request status to refer to the specified request status object
    void SetRequestStatus();

    // Connect to System Agent
    void SysAgentConnectL();

    // Set System Agent Uid
    void SysAgentSetUid(const TUid aUid);

    // Get System Agent state
    void SysAgentGetState(const TUid aUid, TInt& aState);

    // Set iRequested data member
    void SetState(TInt aState);

    // Get iRequested data member
    void GetState(TInt& aState);

    // Handle system Agent Event
    void HandleSystemAgentEventL( TUid aUid, TInt aState );

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
    TInt iState;
    // Owned system agent session.
    RSystemAgent iSysAgent;
    // System agent event.
    TSysAgentEvent iSysAgentEvent;
    // The ConnectionCallback
    MConnectionCallback*     iConnCallback;
    // CSuspendTimer object
    CPeriodic*     iSuspendTimer;
public:     // Friend classes

protected:  // Friend classes

private:    // Friend classes

};

#endif      // CHTTPFILTERCONNHANDLEROBSERVER_H

// End of File

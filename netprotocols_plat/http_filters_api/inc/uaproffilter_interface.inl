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
* Description:  Inline methods for class CHttpUAProfFilterInterface.
*
*
*/


#ifndef HTTP_UAPROF_FILTER_INTERFACE_INL
#define HTTP_UAPROF_FILTER_INTERFACE_INL

// ================= MEMBER FUNCTIONS ==========================================

// -----------------------------------------------------------------------------
// CHttpUAProfFilterInterface::CHttpUAProfFilterInterface()
// -----------------------------------------------------------------------------
//
CHttpUAProfFilterInterface::CHttpUAProfFilterInterface()
: CBase()
    {
    }

// -----------------------------------------------------------------------------
// CHttpUAProfFilterInterface::InstallFilterL()
// -----------------------------------------------------------------------------
//
void CHttpUAProfFilterInterface::InstallFilterL( RHTTPSession& aSession )
    {
    const TUid KUidHttpUAProfFilter = { 0x10200A8A };
    REComSession::CreateImplementationL( KUidHttpUAProfFilter, 
    _FOFF( CHttpUAProfFilterInterface, iEcomDtorID ), &aSession );
    }

// -----------------------------------------------------------------------------
// CHttpUAProfFilterInterface::~CHttpUAProfFilterInterface()
// -----------------------------------------------------------------------------
//
CHttpUAProfFilterInterface::~CHttpUAProfFilterInterface()
    {
    REComSession::DestroyedImplementation( iEcomDtorID );
    }

#endif // HTTP_UAPROF_FILTER_INTERFACE_INL

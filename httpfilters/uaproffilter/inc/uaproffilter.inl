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
* Description:  Inline methods for class CHttpUAProfFilter.
*
*
*/


#ifndef HTTP_UAPROF_FILTER_INL
#define HTTP_UAPROF_FILTER_INL

// ================= MEMBER FUNCTIONS ==========================================

// -----------------------------------------------------------------------------
// CHttpUAProfFilter::StringF()
// -----------------------------------------------------------------------------
//
RStringF CHttpUAProfFilter::StringF( HTTP::TStrings aId )
    {
    return iStringPool.StringF( aId, iStringTable );
    }

#endif // HTTP_UAPROF_FILTER_INL

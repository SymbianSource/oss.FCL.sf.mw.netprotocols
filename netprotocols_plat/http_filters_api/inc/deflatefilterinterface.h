/*
* Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  
*
*/


#ifndef	__DEFLATEFILTERINTERFACE_H__
#define	__DEFLATEFILTERINTERFACE_H__

// INCLUDE FILES
	// System includes
#include <e32base.h>
#include <ecom/ecom.h>
#include <http/rhttpsession.h>

// CLASS DECLARATION

class CHttpDeflateFilter : public CBase
	{
public: 
	inline static void InstallFilterL( RHTTPSession& aSession );

	inline ~CHttpDeflateFilter();

private: 

	TUid iEcomDtorID;
	};

//------------------------------------------------------------------------

const TUid KDeflateFilterUid = { 0x101F9713 };


inline void CHttpDeflateFilter::InstallFilterL( RHTTPSession& aSession )
	{
	REComSession::CreateImplementationL( KDeflateFilterUid,
									_FOFF( CHttpDeflateFilter, iEcomDtorID ),
									&aSession );
	}

/**
	Destructor
*/
inline CHttpDeflateFilter::~CHttpDeflateFilter()
	{
	REComSession::DestroyedImplementation(iEcomDtorID);
	}

#endif	// __DEFLATEFILTERINTERFACE_H__
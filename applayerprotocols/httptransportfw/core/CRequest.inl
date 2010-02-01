// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __CREQUEST_INL__
#define __CREQUEST_INL__

inline CRequest::CRequest()
	{
	}

inline const RStringF& CRequest::Method() const
	{
	return iMethod;
	}
inline RStringF& CRequest::Method()
	{
	return iMethod;
	}

inline CUri8*& CRequest::URI()
	{
	return iURI;
	}

inline RHTTPRequest CRequest::Handle()
	{
	RHTTPRequest r;
	r.iImplementation = this;
	return r;
	}

#endif // __CREQUEST_INL__

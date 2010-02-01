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

/**
 @file CSecurityPolicy.inl
 @warning : This file contains Rose Model ID comments - please do not delete
*/

#ifndef __CSECURITYPOLICY_INL__
#define __CSECURITYPOLICY_INL__


inline CSecurityPolicy* CSecurityPolicy::NewL(RStringPool aStringPool)
	{
	_LIT8(KDataTypeName, "security-policy");

	// CSecurityPolicy ECOM Interface UID = 101F4485
	const TUid KUidSecPolInterface = {0x101F4485};

	// Set up the parameters which allow ECOM's resolver to pick the right implementation
	TEComResolverParams resParams;
	resParams.SetDataType(KDataTypeName());

	// Get the instantiation.  Allow ECOM to use the default resolver.  Pass in the init params.
	TAny* ptr = REComSession::CreateImplementationL(KUidSecPolInterface,
								 _FOFF(CSecurityPolicy, iDtor_ID_Key),
								 &aStringPool,
								 resParams);

	return REINTERPRET_CAST(CSecurityPolicy*, ptr);
	}

inline CSecurityPolicy::~CSecurityPolicy()
	{
	REComSession::DestroyedImplementation(iDtor_ID_Key);
	}

inline CSecurityPolicy::CSecurityPolicy(RStringPool aStrPool)
	: iStrPool(aStrPool)
	{
	}



inline void CSecurityPolicy::Reserved1()
	{}
inline void CSecurityPolicy::Reserved2()
	{}


#endif // __CSECURITYPOLICY_INL__
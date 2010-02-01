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

#ifndef __CMESSAGE_INL__
#define __CMESSAGE_INL__

inline void CMessage::ConstructL(CTransaction* aTransaction)
	{
	iHeaders = CHeaders::NewL(aTransaction->Codec());
	iTransaction = aTransaction;
	}

inline void CMessage::AddBody(MHTTPDataSupplier* aBody)
	{
	iDataSupplier = aBody;
	}

inline TBool CMessage::HasBody() const
	{
	return (iDataSupplier != NULL);
	}

inline MHTTPDataSupplier* CMessage::Body() const
	{
	return iDataSupplier;
	}


inline RHTTPHeaders CMessage::Headers() const
	{
	return iHeaders->Handle();
	}


#endif // __CMESSAGE_INL__

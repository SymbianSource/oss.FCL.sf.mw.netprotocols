// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// uriliststream.h
// @internalComponent
// 
//

#ifndef __URILISTREADSTREAM_H_
#define __URILISTREADSTREAM_H_

#include <s32mem.h>

class RInetUriListSession;

/**
CUriListReadStream manages all the server to client data transfer of query results in client side. 
Query results may contain multiple rows and the client reads the data in blocks. At any given time
the server transfers maximum records which could contain in IPC data size buffer to the client.
The IPC data size buffer is defined in ... On completion of the query results stream this class
closes the stream in the server side via an IPC call. 

@internalComponent
*/

NONSHARABLE_CLASS ( CUriListReadStream ) : public CBase 
	{
	
	public:
		static CUriListReadStream* NewL( RInetUriListSession& aSession, TInt aHandle );

		TInt ReadUriListL ();
		
		~CUriListReadStream ();
		
		RReadStream& BufReadStream ()
		{
		return iReadStream;	
		} 	
	private:
		CUriListReadStream ( RInetUriListSession& aSession, TInt aHandle );		
		void ConstructL ();
		void ResetL ();
		
		void CloseSrvStream ();
	private:
		RBufReadStream			iReadStream;
		RInetUriListSession& 	iListSession;
		CBufFlat* 				iIpcBuffer;
		TInt 					iHandle;
	};
		
#endif // __URILISTREADSTREAM_H_

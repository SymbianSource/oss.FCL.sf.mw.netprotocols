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

#ifndef __URILISTSTREAM_H_
#define __URILISTSTREAM_H_

#include <s32mem.h>
#include "dbaccessor.h"
#include <uri8.h>
#include <ineturilistdef.h>

class CInetUriImpl;
class CUriQueryFilter;
/**
CUriListStream manages all the server to client data transfer query results via IPC. This class
reads the result from the storage( via the MDBTransaction object ), packs the data and send it 
via IPC. The query results contain multiple records and could contain maximum IPC data size. Basically,
this class is used for transfering query results to the client.

@internalComponent
*/
class CUriListStream : public CBase
	{
	
	public:
		static CUriListStream* NewL ( CUriQueryFilter* aQueryFilter );
		virtual ~CUriListStream ();
		TInt WriteUriListL ( TPtrC8& aData );
	private:
		CUriListStream ();
		void ConstructL ( CUriQueryFilter* aQueryFilter );
		void PopulateDataFieldsL ();
		void ResetL ();
		TInt PendingWriteStreamSizeL ();
		CUri8* CreateUriL ();
		void SetUriComponentL ( CUri8& aUri, const TDesC8& aUriComponent, TUriComponent aComponent );

	private:
		CBufFlat* 			iIpcBuffer;
		RBufWriteStream 	iWriteStream;	
		CInetUriImpl*		iInetUri;
		CUriQueryFilter*	iQueryFilter;
	};

//----------------------------------------
		
#endif // __URILISTSTREAM_H_

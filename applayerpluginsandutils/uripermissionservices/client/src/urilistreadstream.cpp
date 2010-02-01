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
//

#include <e32base.h>
#include <s32buf.h>
#include "urilistreadstream.h"
#include "ineturilistsession.h"
#include "ineturilistserver.h"


CUriListReadStream* CUriListReadStream::NewL  ( RInetUriListSession& aSession, TInt aHandle )
	{
	CUriListReadStream* self = new (ELeave)CUriListReadStream (aSession, aHandle);
	CleanupStack::PushL ( self );
	self->ConstructL ();
	CleanupStack::Pop ();
	return self;
	}

CUriListReadStream::CUriListReadStream ( RInetUriListSession& aSession, TInt aHandle )
: iListSession ( aSession ),
iIpcBuffer ( NULL ),
iHandle ( aHandle )
	{
		
	}


void CUriListReadStream::ConstructL ()
	{
	iIpcBuffer = CBufFlat::NewL ( KIpcDataSize );	
	iIpcBuffer->ExpandL ( 0, KIpcDataSize );		
	iReadStream.Open ( *iIpcBuffer );	// Open the stream
	}

/**
Destructor.

Closes the stream setup in the server
*/
CUriListReadStream::~CUriListReadStream ()
	{
	// Delete the buffer
	iReadStream.Close ();
	delete iIpcBuffer;
	iIpcBuffer = NULL;
	iListSession.CloseSrvStream (iHandle);
	
	}
	
/**
Reads the URI list from the server. The server stream handle is already setup.
*/
TInt CUriListReadStream::ReadUriListL ()
	{
	// Reset the stream
	ResetL ();
	TPtr8 ptr ( iIpcBuffer->Ptr(0) );
	TInt result = iListSession.ReadQueryResults ( iHandle, ptr );	
	User::LeaveIfError ( result );
	return result;
	}

/**
Resets the position to the beginning of the stream
*/
void CUriListReadStream::ResetL ()
	{
	MStreamBuf* srcStream = iReadStream.Source ();
	srcStream->SeekL ( MStreamBuf::ERead, EStreamBeginning, 0 );
	}


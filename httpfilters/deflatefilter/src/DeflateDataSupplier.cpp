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
* Description:  ?Description
*
*/

#include <http/rhttptransaction.h>
#include <barsc.h>
#include "DeflateDataSupplier.h"
#include "DecmpStream.h"
//------------------------------------------------------------------------------

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CDeflateDataSupplier::NewL
// Two-phase constructor
// -----------------------------------------------------------------------------
//
CDeflateDataSupplier* CDeflateDataSupplier::NewL( TInt aTransId, MHTTPDataSupplier* iDataBody, TInt aSize, TInt format )
{
	CDeflateDataSupplier* self = new ( ELeave ) CDeflateDataSupplier( aTransId, iDataBody, format );
	CleanupStack::PushL( self );
	self->ConstructL( aSize );
	CleanupStack::Pop();
	return self;
}

// -----------------------------------------------------------------------------
// CDeflateDataSupplier::CDeflateDataSupplier
// constructor
// -----------------------------------------------------------------------------
//
CDeflateDataSupplier::CDeflateDataSupplier( TInt aTransId, MHTTPDataSupplier* iDataBody, TInt format )
	: iTransId( aTransId ), iBuf(0), iBufPtr( 0, 0 ), iHeaderBuf(0), iHeaderBufPtr(0,0),
		iPHData( iDataBody ), iLastChunk( EFalse ), 
		iStream(0), iFormat( format )
{
}

// -----------------------------------------------------------------------------
// CDeflateDataSupplier::ConstructL
// Data allocation constructor, which leaves
// -----------------------------------------------------------------------------
//
void CDeflateDataSupplier::ConstructL( TInt aSize )
{
	iBuf = HBufC8::NewMaxL( aSize );
	iBufPtr.Set( iBuf->Des() );
	iBufPtr.SetLength( 0 );

	iHeaderBuf = HBufC8::NewMaxL( 10 );
	iHeaderBufPtr.Set( iHeaderBuf->Des() );
	iHeaderBufPtr.SetLength( 0 );
}

// destructor
//
CDeflateDataSupplier::~CDeflateDataSupplier()
{
	delete iHeaderBuf;
	iHeaderBuf = 0;

	delete iBuf;
	iBuf = 0;
	iPHData = 0;
	
	delete iStream;  //lint !e1551 no exception to throw here
	iStream = 0;
}

// -----------------------------------------------------------------------------
// CDeflateDataSupplier::AppendDataL
// Append a bulk of data into the supplier's buffer
// reallocation is needed when buffer is not big enough
// -----------------------------------------------------------------------------
//
void CDeflateDataSupplier::AppendDataL( const TDesC8& aDataPart )
{
	TInt curLen = iBufPtr.Length();
	TInt reqLen = curLen + aDataPart.Length();

	if( reqLen > iBufPtr.MaxLength() )
	{
		TRAPD( error, ( iBuf = iBuf->ReAllocL( reqLen + aDataPart.Length() ) ) );  // realloc a bit more data
		if( error != KErrNone ) User::Leave( KErrNoMemory );
		
		iBufPtr.Set( iBuf->Des() );
		iBufPtr.SetLength( curLen );
	}
	iBufPtr.Append( aDataPart );
	curLen = iBufPtr.Length();
}

// -----------------------------------------------------------------------------
// CDeflateDataSupplier::ProcessDataPartL
// process the content
// -----------------------------------------------------------------------------
//
void CDeflateDataSupplier::ProcessDataPartL()
{
	// get the data part
	TPtrC8 aDataPart;
	iLastChunk = iPHData->GetNextDataPart( aDataPart );

	// decompressing
	TRAPD( err, InflateBodyDataL( aDataPart ) );
	
	// error in decompression, pass the original data
	if( err != KErrNone ) 
	{
		ReleaseData();
		AppendDataL( aDataPart );
	}

	iPHData->ReleaseData();
}

// -----------------------------------------------------------------------------
// CDeflateDataSupplier::InflateBodyDataL
// decompress the content
// -----------------------------------------------------------------------------
//
void CDeflateDataSupplier::InflateBodyDataL( const TDesC8& aData )
{
	if( !iStream ) 
	{
		if( iFormat == CDecmpStream::EGzip )
		{
			iHeaderBuf = iHeaderBuf->ReAlloc( aData.Length() + iHeaderBufPtr.Length() );
			iHeaderBufPtr.Set( iHeaderBuf->Des() );
			iHeaderBufPtr.Append( aData );

			if( iHeaderBufPtr.Length() > 10 )		// for too short Gzip header
				iStream = CDecmpStream::NewL( iHeaderBufPtr, iFormat );
		}
		else
			iStream = CDecmpStream::NewL( aData, iFormat );
	}
	else 
		iStream->ResetBuffer( aData );

	if( iStream )
	{
		while( !iStream->NeedInput() && !iStream->Finalized() )
		{
			iStream->InflateL();
			if( !iStream->Finalized() )					// clumsy but necessary
				AppendDataL( iStream->GetOutput() );
			if( iStream->NeedOutput() )
				iStream->ResetOutput();
		}
	}
}

// -----------------------------------------------------------------------------
// CDeflateDataSupplier::GetNextDataPart
// virtual methods from MHTTPDataSupplier
// -----------------------------------------------------------------------------
//
TBool CDeflateDataSupplier::GetNextDataPart(TPtrC8& aDataPart)
{
	aDataPart.Set( iBufPtr );
	return iLastChunk;
}

// -----------------------------------------------------------------------------
// CDeflateDataSupplier::ReleaseData
// virtual methods from MHTTPDataSupplier
// -----------------------------------------------------------------------------
//
void CDeflateDataSupplier::ReleaseData()
{
//	iBufPtr.Set( iBuf->Des() );
	iBufPtr.SetLength( 0 );
}

// -----------------------------------------------------------------------------
// CDeflateDataSupplier::OverallDataSize
// virtual methods from MHTTPDataSupplier
// -----------------------------------------------------------------------------
//
TInt CDeflateDataSupplier::OverallDataSize()
{
	return iPHData->OverallDataSize();
}

// -----------------------------------------------------------------------------
// CDeflateDataSupplier::Reset
// virtual methods from MHTTPDataSupplier
// -----------------------------------------------------------------------------
//
TInt CDeflateDataSupplier::Reset()
{
	return KErrNone;
}

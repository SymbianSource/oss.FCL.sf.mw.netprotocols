/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
*  Deflate decompression filter for HTTP protocol stack.
*
*
*/

#include <ezstream.h>
#include "StreamBufMgr.h"
//-----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// StreamBufMgr::NewL
// Two-phase constructor
// ----------------------------------------------------------------------------
//
StreamBufMgr* StreamBufMgr::NewL( TPtrC8 in )
{
	StreamBufMgr* self = new (ELeave) StreamBufMgr( in );
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop();
	return self;
}//lint !e1746 const reference is not necessary here, because TPtrC8 takes only 4-bytes

// ----------------------------------------------------------------------------
// CDeflateFilter::CDeflateFilter
// Constructor
// ----------------------------------------------------------------------------
//
StreamBufMgr::StreamBufMgr( TPtrC8 in ) 
					: iInput( in ), iOutputDes( 0, 0 ), iOutput(0),
						iNeedInput(EFalse), iNeedOutput(EFalse), 
							iFinalized(EFalse)
{
} //lint !e1746

// ----------------------------------------------------------------------------
// CDeflateFilter::ConstructL
// Initialize the output buffer
// ----------------------------------------------------------------------------
//
void StreamBufMgr::ConstructL()
{
	// buffer
	iOutput = HBufC8::NewMaxL( 1<<17 );
	iOutputDes.Set( iOutput->Des() );
}

// ----------------------------------------------------------------------------
// CDeflateFilter::~CDeflateFilter
// Destructor
// ----------------------------------------------------------------------------
//
StreamBufMgr::~StreamBufMgr()
{
	delete iOutput;
	iOutput = 0;
}

// ----------------------------------------------------------------------------
// CDeflateFilter::InitializeL
// Overriden function from MEZBufferManager
// ----------------------------------------------------------------------------
//
void StreamBufMgr::InitializeL( CEZZStream& aZStream )
{
	// read more
	aZStream.SetInput( iInput );
	iOutputDes.SetLength( 0 );
	aZStream.SetOutput( iOutputDes );
	iNeedInput = EFalse;
	iNeedOutput = EFalse;
}

// ----------------------------------------------------------------------------
// CDeflateFilter::NeedInputL
// Overriden function from MEZBufferManager
// ----------------------------------------------------------------------------
//
void StreamBufMgr::NeedInputL( CEZZStream& /*aZStream*/ )
{
	//aZStream.SetInput( iInput );
	iNeedInput = ETrue;
}

// ----------------------------------------------------------------------------
// CDeflateFilter::NeedOutputL
// Overriden function from MEZBufferManager
// ----------------------------------------------------------------------------
//
void StreamBufMgr::NeedOutputL( CEZZStream& /*aZStream*/ )
{
	//aZStream.SetOutput( iOutputDes );
	iNeedOutput = ETrue;
}

// ----------------------------------------------------------------------------
// CDeflateFilter::NeedOutputL
// Overriden function from MEZBufferManager
// ----------------------------------------------------------------------------
//
void StreamBufMgr::FinalizeL( CEZZStream& /*aZStream*/ )
{
	// do nothing now, should check the checksum value here
	// read the checksum value from the last chunk
	//const TInt32* checksum = REINTERPRET_CAST( const TInt32*, iInput.Ptr() + ( iInput.Length() - sizeof( TInt32 )*2 ) );
	iFinalized = ETrue;
}

// ----------------------------------------------------------------------------
// CDeflateFilter::SetBuffer
// Set the input buffer
// ----------------------------------------------------------------------------
//
void StreamBufMgr::SetBuffer( TPtrC8 in )
{
	iInput.Set( in );
}//lint !e1746

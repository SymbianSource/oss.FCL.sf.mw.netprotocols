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

#include <s32mem.h>
#include <ezdecompressor.h>
#include "StreamBufMgr.h"
#include "DecmpStream.h"
//-----------------------------------------------------------------------------
const TUint8 CDecmpStream::ID1 = 31;
const TUint8 CDecmpStream::ID2 = 139;
// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CDeflateFilter::NewL
// Constructor
// ----------------------------------------------------------------------------
//
CDecmpStream* CDecmpStream::NewL( TPtrC8 in, TInt format )
{
	CDecmpStream* self = new (ELeave) CDecmpStream( format );
	CleanupStack::PushL( self );
	self->ConstructL( in );
	CleanupStack::Pop();
	return self;
} //lint !e1746		no benefit get from const reference here

// ----------------------------------------------------------------------------
// CDeflateFilter::CDecmpStream
// Constructor
// ----------------------------------------------------------------------------
//
CDecmpStream::CDecmpStream( TInt format ) 
		: iDecompressor(0), iBufMgr(0), iAuxBuf( 0 ), iCheckSum(0), iFormat( format )
{
}

// ----------------------------------------------------------------------------
// CDeflateFilter::~CDecmpStream
// Destructor
// ----------------------------------------------------------------------------
//
CDecmpStream::~CDecmpStream()
{
	delete iAuxBuf;
	iAuxBuf = 0;
	delete iDecompressor;//lint !e1551  no exception
	iDecompressor = 0;
	delete iBufMgr;//lint !e1551  no exception
	iBufMgr = 0;
} 

// ----------------------------------------------------------------------------
// CDeflateFilter::ConstructL
// Initiliazing the buffer manager and decompressor
// ----------------------------------------------------------------------------
//
void CDecmpStream::ConstructL( TPtrC8 in )
{
	// buffer manager
	iBufMgr = StreamBufMgr::NewL( in );
	
	if( iFormat == EGzip )
	{
		// read Gzip header
		TEZGZipHeader header;

		ReadHeaderL( header );
		
		// move forward
		TPtrC8 nextin( REINTERPRET_CAST( const TUint8*, iBufMgr->iInput.Ptr() + iOffset ), iBufMgr->iInput.Length() - iOffset );
		iBufMgr->SetBuffer( nextin );

		iDecompressor = CEZDecompressor::NewL( *iBufMgr, -CEZDecompressor::EMaxWBits );
		iCheckSum = crc32(iCheckSum,Z_NULL,0);		// init CRC checksum
	}
	else
	{
		TZlibHeader header;

		// check if the server complies with which RFC
		// accroding to Paul Marquess: http://archive.develooper.com/libwww@perl.org/msg04644.html
		// some web servers implement deflate method specified in RFC1951, which is wrong.  
		// The correct way should be RFC1950. It's reported IE also uses the wrong way. This
		// should be supported in this filter too.
		if( ReadHeaderL( header ) )
		{
			// RFC1950 is used, valid header + compressed data flow
			TPtrC8 nextin( REINTERPRET_CAST( const TUint8*, iBufMgr->iInput.Ptr() + iOffset ), iBufMgr->iInput.Length() - iOffset );
			iBufMgr->SetBuffer( nextin );	
		}
		else
		{
			// RFC1951 is used, only compressed data flow
			TPtrC8 nextin( REINTERPRET_CAST( const TUint8*, iBufMgr->iInput.Ptr() ), iBufMgr->iInput.Length() );
			iBufMgr->SetBuffer( nextin );
		}

		iDecompressor = CEZDecompressor::NewL( *iBufMgr, -CEZDecompressor::EMaxWBits );
		iCheckSum = adler32(iCheckSum,Z_NULL,0);		// init Adler32 checksum
	}
}//lint !e1746		no benefit get from const reference here

// ----------------------------------------------------------------------------
// CDeflateFilter::ResetBuffer
// Reset the input buffer for buffer manager, init the decompressor
// ----------------------------------------------------------------------------
//
void CDecmpStream::ResetBuffer( TPtrC8 in )
{
	iBufMgr->SetBuffer( in );
	iBufMgr->InitializeL( *iDecompressor );
} //lint !e1746

// ----------------------------------------------------------------------------
// CDeflateFilter::InflateL
// Inflate the content
// ----------------------------------------------------------------------------
//
TBool CDecmpStream::InflateL()
{
	// inflating the content
	return iDecompressor->InflateL();
}

// ----------------------------------------------------------------------------
// CDeflateFilter::ReadHeaderL
// Read the header of deflated stream in zlib format, RFC1950
// ----------------------------------------------------------------------------
//
TBool CDecmpStream::ReadHeaderL( TZlibHeader& header )
{
	// construct a stream for reading
	RMemReadStream memBuf;
	memBuf.Open( iBufMgr->iInput.Ptr(), iBufMgr->iInput.Length() );
	TInt obligatoryData = sizeof( header.iCMF ) + sizeof( header.iFLG );

	// copy the header data
	TPtr8 des( &header.iCMF, 0, obligatoryData );
	TRAPD( error, memBuf.ReadL( des ) );
	iOffset = memBuf.Source()->TellL( MStreamBuf::ERead ).Offset();		
	memBuf.Close();

	if( error == KErrEof ) User::Leave( EBadZlibHeader );
	if( des.Size() < obligatoryData ) User::Leave( EBadZlibHeader );

	// check CMF value and FLG value, if the header is valid
	if( (header.iCMF & 0xf) == 8 )
	{
		// check if there is dictionary data, which is not supported by this zlib version
		if( header.iFLG & ( 1<<5 ) ) User::Leave( EDictionaryNotSupported );
		return ETrue;
	}
	else 
		return EFalse;
}

// ----------------------------------------------------------------------------
// CDeflateFilter::ReadHeaderL
// Read the header of Gzip-ed stream in Gzip format, RFC1952
// ----------------------------------------------------------------------------
//
void CDecmpStream::ReadHeaderL( TEZGZipHeader& aHeader )
{
	// construct a stream for reading
	RMemReadStream memBuf;
	memBuf.Open( iBufMgr->iInput.Ptr(), iBufMgr->iInput.Length() );

	TInt obligatoryData = sizeof(aHeader.iId1) + sizeof(aHeader.iId2) + sizeof(aHeader.iCompressionMethod) +
		sizeof(aHeader.iFlags) + sizeof(aHeader.iTime) + sizeof(aHeader.iExtraFlags) + sizeof(aHeader.iOs);

	// copy the header data
	TPtr8 des( &aHeader.iId1,0,obligatoryData );
	TRAPD( error, memBuf.ReadL( des ) );

	if( error == KErrEof )
	{
		User::Leave(EZGZipFile::EBadGZipHeader);
	}
	
	if (des.Size() < obligatoryData)
		User::Leave(EZGZipFile::EBadGZipHeader);		// TODO: should consider the situation when no enough data has arrived.

	if (aHeader.iId1 != ID1 || aHeader.iId2 != ID2)
		User::Leave(EZGZipFile::EBadGZipHeader);
	
	if (aHeader.iFlags & (1 << EZGZipFile::EFExtra)) // then the extra bit is set
	{
		des.Set(REINTERPRET_CAST(TUint8 *,&aHeader.iXlen),0,sizeof(aHeader.iXlen));
		memBuf.ReadL(des);
		if ( des.Size() != sizeof(aHeader.iXlen) || aHeader.iXlen < 0 ) //lint !e737 sign/unsigned doesn't realy matter here
			User::Leave(EZGZipFile::EBadGZipHeader);
		
		aHeader.iExtra = HBufC8::NewMaxL(aHeader.iXlen);
		TPtr8 des1 = aHeader.iExtra->Des();
		memBuf.ReadL(des1);
		if (des1.Size() != aHeader.iXlen)
			User::Leave(EZGZipFile::EBadGZipHeader);
	}
	
	if (aHeader.iFlags & (1 << EZGZipFile::EFName)) // then read in filename
		ReadStringIntoDescriptorL(memBuf,&aHeader.iFname);

	if (aHeader.iFlags & (1 << EZGZipFile::EFComment)) // then read in comment
		ReadStringIntoDescriptorL(memBuf,&aHeader.iComment);
			
	if (aHeader.iFlags & (1 << EZGZipFile::EFHcrc))
	{
		des.Set(REINTERPRET_CAST(TUint8*,&aHeader.iCrc),0,sizeof(aHeader.iCrc));
		memBuf.ReadL(des);
		if (des.Size() != sizeof(aHeader.iCrc)) //lint !e737 unsigned/signed doesn't matter
			User::Leave(EZGZipFile::EBadGZipHeader);
	}

	iOffset = memBuf.Source()->TellL( MStreamBuf::ERead ).Offset();
	memBuf.Close();
}

// ----------------------------------------------------------------------------
// CDeflateFilter::ReadStringIntoDescriptorL
// Read null-terminated strings into the buffer
// ----------------------------------------------------------------------------
//
void CDecmpStream::ReadStringIntoDescriptorL( RMemReadStream& aMem, HBufC8 **aDes ) const
{
	TInt i;
	CArrayFixFlat<TUint8> *bytes = new (ELeave) CArrayFixFlat<TUint8>(16);
	CleanupStack::PushL(bytes);
	TUint8 ch;
	
	while ( ( ch = aMem.ReadUint8L() ) != '\0' )
		bytes->AppendL( ch );
		
	*aDes = HBufC8::NewMaxL(bytes->Count());
	TPtr8 desPtr = (*aDes)->Des(); //lint because error #111 - separate desPtr used
	for (i = 0; i < bytes->Count(); i++)
		desPtr[i] = (*bytes)[i]; //lint !e1058 force conversion here
	CleanupStack::PopAndDestroy(); // delete bytes	
}
// ----------------------------------------------------------------------------
// CDeflateFilter::GetOutput
// Get the decompressed output, with checksum applied
// ----------------------------------------------------------------------------
//
TPtrC8 CDecmpStream::GetOutput()
{
	TPtrC8 od = iDecompressor->OutputDescriptor();

	if( iFormat == EGzip )
		iCheckSum = crc32( iCheckSum,od.Ptr(),od.Size() );  //lint !e732
	else
		iCheckSum = adler32( iCheckSum, od.Ptr(), od.Size() ); //lint !e732

	return iDecompressor->OutputDescriptor();
}

// ----------------------------------------------------------------------------
// CDeflateFilter::GetOutput
// Check if the decompressor needs more input in the last iteration
// ----------------------------------------------------------------------------
//
TBool CDecmpStream::NeedInput() const
{
	return iBufMgr->iNeedInput;
}

// ----------------------------------------------------------------------------
// CDeflateFilter::NeedOutput
// Check if the decompressor needs more output in the last iteration
// ----------------------------------------------------------------------------
//
TBool CDecmpStream::NeedOutput() const
{
	return iBufMgr->iNeedOutput;
}

// ----------------------------------------------------------------------------
// CDeflateFilter::Finalized
// Check if the decompressor is finalized
// ----------------------------------------------------------------------------
//
TBool CDecmpStream::Finalized() const
{
	return iBufMgr->iFinalized;
}

// ----------------------------------------------------------------------------
// CDeflateFilter::ResetOutput
// Reset the output buffer
// ----------------------------------------------------------------------------
//
void CDecmpStream::ResetOutput()
{
	iBufMgr->iOutputDes.SetLength( 0 );
	iDecompressor->SetOutput( iBufMgr->iOutputDes );
	iBufMgr->iNeedOutput = EFalse;
}

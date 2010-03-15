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

 
#ifndef __DECMPSTREAM_H_
#define __DECMPSTREAM_H_
//------------------------------------------------------------------------------

#include <ezgzip.h>
//------------------------------------------------------------------------------

class EZDecompressor;
class StreamBufMgr;
class RMemReadStream;
//------------------------------------------------------------------------------

class TZlibHeader
{
public:
	// ctor & dtor
	TZlibHeader() : iCMF( 0 ), iFLG( 0 )				{}
	~TZlibHeader()										{}
	//----------------------------------------------------------------------

	// data
	TUint8 iCMF;
	TUint8 iFLG;
	//----------------------------------------------------------------------
};


class CDecmpStream : public CBase
{
public:
	enum { EGzip=0, EZlib=1 };
	enum { EBadZlibHeader = -1,EDictionaryNotSupported = -2 };
	//--------------------------------------------------------------------------

	// dtor & ctor
	static CDecmpStream* NewL( TPtrC8 in, TInt format = EGzip );
	virtual ~CDecmpStream();
	//--------------------------------------------------------------------------

	// inflate the content
	TBool InflateL();
	TPtrC8 GetOutput();
	void ResetBuffer( TPtrC8 in );
	TBool NeedInput() const;
	TBool NeedOutput() const;
	TBool Finalized() const;
	void ResetOutput();
	//--------------------------------------------------------------------------

private:
	// ctor
	CDecmpStream( TInt format );
	void ConstructL( TPtrC8 in );
	//--------------------------------------------------------------------------

	// helper functions
	void ReadHeaderL( TEZGZipHeader& header );
	TBool ReadHeaderL( TZlibHeader& header );
	void ReadTrailerL( TEZGZipTrailer& trailer );
	void ReadStringIntoDescriptorL( RMemReadStream& aMem, HBufC8 **aDes ) const;
	//--------------------------------------------------------------------------

	CEZDecompressor* iDecompressor;
	StreamBufMgr* iBufMgr;
	TInt			iOffset;
	HBufC8*			iAuxBuf;		// auxillary buffer, used for the wrong deflate implmentation
	TUint32			iCheckSum;		// CRC or Adler
	TInt			iFormat;
	//--------------------------------------------------------------------------

	// Gzip Magic numbers
	static const TUint8 ID1;
	static const TUint8 ID2;
	//--------------------------------------------------------------------------
};

//------------------------------------------------------------------------------
#endif // !__DECMPSTREAM_H_

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


#ifndef __DEFLATEDATASUPPLIER_H
#define __DEFLATEDATASUPPLIER_H
//-----------------------------------------------------------------------------

#include <http/mhttpdatasupplier.h>
#include "DecmpStream.h"
//-----------------------------------------------------------------------------

/**
* a data supplier used for replace the default supplier in response data
*
* @lib DeflateFilter.lib
* @since Rubik_0336
*/
class CDeflateDataSupplier : public CBase, public MHTTPDataSupplier
{
public:
	//-----------------------------------------------------------------------------

	// ctor & dtor
	static CDeflateDataSupplier* NewL( TInt, MHTTPDataSupplier* iDataBody, TInt aSize = (1<<15), TInt format = CDecmpStream::EGzip );
	virtual ~CDeflateDataSupplier();
	//-------------------------------------------------------------------------

	// accessors and mutators
	
	TPtr8 GetBuf() const							{ return iBufPtr; }
	//-------------------------------------------------------------------------

	// virtual functions from MHTTPDataSupplier
	virtual TBool GetNextDataPart(TPtrC8& aDataPart);
	virtual void ReleaseData();
	virtual TInt OverallDataSize();
	virtual TInt Reset();
	TInt GetTransId() const							{ return iTransId; }
	MHTTPDataSupplier* GetPHDataBody()				{ return iPHData; }
	//-------------------------------------------------------------------------

	void ProcessDataPartL();
	//-------------------------------------------------------------------------
	
private:
	CDeflateDataSupplier( TInt aTransId, MHTTPDataSupplier* iDataBody, TInt format );
	CDeflateDataSupplier( const CDeflateDataSupplier& );			// not implemented
	CDeflateDataSupplier& operator=( const CDeflateDataSupplier& ); // not implemented
	//-------------------------------------------------------------------------

	void ConstructL( TInt aSize );
	void AppendDataL( const TDesC8& aData );
	void InflateBodyDataL( const TDesC8& aData );
	//-------------------------------------------------------------------------
	
	TInt	iTransId;
	HBufC8* iBuf;
	TPtr8	iBufPtr;
	HBufC8*	iHeaderBuf;
	TPtr8	iHeaderBufPtr;
	MHTTPDataSupplier *iPHData;			// data supplier from protocol handler
	TBool	iLastChunk;
	CDecmpStream* iStream;
	TInt	iFormat;
	//-------------------------------------------------------------------------
};
//-----------------------------------------------------------------------------
#endif // __DEFLATEDATASUPPLIER_H 

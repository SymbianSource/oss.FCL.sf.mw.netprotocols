// Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __CHTTPGENERALHEADERREADER_H__
#define __CHTTPGENERALHEADERREADER_H__

#include <e32base.h>
#include <stringpool.h>
#include <http/framework/cheadercodec.h>
 
#include "chttpheaderreader.h"

/**	
	The CHttpGeneralHeaderReader class is used by CHttpGeneralHeaderCodec to 
	decode header fields.
	@internalComponent
	@see		CHttpGeneralHeaderCodec
	@see		CHttpReader
	@see		CHeaderReader
*/
class CHttpGeneralHeaderReader : public CHttpHeaderReader
	{
public: // methods

	static CHttpGeneralHeaderReader* NewL(RStringPool aStringPool);

	virtual ~CHttpGeneralHeaderReader();

private:	// methods from CHeaderReader

	virtual void DecodeHeaderL(RHeaderField& aHeader);

private:	// methods

	CHttpGeneralHeaderReader(RStringPool aStringPool);

	void DecodeCacheControlL(RHeaderField& aHeader) const;
    
	void DecodeConnectionL(RHeaderField& aHeader) const; 
	
	void DecodeTransferEncodingL(RHeaderField& aHeader) const; 

	void DecodeContentEncoding(RHeaderField& aHeader) const; 
	
	void DecodeContentLanguage(RHeaderField& aHeader) const; 
	
	void DecodeContentLengthL(RHeaderField& aHeader) const; 
	
	void DecodeContentLocation(RHeaderField& aHeader) const; 
	
	void DecodeContentMd5(RHeaderField& aHeader) const; 
	
	void DecodeContentTypeL(RHeaderField& aHeader) const; 
	
	void DecodeContentDispositionL(RHeaderField& aHeader) const;
	};

#endif	// __CHTTPGENERALHEADERREADER_H__

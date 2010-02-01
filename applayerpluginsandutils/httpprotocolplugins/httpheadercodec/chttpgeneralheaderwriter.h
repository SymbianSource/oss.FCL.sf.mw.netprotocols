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

#ifndef __CHTTPGENERALHEADERWRITER_H__
#define __CHTTPGENERALHEADERWRITER_H__

#include <e32base.h>
#include <stringpool.h>
#include <http/framework/cheadercodec.h>
#include "timerlogger.h"

#include "chttpheaderwriter.h"



/**
	@internalComponent
	@since			8.0
		
	This class is used by CGeneralHeaderCodec to decode header fields.
	Derived from CTextModeHeaderWriter which in turn is derived from CHeaderWriter.
*/
class CHttpGeneralHeaderWriter : public CHttpHeaderWriter
	{
public: // methods

	virtual ~CHttpGeneralHeaderWriter();
	static CHttpGeneralHeaderWriter* NewL(RStringPool aStrPool);
	virtual void EncodeHeaderL(RHeaderField& aHeader);

private:
	CHttpGeneralHeaderWriter(RStringPool aStrPool);

	void EncodeCacheControlL(RHeaderField& aHeader) const;
    void EncodeConnectionL(RHeaderField& aHeader) const; 
    void EncodeDateL(RHeaderField& aHeader) const; 
	void EncodeTransferEncodingL(RHeaderField& aHeader) const; 

	void EncodeContentEncoding(RHeaderField& aHeader) const; 
	void EncodeContentLengthL(RHeaderField& aHeader) const; 
	void EncodeContentTypeL(RHeaderField& aHeader) const; 
	void EncodeExpiresL(RHeaderField& aHeader) const; 
	void EncodeLastModifiedL(RHeaderField& aHeader) const; 
    __DECLARE_PERFORMANCE_LOG
	};

#endif	// __CHTTPGENERALHEADERWRITER_H__

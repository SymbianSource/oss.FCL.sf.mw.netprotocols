// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __CHTTPDEFAULTHEADERREADER_H__
#define __CHTTPDEFAULTHEADERREADER_H__

#include <e32base.h>
#include <http/framework/cheadercodec.h>

/** A default header reader that just copies received raw data directly into single part
	string header fields. NB would this be better placed in the Codec base class, by making
	EncodeL and DecodeL non-pure virtual? Could therefore eliminate these three 'default'
	classes.
	@todo
*/
class CHttpDefaultHeaderReader : public CHeaderReader
	{
public: // methods

	static CHttpDefaultHeaderReader* NewL(RStringPool aStrPool);

	virtual ~CHttpDefaultHeaderReader();

private:	// methods from CHeaderReader

	virtual void DecodeHeaderL(RHeaderField& aHeader);

private: // methods

	/** Constructor - stores a copy of the string pool handle
	*/
	CHttpDefaultHeaderReader(RStringPool aStringPool);

private:	// attributes

	RStringPool		iStringPool;

	};

#endif	// __CHTTPDEFAULTHEADERREADER_H__

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

#ifndef __CHTTPDEFAULTHEADERWRITER_H__
#define __CHTTPDEFAULTHEADERWRITER_H__

#include <e32base.h>
#include <http/framework/cheadercodec.h>

/** A default header writer that just copies single part string header fields directly into
	raw data.  NB would this be better placed in the Codec base class, by making EncodeL and
	DecodeL non-pure virtual? Could therefore eliminate these three 'default' classes.
	@todo
*/
class CHttpDefaultHeaderWriter : public CHeaderWriter
	{
public: // methods

	static CHttpDefaultHeaderWriter* NewL(RStringPool aStringPool);

	virtual ~CHttpDefaultHeaderWriter();

private:	// methods from CHeaderWriter

	virtual void EncodeHeaderL(RHeaderField& aHeader);

private:	// methods

	CHttpDefaultHeaderWriter(RStringPool aStrignPool);

private:	// attributes

	RStringPool		iStringPool;

	};

#endif	// __CHTTPDEFAULTHEADERWRITER_H__

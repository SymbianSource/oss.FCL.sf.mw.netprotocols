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

#ifndef __CWSPUNKNOWNCAPABILITIES_H__
#define __CWSPUNKNOWNCAPABILITIES_H__

#include <e32base.h>
#include <wsp/mwspunknowncapabilities.h>

class CUnknownCapability;

/**	The CWspUnknownCapabilities class represents a set of Extended Methods 
	capabilities. It implements the MWspUnknownCapabilities interface.
	@see			MWspHeaderCoMWspUnknownCapabilitiesdePages
*/
//##ModelId=3C4C41AC02B5
class CWspUnknownCapabilities : public CBase, public MWspUnknownCapabilities
	{
public:	// Methods

/**	@fn				NewL()
	Static factory constructor.
	@since			7.0
*/
	//##ModelId=3C4C41AC031A
	static CWspUnknownCapabilities* NewL();

/**	@fn				~CWspUnknownCapabilities()
	Standard destructor.
	@since			7.0
*/
	//##ModelId=3C4C41AC0319
	~CWspUnknownCapabilities();

public:	// Methods from MWspUnknownCapabilities

/** @fn				Start() const
	@see			MWspUnknownCapabilities
*/
	//##ModelId=3C4C41AC0311
	virtual TInt Start() const;

/** @fn				GetNext(TPtrC8& aIdentifier, TPtrC8& aValue) const
	@see			MWspUnknownCapabilities
*/
	//##ModelId=3C4C41AC0308
	virtual TInt GetNext(TPtrC8& aIdentifier, TPtrC8& aValue) const;

/** @fn				Reset()
	@see			MWspUnknownCapabilities
*/
	//##ModelId=3C4C41AC0307
	virtual void Reset();

/** @fn				AddUnknownCapabilityL(const TDesC8& aIdentifier, const TDesC8& aValue)
	@see			MWspUnknownCapabilities
*/
	//##ModelId=3C4C41AC02FC
	virtual void AddUnknownCapabilityL(const TDesC8& aIdentifier, const TDesC8& aValue);

private:	// Methods

/**	@fn				CWspUnknownCapabilities()
	First phase constructor.
	@since			7.0
*/
	//##ModelId=3C4C41AC02FB
	CWspUnknownCapabilities();

private:	// Attributes

	/** An array of unknown capabilities.
	*/
	//##ModelId=3C4C41AC02F1
	RPointerArray<CUnknownCapability>		iUnknownCapabilities;

	/** An index with the current position in the array. Is mutable to maintain
		logical const-ness.
	*/
	//##ModelId=3C4C41AC02CD
	mutable TInt							iIndex;

	};

#endif	// __CWSPUNKNOWNCAPABILITIES_H__

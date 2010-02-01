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

#ifndef		__CWSPALIASADDRESSES_H__
#define		__CWSPALIASADDRESSES_H__

#include <e32base.h>
#include <wsp/mwspaliasaddresses.h>

class CAliasAddress;

/**	The CWspAliasAddresses class represents a set of Alias Address capabilities.
	It implements the MWspAliasAddresses interface.
	@see			MWspAliasAddresses
*/
//##ModelId=3C4C419D017D
class CWspAliasAddresses : public CBase, public MWspAliasAddresses
	{
public: // Methods

/**	@fn				NewL()
	Static factory constructor.
	@since			7.0
*/
	//##ModelId=3C4C419D01E1
	static CWspAliasAddresses* NewL();

/**	@fn				~CWspAliasAddresses()
	Standard destructor.
	@since			7.0
*/
	//##ModelId=3C4C419D01DB
	~CWspAliasAddresses();

public:	// Methods from MWspAliasAddresses

/** @fn				Start() const
	@see			MWspAliasAddresses
*/
	//##ModelId=3C4C419D01DA
	virtual TInt Start() const;

/** @fn				GetNext(TWspBearer& aBearer, TUint16& aPort, TPtrC8& aAddress) const
	@see			MWspAliasAddresses
*/
	//##ModelId=3C4C419D01CE
	virtual TInt GetNext(TWspBearer& aBearer, TUint16& aPort, TPtrC8& aAddress) const;

/** @fn				Reset()
	@see			MWspAliasAddresses
*/
	//##ModelId=3C4C419D01CD
	virtual void Reset();

/** @fn				AddAddressL(TWspBearer aBearer, TUint16 aPort, const TDesC8& aAddress)
	@see			MWspAliasAddresses
*/
	//##ModelId=3C4C419D01C3
	virtual void AddAddressL(TWspBearer aBearer, TUint16 aPort, const TDesC8& aAddress);

private:	// Methods

/**	@fn				CWspAliasAddresses()
	First phase constructor.
	@since			7.0
*/
	//##ModelId=3C4C419D01BA
	CWspAliasAddresses();

private:	// Attributes

	/** An array of alias addresses.
	*/
	//##ModelId=3C4C419D01B1
	RPointerArray<CAliasAddress>		iAddresses;

	/** An index with the current position in the array. Is mutable to maintain
		logical const-ness.
	*/
	//##ModelId=3C4C419D0195
	mutable TInt						iIndex;

	};

#endif	// __CWSPALIASADDRESSES_H__

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

#ifndef __CWSPEXTENDEDMETHODS_H__
#define __CWSPEXTENDEDMETHODS_H__

#include <e32base.h>
#include <wsp/mwspextendedmethods.h>

class CExtendedMethod;

/**	The CWspExtendedMethods class represents a set of Extended Methods 
	capabilities. It implements the MWspExtendedMethods interface.
	@see			MWspExtendedMethods
*/
//##ModelId=3C4C419D03CC
class CWspExtendedMethods : public CBase, public MWspExtendedMethods
	{
public:	// Methods

/**	@fn				NewL()
	Static factory constructor.
	@since			7.0
*/
	//##ModelId=3C4C419E003F
	static CWspExtendedMethods* NewL();

/**	@fn				~CWspExtendedMethods()
	Standard destructor.
	@since			7.0
*/
	//##ModelId=3C4C419E003E
	~CWspExtendedMethods();

public:	// Methods from MWspExtendedMethods

/** @fn				Start() const
	@see			MWspExtendedMethods
*/
	//##ModelId=3C4C419E0037
	virtual TInt Start() const;

/** @fn				GetNext(TUint8& aPDUType, TPtrC8& aMethodName) const
	@see			MWspExtendedMethods
*/
	//##ModelId=3C4C419E0034
	virtual TInt GetNext(TUint8& aPDUType, TPtrC8& aMethodName) const;

/** @fn				Reset()
	@see			MWspExtendedMethods
*/
	//##ModelId=3C4C419E002D
	virtual void Reset();

/** @fn				AddExtendedMethodL(TUint8 aPDUType, const TDesC8& aMethodName)
	@see			MWspExtendedMethods
*/
	//##ModelId=3C4C419E002A
	virtual void AddExtendedMethodL(TUint8 aPDUType, const TDesC8& aMethodName);

private:	// Methods

/**	@fn				CWspExtendedMethods()
	First phase constructor.
	@since			7.0
*/
	//##ModelId=3C4C419E0021
	CWspExtendedMethods();

private:	// Attributes

	/** An array of extended methods.
	*/
	//##ModelId=3C4C419E0018
	RPointerArray<CExtendedMethod>		iMethods;

	/** An index with the current position in the array. Is mutable to maintain
		logical const-ness.
	*/
	//##ModelId=3C4C419D03E4
	mutable TInt						iIndex;

	};

#endif	// __CWSPEXTENDEDMETHODS_H__

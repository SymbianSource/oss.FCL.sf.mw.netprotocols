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

#ifndef __CEXTENDEDMETHOD_H__
#define __CEXTENDEDMETHOD_H__

#include <e32base.h>

/**	The CExtendedMethod class represents the Extended Method capability as 
	defined by the WSP Specification, July 2001.
*/
//##ModelId=3C4C41A0005F
class CExtendedMethod : public CBase
	{
public:	// Methods

/**	
	Static factory constructor.
	@since			7.0
	@leave		This function will leave if the object cannot be constructed.
	@param			aPDUType		The PDU type assigned to the method name.
	@param			aMethodName		A reference to a buffer with the method name.
*/
	//##ModelId=3C4C41A0009D
	static CExtendedMethod* NewL(TUint8 aPDUType, const TDesC8& aMethodName);

/**	
	Standard destrcutor.
	@since			7.0
*/
	//##ModelId=3C4C41A0009C
	~CExtendedMethod();

/**	
	Returns the PDU type for this method.
	@since			7.0
	@return			A byte containing the PDU type assigned to the method name.
*/
	//##ModelId=3C4C41A0009B
	inline TUint8 GetPDUType() const;

/**	
	Returns the method name.
	@since			7.0
	@return			A const reference to the buffer containg the method name.
*/
	//##ModelId=3C4C41A00095
	inline const TDesC8& GetMethodName() const;

private:	// Methods

/**	
	First phase constuctor.
	@since			7.0
	@param			aPDUType		The PDU type assigned to the method name.
*/
	//##ModelId=3C4C41A00093
	CExtendedMethod(TUint8 aPDUType);

/**	
	Second phase constructor. Does any necessary allocations.
	@since			7.0
	@param			aMethodName		A reference to a buffer with the method name.
*/
	//##ModelId=3C4C41A00091
	void ConstructL(const TDesC8& aMethodName);

private:	// Attributes

	/** The PDU type assigned to the method specified by iMethodName.
	*/
	//##ModelId=3C4C41A00089
	TUint8		iType;

	/**	A buffer containing the method name.
	*/
	//##ModelId=3C4C41A0007F
	HBufC8*		iName;

	};

#include "CExtendedMethod.inl"

#endif	// __CEXTENDEDMETHOD_H__

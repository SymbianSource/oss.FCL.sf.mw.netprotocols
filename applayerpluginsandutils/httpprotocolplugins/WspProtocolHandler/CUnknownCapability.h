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

#ifndef __CUNKNOWNCAPABILITY_H__
#define __CUNKNOWNCAPABILITY_H__

#include <e32base.h>

/**	The CUnknownCapability class represents the Unknown capability as defined 
	by the WSP Specification, July 2001.
*/
//##ModelId=3C4C41B40112
class CUnknownCapability : public CBase
	{
public:	// Methods

/**	
	Static factory constructor.
	@since			7.0
	@param			aIdentifier	A reference to a buffer containing the identifier
								for the capability.
	@param			aValue		A reference to a buffer containing the parameter 
								value for the capability
*/
	//##ModelId=3C4C41B40150
	static CUnknownCapability* NewL(const TDesC8& aIdentifier, const TDesC8& aValue);

/**	
	Standard destructor
	@since			7.0
*/
	//##ModelId=3C4C41B4014F
	~CUnknownCapability();

/**	
	Returns the identifier for the capability.
	@since			7.0
	@return			A const reference to a buffer containing the identifier for
					the capability.
*/
	//##ModelId=3C4C41B4014E
	inline const TDesC8& GetIdentifier() const;

/**	
	Returns the parameter value for the capability.
	@since			7.0
	@return			A const reference to a buffer containing the parameter value
					for the capability.
*/
	//##ModelId=3C4C41B40147
	inline const TDesC8& GetValue() const;

private:	// Methods

/**	
	First phase constructor
	@since			7.0
*/
	//##ModelId=3C4C41B40146
	CUnknownCapability();

/**	
	Second phase constructor. Does any necessary allocations
	@since			7.0
	@param			aIdentifier	A reference to a buffer containing the identifier
								for the capability.
	@param			aValue		A reference to a buffer containing the parameter 
								value for the capability
*/
	//##ModelId=3C4C41B4013A
	void ConstructL(const TDesC8& aIdentifier, const TDesC8& aValue);

private:	// Attributes

	/**	A buffer containing the pidentifier for the capability.
	*/
	//##ModelId=3C4C41B40132
	HBufC8*		iIdentifier;

	/**	A buffer containing the parameter value for the capability.
	*/
	//##ModelId=3C4C41B40128
	HBufC8*		iValue;

	};

#include "CUnknownCapability.inl"

#endif	// __CUNKNOWNCAPABILITY_H__

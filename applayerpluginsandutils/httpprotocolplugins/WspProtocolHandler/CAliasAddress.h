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

#ifndef __CALIASADDRESS_H__
#define __CALIASADDRESS_H__

#include <e32base.h>
#include <wsp/wsptypes.h>

// Types used for arguments in this file (e.g. TWspBearer) are from the Wap namespace
using namespace Wap;

/**	The CAliasAddress class represents the Alias Address capability as defined
	by the WSP Specification, July 2001.
*/
//##ModelId=3C4C41B2035E
class CAliasAddress : public CBase
	{
public:	// Methods

/**	
	Static factory constructor.
	@since			7.0
	@leave		This function will leave if the object cannot be constructed.
	@param			aBearer		The bearer type.
	@param			aPort		The port number.
	@param			aAddress	A reference to a buffer with the address.
*/
	//##ModelId=3C4C41B203BA
	static CAliasAddress* NewL(TWspBearer aBearer, TUint16 aPort, const TDesC8& aAddress);

/**	
	Standard destructor.
	@since			7.0
*/
	//##ModelId=3C4C41B203B9
	~CAliasAddress();

/**	
	Returns the bearer type.
	@since			7.0
	@return			The bearer type.
*/
	//##ModelId=3C4C41B203B8
	inline TWspBearer GetBearer() const;

/**	
	Returns the port number. If the port number is zero, then the port number is
	invalid.
	@since			7.0
	@return			The port number. A port number of value zero is invalid.
*/
	//##ModelId=3C4C41B203B0
	inline TUint16 GetPort() const;

/**	
	Returns the address.
	@since			7.0
	@return			A const reference to the buffer containg the address.
*/
	//##ModelId=3C4C41B203AF
	inline const TDesC8& GetAddress() const;

private:	// Methods

/**	
	First phase constuctor.
	@since			7.0
	@param			aBearer		The bearer type.
	@param			aPort		The port number.
*/
	//##ModelId=3C4C41B203A4
	CAliasAddress(TWspBearer aBearer, TUint16 aPort);

/**	
	Second phase constructor. Does any necessary allocations.
	@since			7.0
	@param			aAddress	A reference to a buffer with the address.
*/
	//##ModelId=3C4C41B2039A
	void ConstructL(const TDesC8& aAddress);

private:	// Attributes

	/** The bearer type.
	*/
	//##ModelId=3C4C41B20392
	TWspBearer		iBearer;

	/** The port number.
	*/
	//##ModelId=3C4C41B20386
	TUint16			iPort;

	/** A buffer containing the address.
	*/
	//##ModelId=3C4C41B20375
	HBufC8*			iAddress;

	};

#include "CAliasAddress.inl"

#endif	// __CALIASADDRESS_H__

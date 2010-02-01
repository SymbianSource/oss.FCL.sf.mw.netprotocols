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

#ifndef __CHEADERCODEPAGE_H__
#define __CHEADERCODEPAGE_H__

#include <e32base.h>

/**	The CHeaderCodePage class represents the Header Code Page capability as
	defined by the WSP Specification, July 2001.
*/
//##ModelId=3C4C41B300FC
class CHeaderCodePage : public CBase
	{
public:	// Methods

/**	
	Static factory constructor.
	@since			7.0
	@param			aPageCode	The page code assigned to the header code page.
	@param			aPageName	A reference to a buffer containing the header 
								code page name.
*/
	//##ModelId=3C4C41B30143
	static CHeaderCodePage* NewL(TUint8 aPageCode, const TDesC8& aPageName);

/**	
	Standard destructor
	@since			7.0
*/
	//##ModelId=3C4C41B30142
	~CHeaderCodePage();

/**	
	Returns the page code assigned to the header code page.
	@since			7.0
	@return			The page code assigned to the header code page.
*/
	//##ModelId=3C4C41B3013A
	inline TUint8 GetPageCode() const;

/**	
	Returns the header code page name.
	@since			7.0
	@return			A const reference to a buffer containing the header code page name.
*/
	//##ModelId=3C4C41B30139
	inline const TDesC8& GetPageName() const;

private:	// Methods

/**	
	First phase constructor
	@since			7.0
	@param			aPageCode	The page code assigned to the header code page.
*/
	//##ModelId=3C4C41B30130
	CHeaderCodePage(TUint8 aPageCode);

/**	
	Second phase constructor. Does any necessary allocations
	@since			7.0
	@param			aPageName	A reference to a buffer containing the header 
								code page name.
*/
	//##ModelId=3C4C41B3012E
	void ConstructL(const TDesC8& aPageName);

private:	// Attributes

	/** The page code assigned to the header code page specified by iName.
	*/
	//##ModelId=3C4C41B30126
	TUint8		iCode;

	/**	A buffer containing the header code page name.
	*/
	//##ModelId=3C4C41B3011A
	HBufC8*		iName;

	};

#include "CHeaderCodePage.inl"

#endif	// __CHEADERCODEPAGE_H__

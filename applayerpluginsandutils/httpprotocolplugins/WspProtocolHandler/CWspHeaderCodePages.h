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

#ifndef __CWSPHEADERCODEPAGES_H__
#define __CWSPHEADERCODEPAGES_H__

#include <e32base.h>
#include <wsp/mwspheadercodepages.h>

class CHeaderCodePage;

/**	The CWspHeaderCodePages class represents a set of Extended Methods 
	capabilities. It implements the MWspHeaderCodePages interface.
	@see			MWspHeaderCodePages
*/
//##ModelId=3C4C41B201CD
class CWspHeaderCodePages : public CBase, public MWspHeaderCodePages
	{
public:	// Methods

/**	@fn				NewL()
	Static factory constructor.
	@since			7.0
*/
	//##ModelId=3C4C41B20232
	static CWspHeaderCodePages* NewL();

/**	@fn				~CWspHeaderCodePages()
	Standard destructor.
	@since			7.0
*/
	//##ModelId=3C4C41B20231
	~CWspHeaderCodePages();

public:	// Methods from MWspHeaderCodePages

/** @fn				Start() const
	@see			MWspHeaderCodePages
*/
	//##ModelId=3C4C41B2022A
	virtual TInt Start() const;

/** @fn				GetNext(TUint8& aPageCode, TPtrC8& aPageName) const
	@see			MWspHeaderCodePages
*/
	//##ModelId=3C4C41B20227
	virtual TInt GetNext(TUint8& aPageCode, TPtrC8& aPageName) const;

/** @fn				Reset()
	@see			MWspHeaderCodePages
*/
	//##ModelId=3C4C41B2021F
	virtual void Reset();

/** @fn				AddHeaderCodePageL(TUint8 aPageCode, const TDesC8& aPageName)
	@see			MWspHeaderCodePages
*/
	//##ModelId=3C4C41B20215
	virtual void AddHeaderCodePageL(TUint8 aPageCode, const TDesC8& aPageName);

private:	// Methods

/**	@fn				CWspHeaderCodePages()
	First phase constructor.
	@since			7.0
*/
	//##ModelId=3C4C41B20214
	CWspHeaderCodePages();

private:	// Attributes

	/** An array of header code pages.
	*/
	//##ModelId=3C4C41B2020B
	RPointerArray<CHeaderCodePage>		iCodePages;

	/** An index with the current position in the array. Is mutable to maintain
		logical const-ness.
	*/
	//##ModelId=3C4C41B201EB
	mutable TInt						iIndex;

	};

#endif	// __CWSPHEADERCODEPAGES_H__

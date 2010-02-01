// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __BYTELIST_H
#define __BYTELIST_H

#include <e32std.h>
#include <s32stor.h>

class CItem;

//
// class CByteList - First In, First Out temporary data storage for bytes
//
//##ModelId=3B6678E302BB
class CByteList : public CBase
{
public:
	//##ModelId=3B6678E30385
	static CByteList* NewL();
	//##ModelId=3B6678E30384
	static CByteList* NewLC();
	//##ModelId=3B6678E3037C
	virtual ~CByteList();

	//##ModelId=3B6678E3037A
	void AddToListL(HBufC8& aBuffer);
	//##ModelId=3B6678E30371
	void Commit();
	//##ModelId=3B6678E30370
	void RollBack();
	//##ModelId=3B6678E30366
	TInt Byte(TUint8* aByte, TBool aIncrement);
	//##ModelId=3B6678E3035C
	TInt Inc(TInt aCount);

private:
	//##ModelId=3B6678E3035B
	CByteList();
	//##ModelId=3B6678E30352
	void Construct();

private:
	//##ModelId=3B6678E30349
	TSglQue<CItem>     iList;
	//##ModelId=3B6678E30321
	TSglQueIter<CItem> iListIter;
	//##ModelId=3B6678E3030D
	TSglQueIter<CItem> iListCommit;
	//##ModelId=3B6678E302E5
	TInt iByteOffset;
	//##ModelId=3B6678E302DC
	TInt iByteCommit;
};

//
// class CItem - to be used with CByteList
//
//##ModelId=3B6678E40082
class CItem : public CBase
{
public:
	//##ModelId=3B6678E400D2
	static CItem* NewL (const HBufC8& aData);
	//##ModelId=3B6678E400C9
	static CItem* NewLC (const HBufC8& aData); 
	//##ModelId=3B6678E400C1
	void ConstructL (const HBufC8& aData);

	//##ModelId=3B6678E400C0
	virtual ~CItem();
	//##ModelId=3B6678E400BF
	HBufC8* Data() const;
		
	//##ModelId=3B6678E400B6
	static const TInt iOffset;

private:
	//##ModelId=3B6678E400BE
	CItem();

private:	
	//##ModelId=3B6678E400AA
	TSglQueLink iLink;
	//##ModelId=3B6678E400A0
	HBufC8*		iData;
	friend class CList;
};

#endif // __BYTELIST_H

//
// End of file
//

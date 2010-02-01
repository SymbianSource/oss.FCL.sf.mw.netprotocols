// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef __TLDPROPERTIES_H__
#define __TLDPROPERTIES_H__

#include <ineturilistdef.h>
#include <s32mem.h>

/**
This class holds all the associated properties of TLD uri  such as TLD name, List type and Policy data. 

@internalComponent
*/
NONSHARABLE_CLASS ( CTldProperties ) : public CBase
	{
	public:
	static CTldProperties* NewL ( InetUriList::TListType aListType );
	
	~CTldProperties ();
	
	InetUriList::TListType ListType () const;
	const TDesC8& TldName () const;
	const TDesC8& CharacterSet () const;
	
	void SetListType ( InetUriList::TListType aListType );
	void SetTldNameL ( const TDesC8& aTldName );
	void SetCharacterSetL ( const TDesC8& aCharacterSet );
	
	void Clear ();
	
	private:
	CTldProperties ( InetUriList::TListType aListType );

	private:
	RBuf8						iTldName;
	InetUriList::TListType 		iListType;
	RBuf8	 					iCharacterSet;				
	};

#endif // __TLDPROPERTIES_H__

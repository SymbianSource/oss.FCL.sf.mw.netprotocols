// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Uri shortcut parser module header
// 
//

#ifndef __TURISHORTCUTPARSER16_H__
#define __TURISHORTCUTPARSER16_H__

// includes
#include <e32base.h>
#include <e32cmn.h>

class CUri16;


class TUriShortcutParser16
/**
This class provides functionality to create a CUri object from
the user supplied hostname. This class adds the scheme, prefix and/or domain if necessary.

The CUri object is created in the heap.

This class helps to avoid typing "http://", "www.",
".com", ".net" and others.

For example the user supplied string is "mypage" then it will construct a
CUri object with a URI "http://www.mypage.com".

Calling program can set the default domain and scheme.

@internal
@test
@since 9.1
*/
{
    
public:
    
   TUriShortcutParser16();
    
    CUri16* CreateUriL( const TDesC16& aUri ) const;
    
    void SetDefaultDomain( const TDesC16& aUri );
    const TDesC16& DefaultDomain() const;
    void SetDefaultScheme( const TDesC16& aUri );
    const TDesC16& DefaultScheme() const;
    void SetCheckUncommonSchemes( TBool aCheck );
    TBool CheckUncommonSchemes() const;

private:
    static TBool HasScheme( const TDesC16& aUri );
    static TBool HasPath( const TDesC16& aUri );
    static TInt ComponentCount( const TDesC16& aUri );
    
    const TPtrC16 MatchingScheme( const TDesC16& aUri ) const;
    
    //pointer to the default domain
    TPtrC16 iDefaultDomain;
    //pointer to the default scheme
    TPtrC16 iDefaultScheme;
    
    TBool iCheckUncommonSchemes;
    };


#endif    // __TURISHORTCUTPARSER16_H__

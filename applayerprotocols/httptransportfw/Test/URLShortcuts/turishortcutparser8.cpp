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
//

#include <uriutils.h>
#include "turishortcutparser8.h"


_LIT8(KDomainCom,"com");
_LIT8(KLocalhost,"localhost");
_LIT8(KSchemeSeparator,"://");
_LIT(KPanicDescriptor,"Bad Name");

const TUint KUriComponentSeparator      =   '/';
const TUint KUriQueryDelimiter          =   '?';
const TUint KUriFragmentDelimiter       =   '#';
const TUint KSeparatorDot               =   '.';
const TUint KSchemeComponentDelimiter   =   ':';
const TUint KOpeningSquareBracket       =   '[';
const TUint KSizeOfSchemeSeparator      =   3;

_LIT8(KHttp,"http");          
_LIT8(KFtp,"ftp");            
_LIT8(KGopher,"gopher");      
_LIT8(KTelnet,"telnet");      
_LIT8(KWais,"wais");          
_LIT8(KProspero,"prospero");  
_LIT8(KNntp,"nntp");
_LIT8(KWww,"www");          
_LIT8(KNews,"news");


/** Constructor

*/
TUriShortcutParser8::TUriShortcutParser8()
    :iCheckUncommonSchemes( EFalse )
    {
    iDefaultScheme.Set( KHttp );
    iDefaultDomain.Set( KDomainCom );
    }

/** Constructs a CUri8 object from the input string.

Can leave if it cannot allocate memory for a CUri object.
The calling program must destroy the CUri8 object when it is no longer 
needed. 

@return pointer to CUri8 object
@param aUri descriptor to URI
@leave KErrBadName	if the input is invalid

*/    
CUri8* TUriShortcutParser8::CreateUriL( const TDesC8& aUri ) const
    {
    const TInt uriLen = aUri.Length();
    
    if ( uriLen == 0 || UriUtils::HasInvalidChars( aUri ) )
       {
       User::Leave( KErrBadName );
       }
    
    
    TPtrC8 scheme;
    TPtrC8 prefix;
    TPtrC8 tld;

    if ( !HasScheme( aUri ) )
       {
       if ( HasPath( aUri ) )
          {
          scheme.Set( DefaultScheme() );
          }
       else
          {
            
          if ( UriUtils::HostType( aUri ) != UriUtils::ETextHost )
             {
             scheme.Set( DefaultScheme() );
             }
          else
             {
             switch ( ComponentCount( aUri ) )
                   {
                   case 1:
                    
                   if ( aUri.CompareF( KLocalhost ) != 0)
                      {
                      //add prefix, and tld
                      prefix.Set( KWww );
                      tld.Set( DefaultDomain() );
                      }
                       //no break after this case
                        
                   case 2:
                       scheme.Set( DefaultScheme() );
                   break;
                    
                   case 3:
                   default:
                       //it has a prefix
                       scheme.Set( MatchingScheme( aUri ) );
                   break;
                   }
             }
          }
       }
    
    //Calculates the length of the buffer needed to construct the complete URI    
    TInt bufLen = uriLen;
    
    if ( scheme.Length() > 0 )
       {
       bufLen += scheme.Length() + KSizeOfSchemeSeparator;
       }
    
    if ( prefix.Length() > 0 )
       {
       bufLen += prefix.Length() + 1;//dot separator
       }
    
    if ( tld.Length() > 0 )
       {
       bufLen += tld.Length() + 1;//dot separator
       }
    
    //This descriptor must be long enough to contain the entire URI.    
    HBufC8* uriBuf = HBufC8::NewLC( bufLen );
    
    TPtr8 uriDes( uriBuf->Des() );
    
    //Copy the scheme and separator
    if ( scheme.Length() > 0 )
       {
       uriDes.Copy( scheme );
       uriDes.Append( KSchemeSeparator );
       }
    
    //Append the prefix and dot char
    if ( prefix.Length() > 0 )
       {
       uriDes.Append( prefix );
       uriDes.Append( KSeparatorDot );
       }
    
    //Appends user supplied string here
    uriDes.Append( aUri );
    
    //Finally appends the domain
    if ( tld.Length() > 0 )
       {
       uriDes.Append( KSeparatorDot );
       uriDes.Append( tld );
       }
    
    //Constructs the CUri object
    TUriParser8 uriParser;
    User::LeaveIfError( uriParser.Parse( uriDes ) );
    
    CUri8* uri = CUri8::NewL( uriParser );
    
    CleanupStack::PopAndDestroy( uriBuf );
    
    return uri;
    }
    
/** Sets the default domain.

This descriptor should exist for the duration of the use of the class.

@param aDomain Default domain
@panic KErrBadName aDomain should not be empty
*/    
void TUriShortcutParser8::SetDefaultDomain( const TDesC8& aDomain )
    {
    //make sure aScheme contains a valid string
    __ASSERT_ALWAYS( aDomain.Length() > 0, User::Panic( KPanicDescriptor,KErrBadName ) );
    iDefaultDomain.Set( aDomain );
    }
    
/** Gets the default domain    

@return Default domain    
*/    
const TDesC8& TUriShortcutParser8::DefaultDomain() const
    {
    return iDefaultDomain;
    }

    
/** Sets the default scheme    

This descriptor should exist for the duration of the use of the class.

@param aScheme Default scheme
@panic KErrBadName aScheme should not be empty
*/    
void TUriShortcutParser8::SetDefaultScheme( const TDesC8& aScheme )
    {
    //make sure aScheme contains a valid string
    __ASSERT_ALWAYS( aScheme.Length() > 0, User::Panic( KPanicDescriptor,KErrBadName ) );
    iDefaultScheme.Set( aScheme );
    }
    
/** Gets the default scheme    

@return Default scheme    
*/    

const TDesC8& TUriShortcutParser8::DefaultScheme() const
    {
    return iDefaultScheme;
    }

/** If set, it will check for schemes other than http or ftp (gopher, telnet, etc).

By default only http and ftp schemes are recognized.

@param aCheck ETrue for setting    
*/    
void TUriShortcutParser8::SetCheckUncommonSchemes( TBool aCheck )
    {
    iCheckUncommonSchemes = aCheck;
    }
    
/** Gets the CheckUncommonSchemes

@return CheckUncommonSchemes   
*/ TBool TUriShortcutParser8::CheckUncommonSchemes() const
    {
    return iCheckUncommonSchemes;    
    }


/** Tests whether the input has a scheme.

@param aUri descriptor to URI
@return ETrue if it has a scheme    
*/    

TBool TUriShortcutParser8::HasScheme( const TDesC8& aUri )
    {
    int posSchemeComponentDelimiter = aUri.Locate( KSchemeComponentDelimiter );
    
    if ( posSchemeComponentDelimiter > 0 )
       {
       int posOpeningSquareBracket = aUri.Locate( KOpeningSquareBracket );
        
       if ( posOpeningSquareBracket == KErrNotFound  || posOpeningSquareBracket > posSchemeComponentDelimiter )
          {
          //scheme present
          return ETrue;
          }
       }
    return EFalse;
    }
    
/** Tests whether the input has a path.

@param aUri descriptor to URI
@return ETrue if it has a path    
*/
TBool TUriShortcutParser8::HasPath( const TDesC8& aUri )
    {
    
    for ( TInt i = aUri.Length() - 1;i >= 0;--i )
        {
        if ( aUri[i] == KUriComponentSeparator || aUri[i] == KUriQueryDelimiter || aUri[i] == KUriFragmentDelimiter )
           {
           return ETrue;
           }
        }
    return EFalse;
    }
    
/** Gets the number of components in the host name.

@param aUri descriptor to URI
@return Number of components    
@panic KErrBadName URI should not start with a '.'
*/
TInt TUriShortcutParser8::ComponentCount( const TDesC8& aUri )
    {
    //make sure first character is not '.'
    __ASSERT_ALWAYS( aUri[0] != '.', User::Panic( KPanicDescriptor,KErrBadName ) );
    
    TInt nComponentCount = 1;

    for ( TInt i = aUri.Length() - 1;i > 0;i-- )
        {
        if ( aUri[i] == KSeparatorDot )
           {
           nComponentCount++;    
           }
        }
        
    return nComponentCount;
    }
    
/** Gets a matching scheme for the prefix contained in the URI.

@param aUri descriptor to URI
@return Scheme
@panic KErrBadName URI should contain a '.'
*/
const TPtrC8  TUriShortcutParser8::MatchingScheme( const TDesC8& aUri ) const
    {
    TInt pos = aUri.Locate( KSeparatorDot );
    
    //make sure it contains at least one '.'
    __ASSERT_DEBUG( pos > 0 ,User::Panic( KPanicDescriptor,KErrBadName ) );
    
    TPtrC8 prefix( aUri.Left( pos ) );
    TPtrC8 scheme ( DefaultScheme() );

    if ( KWww().CompareF( prefix ) == 0 )
       {
       scheme.Set( KHttp() );
       }
    else if ( KFtp().CompareF( prefix ) == 0 )
            {
            scheme.Set( KFtp() );
            }
    else if( CheckUncommonSchemes() )
            {
                
            if ( KGopher().CompareF( prefix ) == 0 )
                    {
                    scheme.Set( KGopher() );
                    }
            else if ( KTelnet().CompareF( prefix ) == 0 )
                    {
                    scheme.Set( KTelnet() );
                    }
            else if ( KWais().CompareF( prefix ) == 0 )
                    {
                    scheme.Set( KWais() );
                    }
            else if ( KProspero().CompareF( prefix ) == 0 )
                    {
                    scheme.Set( KProspero() );
                    }
            else if ( KNews().CompareF( prefix ) == 0 || KNntp().CompareF( prefix ) == 0 )
                    {
                    scheme.Set( KNntp() );
                    }
            }
            
    return scheme;
    }



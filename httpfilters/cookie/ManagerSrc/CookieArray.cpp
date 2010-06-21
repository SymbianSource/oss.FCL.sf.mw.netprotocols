/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:
*
*/
// INCLUDE FILES
	// System includes
#include <http/thttphdrval.h>
#include <uri8.h>

	// User includes
#include "CookieArray.h"
#include "CookieCommonConstants.h"
#include "CookieLogger.h"
#include "CookieServerPanic.h"

// CONSTANTS

const TUint KCookieAttributeSeparator = ',';	// comma

_LIT8( KCookieSecureScheme, "https" );

// ================= MEMBER FUNCTIONS ======================

// ---------------------------------------------------------
// CCookieArray::CCookieArray
// ---------------------------------------------------------
//
CCookieArray::CCookieArray() : iCookies( KCookieStandardGranularity )
	{
    CLOG( ( ECookieArray, 0, _L( "" ) ) );
    CLOG( ( ECookieArray, 0, _L( "*****************" ) ) );
    CLOG( ( ECookieArray, 0,
					_L( "CCookieArray::CCookieArray" ) ) );
	}

// ---------------------------------------------------------
// CCookieArray::~CCookieArray
// ---------------------------------------------------------
//
CCookieArray::~CCookieArray()
	{
	iCookies.ResetAndDestroy();
	iCookies.Close();
    CLOG( ( ECookieArray, 0,
					_L( "CCookieArray::~CCookieArray ") ) );
    CLOG( ( ECookieArray, 0, _L( "*****************" ) ) );
	}



// ---------------------------------------------------------
// CCookieArray::AddL
// ---------------------------------------------------------
//
void CCookieArray::AddL( const CCookie* aCookie, const TDesC8& aRequestUri )
	{
    CLOG( ( ECookieArray, 0,
			_L( "-> CCookieArray::AddL" ) ) );

    if ( ValidCookieL( *aCookie, aRequestUri ) )
		{
        MakeRoomIfNeeded( aCookie->Size( EFalse ) );
		User::LeaveIfError( iCookies.Append( aCookie ) );
		}
	else
		{
		User::Leave( KErrCorrupt );
		}

    CLOG( ( ECookieArray, 0,
			_L( "<- CCookieArray::AddL" ) ) );
	}

// ---------------------------------------------------------
// CCookieArray::MakeRoomForInsertIfNeededL
// ---------------------------------------------------------
//

void CCookieArray::MakeRoomForInsertIfNeededL( const CCookie* aCookie, const TDesC8& aRequestUri, TInt& aIndex  )
    {
    if( ValidCookieL( *aCookie, aRequestUri ))
    	{
    	TUint32 size = aCookie->Size( EFalse );
	    TUint32 count( Count() );
	    TUint32 totalsize( 0 );
	    for ( TUint32 i=0; i<count; i++ )
	        {
	        totalsize += At( i )->Size( ETrue );
	        }
	    while ( ( ( totalsize + size ) > KCookieMaxFileLength ) && Count() )
	        {
	        totalsize-= At( 0 )->Size( ETrue );	        	          
	        aIndex--;
	        Remove( 0 );
	        if( aIndex < 0) //The insert point in the array is no longer invalid.
	                        //return without making more room. 
	            break;	            
	        };
	    }
	else
		{
		User::Leave( KErrCorrupt );
		}    
    }
	
// ---------------------------------------------------------
// CCookieArray::InsertL
// ---------------------------------------------------------
//
void CCookieArray::InsertL( const CCookie* aCookie, const TInt aIndex )
	{	
    CLOG( ( ECookieArray, 0,
			_L( "-> CCookieArray::InsertL" ) ) );

    User::LeaveIfError( iCookies.Insert( aCookie, aIndex ) );
	
	CLOG( ( ECookieArray, 0,
			_L( "<- CCookieArray::InsertL" ) ) );
	}
	

// ---------------------------------------------------------
// CCookieArray::Remove
// ---------------------------------------------------------
//
void CCookieArray::Remove( TInt aIndex )
    {
    CLOG( ( ECookieArray, 0,
			_L( "-> CCookieArray::Remove" ) ) );
    __ASSERT_DEBUG(
        ( (aIndex>=0) && (aIndex<iCookies.Count()) ),
        PanicServer( EArrayIndexOutOfRange ) );
    delete iCookies[ aIndex ];
    iCookies.Remove( aIndex );

    CLOG( ( ECookieArray, 0,
			_L( "<- CCookieArray::Remove" ) ) );
    }


// ---------------------------------------------------------
// CCookieArray::ClearAllCookies
// ---------------------------------------------------------
//
TInt CCookieArray::ClearAllCookies()
	{
    CLOG( ( ECookieArray, 0,
			_L( "-> CCookieArray::ClearAllCookies" ) ) );
	TInt count = iCookies.Count();

	iCookies.ResetAndDestroy();

    CLOG( ( ECookieArray, 0,
			_L( "<- CCookieArray::ClearAllCookies returned %d" ), count ) );
	return count;
	}


// ---------------------------------------------------------
// CCookieArray::CompareCookies
// ---------------------------------------------------------
//
/*
TInt CCookieArray::CompareCookies( CCookie const & aFirstCookie,
								  CCookie const & aSecondCooke )
	{
	// TBD : the order of the cookies (i.e. which is the first and which
	// is the second) may be unpredictable (I guess, it is). This may have
	// serious impact on the sorting/searching algorithm. TO BE CONSIDERED!!!

	// TBD2 : IMPORTANT!!! Perhaps we should use different TLinearOrder methods
	// depending on whether we want to insert a new element into the array or
	// to find a particular element in the array.
	TInt result;

	TPtrC8 firstPathAttr;
	TPtrC8 secondPathAttr;
	if ( GetFoldedCookieAttr( aFirstCookie, CCookie::EPath, firstPathAttr ) &&
		 GetFoldedCookieAttr( aSecondCooke, CCookie::EPath, secondPathAttr ) )
		{
		// TODO : use PathMatch method here
		result = -1;
		}
	else
		{
		// First, there should be no problem with filling the two attribs,
		// because if they happen to be empty (or missing) in the HTTP request
		// header, then a default value must be assigned to them.
		// Second, if we happen to be here, then we say : the first cookie is
		// less than the second, in other words, the first precedes the second.
		result = -1;
		}

	// if the paths are equal, we have to continue with checking the domain
	// attributes
	if ( !result )
		{
		TPtrC8 firstDomainAttr;
		TPtrC8 secondDomainAttr;
		if ( GetFoldedCookieAttr( aFirstCookie, CCookie::EDomain,
												firstDomainAttr ) &&
			GetFoldedCookieAttr( aSecondCooke, CCookie::EDomain,
												secondDomainAttr ) )
			{
			// TODO : use DomainMatch method here
			}
		}
	else
		{
		// First, there should be no problem with filling the two attribs,
		// because if they happen to be empty (or missing) in the HTTP request
		// header, then a default value must be assigned to them.
		// Second, if we happen to be here, then we say : the first cookie is
		// less than the second, in other words, the first precedes the second.
		result = -1;
		}

	return result;
	}
*/


// ---------------------------------------------------------
// CCookieArray::Count
// ---------------------------------------------------------
//
TInt CCookieArray::Count() const
	{
    CLOG( ( ECookieArray, 0,
			_L( "<- CCookieArray::Count" ) ) );
	return iCookies.Count();
	}


// ---------------------------------------------------------
// CCookieArray::DomainMatch
// Examples :
//	1.) x.y.com domain-matches .y.com
//	2.) x.y.com does NOT domain-match y.com
// ---------------------------------------------------------
//
TBool CCookieArray::DomainMatch( const TDesC8& aDomain1,
                                 const TDesC8& aDomain2,
                                 const TBool aAllowedDot /* = EFalse */ ) const
	{
    CLOG( ( ECookieArray, 0,
			_L( "-> CCookieArray::DomainMatch" ) ) );

    TBool result;
    TPtrC8 domain2noQuotes( CCookie::RemoveQuotes( aDomain2 ) );
    
    // domain2noQuotes should not be NULL or length == zero
    if ( domain2noQuotes.Length() == 0 ) 
	{
		result = EFalse;
	}
	else
	{		
		// match "b.com" with ".b.com", not stricly by spec but thats what other browsers do
	    TPtrC8 d2( domain2noQuotes );
	    if ( domain2noQuotes.Length()>1 && domain2noQuotes[ 0 ] == '.' )
	        {
	        d2.Set(domain2noQuotes.Ptr()+1, domain2noQuotes.Length()-1);
	        }

	    if ( aDomain1.Length() == d2.Length() )
			{
	        result = ( !aDomain1.CompareF( d2 ) ? ETrue : EFalse );
			}
	    else if ( domain2noQuotes.Length() < aDomain1.Length() )
	        {
	        // Considering aDomain1 as "x.y.com", leftDomainFraction will be "x"
	        TPtrC8 leftDomainFraction( aDomain1.Left( aDomain1.Length() -
	                                                                        domain2noQuotes.Length() ) );
	        // Considering aDomain1 as "x.y.com", rightDomainFraction will be
	        // ".y.com"
	        TPtrC8 rightDomainFraction( aDomain1.Right( domain2noQuotes.Length() ) );

	        // x.y.com matches both .y.com and y.com, thats what other browsers do
	        TBool dot = rightDomainFraction[0] == KCookieDomainSeparator ||
	            leftDomainFraction[leftDomainFraction.Length()-1] == KCookieDomainSeparator;
	        if ( dot && !rightDomainFraction.CompareF( domain2noQuotes ) )
	            {
	            result = aAllowedDot || // if dot is allowed doesn't care about them
	                (leftDomainFraction.Locate( KCookieDomainSeparator ) == KErrNotFound );
	            }
	        else
	            {
	            result = EFalse;
	            }
	        }
		else
			{
			result = EFalse;
			}
	}

    CLOG( ( ECookieArray, 0,
			_L( "<- CCookieArray::DomainMatch" ) ) );

    return result;
	}



// ---------------------------------------------------------
// CCookieArray::EffectiveHostNameL
// ---------------------------------------------------------
//
TBool CCookieArray::EffectiveHostNameL( const TDesC8& aHostName,
									   HBufC8*& aEffectiveHostName ) const
	{
    CLOG( ( ECookieArray, 0,
			_L( "-> CCookieArray::EffectiveHostName" ) ) );

	TBool hostNameChanged = EFalse;

	TBuf8<sizeof(KCookieDomainSeparator)> separator;
    separator.Append( KCookieDomainSeparator );
	if ( aHostName.Find( separator ) == KErrNotFound )
		{
		HBufC8* newHostName = HBufC8::NewL( aHostName.Length() +
							KCookieLocalHostNamePostfix().Length() );
		TPtr8 newHostNameDes( newHostName->Des() );

		newHostNameDes.Copy( aHostName );
		newHostNameDes.Append( KCookieLocalHostNamePostfix() );

		aEffectiveHostName = newHostName;

		hostNameChanged = ETrue;
		}
    CLOG( ( ECookieArray, 0,
			_L( "<- CCookieArray::EffectiveHostName" ) ) );
	return hostNameChanged;
	}


// ---------------------------------------------------------
// CCookieArray::GetCookies
// ---------------------------------------------------------
//
TInt CCookieArray::GetCookies( const TDesC8& aRequestUri,
							   RPointerArray<CCookie>& aCookies )
	{
    CLOG( ( ECookieArray, 0,
			_L( "-> CCookieArray::GetCookies for an URI" ) ) );
	TUriParser8 uriParser;
	TInt err = uriParser.Parse( aRequestUri );
	if ( !err )
		{
        // first get the details of the current requestUri,
        // that is, Domain, Path and port
		TPtrC8 requestPath( uriParser.IsPresent( EUriPath ) ?
							uriParser.Extract( EUriPath ) : KNullDesC8() );
		TPtrC8 requestDomain( uriParser.IsPresent( EUriHost ) ?
							uriParser.Extract( EUriHost ) : KNullDesC8() );
		TPtrC8 requestPort( uriParser.IsPresent( EUriPort ) ?
				uriParser.Extract( EUriPort ) : KCookieDefaultRequestPort() );
		TPtrC8 requestScheme( uriParser.IsPresent( EUriScheme ) ?
							uriParser.Extract( EUriScheme ) : KNullDesC8() );

		// now check the existing cookies
        // remove expired ones first, if there are any
        RemoveExpired();
        // and finally, find the cookies...
        TInt count = iCookies.Count();
		for ( TInt i = 0; i < count && err == KErrNone; i++ )
			{
			// Does the cookie have Path attribute?
			TPtrC8 cookiePath;
			if ( !GetFoldedCookieAttr( *iCookies[i],
										CCookie::EPath,
										cookiePath ) )
				{
				continue;
				}

			// Does the cookie have Domain attribute?
			TPtrC8 cookieDomain;
			if ( !GetFoldedCookieAttr( *iCookies[i],
										CCookie::EDomain,
										cookieDomain ) )
				{
				continue;
				}

			TPtrC8 cookiePort;
			GetFoldedCookiePortAttr( *iCookies[i], cookiePort );

			if ( PathMatch( requestPath, cookiePath ) &&
                DomainMatch( requestDomain, cookieDomain, ETrue ) &&
				PortMatch( requestPort, cookiePort ) &&
				SecureMatch( requestScheme, *iCookies[i] ) )
				{
				err = aCookies.Append( iCookies[i] );
				}
			}
        aCookies.Sort( TLinearOrder<CCookie> (CCookieArray::CompareCookiesPath) );
		}
    CLOG( ( ECookieArray, 0,
			_L( "<- CCookieArray::GetCookies  for an URI" ) ) );
	return err;
	}


// ---------------------------------------------------------
// CCookieArray::GetCookies
// ---------------------------------------------------------
//
TInt CCookieArray::GetCookies( RPointerArray<CCookie>& aCookieArray )
	{
    CLOG( ( ECookieArray, 0,
			_L( "-> CCookieArray::GetCookies" ) ) );
	TInt err = KErrNone;
    // Remove expired ones first, if there is any...
    RemoveExpired();
	TInt count = iCookies.Count();
	for ( TInt i = 0; i < count && err == KErrNone; i++ )
		{
		err = aCookieArray.Append( iCookies[i] );
		}

    CLOG( ( ECookieArray, 0,
			_L( "<- CCookieArray::GetCookies" ) ) );
    return err;
	}


// ---------------------------------------------------------
// CCookieArray::operator[]
// ---------------------------------------------------------
//
const CCookie* CCookieArray::operator[](TInt aIndex) const
    {
    return iCookies[aIndex];
    }


// ---------------------------------------------------------
// CCookieArray::At
// ---------------------------------------------------------
//
const CCookie* CCookieArray::At(TInt aIndex) const
    {
    return iCookies[ aIndex ];
    }


// ---------------------------------------------------------
// CCookieArray::DoesAlreadyExists
// ---------------------------------------------------------
//
TBool CCookieArray::DoesAlreadyExists( const CCookie* aCookie,
                                       TInt& aIndex ) const
    {
    CLOG( ( ECookieArray, 0,
			_L( "->CCookieArray::DoesAlreadyExists" ) ) );

    TBool  result( EFalse );

    TInt i(0);
    TInt count( iCookies.Count() );

    for( i = 0; i< count; i++ )
        {
        // first check their names, case-sensitive
        if ( CompareAttribute( aCookie, iCookies[ i ],
                               CCookie::EName, EFalse ) == 0 )
            {
            // their NAME-s are EQUAL
            // now shall check their DOMAIN, case-insensitive (F)
            if ( CompareAttribute( aCookie, iCookies[ i ],
                                   CCookie::EDomain, ETrue ) == 0 )
                {
                // their DOMAIN is matching
                // now check Path, case-sensitive
                if ( CompareAttribute( aCookie, iCookies[ i ],
                                       CCookie::EPath, EFalse ) == 0 )
                    { // their path is the same, too
                    // so it already exists, should OVERWRITE!!!!!!!!
                    result = ETrue;
                    aIndex = i;
                    }
                }
            }
        }
    CLOG( ( ECookieArray, 0,
			_L( "<- CCookieArray::DoesAlreadyExists" ) ) );
    return result;
    }


// ---------------------------------------------------------
// CCookieArray::CookieArray
// ---------------------------------------------------------
//
RPointerArray<CCookie>& CCookieArray::CookieArray()
    {
    return iCookies;
    }


// ---------------------------------------------------------
// CCookieArray::GetFoldedCookieAttr
// ---------------------------------------------------------
//
TBool CCookieArray::GetFoldedCookieAttr( const CCookie& aCookie,
										 CCookie::TCookieAttributeName aAttr,
										 TPtrC8& aAttrDes ) const
	{
    CLOG( ( ECookieArray, 0,
			_L( "-> CCookieArray::GetFoldedCookieAttr" ) ) );
	TBool result = EFalse;
	THTTPHdrVal attrVal;
	if ( aCookie.Attribute( aAttr, attrVal ) == KErrNone )
		{
		if ( attrVal.Type() == THTTPHdrVal::KStrFVal )
			{
			aAttrDes.Set( attrVal.StrF().DesC() );
			result = ETrue;
			}
		else if ( attrVal.Type() == THTTPHdrVal::KStrVal )
			{
			aAttrDes.Set( attrVal.Str().DesC() );
			result = ETrue;
			}
		}

    CLOG( ( ECookieArray, 0,
			_L( "<- CCookieArray::GetFoldedCookieAttr" ) ) );
	return result;
	}


// ---------------------------------------------------------
// CCookieArray::FoldedCookiePortAttr
// ---------------------------------------------------------
//
void CCookieArray::GetFoldedCookiePortAttr( const CCookie& aCookie,
										   TPtrC8& aPort ) const
	{
	// We have to check if the cookie has Port attribute
	if ( !GetFoldedCookieAttr( aCookie, CCookie::EPort, aPort ) )
		{
		aPort.Set( KNullDesC8() );
		}
	}

// ---------------------------------------------------------
// CCookieArray::PathMatch
// Examples :
//	1.) /tec path-matches /tec
//	2.) /tec/waldo path-matches /tec
// ---------------------------------------------------------
//
TBool CCookieArray::PathMatch( const TDesC8& aPath1,
							  const TDesC8& aPath2 ) const
	{
    if( aPath1.Length() == 0 ) // root folder is requested
        return ETrue;
    TPtrC8 path2noQuotes( CCookie::RemoveQuotes( aPath2 ) );
	return ( aPath1.Find( path2noQuotes ) == 0 );
	}


// ---------------------------------------------------------
// CCookieArray::PortMatch
// ---------------------------------------------------------
//
TBool CCookieArray::PortMatch( const TDesC8& aRequestPort,
							  const TDesC8& aCookiePort ) const
	{
    CLOG( ( ECookieArray, 0,
			_L( "-> CCookieArray::PortMatch" ) ) );

    TBool result;

	// cookiePort - it is a 'global' port string that may contain more
	// than one ports, each is separated from the other by
	// KCookieAttributeSeparator. We use it for positioning in the original
	// string, that is, in this string.
	TPtrC8 cookiePort( CCookie::RemoveQuotes( aCookiePort ) );
	if ( !aCookiePort.Length() )
		{
		// if there is no Port attribute in the cookie, then it may be
		// be sent to any ports
		result = ETrue;
		}
	else if ( !aRequestPort.Length() )
		{
		// there is Port attribute, but there is no request-port
		// (which may never happen in theory)
		result = EFalse;
		}
	else
		{
		// portList - it is a local port string that may contain more
		// than one ports, each is separated from the other by
		// KCookieAttributeSeparator. We use it for searching the
		// request-port inside this string.
		TPtrC8 portList( cookiePort );
		// globalPos - for positioning in the 'global' port list string
		// whose size does not change
		TInt globalPos = 0;
		// localPos - for positioning in the local port list string
		// that is getting shorter and shorter
		TInt localPos = 0;

		TBool loopRunning = ETrue;	// loop variable
		while ( loopRunning )
			{
			localPos = portList.Find( aRequestPort );
			if ( localPos == KErrNotFound )
				{
				// if the pattern cannot be located in the string, then
				// the result is EFalse, too
				loopRunning = EFalse;
				}
			else
				{
				globalPos += localPos;
				TInt newLocalPos = localPos + aRequestPort.Length();
				// Assume aRequestPort is 80! Next statement is to filter
				// out "...80"-type matching : "80" or "...,80"
				if ( newLocalPos == portList.Length() &&
					( globalPos == 0 ||	cookiePort[ globalPos - 1 ] ==
										KCookieAttributeSeparator ) )
					{
					// we found the location of the pattern in question
					// and there are no characters beyond this location
					loopRunning = EFalse;
					}
				// Assume aRequestPort is 80! Next statement is to filter
				// out "...80,..."-type matching : "80,..." or
				// "...,80,..."
				else if ( portList[ newLocalPos ] ==
							KCookieAttributeSeparator &&
						( globalPos == 0 ||	cookiePort[ globalPos - 1 ] ==
											KCookieAttributeSeparator ) )
					{
					// we found the pattern and there is a separator char
					// right after it ==> this is the perfect port-match
					loopRunning = EFalse;
					}
				else
					{
					// we have to continue searching from the new position
					portList.Set( portList.Right( portList.Length() -
												newLocalPos ) );
					globalPos += newLocalPos - localPos;
					}
				}
			}

		// ETrue if we found something, EFalse if the result was
		// KErrNotFound
		result = ( localPos != KErrNotFound );
		}
    CLOG( ( ECookieArray, 0,
			_L( "<- CCookieArray::PortMatch" ) ) );
	return result;
	}




// ---------------------------------------------------------
// CCookieArray::SecureMatch
// ---------------------------------------------------------
//
TBool CCookieArray::SecureMatch( const TDesC8& aUriScheme,
								 const CCookie& aCookie ) const
	{
    CLOG( ( ECookieArray, 0,
			_L( "-> CCookieArray::SecureMatch" ) ) );
    TBool result;

	TPtrC8 cookieSecurity;	// the value will never be used
	if ( !GetFoldedCookieAttr( aCookie, CCookie::ESecure, cookieSecurity ) )
		{
		// if there is no ESecure attribute - the cookie is not secure ==>
		// it can be sent even over an insecure connection
		result = ETrue;
		}
	else	// there is ESecure attribute - the cookie is secure
		{
		result = ( aUriScheme.CompareF( KCookieSecureScheme ) == 0 );
		}

    CLOG( ( ECookieArray, 0,
			_L( "<- CCookieArray::SecureMatch" ) ) );
	return result;
	}


// ---------------------------------------------------------
// CCookieArray::ValidCookie
// The validation rules may be different for Netscape and RFC
// cookies :
//	- A. RFC (reject cookie when) :
//		==> 1. Version attribute is missing
//		==> 2. Path attribute is not a prefix of the request-URI
//		==> 3. Domain attribute contains no embedded dots, and the
//			value is not .local
//		==> 4. Effective host name does not domain-match the Domain
//			attribute
//		==> 5.* The request-host is a HDN and has the form HD, where
//			D is the value of the Domain attribute, and H is a string
//			that contains one or more dots
//		==> 6. The Port attribute has a "port-list", and the request-
//			port is not in the list
//	- B. Netscape (reject cookie when) :
//		==> 1.* Only hosts within the specified domain can set a cookie
//			for a domain
//		==> 2.* Any domain that falls within one of the seven special
//			top level domains (COM, EDU, NET, ORG, GOV, MIL and INT)
//			only require two periods. Any other domain requires
//			at least three
//	- C. Common (reject cookie when) :
//		==> 1. NAME or VALUE attribute is missing
// Note : numbers marked with * (asterisk) indicate features to be
// implemented.
// ---------------------------------------------------------
//
TBool CCookieArray::ValidCookieL( const CCookie& aCookie,
								 const TDesC8& aRequestUri ) const
	{
    CLOG( ( ECookieArray, 0,
			_L( "-> CCookieArray::ValidCookieL" ) ) );

    // TODO : validation must take cookie-type into account ==> whether it is
	// a Netscape- or RFC-type cookie
	TBool validCookie = ETrue;
	TBool newCookie = aCookie.FromCookie2();
	THTTPHdrVal hdrVal;

	// C.1. (Name attribute)
	TInt findAttrib = aCookie.Attribute( CCookie::EName, hdrVal );
	if ( findAttrib != KErrNone )
		{
		validCookie = EFalse;
		}

	// C.1. (Value attribute)
	if ( validCookie )
		{
		findAttrib = aCookie.Attribute( CCookie::EValue, hdrVal );
		if ( findAttrib != KErrNone )
			{
			validCookie = EFalse;
			}
		}


	// General rule : if the request-URI is invalid, then the cookie is invalid
	TUriParser8 uriParser;
	if ( validCookie )
		{
		if ( ( uriParser.Parse( aRequestUri ) != KErrNone ) ||
			!uriParser.IsPresent( EUriHost ) )
			{
			validCookie = EFalse;
			}
		}


    // A.1.
	if ( validCookie && newCookie )
		{
		findAttrib = aCookie.Attribute( CCookie::EVersion, hdrVal );
		if ( findAttrib != KErrNone )
			{
			validCookie = EFalse;
			}
		}


	// Domain checking is done here
	// TODO : B.1. and B.2. ==> newCookie verification must be placed deeper
	if ( validCookie && newCookie )
		{
		findAttrib = aCookie.Attribute( CCookie::EDomain, hdrVal );
		// TBD : what if there is no Domain attribute (even defaulted)?
		// Should we bother with filtering out this anomaly?
		if ( findAttrib == KErrNone )
			{
			TPtrC8 cookieDomain( hdrVal.StrF().DesC() );

			TBuf8<sizeof(KCookieDomainSeparator)> separator;
			separator.Append( KCookieDomainSeparator );

			// A.3.
			if ( cookieDomain.Find( separator ) == KErrNotFound ||
				!cookieDomain.CompareF( KCookieLocalHostNamePostfix() ) )
				{
				validCookie = EFalse;
				}

			TPtrC8 requestDomain( uriParser.Extract( EUriHost ) );

			// A.4.
			if ( validCookie )
				{
				HBufC8* effHostName = NULL;
				if ( EffectiveHostNameL( requestDomain, effHostName ) )
					{
					validCookie = ( DomainMatch( *effHostName,
									cookieDomain ) ? ETrue : EFalse );

					delete effHostName;
					}
				else
					{
					validCookie = ( DomainMatch( requestDomain,
									cookieDomain ) ? ETrue : EFalse );
					}
				}

			// A.5.
			if ( validCookie )
				{
				// TODO
				}
			}
		}

	// A.2. (Path)
	// Note : although Path attribute may be used in a Netscape-cookie as well,
	// we need not bother with it as the Netscape specification do not say
	// anything about cookie-path validation
	if ( validCookie && newCookie )
		{
		findAttrib = aCookie.Attribute( CCookie::EPath, hdrVal );
		if ( findAttrib == KErrNone )
			{
			TPtrC8 cookiePath( hdrVal.StrF().DesC() );
			TPtrC8 requestPath( uriParser.IsPresent( EUriPath ) ?
							uriParser.Extract( EUriPath ) : KNullDesC8() );
   			validCookie = ( PathMatch( requestPath, cookiePath ) ?
   			    ETrue : EFalse );
			}
		}

	// A.6. (Port)
	// Note : it is RFC2965-specific
	if ( validCookie && newCookie )
		{
		TPtrC8 cookiePort;
		GetFoldedCookiePortAttr( aCookie, cookiePort );
		TPtrC8 requestPort( uriParser.IsPresent( EUriPort ) ?
				uriParser.Extract( EUriPort ) : KCookieDefaultRequestPort() );

		validCookie = ( PortMatch( requestPort, cookiePort ) ? ETrue
															: EFalse );
		}

    CLOG( ( ECookieArray, 0,
			_L( "<- CCookieArray::ValidCookieL" ) ) );
	return validCookie;
	}




// ---------------------------------------------------------
// CCookieArray::RemoveExpired
// ---------------------------------------------------------
//
void CCookieArray::RemoveExpired()
    {
    CLOG( ( ECookieArray, 0,
			_L( "-> CCookieArray::RemoveExpired" ) ) );
    TInt i( 0 );

    while( i < iCookies.Count() )
        {
        if ( iCookies[ i ]->Expired() )
            {
            delete iCookies[ i ];
            iCookies.Remove( i );
            }
        else
            {
            i++;
            }
        };
    CLOG( ( ECookieArray, 0,
			_L( "<- CCookieArray::RemoveExpired" ) ) );
    }



// ---------------------------------------------------------
// CCookieArray::RemoveNonPersistent
// ---------------------------------------------------------
//
void CCookieArray::RemoveNonPersistent()
    {
    CLOG( ( ECookieArray, 0,
			_L( "-> CCookieArray::RemoveNonPersistent" ) ) );
    TInt i( 0 );

    while( i < iCookies.Count() )
        {
        if (!iCookies[ i ]->Persistent() || iCookies[ i ]->Expired() )
            {
            delete iCookies[ i ];
            iCookies.Remove( i );
            }
        else
            {
            i++;
            }
        };
    CLOG( ( ECookieArray, 0,
			_L( "<- CCookieArray::RemoveNonPersistent" ) ) );
    }

// ---------------------------------------------------------
// CCookieArray::CompareAttribute
// ---------------------------------------------------------
//
TInt CCookieArray::CompareAttribute( const CCookie* aCookie1,
                                     const CCookie* aCookie2,
                                     CCookie::TCookieAttributeName aAttr,
                                     TBool aFolded ) const
    {
    CLOG( ( ECookieArray, 0,
			_L( "-> CCookieArray::CompareAttribute" ) ) );

    TInt result( -1 );

    THTTPHdrVal val1;
    THTTPHdrVal val2;


    if ( aCookie1->Attribute( aAttr, val1 ) == KErrNone )
        {
        if ( aCookie2->Attribute( aAttr, val2 ) == KErrNone )
            {
            if ( aFolded )
                {
                if( val1.Type() == THTTPHdrVal::KStrFVal )
                    result = val1.StrF().DesC().CompareF( val2.StrF().DesC() );
                else if( val1.Type() == THTTPHdrVal::KStrVal )
                    result = val1.Str().DesC().CompareF( val2.Str().DesC() );
                }
            else
                {
                if( val1.Type() == THTTPHdrVal::KStrFVal )
                    result = val1.StrF().DesC().Compare( val2.StrF().DesC() );
                else if( val1.Type() == THTTPHdrVal::KStrVal )
                    result = val1.Str().DesC().Compare( val2.Str().DesC() );
                }
            }
        }
    CLOG( ( ECookieArray, 0,
			_L( "<- CCookieArray::CompareAttribute" ) ) );
    return result;
    }


// ---------------------------------------------------------
// CCookieArray::CompareCookiesPath
// ---------------------------------------------------------
//
TInt CCookieArray::CompareCookiesPath( const CCookie & aFirstCookie,
                                        const CCookie & aSecondCookie )
{
    TInt result( -1 );

    THTTPHdrVal val1;
    THTTPHdrVal val2;

    if ( aFirstCookie.Attribute( CCookie::EPath, val1 ) == KErrNone )
        {
        if ( aSecondCookie.Attribute( CCookie::EPath, val2 ) == KErrNone )
            {
            TInt length1 = val1.StrF().DesC().Length();
            TInt length2 = val2.StrF().DesC().Length();
            if( length1 > length2 )
                {
                result = -1;
                }
            else if( length1 < length2)
                {
                result = 1;
                }
            else
                {
                result = 0;
                }
            }
        }

    return result;
}

// ---------------------------------------------------------
// CCookieArray::MakeRoomIfNeeded
// ---------------------------------------------------------
//
void CCookieArray::MakeRoomIfNeeded( TUint32 aSize )
    {
    TUint32 count( Count() );
    TUint32 totalsize( 0 );
    for ( TUint32 i=0; i<count; i++ )
        {
        totalsize += At( i )->Size( ETrue );
        }
    while ( ( ( totalsize + aSize ) > KCookieMaxFileLength ) && Count() )
        {
        totalsize-= At( 0 )->Size( ETrue );
        Remove( 0 );
        };
    }
// ---------------------------------------------------------
// CCookieArray::ReserveL
// ---------------------------------------------------------
//
void CCookieArray::ReserveL( TInt aNumberOfCookies )
    {
    iCookies.ReserveL( aNumberOfCookies );
    }
// ---------------------------------------------------------
// CCookieArray::GetCookies
// ---------------------------------------------------------
//
TInt CCookieArray::GetCookies( const TDesC8& aRequestUri,
                               RPointerArray<CCookie>& aCookies, TBool& aFound )
    {
    CLOG( ( ECookieArray, 0,
            _L( "-> CCookieArray::GetCookies for an URI" ) ) );
    TUriParser8 uriParser;
    TInt err = uriParser.Parse( aRequestUri );
    if ( !err )
        {
        // first get the details of the current requestUri,
        // that is, Domain, Path and port
        TPtrC8 requestPath( uriParser.IsPresent( EUriPath ) ?
                            uriParser.Extract( EUriPath ) : KNullDesC8() );
        TPtrC8 requestDomain( uriParser.IsPresent( EUriHost ) ?
                            uriParser.Extract( EUriHost ) : KNullDesC8() );
        TPtrC8 requestPort( uriParser.IsPresent( EUriPort ) ?
                uriParser.Extract( EUriPort ) : KCookieDefaultRequestPort() );
        TPtrC8 requestScheme( uriParser.IsPresent( EUriScheme ) ?
                            uriParser.Extract( EUriScheme ) : KNullDesC8() );

        // now check the existing cookies
        // remove expired ones first, if there are any
        RemoveExpired();
        // and finally, find the cookies...
        TInt count = iCookies.Count();
        for ( TInt i = 0; i < count && err == KErrNone; i++ )
            {
            // Does the cookie have Path attribute?
            TPtrC8 cookiePath;
            if ( !GetFoldedCookieAttr( *iCookies[i],
                                        CCookie::EPath,
                                        cookiePath ) )
                {
                continue;
                }

            // Does the cookie have Domain attribute?
            TPtrC8 cookieDomain;
            if ( !GetFoldedCookieAttr( *iCookies[i],
                                        CCookie::EDomain,
                                        cookieDomain ) )
                {
                continue;
                }

            TPtrC8 cookiePort;
            GetFoldedCookiePortAttr( *iCookies[i], cookiePort );

            if ( PathMatch( requestPath, cookiePath ) &&
                DomainMatch( requestDomain, cookieDomain, ETrue ) &&
                PortMatch( requestPort, cookiePort ) &&
                SecureMatch( requestScheme, *iCookies[i] ) )
                {
                CCookie* clone = CCookie::CloneL( *iCookies[i],requestDomain,requestPath,requestPort );
                CleanupStack::PushL( clone );
                err = aCookies.Append(clone);
                CleanupStack::Pop(clone);
                aFound = ETrue;
                }
            }
        aCookies.Sort( TLinearOrder<CCookie> (CCookieArray::CompareCookiesPath) );
        }
    
    

    CLOG( ( ECookieArray, 0,
            _L( "<- CCookieArray::GetCookies  for an URI" ) ) );
    return err;
    }

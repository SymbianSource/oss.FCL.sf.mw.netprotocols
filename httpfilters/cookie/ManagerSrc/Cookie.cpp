/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  implementation of class CCookie
*               
*
*/


// INCLUDE FILES
	// System includes
#include <httpstringconstants.h>
#include <http/rhttpheaders.h>
#include <http/rhttpsession.h>
#include <uri8.h>

	// User includes
#include "cookie.h"
#include "CookieCommonConstants.h"


const TUint8 KDayOfWeekSeparator    = ',';
// const TUint8 KTimeSeparator         = ':';

_LIT8 ( KUT, "UT" );
_LIT8 ( KGMT, "GMT" );
_LIT8 ( KEST, "EST" );
_LIT8 ( KEDT, "EDT" );
_LIT8 ( KCST, "CST" );
_LIT8 ( KCDT, "CDT" );
_LIT8 ( KMST, "MST" );
_LIT8 ( KMDT, "MDT" );
_LIT8 ( KPST, "PST" );
_LIT8 ( KPDT, "PDT" );

const TUint8 KLocalTimeZoneSeparator1        = '+';
const TUint8 KLocalTimeZoneSeparator2        = '-';

const TInt KDateTokenCount = 3;
const TInt KTimeTokenCount = 3;
const TInt KAsciiTokenCount = 4;

_LIT8( KLocalSeparators, " -");
_LIT8( KLocalTimeSeparator, ":");

_LIT8( KLocalMonthNames, "Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec" );




// ---------------------------------------------------------
// CCookie::NewL
// ---------------------------------------------------------
//
EXPORT_C CCookie* CCookie::NewL( RHTTPHeaders aRequestHeaders, TInt aPartIndex,
								RStringF aFieldName, RStringPool aStringPool,
								const TUriC8& aUri )
	{
	CCookie* self = new (ELeave) CCookie( aStringPool );

	CleanupStack::PushL( self );

	self->ConstructL( aRequestHeaders, aPartIndex, aFieldName, aUri );

	CleanupStack::Pop();	// self

	return self;
	}


// ---------------------------------------------------------
// CCookie::NewL
// ---------------------------------------------------------
//
EXPORT_C CCookie* CCookie::NewL( RStringPool aStringPool )
	{
	return new (ELeave) CCookie( aStringPool );
	}
	

// ---------------------------------------------------------
// CCookie::~CCookie
// ---------------------------------------------------------
//
EXPORT_C CCookie::~CCookie()
	{
	// go through each attribute closing any attribute strings	
	TInt numAttributes = iAttributes.Count();
	for ( TInt ii = 0; ii < numAttributes; ++ii )
		{
		iAttributes[ii].Close();
		}

	iAttributes.Close();
	}


// ---------------------------------------------------------
// CCookie::AddDefaultPathL
// ---------------------------------------------------------
//
void CCookie::AddDefaultPathL( const TUriC8& aUri )
	{
	// RFC2965 : Defaults to the path of the request URL that generated the
	// Set-Cookie2 response, up to and including the right-most /.
	// Note : there is a contradiction as we must not include the right-most /
	// per RFC2109, which is obsolete. Decision : we support RFC2965.
	const TUint8 KCookiePathSeparator = '/';
	TPtrC8 requestPath;
	if ( aUri.IsPresent( EUriPath ) )
		{
		requestPath.Set( aUri.Extract( EUriPath ) );

		TInt sepPos = requestPath.LocateReverse( KCookiePathSeparator );
		// if / is not the last character
		if ( 0 <= sepPos && ( ( sepPos + 1 != requestPath.Length() ) ) )
			{
			// then remove characters after the right-most /
			requestPath.Set( requestPath.Left( sepPos + 1 ) );
			}
		}
	else
		{
		requestPath.Set( &KCookiePathSeparator,
						sizeof( KCookiePathSeparator ) );
		}

    RStringF defPath = iStringPool.OpenFStringL( requestPath );
    THTTPHdrVal defPathVal( defPath );
	SetAttribute( EPath, defPathVal, ETrue );// ignore the result 
    defPath.Close();
	}

// ---------------------------------------------------------
// CCookie::AddToRequestL
// ---------------------------------------------------------
//
EXPORT_C void CCookie::AddToRequestL( RHTTPHeaders aRequestHeaders,
									 TInt aPartIndex ) const
	{
	const TStringTable& commonStringTable = RHTTPSession::GetTable();

	// We're writing out the "Cookie:" string
	RStringF cookieString =
					iStringPool.StringF( HTTP::ECookie, commonStringTable );
	aRequestHeaders.SetFieldL( cookieString, THTTPHdrVal() );

	THTTPHdrVal hVal;

	// We're writing out the Version attribute - it is a serious error if this
	// attribute is missing
	if ( Attribute( CCookie::EVersion, hVal ) == KErrNone )
		{
		aRequestHeaders.SetParamL( cookieString, 
							iStringPool.StringF( HTTP::EVersion,
												commonStringTable ),
							hVal,
							aPartIndex );
		}
	else
		{
		if ( iSetCookie2 )
			{
			// if this is a new-style cookie (it is from a Set-Cookie2 header),
			// then it must have a Version attribute.
			// TBD : or should we insert a '$Version=1' string instead?
			User::Leave( KErrCorrupt );
			}
		}

	// We're writing out the NAME attribute - it is a serious error if this
	// attribute is missing
	User::LeaveIfError( Attribute( CCookie::EName, hVal ) );
	aRequestHeaders.SetParamL( cookieString,
							iStringPool.StringF( HTTP::ECookieName,
												commonStringTable ), 
							hVal,
							aPartIndex );

	// We're writing out the VALUE attribute - it is a serious error if this
	// attribute is missing
	User::LeaveIfError( Attribute( CCookie::EValue, hVal ) );
	aRequestHeaders.SetParamL( cookieString,
							iStringPool.StringF( HTTP::ECookieValue,
												commonStringTable ),
							hVal,
							aPartIndex );

	// Writing out the rest : domain, path and port. These are not mandatory,
	// thus we do not leave if they are missing.
	// Writing out Domain attribute
	if ( Attribute( CCookie::EDomain, hVal ) == KErrNone && hVal.StrF().DesC().Length() > 0 )
        {
		aRequestHeaders.SetParamL( cookieString,
								iStringPool.StringF( HTTP::EDomain,
													commonStringTable ),
								hVal,
								aPartIndex );
        }

	// Writing out Path attribute
	if ( Attribute( CCookie::EPath, hVal ) == KErrNone )
        {
		aRequestHeaders.SetParamL( cookieString,
								iStringPool.StringF( HTTP::EPath,
													commonStringTable ),
								hVal,
								aPartIndex );
        }

	// Writing out Port attribute
	if ( Attribute( CCookie::EPort, hVal ) == KErrNone )
        {
		aRequestHeaders.SetParamL( cookieString,
								iStringPool.StringF( HTTP::ECookiePort,
													commonStringTable ),
								hVal,
								aPartIndex );
        }
	}


// ---------------------------------------------------------
// CCookie::Attribute
// ---------------------------------------------------------
//
EXPORT_C TInt CCookie::Attribute
							( CCookie::TCookieAttributeName aAttributeName,
								THTTPHdrVal& aAttributeVal) const
	{
	TCookieAttribute attribute;
	if ( FindAttribute( aAttributeName, attribute ) != KErrNotFound )
		{
		aAttributeVal = attribute.Value();
		return KErrNone;
		}

	return KErrNotFound;
	}


// ---------------------------------------------------------
// CCookie::SetAttribute
// ---------------------------------------------------------
//
EXPORT_C TInt CCookie::SetAttribute
							( CCookie::TCookieAttributeName aAttributeName, 
								const THTTPHdrVal& aAttributeVal )
	{
	return SetAttribute( aAttributeName, aAttributeVal, EFalse );
	}



// ---------------------------------------------------------
// CCookie::Attribute
// ---------------------------------------------------------
//
TInt CCookie::Attribute( TCookieAttributeName aAttributeName,
						 THTTPHdrVal& aAttributeVal,
						 TBool& aDefaulted ) const
	{
	TCookieAttribute attribute;
	if ( FindAttribute( aAttributeName, attribute ) != KErrNotFound )
		{
		aAttributeVal = attribute.Value();
		aDefaulted = attribute.Defaulted();
		return KErrNone;
		}

	return KErrNotFound;
	}


// ---------------------------------------------------------
// CCookie::SetAttribute
// ---------------------------------------------------------
//
TInt CCookie::SetAttribute(  TCookieAttributeName aAttributeName,
							const THTTPHdrVal& aAttributeVal,
							TBool aDefaulted )
{
	TInt result( KErrNone );

	TCookieAttribute attribute;
	TInt index = FindAttribute( aAttributeName, attribute );
	if ( index != KErrNotFound )
		{
		result = KErrAlreadyExists;
		}
	else
		{
		TCookieAttribute newAttribute( aAttributeName, 
									   aAttributeVal, aDefaulted );

		result = iAttributes.Append( newAttribute );
		if ( result == KErrNone )
			{
			iSize += ( KCookieAttributePrefixLength + newAttribute.Size() + 
                       KCookieAttributeDefaultedLength + 
                       KCookieAttributeFoundLength );
			}
		else
			{
			newAttribute.Close();
			}
		}
	return result;
}

// ---------------------------------------------------------
// CCookie::CopyFromL
// ---------------------------------------------------------
//
void CCookie::CopyFromL( const CCookie& aCopyFrom  )
    {
	THTTPHdrVal attributevalue;
    TBool aDefaulted( EFalse );

    if ( aCopyFrom.Attribute( EName, attributevalue, aDefaulted ) != KErrNotFound )
        {
        SetAttribute( EName, attributevalue, aDefaulted );
        }

	if ( aCopyFrom.Attribute( EValue, attributevalue, aDefaulted ) != KErrNotFound )
        {
        SetAttribute( EValue, attributevalue, aDefaulted );
        }

	if ( aCopyFrom.Attribute( EComment, attributevalue, aDefaulted ) != KErrNotFound )
        {
        SetAttribute( EComment, attributevalue, aDefaulted );
        }

	if ( aCopyFrom.Attribute( ECommentURI, attributevalue, aDefaulted ) != KErrNotFound )
        {
        SetAttribute( ECommentURI, attributevalue, aDefaulted );
        }

	if ( aCopyFrom.Attribute( EDiscard, attributevalue, aDefaulted ) != KErrNotFound )
        {
        SetAttribute( EDiscard, attributevalue, aDefaulted );
        }

	if ( aCopyFrom.Attribute( EDomain, attributevalue, aDefaulted ) != KErrNotFound )
        {
        SetAttribute( EDomain, attributevalue, aDefaulted );
        }

	if ( aCopyFrom.Attribute( EMaxAge, attributevalue, aDefaulted ) != KErrNotFound )
        {
        SetAttribute( EMaxAge, attributevalue, aDefaulted );
        }

	if ( aCopyFrom.Attribute( EPath, attributevalue, aDefaulted ) != KErrNotFound )
        {
        SetAttribute( EPath, attributevalue, aDefaulted );
        }

	if ( aCopyFrom.Attribute( EPort, attributevalue, aDefaulted ) != KErrNotFound )
        {
        SetAttribute( EPort, attributevalue, aDefaulted );
        }

	if ( aCopyFrom.Attribute( ESecure, attributevalue, aDefaulted ) != KErrNotFound )
        {
        SetAttribute( ESecure, attributevalue, aDefaulted );
        }

	if ( aCopyFrom.Attribute( EVersion, attributevalue, aDefaulted ) != KErrNotFound )
        {
        SetAttribute( EVersion, attributevalue, aDefaulted );
        }

	if ( aCopyFrom.Attribute( EExpires, attributevalue, aDefaulted ) != KErrNotFound )
        {
        SetAttribute( EExpires, attributevalue, aDefaulted );
        }

    SetCookie2( aCopyFrom.FromCookie2() );

    }




// ---------------------------------------------------------
// CCookie::StringPool
// ---------------------------------------------------------
//
RStringPool CCookie::StringPool()
    {
    return iStringPool;
    }



// ---------------------------------------------------------
// CCookie::Expired
// ---------------------------------------------------------
//
TBool CCookie::Expired() const
	{
    // the cookie has expired if:
    // 1. it's Age > Max-Age
    // 2. current_date > expires
    // Expires is in Netscape cookie only !!!

    TBool retval( EFalse );
	THTTPHdrVal attrVal;
    TInt err( KErrNone );

	if ( FromNetscapeVersion() && ( Attribute( CCookie::EExpires, attrVal ) == KErrNone ))
        {
	    if ( attrVal.Type() == THTTPHdrVal::KDateVal )
		    {
            TDateTime expires;
		    expires = attrVal.DateTime();
            TTime Expirytime( expires );
            TTime NowTime;
            NowTime.UniversalTime();
            if ( Expirytime <= NowTime )
                { // EXPIRED !!!!
                retval = ETrue;
                }
            }
        else
            {
		    if ( attrVal.Type() == THTTPHdrVal::KStrFVal )
                {
                TRAP_IGNORE( retval = ExpiredNetscapeL( attrVal ) );
                // The only real error is KErrNoMemory, 
                // so we simply ignore the error here
                }
            }
		}
    else
        {
        if ( Attribute( CCookie::EMaxAge, attrVal ) == KErrNone )
            {
            TInt MaxAge(0);
            if ( attrVal.Type() == THTTPHdrVal::KTIntVal )
                {
                MaxAge = attrVal.Int();
                }
            else
                {
                TPtrC8 MaxAgePtr=CCookie::RemoveQuotes( attrVal.StrF().DesC() );
                TLex8 lex( MaxAgePtr );
                err = lex.Val( MaxAge );
                }
            if ( err == KErrNone )
                {
                        // it's Age > Max-Age?
                        // Age = ( now_time - EDate )
                        // TTime.SecondsFrom(TTime)
                        // UniversalTime.SecondsFrom( EDate_time ): real age
                        TTime NowTime;
                        NowTime.UniversalTime();
                        TTimeIntervalSeconds CurrAgeInterval;
                        err = NowTime.SecondsFrom( iReceivedTime, CurrAgeInterval );
                        // now compare to specified Max-Age
                        if ( MaxAge <= CurrAgeInterval.Int() )
                            { // EXPIRED !!!!
                            retval = ETrue;
                            }
                        
                    
                }
			}
		}
	return retval;
	}
// ---------------------------------------------------------
// CCookie::RemoveQuotes
// ---------------------------------------------------------
//
 TPtrC8 CCookie::RemoveQuotes( const TDesC8& aDes ) 
	{
   
	TInt firstChar = 0;					// position of the first character
	TInt lastChar = aDes.Length() - 1;	// position of the last character
	TPtrC8 result;
	if ( lastChar < 0 )	// that is, aDes has a length of zero
		{
		result.Set( aDes );	// the result is the same
		}
	else
		{
		TUint8 quotationMark = '"';

		if ( aDes[ firstChar ] == quotationMark )
			{
			firstChar++;
			}

		if ( aDes[ lastChar ] == quotationMark )
			{
			lastChar--;
			}

		if ( lastChar < firstChar )
			{
			result.Set( KNullDesC8() );
			}
		else
			{
			result.Set( aDes.Mid( firstChar, lastChar - firstChar + 1 ) );
			}
		}

    return result;
	}



// ---------------------------------------------------------
// CCookie::ExpiredNetscapeL
// ---------------------------------------------------------
//
TBool CCookie::ExpiredNetscapeL( THTTPHdrVal aAttrVal) const
    {
    TBool retval( EFalse );
    TPtrC8 datePtr8( aAttrVal.StrF().DesC() );
    TInt dateLen( datePtr8.Length() );
    if( dateLen == 0 )
        {
        // blank "expires" attribute evaluate as not expired
        // additional check is done in Persistent() function
        return EFalse;
        }
    HBufC8* temp1 = HBufC8::NewLC( dateLen );
    TPtr8 temp1Des( temp1->Des() );
    temp1Des.Copy( datePtr8 );
    HBufC8* temp2;
    HBufC8 *desDate;
    
    // ascii time format
    // asctime-date = wkday SP date3 SP time SP 4DIGIT
    // date3        = month SP ( 2DIGIT | ( SP 1DIGIT ))
    TBool asciiTime = EFalse;
    
    if(KErrNotFound == temp1->Locate( KDayOfWeekSeparator ))
    {
       asciiTime=ETrue;
    }
    
    if(asciiTime)  
    {
       temp2 = HBufC8::NewLC( temp1->Length() );
       temp2 = temp1;
    }
    else
    {
       temp2 = GetCleanedDateTimeLC( temp1 );		
    }
    
    TInt cleanupNum( 2 );  // temp1, temp2
    TPtr8 temp2Des( temp2->Des() );
    temp2Des.Trim();

    TDateTime tempDate( 1997, EJanuary, 1, 0, 0, 0, 0 );
    TLex8 lex;
    TInt error( KErrNone );
    TInt tempInt;
    TInt sepPos[4];

    // date string is formatted as: "DD-Mon-YYYY HH:MM:SS"
    // and must be evaluated as a GMT time

    // parsing Date
    // find token's position
    TInt token = FindTokens( temp2Des, KLocalSeparators, asciiTime?KAsciiTokenCount:KDateTokenCount, sepPos );
    
    if( token < (asciiTime?KAsciiTokenCount:KDateTokenCount -1) )
       	{
       	CleanupStack::PopAndDestroy( cleanupNum ); // temp1, temp2
       	return ETrue;
       	}
       	
    if(!asciiTime)
		{
    	if( token == KDateTokenCount - 1 )   // if no Time is specified
        	{
        	sepPos[2] = temp2Des.Length();
        	}
    	}

    // initizalize Date
    
    if(asciiTime)
    	{
    	desDate=temp2Des.AllocLC();
    	}
    else
    	{
    	desDate = temp2Des.Left( sepPos[2] ).AllocLC();
    	}
   
    cleanupNum++;  // temp1, temp2, desDate
    desDate->Des().Trim();

    // day
    if(asciiTime)
    	{
    	lex = desDate->Des().Mid( sepPos[1]+1,sepPos[2]- sepPos[1] - 1);		
    	}
    else
    	{
    	lex = desDate->Des().Left( sepPos[0] );	
    	}
    
    error += lex.Val( tempInt );
    error += tempDate.SetDay( tempInt - 1 );  // day's offset is 0!

    // year
	if(asciiTime)
		{
		lex = desDate->Des().Right( desDate->Length() - sepPos[3] - 1 );	
		}
	else
		{
		lex = desDate->Des().Right( desDate->Length() - sepPos[1] - 1 );		
		}
    error += lex.Val( tempInt );
    if(tempInt < 100)  // make sure it has four digit length
        {
        tempInt += 2000;
        }
    error += tempDate.SetYear( tempInt );

    // month
    TPtrC8 monthPtr = desDate->Des().Mid( sepPos[0] + 1, sepPos[1] - sepPos[0] - 1 );
    TPtrC8 monthNamesPtr( KLocalMonthNames );
    TInt monthNum = monthNamesPtr.Find( monthPtr );
    if( monthNum == KErrNotFound )
        {
        // probably using digits for month rather than its English name
        lex = monthPtr;
        error += lex.Val( monthNum );
        monthNum--; // month's offset is 0!
        if ( (monthNum < 0) || (monthNum > 11) )
            // invalid month, cookie must be removed
            {
            CleanupStack::PopAndDestroy( cleanupNum ); // temp1, temp2, desDate
            return ETrue;
            }
        }
    // now we have a position for month's English name
    else if( (monthNum % 4 ) != 0 )
        // something strange happened, cookie must be removed
        {
        CleanupStack::PopAndDestroy( cleanupNum ); // temp1, temp2, desDate
        return ETrue;
        }
    else
        {
        // divide by 4 to convert from index into month's English name to month number, zero relative
        monthNum >>= 2;
        }
    error += tempDate.SetMonth( TMonth( monthNum ) );  // month's offset is 0!

    // parsing Time
    if( (sepPos[2] != temp2Des.Length()) &&  // Time is present
        (error == KErrNone) )  // and no error yet
        {
        	HBufC8 *desTime;
        // initialize Time
    if(asciiTime)
    	{
    	desTime=temp2Des.Mid( sepPos[2] + 1, sepPos[3] - sepPos[2] - 1 ).AllocLC();
    	}
    else
        {
    	desTime = temp2Des.Right( temp2Des.Length() - sepPos[2] - 1 ).AllocLC();
        }
        cleanupNum++;  // 4
        desTime->Des().Trim();

        token = FindTokens( desTime->Des(), KLocalTimeSeparator, KTimeTokenCount, sepPos );
        if(token >= KTimeTokenCount - 1)  // 2, 3 tokens found
            {
            // hour
            lex = desTime->Des().Left( sepPos[0] );
            error += lex.Val( tempInt );
            error += tempDate.SetHour( tempInt );

            // second
            lex = desTime->Des().Right( desTime->Length() - sepPos[1] - 1 );
            error += lex.Val( tempInt );
            error += tempDate.SetSecond( tempInt );

            // minute
            lex = desTime->Des().Mid( sepPos[0] + 1, sepPos[1] - sepPos[0] - 1 );
            error += lex.Val( tempInt );
            error += tempDate.SetMinute( tempInt );
            }
        }

    // is parsing OK?
    if( error == KErrNone )
        {
        TTime expiryTime( tempDate );
        THTTPHdrVal aAttributeVal;
    	TCookieAttribute attribute;
    	TDateTime ResponseDate;
		if ( FindAttribute( EDate, attribute ) != KErrNotFound )
			{
			aAttributeVal = attribute.Value();
			ResponseDate=aAttributeVal.DateTime();
			}
        TTime RespTime(ResponseDate);
        TTimeIntervalMicroSeconds diffMsec=expiryTime.MicroSecondsFrom(RespTime);
        TTime nowTime;
        nowTime.UniversalTime();
//        TDateTime temptime2;
        TTime newexpiryTime = iReceivedTime + diffMsec;
//        temptime2 = nowTime.DateTime();
        if( newexpiryTime <= nowTime )
            { // EXPIRED !!!!
            retval = ETrue;
            }
        }
    else  // invalid Date is expired!
        {
        retval = ETrue;
        }
    CleanupStack::PopAndDestroy( cleanupNum ); // temp1, temp2, desDate, desTime
    return retval;
    }


// ---------------------------------------------------------
// CCookie::FindTokens
// ---------------------------------------------------------
//
TInt CCookie::FindTokens( const TDesC8& aString, 
                         const TDesC8& aTokenSeparators, 
                         const TInt aTokenCount, 
                         TInt* aTokens ) const
    {
    TLex8 lex = aString;
    TChar ch;
    TInt tokenCount( 0 ), ii;
    for(ii=0; ii<aTokenCount; aTokens[ii++] = KErrNotFound) ;

    while( !( (tokenCount==aTokenCount) || lex.Eos() ) )
        {
        ch = lex.Get();
        if( aTokenSeparators.Locate( ch ) != KErrNotFound ) 
            {
            aTokens[tokenCount++] = lex.Offset()-1;
            }
        }
    return tokenCount;
    }

// ---------------------------------------------------------
// CCookie::Persistent
// ---------------------------------------------------------
//
TBool CCookie::Persistent() const
    {
    TBool result( EFalse );
	THTTPHdrVal attrVal;

    if ( FromNetscapeVersion() )
        {
        if ( Attribute( CCookie::EExpires, attrVal ) == KErrNone )
            {
            // blank "expires" attribute evaluate as expired.
            // check only for string type attribute
            if ( attrVal.Type() == THTTPHdrVal::KStrFVal )
                {
                if( attrVal.StrF().DesC().Length() > 0 )
                    {
                    result = ETrue;
                    }
                }
            else
                {
                result = ETrue;
                }
            }
        }
    else
        {
        if ( Attribute( CCookie::EDiscard, attrVal ) == KErrNotFound )
            {
            if ( Attribute( CCookie::EMaxAge, attrVal ) == KErrNone )
                { // has max age, to be stored until it expires...
                // check if MaxAge is 0, in that case, discard immediatelly.
                TInt MaxAge(0);
                if ( attrVal.Type() == THTTPHdrVal::KTIntVal )
                    {
                    MaxAge = attrVal.Int();
                    }
                else
                    {
                    TPtrC8 MaxAgePtr( attrVal.StrF().DesC() );
                    TLex8 lex( MaxAgePtr );
                    lex.Val( MaxAge );
                    }
                if ( MaxAge > 0)
                    {
                    result = ETrue;
                    }
                }
            }
        }
    return result;
    }

// ---------------------------------------------------------
// CCookie::IsUnknownVersion
// ---------------------------------------------------------
//
TBool CCookie::IsUnknownVersion() const
    {
    TBool unknownVersion( EFalse );
    THTTPHdrVal attrVal;
    if( Attribute( CCookie::EVersion, attrVal ) != KErrNotFound )
        {
        if( attrVal.StrF().DesC().Compare( KCookieKnownVersion() ) != 0 )
            {
            unknownVersion = ETrue;
            }
        }
    return unknownVersion;
    }

// ---------------------------------------------------------
// CCookie::CCookie
// ---------------------------------------------------------
//
CCookie::CCookie( RStringPool aStringPool )
: iStringPool( aStringPool ), iSetCookie2( EFalse ), iNetscape( EFalse )
	{
	iReceivedTime.UniversalTime();
	}


// ---------------------------------------------------------
// CCookie::ConstructL
// ---------------------------------------------------------
//
void CCookie::ConstructL( RHTTPHeaders aRequestHeaders, TInt aPartIndex,
						RStringF aFieldName, const TUriC8& aUri )
	{
	const TStringTable& commonStringTable = RHTTPSession::GetTable();

	// Determining if this object is constructed from a Set-Cookie2 HTTP header
	RStringF setCookie2Name = iStringPool.StringF( HTTP::ESetCookie2,
                                                   commonStringTable );

	iSetCookie2 = ( aFieldName == setCookie2Name );

    TBool DefaultDiscard( EFalse );

	THTTPHdrVal hVal;

	// Retrieval of NAME attribute
    // ---------------------------------------------------------
	// retrieving Name atribute
    // ---------------------------------------------------------
	if ( aRequestHeaders.GetParam( aFieldName,
									iStringPool.StringF( HTTP::ECookieName,
														commonStringTable ),
									hVal,
									aPartIndex ) == KErrNone )
		{
		SetAttribute( EName, hVal );	// ignore the result of this method
		}

    // ---------------------------------------------------------
	// retrieving Value atribute
    // ---------------------------------------------------------
	if ( aRequestHeaders.GetParam( aFieldName,
									iStringPool.StringF( HTTP::ECookieValue,
														commonStringTable ),
									hVal,
									aPartIndex ) == KErrNone )
		{
		SetAttribute( EValue, hVal );	// ignore the result of this method
		}

    // now get the VERSION attribute.
    // if it is there, then it is either RFC 2109 or RFC 2965
    // if it is not there, then it is NETSCAPE style cookie
    // ---------------------------------------------------------
	// retrieving Version atribute
    // ---------------------------------------------------------
	if ( aRequestHeaders.GetParam(
                        aFieldName, 
                        iStringPool.StringF( HTTP::EVersion,
											commonStringTable ), 
                        hVal, aPartIndex ) == KErrNone )
        {
        iNetscape = EFalse;
		SetAttribute( EVersion, hVal );	// ignore the result of this method
        }
    else
        {
        iNetscape = ETrue;
        }

    // now get common attributes for all
    // that is, Path, Domain, Secure
	// Add Path attribute if it exists + defaulting
    // ---------------------------------------------------------
	// retrieving Path atribute
    // ---------------------------------------------------------
    if ( aRequestHeaders.GetParam(
                            aFieldName, 
                            iStringPool.StringF( HTTP::EPath,
												commonStringTable ), 
                            hVal, aPartIndex ) == KErrNone )
        {
		SetAttribute( EPath, hVal );	// ignore the result of this method
        }
    else
        {
		// defaulting
		AddDefaultPathL( aUri );
        }

    // ---------------------------------------------------------
	// retrieving Domain atribute
    // ---------------------------------------------------------
	if ( aRequestHeaders.GetParam(
                            aFieldName, 
                            iStringPool.StringF( HTTP::EDomain,
												commonStringTable ), 
                            hVal, aPartIndex ) == KErrNone && hVal.StrF().DesC().Length() > 0)
        {
		SetAttribute( EDomain, hVal );	// ignore the result of this method
        }
    else
        { // Default handling 
        // in this case the default is the effective request host        
        RStringF defDomain =
						iStringPool.OpenFStringL( aUri.Extract( EUriHost ) );
        THTTPHdrVal defdomVal( defDomain );
		SetAttribute( EDomain, defdomVal, ETrue );// ignore the result 
        defDomain.Close();
        }

    // ---------------------------------------------------------
	// retrieving Secure atribute
    // ---------------------------------------------------------
	if ( aRequestHeaders.GetParam( 
                        aFieldName, 
                        iStringPool.StringF( HTTP::ESecure,
											commonStringTable ), 
                        hVal, aPartIndex ) == KErrNone )
        {
		SetAttribute( ESecure, hVal );	// ignore the result of this method
        }
    else
        { // Default handling if applies
		// if not present, then cookie is not 'secure'
		// no action here as if attribute is not present, 
		// cookie might be sended by default.
        }


    // now if it is Netscape, get Netscape specific parts
    if ( iNetscape )
        {
        // that is, Expires
        // ---------------------------------------------------------
	    // retrieving Expires atribute
        // ---------------------------------------------------------
        if ( aRequestHeaders.GetParam(
                            aFieldName, 
                            iStringPool.StringF( HTTP::EExpires,
											    commonStringTable ), 
                            hVal, aPartIndex ) == KErrNone )
            {
		    SetAttribute( EExpires, hVal );	// ignore the result of this method
            }
        //----------------------------------------------------------- 
        // Default handling is does not applies.
		// Reference: error ID:ESAN-6XWHRL
		// If "version" is not present but "max-age/comment" is present
		// then we can considere cookie as RFC 2109 or RFC 2965 type 
        else if(( aRequestHeaders.GetParam( aFieldName, 
								iStringPool.StringF( HTTP::EComment,
	                            commonStringTable ), 
	                            hVal, aPartIndex ) == KErrNone ) ||
	           ( aRequestHeaders.GetParam( aFieldName, 
	                            iStringPool.StringF( HTTP::EMaxAge,
								commonStringTable ),
	                            hVal, aPartIndex ) == KErrNone ))
				{
					// Not a NETSCAPE style cookie
					iNetscape = EFalse;
				}
        }
		
    // ---------------------------------------------------------
    // retrieving Date atribute
    // ---------------------------------------------------------
	// get the Date header field name 
	RStringF dateFieldName = iStringPool.StringF( HTTP::EDate,
                                                  commonStringTable );
    if ( aRequestHeaders.GetField( dateFieldName, 0, hVal) == KErrNone ) 

        {
	    SetAttribute( EDate, hVal );	// ignore the result of this method
        }
    else 

    // as the expiration comparison is based on the UniversalTime,
    // we use the curreent (arriving time) for the cookie's born time too. :-)
        { // Add default handling if applies
        TTime nowTime;
        nowTime.UniversalTime();
        // TDateTime dateTime( nowTime.DateTime() );
        hVal.SetDateTime( nowTime.DateTime() );
        SetAttribute( EDate, hVal );
        }
    if( EFalse == iNetscape )
        {
        // now get RFC SetCookie and SetCookie2 common parts
        // that is, Comment, Max-Age
        // get EDate to be able to calculate cookie age if needed.
        // ---------------------------------------------------------
	    // retrieving Comment atribute
        // ---------------------------------------------------------
	    if ( aRequestHeaders.GetParam( aFieldName, 
                                       iStringPool.StringF( HTTP::EComment, 
                                                            commonStringTable ), 
                                       hVal, aPartIndex ) == KErrNone )
            {
		    SetAttribute( EComment, hVal );	// ignore the result of this method
            }
        else
            { // Add default handling if applies
		    // no defaulting for EComment
            }
        
        // ---------------------------------------------------------
	    // retrieving Max-Age atribute
        // ---------------------------------------------------------
        if ( aRequestHeaders.GetParam( 
                                aFieldName, 
                                iStringPool.StringF( HTTP::EMaxAge,
												    commonStringTable ),
                                hVal, aPartIndex ) == KErrNone )
            {
		    SetAttribute( EMaxAge, hVal );	// ignore the result of this method
            }
        else
            { // Add default handling if applies
            // Defaults to Discard, discard the cookie on agent termination
            // -> transient cookie
            // in case of SetCookie2, also set Discard, defaulted
            if ( iSetCookie2 )
                {
                DefaultDiscard = ETrue;
                }
           }

        // now if it is SetCookie2, than get SetCookie2 specific parts
        if ( iSetCookie2 )
            {
            // that is, CommentUrl, Discard, Port
            // ---------------------------------------------------------
	        // retrieving CommentURL atribute
            // ---------------------------------------------------------
	        if ( aRequestHeaders.GetParam( aFieldName, 
                                           iStringPool.StringF( HTTP::ECommentURL, 
                                                                commonStringTable ), 
                                           hVal, aPartIndex ) == KErrNone )
                {
		        SetAttribute( ECommentURI, hVal );	// ignore the result of this method
                }
            else
                { // Add default handling if applies
		        // no defaulting for ECommentURL
                }

            // ---------------------------------------------------------
	        // retrieving Discard atribute
            // ---------------------------------------------------------
	        if ( aRequestHeaders.GetParam( 
                                    aFieldName, 
                                    iStringPool.StringF( HTTP::EDiscard,
												        commonStringTable ),
                                    hVal, aPartIndex ) == KErrNone )
                {
		        SetAttribute( EDiscard, hVal );	// ignore the result of this method
                }
            else
                { // Add default handling if applies
		        // no defaulting for EDiscard
                // only if it is caused by MAx-Age beeing not supplied
                if ( DefaultDiscard )
                    {
		            RStringF emptyStr = iStringPool.OpenFStringL( KNullDesC8() );
                    THTTPHdrVal emptyVal( emptyStr );
    		        SetAttribute( EDiscard, emptyVal, ETrue );
	    	        emptyStr.Close();
                    }
                }

	        // Add Port attribute if it exists + defaulting
            // ---------------------------------------------------------
	        // retrieving Port atribute
            // ---------------------------------------------------------
	        if ( aRequestHeaders.GetParam( aFieldName, 
								        iStringPool.StringF( HTTP::ECookiePort,
													        commonStringTable ),
								        hVal,
								        aPartIndex ) == KErrNone )
                {
		        // if it is a string and contains at least 1 character, then store it
		        if ( ( hVal.Type() == THTTPHdrVal::KStrFVal &&
				        hVal.StrF().DesC().Length() ) ||
			        ( hVal.Type() == THTTPHdrVal::KStrVal &&
				        hVal.Str().DesC().Length() ) )
			        {
			        SetAttribute( EPort, hVal, ETrue );	// ignore the result of this method
			        }
		        else	// the Port attrib is either empty or incorrect
			        {
			        RStringF requestPort;
			        if ( aUri.IsPresent( EUriPort ) )
				        {
				        requestPort =
						        iStringPool.OpenFStringL( aUri.Extract( EUriPort ) );
				        }
			        else
				        {
				        requestPort = iStringPool.OpenFStringL
											        ( KCookieDefaultRequestPort() );
				        }

			        THTTPHdrVal reqPort( requestPort );

			        SetAttribute( EPort, reqPort, ETrue ); // ignore the result 

			        requestPort.Close();
			        }
                }
            else
                {
		        // if the Port attrib does not exist, leave it blank
                }
            }
        }
	}



// ---------------------------------------------------------
// CCookie::FindAttribute
// ---------------------------------------------------------
//
TInt CCookie::FindAttribute( TCookieAttributeName aAttributeName,
							TCookieAttribute& aAttribute ) const
	{
	const TInt numAttributes = iAttributes.Count();
	for ( TInt index = 0; index < numAttributes; ++index )
		{
		if ( iAttributes[index].Name() == aAttributeName )
			{
			aAttribute = iAttributes[index];
			return index;
			}
		}

	return KErrNotFound;
	}

// ---------------------------------------------------------
// CCookie::RemoveAttribute
// ---------------------------------------------------------
//
void CCookie::RemoveAttribute( TInt aIndex )
	{
	iSize -= ( KCookieAttributePrefixLength + iAttributes[ aIndex ].Size() + 
               KCookieAttributeDefaultedLength + 
               KCookieAttributeFoundLength );
	iAttributes[ aIndex ].Close();
	iAttributes.Remove( aIndex );
	}

// ---------------------------------------------------------
// CCookie::RemoveAttribute
// ---------------------------------------------------------
//
void CCookie::RemoveAttribute( TCookieAttributeName aAttributeName )
    {
    TCookieAttribute attribute;
    TInt index( FindAttribute( aAttributeName, attribute ) );
    if( index != KErrNotFound )
        {
        RemoveAttribute( index );
        }
    }

// ---------------------------------------------------------
// CCookie::GetLocalOffset
// ---------------------------------------------------------
//
TInt CCookie::GetLocalOffset( HBufC8* aDate, TInt& aHour, 
                         TInt& aMinute, TInt& aLength ) const
    {
    TInt retval( 0 );
    TInt pos(0);
//    TInt fwspos(0);
//    TBool plus( ETrue );
    TPtrC8 datePtr( aDate->Des() );


    TLex8 lex( datePtr );
    TInt oldoffset( 0 );
    TInt curroffset( 0 );
    TInt i( 0 );
    TInt tokencount( 0 );
    TInt deltaoffset( 0 );
    // firstoffset is to store the first token length/endposition
    TInt firstoffset( 0 );
    TBool repeat( ETrue );
    do
        {
        lex.SkipCharacters();
        curroffset = lex.Offset();
        deltaoffset = curroffset-oldoffset;
        if ( deltaoffset )
            {
            if ( !tokencount )
                {
                firstoffset = deltaoffset;
                }
            for(i=0; i<deltaoffset; i++)
                {
                lex.Get();
                }
            tokencount++;
/*
            if ( deltaoffset != aDate->Length() )
                {
                tokencount++;
                }
*/
            oldoffset = curroffset;
            }
        else
            { // end of string, oldoffset1 holds position
            repeat = EFalse;
            }
        }while (repeat);



    pos = aDate->LocateReverse( KLocalTimeZoneSeparator1 );

    if ( pos > 0 )
        { // found at least one "+"        
//        plus = ETrue;
        }
    else
        {
        pos = aDate->LocateReverse( KLocalTimeZoneSeparator2 );
        if ( pos > 0 )
            { // found at least one "-"
            // now check if it is in the date...
            // if it is in the date part, then we did found only one tokens...
            // and the position MUST be larger than the end of 
            // the first token (date)...
            if ( ( tokencount > 1) && ( firstoffset < pos ) )
                {
//                plus = EFalse;
                }
            else
                {
                pos = KErrNotFound;
                retval = KErrNotFound;
                }
            }
        else
            {
            retval = KErrNotFound;
            }
        }
    if ( pos > 0 )
        {
        // get the length
        aLength = aDate->Length()-pos;
        // now get the offset
        HBufC8* temp = aDate->Right( aLength ).AllocLC();
        temp->Des().Trim();
        TLex8 lex(temp->Left(2));
        lex.Val( aHour );
        TLex8 lex1(temp->Right(2));
        lex1.Val( aMinute );
        CleanupStack::PopAndDestroy( temp ); // temp
        }
    return retval;
    }




// ---------------------------------------------------------
// CCookie::GetMilitaryOffset
// ---------------------------------------------------------
//
TInt CCookie::GetMilitaryOffset( HBufC8* /*aDate*/, TInt& aHour ) const
    {
    // TODO: Add military timezone handling here...
    TInt retval( KErrNotFound );
    aHour = 0;
    return retval;
    }



// ---------------------------------------------------------
// CCookie::GetTimeZone
// ---------------------------------------------------------
//
TInt CCookie::GetTimeZone( HBufC8* aDate, TInt& aHour, 
                         TInt& aLength ) const
    {
    TInt retval( 0 );
    TInt zonepos = aDate->Find( KUT() );
    if ( zonepos != KErrNotFound )
        {
        aHour = 0;
        }
    else
        {
        zonepos = aDate->Find( KGMT );
        if ( zonepos  != KErrNotFound )
            {
            aHour = 0;
            }
        else
            {
            zonepos = aDate->Find( KEST );
            if ( zonepos  != KErrNotFound )
                {
                aHour = -5;
                }
            else
                {
                zonepos = aDate->Find( KEDT );
                if ( zonepos  != KErrNotFound )
                    {
                    aHour = -4;
                    }
                else
                    {
                    zonepos = aDate->Find( KCST );
                    if ( zonepos  != KErrNotFound )
                        {
                        aHour = -6;
                        }
                    else
                        {
                        zonepos = aDate->Find( KCDT );
                        if ( zonepos  != KErrNotFound )
                            {
                            aHour = -5;
                            }
                        else
                            {
                            zonepos = aDate->Find( KMST );
                            if ( zonepos  != KErrNotFound )
                                {
                                aHour = -7;
                                }
                            else
                                {
                                zonepos = aDate->Find( KMDT );
                                if ( zonepos  != KErrNotFound )
                                    {
                                    aHour = -6;
                                    }
                                else
                                    {
                                    zonepos = aDate->Find( KPST );
                                    if ( zonepos  != KErrNotFound )
                                        {
                                        aHour = -8;
                                        }
                                    else
                                        {
                                        zonepos = aDate->Find( KPDT );
                                        if ( zonepos  != KErrNotFound )
                                            {
                                            aHour = -7;
                                            }
                                        else
                                            { 
                                            retval = KErrNotFound;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

    if ( zonepos != KErrNotFound )
        {
        // get the length
        aLength = aDate->Length() - zonepos;
        }

    return retval;
    }



// ---------------------------------------------------------
// CCookie::GetCleanedDateTimeLC
// ---------------------------------------------------------
//
HBufC8* CCookie::GetCleanedDateTimeLC( HBufC8* aDate ) const
    {
    // as http time might contain a "DayOfWeek," optional part
    // which is not understood by the parser, we need to 
    // remove it 
    TInt pos = aDate->Locate( KDayOfWeekSeparator );
    if ( pos < 0 )
        {
        pos = 0;
        }
    else 
        {
        if ( pos )
            {
            pos++;
            }
        }
    
    HBufC8* result = HBufC8::NewLC( aDate->Length() ); 
    HBufC8* temp = aDate->Mid( pos ).AllocLC();
    // reduce multiple spaces
    temp->Des().TrimAll();

    // now try to remove time zone values...    
    TInt hour( 0 );
    TInt minute( 0 );
    TInt length( 0 );

    // Accordign to RFC 2822, the correct date-time syntax:
    // [dayofweek","] date FWS time [CFWS]
    // where:
    // date :   day month year
    // time :   time-of-day FWS zone
    //
    // zone :   (( "+" / "-" ) 4DIGIT) / obs-zone
    // obs-zone : "GMT", "UT", "EST", "EDT", "CST", "CDT", 
    //            "MST", "MDT", "PST", "PDT"
    //           and the military 1 letter version
    // so first try to get the +/-hhmm format...
    // check if time is locale offset in +/- HHMM form
    if ( GetLocalOffset( temp, hour, minute, length ) == KErrNotFound )
        { 
        // not a local offset, check if it is one of the 
        // known 2 or three digit ones
        if ( GetTimeZone( temp, hour, length ) == KErrNotFound )
            {
            //Not, check if it is Military, 
            //which is a one digit ALPHA
            if ( GetMilitaryOffset( temp, hour ) == KErrNotFound )
                {
                // not a military one, 
                // error, unknown time zone or none...
                }
            else
                { // Military offset
                length = 1;
                }
            }
        else
            {// other know one
            }
        }
    else
        { // locale offset
        }
    *result = temp->Left( temp->Length() - length);
    // return ( temp->Left( temp->Length() - length).AllocLC() );
    CleanupStack::PopAndDestroy( temp );
    return result;
    }






// ---------------------------------------------------------
// CCookie::TCookieAttribute::TCookieAttribute
// ---------------------------------------------------------
//
CCookie::TCookieAttribute::TCookieAttribute( TCookieAttributeName aName,
											const THTTPHdrVal& aHdrVal, 
											TBool aDefaulted )
: iName( aName ),
iDefaulted( aDefaulted )
	{
	iValue = aHdrVal.Copy();
	

	switch ( iValue.Type() )
		{
		case THTTPHdrVal::KTIntVal :
			{
			iSize = sizeof( TInt );
			break;
			}
		case THTTPHdrVal::KStrVal :
			{
			iSize = iValue.Str().DesC().Length();
			break;
			}
		case THTTPHdrVal::KDateVal :
			{
			iSize = sizeof( TDateTime );
			break;
			}
		case THTTPHdrVal::KStrFVal :
			{
			iSize = iValue.StrF().DesC().Length();
			break;
			}
		default :	// THTTPHdrVal::KNoType
			{
			iSize = 0;
			}
		}
	}


// ---------------------------------------------------------
// CCookie::TCookieAttribute::TCookieAttribute
// ---------------------------------------------------------
//
CCookie::TCookieAttribute::TCookieAttribute()
	{
	}


// ---------------------------------------------------------
// CCookie::TCookieAttribute::Close
// ---------------------------------------------------------
//
void CCookie::TCookieAttribute::Close()
	{
	THTTPHdrVal::THTTPValType type = iValue.Type();
	if ( type == THTTPHdrVal::KStrVal )
		{
		iValue.Str().Close();
		}
	else if ( type == THTTPHdrVal::KStrFVal )
		{
		iValue.StrF().Close();
		}

	// TBD : iValue = THTTPHdrVal();?
	iSize = 0;
	}


// ---------------------------------------------------------
// CCookie::TCookieAttribute::Name
// ---------------------------------------------------------
//
CCookie::TCookieAttributeName CCookie::TCookieAttribute::Name() const
	{
	return iName;
	}

// ---------------------------------------------------------
// CCookie::TCookieAttribute::Size
// ---------------------------------------------------------
//
TInt CCookie::TCookieAttribute::Size() const
	{
	return iSize;
	}


// ---------------------------------------------------------
// CCookie::TCookieAttribute::Defaulted
// ---------------------------------------------------------
//
TBool CCookie::TCookieAttribute::Defaulted() const
	{
	return iDefaulted;
	}

// ---------------------------------------------------------
// CCookie::TCookieAttribute::Value
// ---------------------------------------------------------
//
const THTTPHdrVal& CCookie::TCookieAttribute::Value() const
	{
	return iValue;
	}




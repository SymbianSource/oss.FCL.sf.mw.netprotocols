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
* Description:  Implementation of class TCookiePacker
*
*
*/


// INCLUDE FILES
	// User includes
#include "cookie.h"
#include "cookieipc.h"
#include "CookieLogger.h"

#ifdef __TEST_COOKIE_LOG__
const TInt KCookiePackerLogLevel = 4;
#endif

// Maximum cookie size (defined in RFC2965)
const TInt KMaxCookieBufferSize = 0x1000;   // 4 kilobytes


// ---------------------------------------------------------
// TCookiePacker::TCookiePacker
// ---------------------------------------------------------
//
TCookiePacker::TCookiePacker( RStringPool aStringPool )
: iStringPool( aStringPool )
	{
	}


// ---------------------------------------------------------
// TCookiePacker::AddDateL
// ---------------------------------------------------------
//
TInt TCookiePacker::AddDateL( const CCookie& aCookie,
                            TDes8& aBuffer,
                            TBool aNoDefaulted ) const
    {
    CLOG( ( EClient, KCookiePackerLogLevel, _L("-> TCookiePacker::AddDate") ) );

    THTTPHdrVal attributeVal;
    TInt error = KErrNone;

    TBool defaulted( EFalse );
    TInt found = aCookie.Attribute( CCookie::EDate, attributeVal, defaulted );
    // Date attribute must present in the RFC cookie
    if ( found == KErrNone )
        {
        if (aNoDefaulted && defaulted )
            {// pass a null text, that is, a length of 0
		    AppendAttributeLength( 0, aBuffer );
            }
        else
            {
            TDateTime attrTime = attributeVal.DateTime();
            TTime cookieDate( attrTime );

            // This FormatString must be shorter than sizeof(TDateTime)!
            // As the cookie's date attribute always costs sizeof(TDateTime) byte
            // in the memory (when we calculate its size in GetCookieSize),
            // we have to allocate sizeof(TDateTime) byte in the client-server
            // communication buffer too.
            _LIT(KDateFormatString, "%F%Y%M%D:%H%T%S");
            // convert DateTime to string (16 bit)
            TInt size =  sizeof( TDateTime );
            HBufC *buf = HBufC::NewLC( size );
            TPtr bufDes( buf->Des() );
            bufDes.Fill( ' ', size );
            cookieDate.FormatL( bufDes, KDateFormatString );
            bufDes.SetLength( size );
            // convert to 8 bit
            HBufC8 *buf8 = HBufC8::NewLC( size );
            TPtr8 buf8Des( buf8->Des() );
            buf8Des.Copy( bufDes );

            AppendAttributeLength( buf8Des.Length(), aBuffer );
            aBuffer.Append( buf8Des );

            CleanupStack::PopAndDestroy();  // buf8
            CleanupStack::PopAndDestroy();  // buf
            }
        }
    else  // Netscape cookie?
        { // pass a null text, that is, a length of 0
		AppendAttributeLength( 0, aBuffer );
        }

    // always append 'found' to the end...
    // if found == 0 -> HAS IT, 1
    // otherwise, 0    
    AppendAttributeLength( found ? 0 : 1, aBuffer);

    // always append 'defaulted' to the end...
    AppendAttributeLength( defaulted, aBuffer);

    CLOG( ( EClient, KCookiePackerLogLevel, 
        _L("<- TCookiePacker::AddDate, errcode %d"), error ) );
    return error;
    }

// ---------------------------------------------------------
// TCookiePacker::AddStringF
// ---------------------------------------------------------
//
TInt TCookiePacker::AddStringF( const CCookie::TCookieAttributeName aAttrib,
							   const CCookie& aCookie,
							   TDes8& aBuffer,
                               TBool aNoDefaulted ) const
    {
	CLOG( ( EClient, KCookiePackerLogLevel, _L( "-> TCookiePacker::AddStringF" ) ) );

    THTTPHdrVal attributeVal;
    TInt error = KErrNone;

    TBool defaulted( EFalse );
    TInt found = aCookie.Attribute( aAttrib, attributeVal, defaulted );
    if ( found == KErrNone )
        {
        if ( aNoDefaulted && defaulted )
            {// pass a null text, that is, a length of 0
		    AppendAttributeLength( 0, aBuffer );
            }
        else
            {
            HBufC8* txt( NULL );
            if( attributeVal.Type() == THTTPHdrVal::KStrFVal )
                txt = attributeVal.StrF().DesC().Alloc();
            else if ( attributeVal.Type() == THTTPHdrVal::KStrVal )
                txt = attributeVal.Str().DesC().Alloc();
                
            if ( txt )
                {
                AppendAttributeLength( txt->Length(), aBuffer );
			    aBuffer.Append( *txt );
                delete txt;
                }
            else
                {
                error = KErrNoMemory;
                }
            }
        }
    else
        { // pass a null text, that is, a length of 0
		AppendAttributeLength( 0, aBuffer );
        }

    // always append 'found' to the end...
    // if found == 0 -> HAS IT, 1
    // otherwise, 0    
    AppendAttributeLength( found ? 0 : 1, aBuffer);

    // always append 'defaulted' to the end...
    AppendAttributeLength( defaulted, aBuffer);

	CLOG( ( EClient, KCookiePackerLogLevel,
		_L( "<- TCookiePacker::AddStringF, errcode%d" ), error ) );

    return error;
    }


// ---------------------------------------------------------
// TCookiePacker::AppendAttributeLength
// ---------------------------------------------------------
//
void TCookiePacker::AppendAttributeLength( TInt aLength, TDes8& aBuffer ) const
    {
	CLOG( ( EClient, KCookiePackerLogLevel, _L( "-> TCookiePacker::AppendAttributeLength" ) ) );

	TInt attrLength = aLength;
	TPtrC8 attrDes( (const TUint8*)&attrLength, sizeof( TInt ) );
	aBuffer.Append( attrDes );

	CLOG( ( EClient, KCookiePackerLogLevel, _L( "<- TCookiePacker::AppendAttributeLength" ) ) );
    }



// ---------------------------------------------------------
// TCookiePacker::CliPackCookie
// ---------------------------------------------------------
//
TInt TCookiePacker::CliPackCookie( TDes8& aBuffer, 
                                   const CCookie& aCookie ) const
	{
    return DoPackCookie( aBuffer, aCookie, EFalse );
    }



// ---------------------------------------------------------
// TCookiePacker::SrvPackCookie
// ---------------------------------------------------------
//
TInt TCookiePacker::SrvPackCookie( TDes8& aBuffer, 
                                   const CCookie& aCookie ) const
	{
    return DoPackCookie( aBuffer, aCookie, ETrue );
    }



// ---------------------------------------------------------
// TCookiePacker::GetPackedAttribute
// ---------------------------------------------------------
//
void TCookiePacker::GetPackedAttributeL
						( const CCookie::TCookieAttributeName aAttrib,
						CCookie& aCookie, 
						const TUint8*& aPtrStart ) const
    {
	// the first 4 bytes (i.e. sizeof(TInt)) contains the size
	TInt size;
	TPtr8 sizeDes( (TUint8*)&size, sizeof( TInt ) );
	sizeDes.Copy( aPtrStart, sizeof( TInt ) );
	
	User::LeaveIfError(size);
	// now we have the size, jump to the value
    aPtrStart += sizeof( TInt );
    
    if ( size )
        {
        HBufC8* buf = HBufC8::NewLC( size );
		TPtr8 bufDes( buf->Des() );	// buffer descriptor
	    Mem::Copy( (void*)buf->Ptr(), (void*)aPtrStart, size );
        bufDes.SetLength( size );

		aPtrStart += size;

        // now get the 'found' value 
	    TInt found;
	    TPtr8 foundDes( (TUint8*)&found, sizeof( TInt ) );
	    foundDes.Copy( aPtrStart, sizeof( TInt ) );
        aPtrStart += sizeof( TInt );

        // now get the 'defaulted' value 
	    TInt defaulted;
	    TPtr8 defaultedDes( (TUint8*)&defaulted, sizeof( TInt ) );
	    defaultedDes.Copy( aPtrStart, sizeof( TInt ) );
        aPtrStart += sizeof( TInt );
        
        THTTPHdrVal attributeVal;
        // Name and Value are case-sensitive
        if( ( CCookie::EName == aAttrib ) || ( CCookie::EValue == aAttrib ) )
            {
            RString str = iStringPool.OpenStringL( bufDes );
            attributeVal.SetStr( str );
            aCookie.SetAttribute( aAttrib, 
                attributeVal, 
                defaulted ); // ignore the result
            str.Close();
            }
        else
            {
            RStringF strf = iStringPool.OpenFStringL( bufDes );
            attributeVal.SetStrF( strf );
            aCookie.SetAttribute( aAttrib, 
                attributeVal, 
                defaulted ); // ignore the result
            strf.Close();
            }

		CleanupStack::PopAndDestroy();	// buf
        }
    else
        {
        // now get the 'found' value 
	    TInt found;
	    TPtr8 foundDes( (TUint8*)&found, sizeof( TInt ) );
	    foundDes.Copy( aPtrStart, sizeof( TInt ) );
        aPtrStart += sizeof( TInt );

        // now get the 'defaulted' value 
	    TInt defaulted;
	    TPtr8 defaultedDes( (TUint8*)&defaulted, sizeof( TInt ) );
	    defaultedDes.Copy( aPtrStart, sizeof( TInt ) );
        aPtrStart += sizeof( TInt );

        if ( found )
            { // still add empty str...
            HBufC8* buf = HBufC8::NewLC( 0 );
		    TPtr8 bufDes( buf->Des() );	// buffer descriptor
            bufDes.SetLength( 0 );
            
            THTTPHdrVal attributeVal;
            // Name and Value are case-sensitive
            // It is a serious error if Name or Value is empty!
            if( ( CCookie::EName == aAttrib ) || ( CCookie::EValue == aAttrib ) )
                {
                RString str = iStringPool.OpenStringL( bufDes );
                attributeVal.SetStr( str );
                aCookie.SetAttribute( aAttrib, 
                    attributeVal, 
                    defaulted ); // ignore the result
                str.Close();
                }
            else
                {
                RStringF strf = iStringPool.OpenFStringL( bufDes );
                attributeVal.SetStrF( strf );
                aCookie.SetAttribute( aAttrib, 
                    attributeVal, 
                    defaulted ); // ignore the result
                strf.Close();
                }

    		CleanupStack::PopAndDestroy();	// buf
            }
        }
    }


// ---------------------------------------------------------
// TCookiePacker::GetPackedDateAttributeL
// ---------------------------------------------------------
//
void TCookiePacker::GetPackedDateAttributeL( CCookie& aCookie,
                                            const TUint8*& aPtrStart ) const
    {
	// the first 4 bytes (i.e. sizeof(TInt)) contains the size
	TInt size;
	TPtr8 sizeDes( (TUint8*)&size, sizeof( TInt ) );
	sizeDes.Copy( aPtrStart, sizeof( TInt ) );
	User::LeaveIfError(size);
	// now we have the size, jump to the value
    aPtrStart += sizeof( TInt );
    if ( size )
        {
        HBufC8* buf8 = HBufC8::NewLC( size );
		TPtr8 buf8Des( buf8->Des() );	// buffer descriptor
	    Mem::Copy( (void*)buf8->Ptr(), (void*)aPtrStart, size );
        buf8Des.SetLength( size );
        aPtrStart += size;

        // now get the 'found' value 
	    TInt found;
	    TPtr8 foundDes( (TUint8*)&found, sizeof( TInt ) );
	    foundDes.Copy( aPtrStart, sizeof( TInt ) );
        aPtrStart += sizeof( TInt );

        // now get the 'defaulted' value 
	    TInt defaulted;
	    TPtr8 defaultedDes( (TUint8*)&defaulted, sizeof( TInt ) );
	    defaultedDes.Copy( aPtrStart, sizeof( TInt ) );
        aPtrStart += sizeof( TInt );

        // convert string to 16 bit
        HBufC *buf = HBufC::NewLC( size );
        TPtr bufDes( buf->Des() );
        bufDes.Copy( buf8Des );

        // convert to DateTime
        TTime cookieDate;
        // TTime::Set() uses 0 value as offset for Month and Day,
        // we convert those values.
        DecreaseNumericValueL( bufDes, 5 );  // Month
        DecreaseNumericValueL( bufDes, 7 );  // Day
        bufDes.Trim();  // spaces must be removed before TTime::Set()
        // TInt parseError = 
        cookieDate.Set( bufDes );
        TDateTime temp = cookieDate.DateTime();
        THTTPHdrVal attributeVal( temp );
        aCookie.SetAttribute( CCookie::EDate, attributeVal, defaulted );

		CleanupStack::PopAndDestroy();	// buf
		CleanupStack::PopAndDestroy();	// buf8
        }
    else
        {
        // now get the 'found' value 
	    TInt found;
	    TPtr8 foundDes( (TUint8*)&found, sizeof( TInt ) );
	    foundDes.Copy( aPtrStart, sizeof( TInt ) );
        aPtrStart += sizeof( TInt );

        // now get the 'defaulted' value 
	    TInt defaulted;
	    TPtr8 defaultedDes( (TUint8*)&defaulted, sizeof( TInt ) );
	    defaultedDes.Copy( aPtrStart, sizeof( TInt ) );
        aPtrStart += sizeof( TInt );

        if ( found )
            {
            TTime nowTime;
            nowTime.UniversalTime();
            TDateTime attrTime = nowTime.DateTime();
		    THTTPHdrVal attributeVal( attrTime );
            aCookie.SetAttribute( CCookie::EDate, attributeVal, defaulted );
            }

        }
    }

// ---------------------------------------------------------
// TCookiePacker::UnpackCookieL
// get the cookie entries, one after the other
// an entry with a length of 0 and no txtbuf means empty value
// ---------------------------------------------------------
//
void TCookiePacker::UnpackCookieL( const TUint8*& aPtr,
								  CCookie& aCookie ) const
    {
	CLOG( ( EClient, KCookiePackerLogLevel, 
	    _L( "-> TCookiePacker::UnpackCookieL" ) ) );
    GetPackedAttributeL( CCookie::EName, aCookie, aPtr );
	GetPackedAttributeL( CCookie::EValue, aCookie, aPtr );
	GetPackedAttributeL( CCookie::EComment, aCookie, aPtr );
	GetPackedAttributeL( CCookie::ECommentURI, aCookie, aPtr );
	GetPackedAttributeL( CCookie::EDiscard, aCookie, aPtr );
	GetPackedAttributeL( CCookie::EDomain, aCookie, aPtr );
	GetPackedAttributeL( CCookie::EMaxAge, aCookie, aPtr );
	GetPackedAttributeL( CCookie::EPath, aCookie, aPtr );
	GetPackedAttributeL( CCookie::EPort, aCookie, aPtr );
	GetPackedAttributeL( CCookie::ESecure, aCookie, aPtr );
	GetPackedAttributeL( CCookie::EVersion, aCookie, aPtr );
	GetPackedAttributeL( CCookie::EExpires, aCookie, aPtr );
    GetPackedDateAttributeL( aCookie, aPtr );
	// now get the FromCookie2 TBool, which comes a TInt...
    TInt fromcookie2( 0 );
	TPtr8 sizeDes( (TUint8*)&fromcookie2, sizeof( TInt ) );
	sizeDes.Copy( aPtr, sizeof( TInt ) );
    aPtr += sizeof( TInt );
	// now we have the size, jump to the value
    aCookie.SetCookie2( fromcookie2 );

    THTTPHdrVal hVal;
	// We're checking the Version attribute - this
	// attribute is present in RFC but not in Netscape cookies
	if ( aCookie.Attribute( CCookie::EVersion, hVal ) == KErrNone )
        {
        aCookie.SetFromNetscape( EFalse );
        }
    else
        {
        aCookie.SetFromNetscape( ETrue );
        }
    TrimNameAndValueL( aCookie );
	CLOG( ( EClient, KCookiePackerLogLevel, 
	    _L( "<- TCookiePacker::UnpackCookieL" ) ) );
    }

// ---------------------------------------------------------
// TCookiePacker::TrimNameAndValueL
// ---------------------------------------------------------
//
void TCookiePacker::TrimNameAndValueL( CCookie& aCookie ) const
    {
	CLOG( ( EClient, KCookiePackerLogLevel, 
	    _L( "-> TCookiePacker::TrimNameAndValueL" ) ) );
    THTTPHdrVal hValName;
    if( aCookie.Attribute( CCookie::EName, hValName ) == KErrNone )
        {
        THTTPHdrVal hValValue;
        if( aCookie.Attribute( CCookie::EValue, hValValue ) == KErrNone )
            {
            TInt lenName( hValName.Str().DesC().Length() );
            TInt lenValue( hValValue.Str().DesC().Length() );
            CLOG( ( EClient, KCookiePackerLogLevel, 
                _L( "NameLen: %d, ValueLen: %d" ), lenName, lenValue ) );
            if( lenName + lenValue < KMaxCookieBufferSize )
                {
                // everything is OK.
                return;
                }
            if( lenName >= KMaxCookieBufferSize )
                {
                // Cookie should be discarded
                User::Leave( KErrTooBig );
                }
            else
                {
                // trim value
                TInt size( KMaxCookieBufferSize - lenName );
                HBufC8* buf = HBufC8::NewLC( size + 1 );
                TPtr8 bufDes( buf->Des() );	// buffer descriptor
                bufDes.Copy( hValValue.Str().DesC().Left( size ) );
                bufDes.ZeroTerminate();
                // modify aCookie
                RString str = iStringPool.OpenStringL( bufDes );
                THTTPHdrVal attributeVal( str );
                aCookie.RemoveAttribute( CCookie::EValue );
                aCookie.SetAttribute( CCookie::EValue, attributeVal );
                str.Close();
                CleanupStack::PopAndDestroy( buf );
                }
            }
        }
	CLOG( ( EClient, KCookiePackerLogLevel, 
	    _L( "<- TCookiePacker::TrimNameAndValueL" ) ) );
    }

// ---------------------------------------------------------
// TCookiePacker::UnpackCookieL
// ---------------------------------------------------------
//
void TCookiePacker::UnpackCookieL( const TDesC8& aBuffer,
								  CCookie& aCookie ) const
    {
	const TUint8* ptr = aBuffer.Ptr();
	UnpackCookieL( ptr, aCookie );
	}

// ---------------------------------------------------------
// TCookiePacker::UnpackCookiesFromBufferL
// ---------------------------------------------------------
//
void TCookiePacker::UnpackCookiesFromBufferL( const TDesC8& aBuffer,
									RPointerArray<CCookie>& aCookies ) const
    {
	const TUint8* bufferPtr = aBuffer.Ptr();
	const TUint8* bufferEndPtr = bufferPtr + aBuffer.Size();

	while ( bufferPtr < bufferEndPtr )
		{
		// coverity [alloc_fn]
		// coverity [assign]	
		CCookie* cookie = CCookie::NewL( iStringPool );
		
		// coverity [push]
		CleanupStack::PushL( cookie );

		UnpackCookieL( bufferPtr, *cookie );

		if ( bufferEndPtr < bufferPtr )	// we read in too many bytes
			{
			CleanupStack::PopAndDestroy();	// destroy the last bad cookie
			}
		else	// everything is okay
			{
			// The cookie pointer array (i.e. iCookies) takes over the
			// ownership
			User::LeaveIfError( aCookies.Append( cookie ) );
			
			// coverity [pop]
			CleanupStack::Pop();	// cookie
			}
		// coverity [memory_leak]	
		}
    }



// ---------------------------------------------------------
// TCookiePacker::DoPackCookie
// ---------------------------------------------------------
//
TInt TCookiePacker::DoPackCookie( TDes8& aBuffer, 
                                  const CCookie& aCookie,
                                  TBool aServer ) const
	{
	CLOG( ( EClient, KCookiePackerLogLevel, _L( "-> TCookiePacker::DoPackCookie" ) ) );
    TInt error = AddStringF( CCookie::EName, aCookie, aBuffer );
    if ( error == KErrNone )
        {
        error = AddStringF( CCookie::EValue, aCookie, aBuffer );
        if ( error == KErrNone )
            {
            error = AddStringF( CCookie::EComment, aCookie, aBuffer );
            if ( error == KErrNone )
                {
                error = AddStringF( CCookie::ECommentURI, aCookie, aBuffer );
                if ( error == KErrNone )
                    {
                    error = AddStringF( CCookie::EDiscard,
										aCookie,
										aBuffer );
                    if ( error == KErrNone )
                        {
                        // If the server is packing, need to take extra care 
                        // for the Domain attribute...
                        // If the Domain is defaulted, defaulted domain 
                        // should not be sent back...
                        // for this, set aNoDefaulted to aServer:
                        // if server is packing, attribute will be suppressed
                        // if the client is packing, attribute
                        // will be transferred normally...
                        error = AddStringF( CCookie::EDomain,
							    			aCookie,
								    		aBuffer,
                                            aServer );
                        if ( error == KErrNone )
                            {
                            error = AddStringF( CCookie::EMaxAge,
												aCookie,
												aBuffer );
                            if ( error == KErrNone )
                                {
                                // If the server is packing, need to take extra care 
                                // If the Path is defaulted, defaulted path
                                // should not be sent back...
                                // for this, set aNoDefaulted to aServer:
                                // if server is packing, attribute will be 
                                // suppressed
                                // if the client is packing, attribute
                                // will be transferred normally...
                                error = AddStringF( CCookie::EPath,
													aCookie,
													aBuffer,
                                                    aServer );
                                if ( error == KErrNone )
                                    {
                                    // If the server is packing, 
                                    // need to take extra care 
                                    // If the Port is defaulted, 
                                    // defaulted Port
                                    // should not be sent back...
                                    // for this, set aNoDefaulted to aServer:
                                    // if server is packing, attribute will be 
                                    // suppressed
                                    // if the client is packing, attribute
                                    // will be transferred normally...
                                    error = AddStringF( CCookie::EPort,
														aCookie,
														aBuffer,
                                                        aServer );
                                    if ( error == KErrNone )
                                        {
                                        error = AddStringF( CCookie::ESecure,
															aCookie,
															aBuffer );
                                        if ( error == KErrNone )
                                            {
                                            error =
												AddStringF( CCookie::EVersion,
															aCookie,
															aBuffer );
                                            if ( error == KErrNone )
                                                {
                                                error = AddStringF
													( CCookie::EExpires,
													aCookie,
													aBuffer );
                                                if ( error == KErrNone )
                                                    {
                                                    TRAPD( err, error = AddDateL
													    ( aCookie,
													    aBuffer ) );
                                                    if ( err != KErrNone )
                                                        {
                                                        error = err;
                                                        }
                                                    if ( error == KErrNone )
                                                        { 
                                                        // handle 
                                                        // CCookie::FromCookie2()
                                                        AppendAttributeLength( 
                                                            aCookie.FromCookie2(), 
                                                            aBuffer );
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
                }
            }
        }

	CLOG( ( EClient, KCookiePackerLogLevel, _L( "<- TCookiePacker::DoPackCookie" ) ) );
    return error;
	}

// ---------------------------------------------------------
// TCookiePacker::DecreaseNumericValueL
// ---------------------------------------------------------
//
TInt TCookiePacker::DecreaseNumericValueL( TDes& aBuffer, const TInt aIndex ) const
{
    HBufC *nums = HBufC::NewLC( 12 );
    TPtr numDes( nums->Des() );
    numDes.Copy( _L("98765432109") );
    HBufC *midChar = HBufC::NewLC( 2 );
    TPtr charDes( midChar->Des() );
    charDes.FillZ( 2 );

    TInt found = numDes.Length() - 1;
    TInt i = aIndex + 1;
    while( (i > 0 ) && ( found == numDes.Length() - 1 ) )
    {
        i--;
        found = numDes.Locate( aBuffer[i] ) + 1;
        charDes = numDes.Mid( found, 1 );
        charDes.ZeroTerminate();
        aBuffer.Replace( i, 1, charDes );
    }

    CleanupStack::PopAndDestroy(); // midChar
    CleanupStack::PopAndDestroy(); // nums
    return ( (i == 0 ) && ( found == numDes.Length() - 1 ) ) ? 
        KErrGeneral : KErrNone;
}


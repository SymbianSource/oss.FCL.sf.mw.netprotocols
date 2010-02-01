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
* Description:  Declaration of class TCookiePacker
*
*
*/


#ifndef __COOKIE_IPC_H__
#define __COOKIE_IPC_H__

// INCLUDE FILES
	// System includes
#include <cookie.h>
#include <e32base.h>
#include <http.h>

// CLASS DECLARATIONS

/** 
* TCookiePackedCookie
* Packs a CCookie's content into a buffer for sending across IPC.
* Also unpacks the data into a CCookie
*
*/
class TCookiePacker
	{
	public :	// construction
		/**
		*
		*/
		TCookiePacker( RStringPool aStringPool );  // DONE

	public :	// common methods
		/**
		*
		*/
		TInt CliPackCookie( TDes8& aBuffer, const CCookie& aCookie ) const;

        /**
		*
		*/
		TInt SrvPackCookie( TDes8& aBuffer, const CCookie& aCookie ) const;

		/**
		* This method unpacks a cookie from a flat buffer to a cookie object.
		* It is used from both client and server side.
		*/
		void UnpackCookieL( const TDesC8& aPtr, CCookie& aCookie ) const;

		/**
		* Processes the buffer and inserts cookies into the array.
		* @param aBuffer Buffer to be processed. It contains the cookies in a
		* form that makes it possible to transfer the cookies over client-
		* server boundary,
		* @param aCookies The array that is supposed to contain the cookies.
		*/
		void UnpackCookiesFromBufferL( const TDesC8& aBuffer,
									RPointerArray<CCookie>& aCookies ) const;

	private :
		/**
		* Add a named (string type) attribute from cookie to the string buffer.
		*/
		TInt AddStringF( const CCookie::TCookieAttributeName aAttrib, 
						 const CCookie& aCookie,
						 TDes8& aBuffer,
                         TBool aNoDefaulted = EFalse ) const;
		/**
		* Add Date attribute from cookie to the string buffer.
		*/
        TInt AddDateL( const CCookie& aCookie,
                        TDes8& aBuffer,
                        TBool aNoDefaulted = EFalse ) const;

		/**
		*
		*/
		void AppendAttributeLength( TInt aLength, TDes8& aBuffer ) const;

		/**
		* Unpack a named (string type) attribute from the buffer to the cookie.
		*/
		void GetPackedAttributeL( const CCookie::TCookieAttributeName aAttrib, 
								 CCookie& aCookie, 
								 const TUint8*& aPtrStart ) const;

		/**
		* Unpack the Date attribute from the buffer to the cookie.
		*/
        void GetPackedDateAttributeL( CCookie& aCookie,
                                        const TUint8*& aPtrStart ) const;

		/**
		* This method unpacks a cookie from a flat buffer to a cookie object.
		* It is used from both client and server side.
		*/
		void UnpackCookieL( const TUint8*& aPtr, CCookie& aCookie ) const;

        /**
		*
		*/
		TInt DoPackCookie( TDes8& aBuffer, 
                           const CCookie& aCookie, TBool aSevrer ) const;

        /**
		* Decrease a numeric value in a numeric formatted buffer.
        * (underflow is handled)
        * @param aBuffer contains numeric value
        * @param aIndex position where to decrease
        * @return KErrNone if no error occurs, otherwise KErrGeneral
        * Leave code: KErrNoMemory
		*/
		TInt DecreaseNumericValueL( TDes& aBuffer, const TInt aIndex ) const;
		
		/**
		* Trim cookie's name and value if their size is bigger than 4K.
		* @param aCookie identifies the cookie
		*/
		void TrimNameAndValueL( CCookie& aCookie ) const;

	private :	// data members
		RStringPool iStringPool;
	};

#endif // __COOKIE_IPC_H__

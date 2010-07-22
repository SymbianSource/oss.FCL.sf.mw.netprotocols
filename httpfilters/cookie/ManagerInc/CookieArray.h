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
#ifndef __COOKIEARRAY_H__
#define __COOKIEARRAY_H__

// INCLUDE FILES
	// System includes
#include "cookie.h"
#include <e32base.h>
#include <e32def.h>

// Maximum file length
const TUint32 KCookieMaxFileLength = 102400;	// 100 kilobytes


// CLASS DECLARATIONS

/**
*
*/
NONSHARABLE_CLASS( CCookieArray ) : public CBase
	{
	public :	// construction, destruction
		/**
		*
		*/
		CCookieArray();

		/**
		*
		*/
		~CCookieArray();

	public :
		/**
		* Appends Cookie at the end of Cookie Pointer Array
		*/
		void AddL( const CCookie* aCookie, const TDesC8& aRequestUri );

		/**
		* Inserts Cookie at the index position of the Cookie Pointer Array
		*/
		void InsertL( const CCookie* aCookie, const TInt aIndex );
		
		/**
		* Makes room for the new cookie by removing the oldest one(s)
        * to fit into a given total size limit. Updates the index.
        * @param aSize The size of the new cookie which should be fitted.
        */        
        void MakeRoomForInsertIfNeededL( const CCookie* aCookie, const TDesC8& aRequestUri, TInt& aIndex );
		
		/**
        * Removes a cookie. 
        * @param aIndex A valid index to remove.
        * 
        */
        void Remove( TInt aIndex );

		/**
		* Clears the cookie array.
		* @return The number of deleted cookies.
		*/
		TInt ClearAllCookies();

		/**
		*
		*/
		TInt Count() const;

		/**
        *
		* Note : this method does not put expired cookies in the array.
		*/
		TInt GetCookies( const TDesC8& aRequestUri,
						RPointerArray<CCookie>& aCookies );
        TInt GetCookies( const TDesC8& aRequestUri,
                                       RPointerArray<CCookie>& aCookies, TBool& iFound );

		/**
		*
		* Note : this method does not put expired cookies in the array.
		*/
		TInt GetCookies( RPointerArray<CCookie>& aCookieArray );


        /**
        * Get access to the cookies in the array
        */
        const CCookie* operator[](TInt aIndex) const;

        /**
        * Get access to the cookies in the array
        */
        const CCookie* At(TInt aIndex) const;

        
        /**
        * Checks if aCookie already exists in the list or not.
        * @param aCookie The cookie to be look for in the array.
        * @return TBool indicating whether aCookie already exist 
        * in the list or not.
        */
        TBool DoesAlreadyExists( const CCookie* aCookie, TInt& aIndex ) const;


        /**
        * Remove non-persistent cookies from the list, 
        * also removes expired ones
        */
        void RemoveNonPersistent();


        RPointerArray<CCookie>& CookieArray();
        void ReserveL( TInt aNumberOfCookies );

	private :	// internal method for cookie comparison
		/**
		*
		*/
/*		static TInt CompareCookies( CCookie const & aFirstCookie,
							CCookie const & aSecondCookie );*/

		/**
		* Compare two cookies based on their Path attribute
        * used for sorting after GetCookies!
		*/
		static TInt CompareCookiesPath( const CCookie & aFirstCookie,
                                        const CCookie & aSecondCookie ) ;

	private :	// common internal methods
		/**
		* Quotation from RFC 2965 chapter 1 :
		* "Host A's name domain-matches host B's if
		*	- their host name strings string-compare equal; or
		*	- A is a HDN string and has the form NB, where N is a non-empty
		*	name string, B has the form .B', and B' is a HDN string. (So,
		*	x.y.com domain-matches .Y.com but not Y.com.)"
		* @param aFoldedDomain1 This argument is host A from the specification,
		* @param aFoldedDomain2 This argument is host B from the specification.
        * @param aAllowedDot Dot char allowed in the leftFraction of Domain1?
		* @return ETrue if aDomain1 domain-matches aDomain2, EFalse otherwise.
		*/
        /*static*/ TBool DomainMatch( const TDesC8& aFoldedDomain1,
                                      const TDesC8& aFoldedDomain2,
                                      const TBool aAllowedDot = EFalse ) const;

		/**
		*
		* @return ETrue if the host name has changed, i.e. '.local'
		* is appended to it. EFalse if the host remained unchanged.
		*/
		TBool EffectiveHostNameL( const TDesC8& aHostName,
								HBufC8*& aEffectiveHostName ) const;


		/**
		*
		*/
		void GetFoldedCookiePortAttr( const CCookie& aCookie,
									TPtrC8& aPort ) const;

		/**
		*
		*/
		/*static*/ TBool GetFoldedCookieAttr( const CCookie& aCookie,
										CCookie::TCookieAttributeName aAttr,
										TPtrC8& aAttrDes ) const;

		/**
		* Quotation from RFC 2965 chapter 1 :
		* "For two strings that represent paths, P1 and P2, P1 path-matches P2
		* if P2 is a prefix of P1 (including the case where P1 and P2 string-
		* compare equal).  Thus, the string /tec/waldo path-matches /tec."
		* @param aPath1 This argument is P1 from the specification,
		* @param aPath2 This argument is P2 from the specification.
		* @return ETrue if aPath1 path-matches aPath2, EFalse otherwise.
		*/
		/*static*/ TBool PathMatch( const TDesC8& aPath1,
									const TDesC8& aPath2 ) const;

		/**
		*
		*/
		TBool PortMatch( const TDesC8& aRequestPort,
						const TDesC8& aCookiePort ) const;


		/**
		*
		*/
		TBool SecureMatch( const TDesC8& aUriScheme,
							const CCookie& aCookie ) const;

		/**
		*
		*/
		TBool ValidCookieL( const CCookie& aCookie,
							const TDesC8& aRequestUri ) const;

        /**
        * Remove expired cookies from the list.
        */
        void RemoveExpired();


        /**
        * Compares the passed two cookies' given attribute according
        * either case-sensitive or Folded
        * @param aCookie1 The first cookie to be compared.
        * @param aCookie2 The second cookie to be compared.
        * @param aAttr The attribute to compare
        * @return The result of the comparison
        * 0 : Equal
        * 1 : The first is greater than the second 
        * -1 : The first is less than the second
        */
        TInt CompareAttribute( const CCookie* aCookie1,
                               const CCookie* aCookie2,
                               CCookie::TCookieAttributeName aAttr,
                               TBool aFolded ) const;



        /**
        * Makes room for the new cookie with removing the oldest one(s)
        * to fit into a given total size limit
        * @param aSize The size of the new cookie which should be fitted.
        */
        void MakeRoomIfNeeded( TUint32 aSize );


	private :	// data members
		RPointerArray<CCookie> iCookies;
	};

#endif	// __COOKIEARRAY_H__

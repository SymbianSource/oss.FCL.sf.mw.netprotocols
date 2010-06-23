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
* Description:  Declaration of classes CCookie
*               
*
*/


#ifndef __CCOOKIE_H__
#define __CCOOKIE_H__

// INCLUDE FILES
	// System includes
#include <e32std.h>
#include <stringpool.h>
#include <http/thttphdrval.h>

// FORWARD DECLARATIONS

class RHTTPHeaders;
class TUriC8;
class TDesC8;
// CONSTANTS

// Attribute prefix : we have to pack cookies when either passing them over
// client-server boundaries or writing them into a file. When this happens we
// put each attribute of a cookie in the destination buffer as follows :
// prefix + attribute value + IsDefaulted,
// where prefix is a number that holds the length of the attribute to be
// stored. This prefix is a TInt.
const TInt KCookieAttributePrefixLength = sizeof( TInt );
const TInt KCookieAttributeDefaultedLength = sizeof( TInt );
const TInt KCookieAttributeFoundLength = sizeof( TInt );


// This is the maximum number of attributes that may be defined in a cookie.
// TBD : should not we treat EExpires and EMaxAge attributes as one variable?
const TInt KCookieMaxAttributeCount = 13;

// CLASS DECLARATION

/** Class to store the data that makes up a cookie
*/
class CCookie : public CBase
	{
	public :
		/** Enumeration of all the elements of data that make up a cookie
		*/
		enum TCookieAttributeName
			{
			EName,			// Name of the cookie - mandatory
			EValue,			// Value associated with the name - mandatory
			EComment,		// Comment which can be displayed to the user
			ECommentURI,	// A URI which can be visited for more information
			EDiscard,		// Flag indicating that the cookie should be discarded
			EDomain,		// Domain which this cookie applies to
			EMaxAge,		// Age after which this cookie should be deleted
			EPath,			// Path which this cookie applies to
			EPort,			// Port numbers that this cookie applies to
			ESecure,		// Flag indicating this cookie should only be used
							// for secure transactions
			EVersion,		// Version of style of cookie - mandatory
			EExpires,		// Time after which this cookie should be deleted -
							// Netscape cookies only
            EDate,          // Date when the response header containing the 
                            // cookie was generated. It is important to know it 
                            // for correct age calculation.
            //EAge,           // The age of the cookie as estimated by the cache
            ELastAttribute
			};

	public :	// exported methods
		/**
		* Constructor.
		* @param aRequestHeaders A collection of header fields associated
		* with the current HTTP message,
		* @param aPartIndex The index of the part in the header,
		* @param aFieldName The HTTP header name,
		* @param aStringPool The current string pool,
		* @param aUri HTTP request's URI.
		*/
		IMPORT_C static CCookie* NewL( RHTTPHeaders aRequestHeaders,
									 TInt aPartIndex,
									 RStringF aFieldName,
									 RStringPool aStringPool,
									 const TUriC8& aUri );
	 	/**
		* Returns a descriptor which is equivalent to the input descriptor,
		* but without the surrounding quotation marks. This method always
		* returns the appropriate string even if there are no quotation marks,
		* or at least one of them is missing.
		* This is required because cookie attributes may be surrounded
		* by double quotes.
		* @param aDes The descriptor to be cleaned.
		* @return Descriptor which points to the part of aDes with no quotes
		*/
		static TPtrC8 RemoveQuotes( const TDesC8& aDes );			 
		/**
		* Constructor.
		* @param aStringPool The current string pool.
		*/
		IMPORT_C static CCookie* NewL( RStringPool aStringPool );


		/**
		* Standard destructor.
		*/
		IMPORT_C virtual ~CCookie();


		/**
		* This method writes the cookie (i.e. its attributes) into the
		* specified part of the HTTP header. The method may leave if writing
		* is not successful (e.g. when RHTTPHeaders::SetParamL leaves) or if
		* the cookie is not properly initialized.
		* @param aRequestHeaders The current HTTP header collection to which we
		* add this cookie,
		* @param aPartIndex The part of the header.
		*/
		IMPORT_C void AddToRequestL( RHTTPHeaders aRequestHeaders,
									TInt aPartIndex ) const;
		

		/** Attribute retrieves the value of an attribute
		* @param	aAttributeName	Identifies the attribute 
		* @param	aAttributeVal	The value being retrieved
		* @return	An error code. KErrNotFound will indicate that the cookie
		* doesn't contain the specified attribute.
		*/
		IMPORT_C TInt Attribute( TCookieAttributeName aAttributeName,
								THTTPHdrVal& aAttributeVal ) const;


		/** SetAttribute sets an attribute on the cookie. 
		* Adding an attribute where one exists will replace the existing one
		* @param aAttributeName identifies the attribute 
		* @param aAttributeVal the value being set
		* @return An error code
		*/
		IMPORT_C TInt SetAttribute( TCookieAttributeName aAttributeName,
									const THTTPHdrVal& aAttributeVal );
		

	public :	// public inline methods
		/**
		* Did this cookie come from a SetCookie2 header?
		* @return Flag indicating if this cookie was received in SetCookie2
		* header (rather than a SetCookie header).
		*/
		inline TBool FromCookie2() const;


		/**
		* Did this cookie come from a Netscape-version header?
		* @return Flag indicating if this cookie was received 
        * in a Netscape-version header
		*/
		inline TBool FromNetscapeVersion() const;


		/**
		* Use this method to specify whether this cookie came from a SetCookie2
		* header.
		* @param aFromCookie2 ETrue, if the cookie came from a SetCookie2
		* header, EFalse otherwise.
		*/
		inline void SetCookie2( TBool aFromCookie2 );


		/**
		* Use this method to specify whether this cookie is a Netscape style
		* one or an RFC compliant.
		* @param aFromNetscape ETrue, if the cookie is a Netscape style one, 
		* EFalse otherwise.
		*/
        inline void SetFromNetscape( TBool aFromNetscape );


		/**
		* Returns the number of bytes this cookie would require if it was
		* stored in a flat buffer and one attribute followed the other.
		* Note : the attributes are stored in a way that we first write out
		* the size (TInt) of the attribute in question and then the attribute
		* value. E.g. assuming a NAME attribute with the value of SESSIONID, we
		* write out 9 as the length of NAME (again, stored as TInt) and then
		* write out the value of NAME (i.e. SESSIONID).
		* @param aSuppressDefaulted If ETrue, the method does not add the
		* size of defaulted attributes to the overall size of the cookie.
		* @return The size of the cookie.
		*/
		inline TInt Size( TBool aSuppressDefaulted ) const;


	public :	// public methods
		/** Attribute retrieves the value of an attribute
		* @param	aAttributeName	Identifies the attribute 
		* @param	aAttributeVal	The value being retrieved
		* @param	aDefaulted	A TBool indicating whether the current 
		*	attribute's value is defaulted or specified.
		* @return	An error code. KErrNotFound will indicate that the cookie
		* doesn't contain the specified attribute.
		*/
		TInt Attribute( TCookieAttributeName aAttributeName,
						THTTPHdrVal& aAttributeVal,
						TBool& aDefaulted ) const;


		/** SetAttribute sets an attribute on the cookie. 
		* Adding an attribute where one exists will replace the existing one
		* @param aAttributeName identifies the attribute 
		* @param aAttributeVal the value being set
		* @param aDefaulted a TBool indicating whether it is a defaulted 
		* or a supplied value
		* @return An error code
		*/
		TInt SetAttribute(  TCookieAttributeName aAttributeName,
							const THTTPHdrVal& aAttributeVal,
							TBool aDefaulted );
        

		/**
		* Removes a cookie attribute. The attribute is closed, removed from
		* the attribute list and the cookie size is also updated.
		* @param aAttributeName identifies the attribute
		*/
		void RemoveAttribute( TCookieAttributeName aAttributeName );
		
        /**
        * Copy from an already existing one.
        * @param aCopyFrom The already existing CCookie object
        */
        void CopyFromL( const CCookie& aCopyFrom, const TDesC8& aDomain, const TDesC8& aPath, const TDesC8& aPort  );

        /**
        * Returns this cookie's StringPool.
        */
        RStringPool StringPool();


		/**
		* Returns whether This cookie has expired or not.
        * @return	TBool indicating whether the cookie had expired or not.
		*/
		TBool Expired() const;


		/**
		* Returns whether This cookie is persistent or not.
        * @return	TBool indicating whether the cookie is persistent or not.
		*/
		TBool Persistent() const;


        /**
        * Returns whether the cookie's Version attribute is known or not.
        * @return TBool indicating the cookies's Version attribute's notoriety.
        */
        TBool IsUnknownVersion() const;
        static CCookie* CloneL( const CCookie& aCopy,const TDesC8& aDomain, const TDesC8& aPath, const TDesC8& aPort);

	private :	// class for internal use
        /**
        * Determines if the cookie had expired in case of Netscape style
        * Method may leave with 
        *   KErrNoMemory 
        *   KErrNotFound, if parsing the date string fails (not a well formed string)
        * @param aAttrVal The THTTPHdrVal holding the Expires date as a string
        * @return A TBool holding the result
        */
        TBool ExpiredNetscapeL( THTTPHdrVal aAttrVal ) const;
        
        /* For logging Cookie attributes
         * 
         */
        void Log( TInt aAssociatedId = 0 ) const;
        /**
        * Tokenize a string.
        * @return actual number of tokens
        * @param aString string to be tokenized
        * @param aTokenSeparators separator characters
        * @param aTokenCount expected number of tokens
        * @param aTokens position of token separators found in aString
        */
        TInt FindTokens( const TDesC8& aString, 
                         const TDesC8& aTokenSeparators, 
                         const TInt aTokenCount, 
                         TInt* aTokens ) const;
		/**
		* Class to store the name, value and the size of the value
		* of an individual cookie attribute
		*/
		class TCookieAttribute
			{
			public :	// constructor
				/**
				* Constructor that initialises the name, value and size
				* attributes.
				* @param aName Identifies the attribute to be set
				* @param aHdrVal The value of the attribute.
				*/
				TCookieAttribute( TCookieAttributeName aName,
								const THTTPHdrVal& aHdrVal, 
								TBool aDefaulted = EFalse );

				/**
				* Default constructor. It creates an uninitialized cookie
				* attribute that may be initialized by the common = operator.
				*/
				TCookieAttribute();

			public :	// accessor methods
						// note that there are no 'set' methods as the only way
						// to initialize a cookie attribute (apart from calling
						// the detailed constructor) is using the = operator.
				/**
				* Get the name of the cookie attribute.
				*/
				TCookieAttributeName Name() const;

				/**
				* Get the value of the cookie attribute.
				*/
				const THTTPHdrVal& Value() const;

				/**
				* Get the size of the cookie attribute in bytes.
				*/
				TInt Size() const;

				/**
				* Gets whether the attribute is defulted or specified.
				*/
				TBool Defaulted() const;

			public :	// other common methods
				/**
				* Closes an attribute - used to ensure string attributes are
				* properly closed on destruction.
				*/
				void Close();

			private :	// data members
				/** The cookie attribute
				*/
				TCookieAttributeName iName;

				/** The value of this attribute
				*/
				THTTPHdrVal iValue;

				/**
				* The size of this attribute in bytes. If this attribute
				* refers to a string (folded or normal), then it is the length
				* of the string, otherwise it contains the size in bytes
				* returned by sizeof() operator.
				*/
				TInt iSize;

				/**
				* Stores that this attribute is defaulted ( ETrue )
				* or was originally supplied ( EFalse )
				* Needed to be able to decide whether the attribute should be 
				* added when returning cookies to server
				*/
				TBool iDefaulted;
				};

	private :	// constructor
		/**
		* Constructor.
		* @param aStringPool The current string pool.
		*/
		CCookie( RStringPool aStringPool );

		/**
		* Constructor.
		* @param aRequestHeaders A collection of header fields associated
		* with the current HTTP message,
		* @param aPartIndex The index of the part in the header,
		* @param aFieldName The HTTP header name,
		* @param aUri HTTP request's URI.
		*/
		void ConstructL( RHTTPHeaders aRequestHeaders,
						 TInt aPartIndex,
						 RStringF aFieldName,
						 const TUriC8& aUri );

	private :	// common internal methods
		/**
		* When the cookie does not have a Path attribute, we have to add
		* a default Path attribute. This method does this addition.
		* @param aUri An initialized URI parser.
		*/
		void AddDefaultPathL( const TUriC8& aUri );

		/** Finds the specified attribute in this cookie
			@param aAttribute		
			@param aAttributeName	
			@return The index in iAttributes of the specified attribute.
			KErrNotFound indicates that the cookie does not contain the
			attribute.
		*/
		TInt FindAttribute( TCookieAttributeName aAttributeName,
							TCookieAttribute& aAttribute ) const;

        /**
        *
        */
        TInt GetLocalOffset( HBufC8* aDate, TInt& aHour, 
                             TInt& aMinute, TInt& aLength ) const;

        /**
        *
        */
        TInt GetMilitaryOffset( HBufC8* aDate, TInt& aHour ) const;

        /**
        *
        */
        TInt GetTimeZone( HBufC8* aDate, TInt& aHour, 
                         TInt& aLength ) const;


        /**
        * Takes an HTTP header date-time information and returnes it in a 
        * format supported by TTime. 
        */
        HBufC8* GetCleanedDateTimeLC( HBufC8* aDate ) const;

		/**
		* Removes a cookie attribute. The attribute is closed, removed from
		* the attribute list and the cookie size is also updated.
		* @param aIndex The position of the cooke attribute in the attribute
		* list.
		*/
		void RemoveAttribute( TInt aIndex );

	private :	// data members
		/** String pool that can be used by this cookie
		*/
		RStringPool iStringPool;
		
		/** Array of the attributes of this cookie
		*/
		RArray<TCookieAttribute> iAttributes;

		/**
		* Flag indicating whether this cookie was received in a
		* SetCookie2 header
		*/
		TBool iSetCookie2;

        /**
		* Flag indicating whether this cookie is Netscape or RFC compliant
		* It is important to know it as they have different fields,
        * different defaulting and handling.
		*/
		TBool iNetscape;

		TInt iSize;
		TTime iReceivedTime;
	};

#include <cookie.inl>

#endif //	__CCOOKIE_H__

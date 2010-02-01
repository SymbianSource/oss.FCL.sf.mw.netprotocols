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
// Place holder for common constants, type definitions and enums.
//
//

/**
 @file
 @publishedAll
 @released
*/

#ifndef __INETURILISTDEF_H__
#define __INETURILISTDEF_H__

#include <e32base.h>

/**
@publishedAll
@released
*/
namespace InetUriList
	{
	/**
	Types of supported service type.
	
	@publishedAll
	@released
	*/		
	enum TServiceType
		{
		/** Browser */
		EBrowser =0x01, 
		/** WapPush */
		EWapPush =0x02,
		/** PushEMail */ 
		EPushEMail =0x03,
		/** DevProv */
		EDevProv =0x04,
		/** VOIP */
		EVoip =0x05
		};

	/**
	Types of supported list type.

	@publishedAll
	@released
	*/	
	 enum TListType
		{
		/** BlackList **/
		EBlackList =0x01,
		/** WhiteList **/
		EWhiteList =0x02
		};
	
	/**
	Types of permission.

	@publishedAll
	@released
	*/	
	enum TPermission
		{
		/** ReadOnly **/
		EReadOnly =0x01,
		/** ReadWrite **/ 
		EReadWrite =0x02
		};
	
	/**
	Types of TLD Query type.
	
	@publishedAll
	@released
	*/
	enum TTLDQueryType
		{
		/** Top Level Domain List type **/
		EPolicyListType=0x01,
		/** Top Level Domain Policy data **/
		EPolicyCharSet=0x02
		};
	
	/**
	Types of matches.
	
	@publishedAll
	@released
	*/
	enum TURIMatch
		{
		/** Exact match **/	
		EExact =0x01,
		/** Domain match **/
		EDomain =0x02,
		/** Exact path match **/
		EExactPath =0x03,
		/** partial prefix path match **/
		EPartialPrefixPath =0x04,
		/** Partial suffix path match **/
		EPartialSuffixPath =0x05
		};
		
	/**
	Error codes error numbers -7650 to -7699 have been reserved for InetUriList.
	*/
	static const TInt KErrorBase 			= -7650;	
	/**
	URI already exists in database.
	*/
	static const TInt KErrUriAlreadyExists 			= KErrorBase;
	/**
	URI not found in database. 
	*/
	static const TInt KErrUriNotFound	 			= KErrorBase - 1;
	/** 
	Addition of URI failed. 
	*/
	static const TInt KErrUriAddFailed				= KErrorBase - 2;
	/** 
	Removal of URI failed.
	*/
	static const TInt KErrUriRemoveFailed			= KErrorBase - 3;
	/** 
	Updation of URI properties failed.
	*/
	static const TInt KErrUriUpdateFailed			= KErrorBase - 4;
	/** 
	URI is read only.
	*/
	static const TInt KErrUriReadOnly				= KErrorBase - 5;
	/** 
	Service type of URI is missing.
	*/
	static const TInt KErrServiceTypeNotPresent 	= KErrorBase - 6;	
	/** 
	Handle is not open.
	*/
	static const TInt KErrNotOpen				 	= KErrorBase - 7;	
	/**
	Invalid stream handle
	*/
	static const TInt KErrInvalidStreamHandle 		= KErrorBase - 8;
	/** 
	Tld URI is missing.
	*/
	static const TInt KErrTldUriNotPresent 			= KErrorBase - 9;
	/** 
	Query Type of URI is missing.
	*/
	static const TInt KErrTldQueryTypeNotPresent 	= KErrorBase - 10;
	/** 
	Policy Data is not present.
	*/
	static const TInt KErrPolicyDataNotPresent 		= KErrorBase - 11;	
	/** 
	TLD list type is not present.
	*/
	static const TInt KErrPolicyListTypeNotPresent	= KErrorBase - 12;	
	/** 
	Requested TLD  type is not supported.
	*/
	static const TInt KErrInvalidTLD				= KErrorBase - 13;	
	/** 
	Panic text if handle is not open.
	*/
	_LIT(KInetUriListErrHandleNotOpen,		"Handle not open");
	/** 
	Panic text if handle is not open.
	*/
	_LIT(KTldInvalidRequest,				"Invalid Request");

	}

/**
This class defines the argument parameters for the query operation. It is possible to 
set the arguments in various combinations. The argument types can be ServiceType, 
ListType, URI in TDesC8 form, and URIMatch.

@publishedAll
@released
*/
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
class TQueryArgs
	{
  	public:
		/**
		The argument types.	
		*/
		enum TArgType
			{
			EUri = 1,
			EServiceType,
			EListType,
			EURIMatch
			};
		
		/**
		Default constructor
		*/
		inline TQueryArgs ()
			:iFlags ( 0 )
			{}


		/**
		A templated constructor that constructs the query argument.
		It takes one argument.
		*/
		template < class T0 >
		explicit inline TQueryArgs ( T0 a0 )
			{
			Assign ( a0 );
			iFlags=(Type(a0)<<(( Type(a0))*KBitsPerType));
			}

		/**
		A templated constructor that constructs the query argument.
		It takes two arguments.
		*/
		template < class T0, class T1 >
		inline TQueryArgs ( T0 a0, T1 a1 )
			{
			Assign ( a0 );
			Assign ( a1 );
			iFlags=(Type(a0)<<(( Type(a0))*KBitsPerType)) |
					(Type(a1)<<(( Type(a1))*KBitsPerType));
			}

		/**
		A templated constructor that constructs the query argument.
		It takes three arguments.
		*/
		template < class T0, class T1, class T2 >
		inline TQueryArgs ( T0 a0, T1 a1, T2 a2 )
			{
			Assign ( a0 );
			Assign ( a1 );
			Assign ( a2 );
			iFlags=(Type(a0)<<(Type(a0)*KBitsPerType)) | 
					(Type(a1)<<(Type(a1)*KBitsPerType)) |
					(Type(a2)<<(Type(a2)*KBitsPerType));
			}

		/**
		A templated constructor that constructs the query argument.
		It takes four arguments.
		*/
		template < class T0, class T1, class T2, class T3 >
		inline TQueryArgs ( T0 a0, T1 a1, T2 a2, T3 a3 )
			{
			Assign ( a0 );
			Assign ( a1 );
			Assign ( a2 );
			Assign ( a3 );				
			iFlags=(Type(a0)<<(Type(a0)*KBitsPerType)) | 
					(Type(a1)<<(Type(a1)*KBitsPerType)) |
					(Type(a2)<<(Type(a2)*KBitsPerType)) |
					(Type(a3)<<(Type(a3)*KBitsPerType));
			}
		
		/**
		Returns the argument if set, otherwise returns KErrNotFound.		
		*/		
		TInt Get ( TArgType aType ) const
			{
			if ( IsSet ( aType ) )
				return iArgs[aType - 1];
			return KErrNotFound;			
			}
							
	private:		
		/**
		Bit width of the type.		
		*/
		enum 
			{
			KBitsPerType = 3			
			};
		/**
		Maximum number of arguments. Currently set as 4.
		*/
		enum
			{
			KMaxArguments = 4
			};
		/**
		Checks whether the flag is set for the given argument type.
		*/
		TBool IsSet ( TArgType aType ) const
			{
			TInt val = iFlags & ( aType << ( aType  * KBitsPerType ) );
			return iFlags & ( aType << ( aType  * KBitsPerType ) );
			}

		TArgType Type ( const TDesC8* )
			{
			return EUri;				
			}
			
		TArgType Type ( InetUriList::TServiceType )
			{
			return EServiceType;				
			}

		TArgType Type ( InetUriList::TListType )
			{
			return EListType;				
			}

		TArgType Type ( InetUriList::TURIMatch )
			{
			return EURIMatch;				
			}
		
		void Assign ( const TDesC8* aValue )
			{
			iArgs[Type(aValue)-1] = (TInt)aValue;
			}
			
		void Assign ( InetUriList::TServiceType aValue )
			{
			iArgs[Type(aValue)-1] = aValue;
			}
			
		void Assign ( InetUriList::TListType aValue )
			{
			iArgs[Type(aValue)-1] = aValue;
			}
			
		void Assign ( InetUriList::TURIMatch aValue )
			{
			iArgs[Type(aValue)-1] = aValue;
			}
			
	private:	
		TInt iArgs [KMaxArguments];
		TInt iFlags;
	};

#else

class TQueryArgs
	{
	public:
		/**
		The argument types.
		
		@internalComponent
		*/
		enum TArgType
			{
			EUri = 1,
			EServiceType,
			EListType,
			EURIMatch
			};
		/**
		Bit width of the type.
		
		@internalComponent
		*/
		enum 
			{
			KBitsPerType = 3			
			};
		
		/**
		Default constructor
		*/
		inline TQueryArgs ()
			:iFlags ( 0 )
			{}


		/**
		A templated constructor that constructs the query argument.
		It takes one argument.
		*/
		template < class T0 >
		explicit inline TQueryArgs ( T0 a0 )
			{
			Assign ( a0 );
			iFlags=(Type(a0)<<(( Type(a0))*KBitsPerType));
			}

		/**
		A templated constructor that constructs the query argument.
		It takes two arguments.
		*/
		template < class T0, class T1 >
		inline TQueryArgs ( T0 a0, T1 a1 )
			{
			Assign ( a0 );
			Assign ( a1 );
			iFlags=(Type(a0)<<(( Type(a0))*KBitsPerType)) |
					(Type(a1)<<(( Type(a1))*KBitsPerType));
			}

		/**
		A templated constructor that constructs the query argument.
		It takes three arguments.
		*/
		template < class T0, class T1, class T2 >
		inline TQueryArgs ( T0 a0, T1 a1, T2 a2 )
			{
			Assign ( a0 );
			Assign ( a1 );
			Assign ( a2 );
			iFlags=(Type(a0)<<(Type(a0)*KBitsPerType)) | 
					(Type(a1)<<(Type(a1)*KBitsPerType)) |
					(Type(a2)<<(Type(a2)*KBitsPerType));
			}

		/**
		A templated constructor that constructs the query argument.
		It takes four arguments.
		*/
		template < class T0, class T1, class T2, class T3 >
		inline TQueryArgs ( T0 a0, T1 a1, T2 a2, T3 a3 )
			{
			Assign ( a0 );
			Assign ( a1 );
			Assign ( a2 );
			Assign ( a3 );				
			iFlags=(Type(a0)<<(Type(a0)*KBitsPerType)) | 
					(Type(a1)<<(Type(a1)*KBitsPerType)) |
					(Type(a2)<<(Type(a2)*KBitsPerType)) |
					(Type(a3)<<(Type(a3)*KBitsPerType));
			}

			
		/**
		Checks whether the flag is set for the given argument type.
				
		@internalComponent
		*/
		TBool IsSet ( TArgType aType ) const
			{
			TInt val = iFlags & ( aType << ( aType  * KBitsPerType ) );
			return iFlags & ( aType << ( aType  * KBitsPerType ) );
			}
		
		/**
		Returns the argument if set, otherwise returns KErrNotFound.
		
		@internalComponent
		*/		
		TInt Get ( TArgType aType ) const
			{
			if ( IsSet ( aType ) )
				return iArgs[aType - 1];
			return KErrNotFound;			
			}
		
		/**
		Maximum number of arguments. Currently set as 4.

		@internalComponent
		*/
		enum
			{
			KMaxArguments = 4
			};
					
	private:

		/**
		@internalComponent
		*/			
		TArgType Type ( const TDesC8* )
			{
			return EUri;				
			}
			
		TArgType Type ( InetUriList::TServiceType )
			{
			return EServiceType;				
			}

		TArgType Type ( InetUriList::TListType )
			{
			return EListType;				
			}

		TArgType Type ( InetUriList::TURIMatch )
			{
			return EURIMatch;				
			}
		
		void Assign ( const TDesC8* aValue )
			{
			iArgs[Type(aValue)-1] = (TInt)aValue;
			}
			
		void Assign ( InetUriList::TServiceType aValue )
			{
			iArgs[Type(aValue)-1] = aValue;
			}
			
		void Assign ( InetUriList::TListType aValue )
			{
			iArgs[Type(aValue)-1] = aValue;
			}
			
		void Assign ( InetUriList::TURIMatch aValue )
			{
			iArgs[Type(aValue)-1] = aValue;
			}
			
	private:	
		TInt iArgs [KMaxArguments];
		TInt iFlags;
	};


#endif	//SYMBIAN_ENABLE_SPLIT_HEADERS
	
#endif // __INETURILISTDEF_H__

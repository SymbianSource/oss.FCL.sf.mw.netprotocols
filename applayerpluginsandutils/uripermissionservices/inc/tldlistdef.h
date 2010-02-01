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


/** @file
@publishedAll
@released
*/

#ifndef __TLDLISTDEF_H__
#define __TLDLISTDEF_H__

#include <e32base.h>
#include <ineturilistdef.h>

using namespace InetUriList;

#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS

class TBase
	{
  friend class TPolicyQueryArgs;
  friend class TQueryResults;

public:			
	/**
	Default constructor
	*/
	inline TBase ()
		:iFlags ( 0 )
		{}

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

protected:	
	TInt iArgs [KMaxArguments];
	TInt iFlags;
	};

//-----------------------------------------------------------------
class TPolicyQueryArgs:public TBase
	{
	public:
		/**
		The argument types.		
		*/
		enum TPolicyArgType
			{
			ETldUri = 1,
			ETldListType,
			ETldQueryType
			};

		/**
		Default constructor
		*/
		inline TPolicyQueryArgs ()
			:TBase()
			{}

		/**
		A templated constructor that constructs the query argument.
		It takes one argument.
		*/
		template < class T0 >
		explicit inline TPolicyQueryArgs ( T0 a0 )
			{
			Assign ( a0 );
			iFlags=(Type(a0)<<(( Type(a0))*KBitsPerType));
			}

		/**
		A templated constructor that constructs the query argument.
		It takes two arguments.
		*/
		template < class T0, class T1 >
		inline TPolicyQueryArgs ( T0 a0, T1 a1 )
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
		inline TPolicyQueryArgs ( T0 a0, T1 a1, T2 a2 )
			{
			Assign ( a0 );
			Assign ( a1 );
			Assign ( a2 );
			iFlags=(Type(a0)<<(Type(a0)*KBitsPerType)) | 
					(Type(a1)<<(Type(a1)*KBitsPerType)) |
					(Type(a2)<<(Type(a2)*KBitsPerType));
			}
		/**
		Returns the argument if set, otherwise returns KErrNotFound.	
		*/		
		TInt Get ( TPolicyArgType aType ) const
			{
			if ( IsSet ( aType ) )
				return iArgs[aType - 1];
			return KErrNotFound;			
			}
						
	private:	   
		/**
		Checks whether the flag is set for the given argument type.		
		*/
		TBool IsSet ( TPolicyArgType aType ) const
			{
			TInt val = iFlags & ( aType << ( aType  * KBitsPerType ) );
			return iFlags & ( aType << ( aType  * KBitsPerType ) );
			}		
		
		TPolicyArgType Type ( const TDesC8* )
			{
			return ETldUri;				
			}
			
		TPolicyArgType Type ( InetUriList::TListType )
			{
			return ETldListType;				
			}

		TPolicyArgType Type ( InetUriList::TTLDQueryType )
			{
			return ETldQueryType;				
			}
		
		void Assign ( const TDesC8* aValue )
			{
			iArgs[Type(aValue)-1] = (TInt)aValue;
			}
			
		void Assign ( InetUriList::TListType aValue )
			{
			iArgs[Type(aValue)-1] = aValue;
			}
			
		void Assign ( InetUriList::TTLDQueryType aValue )
			{
			iArgs[Type(aValue)-1] = aValue;
			}
			
	
	};

#else
class TBase
	{
public:	
	/**
	Bit width of the type.
	
	@internalComponent
	*/
	enum 
		{
		KBitsPerType = 3			
		};
	
	/**
	Maximum number of arguments. Currently set as 4.

	@internalComponent
	*/
	enum
		{
		KMaxArguments = 4
		};
	
	/**
	Default constructor
	*/
	inline TBase ()
		:iFlags ( 0 )
		{}

protected:	
	TInt iArgs [KMaxArguments];
	TInt iFlags;
	};

//-----------------------------------------------------------------
class TPolicyQueryArgs:public TBase
	{
	public:
		/**
		The argument types.
		
		@internalComponent
		*/
		enum TPolicyArgType
			{
			ETldUri = 1,
			ETldListType,
			ETldQueryType
			};

		/**
		Default constructor
		*/
		inline TPolicyQueryArgs ()
			:TBase()
			{}


		/**
		A templated constructor that constructs the query argument.
		It takes one argument.
		*/
		template < class T0 >
		explicit inline TPolicyQueryArgs ( T0 a0 )
			{
			Assign ( a0 );
			iFlags=(Type(a0)<<(( Type(a0))*KBitsPerType));
			}

		/**
		A templated constructor that constructs the query argument.
		It takes two arguments.
		*/
		template < class T0, class T1 >
		inline TPolicyQueryArgs ( T0 a0, T1 a1 )
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
		inline TPolicyQueryArgs ( T0 a0, T1 a1, T2 a2 )
			{
			Assign ( a0 );
			Assign ( a1 );
			Assign ( a2 );
			iFlags=(Type(a0)<<(Type(a0)*KBitsPerType)) | 
					(Type(a1)<<(Type(a1)*KBitsPerType)) |
					(Type(a2)<<(Type(a2)*KBitsPerType));
			}

		/**
		Checks whether the flag is set for the given argument type.
				
		@internalComponent
		*/
		TBool IsSet ( TPolicyArgType aType ) const
			{
			TInt val = iFlags & ( aType << ( aType  * KBitsPerType ) );
			return iFlags & ( aType << ( aType  * KBitsPerType ) );
			}
		
		/**
		Returns the argument if set, otherwise returns KErrNotFound.
		
		@internalComponent
		*/		
		TInt Get ( TPolicyArgType aType ) const
			{
			if ( IsSet ( aType ) )
				return iArgs[aType - 1];
			return KErrNotFound;			
			}
		
				
	private:

		/**
		@internalComponent
		*/			
		TPolicyArgType Type ( const TDesC8* )
			{
			return ETldUri;				
			}
			
		TPolicyArgType Type ( InetUriList::TListType )
			{
			return ETldListType;				
			}

		TPolicyArgType Type ( InetUriList::TTLDQueryType )
			{
			return ETldQueryType;				
			}
		
		void Assign ( const TDesC8* aValue )
			{
			iArgs[Type(aValue)-1] = (TInt)aValue;
			}
			
		void Assign ( InetUriList::TListType aValue )
			{
			iArgs[Type(aValue)-1] = aValue;
			}
			
		void Assign ( InetUriList::TTLDQueryType aValue )
			{
			iArgs[Type(aValue)-1] = aValue;
			}
			
	
	};


#endif	//SYMBIAN_ENABLE_SPLIT_HEADERS

	
//------------------------------------------------------------------------
class TQueryResults: public TBase
	{
	public:
		/**
		Default constructor
		*/
		inline TQueryResults ()
			:TBase()
			{}
		
		~TQueryResults ()
			{
			if ( IsSet( TQueryResults::ETldCharSet ) )
				{
				//Delete memory on Heap.
				HBufC8* uriBuf = (reinterpret_cast<HBufC8*> ( Get ( TQueryResults::ETldCharSet ) ));
				delete uriBuf;
				}
			}
			
		/**
		returns a pointer to policydata
		*/
		HBufC8* CharsetL()
			{
			__ASSERT_ALWAYS( IsSet(TQueryResults::ETldCharSet), User::Panic( KTldInvalidRequest, KErrPolicyDataNotPresent ));
			const TDesC8& uri = *( reinterpret_cast<TDesC8*> (Get ( TQueryResults::ETldCharSet ) ) );
			return uri.AllocL();
			}

		/**
		returns a list type
		*/
		InetUriList::TListType ListType()
			{
			__ASSERT_ALWAYS( IsSet(TQueryResults::ETldListType), User::Panic( KTldInvalidRequest, KErrPolicyListTypeNotPresent ));
			return (static_cast<InetUriList::TListType> (Get ( TQueryResults::ETldListType ) ));
			}
			
		/**
		A templated constructor that constructs the query argument.
		It takes one argument. --Check this
		*/
		template < class T0 >
		inline void Set ( T0 a0 )
			{
			Assign ( a0 );
			iFlags=(Type(a0)<<(( Type(a0))*KBitsPerType));
			}

	private:
		#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
		/**
		The argument types.	
		*/
		#else
		/**
		The argument types.
		
		@internalComponent
		*/
		#endif	//SYMBIAN_ENABLE_SPLIT_HEADERS
		enum TResultsArgType
			{
			ETldCharSet = 1,
			ETldListType
			};	
		
		#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
       /**
		Checks whether the flag is set for the given argument type.		
		*/
		#else
		/**
		Checks whether the flag is set for the given argument type.
				
		@internalComponent
		*/
		#endif	//SYMBIAN_ENABLE_SPLIT_HEADERS
		TBool IsSet ( TResultsArgType aType ) const
			{
			TInt val = iFlags & ( aType << ( aType  * KBitsPerType ) );
			return iFlags & ( aType << ( aType  * KBitsPerType ) );
			}

		#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
		/**
		Returns the argument if set, otherwise returns KErrNotFound.		
		*/	
		#else
		/**
		Returns the argument if set, otherwise returns KErrNotFound.
		
		@internalComponent
		*/		
		#endif	//SYMBIAN_ENABLE_SPLIT_HEADERS
		TInt Get ( TResultsArgType aType ) const
			{
			if ( IsSet ( aType ) )
				return iArgs[aType - 1];
			return KErrNotFound;			
			}		
	
	private:
		#ifndef SYMBIAN_ENABLE_SPLIT_HEADERS
		/**
		@internalComponent
		*/			
		#endif	//SYMBIAN_ENABLE_SPLIT_HEADERS
		
		TResultsArgType Type ( const TDesC8* )
			{
			return ETldCharSet;				
			}
			
		TResultsArgType Type ( InetUriList::TListType )
			{
			return ETldListType;				
			}

		void Assign ( const TDesC8* aValue )
			{
			iArgs[Type(aValue)-1] = (TInt)aValue;
			}
			
		void Assign ( InetUriList::TListType aValue )
			{
			iArgs[Type(aValue)-1] = aValue;
			}
	};

#endif // __TLDLISTDEF_H__


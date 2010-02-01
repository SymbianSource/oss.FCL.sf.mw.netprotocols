// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <e32base.h>
#include <ineturilistdef.h>
#include <tldlistdef.h>
#include "dbaccessor.h"
#include "uriqueryfilter.h"
#include "urilist.h"


CUriQueryFilter* CUriQueryFilter::NewL ( MDBTransaction* aDBTrans )
	{
	CUriQueryFilter* self = new ( ELeave ) CUriQueryFilter;
	// Pass the ownnership
	self->Initialize ( aDBTrans );
	return self;
	}

CUriQueryFilter::CUriQueryFilter ()
	{
		
	}

CUriQueryFilter::~CUriQueryFilter ()
	{
	if ( iDBTransaction )
		iDBTransaction->Release();		
	}


MDBTransaction& CUriQueryFilter::DBTransaction() const
	{
	return *iDBTransaction;	
	}

TBool CUriQueryFilter::MatchRecordL ()
	{
	return ETrue;		
	}

/**
Counts the number of records present for the operation performed.
*/
TInt CUriQueryFilter::MatchRecordCountL ()
	{
	TInt totalCount = 0;
	while ( iDBTransaction->Next() && MatchRecordL () )
		{
		++totalCount;					
		}
	return totalCount;			
	}

void CUriQueryFilter::Initialize ( MDBTransaction* aDbTrans )
	{
	iDBTransaction = aDbTrans;		
	}
// -------------------------------

CUriPathFilter* CUriPathFilter::NewL ( MDBTransaction* aDBTrans, InetUriList::TURIMatch aMatch, const TDesC8& aPartialPath )
	{
	CUriPathFilter* self = new (ELeave)CUriPathFilter;
	CleanupStack::PushL ( self );
	self->ConstructL (  aDBTrans, aPartialPath, aMatch );
	CleanupStack::Pop (); // self
	return self;
	}

CUriPathFilter::CUriPathFilter ( )
	{		
	}

void CUriPathFilter::ConstructL ( MDBTransaction* aDBTrans, const TDesC8& aPartialPath, InetUriList::TURIMatch aMatch )
	{
	_LIT8 ( KMatchChar, "*" );
	iPartialPath = HBufC8::NewL ( aPartialPath.Length() + 1 );
	TPtr8 ptr ( iPartialPath->Des() );
	
	ptr.Copy ( aPartialPath );
	if ( aMatch == InetUriList::EPartialPrefixPath )
		{
		ptr.Append ( KMatchChar() );
		}
	else if ( aMatch == InetUriList::EPartialSuffixPath )
		{
		ptr.Insert ( 0, KMatchChar() );
		}
	
	// Now it is safe to pass the ownership of the aDbTrans object
	Initialize ( aDBTrans );
	}

CUriPathFilter::~CUriPathFilter ()
	{
	delete iPartialPath;		
	}

/**
Only partial suffix & prefix mathes will be performed. If the path search criteria matches function returns 
ETrue. If the search criteria doesn't match function returns EFalse
*/
TBool CUriPathFilter::MatchRecordL ()
	{
	TPtrC8 path ( iDBTransaction->ColumnTextL ( URILIST::EPath ) );
	TInt ret = path.Match ( *iPartialPath );
	return (ret != KErrNotFound) ? ETrue : EFalse;
	}
// ------------------------------------

CUriDomainFilter* CUriDomainFilter::NewL ( MDBTransaction* aDBTrans, const TDesC8& aDomainName )
	{
	CUriDomainFilter* self = new (ELeave)CUriDomainFilter;
	CleanupStack::PushL ( self );
	self->ConstructL ( aDBTrans, aDomainName );
	CleanupStack::Pop (); // self
	return self;
	}

CUriDomainFilter::CUriDomainFilter ( )
	{		
	}

void CUriDomainFilter::ConstructL ( MDBTransaction* aDBTrans, const TDesC8& aDomainName )
	{
	iDomainName = aDomainName.AllocL ();
	iComparisonStr.Set ( IsHDNStringWithNBForm ( *iDomainName ) );
	// Now it is safe to pass the ownership of the aDbTrans object
	Initialize ( aDBTrans );	
	}

CUriDomainFilter::~CUriDomainFilter ()
	{
	delete iDomainName;		
	}

/**
Filters the query results for domain matching. The domain matching algorithm is 
implemented as said in RFC2965 Sec 1
*/
TBool CUriDomainFilter::MatchRecordL ()
	{
	TPtrC8 host ( iDBTransaction->ColumnTextL ( URILIST::EHost ) );
	TPtrC8 domainPtr ( iDomainName->Des() );
	// RFC2965 Sec 1 says	
	// Host A's name domain-matches host B's if
    // *  their host name strings string-compare equal; or
    // * A is a HDN string and has the form NB, where N is a non-empty
    // name string, B has the form .B', and B' is a HDN string.  (So,
    // x.y.com domain-matches .Y.com but not Y.com.)
    TBool result = EFalse;
    // First look for exact mathc
	if ( domainPtr.Compare( host ) == 0 )
		{
		result = ETrue;				
		}
	else if ( iComparisonStr.Length() )
		{
		
		TPtrC8 hostComparisonPtr ( IsHDNStringWithNBForm ( host ) );
		// Compare with the extracted string or do a direct compare with host.
		if ( ( iComparisonStr.Compare ( hostComparisonPtr ) == 0 )  ||
			 ( iComparisonStr.Compare ( host ) == 0 ) )
			result = ETrue;				
		}			
	return result;	
	}

/**
Checks the domain name is in NB form. See CUriDomainFilter::MatchRecordL for more
details for the NB form.
*/
TPtrC8 CUriDomainFilter::IsHDNStringWithNBForm ( const TDesC8& aDomainName )
	{
	const TChar KDot = '.';
	TInt firstDotPos = 	aDomainName.Locate ( KDot );
	if ( firstDotPos == KErrNotFound || ( ( firstDotPos + 1 ) > aDomainName.Length() ) )
		return KNullDesC8(); // Not a valid domain name
		
	TPtrC8 midPtr ( aDomainName.Mid ( firstDotPos + 1 ) );
	
	if ( midPtr.Locate( KDot ) != KErrNotFound )
		return midPtr;
	
	return KNullDesC8();
	
	}

//----------------------------------------------------

CTldPolicyDataFilter* CTldPolicyDataFilter::NewL ( MDBTransaction* aDBTrans )
	{
	CTldPolicyDataFilter* self = new ( ELeave ) CTldPolicyDataFilter;
	// Pass the ownnership
	self->ConstructL ( aDBTrans );
	return self;
	}

void CTldPolicyDataFilter::ConstructL ( MDBTransaction* aDBTrans )
	{
	Initialize ( aDBTrans );
	}
	
CTldPolicyDataFilter::CTldPolicyDataFilter ()
	{
		
	}

CTldPolicyDataFilter::~CTldPolicyDataFilter ()
	{
	}


TBool CTldPolicyDataFilter::MatchRecordL ()
	{
	return ETrue;		
	}


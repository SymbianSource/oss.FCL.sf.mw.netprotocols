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

#ifndef __URIQUERYFILTER_H__
#define __URIQUERYFILTER_H__

class CUriQueryFilter : public CBase
	{
	public:
	static CUriQueryFilter* NewL ( MDBTransaction* aDBTrans );
	virtual ~CUriQueryFilter ();
	
	virtual TBool MatchRecordL ();	
	MDBTransaction& DBTransaction() const;
	
	TInt MatchRecordCountL ();
	
	protected:
	CUriQueryFilter();
	void Initialize( MDBTransaction* aDBTrans );
	
	protected:
	MDBTransaction*  iDBTransaction;
	};

// -------------------

class CUriPathFilter : public CUriQueryFilter
	{
	public:
	static CUriPathFilter* NewL ( MDBTransaction* aDBTrans, InetUriList::TURIMatch aMatch, const TDesC8& aPartialPath );
	~CUriPathFilter ();
	
	private:
	CUriPathFilter ();
	void ConstructL ( MDBTransaction* aDBTrans, const TDesC8& aPartialPath, InetUriList::TURIMatch aMatch );

	TBool MatchRecordL ();

	private:
	HBufC8* 				iPartialPath;				
	};

// -----------------------

class CUriDomainFilter : public CUriQueryFilter
	{
	public:
		static CUriDomainFilter* NewL ( MDBTransaction* aDBTrans, const TDesC8& aDomainName );
		~CUriDomainFilter ();
	private:
		CUriDomainFilter ();
		void ConstructL ( MDBTransaction* aDBTrans, const TDesC8& aDomainName );
		TBool MatchRecordL ();
		
		TPtrC8 IsHDNStringWithNBForm ( const TDesC8& aDomainName );
		
	private:
		HBufC8* iDomainName;
		TPtrC8	iComparisonStr;					
	};

// -----------------------

class CTldPolicyDataFilter : public CUriQueryFilter
	{
	public:
	static CTldPolicyDataFilter* NewL ( MDBTransaction* aDBTrans );
	~CTldPolicyDataFilter ();
	
	private:
	CTldPolicyDataFilter ();
	void ConstructL ( MDBTransaction* aDBTrans );

	TBool MatchRecordL ();
			
	};

#endif // __URIQUERYFILTER_H__

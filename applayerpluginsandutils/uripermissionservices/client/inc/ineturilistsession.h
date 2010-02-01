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

#ifndef __INETURILISTSESSION_H__
#define __INETURILISTSESSION_H__


#include <ineturilistdef.h>
#include <tldlistdef.h>
#include <ineturi.h>
#include "urilistreadstream.h"

/**
The ineturilist client session class which gets instantiated once the application 
loads the ineturilist dll. This connects to ineturilist server and perform all IPC 
operations.

@internalComponent
*/
class RInetUriListSession : public RSessionBase
{

public:
	RInetUriListSession ();
	~RInetUriListSession ();

	TInt Connect ();
	void Close ();	
	
	void InitializeL ();
	void AddL ( CInetUriImpl& aInetUri );
	void RemoveL ( const CInetUriImpl& aInetUri );
	void UpdateL ( const CInetUriImpl& aInetUri );
	
	TInt Count ( InetUriList::TServiceType aServiceType, InetUriList::TListType aListType );
	TInt GetListTypeL ( const TDesC8& aUri, InetUriList::TServiceType aServiceType, InetUriList::TListType& aListType );

	CUriListReadStream* QueryUriL ( const TQueryArgs& aArgs );
	CUriListReadStream* QueryUriL ( const TDesC8& aUri, const TQueryArgs& aArgs );
	
	TInt QueryTldInfo ( const TPolicyQueryArgs& aArgs, TPtr8& aCharSet );
	TInt GetHostType ( const TPolicyQueryArgs& aArgs );

	TInt ReadQueryResults ( TInt aHandle, TPtr8& aPtr );
	TInt PolicyDataLength ( const TPolicyQueryArgs& aArgs );
	void CloseSrvStream ( TInt aHandle );
private:
	TInt StartServer ();
	CBufFlat* GetBufferLC ( TInt aSize );	
};

#endif // __INETURILISTSESSION_H__

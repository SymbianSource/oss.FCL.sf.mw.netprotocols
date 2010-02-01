// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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

// System includes
#include <e32base.h>
#include <httpstringconstants.h>
#include <http/rhttpsession.h>

// Local includes

// Class signature
#include "CHTTPManager.h"

//##ModelId=3A360E89014E
CHTTPManager* CHTTPManager::CreateOrFindL()
	{
	CHTTPManager* mgr = static_cast<CHTTPManager*>(Dll::Tls());
	if (mgr == NULL)
		{
		mgr = new (ELeave) CHTTPManager;
		CleanupStack::PushL(mgr);
		mgr->ConstructL();
		Dll::SetTls(mgr);
		CleanupStack::Pop(mgr);
		}
	mgr->iUseCount++;
	return mgr;
	}

void CHTTPManager::ConstructL()
	{
	iPool.OpenL(RHTTPSession::GetTable());
	}

void CHTTPManager::Release()
	{
	if ((iUseCount > 0) && (!--iUseCount))
		{
		delete this;
		Dll::SetTls(NULL);
		}
	}

CHTTPManager::~CHTTPManager()
	{
	iPool.Close();
	}


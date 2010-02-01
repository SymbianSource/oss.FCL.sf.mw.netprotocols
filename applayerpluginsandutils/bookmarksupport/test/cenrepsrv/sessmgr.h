// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef SESSMGR_H
#define SESSMGR_H

#include <e32base.h>

class CServerSession;

class CSessionManager : public CServer2
	{
public:
	static CSessionManager* NewLC();

private:
	CSessionManager();
	void ConstructL();

	CSession2* NewSessionL(const TVersion& aVersion, const RMessage2& aMessage) const;
	};

#endif // SESSMGR_H

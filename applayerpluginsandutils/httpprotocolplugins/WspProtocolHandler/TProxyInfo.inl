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

#include "tproxyinfo.h"

inline TBool TProxyInfo::operator!=(const TProxyInfo& aProxyInfo) const
	{
	return !(*this == aProxyInfo);
	}

inline void TProxyInfo::Close()
	{
	iProxyAddress.Close();
	iProxyAddress = RStringF();
	iServiceCentreNumber.Close();
	iServiceCentreNumber = RStringF();
	}

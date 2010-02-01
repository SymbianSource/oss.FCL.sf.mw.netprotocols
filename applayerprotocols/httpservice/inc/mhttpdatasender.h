// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __MHTTPDATASENDER_H__
#define __MHTTPDATASENDER_H__

class MHttpDataSender
	{
	public:	
	virtual void Notify(const TDesC8& aData, TBool aLast) =0;
	virtual void Error(TInt aError) =0;
	};

#endif // __MHTTPDATASENDER_H__

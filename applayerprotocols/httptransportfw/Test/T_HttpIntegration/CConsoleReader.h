// Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// $Header$
// 
//

#ifndef __CCONSOLEREADER_H__
#define __CCONSOLEREADER_H__

//-----------------------------------------------------------------------------

#include <e32base.h>
#include <e32cons.h>

//-----------------------------------------------------------------------------

#include "GT0149.h"

//-----------------------------------------------------------------------------

class CConsoleReader : public CActive
{
public:	// Methods

	static CConsoleReader* NewL(CConsoleBase& aConsole);
	~CConsoleReader();

	void ReadLine(TDes& aData, TRequestStatus& aCallbackStatus);
	void ReadChar(TDes& aChar, TRequestStatus& aCallbackStatus);
	void ReadCancel();

private:	// Methods from CActive

	virtual void RunL();
	virtual void DoCancel();

private:	// Methods

	CConsoleReader(CConsoleBase& aConsole);
	void ProcessKeyPress(TChar aChar);
	void IssueRequest();

private:	// Enums

	enum TReadMode
		{
		ELineMode		= 0,
		ECharMode
		};

private:	// Attributes

	CConsoleBase		&iConsole;
	TDes						*iData;
	TRequestStatus	*iCallbackStatus;
	TReadMode				iMode;

};

#endif	// __CCONSOLEREADER_H__

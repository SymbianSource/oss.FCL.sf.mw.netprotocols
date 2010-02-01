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

#include "t_detectkey.h"
#include <e32cons.h>

CDetectKeyPress* CDetectKeyPress::NewLC(CConsoleBase& aConsole, MTestInteractionObserver& aObserver)
	{
	CDetectKeyPress* self = new (ELeave) CDetectKeyPress(aConsole,aObserver);
	CleanupStack::PushL(self);
	self->RequestKey();
	return self;
	}


CDetectKeyPress::CDetectKeyPress(CConsoleBase& aConsole, MTestInteractionObserver& aObserver):
	CActive(CActive::EPriorityUserInput), iConsole(aConsole), iObserver(aObserver)
	{
	CActiveScheduler::Add(this);
	}


CDetectKeyPress::~CDetectKeyPress()
	{
	Cancel();
	}

void CDetectKeyPress::DoCancel()
	{
	iConsole.ReadCancel();
	}

void  CDetectKeyPress::RunL()
	{
	TChar key = iConsole.KeyCode();
	TUint modifiers = iConsole.KeyModifiers();

	if (modifiers & EModifierCtrl)
		{
		key += 64;
		if (key == 'n' || key == 'N')
			iObserver.TestInteractionDetectedL(MTestInteractionObserver::EStopCurrentTest);
		}

	
	iConsole.Read(iStatus);
	SetActive();
	}

void CDetectKeyPress::RequestKey()
	{
	iConsole.Read(iStatus);
	SetActive();
	} 


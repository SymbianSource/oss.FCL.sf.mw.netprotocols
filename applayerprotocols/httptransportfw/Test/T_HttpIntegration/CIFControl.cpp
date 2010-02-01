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
// This module implements the collection of stadard command classes for the
// tester framework.
// rev:	mjdavey, symbian@mjdss.com, July 2002
// for:	Typhoon (7.0s) & JetStream (8.0)
// Include Files  
// 
//

#include <e32std.h>                     //

//-----------------------------------------------------------------------------
//  Include Definition Files  

#include "CIFControl.h"

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//	Command:	LOGFILE command.
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

CIFControl *CIFControl::NewL()
{
CIFControl *self = NewLC();
CleanupStack::Pop();
return self; 
}

//-----------------------------------------------------------------------------

CIFControl *CIFControl::NewLC()
{
CIFControl *self = new (ELeave) CIFControl();
CleanupStack::PushL(self);
self->ConstructL(ENotIf, EFalse, EIgnoreIF);
return self;
}

//-----------------------------------------------------------------------------

CIFControl *CIFControl::NewL (const CIFControl::TIFMode &aMode,
														  const TBool &aStatus, 
													    const CIFControl::TIFProcessing &aProcess)
{
CIFControl *self = NewLC(aMode, aStatus, aProcess);
CleanupStack::Pop();
return self; 
}

//-----------------------------------------------------------------------------

CIFControl *CIFControl::NewLC(const CIFControl::TIFMode &aMode,
														  const TBool &aStatus, 
													    const CIFControl::TIFProcessing &aProcess)
{
CIFControl *self = new (ELeave) CIFControl();
CleanupStack::PushL(self);
self->ConstructL(aMode, aStatus, aProcess);
return self;
}

//-----------------------------------------------------------------------------

void CIFControl::ConstructL(const CIFControl::TIFMode &aMode,
														const TBool &aStatus, 
													  const CIFControl::TIFProcessing &aProcess)
{
SetIFState(aProcess);
SetIFMode(aMode);
iIfResult = aStatus;

//iIfMode = aMode;
//iIfStatus = aProcess;

iInIF = EFalse;
iIfs = 0;
iElses = 0;
}

//-----------------------------------------------------------------------------

CIFControl::~CIFControl() 
{
}

//-----------------------------------------------------------------------------

CIFControl::TIFMode CIFControl::GetIFMode() const 
{
return iIfMode; 
}

//-----------------------------------------------------------------------------

void CIFControl::SetIFMode(const TIFMode &eMode)//, const TBool &aResult) 
{
iIfMode = eMode; 
//iIfResult = aResult;
}

//-----------------------------------------------------------------------------

CIFControl::TIFProcessing CIFControl::GetIFState() const 
{
return iIfStatus; 
}

//-----------------------------------------------------------------------------

void CIFControl::SetIFState(const TIFProcessing &eProc) 
{
iIfStatus = eProc; 
}

//-----------------------------------------------------------------------------

TBool CIFControl::GetIFResult() 
{
return iIfResult; 
}

//-----------------------------------------------------------------------------

TInt CIFControl::GetIfCount() 
{
return iIfs; 
}

//-----------------------------------------------------------------------------

TInt CIFControl::GetElseCount() 
{
return iElses; 
}

//-----------------------------------------------------------------------------

void CIFControl::If(void) 
{
++iIfs; 
}

//-----------------------------------------------------------------------------

void CIFControl::Else(void) 
{
++iElses; 
}

//-----------------------------------------------------------------------------

void CIFControl::EndIf(void)
{
--iIfs; 
--iElses; 
}

//-----------------------------------------------------------------------------
// End of File
//-----------------------------------------------------------------------------

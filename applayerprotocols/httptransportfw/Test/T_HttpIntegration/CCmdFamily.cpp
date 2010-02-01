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
// GT0149 Applications Protocol Integration Test Harness
// blank application
// This module implements the CCmdFamily class: a generic collection of
// registered CCmdBase class derived commands.
// 
//

#include "CCmdBase.h"            //

//	bizarre configuration means that this is the only way this will compile...
//#include "CCmdFamily.h"            //

//-----------------------------------------------------------------------------

CCmdFamily* CCmdFamily::NewL ()
{
CCmdFamily* self = NewLC();
CleanupStack::Pop();
return self; 
}

//-----------------------------------------------------------------------------

CCmdFamily* CCmdFamily::NewLC()
{
CCmdFamily* self = new (ELeave) CCmdFamily();
CleanupStack::PushL(self);
self->ConstructL();
return self;
}

//-----------------------------------------------------------------------------

void CCmdFamily::ConstructL()
{

iMachine  = NULL;
iStepOver = EFalse;

// Perhaps the granularity should be approximated in advance?
TInt granularity = 32; // this one here is just a wild guess
iCommands = new (ELeave) CArrayPtrSeg<CCmdBase>(granularity);

//
// Assuming a TBool array to be initially full of EFalse's?
//

}

//-----------------------------------------------------------------------------

CCmdFamily::~CCmdFamily()
{
iCommands->ResetAndDestroy();
delete iCommands;
}

//-----------------------------------------------------------------------------

void CCmdFamily::SetMachine(CTEngine *aTestMachine)
{
iMachine = aTestMachine;
}

//-----------------------------------------------------------------------------

CTEngine* CCmdFamily::Machine()
{
return iMachine;
}

//-----------------------------------------------------------------------------
//	use this to append a new command to the list

void CCmdFamily::RegisterL(CCmdBase* aCommand) 
{
if (aCommand != 0)
	{
	iCommands->AppendL(aCommand);
	aCommand->SetFamily(this);
	}
}

//-----------------------------------------------------------------------------
//	returns how many commands currently available

TInt CCmdFamily::Count() const
{
return iCommands->Count();
}

//-----------------------------------------------------------------------------
//	returns a pointer to aIndex'th command

CCmdBase* CCmdFamily::At(TInt aIndex) const
{
return iCommands->At(aIndex);
}

//-----------------------------------------------------------------------------
//	operator to get at particular command

CCmdBase* CCmdFamily::operator[](TInt aIndex) const
{
return iCommands->At(aIndex);
}

//-----------------------------------------------------------------------------
//	get command with this ID

CCmdBase* CCmdFamily::Command(TInt aCommandId)
{
for (TInt i = 0; i < Count(); ++i)
	{
	CCmdBase* command = At(i);
	if (command->CommandId() == aCommandId) 
		return command;
	}

// Not found
return NULL;
}

//-----------------------------------------------------------------------------
//	Get Command by command string i.e. recognize the command.

CCmdBase *CCmdFamily::Recognize(const TDesC& aCommand)
{
for (TInt i = 0; i < Count(); ++i)
	{
	CCmdBase *command = At(i);
	if (command->Recognize(aCommand)) 
		return command;
	}

// Not found
return NULL;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//
//	Command Family Methods
//
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//	List all the commands out one by one!
//	added mjd, Aug.2002

void CCmdFamily::ListAll(CConsoleBase *aConsole)
{
_LIT(KHdr, "\r\n    Command            Help Information");
_LIT(KMsg, "\r\n%3d %-16S - %-52S");

if (aConsole != 0)
	aConsole->Printf(KHdr);

TInt i = 0;
TInt j = 0;
while (j < Count())
	{
	if (aConsole)
		{
		CCmdBase* iCmd = At(j);
		aConsole->Printf(KMsg, j, &iCmd->Keyphrase(), &iCmd->HelpText());
		if (i > 12)
			{
			aConsole->Printf(_L("\r\n[Press any key]"));
			aConsole->Getch();
			aConsole->Printf(KHdr);
			i = 0;
			}
		}
	++i;
	++j;
	}

//	one final CRLF to ensure we display the prompt on a new line...
aConsole->Printf(_L("\n"));
}

//-----------------------------------------------------------------------------
//	Set the switch (default aBoolean to TRUE)

void CCmdFamily::SetSwitch(const TSwitch &aSwitch, TBool aBoolean)
{
iSwitch[aSwitch] = aBoolean;
}

//-----------------------------------------------------------------------------
//	Get the switch

TBool CCmdFamily::Switch(const TSwitch &aSwitch)
{
return iSwitch[aSwitch];
}

//-----------------------------------------------------------------------------
//	Set the StepOver flag.

void CCmdFamily::SetStepOver(const TBool &aBoolean)
{
iStepOver = aBoolean;
}

//-----------------------------------------------------------------------------
//	Get the StepOver flag.

TBool CCmdFamily::StepOver()
{
return iStepOver;
}

//-----------------------------------------------------------------------------
//  End of File  
//-----------------------------------------------------------------------------

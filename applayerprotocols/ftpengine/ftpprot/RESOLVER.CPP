// Copyright (c) 1998-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Active object to perform DNS lookup
// Author:	Philippe Gabriel
// Performs DNS lookup for the FTP client
// 
//

/**
 @file RESOLVER.CPP
 @internalComponent
*/

#include "FTPDEF.H"
#include "DEBUG.H"
#include "RESOLVER.H"
#include "FTPPROT.H"
#include <e32base.h>
#include <in_sock.h>

CFTPResolver::CFTPResolver(MFTPResolverNotifier* aNotifier
						   ):CActive(CActive::EPriorityStandard)
	{
	FTPPROTDEBUG(_DBGResolver,_L("CFTPResolver::CFTPResolver called\n"));
	iNotifier = aNotifier;
	}

CFTPResolver* CFTPResolver::NewL(MFTPResolverNotifier* aNotifier,RSocketServ& aSockServ)
	{
	CFTPResolver* self = new (ELeave) CFTPResolver(aNotifier);
    CleanupStack::PushL(self);
    self->ConstructL(aSockServ);
    CleanupStack::Pop();
	return self;
	} 

void CFTPResolver::ConstructL(RSocketServ& aSockServ)
	{
	FTPPROTDEBUG(_DBGResolver,_L("CFTPResolver::ConstructL called\n"));
	User::LeaveIfError(iResolver.Open(aSockServ, KAfInet, KProtocolInetTcp));
	CActiveScheduler::Add(this);
	} 

void CFTPResolver::DoCancel()
	{
	iResolver.Cancel();
	}

void CFTPResolver::RunL()
	{
	FTPPROTDEBUG(_DBGResolver,_L("CFTPResolver::RunL called "));
	switch (iStatus.Int())
		{
		case KErrNone:
			FTPPROTDEBUG(_DBGResolver,_L("iStatus: NoError\n"));
			// Call Upper layer notifier
			iNotifier->FTPResolverNotifier(MFTPResolverNotifier::EDtpLookupComplete);
			break;
		default:
			FTPPROTDEBUG(_DBGResolver,_L("iStatus: Error\n"));
			iNotifier->FTPResolverNotifier(MFTPResolverNotifier::EDtpLookupFailed);
			break;
		}
	} 
CFTPResolver::~CFTPResolver()
	{
	FTPPROTDEBUG(_DBGResolver,_L("CFTPResolver::~CFTPResolver called\n"));
	iResolver.Close();
	}

void CFTPResolver::Lookup(const THostName& aServerName)
{
	FTPPROTDEBUG(_DBGResolver,_L("CFTPResolver::Lookup resolving:>"));
	FTPPROTDEBUG(_DBGResolver,aServerName);
	FTPPROTDEBUG(_DBGResolver,_L("<\n"));
	iResolver.GetByName(aServerName, iNameEntry, iStatus);
	SetActive();
}

void CFTPResolver::SetAddress(TInetAddr& aAddress)
{
	// Get the 1st entry resolved
	iNameRecord = iNameEntry();
	// Save the address (take care not to overwrite the port, might have been already set)
	if (TInetAddr::Cast(iNameRecord.iAddr).Family() == KAfInet)
		aAddress.SetAddress(TInetAddr::Cast(iNameRecord.iAddr).Address());
	else
		aAddress.SetAddress(TInetAddr::Cast(iNameRecord.iAddr).Ip6Address());

#if defined(__FTPPROTDEBUG__)
	TBuf<512> debugBuffer; // A Buffer to output the resolved IP 
	
	aAddress.Output(debugBuffer);
	debugBuffer.Append(_L(","));
	debugBuffer.AppendNum(aAddress.Port(), EDecimal); 
	debugBuffer.Append(_L("\n"));
#endif
	FTPPROTDEBUG(_DBGResolver,_L("Resolved address is:"));
	FTPPROTDEBUG(_DBGResolver,debugBuffer);
}

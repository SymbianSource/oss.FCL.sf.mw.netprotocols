/**
* Copyright (c) 1998-2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:
* Active object to perform DNS lookup
* Author:	Philippe Gabriel
* Performs DNS lookup for the FTP client
* 
*
*/



/**
 @file RESOLVER.H
 @internalComponent
*/

#if !defined(__RESOLVER_H__)
#define __RESOLVER_H__
#include "DEBUG.H"
#include "FTPDEF.H"
#include <e32base.h>
#include <in_sock.h>

//////////////////////////////////////////////////////////////
// Definitions
//////////////////////////////////////////////////////////////

class CFtpProtocol;
class MFTPResolverNotifier
/**
@internalComponent
*/
{
public:
/** 
Operation completion return codes.
*/ 
	enum TFTPResolverNotificationCode 
		{
		EDtpLookupComplete=0, 
		EDtpLookupFailed
		};
public:
		/** Notify of normal completion of an operation */
		virtual void FTPResolverNotifier(const TFTPResolverNotificationCode)=0;
};
NONSHARABLE_CLASS(CFTPResolver) : public CActive
/**
@internalComponent
*/
{
public:
	CFTPResolver(MFTPResolverNotifier*);
	~CFTPResolver();
	static CFTPResolver* NewL(MFTPResolverNotifier*,RSocketServ&);
	void ConstructL(RSocketServ& aSockServ);
	void Lookup(const THostName& aServerName);
	void SetAddress(TInetAddr& aAddress);
protected:
	void RunL();
    void DoCancel(void);
private:
	/** Used for resoving Ip Address from a name and vice versa */
	RHostResolver iResolver;  
	/** PlaceHolder for the resolved address */
	TInetAddr* iAddress; 
	TNameEntry iNameEntry;
	TNameRecord iNameRecord;
	MFTPResolverNotifier* iNotifier;
	TBuf8<40> iServerName;
};
#endif // __RESOLVER_H__

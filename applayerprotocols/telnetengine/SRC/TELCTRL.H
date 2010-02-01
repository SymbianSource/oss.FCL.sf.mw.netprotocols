/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies).
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
*
*/


#ifndef _TELENET_ENGINE_H_
#define _TELENET_ENGINE_H_

#include <telsess.h>
#include "TELFSM.H"

class CTelnetResolver;
class CAtiveWriter;
class CActiveReader;
class CIOControl;
class CReadTrigger;
class MIONotifier;
class CProto;

const int KPortReadBufferSize = 128;

const int KClientReadBufferSize = 512;

NONSHARABLE_CLASS(CTelnetControl) : public CBase , public MTelnetResolver, public MIONotifier , public MProtoEvent
/**
@internalComponent
*/
	{
public:

	CTelnetControl();
	~CTelnetControl();

	static CTelnetControl* NewL(const TTelnetConfig& aConfig,MTelnetNotification* aTelnetNotification);
	void ConstructL(const TTelnetConfig& aConfig,MTelnetNotification* aTelnetNotification);
	void Reset();

	TInt Connect(const TDesC& aServerName, TUint aPort);
	TInt Connect(const TInetAddr& aInetAddr, TUint aPort);

	TInt SetOption(const TInt aOption);
	TInt SetConfig(const TTelnetConfig& aConfig);
	TInt OptionStatus(TOptionStatus& aStatus);

	TInt Disconnect();

	TInt Read();
	TInt Write(const TDesC8& aBuffer);
	TInt Write(TTelnetUserControl& aControlCode);


	// MTelnetResolver
	void ResolverConnectedL();
	void ResolverDisconnected();
	void ResolverError(TInt aError);

	// MIONotification
  	void WriteComplete(); 
	void WriteError(TInt aError);
	void ReadCompleteL();
	void ReadComplete(TInt aError);
	void Event(TInt aEvent, TInt aEventCode);

	// MProtoEvent
	void ProtoError(TInt aError);
	void ProtoEvent();


private:
	// For passing data to the client app
	TBuf8<KClientReadBufferSize>	iClientReadBuffer;
	// Passed to IO control for receiving data from the server
	TBuf8<KPortReadBufferSize>		iPortReadBuffer;

	TBool					iClientWriteOutstanding;

    MTelnetNotification*	iTelnetNotification;

	CTelnetResolver*		iTelnetResolver;

	CActiveWriter*			iPortWriter;
	CActiveReader*			iPortReader;
	CIOBufferControl*		iPortIOControl;

	CProto*					iProto;

	};

#endif

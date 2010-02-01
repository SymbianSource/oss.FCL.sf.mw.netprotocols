/**
* Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Buffer class declarations
* Contains I/O Buffer class declaration
* 
*
*/



/**
 @file Iobuffer.h
 @internalComponent 
*/

#ifndef __IO_BUFFERS_H
#define __IO_BUFFERS_H

#include <telsess.h>

class MIONotifier
/**
@internalComponent
*/
	{
public:
	
	virtual void WriteComplete() = 0;
	virtual void WriteError(TInt aError) = 0;
	virtual void ReadComplete(TInt aError) = 0;
	virtual void ReadCompleteL() = 0;
	virtual void Event(TInt aEvent,TInt aEventCode) = 0;
	};

class CActiveWriter;
class CActiveReader;

NONSHARABLE_CLASS(CIOBufferControl) : public CBase , public MIONotifier
/**
@internalComponent
*/
	{ 
public:
	CIOBufferControl();
	~CIOBufferControl();

	static CIOBufferControl* NewL(MIONotifier* aNotifier);
	void ConstructL(MIONotifier* aNotifier);
	TInt WriteUrgent(HBufC8* aBuffer);
	
	TInt Write(HBufC8* aBuffer);
	TInt Read(TDes8& aBuffer);
	void SetWriter(CActiveWriter* aWriter);
	void SetReader(CActiveReader* aReader);

	void Reset();

	// MIONotifier
	void WriteComplete();
	void WriteError(TInt aError);
	void ReadCompleteL();
	void ReadComplete(TInt aError);
	void Event(TInt aEvent,TInt aEventCode);

public:

private:
	CActiveWriter*								iWriter;
	CActiveReader*								iReader;
	MIONotifier*								iNotifier;

	HBufC8*										iWriteBuffers[2];
	TInt iLiveIndex;
	TInt iQueueIndex;
	HBufC8*										iUrgent;

	};

#endif

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
* Description:  ?Description
*
*/


#ifndef __GZIPSTREAMBUFMGR_H_
#define __GZIPSTREAMBUFMGR_H_
//------------------------------------------------------------------------------

#include <e32base.h>
#include <ezbufman.h>
//------------------------------------------------------------------------------

class CGzipStream;
//------------------------------------------------------------------------------

class StreamBufMgr : public MEZBufferManager
{
public:
	// ctor
	static StreamBufMgr* NewL( TPtrC8 in );
	virtual ~StreamBufMgr();
	//--------------------------------------------------------------------------

	void InitializeL(CEZZStream &aZStream);
	void NeedInputL(CEZZStream &aZStream);
	void NeedOutputL(CEZZStream &aZStream);
	void FinalizeL(CEZZStream &aZStream);
	void SetBuffer( TPtrC8 in );
	//--------------------------------------------------------------------------

private:
	void ConstructL();
	StreamBufMgr( TPtrC8 in );
	//--------------------------------------------------------------------------

	TPtrC8	iInput;  // always points to start of the input Buffer
	TPtr8	iOutputDes;	// always points to start of the output Buffer
	HBufC8*	iOutput;

	friend class CDecmpStream;

	TBool	iNeedInput;
	TBool	iNeedOutput;
	TBool	iFinalized;
};

//------------------------------------------------------------------------------
#endif // !__GZIPSTREAMBUFMGR_H_

/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
#include <EZDecompressor.h>
#include "DecmpStreamBase.h"
#include "StreamBufMgr.h"

CDecmpStreamBase::~CDecmpStreamBase()
{
	// no c++ trick play here, because of Symbian's NewL mechanism
}

void CDecmpStreamBase::ResetBuffer( TPtrC8 in )
{
	__ASSERT_DEBUG( iBufMgr == 0, 1 );
	iBufMgr->SetBuffer( in );
	iBufMgr->InitializeL( *iDecompressor );
}

TBool CDecmpStreamBase::InflateL()
{
	// inflating the content
	__ASSERT_DEBUG( iDecompressor == 0, 1 );
	return iDecompressor->InflateL();
}

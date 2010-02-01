// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Dummy implementation of CActiveBackupClient class.
// 
//

/**
 @file
 @released
*/

#include "abclient.h"

namespace conn
	{

	CActiveBackupClient::CActiveBackupClient() : iClientSession(NULL), iABCallbackHandler(NULL)
		{
		}


	EXPORT_C CActiveBackupClient* CActiveBackupClient::NewL()
		{
		CActiveBackupClient* self = new (ELeave) CActiveBackupClient();
		CleanupStack::PushL(self);
		self->ConstructL();
		CleanupStack::Pop(self);
		return self;
		}

	EXPORT_C CActiveBackupClient* CActiveBackupClient::NewL(MActiveBackupDataClient* aClient)
		{
		CActiveBackupClient* self = new (ELeave) CActiveBackupClient();
		CleanupStack::PushL(self);
		self->ConstructL(aClient);
		CleanupStack::Pop(self);
		return self;
		}

	void CActiveBackupClient::ConstructL()
		{
		}

	void CActiveBackupClient::ConstructL(MActiveBackupDataClient* /*aClient*/)
		{
		ConstructL();
		}


	EXPORT_C CActiveBackupClient::~CActiveBackupClient()
		{
		}

    EXPORT_C void CActiveBackupClient::BURModeInfoL(TDriveList& /*aDriveList*/, TBURPartType& /*aBackupType*/, TBackupIncType& /*aIncBackupType*/)
		{
		}


    EXPORT_C TBool CActiveBackupClient::DoesPartialBURAffectMeL()
		{
		return ETrue;
		}


    EXPORT_C void CActiveBackupClient::ConfirmReadyForBURL(TInt /*aErrorCode*/)
		{
		}

	} // end of conn namespace

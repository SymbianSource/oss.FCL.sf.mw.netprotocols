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
//


#include "cuntrustedcertdialog.h"

_LIT(KYesText, "Yes");
_LIT(KNoText, "No");
_LIT(KTitleText, "WTLS Dialog");
_LIT(KPromptText, "The remote server has sent an untrusted certificate\r\nDo you wish to proceed?");


/** CDialogBase
	A base class for dialogs using the RNotifier scheme
*/

CDialogBase::CDialogBase(MDialogCallback& aCallback): 
	CActive(CActive::EPriorityStandard), iCallback(aCallback)
	{
	CActiveScheduler::Add(this);
	}	

void CDialogBase::ConstructL()
	{
	User::LeaveIfError(iNotifier.Connect());
	}

CDialogBase::~CDialogBase()
	{
	iNotifier.Close();
	}

//
// Display Secure Connection Dialog
//
CUntrustedCertDialog* CUntrustedCertDialog::NewL(MDialogCallback& aCallback)
	{
	CUntrustedCertDialog* self =new(ELeave) CUntrustedCertDialog(aCallback);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CUntrustedCertDialog::CUntrustedCertDialog(MDialogCallback& aCallback)
	: CDialogBase(aCallback)
	{
	}

CUntrustedCertDialog::~CUntrustedCertDialog()
	{
	}


void CUntrustedCertDialog::DisplayUntrustedCertDialog()
	{
	iTitleBuffer.Copy(KTitleText);
	iPromptBuffer.Copy(KPromptText);
	iYesDes.Copy(KYesText);
	iNoDes.Copy(KNoText);
	iNotifier.Notify(iTitleBuffer,iPromptBuffer,iNoDes,iYesDes,iDoTrustCert,iStatus);
	}

void CUntrustedCertDialog::RunL()
	{
	iCallback.DialogDismissed(iDoTrustCert);
	}

TInt CUntrustedCertDialog::RunError(TInt /*aError*/)
	{
	return KErrNone;
	}

void CUntrustedCertDialog::DoCancel()
	{
	}


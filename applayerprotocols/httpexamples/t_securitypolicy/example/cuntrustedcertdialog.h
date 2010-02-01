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


#ifndef		__CUNTRUSTEDCERTDIALOG_H__
#define		__CUNTRUSTEDCERTDIALOG_H__

#include <e32base.h>

const TInt KGenericBufferSize=80;
const TInt KDialogBufferSize=80;

class MDialogCallback
	{
public:
	virtual void DialogDismissed(TBool aChoice) =0;
	};

class CDialogBase : public CActive
	{
public:
	virtual ~CDialogBase();
protected:
	CDialogBase(MDialogCallback& aCallback);
	void ConstructL();
protected:
	TBuf<4> iYesDes;
	TBuf<4> iNoDes;
	TBuf<KGenericBufferSize> iTitleBuffer;
	TBuf<KDialogBufferSize> iPromptBuffer;
	MDialogCallback& iCallback;
	RNotifier iNotifier;
	};

class CUntrustedCertDialog : public CDialogBase
	{
public:
	static CUntrustedCertDialog* NewL(MDialogCallback& aCallback);
	virtual ~CUntrustedCertDialog();
	void DisplayUntrustedCertDialog();
private:
	CUntrustedCertDialog(MDialogCallback& aCallback);

private: // from CActive
	virtual void RunL();
	virtual TInt RunError(TInt aError);
	virtual void DoCancel();
private:
	TBool iDoTrustCert;
	};

#endif //	__CUNTRUSTEDCERTDIALOG_H__
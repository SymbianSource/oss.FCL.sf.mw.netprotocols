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
//


#ifndef __TUIEDIT_H
#define __TUIEDIT_H

#include <e32cons.h>

class CLineEdit : public CBase
/**
CLineEdit class - borrowed from F32 text window server
@internalComponent
*/
    {
public:
	enum TCursorType {ECursorNone=0,ECursorNormal=20,ECursorInsert=100};
	enum TEditMode {EEditOverWrite,EEditInsert};
public:
	IMPORT_C static CLineEdit* NewL(CConsoleBase* aConsole,TInt aMaxHistory);
	~CLineEdit();
	IMPORT_C void Edit(const TDesC& aPrompt,TDes* aBuf);
protected:
	CLineEdit();
	TPoint Where();
	TInt Lines();
	TInt WordLeft();
	TInt WordRight();
	void ClearLine();
	void ClearLast(TInt aCnt);
	void Recall();
	void Cursor();
	void Refresh();
	inline TDes& Buf() {return *iBuf;}
private:
	CArrayFixFlat<HBufC*>* iHistory;
	CConsoleBase* iConsole; // Not owned
	TInt iMaxHistory;
	TInt iWidth;
	TInt iHeight;
	TInt iPos;
	TInt iLine;
	TInt iOrigin;
	TInt iRecall;
	TEditMode iMode;
	TDes* iBuf;
	};

#endif

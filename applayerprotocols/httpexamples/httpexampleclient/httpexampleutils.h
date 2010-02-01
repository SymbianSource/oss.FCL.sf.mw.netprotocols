// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <e32test.h>

const TInt KMaxUserEntrySize = 128;


/** CHttpExampleUtils is a class that provides some user input utilities and holds a RTest used throughout
*/
class CHttpExampleUtils : public CBase
	{
public:
	static void InitCommsL();
	static CHttpExampleUtils* NewL(const TDesC& aTestName);
	~CHttpExampleUtils();
	void GetAnEntry(const TDesC& ourPrompt, TDes& currentstring);
	TInt GetSelection(const TDesC& ourPrompt, const TDesC& validChoices);
	void PressAnyKey();
	void LogIt(TRefByValue<const TDesC> aFmt, ...);	
	RTest& Test();
private:
	CHttpExampleUtils(const TDesC& aTestName);

private:
	RTest iTest;
	}; 

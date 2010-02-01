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



#include <e32base.h>

//##ModelId=3C4C4C3E0193
class MTestInteractionObserver
	{
public:
	
	enum TTestInteraction
		{
		EStopCurrentTest
		};
	
	//##ModelId=3C4C4C3E019D
	virtual void TestInteractionDetectedL(TTestInteraction aTestInteraction)= 0;
	};


//##ModelId=3C4C4C3E0073
class CDetectKeyPress : public CActive
	{
public:
	//##ModelId=3C4C4C3E00FF
	static CDetectKeyPress* NewLC(CConsoleBase& aConsole, MTestInteractionObserver& aObserver);
	//##ModelId=3C4C4C3E00FE
	~CDetectKeyPress();
	//##ModelId=3C4C4C3E00FD
	void RequestKey();
private:
	//##ModelId=3C4C4C3E00F6
	void DoCancel();
	//##ModelId=3C4C4C3E00F5
	void RunL();
	//##ModelId=3C4C4C3E00EC
	CDetectKeyPress(CConsoleBase& aConsole, MTestInteractionObserver& aObserver);
private:
	//##ModelId=3C4C4C3E00E9
	CConsoleBase& iConsole;
	//##ModelId=3C4C4C3E00D7
	MTestInteractionObserver& iObserver;
	};

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
// $Header$
// This header file provides the CIFControl methods and members
// 
//

#ifndef __CIFCONTROL_H__
#define __CIFCONTROL_H__

//-----------------------------------------------------------------------------
//  INCLUDES

#include <e32base.h>

//-----------------------------------------------------------------------------

class CIFControl : public CBase
{
public:

	//	helper defs and methods for IfMode (IF, ELSE, ENDIF) processing
	
	//	IFMode indicates the RESULT of the current IF clause
	enum TIFMode
		{
		ENotIf,
		EIFTrue,
		EELSETrue,
		EENDIF,
		EMAXIFMode
		};

	//	IFProcessing indicates the RESULT of the current IF clause
	enum TIFProcessing
		{
		EIgnoreIF,
		EInIF,
		EInELSE,
		EMAXIFProcesses
		};

	// aMaxLength sets the maximum length of formatted output.
	static CIFControl* NewL ();
	static CIFControl* NewLC();
	static CIFControl* NewL (const CIFControl::TIFMode &,
													 const TBool &, 
													 const CIFControl::TIFProcessing &);
	static CIFControl* NewLC(const CIFControl::TIFMode &,
													 const TBool &, 
													 const CIFControl::TIFProcessing &);
	~CIFControl();

private:    // Constructors and destructors

	void ConstructL(const CIFControl::TIFMode &,
									const TBool &, 
									const CIFControl::TIFProcessing &);
	CIFControl() {};
	
public:     // New functions

	TIFMode GetIFMode() const;
	void SetIFMode(const TIFMode &); 
	TIFProcessing GetIFState() const;
	void SetIFState(const TIFProcessing &);

	TBool GetIFResult();
	TInt GetIfCount();
	TInt GetElseCount();

	void If(void);
	void Else(void);
	void EndIf(void);

private:    // Data

	TIFMode iIfMode;						//	if nonzero then we're in an IF/ELSE/ENDIF (1=IF, 2=ELSE) Endif resets to 0
	TBool iIfResult;						//	this holds the result of the if command that's being executed
	TBool iInIF;								//	indicate where we are when processing
	TIFProcessing iIfStatus;

	//	in clause management
	TInt iIfs;											//	counter IF nesting process...
	TInt iElses;										//	while processing an IF/ELSE

};

//-----------------------------------------------------------------------------

#endif //__CIFCONTROL_H__            

//-----------------------------------------------------------------------------
// End of File
//-----------------------------------------------------------------------------

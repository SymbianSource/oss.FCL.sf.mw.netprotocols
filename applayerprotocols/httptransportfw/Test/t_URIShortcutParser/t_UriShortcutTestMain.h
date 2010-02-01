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
// This is the corresponding header file for main.cpp. It has the macro MANUAL declared.
// For running automated tests, simply comment the macro while for running the Manual Tests
// leave the macro as it is.
// 
//

//#define MANUAL



#ifdef MANUAL

void GetAnEntry(const TDesC& ourPrompt, TDes& currentstring);
void Go();
void DoTestsL(); 

#else 

void DoTestsL();

typedef void ( *TTestFunctionL ) ();

void DoOOMTest( TTestFunctionL aDoTestFunctionL );
void TestOOMLocalhostL();

void TestLocalhostL();

void TestOOMOneComponentL();

void TestOneComponentL();

void TestTwoComponentsL();

void TestOOMTwoComponentsL();

void TestThreeComponentsL();

void TestOOMThreeComponentsL();

void TestIPV4HostL();

void TestOOMIPV4HostL();

void TestIPV6HostL();

void TestOOMIPV6HostL();

void TestInvalidInputsL();

void TestLengthyInputsL();

void TestOOMLengthyInputsL();

void TestVariousSchemesL();

void TestOOMVariousSchemesL();

void TestHierarchicalComponentsL();

void TestOOMHierarchicalComponentsL();

#endif


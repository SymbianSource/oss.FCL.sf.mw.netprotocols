// Copyright (c) 2001-2010 Nokia Corporation and/or its subsidiary(-ies).
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


// for StartC32()
#include <c32comm.h>

#include "httpexampleutils.h"


// PDD names for the physical device drivers that are loaded in wins or arm
#if defined (__WINS__)
#define PDD_NAME		_L("ECDRV")
#else
#define PDD_NAME		_L("EUART1")
#define PDD2_NAME		_L("EUART2")
#define PDD3_NAME		_L("EUART3")
#define PDD4_NAME		_L("EUART4")
#endif

#define LDD_NAME		_L("ECOMM")


void CHttpExampleUtils::InitCommsL()
	{
#ifndef SIROCCO_CODE_MIGRATION
	TInt ret = User::LoadPhysicalDevice(PDD_NAME);
	User::LeaveIfError(ret == KErrAlreadyExists?KErrNone:ret);

#ifndef __WINS__
	ret = User::LoadPhysicalDevice(PDD2_NAME);
	ret = User::LoadPhysicalDevice(PDD3_NAME);
	ret = User::LoadPhysicalDevice(PDD4_NAME);
#endif

	ret = User::LoadLogicalDevice(LDD_NAME);
	User::LeaveIfError(ret == KErrAlreadyExists?KErrNone:ret);
	ret = StartC32();
	User::LeaveIfError(ret == KErrAlreadyExists?KErrNone:ret);
#endif //SIROCCO_CODE_MIGRATION
	}


CHttpExampleUtils* CHttpExampleUtils::NewL(const TDesC& aTestName)
	{
	CHttpExampleUtils* me = new (ELeave) CHttpExampleUtils(aTestName);
	return me;
	}


CHttpExampleUtils::CHttpExampleUtils(const TDesC& aTestName) : iTest(aTestName)
	{
	iTest.Start(KNullDesC);
	}

CHttpExampleUtils::~CHttpExampleUtils()
	{
	iTest.End();
	iTest.Close();
	}

RTest& CHttpExampleUtils::Test()
	{
	return iTest;
	}

void CHttpExampleUtils::PressAnyKey()
	{
	iTest.Printf(TRefByValue<const TDesC>_L("\nPress a key"));	
	iTest.Getch();
	}


TInt CHttpExampleUtils::GetSelection(const TDesC& aPrompt, const TDesC& aValidChoices)
	//
	//	Present the user with a list of options, and get their selection
	{
	TKeyCode key = EKeyNull;
	iTest.Console()->SetPos (0, iTest.Console()->WhereY ());
	iTest.Console()->Printf(_L("%S "), &aPrompt);
	iTest.Console()->Printf(_L("[%S] :"), &aValidChoices);
	TInt retVal = KErrNotFound;
	while (retVal == KErrNotFound)
		{
		key = iTest.Getch();
		// Check that key is in the list of valid choices
		retVal = aValidChoices.Locate((TChar)key);
		}
	iTest.Console()->Printf(_L("%c\n\n"), key);
	return retVal;
	}


void CHttpExampleUtils::LogIt(TRefByValue<const TDesC> aFmt, ...)
	{
	VA_LIST list;
	VA_START(list,aFmt);
	TBuf<KMaxFileName + 4> buf; // 4 for the log prompt
	buf.Zero();
	buf.Append(_L(">  "));
	buf.AppendFormatList(aFmt,list);
	VA_END(list);
	iTest.Printf(_L("%S\n"), &buf);	
	}


void CHttpExampleUtils::GetAnEntry(const TDesC& ourPrompt, TDes& currentstring)
	{
	TBuf16<KMaxUserEntrySize> ourLine;
	TBuf<KMaxUserEntrySize> tempstring;	//tempstring is a unicode descriptor
										//create a temporary buffer where the
										//unicode strings are stored in order to 
										//be displayed
	ourLine.Zero ();
	tempstring.Copy(currentstring);		//Copy current string to Unicode buffer
	TKeyCode key = EKeyNull;						//current string buffer is 8 bits wide.
										//Unicode string bufffer (tempstring) is 16 bits wide.
	FOREVER
		{
		if (ourLine.Length () == 0)
			{
			iTest.Console()->SetPos (0, iTest.Console()->WhereY ());
			iTest.Console()->Printf (_L ("%S"), &ourPrompt);
			if (tempstring.Length () != 0)						//get tempstring's number of items
				iTest.Console()->Printf (_L (" = %S"), &tempstring);	//if not zero print them to iTest.Console()
			iTest.Console()->Printf (_L (" : "));
			iTest.Console()->ClearToEndOfLine ();
			}
		key = iTest.Getch();
		
		  if (key == EKeyBackspace)
				{
					if (ourLine.Length() !=0)
					{
						ourLine.SetLength(ourLine.Length()-1);
						iTest.Console()->Printf (_L ("%c"), key);
						iTest.Console()->SetPos(iTest.Console()->WhereX(),iTest.Console()->WhereY());
						iTest.Console()->ClearToEndOfLine();
					}	// end if (ourLine.Length() !=0)
				}	// end if (key == KeyBackSpace)
		  
		  		  
		  if (key == EKeyDelete) 			
				{
					ourLine.Zero();
					iTest.Console()->SetPos (0, iTest.Console()->WhereY ());
					iTest.Console()->ClearToEndOfLine ();
					tempstring.Copy(ourLine);
					break;
				}
		  
		  if (key == EKeyEnter)
			break;
		
		  if (key < ' ') // ascii code thats not a printable character
			{
			continue;
			}
		
		ourLine.Append (key);
		iTest.Console()->Printf (_L ("%c"), key);
		iTest.Console()->SetPos(iTest.Console()->WhereX(),iTest.Console()->WhereY());
		iTest.Console()->ClearToEndOfLine();
		if (ourLine.Length () == ourLine.MaxLength ())
			break;
		}	// end of for statement

	if ((key == EKeyEnter) && (ourLine.Length () == 0))
		tempstring.Copy (currentstring);				//copy contents of 8 bit "ourLine" descriptor
	
	iTest.Console()->SetPos (0, iTest.Console()->WhereY ());		
	iTest.Console()->ClearToEndOfLine ();
	
	if ((key == EKeyEnter) && (ourLine.Length() !=0))
		tempstring.Copy(ourLine);
	if (tempstring.Length () != 0)						//if temstring length is not zero
		{
		iTest.Console()->Printf (_L (" Entered = %S\n"), &tempstring);	//print the contents to iTest.Console()
		LogIt(_L ("%S = %S\n"), &ourPrompt, &tempstring);
		}

	iTest.Console()->Printf (_L ("\n"));
	currentstring.Copy(tempstring);						//copy 16 bit tempstring descriptor back 
	} 

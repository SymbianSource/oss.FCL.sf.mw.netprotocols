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
//


#define USE_16BITVER

#include <e32base.h>
#include <e32cons.h>
#include <coecntrl.h>
#include "t_UriShortcutTestMain.h"

#ifdef MANUAL

#ifdef USE_16BITVER

#include "turishortcutparser16.h"
#include <uri16.h>
#else
#include "turishortcutparser8.h"
#include <uri8.h>
#endif



// Literal definitions

_LIT(KTxtExampleCode,"TestParser");
_LIT(KFormatFailed,"Failed: leave code=%d\n");
_LIT(KTxtOK,"");
_LIT(KTxtPressAnyKey,"[Press any key]");
const TInt KMaxUserEntrySize = 128;

// Variables
LOCAL_D CConsoleBase* console;      // a new console object for our output

void GetAnEntry(const TDesC& ourPrompt, TDes& currentstring);


void Go()
{
#ifdef USE_16BITVER
    TUriShortcutParser16 parser;
	TBuf16<24> domain;
	TBuf16<24> scheme;
#else
    TUriShortcutParser8 parser;
	TBuf8<24> domain;
	TBuf8<24> scheme;
#endif
   	TBool done = EFalse;
	TKeyCode key = EKeyNull;

   	while(!done)
   	{
   		console->ClearScreen();
		_LIT(KSelectOption, " Select an option \n\n");
		console->Printf(KSelectOption);
		_LIT(KPossibleSelectionsText, " 1 Set default domain \n 2 Default domain \n 3 Set default scheme \n 4 Default scheme \n 5 Set/Unset CheckUnusualSchemes \n 6 Create URI \n 7 Quit \n");
		console->Printf(_L("%S\n"), &KPossibleSelectionsText);

		console->SetPos(0, console->WhereY ());
		key = console->Getch();
		
		switch(key)
		{
			case '1':
			    {
			    
				_LIT(KPrompt1, "Enter domain\n");
#ifdef USE_16BITVER
				GetAnEntry(KPrompt1,domain);
			    parser.SetDefaultDomain(domain);
#else
                TBuf<24> temp;
				GetAnEntry(KPrompt1,temp);
				domain.Copy(temp);
			    parser.SetDefaultDomain(domain);
#endif
			    }
			break;
			case '2':
			    {
			    
				TBuf<24> temp;
				temp.Copy(parser.DefaultDomain());
				console->Printf(_L("Default domain = %S\n"), &temp);
				console->Getch();
			    }
			break;
			case '3':
			    {
			    
				_LIT(KPrompt2, "Enter scheme\n");
#ifdef USE_16BITVER
				GetAnEntry(KPrompt2,scheme);
			    parser.SetDefaultScheme(scheme);
#else
                TBuf<24> temp;
				GetAnEntry(KPrompt2,temp);
				scheme.Copy(temp);
			    parser.SetDefaultScheme(scheme);
#endif
                }
			break;
			case '4':
			    {
				TBuf<24> temp;
				temp.Copy(parser.DefaultScheme());
				console->Printf(_L("Default scheme = %S\n"), &temp);
				console->Getch();
			    }
			break;
			case '5':
			    {
			    
				_LIT(KPrompt2, "Enter '0' for unset or '1' for set\n");
           	    console->Printf(KPrompt2);
               	TKeyCode key = EKeyNull;
               	key = console->Getch();
               	
               	if (key == '1' || key == '0')
                   	{
     			    parser.SetCheckUncommonSchemes(key == '1' ? ETrue : EFalse);
                   	}
               	else
                   	{
               	    console->Printf(_L("Invalid input\n"));
    			    console->Printf(KTxtPressAnyKey);
    			    console->Getch();
                   	}
                }
			break;
			case '6':
			    {
			    
				TBuf<256> uriDes;
				_LIT(KPrompt3, "Enter URI\n");
				GetAnEntry(KPrompt3,uriDes);
#ifdef USE_16BITVER
				TBuf16<256> temp;
				CUri16* uri;
#else
				TBuf8<256> temp;
				CUri8* uri;
#endif
				temp.Copy(uriDes);
			    uri = parser.CreateUriL(temp);
			    if(uri)
				    {
				    TBuf<256> temp;
				    temp.Copy(uri->Uri().UriDes());
				    
					console->Printf(_L("Created URI=%S\n"),&temp );
				    delete uri;
				    }
				else
    				{
					console->Printf(_L("Invalid URI\n"));
    				}
				
			    console->Printf(KTxtPressAnyKey);
			    console->Getch();
			    }
			break;
			case '7':
				done = ETrue;
			
			break;
		}
   	}
   	
}

void GetAnEntry(const TDesC& ourPrompt, TDes& currentstring)
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
			console->SetPos (0, console->WhereY ());
			console->Printf (_L ("%S"), &ourPrompt);
			if (tempstring.Length () != 0)						//get tempstring's number of items
				console->Printf (_L (" = %S"), &tempstring);	//if not zero print them to iTest.Console()
			console->Printf (_L (" : "));
			console->ClearToEndOfLine ();
			}
		key = console->Getch();
		
		  if (key == EKeyBackspace)
				{
					if (ourLine.Length() !=0)
					{
						ourLine.SetLength(ourLine.Length()-1);
						console->Printf (_L ("%c"), key);
						console->SetPos(console->WhereX(),console->WhereY());
						console->ClearToEndOfLine();
					}	// end if (ourLine.Length() !=0)
				}	// end if (key == KeyBackSpace)
		  
		  		  
		  if (key == EKeyDelete) 			
				{
					ourLine.Zero();
					console->SetPos (0, console->WhereY ());
					console->ClearToEndOfLine ();
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
		console->Printf (_L("%c"), key);
		console->SetPos(console->WhereX(),console->WhereY());
		console->ClearToEndOfLine();
		if (ourLine.Length () == ourLine.MaxLength ())
			break;
		}	// end of for statement

	if ((key == EKeyEnter) && (ourLine.Length () == 0))
		tempstring.Copy (currentstring);				//copy contents of 8 bit "ourLine" descriptor
	
	console->SetPos (0, console->WhereY ());		
	console->ClearToEndOfLine ();
	
	if ((key == EKeyEnter) && (ourLine.Length() !=0))
		tempstring.Copy(ourLine);
	if (tempstring.Length () != 0)						//if temstring length is not zero
		{
		console->Printf (_L (" Entered = %S\n"), &tempstring);	//print the contents to iTest.Console()
		}

	console->Printf (_L ("\n"));
	currentstring.Copy(tempstring);						//copy 16 bit tempstring descriptor back 
	} 

void DoTestsL()       // initialize and do main task with cleanup stack
{
    
    console=Console::NewL(KTxtExampleCode,TSize(KConsFullScreen,KConsFullScreen));
    CleanupStack::PushL(console);   // push console

    TRAPD(error,Go());

    if (error)
        console->Printf(KFormatFailed, error);
        else console->Printf(KTxtOK);
    console->Printf(KTxtPressAnyKey);
    console->Getch();               // get and ignore character
    CleanupStack::PopAndDestroy(console);  // close console
    
    
}

#endif




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

#include "TUIEDIT.H"

CLineEdit::CLineEdit()
//
// Constructor
//
	{

//	iHistory=NULL;
//	iConsole=NULL;
//	iMaxHistory=0;
//	iWidth=0;
//	iHeight=0;
//	iPos=0;
//	iOrigin=0;
//	iLine=0;
//	iRecall=0;
//	iBuf=NULL;
//	iPrompt=_L("");
	__DECLARE_NAME(_S("CLineEdit"));
	}


CLineEdit::~CLineEdit()
//
// Destroy the line editor
//
	{

	TInt count=iHistory->Count();
	while (count--)
		User::Free((*iHistory)[count]);
	delete iHistory;
	}

EXPORT_C CLineEdit* CLineEdit::NewL(CConsoleBase* aConsole,TInt aMaxHistory)
//
// Create a new line editor
//
	{

	CLineEdit* pE=new(ELeave) CLineEdit;
	pE->iHistory=new(ELeave) CArrayFixFlat<HBufC*>(aMaxHistory+2);
	pE->iConsole=aConsole;
	pE->iMaxHistory=aMaxHistory;
	pE->iWidth=pE->iConsole->ScreenSize().iWidth;
	pE->iHeight=pE->iConsole->ScreenSize().iHeight;
	return(pE);
	}

TInt CLineEdit::Lines()
//
// The number of lines being edited.
//
    {

    TInt nL=1;
    if (Buf().Length()>=iWidth-2-iOrigin)
		nL+=(Buf().Length()+iOrigin)/(iWidth-2);
    return(nL);
    }

TPoint CLineEdit::Where()
//
// Return the real cursor position.
//
    {

    if (iPos>=(iWidth-2-iOrigin))
		return(TPoint((iPos+iOrigin)%(iWidth-2),((iPos+iOrigin)/(iWidth-2))+iLine));
	return(TPoint(iPos+iOrigin,iLine));
    }

void CLineEdit::ClearLine()
//
// Clears the line being edited.
//
    {

    if (Buf().Length())
		{
		TInt nL=Lines();
		while (nL--)
	    	{
	    	iConsole->SetPos(nL ? 0 : iOrigin,iLine+nL);
	    	iConsole->ClearToEndOfLine();
	    	}
		Buf().Zero();
		iPos=0;
		}
    }

void CLineEdit::ClearLast(TInt aCnt)
//
// Clears the last aCnt characters.
//
    {

    TInt aPos=iPos;
    iPos=((TInt)Buf().Length())-aCnt;
    while (iPos<((TInt)Buf().Length()))
		{
		TPoint p=Where();
		iConsole->SetCursorPosAbs(p);
		iConsole->ClearToEndOfLine();
		iPos+=(iWidth-p.iX);
		}
    iPos=aPos;
    }

void CLineEdit::Recall()
//
// Recall a line for editing.
//
    {

	if (iRecall!=(-1))
		{
		ClearLine();
		HBufC* pL=(*iHistory)[iRecall];
		Buf()=(*pL);
		iConsole->Write(Buf());
		iPos=Buf().Length();
		TInt nL=Lines();
		if ((iLine+nL)>(iHeight-2))
	    	iLine=iHeight-2-nL;
		Buf()=(*pL);
		}
    }

TInt CLineEdit::WordLeft()
//
// Position the cursor to the next word left.
//
    {

    TInt x=iPos-1;
    while (x && TChar(Buf()[x]).IsSpace())
		x--;
    while (x && TChar(Buf()[x]).IsGraph())
		x--;
    if (TChar(Buf()[x]).IsSpace())
		x++;
    return(x);
    }

TInt CLineEdit::WordRight()
//
// Position the cursor to the next word right.
//
    {

    TInt x=iPos;
    while (x<(TInt)Buf().Length() && TChar(Buf()[x]).IsGraph())
		x++;
    while (x<(TInt)Buf().Length() && TChar(Buf()[x]).IsSpace())
		x++;
    return(x);
    }

void CLineEdit::Cursor()
//
// Position the cursor.
//
    {

    iConsole->SetCursorPosAbs(Where());
    }

void CLineEdit::Refresh()
//
// Refresh the line.
//
    {

	iConsole->SetCursorHeight(ECursorNone);
    iConsole->SetPos(iOrigin,iLine);
    iConsole->Write(Buf());
	Cursor();
	iConsole->SetCursorHeight(iMode==EEditOverWrite ? ECursorNormal : ECursorInsert);
    }

EXPORT_C void CLineEdit::Edit(const TDesC& aPrompt,TDes* aBuf)
//
// Start the editor or a single key fetch.
//
    {

	iBuf=aBuf;
    iPos=0;
	Buf().Zero();
	iMode=EEditOverWrite;
	iConsole->SetCursorHeight(iMode==EEditOverWrite ? ECursorNormal : ECursorInsert);
    iConsole->Write(aPrompt);
	iOrigin=iConsole->WhereX();
	iLine=iConsole->WhereY();
	iRecall=(-1);
    TInt hCount=iHistory->Count();
    if (hCount>iMaxHistory)
		hCount=iMaxHistory;
	FOREVER
		{
		TChar gChar=iConsole->Getch();
		switch (gChar)
	    	{
		case EKeyEscape:
	    	ClearLine();
			iRecall=(-1);
	    	break;
		case EKeyHome:
	    	iPos=0;
	    	Cursor();
	    	break;
		case EKeyLeftArrow:
	    	if (iPos)
                {
                if(iConsole->KeyModifiers()==EModifierCtrl)
                    iPos=WordLeft();
                else
    				iPos--;
                Cursor();
                }
	    	break;
		case EKeyRightArrow:
	    	if (iPos<((TInt)Buf().Length()))
                {
                if(iConsole->KeyModifiers()==EModifierCtrl)
                    iPos=WordRight();
                else
    				iPos++;
                Cursor();
                }
	    	break;
		case EKeyEnd:
	    	iPos=((TInt)Buf().Length());
	    	Cursor();
	    	break;
		case EKeyPageUp:
	    	if (hCount==0)
				break;
	    	iRecall=hCount-1;
	    	Recall();
	    	break;
		case EKeyUpArrow:
	    	if (iRecall==(-1))
				{
				if (hCount==0)
		    		break;
				iRecall=0;
				}
	    	else if (iRecall>=(hCount-1))
				{
				ClearLine();
				iRecall=(-1);
				break;
				}
	    	else
				iRecall++;
	    	Recall();
	    	break;
		case EKeyDownArrow:
	    	if (iRecall==(-1))
				{
				if (hCount==0)
		    		break;
				iRecall=hCount-1;
				}
	    	else if (iRecall==0)
				{
				ClearLine();
				iRecall=(-1);
				break;
				}
	    	else
				iRecall--;
	    	Recall();
	    	break;
		case EKeyPageDown:
	    	if (hCount==0)
				break;
	    	iRecall=0;
	    	Recall();
	    	break;
		case EKeyEnter:
	    	iConsole->SetCursorHeight(ECursorNone);
	    	iConsole->SetPos(0,iLine+Lines()-1); // CR on the last line
	    	iConsole->Write(_L("\n")); // Just a line feed
//			iConsole->SetCursorHeight(iDefaultMode==EEditOverWrite ? ECursorNormal : ECursorInsert);
	    	iRecall=(-1);
	    	if (Buf().Length()>2)
				{
				if (iHistory->Count()==iMaxHistory+1)
					{
					User::Free((*iHistory)[iMaxHistory]);
		    		iHistory->Delete(iMaxHistory);
					}
				HBufC* pB=Buf().Alloc();
				if (pB!=NULL)
					{TRAPD(r,iHistory->InsertL(0,pB));}
				}
	    	return;
		case EKeyBackspace:
	    	if (iPos)
				{
				TInt iN=1;
				if (iConsole->KeyModifiers()==EModifierCtrl)
		    		iN=iPos-WordLeft();
				ClearLast(iN);
				iPos-=iN;
				Buf().Delete(iPos,iN);
				Refresh();
				}
	    	break;
		case EKeyDelete:
	    	if (iPos<((TInt)Buf().Length()))
				{
				TInt iN=1;
				if (iConsole->KeyModifiers()==EModifierCtrl)
		    		iN=WordRight()-iPos;
				ClearLast(iN);
				Buf().Delete(iPos,iN);
				Refresh();
				}
	    	break;
		case EKeyInsert:
	    	iMode=(iMode==EEditOverWrite ? EEditInsert : EEditOverWrite);
			iConsole->SetCursorHeight(iMode==EEditOverWrite ? ECursorNormal : ECursorInsert);
	    	break;
		default:
   	    	if (!gChar.IsPrint())
				break;
	    	if (iMode==EEditOverWrite && iPos<((TInt)Buf().Length()))
				Buf()[iPos++]=(TText)gChar;
	    	else if (Buf().Length()<0x100)
				{
				TInt oL=Lines();
				TBuf<0x02> b;
				b.Append(gChar);
				Buf().Insert(iPos++,b);
				TInt nL=Lines();
				if (nL!=oL)
		    		{
		    		iConsole->SetCursorHeight(ECursorNone);
		    		iConsole->SetPos(0,iLine+oL-1);
		    		iConsole->Write(_L("\n"));
		    		iConsole->SetPos(0,iLine);
		    		if (iLine+nL>iHeight-2)
						iLine=iHeight-2-nL;
		    		}
				}
			else
				{
				iConsole->Write(_L("\7"));
				iConsole->SetPos((iOrigin+iPos)%(iWidth-2),iLine+Lines()-1);
				break;
				}
			Refresh();
			}
		}
	}


EXPORT_C void ClientStubOrdinal1() 
	{
	_LIT(KStubPanic, "tuiedit.dll Stub");
	User::Panic(KStubPanic, KErrNotSupported);
	}

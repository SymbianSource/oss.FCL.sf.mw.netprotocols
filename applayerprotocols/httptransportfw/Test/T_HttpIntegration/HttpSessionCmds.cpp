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
// This module implements the collection of HTTP Framework related integration
// test commands and tools
// by:		mjdavey, symbian@mjdss.com, August 2002
// for:	Typhoon (7.0s) & JetStream (8.0)
// Include Files  
// 
//


#include "CHTTPFamily.h"
#include "HttpSessionCmds.h"
#include "HttpFrmwrk.h"
#include "CWspEventDispatcher.h"

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//	Command:	CONNECT command.
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//
//	Form:
//				CONNECT <framework> <connection name>
//	Example:	
//				CONNECT HTTP framework1
//
//	The connection made is assigned the name <connection name> e.g. 'framework1'
//	and is referenced by subsequent tagname replacement.
//	the connection session is then associated with this tagname and can be 
//	used until the corresponding DISCONNECT <connection name> is encountered.
//	The 'Engine' has an arbitrary collection object that is used to maintain these 
//	links

CCmdConnect *CCmdConnect::NewL(TInt aCommandId, const TDesC& aKeyphrase, const TDesC& aHelpPhrase)
	{
	CCmdConnect *self = NewLC( aCommandId, aKeyphrase, aHelpPhrase);
	CleanupStack::Pop();
	return self; 
	}

//-----------------------------------------------------------------------------

CCmdConnect *CCmdConnect::NewLC( TInt aCommandId, const TDesC& aKeyphrase, const TDesC& aHelpPhrase)
	{
	CCmdConnect *self = new (ELeave) CCmdConnect();
	CleanupStack::PushL(self);
	self->ConstructL(aCommandId, aKeyphrase, aHelpPhrase);
	return self;
	}

//-----------------------------------------------------------------------------

void CCmdConnect::ConstructL(TInt aCommandId, const TDesC& aKeyphrase, const TDesC& aHelpPhrase)
	{
	CCmdBase::ConstructL(aCommandId, aKeyphrase, aHelpPhrase);

	//	create the Active Object interface
	iEventDispatcher = CWspEventDispatcher::NewL(*this);  

	WaitForNextEvent (THTTPSessionEvent::EDisconnected);

	}

//-----------------------------------------------------------------------------

CCmdConnect::~CCmdConnect(void)
	{
	iEventDispatcher->CancelWait();

	delete iEventDispatcher;
	// void WaitForNextEvent(TInt);
	}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

void CCmdConnect::DisconnectL(const TDesC& aLabel)
	{
	CAnyObject *obj = (CAnyObject *)Machine()->Domains()->Name(aLabel);
	CFrmwrkSession *frmwk = (CFrmwrkSession *) obj->Ptr();
	TPtrC myName = frmwk->Name() ;
	
	
	Log(_L("CCmdConnect::DisconnectL, Session %S"), &myName);
	frmwk->DisconnectL();
	}

void CCmdConnect::DisconnectL(const TInt &aIndex)
	{
	CAnyObject *obj = (CAnyObject *)Machine()->Domains()->At(aIndex);
	CFrmwrkSession *frmwk = (CFrmwrkSession *) obj->Ptr();
	TPtrC myName = frmwk->Name() ;
	
	
	Log(_L("CCmdConnect::DisconnectL, Session %S"), &myName);
	frmwk->DisconnectL();
	}

void CCmdConnect::Delete(const TDesC& aLabel)
	{
	CAnyObject *obj = (CAnyObject *)Machine()->Domains()->Name(aLabel);
	CFrmwrkSession *frmwk = (CFrmwrkSession *) obj->Ptr();
	TPtrC myName = frmwk->Name() ;

	Log(_L("CCmdConnect::Delete, Session %S"), &myName);
	frmwk->Close(); 
	delete frmwk;
	}

TInt CCmdConnect::ProcessL( const TDesC& aCommand )
{
	// Complete the test machine - will then get the next cmd
	Machine()->CompleteRequest();

	TPtrC framework;
	TPtrC conname;

	TInt error;
	if (( error = ParseCmdArgs( aCommand, framework, conname )) != KErrNone)
		return error;
	
	if (conname.Size() != 0)
		{		
		//	Check if the connection already is defined. 
		//	If it is return error, if not create a session and ptr!
		CAnyObject *cnx = (CAnyObject *)Machine()->Domains()->Name(conname);
	
		if (cnx != NULL)		//	already exists
			return Error(KErrAlreadyExists, THA_KErrCnxionExists, &conname);
		else
			{
			CFrmwrkSession *cithc = CFrmwrkSession::NewLC(conname, framework, Machine());
			cithc->iEventDispatcher = iEventDispatcher ;

			TRAPD(error, cithc->OpenL());
			if (error == KErrNone)
				{
				cithc->SetPropertiesL();
			
				TRAP(error, cithc->ConnectL());
				
				if (error == KErrNone)
					{
					//	now create reference to name in domains
					//	note: the value is 1 for HTTP and 0 for WSP
					Machine()->Domains()->AddL(conname, 0, THA_KHTTP_Connect, (TAny *) cithc);
					WriteDateStamp();
					Log(_L("Connection '%S' has been opened!"), &conname);
					}
				else
					Error(error, KTxtErrConnectFailed);
				}
			
			CleanupStack::Pop();	// cithc
			}
		}
	else
		{		
		(void) ShowSessions () ;
		}

	return error;
}

TInt CCmdConnect::ParseCmdArgs( const TDesC& aCommand, TPtrC& aFramework, TPtrC& aConName ) 
	{
	aFramework.Set(NULL, 0);
	aConName.Set(NULL, 0);
	TInt error = KErrNone;

	//	get the command into a local string
	TPtrC param;
	TRAP(error, param.Set(ParamsL(aCommand)));
	if (error != KErrNone)
		return Error(error, TFR_KFmtErrBadCmd, &Keyphrase());

	//	check its' valid
	TLex parse(param);
	if (!parse.Eos() && !parse.Peek().IsSpace())
		return Error(error, TFR_KFmtErrBadCmd, &Keyphrase());

	//	remove any spaces and see if its got params
	parse.SkipSpace();
	if ( parse.Eos() )	// no params
		return error;
	
	//	Get framework name, must not exceed 16 characters.
	//	should be http or wsp (currently)

	TRAP(error, aFramework.Set(TfrLex::GetL(parse)));
	if ( error != KErrNone) 
		return Error(KErrArgument, TFR_KFmtErrBadCmd, &aFramework);

	//	check is valid length
	aFramework.Set(TfrLex::Peel(aFramework));
	if ( aFramework.Length() > 16 ) 
		return Error(KErrArgument, THA_KErrParameterLong, &aFramework);

	//	check aFramework type valid (HTTP or WSP)
	//	Param shall be HTTP (True) or WSP (False)
	aFramework.Set( TfrLex::TrimAndPeel(aFramework) );
	TBool value = (aFramework.CompareF(THA_KTxtFrameworkHTTP) == 0);
	
	if ( !value && (aFramework.CompareF(THA_KTxtFrameworkWSP) != 0))
		return Error(KErrArgument, THA_KErrFrameworkErr, &aFramework);

	// Get connection/session name value.

	TRAP(error,aConName.Set(TfrLex::GetL(parse)));
	if ( error != KErrNone) 
		return Error(KErrArgument, TFR_KFmtErrBadCmd, &aConName);

	// There shall not be anything more.
	TPtrC remainder = TfrLex::Trim(parse.Remainder());
	if ( remainder.Length() > 0 ) 
		return Error(KErrArgument, TFR_KFmtErrBadCmd, &remainder);

	if (aConName.Length() <= 0)
		return Error(KErrArgument, THA_KErrInvalidConnect, &Keyphrase());

	aConName.Set(TfrLex::Peel(aConName));
	return (KErrNone);
	}

void CCmdConnect::ShowSessions ()
	{
	
	CObjCatalogue *cnxns = Machine()->Domains();
	TInt iCnxns = 0;
	if (cnxns->Count() == 0)
		Log(_L("\tNo connections have been defined"));
	else
		{
		Log(_L("\t   Name   Framework"));
		for (TInt i = 0; i < cnxns->Count(); ++i)
			{
			CAnyObject *obj = (CAnyObject *)cnxns->At(i);
			if (obj->Tag() == THA_KHTTP_Connect)
				{
				TPtrC label = obj->Name();
				TPtrC frmwrk = ((TBool) ((CFrmwrkSession *) obj->Ptr())->UseWSPProtocol()) ? _L("WSP") : _L("HTTP");
				++iCnxns;
				Log(_L("\t%d %S  - %S"), iCnxns, &label, &frmwrk);
				}
			}
		}
	}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//	Command:	DISCONNECT command.
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//
//	Form:
//				DISCONNECT <connection name>
//	Example:	
//				DISCONNECT framework1
//

CCmdDisconnect *CCmdDisconnect::NewL(TInt aCommandId, const TDesC& aKeyphrase, const TDesC& aHelpPhrase)
	{
	CCmdDisconnect *self = NewLC( aCommandId, aKeyphrase, aHelpPhrase);
	CleanupStack::Pop();
	return self; 
	}

//-----------------------------------------------------------------------------

CCmdDisconnect *CCmdDisconnect::NewLC( TInt aCommandId, const TDesC& aKeyphrase, const TDesC& aHelpPhrase)
	{
	CCmdDisconnect *self = new (ELeave) CCmdDisconnect();
	CleanupStack::PushL(self);
	self->ConstructL(aCommandId, aKeyphrase, aHelpPhrase);
	return self;
	}

//-----------------------------------------------------------------------------

void CCmdDisconnect::ConstructL( TInt aCommandId, const TDesC& aKeyphrase, const TDesC& aHelpPhrase)
	{
	CCmdBase::ConstructL(aCommandId, aKeyphrase, aHelpPhrase);
	}

//-----------------------------------------------------------------------------

CCmdDisconnect::~CCmdDisconnect(void) 
	{
	}

TInt CCmdDisconnect::ParseCmdArgs( const TDesC& aCommand, TPtrC& aConName ) 
	{
	aConName.Set(NULL, 0);

	//	get the command into a local string
	TPtrC param;
	TRAPD(error, param.Set(ParamsL(aCommand)));
	if (error != KErrNone)
		return Error(error, TFR_KFmtErrBadCmd, &Keyphrase());

	//	check its' valid
	TLex parse(param);
	if (!parse.Eos() && !parse.Peek().IsSpace())
		return Error(error, TFR_KFmtErrBadCmd, &Keyphrase());

	//	remove any spaces and see if its got params
	parse.SkipSpace();
	if (parse.Eos() ) 
		// If no params return error
		return Error(error, TFR_KFmtErrBadCmd, &Keyphrase());

	// Get connection/session name value.
	TRAP(error,aConName.Set(TfrLex::GetL(parse)));
	if (error != KErrNone) 
		return Error(KErrArgument, TFR_KFmtErrBadCmd, &Keyphrase());

	// There shall not be anything more.
	TPtrC remainder = TfrLex::Trim(parse.Remainder());
	if (remainder.Length() > 0) 
		return Error(KErrArgument, TFR_KFmtErrBadCmd, &Keyphrase());

	if (aConName.Length() <= 0)
		return Error(KErrArgument, THA_KErrInvalidConnect, &aConName);

	aConName.Set(TfrLex::Peel(aConName));
	return (KErrNone);
	}

TInt CCmdDisconnect::ProcessL(const TDesC& aCommand)
	{
	// Complete the test machine - will then get the next cmd
	Machine()->CompleteRequest();

	TInt error;
	TPtrC conname;
	if (( error = ParseCmdArgs( aCommand, conname )) != KErrNone)
		return error;

	//	Check if the connection already is defined. 
	//	If it is return error, if not create a session and ptr!
	CAnyObject *obj = (CAnyObject *) Machine()->Domains()->Name(conname);

	// at this point we can find the session that the disconnect refers to
	// in the Session, save disconnects eventhandler.
		
	if (obj != NULL)	//	connection (name) exists
		{
		//	get reference to list in CmdConnect from Family and retrieve the reference to the session
		CCmdConnect *iConnect = (CCmdConnect *)Family()->Command(THA_KHTTP_Connect);

		iConnect->DisconnectL(conname);
	
		TBool frmwrk = ((CFrmwrkSession *) obj->Ptr())->UseWSPProtocol() ;

		if (!frmwrk)		// HTTP Session so complete request
			{
			//	check that there are no existing transactions (live) for this session
			//	and that the session is not already closed (i.e. in error)

			//XXXXXXX

			iConnect->Delete(conname); 
			Machine()->Domains()->Delete(conname);

			//	what about the iSessionIndex - this is going to be problematic since we need to track the 
			//	index into the session when it changes...
			WriteDateStamp();
			Log(_L("\tConnection '%S' has been disconnected!"), &conname);
			}
		else
			{
			Log(_L("Issued DisconnectL"));
			}
		}
	else
		return Error(KErrNotFound, THA_KErrConnectionErr, &conname);
	
	if ( error != KErrNone )
		Log(TFR_KFmtErrFailed, &Keyphrase(), error);

	return error;
}

void CCmdConnect::WaitForNextEvent(TInt aEventType)
	{
	// Wait for the next event
	iEventRequired	= aEventType;

	iEventDispatcher->WaitForWspEvent();
	}


void CCmdConnect::HandleWspEvent(TRequestStatus aStatus)
{
	TBool fail = EFalse;
	
	if (aStatus == THTTPSessionEvent::EConnect)
		{
		Log(_L("CFrmwrkSession::HandleWspEvent, event EConnect"));
		} 
	else if (aStatus == THTTPSessionEvent::EConnectedOK)
		{
		Log(_L("CFrmwrkSession::HandleWspEvent, event EConnectedOK"));
		}
	else if (aStatus == THTTPSessionEvent::EConnectedWithReducedCapabilities)
		{
		Log(_L("CFrmwrkSession::HandleWspEvent, event EConnectedWithReducedCapabilities"));
		}
	else if (aStatus == THTTPSessionEvent::EDisconnected)
		{
		Log(_L("CFrmwrkSession::HandleWspEvent, event EDisconnected"));
		fail = ETrue;
		}
	else if (aStatus == THTTPSessionEvent::EAuthenticatedOK)
		{
		Log(_L("CFrmwrkSession::HandleWspEvent, event EAuthenticatedOK"));
		} 
	else if (aStatus == THTTPSessionEvent::EAuthenticationFailure)
		{
		Log(_L("CFrmwrkSession::HandleWspEvent, event EAuthenticationFailure"));
		}
	else if (aStatus == THTTPSessionEvent::EConnectionTimedOut)
		{
		Log(_L("CFrmwrkSession::HandleWspEvent, event EConnectionTimedOut"));
		fail = ETrue;
		}
	else
		{
	    Log(_L("CFrmwrkSession::HandleWspEvent, Unrecognised event [%d]"), aStatus.Int());
		fail = ETrue;
		}

	if (fail)
		{
		CObjCatalogue *sessions = Machine()->Domains();

		for (TInt i = 0; i < sessions->Count(); ++i)
			{
			CAnyObject *obj = (CAnyObject *)sessions->At(i);
			if (obj->Tag() == THA_KHTTP_Connect)
				{
				CFrmwrkSession *frmwk = (CFrmwrkSession *) obj->Ptr();

				if (frmwk->UseWSPProtocol() && (frmwk->iState == CFrmwrkSession::EWSPDisconnected))
				{
					TBuf16<256> myNameBuf;
	
					myNameBuf.Copy(frmwk->Name());
		
					Log(_L("CCmdConnect::HandleWspEvent() : Delete %S\r\n"), &myNameBuf);
					Delete(myNameBuf);
					Machine()->Domains()->Delete(myNameBuf);
		
					}
				}
			}
		}
		
	WaitForNextEvent (THTTPSessionEvent::EDisconnected);
}

//-----------------------------------------------------------------------------
// End of File
//-----------------------------------------------------------------------------


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
// This module implements the collection of HTTP Transaction test classes
// by:		mjdavey, symbian@mjdss.com, August 2002
// for:	Typhoon (7.0s) & JetStream (8.0)
// Include Files  
// 
//

#include <e32base.h>
#include "CHTTPFamily.h"
#include "HttpFrmwrk.h"
#include "HttpTransactionCmds.h"
#include "httptransaction.h"


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//	Command:	TRANSACTION command.
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//
//	Form(s):
//				TRANSACTION <connection name> <method> 
//				TRANSACTION <connection name> <method> <URI>
//	Example:	
//				a)	TRANSACTION framework1 get $URI$
//				b)	TRANSACTION framework2 get "http://news.bbc.co.uk/nol/shared/img/global_toolbar/logo.gif"
//				c)	TRANSACTION framework2 post 
//
//	this assumes that <connection name> has already been defined (see CONNECT)
//	examp a) assumes that define URI has already been created (see DEFINE) 
//	examp b) is full form
//	examp c) is for post and will be detailed later

CCmdTransaction *CCmdTransaction::NewL(TInt aCommandId, const TDesC& aKeyphrase, const TDesC& aHelpPhrase)
	{
	CCmdTransaction *self = NewLC( aCommandId, aKeyphrase, aHelpPhrase);
	CleanupStack::Pop();
	return self; 
	}

//-----------------------------------------------------------------------------

CCmdTransaction *CCmdTransaction::NewLC(TInt aCommandId, const TDesC& aKeyphrase, const TDesC& aHelpPhrase)
	{
	CCmdTransaction *self = new (ELeave) CCmdTransaction();
	CleanupStack::PushL( self );
	self->ConstructL(aCommandId, aKeyphrase, aHelpPhrase);
	return self;
	}

//-----------------------------------------------------------------------------

void CCmdTransaction::ConstructL(TInt aCommandId, const TDesC& aKeyphrase, const TDesC& aHelpPhrase)
	{
	CCmdBase::ConstructL(aCommandId, aKeyphrase, aHelpPhrase);

	//	set local vars for convenience
	iTransactionIndex = -1;
	iExpectedError = 0;
	iExpectError = EFalse;
	iTestFail = 0;
	iExpectedStatusCode = 200;
	eStrIndex = HTTP::ECONNECT;
	iHasARequestBody = EFalse;
	iExpectedNumberRedirects = 0;
	}

//-----------------------------------------------------------------------------

CCmdTransaction::~CCmdTransaction(void) {}
	
//-----------------------------------------------------------------------------
//	This must have three parameters: <connection tagname> <methodname> and <uri>
//	the tagname must be checked against existing valid tags and the methodname
//	against a list of our creation. the URI may be an existing define or 
//	a string

TInt CCmdTransaction::ProcessL(const TDesC& aCommand)
	{

	// Complete the test machine - will then get the next cmd
	Machine()->CompleteRequest();

	TPtrC transname;
	TPtrC conname;
	TPtrC transmthd;
	TPtrC transURI;
	TPtrC postData;

	TInt error;
	// ParseCmdArgs indirectly sets eStrIndex
	
	if (( error = ParseCmdArgs( aCommand, 
							    transname, 
								conname, 
								transmthd, 
								transURI, 
								postData)) != KErrNone)
		return error;

	if (conname.Size() == 0)
		{
		(void) ShowTransactions () ;
		return error;
		}

	//	Get the session object we need
	//	If it is return error, if not create a session and ptr!

	CAnyObject *cnx = (CAnyObject *)Machine()->Domains()->Name(conname);
	
	if (cnx == NULL)//	does not already exists
		return Error(KErrArgument, THA_KErrInvalidConnect, &conname);
	else
		{
		//	reset the received status 
		iReceivedError = -1;

		CFrmwrkSession *myConnection = (CFrmwrkSession *) cnx->Ptr();
		// TPtrC myName = myConnection->Name() ;
		
		RHTTPSession mySession = myConnection->Session();
	
		CTestTransaction *transaction = NULL;

		TRAPD(err, transaction = CTestTransaction::NewL( mySession,
														 eStrIndex, 
		                                                 transURI, 
		                                                 postData, 
		                                                 transname,
		                                                 Machine()));
		if (err != KErrNone) 
			{
			Log(_L("Failed to create Transaction %S"), &transname);
			return err;
			}
		else
			{
			Log(_L("Transaction %S created successfully"), &transname);
			}

		TRAP ( err, transaction->SubmitL());

		if (err != KErrNone) 
			{
			Log(_L("Transaction %S,  Submission failed"), &transname);
			return err;
			}
		else
			{
			Log(_L("Transaction %S,  Submission succeeded"), &transname);
			}


		Machine()->Domains()->AddL(transname, eStrIndex, THA_KHTTP_Transaction, (TAny *) transaction);
		}

	if (error != KErrNone)
		Log(TFR_KFmtErrFailed, &Keyphrase(), error);

	return error;
}

//-----------------------------------------------------------------------------

TInt CCmdTransaction::ParseCmdArgs( const TDesC& aCommand, 
								    TPtrC& aTransName,
								    TPtrC& aConName, 
									TPtrC& aTransMthd, 
									TPtrC& aTransURI,
									TPtrC& aPostData)
	{
	aConName.Set(NULL, 0);
	aTransName.Set(NULL, 0);
	aTransMthd.Set(NULL, 0);
	aTransURI.Set(NULL, 0);
	aPostData.Set(NULL, 0);

	//	get the command into a local string
	TPtrC param;
	TInt error = KErrNone;
	TRAP(error, param.Set( ParamsL(aCommand)));
	if (error != KErrNone)
		return Error(error, TFR_KFmtErrBadCmd, &Keyphrase());

	//	check its' valid
	TLex parse(param);
	if (!parse.Eos() && !parse.Peek().IsSpace())
		return Error(error, TFR_KFmtErrBadCmd, &parse);

	//	remove any spaces and see if its got params
	parse.SkipSpace();
	if (parse.Eos())	// If no params, print out present connections.
		return error;

	//-----------------------------
	//	Get transaction name, must not exceed 16 characters.

	TRAP(error, aTransName.Set(TfrLex::GetL(parse)));
	if ( error != KErrNone) 
		return Error(KErrArgument, TFR_KFmtErrBadCmd, &aTransName);

	//	check transaction name is valid length
	aTransName.Set(TfrLex::Peel(aTransName));

	if (aTransName.Length() > 16) 
		return Error(KErrArgument, THA_KErrParameterLong, &aTransName);

	if (aTransName.Length() <= 0)
		return Error(KErrArgument, TFR_KFmtErrMissingParams, &Keyphrase());

	CAnyObject *cnx = (CAnyObject *)Machine()->Domains()->Name(aTransName);
	
	if (cnx != NULL)		//	already exists
		return Error(KErrAlreadyExists, THA_KErrCnxionExists, &aTransName);


	//-----------------------------
	//	Get framework name, must not exceed 16 characters.
	//	should be http or wsp (currently)

	TRAP(error, aConName.Set(TfrLex::GetL(parse)));
	if ( error != KErrNone) 
		return Error(KErrArgument, TFR_KFmtErrBadCmd, &aConName);

	//	check connection name is valid length
	aConName.Set(TfrLex::Peel(aConName));
	if (aConName.Length() > 16) 
		return Error(KErrArgument, THA_KErrParameterLong, &aConName);

	if (aConName.Length() <= 0)
		return Error(KErrArgument, TFR_KFmtErrMissingParams, &Keyphrase());

	//	check connection name exists
	if (!checkConnectionExists(aConName))
		return Error(KErrArgument, THA_KErrConnectionNotExists, &aConName);

	//-----------------------------
	// Get method value.
	TRAP(error, aTransMthd.Set(TfrLex::GetL(parse)));
	if (error != KErrNone) 
		return Error(KErrArgument, TFR_KFmtErrBadCmd, &aTransMthd);

	//	check method name is valid length
	aTransMthd.Set(TfrLex::Peel(aTransMthd));
	if (aTransMthd.Length() > 16) 
		return Error(KErrArgument, THA_KErrParameterLong, &aTransMthd);

	if (aTransMthd.Length() <= 0)
		return Error(KErrArgument, TFR_KFmtErrMissingParams, &Keyphrase());

	//	check method defined
	if (!checkMethodExists(aTransMthd))
		return Error(KErrArgument, THA_KErrMethodNotExists, &aTransMthd);

	//-----------------------------
	// Get URI value.

	TRAP(error, aTransURI.Set(TfrLex::GetL(parse)));
	if (error != KErrNone) 
		return Error(KErrArgument, TFR_KFmtErrBadCmd, &aTransURI);

	//	uri's may be enormous so lets not bother worrying about length!
	aTransURI.Set(TfrLex::Peel(aTransURI));

	if (aTransURI.Length() <= 0)
		return Error(KErrArgument, TFR_KFmtErrMissingParams, &Keyphrase());

	//-----------------------------
	//	Get Additional Parameters
	//	some commands (e.g. POST) may require an additional parameter
	//	to provide either filename (for source data) or block of text
	//	for posting...

	TRAP(error, aPostData.Set(TfrLex::GetL(parse)));
	if (error != KErrNone) 
		return Error(KErrArgument, TFR_KFmtErrBadCmd, &aPostData);

	//	datablocks and filenames may be enormous so lets not bother worrying about length!
	aPostData.Set(TfrLex::Peel(aPostData));

	//-----------------------------
	//	There should not be anything more (currently)
	//  Future additions MAY required optional parameters and therefore the following may
	//	need to be dumped...

	TPtrC remainder = TfrLex::Trim(parse.Remainder());
	if (remainder.Length() > 0) 
		return Error(KErrArgument, TFR_KFmtErrBadCmd, &remainder);

	aConName.Set(TfrLex::Peel(aConName));
	return (KErrNone);
	}

void CCmdTransaction::ShowTransactions() 
	{
	TInt iTrans = 0;
	
	CObjCatalogue *trnsctns = Machine()->Domains();
	Log(_L("\tCurrent Transactions:"));
	Log(_L("\tName  \tMethod  \tURI"));
	for (TInt i = 0; i < trnsctns->Count(); ++i)
		{
		CAnyObject *obj = (CAnyObject *)trnsctns->At(i);
		if (obj->Tag() == THA_KHTTP_Transaction)
			{
			TPtrC label = obj->Name();
			TInt eStrIndex = obj->Index();
			CTestTransaction *trans = (CTestTransaction *)obj->Ptr();
			TBuf<256> uri;
			uri.Copy (trans->Uri()) ;
									
			TPtrC mthd;
						
			switch (eStrIndex)
				{
				case HTTP::EGET : mthd.Set(THA_TxtCmdTransactionGET); break;
				case HTTP::EPOST : mthd.Set(THA_TxtCmdTransactionPOST); break;
				case HTTP::EPUT : mthd.Set(THA_TxtCmdTransactionPUT); break;
				case HTTP::EDELETE : mthd.Set(THA_TxtCmdTransactionDELETE); break;
				case HTTP::EHEAD : mthd.Set(THA_TxtCmdTransactionHEAD); break;
				case HTTP::EOPTIONS : mthd.Set(THA_TxtCmdTransactionOPTIONS); break;
				case HTTP::ETRACE : mthd.Set(THA_TxtCmdTransactionTRACE); break;
				default : break;
				}
			++iTrans;
			Log(_L("\t%d\t%S\t%S\t%S"), iTrans, 
				&label, 
				&mthd,
				&uri);
			}
		}
	if (iTrans == 0)
		Log(_L("\tNo transactions have been defined"));
	}

//-----------------------------------------------------------------------------
//	check connection name exists
//	we have to connect to the object catalogue and look at sessions!

TBool CCmdTransaction::checkConnectionExists(TPtrC &aName)
	{
	CObjCatalogue *sessions = Machine()->Domains();
	for (TInt i = 0; i < sessions->Count(); ++i)
		{
		CAnyObject *obj = (CAnyObject *)sessions->At(i);
		if (obj->Tag() == THA_KHTTP_Connect)
			{
			TPtrC sessname = obj->Name();
			if (sessname.CompareF(aName) == 0)
				return ETrue;
			}
		}

	return EFalse;
	}


//-----------------------------------------------------------------------------
//	check method name exists
//	we have to compare supplied name to our existing list of method
//	use crude compare for now, but perhaps add more elegant list facility?!
//	the enums are from HttpStringConstants.h and should not be interferred with!!!

TBool CCmdTransaction::checkMethodExists(TPtrC &aName)
	{
	if (aName.CompareF(THA_TxtCmdTransactionGET) == 0) 
		{
		eStrIndex = HTTP::EGET;
		return ETrue;
		}
	else if (aName.CompareF(THA_TxtCmdTransactionPOST) == 0) 
		{
		eStrIndex = HTTP::EPOST;
		return ETrue;
		}
	else if (aName.CompareF(THA_TxtCmdTransactionPUT) == 0) 
		{
		eStrIndex = HTTP::EPUT;
		return ETrue;
		}
	else if (aName.CompareF(THA_TxtCmdTransactionHEAD) == 0) 
		{
		eStrIndex = HTTP::EHEAD;
		return ETrue;
		}
	else if (aName.CompareF(THA_TxtCmdTransactionDELETE) == 0) 
		{
		eStrIndex = HTTP::EDELETE;
		return ETrue;
		}
	else if (aName.CompareF(THA_TxtCmdTransactionOPTIONS) == 0) 
		{
		eStrIndex = HTTP::EOPTIONS;
		return ETrue;
		}
	else if (aName.CompareF(THA_TxtCmdTransactionTRACE) == 0) 
		{
		eStrIndex = HTTP::ETRACE;
		return ETrue;
		}

	return EFalse;
	}


CCmdEndTransaction *CCmdEndTransaction::NewL(TInt aCommandId, const TDesC& aKeyphrase, const TDesC& aHelpPhrase)
	{
	CCmdEndTransaction *self = NewLC( aCommandId, aKeyphrase, aHelpPhrase);
	CleanupStack::Pop();
	return self; 
	}

//-----------------------------------------------------------------------------

CCmdEndTransaction *CCmdEndTransaction::NewLC(TInt aCommandId, const TDesC& aKeyphrase, const TDesC& aHelpPhrase)
	{
	CCmdEndTransaction *self = new (ELeave) CCmdEndTransaction();
	CleanupStack::PushL( self );
	self->ConstructL(aCommandId, aKeyphrase, aHelpPhrase);
	return self;
	}

//-----------------------------------------------------------------------------

void CCmdEndTransaction::ConstructL(TInt aCommandId, const TDesC& aKeyphrase, const TDesC& aHelpPhrase)
	{
	CCmdBase::ConstructL(aCommandId, aKeyphrase, aHelpPhrase);
	}

//-----------------------------------------------------------------------------

CCmdEndTransaction::~CCmdEndTransaction(void) {}

//-----------------------------------------------------------------------------

TInt CCmdEndTransaction::ParseCmdArgs( const TDesC& aCommand, 
								    TPtrC& aTransName)
	{
	aTransName.Set(NULL, 0);

	//	get the command into a local string
	TPtrC param;
	TInt error = KErrNone;
	TRAP(error, param.Set( ParamsL(aCommand)));
	if (error != KErrNone)
		return Error(error, TFR_KFmtErrBadCmd, &Keyphrase());

	//	check its' valid
	TLex parse(param);
	if (!parse.Eos() && !parse.Peek().IsSpace())
		return Error(error, TFR_KFmtErrBadCmd, &parse);

	//	remove any spaces and see if its got params
	parse.SkipSpace();
	if (parse.Eos())	// If no params, print out present connections.
		return error;

	//-----------------------------
	//	Get transaction name, must not exceed 16 characters.

	TRAP(error, aTransName.Set(TfrLex::GetL(parse)));
	if ( error != KErrNone) 
		return Error(KErrArgument, TFR_KFmtErrBadCmd, &aTransName);

	//	check transaction name is valid length
	aTransName.Set(TfrLex::Peel(aTransName));

	if (aTransName.Length() > 16) 
		return Error(KErrArgument, THA_KErrParameterLong, &aTransName);

	if (aTransName.Length() <= 0)
		return Error(KErrArgument, TFR_KFmtErrMissingParams, &Keyphrase());

	return (KErrNone);
	}


TInt CCmdEndTransaction::ProcessL(const TDesC& aCommand)
	{
	// Complete the test machine - will then get the next cmd
	Machine()->CompleteRequest();

	TPtrC transname;
	
	TInt error = KErrNone;
		
	if (( error = ParseCmdArgs( aCommand, 
							    transname)) != KErrNone)
		return error;

	CAnyObject *obj = (CAnyObject *)Machine()->Domains()->Name(transname);
	
	if ((obj == NULL) || (obj->Tag() != THA_KHTTP_Transaction))
		return Error(KErrArgument, THA_KErrInvalidTransaction, &transname);
	
	CTestTransaction *transaction = (CTestTransaction *) obj->Ptr();

	transaction->CloseTransaction();
	delete transaction;
	
	Machine()->Domains()->Delete(transname);

	return error;
}

CCmdShowTransaction *CCmdShowTransaction::NewL(TInt aCommandId, const TDesC& aKeyphrase, const TDesC& aHelpPhrase)
	{
	CCmdShowTransaction *self = NewLC( aCommandId, aKeyphrase, aHelpPhrase);
	CleanupStack::Pop();
	return self; 
	}

//-----------------------------------------------------------------------------

CCmdShowTransaction *CCmdShowTransaction::NewLC(TInt aCommandId, const TDesC& aKeyphrase, const TDesC& aHelpPhrase)
	{
	CCmdShowTransaction *self = new (ELeave) CCmdShowTransaction();
	CleanupStack::PushL( self );
	self->ConstructL(aCommandId, aKeyphrase, aHelpPhrase);
	return self;
	}

//-----------------------------------------------------------------------------

void CCmdShowTransaction::ConstructL(TInt aCommandId, const TDesC& aKeyphrase, const TDesC& aHelpPhrase)
	{
	CCmdBase::ConstructL(aCommandId, aKeyphrase, aHelpPhrase);
	}

//-----------------------------------------------------------------------------

CCmdShowTransaction::~CCmdShowTransaction(void) {}

//-----------------------------------------------------------------------------


TInt CCmdShowTransaction::ProcessL(const TDesC& aCommand)
	{
	// Complete the test machine - will then get the next cmd
	Machine()->CompleteRequest();
	
	TPtrC param;
	TInt error = KErrNone;
	TRAP(error, param.Set( ParamsL(aCommand)));
	if (error != KErrNone)
		return Error(error, TFR_KFmtErrBadCmd, &Keyphrase());
	
	CObjCatalogue *trnsctns = Machine()->Domains();
	TInt iTrans = 0;

	Log(_L("\tCurrent Transactions:"));
	Log(_L("\tName  \tMethod  \tURI"));
	for (TInt i = 0; i < trnsctns->Count(); ++i)
		{
		CAnyObject *obj = (CAnyObject *)trnsctns->At(i);
		if (obj->Tag() == THA_KHTTP_Transaction)
			{
			TPtrC label = obj->Name();
			TInt eStrIndex = obj->Index();
			CTestTransaction *trans = (CTestTransaction *)obj->Ptr();
			TBuf<256> uri;
			uri.Copy (trans->Uri()) ;
									
			TPtrC mthd;
						
			switch (eStrIndex)
				{
				case HTTP::EGET : mthd.Set(THA_TxtCmdTransactionGET); break;
				case HTTP::EPOST : mthd.Set(THA_TxtCmdTransactionPOST); break;
				case HTTP::EPUT : mthd.Set(THA_TxtCmdTransactionPUT); break;
				case HTTP::EDELETE : mthd.Set(THA_TxtCmdTransactionDELETE); break;
				case HTTP::EHEAD : mthd.Set(THA_TxtCmdTransactionHEAD); break;
				case HTTP::EOPTIONS : mthd.Set(THA_TxtCmdTransactionOPTIONS); break;
				case HTTP::ETRACE : mthd.Set(THA_TxtCmdTransactionTRACE); break;
				default : break;
				}
			++iTrans;
//			if (trans->iState == CTestTransaction::EActive)
			if (trans->State() == CTestTransaction::EActive)
				{
				Log(_L("\t%d\t%S\t%S\t%S"), iTrans, 
					&label, 
					&mthd,
					&uri);
				}
			}
		}
	if (iTrans == 0)
		Log(_L("\tNo transactions have been defined"));
		
	return KErrNone;
	
	}



//-----------------------------------------------------------------------------
// End of File
//-----------------------------------------------------------------------------


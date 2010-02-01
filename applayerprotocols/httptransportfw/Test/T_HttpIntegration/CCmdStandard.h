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
// The header file introducing the collection of standard command classes
// of the tester framework.
// Include Files  
// 
//

#ifndef __STDCMDS_H__
#define __STDCMDS_H__

//-----------------------------------------------------------------------------
//  Include Files  

#include <e32base.h>
#include <inetprottextutils.h>

//-----------------------------------------------------------------------------

#include "GT0149.h"
#include "TEngine.h"
#include "CCmdFamily.h"
#include "CCmdBase.h"
#include "CLogFile.h"
#include "CIFControl.h"
#include "CCatalogue.h"
#include "TfrLex.h"

//-----------------------------------------------------------------------------
//	Command:	The '?' command which forces a listing of all available commands

class CCmdListAll : public CCmdBase
{
public:

	static CCmdListAll *NewL ( TInt, const TDesC& aKeyphrase, const TDesC& aHelpPhrase);
	static CCmdListAll *NewLC( TInt, const TDesC& aKeyphrase, const TDesC& aHelpPhrase);

protected:  // from CCmdBase

	void ConstructL( TInt aCommandId, const TDesC& aKeyphrase, const TDesC& aHelpPhrase);

public:     // from CCmdBase
	TBool Recognize( const TDesC& aCommand );
	TInt  ProcessL ( const TDesC& aCommand );
};

//-----------------------------------------------------------------------------
//	Command:	The 'Comment' command (either // or #)

class CCmdRemark : public CCmdBase
{
public:

	static CCmdRemark* NewL ( TInt aCommandId, const TDesC& aKeyphrase );
	static CCmdRemark* NewLC( TInt aCommandId, const TDesC& aKeyphrase );

protected:  // from CCmdBase

	void ConstructL( TInt aCommandId, const TDesC& aKeyphrase );

public:     // from CCmdBase
	TBool Recognize( const TDesC& aCommand );
	TInt  ProcessL ( const TDesC& aCommand );
};

//-----------------------------------------------------------------------------
//	Command:	Quit testing.

class CCmdExit : public CCmdBase
{
public:

	static CCmdExit* NewL ( TInt aCommandId, const TDesC& aKeyphrase );
	static CCmdExit* NewLC( TInt aCommandId, const TDesC& aKeyphrase );

protected:
	
	void ConstructL( TInt aCommandId, const TDesC& aKeyphrase );

public:
	
	TInt  ProcessL ( const TDesC& aCommand );
};

//-----------------------------------------------------------------------------
//	Command:	Set echo mode switch.

class CCmdEchoMode : public CCmdBase
{
public:

	static CCmdEchoMode* NewL ( TInt aCommandId, const TDesC& aKeyphrase );
	static CCmdEchoMode* NewLC( TInt aCommandId, const TDesC& aKeyphrase );

protected:

	void ConstructL( TInt aCommandId, const TDesC& aKeyphrase );

public:
	
	TInt  ProcessL ( const TDesC& aCommand );
};

//-----------------------------------------------------------------------------
//	Command:	Hold processing until user presses any key.

class CCmdPause : public CCmdBase
{
public:

	static CCmdPause* NewL ( TInt aCommandId, const TDesC& aKeyphrase );
	static CCmdPause* NewLC( TInt aCommandId, const TDesC& aKeyphrase );

protected:
	
	void ConstructL( TInt aCommandId, const TDesC& aKeyphrase );

public:
	
	TInt  ProcessL ( const TDesC& aCommand );
};

//-----------------------------------------------------------------------------
//	Command:	Hold processing for a while (n seconds).

class CCmdHold : public CCmdBase
{
public:

	static CCmdHold* NewL ( TInt aCommandId, const TDesC& aKeyphrase );
	static CCmdHold* NewLC( TInt aCommandId, const TDesC& aKeyphrase );

protected:

	void ConstructL( TInt aCommandId, const TDesC& aKeyphrase );

public:
	
	TInt  ProcessL ( const TDesC& aCommand );
};

//-----------------------------------------------------------------------------
//	Command:	Set path of output log files.


class CCmdLogPath : public CCmdBase
{
public:

	static CCmdLogPath* NewL ( TInt aCommandId, const TDesC& aKeyphrase );
	static CCmdLogPath* NewLC( TInt aCommandId, const TDesC& aKeyphrase );

protected:

	void ConstructL( TInt aCommandId, const TDesC& aKeyphrase );

public:
	
	TInt  ProcessL ( const TDesC& aCommand );
};

//-----------------------------------------------------------------------------
//	Command:	Open/Create a log file.

class CCmdLogFile : public CCmdBase
{
public:

	static CCmdLogFile* NewL ( TInt aCommandId, const TDesC& aKeyphrase );
	static CCmdLogFile* NewLC( TInt aCommandId, const TDesC& aKeyphrase );

protected:

	void ConstructL( TInt aCommandId, const TDesC& aKeyphrase );

public:

	TInt  ProcessL ( const TDesC& aCommand );
};

//-----------------------------------------------------------------------------
//	Command:	End log file, restore the previous one.

class CCmdEndLog : public CCmdBase
{
public:

	static CCmdEndLog* NewL ( TInt aCommandId, const TDesC& aKeyphrase );
	static CCmdEndLog* NewLC( TInt aCommandId, const TDesC& aKeyphrase );

protected:

	void ConstructL( TInt aCommandId, const TDesC& aKeyphrase );
public:

	TInt ProcessL ( const TDesC& aCommand );
};

//-----------------------------------------------------------------------------
//	Command:	Set command prompt.

class CCmdSetPrompt : public CCmdBase
{
public:

	static CCmdSetPrompt* NewL ( TInt aCommandId, const TDesC& aKeyphrase );
	static CCmdSetPrompt* NewLC( TInt aCommandId, const TDesC& aKeyphrase );

protected:  // from CCmdBase

	void ConstructL( TInt aCommandId, const TDesC& aKeyphrase );

public:     // from CCmdBase

	TInt  ProcessL ( const TDesC& aCommand );
};

//-----------------------------------------------------------------------------
//	Command:	Set command file input path.

class CCmdSetPath : public CCmdBase
{
public:

	static CCmdSetPath* NewL (TInt aCommandId, const TDesC& aKeyphrase);
	static CCmdSetPath* NewLC(TInt aCommandId, const TDesC& aKeyphrase);

protected:  // from CCmdBase

	void ConstructL(TInt aCommandId, const TDesC& aKeyphrase);

public:     // from CCmdBase
	
	TInt ProcessL(const TDesC& aCommand);
};

//-----------------------------------------------------------------------------
//	Command:	Call (execute) a command file.

class CCmdCall : public CCmdBase
{
public:

	static CCmdCall *NewL ( TInt, const TDesC& aKeyphrase, const TDesC& aHelpPhrase);
	static CCmdCall *NewLC( TInt, const TDesC& aKeyphrase, const TDesC& aHelpPhrase);

protected:

	void ConstructL( TInt aCommandId, const TDesC& aKeyphrase, const TDesC& aHelpPhrase);

public:
	
	TInt  ProcessL ( const TDesC& aCommand );
};


//-----------------------------------------------------------------------------
//	Command:	Return from current command file.

class CCmdReturn : public CCmdBase
{
public:

	static CCmdReturn* NewL ( TInt aCommandId, const TDesC& aKeyphrase );
	static CCmdReturn* NewLC( TInt aCommandId, const TDesC& aKeyphrase );

protected:

	void ConstructL( TInt aCommandId, const TDesC& aKeyphrase );

public:
	
	TInt  ProcessL ( const TDesC& aCommand );
};

//-----------------------------------------------------------------------------
//	Command:	Set check mode switch.

class CCmdCheckMode : public CCmdBase
{
public:

	static CCmdCheckMode* NewL ( TInt aCommandId, const TDesC& aKeyphrase );
	static CCmdCheckMode* NewLC( TInt aCommandId, const TDesC& aKeyphrase );

protected:

	void ConstructL( TInt aCommandId, const TDesC& aKeyphrase );

public:

	TInt  ProcessL ( const TDesC& aCommand );
};

//-----------------------------------------------------------------------------
//	Command:	Print text.

class CCmdPrint : public CCmdBase
{
public:

	static CCmdPrint *NewL (TInt, const TDesC &, const TDesC &);
	static CCmdPrint *NewLC(TInt, const TDesC &, const TDesC &);

protected:  // from CCmdBase

	void ConstructL(TInt, const TDesC &, const TDesC &);
	TBool Recognize(const TDesC& aCommand);

public:     // from CCmdBase

	TInt  ProcessL(const TDesC& aCommand);

private:
	TBool iCurEchoMode;
};

//-----------------------------------------------------------------------------
//	Command:	SCRIPT - execute a script from interactive console

class CCmdScript : public CCmdBase
{
public:

	static CCmdScript *NewL (TInt, const TDesC &, const TDesC &);
	static CCmdScript *NewLC(TInt, const TDesC &, const TDesC &);

protected:  // from CCmdBase

	void ConstructL(TInt, const TDesC &, const TDesC &);

public:

	TInt ProcessL(const TDesC& aCommand);
};

//-----------------------------------------------------------------------------
//	Command:	Define a tag.

class CCmdDefine : public CCmdBase
{
public:

	//static CCmdDefine* NewL ( TInt aCommandId, const TDesC& aKeyphrase );
	//static CCmdDefine* NewLC( TInt aCommandId, const TDesC& aKeyphrase );
	static CCmdDefine *NewL (TInt, const TDesC &, const TDesC &);
	static CCmdDefine *NewLC(TInt, const TDesC &, const TDesC &);

protected:  // from CCmdBase

	void ConstructL(TInt, const TDesC &, const TDesC &);

public:     // from CCmdBase
	
	TInt  ProcessL ( const TDesC& aCommand );
};

//-----------------------------------------------------------------------------
//	Command:	IF. Performs test on given defines and executes next command if 
//								true. 

class CCmdIf : public CCmdBase
{
public:

	static CCmdIf *NewL (TInt, const TDesC &, const TDesC &);
	static CCmdIf *NewLC(TInt, const TDesC &, const TDesC &);

protected:  // from CCmdBase

	void ConstructL(TInt, const TDesC &, const TDesC &);
	TBool Recognize(const TDesC& aCommand);

public:     // from CCmdBase

	TInt  ProcessL ( const TDesC& aCommand );
};

//-----------------------------------------------------------------------------
//	Command:	ELSE. If the preceeding IF fails (i.e result is false)
//						then this 'branch' of the script is executed
//						not a practial command line type - more useful for scripts!

class CCmdElse : public CCmdBase
{
public:

	static CCmdElse *NewL (TInt, const TDesC &, const TDesC &);
	static CCmdElse *NewLC(TInt, const TDesC &, const TDesC &);

protected:  // from CCmdBase

	void ConstructL(TInt, const TDesC &, const TDesC &);
	TBool Recognize(const TDesC &);

public:     // from CCmdBase

	TInt ProcessL(const TDesC &);
};

//-----------------------------------------------------------------------------
//	Command:	ENDIF. Closes an IF block does nothing more then ensure that the
//						stepover facility is cleared for the following commands

class CCmdEndIf : public CCmdBase
{
public:

	static CCmdEndIf *NewL (TInt, const TDesC &, const TDesC &);
	static CCmdEndIf *NewLC(TInt, const TDesC &, const TDesC &);

protected:  // from CCmdBase

	void ConstructL(TInt, const TDesC &, const TDesC &);
	TBool Recognize(const TDesC &);

public:     // from CCmdBase

	TInt ProcessL(const TDesC &);

};

//-----------------------------------------------------------------------------

#endif  // __STDCMDS_H_

//-----------------------------------------------------------------------------
// End of File
//-----------------------------------------------------------------------------

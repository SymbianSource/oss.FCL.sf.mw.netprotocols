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
// 
//

#include <e32base.h>
#include <e32cons.h>
#include <f32file.h>

//-----------------------------------------------------------------------------

#ifndef __TENGINE_H__
#define __TENGINE_H__

//-----------------------------------------------------------------------------

#include "GT0149.h"
#include "ECommandIDs.h"
#include "CPile.h"	
#include "CCmdFamily.h"		
#include "CCmdFile.h"			
#include "CLogFile.h"			
#include "CIFControl.h"			
#include "CConsoleReader.h"
#include "CCatalogue.h"
#include "CObjCatalogue.h"
#include "CCmdBase.h"

//-----------------------------------------------------------------------------
//	Forward declare Templated Classes

template<class T> class CPile;

#if !defined(TEMPLATED_CATALOGUE)

class CCatalogue;

#else

template<class T> class CCatalogue;

#endif

//-----------------------------------------------------------------------------

class   TTestMachineOverflow : public TDes16Overflow
{
public:
	virtual void Overflow( TDes16& aDes );
};

//-----------------------------------------------------------------------------

class CTEngine : public CActive
{
public:     // Constructors and destructors

	// Constructors with and without console.
	static CTEngine* NewL ( CConsoleBase* aConsole = NULL );
	static CTEngine* NewLC( CConsoleBase* aConsole = NULL );

	//  Destructor.
	~CTEngine();

private:    // Constructors and destructors

	// Private constructor.
	void ConstructL();

	// Default constructor is not there.
	CTEngine(CConsoleBase *aConsole);

public:     // New functions

	// Set command family (the known commands).
	void SetCmdFamily(CCmdFamily *);

	// Console is enabled?
	TBool ConsoleEnabled();

	// Disable console i/o.
	void DisableConsole();

	// Enable console i/o.
	void EnableConsole();

	// Get console, NULL if is not available.
	CConsoleBase *Console() const;

	// Unformatted Write to console (if enabled).
	void Write  (const TDesC &);
	void Writeln(const TDesC &);
	void Writeln(); // new-line

	// Formatted Printf to console (if enabled).
	void Printf  (TRefByValue<const TDesC> aFmt, ...);
	void Printf  (TRefByValue<const TDesC> aFmt, VA_LIST& aList);
	void Printfln(TRefByValue<const TDesC> aFmt, ...);
	void Printfln(TRefByValue<const TDesC> aFmt, VA_LIST& aList);

	// Wait for a key press
	void Pause(const TDesC& aPrompt);

	// Hold for an interval
	void Hold(TTimeIntervalMicroSeconds32 aInterval);

	// Set and Get the command file path. SetCmdPath returns
	// KErrArgument for illegal aPath param.
	TInt SetCmdPath(const TDesC &);
	TPtrC CmdPath() const;

	TPtrC FileName() const;

	// Call command file and Get the present command file.
	TInt CallCmdFile(const TDesC &);
	TInt CallCmdFile(const TDesC &, const TDesC &);
	CCmdFile *CmdFile();

	// Close the present command file or close them all.
	TInt CloseCmdFile();
	void CloseCmdFiles();

	// Set and Get the log file path. SetLogPath returns
	// KErrArgument for illegal aPath param.
	TInt SetLogPath(const TDesC &);
	TPtrC LogPath() const;

	// Open a log file and Get the present log file.
	TInt OpenLogFile(const TDesC &);
	CLogFile *LogFile();

	// Close the present log file or close them all.
	TInt CloseLogFile();
	void CloseLogFiles();

	// Unformatted Write to the log (if there's one).
	void LogWrite  (const TDesC &);
	void LogWriteln(const TDesC &);
	void LogWriteln();

	// Formatted Printf to the log (if there's one).
	void LogPrintf  (TRefByValue<const TDesC> aFmt, ... );
	void LogPrintf  (TRefByValue<const TDesC> aFmt, VA_LIST &);
	void LogPrintfln(TRefByValue<const TDesC> aFmt, ... );
	void LogPrintfln(TRefByValue<const TDesC> aFmt, VA_LIST &);

	// Standard message output. A message you write goes to the log
	// if there is one open. A message is printed to the display as
	// well, except when running a command file with echo mode off.
	void MsgWrite   (const TDesC& aText);
	void MsgWriteln (const TDesC& aText);
	void MsgWriteln ();
	void MsgPrintf  (TRefByValue<const TDesC> aFmt, ...);
	void MsgPrintf  (TRefByValue<const TDesC> aFmt, VA_LIST& aList);
	void MsgPrintfln(TRefByValue<const TDesC> aFmt, ...);
	void MsgPrintfln(TRefByValue<const TDesC> aFmt, VA_LIST& aList);

	// Set and Get the command Prompt.
	TInt SetPrompt(const TDesC &aPrompt);
	TPtrC Prompt() const;

	// Run commands from the console.
	void Run();

	//	Run with just the file name (no args)
	TInt Run(const TDesC &);

	//	Run with the file name and args
	TInt Run(const TDesC &, const TDesC &);

	// Stop the test engine.
	void Terminate();  // Terminate command files and log files.
	void StopEngine(); // Terminate and stop i.e. exit test run
	TBool Stopped();    // Stop has been raised?

	// Set and Get Echo mode On/Off switch.
	void SetEchoMode(const TBool & = ETrue);
	TBool EchoMode();

	// Get the defines.
	#if !defined(TEMPLATED_CATALOGUE)
		CCatalogue *Defines();

		CObjCatalogue *Domains() const {return iDomain;}
		
	#else
		CPile<CLabeledText> *Defines();
	#endif

	// Complete the asynch request
	void CompleteRequest();

	TInt SetFileName(const TDesC &aName, RFile &aReqFile) ;

	void GetUserInput(const TDesC &, TDes &);
	void DumpData(const TDesC8 &, const TBool & = ETrue);

	TBool Log(void) { return iLogActivity; }

	void SetResultDefine(TInt aValue);
	void SetResultDefine (const TDesC& aDefineTxt, TInt aValue);
	void SetErrorDefine(TInt aValue);
	void SetErrorDefine (const TDesC& aDefineTxt, TInt aValue);
	void DeleteResultDefine (const TDesC& aDefineTxt);
	void DeleteErrorDefine (const TDesC& aDefineTxt);

	void SetShowCmdLine(const TBool &aValue) { iShowCmdLine = aValue; }
	TBool GetShowCmdLine(void) { return iShowCmdLine; }

	TPtrC GetDefine(const TDesC &) const;

	//----------------------------------------------------------------
	//	helper methods for IfMode

	CIFControl::TIFMode GetIfMode();
	void SetIfMode(const CIFControl::TIFMode &);
	CIFControl::TIFProcessing GetIfState();
	void SetIfState(const CIFControl::TIFProcessing &);
	void IfL(const CIFControl::TIFMode &, const TBool &, const CIFControl::TIFProcessing &);
	void EndIf(); 
	void UpdateIFState(const CIFControl::TIFMode &);

//-----------------------------------------------------------------------------

private:

	// Read command line (iCmdline).
	TInt ReadCmdline(TBool aConsole = ETrue);
	TInt ReadCmdFile(); // from command file
	TInt ReadConsole(); // from console

	// Make the command (iCommand) to obey. Copies the command
	// line (iCmdline) to iCommand buffer and expands the tags
	// to their defined values.
	TInt MakeCommand();

	// Obey the command (iCommand).
	TInt ObeyCommandL();

	// Do command tags: expands tags to values.
	TBool DoTags();

	//	IF Management support
	TBool DetermineStepover(CCmdBase *);

private:	// Methods from CActive

	virtual void RunL();
	virtual void DoCancel();

private:	// Enums

	enum TEngineFilePanics
		{
		EBufferPanic,
		EBufferNotAllocated,
		EInvalidFileSizeError
		};

	enum TEngineState
		{
		EIdle				= 0,
		EStopping,
		EGetNextCmd,
		EParseCurrentCmd,
		ERunCurrentCmd
		};

private:    // Data

	// For filename parsing.
	TParse iParse;

	//	for Displaying the command line or CmdLine (non translated version)
	TBool iShowCmdLine;

	// Run control.
	TBool iStopped;							// test run stopped
	TBool iEchoMode;            // echo mode On/Off

	//	need to have a stack of IF structs so that we can process
	//	nested if cases.
	CPile<CIFControl>	*iIFPile;     // cmd file pile
	
	// Overflow handler.
	TTestMachineOverflow iOverflow;

	// State machine state
	TEngineState iState;

	// Active object that gets line input
	CConsoleReader* iConsoleReader;

	// Timer object
	RTimer iTimer;

	//	Timestamp for commands (and logging)
	TTime iTimeStamp;

	// Command family.
	CCmdFamily* iCmdFamily;     // the commands

	// Command buffers:
	TBuf<256> iCmdline;        // command line
	TBuf<1024> iCommand;        // full command

	// Command prompt.
	TBuf<16> iPrompt;           // prompt

	// Console information.
	CConsoleBase	*iConsole;     // console
	TBool iConsoleEnabled;      // enabled?
	TBuf<1024> iPrintBuf;       // formatted...

	// The command files.
	CCmdFile *iCmdFile;     // cmd file now!
	CPile<CCmdFile>	*iCmdPile;     // cmd file pile
	TFileName     iCmdPath;     // cmd file path

	// The log files.
	TFileName     iLogPath;     // log file path
	CLogFile			*iLogFile;     // log file now
	CPile<CLogFile>	*iLogPile;     // log file pile

	// Catalog of the tag and call argument defines.
	// Get the defines.
	#if !defined(TEMPLATED_CATALOGUE)
		CCatalogue *iDefines;

	#else
		CPile<CLabeledText>	*iDefines;
	#endif

	//	need to have access to a generic cat or pile
	//	of (TAny *) which can be cast into pointers
	//	for our domain specific lists (e.g. RHTTPSession)
	//	Object Collection for use as/when by domain tools
	CObjCatalogue *iDomain;

	//	File Management
	//	In some cases access to local files is useful.
	//	as a result, make the services globally accessible from the TestEngine
	RFs iFileServer;
	TFileName iFileName;

	TBool iLogActivity;
};


#endif  // __TENGINE_H__
        
//-----------------------------------------------------------------------------
// End of File
//-----------------------------------------------------------------------------

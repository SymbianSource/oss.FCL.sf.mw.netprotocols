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
// GT0149 Applications Protocol Integration Test Harness
// blank application
// created by: mark j davey
// mjd systems solutions ltd
// mark@mjdss.com
// GT0149 System Info and Defintions
// by mjd
// 
//

#ifndef __GT0149_H__
#define __GT0149_H__

//----------------------------------------------------------------------------
//	Text to welcome a user

_LIT(KTHAWelcome, "Test Harness Application");

//----------------------------------------------------------------------------
//	Version Ident String

_LIT(KTHAVersionIdent, "1.01");

//----------------------------------------------------------------------------
// Defined command line flags
//  -s = script to execute 
//  -c = console required once script executed
//  -d = debug mode on (log to console as well)

_LIT(KControlFlagSeparator, "-");
_LIT(KFileNameExtensionFlag, ".");

//	specify the required script
_LIT(KControlFlagRequiredScript, "-s");

//	if script specified, then provide console once completed
_LIT(KControlFlagRequireConsole, "-c");

//	provide console for debugging purposes (even during script)
_LIT(KControlFlagDebugViaConsole, "-d");

//----------------------------------------------------------------------------
// Other Literals

_LIT(KTxtNewLine,	"\r\n");
_LIT(KTHAWelcomeMessage, "\r\nWelcome to the Integration Test Environment\r\n");

//-----------------------------------------------------------------------------
//  CONSTANTS  

//-----------------------------------------------------------------------------
// New line separator in text output

_LIT( TFR_KTxtNewLine,			"\r\n");

//-----------------------------------------------------------------------------
// Default command prompt

_LIT(THA_CommandPrompt,			"THA:> ");

//----------------------------------------------------------------------------
// Assign characters and separators:

_LIT(KTxtAssign,            "=");
_LIT(KTxtAddition,          "+=");
_LIT(KTxtSetFrom,           ":="); // +TVi
_LIT(KTxtListSeparator,     ",");
_LIT(KTxtCapSeparator,      ":");
_LIT(KTxtValueSeparator,    "&");

//----------------------------------------------------------------------------
// Boolean values:

_LIT(KTxtFalse,             "FALSE");
_LIT(KTxtTrue,              "TRUE");

//----------------------------------------------------------------------------
//	Comparison Operator values:

enum	TComparisonOperators
	{
	TCO_Equals,
	TCO_NotEqual,
	TCO_GreaterThan,
	TCO_LessThan,
	TCO_GreaterThanEq,
	TCO_LessThanEq,
	TCO_MaxComparisonOperators
	};

_LIT(KTxtEquals,						"==");
_LIT(KTxtNotEqual,					"!=");
_LIT(KTxtGreaterThan,				">");
_LIT(KTxtLessThan,					"<");
_LIT(KTxtGreaterThanEq,			">=");
_LIT(KTxtLessThanEq,				"<=");

_LIT(KCompResult,						"Comparison returns %d\r\n");

//----------------------------------------------------------------------------
// Log mode values:

_LIT( KTxtLogALL,           "ALL");
_LIT( KTxtLogNONE,          "NONE");
_LIT( KTxtLogCOMMENTS,      "COMMENTS");
_LIT( KTxtLogCOMMANDS,      "COMMANDS");
_LIT( KTxtLogCASES,         "CASES");

//-----------------------------------------------------------------------------

_LIT(KTxtNone,              "NONE");

//-----------------------------------------------------------------------------
// Framework command phrases

_LIT(THA_TxtCmdRemark,				"#*");
_LIT(THA_TxtCmdComment,				"//*");

_LIT(THA_TxtCmdTrace,         "TRACE");
_LIT(THA_TxtCmdTraceHelp,			"No help available");

_LIT(THA_TxtCmdLog,           "LOG");
_LIT(THA_TxtCmdLogHelp,				"LOG [ALL] [CASES | COMMENTS | COMMANDS]");

_LIT(THA_TxtCmdVersion,				"VER");
_LIT(THA_TxtCmdVersionHelp,		"Lists current THA version ident");

//	this forces a listing of all commands defined...
_LIT(THA_TxtCmdListAll,       "?");
_LIT(THA_TxtCmdListAllHelp,		"Lists all defined commands with help");

_LIT(THA_TxtCmdExit,          "EXIT");
_LIT(THA_TxtCmdQuit,          "QUIT");
_LIT(THA_TxtCmdStop,          "STOP");

_LIT(THA_TxtCmdEchoMode,      "ECHO");

_LIT(THA_TxtCmdPrint,         "PRINT");
_LIT(THA_TxtCmdPrintHelp,			"PRINT directs output to emulator or logfile");

_LIT(THA_TxtCmdCheckMode,     "CHECK");

_LIT(THA_TxtCmdPause,         "PAUSE");

_LIT(THA_TxtCmdHold,          "HOLD");

_LIT(THA_TxtCmdSetPrompt,     "PROMPT");

_LIT(THA_TxtCmdSetPath,       "PATH");

_LIT(THA_TxtCmdCall,          "CALL");
_LIT(THA_TxtCmdCallHelp,			"Execute a script file");

_LIT(THA_TxtCmdReturn,        "RETURN");
_LIT(THA_TxtCmdLogPath,       "LOG PATH");
_LIT(THA_TxtCmdLogFile,       "LOG FILE");
_LIT(THA_TxtCmdEndLog,        "END LOG");

//	to provide the ability to execute scripts from console
//_LIT(THA_TxtCmdScript,        "SCRIPT");
//_LIT(THA_TxtCmdScriptHelp,		"Execute a script directly. Nb setting PATH might be helpful!");

//	variable definition
_LIT(THA_TxtCmdDefine,        "DEFINE");
_LIT(THA_TxtCmdDefineHelp,    "Define variables. DEFINE <variable> or DEFINE (lists all)");

//	new command
_LIT(THA_TxtCmdIf,						"IF");
_LIT(THA_TxtCmdIfS,						"IF*");
_LIT(THA_TxtCmdIfHelp,				"IF <variable OP variable>. Performs a test on operands\r\n");

//	new command (to complement the above IF!)
_LIT(THA_TxtCmdElse,					"ELSE");
_LIT(THA_TxtCmdElseHelp,			"ELSE follows a failed IF evaluation (see $LastResult$)\r\n");

//	new command (to complement the above IF!)
_LIT(THA_TxtCmdEndIf,					"ENDIF");
_LIT(THA_TxtCmdEndIfHelp,			"ENDIF closes an IF block\r\n");

//----------------------------------------------------------------------------
// Miscellaneous [e.g. CASE and SELECT] commands

_LIT(KTxtCase,								"CASE");
_LIT(KTxtEndCase,							"END CASE");
_LIT(KTxtCmdSelect,						"SELECT CASES");

//----------------------------------------------------------------------------
// Framework command identifiers. The identifiers from 0 to the
// TFR_KCmdCustomized - 1 have been reserved for the present and
// future needs of the framework. Please do not overload them!!

enum	THA_FRAMEWORK
	{
	THA_KCmdListAll,
	THA_KCmdRemark,
	THA_KCmdComment,
	THA_KCmdExit,
	THA_KCmdEchoMode,
	THA_KCmdPrint,
	THA_KCmdCheckMode,
	THA_KCmdPause,
	THA_KCmdHold,
	THA_KCmdSetPrompt,
	THA_KCmdSetPath,
	THA_KCmdCall,
	THA_KCmdReturn,
	THA_KCmdLogPath,
	THA_KCmdLogFile,
	THA_KCmdEndLog,
	THA_KCmdDefine,
	THA_KCmdHelp,

	//	these are associated with the IF command - but do not require
	//	result to be recorded
	THA_KCmdElse,
	THA_KCmdEndIf,
	THA_KCmdMaxStandard,

	//	commands below this will have their 'result' recorded
	THA_KCmdIf,
	
	//	add new identifers above list line
	THA_KMaxCmdCustomized
	};

// Maximum number of arguments is a command file call.
// There is no point in defining this one so very big.
#define TFR_MAX_CALL_ARGUMENTS   16

//----------------------------------------------------------------------------
// Error messages
//----------------------------------------------------------------------------

_LIT(KFmtErrInvalidCmd,      "Error: Invalid %S command");
_LIT(KTxtErrNoAddition,      "Error: \"+=\" not allowed");
_LIT(KTxtErrNoSetFrom,       "Error: \":=\" not allowed"); // +TVi
_LIT(KTxtErrValueMissing,    "Error: Value missing");
_LIT(KTxtErrInvalidList,     "Error: Invalid list");
_LIT(KTxtErrInvalidText,     "Error: Invalid text (hyphen missing)");
_LIT(KTxtErrInvalidHexa,     "Error: Invalid hexadecimal format");
_LIT(KTxtErrInvalidBool,     "Error: Invalid value, should be FALSE or TRUE");
_LIT(KTxtErrInvalidBool2,    "Error: Invalid value, should be FALSE, TRUE or NONE");
_LIT(KTxtErrInvalidSessType, "Error: Invalid value, should be CO");
_LIT(KTxtErrInvalidMethod,   "Error: Value should be GET, OPTIONS, HEAD, DELETE, TRACE, POST, PUT or number");
_LIT(KTxtErrAliasAddr,       "Error: Alias addr must be:  [bearer] & [port] & [addr]");
_LIT(KTxtErrItemTooLong,     "Error: Too long item");
_LIT(KTxtErrAddCapFailed,    "Error: Capability adding failed (e.g.code page)");
_LIT(KFmtErrRubbishAtEnd,    "Error: Rubbish at end: %S");
_LIT(KFmtErrNameTooLong,     "Error: Too long name: %S");
_LIT(KFmtErrInvalidName,     "Error: Invalid name: %S");
_LIT(KFmtErrMissing,         "Error: \"%S\" missing");
_LIT(KFmtErrInvalidNbr,      "Error: Invalid number: %S");
_LIT(KFmtErrInvalidBearer,   "Error: Invalid bearer: %S (should be IP, SMS7, SMS, WAPSMS7, WAPSMS or ALL)");
_LIT(KFmtErrInvalidCapName,  "Error: Unknown capability: %S");
_LIT(KFmtErrBulkEnc,         "Error: Invalid bulk encryption algorithm: %S");
_LIT(KFmtErrMac,             "Error: Invalid MAC algorithm: %S");
_LIT(KFmtErrKeyAlgorithm,    "Error: Invalid key exchange algorithm: %S");
_LIT(KFmtErrIdType,          "Error: Invalid id type: %S");
_LIT(KTxtErrSequenceNbrMode, "Error: Invalid value, should be NOTUSED, IMPLICIT, EXPLICIT or NONE");
_LIT(KTxtErrUnknownName,     "Error: Name does not exist");
_LIT(KTxtWarnNameExists,     "Warning: Name already exists");
_LIT(KTxtErrOpenFailed,      "Error: Opening session failed");
_LIT(KFmtErrGetFile,         "Error: Failed to get file: %S"); // +TVi
_LIT(KTxtEmpty,              "");
_LIT(KTxtErrConnectFailed,   "Error: Connect failed");
 

_LIT(THA_KErrInvalidOptr,			"Error: comparison operator [%S] is not recognized or supported\r\n");
_LIT(THA_KErrParameterLong,		"Error: parameter [%S] is of invalid length (<1||>16)\r\n");

// Framework's lexical terms
_LIT( TFR_KTxtTermOn            , "ON" );
_LIT( TFR_KTxtTermOff           , "OFF");
_LIT( TFR_KTxtTermYes           , "YES" );
_LIT( TFR_KTxtTermNo            , "NO");
_LIT( TFR_KTxtTermTrue          , "TRUE" );
_LIT( TFR_KTxtTermFalse         , "FALSE");


// ###
// ### Framework's messages will come come here later
// ###
// ### ...


// Notification i.e. plain messages
_LIT( TFR_KTxtMsgPressAnyKey,	"Press any key");
_LIT( TFR_KTxtMsgRunAborted,	"- R U N  A B O R T E D -\r\n");
_LIT( TFR_KTxtMsgRunStopped,	"- R U N  S T O P P E D -\r\n");
_LIT( TFR_KTxtMsgIsNSU,				" is not supported");
_LIT( TFR_KTxtMsgIsNAV,				" is not available");

// Notification i.e. message formats
_LIT( TFR_KFmtMsgSorryS,			"Sorry: %S");
_LIT( TFR_KFmtMsgSorrySS,			"Sorry: %S %S");
_LIT( TFR_KExecuteScript,			"Executing %S\r\n");

// Error messages
_LIT( TFR_KTxtErrPrintOflo    , "Error: too long text to print out\r\n");
_LIT( TFR_KTxtErrRecursiveTags, "Error: recursive tag definitions?\r\n");
_LIT( TFR_KTxtErrTooLongCmd   , "Error: command is too long to handle\r\n");
_LIT( TFR_KTxtErrUnknownCmd   , "Error: unknown or misspelled command\r\n");

// Error message formats
_LIT( TFR_KFmtErrBadCmd     , "Error: illegal or misspelled %S command\r\n");
_LIT( TFR_KFmtErrFailed     , "Error: %S failed (%d)\r\n");
_LIT( TFR_KFmtErrParams     , "Error: erroneous %S command, bad params\r\n");
_LIT( TFR_KFmtErrMissingParams, "Error: erroneous %S command, missing params\r\n");

_LIT(THA_KFmtErrFileNotFound, "Error: file %S not found\r\n");
_LIT(TFR_KInIFCmdAlready,			"Error: An IF command is already being processed [%S]\r\n");
_LIT(THA_KFmtErrIAPVal,       "Error: [%S] is not a valid IAP value\r\n");

//----------------------------------------------------------------------------
// Dump file names
//----------------------------------------------------------------------------

// extension and default name for dump files
_LIT(KTxtDumpFileExtension, ".dmp");
_LIT(KTxtDumpFileDefault, "unnamed");
_LIT(KTxtDumpFileSeparator, "-");

#endif	//	__GT0149_H__

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//	End of File
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

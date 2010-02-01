// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// WAP Engine Standard 
// Standard status codes used by data handlers and the WAP engine. Note that some are exact duplicates of error
// codes from sub-components (e.g. HTTP) or from components outside of the WAP Browser (e.g. NetDial, ETEL)
// 
//

/**
 @file WapEngStd.h 
 @publishedAll
 @deprecated
*/
 
#ifndef __WAPENGINESTANDARD_H__
#define __WAPENGINESTANDARD_H__

// System includes
//
#include <e32std.h>

//
// Status code offset for HTTP status values and stack connection progress
// e.g. HTTP/200 becomes KHttpPluginStatusBase + 200
//
const TInt KHttpPluginStatusBase = 1000;
const TInt KWapDocumentLoadStatusBase = 2000;

typedef enum TWapEngineStatus
{
	// DRAFT
	// Some of these errors MAY be handled in the control level.

	//
	// HTTP/1.1 Status/Error codes from 40x and 50x series
	//
	EWapErrHttp_505_HTTPVersionNotSupported = -20505,	// has to be 1.1 at the client end; talking to a 1.0 origin server might cause this? Or the client erroneously claiming to do HTTP/3 or something.
	EWapErrHttp_504_GatewayTimeout = -20504,			// not sure if this refers to Gateway in the WAP sense
	EWapErrHttp_503_ServiceUnavailable = -20503,		// ?
	EWapErrHttp_502_BadGateway = -20502,				// could occur with a badly configured WAP GW = e.g. the gateway uses a proxy which cannot be reached = there should probably be some indication to the user.
	EWapErrHttp_501_NotImplemented = -20501,			// duh. Bad server.
	EWapErrHttp_500_InternalServerError = -20500,		// bad news again; this one can either come from the origin server _or_ from the WAP GW, which may convert a different server code into 500 if the WML is bad. Report.

	EWapErrHttp_415_UnsupportedMediaType = -20415,		// see RFC2068
	EWapErrHttp_414_RequestUriTooLong = -20414,			// report to user
	EWapErrHttp_413_RequestEntityTooLarge = -20413,		// see RFC2068
	EWapErrHttp_412_PreconditionFailed = -20412,		// see RFC2068
	EWapErrHttp_411_LengthRequired = -20411,			// error in our use of HTTP if this occurs.
	EWapErrHttp_410_Gone = -20410,						// like 404, except more definite! report to user.
	EWapErrHttp_409_Conflict = -20409,					// see RFC2068
	EWapErrHttp_408_RequestTimeout = -20408,			//  dealt with in HTTP, shouldn't have to be presented to the user
	EWapErrHttp_407_ProxyAuthenticationRequired = -20407,//  - not sure, we don't correctly deal with this at the moment. Is it needed?
	EWapErrHttp_406_NotAcceptable = -20406,				//  this is returned if the resource requested is not acceptable to the client according to its own accept- strings in the HTTP request header. e.g. accept-charset, accept (content types). accept-language. This shows that we're getting our headers wrong, so should be considered a software error and not be reported. I think.
	EWapErrHttp_405_MethodNotAllowed = -20405,			//  e.g. attempt to POST to a URL that is not appropriate for POSTing. Probably a result of poor site design?
	EWapErrHttp_404_NotFound = -20404,					//  document missing on origin server. This one would be a good one to report to the user since it happens frequently with badly maintained sites!
	EWapErrHttp_403_Forbidden = -20403,					// the server HTTPD does refuses to return the requested document, for internal reasons not necessarily disclosed to the client
	EWapErrHttp_402_PaymentRequired = -20402,			//  I don't think this is used in our world of e-commerce. RFC2068 says it is reserved for future use.
//
//														// NOTE THAT HTTP/401 IS NOT TREATED AS AN ERROR, BECAUSE IT IS
//														// FULLY HANDLED IN HTTP - see HTTP status codes below
//
	EWapErrHttp_400_BadRequest = -20400,				// client sent a request with a malformed syntax (in HTTP request headers) = I assume this indicates a programming error on our part ,and should not happen, or be reported?

	//
	// HTTP Data Provider plug-in 'inherited' error codes
	//
	EWapErrHttpReceivingErrorDeck		= -20014,	// Could not find document, so receiving an error deck
	EWapErrHttpCancellationAbort		= -20013,	// Error code for leave when cancellation has occured during RunL()
	EWapErrHttpGatewaySessionDisconnect	= -20012,	// The session was disconnected by the WAP Gateway
	EWapErrHttpGatewayTransactionAbort	= -20011,	// A transaction was aborted by the gateway or the stack
	EWapErrHttpGatewayCannotBeReached	= -20010,	// A session could not be established with the WAP Gateway
	// Note that EWapErrHttpAuthenticationFailed and EWapErrHttpAuthenticationCancelled are now information status codes - see below.
	EWapErrHttpWtlsCipherRejected		= -20009,	// The user rejected the cipher strength negotiated in WTLS
	EWapErrHttpWtlsServerCertRejected	= -20008,	// The user rejected the certificate from the WAP GW
	EWapErrHttpWtlsBadServerCert		= -20007,	// WTLS rejected the certificate obtained from the WAP GW
	EWapErrHttpWtlsConfigFailed			= -20006,	// Failed to configure the WTLS layer of the WAP Stack
	EWapErrHttpNetDialSetupFailed		= -20005,	// Failed to configure Net Dial
	EWapErrHttpWapAPReadFailure			= -20004,	// An error occurred when reading the WAP AP from CommsDB
	EWapErrHttpUnsupportedMethod		= -20003,	// An HTTP method was specified that is not supported
	EWapErrHttpCannotFindPlugin			= -20002,	// Plug-in server failed to create an HTTP plugin
	EWapErrHttpCannotFindServer			= -20001,	// URL specified a non-existent or inaccessible domain
	EWapErrHttpGeneralError				= -20000,	// Unspecified error condition

	//
	//	XMLLIB ERROR CODES
	//

	// XmlLib
#define XmlLibErrorBase -13200
	EWapErrXmlLibInvalidDocument				= XmlLibErrorBase,
	EWapErrXmlLibDocumentBuffered				= XmlLibErrorBase -  1,
	EWapErrXmlLibDocumentAlreadyValid			= XmlLibErrorBase -  2,
	EWapErrXmlLibInvalidDTD						= XmlLibErrorBase -  3,

	// Validator
#define XmlValidatorErrorBase -13100
	EWapErrXmlLibIllegalTagName					= XmlValidatorErrorBase,
	EWapErrXmlLibIllegalFixedAttributeValue		= XmlValidatorErrorBase -  1,
	EWapErrXmlLibMissingRequiredAttribute		= XmlValidatorErrorBase -  2,
	EWapErrXmlLibMissingDocument				= XmlValidatorErrorBase -  4,
	EWapErrXmlLibInvalidDocumentStructure		= XmlValidatorErrorBase -  6,
	EWapErrXmlLibIllegalAttributeValue			= XmlValidatorErrorBase -  7,

	// Parser
#define XmlParserErrorBase -13000
	EWapErrXmlLibMissingCDATASectionEndTag		= XmlParserErrorBase,
	EWapErrXmlLibInvalidAttributeDeclaration	= XmlParserErrorBase -  1,
	EWapErrXmlLibEndTagMismatch					= XmlParserErrorBase -  2,
	EWapErrXmlLibInvalidCharacterReference		= XmlParserErrorBase -  3,
	EWapErrXmlLibUnknownEntityReference			= XmlParserErrorBase -  4,
	EWapErrXmlLibNoDTD							= XmlParserErrorBase -  5,
	EWapErrXmlLibMissingDocumentRootNode		= XmlParserErrorBase -  6,
	EWapErrXmlLibInvalidXmlVersionDefinition	= XmlParserErrorBase -  7,
	EWapErrXmlLibRootElementNameMismatch		= XmlParserErrorBase -  8,


	//
	//	WMLLIB ERROR CODES
	//

	// WmlLib
	// No WmlLib (interface module) specific errors defined

	// Validator
#define WmlValidatorErrorBase -12100
	EWapErrWmlLibAccessViolation			= WmlValidatorErrorBase -   1,
	EWapErrWmlLibInvalidVariableReference	= WmlValidatorErrorBase - 	2,
	EWapErrWmlLibInvalidConversionMethod	= WmlValidatorErrorBase -   3,
	EWapErrWmlLibIllegalVariableName		= WmlValidatorErrorBase -   4,
	EWapErrWmlLibIllegalTaskCountOnAnchor	= WmlValidatorErrorBase -   5,
	EWapErrWmlLibNULLVariableName			= WmlValidatorErrorBase -   6,
	EWapErrWmlLibNestedTables               = WmlValidatorErrorBase -   7,
	EWapErrWmlLibDuplicateDoNodeName        = WmlValidatorErrorBase -   8,
	EWapErrWmlLibUsageOfReservedWord        = WmlValidatorErrorBase -   9,
	EWapErrWmlLibZeroTableColumns           = WmlValidatorErrorBase -  10,
	EWapErrWmlLibNonWmlDocument				= WmlValidatorErrorBase -  11,
	EWapErrWmlLibMultipleMetaPropertyName   = WmlValidatorErrorBase -  12,
	EWapErrWmlLibEventBindingConflict       = WmlValidatorErrorBase -  14,
	EWapErrWmlLibMultipleAccessElements		= WmlValidatorErrorBase -  15,
	EWapErrWmlLibNonUniqueIds				= WmlValidatorErrorBase -  16,
	EWapErrWmlLibVariableInInvalidLocation	= WmlValidatorErrorBase -  17,

	// Parser
#define WmlParserErrorBase -12000
	EWapErrWmlLibNullNode					= WmlParserErrorBase,
	EWapErrWmlLibCDATANodeWithoutParent		= WmlParserErrorBase -   1,


	// 
	// WAP Engine 'native' error codes
	//
	EWapErrSubDocumentNotFound	= -10027,
	EWapErrCannotCreateDeck		= -10026,			// Failed to create a deck
	EWapErrMimeTypeMissing		= -10025,			// Engine tried to access the mime-type attribute in a
													// tree node but it was missing
	EWapErrUnknownDocument		= -10024,
	EWapErrInvalidDTD			= -10023,
	EWapErrDTDUnavailable		= -10022,			// Document cannot be validated as DTD is not available

	EWapErrUnknownScheme = -10021,					// unknown scheme
	EWapErrCorruptScheme = -10020,					// scheme contains invalid characters
	EWapErrCorruptUrl	 = -10019,					// part of url contains invalid characters

	EWapErrPluginNotFound = -10018,					// couldn't find the requested plugin

	EWapErrImageConversionFailed = -10017,			// image data handler got error from media server

	EWapErrAccessPointNotSetUpForCSD	=  -10016,	// Access point lacked info for Data call
	EWapErrAccessPointNotSetUpForSMS	=  -10015,
	EWapErrAccessPointNotSetUpForUSSD	=  -10014,
	EWapErrAccessPointNotSetUpGeneric	=  -10013,
	EWapErrProtocolNotSupported			=  -10012,	// Current network does not support the requested service

	EWapErrAuthorizationFailed			= -10011,
		
	EWapErrScriptObscureLibraryCall	= -10010,		// The script function was unavailable
	EWapErrScriptIllegalAction		= -10009,		// The script attempted an illegal action and was terminated
	EWapErrScriptCorrupt			= -10008,		// Script had errors
	EWapErrScriptError				= -10007,		// Script returned a fatal error
	EWapErrScriptNotFound			= -10006,		// Requested script was not found
		
	EWapErrDataCallUnavailable	= -10005,			// The data call is being used by another application
	EWapErrDataCallDropped		= -10004,			// CSD connection was closed 

	EWapErrAccessViolation		= -10003,			// Access to the document was denied
	EWapErrDocumentCorrupted	= -10002,
	EWapErrDocumentNotFound		= -10001,
	EWapErrGeneral				= -10000,

	//
	// PPP connection 'inherited' error codes
	//
	EWapErrPppIfLRDBadLine			   = -3059,	// ??
	EWapErrPppIfDNSNotFound			= -3058,	// ??
	EWapErrPppIfCallbackNotAcceptable = -3057,	// NTRAS: client requested server Callback but was refused
	EWapErrPppIfChangingPassword      = -3056,	// NTRAS: User's password is currently being changed
	EWapErrPppIfNoDialInPermission    = -3055,	// NTRAS: User has not got dial-in permission on the server
	EWapErrPppIfPasswdExpired         = -3054,	// NTRAS: User's password has expired
	EWapErrPppIfRestrictedLogonHours  = -3053,	// NTRAS: User is attempting to log-in outside their hours
	EWapErrPppIfAccountDisabled       = -3052,	// NTRAS: User's account has been disabled
	EWapErrPppIfAuthNotSecure         = -3051,	// ??
	EWapErrPppIfAuthenticationFailure = -3050,	// The supplied password was wrong??

	//
	// NetDial 'inherited' error codes
	//
	EWapErrNetDialScriptError	= -3005,	// NetDial Exit Condition - Script Error
	EWapErrNetDialScriptTimeOut	= -3004,	// NetDial Exit Condition - Script Timed Out
	EWapErrNetDialLoginFail		= -3003,	// NetDial Exit Condition - Login Failure
	EWapErrNetDialModemError	= -3002,	// NetDial Exit Condition - Modem error
	EWapErrNetDialNoModem		= -3001,	// NetDial Exit Condition - No Modem Response

	//
	// ETEL 'inherited' error codes
	//
	EWapErrEtelModemSettingsCorrupt		= -2018,
	EWapErrEtelModemNotDetected			= -2017,
	EWapErrEtelAnswerAlreadyOutstanding	= -2016,
	EWapErrEtelUnknownModemCapability		= -2015,
	EWapErrEtelWrongModemType				= -2014,
	EWapErrEtelPortNotLoanedToClient		= -2013,
	EWapErrEtelNotFaxOwner					= -2012,
	EWapErrEtelFaxChunkNotCreated			= -2011,
	EWapErrEtelConfigPortFailure			= -2010,
	EWapErrEtelNoDialTone					= -2009,
	EWapErrEtelNoAnswer					= -2008,
	EWapErrEtelCallNotActive				= -2007,
	EWapErrEtelInitialisationFailure		= -2006,
	EWapErrEtelNoClientInterestedInThisCall= -2005,
	EWapErrEtelBusyDetected				= -2004,
	EWapErrEtelNoCarrier					= -2003,
	EWapErrEtelAlreadyCallOwner			= -2002,
	EWapErrEtelDuplicatePhoneName			= -2001,
	EWapErrEtelNotCallOwner				= -2000,

	//
	// General EPOC 'inherited' error codes
	//
	EWapErrEpocTimedOut						= -33,
	EWapErrEpocNotFound						= -1,
	EWapErrEpocAccessDenied						= -21,

	// 
	// 
	// Wap engine status messages
	//
	EWapStatusDownloadProgress = 1,		// Amount complete in aValue (0...100)
	EWapStatusDownloadComplete,			// Document fetch is complete
	EWapStatusSubDocumentChanged,		// XML subdocument (card in WML) has changed 
	EWapStatusRefreshRequest,			// Observer is requested to refresh display
	EWapStatusError,					// Error
	EWapStatusInformational,			// Information other than download progress
	EWapStatusInitialisingConnection	= KHttpPluginStatusBase,
	EWapStatusConnecting				= KHttpPluginStatusBase + 1,
	EWapStatusAuthenticating			= KHttpPluginStatusBase + 2,
	EWapStatusConnected					= KHttpPluginStatusBase + 3,
	EWapStatusDisconnecting				= KHttpPluginStatusBase + 4,
	EWapStatusHttpAuthenticationFailed	= KHttpPluginStatusBase + 10,		// The user's password was rejected by the origin server
	EWapStatusHttpAuthenticationAborted	= KHttpPluginStatusBase + 11,		// The user cancelled the password box
	EWapStatusHttp_401_Unauthorized		= KHttpPluginStatusBase + 401,		// occurs when HTTP authentication is used, username/password dialogs raised as a response. Need to consider how UI deals with authentication failures here.
	EWapStatusValidating				= KWapDocumentLoadStatusBase,		// Validating document
	EWapStatusReceivingErrorDeck		= KWapDocumentLoadStatusBase + 1	// Receiving error deck

} TWapEngineStatus;

enum TWmlFetchFlag
	{
	EWmlFetchNoFlag		= 0x00,
	EWmlFetchReload		= 0x01,
	EWmlFetchTimerFetch	= 0x02
	};

#endif // __WAPENGINESTANDARD_H__

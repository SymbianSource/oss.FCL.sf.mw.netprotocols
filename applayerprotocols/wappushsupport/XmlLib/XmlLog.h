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
// Purpose:		Defines logging marcos for XmlLib component.
// If logging is required in the Xml parser, the appropriate macros from LogDef.h are used. Otherwise, 
// empty marcos are defined.
// 
//

#ifndef __XMLLOG_H_
#define __XMLLOG_H_

//#define __XML_LOGGING

#if !defined (__WAPLOG_H__)
	#include "waplog.h"
#endif

#if !defined (__LOGDEF_H__)
	#include "logdef.h"		// Provides logging capabilities
#endif

#if defined (__XML_LOGGING)
	// Non-empty macros
	#define __XML_DECLARE_LOG				__DECLARE_LOG
	#define __XML_OPEN_LOG(aLogName)		__OPEN_LOG(aLogName)
	#define __XML_OPEN_CLEANLOG(aLogName)	__OPEN_CLEANLOG(aLogName)
	#define __XML_CLOSE_LOG					__CLOSE_LOG
	#define __XML_PUSHLOGL					__PUSHLOGL
	#define __XML_POPLOG					__POPLOG
	#define __XML_LOG_ENTER(aText)			__LOG_ENTER(aText)
	#define __XML_LOG_RETURN				__LOG_RETURN 
	#define __XML_LOG(aText)				__LOG(aText)
	#define __XML_LOG1(aText,aV1)			__LOG1(aText,aV1)
	#define __XML_LOGX(aLevel,aText)		__LOGX(aLevel,aText)
	#define __XML_LOGX1(aLevel,aText,aV1)	__LOGX1(aLevel,aText,aV1)
	#define __XML_PROFILELOG(aText)			__PROFILELOG(aText)
	#define __XML_PROFILELOG1(aText,aV1)	__PROFILELOG1(aText,aV1)
	#define __XML_TRACELOG(aText)			__TRACELOG(aText)
	#define __XML_TRACELOG1(aText,aV1)		__TRACELOG1(aText,aV1)
	#define __XML_QINFO(aText)				__QINFO(aText)
	#define __XML_ENABLE_LOGRDEBUG()		__ENABLE_LOGRDEBUG()
	#define __XML_DISABLE_LOGRDEBUG()		__DISABLE_LOGRDEBUG()

#else
	// Empty macros for non-logging builds
	#define __XML_DECLARE_LOG
	#define __XML_OPEN_LOG(aLogName)
	#define __XML_OPEN_CLEANLOG(aLogName)
	#define __XML_CLOSE_LOG
	#define __XML_PUSHLOGL
	#define __XML_POPLOG
	#define __XML_LOG_ENTER(aText)
	#define __XML_LOG_RETURN 
	#define __XML_LOG(aText)
	#define __XML_LOG1(aText,aV1)
	#define __XML_LOGX(aLevel,aText)
	#define __XML_LOGX1(aLevel,aText,aV1)
	#define __XML_PROFILELOG(aText)
	#define __XML_PROFILELOG1(aText,aV1)
	#define __XML_TRACELOG(aText)
	#define __XML_TRACELOG1(aText,aV1)
	#define __XML_QINFO(aText)
	#define __XML_ENABLE_LOGRDEBUG()
	#define __XML_DISABLE_LOGRDEBUG()
#endif

#if defined (__LOGGING) && 	defined (_DEBUG) && defined (__EXTENDED_LOGGING_XML) && defined (__XML_LOGGING)
	#define __XML_EXT_LOG_ENTER(aText)		{CXmlParser& parser = REINTERPRET_CAST(CXmlParser&, aParser);if(parser.iLogPtr != NULL) parser.iLogPtr->LogEnter(aText,CLogClient::ELogProfile); }
	#define __XML_EXT_LOG_RETURN				{CXmlParser& parser = REINTERPRET_CAST(CXmlParser&, aParser);if(parser.iLogPtr != NULL) parser.iLogPtr->LogReturn(); }
	#define __XML_EXT_LOG1(aText,aV1)		{CXmlParser& parser = REINTERPRET_CAST(CXmlParser&, aParser);if(parser.iLogPtr != NULL) parser.iLogPtr->Log(CLogClient::ELogLight,aText,aV1);}
#else
	#define __XML_EXT_LOG_ENTER(aText)		
	#define __XML_EXT_LOG_RETURN				
	#define __XML_EXT_LOG1(aText,aV1)		
#endif

#endif	// __XMLLOG_H_

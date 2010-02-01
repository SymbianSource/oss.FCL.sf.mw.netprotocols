// Copyright (c) 2000-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "curl.h"
#include <escapeutils.h>


EXPORT_C void CUrl::Stub7()
	{
	}
	
EXPORT_C void CUrl::Stub8()
	{
	}
	
EXPORT_C void CUrl::Stub9()
	{
	}
	
EXPORT_C void CUrl::Stub10()
	{
	}
	
EXPORT_C void CUrl::Stub11()
	{
	}
	
EXPORT_C void CUrl::Stub12()
	{
	}
	
EXPORT_C void CUrl::Stub13()
	{
	}
	
EXPORT_C void CUrl::Stub14()
	{
	}
	
EXPORT_C void CUrl::Stub15()
	{
	}
	
EXPORT_C void CUrl::Stub16()
	{
	}
	
EXPORT_C void CUrl::Stub18()
	{
	}
	
EXPORT_C void CUrl::Stub19()
	{
	}
	
EXPORT_C void CUrl::Stub20()
	{
	}
		
EXPORT_C void CUrl::Stub21()
	{
	}


EXPORT_C HBufC* CUrl::EscapeDecodeL(const TDesC& aString)
//
// Decodes any escape triples in aString back into excluded characters
	{
	return EscapeUtils::EscapeDecodeL(aString);
	}

EXPORT_C HBufC* CUrl::EscapeEncodeL(const TDesC& aString, TInt aEscapeMode)
//
// Encodes any excluded characters in aString as escape triples - excluded characters set by aEscapeMode
	{
	// Generic Compare is first 13 Chars ending in \"
	// Path is first 21 chars ending in ,
	// Scheme is full 23 chars 
	_LIT(KReservedChars,"{}|\\^[]\'<>#%\";?@&=+$,/:");
	TInt numChars = 13;
	
	switch (aEscapeMode)
		{
	case EUrlGenericCompare:
		break;
		
	case EUrlScheme:
		numChars = 23;
		break;	
	case EUrlPath:
		numChars = 21;
		break;	
	default:
		return NULL;
		}
	
	return EscapeUtils::EscapeEncodeL(aString,KReservedChars().Left(numChars));	
	}

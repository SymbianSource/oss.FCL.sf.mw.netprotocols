// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#if !defined(__WBENUM_H__)
#define __WBENUM_H__

enum TWbxmlPanic 
{
	EWbxmlPanicCantHandleElement,	// A rendering plugin received a node of type (tag) that it cannot handle
	EWbxmlPanicPluginLoadFailed,
	EWbxmlPanicInvalidUrl
};
GLREF_C void Panic (TWbxmlPanic aPanic);

enum TWbxmlLeave
{
	EWbxmlNoSource = 1,
	EWbxmlUnknownXml,
	EWbxmlUnknownPublicId,
	EWbxmlUnsupportedPublicId,
	EWbxmlTokenTags,
	EWbxmlTokenAttributes
};

#endif    // __WBENUM_H__

//
// End of file
//

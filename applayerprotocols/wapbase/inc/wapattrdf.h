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
// WAP Attribute Std
// Standard attribute keys
// 
//

/**
 @file WAPATTRDF.h 
 @publishedAll
 @deprecated
*/
 
#ifndef __WAPATTRIBUTESTD_H
#define __WAPATTRIBUTESTD_H

// Document node
//
_LIT(KWAPNodeMimeTypeAttributeName,"KWapAttrNodeMimeType");							// node mime type
_LIT(KWAPNodeContentEncodingTypeAttributeName,"KWapAttrNodeContentEncodingType");	// node content encoding type
_LIT(KWAPDocumentNodeURLAttributeName,"KWapAttrNodeURL");							// URL of node


// Data Storage node
//
_LIT(KWAPNodeErrorCodeAttributeName,"KWapAttrNodeErrorCode"); // node error code

// Image nodes
//
_LIT(KWAPBitmapHandleAttributeName,"KWapAttrImgBmpHnd"); // standard bitmap handle
_LIT(KWAPZoomedBitmapHandleAttributeName,"KWapAttrImgZoomBmpHnd"); // zoomed bitmap handle
_LIT(KWAPZoomedBitmapZoomFactorAttributeName,"KWapAttrImgZoomFactor"); // zoomed bitmap zoom factor

// Document root node
//
_LIT(KWAPDocumentValidStatusAttributeName,"KWapAttrDocValid"); // set if the doucment has been validated
_LIT(KWAPDocumentCompletedAttributeName,"KWapAttrDocCompleted"); // set if the doucment is complete
_LIT(KWAPDocumentWmlVersion,"KWapAAttrWmlVersion"); //1.1 or 1.2

// Script Node
_LIT(KWAPScriptReferingCardUrl, "KWapAttrReferingUrl"); // URL to the card that refered to this script


#endif // __WAPATTRIBUTESTD_H

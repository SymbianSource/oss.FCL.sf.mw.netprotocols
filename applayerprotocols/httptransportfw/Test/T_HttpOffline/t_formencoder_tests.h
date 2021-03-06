// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// This header file contains all literal strings used as test cases in t_formencoder
// 
//


_LIT8(KName, "Name"); const TInt KNameEncodedLength = 4;
_LIT8(KValue1, "value"); const TInt KValue1EncodedLength = 5;
_LIT8(KValue2, "space afterspace"); const TInt KValue2EncodedLength = 16;
_LIT8(KValue3, "<value>"); const TInt KValue3EncodedLength = 11;
_LIT8(KValue4, "<value>\rvalue"); const TInt KValue4EncodedLength = 22 ;
_LIT8(KValue5, "<value>\r\rvalue"); const TInt KValue5EncodedLength = 28;
_LIT8(KValue6, "<value> \r\r value=?"); const TInt KValue6EncodedLength = 36;
_LIT8(KValue7, "?@!\""); const TInt KValue7EncodedLength = 12;
_LIT8(KValue8, "= \r");  const TInt KValue8EncodedLength = 10;
_LIT8(KValue9, "????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????");  const TInt KValue9EncodedLength = 600;
_LIT8(KValue10, "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"); const TInt KValue10EncodedLength = 300;

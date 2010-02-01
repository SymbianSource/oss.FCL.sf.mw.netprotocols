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
//



#ifndef __TESTSERVERS_H__
#define __TESTSERVERS_H__
/** This file contains definitions for test servers used for acceptance testing for http 



*/



#define __HTTP_SERVER_IP	_LIT8(KHttpTestServer, "http://10.29.23.4"); 
#define __HTTP_SERVER_NTRAS _LIT8(KHttpTestServer, "http://10.29.23.12");
#define __HTTP_SERVER_DIALIN _LIT8(KHttpTestServer, "http://10.29.23.12");

/** Following are virtual hosts


*/




//#define __HTTP_TESTSERVER(_leaf)	__HTTP_SERVER_DIALIN _LIT8(KUrlEnd, _leaf); TBuf8<256> uri(KHttpTestServer); uri.Append(KUrlEnd);

//#define __HTTP_TESTSERVER(_leaf)	__HTTP_SERVER_NTRAS _LIT8(KUrlEnd, _leaf); TBuf8<256> uri(KHttpTestServer); uri.Append(KUrlEnd);

#define __HTTP_TESTSERVER(_leaf)	__HTTP_SERVER_IP _LIT8(KUrlEnd, _leaf); TBuf8<256> uri(KHttpTestServer); uri.Append(KUrlEnd);


#endif // __TESTSERVERS_H__

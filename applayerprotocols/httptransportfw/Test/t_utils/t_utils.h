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
// RHttpUtils contains utilities such as a global comms init function for starting c32 
// and loading device drives for test code
// 
//


# ifndef	__T_UTILS_H__
# define	__T_UTILS_H__

class RHttpTestUtils
	{
public:
	static void InitCommsL(); 
	};

#endif  //	__T_UTILS_H__

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

#ifndef __MCONSOLEDUMPER_H__
#define __MCONSOLEDUMPER_H__

// System includes
#include <e32std.h>


/** Definition of interface 'MConsoleDumper'.
*/

class MConsoleDumper
	{
public: // methods

	/** 
	*/
	virtual void StartDump() = 0;

	/** 
	*/
	virtual void WriteData(const TDesC16& aData) = 0;

	/** 
	*/
	virtual void WriteData(const TDesC8& aData) = 0;

	/** 
	*/
	virtual void DumpData(const TDesC8& aData) = 0;

	/** 
	*/
	virtual void EndDump() = 0;
	};


#endif // __MCONSOLEDUMPER_H__
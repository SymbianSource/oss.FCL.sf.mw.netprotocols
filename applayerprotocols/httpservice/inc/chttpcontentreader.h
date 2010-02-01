// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __CHTTPCONTENTREADER_H__
#define __CHTTPCONTENTREADER_H__

#include <e32base.h>
#include <f32file.h>
class MHttpDataSender;

class CHttpFileReader : public CActive
	{
	public:
	static CHttpFileReader* New(RFile aFile, MHttpDataSender& aReceiver);
	~CHttpFileReader();
	
	void Read();
	TInt Size() const
        {  
        return iTotalSize;
        }
	private:
	// From CActive
	
	void RunL();
	void DoCancel();
	TInt RunError(TInt aError);
	
	
	CHttpFileReader(RFile aFile, MHttpDataSender& aReceiver);
	TInt Construct();
	private:
	RBuf8				iContentBuffer;
	RFile 				iFile;
	MHttpDataSender& 	iDataSender;		
	TInt                iReadBytes;
	TInt                iTotalSize;
	};


	
#endif // __CHTTPCONTENTREADER_H__

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

#ifndef __CHTTPCONTENTWRITER_H__
#define __CHTTPCONTENTWRITER_H__

#include <e32base.h>
#include <f32file.h>
class MHttpDataReceiver;

class CHttpFileWriter : public CActive
	{
	public:
	static CHttpFileWriter* New(RFile aFile, MHttpDataReceiver& aReceiver);
	~CHttpFileWriter();
	
	void Write(const TDesC8& aData);

	private:
	// From CActive
	
	void RunL();
	void DoCancel();
	TInt RunError(TInt aError);
		
	CHttpFileWriter(RFile aFile, MHttpDataReceiver& aReceiver);
	
	private:
	RFile 							iFile;
	MHttpDataReceiver& 	iDataReceiver;		
	};


	
#endif // __CHTTPCONTENTREADER_H__

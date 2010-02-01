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

#ifndef UNZIP_H
#define UNZIP_H

#include <e32base.h>

class RFs;
class CZipFile;
class CZipFileMember;

class CUnzip : public CBase
	{
public:
	IMPORT_C static CUnzip* NewLC(RFs& aFs, const TDesC& aFileName);
	virtual ~CUnzip();
	
	IMPORT_C void ExtractL(const TDesC& aOutDir);
private:
	CUnzip(RFs& aFs, const TDesC& aFileName);
	void ExtractFileL(const CZipFileMember* aMember, const TDesC& aOutDir);
	TFileName iZipFile;
	RFs& iFs;
	CZipFile* iZip; 
	};

	
#endif // UNZIP_H

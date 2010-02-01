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

#include "unzip.h"
#include <zipfile.h>

CUnzip::CUnzip(RFs& aFs, const TDesC& aFileName) : iZipFile(aFileName), iFs(aFs) 
	{
	}

CUnzip::~CUnzip()
	{
	delete iZip;
	}

EXPORT_C CUnzip* CUnzip::NewLC(RFs& aFs, const TDesC& aFileName)
	{
	CUnzip* self = new(ELeave) CUnzip(aFs, aFileName);
	CleanupStack::PushL(self);
	self->iZip = CZipFile::NewL(self->iFs, self->iZipFile);
	return self;
	}


EXPORT_C void CUnzip::ExtractL(const TDesC& aOutDir)
	{
	CZipFileMemberIterator*	members = iZip->GetMembersL();
	CleanupStack::PushL(members);
	
	CZipFileMember* member = members->NextL();
	while (member)
		{
		CleanupStack::PushL(member);
		ExtractFileL(member, aOutDir);
		CleanupStack::PopAndDestroy(member); 
		member = members->NextL();
		}

	CleanupStack::PopAndDestroy(members); 
	}


void CUnzip::ExtractFileL(const CZipFileMember* aMember, const TDesC& aOutDir)
	{
	HBufC* name = aMember->Name()->AllocLC();
	for (TInt i = 0; i<name->Length(); i++)
		{
		if ((*name)[i] == '/')
			{
			name->Des()[i] = '\\';
			}
		}
	
	TFileName outFile;
	outFile.Append(aOutDir);
	outFile.Append('\\');
	outFile.Append(*name);
	
	RFile out;
	CleanupClosePushL(out);

	iFs.MkDirAll(outFile);
	out.Replace(iFs, outFile, EFileShareAny|EFileWrite);

	RZipFileMemberReaderStream* fileStream;
	TInt error = iZip->GetInputStreamL(aMember, fileStream);
	if (error != KErrNone)
		{
		iFs.Delete(outFile);
		User::Leave(error);
		}
	CleanupStack::PushL(fileStream);

	TUint32 size = aMember->UncompressedSize();
	HBufC8* bytes = HBufC8::New(size);
	CleanupStack::PushL(bytes);
	TPtr8 ptr = bytes->Des();
	User::LeaveIfError(fileStream->Read(ptr,size));
	User::LeaveIfError(out.Write(ptr));
	CleanupStack::PopAndDestroy(4,name); // bytes, fileStream, out and name
	}

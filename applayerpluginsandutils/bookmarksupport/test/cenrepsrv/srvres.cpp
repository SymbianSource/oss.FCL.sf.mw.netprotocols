// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "srvres.h"
#include "cachemgr.h"

RFs TServerResources::iFs;

HBufC* TServerResources::iRomDirectory;
HBufC* TServerResources::iDataDirectory;
HBufC* TServerResources::iInstallDirectory;
HBufC* TServerResources::iBURDirectory;


HBufC* TServerResources::iIniExt;
HBufC* TServerResources::iCreExt;
HBufC* TServerResources::iTrnsExt;

TUint8 TServerResources::iPersistsVersion;

RArray<TOwnerIdMapping> TServerResources::iOwnerIdLookUpTable;

CRepositoryCacheManager* TServerResources::iCacheManager;
TInt TServerResources::nPushed;
#ifdef __CENTREP_SERVER_MEMTEST__
TInt32 TServerResources::iMemTestData[KMemBufMaxEntry];
TInt32 TServerResources::iMemTestDataCount = 0;
#endif //__CENTREP_SERVER_MEMTEST__

TTime TServerResources::CentrepFileTimeStampL(TUid aUid, TCentRepLocation aLocation)
	{
	TEntry entry;
	HBufC* fileName(NULL);
	TServerResources::CreateRepositoryFileNameLC(fileName,aUid,aLocation,ECre);
	TInt err=TServerResources::iFs.Entry(fileName->Des(), entry);
   	CleanupStack::PopAndDestroy(fileName);

	if(err==KErrNone)
		{
		return entry.iModified;
		}
		
	TServerResources::CreateRepositoryFileNameLC(fileName,aUid,aLocation,EIni);
	User::LeaveIfError(TServerResources::iFs.Entry(fileName->Des(), entry));
   	CleanupStack::PopAndDestroy(fileName);
   			   					  
	return entry.iModified;
	}
	
TBool TServerResources::CentrepFileExistsL(TUid aUid, TCentRepLocation aLocation, TCentRepFileType aType)
	{
	HBufC* fileName(NULL);
	TServerResources::CreateRepositoryFileNameLC(fileName,aUid,aLocation,aType);
	TEntry entry;
	TInt err=TServerResources::iFs.Entry(fileName->Des(), entry);
   	CleanupStack::PopAndDestroy(fileName);		   					  
 	
 	TBool r=EFalse;
 	
 	if(err==KErrNone)
 		{
 		r=ETrue;
 		}
 	else if(err==KErrNotFound)
 		{
 		r=EFalse;
 		}
 	// Looking for a file on a composite file system may return KErrPathNotFound when 
 	// the ROM file doesn't exist, so check for this return code as well.
 	else if((aType==ERom) && (err== KErrPathNotFound))
  		{
 		r=EFalse;
 		}
 	else
 		{
 		User::Leave(err);
 		}
	
  	return r;	 
	}

TBool TServerResources::CentrepFileExistsL(TUid aUid, TCentRepLocation aLocation)
	{
	return( CentrepFileExistsL( aUid, aLocation, EIni) || CentrepFileExistsL( aUid, aLocation, ECre));
	}
	
TBool TServerResources::InstallFileExistsL(TUid aUid)
	{
	return CentrepFileExistsL(aUid, EInstall);
	}

TBool TServerResources::RomFileExistsL(TUid aUid)
	{
	return CentrepFileExistsL(aUid, ERom);
	}
	
TBool TServerResources::PersistsFileExistsL(TUid aUid)
	{
	return CentrepFileExistsL(aUid, EPersists);
	}
		
void TServerResources::DeletePersistsFileL(TUid aUid)
	{
	HBufC* iniFileName(NULL);
	HBufC* creFileName(NULL);
	TServerResources::CreateRepositoryFileNameLC(iniFileName,aUid,EPersists,EIni);
	TServerResources::CreateRepositoryFileNameLC(creFileName,aUid,EPersists,ECre);
	
	if(CentrepFileExistsL( aUid, EPersists, EIni))	
		{
		User::LeaveIfError(TServerResources::iFs.Delete(iniFileName->Des()));
		}
	if(CentrepFileExistsL( aUid, EPersists, ECre))	
		{
		User::LeaveIfError(TServerResources::iFs.Delete(creFileName->Des()));
		}

	CleanupStack::PopAndDestroy(creFileName);	 
	CleanupStack::PopAndDestroy(iniFileName);	 
	}	
		   					  

/**
Generic routine for creating a full repository file name.
aFullFileName is created on the heap and it is caller responsibility
to delete it. 
*/
void TServerResources::CreateRepositoryFileNameLC(HBufC*& aFullFileName,
												  TUid aUid,
                                                  TCentRepLocation aLocation,
                                                  TCentRepFileType aFileType)
    {
    const TInt KExtLen = 4;
    const TInt KDirLen = 40;
    const TInt KUidLen = 8;

    TBuf<KDirLen> directory;
    TBuf<KExtLen> ext;

	//path
	switch (aLocation)
	    {
	    case EPersists:
	        {
	        directory.Copy(iDataDirectory->Des());
	        }
	    	break;
	    case EInstall:
	        {
	        directory.Copy(iInstallDirectory->Des());
	        }
	    	break;
	    case ERom:
	        {
	        directory.Copy(iRomDirectory->Des());
	        }
	    	break;
	    default:
	        User::Leave(KErrNotFound); //should never get here
	    }
	    
    //file name	    
	TBuf<KUidLen> name;
	name.NumFixedWidth(aUid.iUid, EHex, KUidLen);

	//extension
    switch (aFileType)
        {
        case ECre:
            {
            ext.Copy(iCreExt->Des());
            }
            break;
        case EIni:
            {
            ext.Copy(iIniExt->Des());
            }
            break;
        case ETmp:
	    	{
	    	ext.Copy(iTrnsExt->Des());
	    	}
	    	break;
        default:
	        User::Leave(KErrNotFound); //should never get here
        }

    TBuf<KMaxFileName>  fullFileName;
    fullFileName.Append(directory);
    fullFileName.Append(name);
    fullFileName.Append(ext);
    //allocates memory on the heap. It is caller's resposibility to delete aFullFileName
    aFullFileName = fullFileName.AllocLC(); 
    }

void TServerResources::InitialiseL()
	{
	iPersistsVersion = KPersistFormatVersion;							// Version 0 of persists
	User::LeaveIfError(iFs.Connect());
	CleanupClosePushL(iFs);
	nPushed = 1;

	_LIT(KWriteableDrive, "c:"); 	// This will probably always be c:
	_LIT(KRomDrive, "z:");			// This may not always be z:
	_LIT(KPersistsDir, "persists\\");
	_LIT(KBURDir, "bur\\");
	
	// File extensions
	
	_LIT(KIniFileExtension, ".txt");
	_LIT(KExternalizedPersistsFileExt, ".cre");
	_LIT(KTransactFileExt, ".tmp");

	const TInt KMaxExtLength=4;
	
	iIniExt=HBufC::NewLC(KMaxExtLength);
	++nPushed;
	iCreExt=HBufC::NewLC(KMaxExtLength);
	++nPushed;
	iTrnsExt=HBufC::NewLC(KMaxExtLength);
	++nPushed;

	iIniExt->Des().Copy(KIniFileExtension);
	iCreExt->Des().Copy(KExternalizedPersistsFileExt);
	iTrnsExt->Des().Copy(KTransactFileExt);
	
	TBuf<KMaxFileName> path;
	User::LeaveIfError(iFs.PrivatePath(path));

	const TInt pathLen = path.Length();
	
	//
	// ROM-drive cenrep directory
	//
	
	iRomDirectory = HBufC::NewLC(KRomDrive().Length()+pathLen);
	++nPushed;
	TPtr ptr(iRomDirectory->Des());
	ptr.Append(KRomDrive);
	ptr.Append(path);
	// If the ROM directory does not exist (very unlikely) we set iRomDirectory to zero.
	TEntry fsEntry;
	if(iFs.Entry(*iRomDirectory, fsEntry)!=KErrNone || !fsEntry.IsDir())
		{
		CleanupStack::PopAndDestroy(iRomDirectory);
		--nPushed;
		iRomDirectory = 0;
		}

	//
	// Cenrep install directory
	//

	iInstallDirectory = HBufC::NewLC(KWriteableDrive().Length()+pathLen);
	++nPushed;
	ptr.Set(iInstallDirectory->Des());
	ptr.Append(KWriteableDrive);
	ptr.Append(path);
	TInt r = iFs.MkDirAll(*iInstallDirectory);
	if(r!=KErrNone && r!=KErrAlreadyExists)
		{
		User::Leave(r);		
		}
	//
	// Writeable-drive data directory
	//

	iDataDirectory = HBufC::NewLC(KWriteableDrive().Length()+pathLen+KPersistsDir().Length());
	++nPushed;
	
	ptr.Set(iDataDirectory->Des());
	ptr.Append(KWriteableDrive);
	ptr.Append(path);
	ptr.Append(KPersistsDir);
	
	r = iFs.MkDirAll(*iDataDirectory);
	if(r!=KErrNone && r!=KErrAlreadyExists)
		User::Leave(r);

	//
	// Writeable-drive backup/restore directory
	//
	iBURDirectory = HBufC::NewLC(KWriteableDrive().Length()+pathLen+KBURDir().Length());
	++nPushed;
	
	ptr.Set(iBURDirectory->Des());
	ptr.Append(KWriteableDrive);
	ptr.Append(path);
	ptr.Append(KBURDir);
	
	r = iFs.MkDirAll(*iBURDirectory);
	if(r!=KErrNone && r!=KErrAlreadyExists)
		User::Leave(r);
	
	//
	// Cache Manager 
	//
	iCacheManager = CRepositoryCacheManager::NewLC();
	++nPushed;

	CleanupStack::Pop(nPushed); // i*Ext, iInstallDirectory, iDataDirectory, (iRomDirectory?), iFs, iCacheManager
	
	iOwnerIdLookUpTable.Reset();
	}

void TServerResources::Close()
	{
	delete iCacheManager;
	delete iInstallDirectory;
	delete iDataDirectory;
	delete iRomDirectory;
	delete iBURDirectory;
	delete iIniExt;
	delete iCreExt;
	delete iTrnsExt;
	iFs.Close();

	iCacheManager=NULL;
	iInstallDirectory=NULL;
	iDataDirectory=NULL;
	iRomDirectory=NULL;
	iBURDirectory=NULL;	
	iIniExt=NULL;	
	iCreExt=NULL;	
	iTrnsExt=NULL;	
	
	iOwnerIdLookUpTable.Close() ;
	}
	
TBool TServerResources::PersistFileExistsL(TUid aUid)
	{
	CDir* persistDir;
	TPtr dataDirectory = TServerResources::iDataDirectory->Des();
	User::LeaveIfError(TServerResources::iFs.GetDir(dataDirectory,
													KEntryAttNormal,
													ESortNone,
													persistDir));

	CleanupStack::PushL(persistDir);

	const TInt fileCount = persistDir->Count();
	TBool found = EFalse;

	// Open each repositories in the persist directory
	for(TInt i = 0; i < fileCount; ++i)
		{
		TUid uid;
		if (!GetUid(const_cast<TEntry&>((*persistDir)[i]), uid))
			{
			if (uid == aUid)
				{
				found = ETrue;
				break;
				}
			}
		}

	CleanupStack::PopAndDestroy(persistDir);

	return found;
	}

TInt TServerResources::GetUid(TEntry& aEntry, TUid& aUid)
	{
		const TInt KUidLen = 8;
		TPtrC uidPtr = aEntry.iName.Des().LeftTPtr(KUidLen);
		TLex  lex=uidPtr;
		TUint32 uidValue;

		if (lex.Val(uidValue, EHex) == KErrNone)
			{
			aUid = TUid::Uid(uidValue);
			}
		else
			{
			return KErrNotFound;
			}
			
	return KErrNone;
	}

TInt TOwnerIdMapping::CompareUids (const TOwnerIdMapping& aOwnerIdMapping1, const TOwnerIdMapping& aOwnerIdMapping2) 
	{
	if (aOwnerIdMapping1.iRepUid < aOwnerIdMapping2.iRepUid)
		return -1 ;
	else if (aOwnerIdMapping1.iRepUid > aOwnerIdMapping2.iRepUid)
		return 1 ;
	else
		return 0 ;
	}
		

#ifdef __CENTREP_SERVER_MEMTEST__

void TServerResources::StopRecordTimerResult()
	{
	iMemTestDataCount = KMemBufMaxEntry;
	}

//aLocation: location where the memory reading is done (a method specifier)
//aIdentifier: identifier of the memory reading (e.g. repository id, 10th reading etc)
void TServerResources::RecordTimerResult(TMemTestLocationIdentifier aLocation, TInt32 aIdentifier)
	{
	if(iMemTestDataCount+6 > KMemBufMaxEntry)
		{
		if(iMemTestDataCount+3 <= KMemBufMaxEntry)
			{
			iMemTestData[iMemTestDataCount++] = static_cast<TInt32>(KMagicMemTestOutOfBounds);
			iMemTestData[iMemTestDataCount++] = static_cast<TInt32>(KMagicMemTestOutOfBounds);
			iMemTestData[iMemTestDataCount++] = static_cast<TInt32>(KMagicMemTestOutOfBounds);
			}
		}
	else
		{
		RHeap& heap = User::Heap();
		TInt biggestBlock;
		
		iMemTestData[iMemTestDataCount++] = aLocation;
		iMemTestData[iMemTestDataCount++] = aIdentifier;
		iMemTestData[iMemTestDataCount++] = static_cast<TInt32>(heap.Size() - heap.Available(biggestBlock));
		}
	}
#endif //__CENTREP_SERVER_MEMTEST__

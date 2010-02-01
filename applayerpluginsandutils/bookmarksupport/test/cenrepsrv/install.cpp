// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Implements installed file notification
// 
//

/**
 @internalComponent
*/

#include "install.h"
#include "srvrepos.h"
#include "srvres.h"
#include "srvparams.h"
#include "cachemgr.h"

CCentRepSWIWatcher* CCentRepSWIWatcher::NewL(RFs& aFs)
	{
	CCentRepSWIWatcher* self = new(ELeave) CCentRepSWIWatcher(aFs);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}
	
void CCentRepSWIWatcher::ConstructL()
	{
	// Attach to SWI property
	User::LeaveIfError(iSWIKey.Attach( KUidSystemCategory, KUidSoftwareInstallKey));
	
	// Initialise SWI operation and status
	TInt swiProperty;
	const TInt error = iSWIKey.Get(KUidSystemCategory, KUidSoftwareInstallKey, swiProperty);
	
	if (error == KErrNone)
		{
		iSWIOperation=swiProperty & KSwisOperationMask;
		iSWIStatus= swiProperty & KSwisOperationStatusMask;
		}
	else if (error != KErrNotFound)
		{
		User::LeaveIfError(error);
		}
	
	// Get last saved contents of install directory
	GetInstallDirL();
	
	// Do any actions required by pre server start-up SWI activity
	TRAP_IGNORE(FindChangedEntriesL(ETrue));
	}
	
CCentRepSWIWatcher::CCentRepSWIWatcher(RFs& aFs) :
 	CActive(EPriorityStandard), 
 	iInstallDir(*TServerResources::iInstallDirectory),
 	iFs(aFs),
 	iSWIOperation( ESwisNone),
	iSWIStatus( ESwisStatusSuccess)
	{
 	CActiveScheduler::Add(this);
	}
	
CCentRepSWIWatcher::~CCentRepSWIWatcher()
	{
	Cancel();
	iSWIKey.Cancel();
	iSWIKey.Close();
	iInstallEntryArray.ResetAndDestroy();
	iCurrentInstallDirEntries.ResetAndDestroy();
	}

void CCentRepSWIWatcher::Start()
	{
	if(IsActive())
		return;
	
	NotifyChange();
	}
	
 void CCentRepSWIWatcher::NotifyChange()
 	{
	
	// Register for P&S of SWI flag
	iSWIKey.Subscribe(iStatus);
	
	SetActive();
	}

void CCentRepSWIWatcher::RunL()
	{	
	// Get SWI Key 
	TInt swiProperty;
	User::LeaveIfError(iSWIKey.Get(KUidSystemCategory, KUidSoftwareInstallKey, swiProperty));
	
	HandleSWIEventL(swiProperty);
	
	NotifyChange();
	}
	
void CCentRepSWIWatcher::HandleSWIEventL(TInt aSWIProperty)
	{	
	iSWIOperation=aSWIProperty & KSwisOperationMask;
	iSWIStatus=   aSWIProperty & KSwisOperationStatusMask;

	// Need to handle successful and aborted install/uninstall and successful restore
	// Can't handle aborted restore
	switch(iSWIOperation)
		{
		case ESwisNone:
			break;
		case ESwisInstall:
		case ESwisUninstall:
			if(iSWIStatus==ESwisStatusSuccess)
				{
				// Handle SWI events
				FindChangedEntriesL();
				}
			else if(iSWIStatus==ESwisStatusAborted)
				{
				// Update directory to reset timestamps
				ReadInstallDirL(iInstallEntryArray);
				SaveInstallDirL();
				}
			break;
		case ESwisRestore:
			break;
		default:
			break;
		}
	}
	
// Catch leaves so they don't stop the server
TInt CCentRepSWIWatcher::RunError( TInt aError)
	{
	static_cast<void>(aError);
	
	RDebug::Print(_L("Run error %d"), aError);
	
	// Reinitialise directory list
	iInstallEntryArray.ResetAndDestroy();
	
	if(!IsActive())
		{
		NotifyChange();
		}
	return KErrNone;
	}
	
void CCentRepSWIWatcher::DoCancel()
	{
	// Cancel subscription to SW P&S flag
	iSWIKey.Cancel();
	iSWIKey.Close();
	}

void CCentRepSWIWatcher::ReadInstallDirL(RPointerArray<CInstallEntry> &aEntryArray)
	{
	CDir* installDir=NULL;
	//Empty contents of directory
	aEntryArray.ResetAndDestroy();
	// Read contents of install directory
	User::LeaveIfError(iFs.GetDir(iInstallDir,KEntryAttNormal, ESortByDate, installDir));
	CleanupStack::PushL(installDir);
	const TInt fileCount = installDir->Count();
	TInt i = 0;
	for(;i<fileCount; ++i)
		{
		CInstallEntry* installEntry = CInstallEntry::NewL();
		CleanupStack::PushL(installEntry);
		TEntry entry=(*installDir)[i];
		installEntry->SetL(entry);
		User::LeaveIfError(aEntryArray.Append(installEntry));
		CleanupStack::Pop(installEntry);
		}
	CleanupStack::PopAndDestroy(installDir);
	}
	
TBool CCentRepSWIWatcher::MatchEntries( const CInstallEntry &aSource, const CInstallEntry &aTarget)
	{
	return((aSource.Uid()==aTarget.Uid()) &&
	       (aSource.FileExt()==aTarget.FileExt()));
	}
		
void CCentRepSWIWatcher::FindChangedEntriesL(TBool aStartup)
	{
	// Find added or updated entries
	ReadInstallDirL(iCurrentInstallDirEntries);
		
	TInt newCount=iCurrentInstallDirEntries.Count();
	TInt currentCount=iInstallEntryArray.Count();
	TInt i;
	TInt r;
	TInt operation;

	// If both counts are 0, we shouldn't have been notified, just return
	if( (newCount==0) && (currentCount==0))
		return;
	
	if(aStartup)
		{
		operation=ESwisNone;
		}
	else
		{
		operation=iSWIOperation;
		}

	if( newCount==0)						// currentCount > 0, newCount = 0
		{									// All installed files have been deleted
		// Handle deletes of all files
		for(i=0;i<currentCount;i++)
			{
			iInstallEntryArray[i]->HandleFileDeleteL(operation);
			}
		// Free memory of elements
		iInstallEntryArray.ResetAndDestroy();
		}
	else 
		{	
		if( currentCount==0)				// currentCount = 0, newCount > 0
			{								// All new files need to be handled
			for(i=0;i<newCount;i++)
				{
				CInstallEntry* newEntry=iCurrentInstallDirEntries[i];
				newEntry->HandleFileCreateL(operation);
				}
			}
		else								// currentCount > 0, newCount > 0
			{
			// Find added and modified entries by going through new entries and
			// looking for them in current array
			for(i=0;i<newCount;i++)
				{
				CInstallEntry* newEntry=iCurrentInstallDirEntries[i];
				r=iInstallEntryArray.Find( newEntry, MatchEntries);
				// If we find new entry in current array, check modification date
				if(r>=KErrNone)
					{
					CInstallEntry* currentEntry=iInstallEntryArray[r];
					if( newEntry->Modified() > currentEntry->Modified())
						{												
						// Deal with newly installed file, note use newEntry
						// so we use new timestamp
						newEntry->HandleFileUpdateL(operation);
						}
					}
				else if(r==KErrNotFound)		// File has been added
					{
					// Handle add
					newEntry->HandleFileCreateL(operation);
					// Don't leave on KErrNotFound
					r=KErrNone;
					}
				User::LeaveIfError(r);
				}
	
			// Find deleted entries by going through current entries and looking for them 
			// in new array
			for(i=0;i<currentCount;i++)
				{
				CInstallEntry* currentEntry=iInstallEntryArray[i];
				r=iCurrentInstallDirEntries.Find( currentEntry, MatchEntries);
				// If we don't find current entry in new array, it's been deleted
				if(r==KErrNotFound)
					{
					// Deal with uninstalls
					currentEntry->HandleFileDeleteL(operation);
					// Don't leave on KErrNotFound
					r=KErrNone;
					}
				User::LeaveIfError(r);
				}
			}

		// Clear out old list
		iInstallEntryArray.ResetAndDestroy();
		
		// Re-read directory - if any files were corrupt they have been deleted
		// during the merge, so we need to re-read in case this has occurred
		ReadInstallDirL(iInstallEntryArray);
		}
		
	SaveInstallDirL();
	iCurrentInstallDirEntries.ResetAndDestroy();
	} 

CInstallEntry::CInstallEntry() :
	iUid(KNullUid),
	iModified(0),
	iFileExt(EUnknown)
	{
	}

CInstallEntry::~CInstallEntry()
	{
	if( iRepository)
		{
		iRepository->Close(*iNotifier);
		delete iRepository;
		}
	delete iNotifier;
	}

void CInstallEntry::SetL(TEntry& aEntry)	
	{
	// Get uid from file name
	const TInt KUidLen = 8;
	TPtrC uidPtr=aEntry.iName.Des().LeftTPtr(KUidLen);
	TLex  lex=uidPtr;

	TUint32 uid;
	User::LeaveIfError(lex.Val(uid,EHex)); 
	iUid.iUid=static_cast<TInt32>(uid);
	
	// save extension type 
	_LIT(KIniFileExtension, ".txt");
	_LIT(KExternalizedFileExt, ".cre");

	const TInt KExtLen = 4;
	TPtrC extPtr=aEntry.iName.Des().RightTPtr(KExtLen);
	
	if(extPtr.Compare(KIniFileExtension)==0)
		{
		iFileExt=EIni;
		}
	else if(extPtr.Compare(KExternalizedFileExt)==0)
		{
		iFileExt=ECre;
		}
	else
		{
		iFileExt=EUnknown;
		}
		
	iModified=aEntry.iModified;
	}
	
void CInstallEntry::ConstructL()	
	{
	// Create repository object
	iRepository = new(ELeave) CServerRepository;
	// Notifier needed to open repositories.		
	iNotifier = new(ELeave)CSessionNotifier ;
	}
	
CInstallEntry* CInstallEntry::NewL()
	{
	CInstallEntry* self=new(ELeave)CInstallEntry();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

TUid CInstallEntry::Uid() const
	{
	return iUid;
	}
	
TTime CInstallEntry::Modified() const
	{
	return iModified;
	}
	
TCentRepFileType CInstallEntry::FileExt() const
	{
	return iFileExt;
	}
 	
void CInstallEntry::ExternalizeL(RWriteStream& aStream) const
	{
 	aStream << iUid.iUid;
	aStream << iModified.Int64();
	TUint32 fileExt=iFileExt;
	aStream << fileExt;
 	}
 	
void CInstallEntry::InternalizeL(RReadStream& aStream)
	{
 	aStream >> iUid.iUid;
 	TInt64 time;
	aStream >> time;
	iModified=time;
	TUint32 fileExt;
	aStream >> fileExt;
	iFileExt =  static_cast<TCentRepFileType>(fileExt);
 	}
 		
void CCentRepSWIWatcher::GetInstallDirL()
	{
	_LIT(KInstallDirFile, "installdir.bin");
	
	TBuf<KMaxFileName> installDirFilePath;
	installDirFilePath.Append(*TServerResources::iDataDirectory);
	installDirFilePath.Append(KInstallDirFile);

	TEntry entry;
	TInt e = TServerResources::iFs.Entry(installDirFilePath, entry);	
	if(e==KErrNotFound || e==KErrPathNotFound)
		{
		// No file saved - read initial contents of directory
		ReadInstallDirL(iInstallEntryArray);
		SaveInstallDirL();
		return;
		}

	CDirectFileStore* store = 
	    CDirectFileStore::OpenLC (TServerResources::iFs,installDirFilePath, EFileRead|EFileShareReadersOnly);
	if(store->Type()[0] != KDirectFileStoreLayoutUid)
		{
		TServerResources::iFs.Delete(installDirFilePath);
		}

	iInstallEntryArray.ResetAndDestroy();

	// Get the root stream and attempt to read the index from it
	TStreamId rootStreamId = store->Root() ;
	RStoreReadStream rootStream ;
	rootStream.OpenLC(*store, rootStreamId);
	
	// Internalize the repository	
	TUint32 count;
	rootStream >> count;
	for(TInt i=0; i<count;i++)
		{
		CInstallEntry* installEntry = CInstallEntry::NewL();
		rootStream >> *installEntry;
		CleanupStack::PushL(installEntry);
		User::LeaveIfError(iInstallEntryArray.Append(installEntry));
		CleanupStack::Pop(installEntry);
		}
		
	CleanupStack::PopAndDestroy(&rootStream);
	CleanupStack::PopAndDestroy(store);
	}
	
void CCentRepSWIWatcher::SaveInstallDirL() 
	{	
	_LIT(KInstallDirFile, "installdir.bin");
	_LIT(KInstallDirTmpFile, "installdir.tmp");
	
	TBuf<KMaxFileName> installDirTrnsFilePath;
	installDirTrnsFilePath.Append(*TServerResources::iDataDirectory);
	installDirTrnsFilePath.Append(KInstallDirTmpFile);
	
	// Create file store
	CDirectFileStore* store = CDirectFileStore::ReplaceLC(TServerResources::iFs, installDirTrnsFilePath,
		                                                      (EFileWrite | EFileShareExclusive));
	const TUid uid2  = KNullUid ;	                                                     
	store->SetTypeL(TUidType(KDirectFileStoreLayoutUid, uid2, KServerUid3)) ; 
		
	// Write the stream index/dictionary as root stream within the store
	// so we can access it when we do a restore later on
	RStoreWriteStream rootStream ;
	TStreamId rootStreamId = rootStream.CreateLC(*store) ;
	
	TUint32 count=iInstallEntryArray.Count();
	rootStream << count;
	for(TInt i=0; i<count;i++)
		{
		rootStream << *iInstallEntryArray[i];
		}
		
	rootStream.CommitL() ;
		
	CleanupStack::PopAndDestroy(&rootStream) ;
	store->SetRootL(rootStreamId);
	store->CommitL();
	CleanupStack::PopAndDestroy(store) ; 	
	
	TBuf<KMaxFileName> installDirFilePath;
	installDirFilePath.Append(*TServerResources::iDataDirectory);
	installDirFilePath.Append(KInstallDirFile);

	TEntry entry;
	if(TServerResources::iFs.Entry(installDirFilePath,entry)==KErrNone)
		{
    	User::LeaveIfError(TServerResources::iFs.Replace(installDirTrnsFilePath, installDirFilePath));
		}
	else
		{
		User::LeaveIfError(TServerResources::iFs.Rename(installDirTrnsFilePath, installDirFilePath));
		}   
	}


void CInstallEntry::HandleFileDeleteL(TInt aOperation)
	{
	// File should only have been deleted if operation was uninstall
	// or in startup case
	if((aOperation!=ESwisNone) && (aOperation!=ESwisUninstall))
		User::Leave(KErrAbort);
					
	// We don't want cache activity during SWI operations
	TServerResources::iCacheManager->DisableCache();

	iRepository->HandleSWIDeleteL(Uid(), *iNotifier);
					
	// Renable cache activity
	TServerResources::iCacheManager->EnableCache();
	}
	
void CInstallEntry::HandleFileCreateL(TInt aOperation)
	{
	// File should only have been created if operation was install
	// or in startup case
	if((aOperation!=ESwisNone) && (aOperation!=ESwisInstall))
		User::Leave(KErrAbort);
					
	// We don't want cache activity during SWI operations
	TServerResources::iCacheManager->DisableCache();
	
	iRepository->HandleSWIUpdateL(Uid(), Modified(), *iNotifier);
					
	// Renable cache activity
	TServerResources::iCacheManager->EnableCache();
	}
	
void CInstallEntry::HandleFileUpdateL(TInt aOperation)
	{
	// File should only have been modified if operation was install
	// or in startup case
	if((aOperation!=ESwisNone) && (aOperation!=ESwisInstall))
		User::Leave(KErrAbort);
					
	// We don't want cache activity during SWI operations
	TServerResources::iCacheManager->DisableCache();
	
	iRepository->HandleSWIUpdateL(Uid(), Modified(), *iNotifier);
					
	// Renable cache activity
	TServerResources::iCacheManager->EnableCache();
	}

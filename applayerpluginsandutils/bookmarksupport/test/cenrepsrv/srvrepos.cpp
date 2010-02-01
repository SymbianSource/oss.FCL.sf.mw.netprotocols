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

#include "panic.h"
#include "shrepos.h"
#include "srvrepos.h"
#include "srvres.h"
#include "cachemgr.h"
#include "sessnotf.h"
#include "srvPerf.h"

#define TRAP_UNIFIED(_unifiedLeave, _function)	\
	{ \
	TInt _returnValue = 0; \
	TRAP(_unifiedLeave, _returnValue = _function);	\
	TInt& __rref = _unifiedLeave; \
	__rref = _unifiedLeave | _returnValue; \
	}

RPointerArray<CSharedRepository> CServerRepository::iOpenRepositories;

CServerRepository::~CServerRepository()
	{
	if(iOpenRepositories.Count()==0)
		iOpenRepositories.Reset();
	}
	
TInt CServerRepository::ReadIniFileL(CSharedRepository*& aRepository, TCentRepLocation aLocation)
	{
	TInt r=KErrNone;
	CIniFileIn* inifile = 0;

	HBufC* fileName(NULL);
    //allocates memory on the heap
    TServerResources::CreateRepositoryFileNameLC(fileName,aRepository->iUid,aLocation,EIni);		
	r = CIniFileIn::NewLC(inifile,fileName,aLocation);
	if(r==KErrNone)
		{			
		r=ReadSettingsL(inifile, aRepository);		
		if(r==KErrCorrupt)
			{
			// File is corrupt, if it's not the ROM file, delete it
			if(fileName && aLocation != ERom)
				User::LeaveIfError(TServerResources::iFs.Delete(*fileName));
			// Delete any repository settings that may have been read in
			aRepository->iSettings.Reset();
			}
		}

	CleanupStack::PopAndDestroy(inifile); // inifile	 
	CleanupStack::PopAndDestroy(fileName);	// filename
	return r;
	}
	
TInt CServerRepository::CreateRepositoryL(CSharedRepository*& aRepository, TCentRepLocation aLocation)
	{
	aRepository->iSettings.SetIsDefault(aLocation!=EPersists);
    TInt err(KErrNotFound);
    
	err = aRepository->CreateRepositoryFromCreFileL(aLocation);
	if(err==KErrNotFound)
		{
		err = ReadIniFileL(aRepository,aLocation);
		}
	return( err);
	}
	
/**
In order to create a repository this routine looks for a .cre or .txt file. 
Txt and cre files can co-exist in install and ROM but not in persists location.
If a persists txt file exists, the first write to the repository will cause a
cre file to be created and the txt file to be deleted.
If both files exist in the same location, the .cre is picked up first. 
If the .cre file is not found,a .txt file from the same location is tried.
Otherwise if the .cre file is corrupted,it tries a .cre file from a next location. 

Note:
If a cre file exists at a particular location, even if the cre file is corrupt a txt 
file will not be searched for in the same location. 
*/

TInt CServerRepository::CreateRepositoryL(CSharedRepository*& aRepository, CIniFileIn::TIniFileOpenMode aIniFileOpenMode)
    {
    TInt err(KErrNotFound);
    
    switch (aIniFileOpenMode)
	    {
	    case CIniFileIn::EAuto:
	        {
	        // Look in persists dir	 
	        err=CreateRepositoryL(aRepository, EPersists);		
	        				
			if(err==KErrNone)
				{
				return err;
				}
			 
	        // No persists file - look in ROM dir	 
	        // Do this before looking in the install dir, because if there is
	        // a ROM file, an install file and no persists file then this
	        // repository is being opened after a SW install but before the 
	        // merge. In this case the install file shouldn't be opened
	        // until after the merge.
	        err=CreateRepositoryL(aRepository, ERom);		
	        				
			if(err==KErrNone)
				{
				break;
				}
			else if(err==KErrNotFound)
				{				
			     // Look in install directory only if there was no ROM or persists file	            
				err=CreateRepositoryL(aRepository, EInstall);
				if(err==KErrNone)
					{
					TTime installFileTimeStamp=TServerResources::CentrepFileTimeStampL(aRepository->iUid, EInstall);
					aRepository->SetInstallTime(installFileTimeStamp);
					}
				}
	        break;
	        }
	        
	    case CIniFileIn::EInstallOnly:
	        {
	    	err=CreateRepositoryL(aRepository, EInstall);		
	        break;
	        }

	    case CIniFileIn::ERomOnly:
	        {
	    	err=CreateRepositoryL(aRepository, ERom);		
	        break;
	        }
	    }

	return err;
	}
	
TInt CServerRepository::ReadSettingsL(CIniFileIn *aIniFile, CSharedRepository* aRep)
	{	
	return aRep->ReloadContentL(*aIniFile);
	}

void CServerRepository::LoadRepositoryLC(TUid aUid, TBool aFailIfNotFound, CSharedRepository*& aRepository, CIniFileIn::TIniFileOpenMode aIniFileOpenMode)
	{
	// Calculate the amount of memory this repository will take in the heap
	// by checking the heap size before and after the internalization
	RHeap& myHeap = User::Heap();
	TInt firstSize = myHeap.Size();
	TInt biggestBlock;
	TInt firstAvail = myHeap.Available(biggestBlock);
	
	aRepository = new CSharedRepository(aUid);
#ifdef CACHE_OOM_TESTABILITY
	if ((aRepository==NULL)&&!iTrapOOMOnOpen)	
		{
		User::Leave(KErrNoMemory);
		}
#endif	
	if ((aRepository==NULL)&&TServerResources::iCacheManager->Enabled())
		{
		// If cache enabled, try recovery by releasing the cache
		TServerResources::iCacheManager->FlushCache();
		// retry
		aRepository = new CSharedRepository(aUid);
		}
	// If still no memory, return error
	if (aRepository==NULL)
		{
		User::Leave(KErrNoMemory);
		}
	else // successfully created the object, so push it into the cleanup stack
		{
		CleanupStack::PushL(aRepository);
		}
		
	// Now that we have enough memory for the object and constructed it properly
	// we try to load it. We trap all errors, either from leaving functions or error code
	// returning functions and unify them (in all cases only one of these codes will
	// contain a valid value and the other will be 0, and for our purposes we treat
	// all errors the same no matter if they're thrown or returned)

	TInt unifiedErrorCode;
	TRAP_UNIFIED(unifiedErrorCode, CreateRepositoryL(aRepository, aIniFileOpenMode));
	
	switch(unifiedErrorCode)
		{
		case KErrNoMemory:
			{
			if (TServerResources::iCacheManager->Enabled()) // cache enabled
				{
#ifdef CACHE_OOM_TESTABILITY
				if (!iTrapOOMOnOpen)	
					{
					User::Leave(KErrNoMemory);
					}
#endif	
				// Flush cache
				TServerResources::iCacheManager->FlushCache();

				firstSize = myHeap.Size();
				firstAvail = myHeap.Available(biggestBlock);
	
				//retry
				TRAP_UNIFIED(unifiedErrorCode, CreateRepositoryL(aRepository, aIniFileOpenMode));
				}
			}
			break;
		case KErrNotFound:
		case KErrPathNotFound:		
			{
			if (!aFailIfNotFound) // backup open
				{
				// override error condition and continue normally
				unifiedErrorCode = KErrNone;
				}
			}
			break;
		}

	// If unhandled, leave
	User::LeaveIfError(unifiedErrorCode);

	// Otherwise, finalize calulations
	TInt lastSize = myHeap.Size();
	TInt lastAvail = myHeap.Available(biggestBlock);

	TInt calcSize = (lastSize - lastAvail) - (firstSize - firstAvail);
	// record repository size for cache algorithm purposes
	aRepository->SetSize(calcSize);
	}

void CServerRepository::OpenL(TUid aUid, MObserver& aObserver, TBool aFailIfNotFound)
	{
	TInt i = FindRepository(aUid);
	if(i!=KErrNotFound)
		{
		CSharedRepository* rep = iOpenRepositories[i];
		rep->AddObserverL(aObserver);
		iRepository = rep;
		
		// find the item in the cache and remove it because it's in-use again
		TServerResources::iCacheManager->RemoveIdleRepository(iRepository);
		}
	else
		{
		CSharedRepository* rep = NULL;
		
		// For memory usage testing purposes
		RECORD_HEAP_SIZE(EMemLcnRepositoryOpen, aUid.iUid);
		// Various error conditions are handled in this function 
		LoadRepositoryLC(aUid, aFailIfNotFound, rep, CIniFileIn::EAuto);
		// For memory usage testing purposes
		RECORD_HEAP_SIZE(EMemLcnRepositoryOpen, aUid.iUid);
				
		rep->AddObserverL(aObserver);
		iOpenRepositories.AppendL(rep);
		iRepository = rep;

		// Add owner mapping to list - Will fail if an entry already exists
		// with this Repository UID but this doesn't matter
		TUid owner = iRepository->Owner() ;
		TServerResources::AddOwnerIdLookupMapping (aUid.iUid, owner.iUid) ;

		CleanupStack::Pop(rep);
		}
	}

void CServerRepository::Close(MObserver& aObserver)
	{
	if(iRepository)
		{
		// cancel to ensure any read/write locks are released and transaction settings cleaned up
		CancelTransaction();
		if(iRepository->RemoveObserver(aObserver)==0)
			{
			TInt i = FindRepository(iRepository->Uid());
			__ASSERT_ALWAYS(i>=0, Panic(ERepositoryNotFound));

			// Check cache size and carry out forced eviction if necessary			
			// Start Eviction if the repository fits in the cache	
			if (TServerResources::iCacheManager->Enabled()&&
				TServerResources::iCacheManager->StartEviction(iRepository))
				{
				// Repository added to the idle list in eviction order
				__CENTREP_TRACE2("CENTREP: Repository Became Idle when closing repository %x size %d", iRepository->Uid().iUid, iRepository->Size());
				}
			else
				{
				iOpenRepositories.Remove(i);
				delete iRepository;
				__CENTREP_TRACE1("CENTREP: Repository Delete when closing repository %x", iRepository->Uid().iUid);
				}
			}
		else
			{
			__CENTREP_TRACE1("CENTREP: Observer Removed when closing repository %x", iRepository->Uid().iUid);
			}
		iRepository = 0;
		}
	}
	
/**
The method will make an attempt to restore current repository's consistency if previous
CommitChangesL() failed.
@leave System-wide error codes
*/
void CServerRepository::RestoreConsistencyL()
	{
	if(iRepository)
		{
		iRepository->RestoreConsistencyL();
		}
	}
	
/**
Attempt to reset a single key to it's value in the file in the given location. Routine
attempts to find a .cre file first. If ( and only if ) a cre file doesn't exist the 
routine attempts to find a txt file.
Note that it would be possible to use LoadRepositoryLC here but for the txt file
that would take longer. This is because in LoadRepositoryLC the txt file is 
completely processed. The Reset specific txt file opening code below is quicker because 
it is just attempting to find the reset key.
*/	
void CServerRepository::ResetFromIniFileL(TUint32 aId, 
										  TCentRepLocation aLocation,
										  TBool& aKeyFound)
	{
	aKeyFound=EFalse;

	// Attempt to reset key to value in cre file if it exists
	
	// Attempt to create a temporary repository from the cre file in aLocation
	CSharedRepository* rep = new(ELeave) CSharedRepository(iRepository->Uid());
	CleanupStack::PushL(rep);
	TInt err = rep->CreateRepositoryFromCreFileL(aLocation);

	// Search for aId in the temporary repository
	if (err!=KErrNotFound)
		{		
		// Note that for all errors except KErrNotFound code leaves and doesn't
		// attempt to look for txt file. This is intentional. Code does not 
		// attempt to support coexisting cre and txt files.
		User::LeaveIfError(err);
		
		// Search for aId in the temporary repository
		TServerSetting* s = rep->iSettings.Find(aId);					
		if(s)
			{
			aKeyFound=ETrue;
			// Mark the setting as default again
			s->SetClean();
			iRepository->ResetAndPersistL(*s);
			s->SetAccessPolicy(GetFallbackAccessPolicy(aId));
			}
			
		CleanupStack::PopAndDestroy(rep);
		return;
		}
	else
		{
		CleanupStack::PopAndDestroy(rep);
		}
		
	HBufC* fileName(NULL);	
	TServerResources::CreateRepositoryFileNameLC(fileName,iRepository->Uid(),aLocation,EIni);
	
	CIniFileIn* inputFile = 0;
	TInt r = CIniFileIn::NewLC(inputFile,fileName,aLocation);
	if(r==KErrNone)
		{
		//we don't want to read this stuff again... just skip over to get to settings!
		inputFile->SkipOwnerSectionL() ;
		inputFile->SkipTimeStampSectionL() ;
		inputFile->SkipDefaultMetaSectionL() ;
		inputFile->SkipPlatSecSectionL();
		
		// Find start of Main section
		inputFile->FindMainSectionL();
	
		TServerSetting s;
		TBool singleMetaFound=EFalse;
		TBool singleReadPolicyFound=EFalse;
		TBool singleWritePolicyFound=EFalse;
		TSecurityPolicy singleReadPolicy;
		TSecurityPolicy singleWritePolicy;

		// Note that calling CIniFile::ReadSettingL causes the single policy ( if it exists ) to be read from the
		// file being reset to, but doesn't update the single policy array, which is not required in the reset case. 
		while((r=inputFile->ReadSettingL(s,singleReadPolicy, singleWritePolicy, singleReadPolicyFound, singleWritePolicyFound, singleMetaFound)) == KErrNone)
			{	
			iRepository->SetMetaDataOnRead( s, singleMetaFound);			
			if(s.Key()==aId)
				{
				// Mark the setting as default again
				s.SetClean();
				iRepository->ResetAndPersistL(s);
				s.SetAccessPolicy(GetFallbackAccessPolicy(aId));
				aKeyFound = ETrue;
				break;
				}
			s.Reset();
			}

	
		}
	CleanupStack::PopAndDestroy(inputFile);	 // inputFile
	CleanupStack::PopAndDestroy(fileName);	 // filename
	}

TInt CServerRepository::ResetL(TUint32 aId)
	{
	// not yet supported in transactions
	ASSERT(!IsInTransaction());

	// if setting has not changed, there nothing to do
	TServerSetting *targetSetting = GetSetting(aId) ;

	if (targetSetting)
		{
		if ((targetSetting->Meta() & KMetaDefaultValue))
			{
			return KErrNone;
			}
		}

	TInt error = KErrNone;
	TBool keyReset = EFalse;

	// Check for default value in any installed file first
	ResetFromIniFileL(aId, EInstall, keyReset);
	if (keyReset)
		return KErrNone;

	// Either we couldn't find a matching key or
	// there wasn't an installed file - try for a ROM
	// file
	ResetFromIniFileL(aId, ERom, keyReset);
	if (keyReset)
		return KErrNone;
	
	// No default value found in install or ROM file
	// delete the key!
	error = iRepository->DeleteAndPersist(aId);

	return error ;
	}
	
void CServerRepository::ResetRepositoriesL()
	{
	// Read contents of persist directory to get a list of repositories
	CDir* persistDir;
	TPtr dataDirectory = TServerResources::iDataDirectory->Des();
	User::LeaveIfError(TServerResources::iFs.GetDir(dataDirectory,
													KEntryAttNormal,
													ESortNone,
													persistDir));

	CleanupStack::PushL(persistDir);

	const TInt fileCount = persistDir->Count();

	// Open each repositories in the persist directory
	// and restore the settings.
	for(TInt i = 0; i < fileCount; ++i)
		{
		// Attempt to extract a repository UID from directory entry
		TUid uid;
		if (!TServerResources::GetUid(const_cast<TEntry&>((*persistDir)[i]), uid))
			{
			CSessionNotifier notifier;

			// Create shared repository
			CServerRepository *repository = new(ELeave) CServerRepository;
			CleanupStack::PushL(repository);

			repository->OpenL(uid, notifier);
		
			// Restore settings
			repository->ResetRepositoryL();

			// delete repository.
			repository->Close(notifier);
			CleanupStack::PopAndDestroy(repository);
			}
		}
	CleanupStack::PopAndDestroy(persistDir);
	}

TInt CServerRepository::ResetRepositoryL()
	{
	// for each key in combined ROM/Install restore
	TUid uid = iRepository->Uid();

	CSharedRepository* repository = 0;
	
	// Create a rep using the ROM file
  	TBool romExists=TServerResources::RomFileExistsL(uid);
	if(romExists)
		{
		LoadRepositoryLC(uid, ETrue, repository, CIniFileIn::ERomOnly);
		}
		
	// Create install rep for merging
	CSharedRepository *installRep = 0;
	TBool installExists=TServerResources::InstallFileExistsL(uid);
	if(installExists)
		{			
		LoadRepositoryLC(uid, ETrue, installRep, CIniFileIn::EInstallOnly);
		}
		
	TInt err=KErrNone;
	if(	romExists && installExists)
		{
		// If install and ROM exist create a merged rep to Reset against
		repository->MergeL(*installRep, ESWIUpgradeMerge);
		}
	else if(!romExists && !installExists)
		{
		// Reset against empty repository
		repository = new(ELeave) CSharedRepository(uid);
		CleanupStack::PushL(repository);
		}

	for(TInt i = 0; i < repository->iSettings.Count(); i++)
		{
		TServerSetting* setting = &repository->iSettings[i];

		if ((setting->Meta() & KMetaRfsValue) != KMetaRfsValue)
			{
			continue;
			}
		TUint32 key = setting->Key();

		// If the clean is set on setting in the persist, nothing to do
		TServerSetting *targetSetting = GetSetting(key);

		if (targetSetting)
			{
			if (targetSetting->Meta() & KMetaDefaultValue)
				{
				continue;
				}
			}
		iRepository->ResetNoPersistL(*setting);
		}

	// Persist settings
	iRepository->CommitChangesL();

	CleanupStack::PopAndDestroy(repository);

	return err;
	}


TInt CServerRepository::ResetAllL()
	{
	// not yet supported in transactions
	ASSERT(!IsInTransaction());
	// fail all sessions' transactions first
	iRepository->FailAllTransactions(/*aExcludeTransactor*/NULL);

	TUid uid = iRepository->Uid();

	// Reset
 	 	
	// Create a rep using the ROM file
	CSharedRepository* rep = 0;
  	TBool romExists=TServerResources::RomFileExistsL(uid);
	if(romExists)
		{
		LoadRepositoryLC(uid, ETrue, rep, CIniFileIn::ERomOnly);
		}
		
	// Create install rep for merging
	CSharedRepository *installRep = 0;
	TBool installExists=TServerResources::InstallFileExistsL(uid);
	if(installExists)
		{			
		LoadRepositoryLC(uid, ETrue, installRep, CIniFileIn::EInstallOnly);
		}
		
	TInt err=KErrNone;
	if(	romExists && installExists)
		{
		// If install and ROM exist create a merged rep to Reset against
		rep->MergeL(*installRep, ESWIUpgradeMerge);
		err=iRepository->ResetAllNoPersistL(*rep);
		CleanupStack::PopAndDestroy(installRep);
		CleanupStack::PopAndDestroy(rep);
		}
	else if(romExists)
		{
		// Reset against ROM
		err=iRepository->ResetAllNoPersistL(*rep);
		CleanupStack::PopAndDestroy(rep);
		}
	else if(installExists)
		{
		// Reset against install
		err=iRepository->ResetAllNoPersistL(*installRep);
		CleanupStack::PopAndDestroy(installRep);
		}
	else
		{
		// Reset against empty repository
		rep = new(ELeave) CSharedRepository(uid);
		CleanupStack::PushL(rep);
		err=iRepository->ResetAllNoPersistL(*rep);
		CleanupStack::PopAndDestroy(rep);
		}
	
	return err;
	}

TInt CServerRepository::FindRepository(TUid aUid) const
	{
	TInt i;
	for(i=iOpenRepositories.Count()-1; i>=0; i--)
		if(iOpenRepositories[i]->Uid()==aUid)
			break;
	return i;
	}
	
// Handle install directory file update. 
void CServerRepository::HandleSWIUpdateL(TUid aUid, TTime aModified, CSessionNotifier &aNotifier)
	{		
	// A file create or update has just occurred in the SWI directory. 
	// Need to check if this is a new install. 
	
	if(TServerResources::PersistsFileExistsL(aUid) ||
	   TServerResources::RomFileExistsL(aUid))
		{	
		// Create a rep using the ROM or persists file
		OpenL(aUid, aNotifier);
		if(iRepository->IsTransactionActive())			
			{
			// Fail transactions on any currently open session
			iRepository->FailAllTransactions(NULL);
			}
			
		// Create install rep for merging
 		CSharedRepository *installRep = 0;
 		LoadRepositoryLC(aUid, ETrue, installRep, CIniFileIn::EInstallOnly);
	
		// Perform merge
		iRepository->HandleUpdateMergeL(aModified, *installRep);
						
		CleanupStack::PopAndDestroy(installRep);
		Close(aNotifier);
		}
	else	// No ROM or persists
		{
		// Create install rep for persisting
		OpenL(aUid, aNotifier);
	
		iRepository->CommitChangesL();
		Close(aNotifier);
		}
	}


// Handle install directory file delete 
void CServerRepository::HandleSWIDeleteL(TUid aUid, CSessionNotifier &aNotifier)
	{			
	// A file delete has just occurred in the SWI directory. If there is no ROM file
	// this is a complete uninstall, so delete persists file.Otherwise, do downgrade
	// merge.
	
	if(TServerResources::RomFileExistsL(aUid))		// ROM file, this is an upgrade uninstall
		{
		if(!TServerResources::PersistsFileExistsL(aUid))
			{
			// If we are downgrading the ROM, there should be a persists file because the
			// original upgrade should have created one.
			// However if there isn't a persists file, there's nothing to do, so just return
			return;
			}
			
		// Create a rep using the persists file
		OpenL(aUid, aNotifier);
		if(iRepository->IsTransactionActive())			
			{
			// Fail transactions on any currently open session
			iRepository->FailAllTransactions(NULL);
			}
		
		// Create ROM rep for merging
	 	CSharedRepository *romRep = 0;
		LoadRepositoryLC(aUid, ETrue, romRep, CIniFileIn::ERomOnly);

		// Perform merge
		iRepository->HandleDeleteMergeL(*romRep);
		
		CleanupStack::PopAndDestroy(romRep);
		Close(aNotifier);
		}
	else											// No ROM file, this is a complete uninstall
		{		
		if(TServerResources::PersistsFileExistsL(aUid))
			{
		 	TServerResources::DeletePersistsFileL(aUid);
		 	
			// Check if the repository was open
			TInt i = FindRepository(aUid);

			// If repository is open, fail all transactions 
			if(i>KErrNotFound)			   					  
				{
				OpenL(aUid, aNotifier);
				if(iRepository->IsTransactionActive())			
					{
					// Fail transactions on any currently open session
					iRepository->FailAllTransactions(NULL);
					}
				iRepository->ResetContent();
				Close(aNotifier);				
				}
			}
		}
	}

void CServerRepository::StoreRepositoryContentsL(CStreamStore& aStore, TStreamId & aSettingStreamId, TStreamId & aDeletedSettingsStreamId) const
	{
	RStoreWriteStream outStream;
	aSettingStreamId = outStream.CreateLC(aStore); // Creates the write stream
	iRepository->WriteBackupStream(outStream); // Only care about repository contents.
	outStream.CommitL(); // Commits the stream
	CleanupStack::PopAndDestroy(&outStream); // Performs cleanup on the write stream object

	
	aDeletedSettingsStreamId = outStream.CreateLC(aStore); // Creates the write for settings stream
	iRepository->WriteDeletedSettingsStream(outStream) ;
	outStream.CommitL(); // Commits the stream
	CleanupStack::PopAndDestroy(&outStream); // Performs cleanup on the write stream object
	}
	
void CServerRepository::RestoreRepositoryContentsL(CStreamStore& aStore, TStreamId aSettingStreamId, TStreamId aDeletedSettingsStreamId)
	{
	RStoreReadStream inStream;
	inStream.OpenLC(aStore, aSettingStreamId); // Creates the write stream
	iRepository->InternalizeL(inStream); // Only care about repository contents.
	CleanupStack::PopAndDestroy(&inStream);    // Perform cleanup on the read stream object

	// If the backup contains a list of deleted settings read them in and apply them.
	if (aDeletedSettingsStreamId != KNullStreamId)
		{
		inStream.OpenLC(aStore, aDeletedSettingsStreamId); // Creates read stream for deleted settings (if available)

		TCardinality numDeletedSettings ;
		inStream >> numDeletedSettings ;
		
		for (TInt i = 0; i < numDeletedSettings; i++)
			{
			TUint32 settingToDelete ;
			inStream >> settingToDelete ;
			iRepository->DeleteNoPersist(settingToDelete) ; 		
			}
		CleanupStack::PopAndDestroy(&inStream);            // Perform cleanup on the read stream object		
		}
	return;
	}

static void CancelTransactionCleanupOperation(TAny* aRepository)
	{
	static_cast<CServerRepository*>(aRepository)->CancelTransaction();
	}

// So CancelTransaction is called in case of Leave. Must pop with CleanupStack::Pop() or similar
void CServerRepository::CleanupCancelTransactionPushL()
	{
	CleanupStack::PushL(TCleanupItem(CancelTransactionCleanupOperation, this));
	}

/**
@internalTechnology
Check the range of security policies against RMessage
@return
	KErrNone if read access policies of all settings in array pass,
	KErrPermissionDenied if any single policy fails.
*/
TInt CServerRepository::CheckReadPermissions(RSettingPointerArray& aSettings, const TClientRequest& aMessage, const char *aDiagnostic)
	{
	TInt error = KErrNone;
	TInt numSettings = aSettings.Count();
	for (TInt i = 0; i < numSettings; i++)
		{
		ASSERT(aSettings[i]);
		const TServerSetting& setting = *aSettings[i];
		if (!aMessage.CheckPolicy(GetReadAccessPolicy(setting),aDiagnostic))
			{
			error = KErrPermissionDenied;
			break;
			}
		}
	return error;
	}

/** Returns pointer array of settings whose keys match the partial key and mask. Combines
settings from the persistent list with those in the transaction, with priority given to the
latter, including settings flagged as deleted eliminating the corresponding entry from the 
persistent settings (plus themselves so the final list has no settings flagged as deleted in it).
Can also call this method when not in a transaction.
In case of error, aMatches may contain entries and must be Reset.
*/
TInt CServerRepository::FindSettings(TUint32 aPartialKey, TUint32 aIdMask, RSettingPointerArray& aMatches) const
	{
	TInt error = FindPersistentSettings(aPartialKey, aIdMask, aMatches);
	// try to be most efficient when no transaction changes
	if ((iTransactionSettings.Count() > 0) && IsInActiveReadWriteTransaction() && (KErrNone == error))
		{
		RSettingPointerArray transactionSettings;
		error = FindTransactionSettings(aPartialKey, aIdMask, transactionSettings);
		if (error == KErrNone)
			{
			error = RSettingsArray::Merge(aMatches, transactionSettings);
			}
		transactionSettings.Reset();
		}
	return error;
	}

TInt CServerRepository::TransactionDeleteRangeL(const TClientRequest& aMessage, TUint32& aErrorKey)
	{
	// all write operations now done in a transaction
	ASSERT(IsInActiveReadWriteTransaction());
	TInt error = KErrNone;
	aErrorKey = KUnspecifiedKey;
		
	TUint32 partialKey = aMessage.Int0();
	TUint32 keyMask = aMessage.Int1();
	
	RSettingPointerArray settingsToDelete;
	error = FindSettings(partialKey, keyMask, settingsToDelete);
	CleanupClosePushL(settingsToDelete);
	
	TInt numSettings = settingsToDelete.Count();

	if ((error == KErrNone) && (numSettings == 0))
		{
		error = KErrNotFound;
		aErrorKey = partialKey;
		}
		
	for (TInt i = 0; (i < numSettings) && (error == KErrNone); i++)
		{
		ASSERT(settingsToDelete[i]);
		TServerSetting& settingToDelete = *(settingsToDelete[i]);
		TUint32 key = settingToDelete.Key();
		// must pass write access policies of key
		if (!aMessage.CheckPolicy(GetWriteAccessPolicy(settingToDelete),
			__PLATSEC_DIAGNOSTIC_STRING("CenRep: CServerRepository::DeleteRangeL - Attempt made to delete a setting")))
			{
			error = KErrPermissionDenied;
			aErrorKey = key;
			}
		else
			{
			// delete it
			// Ensure there is a delete placeholder at the location
			if (GetTransactionSetting(key) == &settingToDelete)
				{
				// we are deleting a setting that is already in the transaction list: Flag it as deleted
				settingToDelete.Reset();
				settingToDelete.SetDeleted();
				}
			else
				{
				// create a new placeholder and set as deleted
				TServerSetting newSetting(key);
				newSetting.SetDeleted();
				iTransactionSettings.OrderedInsertL(newSetting);
				}
			}
		}
	
	CleanupStack::PopAndDestroy(&settingsToDelete);

	if ((error != KErrNone) && (error != KErrNotFound))
		{
		FailTransaction(error, aErrorKey);
		}
	return error;
	}

TInt CServerRepository::TransactionMoveL(const TClientRequest& aMessage, TUint32& aErrorKey)
	{
	// all write operations now done in a transaction
	ASSERT(IsInActiveReadWriteTransaction());
	TInt error = KErrNone;
	aErrorKey = KUnspecifiedKey;

	TUint32 sourcePartialKey = aMessage.Int0();
	TUint32 targetPartialKey = aMessage.Int1();	
	TUint32 idMask = aMessage.Int2();
	TUint32 maskedSourcePartialKey = sourcePartialKey & idMask;
	TUint32 maskedTargetPartialKey = targetPartialKey & idMask;
	TUint32 sourceToTarget = maskedSourcePartialKey ^ maskedTargetPartialKey;
	if (sourceToTarget == 0)
		{
		// not moving anywhere: must return now as this trivial case fails with later logic
		return KErrNone;
		}

	RSettingPointerArray sourceSettings;
	error = FindSettings(maskedSourcePartialKey, idMask, sourceSettings);
	CleanupClosePushL(sourceSettings);

	TInt numSettings = sourceSettings.Count();

	if ((error == KErrNone) && (numSettings == 0))
		{
		error = KErrNotFound;
		aErrorKey = sourcePartialKey;
		}

	for (TInt i = 0; (i < numSettings) && (error == KErrNone); i++)
		{
		ASSERT(sourceSettings[i]);
		TServerSetting& sourceSetting = *(sourceSettings[i]);
		TUint32 sourceKey = sourceSetting.Key();
		TUint32 targetKey = sourceKey ^ sourceToTarget;
		TServerSetting* targetSetting = GetSetting(targetKey);
		// must pass both read and write access policies of source key
		if (!aMessage.CheckPolicy(GetReadAccessPolicy(sourceSetting),
			__PLATSEC_DIAGNOSTIC_STRING("CenRep: CServerRepository::MoveL - Attempt made to read a setting")))
			{
			error = KErrPermissionDenied;
			aErrorKey = sourceKey;
			}
		else if (!aMessage.CheckPolicy(GetWriteAccessPolicy(sourceSetting),
			__PLATSEC_DIAGNOSTIC_STRING("CenRep: CServerRepository::MoveL - Attempt made to delete a setting")))
			{
			error = KErrPermissionDenied;
			aErrorKey = sourceKey;
			}
		else if (targetSetting && !targetSetting->IsDeleted())
			{
			// set error to KErrPermissionDenied in preference to KErrAlreadyExists
			if (!aMessage.CheckPolicy(GetWriteAccessPolicy(*targetSetting),
				__PLATSEC_DIAGNOSTIC_STRING("CenRep: CServerRepository::MoveL - Attempt made to create a setting")))
				{
				error = KErrPermissionDenied;
				}
			else
				{
				error = KErrAlreadyExists;
				}
			aErrorKey = targetKey;
			}
		else if (!aMessage.CheckPolicy(GetFallbackWriteAccessPolicy(targetKey),
				__PLATSEC_DIAGNOSTIC_STRING("CenRep: CServerRepository::MoveL - Attempt made to create a setting")))
			{
			error = KErrPermissionDenied;
			aErrorKey = targetKey;
			}
		else
			{
			// move it
			// 1. create a copy at the new location
			if (targetSetting)
				{
				// must be set as deleted and already in the transaction settings so can overwrite
				ASSERT(targetSetting->IsDeleted());
				error = targetSetting->Replace(sourceSetting);
				if (error == KErrNone)
					{
					targetSetting->SetKey(targetKey);
					targetSetting->SetMeta(sourceSetting.Meta() & (~KMetaDefaultValue));
					// setting takes the access policy of the target key
					targetSetting->SetAccessPolicy(GetFallbackAccessPolicy(targetKey));
					}
				}
			else
				{
				TServerSetting newSetting;
				error = newSetting.Replace(sourceSetting);
				if (error == KErrNone)
					{
					newSetting.SetKey(targetKey);
					newSetting.SetMeta(sourceSetting.Meta() & (~KMetaDefaultValue));
					// setting takes the access policy of the target key
					newSetting.SetAccessPolicy(GetFallbackAccessPolicy(targetKey));
					newSetting.PushL(); // only needed for strings
					iTransactionSettings.OrderedInsertL(newSetting);
					newSetting.Pop();	// only needed for strings
					}
				}
			// 2. ensure there is a delete placeholder at the old location
			if (GetTransactionSetting(sourceKey) == &sourceSetting)
				{
				// we are moving a setting that is already in the transaction list: Set it deleted
				sourceSetting.Reset();
				sourceSetting.SetDeleted();
				}
			else
				{
				// create a new placeholder and set as deleted
				TServerSetting newSetting(sourceKey);
				newSetting.SetDeleted();
				iTransactionSettings.OrderedInsertL(newSetting);
				}
			}
		}
		
	CleanupStack::PopAndDestroy(&sourceSettings);

	if ((error != KErrNone) && (error != KErrNotFound))
		{
		FailTransaction(error, aErrorKey);
		}
	return error;
	}

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

#include <e32def.h>
#include <e32property.h>
#include <s32file.h>
#include <connect/sbdefs.h>
#include "srvres.h"
#include "cachemgr.h"
#include "srvparams.h"
#include "shrepos.h"
#include "sessnotf.h"
#include "backup.h"

#define UNUSED_VAR(a) a = a

_LIT (KBackupFileName, "BACKUP") ;
_LIT (KRestoreFileName, "RESTORE") ;
_LIT(KBackupFileExt, ".bak");
_LIT(KRestoreFileExt, ".rst");


TRepositoryBackupState CRepositoryBackupClient::iBackupStatus ;


//
// Backup stream index class - Used to hold association between a UID (in
// our case the UID of a repository) and a stream ID - Can't use CStreamDictionary
// because that only lets you retrieve stream IDs by a (previously known) UID rather
// than iterate through its contentsretrieving UID/StreamID pairs...
// 


//
// CRepositoryBackupStreamIndex::AddL
//
// Add a new repository UID and stream ID pair to the index
void CRepositoryBackupStreamIndex::AddL(TUid aUid, TStreamId aSettingStreamId, TStreamId aDeletedSettingsStreamId) 
	{
	TRepositoryBackupStreamIndexElement newIndexElement;
	newIndexElement.Set(aUid,aSettingStreamId, aDeletedSettingsStreamId) ;
	iStreamIndex.AppendL(newIndexElement);
	}


// 
// CRepositoryBackupStreamIndex::GetNext
//
// Iterate through the index retrieving the next Reposirory UID and Stream ID
// pair.
//
TInt CRepositoryBackupStreamIndex::GetNext(TUid& aUid, TStreamId& aSettingsStreamId, TStreamId& aDeletedSettingsStreamId)
	{
	TInt error = KErrNone ;
	if (iIndex < iStreamIndex.Count())
		{
		iStreamIndex[iIndex++].Get(aUid, aSettingsStreamId, aDeletedSettingsStreamId) ;
		}
	else
		{
		error = KErrNotFound ;
		}
	return error ;
	}


//
// Backup client class.
//
// Has Active object functionality to monitor the state of the publish and subscribe 
// flags associated with backup and restore and also implements MActiveBackupDataClient
// to perform active backup according to the "proxy data holder" model.
//



//
// Usual 2 phase construction factory NewL NewLC classes
//	
CRepositoryBackupClient* CRepositoryBackupClient::NewLC(RFs& aFs) 
	{
	CRepositoryBackupClient* me = new(ELeave)CRepositoryBackupClient(aFs);
	CleanupStack::PushL(me) ;
	me->ConstructL() ;
	return me ;
	}
	
	
CRepositoryBackupClient* CRepositoryBackupClient::NewL(RFs& aFs) 
	{
	CRepositoryBackupClient* me = CRepositoryBackupClient::NewLC(aFs) ;
	CleanupStack::Pop(me) ;
	return me ;
	}
	
		
		
// 
// Constructor - doesn't really do anything!
//
CRepositoryBackupClient::CRepositoryBackupClient(RFs& aFs) : CActive(EPriorityStandard), iFs(aFs), iRomScanDone(EFalse)
	{
	}


// 
// Phase 2 constructor
//	
void CRepositoryBackupClient::ConstructL()
	{
	// Create repository object
	iRepository = new(ELeave) CServerRepository;
	
	// Notifier needed to open repositories.		
	iNotifier = new(ELeave)CSessionNotifier ;

	// Attach to Backup/Restore Pub/Sub property.	
	User::LeaveIfError(iBackupRestoreProperty.Attach(KUidSystemCategory, KUidBackupRestoreKey)) ;
	
	// Add ourself to the active scheduler
	CActiveScheduler::Add(this) ;
	
	// Initialise backup/restore status
	iBackupStatus = ENoBackupActivty ;
	
	// Set active and request notification of changes to backup/restore
	// Pub/Sub property.
	StartL() ;
	}



//
// Destructor
//
CRepositoryBackupClient::~CRepositoryBackupClient()
	{
	Cancel();
	
	iBackupRestoreProperty.Close() ;
	
	if (iRepository)
		{
		iRepository->Close(*iNotifier) ;
		delete(iRepository);
		}
		
	if (iNotifier)
		delete(iNotifier) ;
	
	if (iActiveBackupClient)
		delete(iActiveBackupClient) ;	
	}




// 
// DoCancel - mandatory for active objects.
//	
void CRepositoryBackupClient::DoCancel()
	{
	iBackupRestoreProperty.Cancel() ;
	}



//
// RunError
//
TInt CRepositoryBackupClient::RunError(TInt aError)
	{
	UNUSED_VAR(aError);
	return KErrNone;
	}




//
// Test BUR Pub/Sub property set status, and notify BUR that we're
// ready to go as appropriate. 
//
void CRepositoryBackupClient::TestBURstatusL(void)
	{
	TInt BURstatus ;

	if (iBackupRestoreProperty.Get(BURstatus) != KErrNotFound)
		{
		BURstatus &= KBURPartTypeMask ;
		switch (BURstatus)
			{
			case EBURUnset:	// State not yet set. Treat as no backup/restore in progress.
			case EBURNormal:
				// No backup or restore in progress. Probably
				// means we've just completed an operation?
				iBackupStatus = ENoBackupActivty ;
				
				// Back to normal, so enable cache
				TServerResources::iCacheManager->EnableCache();				
				// delete the CActiveBackupClient
				if (iActiveBackupClient)
					{
					delete iActiveBackupClient ;
					iActiveBackupClient = NULL ;
					}
				break ;
				
			case EBURBackupFull :
			case EBURBackupPartial :
				// We don't distinguish between full and partial backups
				// as the Backup engine will give us UIDs for all the
				// repository data owners that want their stuff backed up
				// anyway.
				
				// We don't want cache activity during backup
				TServerResources::iCacheManager->DisableCache();
				
				// Any (and all!) repositories which have been opened in the
				// course of system boot and normal operation will have been
				// added to TServerResources::iOwnerIdLookUpTable as they were
				// opened but there may well be repositories which need backing
				// up and haven't yet been opened so we need to make sure that
				// the lookup table is complete.
				CompleteOwnerIdLookupTableL();

				// Register with BUR engine
				if (!iActiveBackupClient)
					{
					iActiveBackupClient = CActiveBackupClient::NewL(this) ;
					}	
				iActiveBackupClient->ConfirmReadyForBURL(KErrNone);
				iBackupStatus = EBackupInProgress ;
				break ;
				
			case EBURRestoreFull :
			case EBURRestorePartial :
				// We don't distinguish between full and partial restore
				// either!
				
				// We don't want cache activity during restore either!
				TServerResources::iCacheManager->DisableCache();
				
				// Register with BUR engine
				if (!iActiveBackupClient)
					{
					iActiveBackupClient = CActiveBackupClient::NewL(this) ;
					}
				iActiveBackupClient->ConfirmReadyForBURL(KErrNone);
				iBackupStatus = ERestoreInProgress ;
				break ;
							
			}
		}
	}


//
// Request notification of changes in BUR Pub/Sub status
//
void CRepositoryBackupClient::StartL()
	{
	if (!IsActive())
		{
		TestBURstatusL();	
		NotifyChange();
		}
	}


//
// Request notification of changes in BUR Pub/Sub status
//
void CRepositoryBackupClient::NotifyChange()
	{
	// Watch for changes in the property state. 
	iBackupRestoreProperty.Subscribe(iStatus) ;
	SetActive();
	}

	
//
// Handle changes of backup state through publish/subscribe
//
void CRepositoryBackupClient::RunL()
	{	
	NotifyChange() ;
	TestBURstatusL();
	}


//
// We can't estimate data size without A) having the SID of the data owner who's data
// is to be backed up and B) going through the whole process of preparing the backup.
//
// The only sensible thing we can do is return an arbitrary value!
//
TUint CRepositoryBackupClient::GetExpectedDataSize(TDriveNumber /* aDrive */)
	{
	return KArbitraryNumber ;
	}	




//
// Called by BUR engine to request a chunk of backup data.
//	
void CRepositoryBackupClient::GetBackupDataSectionL(TPtr8& aBuffer, TBool& aFinished)
	{
	const TInt chunkSize = aBuffer.MaxSize() ;

	aFinished = EFalse ;
		
	// Pass a chunk of our prepared backup data in aBuffer
	User::LeaveIfError(iFile.Read(aBuffer, chunkSize)) ;
	TInt bytesRead = aBuffer.Length() ;

	// Check to see if this was the last chunk of data.
	if (bytesRead < chunkSize)
		{
		// Set "finished" flag so that BUR knows we're through...
		aFinished = ETrue ;
		
		// ...and then tidy up by closing and deleting the backup file.
		iFile.Close() ;
		TParse backupFilePath ;
		User::LeaveIfError(backupFilePath.Set(KBackupFileName, TServerResources::iBURDirectory, &KBackupFileExt));
		iFs.Delete(backupFilePath.FullName()) ;
		}
	}
	
	
	



//
// CRepositoryBackupClient::RestoreComplete
//
// Called when a Complete set of backup data has been received and written
// to a file. We now need to open the file as a stream store, get the
// index (list of repository UID and corresponding stream ID pairs, and then
// reconstruct and save each repository in turn.
// 
void CRepositoryBackupClient::RestoreComplete(TDriveNumber /* aDrive */)
	{
	}


	
//
// CRepositoryBackupClient::RestoreCompleteL
//
// Does the actual work of reconstructing repositories from backup data
//
//
void CRepositoryBackupClient::RestoreCompleteL()
	{
	// All restore data recived so we can now recreate the repositories from the
	// backup store
	// Attempt to open the restore file as a CDirectFileStore
	TParse restoreFilePath ;
	User::LeaveIfError(restoreFilePath.Set(KRestoreFileName, TServerResources::iBURDirectory, &KRestoreFileExt));
	CDirectFileStore* store = CDirectFileStore::OpenLC (iFs,restoreFilePath.FullName(), EFileRead|EFileShareReadersOnly);
	if (store->Type()[0] != KDirectFileStoreLayoutUid)
		{
		// store wasn't quite what we were expecting - can't return an error, can't leave
		// so all we can do is close the file, tidy up as best we can, and bail out!!!! 
		CleanupStack::PopAndDestroy(store);
		iFs.Delete(restoreFilePath.FullName()) ;
		User::Leave(KErrCorrupt);
		}		



	// Get the root stream and attempt to read a backup file header from it
	TStreamId rootStreamId = store->Root() ;
	RStoreReadStream rootStream ;
	RStoreReadStream indexStream ;
	rootStream.OpenLC(*store, rootStreamId);
	TRepositoryBackupStreamHeader header ;
	TRAPD(err, header.InternalizeL(rootStream)) ;
	
	// Check for a valid header by checking that the UID matches the UID
	// of Central Repository and that the version number is sane.
	if (err == KErrNotSupported)
	    {
   		// Not a valid header - assume it's an old style backup stream,
		// set extensions supported to none, set index stream to be
		// root stream and reset read pointer to beginning.
		iBackupExtensionsSupported = ENoBackupExtensions ;

		CleanupStack::PopAndDestroy(&rootStream) ;
		CleanupStack::PopAndDestroy(store) ;

		// Try re-opening as old-style backup stream with index
		// as root stream.		
		CDirectFileStore* store = CDirectFileStore::OpenLC (iFs,restoreFilePath.FullName(), EFileRead|EFileShareReadersOnly);
		indexStream.OpenLC(*store, rootStreamId) ;
	    }
	else
		{
	    // Got a valid header. Check for extensions supported by this
	    // stream and get stream to read index from
	    CleanupStack::PopAndDestroy(&rootStream) ;
	    iBackupExtensionsSupported = header.getBackupExtensionsSupported();	
	    TStreamId indexStreamId = header.getIndexStreamId() ;
	    indexStream.OpenLC (*store, indexStreamId) ; 		
		}
	
	CRepositoryBackupStreamIndex *restoreStreamIndex = CRepositoryBackupStreamIndex::NewLC();	
	restoreStreamIndex->InternalizeL(indexStream, iBackupExtensionsSupported);

	
	// Iterate through index and attempt restore of each repository stream
	// we find in it.
	restoreStreamIndex->Reset() ;	
	TUid repositoryUid ;
	TStreamId settingsStreamId ;
	TStreamId deletedSettingsStreamId ;
	
	while (restoreStreamIndex->GetNext(repositoryUid, settingsStreamId, deletedSettingsStreamId) == KErrNone)
		{
		iRepository->OpenL(repositoryUid, *iNotifier, EFalse);
		iRepository->FailAllTransactions();
		iRepository->RestoreRepositoryContentsL(*store, settingsStreamId, deletedSettingsStreamId);
		iRepository->CommitChangesL();
		iRepository->Close(*iNotifier) ;
		}

	CleanupStack::PopAndDestroy(restoreStreamIndex) ;
	CleanupStack::PopAndDestroy(&indexStream);
	CleanupStack::PopAndDestroy(store);
	iFs.Delete(restoreFilePath.FullName()) ;
	}
	
//
// CRepositoryBackupClient::CompleteOwnerIdLookupTableL
//
// Open each repository in TServerResources::iDataDirectory.
// Save the Rep UID and Owner Id of the rep to be used by
// InitialiseGetProxyBackupDataL.
void CRepositoryBackupClient::CompleteOwnerIdLookupTableL()
	{

	// Read contents of persist, install, and ROM directories and
	// use them to build a list of repository candidates.
	RArray <TUint32> repositoryList ;
	CleanupClosePushL(repositoryList) ;

	for (TBackupDirectoryScan scanDir = EScanRom; scanDir <= EScanPersist; scanDir = (TBackupDirectoryScan)(scanDir+1))
		{
		TPtrC directoryName ;
		switch (scanDir)
			{
				case EScanRom :
					directoryName.Set(TServerResources::iRomDirectory->Des()) ;
					break ;
					
				case EScanInstall :
					directoryName.Set(TServerResources::iInstallDirectory->Des()) ;
					break ;
					
				case EScanPersist :
					directoryName.Set(TServerResources::iDataDirectory->Des()) ;
					break ;				
			}
			
		CDir* directory;
		User::LeaveIfError(iFs.GetDir(directoryName, KEntryAttNormal, ESortNone, directory));		
		CleanupStack::PushL(directory);
		const TInt directoryFileCount = directory->Count();
		for (TInt i = 0; i < directoryFileCount; i++)
			{
			// Attempt to extract a repository UID from directory entry
			TUid uid;	
			if (KErrNone == TServerResources::GetUid(const_cast<TEntry&>((*directory)[i]), uid))
    			{
    			repositoryList.InsertInUnsignedKeyOrder(uid.iUid) ;

    			}
			}
		CleanupStack::PopAndDestroy(directory);
		}
		
	// Open all repositories in turn. Save repository UID and owner ID
	// in lookup table.
	for(TInt i = 0; i<repositoryList.Count(); i++)
		{
		// Look to see if this repository already has an entry in the owner ID lookup table
		if ( TServerResources::FindOwnerIdLookupMapping(repositoryList[i]) == KErrNotFound)
			{
			// Need to TRAP here as otherwise if ANY repository fails to open
			// (e.g. due to corruption) it would cause the entire backup to
			// fail
			TRAPD(err, iRepository->OpenL(TUid::Uid(repositoryList[i]), *iNotifier));
			if (err == KErrNoMemory)
				{
				User::Leave(err) ;
				}
			
			else if (err == KErrNone)
				{
				// The act of opening a repository will cause it to add itself to the
				// Repository/Owner UID mapping table so we don't need to do anything
				// and can close it immediately!
				iRepository->Close(*iNotifier);
				}
			}
		}
		
	CleanupStack::PopAndDestroy() ; // repositoryList	
	}

// CRepositoryBackupClient::InitialiseGetProxyBackupDataL
//
// Prepare data to be backed up. We get the Sid/Uid of the entity whos data
// is to be backed up. What we do is to open each repository in turn (identified
// by directory listing), check its owner, and if it matches the Sid/Uid we've
// been given by secure backup externalise it to a stream within a file store.
//
void CRepositoryBackupClient::InitialiseGetProxyBackupDataL(TSecureId aSID, TDriveNumber /* aDrive */) 
	{
	// Prepare data for backup.

	// Create file store
	TParse backupFilePath ;
	User::LeaveIfError(backupFilePath.Set(KBackupFileName, TServerResources::iBURDirectory, &KBackupFileExt));
	CDirectFileStore* store = CDirectFileStore::ReplaceLC(iFs, backupFilePath.FullName(),
	                                                      (EFileWrite | EFileShareExclusive));
	const TUid uid2  = KNullUid ;	                                                     
	store->SetTypeL(TUidType(KDirectFileStoreLayoutUid, uid2, KServerUid3)) ; 
	
	// Create a Backup Stream index
	CRepositoryBackupStreamIndex* backupStreamIndex = CRepositoryBackupStreamIndex::NewLC();	
	  
	// Find the reps owned by aSID
	for(TInt i = 0; i < TServerResources::iOwnerIdLookUpTable.Count(); ++i)
		{
		const TOwnerIdMapping& lookupTableEntry = TServerResources::iOwnerIdLookUpTable[i];
		
		if ( lookupTableEntry.iOwner == aSID )
			{
			// Found one match, open the repository and externalise content.
			TUid uid = TUid::Uid(lookupTableEntry.iRepUid);
			TRAPD(err,iRepository->OpenL(uid, *iNotifier));
			if (err == KErrNoMemory)
				{
				User::Leave(err) ;
				}
			else if (err == KErrNone)
				{			
				iRepository->FailAllTransactions();
				// Add to store index and externalise
				// repository contents
				TStreamId settingStreamId ;
				TStreamId deletedSettingsStreamId ;
				iRepository->StoreRepositoryContentsL(*store, settingStreamId, deletedSettingsStreamId);
				backupStreamIndex->AddL(uid, settingStreamId, deletedSettingsStreamId) ;
				// Close repository.
				iRepository->Close(*iNotifier);
				}
			}
		}
	// Write the stream index/dictionary as root stream within the store
	// so we can access it when we do a restore later on
	RStoreWriteStream indexStream ;
	TStreamId indexStreamId = indexStream.CreateLC(*store) ;
	backupStreamIndex->ExternalizeL(indexStream) ;
	indexStream.CommitL() ;
	CleanupStack::PopAndDestroy(&indexStream) ;
	CleanupStack::PopAndDestroy (backupStreamIndex) ;
		
	// Create the Header and write it as the root stream within the store
	// so we can access it when we do a restore later on
	TRepositoryBackupStreamHeader header (indexStreamId) ;
	RStoreWriteStream rootStream ;
	TStreamId rootStreamId = rootStream.CreateLC(*store) ;
	header.ExternalizeL(rootStream) ;
	rootStream.CommitL() ;
	
	CleanupStack::PopAndDestroy(&rootStream) ;
	store->SetRootL(rootStreamId);
	store->CommitL();
	CleanupStack::PopAndDestroy(store) ; 
	
	// Attempt to open file containing store ready to read back and send to
	// BUR engine as a stream of bytes.
	User::LeaveIfError(iFile.Open(iFs, backupFilePath.FullName(), (EFileRead | EFileShareExclusive))) ;
	}




//
// CRepositoryBackupClient::InitialiseRestoreProxyBaseDataL
//
// Called when secure backup is about to start sending restore data.
//
void CRepositoryBackupClient::InitialiseRestoreProxyBaseDataL(TSecureId aSID, TDriveNumber /* aDrive*/)
	{
	// prepare for restore - Don't think we need to do anything here except prepare
	// data structures to receive incoming data
	
	// Save SID so we can check that it corresponds with the owner information 
	// in the restored data.
	iSid = aSID ;
	
	// Open file to receive restored data
	TParse restoreFilePath ;
	User::LeaveIfError(restoreFilePath.Set(KRestoreFileName, TServerResources::iBURDirectory, &KRestoreFileExt));
	User::LeaveIfError(iFile.Replace (iFs, restoreFilePath.FullName(), (EFileWrite | EFileShareExclusive)));
	}




//
// CRepositoryBackupClient::RestoreBaseDataSectionL
//
// Called when secure backup has a chunk of restore data for us. Last data
// segment identified by aFinished.
//
void CRepositoryBackupClient::RestoreBaseDataSectionL(TDesC8& aBuffer, TBool aFinished)
	{
	// Receive a chunk of restore data in aBuffer 
	User::LeaveIfError(iFile.Write (aBuffer)) ;
	if (aFinished)
		{
		iFile.Close() ;
		
		// All restore data recived so we can now recreate the repositories from the
		// backup store	
		RestoreCompleteL();	
		}
	}
	
void CRepositoryBackupClient::TerminateMultiStageOperation()
	{
	// Backup/Restore operation aborted!
	// Tidy up all temporary data.
	}


//
// CRepositoryBackupClient::GetDataChecksum
//
// Not required and we don't implement it.
//
TUint CRepositoryBackupClient::GetDataChecksum(TDriveNumber /* aDrive */) 
	{
	return KArbitraryNumber;
	}

// 
// CRepositoryBackupClient::GetSnapshotDataL
//
// Only required for incremental backup (which we don't support
//
void CRepositoryBackupClient::GetSnapshotDataL(TDriveNumber /* aDrive */, TPtr8& /* aBuffer */, TBool& /* aFinished */) 
	{
	User::Leave(KErrNotSupported) ;
	}

// 
// CRepositoryBackupClient::InitialiseGetBackupDataL
//
// Used by "normal" active backup to prepare data - we use
// InitialiseRestoreProxyBaseDataL so this shouldn't be called!
//	
void CRepositoryBackupClient::InitialiseGetBackupDataL(TDriveNumber /* aDrive */)
	{
	User::Leave(KErrNotSupported) ;
	}
	


void CRepositoryBackupClient::InitialiseRestoreBaseDataL(TDriveNumber /* aDrive */)
	{
	// Check this! I Don't think this method should get called as we're a proxy
	// and so implement InitialiseGetProxyBackupDataL!! 
	User::Leave(KErrNotSupported) ;		
	}
	



void CRepositoryBackupClient::InitialiseRestoreIncrementDataL(TDriveNumber /* aDrive */)
	{
	// Check this! I Don't think this method should get called as we're a proxy
	// so don't do incremental backup!! 
	User::Leave(KErrNotSupported) ;			
	}



void CRepositoryBackupClient::RestoreIncrementDataSectionL(TDesC8& /* aBuffer */, TBool /* aFinished */)
	{
	// Check this! I Don't think this method should get called as we're a proxy
	// so don't do incremental backup!! 
	User::Leave(KErrNotSupported) ;		
	}


//
// Incremental backup isn't supported for the proxy data holder model so this
// method should never be called.
//
// Not acceptable to leave even though it's an ...L function which we don't implement
void CRepositoryBackupClient::AllSnapshotsSuppliedL() 
	{
	;
	}



//
// Incremental backup not supported
//	
void CRepositoryBackupClient::ReceiveSnapshotDataL(TDriveNumber /* aDrive */, TDesC8& /* aBuffer */, TBool /* aLastSection */) 
	{
	User::Leave(KErrNotSupported) ;
	}



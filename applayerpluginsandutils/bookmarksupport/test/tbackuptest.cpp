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

#include <bookmarkdatabase.h>
#include "testutils.h"
#include "tbookmarktests.h"

#include <e32test.h>
#include <f32file.h>
#include <badesca.h>
#include <connect/sbdefs.h>
#include <connect/sbeclient.h>

#include "srvrepos.h"
#include "srvres.h"
#include "cachemgr.h"
#include "backup.h"

#define TEST2(aValue, aExpected) ::CheckL(aValue, aExpected)

_LIT ( KMyFolder, "BackupFolder" );
_LIT(KTxtBookmark1, "Bookmark 1");
_LIT8(KBookmarkUri, "www.ft.com");
_LIT(KTitle, "Bookmark Backup Restore Test");
LOCAL_D RTest gTest(KTitle());

LOCAL_D CTestWrapper* gTestWrapper;
CActiveScheduler* globalAS;

CRepositoryBackupClient* backupClient;

CDesC8ArraySeg* originalData1;
CDesC8ArraySeg* originalData2;

_LIT_SECURE_ID( SecureId1, 0x102078CB );

_LIT( K10205af8, "10205AF8.txt" );
_LIT( K10205af9, "10205AF9.txt" );
_LIT( K10205afa, "10205AFA.txt" );
_LIT( K10205afb, "10205AFB.txt" );
	
const TUid KUidBookmarkDatabaseRepository   = { 0x10205AF8 };
const TUid KUidFolderRepository             = { 0x10205AF9 };
const TUid KUidBookmarkRepository           = { 0x10205AFA };
const TUid KUidIconRepository               = { 0x10205AFB };
	

LOCAL_C	void ResetRepositoryL ( TUid aUid )
	{
	CRepository* repository = CRepository::NewL ( aUid );
	CleanupStack::PushL ( repository );
	User::LeaveIfError ( repository->Reset() );	
	CleanupStack::PopAndDestroy ( repository );
	}



/**
Creates a database.
Creates a folder.
Creates a bookmark.
*/
LOCAL_C void CreateBookmarkInFolderL()
	{
	
	//This will ensure that the old repositories are deleted
	ResetRepositoryL ( KUidBookmarkDatabaseRepository );
	ResetRepositoryL ( KUidFolderRepository );
	ResetRepositoryL ( KUidBookmarkRepository );
	ResetRepositoryL ( KUidIconRepository );
	
	// Open the database
	RBkDatabase database;
	database.OpenL();
	CleanupClosePushL(database);
			
	//Create a folder
	RBkFolder folder = database.CreateFolderL(KMyFolder);
	CleanupClosePushL(folder);
		
	// Create a bookmark in the new folder
	RBkBookmark bmrk = database.CreateBookmarkL(&folder);
	CleanupClosePushL(bmrk);
		
	// Set some bookmark properties
	bmrk.SetTitleL(KTxtBookmark1);
	bmrk.SetUriL(KBookmarkUri);
		
	// Close the bookmark
	CleanupStack::PopAndDestroy(&bmrk);
	
	// Close the folder
	CleanupStack::PopAndDestroy(&folder);
	
	// Commit the changes to the database and close it
	database.CommitL();
	CleanupStack::PopAndDestroy(&database);
		
	gTest.Printf(_L("\n*** Created database, folder, bookmark ***\n"));		
	}
	
	
/**
Deletes all the files in the temporary repository folder.
The temporary repository folder used is 10281FE5.
*/
LOCAL_C void DeleteFilesL()
	{
	_LIT( KOldInstallFiles, "c:\\private\\10281FE5\\*.*" );
	_LIT( KOldPersistFiles, "c:\\private\\10281FE5\\persists\\*.*" );
	
	CFileMan* fm = CFileMan::NewL( TServerResources::iFs );
	CleanupStack::PushL( fm );

	TInt r = fm->Delete( KOldInstallFiles );
	if ( r != KErrNone && r != KErrNotFound && r != KErrPathNotFound )
		User::Leave(r);
	r = fm->Delete( KOldPersistFiles );
	if ( r != KErrNone && r != KErrNotFound && r != KErrPathNotFound )
		User::Leave(r);
	
	gTest.Printf(_L("\n*** Deleted files from temporary repository ***\n"));
	CleanupStack::PopAndDestroy( fm );
	}
		
		
/**
Deletes all the files in the original bookmarks repository folder.
The bookmarks repository folder is 10202be9
*/
LOCAL_C void DeleteRepositoryFilesL()
	{
	_LIT( KOldInstallFiles, "c:\\private\\10202be9\\*.cre" );
	_LIT( KOldPersistFiles, "c:\\private\\10202be9\\persists\\*.cre" );
	
	CFileMan* fm = CFileMan::NewL( TServerResources::iFs );
	CleanupStack::PushL( fm );

	TInt r = fm->Delete( KOldInstallFiles );
	if ( r != KErrNone && r != KErrNotFound && r != KErrPathNotFound )
		User::Leave(r);
	r = fm->Delete( KOldPersistFiles );
	if ( r != KErrNone && r != KErrNotFound && r != KErrPathNotFound )
		User::Leave(r);
	
	gTest.Printf(_L("\n*** Deleted files from original repository ***\n"));
	CleanupStack::PopAndDestroy( fm );
	}
			

LOCAL_C void CheckL( TInt aValue, TInt aExpected)
	{
	if ( aValue != aExpected )
		{
		DeleteFilesL();
		RDebug::Print( _L( "*** Expected error: %d, got: %d\r\n"), aExpected, aValue );
		}
	}

/**
Does a copy of the config files from the ROM area to the temporary repository folder.
The bookmark component has 4 config files 10205AF8.txt, 10205AF9.txt, 10205AFA.txt and
10205AFB.txt.
*/	
LOCAL_C void InstallFileSetL(const TDesC& afilename)
	{
	_LIT( KFileSrcPath, "\\private\\10202be9\\" );
	_LIT( KInstallFileDestPath, "\\private\\10281FE5\\" );
	_LIT( KPersistFileDestPath, "\\private\\10281FE5\\persists\\" );
	_LIT( KDriveC, "c:" );
	_LIT( KDriveZ, "z:" );

	TBuf<KMaxFileName> dest;
	TBuf<KMaxFileName> src;
	TBuf<KMaxFileName> src2;
	TBuf<KMaxFileName> dest2;
	TInt r;

	CFileMan* fm = CFileMan::NewL( TServerResources::iFs );
	CleanupStack::PushL( fm );

	dest.Copy( KDriveC );
	dest.Append( KInstallFileDestPath );
	dest.Append( afilename );
	r = fm->Delete( dest );
	if ( r != KErrNone && r != KErrNotFound && r != KErrPathNotFound )
	User::Leave( r );
	r = TServerResources::iFs.MkDirAll( dest );
	if ( r != KErrNone && r != KErrAlreadyExists )
	User::Leave( r );
		
	src.Copy( KDriveZ );
	src.Append( KFileSrcPath );
	src.Append( afilename );
	User::LeaveIfError( fm->Copy( src, dest ) );
	r = fm->Attribs( dest, KEntryAttArchive, KEntryAttReadOnly, TTime( 0 ), CFileMan::ERecurse );
	TEST2( r, KErrNone );
	
	dest2.Copy( KDriveC );
	dest2.Append( KPersistFileDestPath );
	dest2.Append( afilename  );
	r = fm->Delete( dest2 );
	if ( r != KErrNone && r != KErrNotFound && r != KErrPathNotFound )
	User::Leave( r );
	r = TServerResources::iFs.MkDirAll( dest2 );
	if ( r != KErrNone && r != KErrAlreadyExists )
	User::Leave( r );
	src2.Copy( KDriveZ );
	src2.Append( KFileSrcPath );
	src2.Append( afilename );
	User::LeaveIfError( fm->Copy( src2, dest2 ) );
	r = fm->Attribs( dest2, KEntryAttArchive, KEntryAttReadOnly, TTime( 0 ), CFileMan::ERecurse );
	TEST2( r, KErrNone );
	
	CleanupStack::PopAndDestroy( fm );
	}


/**
Does the backup of the repository.
*/
LOCAL_C void BackupRepositoryL( CDesC8ArraySeg* aDataStream )
	{
	backupClient->CompleteOwnerIdLookupTableL();
	
	//The SecureId1 has to be the same as the value for [owner] tag in each of the
	//config files
	backupClient->InitialiseGetProxyBackupDataL( SecureId1, EDriveC );
	
	HBufC8* transferBuffer = HBufC8::NewL( 128 );
	CleanupStack::PushL( transferBuffer );
	TPtr8 bufferToSend = transferBuffer->Des();
	TBool finished = EFalse;

	aDataStream->Reset();
	
	do 
		{
		bufferToSend.Zero();
		backupClient->GetBackupDataSectionL( bufferToSend, finished );
		aDataStream->AppendL( bufferToSend );
		} while ( !finished );
		
	gTest.Printf(_L("\n*** Backup process completed successfully ***\n"));	
	CleanupStack::PopAndDestroy( transferBuffer );
	}


/**
Does a restore of the backed up data.
@param aDataStream
	Carries the backed up data.	
*/
LOCAL_C void RestoreRepositoryL( CDesC8ArraySeg* aDataStream )
	{
	backupClient->InitialiseRestoreProxyBaseDataL( SecureId1, EDriveC );
	
	HBufC8* transferBuffer = HBufC8::NewL( 128 );
	CleanupStack::PushL( transferBuffer );
	TPtr8 bufferToSend = transferBuffer->Des();

	TBool finished = EFalse;
	TInt count = aDataStream->Count();
	TInt element = 0;
	
	for ( element = 0; element < count; element++ ) 
		{
		bufferToSend.Zero();
		bufferToSend.Append( (*aDataStream)[ element ] );
		if ( element + 1 == count )
			finished = ETrue;
		backupClient->RestoreBaseDataSectionL( bufferToSend, finished );
		}
	backupClient->RestoreComplete( EDriveC );
		
	gTest.Printf(_L("\n*** Restoration completed successfully ***\n"));
	CleanupStack::PopAndDestroy( transferBuffer );
	}


/**
Initializes the descriptors required for the backup and restore process
*/
LOCAL_C void InitializationLC()    
    {
    originalData1 = new(ELeave)CDesC8ArraySeg( 128 );
	CleanupStack::PushL( originalData1 );
	originalData2 = new(ELeave)CDesC8ArraySeg( 128 );
	CleanupStack::PushL( originalData2 );
	}
		
		
/**
Cleans up the used resources
*/
LOCAL_C void CleanupL()
    {
    CleanupStack::PopAndDestroy( originalData2 );
    CleanupStack::PopAndDestroy( originalData1 );
    }


/**
Move the restored (.cre) files from the temporary repository folder to the original repository 
folder.
*/
LOCAL_C void MoveFilesToOriginalRepositoryL()
{
	_LIT( KFileSrcPath, "\\private\\10281FE5\\persists\\" );
	_LIT( KFileDestPath, "\\private\\10202be9\\persists\\" );
	_LIT( KDriveC, "c:" );
	_LIT( KAllFiles, "*.*");
	
	TBuf<KMaxFileName> dest;
	TBuf<KMaxFileName> src;
	TInt r;
	
	CFileMan* fm = CFileMan::NewL( TServerResources::iFs );
						CleanupStack::PushL( fm );

	dest.Copy( KDriveC );
	dest.Append( KFileDestPath );
	dest.Append( KAllFiles );
	r = TServerResources::iFs.MkDirAll( dest );
	if ( r != KErrNone && r != KErrAlreadyExists )
	User::Leave( r );
	src.Copy( KDriveC );
	src.Append( KFileSrcPath );
	src.Append( KAllFiles );
	User::LeaveIfError( fm->Copy( src, dest ) );
	r = fm->Attribs( dest, KEntryAttArchive, KEntryAttReadOnly, TTime( 0 ), CFileMan::ERecurse );
	TEST2( r, KErrNone );
	
	gTest.Printf(_L("\n*** Restored files to original repository ***\n"));
	CleanupStack::PopAndDestroy( fm );
}


LOCAL_D TBool IsItemInFolderL(const TDesC& aName, RBkFolder& aFolder)	
	{
	RBkNode item;
	TBool found = EFalse;
	TInt index = aFolder.Count() - 1;
	for (;index >= 0; --index)
		{
		item = aFolder.OpenItemL(index);
		CleanupClosePushL(item);
		const TDesC& title = item.Title();
		if (title.Compare(aName) == 0)
			{
			found = ETrue;
			}
		CleanupStack::PopAndDestroy(&item);
		if (found)
			{
			break;
			}
		}
	return found;
	}
	

/**
Checks if the Repository is restored along with the foleder and the bookmark created
previously
*/
LOCAL_C void CheckRepositoryL()
{
	RBkDatabase database1;
	database1.OpenL();
	CleanupClosePushL(database1);
	
	RBkFolder folder;
	TRAPD(err1, folder = database1.OpenFolderL(KMyFolder));
	gTestWrapper->TEST(err1 == KErrNone);
	CleanupClosePushL(folder);
	
	gTestWrapper->TEST(IsItemInFolderL(KTxtBookmark1, folder));
	
	CleanupStack::PopAndDestroy(&folder);
	CleanupStack::PopAndDestroy(&database1);
}



/**
Creates a bookmark database. Cretes a temporary repository folder with the repositories in it. 
Does a backup and a restore to the temporary repository and then to the original repository and 
deletes the temporary repository.
*/
LOCAL_C void INC081573L()
	{
	// create and install the active scheduler we need
	CActiveScheduler* s = new(ELeave) CActiveScheduler;
	CleanupStack::PushL( s );
	CActiveScheduler::Replace( s );
		
	//Create folder and bookmark
	CreateBookmarkInFolderL();
	
	backupClient = CRepositoryBackupClient::NewLC(TServerResources::iFs);
		
	TServerResources::iCacheManager->DisableCache();
	
	gTest.Printf(_L("\n*** File installation started ***\n"));
	
	// Install known files
	InstallFileSetL(K10205af8);
	InstallFileSetL(K10205af9);
	InstallFileSetL(K10205afa);
	InstallFileSetL(K10205afb);
	
	gTest.Printf(_L("\n*** File installation completed successfully ***\n"));
	
	// backup known files, 
	BackupRepositoryL( originalData1 );

	// delete files
	DeleteFilesL();
	
	//delete repository files
	DeleteRepositoryFilesL();
	
	// restore when other files do not exist
	RestoreRepositoryL( originalData1 );
	
	//move files back to repository
	MoveFilesToOriginalRepositoryL();
	
	//delete the temporary repository
	DeleteFilesL();
	
	CleanupStack::PopAndDestroy( backupClient );
	
	// Cleanup the scheduler
	CActiveScheduler::Replace( globalAS );
	CleanupStack::PopAndDestroy( s );
	
	CheckRepositoryL();
	}


/**
Starts the backup and restore tests
*/
void DoBackupTestL()
	{
	InitializationLC();
	
	INC081573L();
	
	TServerResources::iOwnerIdLookUpTable.Reset();
	
	CleanupL();
	}


LOCAL_C void MainL()
	{
	__UHEAP_MARK;
	gTest.Start( _L( "\n*** Backup and restore tests ***\n" ) );
	
	gTest.Printf(_L("@SYMTestCaseID IWS-APPPROTOCOLS-BOOKMARKS-TBACKUPTEST-0001 "));	
	
	// create and install the active scheduler we need for the cache manager in TServerResources::InitialiseL
	globalAS=new(ELeave) CActiveScheduler;
	CleanupStack::PushL(globalAS);
	CActiveScheduler::Install(globalAS);

	
	TServerResources::InitialiseL();
	
	gTestWrapper = CTestWrapper::NewLC(gTest);
	gTestWrapper->Start(_L("\n*** Bookmark Test ***\n"));

	DoBackupTestL();

	gTestWrapper->End();
	CleanupStack::PopAndDestroy(gTestWrapper);

	TServerResources::Close();
	CleanupStack::PopAndDestroy(globalAS);
	
	gTest.End();
	gTest.Close();
	__UHEAP_MARKEND;
	}


TInt E32Main()
	{	
	__UHEAP_MARK;
	CTrapCleanup* cleanup = CTrapCleanup::New();
	if ( !cleanup )
		return KErrNoMemory;

	TRAPD( err, MainL()  );
	if ( err != KErrNone )
		User::Panic( _L( "Testing failed: " ), err );
	
	delete cleanup;
	__UHEAP_MARKEND;
	
	return 0;	
	}



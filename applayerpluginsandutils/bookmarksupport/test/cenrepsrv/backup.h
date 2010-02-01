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
//

/**
 @internalTechnology
*/

#ifndef BACKUP_H
#define BACKUP_H

#include <e32base.h>
#include <f32file.h>
#include <e32property.h>
#include <connect/abclient.h>
#include "sessnotf.h"
#include "srvrepos.h"
		
using namespace conn ;

const TUint KArbitraryNumber = 1024 ;
const TUint32 KBackupStreamVersion = 1 ;

// Bitfield representing backup stream features present in the 
// incoming backup stream.
enum TBackupExtensions
	{
	ENoBackupExtensions = 0x00000000,
	EDeletedKeysSupported = 0x00000001,
	};

// Backup stream extension features supported by this version of central 
// repository backup/restore - OR together bitwise values defined in
// TBackupExtensions to get correct value!
const TUint32 KBackupExtensionsSupported = EDeletedKeysSupported ;

class TRepositoryBackupStreamHeader
	{
public :
	inline TRepositoryBackupStreamHeader(TStreamId aIndexStreamId) ;
	inline TRepositoryBackupStreamHeader();
	inline void ExternalizeL(RWriteStream& aStream) const;		
	inline void InternalizeL(RReadStream& aStream);		
	inline TUid getUid();
	inline TStreamId getIndexStreamId() ;
	inline TUint32 getVersion();
	inline TUint32 getBackupExtensionsSupported();

private :
	TUid iBackupStreamUid ;
	TUint32 iBackupStreamVersion ;
	TStreamId iIndexStreamId ;
	TUint32 iBackupExtensionsSupported ;
	
	// Reserved for future expansion
	TUint32 iReserved1 ;
	TUint32 iReserved2 ;	
	TUint32 iReserved3 ;
	TUint32 iReserved4 ;			
	};
	
//
// Backup stream index class - Used to hold association between a UID (in
// our case the UID of a repository) and a stream ID - Can't use CStreamDictionary
// because that only lets you retrieve stream IDs by a (previously known) UID rather
// than iterate through its contents retrieving UID/StreamID pairs...
// 

class TRepositoryBackupStreamIndexElement
	{

public :
	inline void Set (TUid aUid, TStreamId aSettingsStream, TStreamId aDeletedSettingsStream = KNullStreamIdValue) ;
	inline void SetDeletedSettingsStream (TStreamId aDeletedSettingsStream) ;
	inline void Get (TUid& aUid, TStreamId& aSettingsStreamId, TStreamId& aDeletedSettingsStreamId) ;
	inline void GetDeletedSettingsStream (TStreamId& aDeletedSettingsStream) ;
	inline void ExternalizeL(RWriteStream& aStream) const;		
	inline void InternalizeL(RReadStream& aStream, TUint32 aBackupStreamExtensions = ENoBackupExtensions);		

private:
	TUid iUid ;
	TStreamId iSettingsStream ;	
	TStreamId iDeletedSettingsStream ;
	};
	
class CRepositoryBackupStreamIndex : public CBase
	{
public:
	inline static CRepositoryBackupStreamIndex* NewL() ;
	inline static CRepositoryBackupStreamIndex* NewLC();
	inline ~CRepositoryBackupStreamIndex();
	inline void Close(void) ;

	void AddL(TUid aUid, TStreamId aSettingStreamId, TStreamId aDeletedSettingsStreamId) ;
	TInt GetNext(TUid& aUid, TStreamId& aSettingsStreamId, TStreamId& aDeletedSettingsStreamId) ;
	inline void Reset();
	inline void ExternalizeL(RWriteStream& aStream) const;
	inline void InternalizeL(RReadStream& aStream, TUint32 aBackupStreamExtensions = ENoBackupExtensions);
	
private:
	inline CRepositoryBackupStreamIndex(){} ;

private:
	TInt iIndex ;
	RArray<TRepositoryBackupStreamIndexElement> iStreamIndex ;
	};		
	

enum TRepositoryBackupState
{
	ENoBackupActivty,
	EBackupInProgress,
	ERestoreInProgress
};

//
// Backup client class.
//
// Has Active object functionality to monitor the state of the publish and subscribe 
// flags associated with backup and restore and also implements MActiveBackupDataClient
// to perform active backup according to the "proxy data holder" model.
// 
class CRepositoryBackupClient : public CActive, public MActiveBackupDataClient 
	{
public:
	static CRepositoryBackupClient* NewL(RFs& aFs);
	static CRepositoryBackupClient* NewLC(RFs& aFs);
	inline static TRepositoryBackupState GetBackupStatus(void) {return iBackupStatus;} ;
	
	CRepositoryBackupClient(RFs& aFs);
	~CRepositoryBackupClient();
	void ConstructL();
	void StartL();
	void NotifyChange();

	// Implementations of virtual functions inherited from MActiveBackupDataClient
	void AllSnapshotsSuppliedL();
	void ReceiveSnapshotDataL(TDriveNumber aDrive, TDesC8& aBuffer, TBool aLastSection);
	TUint GetExpectedDataSize(TDriveNumber aDrive);
	void GetSnapshotDataL(TDriveNumber aDrive, TPtr8& aBuffer, TBool& aFinished);
	void InitialiseGetBackupDataL(TDriveNumber aDrive);
	void GetBackupDataSectionL(TPtr8& aBuffer, TBool& aFinished);

	void InitialiseGetProxyBackupDataL(TSecureId aSID, TDriveNumber aDrive) ;
	void InitialiseRestoreProxyBaseDataL(TSecureId aSID, TDriveNumber aDrive) ;

	void InitialiseRestoreBaseDataL(TDriveNumber aDrive);
	void RestoreBaseDataSectionL(TDesC8& aBuffer, TBool aFinished);
	void InitialiseRestoreIncrementDataL(TDriveNumber aDrive);
	void RestoreIncrementDataSectionL(TDesC8& aBuffer, TBool aFinished);
	void RestoreComplete(TDriveNumber aDrive);
		
	void TerminateMultiStageOperation();
	TUint GetDataChecksum(TDriveNumber aDrive);
	void CompleteOwnerIdLookupTableL();
	
private:
	// Usual active object stuff
	void RunL() ;
	void DoCancel() ;
	TInt RunError(TInt aError) ;

	void TestBURstatusL(void) ;	
	
	// Leaving version of RestoreComplete to make it a bit easier
	// for us to handle cleanup...
	void RestoreCompleteL();

private:
	enum TBackupDirectoryScan 
		{
		EScanRom,
		EScanInstall,
		EScanPersist
		};
		
private:	
	// Used to open a repository for backup and/or restore
	CServerRepository* iRepository ;
	
	CSessionNotifier* iNotifier ;

	// Used to subscribe to Backup/Restore flag
	RProperty iBackupRestoreProperty ;
	
	// File server session used to get directory listings
	RFs& iFs;
	RFile iFile ;
	
	// Secure ID being restored
	TSecureId iSid ;
	
	// Active backup client to register with BUR engine
	CActiveBackupClient* iActiveBackupClient ;
	
	TBool iRomScanDone;
	// Current state (backup in proress, restore in progress, etc, etc, etc)
	static TRepositoryBackupState iBackupStatus ;

	// Features supported by Backup Stream 
	TUint32 iBackupExtensionsSupported ;	
	};


#include "backup.inl"

#endif // BACKUP_H

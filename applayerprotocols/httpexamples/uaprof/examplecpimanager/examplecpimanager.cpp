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

// System Includes
#include <http/thttphdrval.h>

// User Includes
#include "examplecpimanager.h"


// The storage location for CPI files on the device.
_LIT(KTxtCpiFilesLocation, "c:\\system\\data\\uaprof\\");

// Each CPI file contains two UIDs before the actual CPI data.
// The first UID is to indicate that the file is a UAProf CPI file.
// The second UID is used to differentiate between Profile and Profile-Diff information.
static const TUid KUidUaProfCpi = {0x101F55EE};
static const TUid KUidProfile = {0x101F55F0};
static const TUid KUidProfileDiff = {0x101F55F1};

// Length in bytes of the two UIDs (Cpi File UID and either either a Profile or Profile-Diff 
// file UID) included at the start of all CPI files.
const TInt KFileUIDLength = sizeof(TUint) * 2;

// The index of the second UID in a CPI file 
const TInt KUID2Index = 1;


EXPORT_C CExampleCpiManager* CExampleCpiManager::NewL(RStringPool aStringPool)
/**	Standard C class construction method
	@since		8.0
	@param		aStringPool	The string pool to use for the creation of all strings in this manager
	@return		A pointer to a fully constructed CExampleCpiManager
*/
	{
	CExampleCpiManager* cpiManager = new (ELeave) CExampleCpiManager(aStringPool);
	CleanupStack::PushL(cpiManager);
	cpiManager->ConstructL();
	CleanupStack::Pop(cpiManager);
	return cpiManager;
	}
	
CExampleCpiManager::CExampleCpiManager(RStringPool aStringPool) 
	:	iStringPool(aStringPool)
/**	Standard constructor
	@since		8.0
	@param		aStringPool	The string pool to use for the creation of all strings in this manager
*/
	{
	}
	
void CExampleCpiManager::ConstructL()
/**	Standard 2nd phase constructor
	@since		8.0
*/
	{
	User::LeaveIfError(iFs.Connect());
	FindCpiFilesL();
	DiscoverProfilesL();
	}	

EXPORT_C CExampleCpiManager::~CExampleCpiManager()
/**	Standard destructor
	@since		8.0
*/	
	{
	iCpiFileList.ResetAndDestroy();
	iProfileData.ResetAndDestroy();
	iProfileDiffData.ResetAndDestroy();
	iFs.Close();
	}
	
EXPORT_C RPointerArray<HBufC8>& CExampleCpiManager::GetProfiles()
/**	Gets the Profile data stored on the device
	@since		8.0
	@return		An array of Profiles
*/
	{
	return iProfileData;
	}
	
EXPORT_C RPointerArray<HBufC8>& CExampleCpiManager::GetProfileDiffs()
/**	Gets the Profile-Diff data stored on the device
	@since		8.0
	@return		An array of Profile-Diffs
*/
	{
	return iProfileDiffData;
	}
	
void CExampleCpiManager::FindCpiFilesL()
/**	Looks in the UAProf directory for any CPI files and adds them to the array of CPI files
	@since		8.0
*/
	{
	TUidType uidType(KUidUaProfCpi);
	CDir* dir;
	// Get a list of files that match the CPI UID
	TInt err = iFs.GetDir(KTxtCpiFilesLocation, uidType, ESortByUid, dir);
	CleanupStack::PushL(dir);	
	if (err == KErrPathNotFound)
		User::LeaveIfError(iFs.MkDirAll(KTxtCpiFilesLocation()));
	else
		User::LeaveIfError(err);
	
	const TInt fileCount = dir->Count();
	for (TInt i = 0; i < fileCount; ++i)
		{
		TEntry* entry = NULL;
		entry = new (ELeave) TEntry((*dir)[i]);
		entry->iName = KTxtCpiFilesLocation();
		(entry->iName.Des()).Append((*dir)[i].iName);
		User::LeaveIfError(iCpiFileList.Append(entry));
		}
	CleanupStack::PopAndDestroy(dir);
	}
	
void CExampleCpiManager::DiscoverProfilesL()
/**	Loop through the array of CPI files determining, through looking at the UID in 
	the second array element, whether the CPI files are Profile or Profile-Diff files.
	@since		8.0
*/
	{
	const TInt fileCount = iCpiFileList.Count();
	for (TInt ii = 0; ii < fileCount; ++ii)
		{
		// Look to see if the UID of the file matches that of the UID assigned
		// to Profile files.
		if ((*iCpiFileList[ii])[KUID2Index] == KUidProfile)
			BuildProfilesL(*iCpiFileList[ii], EProfile);
			
		// Look to see if the UID of the file matches that of the UID assigned
		// to Profile-Diff files.
		else if ((*iCpiFileList[ii])[KUID2Index] == KUidProfileDiff)
			BuildProfilesL(*iCpiFileList[ii], EProfileDiff);
		else
			User::Leave(KErrCorrupt);
		}
	}
	
void CExampleCpiManager::BuildProfilesL(TEntry& aEntry, CExampleCpiManager::TCpiType aCpiType)
/**	Appends the Profile and Profile-Diff data discovered to an array of Profile and
	Profile-Diff data respectively
	@since		8.0
	@param		aEntry The CPI file to extract the Profile or Profile-Diff data from
	@param		aCpiType The type of CPI the file contains i.e. Profile or Profile-Diff
*/
	{
	RFile cpiFile;
	User::LeaveIfError(cpiFile.Open(iFs, aEntry.iName, EFileShareExclusive));
	CleanupClosePushL(cpiFile);
	
	TInt fileSize = 0;
	User::LeaveIfError(cpiFile.Size(fileSize));

	HBufC8* fileReader = HBufC8::NewLC(fileSize);
	TPtr8 dataBuffer = fileReader->Des();
	// Start reading from the file following the 2 UIDs found at the beginning of the file
	cpiFile.Read(KFileUIDLength, dataBuffer);
	HBufC8* cpiData = dataBuffer.AllocLC();

	switch(aCpiType)
		{
		case EProfile:
			{
			User::LeaveIfError(iProfileData.Append(cpiData));
			break;
			}
		case EProfileDiff:
			{
			User::LeaveIfError(iProfileDiffData.Append(cpiData));
			break;
			}
		default:
			break;
		}
	CleanupStack::Pop(cpiData);
	CleanupStack::PopAndDestroy(2, &cpiFile); // fileReader, cpiFile
	}


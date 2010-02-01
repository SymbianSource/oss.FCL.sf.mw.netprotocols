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
// cexamplecpimanager.h
// 
//

#ifndef __CEXAMPLECPIMANAGER_H__
#define __CEXAMPLECPIMANAGER_H__

// System Includes
#include <http/mhttpcpimanager.h>
#include <f32file.h>


/** CExampleCpiManager is an example implementation of a CPI Manager class that
	provides the MHTTPCpiManager interface
*/
class CExampleCpiManager : public CBase, public MHTTPCpiManager
	{
public:	// methods

	IMPORT_C static CExampleCpiManager* NewL(RStringPool aStringPool);

	IMPORT_C ~CExampleCpiManager();

	/** Enumeration defining the two different types of CPI information
	*/
	enum TCpiType
		{
		EProfile,
		EProfileDiff
		};


public:	// methods from MHTTPCpiManager

	IMPORT_C virtual RPointerArray<HBufC8>& GetProfiles();

	IMPORT_C virtual RPointerArray<HBufC8>& GetProfileDiffs();


private: // methods

	CExampleCpiManager(RStringPool aStringPool);
	
	void ConstructL();
	
	void FindCpiFilesL();
	
	void DiscoverProfilesL();
	
	void BuildProfilesL(TEntry& aEntry, CExampleCpiManager::TCpiType aCpiType);


private: // attributes

	/** String pool to be used
	*/
	RStringPool iStringPool;
	
	/** Array of CPI files
	*/
	RPointerArray<TEntry> iCpiFileList;

	/** Profile table
	*/
	RPointerArray<HBufC8> iProfileData;
	
	/** Profile-Diff table
	*/
	RPointerArray<HBufC8> iProfileDiffData;
	
	/** Handle to the file server
	*/
	RFs iFs;
	};


#endif //__CEXAMPLECPIMANAGER_H__

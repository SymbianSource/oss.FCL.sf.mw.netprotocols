// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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

class CTestWrapper;

class CBmTests : public CBase
	{
	public:
	
	static CBmTests* NewL ( CTestWrapper& aTestWrapper );
	~CBmTests ();
	
	void OpenDbL ();
	void CloseDb ();
	void CreateTreeL ();
	void ModifyTreeL ();
	void UpdateTreeL ();
	
	void SetBkmrkDbPropertiesL ();
	void UpdateBkmrkDbPropertiesL ();
	
	void CreateCustomPropertiesL ();
	void SetCustomPropertiesValueL ();
	
	void ReadCustomPropertiesValueL ();
	
	void CommitDbL ();
	
	void DeleteAllL ();
	
	private:	
	CBmTests ( CTestWrapper& aTestWrapper );
	RBkFolder CreateFolderLC ( const TDesC& aFolderName, RBkFolder* aParent = NULL );
	RBkFolder OpenFolderLC ( const TDesC& aFolderName );
		
	private:	
	RBkDatabase iBkDatabase;
	CTestWrapper& iTestWrapper;	
	};

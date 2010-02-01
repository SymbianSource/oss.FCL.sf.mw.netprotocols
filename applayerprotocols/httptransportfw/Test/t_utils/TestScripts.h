// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __TESTSCRIPTS_H__
#define __TESTSCRIPTS_H__

#include <e32std.h>
#include <e32base.h>
#include <f32file.h>
#include "httptestutils.h"

class CScriptSection;
class CScriptSectionItem;

_LIT(KScriptPanic, "HTTPTESTSCR");
_LIT(KScriptSectionStart, "[");
_LIT(KScriptCRLF, "\r\n");
_LIT(KScriptLF, "\n");
_LIT(KScriptCR, "\r");
_LIT(KScriptItemEnd, "=");
_LIT(KDefaults, "Defaults");
_LIT(KDefault1, "Def");
_LIT(KDefault2, "Default");
_LIT(KScriptsSection, "Scripts");

//##ModelId=3C4C4C40007E
class CScriptFile : public CBase
	{
	public:
	//##ModelId=3C4C4C400222
		IMPORT_C static CScriptFile* NewL(CHTTPTestUtils& aTestUtils, const TDesC& aComponent, const TDesC& aScriptFile);
	//##ModelId=3C4C4C400237
		IMPORT_C static CScriptFile* NewL(CHTTPTestUtils& aTestUtils, const TDesC& aComponent);
	//##ModelId=3C4C4C400205
		IMPORT_C static CScriptFile* NewLC(CHTTPTestUtils& aTestUtils, const TDesC& aComponent, const TDesC& aScriptFile);
	//##ModelId=3C4C4C400218
		IMPORT_C static CScriptFile* NewLC(CHTTPTestUtils& aTestUtils, const TDesC& aComponent);
	//##ModelId=3C4C4C400204
		IMPORT_C ~CScriptFile();

	//##ModelId=3C4C4C4001E6
		IMPORT_C const TDesC& ItemValue(const TDesC& aSection, const TDesC& aItem, const TDesC& aDefault);
	//##ModelId=3C4C4C4001F1
		IMPORT_C TInt ItemValue(const TDesC& aSection, const TDesC& aItem, const TInt aDefault);
		
	//##ModelId=3C4C4C4001C9
		IMPORT_C static HBufC* ItemValueLC(CHTTPTestUtils& aTestUtils, const TDesC& aComponent, const TDesC& aScript, const TDesC& aSection, const TDesC& aItem, const TDesC& aDefault);
	//##ModelId=3C4C4C4001AB
		IMPORT_C static TInt ItemValueL(CHTTPTestUtils& aTestUtils, const TDesC& aComponent, const TDesC& aScript, const TDesC& aSection, const TDesC& aItem, const TInt aDefault);

	//##ModelId=3C4C4C4001A0
		IMPORT_C void ReadScriptL(const TDesC& aScript);

	//##ModelId=3C4C4C400198
		inline CArrayPtrFlat<CScriptSection>& Sections() const;

	//##ModelId=3C4C4C40018D
		IMPORT_C CScriptSection* Section(const TDesC& aSectionName); //return NULL if section not found
	//##ModelId=3C4C4C400183
		inline CScriptSection& SectionL(const TDesC& aSectionName); //leaves with KErrNotFound if section not found
	//##ModelId=3C4C4C400196
		inline CScriptSection& Section(TInt aIndex) const {return *iSections->At(aIndex);}

	//##ModelId=3C4C4C400164
		IMPORT_C static TInt GetNextWord(TLex& aInput, TChar aDelimiter, TInt& aOutput);
	//##ModelId=3C4C4C400178
		IMPORT_C static TInt GetNextWord(TLex& aInput, TChar aDelimiter, TPtrC& aOutput);
	//##ModelId=3C4C4C400150
		IMPORT_C static void ReplaceL(const TDesC& aOld, const TDesC& aNew, HBufC*& rString);
	//##ModelId=3C4C4C40013D
		IMPORT_C static HBufC* ReplaceLC(const TDesC& aOld, const TDesC& aNew, const TDesC& aOldString);

	protected:
	//##ModelId=3C4C4C400134
		CScriptFile(CHTTPTestUtils& aTestUtils);
	//##ModelId=3C4C4C400132
		void ConstructL(const TDesC& aComponent);

	//##ModelId=3C4C4C4000C6
		CArrayPtrFlat<CScriptSection>* iSections;
	//##ModelId=3C4C4C4000B0
		CScriptSection* iLastSection;
	//##ModelId=3C4C4C40009E
		CHTTPTestUtils& iTestUtils;
	//##ModelId=3C4C4C400095
		HBufC* iComponent;

	//##ModelId=3C4C4C40011F
		TPtrC ParseValue(const TDesC& aText, const TLex& aInput, TInt aCurrentItemStart) const;
	//##ModelId=3C4C4C40010B
		void ParseAndSetItemValueL(const TDesC& aText, const TLex& aInput, TInt aCurrentItemStart, CScriptSectionItem*& arCurrentItem);
	//##ModelId=3C4C4C4000F6
		void FoundNewItemL(const TDesC& aText, TLex& arInput, TInt& arCurrentItemStart, CScriptSection& aSection, CScriptSectionItem*& arCurrentItem);
	//##ModelId=3C4C4C4000EC
		void CopyInDefaultsL(CScriptSection& aSection, const TDesC& aDefaultFile);

	//##ModelId=3C4C4C4000E3
		HBufC* ReadFileLC(const TDesC& aScript) const;
	//##ModelId=3C4C4C4000CF
		HBufC8* ReadFileL(const TDesC& aFile) const;
		
		void ParseScriptSectionsL(const TDesC& aScript, TInt aInsertIndex);
		void ParseEmbeddedScriptsL(CScriptSection& aSection, TInt aInsertIndex);
	};

//##ModelId=3C4C4C4302C7
class CScriptSection : public CBase
	{
	friend class CScriptFile;

	public:
	//##ModelId=3C4C4C4303C1
		IMPORT_C static CScriptSection* NewLC(const TDesC& aSectionName);
	//##ModelId=3C4C4C4303CC
		IMPORT_C static CScriptSection* NewLC(const TDesC& aSectionName, CScriptSection& aDefaults);
	//##ModelId=3C4C4C4303B8
		IMPORT_C ~CScriptSection();
		
	//##ModelId=3C4C4C4303B7
		inline const TDesC& SectionName() const;

	//##ModelId=3C4C4C4303A4
		IMPORT_C CScriptSectionItem* Item(const TDesC& aItem); //return NULL if the item does not exist
	//##ModelId=3C4C4C43039A
		inline CScriptSectionItem& ItemL(const TDesC& aItem); //leaves with KErrNotFound if the item does not exist

	//##ModelId=3C4C4C430385
		IMPORT_C const TDesC& ItemValue(const TDesC& aItem, const TDesC& aDefault);
	//##ModelId=3C4C4C43038F
		IMPORT_C TInt ItemValue(const TDesC& aItem, TInt aDefault);

	//##ModelId=3C4C4C430371
		IMPORT_C CScriptSectionItem& AddItemL(const TDesC& aItem, const TDesC& aValue);
	//##ModelId=3C4C4C430367
		IMPORT_C void AddItemIfNotExistL(const TDesC& aItem, const TDesC& aValue);
	//##ModelId=3C4C4C430353
		IMPORT_C CScriptSectionItem& ReplaceItemL(const TDesC& aItem, const TDesC& aValue);
	//##ModelId=3C4C4C430349
		IMPORT_C void DeleteItemsL(const TDesC& aItem);

	//##ModelId=3C4C4C430341
		inline CArrayPtrFlat<CScriptSectionItem>& Items() const {return *iItems;}
	//##ModelId=3C4C4C4303AD
		inline CScriptSectionItem& Item(TInt aIndex) const {return *iItems->At(aIndex);}

	//##ModelId=3C4C4C43033F
		inline void SetDefaultsL(CScriptSection& aDefaults);
	//##ModelId=3C4C4C430336
		inline CScriptSection* Defaults() const {return iDefaults;}

	//##ModelId=3C4C4C430335
		IMPORT_C CScriptSection* CopyLC();

	private:
	//##ModelId=3C4C4C43032B
		void ConstructL(const TDesC& aSectionName);
	//##ModelId=3C4C4C430322
		CScriptSection();
	//##ModelId=3C4C4C430319
		CArrayPtrFlat<CScriptSectionItem>* iItems;
	//##ModelId=3C4C4C4302FB
		HBufC* iSectionName;
	//##ModelId=3C4C4C4302EF
		CScriptSectionItem* iLastItem;
	//##ModelId=3C4C4C4302DE
		CScriptSection* iDefaults;
	};

//##ModelId=3C4C4C3F022B
class CScriptSectionItem : public CBase
	{
	friend class CScriptSection;
	friend class CScriptItem;

	public:
	//##ModelId=3C4C4C3F027C
		IMPORT_C static CScriptSectionItem* NewLC(CScriptSection& aParent, const TDesC& aItem, const TDesC& aValue);
	//##ModelId=3C4C4C3F027B
		inline CScriptSectionItem* CopyLC();

	//##ModelId=3C4C4C3F0273
		IMPORT_C ~CScriptSectionItem();
	//##ModelId=3C4C4C3F0255
		HBufC* iValue;
	//##ModelId=3C4C4C3F0272
		inline const TDesC& Item() const;
	//##ModelId=3C4C4C3F0271
		inline const TDesC& Value() const;

	//##ModelId=3C4C4C3F024B
		CScriptSection& iParent;
		
	private:
	//##ModelId=3C4C4C3F0268
		CScriptSectionItem(CScriptSection& aParent);
	//##ModelId=3C4C4C3F025D
		void ConstructL(const TDesC& aItem, const TDesC& aValue);
	//##ModelId=3C4C4C3F0242
		HBufC* iItem;
	};

#include "TestScripts.inl"

#endif // __TESTSCRIPTS_H__


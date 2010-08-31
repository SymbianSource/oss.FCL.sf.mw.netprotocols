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

#include "TestScripts.h"
#include "httptestutils.h"

EXPORT_C CScriptFile* CScriptFile::NewL(CHTTPTestUtils& aTestUtils, const TDesC& aComponent, const TDesC& aScript)
	{
	CScriptFile* self = NewLC(aTestUtils, aComponent, aScript);
	CleanupStack::Pop();
	return self;
	}

EXPORT_C CScriptFile* CScriptFile::NewLC(CHTTPTestUtils& aTestUtils, const TDesC& aComponent, const TDesC& aScript)
	{
	CScriptFile* self = NewLC(aTestUtils, aComponent);
	self->ReadScriptL(aScript);
	return self;
	}

EXPORT_C CScriptFile* CScriptFile::NewL(CHTTPTestUtils& aTestUtils, const TDesC& aComponent)
	{
	CScriptFile* self = NewLC(aTestUtils, aComponent);
	CleanupStack::Pop();
	return self;
	}

EXPORT_C CScriptFile* CScriptFile::NewLC(CHTTPTestUtils& aTestUtils, const TDesC& aComponent)
	{
	CScriptFile* self = new (ELeave) CScriptFile(aTestUtils);
	CleanupStack::PushL(self);
	self->ConstructL(aComponent);
	return self;
	}

void CScriptFile::ConstructL(const TDesC& aComponent)
	{
	iSections = new (ELeave) CArrayPtrFlat<CScriptSection>(10);
	iComponent = aComponent.AllocL();
	}

CScriptFile::CScriptFile(CHTTPTestUtils& aTestUtils)
: iTestUtils(aTestUtils)
	{
	}

EXPORT_C CScriptFile::~CScriptFile()
	{
	if (iSections)
		{
		iSections->ResetAndDestroy();
		delete iSections;
		}

	delete iComponent;
	}

EXPORT_C CScriptSection* CScriptFile::Section(const TDesC& aSectionName)
	{
	CScriptSection* section = iLastSection;

	if (!iLastSection || iLastSection->SectionName().CompareF(aSectionName) != 0)
		{
		const TInt count = iSections->Count();
		
		for (TInt i = 0; i < count; i++) //order important
			{
			section = iSections->At(i);
			iLastSection = section;

			if (iLastSection->SectionName().CompareF(aSectionName) == 0)
				{
				break;
				}
			else
				{
				section = NULL;
				}
			}
		}

	return section;
	}

EXPORT_C const TDesC& CScriptFile::ItemValue(const TDesC& aSection, const TDesC& aItem, const TDesC& aDefault)
	{
	CScriptSection* section = Section(aSection);

	if (section != NULL)
		return section->ItemValue(aItem, aDefault);

	return aDefault;
	}

EXPORT_C TInt CScriptFile::ItemValue(const TDesC& aSection, const TDesC& aItem, const TInt aDefault)
	{
	TInt output = aDefault;
	CScriptSection* section = Section(aSection);

	if (section != NULL)
		{
		output = section->ItemValue(aItem, aDefault);
		}

	return output;
	}
		
EXPORT_C HBufC* CScriptFile::ItemValueLC(CHTTPTestUtils& aTestUtils, const TDesC& aComponent, const TDesC& aScript, const TDesC& aSection, const TDesC& aItem, const TDesC& aDefault)
	{
	CScriptFile* self = NewLC(aTestUtils, aComponent, aScript);
	TPtrC output(self->ItemValue(aSection, aItem, aDefault));
	HBufC* buf = output.AllocL();
	CleanupStack::PopAndDestroy(self);
	CleanupStack::PushL(buf);
	return buf;
	}

EXPORT_C TInt CScriptFile::ItemValueL(CHTTPTestUtils& aTestUtils, const TDesC& aComponent, const TDesC& aScript, const TDesC& aSection, const TDesC& aItem, const TInt aDefault)
	{
	CScriptFile* self = NewLC(aTestUtils, aComponent, aScript);
	TInt output = self->ItemValue(aSection, aItem, aDefault);
	CleanupStack::PopAndDestroy(self);
	return output;
	}

HBufC8* CScriptFile::ReadFileL(const TDesC& aFile) const
	{
	HBufC8* ret = iTestUtils.ReadFileLC(aFile);
	CleanupStack::Pop(ret);
	return ret;
	}

HBufC* CScriptFile::ReadFileLC(const TDesC& aScript) const
	{
	HBufC8* file = NULL;

	TRAPD(err, file = ReadFileL(aScript));

	if (err)
		{
		TParse fileOut;
		err = iTestUtils.ResolveFile(*iComponent, aScript, fileOut);

		if (err)
			{
			iTestUtils.Test().Printf(_L("Cannot read file %S. Error=%d"), &aScript, err);
			User::Leave(err);
			}

		file = ReadFileL(fileOut.FullName());
		}

	CleanupStack::PushL(file);

	HBufC* buf = HBufC::NewL(file->Length());
	buf->Des().Copy(*file);
	CleanupStack::PopAndDestroy(file);
	CleanupStack::PushL(buf);
	return buf;
	}

EXPORT_C void CScriptFile::ReadScriptL(const TDesC& aScript)
	{
	iSections->ResetAndDestroy();
	ParseScriptSectionsL(aScript, KErrNotFound);
	}

void CScriptFile::ParseEmbeddedScriptsL(CScriptSection& aSection, TInt aInsertIndex)
	{
	// This must be a Script section
	__ASSERT_DEBUG(aSection.SectionName().CompareF(KScriptsSection)==0, User::Invariant());
	
	// Loop through the script section and load and parse each script file in reverse
	// order so that the sections are inserted in the correct order
	TInt scriptCount = aSection.Items().Count();
	for(TInt ii=scriptCount-1; ii>=0; --ii)
		{
		CScriptSectionItem& item = aSection.Item(ii);
		
		// Sometimes comments are included as part of the value. We need to include
		// some simple parsing here to extract just the filename from the value
		// by just including the text upto a '\n' or ' ' characters.
		TPtrC fileName(item.Value());
		TInt locateWhiteSpace = fileName.LocateF('\n');
		if(locateWhiteSpace>0)
			fileName.Set(fileName.Left(locateWhiteSpace));
			
		locateWhiteSpace = fileName.LocateF(' ');
		if(locateWhiteSpace>0)
		fileName.Set(fileName.Left(locateWhiteSpace));
		
		ParseScriptSectionsL(fileName, aInsertIndex);
		}
	}

void CScriptFile::ParseScriptSectionsL(const TDesC& aScript, TInt aInsertIndex)
	{
	iLastSection = NULL;

	CScriptSection* section = NULL;
	CScriptSectionItem* currentItem = NULL;
	TInt currentItemStart = 0;
	CScriptSection* sectionAll = NULL;
	RArray<TInt> sectionIndexArray;
	CleanupClosePushL(sectionIndexArray);
	TInt indexIncrement = 0;		
	HBufC* scriptContents = ReadFileLC(aScript);

	TLex input(*scriptContents);

	while (!input.Eos())
		{
	    input.SkipSpaceAndMark();

		input.SkipCharacters();
		if ( input.TokenLength() == 0 )    // if valid potential token
			{
			break;
			}

		TPtrC token = input.MarkedToken();

		if (token.CompareF(_L("endscript")) == 0)
			{
			break;
			}
		else if (token.FindF(KScriptSectionStart) == 0 && token.Length() > 2)
			{
			ParseAndSetItemValueL(*scriptContents, input, currentItemStart, currentItem);

			TInt mid = 1;
			TInt len = token.Length() - 2;

			if (sectionAll)
				section = CScriptSection::NewLC(token.Mid(mid, len), *sectionAll);
			else
				section = CScriptSection::NewLC(token.Mid(mid, len));

			if (!sectionAll && section->SectionName().CompareF(_L("All")) == 0)
				sectionAll = section;
			
			// We have a Scripts section, store its index as we need to track where
			// it occured so that the scripts can be added in the same location
			// after the current file has parsed
			if (section->SectionName().CompareF(KScriptsSection) == 0)
				{
				if(aInsertIndex==KErrNotFound)
					{
					// Not an embedded script, simple store its index
					TInt currectSectionsCount = iSections->Count();
					sectionIndexArray.Append(currectSectionsCount);
					}
				else
					{
					// An embedded script, calculate the index it should be inserted at
					TInt currectSectionsCount = indexIncrement+aInsertIndex;
					sectionIndexArray.Append(currectSectionsCount);
					}
				}
			
			// Check if the section needs to be inserted at a particular location
			if(aInsertIndex==KErrNotFound)
				iSections->AppendL(section);
			else
				iSections->InsertL(aInsertIndex + indexIncrement++, section);
				
			CleanupStack::Pop(section);
			}
		else if (section)
			{
			TPtrC itemEnd(KScriptItemEnd);

			if (itemEnd.Length() < token.Length() && token.Right(itemEnd.Length()).CompareF(itemEnd) == 0)
				{
				FoundNewItemL(*scriptContents, input, currentItemStart, *section, currentItem);
				}
			}
		}

	ParseAndSetItemValueL(*scriptContents, input, currentItemStart, currentItem);
	CleanupStack::PopAndDestroy(scriptContents);
	
	// We have been tracking where the script sections have been inserted so we
	// want to load the sections from the embedded script file and insert them
	// at the same point. This must be done in reverse order so that the index
	// values of script sections before the current one is maintained.
	TInt scriptSectionsCount = sectionIndexArray.Count();
	for( TInt ii=scriptSectionsCount-1; ii>=0; --ii)
		{
		TInt indexOfScriptSection = sectionIndexArray[ii];
		ParseEmbeddedScriptsL(*(iSections->At(indexOfScriptSection)), indexOfScriptSection);
		}
	CleanupStack::PopAndDestroy(&sectionIndexArray);
	}

TPtrC CScriptFile::ParseValue(const TDesC& aText, const TLex& aInput, TInt aCurrentItemStart) const
	{
	TInt mid = aCurrentItemStart;
	TInt len = aInput.MarkedOffset() - mid;
	TPtrC ret(KNullDesC);

	if (len > 0)
		ret.Set(aText.Mid(mid, len));

	return ret;
	}

void CScriptFile::ParseAndSetItemValueL(const TDesC& aText, const TLex& aInput, TInt aCurrentItemStart, CScriptSectionItem*& arCurrentItem)
	{
	if (arCurrentItem)
		{
		delete arCurrentItem->iValue;
		arCurrentItem->iValue = NULL;
		arCurrentItem->iValue = ParseValue(aText, aInput, aCurrentItemStart).AllocL();
		arCurrentItem->iValue->Des().Trim();
		ReplaceL(KScriptCRLF, KScriptLF, arCurrentItem->iValue);

		if (arCurrentItem->Item().CompareF(KDefaults) == 0)
			{
			CopyInDefaultsL(arCurrentItem->iParent, arCurrentItem->Value());
			}
		}

	arCurrentItem = NULL;
	}

void CScriptFile::FoundNewItemL(const TDesC& aText, TLex& arInput, TInt& arCurrentItemStart, CScriptSection& aSection, CScriptSectionItem*& arCurrentItem)
	{
	TPtrC token = arInput.MarkedToken();

	ParseAndSetItemValueL(aText, arInput, arCurrentItemStart, arCurrentItem);

	arInput.SkipSpaceAndMark();
	arCurrentItemStart = arInput.Offset();

	TPtrC itemEnd(KScriptItemEnd);
	const TInt length = token.Length() - itemEnd.Length();
	arCurrentItem = &aSection.ReplaceItemL(token.Left(length), KNullDesC);
	}

void CScriptFile::CopyInDefaultsL(CScriptSection& aSection, const TDesC& aDefaultFile)
	{
	CScriptFile* file = CScriptFile::NewLC(iTestUtils, *iComponent, aDefaultFile);

	TInt count = file->Sections().Count();

	if (count > 0)
		{
		CScriptSection& def = *file->Sections()[0];
		aSection.SetDefaultsL(def);
		}
	CleanupStack::PopAndDestroy(file);
	}

EXPORT_C TInt CScriptFile::GetNextWord(TLex& aInput, TChar aDelimiter, TInt& aOutput)
	{
	aOutput = 0;
	TPtrC string;
	TInt err = GetNextWord(aInput, aDelimiter, string);

	if (!err)
		{
		TLex number(string);
		err = number.Val(aOutput);
		}

	return err;
	}

EXPORT_C TInt CScriptFile::GetNextWord(TLex& aInput, TChar aDelimiter, TPtrC& aOutput)
	{
	//Get to the start of the descriptor
	while (!aInput.Peek().IsAlphaDigit() && aInput.Peek() != '+' && !aInput.Eos() && aInput.Peek() != aDelimiter)
		aInput.Inc();

	if (aInput.Eos())
		return KErrNotFound;

	aInput.Mark();

	while (aInput.Peek() != aDelimiter && !aInput.Eos())
		aInput.Inc();

	aOutput.Set(aInput.MarkedToken());

	if (!aInput.Eos())
		aInput.SkipAndMark(1);

	return KErrNone;
	}

EXPORT_C void CScriptFile::ReplaceL(const TDesC& aOld, const TDesC& aNew, HBufC*& rString)
	{
	HBufC* repl = ReplaceLC(aOld, aNew, *rString);
	CleanupStack::Pop(repl);
	delete rString;
	rString = repl;
	}

EXPORT_C HBufC* CScriptFile::ReplaceLC(const TDesC& aOld, const TDesC& aNew, const TDesC& aOldString)
	{
	HBufC* rString = aOldString.AllocLC();
	TInt oldLen = aOld.Length();
	TInt newLen = aNew.Length();

	if (!oldLen)
		return rString;

	for (TInt pos = 0; pos < rString->Length(); pos += newLen)
		{
		TPtrC ptrC = rString->Mid(pos);
		TInt find = ptrC.Find(aOld);

		if (find == KErrNotFound)
			return rString;
	
		pos += find;

		if (newLen > oldLen)
			{
			rString = rString->ReAllocL(rString->Length() + newLen - oldLen);
			CleanupStack::Pop();
			CleanupStack::PushL(rString);
			}

		TPtr ptr(rString->Des());
		ptr.Replace(pos, oldLen, aNew);
		}

	return rString;
	}


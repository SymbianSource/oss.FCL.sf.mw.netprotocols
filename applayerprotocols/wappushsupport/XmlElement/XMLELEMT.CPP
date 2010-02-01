// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// XmlElement.cpp
// XML Element class
// Derived from CTypedNode. Adds functionality
// to make the use of the node simpler
// 
//

#include <f32file.h>
#include <attrlut.h> 

#include <xmlelemt.h>

LOCAL_C TBool Compare(TInt aLength,const TText* aStringA,const TDesC& aStringB)
	{
	if(aLength==aStringB.Length())
		{
		const TText* stringB=aStringB.Ptr();
		while( STATIC_CAST(TUint, aLength) >= (sizeof(TInt32) / sizeof(TText)) )
			{
			if(*(TInt32*)stringB!=*(TInt32*)aStringA)
				return EFalse;
			stringB+=(sizeof(TInt32)/sizeof(TText));
			aStringA+=(sizeof(TInt32)/sizeof(TText));
			aLength-=(sizeof(TInt32)/sizeof(TText));
			}
		while(aLength>0)
			{
			if(*stringB++!=*aStringA++)
				return EFalse;
			aLength--;
			}
		return ETrue;
		}
	return EFalse;
	}


//-------------------------------------------------------------------------

EXPORT_C void CXmlElement::InsertChildL(TInt aIndexToInsertBefore, CXmlElement* aElement)
{
	if(!iChildList)
	{
		iChildList = new (ELeave) CArrayPtrFlat<CNode>(KGranularity);
	}
	iChildList->InsertL(aIndexToInsertBefore, aElement);
}

//-------------------------------------------------------------------------

EXPORT_C const TDesC* CXmlElement::Attribute(const TDesC& aAttributeName) const
{
	const TInt count = AttributeCount();
	TInt length=aAttributeName.Length();
	const TText* stringPtr=aAttributeName.Ptr();
	for (TInt i = 0; i < count; i++)
	{ 
		TXmlAttributeType name;
		CBase* att=AttributeByIndex(i,name);
		if (Compare(length,stringPtr,*name))
			return (TDesC*)((CDataDelete*)att)->Data();
	}  
 
	return NULL;
}

//-------------------------------------------------------------------------

EXPORT_C void CXmlElement::SetAttributeL(const TDesC& aAttributeName, 
										 const TDesC& aAttributeValue, 
										 CAttributeLookupTable& aAttributeLUT)
{
	TXmlAttributeType type = aAttributeLUT.Des2IDL(aAttributeName);

	HBufC* attrValue = aAttributeValue.AllocLC();
	CDataDelete* attrib = new (ELeave) CDataDelete((HBufC16*)attrValue);
	CleanupStack::Pop(); // attrValue
	CleanupStack::PushL(attrib);
	AddAttributeL(type, attrib);
	CleanupStack::Pop(); // attrib
}

//-------------------------------------------------------------------------

EXPORT_C void CXmlElement::SetTextL(const TDesC16& aData)
	{
	HBufC* data = aData.AllocLC();
	CTypedNode<TXmlElementType, TXmlAttributeType>::SetDataL(data);
	CleanupStack::Pop(data);
	}

//-------------------------------------------------------------------------

EXPORT_C void CXmlElement::SetDataItemL(CBase* aDataItem, CAttributeLookupTable& aAttributeLUT)
{
	__ASSERT_DEBUG(aDataItem, Panic(EXmlElPanicInvalidDataItem));

	TXmlAttributeType type = aAttributeLUT.Des2IDL(KXmlElementDataItemAttributeName);
	CDataItemAttribute* attrib = new (ELeave) CDataItemAttribute;

	CleanupStack::PushL(attrib);
	AddAttributeL(type, attrib);
	CleanupStack::Pop(); // attrib

	// Take ownership last
	attrib->iDataItem = aDataItem;
}

//-------------------------------------------------------------------------

EXPORT_C void CXmlElement::DeleteDataItem(CAttributeLookupTable& aAttributeLUT)
{
	TXmlAttributeType type = NULL;
	TRAPD(err, type = aAttributeLUT.Des2IDL(KXmlElementDataItemAttributeName));
	__ASSERT_DEBUG(err==KErrNone, Panic(EXmlElPanicInvalidDataItem));
	if(err==KErrNone)
		{
		// Delete data item
		DeleteAttribute(type);
		}
}

//-------------------------------------------------------------------------

EXPORT_C CBase* CXmlElement::DataItem() const
{
	const TInt count = AttributeCount();
	TPtrC attName(KXmlElementDataItemAttributeName);
	TInt length=attName.Length();
	const TText* stringPtr=attName.Ptr();
	for (TInt i = 0; i < count; i++)
	{
		TXmlAttributeType name;
		CBase* att=AttributeByIndex(i,name);
		if (Compare(length,stringPtr,*name))
			return ((CDataItemAttribute*)att)->iDataItem;
	}
	return NULL;
}

//-------------------------------------------------------------------------

EXPORT_C TInt CXmlElement::ChildIndex(CXmlElement* aChild) const
{
	TInt i;
	for( i=0; i < Count(); i++)
	{
		if( Child(i) == aChild )
			return(i);
	}
	return( -1 );
}

//#ifdef _DEBUG
// Write the name of the class into a debug file
void CXmlElement::WriteNameL(RFile& aFile, TInt aIndentation)
{
	// Write line feed
	aFile.Write(_L8("\r\n"));

	// Write as many tabs as the current indentation level
	for (TInt i = 0; i < aIndentation; i++)
	{
		aFile.Write(_L8("\t"));
	}
 
	// Write the name of element
	TXmlElementType type = Type();

	if (type)
	{
		// Convert to 8 bit
		HBufC8* type8 = HBufC8::NewL(type->Length());
		type8->Des().Copy(*type);
		aFile.Write(*type8);
		delete type8;
	}
	else
		aFile.Write(_L8("Error: Type = NULL"));


	// Write attributes
	const TInt count = AttributeCount();
	if(count > 0 || Data())
	{
		aFile.Write(_L8(" ("));
	}

	for (TInt a = 0; a < count; a++)
	{
		if(a>0 && count>1)
		{
			aFile.Write(_L8("\", "));
		}
		TXmlAttributeType type = AttributeTypeByIndex(a);
		// Convert to 8 bit
		HBufC8* type8 = HBufC8::NewL(type->Length());
		type8->Des().Copy(*type);
		aFile.Write(*type8);
		delete type8;

		aFile.Write(_L8(" = \""));

		const TDesC* attrib = Attribute(a);
		HBufC8* attrib8 = HBufC8::NewL(attrib->Length());
		attrib8->Des().Copy(*attrib);
		aFile.Write(*attrib8);
		delete attrib8;
	}
 
	// Write data
	if (Data())
	{
		// Data is expected to be a text (for now)
		aFile.Write(_L8("\""));
		const TDesC* data = (TDesC*)Data();
		HBufC8* data8 = HBufC8::NewL(data->Length());
		data8->Des().Copy(*data);
		aFile.Write(*data8);
		delete data8;
		aFile.Write(_L8("\""));
	}

	if(count>0)
	{
		// Write ")
		aFile.Write(_L8("\")"));
	}
	else if(Data())
	{
		// Write )
		aFile.Write(_L8(")"));
	}
}

//-------------------------------------------------------------------------
// Write name of class in debug file
void CXmlElement::StartWriteL(RFile& aFile, TInt aIndentation)
{
	// Write itself
	WriteNameL(aFile, aIndentation);

	// Write children with one tab stop more
	aIndentation++;
	const TInt count = Count();
	for (TInt i = 0; i < count; i++)
	{
		Child(i)->StartWriteL(aFile, aIndentation);
	}
}

EXPORT_C void CXmlElement::WriteIntoFileL() 
{ 
	RFs fileSession;
	User::LeaveIfError(fileSession.Connect());
	CleanupClosePushL(fileSession);
	RFile file2;
	TPtrC filePath;
	filePath.Set( _L("c:\\tree.txt"));
	User::LeaveIfError(file2.Replace( fileSession, filePath , EFileWrite ));
	CleanupClosePushL(file2);

	// Write contents
	StartWriteL(file2, 0);
	CleanupStack::PopAndDestroy(2);	// fileSession, file2
}

//#endif
#ifndef __WAP_MONOLITHIC__
// Its a standalone DLL
//-------------------------------------------------------------------------

GLDEF_C void Panic(TXmlElementPanic aPanic)
{
	User::Panic(_L("XML-ELM"),aPanic);	
}

//-----------------------------------------------------------------------------
//    	Dlls entry point
//-----------------------------------------------------------------------------

#ifndef EKA2
GLDEF_C TInt E32Dll (TDllReason)
{
	return KErrNone;
}
#endif // EKA2
#endif // __WAP_MONOLITHIC__


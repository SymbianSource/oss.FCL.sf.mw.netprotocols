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

#ifndef __WBCONVERTER_H
#define __WBCONVERTER_H

#include <e32std.h>
#include <s32stor.h>
#include <f32file.h>

// forward declarations
//
class CWbxmlToken;
class MWbxmlConverterObserver;
class CByteList;
class CCnvCharacterSetConverter;

// consts, defines, enums
//
/**
@publishedAll
@deprecated
*/
_LIT8(KWbXmlVersion10,"<?xml version=\"1.0\"?>");

/**
@publishedAll
@deprecated
*/
_LIT8(KWbPublicIdStartWml,"<!DOCTYPE wml PUBLIC \"");
/**
@publishedAll
@deprecated
*/
_LIT8(KWbPublicIdStartSi,"<!DOCTYPE si PUBLIC \"");
/**
@publishedAll
@deprecated
*/
_LIT8(KWbPublicIdStartSl,"<!DOCTYPE sl PUBLIC \"");


/**
@publishedAll
@deprecated
*/
_LIT(KWbTokenDatabaseWml11,"\\RESOURCE\\WAPPUSH\\WMLC11.TOK");
/**
@publishedAll
@deprecated
*/
_LIT(KWbTokenDatabaseWml12,"\\RESOURCE\\WAPPUSH\\WMLC12.TOK");
/**
@publishedAll
@deprecated
*/
_LIT(KWbTokenDatabaseSI10,"\\RESOURCE\\WAPPUSH\\SI10.TOK");
/**
@publishedAll
@deprecated
*/
_LIT(KWbTokenDatabaseSL10,"\\RESOURCE\\WAPPUSH\\SL10.TOK");

/**
@publishedAll
@deprecated
*/
_LIT8(KWbPublicIdEnd11,"\" \"http://www.wapforum.org/DTD/wml_1.1.xml\">");
/**
@publishedAll
@deprecated
*/
_LIT8(KWbPublicIdEnd12,"\" \"http://www.wapforum.org/DTD/wml_1.2.xml\">");
/**
@publishedAll
@deprecated
*/
_LIT8(KWbPublicIdEndSI10,"\" \"http://www.wapforum.org/DTD/si.dtd\">");
/**
@publishedAll
@deprecated
*/
_LIT8(KWbPublicIdEndSL10,"\" \"http://www.wapforum.org/DTD/sl.dtd\">");

/**
@publishedAll
@deprecated
*/
_LIT8(KWbTagStart,"<");
/**
@publishedAll
@deprecated
*/
_LIT8(KWbTagStartClose,"</");
/**
@publishedAll
@deprecated
*/
_LIT8(KWbTagEndWithContent,">");
/**
@publishedAll
@deprecated
*/
_LIT8(KWbTagEndWithoutContent,"/>");
/**
@publishedAll
@deprecated
*/
_LIT8(KWbVariableStart,"$(");
/**
@publishedAll
@deprecated
*/
_LIT8(KWbVariableEscaped,":E)");
/**
@publishedAll
@deprecated
*/
_LIT8(KWbVariableUnescaped,":U)");
/**
@publishedAll
@deprecated
*/
_LIT8(KWbVariableNoescape,":N)");
/**
@publishedAll
@deprecated
*/
_LIT8(KWbVariableNormal,")");
/**
@publishedAll
@deprecated
*/
_LIT8(KWbQuote,"\"");
/**
@publishedAll
@deprecated
*/
_LIT8(KWbSpace," ");
/**
@publishedAll
@deprecated
*/
_LIT8(KWbTab,"\t");
/**
@publishedAll
@deprecated
*/
_LIT8(KWbLineBreak,"\n");

/**
@publishedAll
@deprecated
*/
const TInt8 KWbxmlIdIndex = 0;
/**
@publishedAll
@deprecated
*/
const TInt8 KWbxmlIdUnknown = 1;
/**
@publishedAll
@deprecated
*/
const TInt8 KWbxmlIdWml10 = 2;
/**
@publishedAll
@deprecated
*/
const TInt8 KWbxmlIdWta10 = 3;
/**
@publishedAll
@deprecated
*/
const TInt8 KWbxmlIdWml11 = 4;
/**
@publishedAll
@deprecated
*/
const TInt8 KWbxmlIdSI = 5;
/**
@publishedAll
@deprecated
*/
const TInt8 KWbxmlIdSL = 6;
/**
@publishedAll
@deprecated
*/
const TInt8 KWbxmlIdWml12 = 9;

// Entity translation literals
/**
@publishedAll
@deprecated
*/
_LIT8(KEntityDollar,"$$");
/**
@publishedAll
@deprecated
*/
_LIT8(KEntityAmp,"&amp;");
/**
@publishedAll
@deprecated
*/
_LIT8(KEntityLt,"&lt;");
/**
@publishedAll
@deprecated
*/
_LIT8(KEntityGt,"&gt;");
/**
@publishedAll
@deprecated
*/
_LIT8(KEntityQuot,"&quot;");
/**
@publishedAll
@deprecated
*/
_LIT8(KEntityApos,"&apos;");
/**
@publishedAll
@deprecated
*/
_LIT8(KEntityNbsp,"&nbsp;");
/**
@publishedAll
@deprecated
*/
_LIT8(KEntityShy,"&shy;");

/**
@publishedAll
@deprecated
*/
const TUint16 KWbxmlSkipLiteralEndTagMagicToken = 0xac1d;


//
// CWbxmlConverter
//
class RFileReadStream;
//##ModelId=3B6678E20007
class CWbxmlConverter : public CBase
/**
@publishedAll
@deprecated
*/
{
public:	// Methods
	//##ModelId=3B6678E30199
	~CWbxmlConverter();

	// NOTE: input data really is 8bit bytecode
	//##ModelId=3B6678E300F9
	IMPORT_C static CWbxmlConverter* NewL(RFs& aFs, MWbxmlConverterObserver* aObserver);
	//##ModelId=3B6678E300BD
	IMPORT_C static CWbxmlConverter* NewLC(RFs& aFs, MWbxmlConverterObserver* aObserver);
	//##ModelId=3B6678E30135
	IMPORT_C static CWbxmlConverter* NewL(RFs& aFs, MWbxmlConverterObserver* aObserver, const TDesC* aCharacterEncoding);
	//##ModelId=3B6678E3009F
	IMPORT_C void ProcessDataL(HBufC8& aBuffer);
	//##ModelId=3B6678E3009E
	IMPORT_C void CommitL();

private:	// Emumerations

	enum TGotoState
	{
		EGotoStreaming = 1,
		EGotoString,
		EGotoOpaque
	};
	enum TConvertState
	{
		EConvertError = 1,
		EConvertVersion,
		EConvertPublicIdValue,
		EConvertCharset,
		EConvertStringTable,
		EConvertPublicIdName,
		EConvertData,
		EConvertFinished
	};

private:	// Methods

	//##ModelId=3B6678E30080
	CWbxmlConverter(RFs& aFs);
	//##ModelId=3B6678E30045
	void ConstructL(MWbxmlConverterObserver* aObserver, const TDesC* aCharacterEncoding);

	//##ModelId=3B6678E30044
	void HandleDataL();
	//##ModelId=3B6678E30009
	void SetNextStateL(const TInt aResult, const TConvertState aState);
	//##ModelId=3B6678E30008
	TInt ConvertWbxmlVersionL();
	//##ModelId=3B6678E203E8
	TInt ConvertPublicIdValueL();	// Extract PublicId
	//##ModelId=3B6678E203E7
	TInt ConvertPublicIdNameL();	// Report PublicId
	//##ModelId=3B6678E203E6
	TInt ConvertCharsetL();
	//##ModelId=3B6678E203DE
	TInt ConvertStringTableL();
	//##ModelId=3B6678E203DD
	TInt ConvertDataL();
	//##ModelId=3B6678E203DC
	TInt ConvertDataStreamingL();
	//##ModelId=3B6678E203D3
	TInt ConvertDataStringL();
	//##ModelId=3B6678E203D2
	TInt ConvertOpaqueDataL();
	//##ModelId=3B6678E203B4
	HBufC16* FormatDateStringL(TDesC8& aData);

	//##ModelId=3B6678E20396
	TInt OpenTokenFileL(HBufC& aFileName);
	//##ModelId=3B6678E2035A
	void ReadTokenFileL(RFileReadStream& aStream, HBufC8& aBuffer);
	//##ModelId=3B6678E2033C
	TInt LoadTokensL(HBufC* aBuffer);

	//##ModelId=3B6678E2030A
	TInt ExtractMultiByte(TUint32* aMultiByte, TInt aSkipHeader);
	//##ModelId=3B6678E202E3
	void Indent(const TInt aCount) const;
	//##ModelId=3B6678E202E2
	void FinishStringL();
	//##ModelId=3B6678E202C4
	HBufC16* CharsetConvertDataL(TDesC8& aData);

	// Gets the full path to wmlc token table. Input aTokenFilePath is the path, without the drive.
	//
	// In:
	// aTokenFilePath	- the path to the token table file.
	//
	// Rtn: the full path to the token table file. Ownership is transfered to caller.
	//
	//##ModelId=3B6678E202A6
	HBufC* FindTokenFileL(const TDesC& aTokenFilePath) const;

	// Replaces characters originating from chracter entity references with the reference string
	// For example chracter '&' will be "decoded" back to "&amp;"
	//
	// In:
	// aString - The original string containing the undedcoded characters.
	//			 This string can get ReAllocated during the process - DO NOT USE THIS POINTER
	//           AFTER THE CALL to this method. (use the returned one instead...)
	// Return:
	// HBufC*  - Pointer to the string with replaced stuff
	//
	//##ModelId=3B6678E20288
	HBufC* ReplaceCharacterEntitiesL(HBufC* aString);

	// MWbxmlConverterObserver interface overloads
	//##ModelId=3B6678E2024C
	void OutputL(const TUint32 aSource) const;

	//##ModelId=3B6678E2022E
	TBool OutputCheckQuoteL(HBufC8& aSource) const;

	// Helper method to optimise the OutputL oveloads
	//##ModelId=3B6678E20211
	void BufferAndOutputL(const TDesC8& aSource) const;

	// Must exist for Binary Compatibility
	//##ModelId=3B6678E20210
	IMPORT_C virtual void CWbxmlConverter_Reserved1();

private:	// Attributes
	
	// For binary compatibility
	//##ModelId=3B6678E20206
	TAny* CWbxmlConverter_Reserved;
	
	// Attributes
	// Note: conversion table from disk ie. you can change it at runtime
	//##ModelId=3B6678E201F2
	CArrayPtrFlat<CWbxmlToken>* iTagArray;
	//##ModelId=3B6678E201D3
	CArrayPtrFlat<CWbxmlToken>* iAttArray;

	//##ModelId=3B6678E2018D
	MWbxmlConverterObserver* iObserver;

	//##ModelId=3B6678E20179
	TUint8 iWbxmlVersion;
	//##ModelId=3B6678E20165
	TUint32 iWbxmlCharset;
	//##ModelId=3B6678E20151
	TUint32 iPublicId;
	//##ModelId=3B6678E2013D
	TUint32 iPublicIdIndex;

	// Note: must handle source as 8bit data even in Unicode compile!
	//##ModelId=3B6678E2012B
	CByteList* iByteList;
	//##ModelId=3B6678E2011F
	HBufC8* iStringTable;
	//##ModelId=3B6678E2010D
	HBufC8* iPublicIdStr;
	//##ModelId=3B6678E20101
	RFs&	iFs;

	//##ModelId=3B6678E200ED
	TInt iState;
	//##ModelId=3B6678E200E3
	TUint32 iContinue;

	//##ModelId=3B6678E200D1
	TUint8 iVariable;
	//##ModelId=3B6678E200C5
	TUint8 hasAttrs;
	//##ModelId=3B6678E200B3
	TUint8 hasContent;


	//##ModelId=3B6678E200A7
	TInt iTagIndex;
	//##ModelId=3B6678E20095
	CArrayFixFlat<TUint16>* iTagStack;
	//##ModelId=3B6678E2006B
	TBool iRestrictedOutput;

	enum TParsing
	{
		EParseNot = 0,
		EParseTag,
		EParseAttrs
	};
	//##ModelId=3B6678E20059
	TParsing iParsing;
	//##ModelId=3B6678E2004F
	TBool iOpenQuote;

	//##ModelId=3B6678E20045
	mutable HBufC8* iOutputBuffer;		// Buffered output buffer : Optimisation

	//##ModelId=3B6678E2003B
	CCnvCharacterSetConverter* iCharsetConverter;

#ifdef __OUTPUT_WMLC
	//##ModelId=3B6678E20031
	RFile iWmlcFile;		// Debug stuff
	//##ModelId=3B6678E20025
	RFs iWmlcFs;			// Debug stuff
#endif

};

#endif // __WBCONVERTER_H

//
// End of file
//

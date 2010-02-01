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

#ifndef CONTENTHANDLERS_H
#define CONTENTHANDLERS_H

#include <xml/contenthandler.h>

class RFile;

namespace Xml
{

class TRebuildingContentHandler : public MContentHandler
	{
public:

	TRebuildingContentHandler(RFile& aOut);

	// From MContentHandler
	void OnStartDocumentL(const RDocumentParameters& aDocParam, TInt aErrorCode);
	void OnEndDocumentL(TInt aErrorCode);
	void OnStartElementL(const RTagInfo& aElement, const RAttributeArray& aAttributes, TInt aErrorCode);
	void OnEndElementL(const RTagInfo& aElement, TInt aErrorCode);
	void OnContentL(const TDesC8& aBytes, TInt aErrorCode);
	void OnStartPrefixMappingL(const RString& aPrefix, const RString& aUri, TInt aErrorCode);
	void OnEndPrefixMappingL(const RString& aPrefix, TInt aErrorCode);
	void OnIgnorableWhiteSpaceL(const TDesC8& aBytes, TInt aErrorCode);
	void OnSkippedEntityL(const RString& aName, TInt aErrorCode) ;
	void OnProcessingInstructionL(const TDesC8& aTarget, const TDesC8& aData, TInt aErrorCode);
	void OnExtensionL(const RString& aData, TInt aToken, TInt aErrorCode);
	void OnError(TInt aErrorCode);
	TAny* GetExtendedInterface(const TInt32 aUid);

	void SetStringPoolL ( RStringPool& aPool );
	
private:
	TBool IsForbiddenTagL ( const TDesC8& aTag );
	
public:
	TInt iError;

private:
	RFile& iOutFile;
	RStringPool* iStringPool;
	};

class TSimpleContentHandler : public MContentHandler
	{
public:
	TSimpleContentHandler();

	// From MContentHandler
	void OnStartDocumentL(const RDocumentParameters& aDocParam, TInt aErrorCode);
	void OnEndDocumentL(TInt aErrorCode);
	void OnStartElementL(const RTagInfo& aElement, const RAttributeArray& aAttributes, TInt aErrorCode);
	void OnEndElementL(const RTagInfo& aElement, TInt aErrorCode);
	void OnContentL(const TDesC8& aBytes, TInt aErrorCode);
	void OnStartPrefixMappingL(const RString& aPrefix, const RString& aUri, TInt aErrorCode);
	void OnEndPrefixMappingL(const RString& aPrefix, TInt aErrorCode);
	void OnIgnorableWhiteSpaceL(const TDesC8& aBytes, TInt aErrorCode);
	void OnSkippedEntityL(const RString& aName, TInt aErrorCode) ;
	void OnProcessingInstructionL(const TDesC8& aTarget, const TDesC8& aData, TInt aErrorCode);
	void OnExtensionL(const RString& aData, TInt aToken, TInt aErrorCode);
	void OnOutOfData();
	void OnError(TInt aErrorCode);
	TAny* GetExtendedInterface(const TInt32 aUid);
	
	void SetStringPoolL ( RStringPool& aPool );
	
private:
	TBool IsForbiddenTagL ( const TDesC8& aTag );

public:
	static const TInt KExpectedLeaveCode;

public:
	TBool iLeaveOnStartElement;
	TInt iNumElements;
	TInt iNumSkippedEntities;
	TInt iNumPrefixMappings;
	TInt iNumPrefixUnmappings;
	TInt iError;
	
	RStringPool* iStringPool;
	};

class TNamespaceContentHandler : public TSimpleContentHandler
	{
public:
	TNamespaceContentHandler(const TDesC8& aDefaultUri, const TDesC8& aElementPrefix, const TDesC8& aElementUri, 
		const TDesC8& aAttributePrefix, const TDesC8& aAttributeUri);

	// From MContentHandler
	void OnStartElementL(const RTagInfo& aElement, const RAttributeArray& aAttributes, TInt aErrorCode);
	void OnEndElementL(const RTagInfo& aElement, TInt aErrorCode);
	void OnStartPrefixMappingL(const RString& aPrefix, const RString& aUri, TInt aErrorCode);
	void OnEndPrefixMappingL(const RString& aPrefix, TInt aErrorCode);

private:
	const TDesC8& iDefaultUri;
	const TDesC8& iElementPrefix;
	const TDesC8& iElementUri;
	const TDesC8& iAttributePrefix;
	const TDesC8& iAttributeUri;
	enum { KNothingMapped, KDefaultNsMapped, KElementNsMapped, KAttributeNsMapped } iState;
	};

class TCapsContentHandler : public TSimpleContentHandler
	{
public:
	// From MContentHandler
	void OnStartElementL(const RTagInfo& aElement, const RAttributeArray& aAttributes, TInt aErrorCode);
	};
}
	
#endif // CONTENTHANDLERS_H

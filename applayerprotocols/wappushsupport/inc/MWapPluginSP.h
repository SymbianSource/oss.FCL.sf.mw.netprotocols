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

#ifndef __MWAPPLUGINSP_H__
#define __MWAPPLUGINSP_H__

// System includes
//
#include <e32def.h>

// Forward class declarations
//
class CDocumentNode;
class CNode;
class CDTDNode;
class CXmlElement;
class CAttributeLookupTable;


typedef TAny CWmlLibrary;


//##ModelId=3B666FA402F4
class MWapPluginSP
/**
@publishedAll
@deprecated
*/
	{
public:

/**
	Intended Usage	:	Resets the content data pointers in the given document 
	node and any dependents. This could also reset the data pointer in an 
	associated data dictionary node and its client document nodes.
	@since			6.0
	@param			aDocumentNode	The pointer to the document root node
	@param			aDataPtr		The pointer to the new data content
	@pre 			None
	@post			The data pointers in the specified document node and 
	dependents have been set to the new data
 */
	//##ModelId=3B666FA40331
	virtual void ResetDocumentNodeData(CDocumentNode* aDocumentNode, TAny* aDataPtr) const =0;

/**
	Intended Usage	:	Gets hold of the actual node that holds the data for the
	specified document node. The document data may actually be shared amongst 
	several document nodes and actually held in the data dictionary.
	@since			6.0
	@param			aDocumentNode The pointer to a document node
	@return			A pointer to node that actual holds the data.
 */
	//##ModelId=3B666FA4032C
	virtual CNode& DataStorageNode(const CDocumentNode* aDocumentNode) const =0;

/**
	Intended Usage	:	Preload the specified DTD for validating recieved WML 
	and WMLC decks. The document data handler needs to call this as soon as it
	knows what type of DTD the document is using. When the document completes,
	this DTD will be used to validate it before the 100% complete message is 
	passed up to the engine observer.
	@since			6.0
	@param			aDocType		The specified DTD type
	@param			aDTDURL			The URL of the DTD to be fetched
	@param			aDocRootNode	The root node of the document to be validated
	@return			A pointer to the DTD if the specified DTD is available, 
	otherwise NULL.
	@todo			Add the pre and post conditions.
	@pre 			None
	@post			Unspecified
 */
	//##ModelId=3B666FA40328
	virtual const CDTDNode* PrepareDTDL(const TDesC& aDocType, 
										const TDesC& aDTDURL,
										const CDocumentNode* aDocRootNode) =0;

/**
	Intended Usage	:	Provide a CWmlLibrary object. This object is ready to 
	process data and build a document tree to the given root node.
	@since			6.0
	@warning		The CWmlLibrary object is owned by the WAP Engine.
	@param			aDocRootNode	A pointer to the root node where the document
	tree is to be built.
	@return			A pointer the CWmlLibrary object. 
 */
	//##ModelId=3B666FA40326
	virtual CWmlLibrary* WmlLibL(CXmlElement* aDocRootNode) =0;

/**
	Intended Usage	:	Gets the Attribute Look Up Table.
	@since			6.0
	@return			A reference to the Attribute Look Up Table.
 */
	//##ModelId=3B666FA4031C
	virtual CAttributeLookupTable& AttributeLUT() =0;

	};

#endif	// __MWAPPLUGINSP_H__

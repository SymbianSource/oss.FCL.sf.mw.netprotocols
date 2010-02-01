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
// DTD nodes inline functions
// DTD Node
// 
//

inline CDTDNode::CDTDNode() : CBNFNode(0)
	{
	}

/** Destructor. */
inline CDTDNode::~CDTDNode()
	{
	}

/** Allocates and constructs a new parentless node.

@return New DTD node
*/
inline CDTDNode* CDTDNode::NewL()
	{
	CTypedNode<TInt,const TDesC*>* aNode = CTypedNode<TInt,const TDesC*>::NewL( 0, 0 );
	return REINTERPRET_CAST(CDTDNode*,aNode);
	}

/** Adds the completed attribute to the node. */
inline void CDTDNode::AddCompletedAttributeL()
	{
	AddAttributeL(CompletedAttrID(),NULL);
	}


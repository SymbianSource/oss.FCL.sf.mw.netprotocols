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
// Tree node implementations
// 
//


#include <cbnfnode.h>


/** Destructor. */
EXPORT_C
CBNFNode::~CBNFNode()
	{
	}

/** Allocates and constructs a new parentless node.

@return New parentless node
@param aType The type of the node
*/
EXPORT_C
CBNFNode*
CBNFNode::NewL(TInt aType)
	{
	return new (ELeave) CBNFNode(aType);
	}

EXPORT_C
CBNFNode::CBNFNode(TInt aType)
    : CTypedNode<TInt, const TDesC*>(aType,NULL)
    {
    }

/** Gets the pre-rule callback attribute type.

@return Pre-rule callback attribute type
*/
_SHAREDSTRINGBODY(PreRuleCallback);
/** Gets the post-rule callback attribute type.

@return Post-rule callback attribute type
*/
_SHAREDSTRINGBODY(PostRuleCallback);
/** Gets the reference attribute type.

@return Reference attribute type
*/
_SHAREDSTRINGBODY(Reference);
/** Gets the range start attribute type.

@return Range start attribute type
*/
_SHAREDSTRINGBODY(RangeStart);
/** Gets the range end attribute type.

@return Range end attribute type
*/
_SHAREDSTRINGBODY(RangeEnd);
/** Gets the match n or more times attribute type.

@return Match n or more times attribute type
*/
_SHAREDSTRINGBODY(NMoreCount);
/** Gets the match n or more minimum times attribute type.

@return Match n or more minimum times attribute type
*/
_SHAREDSTRINGBODY(NMoreMinimum);
/** Gets the match n or more maximum times attribute type.

@return Match n or more maximum times attribute type
*/
_SHAREDSTRINGBODY(NMoreMaximum);

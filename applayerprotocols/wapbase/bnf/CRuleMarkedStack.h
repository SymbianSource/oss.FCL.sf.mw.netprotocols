// Copyright (c) 2000-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// A speciales marked stack specifically for PARSE rules.
// Allows a rule stack to merge rules upto a given mark type.
// 
//

#ifndef __CRULEMARKEDSTACK_H__
#define __CRULEMARKEDSTACK_H__


// includes
#include <e32base.h>
#include <cmarkedstack.h>
#include <cbnfnode.h>

#pragma warning (disable : 4127) // conditional expression is constant


template <TBool StackOwnsEntry>
class CRuleMarkedStack : public CMarkedStack<CBNFNode, StackOwnsEntry>
/**	
@publishedAll
@deprecated
*/
	{
public:
	void MergeToMarkL(TInt aMarkType);
	};

#include <crulemarkedstack.inl>

#pragma warning ( default : 4127 ) // conditional expression is constant

#endif // __CRULEMARKEDSTACK_H__

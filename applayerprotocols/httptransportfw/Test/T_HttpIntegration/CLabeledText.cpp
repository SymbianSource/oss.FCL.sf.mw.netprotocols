// Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// $Header$
// Include Files  
// 
//

#include <e32std.h>                     //

//-----------------------------------------------------------------------------

#include "CLabeledText.h"                    // This module

//-----------------------------------------------------------------------------

CLabeledText *CLabeledText::NewL(const TDesC& aLabel, const TDesC& aValue )
{
CLabeledText* self = NewLC(aLabel, aValue);
CleanupStack::Pop();
return self; 
}

//-----------------------------------------------------------------------------

CLabeledText* CLabeledText::NewLC(const TDesC& aLabel, const TDesC& aValue )
{
CLabeledText* self = new (ELeave) CLabeledText();
CleanupStack::PushL(self);
self->ConstructL(aLabel, aValue);
return self;
}

//-----------------------------------------------------------------------------

CLabeledText::~CLabeledText()
{
delete iLabel;
iLabel = NULL;
delete iValue;
iValue = NULL;
}

//-----------------------------------------------------------------------------

void CLabeledText::ConstructL(const TDesC& aLabel, const TDesC& aValue )
{
iLabel = aLabel.AllocL();
iValue = aValue.AllocL();
}

//-----------------------------------------------------------------------------

TPtrC CLabeledText::Label() const
{
TPtrC label(iLabel->Des());
return label;
}

//-----------------------------------------------------------------------------

TPtrC CLabeledText::Value() const
{
TPtrC value(iValue->Des());
return value;
}

//-----------------------------------------------------------------------------

void CLabeledText::SetL(const TDesC& aValue)
{
delete iValue;
iValue = NULL;
iValue = aValue.AllocL();
}

//-----------------------------------------------------------------------------
//  End of File  
//-----------------------------------------------------------------------------


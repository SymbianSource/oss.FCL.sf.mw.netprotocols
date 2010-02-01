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

// System includes
#include <e32std.h>
#include <http/rhttppropertyset.h>// Class signature

// Local includes
#include "CHeaderField.h"
#include "CHTTPManager.h"


EXPORT_C RHTTPPropertySet::RHTTPPropertySet()
		: iImplementation(NULL)
	{
	}

EXPORT_C
TBool RHTTPPropertySet::Property(RStringF aPropertyName, 
								 THTTPHdrVal& aValue) const
	{
	__ASSERT_DEBUG(iImplementation, User::Invariant());
	CHeaderFieldParam* param = iImplementation->Parameter(aPropertyName);
	if (param)
		aValue = param->Value();
	return (param != NULL);
	}

EXPORT_C
void RHTTPPropertySet::SetPropertyL(RStringF aPropertyName, 
										   THTTPHdrVal aValue)
	{
	User::LeaveIfError(SetProperty(aPropertyName, aValue));
	}

EXPORT_C TInt RHTTPPropertySet::SetProperty(RStringF aPropertyName, THTTPHdrVal aValue)
    {
    __ASSERT_DEBUG(iImplementation, User::Invariant());
    CHeaderFieldParam* param = iImplementation->Parameter(aPropertyName);
    
    if (!param)
        {
        param = CHeaderFieldParam::New(aPropertyName, aValue);
        User::LeaveIfNull(param);
        TInt err = iImplementation->AddParam(param);
        if(err != KErrNone)
            {
            delete param;
            return err;
            }
        }

    param->SetValue(aValue);
    return KErrNone;
    }


EXPORT_C
void RHTTPPropertySet::RemoveProperty(RStringF aPropertyName)
	{
	__ASSERT_DEBUG(iImplementation, User::Invariant());
	CHeaderFieldParam* param = iImplementation->Parameter(aPropertyName);
	if (param)
		iImplementation->RemoveParam(param);
	}

EXPORT_C
void RHTTPPropertySet::RemoveAllProperties()
	{
	__ASSERT_DEBUG(iImplementation, User::Invariant());

	TInt numParams = iImplementation->NumParameters();
	while (numParams)
		{
		--numParams;
		CHeaderFieldParam* param = iImplementation->Parameter(numParams);
		if (param)
			iImplementation->RemoveParam(param);
		}
	}


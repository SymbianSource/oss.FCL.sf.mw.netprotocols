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

//system includes
#include <e32std.h>
#include <u32std.h>


//class signature
#include "clientrequest.h"

#ifndef CENTRAL_REPOSITORY_SERVER_TESTING

//
// TClientRequest
//
TClientRequest::TClientRequest(const RMessage2& aMessage)
: iMessage(aMessage)
	{
	}
	
TBool TClientRequest::IsNull() const
	{
	return iMessage.IsNull();
	}

TInt TClientRequest::Function() const
	{
	return iMessage.Function();
	}

void TClientRequest::Complete(TInt aReason) const
	{
	iMessage.Complete(aReason);
	}

void TClientRequest::Panic(const TDesC& aCategory, TInt aReason) const
	{
	iMessage.Panic(aCategory, aReason);
	}

TInt TClientRequest::Int0() const
	{
	return iMessage.Int0();
	}

TInt TClientRequest::Int1() const
	{
	return iMessage.Int1();
	}

TInt TClientRequest::Int2() const
	{
	return iMessage.Int2();
	}

TInt TClientRequest::Int3() const
	{
	return iMessage.Int3();
	}

TInt TClientRequest::GetDesLengthL(TInt aParam) const
	{
	return iMessage.GetDesLengthL(aParam);
	}

TInt TClientRequest::GetDesMaxLength(TInt aParam) const
	{
	return iMessage.GetDesMaxLength(aParam);
	}

void TClientRequest::ReadL(TInt aParam, TDes8& aDes, TInt aOffset) const
	{
	iMessage.ReadL(aParam, aDes, aOffset);
	}

void TClientRequest::ReadL(TInt aParam, TDes& aDes, TInt aOffset) const
	{
	iMessage.ReadL(aParam, aDes, aOffset);
	}
	
void TClientRequest::Read(TInt aParam, TDes8& aDes, TInt aOffset) const
	{
	iMessage.Read(aParam, aDes, aOffset);
	}

void TClientRequest::Read(TInt aParam, TDes& aDes, TInt aOffset) const
	{
	iMessage.Read(aParam, aDes, aOffset);
	}

TInt TClientRequest::Write(TInt aParam, const TDesC8& aDes, TInt aOffset) const
	{
	return iMessage.Write(aParam, aDes, aOffset);
	}

void TClientRequest::WriteL(TInt aParam, const TDesC8& aDes) const
	{
	iMessage.WriteL(aParam, aDes); 
	}

void TClientRequest::WriteL(TInt aParam, const TDesC& aDes, TInt aOffset) const
	{
	iMessage.WriteL(aParam, aDes, aOffset);
	}
	
TBool TClientRequest::CheckPolicy(const TSecurityPolicy& aSecurityPolicy,const char *aDiagnostic) const
	{
	return (aSecurityPolicy.CheckPolicy(iMessage,aDiagnostic));	
	}
	
//
//
#else //IF _UNIT_TESTING_

TClientRequest::TClientRequest()
: iFunction(KMinTInt), iCapability(0U), iCompletion(KRequestPending),
iStatusActive(EFalse), iIdentity(KNullUid)
	{
	Mem::FillZ(&iParams, sizeof(TInt) * KMaxMessageArguments);
	}

TBool TClientRequest::IsNull() const
	{
	return iFunction == KMinTInt;
	}

TInt TClientRequest::Function() const
	{
	return iFunction;
	}

TUid TClientRequest::Identity() const
	{
	return iIdentity;
	}

void TClientRequest::Complete(TInt aReason) const
	{
	iCompletion = aReason;
	if(iStatusActive)
		{
		TRequestStatus* stat = iStatus;
		User::RequestComplete(stat, aReason);
		}
	}

void TClientRequest::Panic(const TDesC& aCategory, TInt aReason) const
	{
	User::Panic(aCategory, aReason);
	}

TInt TClientRequest::Int0() const
	{
	return iParams[0];
	}

TInt TClientRequest::Int1() const
	{
	return iParams[1];
	}

TInt TClientRequest::Int2() const
	{
	return iParams[2];
	}

TInt TClientRequest::Int3() const
	{
	return iParams[3];
	}

TInt TClientRequest::GetDesLengthL(TInt aParam) const
	{
	const TDesC8* desPtr = (const TDesC8*)iParams[aParam];
	return desPtr->Length();
	}

TInt TClientRequest::GetDesMaxLength(TInt aParam) const
	{
	const TDes8* desPtr = (const TDes8*)iParams[aParam];
	return desPtr->MaxLength();
	}

void TClientRequest::ReadL(TInt aParam, TDes8& aDes, TInt aOffset) const
	{
	const TDesC8* desPtr = (const TDesC8*)iParams[aParam];
	aDes.Copy(desPtr->Mid(aOffset));
	}

void TClientRequest::ReadL(TInt aParam, TDes& aDes, TInt aOffset) const
	{
	const TDesC* desPtr = (const TDesC*)iParams[aParam];
	aDes.Copy(desPtr->Mid(aOffset));
	}
	
void TClientRequest::Read(TInt aParam, TDes8& aDes, TInt aOffset) const
	{
	const TDesC8* desPtr = (const TDesC8*)iParams[aParam];
	aDes.Copy(desPtr->Mid(aOffset));
	}

void TClientRequest::Read(TInt aParam, TDes& aDes, TInt aOffset) const
	{
	const TDesC* desPtr = (const TDesC*)iParams[aParam];
	aDes.Copy(desPtr->Mid(aOffset));
	}	

TInt TClientRequest::Write(TInt aParam, const TDesC8& aDes, TInt aOffset) const
	{
	TDes8* desPtr = (TDes8*)iParams[aParam];
	desPtr->Copy(aDes.Mid(aOffset));
	return KErrNone;
	}

void TClientRequest::WriteL(TInt aParam, const TDesC8& aDes) const
	{
	User::LeaveIfError(Write(aParam, aDes));
	}

void TClientRequest::WriteL(TInt aParam, const TDesC& aDes, TInt aOffset) const
	{
	TDes* desPtr = (TDes*)iParams[aParam];
	desPtr->Copy(aDes.Mid(aOffset));
	}
	
TBool TClientRequest::CheckPolicy(const TSecurityPolicy& /*aSecurityPolicy*/,const char * /*aDiagnostic*/) const
	{
	return iPolicyCheck;	
	}

#endif //CENTRAL_REPOSITORY_SERVER_TESTING

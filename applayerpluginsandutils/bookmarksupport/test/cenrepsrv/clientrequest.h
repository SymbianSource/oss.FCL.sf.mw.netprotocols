// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __CLIENTREQUEST_H__
#define __CLIENTREQUEST_H__

#include <e32std.h>

//
// MClientRequest
//
// Provides an interface identical to RMessage2.
// This facilitiates unit testing of the session by enabling RMessage2 to be
// replaced in unit test code.
//
class MClientRequest
/**
@internalComponent
*/
	{
	public:
		virtual TBool IsNull() const = 0;
		virtual TInt Function() const = 0;
		virtual void Complete(TInt aReason) const = 0;	
		virtual void Panic(const TDesC& aCategory, TInt aReason) const = 0;
	public:
		virtual TInt Int0() const = 0;
		virtual TInt Int1() const = 0;
		virtual TInt Int2() const = 0;
		virtual TInt Int3() const = 0;
	public:
		virtual TInt GetDesLengthL(TInt aParam) const = 0;
		virtual TInt GetDesMaxLength(TInt aParam) const = 0;
		virtual void ReadL(TInt aParam, TDes8& aDes, TInt aOffset=0) const = 0;
		virtual void ReadL(TInt aParam, TDes& aDes,  TInt aOffset=0) const = 0;
		virtual void Read(TInt aParam, TDes8& aDes, TInt aOffset=0) const = 0;
		virtual void Read(TInt aParam, TDes& aDes,  TInt aOffset=0) const = 0;
		virtual TInt Write(TInt aParam, const TDesC8& aDes, TInt aOffset=0) const = 0;
		virtual void WriteL(TInt aParam, const TDesC8& aDes) const = 0;
		virtual void WriteL(TInt aParam, const TDesC& aDes, TInt aOffset=0) const = 0;
		virtual TBool CheckPolicy(const TSecurityPolicy& aSecurityPolicy,const char *aDiagnostic) const =0;
	};

//
//

# ifndef CENTRAL_REPOSITORY_SERVER_TESTING
//
// TClientRequest
//
// Concrete implementation of MClientRequest that acts as an adaptor for
// RMessage.
//
class TClientRequest : public MClientRequest
/**
@internalComponent
*/
	{
	public:
		TClientRequest() {}
		TClientRequest(const RMessage2&);
		virtual TBool IsNull() const;
		virtual TInt Function() const;
		virtual void Complete(TInt aReason) const;
		virtual void Panic(const TDesC& aCategory, TInt aReason) const;
		virtual TInt Int0() const;
		virtual TInt Int1() const;
		virtual TInt Int2() const;
		virtual TInt Int3() const;
		virtual TInt GetDesLengthL(TInt aParam) const;
		virtual TInt GetDesMaxLength(TInt aParam) const;
		virtual void ReadL(TInt aParam, TDes8& aDes, TInt aOffset=0) const;
		virtual void ReadL(TInt aParam, TDes& aDes,  TInt aOffset=0) const;
		virtual void Read(TInt aParam, TDes8& aDes, TInt aOffset=0) const;
		virtual void Read(TInt aParam, TDes& aDes,  TInt aOffset=0) const;
		virtual TInt Write(TInt aParam, const TDesC8& aDes, TInt aOffset=0) const;
		virtual void WriteL(TInt aParam, const TDesC8& aDes) const;
		virtual void WriteL(TInt aParam, const TDesC& aDes, TInt aOffset=0) const;
		virtual TBool CheckPolicy(const TSecurityPolicy& aSecurityPolicy,const char *aDiagnostic) const;
	private:
		RMessage2 iMessage;
	};

//
//
# else //_UNIT_TESTING_
//
// TClientRequest
//
// Unit test version.
//
class TClientRequest : public MClientRequest
/**
@internalComponent
*/
	{
	public:
		TClientRequest();
		TClientRequest(const RMessage2&) {}; // Needed for compilation, but not used in testing.
		virtual TBool IsNull() const;
		virtual TInt Function() const;
		virtual void Complete(TInt aReason) const;
		virtual void Panic(const TDesC& aCategory, TInt aReason) const;
		virtual TInt Int0() const;
		virtual TInt Int1() const;
		virtual TInt Int2() const;
		virtual TInt Int3() const;
		virtual TInt GetDesLengthL(TInt aParam) const;
		virtual TInt GetDesMaxLength(TInt aParam) const;
		virtual void ReadL(TInt aParam, TDes8& aDes, TInt aOffset=0) const;
		virtual void ReadL(TInt aParam, TDes& aDes, TInt aOffset=0) const;
		virtual void Read(TInt aParam, TDes8& aDes, TInt aOffset=0) const;
		virtual void Read(TInt aParam, TDes& aDes,  TInt aOffset=0) const;
		virtual TInt Write(TInt aParam, const TDesC8& aDes, TInt aOffset=0) const;
		virtual void WriteL(TInt aParam, const TDesC8& aDes) const;
		virtual void WriteL(TInt aParam, const TDesC& aDes, TInt aOffset=0) const;
		virtual TBool CheckPolicy(const TSecurityPolicy& aSecurityPolicy,const char *aDiagnostic) const;
	public://debug only
		inline void SetRequest(TRequestStatus& aStatus);
		inline void SetFunction(TInt);
		inline void SetCapability(TUint32);
		inline void SetParam(TInt aParam, TInt aValue);
		inline void SetParam(TInt aParam, TDes8* aValue);
		inline void SetParam(TInt aParam, const TDesC8* aValue);
		inline void SetParam(TInt aParam, TDes* aValue);
		inline void SetParam(TInt aParam, const TDesC* aValue);
		inline void SetIdentity(TUid aIdentity);
		inline void SetPolicyCheck(TBool aPolicyCheck);
		TUid Identity() const;
		inline TInt CompletionCode() const;
		inline void Reset();
		
	private:
		TInt iFunction;
		TUint32 iCapability;
		mutable TInt iCompletion;
		TInt iParams[KMaxMessageArguments];
		mutable TRequestStatus* iStatus;
		TBool iStatusActive;
		TUid iIdentity;
		TBool iPolicyCheck;
	};

//
inline void TClientRequest::SetRequest(TRequestStatus& aStatus) {iStatus = &aStatus; iStatusActive = ETrue;}
inline void TClientRequest::SetFunction(TInt aFunction) { iFunction = aFunction; }
inline void TClientRequest::SetCapability(TUint32 aCap) { iCapability = aCap; }
inline void TClientRequest::SetParam(TInt aParam, TInt aValue) { iParams[aParam] = aValue; }
inline void TClientRequest::SetParam(TInt aParam, TDes8* aValue) { iParams[aParam] = TInt(aValue); }
inline void TClientRequest::SetParam(TInt aParam, const TDesC8* aValue) { iParams[aParam] = TInt(aValue); }
inline void TClientRequest::SetParam(TInt aParam, TDes* aValue) { iParams[aParam] = TInt(aValue); }
inline void TClientRequest::SetParam(TInt aParam, const TDesC* aValue) { iParams[aParam] = TInt(aValue); }
inline void TClientRequest::SetIdentity(TUid aIdentity) { iIdentity = aIdentity; }
inline TInt TClientRequest::CompletionCode() const { return iCompletion; }
inline void TClientRequest::Reset() { *this = TClientRequest(); }
inline void TClientRequest::SetPolicyCheck(TBool aPolicyCheck) {iPolicyCheck=aPolicyCheck;}

//
//
#endif //CENTRAL_REPOSITORY_SERVER_TESTING

//
//
//
#endif //__CLIENTREQUEST_H__

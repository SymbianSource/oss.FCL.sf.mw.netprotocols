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

#ifndef __DRIVERCUSTOMVALIDATORS_H__
#define __DRIVERCUSTOMVALIDATORS_H__

// System includes
#include <http/thttpevent.h>

// Local includes
#include "cchatvalidator.h"
#include "t_wspeventdriver/cwspeventdriver.h"

// Forward declarations


/** Definition of class 'CChatTHTTPEventValidator'.  This class validates THTTPEvent parameter values,
	in method invocations from the unit under test.
*/
class CChatTHTTPEventValidator : public CChatValidator
	{
public: // methods

	/** Factory construction: create a validator based on a THTTPEvent value.  This is used to package parameter values
		passed into the Driver.
	*/
	static CChatTHTTPEventValidator* NewL(const TDesC& aParamName, THTTPEvent aEvent);

	/** Factory construction: create a validator based on an 'stringified' parameter.  This is used to package parameter
		values read from the chat script.
	*/
	static CChatTHTTPEventValidator* NewL(const CChatParameter& aScriptParam);

	/** Destructor
	*/
	virtual ~CChatTHTTPEventValidator();

	/** Do the validation check.  Validate that the descriptor value is equivalent to the stringified parameter passed in.
	*/
	virtual TBool IsValid(const CChatParameter& aParamToCompare, TDes& aErrorMessage) const;

	/** Get the type of this validator
	*/
	virtual const TDesC& ClassName() const;

	/** Obtain the THTTPEvent held by this validator
	*/
	THTTPEvent EventValue() const;

private:

	/** Default constructor
	*/
	CChatTHTTPEventValidator();

	/** Normal constructor
	*/
	CChatTHTTPEventValidator(THTTPEvent aEvent);

	/** Second phase construction, stores a named parameter
	*/
	void ConstructL(const TDesC& aParamName);

	/** Second phase construction, reads a stringified parameter and its name based on the data from the chat script
	*/
	void ConstructL(const CChatParameter& aScriptParam);

	/** Helper method to convert stringified THTTPEvent's into enum values
	*/
	static TInt Convert(const CChatParameter& aParamToCompare);

	/** Helper method to convert stringified THTTPEvent's into integer values
	*/
	static TInt ConvertInteger(const CChatParameter& aParamToCompare);

private:

	/** The 'HTTP Event' value to be validated
	*/
	THTTPEvent iEventVal;
	};


/** Definition of class 'CChatTHdrValTypeValidator'.  This class validates THdrValType parameter values,
	in method invocations from the unit under test.
*/
class CChatTHdrValTypeValidator : public CChatValidator
	{
public: // methods

	/** Factory construction: create a validator based on a THdrValType value.  This is used to package parameter values
		passed into the Driver.
	*/
	static CChatTHdrValTypeValidator* NewL(const TDesC& aParamName, CDriverTrans::THdrValType aType);

	/** Factory construction: create a validator based on an 'stringified' parameter.  This is used to package parameter
		values read from the chat script.
	*/
	static CChatTHdrValTypeValidator* NewL(const CChatParameter& aScriptParam);

	/** Destructor
	*/
	virtual ~CChatTHdrValTypeValidator();

	/** Do the validation check.  Validate that the descriptor value is equivalent to the stringified parameter passed in.
	*/
	virtual TBool IsValid(const CChatParameter& aParamToCompare, TDes& aErrorMessage) const;

	/** Get the type of this validator
	*/
	virtual const TDesC& ClassName() const;

	/** Obtain the THdrValType held by this validator
	*/
	CDriverTrans::THdrValType TypeValue() const;

private:

	/** Default constructor
	*/
	CChatTHdrValTypeValidator();

	/** Normal constructor
	*/
	CChatTHdrValTypeValidator(CDriverTrans::THdrValType aType);

	/** Second phase construction, stores a named parameter
	*/
	void ConstructL(const TDesC& aParamName);

	/** Second phase construction, reads a stringified parameter and its name based on the data from the chat script
	*/
	void ConstructL(const CChatParameter& aScriptParam);

	/** Helper method to convert stringified THdrValType's into enum values
	*/
	static CDriverTrans::THdrValType Convert(const CChatParameter& aParamToCompare);

private:

	/** The header value type to be validated
	*/
	CDriverTrans::THdrValType iType;
	};


#endif // __DRIVERCUSTOMVALIDATORS_H__

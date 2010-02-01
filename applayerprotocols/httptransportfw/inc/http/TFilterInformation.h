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

/**
 @file TFilterInformation.h
 @warning : This file contains Rose Model ID comments - please do not delete
*/

#ifndef __TFILTERINFORMATION_H__
#define __TFILTERINFORMATION_H__


//##ModelId=3C4C37DF0343
struct TFilterInformation
/**
This structure is used by the TFilterConfigurationIterator to hold the information of the
current filter. This structure simply contains public members containing the following
information, the display name, data type, version, UID, category and install status.
@publishedAll
@released
@see			TFilterConfigurationIterator
*/
	{
public:
	/**
	Public enum defining the filter categories available to the client.
	*/
	enum TFilterCategory
		{
		EImplicitFilter,
		EExplicitFilter
		};

	/** 
		Contains a constant reference to the descriptor containing the filter's display name 
	*/
	//##ModelId=3C4C37DF03A7
	const TDesC& iDisplayName;

	/** 
		Contains a constant reference to the descriptor containing the filter's data type 
	*/
	//##ModelId=3C4C37DF039D
	const TDesC8& iDataType;

	/**
		The filter's version number
	*/
	//##ModelId=3C4C37DF038B
	TInt iVersion;

	/** 
		The filter's plugin UID
	*/
	//##ModelId=3C4C37DF0381
	TUid iFilterUid;

	/**
		The filter's category
	*/
	//##ModelId=3C4C37DF0375
	TFilterCategory iFilterCategory;

	/** 
		The install status of the filter. Contains ETrue if installed, otherwise EFalse 
	*/
	//##ModelId=3C4C37DF036B
	TBool iInstallStatus;

	/**
	Default constructor that simply initialises the data members.
	@param			aDisplayName Reference to descriptor containing the display name
	@param			aDataType Reference to descriptor containing the data type
	@param			aVersion Integer of the version number
	@param			aFilterUid The UID of the filter plugin
	@param			aFilterCategory The category of the filter
	@param			aInstallStatus The install status of the filter plugin. Contains ETrue
					if installed or EFalse otherwise.
	@pre			None
	@post			Object is fully constructed and initialised
	*/
	//##ModelId=3C4C37DF03B1
	inline TFilterInformation(const TDesC& aDisplayName, const TDesC8& aDataType,
					   TInt aVersion, TUid aFilterUid,
					   TFilterCategory aFilterCategory, TBool aInstallStatus);
	};

// Inline constructor, initialises data members
inline TFilterInformation::TFilterInformation(const TDesC& aDisplayName, const TDesC8& aDataType,
											  TInt aVersion, TUid aFilterUid,
											  TFilterCategory aFilterCategory, TBool aInstallStatus)
	:iDisplayName(aDisplayName),
	iDataType(aDataType),
	iVersion(aVersion),
	iFilterUid(aFilterUid),
	iFilterCategory(aFilterCategory),
	iInstallStatus(aInstallStatus)
	{
	}

#endif // __TFILTERINFORMATION_H__

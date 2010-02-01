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
 @file CEComFilter.h
 @warning : This file contains Rose Model ID comments - please do not delete
*/

#ifndef	__CECOMFILTER_H__
#define	__CECOMFILTER_H__

// System includes
#include <e32base.h>
#include <ecom/ecom.h>
#include <http/rhttpsession.h>

/**
The ECom filter interface UID
@publishedAll
@released
*/
const TUid KUidFilterPluginInterface = {0x101F446D}; 


//##ModelId=3C4C0F400280
class CEComFilter : public CBase
/**
This class serves as the base class for ECom filter plugins. It provides minimal functionality
to simply install and destroy filters specified by their UID or data type. The RHTTPSession
passed in is passed onto the filter as a TAny* pointer and therefore must be casted back to
an RHTTPSession pointer by the filter.
@publishedAll
@released
*/
	{
public: // Methods
	/**
	@fn				InstallFilterL(RHTTPSession aSession, const TUid aEComFilterUid)
	Intended Usage:	This method is used to install filters to the filter queue. The implementation
					UID of the filter plugin is passed to identify the plugin that is required.
					The returned pointer is not owned as the filters must be self-destroying when
					they are unloaded.
	@param			aSession A handle to the transport session
	@param			aEComFilterUid The implementation UID of the plugin
	@return			Pointer to the newly installed plugin
	@pre 			The session had already been setup
	@post			The filter(s) have been installed
	*/
	//##ModelId=3C4C0F4002A8
	inline static CEComFilter* InstallFilterL(RHTTPSession aSession, const TUid aEComFilterUid);

	/**
	@fn				InstallFilterL(RHTTPSession aSession, const TDesC8& aEComFilterDataType)
	Intended Usage:	This method is used to install filters to the filter queue. The implementation
					UID of the filter plugin is passed to identify the plugin that is required.
					The returned pointer is not owned as the filters must be self-destroying when
					they are unloaded.
	@param			aSession A handle to the transport session
	@param			aEComFilterDataType A descriptor containing the datatype of the filter to load
	@return			Pointer to the newly installed plugin
	@pre 			The session had already been setup
	@post			The filter(s) have been installed
	*/
	//##ModelId=3C4C0F4002AB
	inline static CEComFilter* InstallFilterL(RHTTPSession aSession, const TDesC8& aEComFilterDataType);

	/**
	@fn				~CEComFilter()
	Intended Usage:	Virtual destructor
	@pre 			The object identified by the destructor key in iEcomDtorID exists
	@post			The object is destroyed
	*/
	//##ModelId=3C4C0F40029E
	inline ~CEComFilter();

private: // Attributes
	// The ECom destructor key identifier
	//##ModelId=3C4C0F400297
	TUid iEcomDtorID;
	};

//----------------------------------------------------------------------------------------

inline CEComFilter* CEComFilter::InstallFilterL(RHTTPSession aSession, const TUid aEComFilterUid)
	{
	TAny* filterPlugin = REComSession::CreateImplementationL(aEComFilterUid, _FOFF(CEComFilter,iEcomDtorID), &aSession);
	return REINTERPRET_CAST(CEComFilter*, filterPlugin);
	}

inline CEComFilter* CEComFilter::InstallFilterL(RHTTPSession aSession,const TDesC8& aEComFilterDataType)
	{
	TEComResolverParams resolverParams;
	resolverParams.SetDataType(aEComFilterDataType);
	// This will leave if the plugin is not found
	TAny* filterPlugin = REComSession::CreateImplementationL(KUidFilterPluginInterface, 
											_FOFF(CEComFilter,iEcomDtorID), 
											&aSession, resolverParams);
	return REINTERPRET_CAST(CEComFilter*, filterPlugin);
	}

inline CEComFilter::~CEComFilter()
	{
	REComSession::DestroyedImplementation(iEcomDtorID);
	}

#endif // __CECOMFILTER_H__

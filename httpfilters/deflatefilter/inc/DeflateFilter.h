/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  ?Description
*
*/


#ifndef __DEFLATEFILTER_H_
#define __DEFLATEFILTER_H_

#include <http/mhttpfilter.h>
#include <http/rhttpsession.h>
#include <http/framework/httplogger.h>
#include "DeflateDataSupplier.h"
#include "deflatefilterinterface.h"

#ifndef _DEBUG_LOGGING
#undef _LOGGING
#endif

class RHTTPResponse;
class RHTTPTransaction;

/**
*  major interface class for DEFLATE HTTP Filter
*
*  @lib DeflateFilter.lib
*  @since Rubik_0328.222
*/

class CDeflateFilter : public CHttpDeflateFilter, public MHTTPFilter
{
public:

	static CHttpDeflateFilter* InstallFilterL(TAny* aSession);	
 
	virtual ~CDeflateFilter();

public:	// Methods from MHTTPFilterBase

	/// @see MHTTPFilterBase::MHFRunL
	virtual void MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent);

	/// @see MHTTPFilterBase::MHFSessionRunL
	virtual void MHFSessionRunL(const THTTPSessionEvent& aEvent);

	/// @see MHTTPFilterBase::MHFRunError
	virtual TInt MHFRunError(TInt aError, RHTTPTransaction aTransaction,
							 const THTTPEvent& aEvent);

	/// @see MHTTPFilterBase::MHFSessionRunError
	virtual TInt MHFSessionRunError(TInt aError, const THTTPSessionEvent& aEvent);

public:	// Methods from MHTTPFilter

	/// @see MHTTPFilter::MHFUnload
	virtual void MHFUnload(RHTTPSession aSession, THTTPFilterHandle aHandle);

	/// @see MHTTPFilter::MHFLoad
	virtual void MHFLoad(RHTTPSession aSession, THTTPFilterHandle aHandle);

 private:
	
	CDeflateFilter(RHTTPSession aSession);

	///ContructL also Installs the filter.
	void ConstructL(RHTTPSession aSession);
	
#if defined (_DEBUG) && defined (_LOGGING)
	void DumpHeadersL( RHTTPHeaders );
#endif

	void AlterRequestHeadersL( const RHTTPTransaction& aTrans );
	void CheckResponseHeadersL( const RHTTPTransaction& aTrans );
	void ProcessBodyPartL( RHTTPTransaction& aTrans );
	TInt GetTransIdx( const RHTTPTransaction& aTrans ) const;
	void Cleanup( const RHTTPTransaction& aTrans );
	void CleanupAll();
	void SetHeaderL(RHTTPHeaders aHeaders, TInt aHdrField, const TDesC8& aHdrValue) const;
 private:
	/// A count to make sure we delete at the right time.
	TInt iLoadCount;
	RStringPool iStringPool;
	RHTTPSession iSession;
	RPointerArray<CDeflateDataSupplier>		iDataSups;

	//x-gzip functionality
	RStringF iXGzipVal;

	// used for log purpose
	__DECLARE_LOG
};


#endif //__DEFLATEFILTER_H_

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
 @file
 @warning : This file contains Rose Model ID comments - please do not delete
*/

#ifndef __RHTTPTRANSACTIONPROPERTYSET_H__
#define __RHTTPTRANSACTIONPROPERTYSET_H__ 

// System includes
#include <http/rhttppropertyset.h>


//##ModelId=3B1E670402C2
class RHTTPTransactionPropertySet : public RHTTPPropertySet
/** 
The set of properties of a transaction. This is used by filters to
store per-transaction information, and is also used by the client
to specify transaction properties (such as no caching, reload and
so on) 

Transaction properties always take precendence over session properties.

To lookup/change any of the following properties, use 
RHTTPPropertySet::Property(...) and RHTTPPropertySet::SetPropertyL(...) 

To remove properties, use RHTTPPropertySet::RemoveProperty(...) for a named property,
or RHTTPPropertySet::RemoveAllProperties() for all.


The following values are currently defined.

HTTP::EProxyUsage (HTTP::EDoNotUseProxy | HTTP::EUseProxy)

HTTP::EProxyAddress (A Uri for the Proxy server. Should include the port number
if it is not the default of 8080)

HTTP::EHttpPipelining (default HTTP::EEnablePipelining) (HTTP::EEnablePipelining | HTTP::EDisablePipelining)
@publishedAll
@released
*/
	{
	};

#endif //	__RHTTPTRANSACTIONPROPERTYSET_H__ 

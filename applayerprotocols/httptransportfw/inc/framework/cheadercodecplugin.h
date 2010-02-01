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

#ifndef __CHEADERCODECPLUGIN_H__
#define __CHEADERCODECPLUGIN_H__

#include <e32base.h>
#include <http/framework/cheadercodec.h>
#include <stringpool.h>
#include <ecom/ecom.h>

/**
ECOM interface definition.


This interface is derived from CHeaderCodec and it performs construction of Codec plugins 
and allows the caller to determine which codecs are used and in which order. 


The caller specifies the order of the desired codecs using a slash-separated string of names, 
starting with the protocol name.


for example:


@code
_LIT8(KHttpClientCodecName, "HTTP/client");
CHeaderCodec* codec = CHeaderCodecPlugin::NewL( KHttpClientCodecName, iSession.StringPool());
@endcode


This says to use, for the HTTP protocol a client Codec, in addition to these are the implicit
general and default HTTP Codecs.


@publishedPartner
@released
*/

class CHeaderCodecPlugin : public CHeaderCodec
	{
public: // methods
	IMPORT_C static CHeaderCodecPlugin* NewL(const TDesC8& aCodec, RStringPool aStringPool);
	IMPORT_C virtual ~CHeaderCodecPlugin();

protected:
	IMPORT_C virtual CHeaderCodec* CreateDelegateCodecL(RStringPool aStringPool) const;

private:	// methods
	static TUid FindImplementationUidL(const TDesC8& aCodec, TPtrC8& aNext);
	void ConstructL(TDesC8& aDelegateName);
	
private:	// attributes
	HBufC8*  iDelegateName; // the name of the delegate to create
	TUid  iDtor_ID_Key;   // Destructor key identification required for ECOM
	TAny* iData;		//reserved for future use
	};

#endif //__CHEADERCODECPLUGIN_H__
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

#include <http/framework/cheadercodecplugin.h>

const TUid KUidHeaderCodecPlugin = {0x10272E5C};



/**
Constructs a CHeaderCodecPlugin codec object.


The caller specifies the order of the desired codecs using the codec list (aCodec). This is slash-separated 
string of names, starting with the protocol name. For example: "protocol/name1/name2/../nameN". 
When attempting to encode or decode a header it tries to encode with the first codec it finds in the list 
which can handle the specified header. It always starts with the final one and ends with the first one in the list 
(it goes through the list from right to left). The example list above will try the codecs in the following order:

  
@code
	"protocol/nameN"
	...
	"protocol/name2"
	"protocol/name1"
	"protocol/<!-- comment -->*"	(the "general codec" for the protocol)
	"protocol"		(the "default codec" for the protocol)
@endcode


There are three types of codecs. Every codec consists of a protocol and an optional name. A "named codec" 
takes the form "protocol/name". For example, the named codec "HTTP/client" is a codec for encoding and 
encoding headers used by a "client" (i.e the user agent) using the HTTP protocol. Note that a codec "WSP/client" 
would encode those same headers using WSP. Any number of named protocols can be specified in the list. 


The "general codec" for a protocol takes the form "protocol/<!-- comment -->*". This is always implicitly loaded, and 
will always be tried before the default codec. Note that not all protocols have a general codec.  


The "default codec" for a protocol takes the form "protocol". This is always the final codec tried.


In the example, the named codec "protocol/nameN" will be tried first and the default codec for "protocol" 
will be tried last. 


If a general codec exists for the protocol (that is, when the  plugin matches with  "protocol/<!-- comment -->*") it will be 
tried after "protocol/name1", but before "protocol". 


The selection of codecs uses the following rules: 
      
		
    1.  If there is more than one slash, the implementation will load the last codec listed. 
		For "protocol/name1/name2/nameN" it will load "protocol/nameN". If that codec cannot handle 
		a desired header, the codec will load a "delegate" using a new codec list: "protocol/name1/name2".     


    2.  If there is only one slash, and the list takes the form "protocol/name1". The implementation 
		will load the "name1" codec for this protocol (i.e. "protocol/name1"). The named codec's 
		delegate would be the general codec ("protocol/<!-- comment -->*")


    3.  If there is one slash, and the list takes the form "protocol/<!-- comment -->*", the implementation will 
		load the general codec for this protocol. The general codec's delegate would be the default 
		one ("protocol").

  
    4.  If there are no slashes in the list, aCodec will be assumed to be default codec for the 
		protocol. The default codec has no delegate. For example, if aCodec is "HTTP" it will load 
		the default HTTP plugin "HTTP".

  
Note that if a listed codec does not exist, it will be silently skipped. This method will only fail if 
it finds no codecs for the protocol.


for example: 


@code
_LIT8(KHttpClientCodecName, "HTTP/client/Delta/WebDav"); 
CHeaderCodec* codec = CHeaderCodecPlugin::NewL( KHttpClientCodecName, iSession.StringPool()); 
@endcode

  
This says to use, for the HTTP protocol, in order of preference, a WebDav Codec ("HTTP/WebDav"), a Delta-encoding 
Codec ("HTTP/Delta"), and the client Codec ("HTTP/client"). In addition to these are the implicit general ("HTTP/<!-- comment -->*") 
and default ("HTTP") HTTP Codecs. In this case the implementation will load "HTTP/ WebDav" which will in turn use 
the codec list "HTTP/client/Delta" to load its delegate.
@param		aCodec	Protocol and optional Codec names seperated by slash.
@param		aStringPool	The current string pool.
@return		A pointer to a fully initialised object.
*/
EXPORT_C CHeaderCodecPlugin* CHeaderCodecPlugin::NewL(const TDesC8& aCodec, RStringPool aStringPool)
	{
	TPtrC8 next;
	TUid implementation = CHeaderCodecPlugin::FindImplementationUidL(aCodec,next);
	TAny* ptr = REComSession::CreateImplementationL(implementation,
		 		_FOFF(CHeaderCodecPlugin, iDtor_ID_Key),  &aStringPool);
	CHeaderCodecPlugin* codec = REINTERPRET_CAST(CHeaderCodecPlugin*, ptr);
	CleanupStack::PushL(codec);
	codec->ConstructL(next);
	CleanupStack::Pop(codec);
	return codec;
	}

/**
Constructs a delegate Codec


All the classes derived from this class must call bases class CreateDelegateCodecL.
@param		aStringPool	The current string pool.
@return		A pointer to a fully initialised object.
*/
EXPORT_C CHeaderCodec* CHeaderCodecPlugin::CreateDelegateCodecL(RStringPool aStringPool) const
	{
	return CHeaderCodecPlugin::NewL(*iDelegateName, aStringPool);
	}

/**
Destructor


Destructs previously created codec instance.
*/
EXPORT_C CHeaderCodecPlugin::~CHeaderCodecPlugin()
	{
	delete iDelegateName;
	REComSession::DestroyedImplementation(iDtor_ID_Key);
	}


/**
Finds Implementation UID.
@param		aCodec	Codec names.
@param		aNext	Pointer to 8-bit data that is to recieve the delegate name.
@return		always returns a valid UID of the codec plugin.
@leave		KErrNotFound	Specified Codec.
@internalComponent
*/
TUid CHeaderCodecPlugin::FindImplementationUidL(const TDesC8& aCodec, TPtrC8& aNext)
{
	TEComResolverParams resolverParams;
	TPtrC8 codec(aCodec);		// The codec to look for (may be changed so we need our own pointer)
	TUid id = KNullUid;
	
	while(id == KNullUid)		// while there is no match
		{
		// if the string we need to match can't be made by a TPtrC, we will create it in resolve
		HBufC8* resolve  = NULL;			
		resolverParams.SetWildcardMatch(ETrue);
		//  find the first slash of text in the form: "protocol/type/name/etc" or  "protocol/type" or  "protocol"
		//	This will be the name of the protocol
		TInt firstSlash = codec.Locate('/');		
		if( firstSlash < 0 )   // no slashes -- this is just the protocol name ("protocol")
			{
			resolverParams.SetDataType(codec);		// look for a match for "protocol"
			aNext.Set(KNullDesC8);				// no next codec in chain.
			}
		else if( firstSlash == codec.Length() - 1)  // just  "protocol/"
			// it's easier to use this than to pass around with the *, since this 
			// way aNext always points to the original descriptor
			{
			resolve = HBufC8::NewL(codec.Length()+1);	
			TPtr8 res = resolve->Des();
			res.Copy(codec);
			res.Append('*');
			resolverParams.SetDataType(res);	 // the string "protocol/*"
			aNext.Set( codec.Left(firstSlash) );	// next codec is "protocol" (no slash)
			}		
		else // there is at least one slash with subsequent content ("protocol/name")
			{
			TInt lastSlash = codec.LocateReverse('/');	// find the last slash

			// must match exactly otherwise we'd always resolve "protocol/*"
			resolverParams.SetWildcardMatch(EFalse); 

			if (lastSlash > firstSlash)
				{// has at least 2 slashes ("protocol/name1/name2")
				// resolve = up to and including first slash, and everything after last slash  ("protocol/name2")
				resolve = HBufC8::NewL(firstSlash+1 + codec.Length()-lastSlash); 
				TPtr8 res = resolve->Des();
				res.Copy(codec.Left(firstSlash+1)); // up to and including first slash
				res.Append(codec.Mid(lastSlash+1));  // everything after last slash
				aNext.Set( codec.Left(lastSlash) ); // up to,but not including the last slash
				resolverParams.SetDataType(res); 
				}
			else 	// just the one slash ("protocol/name")
				{
				resolverParams.SetDataType(codec);	// look for "protocol/name"
				aNext.Set(codec.Left(firstSlash+1));	// next codec is "protocol" 
				}
			}
		CleanupStack::PushL(resolve);	 //  resolve might be NULL, but that's ok
		RImplInfoPtrArray implArray;		// the list of plugins will be put here.	
		REComSession::ListImplementationsL(KUidHeaderCodecPlugin, resolverParams,  implArray);
		TInt count = implArray.Count();;			// number of matches
		CleanupStack::PopAndDestroy(resolve);	// don't need this anymore
		if(count != 0) 	// we found the match. save it in id
			{
			id = implArray[0]->ImplementationUid();
			}
		implArray.ResetAndDestroy();
		if(count == 0 && aNext.Length() == 0) // no more codecs to try
			{
			User::Leave(KErrNotFound); // No suitable implementation is present
			}
		codec.Set(aNext);
		}
		
		
	return id;
	
	}

/**
Allocates storage for holding delegate name.
@param		aDelegateName	Delegate name.
@internalComponent
*/
void CHeaderCodecPlugin::ConstructL(TDesC8& aDelegateName)
{
iDelegateName = aDelegateName.AllocL();
}

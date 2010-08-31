/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  HTTP filter for handling dynamic accept headers
*
*/



// INCLUDE FILES
#include <http.h>

#include <bldvariant.hrh>

#include "featmgr.h"

// User Includes
#include "HttpFilterAcceptHeader.h"
#include "httpfiltercommonstringsext.h"

#include "HttpFilterAcceptHeaderVariant.hrh"
#include <syslangutil.h>

#include <centralrepository.h>
#include <HttpFilterAcceptHeaderPrivateCRKeys.h>
#include <BrowserUiSDKCRKeys.h>

// EXTERNAL FUNCTION PROTOTYPES

// CONSTANTS
_LIT8(KFieldSeparator, "\n");
_LIT8(KCommaSeperator, ", "); 
_LIT8(KQHalfValue, ";q=0.5"); 
_LIT8(KQWholeValue, ";q=1.0"); 

// Initial number of MIME types. This is just for setting the granuality.
const TUint8 KNumDefaultNumOfTypes = 20;

// Initial size for mime type buffer. 
const TUint KSizeofMimeTypeBuffer = 512;

// List of MIME types supported by the browser
_LIT8(KBrowserMimeTypeList,"text/html,application/vnd.wap.xhtml+xml,application/xhtml+xml,text/css,multipart/mixed,text/vnd.wap.wml,application/vnd.wap.wmlc,application/vnd.wap.wmlscriptc");

_LIT8( KStarSlashStar, "*/*" );

_LIT8(KCssStr, "text/css" ); 

_LIT8(KAppJavaScript, "application/x-javascript"); 

_LIT8(KECMAScript, "text/ecmascript"); 

_LIT8(KJavaScript, "text/javascript"); 

// LOCAL FUNCTIONS

// -----------------------------------------------------------------------------
// PanicHttpFilter
// Returns: void
// -----------------------------------------------------------------------------
//
void PanicHttpFilter( TInt aErr = KErrUnknown )
    {
    User::Panic( _L( "HTTP FILTER ACCEPT HEADER" ), aErr );
    }

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CHttpFilterAcceptHeader::CHttpFilterAcceptHeader
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CHttpFilterAcceptHeader::CHttpFilterAcceptHeader( RHTTPSession* aSession ): 
	iSession( aSession )
	{
    __ASSERT_DEBUG( aSession != NULL, PanicHttpFilter( KErrArgument ) );
	}


// -----------------------------------------------------------------------------
// CHttpFilterAcceptHeader::ReadFeatureBitmaskL
// Read feature bitmask into member variable
// -----------------------------------------------------------------------------
//
void CHttpFilterAcceptHeader::ReadFeatureBitmaskL( )
    {
  //Create a new repository with KSDUidCacheManager
  CRepository* repository = CRepository::NewL(KCRUidAcceptHeaderLV); 
  User::LeaveIfNull(repository);

  //Read the setting for CacheEnabled
  repository->Get(KAcceptHeaderLV, iFeatureBitmask);
    
  //delete the repository
  delete repository;
  repository = NULL;
    }

//-----------------------------------------------------------------------------
// CHttpFilterAcceptHeader::ConstructL
// Epoc Constructor
//-----------------------------------------------------------------------------
//
void CHttpFilterAcceptHeader::ConstructL()
	{	

    iSession->StringPool().OpenL( HttpFilterCommonStringsExt::GetTable() );

    iDataTypes = new ( ELeave ) CDataTypeArray( KNumDefaultNumOfTypes ); 

    // Add hardcoded data types that the browser supports to the array
    AddBrowserSupportedTypesL(); 

    ReadFeatureBitmaskL();        
    // Try adding external data types from shared data
    ReadMimeTypesL();
    
    FeatureManager::InitializeLibL();
    if (( iFeatureBitmask & KHttpAcceptMultipleAcceptLanguagesSupport)) 
	{ 	// Compose Multiple Languages String
		ComposeMultipleLanguagesStringL();
	}

    iAcceptStr = iSession->StringPool().StringF( 
                       HTTP::EAccept, 
                       RHTTPSession::GetTable() );

    iLoadTypeStr = iSession->StringPool().StringF(
                       HttpFilterCommonStringsExt::ELoadType,
                       HttpFilterCommonStringsExt::GetTable() );

    iDone = iSession->StringPool().StringF(
                       HttpFilterCommonStringsExt::EAcceptHeadersDone,
                       HttpFilterCommonStringsExt::GetTable() );
    
    // Register the filter. we are interested in transaction event
    // ESubmit. 
    iSession->FilterCollection().AddFilterL( *this,
                                           THTTPEvent::ESubmit,
                                           RStringF(),
                                           KAnyStatusCode,
                                           EClientFilters,
                                           iAcceptStr );
	}

//-----------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------
CHttpFilterAcceptHeader::~CHttpFilterAcceptHeader()
	{
    FeatureManager::UnInitializeLib();
	delete iDataTypes;
	}

// Install filter
CHttpFilterAcceptHeader* CHttpFilterAcceptHeader::InstallFilterL( 
    TAny* aSession )
	{
    __ASSERT_DEBUG( aSession != NULL, PanicHttpFilter() );
    RHTTPSession* session = REINTERPRET_CAST( RHTTPSession*, aSession );
    CHttpFilterAcceptHeader* filter =
        new ( ELeave ) CHttpFilterAcceptHeader( session );
    CleanupStack::PushL( filter );
    filter->ConstructL();
    CleanupStack::Pop();
    return filter;
	}

void CHttpFilterAcceptHeader::MHFLoad( RHTTPSession, THTTPFilterHandle )
	{
    ++iLoadCount;
	}

void CHttpFilterAcceptHeader::MHFUnload( 
    RHTTPSession /*aSession*/, 
    THTTPFilterHandle /*aFilterHandler*/ )
    {
    __ASSERT_DEBUG( iLoadCount >= 0, PanicHttpFilter() );
    if ( --iLoadCount )
        {
        return;
        }

    delete this;
	}


void CHttpFilterAcceptHeader::MHFRunL( 
    RHTTPTransaction aTransaction,
    const THTTPEvent& aEvent )
	{
    switch (aEvent.iStatus)
		{
		case THTTPEvent::ESubmit:
			AddHeadersL( aTransaction );
			AddLanguageHeaderL( aTransaction );
			break;
		default:
			break;
		}
	}

TInt CHttpFilterAcceptHeader::MHFRunError(
    TInt aError,
    RHTTPTransaction /*aTransaction*/,
    const THTTPEvent& /*aEvent*/ )
	{
    return aError;
	}
	
void CHttpFilterAcceptHeader::MHFSessionRunL(
    const THTTPSessionEvent& /*aEvent*/ )
    {}

TInt CHttpFilterAcceptHeader::MHFSessionRunError(
    TInt aError,
    const THTTPSessionEvent& /*aEvent*/ )
    {
    return aError;
    }


//-----------------------------------------------------------------------------
// CHttpFilterAcceptHeader::AddBrowserSupportedTypesL
// Add hardcoded MIME types into the array.
//-----------------------------------------------------------------------------
void CHttpFilterAcceptHeader::AddBrowserSupportedTypesL()
    {
    HBufC8* buf8 = HBufC8::NewLC( KBrowserMimeTypeList.iTypeLength);    
    TPtr8 ptr8 = buf8->Des();
    ptr8.Copy(KBrowserMimeTypeList);
    ParseAndStoreMimeListL(ptr8);
    CleanupStack::PopAndDestroy(); // buf8
    }

//-----------------------------------------------------------------------------
// CHttpFilterAcceptHeader::ReadMimeTypesL
// Query the supported MIME types from the shared data and store the result array.
//-----------------------------------------------------------------------------
void CHttpFilterAcceptHeader::ReadMimeTypesL()
    {   
  TInt error = KErrNone;

    //Create a new repository with KSDUidCacheManager
    CRepository* repository = CRepository::NewL(KCRUidBrowser);
    User::LeaveIfNull(repository);

    if (error == KErrNone)
        {
        // Allocate wide buffer for data types
        HBufC16* buf16 = HBufC16::NewLC( KSizeofMimeTypeBuffer );
        TPtr16 ptr16 = buf16->Des();

        // Read the data types from SharedData, increase the buffer
        // if the initial size not enough.
        do 
            {
               // SharedData Key should be included from the variant.hrh when available
            error = repository->Get(KBrowserNGMimeTypes, ptr16);
            if (error == KErrNotFound || (ptr16.Length() < 3))
                {
                CleanupStack::PopAndDestroy(); // buf16
                return;
                }
            if (error == KErrTooBig || error == KErrOverflow)
                {
                buf16->ReAllocL(ptr16.MaxLength() + KSizeofMimeTypeBuffer);
                ptr16.Set(buf16->Des());
                }
            }
        while (error != KErrNone);

        delete repository;
        repository = NULL;
        
        // Allocate narrow buffer and copy data types there
        HBufC8* buf8 = HBufC8::NewLC( buf16->Length() );
        TPtr8 ptr8 = buf8->Des();
        ptr8.Copy(ptr16);

        // Clean up the list and store it to array
        ParseAndStoreMimeListL( ptr8 );

        CleanupStack::PopAndDestroy(2); // buf16, buf8
        } //if
    }

//-----------------------------------------------------------------------------
// CHttpFilterAcceptHeader::AddToArrayL
// Store individual result to an array after some sanity checking
//-----------------------------------------------------------------------------
void CHttpFilterAcceptHeader::AddToArrayL( TPtr8& aPtr )
    {
    aPtr.Trim();
    if ( aPtr.Length() > 3)
        {
        iDataTypes->AppendL(TDataType(aPtr));
        }
    }

//-----------------------------------------------------------------------------
// CHttpFilterAcceptHeader::ParseAndStoreMimeListL
// Parse comma separate list of data types and store them.
//-----------------------------------------------------------------------------
void CHttpFilterAcceptHeader::ParseAndStoreMimeListL( TPtr8& aPtr )
    {
    TInt left = 0;
    TInt right = 0;
    
    if (aPtr.Length() == 0)
        return;
    // Start from left to right until no commas found
    do
        {
        aPtr = aPtr.Mid(left, aPtr.Length() - left);
        right = aPtr.LocateF(TChar(','));
        if (right > 0 )
            {
            HBufC8* mimestr = HBufC8::NewLC(right);
            TPtr8 ptr = mimestr->Des();
            ptr = aPtr.Left(right);
            AddToArrayL(ptr);             
            CleanupStack::PopAndDestroy(); // mimestr
            }
        left = right+1;
        }
    while ( right >= 0 ); 

    // Handle remaining or the only part
    HBufC8* mimestr = HBufC8::NewLC(aPtr.Length());
    TPtr8 ptr = mimestr->Des();
    ptr.Copy(aPtr);
    AddToArrayL(ptr);
    CleanupStack::PopAndDestroy(); // mimestr
    }

//-----------------------------------------------------------------------------
// CHttpFilterAcceptHeader::AddAllHeadersL
// Add all MIME types from the array to the headers
//-----------------------------------------------------------------------------
void CHttpFilterAcceptHeader::AddAllHeadersL(
    RHTTPHeaders& aHeaders,
    RStringF aAcceptStr )
    {
   TDataType type;
    for ( TInt i= 0; i < iDataTypes->Count() ; i++ )
        {
        type = iDataTypes->At(i);
        SetHeaderL( aHeaders, aAcceptStr, type.Des8() );
        }
    TPtrC8 strStarSlashStar(KStarSlashStar);
//    SetHeaderL( aHeaders, aAcceptStr, TPtrC8( KJavaScript));
//    SetHeaderL( aHeaders, aAcceptStr, TPtrC8( KAppJavaScript));
//    SetHeaderL( aHeaders, aAcceptStr, TPtrC8( KECMAScript) );    
    SetHeaderL( aHeaders, aAcceptStr, strStarSlashStar );
    }

//-----------------------------------------------------------------------------
// CHttpFilterAcceptHeader::AddNpiHeadersL
// We just add */* because the response goes directly to the plugin 
//-----------------------------------------------------------------------------
void CHttpFilterAcceptHeader::AddNpiHeadersL(
    RHTTPHeaders& aHeaders,
    RStringF aAcceptStr )
    {    
    TPtrC8 strNpi(KStarSlashStar);
    SetHeaderL( aHeaders, aAcceptStr, strNpi ); 
    }


//-----------------------------------------------------------------------------
// CHttpFilterAcceptHeader::AddHeaders
// Decide which subset of the MIME types we should add to the headers.
//-----------------------------------------------------------------------------
void CHttpFilterAcceptHeader::AddHeadersL(RHTTPTransaction aTransaction)
	{
	RHTTPTransactionPropertySet properties = aTransaction.PropertySet();

    THTTPHdrVal hdrVal;

    if ( properties.Property( iDone, hdrVal ) )
        {
        return;        
        }
    else
        {
        AllHeadersSetL( properties );
        }
        

	if (iDataTypes->Count() > 0)
		{
   		RHTTPHeaders requestHeaders( 
			aTransaction.Request().GetHeaderCollection() );

        if ( !properties.Property( iLoadTypeStr, hdrVal ) )
            {
            AddAllHeadersL( requestHeaders, iAcceptStr );
            return;
            }

        if ( hdrVal.Type() == THTTPHdrVal::KTIntVal )
            {
            switch ( hdrVal.Int() )
                {
                case ( EAcceptNone ) :
                    break;
                case ( EAcceptImages ) :
                    AddImageHeadersL( requestHeaders, iAcceptStr );
                    break;
                case ( EAcceptCss ) :
                    AddCssHeadersL( requestHeaders, iAcceptStr );
                    break;
                case ( EAcceptJavaScript) :
                    AddJavaScriptHeadersL( requestHeaders, iAcceptStr );
                    break;
                case EAcceptAll:
                default:
                    AddAllHeadersL( requestHeaders, iAcceptStr );
                    break;
		        }
            }
        else
            {
            AddAllHeadersL( requestHeaders, iAcceptStr );
            }
        }
	}



//-----------------------------------------------------------------------------
// CHttpFilterAcceptHeader::AllHeadersSetL
// We have processed this transaction already once. In case of redirects
// we don't have to do it again. Set the value of "ahd" to one. Next time we
// will check if the properity is already there.
//-----------------------------------------------------------------------------
void CHttpFilterAcceptHeader::AllHeadersSetL(RHTTPTransactionPropertySet& aProperties)
    {
    aProperties.SetPropertyL( iDone, THTTPHdrVal(1) );
    }

//-----------------------------------------------------------------------------
// CHttpFilterAcceptHeader::AddImageHeadersL
// We just add */* so that we can concatenate as many image requests 
// possible.
//-----------------------------------------------------------------------------
void CHttpFilterAcceptHeader::AddImageHeadersL(
    RHTTPHeaders& aHeaders,
    RStringF aAcceptStr )
    {    
    TPtrC8 strImages(KStarSlashStar);
    SetHeaderL( aHeaders, aAcceptStr, strImages ); 
    }

//-----------------------------------------------------------------------------
// CHttpFilterAcceptHeader::AddCssHeadersL
// Add text/css to the headers.
//-----------------------------------------------------------------------------
void CHttpFilterAcceptHeader::AddCssHeadersL(
    RHTTPHeaders& aHeaders,
    RStringF aAcceptStr )
    {
    TPtrC8 strCss( KCssStr );
    SetHeaderL( aHeaders, aAcceptStr, strCss );
    }

//-----------------------------------------------------------------------------
// CHttpFilterAcceptHeader::AddJavaScriptHeadersL
// Add "application/x-javascript and text/ecmascript"
//-----------------------------------------------------------------------------
void CHttpFilterAcceptHeader::AddJavaScriptHeadersL(
    RHTTPHeaders& aHeaders,
    RStringF aAcceptStr )
    {
      SetHeaderL( aHeaders, aAcceptStr, TPtrC8( KJavaScript));
      SetHeaderL( aHeaders, aAcceptStr, TPtrC8( KAppJavaScript));
      SetHeaderL( aHeaders, aAcceptStr, TPtrC8( KECMAScript) );    
    }

// -----------------------------------------------------------------------------
// CHttpFilterAcceptHeader::SetHeaderL()
// -----------------------------------------------------------------------------
void CHttpFilterAcceptHeader::SetHeaderL(
    RHTTPHeaders aHeaders,
    const RStringF aKey,
    const TPtrC8& aValue )
    {
    RStringF headerValue = iSession->StringPool().OpenFStringL( aValue ); 
    THTTPHdrVal val( headerValue );
    aHeaders.SetFieldL( aKey, val );
    headerValue.Close();  // close headerValue
    }

// -----------------------------------------------------------------------------
// CHttpFilterAcceptHeader::AddLanguageHeaderL()
// -----------------------------------------------------------------------------
void CHttpFilterAcceptHeader::AddLanguageHeaderL(RHTTPTransaction aTransaction)
    {
	RHTTPHeaders requestHeaders( aTransaction.Request().GetHeaderCollection() );
	RStringPool aStringPool = iSession->StringPool();

    if (( iFeatureBitmask & KHttpAcceptMultipleAcceptLanguagesSupport)) 
		{	// If KHttpAcceptMultipleAcceptLanguagesSupport enabled
			// need to put user selected Language and other installed language in the format as below:
			// en;q=1.0, es;q=0.5, fr;q=0.5

			// Get User current selected language
			TLanguage aCurrentUserSelectedLang = User::Language();
			if ( aCurrentUserSelectedLang != iUserSelectedLang )
			{   // If User current selected language is not User selected language at contructor time
				// Compose Multiple Languages String again
				ComposeMultipleLanguagesStringL();
			}
		
			RStringF acceptLangStr = aStringPool.StringF(HTTP::EAcceptLanguage, RHTTPSession::GetTable());
 
			// First check if EAcceptLanguage field is already added
			const TInt parts = requestHeaders.FieldPartsL( acceptLangStr );
			if( !parts ) 
			{
			requestHeaders.SetRawFieldL( acceptLangStr,	iInstalledLangBuf, KFieldSeparator );
			} // else: If EAcceptLanguage field is is already added, do nothing 
		}
	else
		{	//If KHttpAcceptMultipleAcceptLanguagesSupport not enabled, only put "en"
			requestHeaders.SetFieldL(aStringPool.StringF(HTTP::EAcceptLanguage,
					  RHTTPSession::GetTable()),
					  HttpFilterCommonStringsExt::GetLocaleLanguage(aStringPool));
		}	
    }

// -----------------------------------------------------------------------------
// CHttpFilterAcceptHeader::ComposeMultipleLanguagesStringL()
// -----------------------------------------------------------------------------
void CHttpFilterAcceptHeader::ComposeMultipleLanguagesStringL()
    {
	RStringPool aStringPool = iSession->StringPool();

	// Declare a language arrays
	CArrayFixFlat<TInt>* systemEpocLanguageCodes = 0;
	// Get installed language codes
	// coverity [alloc_fn]
	// coverity [freed_arg]
	User::LeaveIfError( SysLangUtil::GetInstalledLanguages(systemEpocLanguageCodes) );
		
	// coverity [double_free] 
	CleanupStack::PushL( systemEpocLanguageCodes ); 

	// Compose the String such as "en;q=1.0, es;q=0.5, fr;q=0.5"
	// Get user selected language
	iUserSelectedLang = User::Language();
	// Get user selected language string
	iUserSelectedLangString = HttpFilterCommonStringsExt::GetLocaleLanguage(aStringPool);
	// Compose the Installed Languages Buf
	iInstalledLangBuf.Copy(iUserSelectedLangString.DesC());

	if ( systemEpocLanguageCodes->Count() > 1)
		{	// If we have more than one one language installed, do...
			iInstalledLangBuf.Append(KQWholeValue);  // _LIT8(KQWholeValue, ";q=1.0"); 

			// Append the other installed languages
			for ( TInt i = 0; i < systemEpocLanguageCodes->Count(); ++i )
			{
				TLanguage language = (TLanguage) systemEpocLanguageCodes->At( i );
				{
					if ( language != iUserSelectedLang)
					{
						RStringF extraLang = HttpFilterCommonStringsExt::GetLanguageString(aStringPool, language);
						iInstalledLangBuf.Append(KCommaSeperator);  // _LIT8(KCommaSeperator, ", "); 
						iInstalledLangBuf.Append(extraLang.DesC());
						iInstalledLangBuf.Append(KQHalfValue);  // _LIT8(KQHalfValue, ";q=0.5"); 
 					}
				}
			}
		} // else: Don't append q value if we only have one language installed
		
	// clean up 
	CleanupStack::PopAndDestroy( 1 );   // systemEpocLanguageCodes
    }

//  End of File


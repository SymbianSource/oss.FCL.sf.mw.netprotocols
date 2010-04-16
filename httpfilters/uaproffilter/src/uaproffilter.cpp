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
* Description:  Implementation of class CHttpUAProfFilter.
*
*
*/


// INCLUDE FILES

#include "uaproffilter.h"

#include <e32std.h>
#include <etelmm.h>
#include <mmtsy_names.h>

#include <http/rhttptransaction.h>
#include <http/rhttpconnectioninfo.h>
#include <http/rhttpheaders.h>
#include <httperr.h>

#include <BrowserUiSDKCRKeys.h>
#include <WebUtilsInternalCRKeys.h>//for the profile strings

#include <CUserAgent.h>
#include <featmgr.h>
#include <bldvariant.hrh>


// Token for IMEI/SN code in UA-header.
_LIT8( KImeiSnToken, "SN" );
_LIT8( KSlash, "/" );

// Space character.
LOCAL_D const TUint8 KSpaceChar = ' ';

// MACROS

/// OOM test helper.
//#define __TEST_OOM_IN_INSTALL
/// OOM test helper.
//#define __TEST_OOM_IN_MHFRUNL

// CONSTANTS

/// Initial buffer size for reading from Shared Data.
LOCAL_D const TInt KSdReadBufSize = 128;
/// profile
_LIT8( KProfile, "profile" );
/// x-wap-profile
_LIT8( KXWapProfile, "x-wap-profile" );
#if defined( _DEBUG )
/// Panic name for UAProf Filter.
_LIT( KUAProfFilter, "UAProfFilter" );
#endif
/// Length for imei code.
LOCAL_D const TInt KImeiLength = 15;


#if defined( __WINS__ )
/// Dummy IMEI value for WINS emulator.
_LIT( KEmulatorImei, "123456789012345" );
#endif

// ================= MEMBER FUNCTIONS ==========================================

// -----------------------------------------------------------------------------
// CHttpUAProfFilter::InstallFilterL()
// -----------------------------------------------------------------------------
//
CHttpUAProfFilterInterface* CHttpUAProfFilter::InstallFilterL( TAny* aSession )
    {
#ifdef __TEST_OOM_IN_INSTALL
    __UHEAP_SETFAIL( RHeap::ETrueRandom, 40 );
#endif /* def __TEST_OOM_IN_INSTALL */

    __ASSERT_DEBUG( aSession, User::Panic( KUAProfFilter, KErrArgument ) );
    RHTTPSession* session = REINTERPRET_CAST( RHTTPSession*, aSession );
    CHttpUAProfFilter* filter = new (ELeave) CHttpUAProfFilter( *session );
    CleanupStack::PushL( filter );
    filter->ConstructL();
    CleanupStack::Pop( filter );
    return filter;

#ifdef __TEST_OOM_IN_INSTALL
    __UHEAP_SETFAIL( RHeap::ENone, 1 );
#endif /* def __TEST_OOM_IN_INSTALL */
    }

// -----------------------------------------------------------------------------
// CHttpUAProfFilter::~CHttpUAProfFilter()
// -----------------------------------------------------------------------------
//
CHttpUAProfFilter::~CHttpUAProfFilter()
    {
    // If we've been destroyed from the cleanup stack during creation
    // of the object, it might still be loaded. So check. (Normally the
    // delete is initiated by the 'delete this' in MHFUnload)
    if( iLoadCount > 0)
        {
        // As we're already in a destructor, MHFUnload must not delete us
        // again. So -1 value is set.
        iLoadCount = -1;
        iSession.FilterCollection().RemoveFilter( StringF( HTTP::EUserAgent ) );
        }
    // Make sure we don't close an unopened RStringF
    if( iUaProf3GOpen )
        {
        iUaProf3G.Close();
        }
    if( iUaProfOpen )
        {
        iUaProf.Close();
        }
    if( iUaNameOpen )
        {
        iUaName.Close();
        }
    if ( iUaNameWithImeiOpen )
        {
        iUaNameWithImei.Close();
        }
    if( iUaNameMmsOpen )
        {
        iUaNameMms.Close();
        }
    if ( iUaNameMmsWithImeiOpen )
        {
        iUaNameMmsWithImei.Close();
        }
    iMobilePhone.Close();
    iTelServer.Close();

    if ( iNotifyHandler )
    	{
    	iNotifyHandler->StopListening();	
    	}
    delete iRepositoryBrowser;
    delete iRepositoryCommonEngine;
    delete iNotifyHandler;
    }

// -----------------------------------------------------------------------------
// CHttpUAProfFilter::MHFRunL()
// -----------------------------------------------------------------------------
//
void CHttpUAProfFilter::MHFRunL
( RHTTPTransaction aTransaction, const THTTPEvent& aEvent )
    {
#ifdef __TEST_OOM_IN_MHFRUNL
    __UHEAP_SETFAIL( RHeap::ETrueRandom, 30 );
#endif /* def __TEST_OOM_IN_MHFRUNL */

    switch( aEvent.iStatus )
        {
        case THTTPEvent::ESubmit:
            {
            SubmitL( aTransaction );
            break;
            }

        default:
            {
            break;
            }
        }

#ifdef __TEST_OOM_IN_MHFRUNL
    __UHEAP_SETFAIL( RHeap::ENone, 1 );
#endif /* def __TEST_OOM_IN_MHFRUNL */
    }


// -----------------------------------------------------------------------------
// CHttpUAProfFilter::MHFRunError()
// -----------------------------------------------------------------------------
//
TInt CHttpUAProfFilter::MHFRunError
( TInt /*aError*/, RHTTPTransaction aTransaction, const THTTPEvent& /*aEvent*/ )
    {
    aTransaction.Fail();
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CHttpUAProfFilter::MHFSessionRunL()
// -----------------------------------------------------------------------------
//
void CHttpUAProfFilter::MHFSessionRunL( const THTTPSessionEvent& /*aEvent*/ )
    {
    }

// -----------------------------------------------------------------------------
// CHttpUAProfFilter::MHFSessionRunError()
// -----------------------------------------------------------------------------
//
TInt CHttpUAProfFilter::MHFSessionRunError
( TInt aError, const THTTPSessionEvent& /*aEvent*/ )
    {
    return aError;
    }

// -----------------------------------------------------------------------------
// CHttpUAProfFilter::MHFUnload()
// -----------------------------------------------------------------------------
//
void CHttpUAProfFilter::MHFUnload( RHTTPSession /*aSession*/, 
                                   THTTPFilterHandle /*aHandle*/ )
    {
    // We must be only registered on one session, as we register in our
    // ConstructL and no-one else has a pointer to us. Therefore, we
    // know we can be deleted at this point.
    if( --iLoadCount == 0 )
        {
        delete this;
        }
    }

// -----------------------------------------------------------------------------
// CHttpUAProfFilter::MHFLoad()
// -----------------------------------------------------------------------------
//
void CHttpUAProfFilter::MHFLoad
( RHTTPSession /*aSession*/, THTTPFilterHandle /*aHandle*/ )
    {
    ++iLoadCount;
    }

// -----------------------------------------------------------------------------
// CHttpUAProfFilter::HandleNotifyInt()
// -----------------------------------------------------------------------------
//
void CHttpUAProfFilter::HandleNotifyInt(TUint32 aId, TInt aValue)
    {
    if ( aId == KBrowserIMEINotification )
        {
        TInt imeiSendingOn = aValue;
        iImeiSendingOn = (imeiSendingOn != 0);
        }
    #ifdef _DEBUG
        else
            {
            // This notification was not requested.
            User::Panic( KUAProfFilter, KErrGeneral );
            }
    #endif
    }

// -----------------------------------------------------------------------------
// CHttpUAProfFilter::CHttpUAProfFilter()
// -----------------------------------------------------------------------------
//
CHttpUAProfFilter::CHttpUAProfFilter( RHTTPSession aSession )
: iUaProfOpen( EFalse ),
  iUaProf3GOpen( EFalse ),
  iUaNameOpen( EFalse ),
  iUaNameWithImeiOpen( EFalse ),
  iUaNameMmsOpen( EFalse ),
  iUaNameMmsWithImeiOpen( EFalse ),
  iSession( aSession ),
  iStringPool( aSession.StringPool() ),
  iStringTable( RHTTPSession::GetTable() ),
  iImeiFeatureEnabled( EFalse ),
  iImeiSendingOn( EFalse )
    { 
    }

// -----------------------------------------------------------------------------
// CHttpUAProfFilter::ConstructL()
// -----------------------------------------------------------------------------
//
void CHttpUAProfFilter::ConstructL()
    {

    //iRepositoryCommonEngine object will be created in OpenProfileString()
    //iRepositoryBrowser object will be SetupImeiNotifyL()

    // This filter is opened only on one session, so it is safe to pre-open
    // the strings we are going to use.

    // UAProf string.
    OpenProfileStringsL();  // Uses iSdCli.

    // If 2G/3G distinction is supported, prepare querying the network type.
    // If no such distinction is made, we don't need RMobilePhone at all.
    if ( iUaProf3GOpen )
        {
        User::LeaveIfError( iTelServer.Connect() );
        User::LeaveIfError( iTelServer.LoadPhoneModule( KMmTsyModuleName ) );
        TInt numPhones;
        User::LeaveIfError( iTelServer.EnumeratePhones( numPhones ) );
        if( numPhones <= 0 )
            {
            // Huh???
            User::Leave( KErrCorrupt );
            }
        RTelServer::TPhoneInfo phoneInfo;
        User::LeaveIfError( iTelServer.GetPhoneInfo( 0, phoneInfo ) );
        User::LeaveIfError( iMobilePhone.Open( iTelServer, phoneInfo.iName ) );
        User::LeaveIfError( iMobilePhone.Initialise() );
        }

    // Setup IMEI-notify stuff, ignore any errors.
    // Expected error: Browser's INI file may not be present.
    TRAP_IGNORE( SetupImeiNotifyL() );

    // User Agent name string.
    OpenUserAgentNameStringsL();

    // Register filter.
    iSession.FilterCollection().AddFilterL
        (
        *this,
        THTTPEvent( THTTPEvent::ESubmit ),  // submit event
        MHTTPFilter::ETidyUp,               // priority
        StringF( HTTP::EUserAgent )         // name
        );
    }


// -----------------------------------------------------------------------------
// CHttpUAProfFilter::SetupImeiNotifyL()
// -----------------------------------------------------------------------------
//
void CHttpUAProfFilter::SetupImeiNotifyL()
    {
    // First check if this feature is enabled.
    FeatureManager::InitializeLibL();   // Warning: no leaving...
    iImeiFeatureEnabled = 
        FeatureManager::FeatureSupported( KFeatureIdBrowserIMEINotification );
    FeatureManager::UnInitializeLib();  // ... until this point!

    // If feature is enabled, query value and register for notification.
    if ( iImeiFeatureEnabled )
        {
        TInt imeiSendingOn( 0 );

        CRepository* iRepositoryBrowser = CRepository::NewL( KCRUidBrowser );
        iRepositoryBrowser->Get(  KBrowserIMEINotification, imeiSendingOn );
        iImeiSendingOn = (imeiSendingOn != 0);
        iNotifyHandler = CCenRepNotifyHandler::NewL(*this, 
                                                 *iRepositoryBrowser, 
                                                 CCenRepNotifyHandler::EIntKey, 
                                                 KBrowserIMEINotification);
        iNotifyHandler->StartListeningL();
        }

    }

// -----------------------------------------------------------------------------
// CHttpUAProfFilter::SubmitL()
// -----------------------------------------------------------------------------
//
void CHttpUAProfFilter::SubmitL( RHTTPTransaction aTransaction )
    {
    RHTTPHeaders headers( aTransaction.Request().GetHeaderCollection() );
    
    // User-Agent: protocol independent header.

    // First check if client is MMS.
    TBool uaMms( EFalse );
    RStringF uaClient = iStringPool.OpenFStringL( KUserAgentClientId );
    CleanupClosePushL<RStringF>( uaClient );
    THTTPHdrVal val;
    if ( aTransaction.PropertySet().Property( uaClient, val ) &&
         val.Type() == THTTPHdrVal::KTIntVal &&
         val.Int() == KUserAgentCliMMS )
        {
        uaMms = ETrue;
        }
    CleanupStack::PopAndDestroy();  // Close uaClient.

    // If IMEI-notify is on (implies that feature is supported), use that.
    RStringF hdrVal;
    if ( uaMms )
        {
        hdrVal = iImeiSendingOn ? iUaNameMmsWithImei : iUaNameMms;
        }
    else
        {
        hdrVal = iImeiSendingOn ? iUaNameWithImei : iUaName;
        }
    AddOrReplaceHeaderL( headers, StringF( HTTP::EUserAgent ), hdrVal );

    // UAProf: protocol dependent.
    RStringF xProfile;
    THTTPHdrVal protocol;
    TBool protocolExists = iSession.ConnectionInfo().Property
        ( StringF( HTTP::EProtocol ), protocol );
    if( protocolExists &&
        protocol.Type() == THTTPHdrVal::KStrFVal &&
        protocol.StrF() == StringF( HTTP::EWSP ) )
        {
        // It's WSP - set "Profile".
        xProfile = iStringPool.OpenFStringL( KProfile );
        }
    else
        {
        // It's HTTP - set "x-wap-profile".
        xProfile = iStringPool.OpenFStringL( KXWapProfile );
        }
    CleanupClosePushL<RStringF>( xProfile );
    // Use 3G profile if:
    // - we have such (i.e. feature is supported); and
    // - network is 3G.
    hdrVal = (iUaProf3GOpen && Is3gNetworkL()) ? iUaProf3G : iUaProf;
    AddOrReplaceHeaderL( headers, xProfile, hdrVal );
    CleanupStack::PopAndDestroy();  // close xProfile
    }

// -----------------------------------------------------------------------------
// CHttpUAProfFilter::AddOrReplaceHeaderL()
// -----------------------------------------------------------------------------
//
void CHttpUAProfFilter::AddOrReplaceHeaderL
( RHTTPHeaders& aHeaders, const RStringF& aName, const RStringF& aValue )
    {
    // Checking whether the header field is already set or not would be waste:
    // that is executed inside RemoveField anyway.
    (void)aHeaders.RemoveField( aName );
    aHeaders.SetFieldL( aName, THTTPHdrVal( aValue ) );
    }

// -----------------------------------------------------------------------------
// CHttpUAProfFilter::OpenUserAgentNameStringsL()
// -----------------------------------------------------------------------------
//
void CHttpUAProfFilter::OpenUserAgentNameStringsL()
    {
    CUserAgent* usrAgnt = CUserAgent::NewL();
    CleanupStack::PushL( usrAgnt );//C.S: 1

    HBufC8* bufUA = usrAgnt->UserAgentL();
    CleanupStack::PushL( bufUA );//C.S 2
    if ( !bufUA || !bufUA->Length() )
        {
        // Reject empty value.
        //User::Leave( KErrCorrupt );
        User::Panic(_L("Improper UA string"), KErrCorrupt);
        }
    iUaName = iStringPool.OpenFStringL( *bufUA );
    iUaNameOpen = ETrue;
    
    //Read the MMS useragent string now
    HBufC8* bufUAMMS = usrAgnt->MMSUserAgentL();
    CleanupStack::PushL( bufUAMMS );//C.S 2
    if ( !bufUAMMS || !bufUAMMS->Length() )
        {
        // Reject empty value.
        //User::Leave( KErrCorrupt );
        User::Panic(_L("Improper UA string"), KErrCorrupt);
        }
    iUaNameMms = iStringPool.OpenFStringL( *bufUAMMS );
    iUaNameMmsOpen = ETrue;
    
    if ( iImeiFeatureEnabled )
        {
        // User Agent name string with IMEI code.
        TBuf<KImeiLength> imei16;
        GetImeiL( imei16 );
        HBufC8* imei8 = ConvertL( imei16 );
        CleanupStack::PushL( imei8 );//C.S 2

        // Add IMEI string to the UA-header.
        TPtr8 ptrUA = bufUA->Des();
        AddImeiToUserAgentL(ptrUA, *imei8);
        iUaNameWithImei =  iStringPool.OpenFStringL( *bufUA );
        iUaNameWithImeiOpen = ETrue;

        // Add IMEI string to the UA-MMS-header.
        TPtr8 ptrUAMMS = bufUAMMS->Des();
        AddImeiToUserAgentL(ptrUAMMS, *imei8);
        iUaNameMmsWithImei =  iStringPool.OpenFStringL( *bufUAMMS );
        iUaNameMmsWithImeiOpen = ETrue;
        
        CleanupStack::PopAndDestroy( imei8 );//C.S 2
        }
        
        
	CleanupStack::PopAndDestroy( bufUAMMS );
	CleanupStack::PopAndDestroy( bufUA );
    CleanupStack::PopAndDestroy( usrAgnt );//C.S 1 => OK
    }

void CHttpUAProfFilter::AddImeiToUserAgentL(TPtr8 &aUAStringPtr, const TDesC8& aImei)
    {

    TInt firstSpaceIndex = aUAStringPtr.Locate( TChar( KSpaceChar ) );

    if ( firstSpaceIndex != KErrNotFound )
        {
        // Insert IMEI before first space, separated by '/'
        aUAStringPtr.Insert( firstSpaceIndex++, KSlash);
        aUAStringPtr.Insert( firstSpaceIndex, KImeiSnToken);
        firstSpaceIndex += KImeiSnToken.iTypeLength;
        aUAStringPtr.Insert( firstSpaceIndex, aImei );
        }
    else  
        {
        // If there is no spaces, add IMEI to end of ENameMMS, separated by '/'
        aUAStringPtr.Append( KSlash );
        aUAStringPtr.Append( KImeiSnToken );
        aUAStringPtr.Append( aImei );
        }

    }

// -----------------------------------------------------------------------------
// CHttpUAProfFilter::OpenProfileStringsL()
// -----------------------------------------------------------------------------
//
void CHttpUAProfFilter::OpenProfileStringsL()
    {
    // UAProf string.
	TRAPD(err1,iRepositoryCommonEngine = CRepository::NewL( KCRUidWebUtils ));
	
	if(KErrNone != err1)
		User::Panic( _L("CHttpUAProfFilter"), err1 );

    HBufC* buf16;

    User::LeaveIfError( ReadSdString( KWebUtilsUaProf , buf16 ) );

    CleanupStack::PushL( buf16 );
    if ( !buf16 || !buf16->Length() )
        {
        // Reject empty value.
        User::Leave( KErrCorrupt );
        }
    HBufC8* buf8 = ConvertL( *buf16 );
    CleanupStack::PushL( buf8 );    
    iUaProf = iStringPool.OpenFStringL( *buf8 );
    iUaProfOpen = ETrue;
    CleanupStack::PopAndDestroy( 2, buf16 );    // buf8, buf16

    // UAProf3G string. If missing/empty, not opened - this indicates that
    // the 2G/3G distinction feature is not supported.
    TInt err = ReadSdString( KWebUtilsUaProf3G, buf16 );

    CleanupStack::PushL( buf16 );
    if ( err && err != KErrNotFound )
        {
        // Error other than KErrNotFound, leave. KErrNotFound is OK, indicates
        // that this key is missing -> feature is not supported.
        User::Leave( err );
        }
    if ( buf16 && buf16->Length() )
        {
        buf8 = ConvertL( *buf16 );
        CleanupStack::PushL( buf8 );
        iUaProf3G = iStringPool.OpenFStringL( *buf8 );
        iUaProf3GOpen = ETrue;  // Also indicates that feature is supported.
        CleanupStack::PopAndDestroy( buf8 );
        }

    CleanupStack::PopAndDestroy( buf16 );
    }

// -----------------------------------------------------------------------------
// CHttpUAProfFilter::ConvertL()
// -----------------------------------------------------------------------------
//
HBufC8* CHttpUAProfFilter::ConvertL( const TDesC16& aSrc )
    {
    HBufC8* buf = HBufC8::NewL( aSrc.Length() );
    buf->Des().Copy( aSrc );
    return buf;
    }

// -----------------------------------------------------------------------------
// CHttpUAProfFilter::ReadSdString()
// -----------------------------------------------------------------------------
//
TInt CHttpUAProfFilter::ReadSdString( const TUint32 aKey, HBufC*& aBuf )
    {
    // We don't know the size of the string we are going to read (and Shared
    // data can't tell it). A trial-and-error method is used: we start with
    // a fixed size buffer and retry with increasing buffer size until success.
    TInt err( KErrTooBig );
    TInt round = 0;
    HBufC* buf = NULL;

    while( err == KErrTooBig )
        {
        // Increase buffer size until returned data can fit.
        delete buf;
        ++round;
        buf = HBufC::New( KSdReadBufSize * round );
        if( buf )
            {
            TPtr bufP( buf->Des() );
                err = iRepositoryCommonEngine->Get( aKey, bufP );
            }
        else
            {
            err = KErrNoMemory;
            }
        }

    if( err )
        {
        delete buf;
        buf = NULL; // Make lint happy.
        }

    aBuf = buf;
    return err;
    }

// -----------------------------------------------------------------------------
// CHttpUAProfFilter::GetImeiL()
// -----------------------------------------------------------------------------
//
void CHttpUAProfFilter::GetImeiL( TDes& aBuf ) const
    {
    aBuf.Zero();  

    // Read IMEI on HW, use dummy under WINS
#if !defined( __WINS__ )
	TUint32 identityCaps;
	User::LeaveIfError(iMobilePhone.GetIdentityCaps(identityCaps));
	TBuf<RMobilePhone::KPhoneSerialNumberSize> serNumber;

if (identityCaps & RMobilePhone::KCapsGetSerialNumber)
    {
    TRequestStatus status;
    RMobilePhone::TMobilePhoneIdentityV1 mobilePhoneIdentity;
    iMobilePhone.GetPhoneId(status, mobilePhoneIdentity);
    User::WaitForRequest(status);
    User::LeaveIfError(status.Int());
    serNumber = mobilePhoneIdentity.iSerialNumber;
	aBuf.Copy(serNumber);
    }
#else
    aBuf.Copy( KEmulatorImei);
#endif
}

// -----------------------------------------------------------------------------
// CHttpUAProfFilter::Is3gNetworkL()
// -----------------------------------------------------------------------------
//
TBool CHttpUAProfFilter::Is3gNetworkL() const
    {
    // This method must not be called if 2G/3G distinction is not supported;
    // the RMobilePhone-related classes are not open in that case.
    __ASSERT_DEBUG( iUaProf3GOpen, \
        User::Panic( KUAProfFilter, KErrNotReady ) );
    TBool is3g( EFalse );
#if !defined( __WINS__ )
    RMobilePhone::TMobilePhoneNetworkMode networkMode;
    User::LeaveIfError( iMobilePhone.GetCurrentMode( networkMode ) );
    if( networkMode == RMobilePhone::ENetworkModeCdma2000 ||
        networkMode == RMobilePhone::ENetworkModeWcdma )
        {
        is3g = ETrue;
        }
#endif
    return is3g;
    }

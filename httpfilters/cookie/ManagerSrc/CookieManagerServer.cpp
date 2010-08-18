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
* Description:  Implementation of CCookieManagerServer.
*
*/


// INCLUDE FILES
// System includes

#include <e32std.h>

#include <sysutil.h>
#include <bautils.h>
#include <xml/dom/xmlengdomimplementation.h>
#include <xml/dom/xmlengdocument.h>
#include <xml/dom/xmlengdomparser.h>
#include <xml/dom/xmlengattr.h>
#include <xml/dom/xmlengnodelist.h>
#include <xml/dom/xmlengelement.h>


// #include <thttphdrval.h>

// User includes
#include "cookie.h"
#include "CookieArray.h"
#include "cookieipc.h"
#include "CookieLogger.h"
#include "CookieManagerServer.h"
#include "CookieManagerSession.h"
#include "CookieServerDef.h"
#include "CookieGroupDataArray.h"
#include "GroupIdInfoArray.h"
#include "CookieGroupData.h"

// CONSTANTS

#if defined(__WINS__)

// the server closes after the last session closes
const TInt KCookieCloseTime = 5000000;		// 5 seconds
const TInt KCookieInitCloseTime = 60000000;	// 1 minute

#else

// the server closes after the last session closes
const TInt KCookieCloseTime = 60000000;		// 1 minute
const TInt KCookieInitCloseTime = 60000000; // 1 minute

#endif

// This means 128 characters, considering Unicode
//const TInt KCookieMaxFileNameLength = 256;

// Maximum file length
//const TInt KCookieMaxFileLength = 204800;	// 200 kilobytes


// Literals
_LIT( KDefaultCookieFolder, "C:\\Private\\" );
_LIT( KDefaultCookieXmlFolder, "Z:\\Private\\" );
_LIT( KDefaultCookieFile,   "\\Cookies" );
_LIT( KDefaultExtension, ".dat");
_LIT( KUnderScore, "_");
_LIT8(KHexDel,"0x");
_LIT( KDefaultCookieGroupFile,   "\\CookieGroup.xml" );
_LIT8(KGroupId,"id");
_LIT8(KUid3,"uid3");
_LIT8(KCookieSharable,"cookie_apps_share");
_LIT8(KGroupName,"name");
_LIT8(KOff,"off");

// capability checking structures
const TUint cookieServerPolicyRangeCount = 9;

// server messages
const TInt  cookieServerPolicyRanges[ cookieServerPolicyRangeCount ] =
    {
    0,  // EStoreCookie
    1,  // EClearAllCookies
    2,  // EGetCookieSize
    3,  // EGetCookies
    4,  // ESetAppUid
    5,   //EDestroyCookies
    6,  //EGetCookieSharableFlag
    7,  //EClearAppUidCookies
    8
    };

// connection between messages and events
const TUint8 cookieServerPolicyElementsIndex[ cookieServerPolicyRangeCount ] =
    {
    0,  // EStoreCookie
    0,  // EClearAllCookies
    1,  // EGetCookieSize
    1,  // EGetCookies
    0,  // ESetAppUid
    0,  // EDestroyCookies
    0,  //EGetCookieSharableFlag
    0,  //EClearAppUidCookies
   	CPolicyServer::ENotSupported 	// applies all out of range requests
    };

// policy checking events
const CPolicyServer::TPolicyElement cookieServerPolicyElements[] =
    {
    { _INIT_SECURITY_POLICY_C1(ECapabilityWriteDeviceData), CPolicyServer::EFailClient },
    { _INIT_SECURITY_POLICY_C1(ECapabilityReadDeviceData), CPolicyServer::EFailClient }
    };

// main structure for policy check
const CPolicyServer::TPolicy cookiePolicy =
    {
    CPolicyServer::EAlwaysPass,       // iOnConnect
    cookieServerPolicyRangeCount,     // rangecount
    cookieServerPolicyRanges,         // ranges
    cookieServerPolicyElementsIndex,  // elements index
    cookieServerPolicyElements        // elements
    };

// ---------------------------------------------------------
// CCookieManagerServer::CCookieManagerServer
// ---------------------------------------------------------
//
CCookieManagerServer::CCookieManagerServer( TInt aPriority ) 
    : CPolicyServer( aPriority /* EPriorityStandard */, cookiePolicy ),
iSessionCount( 0 ),
iServerClosing( EFalse )//, iCloseServer( EFalse )
    {
    CLOG(( EServer, 0, _L("") ));
    CLOG(( EServer, 0, _L("*****************") ));
    CLOG(( EServer, 0, _L("CCookieManagerServer::CCookieManagerServer") ));
    }

// ---------------------------------------------------------
// CCookieManagerServer::ConstructL
// ---------------------------------------------------------
//
void CCookieManagerServer::ConstructL()
    {
    CLOG( ( EServer, 0, _L( "-> CCookieManagerServer::ConstructL" ) ) );

    iCloseTimer = CCookieTimer::NewL( /*ETrue*/ );
	iCloseTimer->After( KCookieInitCloseTime );

    iStringPool.OpenL();

    //iCookiePacker = new (ELeave) TCookiePacker( iStringPool );
    iCookieGroupDataArray = CCookieGroupDataArray::NewL();
    iGroupIdArray = CGroupIdArray::NewL();

    RFs iFs;
    if ( iFs.Connect() == KErrNone )  // we could connect to the file server
        {
         TBuf<60> groupfilePath(KNullDesC);
         groupfilePath.Copy( KDefaultCookieXmlFolder );
         groupfilePath.AppendNum( RProcess().SecureId(), EHex );
         groupfilePath.Append( KDefaultCookieGroupFile );
         if ( BaflUtils::FileExists( iFs, groupfilePath ) )
            {
             TRAPD(ret, LoadGroupDataFromFileL(iFs) );
             if( ret != KErrNone )
                 {
                 CLOG( ( EServer, 0, _L( "CCookieManagerServer::ConstructL: Loading Group data Failed" ) ) );
                 //Do Nothing
                 }
            }
         iFs.Close();
        }
	StartL( KCookieServerName );

    CLOG( ( EServer, 0, _L( "<- CCookieManagerServer::ConstructL" ) ) );
    }

// ---------------------------------------------------------
// CCookieManagerServer::NewL
// ---------------------------------------------------------
//
CCookieManagerServer* CCookieManagerServer::NewL()
    {
    CLOG( ( EServer, 0, _L( "-> CCookieManagerServer::NewL" ) ) );

	CCookieManagerServer* self =
				new (ELeave) CCookieManagerServer( EPriorityStandard );

	CleanupStack::PushL( self );

	self->ConstructL();

	CleanupStack::Pop();	// self

    CLOG( ( EServer, 0, _L( "<- CCookieManagerServer::NewL" ) ) );

	return self;
    }

// ---------------------------------------------------------
// CCookieManagerServer::~CCookieManagerServer
// ---------------------------------------------------------
//
CCookieManagerServer::~CCookieManagerServer()
    {
    CLOG( ( EServer, 0, _L( "-> CCookieManagerServer::~CCookieManagerServer" ) ) );
    iServerClosing = ETrue;
    if(iCookieGroupDataArray)
        {
        delete iCookieGroupDataArray;
        iCookieGroupDataArray = NULL;
        }
    delete iGroupIdArray;

    delete iCloseTimer;
    iStringPool.Close();
    CLOG( ( EServer, 0, _L( "<-CCookieManagerServer::~CCookieManagerServer") ) );
    CLOG( ( EServer, 0, _L( "*****************" ) ) );
    }

// ---------------------------------------------------------
// CCookieManagerServer::CheckDiskSpace
// ---------------------------------------------------------
//
TBool CCookieManagerServer::CheckDiskSpace( RFs& aFileSystem,
										   const TDesC& aFileName ) const
	{
	TInt err;

	TParse parse;
	err = parse.SetNoWild( aFileName, NULL, NULL );
	if ( err == KErrNone )
		{
		// This is in the form : drive-letter: (letter + semi-colon)
		TBuf<2> driveBuf( parse.Drive() );
		TCharF driveLetter( driveBuf[0] );
		TCharF driveALetter( 'A' );
		TDriveNumber driveNum = (TDriveNumber)( (TUint)(driveLetter) -
												(TUint)(driveALetter) );

		TBool noSpace = EFalse;
		TRAP( err, noSpace = SysUtil::DiskSpaceBelowCriticalLevelL
					( &aFileSystem, KCookieMaxFileLength, driveNum ) );
		if ( err == KErrNone && noSpace )
			{
			err = KErrDiskFull;
			}
		}

	return ( err == KErrNone ? ETrue : EFalse );
	}

// ---------------------------------------------------------
// CCookieManagerServer::NewSessionL
// ---------------------------------------------------------
//
CSession2* CCookieManagerServer::NewSessionL( const TVersion& /*aVersion*/,
                                              const RMessage2& /*aMessage*/) const
    {
    CLOG( ( EServer, 0, _L( "<-> CCookieManagerServer::NewSessionL" ) ) );

	return ((CCookieManagerServer*)this)->DoNewSessionL();
    }

// ---------------------------------------------------------
// CCookieManagerServer::DoNewSessionL
// ---------------------------------------------------------
//
CSession2* CCookieManagerServer::DoNewSessionL()
    {
    CLOG(( EServer, 0, _L("-> CCookieManagerServer::DoNewSessionL") ));

    CCookieManagerSession* session =
					CCookieManagerSession::NewL( *this );
    iSessionCount++;

	iCloseTimer->Cancel();

    CLOG( ( EServer, 0, _L( " New session created OK" ) ) );
    CLOG( ( EServer, 0, _L( "<- CCookieManagerServer::DoNewSessionL" ) ) );

    return session;
    }

// ---------------------------------------------------------
// CCookieManagerServer::CloseSession
// ---------------------------------------------------------
//
void CCookieManagerServer::CloseSession()
    {
    CLOG( ( EServer, 0, _L( "-> CCookieManagerServer::CloseSession" ) ) );

    
    if ( --iSessionCount == 0 )
        { 
        // no more sessions left so we can close the server.
        // however, it is advantageous to wait a lilltle, 
        // e.g. 1 minute before doing so as starting a server is expensive
        // in many ways.
        CLOG( ( EServer, 0, _L( "Closing Server" ) ) );
        iCloseTimer->After( KCookieCloseTime );
        //just write cookies back to the file when browser is closed,
        //no need wait till cookie server is shutdown.
        //WriteCookiesToFile();
        }

    CLOG( ( EServer, 0, _L( "<- CCookieManagerServer::CloseSession" ) ) );
    }



// ---------------------------------------------------------
// CCookieManagerServer::StringPool
// ---------------------------------------------------------
//
RStringPool* CCookieManagerServer::StringPool()
    {
    return &iStringPool;
    }


// ---------------------------------------------------------
// CCookieManagerServer::CookieArray
// ---------------------------------------------------------
//
CCookieArray* CCookieManagerServer::CookieArray(TInt aIndex)
    {
    return iCookieGroupDataArray->CookieArray(aIndex);
    }


// ---------------------------------------------------------
// CCookieManagerServer::CookieGroupDataArray
// ---------------------------------------------------------
//
CCookieGroupDataArray* CCookieManagerServer::CookieGroupDataArray()
    {
    return iCookieGroupDataArray;
    }

// ---------------------------------------------------------
// CCookieManagerServer::GroupIdArray
// ---------------------------------------------------------
//
CGroupIdArray* CCookieManagerServer::GroupIdArray()
    {
    return iGroupIdArray;
    }

// ---------------------------------------------------------
// CCookieManagerServer::GetCookies
// ---------------------------------------------------------
//
TInt CCookieManagerServer::GetCookies( const TDesC8& aRequestUri,
                                      RPointerArray<CCookie>& aCookies,TInt aIndex ) const
	{
    return iCookieGroupDataArray->GetCookies( aRequestUri, aCookies, aIndex);
	}


//**********************************
// CCookieTimer
//**********************************

// ---------------------------------------------------------
// CCookieTimer::CCookieTimer
// ---------------------------------------------------------
//
CCookieTimer::CCookieTimer() : CTimer( EPriorityLow )
	{}

// ---------------------------------------------------------
// CCookieTimer::~CCookieTimer
// ---------------------------------------------------------
//
CCookieTimer::~CCookieTimer()
	{
	Cancel();
	}

// ---------------------------------------------------------
// CCookieTimer::RunL
// ---------------------------------------------------------
//
void CCookieTimer::RunL()
	{
	CActiveScheduler::Stop();
	}

// ---------------------------------------------------------
// CCookieTimer::NewL
// ---------------------------------------------------------
//
CCookieTimer* CCookieTimer::NewL()
	{
	CCookieTimer* self = new (ELeave) CCookieTimer;

	CleanupStack::PushL( self );

	self->ConstructL(); // CTimer

	CActiveScheduler::Add( self );

	CleanupStack::Pop();	// self

	return self;
	}
// ---------------------------------------------------------
// CCookieTimer::LoadGroupDataFromFileL
// ---------------------------------------------------------
//
TInt CCookieManagerServer::LoadGroupDataFromFileL( RFs& afileSession )
    {
    CLOG( ( EServer, 0, _L( "-> CCookieManagerServer::LoadGroupDataFromFileL" ) ) );
    TBuf<60> groupfile(KNullDesC);
    groupfile.Copy( KDefaultCookieXmlFolder );
    groupfile.AppendNum( RProcess().SecureId(), EHex );
    groupfile.Append( KDefaultCookieGroupFile );
    RXmlEngDOMImplementation DOM_impl;
    DOM_impl.OpenL();
    RXmlEngDocument doc;
    RXmlEngDOMParser parser;
    TInt error = parser.Open( DOM_impl );
    
    if (error == KErrNone)
     {


       TRAPD( err, doc = parser.ParseFileL( afileSession, groupfile ) );
       if ( ! err )
           {
           TXmlEngNode node;
           TXmlEngElement element;
           RXmlEngNodeList<TXmlEngElement> nodelist1;
           RXmlEngNodeList<TXmlEngElement> nodelist2;
           node = doc.DocumentElement();
           node.AsElement().GetChildElements(nodelist1);
           CleanupClosePushL(nodelist1);
           CleanupClosePushL(nodelist2);
           
           while ( nodelist1.HasNext() ) //Parent Node
             {
                element = nodelist1.Next();
                element.GetChildElements(nodelist2);
                TPtrC8 name = element.Name();
                     RArray<TUint32> sharedAppUidArray(5);
                     TUint32 groupId = 0;
                     TBool cookieSharableFlag;
                     TBool entryHasAttributes = element.HasAttributes();
                     if ( entryHasAttributes )
                      {
                        RXmlEngNodeList<TXmlEngAttr> attributeList;
                        element.GetAttributes(attributeList);
                        CleanupClosePushL(attributeList);
                        while ( attributeList.HasNext() )
                            {
                            TXmlEngAttr attr = attributeList.Next();
                            TPtrC8 attrName = attr.Name();
                            TPtrC8 attrData = attr.Value();
                            SettingAttribute(attrName,attrData,groupId,sharedAppUidArray,cookieSharableFlag );
                            }
                        CleanupStack::PopAndDestroy(); //attributeList
                       }
                         while( nodelist2.HasNext() )//Child Node
                            {
                              element = nodelist2.Next();
                              if ( ! element.IsNull() )
                                {
                                  TPtrC8 name = element.Name();
                                  TBool hasAttributes = element.HasAttributes();
                                  RXmlEngNodeList<TXmlEngAttr> attributeList;
                                  element.GetAttributes(attributeList);
                                  TInt count = attributeList.Count();
                                  CleanupClosePushL(attributeList);
                                  while ( attributeList.HasNext() )
                                      {
                                      TXmlEngAttr attr = attributeList.Next();
                                      TPtrC8 attrName = attr.Name();
                                      TPtrC8 attrData = attr.Value();
                                      SettingAttribute(attrName,attrData,groupId,sharedAppUidArray,cookieSharableFlag );
                                      }
                                  CleanupStack::PopAndDestroy(); //attributeList
                                }
                            }
                     CGroupIdInfo* groupIdInfo = CGroupIdInfo::NewL( groupId, sharedAppUidArray, cookieSharableFlag );
                     CleanupStack::PushL( groupIdInfo );
                     iGroupIdArray->AddGroupIdL( groupIdInfo );
                     CleanupStack::Pop( groupIdInfo );
                     CCookieGroupData* cookieGroupData = CCookieGroupData::NewL(groupId,sharedAppUidArray,cookieSharableFlag);
                     CleanupStack::PushL( groupIdInfo );
                     iCookieGroupDataArray->AddGroupDataL(cookieGroupData);
                     CleanupStack::Pop( groupIdInfo );
                     sharedAppUidArray.Close();
                 }
           CleanupStack::PopAndDestroy(); //nodelist2
           CleanupStack::PopAndDestroy(); //nodelist1
           }
     }
    doc.Close();               
    parser.Close();
    DOM_impl.Close();
    CLOG( ( EServer, 0, _L( "<- CCookieManagerServer::LoadGroupDataFromFileL" ) ) );
    return KErrNone;
    }

// ---------------------------------------------------------
// CCookieTimer::SettingAttribute
// ---------------------------------------------------------
//
void CCookieManagerServer::SettingAttribute(TDesC8& attrName, TDesC8& attrData,TUint32& aGroupId
        , RArray<TUint32>& aSharedAppUidArray, TBool& aCookieSharableFlag  )
    {
    CLOG( ( EServer, 0, _L( "-> CCookieManagerServer::SettingAttribute" ) ) );
    TBuf8<100> groupname(KNullDesC8);
    TBuf8<100> bufGroupId(KNullDesC8);
    TBuf8<100> bufSharedAppUidName(KNullDesC8);
    TUint32 sharedAppUid = 0;
    if ( ! attrName.CompareF(KGroupName))
         {
          groupname.Copy(attrData);
         } else if ( ! attrName.CompareF(KGroupId)) 
              {
              bufGroupId.Copy(attrData);
              TInt err = ChangeToDecimal(bufGroupId, aGroupId);
              
              }else if ( ! attrName.CompareF(KUid3))
                 {
                 bufSharedAppUidName.Copy(attrData);
                 TInt err = ChangeToDecimal(bufSharedAppUidName, sharedAppUid);
                 if (err == KErrNone)
                     aSharedAppUidArray.AppendL(sharedAppUid);
                 
                 } else if (! attrName.CompareF(KCookieSharable))
                     {
                      if ( !attrData.CompareF(KOff) )
                          aCookieSharableFlag = EFalse;
                      else
                          aCookieSharableFlag = ETrue;
                     }
    CLOG( ( EServer, 0, _L( "<- CCookieManagerServer::SettingAttribute" ) ) );
    }

// ---------------------------------------------------------
// CCookieTimer::ChangeToDecimal
// ---------------------------------------------------------
//
TInt CCookieManagerServer::ChangeToDecimal( TDes8& aBuf,TUint32& aUid )
    {
    CLOG( ( EServer, 0, _L( "-> CCookieManagerServer::ChangeToDecimal" ) ) );
     TBuf8<100> tempBuf;
     TPtrC8 tempPtr = aBuf.Mid( KHexDel().Length());
     tempBuf.Copy(tempPtr);
     TLex8 lex(tempBuf);
     TInt ret = lex.Val(aUid,EHex);
     CLOG( ( EServer, 0, _L( "<- CCookieManagerServer::ChangeToDecimal" ) ) );
     return ret;
    }
//EOF

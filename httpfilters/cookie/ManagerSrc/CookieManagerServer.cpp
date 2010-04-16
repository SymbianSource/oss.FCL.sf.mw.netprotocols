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
// #include <thttphdrval.h>

// User includes
#include "cookie.h"
#include "CookieArray.h"
#include "cookieipc.h"
#include "CookieLogger.h"
#include "CookieManagerServer.h"
#include "CookieManagerSession.h"
#include "CookieServerDef.h"

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
const TInt KCookieMaxFileNameLength = 256;

// Maximum file length
//const TInt KCookieMaxFileLength = 204800;	// 200 kilobytes


// Literals
_LIT( KDefaultCookieFolder, "C:\\Private\\" );
_LIT( KDefaultCookieFile,   "\\Cookies" );
_LIT( KDefaultExtension, ".dat");
_LIT( KUnderScore, "_");

// capability checking structures
const TUint cookieServerPolicyRangeCount = 6;

// server messages
const TInt  cookieServerPolicyRanges[ cookieServerPolicyRangeCount ] =
    {
    0,  // EStoreCookie
    1,  // EClearAllCookies
    2,  // EGetCookieSize
    3,  // EGetCookies
    4,  // ESetAppUid
    5
    };

// connection between messages and events
const TUint8 cookieServerPolicyElementsIndex[ cookieServerPolicyRangeCount ] =
    {
    0,  // EStoreCookie
    0,  // EClearAllCookies
    1,  // EGetCookieSize
    1,  // EGetCookies
    0,  // ESetAppUid
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

	iCookiePacker = new (ELeave) TCookiePacker( iStringPool );

	iCookieFileName = HBufC::NewL( KCookieMaxFileNameLength );

	iPersistentCookies = new (ELeave) CCookieArray;

    TPtr fileName( iCookieFileName->Des() );

    fileName.Copy( KDefaultCookieFolder );
    fileName.AppendNum( RProcess().SecureId(), EHex );
    fileName.Append( KDefaultCookieFile );
    fileName.Append( KDefaultExtension );

    if ( iFs.Connect() == KErrNone )  // we could connect to the file server
    ReadCookiesFromFile();

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
    iServerClosing = ETrue;

	delete iPersistentCookies;

	delete iCookieFileName;

	iFs.Close();
    iStringPool.Close();

	delete iCookiePacker;

    delete iCloseTimer;

    CLOG( ( EServer, 0, _L( "CCookieManagerServer::~CCookieManagerServer") ) );
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
        iPersistentCookies->RemoveNonPersistent();
		iCloseTimer->After( KCookieCloseTime );
        //just write cookies back to the file when browser is closed,
        //no need wait till cookie server is shutdown.
        WriteCookiesToFile();
        }

    CLOG( ( EServer, 0, _L( "<- CCookieManagerServer::CloseSession" ) ) );
    }


// ---------------------------------------------------------
// CCookieManagerServer::ReadCookiesFromFile
// ---------------------------------------------------------
//
TInt CCookieManagerServer::ReadCookiesFromFile()
    {
	CLOG( ( EServer, 0,
					_L( "-> CCookieManagerServer::ReadCookiesFromFile" ) ) );

	TInt err;
	if ( iCookieFileName->Length() != 0 )
		{
		RFile file;
		err = file.Open( iFs, *iCookieFileName,
							EFileShareExclusive | EFileStream | EFileRead );
			if ( err == KErrNone )	// the file does exist and could be opened
				{
				TInt size;
				err = file.Size( size );
				if ( err == KErrNone )	// size query was successful
					{
					HBufC8* fileBuffer = HBufC8::New( size );
					if ( fileBuffer )// there was enough memory for fileBuffer
						{
						TPtr8 fileBufferDes( fileBuffer->Des() );
						err = file.Read( fileBufferDes );
						if ( err == KErrNone )
							{
							// unfortunately this method might leave, because
							// it allocates memory for cookies dynamically
							TRAP( err,
								iCookiePacker->UnpackCookiesFromBufferL
												( *fileBuffer, iPersistentCookies->CookieArray() ) );
							if ( err != KErrNone )
							    {
								delete fileBuffer;
								file.Close();
								iFs.Delete(*iCookieFileName); //Delete file.
								return KErrNotFound;
							    }
							}

						delete fileBuffer;
						}
					else
						{
						err = KErrNoMemory;
						}
					}

				file.Close();
			}
		}
	else	// if iCookieFileName->Length() == 0
		{
		err = KErrNotFound;
		}

	CLOG( ( EServer, 0,
				_L( "<- CCookieManagerServer::ReadCookiesFromFile, errcode%d"),
				err ) );

	return err;
    }

// ---------------------------------------------------------
// CCookieManagerServer::WriteCookiesToFile
// ---------------------------------------------------------
//
TInt CCookieManagerServer::WriteCookiesToFile()
    {
    CLOG( ( EServer, 0,
				_L( "-> CCookieManagerServer::WriteCookiesToFile" ) ) );

    TInt err(KErrNone);
	if ( !iPersistentCookies->Count() )
		{
		CLOG( ( EServer, 0,
				_L( "<- CCookieManagerServer::WriteCookiesToFile, errcode%d" ),
				KErrNone ));

        // delete cookie file
    err = iFs.Delete( *iCookieFileName );
		return err;
		}

	if ( iCookieFileName->Length() != 0 )
		{
		if ( CheckDiskSpace( iFs, *iCookieFileName ) )
			{
      iFs.CreatePrivatePath( EDriveC );
			RFile file;
			iFs.MkDirAll(*iCookieFileName);
			err = file.Replace( iFs, *iCookieFileName,
							EFileShareExclusive | EFileStream | EFileWrite );
				if ( err == KErrNone )
					{
					// get the maximum length of cookies
    				TInt cookieCount( iPersistentCookies->Count() );
    				TInt size( 0 );
    				TInt maxSize( 0 );
					for( TInt i = 0; i < cookieCount; i++ )
    					{
    					if ( (*iPersistentCookies)[i]->Persistent() && 
    						 !(*iPersistentCookies)[i]->Expired() )
    						{
	    					size = (*iPersistentCookies)[i]->Size( EFalse );
	    					if( size > maxSize )
	        					{
		    				    maxSize = size;
		    				    }
		    				}
		    			}
		    		maxSize++;
		    		CLOG( ( EServer, 0, _L("maxSize: %d"), maxSize ) );
		    		// allocate buffer for it
					HBufC8* fileBuffer = HBufC8::New( maxSize );
					if ( fileBuffer )
						{
						TPtr8 fileBufferDes = fileBuffer->Des();

						// we ignore a possible packing or file writing error
						// in this loop as these kinds of errors are not fatal
						// and may not reappear during the next iteration
						for ( TInt i = 0; i < cookieCount; i++ )
							{
    					if ( (*iPersistentCookies)[i]->Persistent() && 
    						 !(*iPersistentCookies)[i]->Expired() )
    						{
								fileBufferDes.SetLength(0);
	
								// use CliPackCookie as SrvPackCookie will 
	                            // suppress the defaulted domain attribute...
	                            err = iCookiePacker->CliPackCookie( fileBufferDes,
																(*(*iPersistentCookies)[i]) );
	
								if ( err == KErrNone )
									{
									err = file.Write( *fileBuffer );
									}
								}
							}

						delete fileBuffer;
						}
					else
						{
						err = KErrNoMemory;
						}

					file.Close();
					}
				}
			else	// there is not enough disk space
				{
				err = KErrDiskFull;
			}
		}
	else	// if ( iCookieFileName->Length() == 0 )
		{
		err = KErrNotFound;
		}

	CLOG( ( EServer, 0,
		_L( "<- CCookieManagerServer::WriteCookiesToFile, errcode%d" ), err ) );

	return err;
    }

// ---------------------------------------------------------
// CCookieManagerServer::ClearCookies
// ---------------------------------------------------------
//
TInt CCookieManagerServer::ClearAllCookies()
    {
    TInt count = iPersistentCookies->ClearAllCookies();

    // delete cookie file, just for sure
    // this is done also in destructor
    iFs.Delete( *iCookieFileName );
    return count;
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
CCookieArray* CCookieManagerServer::CookieArray()
    {
    return iPersistentCookies;
    }

// ---------------------------------------------------------
// CCookieManagerServer::StorePersistentCookie
// ---------------------------------------------------------
//
void CCookieManagerServer::StorePersistentCookieL( CCookie* aCookie,
												 const TDesC8& aRequestUri,
												 const TInt aIndex )
	{	 
	if (aIndex == -1)
		{
		iPersistentCookies->AddL( aCookie, aRequestUri);
		}
	else
		{
		iPersistentCookies->InsertL( aCookie, aIndex);
		}
    }


// ---------------------------------------------------------
// CCookieManagerServer::GetCookies
// ---------------------------------------------------------
//
TInt CCookieManagerServer::GetCookies( const TDesC8& aRequestUri,
									  RPointerArray<CCookie>& aCookies ) const
	{
	return iPersistentCookies->GetCookies( aRequestUri, aCookies );
	}

// ---------------------------------------------------------
// CCookieManagerServer::SetFileName
// ---------------------------------------------------------
//
void CCookieManagerServer::SetFileName(TUint32& aAppUid) 
    {
    *iCookieFileName = KNullDesC;
    TPtr fileName( iCookieFileName->Des() );
    fileName.Copy( KDefaultCookieFolder );
    fileName.AppendNum( RProcess().SecureId(), EHex );
    TBuf<KMaxFileName> buf(KDefaultCookieFile);
    if(aAppUid)
        {       
        buf.Append(KUnderScore);        
        buf.AppendNum(aAppUid,EHex);        
        }
    fileName.Append(buf);
    fileName.Append(KDefaultExtension);      
	//just write cookies back to the file before clearallcookies
	WriteCookiesToFile();
    //Delete the cookie list as we are going to read from File
    iPersistentCookies->ClearAllCookies() ;
    ReadCookiesFromFile();
    }

// ---------------------------------------------------------
// CCookieManagerServer::GetFileName
// ---------------------------------------------------------
//
TDesC& CCookieManagerServer::GetFileName() const
    {
    return *iCookieFileName;
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

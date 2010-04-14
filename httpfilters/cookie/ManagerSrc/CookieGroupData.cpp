/*
 * CCookieGroupData.cpp
 *
 *  Created on: Nov 24, 2009
 *      Author: mohanti
 */

//System Includes
#include <e32base.h>
#include <sysutil.h>
#include <e32cmn.h>

//User Includes
#include "CookieGroupData.h"
#include "CookieArray.h"
#include "cookieipc.h"
#include "CookieLogger.h"
//Constants

// Literals
_LIT( KDefaultCookieFolder, "C:\\Private\\" );
_LIT( KDefaultCookieFile,   "\\Cookies" );
_LIT( KDefaultExtension, ".dat");
_LIT( KUnderScore, "_");

//Member Functions

// ---------------------------------------------------------
// CCookieGroupData::NewL
// ---------------------------------------------------------
//
CCookieGroupData* CCookieGroupData::NewL(TUint32 aGroupId, const RArray<TUint32>& aAppuid, TBool aCookieSharableFlag)
    {
    CLOG( ( EClient, 0, _L( "-> CCookieGroupData::NewL" ) ) );
    
    CCookieGroupData* self = CCookieGroupData::NewLC(aGroupId,aAppuid,aCookieSharableFlag);
    CleanupStack::Pop();
    
    CLOG( ( EClient, 0, _L( "<- CCookieGroupData::NewL" ) ) );
    return self;
    }

// ---------------------------------------------------------
// CCookieGroupData::NewLC
// ---------------------------------------------------------
//
CCookieGroupData* CCookieGroupData::NewLC(TUint32 aGroupId,const RArray<TUint32>& aAppuid,TBool aCookieSharableFlag)
    {
    CLOG( ( EClient, 0, _L( "-> CCookieGroupData::NewLC" ) ) );
    
    CCookieGroupData* self = new (ELeave) CCookieGroupData(aGroupId,aCookieSharableFlag);
    CleanupStack::PushL(self);
    self->ConstructL(aAppuid);
    
    CLOG( ( EClient, 0, _L( "<- CCookieGroupData::NewLC:" ) ) );
    return self;
    }

// ---------------------------------------------------------
// CCookieGroupData::CCookieGroupData
// ---------------------------------------------------------
//
CCookieGroupData::CCookieGroupData(TUint32 aGroupId, TBool aCookieSharableFlag)
  :iGroupId(aGroupId),iCookieSharableFlag(aCookieSharableFlag)
    {
    CLOG( ( EClient, 0, _L( "-> CCookieGroupData::CCookieGroupData:" ) ) );
    CLOG( ( EClient, 0, _L( "<- CCookieGroupData::CCookieGroupData:" ) ) );
    }

// ---------------------------------------------------------
// CCookieGroupData::CookieArray
// ---------------------------------------------------------
//
CCookieArray* CCookieGroupData::CookieArray()
    {
    return iCookieArray;
    }
TInt CCookieGroupData::TotalAppUid()
    {
    return iSharedUid.Count();
    }
// ---------------------------------------------------------
// CCookieGroupData::~CCookieGroupData
// ---------------------------------------------------------
//
CCookieGroupData::~CCookieGroupData()
    {
    CLOG( ( EClient, 0, _L( "-> CCookieGroupData::~CCookieGroupData:" ) ) );
    
    WriteCookiesToFile();
    delete iCookieArray;
    delete iCookieFileName;
    iStringPool.Close();
    delete iCookiePacker;
    iFs.Close();
    iSharedUid.Close();
    CLOG( ( EClient, 0, _L( "<- CCookieGroupData::~CCookieGroupData:" ) ) );
    }

// ---------------------------------------------------------
// CCookieGroupData::StorePersistentCookieL
// ---------------------------------------------------------
//
void CCookieGroupData::StorePersistentCookieL( CCookie* aCookie,
                                                 const TDesC8& aRequestUri,
                                                 const TInt aIndex )
    {
    CLOG( ( EClient, 0, _L( "-> CCookieGroupData::StorePersistentCookieL:aRequestUri = %S" ), &aRequestUri) );
    
    if (aIndex == -1)
        {
        iCookieArray->AddL( aCookie, aRequestUri);
        }
    else
        {
        iCookieArray->InsertL( aCookie, aIndex);
        }
    
    CLOG( ( EClient, 0, _L( "<- CCookieGroupData::StorePersistentCookieL:" ) ) );

    }

// ---------------------------------------------------------
// CCookieGroupData::Count
// ---------------------------------------------------------
//
TInt CCookieGroupData::Count()const
    {
    return iCookieArray->Count();
    }

TUint32 CCookieGroupData::operator[](TInt aIndex) const
    {
    return (iSharedUid[aIndex]);
    }

// ---------------------------------------------------------
// CGroupIdInfo::At
// ---------------------------------------------------------
//
TUint32 CCookieGroupData::At(TInt aIndex) const
    {
    return (iSharedUid[ aIndex ]);
    }

// ---------------------------------------------------------
// CGroupIdInfo::GetSharedUid
// ---------------------------------------------------------
//
TUint32 CCookieGroupData::GetSharedUid(TInt aIndex)
    {
    return At(aIndex);
    }

// ---------------------------------------------------------
// CCookieGroupData::ConstructL
// ---------------------------------------------------------
//
void CCookieGroupData::ConstructL(const RArray<TUint32>& aAppuid)
    {
    CLOG( ( EClient, 0, _L( "-> CCookieGroupData::ConstructL:" ) ) );
    TInt count = aAppuid.Count();
    CLOG( ( EClient, 0, _L( "-> CGroupIdArray::count = %d" ), count ) );
    for(TInt i=0; i<count; i++)
        {
        CLOG( ( EClient, 0, _L( "-> CGroupIdArray::aAppuid[%d] = %x " ), aAppuid[i] ) );
        iSharedUid.AppendL(aAppuid[i]);
        }
    iCookieArray = new (ELeave) CCookieArray;
    iStringPool.OpenL();
    iCookiePacker = new (ELeave) TCookiePacker( iStringPool );
    iCookieFileName = HBufC::NewL( KCookieMaxFileNameLength );

    TPtr fileName( iCookieFileName->Des() );
    fileName.Copy( KDefaultCookieFolder );
    //fileName.AppendNum( iGroupId, EHex );
    fileName.AppendNum( RProcess().SecureId(), EHex );
    TBuf<KMaxFileName> buf(KDefaultCookieFile);
    buf.Append(KUnderScore);
    buf.AppendNum(iGroupId,EHex);
    //For Widget case file name shobe becookie_<sid>_<appuid>.dat
    if(!iCookieSharableFlag && iSharedUid[0]!=0)
        {
        buf.Append(KUnderScore);
        buf.AppendNum(iSharedUid[0],EHex);
        }
    fileName.Append(buf);
    fileName.Append(KDefaultExtension);
    if ( iFs.Connect() == KErrNone )
        {
        ReadCookiesFromFile();
        }

    CLOG( ( EClient, 0, _L( "<- CCookieGroupData:::ConstructL:" ) ) );
    }

// ---------------------------------------------------------
// CCookieGroupData::ReadCookiesFromFile
// ---------------------------------------------------------
//
TInt CCookieGroupData::ReadCookiesFromFile()
    {
    CLOG( ( EServer, 0,_L( "-> CCookieGroupData::ReadCookiesFromFile" ) ) );
    
    TInt err;
    if ( iCookieFileName->Length() != 0 )
        {
//        RFs iFs;
//        User::LeaveIfError(iFs.Connect());
        RFile file;
        err = file.Open( iFs, *iCookieFileName,
                            EFileShareExclusive | EFileStream | EFileRead );
            if ( err == KErrNone )  // the file does exist and could be opened
                {
                TInt size;
                err = file.Size( size );
                if ( err == KErrNone )  // size query was successful
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
                                                ( *fileBuffer, iCookieArray->CookieArray() ) );
                            if ( err != KErrNone )
                                {
                                delete fileBuffer;
                                file.Close();
                                iFs.Delete(*iCookieFileName); //Delete file.
                                //iFs.Close();
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
                //iFs.Close();
            }
        }
    else    // if iCookieFileName->Length() == 0
        {
        err = KErrNotFound;
        }

    CLOG( ( EServer, 0,_L( "<- CCookieGroupData::ReadCookiesFromFile, errcode%d"), err ) );

    return err;
    }

// ---------------------------------------------------------
// CCookieGroupData::WriteCookiesToFile
// ---------------------------------------------------------
//
TInt CCookieGroupData::WriteCookiesToFile()
    {
    CLOG( ( EServer, 0,_L( "-> CCookieGroupData::WriteCookiesToFile" ) ) );
//    RFs iFs;
//    User::LeaveIfError(iFs.Connect());

    TInt err(KErrNone);
    if ( !iCookieArray->Count() )
        {
         CLOG( ( EServer, 0,_L( "<- CCookieGroupData::WriteCookiesToFile, errcode%d" ), KErrNone ));

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
                    TInt cookieCount( iCookieArray->Count() );
                    TInt size( 0 );
                    TInt maxSize( 0 );
                    for( TInt i = 0; i < cookieCount; i++ )
                        {
                        if ( (*iCookieArray)[i]->Persistent() &&
                             !(*iCookieArray)[i]->Expired() )
                            {
                            size = (*iCookieArray)[i]->Size( EFalse );
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
                        if ( (*iCookieArray)[i]->Persistent() &&
                             !(*iCookieArray)[i]->Expired() )
                            {
                                fileBufferDes.SetLength(0);

                                // use CliPackCookie as SrvPackCookie will
                                // suppress the defaulted domain attribute...
                                err = iCookiePacker->CliPackCookie( fileBufferDes,
                                                                (*(*iCookieArray)[i]) );

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
                    //iFs.Close();
                    }
                }
            else    // there is not enough disk space
                {
                err = KErrDiskFull;
            }
        }
    else    // if ( iCookieFileName->Length() == 0 )
        {
        err = KErrNotFound;
        }

    CLOG( ( EServer, 0,
        _L( "<- CCookieGroupData::WriteCookiesToFile, errcode%d" ), err ) );

    return err;
    }

// ---------------------------------------------------------
// CCookieGroupData::CheckDiskSpace
// ---------------------------------------------------------
//
TBool CCookieGroupData::CheckDiskSpace( RFs& aFileSystem,
                                           const TDesC& aFileName ) const
    {
    CLOG( ( EClient, 0, _L( "-> CCookieGroupData::CheckDiskSpace:" ) ) );

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
    CLOG( ( EClient, 0, _L( "<- CCookieGroupData::CheckDiskSpace:" ) ) );
    
    return ( err == KErrNone ? ETrue : EFalse );
    }

// ---------------------------------------------------------
// CCookieGroupData::ClearCookies
// ---------------------------------------------------------
//
TInt CCookieGroupData::ClearAllCookies()
    {
    CLOG( ( EClient, 0, _L( "-> CCookieGroupData::ClearAllCookies:" ) ) );
    
    TInt count = iCookieArray->ClearAllCookies();
    iFs.Delete( *iCookieFileName );

    CLOG( ( EClient, 0, _L( "<- CCookieGroupData::ClearAllCookies:count = %d " ), count ) );
    return count;
    }

// ---------------------------------------------------------
// CCookieGroupData::GetFileName
// ---------------------------------------------------------
//
const TDesC& CCookieGroupData::GetFileName() const
    {
    CLOG( ( EClient, 0, _L( "-> CCookieGroupData:::GetFileName" ) ) );

    if ( iCookieFileName )
        {
        CLOG( ( EClient, 0, _L( "<- CCookieGroupData::GetFileName:iCookieFileName = %S" ), &iCookieFileName ) );
        return *iCookieFileName;
        }
    CLOG( ( EClient, 0, _L( "<- CCookieGroupData::GetFileName:iCookieFileName = %S" ), &iCookieFileName ) );
    return KNullDesC();
    }

// ---------------------------------------------------------
// CCookieGroupData::SetFileName
// ---------------------------------------------------------
//
void CCookieGroupData::SetFileName(TUint32& aAppUid,TUint32& aSecureId)
    {
    CLOG( ( EClient, 0, _L( "<- CCookieGroupData::SetFileName" ) ) );
    *iCookieFileName = KNullDesC;
    TPtr fileName( iCookieFileName->Des() );
    fileName.Copy( KDefaultCookieFolder );
    fileName.AppendNum( RProcess().SecureId(), EHex );
    TBuf<KMaxFileName> buf(KDefaultCookieFile);
    buf.Append(KUnderScore);
    buf.AppendNum(aSecureId,EHex);
    //For Widget case file name shobe becookie_<sid>_<appuid>.dat
    if(!iCookieSharableFlag && iSharedUid[0]!=0)
        {
        buf.Append(KUnderScore);
        buf.AppendNum(aAppUid,EHex);
        }
    fileName.Append(buf);
    fileName.Append(KDefaultExtension);
    CLOG( ( EClient, 0, _L( "<- CCookieGroupData::SetFileName" ) ) );
    }
//eof

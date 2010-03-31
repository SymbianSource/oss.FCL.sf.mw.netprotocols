/*
 * CCookieGroupData.h
 *
 *  Created on: Nov 24, 2009
 *      Author: mohanti
 */

#ifndef COOKIEGROUPDATA_H_
#define COOKIEGROUPDATA_H_

//System Includes
#include <stringpool.h> 
#include <f32file.h>
#include <e32def.h>
#include <e32base.h>
#include <e32cmn.h>

//Forward Class declaration
class CCookieArray;
class CCookie;
class TCookiePacker;

//Constants

const TInt KCookieMaxFileNameLength = 256;

/** Class to store the Cookie Data based on secure id and appuid
*/

NONSHARABLE_CLASS( CCookieGroupData ):public CBase
    {
public:
    /**Constructor.
    * 
    * @param aGroupId identifies the secure id of a client request
    * @param aAppUid  identifies the Appuid sets in the transaction property of a client request
    * @return instance of CCookieGroupData
    */
    static CCookieGroupData* NewL(TUint32 aGroupId,const RArray<TUint32>& aAppuid, TBool aCookieSharableFlag = EFalse );

    /**Constructor.
    * 
    * @param aGroupId identifies the secure id of a client request
    * @param aAppUid  identifies the Appuid sets in the transaction property of a client request
    * @return instance of CCookieGroupData
    */
    static CCookieGroupData* NewLC(TUint32 aGroupId,const RArray<TUint32>& aAppuid,TBool aCookieSharableFlag = EFalse);


    /**Standard destructor.
    */
    ~CCookieGroupData();

    /**
    * @return Instance of CCookieArray object.
    */
    CCookieArray* CookieArray();
    
    /**
    * @return GroupId
    */
    inline const TUint32 GetGroupId(){return iGroupId;}

    /* Gets Shared uid based on index no.
    *@param aIndex Indicates index no. 
    * @return AppUid
    */
    TUint32 GetSharedUid(TInt aIndex);
    
    /*Gets Cookie Sharable Flag
     * @return iCookieSharableFlag indicates sharable flag
     */
    inline TBool GetCookieSharableFlag(){return iCookieSharableFlag;}
    
    /*Returns appuid based on index no
     * 
     */
    TUint32 At(TInt aIndex) const;
    
    /* Subscripting Operator
     * 
     */
    TUint32 operator[](TInt aIndex) const;

    /**
    * Store a cookie in a persistent way. We take over the ownership
    * of the cookie.
    * If the cookie exists, that is, there is already a cookie whose NAME
    * attribute is the same, then the new cookie supersedes the old when :
    *   - the old and new Domain attribute value compare equal (case-
    * insensitive),
    *   - the old and new Path attribute values string-compare equal
    * (case-sensitive).
    * However, if the Max-Age attribute is zero, then the old and new cookies
    * are discarded.
    * @param aCookie The cookie to be stored.
    * @param aRequestUri The current HTTP request-URI.
    * @param aIndex Cookie is inserted at the position of existing cookie, and exisiting cookie is removed.
    */
    void StorePersistentCookieL( CCookie* aCookie, const TDesC8& aRequestUri, const TInt aIndex = -1 );

    
    //void StorePersistentCookieL( CCookie* aCookie,const TDesC8& aRequestUri,const TInt aIndex );

    /**Reads the Cookie related info specific for a group for a file. 
    * 
    * @return KErrNotFound in case of File not found from the path
    *         KErrNoMemory in case no Memory
    *         KErrNone Incase of successful reading of the cookie info from the file.
    */
    TInt ReadCookiesFromFile();

    /**
    * @return KErrNone if Succesfully Writes the Cookie info in to the file.
    */
    TInt WriteCookiesToFile();
    
    /**All the cookies will be deleted from the memory as well as from the file for a group. 
    * @return the number of files deleted.
    */
    TInt ClearAllCookies();

    /** Returns the Number of Cookie objects present for a group
    * @return Numaaber of Cookie Objects
    */
    TInt Count()const;

    /* Return Filename
    * @return returns the Filename 
    */
    const TDesC& GetFileName() const;

    /** Sets Filename 
    * 
    * @param aAppUid Indicates appuid
    * @param aSecureId Indicates Client Id
    * @return void
    */
    void SetFileName(TUint32& aAppUid,TUint32& aSecureId);
    
    /* Constructor
     * 
     */
    void ConstructL(const RArray<TUint32>& aAppuid);
    
    /*Total Appuids
     * 
     */
    TInt TotalAppUid();
protected:
private:
    /*Constructor
     * aGroupId Indicates group Id
     * aCookieSharableFlag CookieSharable Flag
     */
    CCookieGroupData(TUint32 aGroupId, TBool aCookieSharableFlag = EFalse);
 
    /* Check Diskspace available before saving the cookies in to File System
    * @param 
    * @param 
    * @param
    * @return 
    */
    TBool CheckDiskSpace( RFs& aFileSystem, const TDesC& aFileName ) const;

public:
    
protected:
    
private:
    CCookieArray*           iCookieArray;
    HBufC*                  iCookieFileName;
    RStringPool             iStringPool;
    TCookiePacker*          iCookiePacker;
    RFs                   iFs;
    TUint32 iGroupId;
    TBool iCookieSharableFlag;
    RArray<TUint32> iSharedUid;
    //TUint32 iAppUid;
    };

#endif /* COOKIEGROUPDATA_H_ */

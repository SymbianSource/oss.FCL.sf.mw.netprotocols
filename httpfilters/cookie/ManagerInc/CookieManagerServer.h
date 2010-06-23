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
* Description:  Declaration of class CCookieManagerServer
*         
*
*/


#ifndef __COOKIEMANAGER_SERVER_H__
#define __COOKIEMANAGER_SERVER_H__

// INCLUDE FILES
	// System includes
#include <http.h>
#include <f32file.h>
#include <e32def.h>
#include <stringpool.h> 

// Opcodes used in message passing between client and server
enum TCookieServerRqst 
	{
	EStoreCookie,
    EClearAllCookies,
	EGetCookieSize,
	EGetCookies,
	ESetAppUid,
	EDestroyCookies,
	EGetCookieSharableFlag,
	EClearAppUidCookies
    };

// FORWARD DECLARATIONS

class CCookie;
class CCookieArray;
class CCookieTimer;
class CCookieManagerSession;
class TCookiePacker;
class CCookieGroupDataArray;
class CGroupIdArray;
class TXmlEngElement;
// CLASS DECLARATIONS

/**
*
*/
NONSHARABLE_CLASS( CCookieManagerServer ) :
                             public CPolicyServer
	{
//    friend CCookieManagerSession;
public:
    /**
    *
    */
    static CCookieManagerServer* NewL();

protected:
	CCookieManagerServer( TInt aPriority );

public:
    /**
    * Standard destructor
    */
    ~CCookieManagerServer();

    /**
    *
    */
    void ConstructL();

    /**
    * Session notification on closing.
    * Care should be taken in clients NOT to call it if SERVER is closing...
    */
    void CloseSession();

    /**
    * Clears all cookies
    * @return The number of cookies removed
    */
    //TInt ClearAllCookies();

    /**
    * Returns a pointer to the stringpool we currently use.
    */
    RStringPool* StringPool();


    /**
    * Returns a poitner for the persistent cookies we currently have.
    */
    //CCookieArray* CookieArray();


    /**
    * Store a cookie in a persistent way. We take over the ownership
	* of the cookie.
	* If the cookie exists, that is, there is already a cookie whose NAME
	* attribute is the same, then the new cookie supersedes the old when :
	*	- the old and new Domain attribute value compare equal (case-
	* insensitive),
	*	- the old and new Path attribute values string-compare equal
	* (case-sensitive).
	* However, if the Max-Age attribute is zero, then the old and new cookies
	* are discarded.
	* @param aCookie The cookie to be stored.
	* @param aRequestUri The current HTTP request-URI.
	* @param aIndex Cookie is inserted at the position of existing cookie, and exisiting cookie is removed.
    */
    //void StorePersistentCookieL( CCookie* aCookie, const TDesC8& aRequestUri, const TInt aIndex = -1 );

	/**
	*
	*/
//	TInt GetCookies( const TDesC8& aRequestUri,
//					RPointerArray<CCookie>& aCookies ) const;
	
	/**
	*Sets the File Name of the Cookie File using AppUid				
    *@param aFileName The AppUid of the Application
    */
    //void SetFileName(TUint32& aAppUid);
    
    /**
    *@return the File Name of the Cookie file
    */    
    TDesC& GetFileName() const;
    
    /* Loading the group specific info from CookieGroupData.xml file
     * 
     */
    TInt LoadGroupDataFromFileL( RFs& aFileSession );
   
    /* Parsing the  Xml elements
     * 
     */
    void ParseElement( TXmlEngElement& aElement );
    
    /* 
     * 
     */    
    CCookieArray* CookieArray( TInt aIndex );
    
    /*
     * 
     */
    CCookieGroupDataArray* CookieGroupDataArray();
    
    /** 
    * Get the relevant cookies for a transaction
    * @param aRequestUri The URI of the current HTTP request.
    * @param aCookies An array which will be filled with the cookies
    * for aTransaction based on the index value.
    * @param aIndex Indiactes Index Value.
    */    
    TInt GetCookies( const TDesC8& aRequestUri, RPointerArray<CCookie>& aCookies, TInt aIndex ) const;
    
    /* 
     * 
     */
    CGroupIdArray* GroupIdArray();
    
    /*Changes Hexadecimal value to Decimal Value
     * 
     */
    TInt ChangeToDecimal( TDes8& aBuf,TUint32& aUid );
    
    /* Extracting the attribute information from parsed xml element
     * 
     */
    void SettingAttribute(TDesC8& aAttr, TDesC8& aVal,TUint32& aGroupId
            , RArray<TUint32>& aSharedAppUidArray, TBool& aCookieSharableFlag  );

private:
	/**
	* This method checks if there is enough space to persist the cookies.
	* The requirement is to have at least KCookieMaxFileLength bytes on the
	* specified (it is stored in the ini file) drive.
	* @param aFileSystem File system reference, it is used for a system call,
	* @param aFileName The name of the file into which we want to write
	* cookies.
	* @return ETrue if there is enough space for the file operation,
	* EFalse otherwise.
	*/
	TBool CheckDiskSpace( RFs& aFileSystem, const TDesC& aFileName ) const;

    /**
    *
    */
    CSession2* DoNewSessionL();


	/**
	*
	*/
	virtual CSession2* NewSessionL( const TVersion& aVersion, 
                                    const RMessage2& aMessage ) const;


    /**
    * Reads the cookies from the file into a buffer. After reading the file
	* content we can process it by calling ParseCookiesFromBufferL method.
	* @return The error code indicating the type of failure.
    */
    //TInt ReadCookiesFromFile();


    /**
    * Writes the currently valid cookies into a file.
	* Note : it may happen that although this method returns an error code
	* other than KErrNone, the cookies (not all, however) have been persisted.
	* In this case, it indicates a failure during persisting a particular
	* cookie, but it may not neccessarily be true for ALL cookies.
	* @return The error code indicating the type of failure.
    */
    //TInt WriteCookiesToFile();

private:
    TUint32                 iSessionCount;
    TBool                   iServerClosing;

	CCookieTimer*           iCloseTimer;

	//HBufC*                  iCookieFileName;
    //RFs 					iFs;
    //CCookieArray*			iPersistentCookies;	// new-style

    RStringPool             iStringPool;

	// As we open our string pool only in ConstructL we cannot instantiate the
	// cookie packer in the constructor :(
	TCookiePacker*			iCookiePacker;
	CCookieGroupDataArray*  iCookieGroupDataArray;
	CGroupIdArray*          iGroupIdArray;
	
	};


//**********************************
// CCookieTimer
//**********************************
//
NONSHARABLE_CLASS( CCookieTimer ) : public CTimer
	{
	public :	// construction, destruction
		/**
		* Two-phase constructor
		*/
		static CCookieTimer* NewL();

		/**
		* Destructor
		*/
		~CCookieTimer();

	protected :	// from CTimer
		/**
		* @see CActive::RunL
		*/
		void RunL();

	private :	// construction
		/**
		* Constructor.
		*/
		CCookieTimer();
	};

#endif //__COOKIEMANAGER_SERVER_H__

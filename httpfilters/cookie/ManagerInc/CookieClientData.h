/*
 * CookieClientData.h
 *
 *  Created on: Dec 28, 2009
 *      Author: mohanti
 */

#ifndef COOKIECLIENTDATA_H_
#define COOKIECLIENTDATA_H_

//System Includes
#include <e32base.h>

//Forward Declaration
class CCookieArray;
class CCookie;

//THis class holds the client side cookies of a specific Client 
NONSHARABLE_CLASS( CCookieClientData ):public CBase
    {
public:
    /**Constructor 
    */
    static CCookieClientData* NewL(TUint32 aGroupId);
    
    /**Constructor 
    */
    static CCookieClientData* NewLC(TUint32 aGroupId);
    
    static CCookieClientData* NewL(TUint32 aGroupId, TUint32 aWidgetUid, TBool aCookieSharableFlag = EFalse, TBool aInitFlag = EFalse);
    
    static CCookieClientData* NewLC(TUint32 aGroupId, TUint32 aWidgetUid, TBool aCookieSharableFlag, TBool aInitFlag);
    /**Standard Constructor
    */
    void ConstructL();
    
    /**Standard Destructor 
    */
    ~CCookieClientData();

    /** Cookie Array Handle
     * @return returns the pointer of CookieArray object. 
    */
    CCookieArray* CookieArray();
    
    /** Stores the CCookie objects for a specific Uri .
        @param aCookie Indicates the Cookie Objects to be Stored      
        @param aRequestUri Indicates the Uri for which cookie Objects needs to be Stored
        @param aIndex   
        @return  void 
1    */
    void StorePersistentCookieL( const CCookie* aCookie, const TDesC8& aRequestUri, const TInt aIndex = -1 );
    
    /** Stores the CCookie objects for a specific Uri in Client Address Space.
        @param aCookie Indicates the Cookie object to be stored.      
        @param aUri    Indicates the Uri for which the Cookie Information need to be stored.
        @return void .
        
    */
    void StoreCookieAtClientSideL( const CCookie* aCookie,const TDesC8& aUri );

    /** Finds the CCookie objects for a specific Uri in Client Address Space
        @param aRequestUri Indicates the Uri for which Cookie info is required       
        @param aCookies Reference to an array of Cookie objects for a uri after completion of the method.
        @param aFound   Indiacates whether cookie info for a requested Uri is present or not in the Client address space or not.
        @return .
    */
    TInt GetClientSideCookies( const TDesC8& aRequestUri,RPointerArray<CCookie>& aCookies,TBool& aCookieFound ) const;
        /*Setting the AppUid
     * @param aAppUid Appuid to be set in case of Widgets running in the same process  
     */
    void SetAppUid(TUint32 aAppUid );

    /** Number of CCookie objects present in Array.
     * @return Number of Cookie objects present in the array
    */
    TInt Count()const;
    
    /* Sets appuid
     * @param aWidgetUid indicates appuid of the widget
     */
    inline void SetWidgetUid(TUint32 aWidgetUid ){iWidgetUid = aWidgetUid;}
    /*Sets Cookie Sharable flag
     * @param  aCookieSharableFlag indicates whether cookie sharing is allowed or not
     */
    inline void SetCookieCookieSharableFlag(TBool aCookieSharableFlag){iCookieSharableFlag = aCookieSharableFlag;}
    
    /*Sets the initailization flag once the initialization of CCookieClientData object has been completed 
     * @param aInitFlag indicates initialization flag
     */
    inline void SetInitFlag(TBool aInitFlag){iInitFlag = aInitFlag;}
    
    /*Gets Cookie Sharable flag 
     * @return iCookieSharableFlag indicates Cookie Sharable flag
     */
    inline const TBool& GetCookieSharableFlag(){return iCookieSharableFlag;}
    
    /*Gets Initialization flag
     * @return iInitFlag indicates initialization flag for CCookieClientData object
     */
    inline const TBool& GetInitFlag(){return iInitFlag;}
    
    /*Gets Groupid of the CCookieClientData object
     * @return iGroupId indicates group id of CCookieClientData object
     */
    inline const TUint32& GetGroupId(){return iGroupId;}
    
    /*Gets AppUid in case of WRT Widget 
     * @return iWidgetUid indicates appuid of the WRT Widget
     */
    inline const TUint32& GetWidgetUid(){return iWidgetUid;}
protected:
private:
    /*Constructor
     * 
     */
    CCookieClientData(TUint32 aGroupId);
    /*Constructor
     * 
     */
    CCookieClientData(TUint32 aGroupId, TUint32 aWidgetUid, TBool aCookieSharableFlag, TBool aInitFlag);
public:
protected:
    
private:
    TBool iInitFlag;
    TBool iCookieSharableFlag;
    TUint32 iGroupId;
    //Appuid of client in case of secure WRT Widget
    TUint32 iWidgetUid;
    CCookieArray* iCookieArray;
    };


#endif /* COOKIECLIENTDATA_H_ */

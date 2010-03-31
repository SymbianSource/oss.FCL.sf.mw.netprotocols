/*
 * CCookieGroupDataArray.h
 *
 *  Created on: Nov 24, 2009
 *      Author: mohanti
 */

#ifndef CCOOKIEGROUPDATAARRAY_H_
#define CCOOKIEGROUPDATAARRAY_H_
//System Include

//User Includes

//Forward Declaration
class CCookieGroupData;
class CGroupIdArray;
class CCookieArray;
class CCookie;

/*This class is used to hold an array of CCookieGroupData objects. 
 * 
 */
NONSHARABLE_CLASS( CCookieGroupDataArray ):public CBase
    {
public:

    /* Constructor
     * 
     */
    static CCookieGroupDataArray* NewL();
    
    /* Constructor
     * 
     */
    static CCookieGroupDataArray* NewLC();
        
    /* Destructor
     * 
     */
    ~CCookieGroupDataArray();
    
    
    /*Gets CCookieGroupData object index in the array based on Group Id and appuid
     * @param aGroupId Indicates group id
     * @param aAppUid Indicates appuid
     * @param  aCookieSharable On return of this method it holds the Cookie Sharable 
     *         status for the CCookieGroupData Object
     * @param aIndex On return of this method it holds the Index value of the CCookieGroupData Object
     *               in the array         
     */
    void GetGroupDataIndexL(TUint32 aGroupId,TUint32 aAppUid,TBool& aCookieSharable, TInt& aIndex);
    
    /* Find the Index value of the CCookieGroupData Object based on Group Id
     * @param aGroupId Indicates Group Id
     * @param  aIndex Indicates the Index value
     */
    TInt FindIndex(TUint32 aGroupId,TInt& aIndex );
    
    /*Find the Index value of the CCookieGroupData Object based on Group Id and appuid
     * @param aGroupId Indicates group Id
     * @param aAppUid Indicates appuid
     * @aIndex On return of this method,it indicates the index value of CCookieGroupData object.  
     */
    TInt FindIndex(TUint32 aGroupId,TUint32 aAppUid,TInt& aIndex );
    
    /* Gets the address of CCookieGroupData object from the array based on the index value.
     * @param aIndex Indicates the index value
     */
    CCookieGroupData* GetGroupData(TInt aIndex);
    
    /* Adding CCookieGroupData object in to the array
     * @param address of CCookieGroupData object
    */
    void AddGroupDataL(CCookieGroupData* aCookieGroupData);
    
    /* Getting the address of CCookieGroupData object based on index value
     * 
     */
    CCookieGroupData* At(TInt aIndex) const;
    
    /* Subscripting Operator
     * 
     */
    CCookieGroupData* operator[](TInt aIndex) const;
    
    /* Return the number of CCookieGroupData objects from the array.
     * 
     */
    TInt Count()const;
     
    /* Returns the address of CCookieArray object based on Index Value
     * 
     */
    CCookieArray* CookieArray(TInt aIndex);
    
    /** 
    * Get the relevant cookies for a transaction
    * @param aRequestUri The URI of the current HTTP request.
    * @param aCookies An array which will be filled with the cookies
    * for aTransaction based on the index value.
    * @param aIndex Indiactes Index Value.
    */    
    TInt GetCookies( const TDesC8& aRequestUri,RPointerArray<CCookie>& aCookies,TInt aIndex ) const;
    
    /* Removes CCookieGroupData object from the array based on Index value.
     * @param aIndex Indicates index value.
     */
    void Remove(TInt aIndex);
    
    /* Compreses the Array
     * 
     */
    void Compress();
    
    /* Destroys a CCookieGroupData object from the array based on Group id 
     * 
     */
    TInt DestroyGroupData(TUint32 aGroupId);
    
protected:
private:
    
    /* Constructor
     * 
     */
    void ConstructL();
    
    /*Constructor
     * 
     */
    CCookieGroupDataArray();
public:
    
protected:
    
private:
    RPointerArray<CCookieGroupData>* iCookieGroupData;
    };

#endif /* CCOOKIEGROUPDATAARRAY_H_ */

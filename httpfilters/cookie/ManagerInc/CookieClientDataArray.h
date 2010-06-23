/*
 * CookieClientDataArray.h
 *
 *  Created on: Dec 18, 2009
 *      Author: mohanti
 */

#ifndef COOKIECLIENTDATAARRAY_H_
#define COOKIECLIENTDATAARRAY_H_

//System Includes

//User Includes

//Forward Class Declaration
class CCookieClientData;
class CCookieArray;

//This class holds an array of CCookieClientdata objects.
//This a singleton class and is allocated in the Thread local storage(TLS)

NONSHARABLE_CLASS( CCookieClientDataArray ):public CBase
    {
public:
    
    /* Constructor 
     * 
     */

     static CCookieClientDataArray* New();

	/* Initialization
     *
	 */
	void Init();
         
    ~CCookieClientDataArray();
    
    /* Constructor
     * 
     */
    CCookieClientDataArray();

    /* Finds the address of CCookieClientData object from the array based on group id and appuid
     * @param aGroupId Indicates group id
     * @param aWidgetUid Indicates appuid
     */
    CCookieClientData*  Find(const TUint32& aGroupId,const TUint32& aWidgetUid);
    
    /* Finds the Index of CCookieClientData object from the array based on group id and appuid
     * @param aGroupId Indicates Group Id 
     * @param aWidgetUid Indicates Appuid
     * @param aIndex On return of the method,it indicates the index no where the intended 
     *        CCookieClientData Object is present
     */
    TInt FindIndex(const TUint32& aGroupId, const TUint32& aWidgetUid,TInt& aIndex );

    /* Adding CCookieClientData object in to the array.
     * @param aCookieClientData Indiactes the address of CCookieClientData Object
     */
    void AddClientGroupDataL(CCookieClientData* aCookieClientData);
    
    /* Returns the CCookieClientData object from the array based on index no.
     * @param aIndex Indiactes index no.
     */
    CCookieClientData* At(TInt aIndex) const;
    
    /* Subscripting operator
     * 
     */
    CCookieClientData* operator[](TInt aIndex) const;
    
    /* Returns the no of CCookieClientData object in the array
     * 
     */
    TInt Count()const;
    
    /* Returns the cookiearray from the array based on the index no.
     * @param aIndex indeicates the index no
     */
    CCookieArray* CookieArray(TInt aIndex); 
    
    /* Removes a CCookieClientData object from the array based on index no.
     * 
     */
    void Remove(TInt aIndex);
    
    /* Compresses the array
     * 
     */
    void Compress();
    
    /* Deletes a CCookieClientData object from the array based on appuid 
     * @param aAppUid Indicates appuid of a Secure Widget.
     */
    TInt DestroyClientData(const TUint32 aAppUid);
protected:
    
private:

    /* Constructor
     * 
     */
    CCookieClientDataArray(const CCookieClientDataArray&);
    
    /* Assignment operator
     * 
     */
    CCookieClientDataArray& operator=(const CCookieClientDataArray&);
public:
    
protected:
    
private:
    RPointerArray<CCookieClientData>* iCookieClientData;
    };


#endif /* COOKIECLIENTDATAARRAY_H_ */

/*
 * CGroupIdInfoArray.h
 *
 *  Created on: Nov 24, 2009
 *      Author: mohanti
 */

#ifndef GROUPIDINFOARRAY_H_
#define GROUPIDINFOARRAY_H_
//System Includes
#include <e32base.h>
#include <e32def.h>
#include <e32cmn.h>
//User Includes

//Constants

/*This class is used to hold group related info present in the xml file
 * 
 */
NONSHARABLE_CLASS( CGroupIdInfo ):public CBase
    {
  public:
      /*Constructor
       * 
       */
    static CGroupIdInfo* NewL(TUint32 aGroupId, const RArray<TUint32>& aAppuid, TBool aCookieSharableFlag = EFalse );
    
    /*Constructor
     * 
     */
    static CGroupIdInfo* NewLC(TUint32 aGroupId, const RArray<TUint32>& aAppuid, TBool aCookieSharableFlag = EFalse );
    
    /*Standard Constructor
     * 
     */
    CGroupIdInfo( TUint32 aGroupId,TBool aCookieSharableFlag = EFalse );
    
    /*Standard Destructor
     * 
     */
    ~CGroupIdInfo();
    
    /*
     * 
     */
    TInt GetGroupName(TUint32 aSecureId,TUint32& aGroupId);
     
    /*
     * 
     */
    void AddClientL(TUint32 aClientId);
    
    /*
     * 
     */
    TUint32 At(TInt aIndex) const;
    
    /*
     * 
     */
    TUint32 operator[](TInt aIndex) const;
    
    /*
     * 
     */
    const TUint32& GetGroupId();
    TBool GetCookieSharableFlag();
    TBool IsThisPartOfSharedGroup( TUint32& aClientId );
  protected :
  private:
       
      /*
       * 
       */
      void ConstructL(const RArray<TUint32>& aAppuid);
  public:    
  protected:
      
  private:
    TBool iCookieSharableFlag;  
    TUint32 iGroupId;
    //TUint32 iSecureId;
    RArray<TUint32> iAppUid;//List of application uid with which Group Info is shared
    };

/*This class is used to hold an array of CGroupIdInfo objects
 * 
 */

NONSHARABLE_CLASS( CGroupIdArray ): public CBase
    {
   public:
       static CGroupIdArray* NewL();
       static CGroupIdArray* NewLC();
       void ConstructL();
       ~CGroupIdArray();
   protected:
   private:
       CGroupIdArray();
   public:
       TInt Count();
       CGroupIdInfo& At(TInt aIndex) const;
       CGroupIdInfo& operator[](TInt aIndex) const;
       void AddGroupIdL(CGroupIdInfo* aCGroupIdInfo);
       TInt GetGroupId(TUint32 aClientId,TUint32& aGroupId);
       TInt GetGroupId(TUint32 aClientId,TUint32& aGroupId,TBool& aCookieSharableFlag);
   protected:    
   private:
   RPointerArray<CGroupIdInfo>* iGroupIdInfoArray;     
    };

#endif /* GROUPIDINFOARRAY_H_ */

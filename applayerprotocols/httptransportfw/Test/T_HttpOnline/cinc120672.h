// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
//

#ifndef __CINC120672_H__
#define __CINC120672_H__

#include <e32base.h>
#include <http.h>
#include <http/mhttpauthenticationcallback.h>
#include "HttpTestCore.h"


class CHTTPTestUtils;
class CScriptFile;
class CHttpTimer1;

class MTestClient1
   {
public:     
   virtual void Timedout()=0;	   
   };

/*	
Test case for defect fix INC120672. 
*/

class CINC120672_1 :public CHttpTestTransBase, public MHTTPTransactionCallback,
                    public MHTTPAuthenticationCallback,public MTestClient1
	{
public:
	static CINC120672_1* NewL(TInt aTestNumber, CScriptFile* aIniSettingsFile);
	virtual ~CINC120672_1();
	void MHFRunL(RHTTPTransaction aTransaction,const THTTPEvent& aEvent) ;
	virtual void Timedout();
	TInt MHFRunError(TInt aError, RHTTPTransaction aTransaction,
							 const THTTPEvent& aEvent) ;
	// method inherited from MHTTPAuthenticationCallback
	virtual TBool GetCredentialsL(const TUriC8& aURI, RString aRealm, 
								 RStringF aAuthenticationType,
								 RString& aUsername, 
								 RString& aPassword);							 

private:
	CINC120672_1(TInt aTestNumber, CScriptFile* aIniSettingsFile);
	const TDesC& TestName();
	virtual void DoRunL();
	virtual TInt RunError(TInt aErr);	
	virtual void DoCancel();
		
private:
	/** This has the instance of the settings.ini file.  This instance is passed
	 to ReplaceHostNameL() function which uses this instance to retrieve the
	 data from settings.ini file.
	 */
	CScriptFile* iIniSettingsFile;
		
protected:

    TInt iOpenTransactionCount;
    TInt iFailureError;
    TInt iTestNumber;
 
	};
	
	
class CINC120672_2 : public CHttpTestTransBase,public MHTTPTransactionCallback,
                     public MHTTPAuthenticationCallback,public MTestClient1
     {
public:
     static CINC120672_2* NewL(TInt aTestNumber, CScriptFile* aIniSettingsFile);
     virtual ~CINC120672_2();
     void MHFRunL(RHTTPTransaction aTransaction,const THTTPEvent& aEvent) ;
     virtual void Timedout();
     TInt MHFRunError(TInt aError, RHTTPTransaction aTransaction,const THTTPEvent& aEvent) ;
     // method inherited from MHTTPAuthenticationCallback
     virtual TBool GetCredentialsL(const TUriC8& aURI, RString aRealm, 
			                       RStringF aAuthenticationType,
			                       RString& aUsername, 
			                       RString& aPassword);							 

private:	 
      CINC120672_2(TInt aTestNumber, CScriptFile* aIniSettingsFile);
	  const TDesC& TestName();
	  virtual void DoRunL();
	  virtual TInt RunError(TInt aErr);	
	  virtual void DoCancel();
	 
private:
	   /**This has the instance of the settings.ini file.  This instance is passed
	    to ReplaceHostNameL() function which uses this instance to retrieve the
		data from settings.ini file.
	    */			
	 CScriptFile* iIniSettingsFile;


protected:

			TInt iOpenTransactionCount;
			TInt iFailureError;
			TInt iTestNumber;

};	

class CHttpTimer1: public CActive
	{
public:
	    CHttpTimer1(MTestClient1& aClient);
		~CHttpTimer1();
		void After(TTimeIntervalMicroSeconds32 anInterval);
		void DoCancel();
		void RunL();
		
private:
	    RTimer iTimer;
		MTestClient1& iClient;	
	};

	
#endif // __CINC120672_H__

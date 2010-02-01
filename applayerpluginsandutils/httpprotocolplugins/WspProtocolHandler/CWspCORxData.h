// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __CWSPCORXDATA_H__
#define __CWSPCORXDATA_H__

// System includes
#include <e32base.h>
#include <http/framework/crxdata.h>

// Forward declarations
class MWspCORxDataCallback;


//##ModelId=3BBD80690108
class CWspCORxData : public CRxData
	{
public:	// Methods

/**	@fn				NewL(CProtTransaction& aProtTrans, MRxDataObserver& aObserver, MWspCORxDataCallback&	aMethodCallback)
	Intended Usage	:	Static factory constructor.
	@since			7.0
	@leave			Leaves with KErrNoMemory if there was not enough memory to
					create the object.
	@param			aTransaction		The protocol transaction object associated
										with the Rx data object.
	@param			aObserver			A reference to the Rx data observer 
										object.
	@param			aMethodCallback		A reference to the callback object.
	@return			A pointer to newly created object.
*/
	//##ModelId=3C4C41AD03E3
	static CWspCORxData* NewL(
							 CProtTransaction&		aTrasaction, 
							 MRxDataObserver&		aObserver, 
							 MWspCORxDataCallback&	aMethodCallback
							 );

/**	@fn				~CWspCORxData()
	Intended Usage	:	Standard destructor.
	@since			7.0
*/	
	//##ModelId=3C4C41AD03D9
	~CWspCORxData();

/**	@fn				SetResponseDataL(const TDesC8& aResponseHeaders, MHTTPDataSupplier& aResponseBody)
	Intended Usage	:	Sets the header and body data for the request response. 
						The header data is segmented into the individual header 
						fields and set into the response object's headers. 
						
						The client is informed using the EGotResponseHeaders 
						event. The raw field value is not decoded. This is left
						for when the field value is required. 
						
						If there is any body data then the client is informed 
						using the EGotResponseBodyData event. If there is no 
						body data and no more primitives are expected, the 
						RxData observer is sent the EResponseComplete status.

						The more data flag that indicates if there is more data
						to be received for this transaction in subsequent 
						S-MethodResultData primitives to follow.							
	@since			7.0
	@leave			The function will leave if setting the header data or body 
					data fails.
	@param			aResponseHeaders	A reference to a buffer with the the 
										header data.
	@param			aResponseBody		A reference to the data supplier object
										for the body data.
	@param			aMoreResponseData	A flag indicating that there is more data
										to be received for this transaction in 
										subsequent S-MethodResultData primitives 
										to follow.										.
	@pre			A S-MethodResult primitive has been received.
	@post			The data supplier for thebody data is stored. The header 
					data has been segmented into the individual header fields 
					and set in the response object.
*/
	//##ModelId=3C4C41AD0392
	void SetResponseDataL(const TDesC8& aResponseHeaders, MHTTPDataSupplier& aResponseBody, TBool aMoreResponseData);

/**	@fn				UpdateResponseDataL(const TDesC8& aTrailerHeaders, TBool aMoreResponseData)
	Intended Usage	:	Fetches any extra body data from the stored data 
						supplier. Trailer header data is only allowed if this is
						the last S-MethodResultData primitive. 
						
						If this is the last primitive and there is some header 
						data, this is segmented into the individual header 
						fields and added to the response object's headers. The 
						raw field value is not decoded. This is left for when 
						the field value is required. 

						The client is sent the EGotResponseTrailerHeaders event.
					
						If there is any extra body data and the client has 
						already released the previous batch of body data, then 
						the EGotResponseBodyData event is sent.

						The more data flag that indicates if there is more data
						to be received for this transaction in subsequent 
						S-MethodResultData primitives to follow.							
	@since			7.0
	@leave			The function will leave with KWspErrNotExpectingTrailers if 
					there is trailer header data but more S-MethodResulData 
					primitives are expected. Also, it will leave if it fails to 
					update the body data.
	@param			aTrailerHeaders		A reference to a buffer with the encoded 
										trailer headers. This must be empty 
										unless this is the last S-MethodResultData
										primitive.
	@param			aMoreResponseData	A flag indicating that there is more data
										to be received for this transaction in 
										subsequent S-MethodResultData primitives 
										to follow.										.
	@pre			A S-MethodResultData primitive has been received.
	@post			Any trailer header data is segmented into the individual 
					header fields and added to the response object's headers.
					The client is notified of more body data if there is any
					and the previous batch of data has been released.
*/
	//##ModelId=3C4C41AD036A
	void UpdateResponseDataL(const TDesC8& aTrailerHeaders, TBool aMoreResponseData);

private:	// Methods from CRxData

/**	@fn				void ResetRxData()
	@see			CRxData
*/
	//##ModelId=3C4C41AD034C
	virtual void ResetRxData(); 

private:	// Methods from MHTTPDataSupplier

/**	@fn				GetNextDataPart(TPtrC8& aDataPart)
	@see			MHTTPDataSupplier
*/
	//##ModelId=3C4C41AD0324
	virtual TBool GetNextDataPart(TPtrC8& aDataPart);

/**	@fn				ReleaseData()
	@see			MHTTPDataSupplier
*/
	//##ModelId=3C4C41AD0310
	virtual void ReleaseData();

/**	@fn				OverallDataSize()
	@see			MHTTPDataSupplier
*/
	//##ModelId=3C4C41AD02FC
	virtual TInt OverallDataSize();

/**	@fn				Reset()
	@see			MHTTPDataSupplier
*/
	//##ModelId=3C4C41AD02E8
	virtual TInt Reset();

private:	// Methods

/**	@fn				CWspCORxData(CProtTransaction& aTransaction, MRxDataObserver& aObserver, MWspCORxDataCallback&	aMethodCallback)
	Intended Usage	:	First phase constructor.
	@since			7.0
	@param			aTransaction		The protocol transaction object associated
										with the Rx data object.
	@param			aObserver			A reference to the Rx data observer 
										object.
	@param			aMethodCallback		A reference to the callback object.
*/
	//##ModelId=3C4C41AD02AC
	CWspCORxData(
				CProtTransaction&		aTransaction, 
				MRxDataObserver&		aObserver, 
				MWspCORxDataCallback&	aMethodCallback
				);

/**	@fn				SetHeaderDataL(const TDesC8& aResponseHeaders)
	Intended Usage	:	
	@since			7.0
	@leave			
	@param			aResponseHeaders	A reference a buffer with the header
										data.
	@pre			A S-MethodResult primitive has been received.
	@post			The trailer header data has been segmented into the 
					individual header fields and set in the response object.
*/
	//##ModelId=3C4C41AD0298
	void SetHeaderDataL(const TDesC8& aResponseHeaders);

/**	@fn				SetBodyDataL(MHTTPDataSupplier& aResponseBody)
	Intended Usage	:	
	@since			7.0
	@leave			
	@param			aResponseBody	A reference to the data supplier object for
									the body data.
	@pre			A S-MethodResult primitive has been received. The 
					EGotResponseHeaders event has been sent.
	@post			The body data supplier is stored.
*/
	//##ModelId=3C4C41AD027A
	void SetBodyDataL(MHTTPDataSupplier& aResponseBody);

/**	@fn				UpdateHeaderDataL(const TDesC8& aTrailerHeaders)
	Intended Usage	:	
	@since			7.0
	@leave			
	@param			aTrailerHeaders	A reference to a buffer with the encoded 
									trailer headers. This must be empty unless
									this is the last S-MethodResultData	
									primitive.
	@pre			A S-MethodResultData primitive has been received.
	@post			Any extra header data is segmented into the individual 
					header fields and added to the response object's headers.
*/
	//##ModelId=3C4C41AD0266
	void UpdateHeaderDataL(const TDesC8& aTrailerHeaders);

/**	@fn				UpdateBodyDataL()
	Intended Usage	:	
	@since			7.0
	@leave			
	@pre			A S-MethodResultData primitive has been received.
	@post			Any extra body data is appended to the buffer.
*/
	//##ModelId=3C4C41AD025C
	void UpdateBodyDataL();

/**	@fn				ResponseCompleteL()
	Informs the observer that the response is complete. Also, if there are 
	trailer headers, the client is sent the EGotResponseTrailerHeaders event.
	@since			7.0
	@pre			All the response data has been received and sent to the client.
	@post			The observer has been told that the response is complete. If 
					trailer headers have been received, the client is sent the
					EGotResponseTrailerHeaders event.
*/
	void ResponseCompleteL();

private:	// Attributes

	/** A reference to the method callback object.
	*/
	//##ModelId=3C4C41AD0252
	MWspCORxDataCallback&	iMethodCallback;

	/** A pointer to the data supplier for the body data - not owned.
	*/
	//##ModelId=3C4C41AD0240
	MHTTPDataSupplier*		iBodyDataSupplier;

	/** A flag that indicates whether subsequent S-MethodResultData primitives 
		are expected for the response.
	*/
	//##ModelId=3C4C41AD0236
	TBool					iMoreData;

	/** A flag to indicate whether there are more data chunks to be obtained from
		the data supplier object for the current SDU.
	*/
	TBool					iLastChunk;

	/** A flag that indicates whether there is a current response body data part.
	*/
	//##ModelId=3C4C41AD022A
	TBool					iGotBodyData;

	/** A descriptor pointer to a buffer containing the current reponse body 
		data part.
	*/
	//##ModelId=3C4C41AD0204
	TPtrC8					iBodyData;

	/** The overall size of the body data. If this is not known, e.g. the 
		response is divided over several primitives, then this is set to 
		KErrNotFound.
	*/
	//##ModelId=3C4C41AD01F8
	TInt					iOverallDataSize;

	/** A flag that indicates the there are trailer headers.
	*/
	TBool					iHasTrailer;

	};

#endif	// __CWSPCORXDATA_H__

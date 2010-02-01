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

#ifndef __CWSPCOTXDATA_H__
#define __CWSPCOTXDATA_H__

// System includes
#include <e32base.h>
#include <http/framework/ctxdata.h>

// Forward declarations
class MWspCOTxDataCallback;
class MWspCapabilityViewer;
class MHTTPDataSupplier;

// @todo docing

//##ModelId=3BBD80700021
class CWspCOTxData : public CTxData
	{
public:	// Methods

/**	@fn				NewL(CProtTransaction& aTransaction, MWspCOTxDataCallback& aObserver, MWspCapabilityViewer& aNegotiatedCapInfo)
	Intended Usage	:	Static factory constructor.
	@since			7.0
	@leave			Leaves with KErrNoMemory if there was not enough memory to
					create the object.
	@param			aTransaction		The protocol transaction object 
										associated with the Tx data object.
	@param			aObserver			A reference to the Tx data observer
	@param			aNegotiatedCapInfo	A reference to the negotiated 
										capability viewer object.
	@return			A pointer to newly created object.
*/
	//##ModelId=3C4C41A801D3
	static CWspCOTxData* NewL(
							 CProtTransaction&		aTransaction, 
							 MWspCOTxDataCallback&	aObserver, 
							 MWspCapabilityViewer&	aNegotiatedCapInfo
							 );

/**	@fn				~CWspCOTxData()
	Intended Usage	:	Standard destructor.
	@since			7.0
*/
	//##ModelId=3C4C41A801C9
	~CWspCOTxData();

/**	@fn				RequestHeadersData()
	Intended Usage	:	Provides a buffer with the request headers.
	@since			7.0
	@return			A const reference to a buffer that contains the	request 
					headers.
*/
	//##ModelId=3C4C41A801B5
	const TDesC8& RequestHeadersData();

/**	@fn				RequestBodyData()
	Intended Usage	:	Provides the data supplier object for the request 
						body data. This data supplier supplies the more data
						flag via the return from GetNextDataPart().
	@since			7.0
	@return			A reference to a MHTTPDataSupplier object that provides the
					request body.
*/
	//##ModelId=3C4C41A801AB
	MHTTPDataSupplier& RequestBodyData();

/**	@fn				SetRequestDataL()
	Intended Usage	:
	@since			7.0
	@todo			finish docing
*/
	//##ModelId=3C4C41A80197
	void SetRequestDataL();

/**	@fn				ReceivedCnf()
	The cnf for the current invoke primitive has been received. If there is more
	data to send, then the observer is asked to send an S-MethodInvokeData 
	primitive. If there is no more data to send and the request is not complete
	then wait for the client to send more data.
	@since			7.0
	@pre			The state is either EDone or EGotMoreData.
	@post			The has changed to EIdle if the originally EDone. This 
					indicates that the request is complete. If the state was
					EGotMoreData then the state is EDone if the remaining data
					can be sent in the next SDU, EGotMoreData if the remaining
					data cannot be all sent in the next SDU, or EWaitForData if
					the request is not complete and remaining data does not fill
					the next SDU.
*/
	void ReceivedCnf();

/**	@fn				NotifyMoreRequestData()
	Intended Usage	:	The client has got more request data. 
	@since			7.0
	@post			The fact that the client has nore request data has been flagged.
*/
	//##ModelId=3C4C41A80179
	void NotifyMoreRequestData();
	
private:	// Methods from MHTTPDataSupplier

/**	@fn				GetNextDataPart(TPtrC8& aDataPart)
	@see			MHTTPDataSupplier
*/
	//##ModelId=3C4C41A8013C
	virtual TBool GetNextDataPart(TPtrC8& aDataPart);

/**	@fn				ReleaseData()
	@see			MHTTPDataSupplier
*/
	//##ModelId=3C4C41A80128
	virtual void ReleaseData();

/**	@fn				OverallDataSize()
	@see			MHTTPDataSupplier
*/
	//##ModelId=3C4C41A80114
	virtual TInt OverallDataSize();

/**	@fn				Reset()
	@see			MHTTPDataSupplier
*/
	//##ModelId=3C4C41A80100
	virtual TInt Reset();

private:	// Methods

/**	@fn				CWspCOTxData(CProtTransaction& aTransaction, MWspCOTxDataCallback& aObserver, MWspCapabilityViewer& aNegotiatedCapInfo)
	Intended Usage	:	First phase constructor.
	@since			7.0
	@param			aTransaction		The protocol transaction object 
										associated with the Tx data object.
	@param			aObserver			A reference to the Tx observer.
	@param			aNegotiatedCapInfo	A reference to the negotiated 
										capability viewer object.
*/
	//##ModelId=3C4C41A8009C
	CWspCOTxData(
				CProtTransaction&		aTransaction, 
				MWspCOTxDataCallback&	aObserver, 
				MWspCapabilityViewer&	aNegotiatedCapInfo
				);

/**	@fn				SetSessionPropertyL(TInt aProperty, THTTPHdrVal aFieldValue)
	Intended Usage	:
	@since			7.0
	@todo			docing
	@param			aProperty		An integer specifying the property to be 
									set.
	@param			aFieldValue		The value the property is to be set to.
	@post			The property has been set to the specified value/
*/
	//##ModelId=3C4C41A80056
	void SetSessionPropertyL(TInt aProperty, THTTPHdrVal aFieldValue);

/**	@fn				SetHeaderDataL(RHTTPRequest aRequest)
	Intended Usage	:
	@since			7.0
	@todo			finish docing
	@param			aRequest	The request object.
	@pre
	@post
*/
	//##ModelId=3C4C41A8002E
	void SetHeaderDataL(RHTTPRequest aRequest);

/**	@fn				EncodeHeadersL(RHTTPHeaders aHeaders)
	Intended Usage	:
	@since			7.0
	@todo			finish docing
	@param			aHeaders	The headers object
	@pre
	@post
*/
	//##ModelId=3C4C41A80006
	void EncodeHeadersL(RHTTPHeaders aHeaders);

/**	@fn				SetBodyDataL(MHTTPDataSupplier& aRequestBody)
	Intended Usage	:
	@since			7.0
	@todo			finish docing
	@param			aRequestBody	The data supplier for the request body data.
	@pre
	@post
*/
	//##ModelId=3C4C41A703D0
	void SetBodyDataL(MHTTPDataSupplier& aRequestBody);

/**	@fn				UpdateBodyData()
	Intended Usage	:
	@since			7.0
	@todo			finish docing
	@pre
	@post
*/
	//##ModelId=3C4C41A703C6
	void UpdateBodyData();

/**	@fn				CalculateUIntVarLength(TUint32 aUint)
	Intended Usage	:	A utility function to calculate how many bytes are
						required to represent a number in UIntVar format, as
						defined by the WSP Specification, July 2001.
	@since			7.0
	@param			aUint	The unsigned positive number for which to calculate
							the length of its UIntVar representation.
	@return			The number of bytes required to represent the input integer
					as a UIntVar.
*/
	//##ModelId=3C4C41A70394
	TInt CalculateUIntVarLength(TUint32 aUint);

/**	@fn				UpdateState()

	@since			7.0
*/
	void UpdateState();

/** @fn				CheckTrailer()

	@since			7.0
*/
	void CheckTrailer();

/**	@fn				DecideObserverAction()

	@since			7.0
*/
	void DecideObserverAction();

/**	@fn				Cleanup()

	@since			7.0
*/
	void Cleanup();

private:	// Enums

	/** The TRequestState type represents the states of the 
		request data object.
	*/
	enum TRequestState
		{
		/**
		*/
		EIdle					= 0,
		/**
		*/
		EDone,
		/**
		*/
		EGotMoreData,
		/**
		*/
		EWaitForMoreData,
		/**
		*/
		EAbortMethod
		};

private:	// Attributes

	/** The state of the request data.
	*/
	TRequestState			iState;

	/**	A reference to Tx data callback object.
	*/
	//##ModelId=3C4C41A7038A
	MWspCOTxDataCallback&	iObserver;

	/** A reference to the negotiated capability viewer.
	*/
	//##ModelId=3C4C41A70376
	MWspCapabilityViewer&	iNegotiatedCapInfo;

	/** A pointer to the request body data supplier. Not owned.
	*/
	//##ModelId=3C4C41A70362
	MHTTPDataSupplier*		iBodyDataSupplier;

	/** A buffer with the encoded request header data.
	*/
	//##ModelId=3C4C41A70350
	HBufC8*					iHeaderData;

	/** A buffer with the encoded request trailer header data.
	*/
	//##ModelId=3C4C41A70344
	HBufC8*					iTrailerData;

	/** A buffer with the current request data.
	*/
	HBufC8*					iBodyData;

	/** A flag indicating that there are trailer header data.
	*/
	//##ModelId=3C4C41A70312
	TBool					iHasTrailer;

	/** A flag that indicates whether all the request data has been received 
		from the client.
	*/
	//##ModelId=3C4C41A702F4
	TBool					iRequestComplete;

	/** A flag that indicates whether there is a current request body data part.
	*/
	//##ModelId=3C4C41A702D6
	TBool					iGotBodyData;

	/** A descriptor pointer to a buffer containing the current request body 
		data part.
	*/
	//##ModelId=3C4C41A702C4
	TPtrC8					iDataPart;

	/** The overall size of the body data. If this is not known, e.g. the 
		resquest is divided over several primitives, then this is set to 
		KErrNotFound.
	*/
	//##ModelId=3C4C41A702B8
	TInt					iOverallDataSize;

	/** The remaining server SDU space for the current primitive.
	*/
	//##ModelId=3C4C41A702A6
	TUint32					iRemainingSDUSpace;

	/** The remaining server message space.
	*/
	//##ModelId=3C4C41A7029A
	TUint32					iRemainingMessageSpace;

	/** A flag to indicate whether LDT is supported.
	*/
	//##ModelId=3C4C41A70287
	TBool					iSupportLDT;

	/** The length of the trailer header info.
	*/
	TUint32					iTrailerInfoLength;

	/** A flag to indicate wether the Server Message is limited.
	*/
	TBool					iLimitedMessageSize;

	};

#endif	// __CWSPCOTXDATA_H__

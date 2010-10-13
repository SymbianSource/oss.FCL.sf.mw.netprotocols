// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __CHTTPTRANSPORTLAYER_H__
#define __CHTTPTRANSPORTLAYER_H__

#include <e32base.h>
#include "msocketfactory.h"

class MConnectionPrefsProvider;

class THttpTransportConstructionParams
/**
This class is defines the construction parameters required by the 
CHttpTransportLayer ECom interface.
@see		CHttpTransportLayer
*/
	{
public: // Methods

	inline THttpTransportConstructionParams(MConnectionPrefsProvider& aConnectionPrefsProvider);	

public:	//	attributes

	/**	Reference to a comms connection preferences provider 
	*/
	MConnectionPrefsProvider& iConnectionPrefsProvider;
	
	TBool iPriority;

	};

class CHttpTransportLayer : public CBase,
							public MSocketFactory
/**
This is the ECOM interface class for the http transport layer. 
Implementations are identified by the underlying transport protocol they use.
An implementation of this class needs to provide implementations for the 
MSocketFactory API, the MInputStream API and the MOutputStream API.
@see		MSocketFactory
@see		MInputStream
@see		MOutputStream
*/
	{
public:	// methods

	inline static CHttpTransportLayer* NewL(const TDesC8& aTransportType, THttpTransportConstructionParams& aHttpTransportParams);
	
	inline virtual ~CHttpTransportLayer();
	
	inline MSocketFactory& SocketFactory();
	
protected:	// methods

	inline CHttpTransportLayer();

private:	// methods

/**	
	Reserved function for future expansion.
*/
	virtual void CHttpTransportLayer_Reserved() =0;

private:	// attributes

/** D'tor key identification required for ECOM
*/
	TUid	iDtor_ID_Key;

	};
	
#include "chttptransportlayer.inl"

#endif	// __CHTTPTRANSPORTLAYER_H__


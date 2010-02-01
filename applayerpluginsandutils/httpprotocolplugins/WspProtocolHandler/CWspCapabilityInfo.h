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

#ifndef __CWSPCAPABILITYINFO_H__
#define __CWSPCAPABILITYINFO_H__

#include <e32base.h>
#include <wsp/mwspcapabilityviewer.h>
#include <wsp/mwspcapabilitysetter.h>

class CWspAliasAddresses;
class CWspExtendedMethods;
class CWspHeaderCodePages;
class CWspUnknownCapabilities;

// Types used for arguments in this file (e.g. TWspBearer) are from the Wap namespace
using namespace Wap;

/** The CWspCapabilityInfo class implements the MWspCapabilityViewer and 
	MWspCapabilitySetter APIs.
	@see			MWspCapabilityViewer
	@see			MWspCapabilitySetter
*/
//##ModelId=3C4C419F0099
class CWspCapabilityInfo : public CBase, public MWspCapabilityViewer,
										 public MWspCapabilitySetter
	{
public:	// Methods

/** Static factory function.
	@since			7.0
	@leave			This function will leave if the object fails to be created.
	@return			A pointer to created CWspCapabilityInfo object. 
*/
	//##ModelId=3C4C419F0252
	static CWspCapabilityInfo* NewL();

/** Destructor.
	@since			7.0
*/
	//##ModelId=3C4C419F0249
	~CWspCapabilityInfo();

public:	// Methods from MWspCapabilityViewer

/** 
	@see			MWspCapabilityViewer
*/
	//##ModelId=3C4C419F0248
	virtual const MWspAliasAddresses& GetAliasAddresses() const;

/** 
	@see			MWspCapabilityViewer
*/
	//##ModelId=3C4C419F0240
	virtual const MWspExtendedMethods& GetExtendedMethods() const;

/** 
	@see			MWspCapabilityViewer
*/
	//##ModelId=3C4C419F023F
	virtual const MWspHeaderCodePages& GetHeaderCodePages() const;

/** 
	@see			MWspCapabilityViewer
*/
	//##ModelId=3C4C419F023E
	virtual TUint8 GetMethodMOR() const;

/** 
	@see			MWspCapabilityViewer
*/
	//##ModelId=3C4C419F0235
	virtual TUint8 GetPushMOR() const;

/** 
	@see			MWspCapabilityViewer
*/
	//##ModelId=3C4C419F0234
	virtual TUint8 GetProtocolOptions() const;

/** 
	@see			MWspCapabilityViewer
*/
	//##ModelId=3C4C419F022B
	virtual TUint32 GetClientSDUSize() const;

/** 
	@see			MWspCapabilityViewer
*/
	//##ModelId=3C4C419F022A
	virtual TUint32 GetServerSDUSize() const;

/** 
	@see			MWspCapabilityViewer
*/
	//##ModelId=3C4C419F0221
	virtual TUint32 GetClientMessageSize() const;

/** 
	@see			MWspCapabilityViewer
*/
	//##ModelId=3C4C419F0220
	virtual TUint32 GetServerMessageSize() const;

/** 
	@see			MWspCapabilityViewer
*/
	//##ModelId=3C4C419F0216
	virtual const MWspUnknownCapabilities& GetUnknownCapabilities() const;

public:	// Methods from MWspCapabilitySetter

/** 
	@see			MWspCapabilitySetter
*/
	//##ModelId=3C4C419F020C
	virtual void Reset(TWspCapability aCapability = EAllCapabilities);

/** 
	@see			MWspCapabilitySetter
*/
	//##ModelId=3C4C419F01E4
	virtual void AddAliasAddressL(TWspBearer aBearer, TUint16 aPort, const TDesC8& aAddress);

/** 
	@see			MWspCapabilitySetter
*/
	//##ModelId=3C4C419F01D0
	virtual void AddExtendedMethodL(TUint8 aPDUType, const TDesC8& aMethodName);

/** 
	@see			MWspCapabilitySetter
*/
	//##ModelId=3C4C419F01B2
	virtual void AddHeaderCodePageL(TUint8 aPageCode, const TDesC8& aPageName);

/** 
	@see			MWspCapabilitySetter
*/
	//##ModelId=3C4C419F01A8
	virtual void SetMethodMOR(TUint8 aMOR);

/** 
	@see			MWspCapabilitySetter
*/
	//##ModelId=3C4C419F019E
	virtual void SetPushMOR(TUint8 aMOR);

/** 
	@see			MWspCapabilitySetter
*/
	//##ModelId=3C4C419F018A
	virtual void SetProtocolOptions(TUint8 aProtocolOptions);

/** 
	@see			MWspCapabilitySetter
*/
	//##ModelId=3C4C419F0180
	virtual void SetClientSDUSize(TUint32 aSize);

/** 
	@see			MWspCapabilitySetter
*/
	//##ModelId=3C4C419F016C
	virtual void SetServerSDUSize(TUint32 aSize);

/** 
	@see			MWspCapabilitySetter
*/
	//##ModelId=3C4C419F0162
	virtual void SetClientMessageSize(TUint32 aSize);

/** 
	@see			MWspCapabilitySetter
*/
	//##ModelId=3C4C419F0158
	virtual void SetServerMessageSize(TUint32 aSize);

/** 
	@see			MWspCapabilitySetter
*/
	//##ModelId=3C4C419F013B
	virtual void AddUnknownCapabilityL(const TDesC8& aIdentifier, const TDesC8& aValue);

private:	// Methods

/** 
	Default constructor.
	@since			7.0
*/
	//##ModelId=3C4C419F013A
	CWspCapabilityInfo();

/**Second phase construction function. Does any required
						allocations needed to initialise object.
	@since			7.0
*/
	//##ModelId=3C4C419F0139
	void ConstructL();

private:	// Attributes

	/** The set of alias addresses.
	*/
	//##ModelId=3C4C419F012F
	CWspAliasAddresses*			iAliasAddresses;

	/** The set of extended methods.
	*/
	//##ModelId=3C4C419F011D
	CWspExtendedMethods*		iExtendedMethods;

	/** The set of header code pages.
	*/
	//##ModelId=3C4C419F0111
	CWspHeaderCodePages*		iHeaderCodePages;

	/** The method MOR value.
	*/
	//##ModelId=3C4C419F0107
	TUint8						iMethodMOR;

	/** The push MOR value.
	*/
	//##ModelId=3C4C419F00F5
	TUint8						iPushMOR;

	/** The set of protocol options.
	*/
	//##ModelId=3C4C419F00EB
	TUint8						iProtocolOptions;

	/** The Client SDU size.
	*/
	//##ModelId=3C4C419F00DF
	TUint32						iClientSDUSize;

	/** The Server SDU size.
	*/
	//##ModelId=3C4C419F00CD
	TUint32						iServerSDUSize;

	/** The Client message size.
	*/
	//##ModelId=3C4C419F00C3
	TUint32						iClientMessageSize;

	/** The Server message size.
	*/
	//##ModelId=3C4C419F00B9
	TUint32						iServerMessageSize;

	/** The set of unknown capabilities.
	*/
	//##ModelId=3C4C419F00B0
	CWspUnknownCapabilities*	iUnknownCapabilities;

	};

#endif	// __CWSPCAPABILITYINFO_H__

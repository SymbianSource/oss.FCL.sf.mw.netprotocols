// Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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

/**
 @file CNwssTransLookUpTable.h
 @warning : This file contains Rose Model ID comments - please do not delete
*/

#ifndef	__CNWSSTRANSLOOKUPTABLE_H__
#define __CNWSSTRANSLOOKUPTABLE_H__

// System includes
#include <e32base.h>
#include <wapcli.h>


// Forward declarations
class MWspCOMethodCallback;
class MHTTPDataSupplier;
class CNwssWspTrHndDataSupplier;


/**
	@since			7.0
	This class implements a simple, pre-allocated look-up table that is used in
	the WSP Transport Handler to associate the transaction-related items stored in
	instances of the embedded class CNwssTransLUTEntry.
 */

//##ModelId=3BBD8D0600BB
class CNwssTransLookUpTable : public CBase
	{
public: // embedded classes

	/**
		@since			7.0
		This class implements a single entry in the transaction look-up table.  It is
		an association of the following five items :
		* NWSS Wap Stack transaction handler (RWSPCOTrans)
		* NWSS Wap Stack transaction ID (RWSPCOTrans::TTransID) - for convenience
		* Client transaction callback (MWspCOMethodCallback)
		* Client's request body data supplier (MHTTPDataSupplier)
		* WSP Transport Handler response body handler (CNwssWspTrHndDataSupplier)
	 */
	//##ModelId=3BBD8D150198
	class CNwssTransLUTEntry : public CBase
		{
	public: // methods

	/**
		Factory construction.
		@since			7.0
	*/
		static CNwssTransLUTEntry* NewL();

	/** 
		Destructor.
		@since			7.0
	*/
		virtual ~CNwssTransLUTEntry();

	private: // methods

	/** 
		Default constructor.
		@since			7.0
	*/
		//##ModelId=3C4C49C30046
		CNwssTransLUTEntry();

	public: // attributes

		/** Client callback pointer for the transaction. This uniquely identifies the
			transaction.
		*/
		//##ModelId=3C4C49C3003C
		MWspCOMethodCallback* iCallback;

		/** WAP Stack transaction handle
		*/
		//##ModelId=3C4C49C30014
		RWSPCOTrans iStackTrans;

		/** WAP Stack transaction ID
		*/
		//##ModelId=3C4C49C30002
		RWSPCOTrans::TTransID iStackTransID;

		/** Response body data handler
		*/
		//##ModelId=3C4C49C203DE
		CNwssWspTrHndDataSupplier* iResponseBodyHandler;

		/** A flag to indicate whether this transaction has been aborted by the 
			client, but is in the EDone state.
		*/
		TBool iAborted;
		};

public: // methods

/** 
	Factory construction.  Pre-allocates a table of the specified size.
	@since			7.0
	@param			aTableSize	(in) The initial size of the look-up table.
	@return			The newly constructed instance.
	@leave			System error codes, e.g. KErrNoMemory.
 */
	//##ModelId=3C4C49D601AC
	static CNwssTransLookUpTable* NewL(TInt aTableSize);

/**
	Destructor.
	@since			7.0
 */
	//##ModelId=3C4C49D6015C
	virtual ~CNwssTransLookUpTable();

/** 
	Returns the next available LUT entry at the top of the table.
	@since			7.0
	@return			A reference to the entry.
 */
	//##ModelId=3C4C49D60101
	CNwssTransLUTEntry& NewEntry();

/**
	Locate a look-up table entry that matches the supplied transaction
					ID.  It is assumed that the look-up table user does not allow
					duplicates to be entered in the table.
	@since			7.0
	@param			aTransId	(in)  The WAP Stack transaction ID.
	@param			aFound		(out) A flag indicating if the entry was found.
	@return			A reference to the located entry.  Note, if aFound is EFalse
					the reference should be discard, as it will be garbage.
 */
	//##ModelId=3C4C49D50345
	CNwssTransLUTEntry& LookUpByTransId(RWSPCOTrans::TTransID aTransId, TBool& aFound);

/**
	Locate a look-up table entry that matches the supplied client
					method callback.  It is a requirement on the client that it
					uses unique MWspCOMethodCallback objects for each transaction.
					Hence there is an assumption that the look-up table user does
					not allow entries with duplicate callback's to be entered in
					the table.
	@since			7.0
	@param			aCallback	(in)  The client transaction callback.
	@param			aFound		(out) A flag indicating if the entry was found.
	@return			A reference to the located entry.  Note, if aFound is EFalse
					the reference should be discard, as it will be garbage.
 */
	//##ModelId=3C4C49D501AA
	CNwssTransLUTEntry& LookUpByCallback(MWspCOMethodCallback& aCallback, TBool& aFound);

/**
	Removes the entry corresponding to aCallback from the table. Note
					that since the table is pre-allocated, the actual entry is just
					recycled by moving it to the top of the table, adjusting the high-
					water mark (if necessary) to point at it.
	@since			7.0
	@param			aCallback	(in) A callback, uniquely identifying the table entry
									 to remove.
 */
	//##ModelId=3C4C49D500B0
	void RemoveEntry(MWspCOMethodCallback& aCallback);

/** 
  		Resizes the look-up table.  If aNewSize exceeds the current
					size, then entries are added.  If it is less than the current
					size, then surplus entries are deleted.
  	@since			7.0
	@param			aNewSize	(in) The new table size
	@leave			System error codes, e.g. KErrNoMemory if the table can't be
					enlarged.
 */
	//##ModelId=3C591A9A038E
	void ResizeTableL(TInt aNewSize);

/**
	Indicates whether there are any 'live' entries in the lookup table,
	@since			7.0
*/
	//##ModelId=3C9B095601B8
	TBool IsEmpty() const;

/**
	Returns the first entry in the lookup table.
	@param			aFound		(out) A flag indicating if the entry was found.
	@return			A reference to the located entry.  Note, if aFound is EFalse
					the reference should be discard, as it will be garbage.
*/	
	//##ModelId=3C9B09560186
	CNwssTransLUTEntry& Head(TBool& aFound);

private: // methods

/**
	Normal constructor.
	@since			7.0
 */
	//##ModelId=3C4C49D5004C
	CNwssTransLookUpTable();

/**
	Second phase construction.  Pre-allocates table entries upto
					the specified table size.
	@since			7.0
	@param			aTableSize	(in) The required initial table size.
	@leave			System error codes, e.g. KErrNoMemory.
 */
	//##ModelId=3C4C49D40325
	void ConstructL(TInt aTableSize);
  
/**
	Helper in making look-ups based on a call-back object.
	@since			7.0
	@param			aCallback	(in) A callback, uniquely identifying a table entry
	@param			aIndex		(out) The index in the table at which the entry was located
	@return			Pointer to the table entry if found, or NULL if not found.
 */
	//##ModelId=3C9B0956012B
	CNwssTransLUTEntry* LookUp(MWspCOMethodCallback& aCallback, TInt& aIndex);

private: // attributes

	/** The number of active entries in the look-up table.
		iEntries[iNumActiveLUTEntries] will return the next free entry.
	*/
	//##ModelId=3C4DA52D0338
	TInt iNumActiveLUTEntries;

	/** The pre-allocated look-up table.  It is ready populated with
		entry structures.
	*/
	//##ModelId=3C4C49D402AD
	RPointerArray<CNwssTransLUTEntry> iEntries;
	};


#endif // __CNWSSTRANSLOOKUPTABLE_H__

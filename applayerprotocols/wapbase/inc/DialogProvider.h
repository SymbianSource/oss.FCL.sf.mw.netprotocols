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

#ifndef __DIALOGPROVIDER_H__
#define __DIALOGPROVIDER_H__

// System includes
//
#include <e32std.h>
#include <wapcli.h> // for Certificate Information structure

// Forward declarations
class MWtaiCancelOperationObserver;
class MHTTPDataSupplier;

/**
	@class MDialogProvider
	Dependencies : none
	Comments : This class provides an API to be called by low level components,
	like data collectors, and implemented by the UI.

	It is mainly used for dialog creation and information passing.
	@publishedAll
	@deprecated
	@since	6.0
	@todo	Should this be renamed?
 */
class MDialogProvider
	{
public:

/**
	Intended Usage	:	This creates an input dialog for the user to enter a text 
	string into a single text field.
	@warning		This function must be a blocking function.
	@since			6.0
	@param			aMessage		A descriptor with the label/title of the text field.
	@param			aDefaultInput	A descriptor with (if any) default input text.
	@return			A pointer to a buffer with the input text. 
	@todo			Is warning correct?
 */
	virtual HBufC* PromptL( const TDesC& aMessage, const TDesC& aDefaultInput) =0;

/**
	Intended Usage	:	Creates a password dialog for user to enter some hidden text.
	@warning		This function must be a blocking function.
	@since			6.0
	@param			aPasswd	An output argument which is set the entered password.
	@return			void.
	@todo			Is warning correct?
 */
	virtual void PasswordL(TPassword& aPasswd) =0;

/**
	Intended Usage	:	Creates a simple confirmation dialog with Ok and Cancel (or 
	similar) buttons. Allows the user to confirm an action or message. Some text 
	is displayed indicating the nature of the action.
	@warning		This function must be a blocking function.
	@since			6.0
	@param			aMessage			A descriptor with a display message. 
	@param			aOkButtonLabel		A descriptor with the label for the Ok button.
	@param			aCancelButtonLabel	A descriptor with the label for the Cancel button.
	@return			A boolean value of ETrue if the Ok button was pressed, or a value 
	of EFalse if the cancel button was pressed.
	@todo			Is warning correct?
 */
	virtual TBool ConfirmationL(const TDesC& aMessage, const TDesC& aOkButtonLabel, const TDesC& aCancelButtonLabel ) =0;

/**
	Intended Usage	:	Created a certificate confirmation dialog. Requests that the
	user accept or reject the displayed certificate.
	@warning		This function must be a blocking function.
	@since			6.0
	@param			aCertificateInfo	The certificate details.
	@return			A boolean value of ETrue if the certifiacte was accepted, or a 
	value of EFalse if it was not.
	@todo			Is warning correct?
 */
	virtual TBool CertificateConfirmationL(const RCertificate::TInfo& aCertificateInfo) =0;

/**
	Intended Usage	:	Creates an alert dialog to inform the user. A message is
	displayed and there is an Ok (or similar) button to close the dialog.
	@warning		This function must be a blocking function.
	@since			6.0
	@param			aMessage	A descriptor with the alert message.
	@return			void.
	@todo			Is warning correct?
 */
	virtual void  AlertL(const TDesC& aMessage) =0;

/**
	Intended Usage	:	This creates a dialog which indicates that a connection
	is being done.
	@warning		This function must be a non blocking function.
	@since			6.0
	@return			virtual void 
	@todo			Is warning ok? Also, need to change this to take a message.
	@pre 			A connecting dialog is not already been created.
	@post			Unspecified
 */
	virtual void DisplayConnectingDialogL() =0;

/**
	Intended Usage	:	Cancels the current connecting dialog.
	@since			6.0
	@return			void.
 */
	virtual void CancelConnectingDialog() =0;

/**
	Intended Usage	: Displays a non-blocking cancel dialog that allows the user to cancel an
					  an operation. When the user cancels the operation, the dialog must close
					  and call the CancelOperation() method provided by the MWtaiPublicCancelOperation
					  object passed in by the object that requires the use of the cancel dialog.
	@since			6.0
	@param			aMessage Descriptor containing the message to display on the dialog
	@param			aCancelOperation A pointer to the object that implements the MWtaiPublicCancelOperation class
	@pre 			None
	@post			A non-blocking cancel dialog appears with the paramater text passed into it
 */
	virtual void DisplayCancelDialogL(const TDesC& aMessage, MWtaiCancelOperationObserver* aCancelOperation) = 0;

/**
	Intended Usage	: This method closes the cancel dialog created from the DisplayCancelDialogL above.
	@since			6.0
	@pre 			The DisplayCancelDialogL() method must have been called and the dialog is on display
	@post			The cancel dialog closes
 */
	virtual void CloseCancelDialog() = 0;

/**
	Intended Usage	: This method is called when unknown data is first recieved and should be handled.
					  Data is recieved in chunks and this method is called when the first chunk has
					  arrived.
	@since			6.0
	@param			aMimeType A descriptor containing the mime type of the data
	@param			aDataSupplier A pointer to the data supplier object that contains the data
	@return			A boolean value that returns ETrue if the data will be handled and to continue
					downloading the data or EFalse to say not to handle the data and cancel the download.
	@pre 			Unknown data content will have begun downloading
	@post			The data will continue downloading if ETrue is returned
 */
	virtual TBool GotUnknownData(const TDesC& aMimeType, MHTTPDataSupplier* aDataSupplier) = 0;

/**
	Intended Usage	: This method will be called when more unknown data from an existing fetch has
					  arrived. As the data arrives in chunks, this methos will be called when data
					  arrives after the GotUnknownData() method has been called.
	@since			6.0
	@param			aDataSupplier A pointer to the data supplier object that contains the data
	@pre 			The first chunk of data has already arrived and GotUnknownData() method has been called
 */
	virtual void GotMoreUnknownData(MHTTPDataSupplier* aDataSupplier) = 0;

/**
	Intended Usage	: The method is called when an existing unknown data fetch has completed and
					  no more unknown data is to be expected.
	@since			6.0
	@param			aDataSupplier A pointer to the data supplier that contains the unknown data
	@param			aErrorCode An error value, KErrNone if no error, KErrCancel if the download is cancelled
	@pre 			An unknown data fetch has been completed and all data handled
 */
	virtual void NoMoreUnknownData(MHTTPDataSupplier* aDataSupplier, TInt aErrorCode) = 0;

/**
	Intended Usage	:	Function placeholder for BC proofing.
	@since			6.0
 */
	virtual void MDialogProvider_Reserved1() = 0;
	};

#endif // __DIALOG_PROVIDER_H__

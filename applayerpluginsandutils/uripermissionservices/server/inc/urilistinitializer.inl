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

using namespace Xml;

// Virtual functions from MContentHandler
void CUriListInitializer::OnStartPrefixMappingL ( const RString& /* aPrefix */, const RString& /* aUri */, TInt aErrorCode )
	{
	User::LeaveIfError ( aErrorCode );
			
	}

void CUriListInitializer::OnEndPrefixMappingL ( const RString& /* aPrefix */, TInt aErrorCode )
	{
	User::LeaveIfError ( aErrorCode );		
	}

void CUriListInitializer::OnIgnorableWhiteSpaceL ( const TDesC8& /* aBytes */, TInt aErrorCode )
	{
	User::LeaveIfError ( aErrorCode );		
	}

void CUriListInitializer::OnSkippedEntityL ( const RString& /* aName */, TInt aErrorCode )
	{
	User::LeaveIfError ( aErrorCode );		
	}

void CUriListInitializer::OnProcessingInstructionL ( const TDesC8& /* aTarget */, const TDesC8& /* aData */, TInt aErrorCode )
	{
	User::LeaveIfError ( aErrorCode );	
	}

void CUriListInitializer::OnError ( TInt /* aErrorCode */ )
	{
	}

TAny* CUriListInitializer::GetExtendedInterface ( const TInt32 /* aUid */ )
	{
	return NULL;
	}

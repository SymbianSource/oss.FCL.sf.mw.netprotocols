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

void CTldListInitializer::OnStartPrefixMappingL ( const RString& /* aPrefix */, const RString& /* aUri */, TInt aErrorCode )
	{
	User::LeaveIfError ( aErrorCode );
			
	}

void CTldListInitializer::OnEndPrefixMappingL ( const RString& /* aPrefix */, TInt aErrorCode )
	{
	User::LeaveIfError ( aErrorCode );		
	}

void CTldListInitializer::OnIgnorableWhiteSpaceL ( const TDesC8& /* aBytes */, TInt aErrorCode )
	{
	User::LeaveIfError ( aErrorCode );		
	}

void CTldListInitializer::OnSkippedEntityL ( const RString& /* aName */, TInt aErrorCode )
	{
	User::LeaveIfError ( aErrorCode );		
	}

void CTldListInitializer::OnProcessingInstructionL ( const TDesC8& /* aTarget */, const TDesC8& /* aData */, TInt aErrorCode )
	{
	User::LeaveIfError ( aErrorCode );	
	}

void CTldListInitializer::OnError ( TInt /* aErrorCode */ )
	{
	}

TAny* CTldListInitializer::GetExtendedInterface ( const TInt32 /* aUid */ )
	{
	return NULL;
	}

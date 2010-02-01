// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "srvparams.h"

inline void TRepositoryBackupStreamIndexElement::Set (TUid aUid, TStreamId aSettingsStream, TStreamId aDeletedSettingsStream) 
	{
	iUid = aUid;
	iSettingsStream = aSettingsStream ;
	iDeletedSettingsStream = aDeletedSettingsStream;
	} 

inline void TRepositoryBackupStreamIndexElement::Get (TUid& aUid, TStreamId& aSettingsStreamId, TStreamId& aDeletedSettingsStreamId) 
	{
	aUid = iUid;
	aSettingsStreamId = iSettingsStream;
	aDeletedSettingsStreamId = iDeletedSettingsStream ;
	} 

inline void TRepositoryBackupStreamIndexElement::ExternalizeL(RWriteStream& aStream) const
	{
	aStream << iUid ;
	aStream << iSettingsStream ;
	aStream << iDeletedSettingsStream ;
	}

inline void TRepositoryBackupStreamIndexElement::InternalizeL(RReadStream& aStream, TUint32 aBackupStreamExtensions)
	{
	TUint32 intUid ;
	aStream >> intUid ;
	iUid.iUid = intUid ;
	aStream >> iSettingsStream ;
	if (aBackupStreamExtensions & EDeletedKeysSupported)
		{
		aStream >> iDeletedSettingsStream ;
		}
	}


inline CRepositoryBackupStreamIndex* CRepositoryBackupStreamIndex::NewL()
	{
	return new (ELeave) CRepositoryBackupStreamIndex ;
	} 

inline CRepositoryBackupStreamIndex* CRepositoryBackupStreamIndex::NewLC()
	{
	CRepositoryBackupStreamIndex* self = CRepositoryBackupStreamIndex::NewL();
	CleanupStack::PushL(self) ;
	return self ;
	}

inline CRepositoryBackupStreamIndex::~CRepositoryBackupStreamIndex() 
	{
	iStreamIndex.Close() ;
	}
	
inline void CRepositoryBackupStreamIndex::Close(void) 
	{
	iStreamIndex.Close() ;
	} 

inline void CRepositoryBackupStreamIndex::Reset() 
	{
	iIndex = 0;
	}

inline void CRepositoryBackupStreamIndex::ExternalizeL(RWriteStream& aStream) const
	{
	TCardinality nElements = iStreamIndex.Count() ;
	aStream << nElements ;
	for (int i = 0; i < nElements; i++)
		{
		aStream << iStreamIndex[i] ;
		}
	} 
	
inline void CRepositoryBackupStreamIndex::InternalizeL(RReadStream& aStream, TUint32 aBackupStreamExtensions) 
	{
	TCardinality nElements ;
	aStream >> nElements ;
		
	TRepositoryBackupStreamIndexElement indexElement ;
	for (int i = 0; i < nElements; i++)
		{			
		indexElement.InternalizeL(aStream, aBackupStreamExtensions);
		iStreamIndex.Append(indexElement) ;
		}		
	}
	

inline TRepositoryBackupStreamHeader::TRepositoryBackupStreamHeader(TStreamId aIndexStreamId) : iBackupStreamUid(KServerUid3), iBackupStreamVersion(KBackupStreamVersion), iIndexStreamId(aIndexStreamId), iBackupExtensionsSupported(KBackupExtensionsSupported)
	{
	iReserved1 = 0 ;
	iReserved2 = 0 ;
	iReserved3 = 0 ;
	iReserved4 = 0 ;
	}
	
inline TRepositoryBackupStreamHeader::TRepositoryBackupStreamHeader()
	{	
	iReserved1 = 0 ;
	iReserved2 = 0 ;
	iReserved3 = 0 ;
	iReserved4 = 0 ;
	}
		
inline void TRepositoryBackupStreamHeader::ExternalizeL(RWriteStream& aStream) const
	{
	aStream << iBackupStreamUid ;
	aStream << iBackupStreamVersion ;
	aStream << iIndexStreamId ;
	aStream << iBackupExtensionsSupported ;
	aStream << iReserved1 ;
	aStream << iReserved2 ;
	aStream << iReserved3 ;
	aStream << iReserved4 ;	
	} 
	
inline void TRepositoryBackupStreamHeader::InternalizeL(RReadStream& aStream) 
	{
	aStream >> iBackupStreamUid ;
	aStream >> iBackupStreamVersion ;
	if ((iBackupStreamUid != KServerUid3) || 
	    (iBackupStreamVersion > KBackupStreamVersion))
	    {
	    // No valid header found in stream - bail out before we
	    // read off the end of the stream!
	    User::Leave(KErrNotSupported) ;
	    }
	    
	aStream >> iIndexStreamId ;
	aStream >> iBackupExtensionsSupported ;
	aStream >> iReserved1 ;
	aStream >> iReserved2 ;
	aStream >> iReserved3 ;
	aStream >> iReserved4 ;	
	}

inline TUid TRepositoryBackupStreamHeader::getUid()
	{
	return iBackupStreamUid ;		
	}

inline TStreamId TRepositoryBackupStreamHeader::getIndexStreamId() 
	{
	return iIndexStreamId ;
	}
	
inline TUint32 TRepositoryBackupStreamHeader::getVersion()
	{
	return iBackupStreamVersion ;
	}
	
inline TUint32 TRepositoryBackupStreamHeader::getBackupExtensionsSupported()
	{
	return iBackupExtensionsSupported ;
	}

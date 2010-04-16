/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description: 
*  Deflate decompression filter for HTTP protocol stack.
*
*
*/


#include <http/rhttptransaction.h>
#include <http/rhttpheaders.h>
#include <http/rhttpresponse.h>
#include <httpstringconstants.h>
#include "DeflateFilter.h"
#include <bautils.h>
#include <eikenv.h>
#include <bldvariant.hrh>
#include <httpfiltercommonstringsext.h>
//------------------------------------------------------------------------

_LIT8( KDeflateFilterName, "Deflate");

#ifndef __SERIES60_
_LIT8(KGzipDeflateStr, "gzip,deflate");
#else
_LIT8(KGzipDeflateStr, "gzip, deflate, x-gzip, identity; q=0.9");
#endif
//------------------------------------------------------------------------

// format for output of data/time values
#if defined (_DEBUG) && defined (_LOGGING)
#define LOG_FILE "deflate.log"						//lint !e1923 LOG_FILE only used in debug build
_LIT(KDateFormat,"%D%M%Y%/0%1%/1%2%/2%3%/3 %:0%H%:1%T%:2%S.%C%:3");
#endif

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CDeflateFilter::CDeflateFilter
// Constructor
// -----------------------------------------------------------------------------
//
CDeflateFilter::CDeflateFilter(RHTTPSession aSession)
		: iSession(aSession)
{
	iDataSups.Reset();
} //lint !e1746			RHTTPSession is a light object, just a wrapper to CHTTPSession

// -----------------------------------------------------------------------------
// CDeflateFilter::InstallFilterL
// Initialize the filter and register it to sesssion's filter collection
// -----------------------------------------------------------------------------
//
CHttpDeflateFilter* CDeflateFilter::InstallFilterL(TAny* aSession)
{
	RHTTPSession* session = REINTERPRET_CAST(RHTTPSession*, aSession);
	CDeflateFilter* filter = new (ELeave) CDeflateFilter(*session);
	CleanupStack::PushL(filter);
	filter->ConstructL(*session);
	CleanupStack::Pop(filter);
	return filter;
}

// -----------------------------------------------------------------------------
// CDeflateFilter::ConstructL
// Memory and resource allocation, leaves
// -----------------------------------------------------------------------------
//
void CDeflateFilter::ConstructL(RHTTPSession aSession)
{
    // load resources if UI is available
	iStringPool = aSession.StringPool();
	RStringF filterName = iStringPool.OpenFStringL( KDeflateFilterName );
	CleanupClosePushL( filterName );

	iSession.FilterCollection().AddFilterL( *this, 
											THTTPEvent::ESubmit,
											MHTTPFilter::EProtocolHandler+5,			
											filterName );
	iSession.FilterCollection().AddFilterL( *this,
											THTTPEvent::EGotResponseHeaders,
											MHTTPFilter::EProtocolHandler+5,			
											filterName );								
	iSession.FilterCollection().AddFilterL( *this, 
											THTTPEvent::EGotResponseBodyData,
											MHTTPFilter::EProtocolHandler+5,	
											filterName );
	iSession.FilterCollection().AddFilterL( *this, 
											THTTPEvent::EResponseComplete,
											MHTTPFilter::EProtocolHandler+5,
											filterName );
	iSession.FilterCollection().AddFilterL( *this,
											THTTPEvent::EClosed,
											MHTTPFilter::EProtocolHandler+5,
											filterName );
	CleanupStack::PopAndDestroy();

	iXGzipVal = iStringPool.StringF(HttpFilterCommonStringsExt::EXGzip, HttpFilterCommonStringsExt::GetTable());
	// log file
	__OPEN_LOG( "deflate.log" );
	__LOG( _L( "--Initialize Deflate Filter---\n" ) );
} //lint !e1746  RHTTPSession is a light weight class, just wrapper for CHTTPSession


//------------------------------------------------------------------------
// CDeflateFilter::~CDeflateFilter
// Destructor
//------------------------------------------------------------------------
//
CDeflateFilter::~CDeflateFilter()
{
	if (iLoadCount)
	{
		iLoadCount = -1;
        RStringF filterName;	//lint !e1551
		TRAPD( error, ( filterName = iStringPool.OpenFStringL( KDeflateFilterName ) ) );	//lint !e1551  
		if( error == KErrNone ) iSession.FilterCollection().RemoveFilter( filterName ); //lint !e1551 
        filterName.Close(); //lint !e1551 
	}

	CleanupAll();															//lint !e1551 
	iDataSups.Close();														//lint !e1551 
}

//------------------------------------------------------------------------
// CDeflateFilter::MHFLoad
// See MHTTPFilterBase::MHFRunL
//------------------------------------------------------------------------
//
void CDeflateFilter::MHFLoad(RHTTPSession, THTTPFilterHandle)
{
	__LOG( _L( "--Load Deflate Filter---\n" ) );
	++iLoadCount;
}

//------------------------------------------------------------------------
// CDeflateFilter::MHFUnload
// See MHTTPFilterBase::MHFRunError
//------------------------------------------------------------------------
//
void CDeflateFilter::MHFUnload(RHTTPSession , THTTPFilterHandle)
{
	__LOG( _L( "--Unload Deflate Filter---\n" ) );
	if (--iLoadCount)
	{
		return;
	}

	delete this;
}

//------------------------------------------------------------------------
// CDeflateFilter::MHFRunL
// See MHTTPFilterBase::MHFRunL 
//------------------------------------------------------------------------
//
void CDeflateFilter::MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent)
{
	if (aEvent.iUID != KHTTPUid) return;

	switch(aEvent.iStatus)
	{
		case THTTPEvent::ESubmit:
		{
			__LOG( _L( "Event: ESubmit\n" ) );
			AlterRequestHeadersL( aTransaction );
		}
		break;
		case THTTPEvent::EGotResponseHeaders:
		{
			__LOG( _L( "Event: EGotResponseHeaders\n" ) ); 
			CheckResponseHeadersL( aTransaction );
		}
		break;
		case THTTPEvent::EGotResponseBodyData:
		{
			__LOG( _L( "Event: EGotResponseBodyData\n" ) );
			TInt idx = GetTransIdx( aTransaction );
			if( idx != KErrNotFound )
			{
				iDataSups[idx]->ProcessDataPartL();
			}
		}
		break;
		case THTTPEvent::EResponseComplete:
		{
			__LOG( _L( "Event: EResponseComplete\n" ) );
			Cleanup( aTransaction );
		}
		break;
		case THTTPEvent::EClosed:
		{
			__LOG( _L( "Event: EFailed\n" ) );
			Cleanup( aTransaction );
		}
		break;
		default: 
		{
			__LOG1( _L( "Unknow Event: ID - %d\n" ), aEvent.iStatus );
		}
		break;
	}
}

//------------------------------------------------------------------------
// CDeflateFilter::MHFRunError
// See MHTTPFilterBase::MHFRunError
//------------------------------------------------------------------------
//
TInt CDeflateFilter::MHFRunError(TInt /*aError*/, RHTTPTransaction /*aTransaction*/, const THTTPEvent& )
{
	// error happened, cleanup the datasupplier related with this transaction
//	Cleanup( aTransaction );
//	aTransaction.SendEventL( THTTPEvent::EFailed, THTTPEvent::EIncoming, THTTPFilterHandle::ECurrentFilter );
	return KErrNone;
}

//------------------------------------------------------------------------
// CDeflateFilter::MHFSessionRunL
// See MHTTPFilterBase::MHFSessionRunL
//------------------------------------------------------------------------
//
void CDeflateFilter::MHFSessionRunL(const THTTPSessionEvent& )
{
	// do nothing
}

//------------------------------------------------------------------------
// CDeflateFilter::MHFSessionRunL
// See MHTTPFilterBase::MHFSessionRunL
//------------------------------------------------------------------------
//
TInt CDeflateFilter::MHFSessionRunError(TInt aError, const THTTPSessionEvent& )
{
	// session problem, need to close Deflate engine
	CleanupAll();
	return aError;
}

//------------------------------------------------------------------------
// CDeflateFilter::DumpResponseHeadersL
// Dump the response headers to LOG file
//------------------------------------------------------------------------
//
#if defined (_DEBUG) && defined (_LOGGING)
void CDeflateFilter::DumpHeadersL( RHTTPHeaders aHeaders )
{
	__LOG( _L( "Dump the header...\n" ) );

	RStringPool strP = iSession.StringPool();
	THTTPHdrFieldIter it = aHeaders.Fields();
	
	while( it.AtEnd() == EFalse )
	{
		RStringTokenF fieldName = it();
		RStringF fieldNameStr = strP.StringF( fieldName );
		
		THTTPHdrVal fieldVal;

		// how many parts in this field?
		TInt parts = aHeaders.FieldPartsL( fieldNameStr );

		// dump all parts of  a  header field
		for( TInt i=0; i<parts; ++i )
		{
			if( aHeaders.GetField( fieldNameStr, i, fieldVal ) == KErrNone )
			{
				const TDesC8& fieldNameDesC = fieldNameStr.DesC();
				if( i==0 )
				{
					__DUMPBIN( fieldNameDesC );
					__LOG( _L ( " : " ) );
				}
				else
				{
					__LOG( _L ( "," ) );
				}

				switch( fieldVal.Type() )
				{
				case THTTPHdrVal::KTIntVal:
					{
						__LOG1( _L("%d"), fieldVal.Int() );
					}
					break;
				case THTTPHdrVal::KStrFVal:
					{
						RStringF fieldValStr = strP.StringF( fieldVal.StrF() );
						const TDesC8& fieldValDesC = fieldValStr.DesC();
						__DUMPBIN( fieldValDesC );
					}
					break;
				case THTTPHdrVal::KStrVal:
					{
						RString fieldValStr = strP.String( fieldVal.Str() );
						const TDesC8& fieldValDesC = fieldValStr.DesC();
						__DUMPBIN( fieldValDesC );
					}
					break;
				case THTTPHdrVal::KDateVal:
					{
						TDateTime date = fieldVal.DateTime();
						TBuf<40> dateTimeString;
						TTime t( date );
						t.FormatL( dateTimeString, KDateFormat );
						TBuf8<40> dtStr;
						dtStr.Copy( dateTimeString.Left( 128 ) );
						__DUMPBIN( dtStr );
					}
					break;
				case THTTPHdrVal::KNoType:
				default:
					{
						__LOG( _L ( "Unrecognized value type.\n" ) );
					}
					break;
				}

				__LOG( _L ( "\n" ) );
			}
		}

		++it;
	}
	__LOG( _L( "Header is Dumped already...\n" ) );
}//lint !e1746	no need to be const reference, just for debugging
#endif

//------------------------------------------------------------------------
// CDeflateFilter::AlterRequestHeadersL
// Check HTTP headers and extract MIME type
//------------------------------------------------------------------------
// 
void CDeflateFilter::AlterRequestHeadersL( const RHTTPTransaction& aTrans )
{
#if defined (_DEBUG) && defined (_LOGGING)
	__LOG( _L( "Dump the request header before modification.\n" ) );
	DumpHeadersL( aTrans.Request().GetHeaderCollection() );
#endif
	
	// remove the original Accept-Encoding field set by HTTP client
	aTrans.Request().GetHeaderCollection().RemoveField( 
			iStringPool.StringF( HTTP::EAcceptEncoding, RHTTPSession::GetTable()) );
	SetHeaderL( aTrans.Request().GetHeaderCollection(), HTTP::EAcceptEncoding, KGzipDeflateStr );

#if defined (_DEBUG) && defined (_LOGGING)
	__LOG( _L( "Dump the request header after modification.\n" ) );
	DumpHeadersL( aTrans.Request().GetHeaderCollection() );
#endif
}

//------------------------------------------------------------------------
// CDeflateFilter::CheckHeadersL
// Check HTTP headers and extract MIME type
//------------------------------------------------------------------------
// 
void CDeflateFilter::CheckResponseHeadersL( const RHTTPTransaction& aTrans )
{
	// read the header data and check the MIME type here	
	// check the status and body
	RHTTPResponse response = aTrans.Response();
	
	#if defined (_DEBUG) && defined (_LOGGING)
	DumpHeadersL( aTrans.Response().GetHeaderCollection() );
	#endif

	RHTTPHeaders headers = response.GetHeaderCollection();
	RStringF fieldNameStr = iStringPool.StringF( HTTP::EContentEncoding, RHTTPSession::GetTable() );

	// read the first part of content-encoding field
	THTTPHdrVal fieldVal;

/*
	---------
	if( headers.GetField( fieldNameStr, 0, fieldVal ) == KErrNone &&
		( fieldVal.StrF().Index( RHTTPSession::GetTable() ) == HTTP::EGzip ||
		  fieldVal.StrF().Index( RHTTPSession::GetTable() ) == HTTP::EDeflate ) )
	{
		// find gzip/deflate content-decoding header
		__LOG( _L( "gzip/deflate content-decoding exists...\n" ) );

		// we shouldn't store the same transaction twice
		TInt idx = GetTransIdx( aTrans );
		if( idx != KErrNotFound )
		{
			// remove the previous one
			CDeflateDataSupplier* iSup = iDataSups[idx];
			iDataSups.Remove( idx );
			delete iSup;
		}

		CDeflateDataSupplier* pSup = CDeflateDataSupplier::NewL( aTrans.Id(), response.Body(),
			(1<<15), fieldVal.StrF().Index( RHTTPSession::GetTable() ) == HTTP::EGzip ? 0 : 1 ); 				//CDecmpStream::EGzip : CDecmpStream::EZlib ); 
		iDataSups.Append( pSup );
		response.SetBody( *pSup );

		// remove the fields of deflate and gzip
		aTrans.Response().GetHeaderCollection().RemoveField( 
			iStringPool.StringF( HTTP::EContentEncoding, RHTTPSession::GetTable()) );
	}
	-----
*/
/*********************************************************************/
	if( headers.GetField( fieldNameStr, 0, fieldVal ) == KErrNone )
		{	
		if( ( fieldVal.StrF().Index( RHTTPSession::GetTable() ) == HTTP::EGzip ||
		 	 fieldVal.StrF().Index( RHTTPSession::GetTable() ) == HTTP::EDeflate ||
			 fieldVal.StrF().Index( HttpFilterCommonStringsExt::GetTable() ) == HttpFilterCommonStringsExt::EXGzip ) )
			{
			// find gzip/deflate/x-gzip content-decoding header
			__LOG( _L( "gzip/deflate/x-gzip content-decoding exists...\n" ) );

			/**/
			// we shouldn't store the same transaction twice
			TInt idx = GetTransIdx( aTrans );
			if( idx != KErrNotFound )
				{
					// remove the previous one
					CDeflateDataSupplier* iSup = iDataSups[idx];
					iDataSups.Remove( idx );
					delete iSup;
				}
			/**/

			CDeflateDataSupplier* pSup = NULL;

			if (fieldVal.StrF().Index( RHTTPSession::GetTable() ) == HTTP::EGzip ||
				fieldVal.StrF().Index( HttpFilterCommonStringsExt::GetTable() ) == HttpFilterCommonStringsExt::EXGzip )
				{
				pSup = CDeflateDataSupplier::NewL( aTrans.Id(), response.Body(), (1<<15), 0 ); // CDecmpStream::EGzip
				}
			else // if fieldVal.StrF().Index( RHTTPSession::GetTable() ) == HTTP::EDeflate
				{
				pSup = CDeflateDataSupplier::NewL( aTrans.Id(), response.Body(), (1<<15), 1 ); // CDecmpStream::EZlib
				}

			iDataSups.Append( pSup );
			response.SetBody( *pSup );				 

			// remove the fields of deflate and gzip
			aTrans.Response().GetHeaderCollection().RemoveField( 
				iStringPool.StringF( HTTP::EContentEncoding, RHTTPSession::GetTable()) );

			// Adding Transaction parameter so Download manager knows that content was compressed.
			RHTTPTransactionPropertySet propSet = aTrans.PropertySet();
			// Key = "ContentInflated", fieldVal = "gzip" or "deflate" or "x-gzip"
			propSet.RemoveProperty( iStringPool.StringF(HttpFilterCommonStringsExt::EContentInflated, HttpFilterCommonStringsExt::GetTable()));
			propSet.SetPropertyL( iStringPool.StringF(HttpFilterCommonStringsExt::EContentInflated, HttpFilterCommonStringsExt::GetTable()), fieldVal ); 
			}
		}
	else
		{
		TInt idx = GetTransIdx( aTrans );
		if( idx != KErrNotFound )
			{
			// remove the previous one
			CDeflateDataSupplier* iSup = iDataSups[idx];
			iDataSups.Remove( idx );
			delete iSup;
			iSup = NULL;
			}
		}
}

//-----------------------------------------------------------------------------
// CDeflateFilter::GetDeflateTransIdx
// Retrieve the Deflate datasupplier
//-----------------------------------------------------------------------------
//
TInt CDeflateFilter::GetTransIdx( const RHTTPTransaction& aTrans ) const
{
	for( TInt i=0; i<iDataSups.Count(); ++i )
		if( iDataSups[i]->GetTransId() == aTrans.Id() )
			return i;
	return KErrNotFound;
}


//-----------------------------------------------------------------------------
// CDeflateFilter::Cleanup
// Cleanup the resource related with a transaction
//-----------------------------------------------------------------------------
//
void CDeflateFilter::Cleanup( const RHTTPTransaction& aTrans )
{
	TInt idx = GetTransIdx( aTrans );
	if( idx != KErrNotFound )
	{
		// remove the problematic data supplier
		CDeflateDataSupplier* iSup = iDataSups[idx];
		iDataSups.Remove( idx );
		delete iSup;
		iSup = NULL;
	}

	// shutdown the Deflate session
	if( iDataSups.Count()==0 ) CleanupAll();
}

//-----------------------------------------------------------------------------
// CDeflateFilter::CleanupAll
// Cleanup all the Deflate transactions, in case a session error happens or a session
// is closed.
//-----------------------------------------------------------------------------
//
void CDeflateFilter::CleanupAll()
{
	iDataSups.ResetAndDestroy();
}

//-----------------------------------------------------------------------------
// CDeflateFilter::SetHeaderL
// Add string to the header
//-----------------------------------------------------------------------------
//
void CDeflateFilter::SetHeaderL(RHTTPHeaders aHeaders, TInt aHdrField, const TDesC8& aHdrValue) const
{
	RStringF valStr = iStringPool.OpenFStringL(aHdrValue);
	THTTPHdrVal val(valStr);
	aHeaders.SetFieldL(iStringPool.StringF(aHdrField,RHTTPSession::GetTable()), val);
	valStr.Close();
}

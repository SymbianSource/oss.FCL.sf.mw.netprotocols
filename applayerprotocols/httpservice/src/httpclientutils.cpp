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

#include "httpclientutils.h"
#include <stringpool.h>

TInt HttpClientUtils::OpenStringF(const TDesC8& aDesc, RStringPool& aPool, RStringF& aStringF)
	{
	// Usage of TRAP here is not correct. Currently we do not have any non-leaving variant in
	// the stringpool implementation. Need to be requested to provide.
	TRAPD(err, aStringF = aPool.OpenFStringL(aDesc));
	return err;
	}

THttpHeaderValueVariant HttpClientUtils::CopyHttpHdrVal(const THTTPHdrVal& aVal)
  {
  switch(aVal.Type())
      {
      case THTTPHdrVal::KTIntVal:
          {
          return THttpHeaderValueVariant(aVal.Int());
          }
      
      
      case  THTTPHdrVal::KStrVal:
          {
          return THttpHeaderValueVariant(aVal.Str().DesC());
          }
      
      
      case THTTPHdrVal::KStrFVal:
          {
          return THttpHeaderValueVariant(aVal.StrF().DesC());
          }
      
      
      case THTTPHdrVal::KDateVal:
          {
          return THttpHeaderValueVariant(aVal.DateTime());
          }
      
      }
  return THttpHeaderValueVariant();
  }

THTTPHdrVal HttpClientUtils::CopyHttpHdrVal(const THttpHeaderValueVariant& aVal, RStringPool& aStrPool)
  {
  switch(aVal.Type())
      {
      case THttpHeaderValueVariant::EIntType:
          {
          return THTTPHdrVal(aVal.Int());
          }
      
      
      case THttpHeaderValueVariant::EStrType:
          {
          RStringF str;
          TRAP_IGNORE(str = aStrPool.OpenFStringL(aVal.Str()));
          if(!((RStringTokenBase)str).IsNull())
              {
              return THTTPHdrVal(str);
              }
          break;
          }
      
      case THttpHeaderValueVariant::EDateTimeType:
          {
          return THTTPHdrVal(aVal.DateTime());
          }
      
    
      }
  return THTTPHdrVal();     
	}

void HttpClientUtils::CloseString(const THTTPHdrVal& aVal)
  {
  if(aVal.Type() == THTTPHdrVal::KStrVal)
      {
      aVal.Str().Close();
      return;
      }
  
  if(aVal.Type() == THTTPHdrVal::KStrFVal)
      {
      aVal.StrF().Close();
      return;
      }                
  }

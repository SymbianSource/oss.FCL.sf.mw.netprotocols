#
# Copyright (c) 2001-2008 Nokia Corporation and/or its subsidiary(-ies).
# All rights reserved.
# This component and the accompanying materials are made available
# under the terms of "Eclipse Public License v1.0"
# which accompanies this distribution, and is available
# at the URL "http://www.eclipse.org/legal/epl-v10.html".
#
# Initial Contributors:
# Nokia Corporation - initial contribution.
#
# Contributors:
#
# Description:
#
!/** 
!HTTP String Table
!@publishedAll 
!@released
!*/
fstringtable HTTP
# Internal Strings :
# Are _HHH where HHH is a 3 digit hex string
# Currently assigned upto 043
ECONNECT CONNECT
EDELETE DELETE
EGET GET
EHEAD HEAD
EOPTIONS OPTIONS
EPOST POST
EPUT PUT
ETRACE TRACE
# HTTP Versions
EHttp10 HTTP/1.0
EHttp11 HTTP/1.1
#
# Header field names
#
# GENERAL HTTP Headers
ECacheControl Cache-Control
EConnection Connection
EDate Date
EPragma Pragma
ETransferEncoding Transfer-Encoding
EUpgrade Upgrade
EVia Via
EWarning Warning
#
# REQUEST HTTP Headers
EHost Host
EAccept Accept
EAcceptCharset Accept-Charset
EAcceptEncoding Accept-Encoding
EAcceptLanguage Accept-Language
EAuthorization Authorization
ECookie Cookie
ECookie2 Cookie2
EExpect Expect
EFrom From
EIfModifiedSince If-Modified-Since
EIfMatch If-Match
EIfNoneMatch If-None-Match
EIfRange If-Range
EIfUnmodifiedSince If-Unmodified-Since
EKeepAlive	Keep-Alive
EMaxForwards Max-Forwards
EProxyAuthorization Proxy-Authorization
ERange Range
EReferer Referer
EUserAgent User-Agent
#
#RESPONSE HTTP Headers
EAcceptRanges Accept-Ranges
EAge Age
EETag ETag
ELocation Location
EProxyAuthenticate Proxy-Authenticate
ERetryAfter Retry-After
EServer Server
ESetCookie Set-Cookie
ESetCookie2 Set-Cookie2
EVary Vary
EWWWAuthenticate WWW-Authenticate
#
# ENTITIY HTTP Headers
EAllow Allow
EContentBase Content-Base
EContentDisposition Content-Disposition
EContentEncoding Content-Encoding
EContentLanguage Content-Language
EContentLength Content-Length
EContentLocation Content-Location
EContentMD5 Content-MD5
EContentRange Content-Range
EContentType Content-Type
EExpires Expires
ELastModified Last-Modified
# Standard token values, enumerations or parameters defined in RFC2616 to be used in header values.
#
EQ q
EGzip gzip
ECompress compress
EDeflate deflate
EBytes bytes
ENone none
ENoCache no-cache
ENoStore no-store
EMaxAge max-age
EMaxStale max-stale
EMinFresh min-fresh
ENoTransform no-transform
EOnlyIfCached only-if-cached
EPublic public
EPrivate private
EMustRevalidate must-revalidate
EProxyRevalidate proxy-revalidate
ESMaxAge s-maxage
EClose close
ECharset charset
E100Continue 100-continue
ETE	TE
ETrailer Trailer
EChunked chunked
EBasic Basic
EDigest Digest
ERealm realm
EDomain domain
ENonce nonce
EOpaque opaque
EStale stale
EAlgorithm algorithm
EQop qop
EAuth auth
EResponse response
EUri uri
ECnonce cnonce
ENc nc
E00000001 00000001
# Parameter names used internally by HTTP Client 6.2 only, in order to ease the parsing, but never transmitted
EPort _000
# Names of filters
EAuthentication _Auth
ERedirect	_Red
EValidation _Val
EProtocolHandler _ProtH
EClient Client
EHttpClientFilter	_clf
ESessionHeaderFilter	_shf
ECookieFilter _ckf
EHttpConnectFilter	_cnf
# Session connection information. Property names and values. Actual string values are unimportant since they are
# only used internally
EProxyUsage _001
EProxyAddress _002
EUseProxy _003
EDoNotUseProxy _004
EMaxNumTransportHandlers _005
EMaxNumTransPerTranspHndlr _006
EHTTPVersion _007
EMaxRedirect _008
EProtocol Protocol
EHTTP HTTP
EHTTPS HTTPS
EWSP WSP
# Session properties used to configure WSP capabilities
EWspCapClientMessageSize _009
EWspCapServerMessageSize _00A
EWspCapUseAcknowledgements _00B
EWspCapSupportSuspendResume _00C
EWspCapMaxOutstandingRequests _00D
# Session properties used to specify a proxy
EWspProxyAddress _00E
EWspBearer _00F
EWspIp _010
EWspSMS _011
EWspLocalPort _012
EWspRemotePort _013
EWspServiceNumber _014
EWspSessionType _015
EWspConnectionOriented _016
EWspConnectionLess _017
EWspSecurity _018
EWspUseWtls _019
EWspDoNotUseWtls _01A
# Wsp Misc properties
EWspProxyConnectionTimeout _01B
EWspProxyExceptionInfo _01C
# Wsp transaction properties
EWspRemainingSDUSize _01D
EWspRemainingMessageSize _01E
# Authentication information
EAuthenticatedOK _01F
EAuthenticationFailure _020
# Values used by filters
EUsername username
EPassword Password
EAuthenticationStatus Authentication:Status
ERedirectCount	_021
EProtTrans _022
#Mime types
EAnyAny */*
ETextAny text/*
ETextHtml text/html
ETextPlain text/plain
ETextVndWapWml text/vnd.wap.wml
EApplicationOctetStream application/octet-stream
EApplicationXWwwFormUrlEncoded application/x-www-form-urlencoded
# Values used for cookies
#
ECookieName _023
ECookieValue _024
EPath Path
EComment Comment
ECommentURL CommentURL
EDiscard Discard
ECookiePort Port
ESecure Secure
EVersion Version
# Values for Multi-homing properties
#
EHttpSocketServ _025
EHttpSocketConnection _026
# Value for Secure Dialog property
#
ESecureDialog _027
EDialogPrompt _028
EDialogNoPrompt _029
# Transaction property for CONNECT support
#
ETunnel	_02A
# Property name and values for pipelining support
#
EHttpPipelining	_02B
EEnablePipelining _02C
EDisablePipelining _02D
# Property name an value for Cipher Suite support
#
ECipherSuiteValue _02E
# Property name and value for pipelining batching support
#
EHttpBatching _2F
EDoNotEnableBatching _030
EEnableBatching _031
EBatchingBufferSize _32
# Property name and values for socket shutdown modes
#
ESocketShutdownMode _033
ESocketShutdownNormal _034
ESocketShutdownImmediate _035
ESessionClosing _036
#
EProfile Profile
EProfileDiff Profile-Diff
EUAProfFilter _UaProf
#
# Property name for pipelining control
#
EMaxNumTransactionsToPipeline _037
ESessionId _038
#
# Property name and value for disconnect notification
#
ENotifyOnDisconnect _03A
EEnableDisconnectNotification _03B
EDisableDisconnectNotification _03C
#
# Property to set the variable receive buffer size
#
ERecvBufferSize _03D
#
# Property to disable the optimiser for a particular transaction
#
EHTTPTransOptimiser _03E
EEnableHTTPTransOptimiser _03F
EDisableHTTPTransOptimiser _040
#
# NTLM authentication scheme
#
ENTLM NTLM
#
# Property name and value to notify a 100-Continue response to the client application
#
ENotify100Continue _041
EEnableNotification _042
EDisableNotification _043
#
# Property name and value to notify send and receive timeout to client application
#
ESendTimeOutValue _051
EReceiveTimeOutValue _052
ETranspHndlrPriority _053
EEnableTranspHndlrPriority _054
EDisableTranspHndlrPriority _055
#
# Property name and value to enable strict CONNECT method header checking the HTTP Stack 
#
EStrictConnectHeaders _056
EEnableStrictConnectHeaders _057
#
# Property name and value for the optimal pipelining
# 
EHttpOptimalPipelining _061
EHttpEnableOptimalPipelining _062

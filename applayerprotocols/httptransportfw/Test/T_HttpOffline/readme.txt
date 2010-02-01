T_HTTPOFFLINE

This is a test suite of all the offline unit tests for HTTP. It consists of the following tests

1 Core
2 Form Encoding
3 Header Codes
4 RespParse
5 HTTPhdr
6 TextModeHeaderFilter

1 Core
---------------
Tests basic functionality

1.1 Creating a session
1.2 Adding filters to the session
1.3 Creating a transaction, 
1.4 Checking and adding a body to the transaction
1.5 Checking and altering uri to the transaction
1.6 Checking and altering status code of the transaction
1.7 Deleting the transaction 
1.8 Altering the connection info of the session

2 Form Encoding
---------------
Tests CHTTPFormEncoder class.

2.1 Test Single name pair values
2.2 Test Empty name fails correctly
2.3 Test various combinations of name pair values 


3 Header Codes
---------------
Tests encoding and decoding of http headers

3.1  Test encoding of accept header
3.2  Test encoding of accept charset header
3.3  Test encoding of authorization header
3.4  Test encoding of connection header
3.5  Test encoding of content length header
3.6  Test encoding of content type header
3.7  Test encoding of host header
3.8  Test encoding of transfer encoding header
3.9  Test encoding of user agent header
3.10 Test decoding of connection header
3.11 Test decoding of content length header
3.12 Test decoding of content length header
3.13 Test decoding of server header
3.14 Test decoding of date header
3.15 Test decoding of transfer encoding header
3.16 Test decoding of WWWAuthenticate header
3.17 Test round trip encoding and decoding of connection header
3.18 Test round trip encoding and decoding of content length header
3.19 Test round trip encoding and decoding of content type header
3.20 Test round trip encoding and decoding of transfer encoding header
3.21 Test round trip encoding and decoding of invalid content length headers

4 RespParse
-----------

Validates the parsing of response headers

1. A single complete set of headers
2. A status line split across two buffers
3. Split across several buffers + a simple response
4. Full response, non-chunked, split over several buffers
5. Chunked response
6. Single buffer containing a HEAD response from Apache
7. Single buffer containing a HEAD response from IIS
8. Entire response in one buffer; test will iterate through it in increasing buffer sizes


5 HTTPhdr 
---------

Validates the internal representation of headers and their corresponding values

1. Accept: text/html
2. Expires: Mon, 15 Jan 2001 10:30:25 GMT
3. Max-Forwards: 3
4. Accept-Charset: us-ascii, utf-8
5. Accept-Language: en-gb. fr
6. Accept: text/html; q=0.8, text/plain; q=0.2, */*

6 TextModeHeaderFilter
----------------------

Tests the Connection header

1. Connection:close
2. bogus (accept) field to the connection header , should get removed
3. hop by hop header to the connection header, should stay
4. bogus (accept) field to the connection header and a hop by hop header

7 Filters
---------

Tests the filter mechanism with different events together with the order in which filters must handle events. The events tested with the filters are:

1. Handling of error events
2. Testing session delete/leave interaction
3. Deleting from a RunError
4. Testing status code matching with no response
5. Testing status code matching - codes do match
6. Testing status code matching - codes dont match
7. Testing header matching - correct header not present
8. Testing header matching - correct header present


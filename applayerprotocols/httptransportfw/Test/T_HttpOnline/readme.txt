T_HTTPONLINE

This is a test suite of all the online unit tests for HTTP. It consists of the following tests

1 Single POST transaction, with request body submitted in chunks
2 Multiple simultaneous transactions
3 Persistent connections
4 Proxies
5 TCP Transport Handler
6 TCP Transport Handler Server Time-out
7 Response Chunk Size variation
8 Cancellation of single transactions
9 Cancellation of multiple transactions



1 Single POST transaction, with request body submitted in chunks
----------------------------------------------------------------
Requires: http://<test server>/perl/dumpform.pl

Tests client API, protocol handler and transport handler mechanisms for transmitting a request
whose body is submitted by the client in three data chunks, each separated by a time interval

1.1 Runs the test using a 1 second delay between each chunk



2 Multiple simultaneous transactions
------------------------------------
Requires: http://<test server>/http_tests/prothnd/1.txt through 8.txt

Launches a number of transactions in parallel, each to a different URL.  The response body data
is checked to ensure that each block of data is associated with the correct transaction.  This is
easily done by using test files where a repeated single character is present in each file - i.e.
1.txt contains repeated '1' characters, etc.

2.1 Test 1 transaction
2.2 Test 2 concurrent transactions
2.3 Test 3 concurrent transactions
2.4 Test 4 concurrent transactions
2.5 Test 5 concurrent transactions
2.6 Test 6 concurrent transactions
2.7 Test 7 concurrent transactions
2.8 Test 8 concurrent transactions



3 Persistent Connections
------------------------
Requires: http://<test server>/perl/dumpform.pl

Launches a number of transactions sequentially.  Each one is identical (ie. the same method and URL)
but the last in a sequence includes the 'Connection: close' header.  Since HTTP/1.1 is used for all
transactions the sequence should re-use a single socket connection.  This must be verified by
checking the log-file C:\logs\http\httptcphnd.txt manually; the message 'Socket closed by local host'
or 'Socket closed by remote host' should only be seen following the final transaction in the sequence

3.1 Test 1 transaction
3.2 Test 2 sequential transactions
3.3 Test 3 sequential transactions
3.4 Test 4 sequential transactions
3.5 Test 5 sequential transactions



4 Proxies
---------
Requires:  http://<test server>/perl/dumpform.pl  and a proxy at 10.35.2.4:9003

Tests the protocol handler's ability to form correct requests based on the session's proxy settings,
the presence/absence of a Host header and the type of URL.

4.1 Absolute URL; compatible Host header; proxy on
4.2 Absolute URL; incorrect Host header; proxy on
4.3 Absolute URL; missing Host header; proxy on
4.4 Relative URL; correct Host header; proxy on
4.5 Relative URL; missing Host header; proxy on
4.6 Absolute URL; compatible Host header; proxy off
4.7 Absolute URL; incorrect Host header; proxy off
4.8 Absolute URL; missing Host header; proxy off
4.9 Relative URL; correct Host header; proxy off
4.10 Relative URL; missing Host header; proxy off



5 TCP Transport Handler
-----------------------
Requires: http://<test server 1>/http_tests/tcphnd/testxy.html where x := 1->15 and y := a|b
      and http://<test server 2>/http_tests/tcphnd/testxy.html where x := 1->15 and y := a|b

Tests the TCP Transport Handler's ability to correctly queue submissions and maintain the socket
connection appropriately.  This is done by issuing individual and paired requests in a variety of
situations - HTTP/1.0 or HTTP/1.1 mode; persistent or non-persistent; queued or separate; same or
different hosts.

       | HTTP Level | Persistent? | No. requests | use queuing? | same host?
-------+------------+-------------+--------------+--------------+------------
5.1    |     1.0    |      /      |       1      |       /      |      /
5.2    |     1.0    |      /      |       2      |       y      |      y
5.3    |     1.0    |      /      |       2      |       y      |      n
5.4    |     1.0    |      /      |       2      |       n      |      y
5.5    |     1.0    |      /      |       2      |       n      |      n
5.6    |     1.1    |      n      |       1      |       /      |      /
5.7    |     1.1    |      n      |       2      |       y      |      y
5.8    |     1.1    |      n      |       2      |       y      |      n
5.9    |     1.1    |      n      |       2      |       n      |      y
5.10   |     1.1    |      n      |       2      |       n      |      n
5.11   |     1.1    |      y      |       1      |       /      |      /
5.12   |     1.1    |      y      |       2      |       y      |      y
5.13   |     1.1    |      y      |       2      |       y      |      n
5.14   |     1.1    |      y      |       2      |       n      |      y
5.15   |     1.1    |      y      |       2      |       n      |      n



6 TCP Transport Handler Server Time-out
---------------------------------------
Requires: http://<test server>/http_tests/tcphnd/test12a.html
      and http://<test server>/http_tests/tcphnd/test12b.html

Tests the TCP Transport Handler's ability to correctly detect the end of a persistent connection
when the server times out and drops it.  With Apache, this seems to happen at around 17s but this
may vary; so the first two or three tests should use a persistent connection and the remainder will
make a new connection for the second transaction.

       | HTTP Level | Persistent? | No. requests | use queuing? | same host? | time-out between trans
-------+------------+-------------+--------------+--------------+------------+------------------------
6.1    |     1.1    |      y      |       2      |       y      |      y     |            5
6.2    |     1.1    |      y      |       2      |       y      |      y     |           10
6.3    |     1.1    |      y      |       2      |       y      |      y     |           15
6.4    |     1.1    |      y      |       2      |       y      |      y     |           20
6.5    |     1.1    |      y      |       2      |       y      |      y     |           25




7 Response Chunk Size variation
-------------------------------
Requires: (EPOC) C:\req1.txt and C:\resp1.txt - which contain a dummy SyncML request and the
          matching response to send if the request is correctly validated .

The test uses a dummy proxy server running on the EPOC device (as part of the test harness) to check
that the client request is valid, and to return an appropriate response if it is.  The proxy is
configured by the test harness to return the response in data chunks of a preset size.  This test
exercises the protocol handler's independence of operation from the size of the data chunks being
sent to it.

6.1 Chunk size = 8 bytes
6.2 Chunk size = 16 bytes
6.3 Chunk size = 24 bytes
6.4 Chunk size = 32 bytes
...
6.30 Chunk size = 240 bytes
6.31 Chunk size = 248 bytes
6.32 Chunk size = 256 bytes



8 Cancellation of single transactions
-------------------------------------
Requires: http://<test server>/http_tests/prothnd/rfc2616.txt

The test issues a transaction then cancels it after a preset time interval.  This time interval is
initially set small then gradually increased, to cover a variety of points in the transaction
processing.

7.1 Interval length = 50 ms
7.2 Interval length = 100 ms
7.3 Interval length = 150 ms
7.4 Interval length = 200 ms
...
7.48 Interval length = 2400 ms
7.49 Interval length = 2450 ms
7.50 Interval length = 2500 ms



9 Cancellation of multiple transactions
---------------------------------------
Requires: http://<test server>/http_tests/prothnd/1.txt through 9.txt

Launches a number of transactions in parallel, each to a different URL.  One of the transactions
will be cancelled after a particular time interval.  The data received from the remaining
transactions is checked to ensure that no data is obtained from the cancelled transaction.

2.1  Test with: 1 transaction launched;    1st transaction gets cancelled after 50 ms
2.2  Test with: 1 transaction launched;    1st transaction gets cancelled after 100 ms
2.3  Test with: 2 transactions launched;   1st transaction gets cancelled after 50 ms
2.4  Test with: 2 transactions launched;   1st transaction gets cancelled after 100 ms
2.5  Test with: 2 transactions launched;   2nd transaction gets cancelled after 50 ms
2.6  Test with: 2 transactions launched;   2nd transaction gets cancelled after 100 ms
2.7  Test with: 3 transactions launched;   1st transaction gets cancelled after 50 ms
2.8  Test with: 3 transactions launched;   1st transaction gets cancelled after 100 ms
2.9  Test with: 3 transactions launched;   2nd transaction gets cancelled after 50 ms
2.10 Test with: 3 transactions launched;   2nd transaction gets cancelled after 100 ms
2.11 Test with: 3 transactions launched;   3rd transaction gets cancelled after 50 ms
2.12 Test with: 3 transactions launched;   3rd transaction gets cancelled after 100 ms
...
2.58 Test with: 9 transactions launched; 7th transaction gets cancelled after 100ms
2.59 Test with: 9 transactions launched; 8th transaction gets cancelled after 50ms
2.60 Test with: 9 transactions launched; 8th transaction gets cancelled after 100ms

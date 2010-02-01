IMPORTANT:

Note, when testing the WSP Protocol Handler using the T_WspProtHnd test harness, it is essential that the DEBUG variant of the protocol handler DLL (WSPPROTOCOLHANDLER.DLL) is used.  The test harness will NOT run correctly in conjunction with the WSP Transport Handler test stub if the release variant is used.

This warning applies to all test platforms (WINS, ARMI etc.), for both standard tests and OOM tests.

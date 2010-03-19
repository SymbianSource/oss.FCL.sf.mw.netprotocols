File        :     README.TXT
                  Copyright (c) 1998-2010 Nokia Corporation and/or its subsidiary(-ies).
Abstract    :     Overview of the design of the Symbian Telnet implementation
                  and miscellaneous handover notes for BETA Release 1.0

1)  Overall Structure
The Symbian Telnet (Telsess.dll) export entry points are declared in telsess.h
and described in TelApi.Doc .There is a basic sample Telnet Client TTelnet.exe
with Source code that demonstrates most of the API calls.

2)  Class Overview
2.1) CTelnetSession
Main derivation from CBase
Top level class created by client applications.
Creates the CTelnetControl control object and contains the export entry
points for client applications.

2.2) CTelnetControl
Main derivation from CBase
Created by CTelnetSession.
The main Symbian Telnet control object handles client calls/callbacks and
generally coordinates Symbian Telnet operation.
Creates instances of :-

CTelnetResolver
CProto
CIOBufferControl
CActiveWriter
CActiveReader

2.3) CTelnetResolver
Main derivation from CActive
Handles connection/disconnection to Telnet Server using state machine
Owns the Socket handle 

2.4) CActiveWriter
Main derivation from CActive
Handles Writes to the Socket

2.5) CActiveReader
Main derivation from CActive
Handles Reads from the Socket

2.6) CIOBufferControl
Main derivation from CBase
Provides buffering between the ActiveRead/Write objects and the
CTelnetControl control object.

2.7) CProto
Main derivation from CBase
The Telnet Protocol handler coordinates RFC 854
Manipulates data stream to and from the Telnet Server.
ALL calls into this object are effectively synchronous.
Contains the main Server receive state machine.

Creates instances of :-

CBinaryOption            RFC 856
CWindowSizeOption        RFC 1073
CTerminalTypeOption      RFC 1091
CTerminalSpeedOption     RFC 1079
CSuppressGAOption        RFC 858
CStatusOption            RFC 859
CEchoOption              RFC 857
CLogoutOption            RFC 727
CUnknownOption

The above RFC classes are derived from CRFCOptionBase which conatins an 
option negotiation state machine as described in RFC 1143

3)  Creation Heirarchy
                               CTelnetSession
                                     |
                                     |            
                              CTelnetControl
                                     |            
      |--------------|---------------|--------------|----------------|
      |              |               |              |                |
      |              |               |              |                |
 CActiveReader  CActiveWriter     CProto    CTelnetResolver  CIOBufferControl
                                     |
                                     |                               
                               CBinaryOption
                             CWindowSizeOption
                            CTerminalTypeOption
                            CTerminalSpeedOption
                              CSuppressGAOption
                                CStatusOption
                                CEchoOption
                               CLogoutOption
                               CUnknownOption

4) File Overview
4.1)  Telsess.cpp & Telsess.h
Symbian Telnet DLL export methods

4.2)  Telcontrol.cpp & Telcontrol
Code & declarations for the Symbian Telnet control class CTelnetControl

4.3)  Telfsm.cpp & Telfsm.h
Implementation of all the Telnet RFC's

4.4)  Telresol.cpp & Telresol.h
Code & declarations for the Symbian Telnet Resolver class CTelnetResolver

4.5)  IOBuffers.cpp & IOBuffers.h
Code & declarations for the intermmediate buffering class CIOBufferControl 

4.6)  ActiveIO.cpp & ActiveIO.h
Code & declarations for the Socket write and Socket read Active Objects
CActiveReader, CActiveWriter 



5) Connect/Disconnect overview
Data paths travelling from Server to the Client contain notification
(M)Interface callbacks.

5.1) Connect to Server

               Telnet Client            Telnet Client
                    |                         ^
                    |                         |
           CTelnetSession::Connect   (M) CTelnetSession::Connected
                    |                         |
                    |                         |
           CTelnetControl::Connect  (M) CTelnetControl::ResolverConnected           
                    |                         |
                    |                         |
     CTelnetResolver::IssueConnect     CTelnetResolver Async Completion   
                    |                         |
                    V                         |
                    -------Telnet Server------|

5.2) Disconnect by Server
               
               Telnet Server
                    |
                    |
  CTelnetResolver notified of EOF on Socket by CActiveReader
                    |
                    |
      (M) CTelnetControl::ResolverDisconnected
                    |
                    |
      (M) CTelnetSession::ConnectionClosed
                    |
                    V
               Telnet Client


5.3) Disconnect by Client
                   

               Telnet Client             Telnet Client      
                    |                         ^
                    |                         |
    CTelnetSession::Disconnect   (M) CTelnetSession::ConnectionClosed
                    |                         |
                    |                         |
       CTelnetControl::Disconnect (M) CTelnetControl::ResolverDisconnected
                    |                         |
                    |                         |
 CTelnetResolver::IssueDisconnect    CTelnetResolver notified of EOF on Socket
                    |                         |
                    V                         |
                    -------Telnet Server------|
       

6) Data Transfer
Data paths travelling from Server to the Client contain notification
(M)Interface callbacks.
6.1) Write to Server
               
                Telnet Client          Telnet Client
                    |                        ^
                    |                        |
           CTelnetSession::Write      (M) CTelnetSession::WriteComplete
                    |                        ^
                    |                        |
           CTelnetControl::Write -->  (M) CTelnetControl::WriteComplete
                    |       ^       |        |
                    V       |       |        |        
                 CProto::ProtoWrite |        |
                                    |        |
                                    |        |
                    |---------------|        |
                    |                        |
        CIOBufferControl::Write      (M) CIOBufferControl::WriteComplete
                    |                        |
                    |                        |
   CActiveWriter::IssueWrite      CActiveWriter Completes Asynchronously
                    |                        |
                    V                        |
                    ------Telnet Server------|



6.2) Read from Server

                Telnet Client                       Telnet Client
                    |                                     ^
                    |                                     |
           CTelnetSession::Read              (M) CTelnetSession::ReadComplete
                    |                                     |           
                    |                                     |           
                    |          CProto::ProtoRead--->|     |
                    |                        |      |     |
                    |                        |      V     |
           CTelnetSession::Read    (M) CTelnetControl::ReadComplete 
                    |                        |
                    |                        |
           CIOBufferControl::Read  (M) CIOBufferControl::ReadComplete
                    |                        |
                    |                        |
     CActiveReader::IssueRead      CActiveReader Completes Asynchronously
                    |                        |
                    V                        |
                    ------Telnet Server------|


7) Sample Telnet Client
There are only 2 files in this project TTelnet.cpp and TTelnet.h .
This code is not perfect but is fairly easy to understand.

8) Miscellaneous Issues

8.1) Debugs
Debugging has been left in, mainly for Telnet Protocol events. This can be easily removed
when it's no longer useful.

8.2) Testing in Binary Mode
Binary mode is successfully negotiated if a Terminal Type other than "dumb" is selected,
however, we do not have a Terminal emulation to test Binary mode properly.
I have done basic testing to make sure the permitted control signals work in Binary mode
and have carefully desk checked the code that handles the RFC 854 defined behaviour for
receive and transmit in Binary.
As soon as a Terminal emulation, vt100 for example, is "plugged in" over the top of the
Symbian Telnet API, it should be fairly easy to test that Binary mode is working correctly
using "vi" or other Unix applications that require more complex client terminal formatting.

8.3) Finishing Off

i) Error Checks
I need to "Epocise" the code for checking buffer overflows etc

ii) Safe Objects
I'm not sure whether I need to implement things like safe copy constructors.

iii) Error Notifications 
I have not looked into to every single error that can occur on sockets and the resolver,
therefore I've not documented all the error codes that could be passed up to the client.

iv) Coding Standards
Code needs to be checked against the coding standards by myself and then submitted for
review

v) Destructive Testing
Anything I/we can think of to break it.

vi)  Final Documentation
Finish off and make the API official.


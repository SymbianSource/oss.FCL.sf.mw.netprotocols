@rem
@rem Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
@rem All rights reserved.
@rem This component and the accompanying materials are made available
@rem under the terms of "Eclipse Public License v1.0"
@rem which accompanies this distribution, and is available
@rem at the URL "http://www.eclipse.org/legal/epl-v10.html".
@rem
@rem Initial Contributors:
@rem Nokia Corporation - initial contribution.
@rem
@rem Contributors:
@rem
@rem Description:
@rem

copy \epoc32\release\winscw\udeb\ineturilistserver.exe   \epoc32\release\winscw\udeb\Z\sys\bin
copy \epoc32\release\winscw\urel\ineturilistserver.exe   \epoc32\release\winscw\urel\Z\sys\bin
if exist "\epoc32\winscw\c\sys" rmdir /S /Q \epoc32\winscw\c\sys
testexecute.exe c:\testdata\scripts\TestWListBListUriSuite.script
testexecute.exe c:\testdata\scripts\TestWListBListUriSuite_AddMoreUris.script

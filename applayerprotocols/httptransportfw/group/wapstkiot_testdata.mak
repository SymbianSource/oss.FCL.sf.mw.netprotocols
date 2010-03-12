#
# Copyright (c) 2009-2010 Nokia Corporation and/or its subsidiary(-ies).
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

GENERATED_FILES= \
	$(EPOCROOT)epoc32\wins\c\httptest\wapstkiot_testdata.zip

$(EPOCROOT)epoc32\wins\c\httptest\wapstkiot_testdata.zip : ..\test\data\wapstkiot\DO_NOT_REMOVE
	cd ..\test\data\ini\wapstkiot_testdata 
	$(EPOCROOT)epoc32\tools\zip.exe -q -R $(EPOCROOT)epoc32\wins\c\httptest\wapstkiot_testdata.zip *.*
	$(EPOCROOT)epoc32\tools\unzip.exe -qq -o $(EPOCROOT)epoc32\wins\c\httptest\wapstkiot_testdata.zip -d $(EPOCROOT)epoc32\wins\c
	cd ..\..\..\..\..\os\unref\orphan\comgen\group

do_nothing:
	@rem do nothing

#
# The targets invoked by bld...
#

MAKMAKE : $(GENERATED_FILES)

BLD : MAKMAKE

SAVESPACE : MAKMAKE

CLEAN : 
	erase $(GENERATED_FILES) $(EPOCROOT)epoc32\wins\c\httptest\wapstkiot_testdata.zip

FREEZE : do_nothing

LIB : do_nothing

CLEANLIB : do_nothing

RESOURCE : do_nothing

FINAL : do_nothing

RELEASABLES : $(GENERATED_FILES)

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
	$(EPOCROOT)epoc32\wins\c\httptest\t_wspprothnd_driverdata.zip $(EPOCROOT)epoc32\wins\c\httptest\t_wspprothnd_stubdata.zip 

$(EPOCROOT)epoc32\wins\c\httptest\t_wspprothnd_driverdata.zip : ..\test\data\t_wspprothnd\DO_NOT_REMOVE
	cd ..\test\data\ini\wsp_pr_hnd_driver 
	$(EPOCROOT)epoc32\tools\zip.exe -q $(EPOCROOT)epoc32\wins\c\httptest\t_wspprothnd_driverdata.zip *.*
	$(EPOCROOT)epoc32\tools\unzip.exe -qq -o $(EPOCROOT)epoc32\wins\c\httptest\t_wspprothnd_driverdata.zip -d $(EPOCROOT)epoc32\wins\c\httptest\wsp_pr_hnd_driver
	cd ..\..\..\..\..\os\unref\orphan\comgen\group

$(EPOCROOT)epoc32\wins\c\httptest\t_wspprothnd_stubdata.zip : ..\test\data\t_wspprothnd\DO_NOT_REMOVE
	cd ..\test\data\ini\wsp_tr_hnd_stub 
	$(EPOCROOT)epoc32\tools\zip.exe -q $(EPOCROOT)epoc32\wins\c\httptest\t_wspprothnd_stubdata.zip *.*
	$(EPOCROOT)epoc32\tools\unzip.exe -qq -o $(EPOCROOT)epoc32\wins\c\httptest\t_wspprothnd_stubdata.zip -d $(EPOCROOT)epoc32\wins\c\httptest\wsp_tr_hnd_stub
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
	erase $(GENERATED_FILES) $(EPOCROOT)epoc32\wins\c\httptest\t_wspprothnd_driverdata.zip $(EPOCROOT)epoc32\wins\c\httptest\t_wspprothnd_stubdata.zip

FREEZE : do_nothing

LIB : do_nothing

CLEANLIB : do_nothing

RESOURCE : do_nothing

FINAL : do_nothing

RELEASABLES : $(GENERATED_FILES)

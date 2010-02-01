#
# Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
# All rights reserved.
# This component and the accompanying materials are made available
# under the terms of the License "Eclipse Public License v1.0"
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
	$(EPOCROOT)epoc32\build\generated\http\WspParamConstants.cpp \
	$(EPOCROOT)epoc32\include\WspParamConstants.h

$(EPOCROOT)epoc32\build\generated\http\WspParamConstants.cpp : ..\strings\WspParamConstants.st
	perl -S ecopyfile.pl ..\strings\WspParamConstants.st $(EPOCROOT)epoc32\build\generated\http\WspParamConstants.st
	perl $(EPOCROOT)epoc32\tools\stringtable.pl $(EPOCROOT)epoc32\build\generated\http\WspParamConstants.st

$(EPOCROOT)epoc32\include\WspParamConstants.h : $(EPOCROOT)epoc32\build\generated\http\WspParamConstants.cpp
	perl -S ecopyfile.pl $(EPOCROOT)epoc32\build\generated\http\WspParamConstants.h $(EPOCROOT)epoc32\include\WspParamConstants.h

ifeq "$(OS)" "Windows_NT"
ERASE := @erase 2>>nul
else
ERASE := @erase
endif

do_nothing:
	@rem do nothing

#
# The targets invoked by bld...
#

MAKMAKE : $(GENERATED_FILES)

BLD : MAKMAKE

SAVESPACE : MAKMAKE

CLEAN : 
	-$(ERASE) $(GENERATED_FILES) $(EPOCROOT)epoc32\build\generated\http\WspParamConstants.h

FREEZE : do_nothing

LIB : do_nothing

CLEANLIB : do_nothing

RESOURCE : do_nothing

FINAL : do_nothing

RELEASABLES : 
	@echo $(EPOCROOT)epoc32\include\WspParamConstants.h

ROMFILE : do_nothing
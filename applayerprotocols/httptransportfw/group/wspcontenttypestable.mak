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
	$(EPOCROOT)epoc32\build\generated\http\WspContentTypes.cpp \
	$(EPOCROOT)epoc32\include\WspContentTypes.h

$(EPOCROOT)epoc32\build\generated\http\WspContentTypes.cpp : ..\strings\WspContentTypes.st
	perl -S ecopyfile.pl ..\strings\WspContentTypes.st $(EPOCROOT)epoc32\build\generated\http\WspContentTypes.st
	perl $(EPOCROOT)epoc32\tools\stringtable.pl $(EPOCROOT)epoc32\build\generated\http\WspContentTypes.st

$(EPOCROOT)epoc32\include\WspContentTypes.h : $(EPOCROOT)epoc32\build\generated\http\WspContentTypes.cpp
	perl -S ecopyfile.pl $(EPOCROOT)epoc32\build\generated\http\WspContentTypes.h $(EPOCROOT)epoc32\include\WspContentTypes.h

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
	-$(ERASE) $(GENERATED_FILES) $(EPOCROOT)epoc32\build\generated\http\WspContentTypes.h

FREEZE : do_nothing

LIB : do_nothing

CLEANLIB : do_nothing

RESOURCE : do_nothing

FINAL : do_nothing

RELEASABLES : 
	@echo $(EPOCROOT)epoc32\include\WspContentTypes.h

ROMFILE : do_nothing
#
# Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
# Build upgrade SIS file

SISNAME=ineturilist_upgrade
SRCDIR=.\

# Select appropriate directory and ensure it exists

!if "$(PLATFORM)"=="WINS" || "$(PLATFORM)"=="WINSCW"
TARGETDIR=$(EPOCROOT)EPOC32\RELEASE\$(PLATFORM)\$(CFG)\Z\Private\20009d70

!else
TARGETDIR=$(EPOCROOT)EPOC32\Data\Z\Private\20009d70

!endif

$(TARGETDIR) :
	@perl -S emkdir.pl "$(TARGETDIR)" 

# Build upgrade SIS file

SISFILE= $(TARGETDIR)\$(SISNAME).sis

$(SISFILE) : $(SRCDIR)\$(SISNAME).pkg
	makesis $? $@

do_nothing :
	rem do_nothing

# The targets invoked by abld

MAKMAKE : do_nothing

RESOURCE : $(TARGETDIR) $(SISFILE)

SAVESPACE : BLD

BLD : do_nothing

FREEZE : do_nothing

LIB : do_nothing

CLEANLIB : do_nothing

FINAL : do_nothing
CLEAN :
	erase $(SISFILE)

RELEASABLES :
	@echo $(SISFILE)
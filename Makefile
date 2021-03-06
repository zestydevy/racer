#! make -f
#----------------------------------------------------------------------------
# Project:  NITRO-System - build - demos - g3d - samples - simple
# File:     Makefile
#
# Copyright 2004 Nintendo.  All rights reserved.
#
# These coded instructions, statements, and computer programs contain
# proprietary information of Nintendo of America Inc. and/or Nintendo
# Company Ltd., and are protected by Federal copyright law.  They may
# not be disclosed to third parties or copied or duplicated in any form,
# in whole or in part, without the prior written consent of Nintendo.
#
# $Log: Makefile,v $
# Revision 1.4  09/06/2004 02:22:00  hatamoto_minoru
# Please do not use .rsf file
#
# Revision 1.3  08/31/2004 02:53:21  nishida_kenji
# build intermediate files.
#
# Revision 1.3  08/31/2004 02:04:48  nishida_kenji
# build intermediate files.
#
# Revision 1.1  08/17/2004 10:50:49  nishida_kenji
# Initial check in.
#
# $NoKeywords: $
#----------------------------------------------------------------------------

G3D_DEMOLIB		=	$(NITROSYSTEM_ROOT)/build/demos/g3d/demolib

LINCLUDES       =   $(G3D_DEMOLIB)/include
LLIBRARY_DIRS   =   $(G3D_DEMOLIB)/lib/$(NITRO_BUILDTYPE)
LLIBRARIES      =   libg3d_demo.a

#----------------------------------------------------------------------------

SUBDIRS	= 

SRCS		=	main.cpp actor.cpp game.cpp kart.cpp gamepad.cpp camera.cpp file.cpp gfx.cpp math.cpp
TARGET_NEF	=	main.nef
TARGET_BIN	=	main.srl

MAKEROM_ROMROOT  = ./
MAKEROM_ROMFILES = $(G3D_DATABIN)*.*

G3D_DATABIN = data/
G3D_DATASRC = data/src

G3D_IMD = data/src/yoshi.imd data/src/track.imd data/src/sky.imd
G3D_ICA =
G3D_IVA =
G3D_IMA =
G3D_ITP =
G3D_ITA =

G3D_NSBMD_TMP = $(G3D_IMD:.imd=.nsbmd)
G3D_NSBCA_TMP = $(G3D_ICA:.ica=.nsbca)
G3D_NSBVA_TMP = $(G3D_IVA:.iva=.nsbva)
G3D_NSBMA_TMP = $(G3D_IMA:.ima=.nsbma)
G3D_NSBTP_TMP = $(G3D_ITP:.itp=.nsbtp)
G3D_NSBTA_TMP = $(G3D_ITA:.ita=.nsbta)

G3D_NSBMD = $(subst src/,,$(G3D_NSBMD_TMP))
G3D_NSBCA = $(subst src/,,$(G3D_NSBCA_TMP))
G3D_NSBVA = $(subst src/,,$(G3D_NSBVA_TMP))
G3D_NSBMA = $(subst src/,,$(G3D_NSBMA_TMP))
G3D_NSBTP = $(subst src/,,$(G3D_NSBTP_TMP))
G3D_NSBTA = $(subst src/,,$(G3D_NSBTA_TMP))

LDIRT_CLEAN = $(G3D_NSBMD) $(G3D_NSBCA) $(G3D_NSBVA) $(G3D_NSBMA) $(G3D_NSBTP) $(G3D_NSBTA)

#----------------------------------------------------------------------------
include	$(NITROSYSTEM_ROOT)/build/buildtools/commondefs


do-build: binData $(TARGETS)
binData: $(G3D_NSBMD) $(G3D_NSBCA) $(G3D_NSBVA) $(G3D_NSBMA) $(G3D_NSBTP) $(G3D_NSBTA)
%.nsbmd: $(subst data/,data/src/,$(@:.nsbmd=.imd))
	$(NITROSYSTEM_ROOT)/tools/win/bin/g3dcvtr.exe $(subst data/,data/src/,$(@:.nsbmd=.imd)) -o $@
%.nsbca: $(subst data/,data/src/,$(@:.nsbca=.ica))
	$(NITROSYSTEM_ROOT)/tools/win/bin/g3dcvtr.exe $(subst data/,data/src/,$(@:.nsbca=.ica)) -o $@
%.nsbva: $(subst data/,data/src/,$(@:.nsbva=.iva))
	$(NITROSYSTEM_ROOT)/tools/win/bin/g3dcvtr.exe $(subst data/,data/src/,$(@:.nsbva=.iva)) -o $@
%.nsbma: $(subst data/,data/src/,$(@:.nsbma=.ima))
	$(NITROSYSTEM_ROOT)/tools/win/bin/g3dcvtr.exe $(subst data/,data/src/,$(@:.nsbma=.ima)) -o $@
%.nsbtp: $(subst data/,data/src/,$(@:.nsbtp=.itp))
	$(NITROSYSTEM_ROOT)/tools/win/bin/g3dcvtr.exe $(subst data/,data/src/,$(@:.nsbtp=.itp)) -o $@
%.nsbta: $(subst data/,data/src/,$(@:.nsbta=.ita))
	$(NITROSYSTEM_ROOT)/tools/win/bin/g3dcvtr.exe $(subst data/,data/src/,$(@:.nsbta=.ita)) -o $@


include	$(NITROSYSTEM_ROOT)/build/buildtools/modulerules


#===== End of Makefile =====


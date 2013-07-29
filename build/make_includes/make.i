# ------------------------------------------------------------------------- #
# Copyright 2013 Esri                                                       #
#                                                                           #
# Licensed under the Apache License, Version 2.0 (the "License");           #
# you may not use this file except in compliance with the License.          #
# You may obtain a copy of the License at                                   #
#                                                                           #
#     http://www.apache.org/licenses/LICENSE-2.0                            #
#                                                                           #
# Unless required by applicable law or agreed to in writing, software       #
# distributed under the License is distributed on an "AS IS" BASIS,         #
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  #
# See the License for the specific language governing permissions and       #
# limitations under the License.                                            #
# ------------------------------------------------------------------------- #

# ------------------------------------------------------------------------
#
# This is our common rules file for GNU make.
# This make-rules file & the associated makefiles will work in either
# Windoze or Unix.
#
# ------------------------------------------------------------------------
#
# Notes:
#
# 1.  It is expected that the environment variable $TOP_DIR is already
#     set to point to the top-level directory.  This is done manually
#     in the various makefiles.
#
# 2.  The build mode is set by the environment variable $BUILD_MODE.
#     This variable, if defined, is expected to have the values of
#     "debug", "release", or "profile".  If it is not defined, "debug"
#     is assumed.
#
# 3.  If doing a "debug" build, the macro _DEBUG is asserted.
#
# 4.  Windoze build notes:
#
#     1.  All modules, regardless of the BUILD_MODE, are built with the
#         option to create a PDB (program database) file, which contains
#         all the debugging information for the module.  Note that we
#         build a full PDB file for our debugging purposes, and a
#         stripped PDB file for the SDK packages.
#
#     2.  All libraries and executables are NOT incrementally linked.
#
# ------------------------------------------------------------------------
#
# When linking executables, the following options are available:
#
#    LIB_TYPE       dynamic    Use dynamic libraries (default)
#                   static     Use static  libraries (default if profile mode)
#
#    LIB_MODE       separate   Use separate libs
#                   combined   Use combined PE libs  (default)
#
#    VERBOSE_MAKE              If set, we get the full compile/link line.
#                              Otherwise, we get an abbreviated line.
#
#    KEEP_MANIFEST_FILES       If set, manifest files are kept.
#                              Otherwise, they are deleted after being
#                              merged into the DLL or EXE.
#
# ------------------------------------------------------------------------
#
# IMPORTANT - Before using these files, be sure to make the sign of the
#             Berghaus Star...
#
# ------------------------------------------------------------------------

MAKEFLAGS += --no-print-directory

# ------------------------------------------------------------------------
# Determine our architecture.  Our only source for this info is
# the OS_ARCH environment variable.
#
# Choices are:
#    32   32-bit (Default)
#    64   64-bit
#
ifndef OS_ARCH
   OS_ARCH   := 32
endif

# ------------------------------------------------------------------------
# Determine our OS type and platform.
#
# The OS platform comes from the output of 'uname -s' command, after
# converting the string to lower-case and removing any non-alphanumeric
# characters.  Windoze is hard-coded as "windows".
#
# If the variable OS_NAME is defined, it will be used.
# If the variable OS_SUBTYPE is defined, it will be appended to the OS_NAME.
#
ifdef PROCESSOR_REVISION
   OS_TYPE    := windows
   DEV_NULL   := nul
   PATH_SEP   := ;
   ifndef OS_NAME
      OS_NAME := windows
   endif
else
   OS_TYPE    := unix
   DEV_NULL   := /dev/null
   PATH_SEP   := :
   ifndef OS_NAME
      OS_NAME := $(strip $(shell LANG=C export LANG; \
                                 uname -s | \
                                 tr '[A-Z]' '[a-z]' | \
                                 sed -e 's/[^a-z0-9]//g'))
   endif
endif

ifdef OS_SUBTYPE
  OS_NAME := $(OS_NAME)_$(OS_SUBTYPE)
endif

# ------------------------------------------------------------------------
# Common commands
#
MV  := mv -f
CP  := cp -f
RM  := rm -f
RD  := rm -fr

# ------------------------------------------------------------------------
# Determine our build mode
#
ifndef BUILD_MODE
   BUILD_MODE := debug
endif

# ------------------------------------------------------------------------
# C/C++ directories (The DLL directory is OS-dependent.)
#
INT_DIR := $(BUILD_MODE)$(OS_ARCH)
TGT_DIR := $(TOP_DIR)/$(BUILD_MODE)$(OS_ARCH)

ifeq ($(OS_TYPE), windows)
  DLL_DIR_NAME := bin
else
  DLL_DIR_NAME := lib
endif

# These values must be deferred definitions

BIN_DIR = $(TGT_DIR)/bin
LIB_DIR = $(TGT_DIR)/lib
PDB_DIR = $(TGT_DIR)/pdb
PBS_DIR = $(TGT_DIR)/pbs
DLL_DIR = $(TGT_DIR)/$(DLL_DIR_NAME)

# ------------------------------------------------------------------------
# Make sure that "all" is our default target.  If we didn't do this,
# then any targets defined in the following included makefiles would
# become the default.
#
# (GNU make allows an include file to name a default target if it is
# loaded via an "include" directive.  If it is loaded via the
# MAKEFILES environment variable, it cannot name a default target.)
#
first : all

# ------------------------------------------------------------------------
# Set program variables per our OS
#
include $(TOP_DIR)/build/make_includes/make_platform_$(OS_NAME).i
include $(TOP_DIR)/build/make_includes/make_os_$(OS_TYPE).i

# ------------------------------------------------------------------------
# Determine our library type
#
ifneq ($(LIB_TYPE), dynamic)
ifneq ($(LIB_TYPE), static)
   override LIB_TYPE := dynamic
endif
endif

ifeq ($(BUILD_MODE), profile)
   override LIB_TYPE := static
endif

STATIC_LIB_SUFFIX   := _s

ifeq ($(LIB_TYPE), dynamic)
   LIB_TYPE_NAME := D
   LIB_DEP_EXT      := $(LIB_D_EXT)
   LIB_SUFFIX       :=
else
   LIB_TYPE_NAME := S
   LIB_DEP_EXT      := $(LIB_S_EXT)
   LIB_SUFFIX       := $(STATIC_LIB_SUFFIX)
endif

# ------------------------------------------------------------------------
# Determine our library mode and
# get the link command to use for programs
#
ifneq ($(LIB_MODE), separate)
  ifneq ($(LIB_MODE), combined)
   override LIB_MODE := combined
  endif
endif

ifeq ($(LIB_MODE), separate)
   LINK_CMD   = MK_EXE_SEP
   LINKPP_CMD = MK_EXEPP_SEP
endif

ifeq ($(LIB_MODE), combined)
   LINK_CMD   = MK_EXE_CMB
   LINKPP_CMD = MK_EXEPP_CMB
endif

# ------------------------------------------------------------------------
# List of suffixes we are interested in.
#
.SUFFIXES :
.SUFFIXES : .c .cpp .$(OBJ_EXT) $(EXE_EXT)

# ------------------------------------------------------------------------
# Rules to build C/C++ object files in the current directory.
#
%.$(OBJ_EXT) : %.cpp
ifndef VERBOSE_MAKE
	@ echo $(strip $(VCCPP_NAME) -c $(COPTS) $(OPTS) $<)
	@ "$(VCCPP)" -c $(CPPFLAGS) $<
else
	  "$(VCCPP)" -c $(CPPFLAGS) $<
endif

# ------------------------------------------------------------------------
# By telling make that these targets are "phony", it means that the
# targets are not real files. This speeds up processing a bit and prevents
# problems where somebody creates an actual file with one of these names.
#
# This lists all our "common" targets.
#
.PHONY : all compile link clean

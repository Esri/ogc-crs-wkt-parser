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

# ------------------------------------------------------------------------- #
# makefile for various libogc test programs                                 #
#                                                                           #
# Note that these programs can be built to use either the static or         #
# dynamic libraries, but this makefile only builds programs using the       #
# dynamic library.                                                          #
# ------------------------------------------------------------------------- #

# ------------------------------------------------------------------------
# Setup our make environment
#
TOP_DIR  := ..
include $(TOP_DIR)/build/make_includes/make.i

# ------------------------------------------------------------------------
# List of targets
#
TGTBASE_PARSE    := parse
TGTBASE_DATETIME := datetime
TGTBASE_WKT      := wkt

PARSE    := $(BIN_DIR)/$(TGTBASE_PARSE)$(EXE_EXT)
DATETIME := $(BIN_DIR)/$(TGTBASE_DATETIME)$(EXE_EXT)
WKT      := $(BIN_DIR)/$(TGTBASE_WKT)$(EXE_EXT)

TARGETS := \
  $(PARSE)    \
  $(DATETIME) \
  $(WKT)      \
  $(NULL)

# ------------------------------------------------------------------------
# program options
#
CINCS   = \
  -I../include \
  $(NULL)

CPPFLAGS := $(COPTS)  $(OPTS) $(CPPDEFS)   $(CINCS)
EXEFLAGS := $(LDOPTS) $(OPTS) $(EXEDEFS)

# ------------------------------------------------------------------------
# object files
#
PARSE_OBJS    := $(addprefix $(INT_DIR)/, $(TGTBASE_PARSE).$(OBJ_EXT)    )
DATETIME_OBJS := $(addprefix $(INT_DIR)/, $(TGTBASE_DATETIME).$(OBJ_EXT) )
WKT_OBJS      := $(addprefix $(INT_DIR)/, $(TGTBASE_WKT).$(OBJ_EXT)      )

OBJS := \
  $(PARSE_OBJS)    \
  $(DATETIME_OBJS) \
  $(WKT_OBJS)      \
  $(NULL)

# ------------------------------------------------------------------------
# source files
#
C_SRC  = $(notdir $(OBJS:.$(OBJ_EXT)=.cpp))

HDRS   = \
  $(NULL)

OTHER  = \
  $(NULL)

CHSRC   = $(C_SRC) $(HDRS)
DIRSRC  = $(CHSRC) $(OTHER)
ALLSRC  = makefile $(DIRSRC)

# ------------------------------------------------------------------------
# libraries
#
ifeq ($(OS_TYPE), windows)
  LIBSPP     := libogc.$(LIB_EXT)
  LIBSPP_DEP := $(LIB_DIR)/libogc.$(LIB_D_EXT)
else
  LIBSPP     := -logc
  LIBSPP_DEP := $(LIB_DIR)/libogc.$(LIB_D_EXT)
endif

# ------------------------------------------------------------------------
# build targets
#
all : compile link

compile : $(OBJS)

link : $(TARGETS)

$(BIN_DIR)/%$(EXE_EXT) : $(INT_DIR)/%.$(OBJ_EXT) $(LIBSPP_DEP)
	@ $(MK_EXEPP)

# ------------------------------------------------------------------------
# dependencies
#
$(OBJS) : ../include/libogc.h

# ------------------------------------------------------------------------
# list all files
#
srclist :
	@ for i in $(ALLSRC); \
	  do echo etc/$$i; \
	  done

# ------------------------------------------------------------------------
# clean targets
#
clean : notarget
	$(RD) $(INT_DIR)

notarget :
	$(RM) $(TARGETS)

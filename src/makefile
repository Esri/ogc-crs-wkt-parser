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
# makefile for the OGC-WKT library                                          #
#                                                                           #
# Note that while both a static and dynamic library can be built,           #
# this makefile only builds the dynamic one.                                #
# ------------------------------------------------------------------------- #

# ------------------------------------------------------------------------
# Setup our make environment
#
TOP_DIR := ..
include $(TOP_DIR)/build/make_includes/make.i

# ------------------------------------------------------------------------
# program targets
#
TGTBASE   := ogc
LIBNAME   := lib$(TGTBASE)$(STATIC_LIB_SUFFIX)
DLLNAME   := lib$(TGTBASE)
#TGT_LIB  := $(LIB_DIR)/$(LIBNAME).$(LIB_EXT)
TGT_DLL   := $(DLL_DIR)/$(DLLNAME).$(DLL_EXT)

TARGETS = \
  $(TGT_LIB) \
  $(TGT_DLL) \
  $(NULL)

# ------------------------------------------------------------------------
# program options
#
CINCS    := -I../include
CPPFLAGS := $(COPTS)  $(OPTS) $(CPPDEFS) $(CINCS)
LDFLAGS  := $(LDOPTS) $(OPTS) $(DLLDEFS)
RCFLAGS  := $(RCDEFS) $(CINCS)

# ------------------------------------------------------------------------
# object files
#
C_OBJS   := \
  ogc_datetime.$(OBJ_EXT)        \
  ogc_error.$(OBJ_EXT)           \
  ogc_macros.$(OBJ_EXT)          \
  ogc_string.$(OBJ_EXT)          \
  ogc_tables.$(OBJ_EXT)          \
  ogc_token.$(OBJ_EXT)           \
  ogc_unicode.$(OBJ_EXT)         \
  ogc_utils.$(OBJ_EXT)           \
  ogc_vector.$(OBJ_EXT)          \
  \
  ogc_crs.$(OBJ_EXT)             \
  ogc_datum.$(OBJ_EXT)           \
  ogc_extent.$(OBJ_EXT)          \
  ogc_object.$(OBJ_EXT)          \
  \
  ogc_abrtrans.$(OBJ_EXT)        \
  ogc_anchor.$(OBJ_EXT)          \
  ogc_angunit.$(OBJ_EXT)         \
  ogc_area_extent.$(OBJ_EXT)     \
  ogc_axis.$(OBJ_EXT)            \
  ogc_bbox_extent.$(OBJ_EXT)     \
  ogc_bearing.$(OBJ_EXT)         \
  ogc_bound_crs.$(OBJ_EXT)       \
  ogc_citation.$(OBJ_EXT)        \
  ogc_compound_crs.$(OBJ_EXT)    \
  ogc_conversion.$(OBJ_EXT)      \
  ogc_coordop.$(OBJ_EXT)         \
  ogc_cs.$(OBJ_EXT)              \
  ogc_ellipsoid.$(OBJ_EXT)       \
  ogc_engineering_crs.$(OBJ_EXT) \
  ogc_generic_datum.$(OBJ_EXT)   \
  ogc_geocentric_crs.$(OBJ_EXT)  \
  ogc_geodetic_datum.$(OBJ_EXT)  \
  ogc_geog2d_crs.$(OBJ_EXT)      \
  ogc_geog3d_crs.$(OBJ_EXT)      \
  ogc_id.$(OBJ_EXT)              \
  ogc_image_crs.$(OBJ_EXT)       \
  ogc_lenunit.$(OBJ_EXT)         \
  ogc_meridian.$(OBJ_EXT)        \
  ogc_method.$(OBJ_EXT)          \
  ogc_opaccuracy.$(OBJ_EXT)      \
  ogc_order.$(OBJ_EXT)           \
  ogc_parameter.$(OBJ_EXT)       \
  ogc_parameter_file.$(OBJ_EXT)  \
  ogc_parametric_crs.$(OBJ_EXT)  \
  ogc_paramunit.$(OBJ_EXT)       \
  ogc_primem.$(OBJ_EXT)          \
  ogc_projected_crs.$(OBJ_EXT)   \
  ogc_remark.$(OBJ_EXT)          \
  ogc_scaleunit.$(OBJ_EXT)       \
  ogc_scope.$(OBJ_EXT)           \
  ogc_temporal_crs.$(OBJ_EXT)    \
  ogc_time_extent.$(OBJ_EXT)     \
  ogc_timeunit.$(OBJ_EXT)        \
  ogc_unit.$(OBJ_EXT)            \
  ogc_uri.$(OBJ_EXT)             \
  ogc_vert_extent.$(OBJ_EXT)     \
  ogc_vertical_crs.$(OBJ_EXT)    \
  $(NULL)

LIB_OBJS := $(C_OBJS)

LIB_OBJS := $(addprefix $(INT_DIR)/, $(LIB_OBJS))
DLL_OBJS := $(LIB_OBJS)

ifeq ($(OS_TYPE), windows)
  DLL_OBJS += $(addprefix $(INT_DIR)/, libogc.res)
endif

ALL_OBJS := $(DLL_OBJS)

# ------------------------------------------------------------------------
# source files
#
C_SRC := $(C_OBJS:.$(OBJ_EXT)=.cpp)

HDRS  := \
  ogc_common.h \
  $(NULL)

OTHER := \
  libogc.rc \
  $(NULL)

CHSRC   := $(C_SRC) $(HDRS)
DIRSRC  := $(CHSRC) $(OTHER)
ALLSRC  := makefile $(DIRSRC)

# ------------------------------------------------------------------------
# build targets
#
all : compile link

compile : $(ALL_OBJS)

link : $(TARGETS)

# ------------------------------------------------------------------------
# Link targets
#
$(TGT_LIB) : $(LIB_OBJS)
	@ $(MK_LIB)

$(TGT_DLL) : $(DLL_OBJS)
	@ $(MK_SHRPP)

# ------------------------------------------------------------------------
# dependencies
#
$(ALL_OBJS) : $(HDRS) ../include/libogc.h

# ------------------------------------------------------------------------
# list all files
#
srclist :
	@ for i in $(ALLSRC); \
	  do echo src/$$i; \
	  done

# ------------------------------------------------------------------------
# clean targets
#
clean : notarget
	-$(RD) $(INT_DIR)

notarget :
	-$(RM) $(TARGETS)

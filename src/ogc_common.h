/* ------------------------------------------------------------------------- */
/* Copyright 2013 Esri                                                       */
/*                                                                           */
/* Licensed under the Apache License, Version 2.0 (the "License");           */
/* you may not use this file except in compliance with the License.          */
/* You may obtain a copy of the License at                                   */
/*                                                                           */
/*     http://www.apache.org/licenses/LICENSE-2.0                            */
/*                                                                           */
/* Unless required by applicable law or agreed to in writing, software       */
/* distributed under the License is distributed on an "AS IS" BASIS,         */
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  */
/* See the License for the specific language governing permissions and       */
/* limitations under the License.                                            */
/* ------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
/* Internal header for all libogc source files. This file can be used as a   */
/* basis for a precompiled header.                                           */
/* ------------------------------------------------------------------------- */

#ifndef OGC_COMMON_H_INCLUDED
#define OGC_COMMON_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <new>

#ifdef _WIN32
#  pragma warning (disable: 4996) /* same as "-D _CRT_SECURE_NO_WARNINGS" */
#  define OGC_EXPORTING  1
#endif

#include "libogc.h"

namespace OGC {

/* ------------------------------------------------------------------------- */
/* internal tables                                                           */
/* ------------------------------------------------------------------------- */

class ogc_tables
{
private:
    ogc_tables() {}
   ~ogc_tables() {}

public:
   struct obj_tbl
   {
      ogc_obj_type       type;
      const char *       kwd;
   };
   static const obj_tbl ogc_obj_types[];

   struct crs_tbl
   {
      ogc_crs_type       type;
      const char *       kwd;
   };
   static const crs_tbl ogc_crs_types[];

   struct unit_tbl
   {
      ogc_unit_type      type;
      const char *       kwd;
   };
   static const unit_tbl ogc_unit_types[];

   struct datum_tbl
   {
      ogc_datum_type     type;
      const char *       kwd;
   };
   static const datum_tbl ogc_datum_types[];

   struct cs_tbl
   {
      ogc_cs_type        type;
      const char *       kwd;
   };
   static const cs_tbl ogc_cs_types[];

   struct axis_tbl
   {
      ogc_axis_direction type;
      const char *       kwd;
   };
   static const axis_tbl ogc_axis_types[];

   struct error_tbl
   {
      ogc_err_code       err_code;
      const char *       err_str;
   };
   static const error_tbl ogc_errors[];
};

/* ------------------------------------------------------------------------- */
/* internal WKT output processing                                            */
/* ------------------------------------------------------------------------- */

#define OGC_TBUF_MAX          1024    /* temp WKT buffer for sub-objects */
typedef char                  OGC_TBUF [OGC_TBUF_MAX];

#define OGC_WKT_OPT_INTERNAL  0x8000  /* internal call */

#define OGC_ADD_TO_BUF(s)     \
   if ( *s != 0 )             \
   {                          \
      len += strlen(s) + 1;   \
      if ( len < (buflen-1) ) \
      {                       \
         strcat(buffer, ","); \
         strcat(buffer, s);   \
      }                       \
      else                    \
      {                       \
         rc = false;          \
      }                       \
   }

#define OGC_CPY_TO_BUF(s)     \
   if ( *s != 0 )             \
   {                          \
      len += strlen(s);       \
      if ( len < (buflen-1) ) \
      {                       \
         strcat(buffer, s);   \
      }                       \
      else                    \
      {                       \
         rc = false;          \
      }                       \
   }

/* ------------------------------------------------------------------------- */

} /* namespace OGC */

#endif /* OGC_COMMON_H_INCLUDED */

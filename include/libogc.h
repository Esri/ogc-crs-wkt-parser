/* ------------------------------------------------------------------------- */
/* Copyright 2015 Esri                                                       */
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
/* public header for the libogc library                                      */
/* ------------------------------------------------------------------------- */

#ifndef LIBOGC_H_INCLUDED
#define LIBOGC_H_INCLUDED

#include <stddef.h>
#include <time.h>

/* ------------------------------------------------------------------------- */

/* Deal with Windows linkage stuff. */

#if defined(_WIN32) && !defined(OGC_USE_STATIC_LIBS)
#  ifdef OGC_EXPORTING
#    define OGC_EXPORT   __declspec(dllexport)
#  else
#    define OGC_EXPORT   __declspec(dllimport)
#  endif
#else
#  define   OGC_EXPORT
#endif

#ifdef _WIN32
#  define   _CDECL       __cdecl
#else
#  define   _CDECL
#endif

/* ------------------------------------------------------------------------- */

namespace OGC {

/* ------------------------------------------------------------------------- */
/* version info                                                              */
/* ------------------------------------------------------------------------- */

#define OGC_VERSION_MAJOR     2
#define OGC_VERSION_MINOR     0
#define OGC_VERSION_RELEASE   0
#define OGC_VERSION_STR       "2.0.0"

/* ------------------------------------------------------------------------- */
/* max values that may be changed                                            */
/* ------------------------------------------------------------------------- */

/* Note that these lengths all include the terminating null character. */

#define OGC_BUFF_MAX    (1024 * 4)               /* Max WKT     length    */
#define OGC_KEYW_MAX    24                       /* Max keyword length    */
#define OGC_NAME_MAX    80                       /* Max name    length    */
#define OGC_TEXT_MAX    256                      /* Max text    length    */
#define OGC_TIME_MAX    64                       /* Max time    length    */
#define OGC_PATH_MAX    256                      /* Max path    length    */

/* ------------------------------------------------------------------------- */
/* defines and typedefs                                                      */
/* ------------------------------------------------------------------------- */

#define OGC_NULL        0                        /* NULL pointer          */

typedef char  OGC_KEYW [OGC_KEYW_MAX];           /* Keyword  buffer       */
typedef char  OGC_BUFF [OGC_BUFF_MAX];           /* WKT      buffer       */
typedef char  OGC_NAME [OGC_NAME_MAX];           /* Name     buffer       */
typedef char  OGC_TEXT [OGC_TEXT_MAX];           /* Text     buffer       */
typedef char  OGC_TIME [OGC_TIME_MAX];           /* Time     buffer       */
typedef char  OGC_PATH [OGC_PATH_MAX];           /* Path     buffer       */

/* Unicode defines and typedefs */

#define OGC_UTF8_NAME_MAX (OGC_NAME_MAX * 3)     /* Max UTF8 name length  */
#define OGC_UTF8_TEXT_MAX (OGC_TEXT_MAX * 3)     /* Max UTF8 text length  */
#define OGC_UTF8_PATH_MAX (OGC_PATH_MAX * 3)     /* Max UTF8 path length  */
#define OGC_UTF8_BUFF_MAX (OGC_BUFF_MAX * 3)     /* Max UTF8 WKT  length  */

typedef char  OGC_UTF8_BUFF [OGC_UTF8_BUFF_MAX]; /* UTF8 WKT  buffer      */
typedef char  OGC_UTF8_NAME [OGC_UTF8_NAME_MAX]; /* UTF8 name buffer      */
typedef char  OGC_UTF8_TEXT [OGC_UTF8_TEXT_MAX]; /* UTF8 text buffer      */
typedef char  OGC_UTF8_PATH [OGC_UTF8_PATH_MAX]; /* UTF8 path buffer      */

typedef unsigned short  OGC_UCHAR;               /* Unicode character     */

/* number strings */

#define OGC_NBUF_MAX    32                       /* Max chars in a number */
typedef char            OGC_NBUF [OGC_NBUF_MAX]; /* Number buffer         */

/* ------------------------------------------------------------------------- */
/* Numeric constants                                                         */
/* ------------------------------------------------------------------------- */

#define OGC_PI        3.14159265358979323846264
#define OGC_PI180     (OGC_PI / 180.0 )
#define OGC_180PI     (180.0  / OGC_PI)

/* numeric tolerances for comparing doubles */
#define OGC_EPS_53    1.11022302462515654042363e-16  /* 2^(-53)           */
#define OGC_EPS_52    2.22044604925031308084726e-16  /* 2^(-52)           */
#define OGC_EPS_51    4.44089209850062616169452e-16  /* 2^(-51)           */
#define OGC_EPS_50    8.88178419700125232338905e-16  /* 2^(-50)           */
#define OGC_EPS_49    1.77635683940025046467781e-15  /* 2^(-49)           */
#define OGC_EPS_48    3.55271367880050092935562e-15  /* 2^(-48)           */
#define OGC_EPS       OGC_EPS_48                     /* default tolerance */

/* ------------------------------------------------------------------------- */
/* WKT output options                                                        */
/* ------------------------------------------------------------------------- */

#define OGC_WKT_OPT_NONE         0x00    /* No options                    */

#define OGC_WKT_OPT_EXPAND_SP    0x01    /* Multi-line with space indents */
#define OGC_WKT_OPT_EXPAND_TAB   0x02    /* Multi-line with tab   indents */

#define OGC_WKT_OPT_EXPAND       ( OGC_WKT_OPT_EXPAND_SP   | \
                                   OGC_WKT_OPT_EXPAND_TAB  )

#define OGC_WKT_OPT_TOP_ID_ONLY  0x04    /* Show top-level ID only        */
#define OGC_WKT_OPT_NO_IDS       0x08    /* Don't show any ID attributes  */

#define OGC_WKT_OPT_ID           ( OGC_WKT_OPT_TOP_ID_ONLY | \
                                   OGC_WKT_OPT_NO_IDS      )

#define OGC_WKT_OPT_PARENS       0x10    /* Use () instead of [] in WKT   */
#define OGC_WKT_OPT_OLD_SYNTAX   0x20    /* Output in old syntax          */

/* ------------------------------------------------------------------------- */
/* object types and keywords                                                 */
/* ------------------------------------------------------------------------- */

enum ogc_obj_type
{
   OGC_OBJ_TYPE_UNKNOWN          = 0,

   OGC_OBJ_TYPE_CITATION,
   OGC_OBJ_TYPE_URI,
   OGC_OBJ_TYPE_ID,

   OGC_OBJ_TYPE_UNIT,
   OGC_OBJ_TYPE_ANGUNIT,
   OGC_OBJ_TYPE_LENUNIT,
   OGC_OBJ_TYPE_SCALEUNIT,
   OGC_OBJ_TYPE_TIMEUNIT,
   OGC_OBJ_TYPE_PARAMUNIT,

   OGC_OBJ_TYPE_SCOPE,
   OGC_OBJ_TYPE_AREA_EXTENT,
   OGC_OBJ_TYPE_BBOX_EXTENT,
   OGC_OBJ_TYPE_VERT_EXTENT,
   OGC_OBJ_TYPE_TIME_EXTENT,
   OGC_OBJ_TYPE_REMARK,

   OGC_OBJ_TYPE_PARAMETER,
   OGC_OBJ_TYPE_PARAM_FILE,

   OGC_OBJ_TYPE_ELLIPSOID,

   OGC_OBJ_TYPE_ANCHOR,
   OGC_OBJ_TYPE_TIME_ORIGIN,

   OGC_OBJ_TYPE_GEOD_DATUM,
   OGC_OBJ_TYPE_ENGR_DATUM,
   OGC_OBJ_TYPE_IMAGE_DATUM,
   OGC_OBJ_TYPE_PARAM_DATUM,
   OGC_OBJ_TYPE_TIME_DATUM,
   OGC_OBJ_TYPE_VERT_DATUM,

   OGC_OBJ_TYPE_PRIMEM,

   OGC_OBJ_TYPE_ORDER,
   OGC_OBJ_TYPE_MERIDIAN,
   OGC_OBJ_TYPE_BEARING,
   OGC_OBJ_TYPE_AXIS,

   OGC_OBJ_TYPE_CS,

   OGC_OBJ_TYPE_CONVERSION,
   OGC_OBJ_TYPE_DERIVING_CONV,
   OGC_OBJ_TYPE_METHOD,

   OGC_OBJ_TYPE_GEOD_CRS,
   OGC_OBJ_TYPE_ENGR_CRS,
   OGC_OBJ_TYPE_IMAGE_CRS,
   OGC_OBJ_TYPE_PARAM_CRS,
   OGC_OBJ_TYPE_PROJ_CRS,
   OGC_OBJ_TYPE_TIME_CRS,
   OGC_OBJ_TYPE_VERT_CRS,
   OGC_OBJ_TYPE_COMPOUND_CRS,

   OGC_OBJ_TYPE_BASE_GEOD_CRS,
   OGC_OBJ_TYPE_BASE_ENGR_CRS,
   OGC_OBJ_TYPE_BASE_PARAM_CRS,
   OGC_OBJ_TYPE_BASE_PROJ_CRS,
   OGC_OBJ_TYPE_BASE_TIME_CRS,
   OGC_OBJ_TYPE_BASE_VERT_CRS,

   OGC_OBJ_TYPE_OP_ACCURACY,
   OGC_OBJ_TYPE_COORD_OP,

   OGC_OBJ_TYPE_BOUND_CRS,
   OGC_OBJ_TYPE_ABRTRANS
};

/* These are the keywords used in WKT strings */

#define OGC_OBJ_KWD_UNKNOWN          "UNKNOWN"

#define OGC_OBJ_KWD_CITATION         "CITATION"
#define OGC_OBJ_KWD_URI              "URI"
#define OGC_OBJ_KWD_ID               "ID"

#define OGC_OBJ_KWD_UNIT             "UNIT"
#define OGC_OBJ_KWD_ANGUNIT          "ANGLEUNIT"
#define OGC_OBJ_KWD_LENUNIT          "LENGTHUNIT"
#define OGC_OBJ_KWD_SCALEUNIT        "SCALEUNIT"
#define OGC_OBJ_KWD_TIMEUNIT         "TIMEUNIT"
#define OGC_OBJ_KWD_PARAMUNIT        "PARAMETRICUNIT"

#define OGC_OBJ_KWD_SCOPE            "SCOPE"
#define OGC_OBJ_KWD_AREA_EXTENT      "AREA"
#define OGC_OBJ_KWD_BBOX_EXTENT      "BBOX"
#define OGC_OBJ_KWD_VERT_EXTENT      "VERTICALEXTENT"
#define OGC_OBJ_KWD_TIME_EXTENT      "TIMEEXTENT"
#define OGC_OBJ_KWD_REMARK           "REMARK"

#define OGC_OBJ_KWD_PARAMETER        "PARAMETER"
#define OGC_OBJ_KWD_PARAM_FILE       "PARAMETERFILE"

#define OGC_OBJ_KWD_ELLIPSOID        "ELLIPSOID"

#define OGC_OBJ_KWD_ANCHOR           "ANCHOR"
#define OGC_OBJ_KWD_TIME_ORIGIN      "TIMEORIGIN"

#define OGC_OBJ_KWD_GEOD_DATUM       "DATUM"
#define OGC_OBJ_KWD_ENGR_DATUM       "EDATUM"
#define OGC_OBJ_KWD_IMAGE_DATUM      "IDATUM"
#define OGC_OBJ_KWD_PARAM_DATUM      "PDATUM"
#define OGC_OBJ_KWD_TIME_DATUM       "TDATUM"
#define OGC_OBJ_KWD_VERT_DATUM       "VDATUM"

#define OGC_OBJ_KWD_PRIMEM           "PRIMEM"

#define OGC_OBJ_KWD_ORDER            "ORDER"
#define OGC_OBJ_KWD_MERIDIAN         "MERIDIAN"
#define OGC_OBJ_KWD_BEARING          "BEARING"
#define OGC_OBJ_KWD_AXIS             "AXIS"

#define OGC_OBJ_KWD_CS               "CS"

#define OGC_OBJ_KWD_CONVERSION       "CONVERSION"
#define OGC_OBJ_KWD_DERIVING_CONV    "DERIVINGCONVERSION"
#define OGC_OBJ_KWD_METHOD           "METHOD"

#define OGC_OBJ_KWD_GEOD_CRS         "GEODCRS"
#define OGC_OBJ_KWD_ENGR_CRS         "ENGCRS"
#define OGC_OBJ_KWD_IMAGE_CRS        "IMAGECRS"
#define OGC_OBJ_KWD_PARAM_CRS        "PARAMETRICCRS"
#define OGC_OBJ_KWD_PROJ_CRS         "PROJCRS"
#define OGC_OBJ_KWD_TIME_CRS         "TIMECRS"
#define OGC_OBJ_KWD_VERT_CRS         "VERTCRS"
#define OGC_OBJ_KWD_COMPOUND_CRS     "COMPOUNDCRS"

#define OGC_OBJ_KWD_BASE_GEOD_CRS    "BASEGEODCRS"
#define OGC_OBJ_KWD_BASE_ENGR_CRS    "BASEENGCRS"
#define OGC_OBJ_KWD_BASE_PARAM_CRS   "BASEPARAMETRICCRS"
#define OGC_OBJ_KWD_BASE_PROJ_CRS    "BASEPROJCRS"
#define OGC_OBJ_KWD_BASE_TIME_CRS    "BASETIMECRS"
#define OGC_OBJ_KWD_BASE_VERT_CRS    "BASEVERTCRS"

#define OGC_OBJ_KWD_OP_ACCURACY      "OPERATIONACCURACY"
#define OGC_OBJ_KWD_COORD_OP         "COORDINATEOPERATION"

#define OGC_OBJ_KWD_BOUND_CRS        "BOUNDCRS"
#define OGC_OBJ_KWD_ABRTRANS         "ABRIDGEDTRANSFORMATION"

/* keywords for virtual objects */

#define OGC_OBJ_KWD_EXTENT           "extent"
#define OGC_OBJ_KWD_BASE_CRS         "base-crs"
#define OGC_OBJ_KWD_CRS              "crs"
#define OGC_OBJ_KWD_DATUM            "datum"
#define OGC_OBJ_KWD_OBJECT           "object"

/* keywords that are not real objects */

#define OGC_OBJ_KWD_SOURCE_CRS       "SOURCECRS"
#define OGC_OBJ_KWD_TARGET_CRS       "TARGETCRS"
#define OGC_OBJ_KWD_INTERP_CRS       "INTERPOLATIONCRS"

/* alternate keywords */

#define OGC_ALT_KWD_ENGR_DATUM       "ENGINEERINGDATUM"
#define OGC_ALT_KWD_GEOD_DATUM       "GEODETICDATUM"
#define OGC_ALT_KWD_IMAGE_DATUM      "IMAGEDATUM"
#define OGC_ALT_KWD_PARAM_DATUM      "PARAMETRICDATUM"
#define OGC_ALT_KWD_TIME_DATUM       "TIMEDATUM"
#define OGC_ALT_KWD_VERT_DATUM       "VERTICALDATUM"

#define OGC_ALT_KWD_ENGR_CRS         "ENGINEERINGCRS"
#define OGC_ALT_KWD_GEOD_CRS         "GEODETICCRS"
#define OGC_ALT_KWD_PARAM_CRS        "PARAMETRICCRS"
#define OGC_ALT_KWD_PROJ_CRS         "PROJECTEDCCRS"
#define OGC_ALT_KWD_VERT_CRS         "VERTICALCRS"

#define OGC_ALT_KWD_METHOD           "PROJECTION"
#define OGC_ALT_KWD_ELLIPSOID        "SPHEROID"
#define OGC_ALT_KWD_PRIMEM           "PRIMEMERIDIAN"

/* keywords for old (19125) objects */

#define OGC_OLD_KWD_GEOCCS           "GEOCCS"
#define OGC_OLD_KWD_GEOGCS           "GEOGCS"
#define OGC_OLD_KWD_PROJCS           "PROJCS"
#define OGC_OLD_KWD_VERTCS           "VERT_CS"
#define OGC_OLD_KWD_LOCALCS          "LOCAL_CS"
#define OGC_OLD_KWD_VDATUM           "VERT_DATUM"
#define OGC_OLD_KWD_LDATUM           "LOCAL_DATUM"
#define OGC_OLD_KWD_COMPDCS          "COMPD_CS"
#define OGC_OLD_KWD_ID               "AUTHORITY"

/* ------------------------------------------------------------------------- */
/* coordinate reference system types and keywords                            */
/* ------------------------------------------------------------------------- */

enum ogc_crs_type
{
   OGC_CRS_TYPE_UNKNOWN = 0,

   OGC_CRS_TYPE_GEOD,
   OGC_CRS_TYPE_PROJ,
   OGC_CRS_TYPE_VERT,
   OGC_CRS_TYPE_ENGR,
   OGC_CRS_TYPE_IMAGE,
   OGC_CRS_TYPE_PARAM,
   OGC_CRS_TYPE_TIME,

   OGC_CRS_TYPE_BASE_GEOD,
   OGC_CRS_TYPE_BASE_PROJ,
   OGC_CRS_TYPE_BASE_VERT,
   OGC_CRS_TYPE_BASE_ENGR,
   OGC_CRS_TYPE_BASE_PARAM,
   OGC_CRS_TYPE_BASE_TIME,

   OGC_CRS_TYPE_COMPOUND
};

#define OGC_CRS_KWD_UNKNOWN          "unknown"

#define OGC_CRS_KWD_GEOD             "geodetic"
#define OGC_CRS_KWD_PROJ             "projected"
#define OGC_CRS_KWD_VERT             "vertical"
#define OGC_CRS_KWD_ENGR             "engineering"
#define OGC_CRS_KWD_IMAGE            "image"
#define OGC_CRS_KWD_PARAM            "parametric"
#define OGC_CRS_KWD_TIME             "time"

#define OGC_CRS_KWD_BASE_GEOD        "base-geodetic"
#define OGC_CRS_KWD_BASE_PROJ        "base-projected"
#define OGC_CRS_KWD_BASE_VERT        "base-vertical"
#define OGC_CRS_KWD_BASE_ENGR        "base-engineering"
#define OGC_CRS_KWD_BASE_PARAM       "base-parametric"
#define OGC_CRS_KWD_BASE_TIME        "base-time"

#define OGC_CRS_KWD_COMPOUND         "compound"

/* ------------------------------------------------------------------------- */
/* unit types and keywords                                                   */
/* ------------------------------------------------------------------------- */

enum ogc_unit_type
{
   OGC_UNIT_TYPE_UNKNOWN = 0,

   OGC_UNIT_TYPE_ANGLE,
   OGC_UNIT_TYPE_LENGTH,
   OGC_UNIT_TYPE_PARAM,
   OGC_UNIT_TYPE_SCALE,
   OGC_UNIT_TYPE_TIME,
   OGC_UNIT_TYPE_GENERIC
};

#define OGC_UNIT_KWD_UNKNOWN         "unknown"

#define OGC_UNIT_KWD_ANGLE           "angle"
#define OGC_UNIT_KWD_LENGTH          "length"
#define OGC_UNIT_KWD_PARAM           "param"
#define OGC_UNIT_KWD_SCALE           "scale"
#define OGC_UNIT_KWD_TIME            "time"
#define OGC_UNIT_KWD_GENERIC         "generic"

/* ------------------------------------------------------------------------- */
/* pixel-in-cell types and keywords                                          */
/* ------------------------------------------------------------------------- */

enum ogc_pixel_type
{
   OGC_PIXEL_TYPE_UNKNOWN = 0,

   OGC_PIXEL_TYPE_CENTER,
   OGC_PIXEL_TYPE_CORNER
};

#define OGC_PIXEL_TYPE_CENTRE         OGC_PIXEL_TYPE_CENTER

#define OGC_PIXEL_KWD_UNKNOWN         "unknown"

#define OGC_PIXEL_KWD_CENTER          "center"
#define OGC_PIXEL_KWD_CENTRE          "centre"
#define OGC_PIXEL_KWD_CORNER          "corner"

/* ------------------------------------------------------------------------- */
/* datum types and keywords                                                  */
/* ------------------------------------------------------------------------- */

enum ogc_datum_type
{
   OGC_DATUM_TYPE_UNKNOWN = 0,

   OGC_DATUM_TYPE_GENERIC,
   OGC_DATUM_TYPE_ENGR,
   OGC_DATUM_TYPE_GEOD,
   OGC_DATUM_TYPE_IMAGE,
   OGC_DATUM_TYPE_PARAM,
   OGC_DATUM_TYPE_TIME,
   OGC_DATUM_TYPE_VERT
};

#define OGC_DATUM_KWD_UNKNOWN       "unknown"

#define OGC_DATUM_KWD_ENGR          "engineering"
#define OGC_DATUM_KWD_GEOD          "geodetic"
#define OGC_DATUM_KWD_IMAGE         "image"
#define OGC_DATUM_KWD_PARAM         "parametric"
#define OGC_DATUM_KWD_TIME          "time"
#define OGC_DATUM_KWD_VERT          "vertical"

/* ------------------------------------------------------------------------- */
/* Axis directions and keywords                                              */
/* ------------------------------------------------------------------------- */

enum ogc_axis_direction
{
     OGC_AXIS_DIR_TYPE_UNKNOWN            = 0,

     OGC_AXIS_DIR_TYPE_NORTH              = 100,
     OGC_AXIS_DIR_TYPE_NORTH_NORTH_EAST,
     OGC_AXIS_DIR_TYPE_NORTH_EAST,
     OGC_AXIS_DIR_TYPE_EAST_NORTH_EAST,
     OGC_AXIS_DIR_TYPE_EAST,
     OGC_AXIS_DIR_TYPE_EAST_SOUTH_EAST,
     OGC_AXIS_DIR_TYPE_SOUTH_EAST,
     OGC_AXIS_DIR_TYPE_SOUTH_SOUTH_EAST,
     OGC_AXIS_DIR_TYPE_SOUTH,
     OGC_AXIS_DIR_TYPE_SOUTH_WEST,
     OGC_AXIS_DIR_TYPE_WEST_SOUTH_WEST,
     OGC_AXIS_DIR_TYPE_WEST,
     OGC_AXIS_DIR_TYPE_WEST_NORTH_WEST,
     OGC_AXIS_DIR_TYPE_NORTH_WEST,
     OGC_AXIS_DIR_TYPE_NORTH_NORTH_WEST,

     OGC_AXIS_DIR_TYPE_GEOCENTRIC_X       = 200,
     OGC_AXIS_DIR_TYPE_GEOCENTRIC_Y,
     OGC_AXIS_DIR_TYPE_GEOCENTRIC_Z,

     OGC_AXIS_DIR_TYPE_UP                 = 300,
     OGC_AXIS_DIR_TYPE_DOWN,

     OGC_AXIS_DIR_TYPE_FORWARD            = 400,
     OGC_AXIS_DIR_TYPE_AFT,
     OGC_AXIS_DIR_TYPE_STARBOARD,
     OGC_AXIS_DIR_TYPE_PORT,

     OGC_AXIS_DIR_TYPE_CLOCKWISE          = 500,
     OGC_AXIS_DIR_TYPE_COUNTER_CLOCKWISE,

     OGC_AXIS_DIR_TYPE_COLUMN_POSITIVE    = 600,
     OGC_AXIS_DIR_TYPE_COLUMN_NEGATIVE,
     OGC_AXIS_DIR_TYPE_ROW_POSITIVE,
     OGC_AXIS_DIR_TYPE_ROW_NEGATIVE,

     OGC_AXIS_DIR_TYPE_DISPLAY_RIGHT      = 700,
     OGC_AXIS_DIR_TYPE_DISPLAY_LEFT,
     OGC_AXIS_DIR_TYPE_DISPLAY_UP,
     OGC_AXIS_DIR_TYPE_DISPLAY_DOWN,

     OGC_AXIS_DIR_TYPE_FUTURE             = 800,
     OGC_AXIS_DIR_TYPE_PAST,

     OGC_AXIS_DIR_TYPE_TOWARDS            = 900,
     OGC_AXIS_DIR_TYPE_AWAY_FROM,

     OGC_AXIS_DIR_TYPE_UNSPECIFIED        = 1000
};

#define OGC_AXIS_DIR_KWD_UNKNOWN            "unknown"

#define OGC_AXIS_DIR_KWD_NORTH              "north"
#define OGC_AXIS_DIR_KWD_NORTH_NORTH_EAST   "northNorthEast"
#define OGC_AXIS_DIR_KWD_NORTH_EAST         "northEast"
#define OGC_AXIS_DIR_KWD_EAST_NORTH_EAST    "eastNorthEast"
#define OGC_AXIS_DIR_KWD_EAST               "east"
#define OGC_AXIS_DIR_KWD_EAST_SOUTH_EAST    "eastSouthEast"
#define OGC_AXIS_DIR_KWD_SOUTH_EAST         "southEast"
#define OGC_AXIS_DIR_KWD_SOUTH_SOUTH_EAST   "southSouthEast"
#define OGC_AXIS_DIR_KWD_SOUTH              "south"
#define OGC_AXIS_DIR_KWD_SOUTH_WEST         "southWest"
#define OGC_AXIS_DIR_KWD_WEST_SOUTH_WEST    "westSouthWest"
#define OGC_AXIS_DIR_KWD_WEST               "west"
#define OGC_AXIS_DIR_KWD_WEST_NORTH_WEST    "westNorthWest"
#define OGC_AXIS_DIR_KWD_NORTH_WEST         "northWest"
#define OGC_AXIS_DIR_KWD_NORTH_NORTH_WEST   "northNorthWest"

#define OGC_AXIS_DIR_KWD_GEOCENTRIC_X       "geocentricX"
#define OGC_AXIS_DIR_KWD_GEOCENTRIC_Y       "geocentricY"
#define OGC_AXIS_DIR_KWD_GEOCENTRIC_Z       "geocentricZ"

#define OGC_AXIS_DIR_KWD_UP                 "up"
#define OGC_AXIS_DIR_KWD_DOWN               "down"

#define OGC_AXIS_DIR_KWD_FORWARD            "forward"
#define OGC_AXIS_DIR_KWD_AFT                "aft"
#define OGC_AXIS_DIR_KWD_STARBOARD          "starboard"
#define OGC_AXIS_DIR_KWD_PORT               "port"

#define OGC_AXIS_DIR_KWD_CLOCKWISE          "clockwise"
#define OGC_AXIS_DIR_KWD_COUNTER_CLOCKWISE  "counterClockwise"

#define OGC_AXIS_DIR_KWD_COLUMN_POSITIVE    "columnPositive"
#define OGC_AXIS_DIR_KWD_COLUMN_NEGATIVE    "columnNegative"
#define OGC_AXIS_DIR_KWD_ROW_POSITIVE       "rowPositive"
#define OGC_AXIS_DIR_KWD_ROW_NEGATIVE       "rowNegative"

#define OGC_AXIS_DIR_KWD_DISPLAY_RIGHT      "displayRight"
#define OGC_AXIS_DIR_KWD_DISPLAY_LEFT       "displayLeft"
#define OGC_AXIS_DIR_KWD_DISPLAY_UP         "displayUp"
#define OGC_AXIS_DIR_KWD_DISPLAY_DOWN       "displayDown"

#define OGC_AXIS_DIR_KWD_FUTURE             "future"
#define OGC_AXIS_DIR_KWD_PAST               "past"

#define OGC_AXIS_DIR_KWD_TOWARDS            "towards"
#define OGC_AXIS_DIR_KWD_AWAY_FROM          "awayFrom"

#define OGC_AXIS_DIR_KWD_UNSPECIFIED        "unspecified"

/* ------------------------------------------------------------------------- */
/* coordinate system types and keywords                                      */
/* ------------------------------------------------------------------------- */

enum ogc_cs_type
{
   OGC_CS_TYPE_UNKNOWN       = 0,

   OGC_CS_TYPE_AFFINE,
   OGC_CS_TYPE_CARTESIAN,
   OGC_CS_TYPE_CYLINDRICAL,
   OGC_CS_TYPE_ELLIPSOIDAL,
   OGC_CS_TYPE_LINEAR,
   OGC_CS_TYPE_PARAMETRIC,
   OGC_CS_TYPE_POLAR,
   OGC_CS_TYPE_SPHERICAL,
   OGC_CS_TYPE_TEMPORAL,
   OGC_CS_TYPE_VERTICAL
};

#define OGC_CS_KWD_UNKNOWN       "unknown"

#define OGC_CS_KWD_AFFINE        "affine"
#define OGC_CS_KWD_CARTESIAN     "Cartesian"
#define OGC_CS_KWD_CYLINDRICAL   "cylindrical"
#define OGC_CS_KWD_ELLIPSOIDAL   "ellipsoidal"
#define OGC_CS_KWD_LINEAR        "linear"
#define OGC_CS_KWD_PARAMETRIC    "parametric"
#define OGC_CS_KWD_POLAR         "polar"
#define OGC_CS_KWD_SPHERICAL     "spherical"
#define OGC_CS_KWD_TEMPORAL      "temporal"
#define OGC_CS_KWD_VERTICAL      "vertical"

/* ------------------------------------------------------------------------- */
/* error codes                                                               */
/* ------------------------------------------------------------------------- */

/* WKT token parsing error codes */

enum ogc_err_code
{
   /* general errors */

      OGC_ERR_NONE                       = 0,
      OGC_ERR_NO_MEMORY,

   /* These errors come from the from_wkt() & from_tokens() methods */

      /* WKT tokenizing errors */

      OGC_ERR_WKT_EMPTY_STRING           = 100,
      OGC_ERR_WKT_EXPECTING_TOKEN,
      OGC_ERR_WKT_INSUFFICIENT_TOKENS,
      OGC_ERR_WKT_INVALID_KEYWORD,
      OGC_ERR_WKT_INVALID_ESCAPE,
      OGC_ERR_WKT_INVALID_SYNTAX,
      OGC_ERR_WKT_MAX_TOKENS_EXCEEDED,
      OGC_ERR_WKT_MISSING_TOKEN,
      OGC_ERR_WKT_TOO_LONG,
      OGC_ERR_WKT_UNBALANCED_QUOTES,
      OGC_ERR_WKT_TOO_MANY_OPEN_TOKENS,
      OGC_ERR_WKT_TOO_MANY_CLOSE_TOKENS,
      OGC_ERR_WKT_UNKNOWN_KEYWORD,

      /* WKT duplicate object errors */

      OGC_ERR_WKT_DUPLICATE_ABRTRANS,
      OGC_ERR_WKT_DUPLICATE_ANCHOR,
      OGC_ERR_WKT_DUPLICATE_BASE_CRS,
      OGC_ERR_WKT_DUPLICATE_BEARING,
      OGC_ERR_WKT_DUPLICATE_CITATION,
      OGC_ERR_WKT_DUPLICATE_CONVERSION,
      OGC_ERR_WKT_DUPLICATE_CS,
      OGC_ERR_WKT_DUPLICATE_DATUM,
      OGC_ERR_WKT_DUPLICATE_ELLIPSOID,
      OGC_ERR_WKT_DUPLICATE_EXTENT,
      OGC_ERR_WKT_DUPLICATE_FIRST_CRS,
      OGC_ERR_WKT_DUPLICATE_ID,
      OGC_ERR_WKT_DUPLICATE_INTERP_CRS,
      OGC_ERR_WKT_DUPLICATE_MERIDIAN,
      OGC_ERR_WKT_DUPLICATE_METHOD,
      OGC_ERR_WKT_DUPLICATE_OPACCURACY,
      OGC_ERR_WKT_DUPLICATE_ORDER,
      OGC_ERR_WKT_DUPLICATE_PARAMETER,
      OGC_ERR_WKT_DUPLICATE_PARAM_FILE,
      OGC_ERR_WKT_DUPLICATE_PRIMEM,
      OGC_ERR_WKT_DUPLICATE_REMARK,
      OGC_ERR_WKT_DUPLICATE_SCOPE,
      OGC_ERR_WKT_DUPLICATE_SECOND_CRS,
      OGC_ERR_WKT_DUPLICATE_SOURCE_CRS,
      OGC_ERR_WKT_DUPLICATE_TARGET_CRS,
      OGC_ERR_WKT_DUPLICATE_TIME_CRS,
      OGC_ERR_WKT_DUPLICATE_UNIT,
      OGC_ERR_WKT_DUPLICATE_URI,

      /* WKT axis errors */

      OGC_ERR_WKT_TOO_MANY_AXES,
      OGC_ERR_WKT_ORDER_OUT_OF_RANGE,
      OGC_ERR_WKT_AXIS_ALREADY_USED,

      /* other WKT errors */

      OGC_ERR_WKT_TOO_MANY_TOKENS,
      OGC_ERR_WKT_INDEX_OUT_OF_RANGE,

   /* These errors come from the create() methods */

      /* invalid value errors */

      OGC_ERR_INVALID_AXIS_DIRECTION     = 200,
      OGC_ERR_INVALID_BEARING_VALUE,
      OGC_ERR_INVALID_CS_TYPE,
      OGC_ERR_INVALID_DIMENSION,
      OGC_ERR_INVALID_FIRST_CRS,
      OGC_ERR_INVALID_FLATTENING,
      OGC_ERR_INVALID_LATITUDE,
      OGC_ERR_INVALID_LONGITUDE,
      OGC_ERR_INVALID_MERIDIAN_VALUE,
      OGC_ERR_INVALID_ORDER_VALUE,
      OGC_ERR_INVALID_SECOND_CRS,
      OGC_ERR_INVALID_SEMIMAJOR_AXIS,
      OGC_ERR_INVALID_THIRD_CRS,
      OGC_ERR_INVALID_UNIT_FACTOR,
      OGC_ERR_INVALID_PIXEL_TYPE,

      /* missing object errors */

      OGC_ERR_MISSING_ABRTRANS,
      OGC_ERR_MISSING_BASE_CRS,
      OGC_ERR_MISSING_CONVERSION,
      OGC_ERR_MISSING_CS,
      OGC_ERR_MISSING_DATUM,
      OGC_ERR_MISSING_ELLIPSOID,
      OGC_ERR_MISSING_FIRST_CRS,
      OGC_ERR_MISSING_METHOD,
      OGC_ERR_MISSING_SECOND_CRS,
      OGC_ERR_MISSING_SOURCE_CRS,
      OGC_ERR_MISSING_TARGET_CRS,
      OGC_ERR_MISSING_UNIT,

      /* other errors */

      OGC_ERR_EMPTY_FILENAME,
      OGC_ERR_EMPTY_NAME,

      OGC_ERR_MISMATCHED_UNITS,

      OGC_ERR_ABBR_TOO_LONG,
      OGC_ERR_FILENAME_TOO_LONG,
      OGC_ERR_IDENTIFIER_TOO_LONG,
      OGC_ERR_NAME_TOO_LONG,
      OGC_ERR_TEXT_TOO_LONG,
      OGC_ERR_TIME_TOO_LONG,
      OGC_ERR_VERSION_TOO_LONG,

   OGC_ERR_LAST
};

/* ------------------------------------------------------------------------- */
/* error reporting                                                           */
/* ------------------------------------------------------------------------- */

#define OGC_ERR_MSG_MAX  256
typedef char             OGC_ERR_BUF [OGC_ERR_MSG_MAX];


class OGC_EXPORT ogc_error
{
public:
   typedef void (_CDECL OGC_ERR_RTN) (
      void *       usr_data,
      ogc_err_code err_code,
      const char * err_msg);

private:
   static OGC_ERR_RTN * _usr_rtn;
   static void *        _usr_data;

   ogc_err_code _err_code;
   OGC_ERR_BUF  _err_msg;

   void send();

public:
    ogc_error();
   ~ogc_error() {}

   void clear();

   void set(
      ogc_err_code err_code);

   void set(
      ogc_err_code err_code,
      const char * obj_kwd);

   void set(
      ogc_err_code err_code,
      const char * obj_kwd,
      const char * arg);

   void set(
      ogc_err_code err_code,
      const char * obj_kwd,
      int          arg);

   void set(
      ogc_err_code err_code,
      const char * obj_kwd,
      double       arg);

   ogc_err_code err_code () const { return _err_code; }
   const char * err_msg  () const { return _err_msg;  }

   /* Set an error call-back routine.
      If this is set, the routine will be called with each error,
      regardless of whether the user specified an error argument or not.
   */
   static void set_err_rtn(OGC_ERR_RTN * rtn, void * data = OGC_NULL);

   /* static methods */

   static void clear(
      ogc_error *  err);

   static void set(
      ogc_error *  err,
      ogc_err_code err_code);

   static void set(
      ogc_error *  err,
      ogc_err_code err_code,
      const char * obj_kwd);

   static void set(
      ogc_error *  err,
      ogc_err_code err_code,
      const char * obj_kwd,
      const char * arg);

   static void set(
      ogc_error *  err,
      ogc_err_code err_code,
      const char * obj_kwd,
      int          arg);

   static void set(
      ogc_error *  err,
      ogc_err_code err_code,
      const char * obj_kwd,
      double       arg);

   static ogc_err_code err_code(
      const ogc_error *  err);

   static const char * err_msg(
      const ogc_error *  err);

   static const char * err_str(ogc_err_code err_code);
};

/* ------------------------------------------------------------------------- */
/* string tokenizing                                                         */
/* ------------------------------------------------------------------------- */

#define OGC_TOKENS_MAX   512

struct ogc_token_entry
{
   char * str;       /* token string                    */
   int    lvl;       /* token level                     */
   int    idx;       /* token index for this sub-object */
};

class OGC_EXPORT ogc_token
{
private:
   char * _buffer;

   bool pass1(
      const char * str,
      const char * start,
      const char * obj_kwd,
      ogc_error *  err);

   static int parse_substring(
      char ** pb,
      char ** pe);

public:
   ogc_token_entry  _arr [OGC_TOKENS_MAX];
   int              _num;

    ogc_token();
   ~ogc_token();

   bool tokenize(
      const char * str,
      const char * obj_kwd,
      ogc_error *  err = OGC_NULL);
};

/* ------------------------------------------------------------------------- */
/* Base class                                                                */
/* ------------------------------------------------------------------------- */

class OGC_EXPORT ogc_object
{
private:
   static bool  _strict_parsing;

protected:
   ogc_obj_type _obj_type;
   bool         _visible;

   ogc_object() {}

public:
   static const char * obj_kwd();
   static bool is_kwd(const char * kwd);

   /* Strict parsing option.
    *
    * If this is turned off, the following happens:
    *
    *   1. Missing trailing ] chars is allowed.
    *   2. Token parsing stops at the matching ] char to the opening [ char.
    *      (ie. any chars after the closing ] are ignored.)
    *   3. Extra atomic (ie. non sub-object) tokens in an object are ignored.
    */
   static bool set_strict_parsing(bool on_off);
   static bool get_strict_parsing();

   virtual ~ogc_object();
   static ogc_object * destroy(ogc_object * obj);

   static ogc_object * from_tokens(
      const ogc_token * t,
      int               start,
      int *             pend,
      ogc_error *       err = OGC_NULL);

   static ogc_object * from_wkt(
      const char * wkt,
      ogc_error *  err = OGC_NULL);

   static bool to_wkt(
      const ogc_object * obj,
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX);

   virtual bool to_wkt(
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX) const;

   static ogc_object * clone(const ogc_object * obj);
          ogc_object * clone() const;

   static bool is_equal    (const ogc_object * p1,
                            const ogc_object * p2);
          bool is_equal    (const ogc_object * p) const;

   static bool is_identical(const ogc_object * p1,
                            const ogc_object * p2);
          bool is_identical(const ogc_object * p) const;

   ogc_obj_type         obj_type()   const { return _obj_type; }
   virtual const char * name()       const { return "";        }
   bool                 is_visible() const { return _visible;  }

   void                 set_visible(bool val) { _visible = val; }
};

/* ------------------------------------------------------------------------- */
/* base object vector                                                        */
/* ------------------------------------------------------------------------- */

class OGC_EXPORT ogc_vector
{
private:
   int           _length;
   int           _capacity;
   int           _increment;
   ogc_object ** _array;

   ogc_vector() {}

public:
   static ogc_vector * create(
      int capacity,
      int increment);

   static ogc_vector * clone(const ogc_vector * obj);
          ogc_vector * clone() const;

   ~ogc_vector();
   static ogc_vector * destroy(ogc_vector * obj);

   int           length   ()         const { return _length;    }
   int           capacity ()         const { return _capacity;  }
   int           increment()         const { return _increment; }
   ogc_object ** array    ()         const { return _array;     }

   ogc_object *  get         (int n) const;
   ogc_object *  operator [] (int n) const { return get(n);     }

   int add(ogc_object * element);

   typedef int (_CDECL OGC_COMPARE_RTN) (const void * p1, const void * p2);

   void   sort(OGC_COMPARE_RTN * rtn);
   void * find(ogc_object * key, bool sorted, OGC_COMPARE_RTN * rtn) const;

   static bool is_equal    (const ogc_vector * v1,
                            const ogc_vector * v2);
          bool is_equal    (const ogc_vector * v) const;

   static bool is_identical(const ogc_vector * p1,
                            const ogc_vector * p2);
          bool is_identical(const ogc_vector * p) const;
};

/* ------------------------------------------------------------------------- */
/* Citation                                                                  */
/* ------------------------------------------------------------------------- */

class OGC_EXPORT ogc_citation : public ogc_object
{
private:
   OGC_TEXT _text;

   ogc_citation() {}

public:
   static const char * obj_kwd();
   static bool is_kwd(const char * kwd);

   static ogc_citation * create(
      const char * text,
      ogc_error *  err = OGC_NULL);

   virtual ~ogc_citation();
   static ogc_citation * destroy(ogc_citation * obj);

   static ogc_citation * from_tokens(
      const ogc_token * t,
      int               start,
      int *             pend,
      ogc_error *       err = OGC_NULL);

   static ogc_citation * from_wkt(
      const char * wkt,
      ogc_error *  err = OGC_NULL);

   static bool to_wkt(
      const ogc_citation * obj,
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX);

   bool to_wkt(
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX) const;

   static ogc_citation * clone(const ogc_citation * obj);
          ogc_citation * clone() const;

   static bool is_equal    (const ogc_citation * p1,
                            const ogc_citation * p2);
          bool is_equal    (const ogc_citation * p) const;

   static bool is_identical(const ogc_citation * p1,
                            const ogc_citation * p2);
          bool is_identical(const ogc_citation * p) const;

   const char * text() const { return _text; }
};

/* ------------------------------------------------------------------------- */
/* URI                                                                       */
/* ------------------------------------------------------------------------- */

class OGC_EXPORT ogc_uri : public ogc_object
{
private:
   OGC_TEXT _text;

   ogc_uri() {}

public:
   static const char * obj_kwd();
   static bool is_kwd(const char * kwd);

   static ogc_uri * create(
      const char * text,
      ogc_error *  err = OGC_NULL);

   virtual ~ogc_uri();
   static ogc_uri * destroy(ogc_uri * obj);

   static ogc_uri * from_tokens(
      const ogc_token * t,
      int               start,
      int *             pend,
      ogc_error *       err = OGC_NULL);

   static ogc_uri * from_wkt(
      const char * wkt,
      ogc_error *  err = OGC_NULL);

   static bool to_wkt(
      const ogc_uri    * obj,
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX);

   bool to_wkt(
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX) const;

   static ogc_uri * clone(const ogc_uri * obj);
          ogc_uri * clone() const;

   static bool is_equal    (const ogc_uri * p1,
                            const ogc_uri * p2);
          bool is_equal    (const ogc_uri * p) const;

   static bool is_identical(const ogc_uri * p1,
                            const ogc_uri * p2);
          bool is_identical(const ogc_uri * p) const;

   const char * text() const { return _text; }
};

/* ------------------------------------------------------------------------- */
/* ID                                                                        */
/* ------------------------------------------------------------------------- */

class OGC_EXPORT ogc_id : public ogc_object
{
private:
   OGC_NAME        _name;
   OGC_TEXT        _identifier;
   OGC_TEXT        _version;
   ogc_citation *  _citation;
   ogc_uri *       _uri;

   ogc_id() {}

public:
   static const char * obj_kwd();
   static const char * old_kwd();
   static bool is_kwd(const char * kwd);

   static ogc_id * create(
      const char *   name,
      const char *   identifier,
      const char *   version,
      ogc_citation * citation,
      ogc_uri *      uri,
      ogc_error *    err = OGC_NULL);

   virtual ~ogc_id();
   static ogc_id * destroy(ogc_id * obj);

   static ogc_id * from_tokens(
      const ogc_token * t,
      int               start,
      int *             pend,
      ogc_error *       err = OGC_NULL);

   static ogc_id * from_wkt(
      const char * wkt,
      ogc_error *  err = OGC_NULL);

   static bool to_wkt(
      const ogc_id * obj,
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX);

   bool to_wkt(
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX) const;

   static ogc_id * clone(const ogc_id * obj);
          ogc_id * clone() const;

   static bool is_equal    (const ogc_id * p1,
                            const ogc_id * p2);
          bool is_equal    (const ogc_id * p) const;

   static bool is_identical(const ogc_id * p1,
                            const ogc_id * p2);
          bool is_identical(const ogc_id * p) const;

   const char *   name()       const { return _name;       }
   const char *   identifier() const { return _identifier; }
   const char *   version()    const { return _version;    }
   ogc_citation * citation()   const { return _citation;   }
   ogc_uri *      uri()        const { return _uri;        }
};

/* ------------------------------------------------------------------------- */
/* Remark                                                                    */
/* ------------------------------------------------------------------------- */

class OGC_EXPORT ogc_remark : public ogc_object
{
private:
   OGC_UTF8_TEXT _text;

   ogc_remark() {}

public:
   static const char * obj_kwd();
   static bool is_kwd(const char * kwd);

   static ogc_remark * create(
      const char * text,
      ogc_error *  err = OGC_NULL);

   virtual ~ogc_remark();
   static ogc_remark * destroy(ogc_remark * obj);

   static ogc_remark * from_tokens(
      const ogc_token * t,
      int               start,
      int *             pend,
      ogc_error *       err = OGC_NULL);

   static ogc_remark * from_wkt(
      const char * wkt,
      ogc_error *  err = OGC_NULL);

   static bool to_wkt(
      const ogc_remark * obj,
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX);

   bool to_wkt(
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX) const;

   static ogc_remark * clone(const ogc_remark * obj);
          ogc_remark * clone() const;

   static bool is_equal    (const ogc_remark * p1,
                            const ogc_remark * p2);
          bool is_equal    (const ogc_remark * p) const;

   static bool is_identical(const ogc_remark * p1,
                            const ogc_remark * p2);
          bool is_identical(const ogc_remark * p) const;

   const char * text() const { return _text; }
};

/* ------------------------------------------------------------------------- */
/* Generic unit                                                              */
/* ------------------------------------------------------------------------- */

class OGC_EXPORT ogc_unit : public ogc_object
{
protected:
   OGC_NAME      _name;
   ogc_unit_type _unit_type;
   double        _factor;           /* number of SI units per unit */
                                    /* SI units are:               */
                                    /*    angle       radians      */
                                    /*    length      meters       */
                                    /*    parametric  <various>    */
                                    /*    scale       unity        */
                                    /*    time        seconds      */
   ogc_vector *  _ids;

   ogc_unit() {}

public:
   static const char * obj_kwd();
   static bool is_kwd(const char * kwd);

   static ogc_unit * create(
      const char * name,
      double       factor,
      ogc_vector * ids,
      ogc_error *  err = OGC_NULL);

   virtual ~ogc_unit();
   static ogc_unit * destroy(ogc_unit * obj);

   static ogc_unit * from_tokens(
      const ogc_token * t,
      int               start,
      int *             pend,
      ogc_error *       err = OGC_NULL);

   static ogc_unit * from_wkt(
      const char * wkt,
      ogc_error *  err = OGC_NULL);

   static bool to_wkt(
      const ogc_unit   * obj,
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX);

   bool to_wkt(
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX) const;

   static ogc_unit * clone(const ogc_unit * obj);
          ogc_unit * clone() const;

   static bool is_equal    (const ogc_unit * p1,
                            const ogc_unit * p2);
          bool is_equal    (const ogc_unit * p) const;

   static bool is_identical(const ogc_unit * p1,
                            const ogc_unit * p2);
          bool is_identical(const ogc_unit * p) const;

   const char *  name()      const { return _name;      }
   ogc_unit_type unit_type() const { return _unit_type; }
   double        factor()    const { return _factor;    }
   ogc_vector *  ids()       const { return _ids;       }

   int           id_count()  const;
   ogc_id *      id(int n)   const;
};

/* ------------------------------------------------------------------------- */
/* Angle unit                                                                */
/* ------------------------------------------------------------------------- */

class OGC_EXPORT ogc_angunit : public ogc_unit
{
private:
   ogc_angunit() {}

public:
   static const char * obj_kwd();
   static const char * alt_kwd();
   static bool is_kwd(const char * kwd);

   static ogc_angunit * create(
      const char * name,
      double       factor,
      ogc_vector * ids,
      ogc_error *  err = OGC_NULL);

   virtual ~ogc_angunit();
   static ogc_angunit * destroy(ogc_angunit * obj);

   static ogc_angunit * from_tokens(
      const ogc_token * t,
      int               start,
      int *             pend,
      ogc_error *       err = OGC_NULL);

   static ogc_angunit * from_wkt(
      const char * wkt,
      ogc_error *  err = OGC_NULL);

   static bool to_wkt(
      const ogc_angunit * obj,
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX);

   bool to_wkt(
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX) const;

   static ogc_angunit * clone(const ogc_angunit * obj);
          ogc_angunit * clone() const;

   static bool is_equal    (const ogc_angunit * p1,
                            const ogc_angunit * p2);
          bool is_equal    (const ogc_angunit * p) const;

   static bool is_identical(const ogc_angunit * p1,
                            const ogc_angunit * p2);
          bool is_identical(const ogc_angunit * p) const;
};

/* ------------------------------------------------------------------------- */
/* Length unit                                                               */
/* ------------------------------------------------------------------------- */

class OGC_EXPORT ogc_lenunit : public ogc_unit
{
private:
   ogc_lenunit() {}

public:
   static const char * obj_kwd();
   static const char * alt_kwd();
   static bool is_kwd(const char * kwd);

   static ogc_lenunit * create(
      const char * name,
      double       factor,
      ogc_vector * ids,
      ogc_error *  err = OGC_NULL);

   virtual ~ogc_lenunit();
   static ogc_lenunit * destroy(ogc_lenunit * obj);

   static ogc_lenunit * from_tokens(
      const ogc_token * t,
      int               start,
      int *             pend,
      ogc_error *       err = OGC_NULL);

   static ogc_lenunit * from_wkt(
      const char * wkt,
      ogc_error *  err = OGC_NULL);

   static bool to_wkt(
      const ogc_lenunit * obj,
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX);

   bool to_wkt(
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX) const;

   static ogc_lenunit * clone(const ogc_lenunit * obj);
          ogc_lenunit * clone() const;

   static bool is_equal    (const ogc_lenunit * p1,
                            const ogc_lenunit * p2);
          bool is_equal    (const ogc_lenunit * p) const;

   static bool is_identical(const ogc_lenunit * p1,
                            const ogc_lenunit * p2);
          bool is_identical(const ogc_lenunit * p) const;
};

/* ------------------------------------------------------------------------- */
/* Scale unit                                                                */
/* ------------------------------------------------------------------------- */

class OGC_EXPORT ogc_scaleunit : public ogc_unit
{
private:
   ogc_scaleunit() {}

public:
   static const char * obj_kwd();
   static const char * alt_kwd();
   static bool is_kwd(const char * kwd);

   static ogc_scaleunit * create(
      const char * name,
      double       factor,
      ogc_vector * ids,
      ogc_error *  err = OGC_NULL);

   virtual ~ogc_scaleunit();
   static ogc_scaleunit * destroy(ogc_scaleunit * obj);

   static ogc_scaleunit * from_tokens(
      const ogc_token * t,
      int               start,
      int *             pend,
      ogc_error *       err = OGC_NULL);

   static ogc_scaleunit * from_wkt(
      const char * wkt,
      ogc_error *  err = OGC_NULL);

   static bool to_wkt(
      const ogc_scaleunit * obj,
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX);

   bool to_wkt(
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX) const;

   static ogc_scaleunit * clone(const ogc_scaleunit * obj);
          ogc_scaleunit * clone() const;

   static bool is_equal    (const ogc_scaleunit * p1,
                            const ogc_scaleunit * p2);
          bool is_equal    (const ogc_scaleunit * p) const;

   static bool is_identical(const ogc_scaleunit * p1,
                            const ogc_scaleunit * p2);
          bool is_identical(const ogc_scaleunit * p) const;
};

/* ------------------------------------------------------------------------- */
/* Temporal unit                                                             */
/* ------------------------------------------------------------------------- */

class OGC_EXPORT ogc_timeunit : public ogc_unit
{
private:
   ogc_timeunit() {}

public:
   static const char * obj_kwd();
   static const char * alt_kwd();
   static bool is_kwd(const char * kwd);

   static ogc_timeunit * create(
      const char * name,
      double       factor,
      ogc_vector * ids,
      ogc_error *  err = OGC_NULL);

   virtual ~ogc_timeunit();
   static ogc_timeunit * destroy(ogc_timeunit * obj);

   static ogc_timeunit * from_tokens(
      const ogc_token * t,
      int               start,
      int *             pend,
      ogc_error *       err = OGC_NULL);

   static ogc_timeunit * from_wkt(
      const char * wkt,
      ogc_error *  err = OGC_NULL);

   static bool to_wkt(
      const ogc_timeunit * obj,
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX);

   bool to_wkt(
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX) const;

   static ogc_timeunit * clone(const ogc_timeunit * obj);
          ogc_timeunit * clone() const;

   static bool is_equal    (const ogc_timeunit * p1,
                            const ogc_timeunit * p2);
          bool is_equal    (const ogc_timeunit * p) const;

   static bool is_identical(const ogc_timeunit * p1,
                            const ogc_timeunit * p2);
          bool is_identical(const ogc_timeunit * p) const;
};

/* ------------------------------------------------------------------------- */
/* Parametric unit                                                           */
/* ------------------------------------------------------------------------- */

class OGC_EXPORT ogc_paramunit : public ogc_unit
{
private:
   ogc_paramunit() {}

public:
   static const char * obj_kwd();
   static const char * alt_kwd();
   static bool is_kwd(const char * kwd);

   static ogc_paramunit * create(
      const char * name,
      double       factor,
      ogc_vector * ids,
      ogc_error *  err = OGC_NULL);

   virtual ~ogc_paramunit();
   static ogc_paramunit * destroy(ogc_paramunit * obj);

   static ogc_paramunit * from_tokens(
      const ogc_token * t,
      int               start,
      int *             pend,
      ogc_error *       err = OGC_NULL);

   static ogc_paramunit * from_wkt(
      const char * wkt,
      ogc_error *  err = OGC_NULL);

   static bool to_wkt(
      const ogc_paramunit * obj,
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX);

   bool to_wkt(
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX) const;

   static ogc_paramunit * clone(const ogc_paramunit * obj);
          ogc_paramunit * clone() const;

   static bool is_equal    (const ogc_paramunit * p1,
                            const ogc_paramunit * p2);
          bool is_equal    (const ogc_paramunit * p) const;

   static bool is_identical(const ogc_paramunit * p1,
                            const ogc_paramunit * p2);
          bool is_identical(const ogc_paramunit * p) const;
};

/* ------------------------------------------------------------------------- */
/* Scope                                                                     */
/* ------------------------------------------------------------------------- */

class OGC_EXPORT ogc_scope : public ogc_object
{
private:
   OGC_TEXT _text;

   ogc_scope() {}

public:
   static const char * obj_kwd();
   static bool is_kwd(const char * kwd);

   static ogc_scope * create(
      const char * text,
      ogc_error *  err = OGC_NULL);

   virtual ~ogc_scope();
   static ogc_scope * destroy(ogc_scope * obj);

   static ogc_scope * from_tokens(
      const ogc_token * t,
      int               start,
      int *             pend,
      ogc_error *       err = OGC_NULL);

   static ogc_scope * from_wkt(
      const char * wkt,
      ogc_error *  err = OGC_NULL);

   static bool to_wkt(
      const ogc_scope * obj,
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX);

   bool to_wkt(
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX) const;

   static ogc_scope * clone(const ogc_scope * obj);
          ogc_scope * clone() const;

   static bool is_equal    (const ogc_scope * p1,
                            const ogc_scope * p2);
          bool is_equal    (const ogc_scope * p) const;

   static bool is_identical(const ogc_scope * p1,
                            const ogc_scope * p2);
          bool is_identical(const ogc_scope * p) const;

   const char * text() const { return _text; }
};

/* ------------------------------------------------------------------------- */
/* Base extent                                                               */
/* ------------------------------------------------------------------------- */

class OGC_EXPORT ogc_extent : public ogc_object
{
protected:
   ogc_extent() {}

public:
   static const char * obj_kwd();
   static bool is_kwd(const char * kwd);

   virtual ~ogc_extent();
   static ogc_extent * destroy(ogc_extent * obj);

   static ogc_extent * from_tokens(
      const ogc_token * t,
      int               start,
      int *             pend,
      ogc_error *       err = OGC_NULL);

   static ogc_extent * from_wkt(
      const char * wkt,
      ogc_error *  err = OGC_NULL);

   static bool to_wkt(
      const ogc_extent * obj,
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX);

   virtual bool to_wkt(
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX) const;

   static ogc_extent * clone(const ogc_extent * obj);
          ogc_extent * clone() const;

   static bool is_equal    (const ogc_extent * p1,
                            const ogc_extent * p2);
          bool is_equal    (const ogc_extent * p) const;

   static bool is_identical(const ogc_extent * p1,
                            const ogc_extent * p2);
          bool is_identical(const ogc_extent * p) const;
};

/* ------------------------------------------------------------------------- */
/* Area extent                                                               */
/* ------------------------------------------------------------------------- */

class OGC_EXPORT ogc_area_extent : public ogc_extent
{
private:
   OGC_TEXT _text;

   ogc_area_extent() {}

public:
   static const char * obj_kwd();
   static bool is_kwd(const char * kwd);

   static ogc_area_extent * create(
      const char * text,
      ogc_error *  err = OGC_NULL);

   virtual ~ogc_area_extent();
   static ogc_area_extent * destroy(ogc_area_extent * obj);

   static ogc_area_extent * from_tokens(
      const ogc_token * t,
      int               start,
      int *             pend,
      ogc_error *       err = OGC_NULL);

   static ogc_area_extent * from_wkt(
      const char * wkt,
      ogc_error *  err = OGC_NULL);

   static bool to_wkt(
      const ogc_area_extent * obj,
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX);

   bool to_wkt(
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX) const;

   static ogc_area_extent * clone(const ogc_area_extent * obj);
          ogc_area_extent * clone() const;

   static bool is_equal    (const ogc_area_extent * p1,
                            const ogc_area_extent * p2);
          bool is_equal    (const ogc_area_extent * p) const;

   static bool is_identical(const ogc_area_extent * p1,
                            const ogc_area_extent * p2);
          bool is_identical(const ogc_area_extent * p) const;

   const char * text() const { return _text; }
};

/* ------------------------------------------------------------------------- */
/* Bounding box                                                              */
/* ------------------------------------------------------------------------- */

class OGC_EXPORT ogc_bbox_extent : public ogc_extent
{
private:
   double  _ll_lat;
   double  _ll_lon;
   double  _ur_lat;
   double  _ur_lon;

   ogc_bbox_extent() {}

public:
   static const char * obj_kwd();
   static bool is_kwd(const char * kwd);

   static ogc_bbox_extent * create(
      double  ll_lat,
      double  ll_lon,
      double  ur_lat,
      double  ur_lon,
      ogc_error *  err = OGC_NULL);

   virtual ~ogc_bbox_extent();
   static ogc_bbox_extent * destroy(ogc_bbox_extent * obj);

   static ogc_bbox_extent * from_tokens(
      const ogc_token * t,
      int               start,
      int *             pend,
      ogc_error *       err = OGC_NULL);

   static ogc_bbox_extent * from_wkt(
      const char * wkt,
      ogc_error *  err = OGC_NULL);

   static bool to_wkt(
      const ogc_bbox_extent * obj,
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX);

   bool to_wkt(
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX) const;

   static ogc_bbox_extent * clone(const ogc_bbox_extent * obj);
          ogc_bbox_extent * clone() const;

   static bool is_equal    (const ogc_bbox_extent * p1,
                            const ogc_bbox_extent * p2);
          bool is_equal    (const ogc_bbox_extent * p) const;

   static bool is_identical(const ogc_bbox_extent * p1,
                            const ogc_bbox_extent * p2);
          bool is_identical(const ogc_bbox_extent * p) const;

   double ll_lat() const { return _ll_lat; }
   double ll_lon() const { return _ll_lon; }
   double ur_lat() const { return _ur_lat; }
   double ur_lon() const { return _ur_lon; }
};

/* ------------------------------------------------------------------------- */
/* Vertical extent                                                           */
/* ------------------------------------------------------------------------- */

class OGC_EXPORT ogc_vert_extent : public ogc_extent
{
private:
   double        _min_ht;
   double        _max_ht;
   ogc_lenunit * _lenunit;

   ogc_vert_extent() {}

public:
   static const char * obj_kwd();
   static bool is_kwd(const char * kwd);

   static ogc_vert_extent * create(
      double        min_ht,
      double        max_ht,
      ogc_lenunit * lenunit,
      ogc_error *   err = OGC_NULL);

   virtual ~ogc_vert_extent();
   static ogc_vert_extent * destroy(ogc_vert_extent * obj);

   static ogc_vert_extent * from_tokens(
      const ogc_token * t,
      int               start,
      int *             pend,
      ogc_error *       err = OGC_NULL);

   static ogc_vert_extent * from_wkt(
      const char * wkt,
      ogc_error *  err = OGC_NULL);

   static bool to_wkt(
      const ogc_vert_extent * obj,
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX);

   bool to_wkt(
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX) const;

   static ogc_vert_extent * clone(const ogc_vert_extent * obj);
          ogc_vert_extent * clone() const;

   static bool is_equal    (const ogc_vert_extent * p1,
                            const ogc_vert_extent * p2);
          bool is_equal    (const ogc_vert_extent * p) const;

   static bool is_identical(const ogc_vert_extent * p1,
                            const ogc_vert_extent * p2);
          bool is_identical(const ogc_vert_extent * p) const;

   double        min_ht()  const { return _min_ht;  }
   double        max_ht()  const { return _max_ht;  }
   ogc_lenunit * lenunit() const { return _lenunit; }
};

/* ------------------------------------------------------------------------- */
/* Time extent                                                               */
/* ------------------------------------------------------------------------- */

class OGC_EXPORT ogc_time_extent : public ogc_extent
{
private:
   OGC_TIME       _start;
   OGC_TIME       _end;

   ogc_time_extent() {}

public:
   static const char * obj_kwd();
   static bool is_kwd(const char * kwd);

   static ogc_time_extent * create(
      const char *   start,
      const char *   end,
      ogc_error *    err = OGC_NULL);

   virtual ~ogc_time_extent();
   static ogc_time_extent * destroy(ogc_time_extent * obj);

   static ogc_time_extent * from_tokens(
      const ogc_token * t,
      int               start,
      int *             pend,
      ogc_error *       err = OGC_NULL);

   static ogc_time_extent * from_wkt(
      const char * wkt,
      ogc_error *  err = OGC_NULL);

   static bool to_wkt(
      const ogc_time_extent * obj,
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX);

   bool to_wkt(
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX) const;

   static ogc_time_extent * clone(const ogc_time_extent * obj);
          ogc_time_extent * clone() const;

   static bool is_equal    (const ogc_time_extent * p1,
                            const ogc_time_extent * p2);
          bool is_equal    (const ogc_time_extent * p) const;

   static bool is_identical(const ogc_time_extent * p1,
                            const ogc_time_extent * p2);
          bool is_identical(const ogc_time_extent * p) const;

   const char *   start()    const { return _start; }
   const char *   end()      const { return _end;   }
};

/* ------------------------------------------------------------------------- */
/* Parameter                                                                 */
/* ------------------------------------------------------------------------- */

class OGC_EXPORT ogc_parameter : public ogc_object
{
private:
   OGC_NAME     _name;
   double       _value;
   ogc_unit *   _unit;
   ogc_vector * _ids;

   ogc_parameter() {}

public:
   static const char * obj_kwd();
   static bool is_kwd(const char * kwd);

   static ogc_parameter * create(
      const char * name,
      double       value,
      ogc_unit *   unit,
      ogc_vector * ids,
      ogc_error *  err = OGC_NULL);

   virtual ~ogc_parameter();
   static ogc_parameter * destroy(ogc_parameter * obj);

   static ogc_parameter * from_tokens(
      const ogc_token * t,
      int               start,
      int *             pend,
      ogc_error *       err = OGC_NULL);

   static ogc_parameter * from_wkt(
      const char * wkt,
      ogc_error *  err = OGC_NULL);

   static bool to_wkt(
      const ogc_parameter * obj,
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX);

   bool to_wkt(
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX) const;

   static ogc_parameter * clone(const ogc_parameter * obj);
          ogc_parameter * clone() const;

   static bool is_equal    (const ogc_parameter * p1,
                            const ogc_parameter * p2);
          bool is_equal    (const ogc_parameter * p) const;

   static bool is_identical(const ogc_parameter * p1,
                            const ogc_parameter * p2);
          bool is_identical(const ogc_parameter * p) const;

   const char * name()     const { return _name;  }
   double       value()    const { return _value; }
   ogc_unit *   unit()     const { return _unit;  }
   ogc_vector * ids()      const { return _ids;   }

   int          id_count() const;
   ogc_id *     id(int n)  const;
};

/* ------------------------------------------------------------------------- */
/* File parameter                                                            */
/* ------------------------------------------------------------------------- */

class OGC_EXPORT ogc_param_file : public ogc_object
{
private:
   OGC_NAME     _name;
   OGC_PATH     _filename;
   ogc_vector * _ids;

   ogc_param_file() {}

public:
   static const char * obj_kwd();
   static bool is_kwd(const char * kwd);

   static ogc_param_file * create(
      const char * name,
      const char * filename,
      ogc_vector * ids,
      ogc_error *  err = OGC_NULL);

   virtual ~ogc_param_file();
   static ogc_param_file * destroy(ogc_param_file * obj);

   static ogc_param_file * from_tokens(
      const ogc_token * t,
      int               start,
      int *             pend,
      ogc_error *       err = OGC_NULL);

   static ogc_param_file * from_wkt(
      const char * wkt,
      ogc_error *  err = OGC_NULL);

   static bool to_wkt(
      const ogc_param_file * obj,
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX);

   bool to_wkt(
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX) const;

   static ogc_param_file * clone(const ogc_param_file * obj);
          ogc_param_file * clone() const;

   static bool is_equal    (const ogc_param_file * p1,
                            const ogc_param_file * p2);
          bool is_equal    (const ogc_param_file * p) const;

   static bool is_identical(const ogc_param_file * p1,
                            const ogc_param_file * p2);
          bool is_identical(const ogc_param_file * p) const;

   const char * name()     const { return _name;     }
   const char * filename() const { return _filename; }
   ogc_vector * ids()      const { return _ids;      }

   int          id_count() const;
   ogc_id *     id(int n)  const;
};

/* ------------------------------------------------------------------------- */
/* Ellipsoid                                                                 */
/* ------------------------------------------------------------------------- */

class OGC_EXPORT ogc_ellipsoid : public ogc_object
{
private:
   OGC_NAME      _name;
   double        _semi_major_axis;
   double        _flattening;
   ogc_lenunit * _lenunit;
   ogc_vector *  _ids;

   ogc_ellipsoid() {}

public:
   static const char * obj_kwd();
   static const char * alt_kwd();
   static bool is_kwd(const char * kwd);

   static ogc_ellipsoid * create(
      const char *  name,
      double        semi_major_axis,
      double        flattening,
      ogc_lenunit * lenunit,
      ogc_vector *  ids,
      ogc_error *   err = OGC_NULL);

   virtual ~ogc_ellipsoid();
   static ogc_ellipsoid * destroy(ogc_ellipsoid * obj);

   static ogc_ellipsoid * from_tokens(
      const ogc_token * t,
      int               start,
      int *             pend,
      ogc_error *       err = OGC_NULL);

   static ogc_ellipsoid * from_wkt(
      const char * wkt,
      ogc_error *  err = OGC_NULL);

   static bool to_wkt(
      const ogc_ellipsoid * obj,
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX);

   bool to_wkt(
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX) const;

   static ogc_ellipsoid * clone(const ogc_ellipsoid * obj);
          ogc_ellipsoid * clone() const;

   static bool is_equal    (const ogc_ellipsoid * p1,
                            const ogc_ellipsoid * p2);
          bool is_equal    (const ogc_ellipsoid * p) const;

   static bool is_identical(const ogc_ellipsoid * p1,
                            const ogc_ellipsoid * p2);
          bool is_identical(const ogc_ellipsoid * p) const;

   const char *  name()            const { return _name;            }
   double        semi_major_axis() const { return _semi_major_axis; }
   double        flattening()      const { return _flattening;      }
   ogc_lenunit * lenunit()         const { return _lenunit;         }
   ogc_vector *  ids()             const { return _ids;             }

   int           id_count()        const;
   ogc_id *      id(int n)         const;

   /* calculated values */

   double        semi_minor_axis() const;
   double        e_squared()       const;
   bool          is_sphere()       const;
};

/* ------------------------------------------------------------------------- */
/* Datum anchor                                                              */
/* ------------------------------------------------------------------------- */

class OGC_EXPORT ogc_anchor : public ogc_object
{
private:
   OGC_TEXT _text;

   ogc_anchor() {}

public:
   static const char * obj_kwd();
   static bool is_kwd(const char * kwd);

   static ogc_anchor * create(
      const char * text,
      ogc_error *  err = OGC_NULL);

   virtual ~ogc_anchor();
   static ogc_anchor * destroy(ogc_anchor * obj);

   static ogc_anchor * from_tokens(
      const ogc_token * t,
      int               start,
      int *             pend,
      ogc_error *       err = OGC_NULL);

   static ogc_anchor * from_wkt(
      const char * wkt,
      ogc_error *  err = OGC_NULL);

   static bool to_wkt(
      const ogc_anchor * obj,
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX);

   bool to_wkt(
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX) const;

   static ogc_anchor * clone(const ogc_anchor * obj);
          ogc_anchor * clone() const;

   static bool is_equal    (const ogc_anchor * p1,
                            const ogc_anchor * p2);
          bool is_equal    (const ogc_anchor * p) const;

   static bool is_identical(const ogc_anchor * p1,
                            const ogc_anchor * p2);
          bool is_identical(const ogc_anchor * p) const;

   const char * text() const { return _text; }
};

/* ------------------------------------------------------------------------- */
/* Time origin                                                               */
/* ------------------------------------------------------------------------- */

class OGC_EXPORT ogc_time_origin : public ogc_extent
{
private:
   OGC_TIME _origin;

   ogc_time_origin() {}

public:
   static const char * obj_kwd();
   static bool is_kwd(const char * kwd);

   static ogc_time_origin * create(
      const char *   origin,
      ogc_error *    err = OGC_NULL);

   virtual ~ogc_time_origin();
   static ogc_time_origin * destroy(ogc_time_origin * obj);

   static ogc_time_origin * from_tokens(
      const ogc_token * t,
      int               start,
      int *             pend,
      ogc_error *       err = OGC_NULL);

   static ogc_time_origin * from_wkt(
      const char * wkt,
      ogc_error *  err = OGC_NULL);

   static bool to_wkt(
      const ogc_time_origin * obj,
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX);

   bool to_wkt(
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX) const;

   static ogc_time_origin * clone(const ogc_time_origin * obj);
          ogc_time_origin * clone() const;

   static bool is_equal    (const ogc_time_origin * p1,
                            const ogc_time_origin * p2);
          bool is_equal    (const ogc_time_origin * p) const;

   static bool is_identical(const ogc_time_origin * p1,
                            const ogc_time_origin * p2);
          bool is_identical(const ogc_time_origin * p) const;

   const char * origin() const { return _origin; }
};

/* ------------------------------------------------------------------------- */
/* Base datum                                                                */
/* ------------------------------------------------------------------------- */

class OGC_EXPORT ogc_datum : public ogc_object
{
protected:
   OGC_NAME       _name;
   ogc_datum_type _datum_type;
   ogc_anchor *   _anchor;
   ogc_vector *   _ids;

   ogc_datum() {}

public:
   static const char * obj_kwd();
   static bool is_kwd(const char * kwd);

   virtual ~ogc_datum();
   static ogc_datum * destroy(ogc_datum * obj);

   static ogc_datum * from_tokens(
      const ogc_token * t,
      int               start,
      int *             pend,
      ogc_error *       err = OGC_NULL);

   static ogc_datum * from_wkt(
      const char * wkt,
      ogc_error *  err = OGC_NULL);

   static bool to_wkt(
      const ogc_datum * obj,
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX);

   virtual bool to_wkt(
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX) const;

   static ogc_datum * clone(const ogc_datum * obj);
          ogc_datum * clone() const;

   static bool is_equal    (const ogc_datum * p1,
                            const ogc_datum * p2);
          bool is_equal    (const ogc_datum * p) const;

   static bool is_identical(const ogc_datum * p1,
                            const ogc_datum * p2);
          bool is_identical(const ogc_datum * p) const;

   const char *   name()       const { return _name;       }
   ogc_datum_type datum_type() const { return _datum_type; }
   ogc_anchor *   anchor()     const { return _anchor;     }
   ogc_vector *   ids()        const { return _ids;        }

   int            id_count()   const;
   ogc_id *       id(int n)    const;
};

/* ------------------------------------------------------------------------- */
/* Horizontal (geodetic) datum                                               */
/* ------------------------------------------------------------------------- */

class OGC_EXPORT ogc_geod_datum : public ogc_datum
{
private:
   ogc_ellipsoid * _ellipsoid;

   ogc_geod_datum() {}

public:
   static const char * obj_kwd();
   static const char * alt_kwd();
   static bool is_kwd(const char * kwd);

   static ogc_geod_datum * create(
      const char *    name,
      ogc_ellipsoid * ellipsoid,
      ogc_anchor *    anchor,
      ogc_vector *    ids,
      ogc_error *     err = OGC_NULL);

   virtual ~ogc_geod_datum();
   static ogc_geod_datum * destroy(ogc_geod_datum * obj);

   static ogc_geod_datum * from_tokens(
      const ogc_token * t,
      int               start,
      int *             pend,
      ogc_error *       err = OGC_NULL);

   static ogc_geod_datum * from_wkt(
      const char * wkt,
      ogc_error *  err = OGC_NULL);

   static bool to_wkt(
      const ogc_geod_datum * obj,
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX);

   bool to_wkt(
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX) const;

   static ogc_geod_datum * clone(const ogc_geod_datum * obj);
          ogc_geod_datum * clone() const;

   static bool is_equal    (const ogc_geod_datum * p1,
                            const ogc_geod_datum * p2);
          bool is_equal    (const ogc_geod_datum * p) const;

   static bool is_identical(const ogc_geod_datum * p1,
                            const ogc_geod_datum * p2);
          bool is_identical(const ogc_geod_datum * p) const;

   ogc_ellipsoid * ellipsoid() const { return _ellipsoid; }
};

/* ------------------------------------------------------------------------- */
/* Engineering datum                                                         */
/* ------------------------------------------------------------------------- */

class OGC_EXPORT ogc_engr_datum : public ogc_datum
{
private:
   ogc_engr_datum() {}

public:
   static const char * obj_kwd();
   static const char * alt_kwd();
   static const char * old_kwd();
   static bool is_kwd(const char * kwd);

   static ogc_engr_datum * create(
      const char * name,
      ogc_anchor * anchor,
      ogc_vector * ids,
      ogc_error *  err = OGC_NULL);

   virtual ~ogc_engr_datum();
   static ogc_engr_datum * destroy(ogc_engr_datum * obj);

   static ogc_engr_datum * from_tokens(
      const ogc_token * t,
      int               start,
      int *             pend,
      ogc_error *       err = OGC_NULL);

   static ogc_engr_datum * from_wkt(
      const char * wkt,
      ogc_error *  err = OGC_NULL);

   static bool to_wkt(
      const ogc_engr_datum * obj,
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX);

   bool to_wkt(
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX) const;

   static ogc_engr_datum * clone(const ogc_engr_datum * obj);
          ogc_engr_datum * clone() const;

   static bool is_equal    (const ogc_engr_datum * p1,
                            const ogc_engr_datum * p2);
          bool is_equal    (const ogc_engr_datum * p) const;

   static bool is_identical(const ogc_engr_datum * p1,
                            const ogc_engr_datum * p2);
          bool is_identical(const ogc_engr_datum * p) const;
};

/* ------------------------------------------------------------------------- */
/* Image datum                                                               */
/* ------------------------------------------------------------------------- */

class OGC_EXPORT ogc_image_datum : public ogc_datum
{
private:
   ogc_pixel_type   _pixel_type;

   ogc_image_datum() {}

public:
   static const char * obj_kwd();
   static const char * alt_kwd();
   static bool is_kwd(const char * kwd);

   static ogc_image_datum * create(
      const char *   name,
      ogc_pixel_type pixel_type,
      ogc_anchor *   anchor,
      ogc_vector *   ids,
      ogc_error *    err = OGC_NULL);

   virtual ~ogc_image_datum();
   static ogc_image_datum * destroy(ogc_image_datum * obj);

   static ogc_image_datum * from_tokens(
      const ogc_token * t,
      int               start,
      int *             pend,
      ogc_error *       err = OGC_NULL);

   static ogc_image_datum * from_wkt(
      const char * wkt,
      ogc_error *  err = OGC_NULL);

   static bool to_wkt(
      const ogc_image_datum * obj,
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX);

   bool to_wkt(
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX) const;

   static ogc_image_datum * clone(const ogc_image_datum * obj);
          ogc_image_datum * clone() const;

   static bool is_equal    (const ogc_image_datum * p1,
                            const ogc_image_datum * p2);
          bool is_equal    (const ogc_image_datum * p) const;

   static bool is_identical(const ogc_image_datum * p1,
                            const ogc_image_datum * p2);
          bool is_identical(const ogc_image_datum * p) const;

   ogc_pixel_type  pixel_type() const { return _pixel_type; } 
};

/* ------------------------------------------------------------------------- */
/* Parametric datum                                                          */
/* ------------------------------------------------------------------------- */

class OGC_EXPORT ogc_param_datum : public ogc_datum
{
private:
   ogc_param_datum() {}

public:
   static const char * obj_kwd();
   static const char * alt_kwd();
   static bool is_kwd(const char * kwd);

   static ogc_param_datum * create(
      const char * name,
      ogc_anchor * anchor,
      ogc_vector * ids,
      ogc_error *  err = OGC_NULL);

   virtual ~ogc_param_datum();
   static ogc_param_datum * destroy(ogc_param_datum * obj);

   static ogc_param_datum * from_tokens(
      const ogc_token * t,
      int               start,
      int *             pend,
      ogc_error *       err = OGC_NULL);

   static ogc_param_datum * from_wkt(
      const char * wkt,
      ogc_error *  err = OGC_NULL);

   static bool to_wkt(
      const ogc_param_datum * obj,
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX);

   bool to_wkt(
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX) const;

   static ogc_param_datum * clone(const ogc_param_datum * obj);
          ogc_param_datum * clone() const;

   static bool is_equal    (const ogc_param_datum * p1,
                            const ogc_param_datum * p2);
          bool is_equal    (const ogc_param_datum * p) const;

   static bool is_identical(const ogc_param_datum * p1,
                            const ogc_param_datum * p2);
          bool is_identical(const ogc_param_datum * p) const;
};

/* ------------------------------------------------------------------------- */
/* Time datum                                                                */
/* ------------------------------------------------------------------------- */

class OGC_EXPORT ogc_time_datum : public ogc_datum
{
private:
   ogc_time_datum() {}

public:
   static const char * obj_kwd();
   static const char * alt_kwd();
   static bool is_kwd(const char * kwd);

   static ogc_time_datum * create(
      const char * name,
      ogc_anchor * anchor,
      ogc_vector * ids,
      ogc_error *  err = OGC_NULL);

   virtual ~ogc_time_datum();
   static ogc_time_datum * destroy(ogc_time_datum * obj);

   static ogc_time_datum * from_tokens(
      const ogc_token * t,
      int               start,
      int *             pend,
      ogc_error *       err = OGC_NULL);

   static ogc_time_datum * from_wkt(
      const char * wkt,
      ogc_error *  err = OGC_NULL);

   static bool to_wkt(
      const ogc_time_datum * obj,
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX);

   bool to_wkt(
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX) const;

   static ogc_time_datum * clone(const ogc_time_datum * obj);
          ogc_time_datum * clone() const;

   static bool is_equal    (const ogc_time_datum * p1,
                            const ogc_time_datum * p2);
          bool is_equal    (const ogc_time_datum * p) const;

   static bool is_identical(const ogc_time_datum * p1,
                            const ogc_time_datum * p2);
          bool is_identical(const ogc_time_datum * p) const;
};

/* ------------------------------------------------------------------------- */
/* Vertical datum                                                            */
/* ------------------------------------------------------------------------- */

class OGC_EXPORT ogc_vert_datum : public ogc_datum
{
private:
   ogc_vert_datum() {}

public:
   static const char * obj_kwd();
   static const char * alt_kwd();
   static const char * old_kwd();
   static bool is_kwd(const char * kwd);

   static ogc_vert_datum * create(
      const char * name,
      ogc_anchor * anchor,
      ogc_vector * ids,
      ogc_error *  err = OGC_NULL);

   virtual ~ogc_vert_datum();
   static ogc_vert_datum * destroy(ogc_vert_datum * obj);

   static ogc_vert_datum * from_tokens(
      const ogc_token * t,
      int               start,
      int *             pend,
      ogc_error *       err = OGC_NULL);

   static ogc_vert_datum * from_wkt(
      const char * wkt,
      ogc_error *  err = OGC_NULL);

   static bool to_wkt(
      const ogc_vert_datum * obj,
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX);

   bool to_wkt(
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX) const;

   static ogc_vert_datum * clone(const ogc_vert_datum * obj);
          ogc_vert_datum * clone() const;

   static bool is_equal    (const ogc_vert_datum * p1,
                            const ogc_vert_datum * p2);
          bool is_equal    (const ogc_vert_datum * p) const;

   static bool is_identical(const ogc_vert_datum * p1,
                            const ogc_vert_datum * p2);
          bool is_identical(const ogc_vert_datum * p) const;
};

/* ------------------------------------------------------------------------- */
/* Prime meridian                                                            */
/* ------------------------------------------------------------------------- */

class OGC_EXPORT ogc_primem : public ogc_object
{
private:
   OGC_NAME      _name;
   double        _longitude;
   ogc_angunit * _angunit;
   ogc_vector *  _ids;

   ogc_primem() {}

public:
   static const char * obj_kwd();
   static const char * alt_kwd();
   static bool is_kwd(const char * kwd);

   static ogc_primem * create(
      const char *  name,
      double        longitude,
      ogc_angunit * angunit,
      ogc_vector *  ids,
      ogc_error *   err = OGC_NULL);

   virtual ~ogc_primem();
   static ogc_primem * destroy(ogc_primem * obj);

   static ogc_primem * from_tokens(
      const ogc_token * t,
      int               start,
      int *             pend,
      ogc_error *       err = OGC_NULL);

   static ogc_primem * from_wkt(
      const char * wkt,
      ogc_error *  err = OGC_NULL);

   static bool to_wkt(
      const ogc_primem * obj,
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX);

   bool to_wkt(
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX) const;

   static ogc_primem * clone(const ogc_primem * obj);
          ogc_primem * clone() const;

   static bool is_equal    (const ogc_primem * p1,
                            const ogc_primem * p2);
          bool is_equal    (const ogc_primem * p) const;

   static bool is_identical(const ogc_primem * p1,
                            const ogc_primem * p2);
          bool is_identical(const ogc_primem * p) const;

   const char *  name()      const { return _name;      }
   double        longitude() const { return _longitude; }
   ogc_angunit * angunit()   const { return _angunit;   }
   ogc_vector *  ids()       const { return _ids;       }

   int           id_count()  const;
   ogc_id *      id(int n)   const;

   /* GEOGCS version (with angunit of "degree" added) */

   static ogc_primem * from_tokens_geogcs(
      const ogc_token * t,
      int               start,
      int *             pend,
      ogc_error *       err = OGC_NULL);
};

/* ------------------------------------------------------------------------- */
/* Bearing                                                                   */
/* ------------------------------------------------------------------------- */

class OGC_EXPORT ogc_bearing : public ogc_object
{
private:
   double        _value;
   ogc_angunit * _angunit;

   ogc_bearing() {}

public:
   static const char * obj_kwd();
   static bool is_kwd(const char * kwd);

   static ogc_bearing * create(
      double        value,
      ogc_angunit * angunit,
      ogc_error *   err = OGC_NULL);

   virtual ~ogc_bearing();
   static ogc_bearing * destroy(ogc_bearing * obj);

   static ogc_bearing * from_tokens(
      const ogc_token * t,
      int               start,
      int *             pend,
      ogc_error *       err = OGC_NULL);

   static ogc_bearing * from_wkt(
      const char * wkt,
      ogc_error *  err = OGC_NULL);

   static bool to_wkt(
      const ogc_bearing * obj,
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX);

   bool to_wkt(
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX) const;

   static ogc_bearing * clone(const ogc_bearing * obj);
          ogc_bearing * clone() const;

   static bool is_equal    (const ogc_bearing * p1,
                            const ogc_bearing * p2);
          bool is_equal    (const ogc_bearing * p) const;

   static bool is_identical(const ogc_bearing * p1,
                            const ogc_bearing * p2);
          bool is_identical(const ogc_bearing * p) const;

   double        value()   const { return _value;   }
   ogc_angunit * angunit() const { return _angunit; }
};

/* ------------------------------------------------------------------------- */
/* Meridian                                                                  */
/* ------------------------------------------------------------------------- */

class OGC_EXPORT ogc_meridian : public ogc_object
{
private:
   double        _value;
   ogc_angunit * _angunit;

   ogc_meridian() {}

public:
   static const char * obj_kwd();
   static bool is_kwd(const char * kwd);

   static ogc_meridian * create(
      double        value,
      ogc_angunit * angunit,
      ogc_error *   err = OGC_NULL);

   virtual ~ogc_meridian();
   static ogc_meridian * destroy(ogc_meridian * obj);

   static ogc_meridian * from_tokens(
      const ogc_token * t,
      int               start,
      int *             pend,
      ogc_error *       err = OGC_NULL);

   static ogc_meridian * from_wkt(
      const char * wkt,
      ogc_error *  err = OGC_NULL);

   static bool to_wkt(
      const ogc_meridian * obj,
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX);

   bool to_wkt(
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX) const;

   static ogc_meridian * clone(const ogc_meridian * obj);
          ogc_meridian * clone() const;

   static bool is_equal    (const ogc_meridian * p1,
                            const ogc_meridian * p2);
          bool is_equal    (const ogc_meridian * p) const;

   static bool is_identical(const ogc_meridian * p1,
                            const ogc_meridian * p2);
          bool is_identical(const ogc_meridian * p) const;

   double        value()   const { return _value;   }
   ogc_angunit * angunit() const { return _angunit; }
};

/* ------------------------------------------------------------------------- */
/* Order                                                                     */
/* ------------------------------------------------------------------------- */

class OGC_EXPORT ogc_order : public ogc_object
{
private:
   int _value;

   ogc_order() {}

public:
   static const char * obj_kwd();
   static bool is_kwd(const char * kwd);

   static ogc_order * create(
      int         value,
      ogc_error * err = OGC_NULL);

   virtual ~ogc_order();
   static ogc_order * destroy(ogc_order * obj);

   static ogc_order * from_tokens(
      const ogc_token * t,
      int               start,
      int *             pend,
      ogc_error *       err = OGC_NULL);

   static ogc_order * from_wkt(
      const char * wkt,
      ogc_error *  err = OGC_NULL);

   static bool to_wkt(
      const ogc_order * obj,
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX);

   bool to_wkt(
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX) const;

   static ogc_order * clone(const ogc_order * obj);
          ogc_order * clone() const;

   static bool is_equal    (const ogc_order * p1,
                            const ogc_order * p2);
          bool is_equal    (const ogc_order * p) const;

   static bool is_identical(const ogc_order * p1,
                            const ogc_order * p2);
          bool is_identical(const ogc_order * p) const;

   int value() const { return _value; }
};

/* ------------------------------------------------------------------------- */
/* Axis                                                                      */
/* ------------------------------------------------------------------------- */

class OGC_EXPORT ogc_axis : public ogc_object
{
private:
   OGC_NAME           _name;
   OGC_NAME           _abbr;
   ogc_axis_direction _direction;
   ogc_bearing *      _bearing;
   ogc_meridian *     _meridian;
   ogc_order *        _order;
   ogc_unit *         _unit;
   ogc_vector *       _ids;

   ogc_axis() {}

public:
   static const char * obj_kwd();
   static bool is_kwd(const char * kwd);

   static ogc_axis * create(
      const char *        name,
      const char *        abbr,
      ogc_axis_direction  direction,
      ogc_bearing *       bearing,
      ogc_meridian *      meridian,
      ogc_order *         order,
      ogc_unit *          unit,
      ogc_vector *        ids,
      ogc_error *         err = OGC_NULL);

   virtual ~ogc_axis();
   static ogc_axis * destroy(ogc_axis * obj);

   static ogc_axis * from_tokens(
      const ogc_token * t,
      int               start,
      int *             pend,
      ogc_error *       err = OGC_NULL);

   static ogc_axis * from_wkt(
      const char * wkt,
      ogc_error *  err = OGC_NULL);

   static bool to_wkt(
      const ogc_axis * obj,
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX);

   bool to_wkt(
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX) const;

   static ogc_axis * clone(const ogc_axis * obj);
          ogc_axis * clone() const;

   static bool is_equal    (const ogc_axis * p1,
                            const ogc_axis * p2);
          bool is_equal    (const ogc_axis * p) const;

   static bool is_identical(const ogc_axis * p1,
                            const ogc_axis * p2);
          bool is_identical(const ogc_axis * p) const;

   const char *       name()           const { return _name;         }
   const char *       abbr()           const { return _abbr;         }
   ogc_axis_direction axis_direction() const { return _direction;    }
   ogc_bearing *      bearing()        const { return _bearing;      }
   ogc_meridian *     meridian()       const { return _meridian;     }
   ogc_order *        order()          const { return _order;        }
   ogc_unit *         unit()           const { return _unit;         }
   ogc_vector *       ids()            const { return _ids;          }

   const char *       name_or_abbr()   const { return (*_name != 0) ?
                                                      _name : _abbr; }

   const char *       abbr_or_name()   const { return (*_abbr != 0) ?
                                                      _abbr : _name; }

   void               name_and_abbr(OGC_NAME buf) const;

   int                id_count()       const;
   ogc_id *           id(int n)        const;
};

/* ------------------------------------------------------------------------- */
/* Coordinate system type                                                    */
/* ------------------------------------------------------------------------- */

class OGC_EXPORT ogc_cs : public ogc_object
{
private:
   ogc_cs_type  _cs_type;
   int          _dimension;
   ogc_vector * _ids;

   ogc_cs() {}

public:
   static const char * obj_kwd();
   static bool is_kwd(const char * kwd);

   static ogc_cs * create(
      ogc_cs_type  cs_type,
      int          dimension,
      ogc_vector * ids,
      ogc_error *  err = OGC_NULL);

   virtual ~ogc_cs();
   static ogc_cs * destroy(ogc_cs * obj);

   static ogc_cs * from_tokens(
      const ogc_token * t,
      int               start,
      int *             pend,
      ogc_error *       err = OGC_NULL);

   static ogc_cs * from_wkt(
      const char * wkt,
      ogc_error *  err = OGC_NULL);

   static bool to_wkt(
      const ogc_cs     * obj,
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX);

   bool to_wkt(
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX) const;

   static ogc_cs * clone(const ogc_cs * obj);
          ogc_cs * clone() const;

   static bool is_equal    (const ogc_cs * p1,
                            const ogc_cs * p2);
          bool is_equal    (const ogc_cs * p) const;

   static bool is_identical(const ogc_cs * p1,
                            const ogc_cs * p2);
          bool is_identical(const ogc_cs * p) const;

   ogc_cs_type  cs_type()   const { return _cs_type;   }
   int          dimension() const { return _dimension; }
   ogc_vector * ids()       const { return _ids;       }

   int          id_count()  const;
   ogc_id *     id(int n)   const;
};

/* ------------------------------------------------------------------------- */
/* Method                                                                    */
/* ------------------------------------------------------------------------- */

class OGC_EXPORT ogc_method : public ogc_object
{
private:
   OGC_NAME     _name;
   ogc_vector * _ids;

   ogc_method() {}

public:
   static const char * obj_kwd();
   static const char * alt_kwd();
   static bool is_kwd(const char * kwd);

   static ogc_method * create(
      const char * name,
      ogc_vector * ids,
      ogc_error *  err = OGC_NULL);

   virtual ~ogc_method();
   static ogc_method * destroy(ogc_method * obj);

   static ogc_method * from_tokens(
      const ogc_token * t,
      int               start,
      int *             pend,
      ogc_error *       err = OGC_NULL);

   static ogc_method * from_wkt(
      const char * wkt,
      ogc_error *  err = OGC_NULL);

   static bool to_wkt(
      const ogc_method * obj,
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX);

   bool to_wkt(
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX) const;

   static ogc_method * clone(const ogc_method * obj);
          ogc_method * clone() const;

   static bool is_equal    (const ogc_method * p1,
                            const ogc_method * p2);
          bool is_equal    (const ogc_method * p) const;

   static bool is_identical(const ogc_method * p1,
                            const ogc_method * p2);
          bool is_identical(const ogc_method * p) const;

   const char * name()     const { return _name; }
   ogc_vector * ids()      const { return _ids;  }

   int          id_count() const;
   ogc_id *     id(int n)  const;

   /* PROJECTION routines */

   static bool to_wkt_projection(
      const ogc_method * obj,
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX);

   bool to_wkt_projection(
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX) const;
};

/* ------------------------------------------------------------------------- */
/* Conversion                                                                */
/* ------------------------------------------------------------------------- */

class OGC_EXPORT ogc_conversion : public ogc_object
{
private:
   OGC_NAME     _name;
   ogc_method * _method;
   ogc_vector * _parameters;
   ogc_vector * _ids;

   ogc_conversion() {}

public:
   static const char * obj_kwd();
   static bool is_kwd(const char * kwd);

   static ogc_conversion * create(
      const char * name,
      ogc_method * method,
      ogc_vector * parameters,
      ogc_vector * ids,
      ogc_error *  err = OGC_NULL);

   virtual ~ogc_conversion();
   static ogc_conversion * destroy(ogc_conversion * obj);

   static ogc_conversion * from_tokens(
      const ogc_token * t,
      int               start,
      int *             pend,
      ogc_error *       err = OGC_NULL);

   static ogc_conversion * from_wkt(
      const char * wkt,
      ogc_error *  err = OGC_NULL);

   static bool to_wkt(
      const ogc_conversion * obj,
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX);

   bool to_wkt(
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX) const;

   static ogc_conversion * clone(const ogc_conversion * obj);
          ogc_conversion * clone() const;

   static bool is_equal    (const ogc_conversion * p1,
                            const ogc_conversion * p2);
          bool is_equal    (const ogc_conversion * p) const;

   static bool is_identical(const ogc_conversion * p1,
                            const ogc_conversion * p2);
          bool is_identical(const ogc_conversion * p) const;

   const char *     name()       const { return _name;       }
   ogc_method *     method()     const { return _method;     }
   ogc_vector *     parameters() const { return _parameters; }
   ogc_vector *     ids()        const { return _ids;        }

   int              parameter_count() const;
   ogc_parameter *  parameter(int n)  const;

   int             id_count() const;
   ogc_id *        id(int n)  const;
};

/* ------------------------------------------------------------------------- */
/* Deriving conversion                                                       */
/* ------------------------------------------------------------------------- */

class OGC_EXPORT ogc_deriving_conv : public ogc_object
{
private:
   OGC_NAME     _name;
   ogc_method * _method;
   ogc_vector * _parameters;
   ogc_vector * _param_files;
   ogc_vector * _ids;

   ogc_deriving_conv() {}

public:
   static const char * obj_kwd();
   static bool is_kwd(const char * kwd);

   static ogc_deriving_conv * create(
      const char * name,
      ogc_method * method,
      ogc_vector * parameters,
      ogc_vector * param_files,
      ogc_vector * ids,
      ogc_error *  err = OGC_NULL);

   virtual ~ogc_deriving_conv();
   static ogc_deriving_conv * destroy(ogc_deriving_conv * obj);

   static ogc_deriving_conv * from_tokens(
      const ogc_token * t,
      int               start,
      int *             pend,
      ogc_error *       err = OGC_NULL);

   static ogc_deriving_conv * from_wkt(
      const char * wkt,
      ogc_error *  err = OGC_NULL);

   static bool to_wkt(
      const ogc_deriving_conv * obj,
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX);

   bool to_wkt(
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX) const;

   static ogc_deriving_conv * clone(const ogc_deriving_conv * obj);
          ogc_deriving_conv * clone() const;

   static bool is_equal    (const ogc_deriving_conv * p1,
                            const ogc_deriving_conv * p2);
          bool is_equal    (const ogc_deriving_conv * p) const;

   static bool is_identical(const ogc_deriving_conv * p1,
                            const ogc_deriving_conv * p2);
          bool is_identical(const ogc_deriving_conv * p) const;

   const char *     name()        const { return _name;        }
   ogc_method *     method()      const { return _method;      }
   ogc_vector *     parameters()  const { return _parameters;  }
   ogc_vector *     param_files() const { return _param_files; }
   ogc_vector *     ids()         const { return _ids;         }

   int              parameter_count() const;
   ogc_parameter *  parameter(int n)  const;

   int              param_file_count() const;
   ogc_param_file * param_file(int n)  const;

   int             id_count() const;
   ogc_id *        id(int n)  const;
};

/* ------------------------------------------------------------------------- */
/* Coordinate reference system                                               */
/* ------------------------------------------------------------------------- */

class OGC_EXPORT ogc_crs : public ogc_object
{
protected:
   OGC_NAME     _name;
   ogc_crs_type _crs_type;
   ogc_cs *     _cs;
   ogc_axis *   _axis_1;
   ogc_axis *   _axis_2;
   ogc_axis *   _axis_3;
   ogc_unit *   _unit;
   ogc_scope *  _scope;
   ogc_vector * _extents;
   ogc_vector * _ids;
   ogc_remark * _remark;

   ogc_crs() {}

public:
   static const char * obj_kwd();
   static bool is_kwd(const char * kwd);

   virtual ~ogc_crs();
   static ogc_crs * destroy(ogc_crs * obj);

   static ogc_crs * from_tokens(
      const ogc_token * t,
      int               start,
      int *             pend,
      ogc_error *       err = OGC_NULL);

   static ogc_crs * from_wkt(
      const char * wkt,
      ogc_error *  err = OGC_NULL);

   static bool to_wkt(
      const ogc_crs * obj,
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX);

   virtual bool to_wkt(
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX) const;

   static ogc_crs * clone(const ogc_crs * obj);
          ogc_crs * clone() const;

   static bool is_equal    (const ogc_crs * p1,
                            const ogc_crs * p2);
          bool is_equal    (const ogc_crs * p) const;

   static bool is_identical(const ogc_crs * p1,
                            const ogc_crs * p2);
          bool is_identical(const ogc_crs * p) const;

   const char * name()          const { return _name;     }
   ogc_crs_type crs_type()      const { return _crs_type; }
   ogc_cs *     cs()            const { return _cs;       }
   ogc_axis *   axis_1()        const { return _axis_1;   }
   ogc_axis *   axis_2()        const { return _axis_2;   }
   ogc_axis *   axis_3()        const { return _axis_3;   }
   ogc_unit *   unit()          const { return _unit;     }
   ogc_scope *  scope()         const { return _scope;    }
   ogc_vector * extents()       const { return _extents;  }
   ogc_vector * ids()           const { return _ids;      }
   ogc_remark * remark()        const { return _remark;   }

   ogc_axis *   axis(int n)     const;

   int          extent_count()  const;
   ogc_extent * extent(int n)   const;

   int          id_count()      const;
   ogc_id *     id(int n)       const;
};

/* ------------------------------------------------------------------------- */
/* Geodetic coordinate reference system                                      */
/* ------------------------------------------------------------------------- */

class ogc_base_geod_crs;

class OGC_EXPORT ogc_geod_crs : public ogc_crs
{
protected:
   ogc_base_geod_crs * _base_crs;
   ogc_deriving_conv * _deriving_conv;
   ogc_geod_datum *    _datum;
   ogc_primem *        _primem;

   ogc_geod_crs() {}

public:
   static const char * obj_kwd();
   static const char * alt_kwd();
   static const char * old_kwd();
   static bool is_kwd(const char * kwd);

   static ogc_geod_crs * create(
      const char *        name,
      ogc_geod_datum *    datum,
      ogc_primem *        primem,
      ogc_cs *            cs,
      ogc_axis *          axis_1,
      ogc_axis *          axis_2,
      ogc_axis *          axis_3,
      ogc_unit *          unit,
      ogc_scope *         scope,
      ogc_vector *        extents,
      ogc_vector *        ids,
      ogc_remark *        remark,
      ogc_error *         err = OGC_NULL);

   static ogc_geod_crs * create(
      const char *        name,
      ogc_base_geod_crs * base_crs,
      ogc_deriving_conv * deriving_conv,
      ogc_cs *            cs,
      ogc_axis *          axis_1,
      ogc_axis *          axis_2,
      ogc_axis *          axis_3,
      ogc_unit *          unit,
      ogc_scope *         scope,
      ogc_vector *        extents,
      ogc_vector *        ids,
      ogc_remark *        remark,
      ogc_error *         err = OGC_NULL);

   virtual ~ogc_geod_crs();
   static ogc_geod_crs * destroy(ogc_geod_crs * obj);

   static ogc_geod_crs * from_tokens(
      const ogc_token * t,
      int               start,
      int *             pend,
      ogc_error *       err = OGC_NULL);

   static ogc_geod_crs * from_tokens_old(
      const ogc_token * t,
      int               start,
      int *             pend,
      ogc_error *       err = OGC_NULL);

   static ogc_geod_crs * from_wkt(
      const char * wkt,
      ogc_error *  err = OGC_NULL);

   static bool to_wkt(
      const ogc_geod_crs * obj,
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX);

   bool to_wkt(
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX) const;

   static ogc_geod_crs * clone(const ogc_geod_crs * obj);
          ogc_geod_crs * clone() const;

   static bool is_equal    (const ogc_geod_crs * p1,
                            const ogc_geod_crs * p2);
          bool is_equal    (const ogc_geod_crs * p) const;

   static bool is_identical(const ogc_geod_crs * p1,
                            const ogc_geod_crs * p2);
          bool is_identical(const ogc_geod_crs * p) const;

   ogc_geod_datum *    datum()         const { return _datum;         }
   ogc_primem *        primem()        const { return _primem;        }
   ogc_base_geod_crs * base_crs()      const { return _base_crs;      }
   ogc_deriving_conv * deriving_conv() const { return _deriving_conv; }
};

/* ------------------------------------------------------------------------- */
/* Base Geodetic coordinate reference system                                 */
/* ------------------------------------------------------------------------- */

class OGC_EXPORT ogc_base_geod_crs : public ogc_geod_crs
{
private:
   ogc_base_geod_crs() {}

public:
   static const char * obj_kwd();
   static const char * old_kwd();
   static bool is_kwd(const char * kwd);

   static ogc_base_geod_crs * create(
      const char *     name,
      ogc_geod_datum * datum,
      ogc_primem *     primem,
      ogc_unit *       unit,
      ogc_error *      err = OGC_NULL);

   virtual ~ogc_base_geod_crs();
   static ogc_base_geod_crs * destroy(ogc_base_geod_crs * obj);

   static ogc_base_geod_crs * from_tokens(
      const ogc_token * t,
      int               start,
      int *             pend,
      ogc_error *       err = OGC_NULL);

   static ogc_base_geod_crs * from_wkt(
      const char * wkt,
      ogc_error *  err = OGC_NULL);

   static bool to_wkt(
      const ogc_base_geod_crs * obj,
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX);

   bool to_wkt(
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX) const;

   static ogc_base_geod_crs * clone(const ogc_base_geod_crs * obj);
          ogc_base_geod_crs * clone() const;

   static bool is_equal    (const ogc_base_geod_crs * p1,
                            const ogc_base_geod_crs * p2);
          bool is_equal    (const ogc_base_geod_crs * p) const;

   static bool is_identical(const ogc_base_geod_crs * p1,
                            const ogc_base_geod_crs * p2);
          bool is_identical(const ogc_base_geod_crs * p) const;
};

/* ------------------------------------------------------------------------- */
/* Projected coordinate reference system                                     */
/* ------------------------------------------------------------------------- */

class ogc_base_geod_crs;

class OGC_EXPORT ogc_proj_crs : public ogc_crs
{
protected:
   ogc_base_geod_crs * _base_crs;
   ogc_conversion *    _conversion;

   ogc_proj_crs() {}

public:
   static const char * obj_kwd();
   static const char * alt_kwd();
   static const char * old_kwd();
   static bool is_kwd(const char * kwd);

   static ogc_proj_crs * create(
      const char *         name,
      ogc_base_geod_crs *  base_crs,
      ogc_conversion *     conversion,
      ogc_cs *             cs,
      ogc_axis *           axis_1,
      ogc_axis *           axis_2,
      ogc_lenunit *        lenunit,
      ogc_scope *          scope,
      ogc_vector *         extents,
      ogc_vector *         ids,
      ogc_remark *         remark,
      ogc_error *          err = OGC_NULL);

   virtual ~ogc_proj_crs();
   static ogc_proj_crs * destroy(ogc_proj_crs * obj);

   static ogc_proj_crs * from_tokens(
      const ogc_token * t,
      int               start,
      int *             pend,
      ogc_error *       err = OGC_NULL);

   static ogc_proj_crs * from_tokens_old(
      const ogc_token * t,
      int               start,
      int *             pend,
      ogc_error *       err = OGC_NULL);

   static ogc_proj_crs * from_wkt(
      const char * wkt,
      ogc_error *  err = OGC_NULL);

   static bool to_wkt(
      const ogc_proj_crs * obj,
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX);

   bool to_wkt(
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX) const;

   static ogc_proj_crs * clone(const ogc_proj_crs * obj);
          ogc_proj_crs * clone() const;

   static bool is_equal    (const ogc_proj_crs * p1,
                            const ogc_proj_crs * p2);
          bool is_equal    (const ogc_proj_crs * p) const;

   static bool is_identical(const ogc_proj_crs * p1,
                            const ogc_proj_crs * p2);
          bool is_identical(const ogc_proj_crs * p) const;

   ogc_base_geod_crs * base_crs()    const { return _base_crs;            }
   ogc_conversion *    conversion()  const { return _conversion;          }
   ogc_lenunit *       lenunit()     const { return (ogc_lenunit *)_unit; }
};

/* ------------------------------------------------------------------------- */
/* Base Projected coordinate reference system                                */
/* ------------------------------------------------------------------------- */

class OGC_EXPORT ogc_base_proj_crs : public ogc_proj_crs
{
private:
   ogc_base_proj_crs() {}

public:
   static const char * obj_kwd();
   static bool is_kwd(const char * kwd);

   static ogc_base_proj_crs * create(
      const char *         name,
      ogc_base_geod_crs *  base_crs,
      ogc_conversion *     conversion,
      ogc_unit *           unit,
      ogc_error *          err = OGC_NULL);

   virtual ~ogc_base_proj_crs();
   static ogc_base_proj_crs * destroy(ogc_base_proj_crs * obj);

   static ogc_base_proj_crs * from_tokens(
      const ogc_token * t,
      int               start,
      int *             pend,
      ogc_error *       err = OGC_NULL);

   static ogc_base_proj_crs * from_wkt(
      const char * wkt,
      ogc_error *  err = OGC_NULL);

   static bool to_wkt(
      const ogc_base_proj_crs * obj,
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX);

   bool to_wkt(
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX) const;

   static ogc_base_proj_crs * clone(const ogc_base_proj_crs * obj);
          ogc_base_proj_crs * clone() const;

   static bool is_equal    (const ogc_base_proj_crs * p1,
                            const ogc_base_proj_crs * p2);
          bool is_equal    (const ogc_base_proj_crs * p) const;

   static bool is_identical(const ogc_base_proj_crs * p1,
                            const ogc_base_proj_crs * p2);
          bool is_identical(const ogc_base_proj_crs * p) const;
};

/* ------------------------------------------------------------------------- */
/* Vertical coordinate reference system                                      */
/* ------------------------------------------------------------------------- */

class ogc_base_vert_crs;

class OGC_EXPORT ogc_vert_crs : public ogc_crs
{
protected:
   ogc_vert_datum *    _datum;
   ogc_base_vert_crs * _base_crs;
   ogc_deriving_conv * _deriving_conv;

   ogc_vert_crs() {}

public:
   static const char * obj_kwd();
   static const char * alt_kwd();
   static const char * old_kwd();
   static bool is_kwd(const char * kwd);

   static ogc_vert_crs * create(
      const char *         name,
      ogc_vert_datum *     datum,
      ogc_cs *             cs,
      ogc_axis *           axis_1,
      ogc_lenunit *        lenunit,
      ogc_scope *          scope,
      ogc_vector *         extents,
      ogc_vector *         ids,
      ogc_remark *         remark,
      ogc_error *          err = OGC_NULL);

   static ogc_vert_crs * create(
      const char *         name,
      ogc_base_vert_crs *  base_crs,
      ogc_deriving_conv *  deriving_conv,
      ogc_cs *             cs,
      ogc_axis *           axis_1,
      ogc_lenunit *        lenunit,
      ogc_scope *          scope,
      ogc_vector *         extents,
      ogc_vector *         ids,
      ogc_remark *         remark,
      ogc_error *          err = OGC_NULL);

   virtual ~ogc_vert_crs();
   static ogc_vert_crs * destroy(ogc_vert_crs * obj);

   static ogc_vert_crs * from_tokens(
      const ogc_token * t,
      int               start,
      int *             pend,
      ogc_error *       err = OGC_NULL);

   static ogc_vert_crs * from_tokens_old(
      const ogc_token * t,
      int               start,
      int *             pend,
      ogc_error *       err = OGC_NULL);

   static ogc_vert_crs * from_wkt(
      const char * wkt,
      ogc_error *  err = OGC_NULL);

   static bool to_wkt(
      const ogc_vert_crs * obj,
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX);

   bool to_wkt(
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX) const;

   static ogc_vert_crs * clone(const ogc_vert_crs * obj);
          ogc_vert_crs * clone() const;

   static bool is_equal    (const ogc_vert_crs * p1,
                            const ogc_vert_crs * p2);
          bool is_equal    (const ogc_vert_crs * p) const;

   static bool is_identical(const ogc_vert_crs * p1,
                            const ogc_vert_crs * p2);
          bool is_identical(const ogc_vert_crs * p) const;

   ogc_vert_datum *    datum()         const { return _datum;               }
   ogc_base_vert_crs * base_crs()      const { return _base_crs;            }
   ogc_deriving_conv * deriving_conv() const { return _deriving_conv;       }
   ogc_lenunit *       lenunit()       const { return (ogc_lenunit *)_unit; }
};

/* ------------------------------------------------------------------------- */
/* Base Vertical coordinate reference system                                 */
/* ------------------------------------------------------------------------- */

class OGC_EXPORT ogc_base_vert_crs : public ogc_vert_crs
{
private:
   ogc_base_vert_crs() {}

public:
   static const char * obj_kwd();
   static bool is_kwd(const char * kwd);

   static ogc_base_vert_crs * create(
      const char *         name,
      ogc_vert_datum *     datum,
      ogc_unit *           unit,
      ogc_error *          err = OGC_NULL);

   virtual ~ogc_base_vert_crs();
   static ogc_base_vert_crs * destroy(ogc_base_vert_crs * obj);

   static ogc_base_vert_crs * from_tokens(
      const ogc_token * t,
      int               start,
      int *             pend,
      ogc_error *       err = OGC_NULL);

   static ogc_base_vert_crs * from_wkt(
      const char * wkt,
      ogc_error *  err = OGC_NULL);

   static bool to_wkt(
      const ogc_base_vert_crs * obj,
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX);

   bool to_wkt(
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX) const;

   static ogc_base_vert_crs * clone(const ogc_base_vert_crs * obj);
          ogc_base_vert_crs * clone() const;

   static bool is_equal    (const ogc_base_vert_crs * p1,
                            const ogc_base_vert_crs * p2);
          bool is_equal    (const ogc_base_vert_crs * p) const;

   static bool is_identical(const ogc_base_vert_crs * p1,
                            const ogc_base_vert_crs * p2);
          bool is_identical(const ogc_base_vert_crs * p) const;
};

/* ------------------------------------------------------------------------- */
/* Engineering coordinate reference system                                   */
/* ------------------------------------------------------------------------- */

class OGC_EXPORT ogc_engr_crs : public ogc_crs
{
protected:
   ogc_engr_datum *    _datum;
   ogc_crs *           _base_crs;
   ogc_deriving_conv * _deriving_conv;

   ogc_engr_crs() {}

public:
   static const char * obj_kwd();
   static const char * alt_kwd();
   static const char * old_kwd();
   static bool is_kwd(const char * kwd);

   static ogc_engr_crs * create(
      const char *         name,
      ogc_engr_datum *     datum,
      ogc_cs *             cs,
      ogc_axis *           axis_1,
      ogc_axis *           axis_2,
      ogc_axis *           axis_3,
      ogc_unit *           unit,
      ogc_scope *          scope,
      ogc_vector *         extents,
      ogc_vector *         ids,
      ogc_remark *         remark,
      ogc_error *          err = OGC_NULL);

   static ogc_engr_crs * create(
      const char *         name,
      ogc_crs *            base_crs,
      ogc_deriving_conv *  deriving_conv,
      ogc_cs *             cs,
      ogc_axis *           axis_1,
      ogc_axis *           axis_2,
      ogc_axis *           axis_3,
      ogc_unit *           unit,
      ogc_scope *          scope,
      ogc_vector *         extents,
      ogc_vector *         ids,
      ogc_remark *         remark,
      ogc_error *          err = OGC_NULL);

   virtual ~ogc_engr_crs();
   static ogc_engr_crs * destroy(ogc_engr_crs * obj);

   static ogc_engr_crs * from_tokens(
      const ogc_token * t,
      int               start,
      int *             pend,
      ogc_error *       err = OGC_NULL);

   static ogc_engr_crs * from_tokens_old(
      const ogc_token * t,
      int               start,
      int *             pend,
      ogc_error *       err = OGC_NULL);

   static ogc_engr_crs * from_wkt(
      const char * wkt,
      ogc_error *  err = OGC_NULL);

   static bool to_wkt(
      const ogc_engr_crs * obj,
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX);

   bool to_wkt(
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX) const;

   static ogc_engr_crs * clone(const ogc_engr_crs * obj);
          ogc_engr_crs * clone() const;

   static bool is_equal    (const ogc_engr_crs * p1,
                            const ogc_engr_crs * p2);
          bool is_equal    (const ogc_engr_crs * p) const;

   static bool is_identical(const ogc_engr_crs * p1,
                            const ogc_engr_crs * p2);
          bool is_identical(const ogc_engr_crs * p) const;

   ogc_engr_datum *    datum()         const { return _datum;               }
   ogc_crs *           base_crs()      const { return _base_crs;            }
   ogc_deriving_conv * deriving_conv() const { return _deriving_conv;       }
   ogc_lenunit *       lenunit()       const { return (ogc_lenunit *)_unit; }
};

/* ------------------------------------------------------------------------- */
/* Base Engineering coordinate reference system                              */
/* ------------------------------------------------------------------------- */

class OGC_EXPORT ogc_base_engr_crs : public ogc_engr_crs
{
private:
   ogc_base_engr_crs() {}

public:
   static const char * obj_kwd();
   static bool is_kwd(const char * kwd);

   static ogc_base_engr_crs * create(
      const char *         name,
      ogc_engr_datum *     datum,
      ogc_unit *           unit,
      ogc_error *          err = OGC_NULL);

   virtual ~ogc_base_engr_crs();
   static ogc_base_engr_crs * destroy(ogc_base_engr_crs * obj);

   static ogc_base_engr_crs * from_tokens(
      const ogc_token * t,
      int               start,
      int *             pend,
      ogc_error *       err = OGC_NULL);

   static ogc_base_engr_crs * from_wkt(
      const char * wkt,
      ogc_error *  err = OGC_NULL);

   static bool to_wkt(
      const ogc_base_engr_crs * obj,
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX);

   bool to_wkt(
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX) const;

   static ogc_base_engr_crs * clone(const ogc_base_engr_crs * obj);
          ogc_base_engr_crs * clone() const;

   static bool is_equal    (const ogc_base_engr_crs * p1,
                            const ogc_base_engr_crs * p2);
          bool is_equal    (const ogc_base_engr_crs * p) const;

   static bool is_identical(const ogc_base_engr_crs * p1,
                            const ogc_base_engr_crs * p2);
          bool is_identical(const ogc_base_engr_crs * p) const;
};

/* ------------------------------------------------------------------------- */
/* Image coordinate reference system                                         */
/* ------------------------------------------------------------------------- */

class OGC_EXPORT ogc_image_crs : public ogc_crs
{
private:
   ogc_image_datum *  _datum;

   ogc_image_crs() {}

public:
   static const char * obj_kwd();
   static bool is_kwd(const char * kwd);

   static ogc_image_crs *  create(
      const char *         name,
      ogc_image_datum *    datum,
      ogc_cs *             cs,
      ogc_axis *           axis_1,
      ogc_axis *           axis_2,
      ogc_unit *           unit,
      ogc_scope *          scope,
      ogc_vector *         extents,
      ogc_vector *         ids,
      ogc_remark *         remark,
      ogc_error *          err = OGC_NULL);

   virtual ~ogc_image_crs();
   static ogc_image_crs * destroy(ogc_image_crs * obj);

   static ogc_image_crs * from_tokens(
      const ogc_token * t,
      int               start,
      int *             pend,
      ogc_error *       err = OGC_NULL);

   static ogc_image_crs * from_wkt(
      const char * wkt,
      ogc_error *  err = OGC_NULL);

   static bool to_wkt(
      const ogc_image_crs * obj,
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX);

   bool to_wkt(
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX) const;

   static ogc_image_crs * clone(const ogc_image_crs * obj);
          ogc_image_crs * clone() const;

   static bool is_equal    (const ogc_image_crs * p1,
                            const ogc_image_crs * p2);
          bool is_equal    (const ogc_image_crs * p) const;

   static bool is_identical(const ogc_image_crs * p1,
                            const ogc_image_crs * p2);
          bool is_identical(const ogc_image_crs * p) const;

   ogc_image_datum * datum() const { return _datum; }
};

/* ------------------------------------------------------------------------- */
/* Temporal coordinate reference system                                      */
/* ------------------------------------------------------------------------- */

class ogc_base_time_crs;

class OGC_EXPORT ogc_time_crs : public ogc_crs
{
protected:
   ogc_time_datum *    _datum;
   ogc_base_time_crs * _base_crs;
   ogc_deriving_conv * _deriving_conv;

   ogc_time_crs() {}

public:
   static const char * obj_kwd();
   static bool is_kwd(const char * kwd);

   static ogc_time_crs * create(
      const char *         name,
      ogc_time_datum *     datum,
      ogc_cs *             cs,
      ogc_axis *           axis_1,
      ogc_timeunit *       timeunit,
      ogc_scope *          scope,
      ogc_vector *         extents,
      ogc_vector *         ids,
      ogc_remark *         remark,
      ogc_error *          err = OGC_NULL);

   static ogc_time_crs * create(
      const char *         name,
      ogc_base_time_crs *  base_crs,
      ogc_deriving_conv *  deriving_conv,
      ogc_cs *             cs,
      ogc_axis *           axis_1,
      ogc_timeunit *       timeunit,
      ogc_scope *          scope,
      ogc_vector *         extents,
      ogc_vector *         ids,
      ogc_remark *         remark,
      ogc_error *          err = OGC_NULL);

   virtual ~ogc_time_crs();
   static ogc_time_crs * destroy(ogc_time_crs * obj);

   static ogc_time_crs * from_tokens(
      const ogc_token * t,
      int               start,
      int *             pend,
      ogc_error *       err = OGC_NULL);

   static ogc_time_crs * from_wkt(
      const char * wkt,
      ogc_error *  err = OGC_NULL);

   static bool to_wkt(
      const ogc_time_crs * obj,
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX);

   bool to_wkt(
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX) const;

   static ogc_time_crs * clone(const ogc_time_crs * obj);
          ogc_time_crs * clone() const;

   static bool is_equal    (const ogc_time_crs * p1,
                            const ogc_time_crs * p2);
          bool is_equal    (const ogc_time_crs * p) const;

   static bool is_identical(const ogc_time_crs * p1,
                            const ogc_time_crs * p2);
          bool is_identical(const ogc_time_crs * p) const;

   ogc_time_datum *    datum()         const { return _datum;                }
   ogc_base_time_crs * base_crs()      const { return _base_crs;             }
   ogc_deriving_conv * deriving_conv() const { return _deriving_conv;        }
   ogc_timeunit *      timeunit()      const { return (ogc_timeunit *)_unit; }
};

/* ------------------------------------------------------------------------- */
/* Base Temporal coordinate reference system                                 */
/* ------------------------------------------------------------------------- */

class OGC_EXPORT ogc_base_time_crs : public ogc_time_crs
{
private:
   ogc_base_time_crs() {}

public:
   static const char * obj_kwd();
   static bool is_kwd(const char * kwd);

   static ogc_base_time_crs * create(
      const char *      name,
      ogc_time_datum *  datum,
      ogc_unit *        unit,
      ogc_error *       err = OGC_NULL);

   virtual ~ogc_base_time_crs();
   static ogc_base_time_crs * destroy(ogc_base_time_crs * obj);

   static ogc_base_time_crs * from_tokens(
      const ogc_token * t,
      int               start,
      int *             pend,
      ogc_error *       err = OGC_NULL);

   static ogc_base_time_crs * from_wkt(
      const char * wkt,
      ogc_error *  err = OGC_NULL);

   static bool to_wkt(
      const ogc_base_time_crs * obj,
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX);

   bool to_wkt(
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX) const;

   static ogc_base_time_crs * clone(const ogc_base_time_crs * obj);
          ogc_base_time_crs * clone() const;

   static bool is_equal    (const ogc_base_time_crs * p1,
                            const ogc_base_time_crs * p2);
          bool is_equal    (const ogc_base_time_crs * p) const;

   static bool is_identical(const ogc_base_time_crs * p1,
                            const ogc_base_time_crs * p2);
          bool is_identical(const ogc_base_time_crs * p) const;
};

/* ------------------------------------------------------------------------- */
/* Parametric coordinate reference system                                    */
/* ------------------------------------------------------------------------- */

class ogc_base_param_crs;

class OGC_EXPORT ogc_param_crs : public ogc_crs
{
protected:
   ogc_param_datum *    _datum;
   ogc_base_param_crs * _base_crs;
   ogc_deriving_conv *  _deriving_conv;

   ogc_param_crs() {}

public:
   static const char * obj_kwd();
   static bool is_kwd(const char * kwd);

   static ogc_param_crs * create(
      const char *         name,
      ogc_param_datum *    datum,
      ogc_cs *             cs,
      ogc_axis *           axis_1,
      ogc_paramunit *      paramunit,
      ogc_scope *          scope,
      ogc_vector *         extents,
      ogc_vector *         ids,
      ogc_remark *         remark,
      ogc_error *          err = OGC_NULL);

   static ogc_param_crs * create(
      const char *         name,
      ogc_base_param_crs * base_crs,
      ogc_deriving_conv *  deriving_conv,
      ogc_cs *             cs,
      ogc_axis *           axis_1,
      ogc_paramunit *      paramunit,
      ogc_scope *          scope,
      ogc_vector *         extents,
      ogc_vector *         ids,
      ogc_remark *         remark,
      ogc_error *          err = OGC_NULL);

   virtual ~ogc_param_crs();
   static ogc_param_crs * destroy(ogc_param_crs * obj);

   static ogc_param_crs * from_tokens(
      const ogc_token * t,
      int               start,
      int *             pend,
      ogc_error *       err = OGC_NULL);

   static ogc_param_crs * from_wkt(
      const char * wkt,
      ogc_error *  err = OGC_NULL);

   static bool to_wkt(
      const ogc_param_crs * obj,
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX);

   bool to_wkt(
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX) const;

   static ogc_param_crs * clone(const ogc_param_crs * obj);
          ogc_param_crs * clone() const;

   static bool is_equal    (const ogc_param_crs * p1,
                            const ogc_param_crs * p2);
          bool is_equal    (const ogc_param_crs * p) const;

   static bool is_identical(const ogc_param_crs * p1,
                            const ogc_param_crs * p2);
          bool is_identical(const ogc_param_crs * p) const;

   ogc_param_datum *    datum()         const { return _datum;                 }
   ogc_base_param_crs * base_crs()      const { return _base_crs;              }
   ogc_deriving_conv *  deriving_conv() const { return _deriving_conv;         }
   ogc_paramunit *      paramunit()     const { return (ogc_paramunit *)_unit; }
};

/* ------------------------------------------------------------------------- */
/* Base Parametric coordinate reference system                               */
/* ------------------------------------------------------------------------- */

class OGC_EXPORT ogc_base_param_crs : public ogc_param_crs
{
private:
   ogc_base_param_crs() {}

public:
   static const char * obj_kwd();
   static bool is_kwd(const char * kwd);

   static ogc_base_param_crs * create(
      const char *      name,
      ogc_param_datum * datum,
      ogc_unit *        unit,
      ogc_error *       err = OGC_NULL);

   virtual ~ogc_base_param_crs();
   static ogc_base_param_crs * destroy(ogc_base_param_crs * obj);

   static ogc_base_param_crs * from_tokens(
      const ogc_token * t,
      int               start,
      int *             pend,
      ogc_error *       err = OGC_NULL);

   static ogc_base_param_crs * from_wkt(
      const char * wkt,
      ogc_error *  err = OGC_NULL);

   static bool to_wkt(
      const ogc_base_param_crs * obj,
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX);

   bool to_wkt(
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX) const;

   static ogc_base_param_crs * clone(const ogc_base_param_crs * obj);
          ogc_base_param_crs * clone() const;

   static bool is_equal    (const ogc_base_param_crs * p1,
                            const ogc_base_param_crs * p2);
          bool is_equal    (const ogc_base_param_crs * p) const;

   static bool is_identical(const ogc_base_param_crs * p1,
                            const ogc_base_param_crs * p2);
          bool is_identical(const ogc_base_param_crs * p) const;
};

/* ------------------------------------------------------------------------- */
/* Compound coordinate reference system                                      */
/* ------------------------------------------------------------------------- */

class OGC_EXPORT ogc_compound_crs : public ogc_crs
{
private:
   ogc_crs *          _first_crs;
   ogc_crs *          _second_crs;
   ogc_time_crs *     _third_crs;

   ogc_compound_crs() {}

public:
   static const char * obj_kwd();
   static const char * old_kwd();
   static bool is_kwd(const char * kwd);

   static ogc_compound_crs * create(
      const char *       name,
      ogc_crs *          first_crs,
      ogc_crs *          second_crs,
      ogc_time_crs *     third_crs,
      ogc_vector *       ids,
      ogc_remark *       remark,
      ogc_error *        err = OGC_NULL);

   virtual ~ogc_compound_crs();
   static ogc_compound_crs * destroy(ogc_compound_crs * obj);

   static ogc_compound_crs * from_tokens(
      const ogc_token * t,
      int               start,
      int *             pend,
      ogc_error *       err = OGC_NULL);

   static ogc_compound_crs * from_tokens_old(
      const ogc_token * t,
      int               start,
      int *             pend,
      ogc_error *       err = OGC_NULL);

   static ogc_compound_crs * from_wkt(
      const char * wkt,
      ogc_error *  err = OGC_NULL);

   static bool to_wkt(
      const ogc_compound_crs * obj,
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX);

   bool to_wkt(
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX) const;

   static ogc_compound_crs * clone(const ogc_compound_crs * obj);
          ogc_compound_crs * clone() const;

   static bool is_equal    (const ogc_compound_crs * p1,
                            const ogc_compound_crs * p2);
          bool is_equal    (const ogc_compound_crs * p) const;

   static bool is_identical(const ogc_compound_crs * p1,
                            const ogc_compound_crs * p2);
          bool is_identical(const ogc_compound_crs * p) const;

   ogc_crs *      first_crs()  const { return _first_crs;  }
   ogc_crs *      second_crs() const { return _second_crs; }
   ogc_time_crs * third_crs()  const { return _third_crs;  }
};

/* ------------------------------------------------------------------------- */
/* Operation accuracy                                                        */
/* ------------------------------------------------------------------------- */

class OGC_EXPORT ogc_op_accuracy : public ogc_object
{
private:
   double  _accuracy;

   ogc_op_accuracy() {}

public:
   static const char * obj_kwd();
   static bool is_kwd(const char * kwd);

   static ogc_op_accuracy * create(
      double        accuracy,
      ogc_error *   err = OGC_NULL);

   virtual ~ogc_op_accuracy();
   static ogc_op_accuracy * destroy(ogc_op_accuracy * obj);

   static ogc_op_accuracy * from_tokens(
      const ogc_token * t,
      int               start,
      int *             pend,
      ogc_error *       err = OGC_NULL);

   static ogc_op_accuracy * from_wkt(
      const char * wkt,
      ogc_error *  err = OGC_NULL);

   static bool to_wkt(
      const ogc_op_accuracy * obj,
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX);

   bool to_wkt(
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX) const;

   static ogc_op_accuracy * clone(const ogc_op_accuracy * obj);
          ogc_op_accuracy * clone() const;

   static bool is_equal    (const ogc_op_accuracy * p1,
                            const ogc_op_accuracy * p2);
          bool is_equal    (const ogc_op_accuracy * p) const;

   static bool is_identical(const ogc_op_accuracy * p1,
                            const ogc_op_accuracy * p2);
          bool is_identical(const ogc_op_accuracy * p) const;

   double      accuracy()  const { return _accuracy; }
};

/* ------------------------------------------------------------------------- */
/* Coordinate operation                                                      */
/* ------------------------------------------------------------------------- */

class OGC_EXPORT ogc_coord_op : public ogc_object
{
private:
   OGC_NAME          _name;
   ogc_crs *         _source_crs;
   ogc_crs *         _target_crs;
   ogc_crs *         _interp_crs;
   ogc_method *      _method;
   ogc_vector *      _parameters;
   ogc_vector *      _param_files;
   ogc_op_accuracy * _op_accuracy;
   ogc_scope *       _scope;
   ogc_vector *      _extents;
   ogc_vector *      _ids;
   ogc_remark *      _remark;

   ogc_coord_op() {}

public:
   static const char * obj_kwd();
   static bool is_kwd(const char * kwd);

   static ogc_coord_op * create(
      const char *      name,
      ogc_crs *         source_crs,
      ogc_crs *         target_crs,
      ogc_crs *         interp_crs,
      ogc_method *      method,
      ogc_vector *      parameters,
      ogc_vector *      param_files,
      ogc_op_accuracy * op_accuracy,
      ogc_scope *       scope,
      ogc_vector *      extents,
      ogc_vector *      ids,
      ogc_remark *      remark,
      ogc_error *       err = OGC_NULL);

   virtual ~ogc_coord_op();
   static ogc_coord_op * destroy(ogc_coord_op * obj);

   static ogc_coord_op * from_tokens(
      const ogc_token * t,
      int               start,
      int *             pend,
      ogc_error *       err = OGC_NULL);

   static ogc_coord_op * from_wkt(
      const char * wkt,
      ogc_error *  err = OGC_NULL);

   static bool to_wkt(
      const ogc_coord_op * obj,
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX);

   bool to_wkt(
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX) const;

   static ogc_coord_op * clone(const ogc_coord_op * obj);
          ogc_coord_op * clone() const;

   static bool is_equal    (const ogc_coord_op * p1,
                            const ogc_coord_op * p2);
          bool is_equal    (const ogc_coord_op * p) const;

   static bool is_identical(const ogc_coord_op * p1,
                            const ogc_coord_op * p2);
          bool is_identical(const ogc_coord_op * p) const;

   const char *      name()        const { return _name;        }
   ogc_crs *         source_crs()  const { return _source_crs;  }
   ogc_crs *         target_crs()  const { return _target_crs;  }
   ogc_crs *         interp_crs()  const { return _interp_crs;  }
   ogc_method *      method()      const { return _method;      }
   ogc_vector *      parameters()  const { return _parameters;  }
   ogc_vector *      param_files() const { return _param_files; }
   ogc_op_accuracy * op_accuracy() const { return _op_accuracy; }
   ogc_scope *       scope()       const { return _scope;       }
   ogc_vector *      extents()     const { return _extents;     }
   ogc_vector *      ids()         const { return _ids;         }
   ogc_remark *      remark()      const { return _remark;      }

   int                  parameter_count()  const;
   ogc_parameter *      parameter(int n)   const;

   int                  param_file_count() const;
   ogc_param_file *     param_file(int n)  const;

   int                  extent_count()     const;
   ogc_extent *         extent(int n)      const;

   int                  id_count()         const;
   ogc_id *             id(int n)          const;
};

/* ------------------------------------------------------------------------- */
/* Abridged coordinate transformation                                        */
/* ------------------------------------------------------------------------- */

class OGC_EXPORT ogc_abrtrans : public ogc_object
{
private:
   OGC_NAME     _name;
   ogc_method * _method;
   ogc_vector * _parameters;
   ogc_vector * _param_files;
   ogc_scope *  _scope;
   ogc_vector * _extents;
   ogc_vector * _ids;
   ogc_remark * _remark;

   ogc_abrtrans() {}

public:
   static const char * obj_kwd();
   static bool is_kwd(const char * kwd);

   static ogc_abrtrans * create(
      const char * name,
      ogc_method * method,
      ogc_vector * parameters,
      ogc_vector * param_files,
      ogc_scope *  scope,
      ogc_vector * extents,
      ogc_vector * ids,
      ogc_remark * remark,
      ogc_error *  err = OGC_NULL);

   virtual ~ogc_abrtrans();
   static ogc_abrtrans * destroy(ogc_abrtrans * obj);

   static ogc_abrtrans * from_tokens(
      const ogc_token * t,
      int               start,
      int *             pend,
      ogc_error *       err = OGC_NULL);

   static ogc_abrtrans * from_wkt(
      const char * wkt,
      ogc_error *  err = OGC_NULL);

   static bool to_wkt(
      const ogc_abrtrans * obj,
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX);

   bool to_wkt(
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX) const;

   static ogc_abrtrans * clone(const ogc_abrtrans * obj);
          ogc_abrtrans * clone() const;

   static bool is_equal    (const ogc_abrtrans * p1,
                            const ogc_abrtrans * p2);
          bool is_equal    (const ogc_abrtrans * p) const;

   static bool is_identical(const ogc_abrtrans * p1,
                            const ogc_abrtrans * p2);
          bool is_identical(const ogc_abrtrans * p) const;

   const char * name()        const { return _name;        }
   ogc_method * method()      const { return _method;      }
   ogc_vector * parameters()  const { return _parameters;  }
   ogc_vector * param_files() const { return _param_files; }
   ogc_scope *  scope()       const { return _scope;       }
   ogc_vector * extents()     const { return _extents;     }
   ogc_remark * remark()      const { return _remark;      }
   ogc_vector * ids()         const { return _ids;         }

   int              parameter_count()  const;
   ogc_parameter *  parameter(int n)   const;

   int              param_file_count() const;
   ogc_param_file * param_file(int n)  const;

   int              extent_count()     const;
   ogc_extent *     extent(int n)      const;

   int              id_count()         const;
   ogc_id *         id(int n)          const;
};

/* ------------------------------------------------------------------------- */
/* Bound coordinate reference system                                         */
/* ------------------------------------------------------------------------- */

class OGC_EXPORT ogc_bound_crs : public ogc_object
{
private:
   ogc_crs *      _source_crs;
   ogc_crs *      _target_crs;
   ogc_abrtrans * _abrtrans;
   ogc_vector *   _ids;
   ogc_remark *   _remark;

   ogc_bound_crs() {}

public:
   static const char * obj_kwd();
   static bool is_kwd(const char * kwd);

   static ogc_bound_crs * create(
      ogc_crs *      source_crs,
      ogc_crs *      target_crs,
      ogc_abrtrans * abrtrans,
      ogc_vector *   ids,
      ogc_remark *   remark,
      ogc_error *    err = OGC_NULL);

   virtual ~ogc_bound_crs();
   static ogc_bound_crs * destroy(ogc_bound_crs * obj);

   static ogc_bound_crs * from_tokens(
      const ogc_token * t,
      int               start,
      int *             pend,
      ogc_error *       err = OGC_NULL);

   static ogc_bound_crs * from_wkt(
      const char * wkt,
      ogc_error *  err = OGC_NULL);

   static bool to_wkt(
      const ogc_bound_crs * obj,
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX);

   bool to_wkt(
      char     buffer[],
      int      options = OGC_WKT_OPT_NONE,
      size_t   buflen  = OGC_BUFF_MAX) const;

   static ogc_bound_crs * clone(const ogc_bound_crs * obj);
          ogc_bound_crs * clone() const;

   static bool is_equal    (const ogc_bound_crs * p1,
                            const ogc_bound_crs * p2);
          bool is_equal    (const ogc_bound_crs * p) const;

   static bool is_identical(const ogc_bound_crs * p1,
                            const ogc_bound_crs * p2);
          bool is_identical(const ogc_bound_crs * p) const;

   ogc_crs *        source_crs()      const { return _source_crs; }
   ogc_crs *        target_crs()      const { return _target_crs; }
   ogc_abrtrans *   abrtrans()        const { return _abrtrans;   }
   ogc_remark *     remark()          const { return _remark;     }
   ogc_vector *     ids()             const { return _ids;        }

   int              id_count()        const;
   ogc_id *         id(int n)         const;
};

/* ------------------------------------------------------------------------- */
/* string functions                                                          */
/* ------------------------------------------------------------------------- */

class OGC_EXPORT ogc_string
{
private:
    ogc_string() {}
   ~ogc_string() {}

public:
   /* number conversion methods */

   static char   decimal_point_char();

   static bool   is_numeric    (const char * str);

   static int    atoi          (const char * str);
   static double atod          (const char * str);

   static char * itoa          (int    n, OGC_NBUF buf);
   static char * dtoa          (double d, OGC_NBUF buf);

   /* string methods */

   static bool   is_ignore_char(int c);
   static int    unignore_len  (const unsigned char *s);

   static int    strcmp_i      (const char * s1, const char * s2);

   static int    strncmp_ci    (const char * s1, const char * s2, size_t n);
   static int    strncmp_ci1   (const char * s1, const char * s2);
   static int    strncmp_ci2   (const char * s1, const char * s2);
   static int    strcmp_ci     (const char * s1, const char * s2);

   static bool   is_equal      (const char * s1, const char * s2)
                               { return (strcmp_ci(s1, s2) == 0); }

   static int    strupr        (char * buf, const char * str);
   static int    strlwr        (char * buf, const char * str);

   static int    strcpy        (char * buf, const char * str);
   static int    strncpy       (char * buf, const char * str, size_t n);

   static const char *
                 strstr_ci     (const char * str, const char * srch);

   static char * strip         (char * str);
   static char * trim          (char * str);
   static char * trim_buf      (char * str);

   static char * escape_str    (char * buf, const char * str, size_t buflen);
   static char * unescape_str  (char * buf, const char * str, size_t buflen);
   static int    unescape_len  (const char * str);
};

/* ------------------------------------------------------------------------- */
/* date/time methods                                                         */
/* ------------------------------------------------------------------------- */

class OGC_EXPORT ogc_datetime
{
private:
   int    _year;    /* CE year number             (0001  to 9999)         */
   int    _month;   /* month number               (1     to 12)           */
   int    _day;     /* day of the month           (1     to 31)           */
   int    _hour;    /* hour since midnight        (0     to 23)           */
   int    _min;     /* minutes after the hour     (0     to 59)           */
   double _sec;     /* seconds after the minute   (0     to 59.999999999) */
   int    _tz;      /* TZ offset from UTC in mins (-1439 to +1439)        */

   bool parse_date (const char ** str);
   bool parse_time (const char ** str);
   bool parse_tz   (const char ** str);

public:
   ogc_datetime();

   ogc_datetime(int year, int month, int    day,
                int hour, int min,   double sec,
                int tz);

   ogc_datetime(int year, int month, int    day,
                int hour, int min,   double sec);

   ogc_datetime(int year, int month, int    day);

   ogc_datetime(int hour, int min,   double sec);

   ~ogc_datetime() {}

   void clear();

   /* set to today (at 00:00:00) */
   void today();

   /* set to now */
   void now();

   /* parse a date and/or time string */
   bool parse(const char * str);

   /* number of seconds since Jan 1, 1970 UTC */
   time_t unixtime() const;

   /* days since Jan 1 (0-366) */
   int yday() const;

   /* check if yesr is a leap year */
   bool is_leapyear() const;

   /* create timestamp string "YYYY-MM-DDThh:mm:ss.sss[Z|+-hh[:mm]]" */
   char * timestamp_str(OGC_TIME timebuf, int sec_digits = 0) const;

   /* create date string "YYYY-MM-DD" */
   char * date_str(OGC_TIME timebuf) const;

   /* create time string "hh:mm:ss.sss" */
   char * time_str(OGC_TIME timebuf, int sec_digits = 0) const;

   /* create timezone string "Z" or "+-hh[:mm]" */
   char * tz_str(OGC_TIME timebuf) const;

   int    year()  const { return _year;  }
   int    month() const { return _month; }
   int    day()   const { return _day;   }
   int    hour()  const { return _hour;  }
   int    min()   const { return _min;   }
   double sec()   const { return _sec;   }
   int    tz()    const { return _tz;    }
};

/* ------------------------------------------------------------------------- */
/* utility functions                                                         */
/* ------------------------------------------------------------------------- */

class OGC_EXPORT ogc_utils
{
private:
    ogc_utils() {}
   ~ogc_utils() {}

public:
   /* object types */

   static ogc_obj_type       obj_kwd_to_type       (const char *       kwd);
   static const char *       obj_type_to_kwd       (ogc_obj_type       type);

   /* coordinate reference system types */

   static ogc_crs_type       crs_kwd_to_type       (const char *       kwd);
   static const char *       crs_type_to_kwd       (ogc_crs_type       type);

   /* unit types */

   static ogc_unit_type      unit_kwd_to_type      (const char *       kwd);
   static const char *       unit_type_to_kwd      (ogc_unit_type      type);

   /* datum types */

   static ogc_datum_type     datum_kwd_to_type     (const char *       kwd);
   static const char *       datum_type_to_kwd     (ogc_datum_type     type);

   /* coordinate system types */

   static ogc_cs_type        cs_kwd_to_type        (const char *       kwd);
   static const char *       cs_type_to_kwd        (ogc_cs_type        type);
   static bool               cs_type_valid         (ogc_cs_type        type);

   /* axis direction types */

   static ogc_axis_direction axis_kwd_to_direction (const char *       kwd);
   static const char *       axis_direction_to_kwd (ogc_axis_direction dir);
   static bool               axis_direction_valid  (ogc_axis_direction dir);

   /* pixel-in-cell types */

   static ogc_pixel_type     pixel_kwd_to_type     (const char *       kwd);
   static const char *       pixel_type_to_kwd     (ogc_pixel_type     type);
   static bool               pixel_type_valid      (ogc_pixel_type     type);

   /* validate a cs, axes, and unit */

   static bool validate_cs(
      ogc_obj_type obj_type,
      ogc_cs *     cs,
      ogc_axis *   axis_1,
      ogc_axis *   axis_2,
      ogc_axis *   axis_3,
      ogc_unit *   unit,
      ogc_vector * parameters,
      ogc_error *  err = OGC_NULL);

   /* place an axis in the proper location */

   static bool place_axis(
      ogc_axis *   axis,
      ogc_axis **  paxis_1,
      ogc_axis **  paxis_2,
      ogc_axis **  paxis_3,
      const char * obj_kwd,
      ogc_error *  err = OGC_NULL);

   /* expand a WKT string */

   static bool expand_wkt(
      char         out_buffer[],
      const char * inp_buffer,
      const char * prefix,
      int          opts,
      size_t       buflen);

   /* compare routines for sorting/finding */

   static ogc_vector :: OGC_COMPARE_RTN compare_extent;
   static ogc_vector :: OGC_COMPARE_RTN compare_id;
   static ogc_vector :: OGC_COMPARE_RTN compare_parameter;
   static ogc_vector :: OGC_COMPARE_RTN compare_param_file;
};

/* ------------------------------------------------------------------------- */
/* math macros                                                               */
/* ------------------------------------------------------------------------- */

class OGC_EXPORT ogc_macros
{
private:
    ogc_macros() {}
   ~ogc_macros() {}

public:
   /* Get the absolute value of a number */
   static double  abs            (double a);
   static int     abs            (int    a);

   /* Assign the sign of the second number to the first */
   static double  sgn            (double a, double b);
   static double  sgn            (double a, int    b);
   static int     sgn            (int    a, int    b);

   /* Return the maximum value of two numbers */
   static double  max            (double a, double b);
   static int     max            (int    a, int    b);

   /* Return the minimum value of two numbers */
   static double  min            (double a, double b);
   static int     min            (int    a, int    b);

   /* Determine if a double is == another within a given   tolerance */
   static bool    eq_eps         (double a, double b, double e);

   /* Determine if a double is == another within a default tolerance */
   static bool    eq             (double a, double b);

   /* Determine if a double is != another within a given   tolerance */
   static bool    ne_eps         (double a, double b, double e);

   /* Determine if a double is != another within a default tolerance */
   static bool    ne             (double a, double b);

   /* Determine if a double is <= another within a given   tolerance */
   static bool    le_eps         (double a, double b, double e);

   /* Determine if a double is <= another within a default tolerance */
   static bool    le             (double a, double b);

   /* Determine if a double is >= another within a given   tolerance */
   static bool    ge_eps         (double a, double b, double e);

   /* Determine if a double is >= another within a default tolerance */
   static bool    ge             (double a, double b);

   /* Determine if a double is <  another within a given   tolerance */
   static bool    lt_eps         (double a, double b, double e);

   /* Determine if a double is <  another within a default tolerance */
   static bool    lt             (double a, double b);

   /* Determine if a double is >  another within a given   tolerance */
   static bool    gt_eps         (double a, double b, double e);

   /* Determine if a double is >  another within a default tolerance */
   static bool    gt             (double a, double b);

   /* Determine if a double is == 0       within a given   tolerance */
   static bool    zero_eps       (double d, double e);

   /* Determine if a double is == 0       within a default tolerance */
   static bool    zero           (double d);

   /* Determine if an e-squared represents a sphere */
   static bool    is_e2_sphere   (double e2);

   /* Convert degrees to radians (assumes -360 to 360) */
   static double  deg_to_rad     (double d);

   /* Convert radians to degrees (assumes -2PI to 2PI) */
   static double  rad_to_deg     (double r);

   /* Convert deg-min-sec to decimal degrees */
   static double  dms_to_dd      (double d, double m, double s);
};

/* ------------------------------------------------------------------------- */
/* unicode functions                                                         */
/* ------------------------------------------------------------------------- */

class OGC_EXPORT ogc_unicode
{
private:
    ogc_unicode() {}
   ~ogc_unicode() {}

public:
   static int unicode_to_utf8(
      char              target[],
      const OGC_UCHAR * source,
      int               n);

   static int utf8_to_unicode(
      OGC_UCHAR         target[],
      const char *      source,
      int               n);

   static int strcpy_u(
      OGC_UCHAR         target[],
      const OGC_UCHAR * source);

   static int strncpy_u(
      OGC_UCHAR         target[],
      const OGC_UCHAR * source,
      int               n);

   static int strlen_u(
      const OGC_UCHAR * str);

   static int unicode_to_utf8_len(
      const OGC_UCHAR * str);

   static int utf8_to_unicode_len(
      const char *      str);
};

/* ------------------------------------------------------------------------- */

} /* namespace OGC */

#endif /* LIBOGC_H_INCLUDED */

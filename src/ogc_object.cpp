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
/* generic (top-level) object                                                */
/* ------------------------------------------------------------------------- */

#include "ogc_common.h"

namespace OGC {

bool         ogc_object :: _strict_parsing = true;

const char * ogc_object :: obj_kwd() { return OGC_OBJ_KWD_OBJECT; }

/*------------------------------------------------------------------------
 * strict parsing
 */
bool ogc_object :: get_strict_parsing()
{
   return _strict_parsing;
}

void ogc_object :: set_strict_parsing(bool on_off)
{
   _strict_parsing = on_off;
}

/*------------------------------------------------------------------------
 * destroy
 */
ogc_object :: ~ogc_object()
{
}

void ogc_object :: destroy(
   ogc_object * obj)
{
   if ( obj != OGC_NULL )
   {
      delete obj;
   }
}

/*------------------------------------------------------------------------
 * object from tokens
 */
ogc_object * ogc_object :: from_tokens(
   const ogc_token * t,
   int               start,
   int *             pend,
   ogc_error *       err)
{
   if ( t == OGC_NULL )
   {
      ogc_error::set(err, OGC_ERR_WKT_EMPTY_STRING, obj_kwd());
      return OGC_NULL;
   }

   const char * kwd = t->_arr[start].str;

#  define CHECK(o,n) \
   if ( ogc_string::is_equal(kwd, ogc_##n :: obj_kwd()) ) \
      return ogc_##n :: from_tokens(t, start, pend, err)

   CHECK( ABRTRANS,        abrtrans        );
   CHECK( ANCHOR,          anchor          );
   CHECK( ANGUNIT,         angunit         );
   CHECK( AREA_EXTENT,     area_extent     );
   CHECK( AXIS,            axis            );
   CHECK( BBOX_EXTENT,     bbox_extent     );
   CHECK( BEARING,         bearing         );
   CHECK( BOUND_CRS,       bound_crs       );
   CHECK( CITATION,        citation        );
   CHECK( COMPOUND_CRS,    compound_crs    );
   CHECK( CONVERSION,      conversion      );
   CHECK( COORDOP,         coordop         );
   CHECK( CS,              cs              );
   CHECK( ELLIPSOID,       ellipsoid       );
   CHECK( ENGINEERING_CRS, engineering_crs );
   CHECK( GENERIC_DATUM,   generic_datum   );
   CHECK( GEOCENTRIC_CRS,  geocentric_crs  );
   CHECK( GEODETIC_DATUM,  geodetic_datum  );
   CHECK( GEOG2D_CRS,      geog2d_crs      );
   CHECK( GEOG3D_CRS,      geog3d_crs      );
   CHECK( ID,              id              );
   CHECK( IMAGE_CRS,       image_crs       );
   CHECK( LENUNIT,         lenunit         );
   CHECK( MERIDIAN,        meridian        );
   CHECK( METHOD,          method          );
   CHECK( OPACCURACY,      opaccuracy      );
   CHECK( ORDER,           order           );
   CHECK( PARAMETER,       parameter       );
   CHECK( PARAMETER_FILE,  parameter_file  );
   CHECK( PARAMETRIC_CRS,  parametric_crs  );
   CHECK( PARAMUNIT,       paramunit       );
   CHECK( PRIMEM,          primem          );
   CHECK( PROJECTED_CRS,   projected_crs   );
   CHECK( REMARK,          remark          );
   CHECK( SCALEUNIT,       scaleunit       );
   CHECK( SCOPE,           scope           );
   CHECK( TEMPORAL_CRS,    temporal_crs    );
   CHECK( TIME_EXTENT,     time_extent     );
   CHECK( TIMEUNIT,        timeunit        );
   CHECK( UNIT,            unit            );
   CHECK( URI,             uri             );
   CHECK( VERT_EXTENT,     vert_extent     );
   CHECK( VERTICAL_CRS,    vertical_crs    );

   /* alternate object keywords */

   CHECK( SPHEROID,        ellipsoid       );
   CHECK( AUTHORITY,       id              );
   CHECK( PROJECTION,      method          );

   /* legacy object keywords */

   CHECK( GEOGCS,          geog2d_crs      );
   CHECK( PROJCS,          projected_crs   );

#  undef CHECK

   ogc_error::set(err, OGC_ERR_WKT_UNKNOWN_KEYWORD, obj_kwd(), kwd);
   return OGC_NULL;
}

/*------------------------------------------------------------------------
 * object from WKT
 */
ogc_object* ogc_object :: from_wkt(
   const char * wkt,
   ogc_error *  err)
{
   ogc_token t;

   if ( ! t.tokenize(wkt, obj_kwd(), err) )
   {
      return OGC_NULL;
   }

   return from_tokens(&t, 0, OGC_NULL, err);
}

/*------------------------------------------------------------------------
 * object to WKT
 */
bool ogc_object :: to_wkt(
   const ogc_object * obj,
   char      buffer[],
   int       options,
   size_t    buflen)
{
   if ( buffer == OGC_NULL )
      return false;
   *buffer = 0;

   if ( obj == OGC_NULL )
      return true;

   return obj->to_wkt(buffer, options, buflen);
}

bool ogc_object :: to_wkt(
   char      buffer[],
   int       options,
   size_t    buflen) const
{
   if ( buffer == OGC_NULL )
      return false;
   *buffer = 0;

   switch (_obj_type)
   {
#  define CASE(o,n) \
      case OGC_OBJ_TYPE_##o: \
         return (reinterpret_cast<const ogc_##n *>(this))-> \
                to_wkt(buffer, options, buflen)

   CASE( ABRTRANS,        abrtrans        );
   CASE( ANCHOR,          anchor          );
   CASE( ANGUNIT,         angunit         );
   CASE( AREA_EXTENT,     area_extent     );
   CASE( AXIS,            axis            );
   CASE( BBOX_EXTENT,     bbox_extent     );
   CASE( BEARING,         bearing         );
   CASE( BOUND_CRS,       bound_crs       );
   CASE( CITATION,        citation        );
   CASE( COMPOUND_CRS,    compound_crs    );
   CASE( CONVERSION,      conversion      );
   CASE( COORDOP,         coordop         );
   CASE( CS,              cs              );
   CASE( ELLIPSOID,       ellipsoid       );
   CASE( ENGINEERING_CRS, engineering_crs );
   CASE( GENERIC_DATUM,   generic_datum   );
   CASE( GEOCENTRIC_CRS,  geocentric_crs  );
   CASE( GEODETIC_DATUM,  geodetic_datum  );
   CASE( GEOG2D_CRS,      geog2d_crs      );
   CASE( GEOG3D_CRS,      geog3d_crs      );
   CASE( ID,              id              );
   CASE( IMAGE_CRS,       image_crs       );
   CASE( LENUNIT,         lenunit         );
   CASE( MERIDIAN,        meridian        );
   CASE( METHOD,          method          );
   CASE( OPACCURACY,      opaccuracy      );
   CASE( ORDER,           order           );
   CASE( PARAMETER,       parameter       );
   CASE( PARAMETER_FILE,  parameter_file  );
   CASE( PARAMETRIC_CRS,  parametric_crs  );
   CASE( PARAMUNIT,       paramunit       );
   CASE( PRIMEM,          primem          );
   CASE( PROJECTED_CRS,   projected_crs   );
   CASE( REMARK,          remark          );
   CASE( SCALEUNIT,       scaleunit       );
   CASE( SCOPE,           scope           );
   CASE( TEMPORAL_CRS,    temporal_crs    );
   CASE( TIME_EXTENT,     time_extent     );
   CASE( TIMEUNIT,        timeunit        );
   CASE( UNIT,            unit            );
   CASE( URI,             uri             );
   CASE( VERT_EXTENT,     vert_extent     );
   CASE( VERTICAL_CRS,    vertical_crs    );

#  undef CASE

   default:
      break;
   }

   return false;
}

/*------------------------------------------------------------------------
 * clone
 */
ogc_object * ogc_object :: clone(const ogc_object * obj)
{
   if ( obj == OGC_NULL )
      return OGC_NULL;
   return obj->clone();
}

ogc_object * ogc_object :: clone() const
{
   switch (_obj_type)
   {
#  define CASE(o,n) \
      case OGC_OBJ_TYPE_##o: \
         return (reinterpret_cast<const ogc_##n *>(this))->clone()

   CASE( ABRTRANS,        abrtrans        );
   CASE( ANCHOR,          anchor          );
   CASE( ANGUNIT,         angunit         );
   CASE( AREA_EXTENT,     area_extent     );
   CASE( AXIS,            axis            );
   CASE( BBOX_EXTENT,     bbox_extent     );
   CASE( BEARING,         bearing         );
   CASE( BOUND_CRS,       bound_crs       );
   CASE( CITATION,        citation        );
   CASE( COMPOUND_CRS,    compound_crs    );
   CASE( CONVERSION,      conversion      );
   CASE( COORDOP,         coordop         );
   CASE( CS,              cs              );
   CASE( ELLIPSOID,       ellipsoid       );
   CASE( ENGINEERING_CRS, engineering_crs );
   CASE( GENERIC_DATUM,   generic_datum   );
   CASE( GEOCENTRIC_CRS,  geocentric_crs  );
   CASE( GEODETIC_DATUM,  geodetic_datum  );
   CASE( GEOG2D_CRS,      geog2d_crs      );
   CASE( GEOG3D_CRS,      geog3d_crs      );
   CASE( ID,              id              );
   CASE( IMAGE_CRS,       image_crs       );
   CASE( LENUNIT,         lenunit         );
   CASE( MERIDIAN,        meridian        );
   CASE( METHOD,          method          );
   CASE( OPACCURACY,      opaccuracy      );
   CASE( ORDER,           order           );
   CASE( PARAMETER,       parameter       );
   CASE( PARAMETER_FILE,  parameter_file  );
   CASE( PARAMETRIC_CRS,  parametric_crs  );
   CASE( PARAMUNIT,       paramunit       );
   CASE( PRIMEM,          primem          );
   CASE( PROJECTED_CRS,   projected_crs   );
   CASE( REMARK,          remark          );
   CASE( SCALEUNIT,       scaleunit       );
   CASE( SCOPE,           scope           );
   CASE( TEMPORAL_CRS,    temporal_crs    );
   CASE( TIME_EXTENT,     time_extent     );
   CASE( TIMEUNIT,        timeunit        );
   CASE( UNIT,            unit            );
   CASE( URI,             uri             );
   CASE( VERT_EXTENT,     vert_extent     );
   CASE( VERTICAL_CRS,    vertical_crs    );

#  undef CASE

   default:
      break;
   }

   return OGC_NULL;
}

/*------------------------------------------------------------------------
 * compare for computational equality
 */
bool ogc_object :: is_equal(
   const ogc_object * p1,
   const ogc_object * p2)
{
   if ( p1 == OGC_NULL && p2 == OGC_NULL ) return true;
   if ( p1 == OGC_NULL || p2 == OGC_NULL ) return false;
   if ( p1->obj_type() != p2->obj_type() ) return false;

   switch ( p1->obj_type() )
   {
#  define CASE(o,n) \
      case OGC_OBJ_TYPE_##o: \
         return ogc_##n::is_equal(reinterpret_cast<const ogc_##n *>(p1), \
                                  reinterpret_cast<const ogc_##n *>(p2))

   CASE( ABRTRANS,        abrtrans        );
   CASE( ANCHOR,          anchor          );
   CASE( ANGUNIT,         angunit         );
   CASE( AREA_EXTENT,     area_extent     );
   CASE( AXIS,            axis            );
   CASE( BBOX_EXTENT,     bbox_extent     );
   CASE( BEARING,         bearing         );
   CASE( BOUND_CRS,       bound_crs       );
   CASE( CITATION,        citation        );
   CASE( COMPOUND_CRS,    compound_crs    );
   CASE( CONVERSION,      conversion      );
   CASE( COORDOP,         coordop         );
   CASE( CS,              cs              );
   CASE( ELLIPSOID,       ellipsoid       );
   CASE( ENGINEERING_CRS, engineering_crs );
   CASE( GENERIC_DATUM,   generic_datum   );
   CASE( GEOCENTRIC_CRS,  geocentric_crs  );
   CASE( GEODETIC_DATUM,  geodetic_datum  );
   CASE( GEOG2D_CRS,      geog2d_crs      );
   CASE( GEOG3D_CRS,      geog3d_crs      );
   CASE( ID,              id              );
   CASE( IMAGE_CRS,       image_crs       );
   CASE( LENUNIT,         lenunit         );
   CASE( MERIDIAN,        meridian        );
   CASE( METHOD,          method          );
   CASE( OPACCURACY,      opaccuracy      );
   CASE( ORDER,           order           );
   CASE( PARAMETER,       parameter       );
   CASE( PARAMETER_FILE,  parameter_file  );
   CASE( PARAMETRIC_CRS,  parametric_crs  );
   CASE( PARAMUNIT,       paramunit       );
   CASE( PRIMEM,          primem          );
   CASE( PROJECTED_CRS,   projected_crs   );
   CASE( REMARK,          remark          );
   CASE( SCALEUNIT,       scaleunit       );
   CASE( SCOPE,           scope           );
   CASE( TEMPORAL_CRS,    temporal_crs    );
   CASE( TIME_EXTENT,     time_extent     );
   CASE( TIMEUNIT,        timeunit        );
   CASE( UNIT,            unit            );
   CASE( URI,             uri             );
   CASE( VERT_EXTENT,     vert_extent     );
   CASE( VERTICAL_CRS,    vertical_crs    );

#  undef CASE

   default:
      break;
   }

   return false;
}

bool ogc_object :: is_equal(
   const ogc_object * p) const
{
   return is_equal(this, p);
}

/*------------------------------------------------------------------------
 * compare
 */
bool ogc_object :: is_identical(
   const ogc_object * p1,
   const ogc_object * p2)
{
   if ( p1 == OGC_NULL && p2 == OGC_NULL ) return true;
   if ( p1 == OGC_NULL || p2 == OGC_NULL ) return false;
   if ( p1->obj_type() != p2->obj_type() ) return false;

   switch ( p1->obj_type() )
   {
#  define CASE(o,n) \
      case OGC_OBJ_TYPE_##o: \
         return ogc_##n::is_identical(reinterpret_cast<const ogc_##n *>(p1), \
                                      reinterpret_cast<const ogc_##n *>(p2))

   CASE( ABRTRANS,        abrtrans        );
   CASE( ANCHOR,          anchor          );
   CASE( ANGUNIT,         angunit         );
   CASE( AREA_EXTENT,     area_extent     );
   CASE( AXIS,            axis            );
   CASE( BBOX_EXTENT,     bbox_extent     );
   CASE( BEARING,         bearing         );
   CASE( BOUND_CRS,       bound_crs       );
   CASE( CITATION,        citation        );
   CASE( COMPOUND_CRS,    compound_crs    );
   CASE( CONVERSION,      conversion      );
   CASE( COORDOP,         coordop         );
   CASE( CS,              cs              );
   CASE( ELLIPSOID,       ellipsoid       );
   CASE( ENGINEERING_CRS, engineering_crs );
   CASE( GENERIC_DATUM,   generic_datum   );
   CASE( GEOCENTRIC_CRS,  geocentric_crs  );
   CASE( GEODETIC_DATUM,  geodetic_datum  );
   CASE( GEOG2D_CRS,      geog2d_crs      );
   CASE( GEOG3D_CRS,      geog3d_crs      );
   CASE( ID,              id              );
   CASE( IMAGE_CRS,       image_crs       );
   CASE( LENUNIT,         lenunit         );
   CASE( MERIDIAN,        meridian        );
   CASE( METHOD,          method          );
   CASE( OPACCURACY,      opaccuracy      );
   CASE( ORDER,           order           );
   CASE( PARAMETER,       parameter       );
   CASE( PARAMETER_FILE,  parameter_file  );
   CASE( PARAMETRIC_CRS,  parametric_crs  );
   CASE( PARAMUNIT,       paramunit       );
   CASE( PRIMEM,          primem          );
   CASE( PROJECTED_CRS,   projected_crs   );
   CASE( REMARK,          remark          );
   CASE( SCALEUNIT,       scaleunit       );
   CASE( SCOPE,           scope           );
   CASE( TEMPORAL_CRS,    temporal_crs    );
   CASE( TIME_EXTENT,     time_extent     );
   CASE( TIMEUNIT,        timeunit        );
   CASE( UNIT,            unit            );
   CASE( URI,             uri             );
   CASE( VERT_EXTENT,     vert_extent     );
   CASE( VERTICAL_CRS,    vertical_crs    );

#  undef CASE

   default:
      break;
   }

   return false;
}

bool ogc_object :: is_identical(
   const ogc_object * p) const
{
   return is_identical(this, p);
}

} /* namespace OGC */

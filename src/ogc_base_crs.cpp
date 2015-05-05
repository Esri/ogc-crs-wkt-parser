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
/* generic CRS object                                                        */
/* ------------------------------------------------------------------------- */

#include "ogc_common.h"

namespace OGC {

const char * ogc_base_crs :: obj_kwd() { return OGC_OBJ_KWD_BASE_CRS; }

bool ogc_base_crs :: is_kwd(const char * kwd)
{
   return ogc_string::is_equal(kwd, obj_kwd());
}

/*------------------------------------------------------------------------
 * destroy
 */
ogc_base_crs :: ~ogc_base_crs()
{
   ogc_unit :: destroy( _unit );
}

void ogc_base_crs :: destroy(
   ogc_base_crs * obj)
{
   if ( obj != OGC_NULL )
   {
      delete obj;
   }
}

/*------------------------------------------------------------------------
 * object from tokens
 */
ogc_base_crs * ogc_base_crs :: from_tokens(
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
   if ( ogc_##n::is_kwd(kwd) ) \
      return ogc_##n :: from_tokens(t, start, pend, err)

   CHECK( BASE_ENGR_CRS,  base_engr_crs  );
   CHECK( BASE_GEOD_CRS,  base_geod_crs  );
   CHECK( BASE_PARAM_CRS, base_param_crs );
   CHECK( BASE_PROJ_CRS,  base_proj_crs  );
   CHECK( BASE_TIME_CRS,  base_time_crs  );
   CHECK( BASE_VERT_CRS,  base_vert_crs  );

#  undef CHECK

   ogc_error::set(err, OGC_ERR_WKT_INVALID_KEYWORD, obj_kwd(), kwd);
   return OGC_NULL;
}

/*------------------------------------------------------------------------
 * object from WKT
 */
ogc_base_crs * ogc_base_crs :: from_wkt(
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
bool ogc_base_crs :: to_wkt(
   const ogc_base_crs * obj,
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

bool ogc_base_crs :: to_wkt(
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

   CASE( BASE_ENGR_CRS,  base_engr_crs  );
   CASE( BASE_GEOD_CRS,  base_geod_crs  );
   CASE( BASE_PARAM_CRS, base_param_crs );
   CASE( BASE_PROJ_CRS,  base_proj_crs  );
   CASE( BASE_TIME_CRS,  base_time_crs  );
   CASE( BASE_VERT_CRS,  base_vert_crs  );

#  undef CASE

   default:
      break;
   }

   return false;
}

/*------------------------------------------------------------------------
 * clone
 */
ogc_base_crs * ogc_base_crs :: clone(const ogc_base_crs * obj)
{
   if ( obj == OGC_NULL )
      return OGC_NULL;
   return obj->clone();
}

ogc_base_crs * ogc_base_crs :: clone() const
{
   switch (_obj_type)
   {
#  define CASE(o,n) \
      case OGC_OBJ_TYPE_##o: \
         return (reinterpret_cast<const ogc_##n *>(this))->clone()

   CASE( BASE_ENGR_CRS,  base_engr_crs  );
   CASE( BASE_GEOD_CRS,  base_geod_crs  );
   CASE( BASE_PARAM_CRS, base_param_crs );
   CASE( BASE_PROJ_CRS,  base_proj_crs  );
   CASE( BASE_TIME_CRS,  base_time_crs  );
   CASE( BASE_VERT_CRS,  base_vert_crs  );

#  undef CASE

   default:
      break;
   }

   return OGC_NULL;
}

/*------------------------------------------------------------------------
 * compare for computational equality
 */
bool ogc_base_crs :: is_equal(
   const ogc_base_crs * p1,
   const ogc_base_crs * p2)
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

   CASE( BASE_ENGR_CRS,  base_engr_crs  );
   CASE( BASE_GEOD_CRS,  base_geod_crs  );
   CASE( BASE_PARAM_CRS, base_param_crs );
   CASE( BASE_PROJ_CRS,  base_proj_crs  );
   CASE( BASE_TIME_CRS,  base_time_crs  );
   CASE( BASE_VERT_CRS,  base_vert_crs  );

#  undef CASE

   default:
      break;
   }

   return false;
}

bool ogc_base_crs :: is_equal(
   const ogc_base_crs * p) const
{
   return is_equal(this, p);
}

/*------------------------------------------------------------------------
 * compare
 */
bool ogc_base_crs :: is_identical(
   const ogc_base_crs * p1,
   const ogc_base_crs * p2)
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

   CASE( BASE_ENGR_CRS,  base_engr_crs  );
   CASE( BASE_GEOD_CRS,  base_geod_crs  );
   CASE( BASE_PARAM_CRS, base_param_crs );
   CASE( BASE_PROJ_CRS,  base_proj_crs  );
   CASE( BASE_TIME_CRS,  base_time_crs  );
   CASE( BASE_VERT_CRS,  base_vert_crs  );

#  undef CASE

   default:
      break;
   }

   return false;
}

bool ogc_base_crs :: is_identical(
   const ogc_base_crs * p) const
{
   return is_equal(this, p);
}

} /* namespace OGC */

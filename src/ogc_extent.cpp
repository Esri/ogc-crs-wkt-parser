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
/* generic EXTENT object                                                     */
/* ------------------------------------------------------------------------- */

#include "ogc_common.h"

namespace OGC {

const char * ogc_extent :: obj_kwd() { return OGC_OBJ_KWD_EXTENT; }

bool ogc_extent :: is_kwd(const char * kwd)
{
   return ogc_area_extent::is_kwd(kwd) ||
          ogc_bbox_extent::is_kwd(kwd) ||
          ogc_time_extent::is_kwd(kwd) ||
          ogc_vert_extent::is_kwd(kwd) ;
}

/*------------------------------------------------------------------------
 * destroy
 */
ogc_extent :: ~ogc_extent()
{
}

ogc_extent * ogc_extent :: destroy(
   ogc_extent * obj)
{
   if ( obj != OGC_NULL )
   {
      delete obj;
   }
   return OGC_NULL;
}

/*------------------------------------------------------------------------
 * object from tokens
 */
ogc_extent * ogc_extent :: from_tokens(
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

#  define CHECK(n) \
   if ( ogc_##n::is_kwd(kwd) ) \
      return ogc_##n :: from_tokens(t, start, pend, err)

   CHECK( area_extent );
   CHECK( bbox_extent );
   CHECK( time_extent );
   CHECK( vert_extent );

#  undef CHECK

   ogc_error::set(err, OGC_ERR_WKT_INVALID_KEYWORD, obj_kwd(), kwd);
   return OGC_NULL;
}

/*------------------------------------------------------------------------
 * object from WKT
 */
ogc_extent * ogc_extent :: from_wkt(
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
bool ogc_extent :: to_wkt(
   const ogc_extent * obj,
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

bool ogc_extent :: to_wkt(
   char      buffer[],
   int       options,
   size_t    buflen) const
{
   if ( buffer == OGC_NULL )
      return false;
   *buffer = 0;

   if ( !is_visible() )
      return true;

   switch (_obj_type)
   {
#  define CASE(o,n) \
      case OGC_OBJ_TYPE_##o: \
         return (reinterpret_cast<const ogc_##n *>(this))-> \
                to_wkt(buffer, options, buflen)

   CASE( AREA_EXTENT, area_extent );
   CASE( BBOX_EXTENT, bbox_extent );
   CASE( TIME_EXTENT, time_extent );
   CASE( VERT_EXTENT, vert_extent );

#  undef CASE

   default:
      break;
   }

   return false;
}

/*------------------------------------------------------------------------
 * clone
 */
ogc_extent * ogc_extent :: clone(const ogc_extent * obj)
{
   if ( obj == OGC_NULL )
      return OGC_NULL;
   return obj->clone();
}

ogc_extent * ogc_extent :: clone() const
{
   switch (_obj_type)
   {
#  define CASE(o,n) \
      case OGC_OBJ_TYPE_##o: \
         return (reinterpret_cast<const ogc_##n *>(this))->clone()

   CASE( AREA_EXTENT, area_extent );
   CASE( BBOX_EXTENT, bbox_extent );
   CASE( TIME_EXTENT, time_extent );
   CASE( VERT_EXTENT, vert_extent );

#  undef CASE

   default:
      break;
   }

   return OGC_NULL;
}

/*------------------------------------------------------------------------
 * compare for computational equality
 */
bool ogc_extent :: is_equal(
   const ogc_extent * p1,
   const ogc_extent * p2)
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

   CASE( AREA_EXTENT, area_extent );
   CASE( BBOX_EXTENT, bbox_extent );
   CASE( TIME_EXTENT, time_extent );
   CASE( VERT_EXTENT, vert_extent );

#  undef CASE

   default:
      break;
   }

   return false;
}

bool ogc_extent :: is_equal(
   const ogc_extent * p) const
{
   return is_equal(this, p);
}

/*------------------------------------------------------------------------
 * compare
 */
bool ogc_extent :: is_identical(
   const ogc_extent * p1,
   const ogc_extent * p2)
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

   CASE( AREA_EXTENT, area_extent );
   CASE( BBOX_EXTENT, bbox_extent );
   CASE( TIME_EXTENT, time_extent );
   CASE( VERT_EXTENT, vert_extent );

#  undef CASE

   default:
      break;
   }

   return false;
}

bool ogc_extent :: is_identical(
   const ogc_extent * p) const
{
   return is_identical(this, p);
}

} /* namespace OGC */

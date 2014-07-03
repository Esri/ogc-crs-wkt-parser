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
/* generic DATUM object                                                      */
/* ------------------------------------------------------------------------- */

#include "ogc_common.h"

namespace OGC {

/*------------------------------------------------------------------------
 * destroy
 */
ogc_datum :: ~ogc_datum()
{
   ogc_anchor :: destroy( _anchor );
   ogc_vector :: destroy( _ids    );
}

void ogc_datum :: destroy(
   ogc_datum * obj)
{
   if ( obj != OGC_NULL )
   {
      delete obj;
   }
}

/*------------------------------------------------------------------------
 * object from tokens
 */
ogc_datum * ogc_datum :: from_tokens(
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

   CHECK( ENGR_DATUM,     engr_datum     );
   CHECK( GEOD_DATUM,     geod_datum     );
   CHECK( IMAGE_DATUM,    image_datum    );
   CHECK( PARAM_DATUM,    param_datum    );
   CHECK( TIME_DATUM,     time_datum     );
   CHECK( VERT_DATUM,     vert_datum     );

#  undef CHECK

   ogc_error::set(err, OGC_ERR_WKT_INVALID_KEYWORD, obj_kwd(), kwd);
   return OGC_NULL;
}

/*------------------------------------------------------------------------
 * object from WKT
 */
ogc_datum * ogc_datum :: from_wkt(
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
bool ogc_datum :: to_wkt(
   const ogc_datum * obj,
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

bool ogc_datum :: to_wkt(
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

   CASE( ENGR_DATUM,     engr_datum     );
   CASE( GEOD_DATUM,     geod_datum     );
   CASE( IMAGE_DATUM,    image_datum    );
   CASE( PARAM_DATUM,    param_datum    );
   CASE( TIME_DATUM,     time_datum     );
   CASE( VERT_DATUM,     vert_datum     );

#  undef CASE

   default:
      break;
   }

   return false;
}

/*------------------------------------------------------------------------
 * clone
 */
ogc_datum * ogc_datum :: clone(const ogc_datum * obj)
{
   if ( obj == OGC_NULL )
      return OGC_NULL;
   return obj->clone();
}

ogc_datum * ogc_datum :: clone() const
{
   switch (_obj_type)
   {
#  define CASE(o,n) \
      case OGC_OBJ_TYPE_##o: \
         return (reinterpret_cast<const ogc_##n *>(this))->clone()

   CASE( ENGR_DATUM,     engr_datum     );
   CASE( GEOD_DATUM,     geod_datum     );
   CASE( IMAGE_DATUM,    image_datum    );
   CASE( PARAM_DATUM,    param_datum    );
   CASE( TIME_DATUM,     time_datum     );
   CASE( VERT_DATUM,     vert_datum     );

#  undef CASE

   default:
      break;
   }

   return OGC_NULL;
}

/*------------------------------------------------------------------------
 * compare for computational equality
 */
bool ogc_datum :: is_equal(
   const ogc_datum * p1,
   const ogc_datum * p2)
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

   CASE( ENGR_DATUM,     engr_datum     );
   CASE( GEOD_DATUM,     geod_datum     );
   CASE( IMAGE_DATUM,    image_datum    );
   CASE( PARAM_DATUM,    param_datum    );
   CASE( TIME_DATUM,     time_datum     );
   CASE( VERT_DATUM,     vert_datum     );

#  undef CASE

   default:
      break;
   }

   return false;
}

bool ogc_datum :: is_equal(
   const ogc_datum * p) const
{
   return is_equal(this, p);
}

/*------------------------------------------------------------------------
 * compare
 */
bool ogc_datum :: is_identical(
   const ogc_datum * p1,
   const ogc_datum * p2)
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

   CASE( ENGR_DATUM,     engr_datum     );
   CASE( GEOD_DATUM,     geod_datum     );
   CASE( IMAGE_DATUM,    image_datum    );
   CASE( PARAM_DATUM,    param_datum    );
   CASE( TIME_DATUM,     time_datum     );
   CASE( VERT_DATUM,     vert_datum     );

#  undef CASE

   default:
      break;
   }

   return false;
}

bool ogc_datum :: is_identical(
   const ogc_datum * p) const
{
   return is_identical(this, p);
}

/*------------------------------------------------------------------------
 * get ID count
 */
int ogc_datum :: id_count() const
{
   return (_ids == OGC_NULL) ? 0 : _ids->length();
}

/*------------------------------------------------------------------------
 * get the nth ID
 */
ogc_id * ogc_datum :: id(int n) const
{
   return (_ids == OGC_NULL) ? OGC_NULL :
                               reinterpret_cast<ogc_id *>( _ids->get(n) );
}

} /* namespace OGC */

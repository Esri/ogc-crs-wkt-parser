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

const char * ogc_crs :: obj_kwd() { return OGC_OBJ_KWD_CRS; }

bool ogc_crs :: is_kwd(const char * kwd)
{
   return ogc_string::is_equal(kwd, obj_kwd());
}

/*------------------------------------------------------------------------
 * destroy
 */
ogc_crs :: ~ogc_crs()
{
   ogc_cs     :: destroy( _cs      );
   ogc_axis   :: destroy( _axis_1  );
   ogc_axis   :: destroy( _axis_2  );
   ogc_axis   :: destroy( _axis_3  );
   ogc_scope  :: destroy( _scope   );
   ogc_vector :: destroy( _extents );
   ogc_vector :: destroy( _ids     );
   ogc_remark :: destroy( _remark  );
}

void ogc_crs :: destroy(
   ogc_crs * obj)
{
   if ( obj != OGC_NULL )
   {
      delete obj;
   }
}

/*------------------------------------------------------------------------
 * object from tokens
 */
ogc_crs * ogc_crs :: from_tokens(
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

   CHECK( engr_crs     );
   CHECK( geod_crs     );
   CHECK( image_crs    );
   CHECK( param_crs    );
   CHECK( proj_crs     );
   CHECK( time_crs     );
   CHECK( vert_crs     );
   CHECK( compound_crs );

#  undef CHECK

   ogc_error::set(err, OGC_ERR_WKT_INVALID_KEYWORD, obj_kwd(), kwd);
   return OGC_NULL;
}

/*------------------------------------------------------------------------
 * object from WKT
 */
ogc_crs * ogc_crs :: from_wkt(
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
bool ogc_crs :: to_wkt(
   const ogc_crs * obj,
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

bool ogc_crs :: to_wkt(
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

   CASE( ENGR_CRS,     engr_crs     );
   CASE( GEOD_CRS,     geod_crs     );
   CASE( IMAGE_CRS,    image_crs    );
   CASE( PARAM_CRS,    param_crs    );
   CASE( PROJ_CRS,     proj_crs     );
   CASE( TIME_CRS,     time_crs     );
   CASE( VERT_CRS,     vert_crs     );
   CASE( COMPOUND_CRS, compound_crs );

#  undef CASE

   default:
      break;
   }

   return false;
}

/*------------------------------------------------------------------------
 * clone
 */
ogc_crs * ogc_crs :: clone(const ogc_crs * obj)
{
   if ( obj == OGC_NULL )
      return OGC_NULL;
   return obj->clone();
}

ogc_crs * ogc_crs :: clone() const
{
   switch (_obj_type)
   {
#  define CASE(o,n) \
      case OGC_OBJ_TYPE_##o: \
         return (reinterpret_cast<const ogc_##n *>(this))->clone()

   CASE( ENGR_CRS,     engr_crs     );
   CASE( GEOD_CRS,     geod_crs     );
   CASE( IMAGE_CRS,    image_crs    );
   CASE( PARAM_CRS,    param_crs    );
   CASE( PROJ_CRS,     proj_crs     );
   CASE( TIME_CRS,     time_crs     );
   CASE( VERT_CRS,     vert_crs     );
   CASE( COMPOUND_CRS, compound_crs );

#  undef CASE

   default:
      break;
   }

   return OGC_NULL;
}

/*------------------------------------------------------------------------
 * compare for computational equality
 */
bool ogc_crs :: is_equal(
   const ogc_crs * p1,
   const ogc_crs * p2)
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

   CASE( ENGR_CRS,     engr_crs     );
   CASE( GEOD_CRS,     geod_crs     );
   CASE( IMAGE_CRS,    image_crs    );
   CASE( PARAM_CRS,    param_crs    );
   CASE( PROJ_CRS,     proj_crs     );
   CASE( TIME_CRS,     time_crs     );
   CASE( VERT_CRS,     vert_crs     );
   CASE( COMPOUND_CRS, compound_crs );

#  undef CASE

   default:
      break;
   }

   return false;
}

bool ogc_crs :: is_equal(
   const ogc_crs * p) const
{
   return is_equal(this, p);
}

/*------------------------------------------------------------------------
 * compare
 */
bool ogc_crs :: is_identical(
   const ogc_crs * p1,
   const ogc_crs * p2)
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

   CASE( ENGR_CRS,     engr_crs     );
   CASE( GEOD_CRS,     geod_crs     );
   CASE( IMAGE_CRS,    image_crs    );
   CASE( PARAM_CRS,    param_crs    );
   CASE( PROJ_CRS,     proj_crs     );
   CASE( TIME_CRS,     time_crs     );
   CASE( VERT_CRS,     vert_crs     );
   CASE( COMPOUND_CRS, compound_crs );

#  undef CASE

   default:
      break;
   }

   return false;
}

bool ogc_crs :: is_identical(
   const ogc_crs * p) const
{
   return is_equal(this, p);
}

/*------------------------------------------------------------------------
 * get the nth axis
 */
ogc_axis * ogc_crs :: axis(int n) const
{
   switch (n)
   {
      case 1: return _axis_1;
      case 2: return _axis_2;
      case 3: return _axis_3;
   }

   return OGC_NULL;
}

/*------------------------------------------------------------------------
 * get extent count
 */
int ogc_crs :: extent_count() const
{
   return (_extents == OGC_NULL) ? 0 : _extents->length();
}

/*------------------------------------------------------------------------
 * get the nth extent
 */
ogc_extent * ogc_crs :: extent(int n) const
{
   return (_extents == OGC_NULL) ? OGC_NULL :
                           reinterpret_cast<ogc_extent *>( _extents->get(n) );
}

/*------------------------------------------------------------------------
 * get ID count
 */
int ogc_crs :: id_count() const
{
   return (_ids == OGC_NULL) ? 0 : _ids->length();
}

/*------------------------------------------------------------------------
 * get the nth ID
 */
ogc_id * ogc_crs :: id(int n) const
{
   return (_ids == OGC_NULL) ? OGC_NULL :
                               reinterpret_cast<ogc_id *>( _ids->get(n) );
}

} /* namespace OGC */

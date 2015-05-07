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
/* ELLIPSOID object                                                          */
/* ------------------------------------------------------------------------- */

#include "ogc_common.h"

namespace OGC {

const char * ogc_ellipsoid :: obj_kwd() { return OGC_OBJ_KWD_ELLIPSOID; }
const char * ogc_ellipsoid :: alt_kwd() { return OGC_ALT_KWD_ELLIPSOID; }

bool ogc_ellipsoid :: is_kwd(const char * kwd)
{
   return ogc_string::is_equal(kwd, obj_kwd()) ||
          ogc_string::is_equal(kwd, alt_kwd());
}

/*------------------------------------------------------------------------
 * create
 */
ogc_ellipsoid * ogc_ellipsoid :: create(
   const char *  name,
   double        semi_major_axis,
   double        flattening,
   ogc_lenunit * lenunit,
   ogc_vector *  ids,
   ogc_error *   err)
{
   ogc_ellipsoid * p = OGC_NULL;
   bool bad = false;

   /*---------------------------------------------------------
    * error checks
    */
   if ( name == OGC_NULL )
   {
      name = "";
   }
   else
   {
      int len = ogc_string::unescape_len(name);
      if ( len > OGC_NAME_MAX )
      {
         ogc_error::set(err, OGC_ERR_NAME_TOO_LONG, obj_kwd(), len);
         bad = true;
      }
   }

   if ( semi_major_axis <= 0.0 )
   {
      ogc_error::set(err, OGC_ERR_INVALID_SEMIMAJOR_AXIS, obj_kwd(),
         semi_major_axis);
      bad = true;
   }

   if ( flattening < 0.0 || flattening >= 1.0 )
   {
      ogc_error::set(err, OGC_ERR_INVALID_FLATTENING, obj_kwd(), flattening);
      bad = true;
   }

   /*---------------------------------------------------------
    * create the object
    */
   if ( !bad )
   {
      p = new (std::nothrow) ogc_ellipsoid();
      if ( p == OGC_NULL )
      {
         ogc_error::set(err, OGC_ERR_NO_MEMORY, obj_kwd());
         return p;
      }

      ogc_string::unescape_str(p->_name, name, OGC_NAME_MAX);
      p->_obj_type        = OGC_OBJ_TYPE_ELLIPSOID;
      p->_visible         = true;
      p->_semi_major_axis = semi_major_axis;
      p->_flattening      = flattening;
      p->_lenunit         = lenunit;
      p->_ids             = ids;
   }

   return p;
}

/*------------------------------------------------------------------------
 * destroy
 */
ogc_ellipsoid :: ~ogc_ellipsoid()
{
   _lenunit = ogc_lenunit :: destroy( _lenunit );
   _ids     = ogc_vector  :: destroy( _ids     );
}

ogc_ellipsoid * ogc_ellipsoid :: destroy(
   ogc_ellipsoid * obj)
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
ogc_ellipsoid * ogc_ellipsoid :: from_tokens(
   const ogc_token * t,
   int               start,
   int *             pend,
   ogc_error *       err)
{
   const ogc_token_entry * arr;
   const char * kwd;
   bool bad = false;
   int  level;
   int  end;
   int  same;
   int  num;

   ogc_ellipsoid * obj     = OGC_NULL;
   ogc_lenunit *   lenunit = OGC_NULL;
   ogc_id *        id      = OGC_NULL;
   ogc_vector *    ids     = OGC_NULL;
   const char * name;
   double       semi_major_axis;
   double       flattening;

   /*---------------------------------------------------------
    * sanity checks
    */
   if ( t == OGC_NULL )
   {
      ogc_error::set(err, OGC_ERR_WKT_EMPTY_STRING, obj_kwd());
      return OGC_NULL;
   }
   arr = t->_arr;

   if ( start < 0 || start >= t->_num )
   {
      ogc_error::set(err, OGC_ERR_WKT_INDEX_OUT_OF_RANGE, obj_kwd(), start);
      return OGC_NULL;
   }
   kwd = arr[start].str;

   if ( !is_kwd(kwd) )
   {
      ogc_error::set(err, OGC_ERR_WKT_INVALID_KEYWORD, obj_kwd(), kwd);
      return OGC_NULL;
   }

   /*---------------------------------------------------------
    * Get the level for this object,
    * the number of tokens at that level,
    * and the total number of tokens.
    */
   level = arr[start].lvl;
   for (end = start+1; end < t->_num; end++)
   {
      if ( arr[end].lvl <= level )
         break;
   }

   if ( pend != OGC_NULL )
      *pend = end;
   num = (end - start);

   for (same = 0; same < num; same++)
   {
      if ( arr[start+same+1].lvl != level+1 || arr[start+same+1].idx == 0 )
         break;
   }

   /*---------------------------------------------------------
    * There must be 3 tokens: ELLIPSOID[ "name", semi_major_axis, flattening ...
    */
   if ( same < 3 )
   {
      ogc_error::set(err, OGC_ERR_WKT_INSUFFICIENT_TOKENS, obj_kwd(), same);
      return OGC_NULL;
   }

   if ( same > 3 && get_strict_parsing() )
   {
      ogc_error::set(err, OGC_ERR_WKT_TOO_MANY_TOKENS,     obj_kwd(), same);
      return OGC_NULL;
   }

   start++;

   /*---------------------------------------------------------
    * Process all non-object tokens.
    * They come first and are syntcatically fixed.
    */
   name            = arr[start++].str;
   semi_major_axis = ogc_string::atod( arr[start++].str );
   flattening      = ogc_string::atod( arr[start++].str );

   if ( flattening != 0.0 )
   {
      flattening = (1.0 / flattening);
   }

   /*---------------------------------------------------------
    * Now process all sub-objects
    */
   int  next = 0;
   for (int i = start; i < end; i = next)
   {
      if ( ogc_lenunit::is_kwd(arr[i].str) )
      {
         if ( lenunit != OGC_NULL )
         {
            ogc_error::set(err, OGC_ERR_WKT_DUPLICATE_UNIT, obj_kwd());
            bad = true;
         }
         else
         {
            lenunit = ogc_lenunit::from_tokens(t, i, &next, err);
            if ( lenunit == OGC_NULL )
               bad = true;
         }
         continue;
      }

      if ( ogc_id::is_kwd(arr[i].str) )
      {
         id = ogc_id::from_tokens(t, i, &next, err);
         if ( id == OGC_NULL )
         {
            bad = true;
         }
         else
         {
            if ( ids == OGC_NULL )
            {
               ids = ogc_vector::create(1, 1);
               if ( ids == OGC_NULL )
               {
                  ogc_error::set(err, OGC_ERR_NO_MEMORY, obj_kwd());
                  delete id;
                  bad = true;
               }
            }

            if ( ids != OGC_NULL )
            {
               void * p = ids->find(
                             id,
                             false,
                             ogc_utils::compare_id);
               if ( p != OGC_NULL )
               {
                  ogc_error::set(err, OGC_ERR_WKT_DUPLICATE_ID,
                     obj_kwd(), id->name());
                  delete id;
                  bad = true;
               }
               else
               {
                  if ( ids->add( id ) < 0 )
                  {
                     ogc_error::set(err, OGC_ERR_NO_MEMORY, obj_kwd());
                     delete id;
                     bad = true;
                  }
               }
            }
         }
         continue;
      }

      /* unknown object, skip over it */
      for (next = i+1; next < end; next++)
      {
         if ( (arr[next].lvl <= arr[i].lvl) )
            break;
      }
   }

   /*---------------------------------------------------------
    * Create the object
    */
   if ( !bad )
   {
      obj = create(name, semi_major_axis, flattening, lenunit, ids, err);
   }

   if ( obj == OGC_NULL )
   {
      ogc_lenunit :: destroy( lenunit );
      ogc_vector  :: destroy( ids     );
   }

   return obj;
}

/*------------------------------------------------------------------------
 * object from WKT
 */
ogc_ellipsoid * ogc_ellipsoid :: from_wkt(
   const char * wkt,
   ogc_error *  err)
{
   ogc_ellipsoid * obj = OGC_NULL;
   ogc_token t;

   if ( t.tokenize(wkt, obj_kwd(), err) )
   {
      obj = from_tokens(&t, 0, OGC_NULL, err);
   }

   return obj;
}

/*------------------------------------------------------------------------
 * object to WKT
 */
bool ogc_ellipsoid :: to_wkt(
   const ogc_ellipsoid * obj,
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

bool ogc_ellipsoid :: to_wkt(
   char      buffer[],
   int       options,
   size_t    buflen) const
{
   OGC_UTF8_NAME buf_name;
   OGC_TBUF      buf_hdr;
   OGC_TBUF      buf_lenunit;
   OGC_TBUF      buf_id;
   OGC_NBUF      buf_sm_axis;
   OGC_NBUF      buf_flattening;
   int           opts  =  (options | OGC_WKT_OPT_INTERNAL);
   size_t        len   = 0;
   bool          rc    = true;
   const char *  opn   = "[";
   const char *  cls   = "]";
   const char *  kwd   = obj_kwd();

   if ( (options & OGC_WKT_OPT_PARENS) != 0 )
   {
      opn = "(";
      cls = ")";
   }

   if ( (opts & OGC_WKT_OPT_TOP_ID_ONLY) != 0 )
      opts |= OGC_WKT_OPT_NO_IDS;

   if ( buffer == OGC_NULL )
      return false;
   *buffer = 0;

   if ( !is_visible() )
      return true;

   if ( (options & OGC_WKT_OPT_OLD_SYNTAX) != 0 )
      kwd = alt_kwd();

   double flattening = _flattening;
   if ( flattening != 0.0 )
      flattening = (1.0 / flattening);

   ogc_string :: dtoa(_semi_major_axis, buf_sm_axis);
   ogc_string :: dtoa( flattening,      buf_flattening);

   if ( (options & OGC_WKT_OPT_OLD_SYNTAX) == 0 )
      rc &= ogc_lenunit :: to_wkt(_lenunit, buf_lenunit, opts, OGC_TBUF_MAX);
   else
      *buf_lenunit = 0;

   ogc_string::escape_str(buf_name, _name, OGC_UTF8_NAME_MAX);
   sprintf(buf_hdr, "%s%s\"%s\",%s,%s",
      kwd, opn, buf_name, buf_sm_axis, buf_flattening);

   OGC_CPY_TO_BUF( buf_hdr     );
   OGC_ADD_TO_BUF( buf_lenunit );

   if ( _ids != OGC_NULL && (options & OGC_WKT_OPT_NO_IDS) == 0 )
   {
      for (int i = 0; i < id_count(); i++)
      {
         rc &= ogc_id :: to_wkt(id(i), buf_id, opts, OGC_TBUF_MAX);
         OGC_ADD_TO_BUF( buf_id );
         if ( (options & OGC_WKT_OPT_OLD_SYNTAX) != 0 )
            break;
      }
   }

   OGC_CPY_TO_BUF( cls );

   if ( (options & OGC_WKT_OPT_INTERNAL) == 0 &&
        (options & OGC_WKT_OPT_EXPAND)   != 0 )
   {
      rc &= ogc_utils :: expand_wkt(buffer, buffer, "", options, buflen);
   }

   return rc;
}

/*------------------------------------------------------------------------
 * clone
 */
ogc_ellipsoid * ogc_ellipsoid :: clone(const ogc_ellipsoid * obj)
{
   if ( obj == OGC_NULL )
      return OGC_NULL;
   return obj->clone();
}

ogc_ellipsoid * ogc_ellipsoid :: clone() const
{
   ogc_lenunit * lenunit = ogc_lenunit :: clone( _lenunit );
   ogc_vector *  ids     = ogc_vector  :: clone( _ids     );

   ogc_ellipsoid * p = create(_name,
                              _semi_major_axis,
                              _flattening,
                              lenunit,
                              ids,
                              OGC_NULL);
   if ( p == OGC_NULL )
   {
      ogc_lenunit :: destroy( lenunit );
      ogc_vector  :: destroy( ids     );
   }

   return p;
}

/*------------------------------------------------------------------------
 * compare for computational equality
 */
bool ogc_ellipsoid :: is_equal(
   const ogc_ellipsoid * p1,
   const ogc_ellipsoid * p2)
{
   if ( p1 == OGC_NULL && p2 == OGC_NULL ) return true;
   if ( p1 == OGC_NULL || p2 == OGC_NULL ) return false;

   if ( !ogc_string  :: is_equal( p1->name(),            p2->name()            ) ||
        !ogc_macros  :: eq      ( p1->semi_major_axis(), p2->semi_major_axis() ) ||
        !ogc_macros  :: eq      ( p1->flattening(),      p2->flattening()      ) ||
        !ogc_lenunit :: is_equal( p1->lenunit(),         p2->lenunit()         ) )
   {
      return false;
   }

   return true;
}

bool ogc_ellipsoid :: is_equal(
   const ogc_ellipsoid * p) const
{
   return is_equal(this, p);
}

/*------------------------------------------------------------------------
 * compare
 */
bool ogc_ellipsoid :: is_identical(
   const ogc_ellipsoid * p1,
   const ogc_ellipsoid * p2)
{
   if ( p1 == OGC_NULL && p2 == OGC_NULL ) return true;
   if ( p1 == OGC_NULL || p2 == OGC_NULL ) return false;

   if ( !ogc_string  :: is_equal    ( p1->name(),            p2->name()            ) ||
        !ogc_macros  :: eq          ( p1->semi_major_axis(), p2->semi_major_axis() ) ||
        !ogc_macros  :: eq          ( p1->flattening(),      p2->flattening()      ) ||
        !ogc_lenunit :: is_identical( p1->lenunit(),         p2->lenunit()         ) ||
        !ogc_vector  :: is_identical( p1->ids(),             p2->ids()             ) )
   {
      return false;
   }

   return true;
}

bool ogc_ellipsoid :: is_identical(
   const ogc_ellipsoid * p) const
{
   return is_identical(this, p);
}

/*------------------------------------------------------------------------
 * get ID count
 */
int ogc_ellipsoid :: id_count() const
{
   return (_ids == OGC_NULL) ? 0 : _ids->length();
}

/*------------------------------------------------------------------------
 * get the nth ID
 */
ogc_id * ogc_ellipsoid :: id(int n) const
{
   return (_ids == OGC_NULL) ? OGC_NULL :
                               reinterpret_cast<ogc_id *>( _ids->get(n) );
}

/*------------------------------------------------------------------------
 * calculated values
 */
double ogc_ellipsoid :: semi_minor_axis() const
{
   return (_semi_major_axis * (1 - _flattening));
}

double ogc_ellipsoid :: e_squared() const
{
   return (_flattening * (2 - _flattening));
}

bool   ogc_ellipsoid :: is_sphere() const
{
   return ogc_macros::zero(_flattening);
}

} /* namespace OGC */

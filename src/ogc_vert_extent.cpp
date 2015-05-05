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
/* VERT EXTENT (Vertical extent) object                                      */
/* ------------------------------------------------------------------------- */

#include "ogc_common.h"

namespace OGC {

const char * ogc_vert_extent :: obj_kwd() { return OGC_OBJ_KWD_VERT_EXTENT; }

bool ogc_vert_extent :: is_kwd(const char * kwd)
{
   return ogc_string::is_equal(kwd, obj_kwd());
}

/*------------------------------------------------------------------------
 * create
 */
ogc_vert_extent * ogc_vert_extent :: create(
   double        min_ht,
   double        max_ht,
   ogc_lenunit * lenunit,
   ogc_error *   err)
{
   ogc_vert_extent * p = OGC_NULL;
   bool bad = false;

   /*---------------------------------------------------------
    * error checks
    */

   /*---------------------------------------------------------
    * create the object
    */
   if ( !bad )
   {
      p = new (std::nothrow) ogc_vert_extent();
      if ( p == OGC_NULL )
      {
         ogc_error::set(err, OGC_ERR_NO_MEMORY, obj_kwd());
         return p;
      }

      p->_obj_type = OGC_OBJ_TYPE_VERT_EXTENT;
      p->_visible  = true;
      p->_min_ht   = min_ht;
      p->_max_ht   = max_ht;
      p->_lenunit  = lenunit;
   }

   return p;
}

/*------------------------------------------------------------------------
 * destroy
 */
ogc_vert_extent :: ~ogc_vert_extent()
{
   ogc_lenunit :: destroy( _lenunit );
}

void ogc_vert_extent :: destroy(
   ogc_vert_extent * obj)
{
   if ( obj != OGC_NULL )
   {
      delete obj;
   }
}

/*------------------------------------------------------------------------
 * object from tokens
 */
ogc_vert_extent * ogc_vert_extent :: from_tokens(
   const ogc_token * t,
   int               start,
   int *             pend,
   ogc_error *       err)
{
   const ogc_token_entry * arr;
   const char * kwd;
   bool bad = false;
   int  level;
   int  same;
   int  end;
   int  num;

   ogc_vert_extent * obj     = OGC_NULL;
   ogc_lenunit *     lenunit = OGC_NULL;
   double min_ht;
   double max_ht;

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
    * There must be 2 tokens: VERTEXTENT[ min_ht, max_ht ...
    */
   if ( same < 2 )
   {
      ogc_error::set(err, OGC_ERR_WKT_INSUFFICIENT_TOKENS, obj_kwd(), same);
      return OGC_NULL;
   }

   if ( same > 2 && get_strict_parsing() )
   {
      ogc_error::set(err, OGC_ERR_WKT_TOO_MANY_TOKENS,     obj_kwd(), same);
      return OGC_NULL;
   }

   start++;

   /*---------------------------------------------------------
    * Process all non-object tokens.
    * They come first and are syntactically fixed.
    */
   min_ht = ogc_string::atod( arr[start++].str );
   max_ht = ogc_string::atod( arr[start++].str );

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
      obj = create(min_ht, max_ht, lenunit, err);
   }

   if ( obj == OGC_NULL )
   {
      ogc_lenunit :: destroy( lenunit );
   }

   return obj;
}

/*------------------------------------------------------------------------
 * object from WKT
 */
ogc_vert_extent * ogc_vert_extent :: from_wkt(
   const char * wkt,
   ogc_error *  err)
{
   ogc_vert_extent * obj = OGC_NULL;
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
bool ogc_vert_extent :: to_wkt(
   const ogc_vert_extent * obj,
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

bool ogc_vert_extent :: to_wkt(
   char      buffer[],
   int       options,
   size_t    buflen) const
{
   OGC_TBUF      buf_hdr;
   OGC_TBUF      buf_lenunit;
   OGC_NBUF      buf_min_ht;
   OGC_NBUF      buf_max_ht;
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

   if ( buffer == OGC_NULL )
      return false;
   *buffer = 0;

   if ( !is_visible() )
      return true;

   if ( (options & OGC_WKT_OPT_OLD_SYNTAX) == 0 )
      return true;

   ogc_string :: dtoa(_min_ht, buf_min_ht);
   ogc_string :: dtoa(_max_ht, buf_max_ht);

   rc &= ogc_lenunit :: to_wkt(_lenunit, buf_lenunit, opts, OGC_TBUF_MAX);

   sprintf(buf_hdr, "%s%s%s,%s",
      kwd, opn, buf_min_ht, buf_max_ht);

   OGC_CPY_TO_BUF( buf_hdr     );
   OGC_ADD_TO_BUF( buf_lenunit );
   OGC_CPY_TO_BUF( cls         );

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
ogc_vert_extent * ogc_vert_extent :: clone(const ogc_vert_extent * obj)
{
   if ( obj == OGC_NULL )
      return OGC_NULL;
   return obj->clone();
}

ogc_vert_extent * ogc_vert_extent :: clone() const
{
   ogc_lenunit * lenunit = ogc_lenunit :: clone( _lenunit );

   ogc_vert_extent * p = create(_min_ht,
                                _max_ht,
                                lenunit,
                                OGC_NULL);
   if ( p == OGC_NULL )
   {
      ogc_lenunit :: destroy( lenunit );
   }

   return p;
}

/*------------------------------------------------------------------------
 * compare for computational equality
 */
bool ogc_vert_extent :: is_equal(
   const ogc_vert_extent * p1,
   const ogc_vert_extent * p2)
{
   if ( p1 == OGC_NULL && p2 == OGC_NULL ) return true;
   if ( p1 == OGC_NULL || p2 == OGC_NULL ) return false;

   if ( !ogc_macros  :: eq      ( p1->min_ht(),  p2->min_ht()  ) ||
        !ogc_macros  :: eq      ( p1->max_ht(),  p2->max_ht()  ) ||
        !ogc_lenunit :: is_equal( p1->lenunit(), p2->lenunit() ) )
   {
      return false;
   }

   return true;
}

bool ogc_vert_extent :: is_equal(
   const ogc_vert_extent * p) const
{
   return is_equal(this, p);
}

/*------------------------------------------------------------------------
 * compare
 */
bool ogc_vert_extent :: is_identical(
   const ogc_vert_extent * p1,
   const ogc_vert_extent * p2)
{
   if ( p1 == OGC_NULL && p2 == OGC_NULL ) return true;
   if ( p1 == OGC_NULL || p2 == OGC_NULL ) return false;

   if ( !ogc_macros  :: eq          ( p1->min_ht(),  p2->min_ht()  ) ||
        !ogc_macros  :: eq          ( p1->max_ht(),  p2->max_ht()  ) ||
        !ogc_lenunit :: is_identical( p1->lenunit(), p2->lenunit() ) )
   {
      return false;
   }

   return true;
}

bool ogc_vert_extent :: is_identical(
   const ogc_vert_extent * p) const
{
   return is_identical(this, p);
}

} /* namespace OGC */
